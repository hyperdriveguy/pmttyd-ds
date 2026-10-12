#include "game.hpp"
#include "wall_geometry.hpp"
#include "field_camera.hpp"
#include "head_collision.hpp"
#include "floor_contact.hpp"
#include "field_ray.hpp"
#include "battle_unit_resources.hpp"
#include "battle_action_services.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
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
std::vector<RoadWarp> roads;
std::vector<Rect> walls;
bool collisionReady=false;
std::vector<Vec2> wallLevels;
std::vector<GroundContact> wallContacts;
std::vector<WallTriangle> wallTriangles;
struct GroundTriangle { float v[9];GroundContact contact;uint32_t cull=3; };
static_assert(sizeof(GroundContact)==8,"Ground metadata must contain two uint32 words");
static_assert(sizeof(ReturnPoint)==12,"Return position must contain three float32 values");
std::vector<GroundTriangle> ground;
struct HeadTriangle {float v[9];GroundContact contact;uint32_t cull;};
static_assert(sizeof(HeadTriangle)==48,"Head pack record size");
std::vector<HeadTriangle> head;
std::unordered_map<uint32_t,ReturnPoint> groundReturns;
using SpatialIndex=std::unordered_map<uint64_t,std::vector<uint32_t>>;
SpatialIndex wallIndex,groundIndex,headIndex;
Rect roomBounds{-9,-4.5f,9,4};
constexpr float GridSize=2.f;
uint64_t cellKey(int x,int z) {return (uint64_t(uint32_t(x))<<32)|uint32_t(z);}
const std::vector<uint32_t>& nearby(const SpatialIndex& index,Vec2 p) {
    static const std::vector<uint32_t> empty;
    auto cell=index.find(cellKey(int(std::floor(p.x/GridSize)),int(std::floor(p.z/GridSize))));
    return cell==index.end()?empty:cell->second;
}
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
int menuButton(const Input& i) {
    if (i.touchY >= 44 && i.touchY < 82 && i.touchX >= 8 && i.touchX < 312)
        return (i.touchX-8)/76;
    return -1;
}
}
struct CollisionSnapshot {
    std::vector<Rect> savedWalls;
    std::vector<Vec2> savedLevels;
    std::vector<GroundContact> savedWallContacts;
    std::vector<WallTriangle> savedWallTriangles;
    std::vector<GroundTriangle> savedGround;
    std::vector<HeadTriangle> savedHead;
    std::unordered_map<uint32_t,ReturnPoint> savedReturns;
    SpatialIndex savedWallIndex,savedGroundIndex,savedHeadIndex;
    Rect savedBounds{-9,-4.5f,9,4};
    bool ready=false;
    void swapActive() {
        savedWalls.swap(walls);savedLevels.swap(wallLevels);savedGround.swap(ground);
        savedWallContacts.swap(wallContacts);
        savedWallTriangles.swap(wallTriangles);
        savedHead.swap(head);savedHeadIndex.swap(headIndex);
        savedReturns.swap(groundReturns);
        savedWallIndex.swap(wallIndex);savedGroundIndex.swap(groundIndex);
        std::swap(savedBounds,roomBounds);std::swap(ready,collisionReady);
    }
};
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
const char* roomAsset(uint32_t room) {
    static const char* names[]={"map","gor_00","gor_02","gor_03","gor_04"};
    return room<5?names[room]:nullptr;
}
const char* roomName(uint32_t room) {
    static const char* names[]={"Rogueport plaza","Rogueport harbor","Rogueport east side","Rogueport west side","Rogueport station"};
    return room<5?names[room]:"Unknown area";
}
bool wallAtLevel(size_t n,float elevation) {
    if(n<wallContacts.size()) {
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
bool loadWarps(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    uint32_t count=0;bool ok=std::fread(&count,4,1,file)==1 && count<=128;
    std::vector<RoadWarp> data(ok?count:0);
    if(ok)ok=std::fread(data.data(),sizeof(RoadWarp),count,file)==count && std::fgetc(file)==EOF;
    std::fclose(file);
    for(const auto& w:data) if(!roomAsset(w.source) || !roomAsset(w.target) ||
        !std::isfinite(w.x) || !std::isfinite(w.z) || !std::isfinite(w.spawnX) || !std::isfinite(w.spawnZ) || !std::isfinite(w.spawnY))ok=false;
    if(ok)roads.swap(data);
    return ok;
}
uint32_t Game::warpTarget() const {return warpRequest>=0 && size_t(warpRequest)<roads.size()?roads[warpRequest].target:UINT32_MAX;}
bool Game::finishWarp() {
    if(warpRequest<0 || size_t(warpRequest)>=roads.size())return false;
    const auto& w=roads[warpRequest];float height=w.spawnY;
    const auto reverse=std::find_if(roads.begin(),roads.end(),[&](const auto& r){return r.source==w.target && r.target==w.source;});
    if(reverse==roads.end())return false;
    const Vec2 start{2*reverse->x-w.spawnX,2*reverse->z-w.spawnZ};
    Vec2 arrival{w.spawnX,w.spawnZ};
    RoadMotion staged;
    if(!staged.begin({start.x,start.z},{arrival.x,arrival.z}))return false;
    if(!groundAt(arrival,height,height) || !walkable(arrival,height))return false;
    Progress next=progress;next.room=w.target;next.position=arrival;next.elevation=height;
    restore(next);groundHeight=height;partner=arrival;partnerHeight=height;partnerContact=groundContact;
    arrivalStart=start;arrivalEnd=arrival;
    followerPath.clear();warpCooldown=0;saveRequested=true;return true;
}
void Game::beginRoadArrival() {
    roadMotion.begin({arrivalStart.x,arrivalStart.z},{arrivalEnd.x,arrivalEnd.z});
    if(std::fabs(roadMotion.step[0])>.001f)facing=roadMotion.step[0]>0?1:-1;
    turnScale=1;turnTick=0;
    progress.position=arrivalStart;partner=arrivalStart;arriving=true;saveRequested=false;
}
void Game::cancelRoad() {
    if(departureOriginValid) {
        progress.position=departureOrigin;groundHeight=progress.elevation=departureHeight;
        cameraX=departureOrigin.x;cameraZ=departureOrigin.z;cameraHeight=departureHeight;
    }
    warpRequest=-1;warpCooldown=60;departureDone=false;departurePause=0;roadMotion={};arriving=false;saveRequested=false;
    fade.startIn();
}
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
bool loadHead(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    char magic[4];uint32_t count=0;
    bool ok=std::fread(magic,4,1,file)==1 && std::string(magic,4)=="HEAD" && std::fread(&count,4,1,file)==1 && count<=100000;
    std::vector<HeadTriangle> data(ok?count:0);
    for(auto& tri:data) {
        ok=ok && std::fread(&tri,48,1,file)==1;
        for(float v:tri.v)if(!std::isfinite(v))ok=false;
        if(tri.cull>3)ok=false;
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
FieldWallHit fieldWallRay(FieldWallVector origin,FieldWallVector direction,float maximum) {
    // Geometry cache is in native scale. Policy inputs/outputs are engine units.
    origin={origin.x*.01f,origin.y*.01f,origin.z*.01f};maximum*=.01f;
    FieldWallHit nearest;nearest.distance=maximum;
    auto test=[&](std::array<float,9> triangle,uint32_t cull,GroundContact contact,uintptr_t token) {
        if(!ordinaryWallAttributes(contact.attributes))return;
        // Current static room meshes contain no named high-bit NPC owners.
        // Such an admitted record needs the original owner-name filter service.
        assert(!(contact.attributes&0x80000000) || (contact.attributes&0x00080080));
        FieldWallHit hit;
        if(intersectFieldRay(triangle,cull,origin,direction,maximum,hit) &&
           (!nearest.token || hit.distance<nearest.distance)) {
            hit.token=token;hit.attributes=contact.attributes;nearest=hit;
        }
    };
    if(!head.empty()) {
        const float endX=origin.x+direction.x*maximum,endZ=origin.z+direction.z*maximum;
        const int left=std::floor(std::min(origin.x,endX)/GridSize),right=std::floor(std::max(origin.x,endX)/GridSize);
        const int back=std::floor(std::min(origin.z,endZ)/GridSize),front=std::floor(std::max(origin.z,endZ)/GridSize);
        auto testIndex=[&](uint32_t n) {
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
    nearest.distance*=100.f;
    return nearest;
}
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot) {
    return headCollision(p,direction,worldY,velocity,clampedRoot,velocity);
}
bool headCollision(Vec2 p,Vec2 direction,float worldY,float velocity,float& clampedRoot,float rayExtension) {
    const auto result=searchFieldHead(p,direction,worldY,velocity,rayExtension);
    if(result.contact)clampedRoot=result.correction;
    return result.contact;
}
FieldHeadQuery searchFieldHead(Vec2 p,Vec2 direction,float worldY,float velocity,float rayExtension,
                              FieldSlope* slopes,bool narrowCone) {
    FieldHeadQuery result;
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || std::fabs(p.x)>4096 || std::fabs(p.z)>4096 ||
       !std::isfinite(worldY) || !std::isfinite(velocity) || !std::isfinite(rayExtension) ||
       !std::isfinite(length) || length<1e-6f)return result;
    const float x=direction.x*HeadProbeRadius/length,z=direction.z*HeadProbeRadius/length;
    const Vec2 offsets[]={{0,0},{x,z},{-z,x},{z,-x},{-x,-z}};
    const float low=worldY+HeadBodyHeight*.5f,high=low+HeadBodyHeight+rayExtension;
    for(auto offset:offsets) {
        const Vec2 ray{p.x+offset.x,p.z+offset.z};float nearest=high;const HeadTriangle* selected=nullptr;
        for(uint32_t n:nearby(headIndex,ray)) {
            const auto& tri=head[n];if(!headAttributesSolid(tri.contact.attributes))continue;
            const float* v=tri.v;
            // Source normal=(v2-v0) cross (v0-v1), equivalent to standard winding.
            const float normalY=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);
            if(tri.cull==1 && normalY>=0)continue;
            float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
            if(std::fabs(den)<1e-8f)continue;
            float a=((v[5]-v[8])*(ray.x-v[6])+(v[6]-v[3])*(ray.z-v[8]))/den;
            float b=((v[8]-v[2])*(ray.x-v[6])+(v[0]-v[6])*(ray.z-v[8]))/den;
            if(a<0 || b<0 || a+b>1)continue;
            float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
            if(y>=low && y<=high && (!selected || y<nearest)){nearest=y;selected=&tri;}
        }
        if(!selected)continue;
        const auto v=selected->v;
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
        if(result.contact){result.correction=headClampedRoot(contact);result.node=selected->contact;}
        // First admitted ray returns even for a near miss. Later feet cannot
        // replace it with a closer contact; rejected inclinations can continue.
        return result;
    }
    return result;
}
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
static bool groundWithin(Vec2 p,float low,float high,float& height,GroundContact* contact,float* slope=nullptr,
                         FieldSlope* record=nullptr,bool rejectSteep=true) {
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(low) || !std::isfinite(high) || low>high ||
       std::fabs(p.x)>4096 || std::fabs(p.z)>4096)return false;
    if(ground.empty()) {if(low>0 || high<0)return false;height=0;if(contact)*contact={};if(slope)*slope=0;return true;}
    bool found=false;float best=-1e30f;GroundContact bestContact;const float* bestTriangle=nullptr;
    for(uint32_t n:nearby(groundIndex,p)) {
        // Original normal-Mario chkfilterVec rejects bit4 unless its special
        // inclusion bits apply. Name-dependent bit31 cases are not present
        // in the five currently packaged rooms; transform filters are deferred.
        const uint32_t flags=ground[n].contact.attributes;
        if(!(flags&0x00080080) && !(flags&0x00800000) && (flags&4))continue;
        const float* v=ground[n].v;
        const float normalY=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);
        if(ground[n].cull==1 && normalY<=0)continue; // Downward ray backface.
        if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) ||
           p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
        float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
        if(std::fabs(den)<1e-8f)continue;
        float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
        float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
        if(a<-.0001f || b<-.0001f || a+b>1.0001f)continue;
        float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
        if(y<=high && y>=low && y>best) {best=y;bestContact=ground[n].contact;bestTriangle=v;found=true;}
    }
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
        if(rejectSteep && angle>=50) {
            // Projected tilts were written before the cone gate; +218 wasn't.
            if(record)record->inclination=previousInclination;
            return false;
        }
        if(slope)*slope=angle;
        if(groundReturns.count(bestContact.sourceNode))bestContact.attributes|=0x40000000;
        height=best;if(contact)*contact=bestContact;
    }
    return found;
}
bool groundAt(Vec2 p,float reference,float& height,GroundContact* contact,float* slope) {
    // Preserve development/no-pack flat-ground behavior for legacy callers.
    if(ground.empty() && std::isfinite(reference))return groundWithin(p,-.5f,StepHeight,height,contact,slope);
    return groundWithin(p,reference-.5f,reference+StepHeight,height,contact,slope);
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
static bool queryFrontStep(Vec2 p,Vec2 direction,float rootY,float& height,float& slope,FieldSlope* record) {
    if(record)*record={};
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(length) || length<1e-6f)return false;
    // Original front heading uses fixed +.5 whole-degree truncation/revise360.
    float heading=std::atan2(direction.x,-direction.z)*57.295779513f;
    if(heading<0)heading+=360;
    heading=std::trunc(heading+.5f)*3.14159265358979323846f/180.f;
    const Vec2 forward{p.x+std::sin(heading)*FrontStepDistance,p.z-std::cos(heading)*FrontStepDistance};
    // marioChkFrontStep: origin root+18.5, downward length18.5+11+.1 engine units.
    return groundWithin(forward,rootY-.111f,rootY+.185f,height,nullptr,&slope,record,false);
}
bool frontStepGround(Vec2 p,Vec2 direction,float rootY,float& height,float& slope) {
    FieldSlope record;return queryFrontStep(p,direction,rootY,height,slope,&record);
}
bool landingFootprint(Vec2 p,Vec2 direction,float worldY,float velocity,float& height,GroundContact* contact) {
    FieldSlope slope;const auto result=searchFieldSupport(p,direction,worldY,velocity,slope,velocity);
    if(result.found){height=result.height;if(contact)*contact=result.contact;}return result.found;
}
FieldSupport searchFieldSupport(Vec2 p,Vec2 direction,float rootY,float velocity,FieldSlope& slope,float depth) {
    slope={};FieldSupport result;result.height=rootY;
    const float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(length) || length<1e-6f || !std::isfinite(velocity) || !std::isfinite(depth))return result;
    const float x=direction.x*FloorFootRadius/length,z=direction.z*FloorFootRadius/length;
    const Vec2 offsets[]={{0,0},{x,z},{-z,x},{z,-x},{-x,-z}};
    float best=-100.f; // Original -10000-unit sentinel.
    for(auto offset:offsets) {
        float floor=0;GroundContact candidate;
        if(!groundWithin({p.x+offset.x,p.z+offset.z},rootY-std::fabs(depth)-.001f,rootY+.05f+std::fabs(velocity),
                         floor,&candidate,nullptr,&slope))continue;
        const float below=rootY-floor;
        if(below>=GroundedNearDrop && below>std::fabs(velocity)) {result.delta=below;continue;}
        if(!floorCandidateReachable(rootY,velocity,floor) || floor<=best)continue;
        best=floor;result.found=true;result.height=floor;result.delta=0;result.contact=candidate;
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
FieldHeadQuery Game::queryOverhead(float rootY,float velocity,float extension) {
    const auto result=searchFieldHead(progress.position,moveDirection,rootY,velocity,extension,
                                      &fieldMovement.slope,fieldHeadNarrowCone);
    headContact=result.node;headClearance=result.clearance;
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
        fieldMovement.dampFallBase(fieldAirOnScreen,!marioFieldControl && fieldKeysOff,fieldAirDampingBypass);
        if(!fieldAirOnScreen && !fieldAirDampingBypass)fieldJump.suppressDrift=true;
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
        const Vec2 direction=fieldMovement.speed!=0?Vec2{fieldMovement.headingX,fieldMovement.headingZ}:Vec2{facing,0};
        const float offset=fieldMovement.speed!=0?fieldMovement.headingOffsetDegrees*3.14159265358979323846f/180.f:0;
        const Vec2 foot{direction.x*std::cos(offset)+direction.z*std::sin(offset),
                       direction.z*std::cos(offset)-direction.x*std::sin(offset)};
        const auto support=searchFieldSupport(progress.position,foot,worldY,fieldJump.velocity,fieldMovement.slope,fieldJump.velocity);
        groundContact=support.found?support.contact:GroundContact{};
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
bool Game::beginVoid() {
    ReturnPoint destination=roomRespawn,candidate;
    float floor=destination.y;
    if(recoveryReturn(candidate) && groundAt({candidate.x,candidate.z},candidate.y,floor) && walkable({candidate.x,candidate.z},floor))destination={candidate.x,floor,candidate.z};
    floor=destination.y;
    if(!groundAt({destination.x,destination.z},floor,floor) || !walkable({destination.x,destination.z},floor))return false;
    destination.y=floor;
    water.beginVoid({roomRespawn.x,recoveryTopHeight(floor),roomRespawn.z},{destination.x,floor,destination.z});
    progress.position={roomRespawn.x,roomRespawn.z};groundHeight=progress.elevation=floor;
    partner={roomRespawn.x,roomRespawn.z};partnerHeight=roomRespawn.y;partnerContact={};
    cameraX=destination.x;cameraZ=destination.z;cameraHeight=floor;
    fieldJump={};fieldStep={};fieldVerticalVelocity=0;jumpHeight=0;warpRequest=-1;saveRequested=false;
    return true;
}
void clearCollision() {walls.clear();wallLevels.clear();wallContacts.clear();wallTriangles.clear();ground.clear();head.clear();headIndex.clear();groundReturns.clear();wallIndex.clear();groundIndex.clear();collisionReady=false;roomBounds={-9,-4.5f,9,4};}
const std::vector<Rect>& collisionWalls() {return walls;}
bool Game::near(Vec2 p, float d) const { return length(progress.position,p) < d; }
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
    water.begin({progress.position.x,groundHeight,progress.position.z},{target.x,target.y,target.z});
    jumpHeight=0;fieldJump={};fieldStep={};fieldVerticalVelocity=0;warpRequest=-1;saveRequested=false;return true;
}
void Game::restore(const Progress& saved) {
    const Progress p=saved; // Caller may pass this game's own progress.
    *this = Game{};
    progress = p;
    partner = {p.position.x-0.8f,p.position.z};
    groundHeight=p.elevation;partnerHeight=p.elevation;
    if (!walkable(partner,p.elevation)) partner = p.position;
    groundAt(p.position,p.elevation,groundHeight,&groundContact);
    groundMetadataHeight=supportMetadataHeight(groundHeight);
    roomRespawn={p.position.x,groundHeight,p.position.z};
    groundAt(partner,groundHeight,partnerHeight,&partnerContact);
    followerPath.push_back(partner);followerPath.push_back(p.position);
    cameraX=p.position.x;cameraZ=p.position.z;cameraHeight=groundHeight;
}
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
    return "Circle Pad: move   A: jump   X: partner";
}
void Game::move(const Input& i,bool launched) {
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
    // Ordinary head/landing/ground probes use retained travel+1a4 when base
    // speed+180 is nonzero, otherwise the facing conversion (native flip).
    // Current-stick drift and combined travel do not rotate those probes.
    moveDirection=fieldMovement.speed!=0?Vec2{fieldMovement.headingX,fieldMovement.headingZ}:Vec2{facing,0};
    // Ordinary ascent: jump, overhead, move. Falling keeps move before gravity.
    const bool wasFalling=fieldJump.airborne && fieldJump.falling;
    float previousY=0;const bool verticalReady=!wasFalling && integrateVertical(i.jumpHeld,previousY,!launched);
    if(water.active())return;
    // Ascent overhead can clear air flags before marioMove. Recompute the cap
    // from the flags after that owner, rather than before the vertical pass.
    const auto combined=fieldJump.airborne?
        fieldMovement.airborne(x,z,fieldJump.suppressDrift || fieldJump.ticks==0,fieldJump.hasAirFlags()):
        stepping?fieldMovement.retained():fieldMovement.reduceSlope(grounded);
    // Direction gain follows air cap and keeps retained travel heading.
    const auto travel=fieldMovement.applyMovementRate(combined);
    if(len>0){x/=len;z/=len;}
    Vec2 p = progress.position;
    Vec2 candidate{p.x+travel.x,p.z+travel.z};
    const bool sourceWalls=collisionReady;
    if(sourceWalls) {
        FieldWallActor actor;actor.viewFlags=fieldViewFlags;
        actor.motion=fieldJump.airborne?fieldJump.motion:stepping?12:
            fieldMovement.speed==0?0:fieldMovement.speed==DashSpeed?2:1;
        actor.counter=fieldJump.ticks;
        FieldWallServices services;services.ray=[](void*,FieldWallVector from,FieldWallVector direction,float distance) {
            return fieldWallRay(from,direction,distance);
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
            front=searchFieldFrontWall(actual,{moveDirection.x*10,0,moveDirection.z*10},actor,services);
            if(!front.forward.token)front=searchFieldFrontWall(actual,{facing*10,0,0},actor,services);
            candidate=p;
        }
        if(front.forward.token) {
            const size_t n=front.forward.token-1;
            wallContact=!head.empty()?head[n].contact:n<wallContacts.size()?wallContacts[n]:GroundContact{};
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
        const auto support=searchFieldSupport(p,footDirection,groundHeight,fieldVerticalVelocity,fieldMovement.slope);
        groundContact=support.found?support.contact:GroundContact{};
        if(support.found)groundMetadataHeight=supportMetadataHeight(support.height);
        float height=groundHeight,slope=0;
        const bool front=fieldJump.ticks==0 && queryFrontStep(p,moveDirection,groundHeight,height,slope,&fieldMovement.slope);
        if(front && stairRiseAllowed(groundHeight,height,slope) && fieldMovement.speed!=0) {
            fieldStep.begin(groundHeight,height);saveRequested=false;
        } else {
            groundHeight=progress.elevation=support.height;
            if(!support.found && support.delta!=0) {
                jumpBase=groundHeight;fieldJump.beginFall(support.delta>0);
            }
        }
    }
    if(warpCooldown>0)--warpCooldown;
    if(warpCooldown==0 && warpRequest<0 && len>.15f && !fieldStep.active()) {
        for(size_t n=0;n<roads.size();++n) {
            const auto& road=roads[n];
            if(road.source==progress.room && length(p,{road.x,road.z})<.7f &&
                x*(road.x-p.x)+z*(road.z-p.z)>0) {warpRequest=int(n);departureOrigin=p;departureHeight=groundHeight;departureOriginValid=true;saveRequested=false;break;}
        }
    }
    if (std::fabs(x) > .15f && (x>0 ? 1.f : -1.f) != facing) {
        facing = x>0 ? 1.f : -1.f; turnTick=10;
    }
    if (turnTick>0) { --turnTick; turnScale=std::max(.08f,std::fabs(std::cos(turnTick*3.14159265f/10.f))); }
    else turnScale=1;
    if(followerPath.empty()) {followerPath.push_back(partner);followerPath.push_back(p);}
    if(length(followerPath.back(),p)>.08f) followerPath.push_back(p);
    float remaining=length(partner,followerPath.front());
    for(size_t n=1;n<followerPath.size();++n)remaining+=length(followerPath[n-1],followerPath[n]);
    if(remaining>PartnerDistance) {
        float budget=std::min(DashSpeed,remaining-PartnerDistance);
        while(budget>0 && !followerPath.empty()) {
            Vec2 target=followerPath.front();float dist=length(partner,target);
            if(dist<.001f) {followerPath.erase(followerPath.begin());continue;}
            float step=std::min(budget,dist);
            Vec2 q{partner.x+(target.x-partner.x)/dist*step,partner.z+(target.z-partner.z)/dist*step};
            float h=partnerHeight;GroundContact contact;
            if(!walkable(q,partnerHeight) || !groundAt(q,partnerHeight,h,&contact) || !walkable(q,h))break;
            partner=q;partnerHeight=h;partnerContact=contact;budget-=step;
            if(step>=dist)followerPath.erase(followerPath.begin());
        }
    }
    // Bound the trail if the partner cannot traverse an old segment.
    if(followerPath.size()>512) {partner=p;partnerHeight=groundHeight;followerPath.clear();}
    cameraX+=(p.x-cameraX)*.07f;
    cameraZ+=(p.z-cameraZ)*.07f;
    cameraHeight+=(groundHeight-cameraHeight)*.07f;
    if(stepping) {
        // Upstairs motion12 also records contact without the jump-only response.
        queryOverhead(groundHeight,fieldStep.velocity,.1f);
    }
    if(wasFalling)advanceVertical(i.jumpHeld,false);
    else if(verticalReady)finishVertical(previousY);
    if(airborne())saveRequested=false;
}
FieldBattleServices Game::fieldBattleServices() {
    FieldBattleServices s;s.user=this;
    s.work=[](void* p)->FieldBattleWork& {return static_cast<Game*>(p)->fieldBattleWork;};
    // Single NPC/partner development fixtures, not original registry IDs.
    s.npc=[](void*,int32_t)->uintptr_t {return 1;};
    s.prepareMario=[](void* p) {static_cast<Game*>(p)->fieldPrepareRequested=true;};
    s.marioControl=[](void* p,bool enabled) {static_cast<Game*>(p)->marioFieldControl=enabled;};
    s.partyId=[](void*)->int32_t {return 1;};
    s.party=[](void*,int32_t)->uintptr_t {return 1;};
    s.partyControl=[](void* p,uintptr_t,bool enabled) {static_cast<Game*>(p)->partnerFieldControl=enabled;};
    s.soundFlag=[](void* p,uint32_t mask) {static_cast<Game*>(p)->fieldSoundFlags|=mask;};
    s.setSequence=[](void* p,int32_t sequence) {static_cast<Game*>(p)->fieldSequence=sequence;};
    s.sequence=[](void* p)->int32_t {return static_cast<Game*>(p)->fieldSequence;};
    s.stopBattleBgm=[](void* p) {static_cast<Game*>(p)->fieldBgmStopRequested=true;};
    s.postMario=[](void* p) {static_cast<Game*>(p)->fieldPostRequested=true;};
    return s;
}
bool Game::returnFromBattle(int32_t harnessResult) {
    // Development outcome tokens only; original outcome producers are unrecovered.
    fieldBattleWork.result=harnessResult;fieldSequence=2;
    return waitFieldBattleEnd(fieldBattleServices())==2;
}
void Game::startBattle() {
    fieldPrepareRequested=fieldPostRequested=fieldBgmStopRequested=false;fieldSoundFlags=0;
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
void Game::tick(const Input& rawInput) {
    Input input=rawInput;const Input& i=input;
    ++ticks; sound=Sound::None;overheadContact=false;
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
    const bool tap=i.touchX>=0;
    if(arriving) {
        fade.tick();const bool finished=roadMotion.tick();
        progress.position={roadMotion.position[0],roadMotion.position[1]};partner=progress.position;
        if(finished){arriving=false;saveRequested=true;followerPath.clear();followerPath.push_back(partner);}
        return;
    }
    if(fade.active()){fade.tick();return;}
    if(warpRequest>=0) {
        if(size_t(warpRequest)>=roads.size()){cancelRoad();return;}
        // Original road event disables controls and waits for landing first.
        if(fieldJump.airborne){advanceVertical(i.jumpHeld);return;}
        if(!departureDone && !roadMotion.active()) {
            const auto& w=roads[warpRequest];
            const auto reverse=std::find_if(roads.begin(),roads.end(),[&](const auto& r){return r.source==w.target && r.target==w.source;});
            if(reverse==roads.end()){cancelRoad();return;}
            float x=w.x-reverse->spawnX,z=w.z-reverse->spawnZ,d=std::hypot(x,z);
            if(d<.001f){cancelRoad();return;}
            if(!roadMotion.begin({progress.position.x,progress.position.z},{w.x+x*1.5f/d,w.z+z*1.5f/d})){cancelRoad();return;}
        }
        if(!departureDone) {
            if(std::fabs(roadMotion.step[0])>.001f)facing=roadMotion.step[0]>0?1:-1;
            turnScale=1;turnTick=0;
            if(roadMotion.tick()){departureDone=true;departurePause=9;}
            progress.position={roadMotion.position[0],roadMotion.position[1]};
            cameraX+=(progress.position.x-cameraX)*.07f;cameraZ+=(progress.position.z-cameraZ)*.07f;
        } else if(departurePause>0 && --departurePause==0)fade.startOut();
        return;
    }
    if(scene==Scene::FieldDefeat) {if(i.confirm || (tap && i.touchY>=107 && i.touchY<155))reloadRequested=true;return;}
    if(water.active()) {
        const auto event=water.tick(progress.hp);
        progress.position={water.position[0],water.position[2]};
        if(event==WaterEvent::Landed || event==WaterEvent::Complete || event==WaterEvent::Dead) {
            groundHeight=water.position[1];progress.elevation=groundHeight;
            groundAt(progress.position,groundHeight,groundHeight,&groundContact);
        }
        if(event==WaterEvent::Complete) {
            if(water.kind==RecoveryKind::Void){partner=progress.position;partnerHeight=groundHeight;partnerContact=groundContact;}
            followerPath.clear();followerPath.push_back(partner);followerPath.push_back(progress.position);
        } else if(event==WaterEvent::Dead)scene=Scene::FieldDefeat;
        return; // Original camera and NPC updates stay frozen through recovery.
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
    if (scene==Scene::Defeat) {
        if (i.confirm || (tap && i.touchY>=107 && i.touchY<155)) { progress.hp=10; startBattle(); }
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
    const bool launched=i.confirm && !i.cancel && !airborne() &&
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
    if((groundContact.attributes&0x200) && !fieldJump.airborne && beginWater())return;
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
