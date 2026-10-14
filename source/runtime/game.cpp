#include "battle_menu_layout.hpp"
#include "camera_road_view.hpp"
#include "game.hpp"
#include "wall_geometry.hpp"
#include "field_camera.hpp"
#include "camera_road_assets.hpp"
#include "head_collision.hpp"
#include "floor_contact.hpp"
#include "field_ray.hpp"
#include "npc_interaction.hpp"
#include "npc_map_move.hpp"
#include "battle_unit_resources.hpp"
#include "battle_action_services.hpp"
#include "secondary_party_source.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
// Runtime composition and native room geometry cache. Native map/world values
// use .01 original engine-unit scale; source NPC/event/camera records use engine
// units. *100 and *.01 at service boundaries are deliberate conversions.
// Ownership/order/pack legends: docs/code/game-composition.md.
namespace ttyd {
namespace {
// Original callback parameters; loadout, mode and full event-state ownership
// still use explicitly identified harness inputs until their owners are bound.
int harnessDamage(DamageUnit& attacker,DamageUnit& defender,DamagePart& part,DamageWeapon weapon,
                  uint32_t flags,uint32_t actionResult,uint8_t jumpLevel,uint8_t hammerLevel,uint32_t& code) {
    DamageBattleState battle;battle.actionResult=actionResult;
    struct Context {DamageBattleState* battle;DamageWeapon* weapon;uint8_t jump,hammer;}
        context{&battle,&weapon,jumpLevel,hammerLevel};
    WeaponPowerServices power;power.user=&context;
    power.actionResult=[](void* user) {return static_cast<Context*>(user)->battle->actionResult;};
    power.pouchLevel=[](void* user,MarioPowerKind kind) {
        auto& c=*static_cast<Context*>(user);return kind==MarioPowerKind::Jump?c.jump:c.hammer;
    };
    power.bonus=[](void*,const DamageUnit& actor,MarioPowerKind kind) {
        return kind==MarioPowerKind::Jump?actor.jumpPowerBonus:actor.hammerPowerBonus;
    };
    power.powerWord=[](void* user,int32_t index) {
        assert(index>=0 && index<6);return static_cast<Context*>(user)->weapon->powerWords[size_t(index)];
    };
    weapon.callbackData=&power;
    const auto statuses=storedDamageStatusServices();
    const int32_t damage=calculateHpDamage(&attacker,defender,part,weapon,code,flags,battle,statuses);
    // HP and FP share the result-code location. This fixture has no FP callback;
    // original nonzero FP/resource ownership will be bound with the weapon catalog.
    calculateFpDamage(&attacker,defender,part,weapon,code,flags,battle);
    return int(damage);
}
// Room collision is process-global, not owned by each candidate Game. Room
// transactions swap geometry/index/return data; semantic actor state is Game-owned.
// UINT32_MAX room/mask sentinels force the first derived collision-mask update.
std::vector<RoadWarp> roads;
std::vector<uint32_t> roadAttributeNodes;
std::vector<uint32_t> disabledDoorNodes;
uint32_t doorCollisionRoom=UINT32_MAX,doorCollisionMask=UINT32_MAX;
bool fenceCollisionDisabled=false;
bool doorCollisionEnabled(uint32_t node) {
    if(fenceCollisionDisabled && std::binary_search(professorFenceCollisionNodes().begin(),professorFenceCollisionNodes().end(),node))return false;
    return !std::binary_search(disabledDoorNodes.begin(),disabledDoorNodes.end(),node);
}
void applyDoorCollision(uint32_t room,uint32_t mask) {
    if(room==doorCollisionRoom && mask==doorCollisionMask)return;
    doorCollisionRoom=room;doorCollisionMask=mask;
    disabledDoorNodes.clear();
    for(const auto& d:fieldDoors())if(d.room==room) {
        const auto& disabled=(mask&d.mask)?d.outsideNodes:d.interiorNodes;
        disabledDoorNodes.insert(disabledDoorNodes.end(),disabled.begin(),disabled.end());
    }
    std::sort(disabledDoorNodes.begin(),disabledDoorNodes.end());
}
// Road registration overlays original pass-through/inclusion mask0x00800000
// on matching source nodes without mutating the immutable imported attributes.
GroundContact runtimeContact(GroundContact contact) {
    if(std::find(roadAttributeNodes.begin(),roadAttributeNodes.end(),contact.sourceNode)!=roadAttributeNodes.end())
        contact.attributes|=0x00800000;
    return contact;
}
// Stable native room IDs:0 plaza,1 harbor,2 east,3 west,4 station,5 underground.
// Node852904 is harbor outgoing marker;1382128 plaza incoming,1215808 east
// incoming. These are converted original node identities, not world coordinates.
RoadCallback outCallback(const RoadWarp& road) {
    return road.source==1 && road.sourceNode==852904?RoadCallback::HarborOut:RoadCallback::None;
}
RoadCallback inCallback(const RoadWarp& road) {
    if(road.target==0 && road.arrivalNode==1382128)return RoadCallback::PartnerArrival;
    if(road.target==2 && road.arrivalNode==1215808)return RoadCallback::CoinArrival;
    if(road.target==3)return RoadCallback::DebugArrival;
    return RoadCallback::None;
}
std::vector<Rect> walls;
bool collisionReady=false;
std::vector<Vec2> wallLevels;
std::vector<GroundContact> wallContacts;
std::vector<WallTriangle> wallTriangles;
// Triangle positions are three XYZ triples (9 floats). Cull3 means two-sided.
// GroundContact has attributes/source-node words (8 bytes); ReturnPoint XYZ is12.
// Head pack prefix48 bytes =36 positions +8 metadata +4 original cull index.
struct GroundTriangle { float v[9];GroundContact contact;uint32_t cull=3; };
static_assert(sizeof(GroundContact)==8,"Ground metadata must contain two uint32 words");
static_assert(sizeof(ReturnPoint)==12,"Return position must contain three float32 values");
std::vector<GroundTriangle> ground;
struct HeadTriangle {float v[9];GroundContact contact;uint32_t cull;FieldRayBounds bounds;};
static_assert(offsetof(HeadTriangle,bounds)==48,"Head pack prefix size");
std::vector<HeadTriangle> head;
std::unordered_map<uint32_t,ReturnPoint> groundReturns;
using SpatialIndex=std::unordered_map<uint64_t,std::vector<uint32_t>>;
SpatialIndex wallIndex,groundIndex,headIndex;
// Legacy no-pack development bounds. Valid ground imports replace these
// with real room extents; GridSize2 is a native broad-phase cell width.
Rect roomBounds{-9,-4.5f,9,4};
constexpr float GridSize=2.f;
// Pack signed cell coordinates as their uint32 bit patterns in high/low32.
// floor (not truncation) maps negative coordinates into the correct grid cell.
uint64_t cellKey(int x,int z) {return (uint64_t(uint32_t(x))<<32)|uint32_t(z);}
const std::vector<uint32_t>& nearby(const SpatialIndex& index,Vec2 p) {
    static const std::vector<uint32_t> empty;
    auto cell=index.find(cellKey(int(std::floor(p.x/GridSize)),int(std::floor(p.z/GridSize))));
    return cell==index.end()?empty:cell->second;
}
// Native import guards: finite coordinates within +/-4096 world units and
// at most2,000,000 total cell references. These bound memory, not original rooms.
bool indexBounds(SpatialIndex& index,uint32_t n,float x0,float z0,float x1,float z1,size_t& references) {
    if(!std::isfinite(x0) || !std::isfinite(z0) || !std::isfinite(x1) || !std::isfinite(z1) ||
       std::max({std::fabs(x0),std::fabs(z0),std::fabs(x1),std::fabs(z1)})>4096)return false;
    int left=std::floor(x0/GridSize),right=std::floor(x1/GridSize);
    int back=std::floor(z0/GridSize),front=std::floor(z1/GridSize);
    size_t cells=size_t(right-left+1)*size_t(front-back+1);
    if(cells>2000000 || references+cells>2000000)return false;
    references+=cells;
    for(int x=left;x<=right;++x)for(int z=back;z<=front;++z)index[cellKey(x,z)].push_back(n);
    return true;
}


float length(Vec2 a, Vec2 b) { return std::hypot(a.x-b.x, a.z-b.z); }
// Native bottom-screen tab hitbox x[8,312), y[44,82), four76-pixel columns.
// -1 means no press-edge tab hit, not an invalid Menu value.
int menuButton(const Input& i) {
    if (i.touchY >= 44 && i.touchY < 82 && i.touchX >= 8 && i.touchX < 312)
        return (i.touchX-8)/76;
    return -1;
}
}
// Swap-based rollback owns the imported geometry and query indexes together. It also
// carries road attribute overlays; door masks are recalculated separately.
// A failed destination must not leave indexes referring to another room's data.
struct CollisionSnapshot {
    std::vector<Rect> savedWalls;
    std::vector<Vec2> savedLevels;
    std::vector<GroundContact> savedWallContacts;
    std::vector<WallTriangle> savedWallTriangles;
    std::vector<GroundTriangle> savedGround;
    std::vector<HeadTriangle> savedHead;
    std::vector<uint32_t> savedRoadAttributes;
    std::unordered_map<uint32_t,ReturnPoint> savedReturns;
    SpatialIndex savedWallIndex,savedGroundIndex,savedHeadIndex;
    Rect savedBounds{-9,-4.5f,9,4};
    bool ready=false;
    void swapActive() {
        savedWalls.swap(walls);savedLevels.swap(wallLevels);savedGround.swap(ground);
        savedWallContacts.swap(wallContacts);
        savedWallTriangles.swap(wallTriangles);
        savedHead.swap(head);savedHeadIndex.swap(headIndex);
        savedRoadAttributes.swap(roadAttributeNodes);
        savedReturns.swap(groundReturns);
        savedWallIndex.swap(wallIndex);savedGroundIndex.swap(groundIndex);
        std::swap(savedBounds,roomBounds);std::swap(ready,collisionReady);
    }
};
// Construction removes current collision into previous; load stages destination
// data in the global cache. Uncommitted destruction swaps the old cache back.
// Nested Game validation queries the staged cache, not its own collision copy.
CollisionTransition::CollisionTransition():previous(std::make_unique<CollisionSnapshot>()) {previous->swapActive();}
CollisionTransition::~CollisionTransition() {if(!committed)previous->swapActive();}
bool CollisionTransition::load(const char* prefix) {
    if(committed || !prefix)return false;
    clearCollision();staged=false;
    const std::string path(prefix);
    staged=loadCollision((path+".walls").c_str()) && loadGround((path+".ground").c_str()) && loadGroundReturns((path+".returns").c_str()) && loadHead((path+".head").c_str());
    return staged;
}
void CollisionTransition::commit() {if(staged)committed=true;}
// Save and room-pack ABI: keep these IDs stable when adding packaged areas.
// Unknown IDs return null so import/save/transition validation can reject them.
const char* roomAsset(uint32_t room) {
    static const char* names[]={"map","gor_00","gor_02","gor_03","gor_04","tik_01"};
    return room<6?names[room]:nullptr;
}
const char* roomName(uint32_t room) {
    static const char* names[]={"Rogueport plaza","Rogueport harbor","Rogueport east side","Rogueport west side","Rogueport station","Rogueport underground"};
    return room<6?names[room]:"Unknown area";
}
// Normal-Mario inclusion masks0x00080080 or0x00800000 override trigger mask4.
// Height spans are tested at both lower/upper body rays; no levels means legacy
// walls extend through all heights, not that they are absent.
bool wallAtLevel(size_t n,float elevation) {
    if(n<wallContacts.size()) {
        if(!doorCollisionEnabled(wallContacts[n].sourceNode))return false;
        const auto flags=wallContacts[n].attributes;
        if(!(flags&0x00080080) && !(flags&0x00800000) && (flags&4))return false;
    }
    if(n>=walls.size())return false;
    if(wallLevels.empty())return true;
    const auto height=wallLevels[n];
    return (height.x<=elevation+WallLowerRay && height.z>=elevation+WallLowerRay) ||
           (height.x<=elevation+WallUpperRay && height.z>=elevation+WallUpperRay);
}
Rect collisionBounds() {return roomBounds;}
// ROAD v1:12-byte magic/version/count header and48-byte RoadWarp records.
// count<=128 is a native pack bound. Directions0/2/4/6 are cardinal source codes.
// Require exactly one reverse marker pairing before replacing the road registry.
bool loadWarps(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    char magic[4];uint32_t version=0,count=0;
    bool ok=std::fread(magic,1,4,file)==4 && std::string(magic,4)=="ROAD" &&
        std::fread(&version,4,1,file)==1 && version==1 && std::fread(&count,4,1,file)==1 && count<=128;
    static_assert(sizeof(RoadWarp)==48,"Road pack record size");
    std::vector<RoadWarp> data(ok?count:0);
    if(ok)ok=std::fread(data.data(),sizeof(RoadWarp),count,file)==count && std::fgetc(file)==EOF;
    std::fclose(file);
    for(const auto& w:data) if(!roomAsset(w.source) || !roomAsset(w.target) ||
        !std::isfinite(w.x) || !std::isfinite(w.z) || !std::isfinite(w.spawnX) || !std::isfinite(w.spawnZ) || !std::isfinite(w.spawnY) ||
        !std::isfinite(w.centerY) || !w.sourceNode || !w.arrivalNode || w.direction>6 || w.direction%2 || w.arrivalDirection>6 || w.arrivalDirection%2)ok=false;
    for(const auto& w:data) {
        if(std::count_if(data.begin(),data.end(),[&](const auto& other){return other.source==w.source && other.sourceNode==w.sourceNode;})!=1 ||
           std::count_if(data.begin(),data.end(),[&](const auto& other){return other.source==w.target && other.sourceNode==w.arrivalNode && other.target==w.source && other.arrivalNode==w.sourceNode;})!=1)ok=false;
    }
    if(ok)roads.swap(data);
    return ok;
}
void Game::registerRoadCases(bool enabled) {
    roadCasesEnabled=enabled;roadAttributeNodes.clear();
    if(enabled)for(const auto& road:roads)if(road.source==progress.room)roadAttributeNodes.push_back(road.sourceNode);
}
bool Game::resolveRoadCallbacks(const RoadWarp& road) {
    roadCallback=outCallback(road);roadPolicy=roadCallbackPolicy(roadCallback,progress.originalStory);
    if(roadPolicy==RoadPolicy::Generic){roadCallback=inCallback(road);roadPolicy=roadCallbackPolicy(roadCallback,progress.originalStory);}
    return roadPolicy==RoadPolicy::Generic;
}
uint32_t Game::warpTarget() const {return warpRequest>=0 && size_t(warpRequest)<roads.size()?roads[warpRequest].target:UINT32_MAX;}
// Reciprocal marker direction positions arrival .3 native units (30 engine)
// before/after its center. Validate start floor, then restore staged Game.
// This is the scoped native arrival adapter, not the complete mapchange event.
bool Game::finishWarp() {
    if(warpRequest<0 || size_t(warpRequest)>=roads.size())return false;
    const auto& w=roads[warpRequest];float height=w.spawnY;
    if(!resolveRoadCallbacks(w))return false;
    const auto reverse=std::find_if(roads.begin(),roads.end(),[&](const auto& r){return r.source==w.target && r.sourceNode==w.arrivalNode && r.target==w.source;});
    if(reverse==roads.end())return false;
    float dx=0,dz=0;roadDirection(reverse->direction,dx,dz);
    const Vec2 start{reverse->x+dx*.3f,reverse->z+dz*.3f};
    Vec2 arrival{reverse->x-dx*.3f,reverse->z-dz*.3f};
    RoadMotion staged;
    if(!staged.begin({start.x,start.z},{arrival.x,arrival.z}))return false;
    if(!roadStartGround(start,reverse->centerY,height))return false;
    Progress next=progress;next.room=w.target;next.position=arrival;next.elevation=height;next.interiorMask=0;
    const uint32_t systemFlags=fieldSystemFlags;
    const RoadCallback entryCallback=inCallback(w);
    restore(next);groundHeight=height;cameraHeight=height;partner=arrival;partnerHeight=height;partnerContact=groundContact;
    fieldSystemFlags=entryCallback==RoadCallback::DebugArrival?systemFlags&~0x40u:systemFlags;
    rebaseFieldCamera();
    arrivalStart=start;arrivalEnd=arrival;
    registerRoadCases(false);partnerFollow={};saveRequested=true;return true;
}
void Game::beginRoadArrival() {
    beroExec->onOff(0,1); // Native arrival adapter: original info_run request8030ed94.
    roadMotion.begin({arrivalStart.x,arrivalStart.z},{arrivalEnd.x,arrivalEnd.z});
    if(std::fabs(roadMotion.step[0])>.001f)facing=roadMotion.step[0]>0?1:-1;
    turnScale=1;turnTick=0;
    progress.position=arrivalStart;partner=arrivalStart;arriving=true;saveRequested=false;
}
void Game::cancelRoad() {
    beroExec->unknown(); // No recovered cancellation clear; preserve missing provenance.
    if(departureOriginValid) {
        progress.position=departureOrigin;groundHeight=progress.elevation=departureHeight;
        cameraX=departureOrigin.x;cameraZ=departureOrigin.z;cameraHeight=departureHeight;rebaseFieldCamera();
    }
    warpRequest=-1;departureDone=false;departureParentStarted=false;departurePause=0;roadMotion={};arriving=false;saveRequested=false;
    fade.startIn();
}
// Legacy fallback rectangles are harness geometry only. Real walls use exact
// triangle slices at two body-ray heights or radius-inflated segment distance.
// 1e-8 ignores degenerate segment/triangle projections; do not divide by zero.
bool walkable(Vec2 p,float elevation) {
    constexpr float r = BodyRadius;
    if (!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(elevation) || std::fabs(p.x)>4096 || std::fabs(p.z)>4096) return false;
    if(collisionReady) {
        if(ground.empty() && (p.x < -6.7f+r || p.x > 6.7f-r || p.z < -4.5f+r || p.z > 5.8f-r))return false;
        for(uint32_t n:nearby(wallIndex,p)) {
            auto wall=walls[n];
            if(!wallAtLevel(n,elevation))continue;
            if(!wallTriangles.empty()) {
                if(wallSliceBlocks(wallTriangles[n],elevation+WallLowerRay,p.x,p.z,r) ||
                   wallSliceBlocks(wallTriangles[n],elevation+WallUpperRay,p.x,p.z,r))return false;
                continue;
            }
            if(p.x < std::min(wall.x0,wall.x1)-r || p.x > std::max(wall.x0,wall.x1)+r || p.z < std::min(wall.z0,wall.z1)-r || p.z > std::max(wall.z0,wall.z1)+r)continue;
            float dx=wall.x1-wall.x0,dz=wall.z1-wall.z0,den=dx*dx+dz*dz;
            if(den<1e-8f)continue;
            float t=std::clamp(((p.x-wall.x0)*dx+(p.z-wall.z0)*dz)/den,0.f,1.f);
            float x=p.x-wall.x0-t*dx,z=p.z-wall.z0-t*dz;
            if(x*x+z*z<r*r)return false;
        }
        return true;
    }
    if(p.x < -9+r || p.x > 9-r || p.z < -4.5f+r || p.z > 4-r)return false;
    for (auto b : Obstacles)
        if (p.x > b.x0-r && p.x < b.x1+r && p.z > b.z0-r && p.z < b.z1+r) return false;
    return true;
}
// Wall count high bits:0x80000000 height spans,0x40000000 attributes/nodes,
// 0x20000000 triangles; low29 bits count. Metadata requires heights; triangles
// require metadata. Native cap100000. Old records remain valid legacy inputs.
// Build validated geometry and indexes before publishing any replacement.
bool loadCollision(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    uint32_t count=0;bool ok=std::fread(&count,4,1,file)==1;
    bool levels=(count&0x80000000)!=0,tagged=(count&0x40000000)!=0,triangles=(count&0x20000000)!=0;count&=0x1fffffff;
    if((tagged && !levels) || (triangles && !tagged)){std::fclose(file);return false;}
    if(!ok || count>100000) {std::fclose(file);return false;}
    std::vector<Rect> data(count);std::vector<Vec2> heights(levels?count:0);
    std::vector<GroundContact> contacts(tagged?count:0);
    std::vector<WallTriangle> geometry(triangles?count:0);
    for(uint32_t n=0;n<count;++n) {
        ok=ok && std::fread(&data[n],sizeof(Rect),1,file)==1;
        if(levels)ok=ok && std::fread(&heights[n],sizeof(Vec2),1,file)==1;
        if(tagged)ok=ok && std::fread(&contacts[n],sizeof(GroundContact),1,file)==1;
        if(triangles)ok=ok && std::fread(geometry[n].data(),sizeof(WallTriangle),1,file)==1;
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);
    for(auto w:data) if(!std::isfinite(w.x0) || !std::isfinite(w.z0) || !std::isfinite(w.x1) || !std::isfinite(w.z1))ok=false;
    for(auto h:heights)if(!std::isfinite(h.x) || !std::isfinite(h.z) || h.x>h.z)ok=false;
    for(auto tri:geometry)for(float value:tri)if(!std::isfinite(value) || std::fabs(value)>4096)ok=false;
    for(uint32_t n=0;triangles && n<count;++n) {
        const auto& v=geometry[n];
        if(heights[n].x!=std::min({v[1],v[4],v[7]}) || heights[n].z!=std::max({v[1],v[4],v[7]}))ok=false;
    }
    SpatialIndex next;size_t references=0;
    for(uint32_t n=0;ok && n<count;++n) {
        auto w=data[n];
        if(triangles) {
            const auto& v=geometry[n];
            ok=indexBounds(next,n,std::min({v[0],v[3],v[6]})-BodyRadius,std::min({v[2],v[5],v[8]})-BodyRadius,
                std::max({v[0],v[3],v[6]})+BodyRadius,std::max({v[2],v[5],v[8]})+BodyRadius,references);
            continue;
        }
        ok=indexBounds(next,n,std::min(w.x0,w.x1)-BodyRadius,std::min(w.z0,w.z1)-BodyRadius,
                       std::max(w.x0,w.x1)+BodyRadius,std::max(w.z0,w.z1)+BodyRadius,references);
    }
    if(ok) {wallLevels.swap(heights);wallContacts.swap(contacts);wallTriangles.swap(geometry);walls.swap(data);wallIndex.swap(next);collisionReady=true;}
    return ok;
}
// HEAD:magic/count plus48-byte prefixes (positions,contact,cull). Bounds are
// computed locally after import, not read as an ABI-dependent C++ struct tail.
// Cull values0..3 retain original indices; cap100000 is native admission policy.
bool loadHead(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    char magic[4];uint32_t count=0;
    bool ok=std::fread(magic,4,1,file)==1 && std::string(magic,4)=="HEAD" && std::fread(&count,4,1,file)==1 && count<=100000;
    std::vector<HeadTriangle> data(ok?count:0);
    for(auto& tri:data) {
        ok=ok && std::fread(&tri,48,1,file)==1;
        for(float v:tri.v)if(!std::isfinite(v))ok=false;
        if(tri.cull>3)ok=false;
        tri.bounds=fieldRayBounds(tri.v);
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);
    SpatialIndex next;size_t references=0;
    for(uint32_t n=0;ok && n<count;++n) {
        const auto v=data[n].v;
        ok=indexBounds(next,n,std::min({v[0],v[3],v[6]}),std::min({v[2],v[5],v[8]}),
            std::max({v[0],v[3],v[6]}),std::max({v[2],v[5],v[8]}),references);
    }
    if(ok){head.swap(data);headIndex.swap(next);}return ok;
}
FieldShadowHit fieldShadowRay(FieldWallVector origin,FieldWallVector direction,float maximum) {
    // Bounded original downward query; geometry service normal is retained raw
    // from its ray output, without Mario's Q1000/support filters.
    if(direction.x!=0 || direction.y!=-1 || direction.z!=0 || maximum<0)return {};
    const auto nativeOrigin=FieldWallVector{origin.x*.01f,origin.y*.01f,origin.z*.01f};
    FieldShadowHit result;float nearest=maximum*.01f;bool selected=false;
    auto test=[&](const float* v,uint32_t cull,GroundContact contact) {
        if(!doorCollisionEnabled(contact.sourceNode))return;
        std::array<float,9> triangle;std::copy(v,v+9,triangle.begin());
        FieldWallHit hit;
        if(!intersectFieldRay(triangle,cull,nativeOrigin,direction,nearest,hit) || (selected && hit.distance>=nearest))return;
        contact=runtimeContact(contact);nearest=hit.distance;selected=true;
        // Named runtime owners/flags are not in static map packs. A closer
        // unknown tagged owner prevents a claim that a farther floor won.
        result.unresolvedObject=(contact.attributes&0x80000000u)!=0;
        result.found=!result.unresolvedObject;
        result.point={origin.x,origin.y-hit.distance*100.f,origin.z};result.normal=hit.normal;
    };
    if(!head.empty())for(auto n:nearby(headIndex,{nativeOrigin.x,nativeOrigin.z}))
        test(head[n].v,head[n].cull,head[n].contact);
    else for(auto n:nearby(groundIndex,{nativeOrigin.x,nativeOrigin.z}))
        test(ground[n].v,ground[n].cull,ground[n].contact);
    return result;
}
// token0=no hit, static token=n+1, dynamic token follows the static range.
// objectKey identifies a Game-owned object; it is not a source map node.
// Multi-cell rays sort/deduplicate candidates so equal-depth source order survives.
static FieldWallHit fieldMapRay(FieldWallVector origin,FieldWallVector direction,float maximum,const std::vector<FieldObjectTriangle>* objects,bool wallPolicy) {
    // Geometry cache is in native scale. Policy inputs/outputs are engine units.
    origin={origin.x*.01f,origin.y*.01f,origin.z*.01f};maximum*=.01f;
    FieldWallHit nearest;nearest.distance=maximum;
    auto test=[&](std::array<float,9> triangle,uint32_t cull,GroundContact contact,uintptr_t token,uint32_t owner=0) {
        if(!owner && !doorCollisionEnabled(contact.sourceNode))return;
        if(!owner)contact=runtimeContact(contact);
        if(wallPolicy && !ordinaryWallAttributes(contact.attributes))return;
        // Current static room meshes contain no named high-bit NPC owners.
        // Such an admitted record needs the original owner-name filter service.
        assert(!wallPolicy || owner || !(contact.attributes&0x80000000) || (contact.attributes&0x00080080));
        FieldWallHit hit;
        if(intersectFieldRay(triangle,cull,origin,direction,maximum,hit) &&
           (!nearest.token || hit.distance<nearest.distance)) {
            hit.token=token;hit.attributes=contact.attributes;hit.objectKey=owner;nearest=hit;
        }
    };
    if(!head.empty()) {
        const auto segment=fieldRaySegment(origin,direction,maximum);
        const float endX=origin.x+direction.x*maximum,endZ=origin.z+direction.z*maximum;
        const int left=std::floor(std::min(origin.x,endX)/GridSize),right=std::floor(std::max(origin.x,endX)/GridSize);
        const int back=std::floor(std::min(origin.z,endZ)/GridSize),front=std::floor(std::max(origin.z,endZ)/GridSize);
        auto testIndex=[&](uint32_t n) {
            if(!fieldRayMayReach(head[n].bounds,segment))return;
            std::array<float,9> triangle;std::copy(head[n].v,head[n].v+9,triangle.begin());
            test(triangle,head[n].cull,head[n].contact,n+1);
        };
        if(left==right && back==front) {
            for(uint32_t n:nearby(headIndex,{origin.x,origin.z}))testIndex(n);
        } else {
            std::vector<uint32_t> candidates;
            for(int x=left;x<=right;++x)for(int z=back;z<=front;++z) {
                const auto cell=headIndex.find(cellKey(x,z));
                if(cell!=headIndex.end())candidates.insert(candidates.end(),cell->second.begin(),cell->second.end());
            }
            std::sort(candidates.begin(),candidates.end());candidates.erase(std::unique(candidates.begin(),candidates.end()),candidates.end());
            for(uint32_t n:candidates)testIndex(n);
        }
    } else {
        // Older host fixture packs have segments/height intervals, without
        // source cull/winding. Extrusion preserves their explicit double sides.
        for(size_t n=0;n<walls.size();++n) {
            const auto contact=n<wallContacts.size()?wallContacts[n]:GroundContact{};
            if(n<wallTriangles.size()){test(wallTriangles[n],3,contact,n+1);continue;}
            const auto w=walls[n];const auto h=n<wallLevels.size()?wallLevels[n]:Vec2{-4096,4096};
            test({w.x0,h.x,w.z0,w.x1,h.x,w.z1,w.x0,h.z,w.z0},3,contact,n+1);
            test({w.x1,h.x,w.z1,w.x1,h.z,w.z1,w.x0,h.z,w.z0},3,contact,n+1);
        }
    }
    if(objects)for(size_t i=0;i<objects->size();++i) {
        const auto& tri=(*objects)[i];if(wallPolicy && !tri.ordinarySolid)continue;
        test(tri.vertices,tri.cull,{tri.attributes,0},std::max(head.size(),walls.size())+i+1,tri.owner);
    }
    nearest.distance*=100.f;
    return nearest;
}
FieldWallHit fieldWallRay(FieldWallVector origin,FieldWallVector direction,float maximum,const std::vector<FieldObjectTriangle>* objects) {
    return fieldMapRay(origin,direction,maximum,objects,true);
}
FieldPartyAppearanceServices Game::fieldPartyAppearanceServices() {
    FieldPartyAppearanceServices s;s.user=this;
    s.root=[](void* p){const auto& g=*static_cast<Game*>(p);return FieldWallVector{g.progress.position.x*100,(g.groundHeight+g.jumpHeight)*100,g.progress.position.z*100};};
    s.display=[](void* p){return static_cast<Game*>(p)->marioPresentation.display;};
    s.auxiliary=[](void* p){const auto raw=static_cast<Game*>(p)->fieldMarioBattle.partyIndices[1];return raw<128?int32_t(raw):int32_t(raw)-256;};
    s.revise=[](void*,float angle){return fieldPartyWrap(angle);};
    s.heading=[](void* p,float display,float offset,bool second){const auto& g=*static_cast<Game*>(p);return fieldPartyHeading(display,g.fieldMovement.headingOffsetDegrees,g.fieldMarioBattle.flags,g.marioPresentation.display,offset,second);};
    s.sincos=[](void*,float angle){return fieldPartySinCos(angle);};
    s.move=[](void*,FieldWallVector& root,float distance,float angle){fieldPartyMove(root,distance,angle);};
    s.ray=[](void* p,FieldWallVector root,FieldWallVector direction,float length){
        const auto hit=fieldMapRay(root,direction,length,&static_cast<Game*>(p)->fieldObjectTriangles,false);
        return FieldPartyAppearanceHit{hit.token!=0,root.y+direction.y*hit.distance};
    };
    return s;
}
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot) {
    return headCollision(p,direction,worldY,velocity,clampedRoot,velocity);
}
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot,float rayExtension) {
    const auto result=searchFieldHead(p,direction,worldY,velocity,rayExtension);
    if(result.contact)clampedRoot=result.correction;
    return result.contact;
}
// Five source probes: center, forward, left, right, back; normalized heading
// radius HeadProbeRadius. The first admitted ray wins, not the global minimum.
// Head cull1 excludes upward-facing triangles. Native 1e-6 rejects zero headings,
// 1e-8 degenerate projected triangles. Bounds are low-inclusive/high-exclusive.
FieldHeadQuery searchFieldHead(Vec2 p,Vec2 direction,float worldY,float velocity,float rayExtension,
                              FieldSlope* slopes,bool narrowCone,const std::vector<FieldObjectTriangle>* objects) {
    FieldHeadQuery result;
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || std::fabs(p.x)>4096 || std::fabs(p.z)>4096 ||
       !std::isfinite(worldY) || !std::isfinite(velocity) || !std::isfinite(rayExtension) ||
       !std::isfinite(length) || length<1e-6f)return result;
    const float x=direction.x*HeadProbeRadius/length,z=direction.z*HeadProbeRadius/length;
    const Vec2 offsets[]={{0,0},{x,z},{-z,x},{z,-x},{-x,-z}};
    const float low=worldY+HeadBodyHeight*.5f,high=low+HeadBodyHeight+rayExtension;
    for(auto offset:offsets) {
        const Vec2 ray{p.x+offset.x,p.z+offset.z};float nearest=high;const float* selected=nullptr;
        GroundContact selectedContact;uint32_t selectedOwner=0;
        auto test=[&](const float* v,GroundContact contact,uint32_t cull,uint32_t owner) {
            if(!headAttributesSolid(contact.attributes))return;
            // Source normal=(v2-v0) cross (v0-v1), equivalent to standard winding.
            const float normalY=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);
            if(cull==1 && normalY>=0)return;
            float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
            if(std::fabs(den)<1e-8f)return;
            float a=((v[5]-v[8])*(ray.x-v[6])+(v[6]-v[3])*(ray.z-v[8]))/den;
            float b=((v[8]-v[2])*(ray.x-v[6])+(v[0]-v[6])*(ray.z-v[8]))/den;
            if(a<0 || b<0 || a+b>1)return;
            float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
            if(y>=low && y<high && (!selected || y<nearest)){
                nearest=y;selected=v;selectedContact=contact;selectedOwner=owner;
            }
        };
        // Current packs represent one source map record. Its static slots
        // precede subsequently registered mode0 MOBJ slots; equal hits retain
        // the first slot. Multi-record hitMain chain reconstruction is pending.
        for(uint32_t n:nearby(headIndex,ray)) {
            const auto& tri=head[n];if(!doorCollisionEnabled(tri.contact.sourceNode))continue;
            test(tri.v,runtimeContact(tri.contact),tri.cull,0);
        }
        if(objects)for(const auto& tri:*objects)test(tri.vertices.data(),{tri.attributes,0},tri.cull,tri.owner);
        if(!selected)continue;
        const auto v=selected;
        const float ax=v[3]-v[0],ay=v[4]-v[1],az=v[5]-v[2];
        const float bx=v[6]-v[0],by=v[7]-v[1],bz=v[8]-v[2];
        float nx=ay*bz-az*by,ny=az*bx-ax*bz,nz=ax*by-ay*bx;
        const float norm=std::sqrt(nx*nx+ny*ny+nz*nz);
        if(norm==0)continue;
        const float orientation=ny>0?-norm:norm; // Upward ray back hit faces its origin.
        nx/=orientation;ny/=orientation;nz/=orientation;
        FieldSlope projected;projected.normal(nx,ny,nz); // Raw normal; no Q1000 rounding here.
        if(slopes){slopes->tiltX=projected.tiltX;slopes->tiltZ=projected.tiltZ;}
        if(!headInclinationAllowed(projected.inclination,narrowCone))continue;
        const float contact=headContactHeight(nearest);
        result.contact=headContactReached(worldY,velocity,contact);
        result.clearance=result.contact?0:contact-worldY-HeadBodyHeight;
        if(result.contact){result.correction=headClampedRoot(contact);result.node=selectedContact;result.objectKey=selectedOwner;}
        // First admitted ray returns even for a near miss. Later feet cannot
        // replace it with a closer contact; rejected inclinations can continue.
        return result;
    }
    return result;
}
// Ground count high bits:0x80000000 contact metadata,0x40000000 cull index;
// low30 bits count. Cull needs metadata. Each XYZ triple uses3 floats, with
//36-byte legacy geometry,44-byte tagged or48-byte faced records. Cap100000.
bool loadGround(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    uint32_t count=0;bool ok=std::fread(&count,4,1,file)==1;
    bool tagged=count&0x80000000,faced=count&0x40000000;
    count&=0x3fffffff;ok=ok && (!faced || tagged) && count>0 && count<=100000;
    std::vector<GroundTriangle> data(ok?count:0);
    for(auto& tri:data) {
        ok=ok && std::fread(tri.v,sizeof(tri.v),1,file)==1;
        if(tagged)ok=ok && std::fread(&tri.contact,sizeof(tri.contact),1,file)==1;
        if(faced)ok=ok && std::fread(&tri.cull,4,1,file)==1 && tri.cull<=3;
    }
    ok=ok && std::fgetc(file)==EOF;
    std::fclose(file);
    for(const auto& tri:data)for(float v:tri.v)if(!std::isfinite(v))ok=false;
    SpatialIndex next;size_t references=0;
    for(uint32_t n=0;ok && n<count;++n) {
        auto v=data[n].v;
        ok=indexBounds(next,n,std::min({v[0],v[3],v[6]}),std::min({v[2],v[5],v[8]}),
                       std::max({v[0],v[3],v[6]}),std::max({v[2],v[5],v[8]}),references);
    }
    if(ok) {
        const auto v=data.front().v;Rect bounds{v[0],v[2],v[0],v[2]};
        for(const auto& tri:data)for(int n=0;n<9;n+=3) {
            bounds.x0=std::min(bounds.x0,tri.v[n]);bounds.x1=std::max(bounds.x1,tri.v[n]);
            bounds.z0=std::min(bounds.z0,tri.v[n+2]);bounds.z1=std::max(bounds.z1,tri.v[n+2]);
        }
        ground.swap(data);groundIndex.swap(next);groundReturns.clear();roomBounds=bounds;
    }
    return ok;
}
// RPOS v1:12-byte header and16-byte node/XYZ rows. Nodes must be nonzero
// and unique; finite coordinates within +/-4096 are a native import bound.
// Return entries are room-local; replacing ground invalidates the old map.
bool loadGroundReturns(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    char magic[4];uint32_t version=0,count=0;
    bool ok=std::fread(magic,1,4,file)==4 && magic[0]=='R' && magic[1]=='P' && magic[2]=='O' && magic[3]=='S' &&
        std::fread(&version,4,1,file)==1 && version==1 && std::fread(&count,4,1,file)==1 && count<=100000;
    std::unordered_map<uint32_t,ReturnPoint> next;
    for(uint32_t n=0;ok && n<count;++n) {
        uint32_t node=0;ReturnPoint point;
        ok=std::fread(&node,4,1,file)==1 && std::fread(&point,sizeof(point),1,file)==1 && node!=0 &&
            std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z) &&
            std::max({std::fabs(point.x),std::fabs(point.y),std::fabs(point.z)})<=4096 && next.emplace(node,point).second;
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);
    if(ok)groundReturns.swap(next);
    return ok;
}
bool groundReturn(const GroundContact& contact,ReturnPoint& point) {
    auto found=groundReturns.find(contact.sourceNode);if(found==groundReturns.end())return false;
    point=found->second;return true;
}
// Highest triangle in (low,high] wins before slope rejection; a rejected
// steep closest floor does not reveal a lower floor. Native barycentric tolerance
// +/-1e-4 admits edge roundoff. Filter flags differ for Mario and ordinary partner.
// Legacy empty packs can supply Y0; only successful queries publish floor outputs
// (objectKey resets at entry, and slope tilt stores may precede rejection).
static bool groundWithin(Vec2 p,float low,float high,float& height,GroundContact* contact,float* slope=nullptr,
                         FieldSlope* record=nullptr,bool rejectSteep=true,bool actorFilter=true,bool partnerFilter=false,
                         const std::vector<FieldObjectTriangle>* objects=nullptr,uint32_t* objectKey=nullptr) {
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(low) || !std::isfinite(high) || low>high ||
       std::fabs(p.x)>4096 || std::fabs(p.z)>4096)return false;
    if(objectKey)*objectKey=0;
    bool found=ground.empty() && low<0 && high>=0;
    float best=found?0:-1e30f;GroundContact bestContact;const float* bestTriangle=nullptr;uint32_t bestOwner=0;
    auto test=[&](const float* v,uint32_t cull,GroundContact candidate,uint32_t owner) {
        // Original normal-Mario chkfilterVec rejects bit4 unless its special
        // inclusion bits apply. Dynamic name admission is resolved before this callback;
        // transformed filters remain separate owners.
        const uint32_t flags=candidate.attributes;
        if(actorFilter && !(flags&0x00080080) && !(flags&0x00800000) && (flags&4))return;
        // Ordinary partner chkfilter's early static-attribute branches.
        if(partnerFilter && !(flags&0x00080080) && (flags&0x00800004))return;
        const float normalY=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);
        if(cull==1 && normalY<=0)return; // Downward ray backface.
        if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) ||
           p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))return;
        float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
        if(std::fabs(den)<1e-8f)return;
        float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
        float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
        if(a<-.0001f || b<-.0001f || a+b>1.0001f)return;
        float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
        if(y<=high && y>low && y>best) {best=y;bestContact=candidate;bestTriangle=v;bestOwner=owner;found=true;}
    };
    for(uint32_t n:nearby(groundIndex,p)) {
        if(doorCollisionEnabled(ground[n].contact.sourceNode))test(ground[n].v,ground[n].cull,runtimeContact(ground[n].contact),0);
    }
    if(objects)for(const auto& tri:*objects) {
        if(!tri.ordinarySolid)continue;
        test(tri.vertices.data(),tri.cull,{tri.attributes,0},tri.owner);
    }
    if(found && !bestTriangle){height=0;if(contact)*contact={};if(slope)*slope=0;return true;}
    if(found) {
        // Original ray selects the nearest solid hit before rejecting its slope.
        const auto v=bestTriangle;
        const float ax=v[3]-v[0],ay=v[4]-v[1],az=v[5]-v[2];
        const float bx=v[6]-v[0],by=v[7]-v[1],bz=v[8]-v[2];
        float nx=ay*bz-az*by,ny=az*bx-ax*bz,nz=ax*by-ay*bx;
        // The original optimized downward ray orients double-sided back hits
        // toward the origin (80015d20–7c), before normal quantization/angles.
        if(ny<0){nx=-nx;ny=-ny;nz=-nz;}
        const float norm=std::sqrt(nx*nx+ny*ny+nz*nz);
        const float previousInclination=record?record->inclination:0;
        if(record && norm>0) {
            auto q=[](float value){return std::trunc(std::fma(value,1000.f,value<0?-.5f:.5f))/1000.f;};
            nx=q(nx/norm);ny=q(ny/norm);nz=q(nz/norm);
            record->normal(nx,ny,nz);
            // Q1000 acts in original units; geometry is stored at scale .01.
            best=q(best*100.f)*.01f;
        }
        const float angle=floorSlopeDegrees(nx,ny,nz);
        // Original ordinary support cone rejects slopes>=50 degrees. Restore
        // prior inclination on rejection, but retain earlier projected tilt stores.
        if(rejectSteep && angle>=50) {
            // Projected tilts were written before the cone gate; +218 wasn't.
            if(record)record->inclination=previousInclination;
            return false;
        }
        if(slope)*slope=angle;
        if(groundReturns.count(bestContact.sourceNode))bestContact.attributes|=0x40000000;
        height=best;if(contact)*contact=bestContact;if(objectKey)*objectKey=bestOwner;
    }
    return found;
}
bool groundAt(Vec2 p,float reference,float& height,GroundContact* contact,float* slope,
              const std::vector<FieldObjectTriangle>* objects,uint32_t* objectKey) {
    // Preserve development/no-pack flat-ground behavior for legacy callers.
    if(ground.empty() && std::isfinite(reference))return groundWithin(p,-.5f,StepHeight,height,contact,slope,nullptr,true,true,false,objects,objectKey);
    return groundWithin(p,reference-.5f,reference+StepHeight,height,contact,slope,nullptr,true,true,false,objects,objectKey);
}
// Original marker Y survives a missing floor. This broad native downward
// adapter uses10000 WORLD-unit reach; it is not the normal support probe.
bool roadStartGround(Vec2 p,float sourceY,float& height) {
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(sourceY))return false;
    height=sourceY; // Null ray retains the original marker Y.
    if(!ground.empty())groundWithin(p,sourceY-10000.f,sourceY,height,nullptr,nullptr,nullptr,false,false);
    return true;
}
bool landingGround(Vec2 p,float worldY,float velocity,float& height,GroundContact* contact) {
    if(!std::isfinite(velocity))return false;
    const float reach=std::fabs(velocity);
    // Center-only legacy query adapter. The native actor uses all five probes.
    FieldSlope slope;float floor=0;GroundContact selected;
    if(!groundWithin(p,worldY-reach-.001f,worldY+reach+.05f,floor,&selected,nullptr,&slope) ||
       !floorCandidateReachable(worldY,velocity,floor))return false;
    height=floor;if(contact)*contact=selected;return true;
}
static bool queryFrontStep(Vec2 p,Vec2 direction,float rootY,float& height,float& slope,FieldSlope* record,const std::vector<FieldObjectTriangle>* objects=nullptr) {
    if(record)*record={};
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(length) || length<1e-6f)return false;
    // Original front heading uses fixed +.5 whole-degree truncation/revise360.
    float heading=std::atan2(direction.x,-direction.z)*57.295779513f;
    if(heading<0)heading+=360;
    heading=std::trunc(heading+.5f)*3.14159265358979323846f/180.f;
    const Vec2 forward{p.x+std::sin(heading)*FrontStepDistance,p.z-std::cos(heading)*FrontStepDistance};
    // marioChkFrontStep: origin root+18.5, downward length18.5+11+.1 engine units.
    return groundWithin(forward,rootY-.111f,rootY+.185f,height,nullptr,&slope,record,false,true,false,objects);
}
bool frontStepGround(Vec2 p,Vec2 direction,float rootY,float& height,float& slope) {
    FieldSlope record;return queryFrontStep(p,direction,rootY,height,slope,&record);
}
bool landingFootprint(Vec2 p,Vec2 direction,float worldY,float velocity,float& height,GroundContact* contact) {
    FieldSlope slope;const auto result=searchFieldSupport(p,direction,worldY,velocity,slope,velocity);
    if(result.found){height=result.height;if(contact)*contact=result.contact;}return result.found;
}
// Five probes use FloorFootRadius. Native .001 lower margin and .05 upper
// margin represent .1/5 engine units; Q1000 output quantizes ENGINE units.
// *.00001 combines /1000 quantization with .01 engine-to-world conversion.
// A too-distant floor can set delta on a miss; callers use it to choose falling.
FieldSupport searchFieldSupport(Vec2 p,Vec2 direction,float rootY,float velocity,FieldSlope& slope,float depth,const std::vector<FieldObjectTriangle>* objects) {
    slope={};FieldSupport result;result.height=rootY;
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(length) || length<1e-6f || !std::isfinite(velocity) || !std::isfinite(depth))return result;
    const float x=direction.x*FloorFootRadius/length,z=direction.z*FloorFootRadius/length;
    const Vec2 offsets[]={{0,0},{x,z},{-z,x},{z,-x},{-x,-z}};
    float best=-100.f; // Original -10000-unit sentinel.
    for(auto offset:offsets) {
        float floor=0;GroundContact candidate;uint32_t owner=0;
        if(!groundWithin({p.x+offset.x,p.z+offset.z},rootY-std::fabs(depth)-.001f,rootY+.05f+std::fabs(velocity),
                         floor,&candidate,nullptr,&slope,true,true,false,objects,&owner))continue;
        const float below=rootY-floor;
        if(below>=GroundedNearDrop && below>std::fabs(velocity)) {result.delta=below;continue;}
        if(!floorCandidateReachable(rootY,velocity,floor) || floor<=best)continue;
        best=floor;result.found=true;result.height=floor;result.delta=0;result.contact=candidate;result.objectKey=owner;
        const auto q=[](float native){const float engine=native*100.f;return std::trunc(std::fma(engine,1000.f,engine<0?-.5f:.5f))*.00001f;};
        result.point={q(p.x+offset.x),floor,q(p.z+offset.z)};
    }
    return result;
}
bool groundedFootprint(Vec2 p,Vec2 direction,float rootY,float& height,GroundContact* contact) {
    FieldSlope slope;const auto result=searchFieldSupport(p,direction,rootY,0,slope);
    if(result.found){height=result.height;if(contact)*contact=result.contact;}return result.found;
}
void Game::dispatchFieldAir() {
    if(fieldJump.airborne && fieldJump.pendingEntry && fieldJump.motion<3) {
        const float root=jumpBase+fieldJump.height;
        fieldVerticalVelocity=fieldJump.velocity;
        const unsigned posted=fieldJump.motion;
        fieldJump.enterGround(posted,root);
        if(posted==0)fieldVerticalVelocity=0;
        fieldJump.height=0;jumpBase=groundHeight=progress.elevation=root;jumpHeight=0;
    } else fieldJump.dispatch(jumpBase+fieldJump.height);
}
// Dynamic triangles cache by catalog generation, room, known story, interior
// mask and system flags. Object keys use a separate namespace from static nodes.
void Game::refreshFieldObjects() {
    const uint64_t generation=fieldActorCatalogGeneration();
    if(fieldObjectGeneration==generation && fieldObjectRoom==progress.room && fieldObjectStory==progress.originalStory &&
       fieldObjectInterior==fieldDoor.interiorMask && fieldObjectSystem==fieldSystemFlags)return;
    fieldObjectGeneration=generation;fieldObjectRoom=progress.room;fieldObjectStory=progress.originalStory;
    fieldObjectInterior=fieldDoor.interiorMask;fieldObjectSystem=fieldSystemFlags;
    buildFieldObjectCollision(fieldActorCatalog(),progress.room,progress.originalStory,fieldDoor.interiorMask,
                              fieldSystemFlags,fieldObjectTriangles);
}
// Clearance<=.05 native units (5 engine) counts as nearby obstruction even
// without contact. Query publishes metadata separately from a ceiling response.
FieldHeadQuery Game::queryOverhead(float rootY,float velocity,float extension) {
    refreshFieldObjects();
    const auto result=searchFieldHead(progress.position,moveDirection,rootY,velocity,extension,
                                      &fieldMovement.slope,fieldHeadNarrowCone,&fieldObjectTriangles);
    headContact=result.node;headObjectKey=result.objectKey;headClearance=result.clearance;
    overheadContact=result.contact || (result.clearance>=0 && result.clearance<=.05f);
    return result;
}
void Game::advanceVertical(bool held,bool dispatch) {
    if(dispatch)dispatchFieldAir();
    float previousY=0;if(integrateVertical(held,previousY))finishVertical(previousY);
}
bool Game::integrateVertical(bool held,float& previousY,bool update) {
    if(!fieldJump.airborne){jumpHeight=0;if(groundHeight<=VoidThreshold)beginVoid();return false;}
    previousY=jumpBase+fieldJump.height;
    if(fieldJump.falling) {
        // Falling marioMain checks overhead before marioFall updates derivatives.
        // Without the ascent flag, marioChkOverhead extends its ray by10 engine units.
        // Source motion10 records the query but bypasses jump-only response.
        queryOverhead(previousY,fieldJump.velocity,.1f);
        // This caller has already moved. Base mutation affects the next move,
        // and the updated base selects this call's landing probe orientation.
        fieldMovement.dampFallBase(fieldScreen.inScreen(),!marioFieldControl && (fieldKeysOff || saveLocked()),fieldAirDampingBypass);
        if(!fieldScreen.inScreen() && !fieldAirDampingBypass)fieldJump.suppressDrift=true;
        fieldJump.integrate(held);jumpHeight=jumpBase+fieldJump.height-groundHeight;
        return true;
    }
    if(update)fieldJump.integrate(held);
    const auto head=queryOverhead(jumpBase+fieldJump.height,fieldJump.velocity,
                                 fieldJump.jumping?fieldJump.velocity:.1f);
    if(head.contact) {
        float proposed=head.correction;
        if(fieldJump.prepareCeiling(proposed,fieldMovement.speed,WalkSpeed)) {
            jumpBase=proposed;fieldJump.ceilingFall();
            jumpHeight=jumpBase-groundHeight;return false;
        }
    }
    jumpHeight=jumpBase+fieldJump.height-groundHeight;
    return true;
}
void Game::finishVertical(float worldY) {
    if(fieldJump.falling) {
        const float facingAngle=marioPresentation.moveHeading()*3.14159265358979323846f/180.f;
        const Vec2 direction=fieldMovement.speed!=0?Vec2{fieldMovement.headingX,fieldMovement.headingZ}:Vec2{std::sin(facingAngle),-std::cos(facingAngle)};
        const float offset=fieldMovement.speed!=0?fieldMovement.headingOffsetDegrees*3.14159265358979323846f/180.f:0;
        const Vec2 foot{direction.x*std::cos(offset)+direction.z*std::sin(offset),
                       direction.z*std::cos(offset)-direction.x*std::sin(offset)};
        const auto support=searchFieldSupport(progress.position,foot,worldY,fieldJump.velocity,fieldMovement.slope,fieldJump.velocity,&fieldObjectTriangles);
        publishFieldCameraCollision(support);
        groundContact=support.found?support.contact:GroundContact{};groundObjectKey=support.objectKey;
        if(support.found) {
            previousGroundContact={};
            const bool retain=(support.contact.attributes&0x200)!=0;
            const float floor=retain?worldY+fieldJump.velocity:support.height;
            fieldVerticalVelocity=retain?fieldJump.velocity:0;
            fieldJump.land(fieldInputMagnitude>0,retain);fieldStep={};
            groundHeight=progress.elevation=jumpBase=floor;jumpHeight=0;
        }
    }
    if(fieldJump.airborne)jumpHeight=jumpBase+fieldJump.height-groundHeight;
    if(groundHeight+jumpHeight<=VoidThreshold)beginVoid();
}
// Validate an authored remembered return or stable room respawn before starting
// recovery. Bottomless motion33 and water motion31 are original motion IDs.
// Clear pending warp/save requests; transient recovery roots cannot checkpoint.
bool Game::beginVoid() {
    ReturnPoint destination=roomRespawn,candidate;
    float floor=destination.y;
    if(recoveryReturn(candidate) && groundAt({candidate.x,candidate.z},candidate.y,floor) && walkable({candidate.x,candidate.z},floor))destination={candidate.x,floor,candidate.z};
    floor=destination.y;
    if(!groundAt({destination.x,destination.z},floor,floor) || !walkable({destination.x,destination.z},floor))return false;
    destination.y=floor;
    const auto height=bottomlessProbeHeight({destination.x,floor,destination.z},fieldCameraFrame);
    if(!height)return false;
    water.beginVoid({progress.position.x,*height,progress.position.z},{destination.x,floor,destination.z});
    // Entry continues into phase0 in this same Mario callback.
    water.tick(progress.hp);
    progress.position={water.position[0],water.position[2]};groundHeight=progress.elevation=floor;
    marioCamera.retained.x=water.position[0]*100;marioCamera.retained.z=water.position[2]*100;
    partner={roomRespawn.x,roomRespawn.z};partnerHeight=roomRespawn.y;partnerContact={};
    cameraX=destination.x;cameraZ=destination.z;cameraHeight=floor;
    fieldMovement.speed=0;
    fieldJump={};fieldJump.motion=33;fieldStep={};fieldVerticalVelocity=0;jumpHeight=0;warpRequest=-1;saveRequested=false;
    return true;
}
void clearCollision() {doorCollisionRoom=UINT32_MAX;doorCollisionMask=UINT32_MAX;disabledDoorNodes.clear();roadAttributeNodes.clear();walls.clear();wallLevels.clear();wallContacts.clear();wallTriangles.clear();ground.clear();head.clear();headIndex.clear();groundReturns.clear();wallIndex.clear();groundIndex.clear();collisionReady=false;roomBounds={-9,-4.5f,9,4};}
const std::vector<Rect>& collisionWalls() {return walls;}
bool Game::near(Vec2 p, float d) const { return length(progress.position,p) < d; }
NpcTalkPlayer Game::fieldNpcTalkPlayer() const {
    NpcTalkPlayer mario;mario.root={progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100};
    mario.motion=fieldJump.motion;mario.displayTurn=marioPresentation.display;
    mario.keyDepth=int8_t(goombaHandIn.keyDepth()+((goombaAcquisition.active() || goombaReward.active())?1:0));
    mario.cameraSnapshot=marioPresentation.offset;
    mario.viewFlags=marioPresentation.displayFlags;mario.flags=uint32_t(fieldIdle.mainFlags);
    mario.viewYaw=fieldNpcAngles().angle(fieldCameraFrame.eye.x,fieldCameraFrame.eye.z,fieldCameraFrame.at.x,fieldCameraFrame.at.z);
    return mario;
}
// gor_02 Goomba4 key0x2b0a9f83: quest2/item95 and quest13/item107 require
// explicitly known quest/guard state. Legacy unknown switches never imply items.
FieldActorTalkInventory Game::fieldNpcTalkInventory(const FieldActorRecord& actor) const {
    FieldActorTalkInventory items;const auto& state=progress.originalNpcTalkState;
    if(actor.room!=2 || actor.key!=0x2b0a9f83)return items;
    if(!progress.originalPouch || !state.quest1420)return items;
    if(*state.quest1420==2 && state.flag5405 && *state.flag5405)items.item95=progress.originalPouch->keyItemCount(95);
    if(*state.quest1420==13 && state.flag5406 && *state.flag5406)items.item107=progress.originalPouch->keyItemCount(107);
    return items;
}
bool Game::fieldNpcTalkCandidateAlive() const {
    return npcTalkGeneration==fieldActorCatalogGeneration() && npcTalkCandidate &&
        fieldNpcTalkPlayer().talkable() && std::any_of(fieldActorCatalog().actors().begin(),fieldActorCatalog().actors().end(),
            [&](const auto& actor){return actor.key==npcTalkCandidate &&
                actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags);});
}
GoombaRewardServices Game::fieldRewardServices() {
    return {this,[](void* p){auto& g=*static_cast<Game*>(p);return g.progress.originalPouch && g.fieldCoinGetReady() &&
        fieldItemMarioGetAvailable(g.fieldJump,uint32_t(g.fieldIdle.mainFlags)|g.fieldMarioBattle.flags,g.fieldItemMarioPost);},
        [](void* p,int32_t amount,bool first){auto& g=*static_cast<Game*>(p);return g.invokeFieldCoinGet(amount,first,g.fieldRewardTriggers);},
        [](void* p,int32_t flag){auto& g=*static_cast<Game*>(p);if(flag==5376)g.progress.originalNpcTalkState.flag5376=true;}};
}
bool Game::fieldNpcTalkBound(const FieldActorRecord& actor) const {
    if(!progress.originalStory)return false;
    const auto items=fieldNpcTalkInventory(actor);
    return (progress.originalPouch && goombaQuest2RewardAdmitted(actor,progress.originalNpcTalkState) && fieldCoinGetReady() &&
            fieldItemMarioGetAvailable(fieldJump,uint32_t(fieldIdle.mainFlags)|fieldMarioBattle.flags,fieldItemMarioPost)) ||
        (progress.originalPouch && goombaQuest2AcquisitionAdmitted(actor,progress.originalNpcTalkState) &&
            goombaAcquisitionPresentationReady(fieldItemGetPresentation) &&
            fieldItemMarioGetAvailable(fieldJump,uint32_t(fieldIdle.mainFlags)|fieldMarioBattle.flags,fieldItemMarioPost)) ||
        fieldActorTalkAvailable(actor,*progress.originalStory,progress.originalNpcTalkState,items) ||
        (progress.originalPouch && pouchRevisionAvailable(*progress.originalPouch) &&
         (goombaQuest2Admitted(actor,progress.originalNpcTalkState,items) || goombaQuest13Admitted(actor,progress.originalNpcTalkState,items)));
}
const FieldActorRecord* Game::nearbyFieldActor() const {
    if(scene!=Scene::Explore || !fieldControlEnabled() || fieldKeysOff || saveLocked() || water.active() ||
       fieldStep.active() || fieldDoor.active() || arriving || fade.active() || warpRequest>=0 || pipeRequest>=0)return nullptr;
    const auto mario=fieldNpcTalkPlayer();
    FieldWallServices rays;rays.user=const_cast<Game*>(this);
    rays.ray=[](void* p,FieldWallVector from,FieldWallVector direction,float distance) {
        return fieldMapRay(from,direction,distance,&static_cast<Game*>(p)->fieldObjectTriangles,false);
    };
    NpcTalkScan scan;
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(actor.kind || !actor.npcFlags ||
           !actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        // Share current roots and source flags with the side-response owner.
        // Additional registry/event producers remain outside this slice.
        const auto live=fieldNpcs.find(actor.key);
        const auto root=live!=fieldNpcs.end()?live->second.body.root:FieldWallVector{actor.position[0],actor.position[1],actor.position[2]};
        const auto flags=live!=fieldNpcs.end()?live->second.body.flags:actor.npcFlags;
        const bool talkBound=fieldNpcTalkBound(actor);
        scan.consider({actor.key,root,actor.npcWidth,actor.npcHeight,flags,talkBound},mario,fieldNpcAngles(),rays);
    }
    if(scan.candidate)for(const auto& actor:fieldActorCatalog().actors())
        if(actor.key==scan.candidate && actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))return &actor;
    return nullptr;
}
// Candidate/pending/waiting are three distinct invocation phases. Record a
// generation with the candidate; catalog reload must not reuse a stale actor key.
// A confirmed candidate starts next invocation, and returning from a finished
// talk never immediately rescans/restarts it in that same invocation.
void Game::updateFieldNpcTalk(const Input& input) {
    if(troubleBoardLocked())return;
    if(npcTalkWaiting) {
        if(!fieldActorTalk.message.active() && !goombaHandIn.active() && !goombaAcquisition.active() && !goombaReward.active()){npcTalkWaiting=false;npcTalkCandidate=0;}
        return; // Source wait/return owner does not scan again in this invocation.
    }
    if(npcTalkPending) {
        const uint32_t selected=npcTalkPending;npcTalkPending=0;
        if(npcTalkGeneration==fieldActorCatalogGeneration() && scene==Scene::Explore && progress.originalStory)
            for(const auto& actor:fieldActorCatalog().actors())if(actor.key==selected &&
                actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags)) {
                auto current=actor;const auto live=fieldNpcs.find(actor.key);
                if(live!=fieldNpcs.end())current.position={live->second.body.root.x,live->second.body.root.y,live->second.body.root.z};
                const auto items=fieldNpcTalkInventory(current);
                bool started=false;
                if(progress.originalPouch && goombaQuest2RewardAdmitted(current,progress.originalNpcTalkState)) {
                    started=goombaReward.begin(current,progress.originalNpcTalkState,fieldActorTalk,
                        harborMessageFont(),fieldRewardServices(),ticks*1000/60);
                    if(started)applyFieldItemStateRequest(ItemGetRequest::KeyOff);
                } else if(progress.originalPouch && goombaQuest2AcquisitionAdmitted(current,progress.originalNpcTalkState)) {
                    if(fieldItemMarioGetAvailable(fieldJump,uint32_t(fieldIdle.mainFlags)|fieldMarioBattle.flags,fieldItemMarioPost))
                        started=goombaAcquisition.begin(current,*progress.originalPouch,progress.originalNpcTalkState,
                            fieldActorTalk,harborMessageFont(),fieldItemRegistry,fieldItemGetPresentation,ticks*1000/60);
                    if(started)applyFieldItemStateRequest(ItemGetRequest::KeyOff); // NPC talk owns an outer hold; itemMain adds its own.
                } else if((goombaQuest2Admitted(current,progress.originalNpcTalkState,items) ||
                    goombaQuest13Admitted(current,progress.originalNpcTalkState,items)) && progress.originalPouch) {
                    if(!itemSelection)itemSelection=std::make_shared<ItemSelectionPresentation>();
                    started=goombaHandIn.begin(current,*progress.originalPouch,progress.originalNpcTalkState,
                                              fieldActorTalk,*itemSelection,harborMessageFont(),ticks*1000/60,&marioPresentation);
                    if(started)fieldKeysOff=true;
                } else started=fieldActorTalk.begin(current,*progress.originalStory,harborMessageFont(),ticks*1000/60,progress.originalNpcTalkState,items);
                if(started) {
                    if(!(current.npcFlags&0x01000000))fieldActorTalk.actor.yaw=npcReviseAngle(fieldNpcAngles().angle(current.position[0],current.position[2],
                        progress.position.x*100,progress.position.z*100));
                    if(live!=fieldNpcs.end())live->second.movement.yaw=fieldActorTalk.actor.yaw;
                    scene=Scene::Dialogue;saveRequested=false;npcTalkWaiting=true;return;
                }
                break;
            }
        npcTalkCandidate=0;return;
    }
    const FieldActorRecord* actor=nullptr;
    if(fieldNpcEventCandidate) {
        for(const auto& row:fieldActorCatalog().actors())if(row.key==fieldNpcEventCandidate && progress.originalStory &&
            fieldNpcTalkBound(row)){actor=&row;break;}
    } else actor=nearbyFieldActor();
    npcTalkCandidate=fieldNpcEventCandidate?fieldNpcEventCandidate:actor?actor->key:0;
    npcTalkGeneration=fieldActorCatalogGeneration();
    if(!actor || !input.confirm || !progress.originalStory)return;
    npcTalkPending=actor->key;saveRequested=false; // State1→2; next NPC invocation starts the event.
}
const FieldNpcInstance* Game::fieldNpcState(uint32_t key) const {
    if(fieldNpcRoom!=progress.room || fieldNpcGeneration!=fieldActorCatalogGeneration())return nullptr;
    const auto found=fieldNpcs.find(key);return found==fieldNpcs.end()?nullptr:&found->second;
}
bool Game::setFieldNpcPosition(const std::string& name,FieldWallVector position) {
    if(name.empty() || !std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))return false;
    synchronizeFieldNpcs();
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(actor.kind || actor.eventName!=name || !actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        const auto found=fieldNpcs.find(actor.key);
        if(found==fieldNpcs.end() || !found->second.initialized || found->second.scripted)return false;
        auto& state=found->second;
        // Original evt_npc_set_position writes previous first, then current.
        // Preserve flags, direction, pose, timers and regular-event anchors.
        state.body.previous=position;state.body.root=position;state.movement.root=position;
        state.positionOverride=true;++state.placement;return true;
    }
    return false;
}
void Game::synchronizeFieldNpcs() {
    const auto generation=fieldActorCatalogGeneration();
    if(fieldNpcGeneration!=generation || fieldNpcRoom!=progress.room) {
        fieldNpcs.clear();fieldNpcEventCandidate=0;fieldNpcGeneration=generation;fieldNpcRoom=progress.room;
    }
}
// Source wander providers consume the prior pose/contact publications. Native
// sound-name tokens1/2 select left/right keys; they are not original addresses.
// Frame conversion is60Hz; pose epochs are integer milliseconds.
void Game::updateFieldNpcEvents() {
    synchronizeFieldNpcs();npcSoundRequests.clear();
    if((scene!=Scene::Explore && scene!=Scene::Dialogue) || trioBattleActive || water.active() ||
       arriving || fade.active() || warpRequest>=0 || pipeRequest>=0)return;
    struct Context {Game* game;const FieldActorRecord* actor;};
    NpcWanderServices services;
    services.frames=[](void*,int32_t ms){return npcWanderFrames(ms,TickRate);};
    services.random=[](void* p,int32_t bound){return npcWanderRandom(static_cast<Context*>(p)->game->npcRandomSeed,bound);};
    services.turnReady=[](void* p,const NpcWanderActor& a) {
        const auto& pose=static_cast<Context*>(p)->game->fieldNpcs.at(uint32_t(a.token)).presentation;
        return npcWanderTurnReady(pose.flags,pose.retained,pose.target);
    };
    services.angle=[](void*,FieldWallVector a,FieldWallVector b){return fieldNpcAngles().angle(a.x,a.z,b.x,b.z);};
    services.distance=[](void*,FieldWallVector a,FieldWallVector b){const float x=a.x-b.x,z=a.z-b.z;return std::sqrt(std::fma(x,x,z*z));};
    services.sine=[](void*,float r){return float(std::sin(double(r)));};
    services.cosine=[](void*,float r){return float(std::cos(double(r)));};
    services.sound=[](void* p,uintptr_t name,uint32_t mask,FieldWallVector root) {
        const auto& c=*static_cast<Context*>(p);c.game->npcSoundRequests.push_back({name==1?c.actor->wander.soundLeft:c.actor->wander.soundRight,mask,root});
    };
    NpcWanderMario mario;mario.root={progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100};
    mario.travelHeading=fieldNpcAngles().angle(0,0,fieldMovement.headingX,fieldMovement.headingZ);mario.contact=fieldNpcHit;
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(!actor.wander.enabled || !actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        auto& state=fieldNpcs[actor.key];Context context{this,&actor};services.user=&context;
        if(!state.initialized) {
            state.body.root=state.body.previous={actor.position[0],actor.position[1],actor.position[2]};
            state.movement.token=actor.key;state.movement.root=state.body.root;state.movement.yaw=actor.yaw;
            state.movement.flags=actor.npcFlags;state.movement.stepPeriod=uint16_t(actor.wander.stepPeriod);
            state.movement.sounds[0]=actor.wander.soundLeft.empty()?0:1;state.movement.sounds[1]=actor.wander.soundRight.empty()?0:2;
            initializeNpcWander(state.wander,state.movement,state.body.root.x,state.body.root.z,actor.wander.radius,actor.wander.speed,actor.wander.flags,services);
            state.clock=state.poseAt=ticks*1000/TickRate;state.pose=actor.wander.idle;state.initialized=true;
        }
        state.movement.root=state.body.root;
        if(!state.paused)tickNpcWander(state.wander,state.movement,mario,services);
    }
}
void Game::updateFieldNpcCandidates() {
    fieldNpcEventCandidate=0;
    if(scene!=Scene::Explore || !fieldControlEnabled() || fieldKeysOff || saveLocked() || water.active() ||
       fieldStep.active() || fieldDoor.active() || arriving || fade.active() || warpRequest>=0 || pipeRequest>=0)return;
    const auto mario=fieldNpcTalkPlayer();if(!mario.talkable())return;
    FieldWallServices rays{this,[](void* p,FieldWallVector origin,FieldWallVector direction,float length){return fieldMapRay(origin,direction,length,&static_cast<Game*>(p)->fieldObjectTriangles,false);}};
    NpcTalkScan scan;
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(actor.kind || !actor.sourceTalkPresent || !actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        auto it=fieldNpcs.find(actor.key);if(it==fieldNpcs.end())continue;auto& state=it->second;
        if(!(state.body.flags&2) || (state.body.flags&8))continue; // Source skipped slots do not resume.
        const bool admitted=scan.consider({actor.key,state.body.root,actor.npcWidth,actor.npcHeight,state.body.flags,true},mario,fieldNpcAngles(),rays);
        if(state.scripted)continue;
        NpcTalkTurn turn{state.body.state,state.secondaryFlags,state.movement.yaw,state.savedYaw};
        NpcTalkTurnServices turns{&state,
            [](void* p){static_cast<FieldNpcInstance*>(p)->paused=true;},
            [](void* p){static_cast<FieldNpcInstance*>(p)->paused=false;}};
        if(!actor.wander.enabled)turns={}; // Current static source regular IDs/scripts are null.
        updateNpcTalkTurn(turn,admitted,state.body.flags,state.body.root,mario.root,fieldNpcAngles(),turns);
        state.body.state=turn.state;state.secondaryFlags=turn.secondaryFlags;
        state.movement.yaw=turn.yaw;state.savedYaw=turn.savedYaw;
    }
    fieldNpcEventCandidate=uint32_t(scan.candidate);
}
// NPC active/visible gate is flags&3==3. Source body flag0x8000 publishes
// presentation flag0x100; camera-facing work belongs to simulation, not draw.
void Game::updateFieldNpcPresentation() {
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(actor.kind || !actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        auto found=fieldNpcs.find(actor.key);if(found==fieldNpcs.end() || !found->second.initialized)continue;
        auto& instance=found->second;const auto& body=instance.body;
        if(instance.scripted)continue;
        if(!actor.wander.enabled) {
            instance.poseClock.update(actor.pose,actor.talkPose,
                fieldActorTalk.actor.key==actor.key?&fieldActorTalk.message:nullptr,ticks*1000/TickRate);
            instance.pose=instance.poseClock.pose;instance.poseAt=instance.poseClock.poseAt;
        }
        if((body.flags&3)!=3){instance.presentationReady=false;continue;}
        NpcMatrixInput input;input.root={body.root.x,body.root.y,body.root.z};input.yaw=instance.movement.yaw;input.drawScale=actor.scale;input.flags=body.flags;
        if(body.flags&0x8000)instance.presentation.flags|=0x100;else instance.presentation.flags&=~0x100u;
        NpcCameraService camera{&fieldCameraFrame,[](void* p){const auto& f=*static_cast<FieldCameraFrame*>(p);return NpcCamera{{f.eye.x*100,f.eye.y*100,f.eye.z*100},{f.at.x*100,f.at.y*100,f.at.z*100}};}};
        instance.presentationReady=instance.presentation.prepare(input,fieldNpcAngles(),camera);
    }
}
// Retain prior body root until the admitted source end snapshot. Scripted
// placement generation changes reset that root to avoid spurious collision deltas.
// Wander dt uses milliseconds/1000; a gap>500ms admits16ms as a native bridge.
// Pose speed<.10000000149011612 is stay, below source run threshold is walk.
void Game::updateFieldNpcCollision() {
    fieldNpcHit=0;synchronizeFieldNpcs();
    if((scene!=Scene::Explore && scene!=Scene::Dialogue) || trioBattleActive || water.active() ||
       arriving || fade.active() || warpRequest>=0 || pipeRequest>=0)return;
    NpcSidePlayer mario;mario.root={progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100};
    // Walk/dash publish retained travel; staying/air drift do not replace it
    // with the local wall-query or display heading.
    mario.travelHeading=fieldNpcAngles().angle(0,0,fieldMovement.headingX,fieldMovement.headingZ);
    NpcSideServices services;services.user=this;
    struct MapMoveContext {Game* game;NpcWanderActor* actor=nullptr;} mapContext{this};
    NpcMapMoveServices mapMove;mapMove.user=&mapContext;
    mapMove.direction=[](void*,float x,float z) {
        const float degrees=fieldNpcAngles().angle(0,0,x,z);
        const float radians=(degrees*3.1415927410125732421875f)/180.f;
        return FieldWallVector{float(std::sin(double(radians))),0,-float(std::cos(double(radians)))};
    };
    mapMove.length=[](void*,float x,float z){return std::sqrt(std::fma(x,x,z*z));};
    mapMove.normalLength=[](void*,float x,float z){return std::sqrt(x*x+z*z);};
    mapMove.angle=[](void*,float x,float z){return fieldNpcAngles().angle(0,0,x,z);};
    mapMove.ray=[](void* p,FieldWallVector origin,FieldWallVector direction,float length) {
        return fieldMapRay(origin,direction,length,&static_cast<MapMoveContext*>(p)->game->fieldObjectTriangles,false);
    };
    mapMove.publishContact=[](void* p,const FieldWallHit& hit){static_cast<MapMoveContext*>(p)->actor->wall=hit.token;};
    services.wall=[](void* p,FieldWallVector& root,const NpcSideWallRequest& request) {
        auto& game=*static_cast<Game*>(p);FieldWallActor actor;
        actor.flags=uint32_t(game.fieldIdle.mainFlags);actor.viewFlags=game.fieldViewFlags;
        actor.motion=game.fieldJump.motion;actor.counter=game.fieldJump.ticks;
        actor.width=request.width;actor.height=request.height;
        const float heading=request.heading*.01745329238474369f;
        FieldWallServices rays;rays.user=p;
        rays.ray=[](void* p,FieldWallVector origin,FieldWallVector direction,float length) {
            return fieldWallRay(origin,direction,length,&static_cast<Game*>(p)->fieldObjectTriangles);
        };
        correctFieldWallsAround(root,{std::sin(heading),0,-std::cos(heading)},request.mode,actor,rays);
    };
    for(const auto& actor:fieldActorCatalog().actors()) {
        if(actor.kind || !actor.npcFlags)continue;
        const bool professor=fieldActorCatalog().models()[actor.model].asset=="professor";
        const bool scripted=professor && (progress.room==2 || progress.room==5) &&
            professorStory.triggered;
        if(scripted) {if(actor.storyLow!=8 || !professorStory.actors[2].npcActive)continue;}
        else if(!actor.visible(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags))continue;
        auto root=actor.position;uint32_t flags=actor.npcFlags;uint64_t placement=0;
        if(scripted) {
            const auto& source=professorStory.actors[2];root=source.position;placement=source.placementGeneration;
            flags=(actor.npcFlags&~source.flagMask)|(source.flags&source.flagMask);
        }
        const auto inserted=fieldNpcs.try_emplace(actor.key);auto& instance=inserted.first->second;
        if(!scripted && !instance.initialized && actor.initialization!=FieldActorInit::Static) {
            const auto initial=fieldActorInitialPosition(actor,progress.originalNpcTalkState);
            if(!initial){instance.seen=ticks;continue;} // Unknown source bank retains the pending init.
            root=*initial;instance.body.previous=instance.body.root={root[0],root[1],root[2]};
            instance.movement.root=instance.body.root;instance.positionOverride=true;++instance.placement;
        }
        if(!scripted && instance.positionOverride){root={instance.body.root.x,instance.body.root.y,instance.body.root.z};placement=instance.placement;}
        if(!actor.wander.enabled && !scripted && !instance.initialized) {
            instance.movement.yaw=actor.yaw;instance.initialized=true;
        }
        if(actor.wander.enabled && instance.initialized) {
            root={instance.body.root.x,instance.body.root.y,instance.body.root.z};flags=instance.movement.flags;
        }
        const FieldWallVector position{root[0],root[1],root[2]};
        if(inserted.second || instance.scripted!=scripted || instance.placement!=placement)instance.body.previous=position;
        auto& body=instance.body;body.token=actor.key;body.root=position;body.width=actor.npcWidth;
        body.height=actor.npcHeight;body.flags=flags;
        instance.scripted=scripted;instance.placement=placement;instance.seen=ticks;
        const bool admitted=npcSideUpdateAdmitted(body);
        if(actor.wander.enabled && instance.initialized && (body.state&0x10) && (body.flags&3)==3) {
            const auto now=ticks*1000/TickRate;const auto elapsed=now-instance.clock;instance.clock=now;
            instance.movement.dt=float(elapsed>500?16:elapsed)/1000.f;
            if(instance.movement.dt>0) {
                if(body.flags&0x40000000) {
                    body.root.x+=instance.movement.components.x;body.root.z+=instance.movement.components.z;
                } else {
                    mapContext.actor=&instance.movement;
                    const auto moved=advanceNpcMapSide(body.root,body.width,body.height,
                        instance.movement.components.x,instance.movement.components.z,body.flags,mapMove);
                    assert(moved);body.root=moved->root;
                }
                // Admitted original profiles have no vertical/gravity writer:
                // npcMoveG captures/clears zero and exits before Y/contact writes.
                assert(instance.movement.components.y==0 && !(body.flags&(0x20000|0x10)));
                instance.movement.components={};
            }
        }
        if(admitted)respondNpcSide(body,mario,fieldNpcAngles(),services);
        if(actor.wander.enabled && instance.initialized) {
            instance.movement.root=body.root;
            if((body.flags&0x400) && instance.movement.dt>0) {
                const float x=body.root.x-body.previous.x,z=body.root.z-body.previous.z;
                const float speed=std::sqrt(std::fma(x,x,z*z))/instance.movement.dt;
                const int phase=speed<.10000000149011612f?1:speed<actor.wander.runThreshold?2:3;
                if(phase!=instance.posePhase){instance.posePhase=phase;instance.poseAt=ticks*1000/TickRate;instance.pose=phase==1?actor.wander.idle:phase==2?actor.wander.walk:actor.wander.run;}
            }
        }
        if(scripted)professorStory.actors[2].position={body.root.x,body.root.y,body.root.z};
        // Verified source records have null regular/all interrupt script slots.
        // Profile IDs are gor_02/Chu3 and gor_02/Darkly, not reusable model IDs.
        // Future actor profiles need their own original script providers.
        if((body.flags&3)==3 && !scripted && actor.room==2 &&
           (actor.key==0x525f654d || actor.key==0xe4c10e3d))
            consumeNpcNullInterrupts(instance.secondaryFlags,body.state);
        if((body.flags&3)==3)body.previous=body.root; // Rejected initial slots do not reach the source end snapshot.
    }
    for(auto it=fieldNpcs.begin();it!=fieldNpcs.end();) {
        if(it->second.seen!=ticks)it=fieldNpcs.erase(it);else ++it;
    }
    fieldNpcHit=uint32_t(mario.hitNpc);
    if(mario.hitNpc)progress.position={mario.root.x*.01f,mario.root.z*.01f};
}
bool Game::recoveryReturn(ReturnPoint& point) const {return groundReturn(returnContact,point);}
bool Game::beginWater() {
    ReturnPoint target;
    if(!recoveryReturn(target)) {
        // Original ordinary map fallbacks, engine units/100. The generic
        // vector is zero; gor_00/gor_04 have explicit source overrides.
        target=progress.room==1?ReturnPoint{0,0,-2}:progress.room==4?ReturnPoint{4,.35f,-1}:ReturnPoint{};
    }
    float floor=target.y;
    if(!groundAt({target.x,target.z},target.y,floor))return false;
    target.y=floor;
    water.begin({progress.position.x,groundHeight+jumpHeight,progress.position.z},{target.x,target.y,target.z});
    marioPresentation.displayFlags&=~0x100u;
    const float heading=marioPresentation.adjustMoveDirection()*.01745329238474369f;
    fieldMovement.headingX=std::sin(heading);fieldMovement.headingZ=-std::cos(heading);
    marioPresentation.requestPose("M_F_1",true); // Original water entry effect pose.
    fieldCameraDisplayFlags|=0x10000; // mot_damage entry retains camera-follow publication.
    fieldMovement.speed=0;
    jumpHeight=0;fieldJump={};fieldJump.motion=31;fieldStep={};fieldVerticalVelocity=0;warpRequest=-1;saveRequested=false;return true;
}
// Restore rebuilds per-room/transient Game state but explicitly retains session
// services, original Mario pose handles/facing and camera clocks/history. Copy
// saved BEFORE replacing *this because saved may alias this->progress.
// Native road/story setup is gated by explicit room/story, never harness victory.
void Game::restore(const Progress& saved) {
    const auto retainedCommandPalette=commandPalette;
    const auto retainedShadows=shadows;
    const auto retainedCameras=cameraManager;
    const auto retainedBero=beroExec;
    const auto retainedSequence=originalSequence;
    const auto retainedSoundFlags=soundFlags;
    const auto retainedSpotlight=battleSpotlight;
    const auto retainedTrioRandom=trioRandom;
    const auto retainedPublicSfx=publicSfx;
    const auto retainedPartyCurrentIndex=fieldPartyCurrentIndex;
    const uint32_t retainedFieldMarioCameraId=fieldMarioBattle.cameraId;
    const auto retainedMarioPoseHandles=fieldMarioBattle.poseHandles;
    decltype(fieldParties.poses) retainedMarioPoses;
    for(auto handle:retainedMarioPoseHandles)if(fieldParties.pose(handle))retainedMarioPoses[unsigned(handle)]=std::move(fieldParties.poses[unsigned(handle)]);
    partnerPresentation={};partnerPresentation.startTick=partnerPresentation.nowTick=ticks;
    const Progress p=saved; // Caller may pass this game's own progress.
    const uint64_t cameraClock=fieldCameraSimulationTicks,shiftTimestamp=fieldCameraShift.previousMilliseconds;
    auto cameraEvents=fieldCameraEvents;cameraEvents.roomLoad();
    const auto retainedFrame=fieldCameraFrame;const bool retainedAuthored=fieldCameraAuthored;
    const auto retainedCameraFlags=fieldCameraFlags&~0x10u;
    const auto retainedPresentation=marioPresentation;
    // A full Game reset exceeds the remaining native stack under checkpoint
    // validation. Keep the reset work on the heap, then move its owned state.
    auto reset=std::make_unique<Game>(false);
    *this = std::move(*reset);
    fieldPartyCurrentIndex=retainedPartyCurrentIndex;
    commandPalette=retainedCommandPalette;
    shadows=retainedShadows;
    cameraManager=retainedCameras;
    soundFlags=retainedSoundFlags; // Source sound flags survive field/checkpoint/battle adapters.
    originalSequence=retainedSequence;originalSequence->importNativeField(); // Native checkpoint admission resets unsaved callback history.
    beroExec=retainedBero;beroExec->unknown(); // Restore is not original get_info; unsaved transition history is unavailable.
    battleSpotlight=retainedSpotlight;
    trioRandom=retainedTrioRandom;
    publicSfx=retainedPublicSfx;
    fieldMarioBattle.cameraId=retainedFieldMarioCameraId; // Source reinit preserves the camera ID across record reset.
    fieldMarioBattle.poseHandles=retainedMarioPoseHandles;fieldParties.poses=std::move(retainedMarioPoses);
    fieldParties.reinitializeMario(fieldMarioBattle); // Original live pose material clear, then light|2; clocks/resources survive.
    marioPresentation.facing=retainedPresentation.facing;marioPresentation.display=retainedPresentation.display;marioPresentation.target=retainedPresentation.target;
    fieldCameraEvents=cameraEvents;fieldCameraFlags=retainedCameraFlags;
    fieldCameraFrame=retainedFrame;fieldCameraAuthored=retainedAuthored;
    fieldCameraSimulationTicks=cameraClock;fieldCameraShift.previousMilliseconds=shiftTimestamp;
    progress = p;
    fenceCollisionDisabled=p.room==2 && p.originalStory.value_or(0)>=10;
    troubleDoorNotice=false;
    fieldDoor.interiorMask=p.interiorMask;
    applyDoorCollision(p.room,fieldDoor.interiorMask);
    harborStory.reset(p.room==1 && p.originalStory && *p.originalStory==3);
    harborResume=p.room==1 && p.originalStory==4;
    // Native checkpoint follower placement starts30 engine units left of Mario,
    // falling back to Mario's root when that offset is blocked.
    partner = {p.position.x-0.3f,p.position.z};
    groundHeight=p.elevation;partnerHeight=p.elevation;
    if (!walkable(partner,p.elevation)) partner = p.position;
    refreshFieldObjects();
    groundAt(p.position,p.elevation,groundHeight,&groundContact,nullptr,&fieldObjectTriangles,&groundObjectKey);
    groundMetadataHeight=supportMetadataHeight(groundHeight);
    roomRespawn={p.position.x,groundHeight,p.position.z};
    groundAt(partner,groundHeight,partnerHeight,&partnerContact);
    partnerFollow.reset({partner.x,partnerHeight,partner.z},{p.position.x,groundHeight,p.position.z});
    cameraX=p.position.x;cameraZ=p.position.z;cameraHeight=groundHeight;
    rebaseFieldCamera();
    registerRoadCases(true);
    if(p.room==2 && p.originalStory==10)professorStory.beginLecture(ticks*1000/60,
        {p.position.x*100,groundHeight*100,p.position.z*100},{partner.x*100,partnerHeight*100,partner.z*100},p.originalFlag1207);
    if(p.room==5 && p.originalStory && *p.originalStory<=11)professorStory.beginUnderground(ticks*1000/60,*p.originalStory,
        {p.position.x*100,groundHeight*100,p.position.z*100},{partner.x*100,partnerHeight*100,partner.z*100},p.originalFlag1207);
    if(p.room==2 && p.originalStory==11)professorStory.restoreProfessor(p.originalFlag1207);
    saveObjects=std::make_shared<FieldSaveObjects>();
    saveObjects->configure(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags,ticks*1000/60);
}
void Game::updateSaveObjects() {
    if(!saveObjects || (scene!=Scene::Explore && scene!=Scene::Dialogue) || trioBattleActive ||
       water.active() || fade.active() || arriving || warpRequest>=0 || pipeRequest>=0)return;
    saveObjects->configure(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags,ticks*1000/60);
    saveObjects->switches={progress.originalStory.value_or(0),progress.originalFlag233,progress.originalFlag236,progress.originalSaveMode};
    const bool wasLocked=saveLocked();
    saveObjects->tickObjects(ticks*1000/60,headObjectKey,headContact.sourceNode,fieldJump.motion);
    if(saveLocked()){scene=Scene::Dialogue;saveRequested=false;}
    else if(wasLocked)scene=Scene::Explore;
}
bool Game::checkpointGround(float& height) {
    refreshFieldObjects();
    return groundAt(progress.position,progress.elevation,height,nullptr,nullptr,&fieldObjectTriangles);
}
// Partner floor adapter applies its distinct attribute filter and rejects
// water mask0x200. It samples Game-owned object triangles alongside room ground.
void Game::updatePartner() {
    refreshFieldObjects();
    PartnerFollowServices services;
    services.free=[](CameraPoint p){return walkable({p.x,p.z},p.y);};
    services.floor=[this](CameraPoint p,float low,float high,float& height) {
        GroundContact contact;FieldSlope slope;
        if(!groundWithin({p.x,p.z},low,high,height,&contact,nullptr,&slope,true,false,true,&fieldObjectTriangles))return false;
        return !(contact.attributes&0x200);
    };
    services.project=[this](CameraPoint p){return projectCameraRoot(p,fieldCameraFrame.view,fieldCameraFrame.projection,fieldCameraFrame.viewport);};
    services.ownerDirection={fieldMovement.headingX,0,fieldMovement.headingZ};
    CameraPoint root{partner.x,partnerHeight,partner.z};
    partnerFollow.tick(root,{progress.position.x,groundHeight+jumpHeight,progress.position.z},
                       fieldJump.jumping,fieldMovement.speed,services);
    partner={root.x,root.z};partnerHeight=root.y;partnerContact={};
    if(partnerFollow.motion==PartnerFollow::Motion::Ground) {
        float floor=root.y;groundAt(partner,root.y,floor,&partnerContact,nullptr,&fieldObjectTriangles);
    }
}
// Scoped bank1 follower bridge chooses Goombella vs Mario from source distance
// decision. Paper yaw retains90/270 unless movement heading enters10..170 or
//190..350 degrees; movement tolerance1e-6 is native positional noise policy.
void Game::updateTemporaryProfessor() {
    if(!professorStory.temporaryProfessor || (professorStory.locked() && !(fieldDoor.active() && professorStory.fenceStarted)) || (progress.room!=2 && progress.room!=5) ||
       (scene!=Scene::Explore && scene!=Scene::Dialogue) || warpRequest>=0 || fade.active())return;
    PartnerFollowServices services;
    services.free=[](CameraPoint p){return walkable({p.x,p.z},p.y);};
    services.floor=[](CameraPoint p,float low,float high,float& height) {
        GroundContact contact;FieldSlope slope;
        return groundWithin({p.x,p.z},low,high,height,&contact,nullptr,&slope,true,false,true) && !(contact.attributes&0x200);
    };
    services.project=[this](CameraPoint p){return projectCameraRoot(p,fieldCameraFrame.view,fieldCameraFrame.projection,fieldCameraFrame.viewport);};
    services.ownerDirection={fieldMovement.headingX,0,fieldMovement.headingZ};
    const CameraPoint mario{progress.position.x,groundHeight+jumpHeight,progress.position.z};
    const CameraPoint goombella{partner.x,partnerHeight,partner.z};
    const float dx=mario.x-goombella.x,dy=mario.y-goombella.y,dz=mario.z-goombella.z;
    // This slice binds ordinary Goombella kind1, running mode1, status0. The source
    // selector preserves the special-state branches for future party binding.
    const bool other=secondaryPartyUsesOther(true,1,1,0,std::sqrt(dx*dx+dy*dy+dz*dz)*100.f);
    auto& actor=professorStory.actors[2];
    CameraPoint root{actor.position[0]*.01f,actor.position[1]*.01f,actor.position[2]*.01f};
    const auto before=root;
    professorFollow.tick(root,other?goombella:mario,
        other?partnerFollow.motion==PartnerFollow::Motion::Jump:fieldJump.jumping,fieldMovement.speed,services);
    actor.position={root.x*100.f,root.y*100.f,root.z*100.f};
    actor.moving=PartnerFollow::distance(root,before)>.000001f;
    if(actor.moving) {
        const float heading=PartnerPresentation::wrap(std::atan2(root.x-before.x,before.z-root.z)*57.29577951308232f);
        if(heading>=10 && heading<=170)actor.yaw=90;
        else if(heading>=190 && heading<=350)actor.yaw=270;
    }
}
bool Game::beginHarborEncounter(bool resume) {
    if(!harborEncounter.begin(ticks*1000/60,{progress.position.x*100,groundHeight*100,progress.position.z*100},progress.hp,resume))return false;
    const auto& m=harborEncounter.actors[0].position;
    progress.position={m[0]*.01f,m[2]*.01f};groundHeight=progress.elevation=m[1]*.01f;
    harborStory.enabled=harborStory.triggered=harborStory.finished=true;harborCameraCursor=0;
    return true;
}
// Harness dialogue kind1 has two development notices; other kinds three.
// These notices/prompts are separate from original message IDs and story scenes.
void Game::openDialogue(int kind) {
    scene = Scene::Dialogue; dialogueKind = kind; dialoguePage = 0;
    sound = Sound::Confirm;
}
int Game::dialoguePages() const { return dialogueKind == 1 ? 2 : 3; }
const char* Game::dialogue() const {
    // Development notices only: never impersonate original story dialogue.
    static const char* notices[] = {
        "Development notice:\nOriginal US dialogue is pending.",
        "Development notice:\nStory event sequencing is pending.",
        "Development notice:\nReturn to the exploration harness."
    };
    static const char* encounter[] = {
        "Development notice:\nOriginal Crump scene is pending.",
        "Development notice:\nContinue to the battle harness."
    };
    return dialogueKind == 1 ? encounter[dialoguePage] : notices[dialoguePage];
}
const char* Game::prompt() const {
    if(harborTutorial.running())return harborTutorial.failed?"Development notice: original tutorial initialization unavailable":harborTutorial.introComplete?"Development notice: original tutorial action services pending":harborTutorial.message.waiting()?"A / B: next":"A / B: reveal text";
    if(saveObjects && saveObjects->failed)return "Development notice: save service unavailable";
    if(saveLocked())return saveObjects->presentation.selection.active()?"A: select     B: cancel":"A / B: next";
    if(harborEncounter.locked())return harborEncounter.failed?"Development notice: source encounter unavailable":harborEncounter.battleRequested?"Development notice: original tutorial battle service pending":harborEncounter.message.waiting()?"A / B: next":"A / B: reveal text";
    if(fieldActorTalk.message.active())return fieldActorTalk.message.waiting()?"A / B / touch: next":"A / B: reveal text";
    if(harborStory.locked())return harborStory.failed?"Development notice: source message unavailable":harborStory.message.waiting()?"A / B / touch: next":"A / B: reveal text";
    if(scene==Scene::FieldDefeat)return "A: load save";
    if(water.active() || fade.active() || arriving || warpRequest>=0)return "";
    if (scene == Scene::Dialogue) return "A / touch: next     B: close";
    if (scene == Scene::Victory) return "Crump defeated! A: return to plaza";
    if (scene == Scene::Defeat) return "A: retry     B: return to plaza";
    if (scene == Scene::Battle) {
        if (phase == Phase::Select) return "Choose on touch screen / D-pad + A";
        if (phase == Phase::Windup) return selection == 0 ? "Press A just before the landing!" : "Hold Circle Pad left; release when charged!";
        if (phase == Phase::EnemyWindup) return "Press A just before Crump lands!";
        if (phase == Phase::EnemyImpact) return guarded ? "Guard!" : "Hit taken.";
        return actionSuccess ? "Nice!" : "Keep your timing steady.";
    }
    if (progress.room==0 && !progress.crumpDefeated && near(CrumpPosition)) return "A: confront Lord Crump";
    if (progress.room==0 && near(NpcPosition)) return "A: talk";
    if(fieldNpcTalkCandidateAlive()) {
        for(const auto& actor:fieldActorCatalog().actors())if(actor.key==npcTalkCandidate && progress.originalStory && !fieldNpcTalkBound(actor))return "Development notice: NPC event unavailable";
        return "A: talk";
    } // Retained simulation result; no render-time map ray.
    return "Circle Pad: move   A: jump   X: partner";
}
void Game::rebaseFieldCamera() {
    // Harness placement adapter; original map/event camera setup is distinct.
    marioCamera.retained={progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100};
    fieldCamera.point={cameraX*100,cameraHeight*100,cameraZ*100};fieldCameraInitialized=true;
    if(cameraRoadAsset(progress.room))fieldCameraFlags|=4;
    if(fieldCameraViewAdapter && !fieldCameraAuthored)fieldCameraFrame=adaptedFieldCameraFrame(cameraX,cameraHeight,cameraZ);
}
// Field flags2 pauses this view update;4 enables camera roads. Collision
// target adds1 ENGINE unit to the retained contact Y. Prepare/finish use
// candidates and commit only admitted camera state; view mode3 bypasses roads.
void Game::beginFieldCameraTick() {
    if(!fieldCameraInitialized)rebaseFieldCamera();
    if(fieldCameraFlags&2){fieldMovement.headingOffsetDegrees=cameraHeadingSnapshot(fieldCameraFrame.yaw);return;}
    // camMain precedes EVT/Mario: consume LAST MarioCamMain publication.
    fieldCamera.consume(marioCamera,fieldCameraFlags);
    fieldCameraCollisionTarget={marioCameraCollision.x,marioCameraCollision.y+1,marioCameraCollision.z};
    const uint64_t now=fieldCameraTickMilliseconds(fieldCameraSimulationTicks);
    auto events=fieldCameraEvents;
    const bool prepared=events.prepare(fieldCamera.point,fieldCameraCollisionTarget,now);
    const bool roadUsed=prepared && events.view.state!=3 && (fieldCameraFlags&4);
    const float rootY=water.active()?water.position[1]:groundHeight+jumpHeight;
    if(roadUsed)fieldCameraShift.update(retainedFieldMainX,
        {progress.position.x*100,rootY*100,progress.position.z*100},fieldCameraFlags,
        fieldCameraCandidateActive?fieldCameraCandidateShiftRate:0,now);
    cameraX=fieldCamera.point.x*.01f;cameraHeight=fieldCamera.point.y*.01f;cameraZ=fieldCamera.point.z*.01f;
    if(fieldCameraViewAdapter && prepared) {
        if(const auto* asset=cameraRoadAsset(progress.room)) {
            auto next=cameraRoadWork;
            std::optional<FieldCameraFrame> roadFrame;
            if(roadUsed && next.update(*asset,events.follow,events.look,fieldCameraShift.current,now))roadFrame=cameraRoadFrame(next.current);
            if(!roadUsed || roadFrame) {
                CameraPoint eye=events.storedEye,at=events.storedAt;
                if(roadFrame){eye={roadFrame->eye.x*100,roadFrame->eye.y*100,roadFrame->eye.z*100};at={roadFrame->at.x*100,roadFrame->at.y*100,roadFrame->at.z*100};}
                CameraPoint finalEye,finalAt;
                if(events.finish(eye,at,finalEye,finalAt,roadUsed)) {
                    const auto frame=fieldCameraFrameFromView({finalEye.x*.01f,finalEye.y*.01f,finalEye.z*.01f},
                        {finalAt.x*.01f,finalAt.y*.01f,finalAt.z*.01f},events.fov,events.nearPlane*.01f,events.farPlane*.01f);
                    if(frame) {
                        cameraRoadWork=next;fieldCameraFrame=*frame;fieldCameraAuthored=true;
                        fieldCameraCandidateActive=next.candidate.active;
                        fieldCameraCandidateShiftRate=next.candidate.shiftRate;
                    }
                }
            }
        } else {fieldCameraFrame=adaptedFieldCameraFrame(cameraX,cameraHeight,cameraZ);fieldCameraAuthored=false;}
    }
    if(prepared)fieldCameraEvents=events;
    fieldCameraShift.post(now); // All admitted A branches post, including preset view.
    fieldMovement.headingOffsetDegrees=cameraHeadingSnapshot(fieldCameraFrame.yaw);
}

bool Game::setCameraTarget(CameraPoint xyz,float percent,int32_t durationMs,uint8_t curve) {
    return fieldCameraEvents.setTarget(xyz,percent,durationMs,curve,fieldCameraTickMilliseconds(fieldCameraSimulationTicks));
}
bool Game::releaseCameraTarget(int32_t durationMs,uint8_t curve) {
    return fieldCameraEvents.targetOff(durationMs,curve,fieldCameraTickMilliseconds(fieldCameraSimulationTicks));
}
bool Game::setCameraView(CameraPoint eye,CameraPoint at,int32_t durationMs,uint8_t curve) {
    const auto actualEye=fieldCameraFrame.eye,actualAt=fieldCameraFrame.at;
    return fieldCameraEvents.setView(eye,at,{actualEye.x*100,actualEye.y*100,actualEye.z*100},
        {actualAt.x*100,actualAt.y*100,actualAt.z*100},durationMs,curve,fieldCameraTickMilliseconds(fieldCameraSimulationTicks));
}
bool Game::releaseCameraView(int32_t durationMs,uint8_t curve) {
    const auto eye=fieldCameraFrame.eye,at=fieldCameraFrame.at;
    return fieldCameraEvents.viewOff({eye.x*100,eye.y*100,eye.z*100},{at.x*100,at.y*100,at.z*100},
        durationMs,curve,fieldCameraTickMilliseconds(fieldCameraSimulationTicks));
}
void Game::publishFieldCameraTarget() {
    const float rootY=water.active()?water.position[1]:groundHeight+jumpHeight;
    marioCamera.publish({progress.position.x*100,rootY*100,progress.position.z*100},
                        fieldCameraFrame.yaw,fieldCameraDisplayFlags,fieldJump.motion);
    fieldCameraDisplayFlags&=~0x40000u; // Tail owns the transient flag; retained contact persists.
}
void Game::publishFieldCameraCollision(const FieldSupport& support) {
    if(!support.found || !support.contact.sourceNode)return; // No-pack flat-floor adapter has no source HitObj.
    marioCameraCollision={support.point.x*100,support.point.y*100,support.point.z*100};
    fieldCameraDisplayFlags|=0x40000;
}
void Game::cacheFieldScreen() {
    const float rootY=water.active()?water.position[1]:groundHeight+jumpHeight;
    fieldScreen=projectCameraRoot({progress.position.x,rootY,progress.position.z},
                                 fieldCameraFrame.view,fieldCameraFrame.projection,fieldCameraFrame.viewport);
}
// This function composes recovered owners; order differs for ascent and fall.
// Ground classes0/1/2=stay/walk/dash. Retained travel is independent of current
// stick drift and display facing. An already-launched call skips double gravity.
// Angular constants57.295779513 and.01745329238474369 convert radians/degrees;
// source angles are degrees while host sin/cos take radians.
void Game::move(const Input& i,bool launched) {
    refreshFieldObjects();wallObjectKey=0;
    retainedFieldMainX=fieldKeysOff?0:int8_t(std::clamp(i.x,-1.f,1.f)*70);
    roadPassContact={}; // marioMoveMain clears +1f0 before moving OR staying.
    wallContact={}; // Native forward cache must not retain a departed door.
    // Original marioMoveMain remembers the eligible floor before moving.
    if(groundContact.attributes&0x40000000)returnContact=groundContact;
    float x=i.x, z=i.z, len=std::hypot(x,z);
    fieldInputMagnitude=len;
    // Airborne motion retains takeoff travel and adds current-stick drift;
    // release does not replace retained travel with zero or a fresh walk rate.
    const auto& axes=inputProducer.channels[0].analog;
    const bool stepping=fieldStep.active();
    FieldMovement::Displacement grounded;
    if(!fieldJump.airborne && !stepping) {
        grounded=i.physicalSnapshot?fieldMovement.groundedStick(axes.mainX,int8_t(-axes.mainY)):
            fieldMovement.grounded(x,z);
        fieldJump.enterGround(fieldMovement.speed==0?0:fieldMovement.speed==DashSpeed?2:1,groundHeight);
        if(fieldMovement.speed==0)fieldVerticalVelocity=0; // Idle entry clears jump tuple.
    }
    // Ordinary probes use travel1a4 or source toMovedir(display1ac).
    const float facingAngle=marioPresentation.moveHeading()*3.14159265358979323846f/180.f;
    moveDirection=fieldMovement.speed!=0?Vec2{fieldMovement.headingX,fieldMovement.headingZ}:Vec2{std::sin(facingAngle),-std::cos(facingAngle)};
    // Ordinary ascent: jump, overhead, move. Falling keeps move before gravity.
    const bool wasFalling=fieldJump.airborne && fieldJump.falling;
    float previousY=0;const bool verticalReady=!wasFalling && integrateVertical(i.jumpHeld,previousY,!launched);
    if(water.active())return;
    // Ascent overhead can clear air flags before marioMove. Recompute the cap
    // from the flags after that owner, rather than before the vertical pass.
    const auto combined=fieldJump.airborne?
        fieldMovement.airborne(x,z,fieldJump.suppressDrift || fieldJump.ticks==0,fieldJump.hasAirFlags()):
        stepping?fieldMovement.retained():fieldMovement.reduceSlope(grounded);
    // Direction gain reads PRE-wrapper retained facing1a0; wrapper metadata
    // publishes the requested combined heading only afterward.
    fieldMovement.facingDegrees=marioPresentation.facing;
    const auto travel=fieldMovement.applyMovementRate(combined);
    float requestedHeading=marioPresentation.moveHeading();
    if(combined.x!=0 || combined.z!=0)requestedHeading=MarioPresentation::wrap(std::atan2(combined.x,-combined.z)*180/3.14159265358979323846f);
    const float requestedNorm=std::hypot(combined.x,combined.z)*fieldMovement.movementRate();
    marioPresentation.wallRequest(requestedNorm*100,requestedHeading,fieldJump.ticks);
    if(len>0){x/=len;z/=len;}
    Vec2 p = progress.position;
    Vec2 candidate{p.x+travel.x,p.z+travel.z};
    const bool sourceWalls=collisionReady;
    if(sourceWalls) {
        FieldWallActor actor;actor.viewFlags=fieldViewFlags;
        actor.motion=fieldJump.airborne?fieldJump.motion:stepping?12:
            fieldMovement.speed==0?0:fieldMovement.speed==DashSpeed?2:1;
        actor.counter=fieldJump.ticks;
        FieldWallServices services;services.user=this;services.ray=[](void* context,FieldWallVector from,FieldWallVector direction,float distance) {
            return fieldWallRay(from,direction,distance,&static_cast<Game*>(context)->fieldObjectTriangles);
        };
        FieldWallVector actual{p.x*100,(groundHeight+jumpHeight)*100,p.z*100};
        const FieldWallVector displacement{travel.x*100,0,travel.z*100};
        FieldWallMove front;
        wallContactDistance=10;
        if(travel.x!=0 || travel.z!=0) {
            front=searchFieldFrontWall(actual,displacement,actor,services);
            if(front.forward.token && fieldWallPassThrough(front.forward.attributes,actor)) {
                actual.x+=displacement.x;actual.z+=displacement.z;
            } else {
                actual=front.position;
                if(front.distanceWritten)wallContactDistance=front.contactDistance*.01f;
            }
            // Original wrapper ignores Around's return; its corrected root is
            // already committed. Dynamic contacts/hazards remain separate owners.
            correctFieldWallsAround(actual,displacement,0,actor,services);
            candidate={actual.x*.01f,actual.z*.01f};
        } else {
            // Staying probes disposable positions and never moves the root.
            const float offset=fieldMovement.headingOffsetDegrees*3.14159265358979323846f/180.f;
            const Vec2 priorTravel{fieldMovement.headingX*std::cos(offset)+fieldMovement.headingZ*std::sin(offset),
                                   fieldMovement.headingZ*std::cos(offset)-fieldMovement.headingX*std::sin(offset)};
            front=searchFieldFrontWall(actual,{priorTravel.x*10,0,priorTravel.z*10},actor,services);
            if(!front.forward.token)front=searchFieldFrontWall(actual,{moveDirection.x*10,0,moveDirection.z*10},actor,services);
            candidate=p;
        }
        if(front.forward.token) {
            wallObjectKey=front.forward.objectKey;
            const size_t n=front.forward.token-1;
            wallContact=wallObjectKey?GroundContact{front.forward.attributes,0}:
                runtimeContact(!head.empty()?head[n].contact:n<wallContacts.size()?wallContacts[n]:GroundContact{});
            if(fieldWallPassThrough(front.forward.attributes,actor))roadPassContact=wallContact;
        }
    }
    auto accept=[&](Vec2 candidate) {
        const float y=groundHeight+jumpHeight;
        if(!sourceWalls && !walkable(candidate,y))return false;
        if(sourceWalls && ground.empty() &&
           (candidate.x < -6.7f+BodyRadius || candidate.x > 6.7f-BodyRadius ||
            candidate.z < -4.5f+BodyRadius || candidate.z > 5.8f-BodyRadius))return false;
        p=candidate;return true;
    };
    if(sourceWalls)accept(candidate);
    else {
        // Unconverted development fixtures retain the old placeholder bounds.
        accept({p.x+travel.x,p.z});accept({p.x,p.z+travel.z});
    }
    progress.position=p;progress.elevation=groundHeight;
    if(!fieldJump.airborne && !stepping) {
        queryOverhead(groundHeight,fieldVerticalVelocity,.1f);
    }
    if(!stepping && !fieldJump.airborne) {
        const float offset=fieldMovement.speed!=0?fieldMovement.headingOffsetDegrees*3.14159265358979323846f/180.f:0;
        const Vec2 footDirection{moveDirection.x*std::cos(offset)+moveDirection.z*std::sin(offset),
                                moveDirection.z*std::cos(offset)-moveDirection.x*std::sin(offset)};
        // Support and forward queries are ordered owners. The front query uses
        // the still-unsnapped root and resets the slope record even on a miss.
        const auto support=searchFieldSupport(p,footDirection,groundHeight,fieldVerticalVelocity,fieldMovement.slope,.11f,&fieldObjectTriangles);
        publishFieldCameraCollision(support);
        groundContact=support.found?support.contact:GroundContact{};groundObjectKey=support.objectKey;
        if(support.found)groundMetadataHeight=supportMetadataHeight(support.height);
        float height=groundHeight,slope=0;
        const bool front=fieldJump.ticks==0 && queryFrontStep(p,moveDirection,groundHeight,height,slope,&fieldMovement.slope,&fieldObjectTriangles);
        if(front && stairRiseAllowed(groundHeight,height,slope) && fieldMovement.speed!=0) {
            fieldStep.begin(groundHeight,height);saveRequested=false;
        } else {
            groundHeight=progress.elevation=support.height;
            if(!support.found && support.delta!=0) {
                jumpBase=groundHeight;fieldJump.beginFall(support.delta>0);
            }
        }
    }
    // Legacy billboard turn adapter: stick dead zone.15, ten-tick turn,
    // minimum width scale.08. Source Mario paper presentation is a separate owner.
    if (std::fabs(x) > .15f && (x>0 ? 1.f : -1.f) != facing) {
        facing = x>0 ? 1.f : -1.f; turnTick=10;
    }
    if (turnTick>0) { --turnTick; turnScale=std::max(.08f,std::fabs(std::cos(turnTick*3.14159265f/10.f))); }
    else turnScale=1;
    if(stepping) {
        // Upstairs motion12 also records contact without the jump-only response.
        queryOverhead(groundHeight,fieldStep.velocity,.1f);
    }
    cacheFieldScreen(); // marioMove wrapper projects root before the falling vertical owner.
    if(wasFalling)advanceVertical(i.jumpHeld,false);
    else if(verticalReady)finishVertical(previousY);
    updatePartner();
    if(airborne())saveRequested=false;
    if(!water.active())publishFieldCameraTarget();
}
void Game::checkFieldRoadCases() {
    // Source field root runs caseMain after the NPC owner.
    if(warpRequest<0 && scene==Scene::Explore && !water.active() && !arriving && !fade.active() && pipeRequest<0) {
        for(size_t n=0;n<roads.size();++n) {
            const auto& road=roads[n];
            if(road.source==progress.room && roadContactMatches(roadPassContact.sourceNode,road.sourceNode,roadCasesEnabled,roadAdmission)) {
                warpRequest=int(n);resolveRoadCallbacks(road);beroExec->onOff(1,0x10000);beroExec->onOff(0,1);departureOrigin=progress.position;departureHeight=groundHeight;departureOriginValid=true;saveRequested=false;break;
            }
        }
    }
}
FieldBattleServices Game::fieldBattleServices() {
    FieldBattleServices s;s.user=this;
    s.work=[](void* p)->FieldBattleWork& {return static_cast<Game*>(p)->fieldBattleWork;};
    // Single NPC/partner development fixtures, not original registry IDs.
    s.npc=[](void* p,int32_t name)->uintptr_t {const auto& g=*static_cast<Game*>(p);return name==3 && g.professorStory.underground?3:1;};
    s.prepareMario=[](void* p) {static_cast<Game*>(p)->fieldPrepareRequested=true;};
    s.marioControl=[](void* p,bool enabled) {
        auto& g=*static_cast<Game*>(p);
        if(enabled)fieldMarioControlOn(g.fieldMarioBattle);else fieldMarioControlOff(g.fieldMarioBattle);
        g.marioFieldControl=fieldMarioControlOffCheck(g.fieldMarioBattle)==0;
    };
    s.partyId=[](void* p)->int32_t {const auto& g=*static_cast<Game*>(p);return g.fieldParties.current(g.fieldMarioBattle);};
    s.party=[](void* p,int32_t index)->uintptr_t {return reinterpret_cast<uintptr_t>(static_cast<Game*>(p)->fieldParties.pointer(index));};
    s.partyControl=[](void* p,uintptr_t,bool enabled) {
        auto& g=*static_cast<Game*>(p);g.fieldParties.control(enabled);g.partnerFieldControl=g.fieldParties.controlsEnabled();
    };
    s.soundFlag=[](void* p,uint32_t mask) {auto& g=*static_cast<Game*>(p);g.soundFlags->set(mask,{},g.fieldSoundSetCursor);};
    s.setSequence=[](void* p,int32_t sequence) {auto& g=*static_cast<Game*>(p);g.fieldSequence=sequence;g.originalSequence->request(sequence,0,0);};
    s.sequence=[](void* p)->int32_t {return static_cast<Game*>(p)->fieldSequence;};
    s.stopBattleBgm=[](void* p) {static_cast<Game*>(p)->fieldBgmStopRequested=true;};
    s.postMario=[](void* p) {static_cast<Game*>(p)->fieldPostRequested=true;};
    return s;
}
// Native fieldSequence2 indicates this adapter's field return request. It
// must not be confused with original current-sequence IDs used by other services.
bool Game::returnFromBattle(int32_t harnessResult) {
    // Development outcome tokens only; original outcome producers are unrecovered.
    fieldBattleWork.result=harnessResult;fieldSequence=2;
    return waitFieldBattleEnd(fieldBattleServices())==2;
}
// Legacy Crump harness slots0/1 and fixture stats differ from original tutorial
// and trio registries. Zero defense arrays have5 elemental lanes; reserve32 is
// a native request-vector allocation hint, not a source event queue capacity.
void Game::startBattle() {
    fieldPrepareRequested=fieldPostRequested=fieldBgmStopRequested=false;
    startFieldBattle(0,true,fieldBattleServices());
    scene=Scene::Battle; phase=Phase::Select; phaseTick=0; selection=0;
    enemyHP=BattleRules::CrumpHP; actionSuccess=false; guarded=false;
    generalBattlePads={};actionCommandPad={};defenseCommand={};jumpCommand={};hammerCommand={};
    jumpCommand.callbacksLive=false;
    hammerCommand.mainLive=hammerCommand.resultLive=hammerCommand.displayLive=hammerCommand.deleteLive=false;
    lastDamageCode=0;
    lastActionHelpRequest=0;
    playerBattleUnit={};enemyBattleUnit={};
    playerBattleUnit.kind=222;playerBattleUnit.hp=int16_t(progress.hp);
    playerBattleUnit.baseHp=playerBattleUnit.maxHp=10; // Development player-stat fixture.
    enemyBattleUnit.hp=int16_t(enemyHP); // Boss kind/stats await original unit initialization.
    enemyBattleUnit.baseHp=enemyBattleUnit.maxHp=int16_t(BattleRules::CrumpHP);enemyBattleUnit.belong=1;
    clearUnitStatus(playerBattleUnit);clearUnitStatus(enemyBattleUnit);
    playerBattlePart={};enemyBattlePart={};pendingDamageRequests.clear();pendingDamageRequests.reserve(32);
    static constexpr int8_t zeros[5]={};
    playerBattlePart.defense=enemyBattlePart.defense=zeros;
    playerBattlePart.defenseAttributes=enemyBattlePart.defenseAttributes=zeros;
    jumpHeight=0; fieldJump={};fieldStep={};fieldMovement={};fieldVerticalVelocity=0;
}
void Game::attackDone() {
    playerBattleUnit.hp=int16_t(progress.hp);enemyBattleUnit.hp=int16_t(enemyHP);
    auto weapon=selection==0?ordinaryJumpWeapon():ordinaryHammerWeapon();
    const uint32_t result=selection==0?jumpCommand.result:hammerCommand.result;
    const int damage=harnessDamage(playerBattleUnit,enemyBattleUnit,enemyBattlePart,weapon,0,result,
        jumpLevelFixture,hammerLevelFixture,lastDamageCode);
    applyBattleDamage(true,damage,0,lastDamageCode);
    phase=Phase::Impact; phaseTick=0;
    sound=actionSuccess ? Sound::Success : Sound::Hit;
}
// Record the latest hit's effect/audio/notice/event requests for later binding.
// Clearing the trace does not execute/drain those services. Stored lastHp narrows
// signed8:128 is the sign boundary,256 the byte wrap modulus.
void Game::applyBattleDamage(bool enemyTarget,int32_t hp,int32_t fp,uint32_t code) {
    struct Context {
        DamageUnit* player;DamageUnit* enemy;DamageUnit* target;
        DamagePart* part;std::vector<DamageServiceRequest>* requests;
    } context{&playerBattleUnit,&enemyBattleUnit,enemyTarget?&enemyBattleUnit:&playerBattleUnit,
        enemyTarget?&enemyBattlePart:&playerBattlePart,&pendingDamageRequests};
    pendingDamageRequests.clear();
    DamageApplicationServices services;services.user=&context;
    // Fixed harness slots; original registry/part/position bindings are still open.
    services.getUnit=[](void* user,int32_t id)->DamageUnit* {
        auto& c=*static_cast<Context*>(user);return id==0?c.player:id==1?c.enemy:nullptr;
    };
    services.bodyPart=[](void* user,DamageUnit&)->DamagePart* {return static_cast<Context*>(user)->part;};
    services.hitPosition=[](void*,DamageUnit&,DamagePart&)->DamagePosition {return {};};
    services.unitPosition=[](void*,DamageUnit&)->DamagePosition {return {};};
    services.height=[](void*,DamageUnit&)->int32_t {return 0;};
    services.effect=[](void* user,const DamageEffect& effect) {
        auto& c=*static_cast<Context*>(user);DamageServiceRequest request;
        request.unit=c.target;request.effect=effect;c.requests->push_back(request);
    };
    services.sound=[](void* user,const char* key,DamagePosition position) {
        auto& c=*static_cast<Context*>(user);DamageServiceRequest request;
        request.kind=DamageServiceRequest::Kind::Sound;request.unit=c.target;
        request.soundKey=key;request.position=position;c.requests->push_back(request);
    };
    services.notice=[](void* user,DamageUnit& unit,DamageNotice notice) {
        auto& c=*static_cast<Context*>(user);DamageServiceRequest request;
        request.kind=DamageServiceRequest::Kind::Notice;request.unit=&unit;request.notice=notice;c.requests->push_back(request);
    };
    services.hitEvent=[](void* user,DamageUnit& unit,uint32_t code) {
        auto& c=*static_cast<Context*>(user);DamageServiceRequest request;
        request.kind=DamageServiceRequest::Kind::HitEvent;request.unit=&unit;request.code=code;c.requests->push_back(request);
    };
    services.inventory={nullptr,[](void*,int32_t)->int32_t {return 0;}}; // No revival item in harness fixture.
    applyDirectDamage(enemyTarget?0:1,*context.target,context.part,hp,fp,code,0,1,battleRandom,services);
    lastDamage=context.target->lastHp<128?context.target->lastHp:int(context.target->lastHp)-256;
    progress.hp=playerBattleUnit.hp;enemyHP=enemyBattleUnit.hp;
}
// General pad updates precede harness sequence setup; private AC pad update
// and main callbacks precede later StartAC/result event requests. This chronology
// matters even when the main callback is inactive. Counts advance once per tick.
void Game::battle(const Input& i) {
    BattlePadSample padInput=inputProducer.channels[0].battleSample();
    auto stick=[](float value) {return std::isfinite(value)?int8_t(std::clamp(value,-1.f,1.f)*70):int8_t(0);};
    if(!i.physicalSnapshot) {
        // Abstract callers supply already-produced logical edges (host scenarios).
        padInput.pressed=(i.confirm?BattleButtonA:0)|(i.cancel?BattleButtonB:0);
        padInput.snapshot.buttons=(i.jumpHeld?BattleButtonA:0)|(i.cancelHeld?BattleButtonB:0);
        padInput.released=(i.confirmReleased?BattleButtonA:0)|(i.cancelReleased?BattleButtonB:0);
        padInput.snapshot.mainX=stick(i.x);padInput.snapshot.mainY=stick(-i.z);padInput.repeat=0;
    }
    for(size_t channel=0;channel<4;++channel)
        generalBattlePads[channel].update(channel==0?padInput:inputProducer.channels[channel].battleSample());
    HammerContext hammerInput;
    hammerInput.setupOptions=1; // Ordinary SetupAC(-2,3,1,0), actor token clear in harness.
    hammerInput.difficulty=attackDifficulty(3,0,0); // Default weapon byte3, no badge modifiers.
    // Adapt the Circle Pad's normalized70-unit range to original main-stick X.
    hammerInput.held=generalBattlePads[0].held.entries[0];
    // Native selection IDs0 Jump,1 Hammer,2 Mushroom; wrap modulo3. Touch
    // region x[8,312),y[100,158) uses102-pixel columns. Mushroom heals5 to
    // harness max10. Impact42/enemy-impact36 are provisional simulation delays.
    const bool selecting=phase==Phase::Select;
    bool startCommand=false;
    // Harness sequence adapter runs between general and private pad updates.
    if (selecting) {
        if (i.left) selection=(selection+2)%3;
        if (i.right) selection=(selection+1)%3;
        int touch=-1;
        if (i.touchY>=100 && i.touchY<158 && i.touchX>=8 && i.touchX<312) touch=(i.touchX-8)/102;
        if (touch>=0 && touch<3) selection=touch;
        const bool confirmInput=i.physicalSnapshot?(inputProducer.channels[0].pressed&BattleButtonA)!=0:i.confirm;
        if (confirmInput || touch>=0) {
            if (selection==2) {
                if (progress.mushrooms<=0 || progress.hp==10) { sound=Sound::Confirm; }
                else {
                    --progress.mushrooms; progress.hp=std::min(10,progress.hp+5);
                    playerBattleUnit.hp=int16_t(progress.hp);
                    lastDamage=0;lastDamageCode=0; actionSuccess=false; phase=Phase::Impact; phaseTick=0;
                }
            } else {
                phase=Phase::Windup; phaseTick=0; actionSuccess=false;
                if(selection==0) {
                    // Harness setup: per-attack count reset, ordinary route and
                    // fixed difficulty, pending original script/tutorial binding.
                    jumpCommand={};
                } else {
                    // Ordinary default-weapon policy, pending actual Crump event binding.
                    hammerCommand={};hammerCommand.period=ordinaryHammerPeriod(0x80353de8);
                }
                startCommand=true;
            }
            sound=Sound::Confirm;
        }
    }
    // Original private manager samples even when its AC main is inactive.
    actionCommandPad.update(padInput);
    if(jumpCommand.callbacksLive)jumpCommand.tick();
    if(hammerCommand.mainLive && hammerCommand.tick(hammerInput,battleRandom)==0)
        hammerCommand.mainLive=false;
    // Later event adapter requests StartAC after initialization in this pass.
    struct EventCommandContext {
        TimingACommand* jump;HammerCommand* hammer;
        const BattleTriggerHistory* history;BattleRandom* random;int* help;
    } commandContext{&jumpCommand,&hammerCommand,&generalBattlePads[0].triggers,&battleRandom,&lastActionHelpRequest};
    ActionCommandView command;
    command.user=&commandContext;command.actorFlags=&playerBattleUnit.statusFlags;
    command.help=[](void* user,int32_t type) {*static_cast<EventCommandContext*>(user)->help=type;};
    if(selection==0) {
        command.state=&jumpCommand.state;command.result=&jumpCommand.result;command.count=&jumpCommand.count;
        command.timingOptions=&jumpCommand.flags;
        if(jumpCommand.callbacksLive) {
            command.evaluate=[](void* user)->int32_t {
                auto& c=*static_cast<EventCommandContext*>(user);
                TimingAContext context;context.difficulty=BattleRules::JumpDifficulty;
                return static_cast<int32_t>(c.jump->evaluate(*c.history,context,*c.random));
            };
            command.remove=[](void* user) {static_cast<EventCommandContext*>(user)->jump->state=1000;};
        }
    } else {
        command.state=&hammerCommand.state;command.result=&hammerCommand.result;command.count=&hammerCommand.count;
        if(hammerCommand.resultLive)command.evaluate=[](void* user)->int32_t {
            return static_cast<int32_t>(static_cast<EventCommandContext*>(user)->hammer->result);
        };
        if(hammerCommand.deleteLive)command.remove=[](void* user) {static_cast<EventCommandContext*>(user)->hammer->stop();};
    }
    ActionValueServices eventValues;
    eventValues.resolve=[](void*,int32_t literal)->int32_t {return literal;}; // Harness literal adapter, not original tagged VM.
    if(startCommand)startActionCommand(command,selection==0?1:0,eventValues);
    if(selecting)return;
    ++phaseTick;
    if (phase==Phase::Windup) {
        if (selection==0) {
            if (phaseTick>=BattleRules::JumpImpact) {
                if(resultActionCommandEvent(command)==2) {
                    actionSuccess=(jumpCommand.result&2)!=0;
                    stopActionCommandEvent(command); // Harness call site, original graph still unbound.
                    attackDone(); // One documented hit; follow-up Jump event remains unbound.
                }
            }
        } else {
            if (resultActionCommandEvent(command)==2) {
                actionSuccess=(hammerCommand.result&2)!=0;
                stopActionCommandEvent(command); // Harness call site; original event order remains open.
                attackDone();
            }
        }
    } else if (phase==Phase::Impact && phaseTick>=42) {
        if (enemyHP==0) { scene=Scene::Victory; sound=Sound::Success; }
        else { phase=Phase::EnemyWindup; phaseTick=0; guarded=false; }
    } else if (phase==Phase::EnemyWindup) {
        if (phaseTick>=BattleRules::EnemyImpact) {
            DefenseContext context;context.hp=int16_t(progress.hp);
            // Harness weapon gate0: do not invent tutorial superguard eligibility.
            const int defense=defenseCommand.evaluate(actionCommandPad,context);
            guarded=defense==4 && actionCommandPossible(playerBattleUnit);
            // The recovered normal enemy event requests CheckDamage(flags0x100).
            // Guard flag selection here is still a harness dispatch adaptation.
            playerBattleUnit.hp=int16_t(progress.hp);enemyBattleUnit.hp=int16_t(enemyHP);
            lastDamage=harnessDamage(enemyBattleUnit,playerBattleUnit,playerBattlePart,harborBossWeapon(),
                0x100|(guarded?0x40000:0),0,jumpLevelFixture,hammerLevelFixture,lastDamageCode);
            applyBattleDamage(false,lastDamage,0,lastDamageCode);
            phase=Phase::EnemyImpact; phaseTick=0; sound=guarded?Sound::Success:Sound::Hit;
        }
    } else if (phase==Phase::EnemyImpact && phaseTick>=36) {
        if (progress.hp==0) scene=Scene::Defeat;
        else { phase=Phase::Select; phaseTick=0; }
    }
}
// Source key flag8 uses a BYTE nesting depth; balanced KeyOn/Off requests
// are required (no saturation). Level2 sets low mask3/stops type2; level0
// clears low nibble/resumes types0xef. Camera8 mask0x200/sound mask0x80
// belong to their separate owners, not the Mario control flag2.
bool Game::applyFieldItemStateRequest(ItemGetRequest r) {
    switch(r) {
    case ItemGetRequest::EffectPayload5:acquisitionEffect.camera(5);return true;
    case ItemGetRequest::Camera8FlagOn:itemCamera8Flags|=0x200;return true;
    case ItemGetRequest::Camera8FlagOff:itemCamera8Flags&=~uint16_t(0x200);return true;
    case ItemGetRequest::KeyOff:
        fieldMarioBattle.flags|=8;fieldMarioBattle.keyDepth=uint8_t(fieldMarioBattle.keyDepth+1);fieldKeysOff=true;return true;
    case ItemGetRequest::KeyOn:
        fieldMarioBattle.keyDepth=uint8_t(fieldMarioBattle.keyDepth-1);
        if(!fieldMarioBattle.keyDepth)fieldMarioBattle.flags&=~8u;
        fieldKeysOff=(fieldMarioBattle.flags&8) || goombaAcquisition.active() || goombaReward.active();return true;
    case ItemGetRequest::SystemLevel2:
        itemSystemRequest=2;itemSystemMask&=0xfffffff3;itemSystemMask|=3;goombaAcquisition.stopEvents(2);return true;
    case ItemGetRequest::SystemLevel0:
        itemSystemRequest=0;itemSystemMask&=0xfffffff0;goombaAcquisition.startEvents(0xef);return true;
    case ItemGetRequest::SoundFlag80On:return soundFlags->set(0x80,{},fieldSoundSetCursor);
    case ItemGetRequest::SoundFlag80Off:soundFlags->clear(0x80);return true;
    case ItemGetRequest::ClearMarioMotion:fieldMovement.speed=0;return true; // Unmodeled additive X/Z and norm have no retained native producers.
    case ItemGetRequest::Camera4:fieldMarioBattle.cameraId=4;return true;
    default:return applyFieldItemMarioRequest(r);
    }
}
bool Game::applyFieldItemMarioRequest(ItemGetRequest r) {
    const uint32_t main=uint32_t(fieldIdle.mainFlags)|fieldMarioBattle.flags;
    switch(r) {
    case ItemGetRequest::GetItemMotion:case ItemGetRequest::StayMotion:
        return requestFieldItemMarioMotion(fieldJump,fieldMovement,main,marioPresentation,
            r==ItemGetRequest::GetItemMotion,fieldItemMarioPost);
    case ItemGetRequest::ControlOff:
        fieldMarioControlOff(fieldMarioBattle);marioFieldControl=(fieldMarioBattle.flags&2)==0;return true;
    case ItemGetRequest::ControlOn:
        fieldMarioControlOn(fieldMarioBattle);marioFieldControl=(fieldMarioBattle.flags&2)==0;return true;
    default:return false;
    }
}
void Game::updateMarioPresentation() {
    fieldIdleSounds.clear();
    marioPresentation.offset=fieldMovement.headingOffsetDegrees;
    enterFieldItemMarioMotion(fieldJump,fieldMovement,uint32_t(fieldIdle.mainFlags)|fieldMarioBattle.flags,marioPresentation);
    const bool active=!(fieldMarioBattle.flags&2) && scene==Scene::Explore && fieldJump.motion==0 && !arriving && warpRequest<0 &&
        !water.active() && !fade.active();
    if(active) {
        FieldKeyServices keys;
        keys.partyId=[](){return 1;}; // Current follower harness, not original pouch/party ownership.
        keys.party=[this](int){return &fieldKeyPartner;};
        keys.sequence=[this](){return fieldSequence;};
        keys.inputByte=[this](){return int8_t(fieldMarioBattle.keyDepth);};
        keys.shipChanging=[this](){return fieldShipChanging;};
        keys.rollEnabled=[this](){return fieldRollEnabled;};keys.slitEnabled=[this](){return fieldSlitEnabled;};
        keys.christineStatus=[this](){return fieldChristineStatus;};keys.bomheiKeyCheck=[this](){return fieldBomheiKeyCheck;};
        keys.yoshiStatus=[this](){return fieldYoshiStatus;};keys.holdItem=[this](){return fieldHeldItem;};
        FieldIdleServices services;
        services.hp=[this](){return int(int16_t(progress.hp));};services.rate=[](){return TickRate;};
        services.millisecondsToFrames=[](int ms){return int(int64_t(ms)*TickRate/1000);};
        services.key=[&](){return fieldKeyAdmitted(keys);};
        services.currentPose=[this](){return marioPresentation.selected;};
        services.requestPose=[this](const char* name,bool effect){
            marioPresentation.displayFlags=uint32_t(fieldIdle.displayFlags);
            marioPresentation.poseStartFrame=uint16_t(fieldIdle.startFrame);
            marioPresentation.requestPose(name,effect);fieldIdle.displayFlags=int32_t(marioPresentation.displayFlags);
            fieldIdle.startFrame=int16_t(marioPresentation.poseStartFrame);
        };
        services.sound=[this](int id){fieldIdleSounds.push_back(id);};
        services.adjustMoveDirection=[this](){
            const float h=marioPresentation.adjustMoveDirection();
            const float radians=h*.01745329238474369f;
            fieldMovement.headingX=std::sin(radians);fieldMovement.headingZ=-std::cos(radians);
        };
        fieldIdle.displayFlags=int32_t(marioPresentation.displayFlags);
        if(!fieldIdleWasActive)fieldIdle.enter(services);
        fieldIdle.early(services);fieldIdle.late(services);
        marioPresentation.displayFlags=uint32_t(fieldIdle.displayFlags);
        marioPresentation.poseStartFrame=uint16_t(fieldIdle.startFrame);
    }
    fieldIdleWasActive=active;
    if(scene==Scene::Explore || scene==Scene::Dialogue) {
        marioPresentation.update(fieldJump.motion,fieldMovement.speed,fieldMovement.headingOffsetDegrees,progress.hp,
                                 fieldCameraTickMilliseconds(fieldCameraSimulationTicks),true);
        fieldIdle.displayFlags=int32_t(marioPresentation.displayFlags);
        fieldIdle.startFrame=int16_t(marioPresentation.poseStartFrame);
    }
}
// One simulation visit. Entry consumes prior camera/NPC publications; local
// exit owner always runs on return. Priority branches intentionally prevent
// ordinary input from advancing a locked scene, but neutral movement may keep
// airborne/support physics alive. See game-composition guide for full order.
void Game::tick(const Input& rawInput) {
    fenceCollisionDisabled=progress.room==2 && (progress.originalStory.value_or(0)>=10 || professorStory.fenceCollisionOff);
    applyDoorCollision(progress.room,fieldDoor.interiorMask);
    // Scope-exit publication is gameplay work, not just rendering. Every early
    // return still updates save objects/NPC responses/talk/poses/itemMain/cases.
    // Do not move this tail solely to the ordinary Explore branch.
    struct PresentationOnExit {
        Game& game;Vec2 previous;const Input& raw;
        ~PresentationOnExit(){
            game.updateSaveObjects();
            game.updateTemporaryProfessor();
            game.updateFieldNpcCollision(); // Source NPC response follows Mario/object movement.
            game.progress.interiorMask=uint8_t(game.fieldDoor.interiorMask);
            if(game.harborEncounter.running() || game.fieldDoor.active() || game.professorStory.running() || game.professorStory.temporaryProfessor)game.saveRequested=false;
            if(!game.professorStory.locked() && !game.harborEncounter.locked())game.updateMarioPresentation();
            Input talkInput=raw;
            if(raw.physicalSnapshot)talkInput.confirm=(game.inputProducer.channels[0].pressed&BattleButtonA)!=0;
            game.updateFieldNpcCandidates();
            game.updateFieldNpcTalk(talkInput); // NPC owner follows current Mario/object updates.
            game.updateFieldNpcPresentation();
            if(game.harborStory.enabled && game.harborStory.triggered) {
                auto& story=game.harborStory;
                for(auto& a:story.actors) {
                    const auto root=std::array<float,3>{a.x,a.y,a.z};
                    a.actualSpeed=a.hasPreviousRoot?std::hypot(root[0]-a.previousRoot[0],root[2]-a.previousRoot[2])*TickRate:0;
                    a.previousRoot=root;a.hasPreviousRoot=true;
                }
                NpcCameraService camera{&game.fieldCameraFrame,[](void* p){const auto& f=*static_cast<FieldCameraFrame*>(p);return NpcCamera{{f.eye.x*100,f.eye.y*100,f.eye.z*100},{f.at.x*100,f.at.y*100,f.at.z*100}};}};
                const bool encounter=game.harborEncounter.running();
                story.preparePresentation(fieldNpcAngles(),camera,encounter?&game.harborEncounter.message:&story.message,
                    encounter?game.harborEncounter.speaker:story.speaker,game.ticks*1000/TickRate);
            }
            const uint32_t itemTriggers=raw.physicalSnapshot?game.inputProducer.channels[0].pressed:
                (raw.confirm?0x100u:0u)|(raw.cancel?0x200u:0u);
            game.goombaAcquisition.itemMain(itemTriggers); // Item owner follows this visit's Mario/NPC owners.
            game.checkFieldRoadCases();
            const auto& axes=game.inputProducer.channels[0].analog;
            const int mainX=raw.physicalSnapshot?axes.mainX:std::isfinite(raw.x)?int(std::clamp(raw.x,-1.f,1.f)*70.f):0;
            const int mainY=raw.physicalSnapshot?axes.mainY:std::isfinite(raw.z)?int(std::clamp(raw.z,-1.f,1.f)*70.f):0;
            // Source stick-class radius55 squared=3025; classes1/2 select
            // follower walk/run presentation after mapping to +/-70 PAD units.
            const int movementClass=mainX*mainX+mainY*mainY<=3025?1:2;
            const bool ordinary=(game.scene==Scene::Explore || game.scene==Scene::Dialogue) &&
                !game.water.active() && !game.fade.active() && !game.arriving && game.warpRequest<0;
            game.partnerPresentation.update(ordinary?game.partner.x-previous.x:0,
                ordinary?game.partner.z-previous.z:0,movementClass,game.ticks,game.fieldMovement.headingOffsetDegrees,
                game.partnerFollow.motion!=PartnerFollow::Motion::Ground);
            game.troubleBoardPresentationMain(raw);
        }
    } presentation{*this,partner,rawInput};
    Input input=rawInput;const Input& i=input;
    ++ticks;++fieldCameraSimulationTicks;openingDeparture.advance(ticks,progress.room); sound=Sound::None;overheadContact=false;headObjectKey=0;headContact={};
    if(scene==Scene::Explore || scene==Scene::Dialogue) {
        beginFieldCameraTick();
        fieldItemCamera5Frame=itemGetCamera5Frame(fieldCameraFrame.eye,{0,1,0},fieldCameraFrame.at);
    }
    if(goombaAcquisition.active()) {
        goombaAcquisition.eventMain(); // Event manager precedes this visit's Mario/NPC/item owners.
        if(goombaAcquisition.complete()){applyFieldItemStateRequest(ItemGetRequest::KeyOn);scene=Scene::Explore;}
    }
    if(goombaReward.active()) {
        // EVT precedes current Mario/input-owner publication. Native physical
        // triggers use the retained producer snapshot; host fixtures supply A.
        fieldRewardTriggers=i.physicalSnapshot?inputProducer.channels[0].pressed:
            (i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
        goombaReward.eventMain(ticks*1000/60);
        if(goombaReward.complete()){applyFieldItemStateRequest(ItemGetRequest::KeyOn);scene=Scene::Explore;}
    }
    troubleBoardEventMain(i); // EVT reads the previous input publication; windows run at scope exit.
    if(arriving || warpRequest>=0)retainedFieldMainX=0; // Source road choreography keys off before Mario input publication.
    if(i.physicalSnapshot) {
        std::array<RawPad,4> raw{};raw[0].buttons=i.physicalButtons;
        auto stick=[](float value) {return std::isfinite(value)?int8_t(std::clamp(value,-1.f,1.f)*70):int8_t(0);};
        raw[0].mainX=stick(i.x);raw[0].mainY=stick(-i.z);
        inputProducer.update(raw,TickRate,false); // Native polling/errors/C-stick/motors remain adapters.
        if(scene==Scene::Explore) {
            // Field consumers read the producer's signed-axis/buttons snapshot,
            // including integer quantization and its global input-clear policy.
            const auto& published=inputProducer.channels[0];
            input.x=float(published.analog.mainX)/70.f;
            input.z=-float(published.analog.mainY)/70.f;
            input.confirm=(published.pressed&BattleButtonA)!=0;
            input.jumpHeld=(published.buttons&BattleButtonA)!=0;
        }
    }
    updateFieldNpcEvents(); // Regular USER_FUNC consumes previous NPC publications before Mario.
    const bool tap=i.touchX>=0;
    if(!saveObjects)saveObjects=std::make_shared<FieldSaveObjects>();
    saveObjects->configure(progress.room,progress.originalStory,fieldDoor.interiorMask,fieldSystemFlags,ticks*1000/60);
    saveObjects->switches={progress.originalStory.value_or(0),progress.originalFlag233,progress.originalFlag236,progress.originalSaveMode};
    MessageSelectionInput saveInput;
    if(i.physicalSnapshot) {
        const auto& p=inputProducer.channels[0];
        saveInput={p.pressed,p.directionPressed,p.repeat,p.directionRepeat};
    } else saveInput.buttons=(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u)|(i.z<-.5f?8u:i.z>.5f?4u:0u);
    if(tap)saveInput.buttons|=0x100;
    saveObjects->tickEvents(ticks*1000/60,saveInput);
    progress.originalFlag233=saveObjects->switches.firstHit;progress.originalFlag236=saveObjects->switches.postStoryHit;
    progress.originalSaveMode=saveObjects->switches.mode;
    // Input/control locks suppress actions/saving but advance neutral Mario
    // movement. A held lock is not a universal simulation freeze.
    if(troubleBoardLocked()) {
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});saveRequested=false;return;
    }
    if(saveLocked()) {
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});saveRequested=false;return;
    }
    if(arriving) {
        fade.tick();const bool finished=roadMotion.tick();
        progress.position={roadMotion.position[0],roadMotion.position[1]};partner=progress.position;
        cacheFieldScreen();publishFieldCameraTarget();
        if(finished){arriving=false;registerRoadCases(true);beroExec->onOff(1,1);beroExec->onOff(0,0x10000);saveRequested=true;partnerFollow.reset({partner.x,partnerHeight,partner.z},{progress.position.x,groundHeight,progress.position.z});}
        return;
    }
    if(fade.active()){fade.tick();return;}
    if(pipeRequest>=0)return;
    // DeparturePause11 is the native60Hz bridge for150ms+2 source waits;
    // movement target offset1.5 world units=150 engine. Scene-specific
    // callbacks can refuse this generic transition without fabricating completion.
    if(warpRequest>=0) {
        if(size_t(warpRequest)>=roads.size()){cancelRoad();return;}
        if(!resolveRoadCallbacks(roads[warpRequest]))return; // Preserve unresolved/custom original scene ownership.
        // The original independent inline waits/moves while its parent runs
        // WAIT150ms + WAIT2frames, then requests mapchange without joining it.
        // This 60Hz timer/fade bridge is an adapter to that concurrent ownership;
        // complete EVT credits, key nesting and sequencer remain separate services.
        if(!departureParentStarted){departureParentStarted=true;departurePause=11;}
        const bool requestMap=--departurePause==0;
        if(requestMap)beroExec->onOff(0,8); // Original independent parent request8030dba0.
        // Key-off neutralizes input, not Mario simulation or retained air travel.
        if(fieldJump.airborne || fieldStep.active() || !groundContact.sourceNode) {
            move({});if(requestMap)fade.startOut();return;
        }
        if(!departureDone && !roadMotion.active()) {
            const auto& w=roads[warpRequest];
            float x=0,z=0;roadDirection(w.direction,x,z);
            if(!roadMotion.begin({progress.position.x,progress.position.z},{w.x+x*1.5f,w.z+z*1.5f})){cancelRoad();return;}
        }
        if(!departureDone) {
            if(std::fabs(roadMotion.step[0])>.001f)facing=roadMotion.step[0]>0?1:-1;
            turnScale=1;turnTick=0;
            if(roadMotion.tick()){departureDone=true;beroExec->onOff(0,0x10000);beroExec->onOff(1,1);}
            progress.position={roadMotion.position[0],roadMotion.position[1]};
            cacheFieldScreen();publishFieldCameraTarget();
        }
        if(requestMap)fade.startOut();
        return;
    }
    if(scene==Scene::FieldDefeat) {if(i.confirm || (tap && i.touchY>=107 && i.touchY<155))reloadRequested=true;return;}
    if(water.active()) {
        retainedFieldMainX=fieldKeysOff?0:int8_t(std::clamp(i.x,-1.f,1.f)*70);
        // Recovery phases15(void travel),12/14(water hidden/follow) are
        // original mot_damage states. Display bit1 hides Mario; camera-follow
        // mask0x10000 persists until Complete/Dead, independent of field control.
        const bool bottomlessTravel=water.kind==RecoveryKind::Void && water.state==15;
        const bool waterFollow=water.kind==RecoveryKind::Water && (water.state==12 || water.state==14);
        const int previousRecoveryState=water.state;
        const auto event=water.tick(progress.hp);
        if(water.kind==RecoveryKind::Water) {
            const bool wasHidden=previousRecoveryState==12 || previousRecoveryState==14;
            const bool hidden=water.state==12 || water.state==14;
            if(!wasHidden && hidden)marioPresentation.displayFlags|=1;
            else if(wasHidden && !hidden)marioPresentation.displayFlags&=~1u;
        }
        if(event==WaterEvent::Landed)marioPresentation.requestPose("M_D_2");
        else if(event==WaterEvent::GetUp)marioPresentation.requestPose("M_U_3");
        progress.position={water.position[0],water.position[2]};
        if(waterFollow)marioCamera.retained.y+=water.followDelta*100;
        if(bottomlessTravel || waterFollow){marioCamera.retained.x=water.position[0]*100;marioCamera.retained.z=water.position[2]*100;}
        cacheFieldScreen();
        if(event==WaterEvent::Landed || event==WaterEvent::Complete || event==WaterEvent::Dead) {
            groundHeight=water.position[1];progress.elevation=groundHeight;
            groundAt(progress.position,groundHeight,groundHeight,&groundContact);
        }
        if(event==WaterEvent::Complete) {
            if(water.kind==RecoveryKind::Void){partner=progress.position;partnerHeight=groundHeight;partnerContact=groundContact;}
            partnerFollow.reset({partner.x,partnerHeight,partner.z},{progress.position.x,groundHeight,progress.position.z});
        } else if(event==WaterEvent::Dead)scene=Scene::FieldDefeat;
        if(event==WaterEvent::Complete || event==WaterEvent::Dead)fieldCameraDisplayFlags&=~0x10000u;
        publishFieldCameraTarget();
        return; // Motion publication and partner/NPC lifecycle remain separate owners.
    }
    if(fieldDoor.active()) {
        beroExec->unknown(); // Interior mask choreography has no bound shared Bero child.
        const auto& door=fieldDoors()[size_t(fieldDoor.selected)];
        const auto previous=fieldDoor.position;
        const bool complete=fieldDoor.tick(door);
        progress.position={fieldDoor.position[0],fieldDoor.position[1]};
        const float dx=progress.position.x-previous[0],dz=progress.position.z-previous[1];
        // Scoped native party bridge: retain relative placement while the
        // source requests beside/door movement. Full party event is unbound.
        partner.x+=dx;partner.z+=dz;
        fieldMovement.speed=std::hypot(dx,dz);
        if(fieldMovement.speed) {
            fieldMovement.headingX=dx/fieldMovement.speed;fieldMovement.headingZ=dz/fieldMovement.speed;
            marioPresentation.facing=PartnerPresentation::wrap(std::atan2(dx,-dz)*57.29577951308232f);
        }
        applyDoorCollision(progress.room,fieldDoor.interiorMask);
        groundAt(progress.position,groundHeight,groundHeight,&groundContact);
        progress.elevation=groundHeight;partnerHeight=groundHeight;
        if(professorStory.locked()) {
            professorStory.actors[0].position={progress.position.x*100,groundHeight*100,progress.position.z*100};
            professorStory.actors[1].position={partner.x*100,partnerHeight*100,partner.z*100};
        }
        publishFieldCameraTarget();saveRequested=false;
        if(complete) {
            // Source curve11; closing releases over100ms, opening immediately.
            // Native follower trail is rebased after choreography completes.
            releaseCameraTarget(fieldDoor.opening?0:100,11);
            partnerFollow.reset({partner.x,partnerHeight,partner.z},{progress.position.x,groundHeight,progress.position.z});
        }
        return;
    }
    if(itemSelection && itemSelection->owner.retained()) {
        ItemSelectionInput selection;
        if(i.physicalSnapshot) {
            const auto& pad=inputProducer.channels[0];selection={pad.directionRepeat,pad.directionPressed,pad.pressed};
        } else {
            selection.buttons=(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
            selection.directionRepeat=selection.directionTrigger=i.z>.5f?0x2000u:i.z<-.5f?0x1000u:0;
        }
        if(tap)selection.buttons|=0x100;
        itemSelection->tick(selection,TickRate,uint32_t(ticks));
        if(goombaHandIn.active()) {
            goombaHandIn.tick(ticks*1000/60);
            fieldKeysOff=goombaHandIn.keyDepth()!=0;
            if(!goombaHandIn.active()){scene=Scene::Explore;progress.hp=progress.originalPouch->half(0x70);}
        }
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});saveRequested=false;return;
    }
    if(fieldActorTalk.message.active()) {
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});
        fieldActorTalk.message.tick(ticks*1000/60,(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u));
        if(goombaHandIn.active()) {
            goombaHandIn.tick(ticks*1000/60);
            fieldKeysOff=goombaHandIn.keyDepth()!=0;
            if(!goombaHandIn.active())progress.hp=progress.originalPouch->half(0x70);
        }
        if(troubleDoorNotice && !fieldActorTalk.message.active()) {
            applyFieldItemStateRequest(ItemGetRequest::KeyOn);troubleDoorNotice=false;
        }
        saveRequested=false;if(!fieldActorTalk.message.active() && !goombaHandIn.active() && !goombaAcquisition.active() && !goombaReward.active())scene=Scene::Explore;
        return;
    }
    if(goombaReward.active()){if(fieldJump.airborne)dispatchFieldAir();move(Input{});saveRequested=false;return;}
    if(goombaAcquisition.active()){saveRequested=false;return;}
    if(goombaHandIn.active()) {
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});goombaHandIn.tick(ticks*1000/60);
        fieldKeysOff=goombaHandIn.keyDepth()!=0;
        if(!goombaHandIn.active()){scene=Scene::Explore;progress.hp=progress.originalPouch->half(0x70);}
        saveRequested=false;return;
    }
    // Original trio services own their explicit command/attack continuations.
    // Camera adapter uses original608/480 lens aspect; exported pouch HP lives
    // at0x70. Do not route completion through the legacy Crump victory branch.
    if(trioBattleActive) {
        trioBattleIntro.tick(ticks*1000/60,(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u),false);
        while(trioBattleCameraCursor<trioBattleIntro.cameras.size()) {
            const auto& c=trioBattleIntro.cameras[trioBattleCameraCursor++];
            if(c.off)setCameraView({0,100,450},{0,50,0},0,11);
            else setCameraView({c.eye[0],c.eye[1],c.eye[2]},{c.at[0],c.at[1],c.at[2]},c.duration,uint8_t(c.curve));
        }
        if(trioBattleIntro.finished && !trioBattleIntro.message.active()) {
            if(trioAttack.active()) {
                const uint32_t triggers=i.physicalSnapshot?inputProducer.channels[0].pressed:(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
                const auto sample=inputProducer.channels[0].battleSample();
                // Native field lens snapshot in engine units. Roll/projection-extra history
                // remains an explicit adapter gap rather than a boot-value reset.
                trioAttack.setCameraDepthLensAdapter(fieldCameraFrame.fov,608.f/480.f,fieldCameraFrame.nearPlane*100,fieldCameraFrame.farPlane*100);
                trioAttack.tick(ticks*1000/60,trioBattleIntro,triggers,i.physicalSnapshot?&sample:nullptr);
                if(auto exported=trioAttack.exportedPouch()){progress.originalPouch=std::move(exported);progress.hp=progress.originalPouch->half(0x70);}
                scene=Scene::Dialogue;saveRequested=false;return;
            }
            if(trioAttack.finished()) {
                const uint32_t triggers=i.physicalSnapshot?inputProducer.channels[0].pressed:(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
                const auto sample=inputProducer.channels[0].battleSample();
                // Native field lens snapshot in engine units. Roll/projection-extra history
                // remains an explicit adapter gap rather than a boot-value reset.
                trioAttack.setCameraDepthLensAdapter(fieldCameraFrame.fov,608.f/480.f,fieldCameraFrame.nearPlane*100,fieldCameraFrame.farPlane*100);
                trioAttack.tick(ticks*1000/60,trioBattleIntro,triggers,i.physicalSnapshot?&sample:nullptr);
                if(auto exported=trioAttack.exportedPouch()){progress.originalPouch=std::move(exported);progress.hp=progress.originalPouch->half(0x70);}
                if(trioAttack.active())trioCommands.state=TrioBattleCommands::State::Closed; // Enemy root now owns the action.
                if(trioAttack.prepareCommands(trioCommands,trioBattleIntro) || trioAttack.commandActor()<0) {
                    scene=Scene::Dialogue;saveRequested=false;return;
                }
            }
            if(trioCommands.state==TrioBattleCommands::State::AttackReady && progress.originalPouch && !trioAttack.active() && !trioAttack.finished())progress.originalPouch->half(0x70,int16_t(progress.hp)); // Current field HP enters the original resource snapshot once.
            const auto partyAppearance=fieldPartyAppearanceServices();
            if(trioCommands.state==TrioBattleCommands::State::AttackReady && trioAttack.begin(trioCommands,trioBattleIntro,progress.hp,progress.originalPouch,&professorStory.actors[3].npcBattleFlags,&fieldMarioBattle,&fieldParties,&partyAppearance,battleSpotlight,trioRandom,publicSfx,shadows,commandPalette,cameraManager,beroExec)) {
                trioAttack.setOriginalSequence(originalSequence);
                trioAttack.setSoundFlags(soundFlags);
                const auto sample=inputProducer.channels[0].battleSample();
                const uint32_t triggers=i.physicalSnapshot?sample.pressed:(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
                // Native field lens snapshot in engine units. Roll/projection-extra history
                // remains an explicit adapter gap rather than a boot-value reset.
                trioAttack.setCameraDepthLensAdapter(fieldCameraFrame.fov,608.f/480.f,fieldCameraFrame.nearPlane*100,fieldCameraFrame.farPlane*100);
                trioAttack.tick(ticks*1000/60,trioBattleIntro,triggers,i.physicalSnapshot?&sample:nullptr);
                if(auto exported=trioAttack.exportedPouch()){progress.originalPouch=std::move(exported);progress.hp=progress.originalPouch->half(0x70);}
                scene=Scene::Dialogue;saveRequested=false;return;
            }
            if(trioCommands.state==TrioBattleCommands::State::Closed)trioCommands.begin(trioBattleIntro);
            else {
                trioCommands.syncPositions(trioBattleIntro);
                uint32_t buttons=(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
                // Adapt general direction masks to battle virtual left/right:
                // 0xa000=up/right,0x5000=down/left; physical D-pad6=right/down,
                //9=left/up. Preserve this source-menu axis folding.
                uint32_t repeats=(i.right?0x80000u:0u)|(i.left?0x40000u:0u);
                if(i.physicalSnapshot) {
                    const auto& pad=inputProducer.channels[0];
                    buttons=pad.pressed;
                    repeats=(pad.directionRepeat&0xa000?0x80000u:0u)|(pad.directionRepeat&0x5000?0x40000u:0u)|
                        (pad.repeat&6?0x80000u:0u)|(pad.repeat&9?0x40000u:0u);
                }
                if(tap && (trioCommands.state==TrioBattleCommands::State::Command || trioCommands.state==TrioBattleCommands::State::Target)) {
                    const bool target=trioCommands.state==TrioBattleCommands::State::Target;
                    const int row=BattleMenuLayout::touchedRow(i.touchX,i.touchY,target?trioCommands.count():trioCommands.actor==2?1:2);
                    if(row>=0) {
                        if(target)trioCommands.chooseTarget(unsigned(row));else trioCommands.chooseCommand(unsigned(row));
                        buttons|=0x100;
                    }
                }
                trioCommands.input(buttons,repeats);
            }
        }
        scene=Scene::Dialogue;saveRequested=false;return;
    }
    if(professorStory.running()) {
        const bool passive=!professorStory.locked();
        if(passive) {
            professorStory.actors[0].position={progress.position.x*100,groundHeight*100+jumpHeight*100,progress.position.z*100};
            professorStory.actors[1].position={partner.x*100,partnerHeight*100,partner.z*100};
        }
        const uint32_t keys=(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u);
        MessageSelectionInput choice;
        if(i.physicalSnapshot) {
            const auto& pad=inputProducer.channels[0];choice={pad.pressed,pad.directionPressed,pad.repeat,pad.directionRepeat};
        } else {
            choice.buttons=keys|(i.z<-.5f?8u:0u)|(i.z>.5f?4u:0u);
        }
        professorStory.fieldBattle=professorStory.underground?fieldBattleServices():FieldBattleServices{};
        professorStory.tick(ticks*1000/60,keys,false,choice);
        const auto& m=professorStory.actors[0].position;
        const auto& p=professorStory.actors[1].position;
        if(!passive || professorStory.locked()) {
            progress.position={m[0]*.01f,m[2]*.01f};groundHeight=progress.elevation=m[1]*.01f;
            partner={p[0]*.01f,p[2]*.01f};partnerHeight=p[1]*.01f;
        }
        while(professorCameraCursor<professorStory.cameras.size()) {
            const auto& c=professorStory.cameras[professorCameraCursor++];
            if(c.off)releaseCameraView(c.duration,uint8_t(c.curve));
            else setCameraView({c.eye[0],c.eye[1],c.eye[2]},{c.at[0],c.at[1],c.at[2]},c.duration,uint8_t(c.curve));
        }
        // Original setup4 with pending BattleStart provider0x8004ef54 admits
        // the trio introduction; setup2 below admits harbor tutorial instead.
        if(professorStory.underground && professorStory.battleSetup==4 && professorStory.pendingService==0x8004ef54) {
            if(trioBattleIntro.beginTrioBattleIntro(ticks*1000/60)) {
                trioBattleActive=true;setCameraView({0,100,450},{0,50,0},0,11);scene=Scene::Dialogue;saveRequested=false;return;
            }
        }
        progress.originalStory=professorStory.story;
        progress.originalFlag1207=professorStory.professorDeparted;
        if(professorStory.finished || !professorStory.locked())scene=Scene::Explore;
        else scene=Scene::Dialogue;
        saveRequested=false;
        if(!passive || professorStory.locked())return;
    }
    if(harborTutorial.running()) {
        const auto physical=inputProducer.channels[0].battleSample();
        harborTutorial.tick(ticks*1000/60,(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u),rawInput.physicalSnapshot?&physical:nullptr);
        scene=Scene::Battle;saveRequested=false;return;
    }
    if(progress.room==1 && progress.originalStory==4 && !harborEncounter.triggered)beginHarborEncounter(harborResume);
    if(harborEncounter.running()) {
        const bool passive=!harborEncounter.locked();
        // Key-off suppresses new actions while the ordinary airborne owner keeps
        // running. Scripted XZ travel and the explicit jump own separate lanes.
        auto& mario=harborEncounter.actors[0];
        if(!passive && fieldJump.airborne && !harborEncounter.scriptedJump) {
            dispatchFieldAir();move(Input{});updateMarioPresentation();
            mario.position[1]=(groundHeight+jumpHeight)*100;
            if(!mario.moving){mario.position[0]=progress.position.x*100;mario.position[2]=progress.position.z*100;}
            if(!mario.moving && !mario.poseOverride && !(mario.flags&2)) {
                if(mario.pose!=marioPresentation.front){mario.pose=marioPresentation.front;mario.poseAt=ticks*1000/60;}
            }
        }
        if(passive) {
            mario.position={progress.position.x*100,(groundHeight+jumpHeight)*100,progress.position.z*100};
            mario.yaw=marioPresentation.facing;
            mario.pose=marioPresentation.front;mario.poseAt=marioPresentation.poseStartMilliseconds;
        }
        float support=0;const auto& root=harborEncounter.actors[0].position;
        std::optional<float> floor;
        if(groundAt({root[0]*.01f,root[2]*.01f},root[1]*.01f,support))floor=std::round(support*100000.f)/1000.f;
        harborEncounter.tick(ticks*1000/60,(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u),floor);
        if(!passive || harborEncounter.locked()) {
            const auto& m=harborEncounter.actors[0].position;progress.position={m[0]*.01f,m[2]*.01f};
            if(harborEncounter.scriptedJump || !fieldJump.airborne) {
                groundHeight=progress.elevation=m[1]*.01f;jumpHeight=0;
            }
        }
        if(harborEncounter.locked() && !harborEncounter.scriptedJump && !mario.poseOverride && !(mario.flags&2)) {
            const char* motionPose=fieldJump.airborne?marioPresentation.front:
                mario.moving?(mario.speed>60?"M_R_1":"M_W_1"):nullptr;
            if(motionPose && mario.pose!=motionPose){mario.pose=motionPose;mario.poseAt=ticks*1000/60;}
        }
        for(unsigned n=0;n<4;++n){const auto& source=harborEncounter.actors[n+1];auto& actor=harborStory.actors[n];
            actor.x=source.position[0];actor.y=source.position[1];actor.z=source.position[2];actor.moving=source.moving;actor.display=source.yaw>0 && source.yaw<180?180:0;actor.yaw=source.yaw;actor.flags=source.flags|3;}
        while(harborCameraCursor<harborEncounter.cameras.size()){const auto& c=harborEncounter.cameras[harborCameraCursor++];setCameraView({c.eye[0],c.eye[1],c.eye[2]},{c.at[0],c.at[1],c.at[2]},c.duration,uint8_t(c.curve));}
        progress.hp=harborEncounter.hp;progress.originalStory=harborEncounter.story;saveRequested=false;
        scene=harborEncounter.locked()?Scene::Dialogue:Scene::Explore;
        // The original field event retains its blocking battle-start call.
        // Only source setup2 initialization/phase owns this bounded battle entry.
        if(harborEncounter.battleRequested && harborEncounter.battleSetup==2 && !harborTutorial.triggered && (progress.originalPouch?harborTutorial.beginFromPouch(ticks*1000/60,*progress.originalPouch,progress.originalStory.value_or(0)):harborTutorial.beginFresh(ticks*1000/60,progress.hp,10,progress.originalStory.value_or(0))))scene=Scene::Battle;
        if(harborEncounter.locked())return;
    }
    if(harborStory.locked()) {
        // Source key-off blocks new player actions, not field physics. Keep
        // support/jump/fall owners advancing under neutral native input.
        if(fieldJump.airborne)dispatchFieldAir();
        move(Input{});
        const unsigned before=harborStory.stage;
        const uint32_t keys=(i.confirm || tap?0x100u:0u)|(i.cancel?0x200u:0u);
        harborStory.tick(ticks*1000/60,keys);
        if(harborStory.stage!=before) {
            const auto eye=CameraPoint{fieldCameraFrame.eye.x*100,fieldCameraFrame.eye.y*100,fieldCameraFrame.eye.z*100};
            const auto at=CameraPoint{fieldCameraFrame.at.x*100,fieldCameraFrame.at.y*100,fieldCameraFrame.at.z*100};
            const auto now=fieldCameraTickMilliseconds(fieldCameraSimulationTicks);
            // Source-scene stage IDs2/9 and5 install these ENGINE-unit views
            // with500ms/curve11; they are script stage IDs, not saved story words.
            if(harborStory.stage==2 || harborStory.stage==9)fieldCameraEvents.setView({100,127,45},{100,75,-250},eye,at,500,11,now);
            else if(harborStory.stage==5)fieldCameraEvents.setView({120,105,-53},{120,70,-250},eye,at,500,11,now);
            else if(harborStory.finished)fieldCameraEvents.viewOff(eye,at,500,11,now);
        }
        saveRequested=false;
        if(harborStory.finished){progress.originalStory=4;scene=Scene::Explore;saveRequested=true;beginHarborEncounter(false);}
        return;
    }
    if (scene==Scene::Dialogue) {
        if (i.cancel || (tap && i.touchY>=168)) scene=Scene::Explore;
        else if (i.confirm || tap) {
            ++dialoguePage;
            if (dialoguePage>=dialoguePages()) {
                if (dialogueKind==1) startBattle(); else scene=Scene::Explore;
            }
            sound=Sound::Confirm;
        }
        return;
    }
    if (scene==Scene::Battle) { battle(i); return; }
    if (scene==Scene::Victory && (i.confirm || tap)) {
        if(!returnFromBattle(1))return;
        progress.crumpDefeated=true; scene=Scene::Explore; saveRequested=true; menu=Menu::Status;
        return;
    }
    // Legacy harness retry restores HP10; original story/tutorial defeat
    // ownership is separate. Bottom-screen retry band is y[107,155).
    if (scene==Scene::Defeat) {
        if (i.confirm || (tap && i.touchY>=107 && i.touchY<155)) {
            if(!returnFromBattle(-1))return; // Release this development battle's control hold before retrying.
            progress.hp=10;startBattle();
        }
        else if (i.cancel || tap) {
            if(!returnFromBattle(-1))return;
            progress.hp=10; scene=Scene::Explore;
        }
        return;
    }
    if (scene!=Scene::Explore) return;
    if(!fieldControlEnabled())return;
    int tab=menuButton(i);
    if (tab>=0 && tab<4) { menu=static_cast<Menu>(tab); sound=Sound::Confirm; }
    if (i.left) menu=static_cast<Menu>((static_cast<int>(menu)+3)%4);
    if (i.right) menu=static_cast<Menu>((static_cast<int>(menu)+1)%4);
    // Native field menu action band y[177,216); four tabs wrap modulo4.
    // Mushroom heal5/max10 and exit/save actions belong to the legacy harness.
    if ((i.touchY>=177 && i.touchY<216 && tap) || i.menuConfirm) {
        if (menu==Menu::Settings) { progress.sound=!progress.sound; saveRequested=true; }
        else if (menu==Menu::Items && progress.mushrooms>0 && progress.hp<10) {
            --progress.mushrooms; progress.hp=std::min(10,progress.hp+5); saveRequested=true;
        } else if (menu==Menu::Status) saveRequested=true;
        sound=Sound::Confirm;
    }
    // Enter air motion before launch-call horizontal movement. begin represents
    // that call's phase0/counter1, so it must not consume another vertical tick.
    // Plain idle/walk/dash dispatcher repeats finish input classification before
    // the surviving jump entry. These prompt checks still belong to the harness.
    // Upstairs entry and its three rises precede this invocation's movement.
    // Completion immediately dispatches current ground input/A; earlier edges
    // remain unbuffered. Retain the vertical tuple independently of countdown.
    if(fieldJump.airborne)dispatchFieldAir();
    if(fieldStep.active()) {
        fieldStep.tick();fieldVerticalVelocity=fieldStep.velocity;
        groundHeight=progress.elevation=fieldStep.height;jumpHeight=0;
    }
    if(tryTroubleBoardContact((i.confirm?0x100u:0u)|(i.cancel?0x200u:0u)))return;
    if(i.confirm && !i.cancel && !airborne()) {
        for(size_t n=0;n<fieldDoors().size();++n) {
            const auto& d=fieldDoors()[n];if(d.room!=progress.room)continue;
            const bool opening=!(fieldDoor.interiorMask&d.mask);
            if(!fieldDoorAdmitted(wallContact.sourceNode,opening?d.openNode:d.closeNode,0x100,fieldJump.motion,false,fieldNpcTalkCandidateAlive(),roadAdmission))continue;
            if(!d.unlocked(progress.originalStory)) {
                if(d.mask==4 && progress.originalStory && *progress.originalStory<23) {
                    FieldActorRecord speaker;speaker.room=2;speaker.kind=1;speaker.talkPrefix="gor_02";
                    // Original print operand0 is an unattributed speaker.
                    if(fieldActorTalk.canPrint(speaker,"gor_02_035",harborMessageFont(),ticks*1000/60)) {
                        applyFieldItemStateRequest(ItemGetRequest::KeyOff);
                        if(fieldActorTalk.print(speaker,"gor_02_035",harborMessageFont(),ticks*1000/60)) {
                            fieldActorTalk.message.placementOverride=0; // toge selector1 -> common mode0, no anchor copy.
                            troubleDoorNotice=true;scene=Scene::Dialogue;saveRequested=false;
                        } else applyFieldItemStateRequest(ItemGetRequest::KeyOn); // Native resource admission rollback.
                    }
                }
                return; // Unknown state/resources do not authorize entry or replay A as jump.
            }
            // Professor first-talk/wall-removal and inn upstairs callback
            // ownership remain separate source scenes; no story write occurs.
            fieldDoor.begin(int(n),d,{progress.position.x,progress.position.z});
            if(opening && progress.room==2 && d.mask==2 && progress.originalStory==8 && !professorStory.triggered) {
                if(professorStory.begin(ticks*1000/60,{progress.position.x*100,groundHeight*100,progress.position.z*100},
                                       {partner.x*100,partnerHeight*100,partner.z*100},progress.originalFlag1207))scene=Scene::Dialogue;
            }
            if(!opening && progress.room==2 && d.mask==2 && progress.originalStory==9 && professorStory.temporaryProfessor) {
                professorCameraCursor=0;
                if(professorStory.beginFence(ticks*1000/60,{progress.position.x*100,groundHeight*100,progress.position.z*100},
                    {partner.x*100,partnerHeight*100,partner.z*100}))scene=Scene::Dialogue;
            }
            applyDoorCollision(progress.room,fieldDoor.interiorMask);
            const auto& target=opening?d.inside:d.outside;
            setCameraTarget({target[0]*100,(groundHeight*100)+2,target[2]*100},100,150,11);
            saveRequested=false;return;
        }
    }
    // Prior invocation's talk candidate suppresses A-jump BEFORE this visit
    // rescans NPCs at scope exit. Removing that retention replays a talk edge.
    const bool retainedNpcTalk=fieldNpcTalkCandidateAlive();
    const bool launched=i.confirm && !i.cancel && !airborne() && !retainedNpcTalk &&
        !(progress.room==0 && ((!progress.crumpDefeated && near(CrumpPosition)) || near(NpcPosition)));
    if(launched) {
        if(i.physicalSnapshot) {
            const auto& axes=inputProducer.channels[0].analog;
            fieldMovement.groundedStick(axes.mainX,int8_t(-axes.mainY));
        } else fieldMovement.grounded(i.x,i.z);
        // Same-call ground arbitration established this motion before the
        // surviving jump request; +30 must snapshot that final ground owner.
        fieldJump.enterGround(fieldMovement.speed==0?0:fieldMovement.speed==DashSpeed?2:1,groundHeight);
        previousGroundContact=groundContact;
        jumpBase=groundHeight;fieldJump.begin(groundHeight,fieldMovement.speed);saveRequested=false;sound=Sound::Jump;
    }
    move(i,launched);
    if(warpRequest>=0 || water.active() || fieldStep.active())return;
    const int8_t pipeY=i.physicalSnapshot?inputProducer.channels[0].analog.mainY:int8_t(std::clamp(-i.z,-1.f,1.f)*70);
    if(tryFieldPipe(pipeY))return;
    if((groundContact.attributes&0x200) && !fieldJump.airborne && beginWater())return;
    // Source harbor trigger compares event integer Z after1/1024 engine-unit
    // quantization and truncation to integer against -100. Keep both truncations.
    if(harborStory.enabled && !harborStory.triggered &&
       std::trunc(std::trunc(progress.position.z*100.f*1024.f)/1024.f)<=-100 && harborStory.begin(ticks*1000/60)) {
        scene=Scene::Dialogue;saveRequested=false;return;
    }
    if (i.cancel) openDialogue(2);
    else if (i.confirm && !fieldJump.airborne) {
        if (progress.room==0 && !progress.crumpDefeated && near(CrumpPosition)) openDialogue(1);
        else if (progress.room==0 && near(NpcPosition)) openDialogue(0);
        // A trigger sampled while airborne is not replayed after landing physics.
        // Jump requests were admitted before move; original talk arbitration is
        // still a separate event/interaction dependency.
    }
}
}
