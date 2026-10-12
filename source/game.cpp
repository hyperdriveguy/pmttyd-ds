#include "game.hpp"
#include "wall_geometry.hpp"
#include "field_camera.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
namespace ttyd {
namespace {
std::vector<RoadWarp> roads;
std::vector<Rect> walls;
bool collisionReady=false;
std::vector<Vec2> wallLevels;
std::vector<GroundContact> wallContacts;
std::vector<WallTriangle> wallTriangles;
struct GroundTriangle { float v[9];GroundContact contact; };
static_assert(sizeof(GroundContact)==8,"Ground metadata must contain two uint32 words");
static_assert(sizeof(ReturnPoint)==12,"Return position must contain three float32 values");
std::vector<GroundTriangle> ground;
std::unordered_map<uint32_t,ReturnPoint> groundReturns;
using SpatialIndex=std::unordered_map<uint64_t,std::vector<uint32_t>>;
SpatialIndex wallIndex,groundIndex;
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
    std::unordered_map<uint32_t,ReturnPoint> savedReturns;
    SpatialIndex savedWallIndex,savedGroundIndex;
    Rect savedBounds{-9,-4.5f,9,4};
    bool ready=false;
    void swapActive() {
        savedWalls.swap(walls);savedLevels.swap(wallLevels);savedGround.swap(ground);
        savedWallContacts.swap(wallContacts);
        savedWallTriangles.swap(wallTriangles);
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
    staged=loadCollision((path+".walls").c_str()) && loadGround((path+".ground").c_str()) && loadGroundReturns((path+".returns").c_str());
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
bool loadGround(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    uint32_t count=0;bool ok=std::fread(&count,4,1,file)==1;
    bool tagged=count&0x80000000;count&=0x7fffffff;ok=ok && count>0 && count<=100000;
    std::vector<GroundTriangle> data(ok?count:0);
    for(auto& tri:data) {
        ok=ok && std::fread(tri.v,sizeof(tri.v),1,file)==1;
        if(tagged)ok=ok && std::fread(&tri.contact,sizeof(tri.contact),1,file)==1;
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
static bool groundWithin(Vec2 p,float low,float high,float& height,GroundContact* contact) {
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(low) || !std::isfinite(high) || low>high ||
       std::fabs(p.x)>4096 || std::fabs(p.z)>4096)return false;
    if(ground.empty()) {if(low>0 || high<0)return false;height=0;if(contact)*contact={};return true;}
    bool found=false;float best=-1e30f;GroundContact bestContact;
    for(uint32_t n:nearby(groundIndex,p)) {
        // Original normal-Mario chkfilterVec rejects bit4 unless its special
        // inclusion bits apply. Name-dependent bit31 cases are not present
        // in the five currently packaged rooms; transform filters are deferred.
        const uint32_t flags=ground[n].contact.attributes;
        if(!(flags&0x00080080) && !(flags&0x00800000) && (flags&4))continue;
        const float* v=ground[n].v;
        if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) ||
           p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
        float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
        if(std::fabs(den)<1e-8f)continue;
        float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
        float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
        if(a<-.0001f || b<-.0001f || a+b>1.0001f)continue;
        float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
        if(y<=high && y>=low && y>best) {best=y;bestContact=ground[n].contact;found=true;}
    }
    if(found) {
        if(groundReturns.count(bestContact.sourceNode))bestContact.attributes|=0x40000000;
        height=best;if(contact)*contact=bestContact;
    }
    return found;
}
bool groundAt(Vec2 p,float reference,float& height,GroundContact* contact) {
    // Preserve development/no-pack flat-ground behavior for legacy callers.
    if(ground.empty() && std::isfinite(reference))return groundWithin(p,-.5f,StepHeight,height,contact);
    return groundWithin(p,reference-.5f,reference+StepHeight,height,contact);
}
bool landingGround(Vec2 p,float worldY,float velocity,float& height,GroundContact* contact) {
    if(!std::isfinite(velocity))return false;
    const float reach=std::fabs(velocity);
    return groundWithin(p,worldY-reach-.0001f,worldY+reach+.05f,height,contact);
}
bool landingFootprint(Vec2 p,Vec2 direction,float worldY,float velocity,float& height,GroundContact* contact) {
    float length=std::hypot(direction.x,direction.z);
    if(!std::isfinite(length) || length<1e-6f)return false;
    const float x=direction.x*FloorFootRadius/length,z=direction.z*FloorFootRadius/length;
    const Vec2 offsets[]={{0,0},{x,z},{-z,x},{z,-x},{-x,-z}};
    bool found=false;float best=-1e30f;GroundContact selected;
    for(auto offset:offsets) {
        float floor=0;GroundContact candidate;
        if(landingGround({p.x+offset.x,p.z+offset.z},worldY,velocity,floor,&candidate) &&
           std::fabs(floor-worldY)<=FloorSnapDistance && floor>best){best=floor;selected=candidate;found=true;}
    }
    if(found){height=best;if(contact)*contact=selected;}
    return found;
}
void Game::advanceVertical(bool held) {
    if(!fieldJump.airborne){jumpHeight=0;if(groundHeight<=VoidThreshold)beginVoid();return;}
    const float worldY=jumpBase+fieldJump.height;
    fieldJump.tick(held,false);
    float floor=0;GroundContact contact;
    if(fieldJump.falling && landingFootprint(progress.position,moveDirection,worldY,fieldJump.velocity,floor,&contact)) {
        fieldJump={};groundHeight=progress.elevation=floor;groundContact=contact;jumpBase=floor;jumpHeight=0;
    } else {
        groundContact={};jumpHeight=jumpBase+fieldJump.height-groundHeight;
    }
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
    fieldJump={};jumpHeight=0;warpRequest=-1;saveRequested=false;
    return true;
}
void clearCollision() {walls.clear();wallLevels.clear();wallContacts.clear();wallTriangles.clear();ground.clear();groundReturns.clear();wallIndex.clear();groundIndex.clear();collisionReady=false;roomBounds={-9,-4.5f,9,4};}
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
    jumpHeight=0;fieldJump={};warpRequest=-1;saveRequested=false;return true;
}
void Game::restore(const Progress& saved) {
    const Progress p=saved; // Caller may pass this game's own progress.
    *this = Game{};
    progress = p;
    partner = {p.position.x-0.8f,p.position.z};
    groundHeight=p.elevation;partnerHeight=p.elevation;
    if (!walkable(partner,p.elevation)) partner = p.position;
    groundAt(p.position,p.elevation,groundHeight,&groundContact);
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
        if (phase == Phase::Windup) return selection == 0 ? "Press A just before the landing!" : "Hold B, then release at the marker!";
        if (phase == Phase::EnemyWindup) return "Press A just before Crump lands!";
        return actionSuccess ? "Nice!" : "Keep your timing steady.";
    }
    if (progress.room==0 && !progress.crumpDefeated && near(CrumpPosition)) return "A: confront Lord Crump";
    if (progress.room==0 && near(NpcPosition)) return "A: talk";
    return "Circle Pad: move   A: jump   X: partner";
}
void Game::move(const Input& i) {
    // Original marioMoveMain remembers the eligible floor before moving.
    if(groundContact.attributes&0x40000000)returnContact=groundContact;
    float x=i.x, z=i.z, len=std::hypot(x,z);
    const float speed=len>DashThreshold?DashSpeed:WalkSpeed;
    if(len>0){x/=len;z/=len;}
    if(len>0)moveDirection={x,z};
    Vec2 p = progress.position;
    Vec2 candidate{p.x+x*speed,p.z};
    float nextHeight=groundHeight;
    GroundContact nextContact;
    auto accept=[&](Vec2 candidate) {
        const float y=groundHeight+jumpHeight;
        if(!walkable(candidate,y))return false;
        if(fieldJump.airborne){p=candidate;return true;}
        if(groundAt(candidate,groundHeight,nextHeight,&nextContact) && walkable(candidate,nextHeight)) {
            p=candidate;groundHeight=nextHeight;groundContact=nextContact;return true;
        }
        if(landingFootprint(candidate,moveDirection,y,0,nextHeight,&nextContact)) {
            p=candidate;groundHeight=nextHeight;groundContact=nextContact;return true;
        }
        p=candidate;jumpBase=y;fieldJump.beginFall();groundContact={};return true;
    };
    accept(candidate);
    candidate={p.x,p.z+z*speed};
    accept(candidate);
    progress.position=p;progress.elevation=groundHeight;
    if(warpCooldown>0)--warpCooldown;
    if(warpCooldown==0 && warpRequest<0 && len>.15f) {
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
    advanceVertical(i.jumpHeld);
    if(fieldJump.airborne)saveRequested=false;
}
void Game::startBattle() {
    scene=Scene::Battle; phase=Phase::Select; phaseTick=0; selection=0;
    enemyHP=BattleRules::CrumpHP; actionSuccess=false; guarded=false;
    jumpAttempted=false; hammerArmed=false; guardAttempted=false;
    jumpHeight=0; fieldJump={};
}
void Game::attackDone(int damage) {
    enemyHP=std::max(0,enemyHP-damage); lastDamage=damage;
    phase=Phase::Impact; phaseTick=0;
    sound=actionSuccess ? Sound::Success : Sound::Hit;
}
void Game::battle(const Input& i) {
    if (phase == Phase::Select) {
        if (i.left) selection=(selection+2)%3;
        if (i.right) selection=(selection+1)%3;
        int touch=-1;
        if (i.touchY>=100 && i.touchY<158 && i.touchX>=8 && i.touchX<312) touch=(i.touchX-8)/102;
        if (touch>=0 && touch<3) selection=touch;
        if (i.confirm || touch>=0) {
            if (selection==2) {
                if (progress.mushrooms<=0 || progress.hp==10) { sound=Sound::Confirm; return; }
                --progress.mushrooms; progress.hp=std::min(10,progress.hp+5);
                lastDamage=0; actionSuccess=false; phase=Phase::Impact; phaseTick=0;
            } else {
                phase=Phase::Windup; phaseTick=0; actionSuccess=false;
                jumpAttempted=false; hammerArmed=i.hammerHeld;
            }
            sound=Sound::Confirm;
        }
        return;
    }
    ++phaseTick;
    if (phase==Phase::Windup) {
        if (selection==0) {
            if (i.confirm && !jumpAttempted) {
                jumpAttempted=true;
                actionSuccess=phaseTick>=BattleRules::JumpImpact-BattleRules::JumpWindow && phaseTick<=BattleRules::JumpImpact;
            }
            if (phaseTick>=BattleRules::JumpImpact) attackDone(actionSuccess?2:1);
        } else {
            if (i.hammerHeld) hammerArmed=true;
            if (i.hammerReleased || phaseTick>=BattleRules::HammerRelease+BattleRules::HammerWindow) {
                actionSuccess=hammerArmed && i.hammerReleased && std::abs(phaseTick-BattleRules::HammerRelease)<=BattleRules::HammerWindow;
                attackDone(actionSuccess?2:1);
            }
        }
    } else if (phase==Phase::Impact && phaseTick>=42) {
        if (enemyHP==0) { scene=Scene::Victory; sound=Sound::Success; }
        else { phase=Phase::EnemyWindup; phaseTick=0; guarded=false; guardAttempted=false; }
    } else if (phase==Phase::EnemyWindup) {
        if (i.confirm && !guardAttempted) {
            guardAttempted=true;
            guarded=phaseTick>=BattleRules::EnemyImpact-BattleRules::GuardWindow && phaseTick<=BattleRules::EnemyImpact;
        }
        if (phaseTick>=BattleRules::EnemyImpact) {
            lastDamage=guarded?0:BattleRules::CrumpAttack;
            progress.hp=std::max(0,progress.hp-lastDamage);
            phase=Phase::EnemyImpact; phaseTick=0; sound=guarded?Sound::Success:Sound::Hit;
        }
    } else if (phase==Phase::EnemyImpact && phaseTick>=36) {
        if (progress.hp==0) scene=Scene::Defeat;
        else { phase=Phase::Select; phaseTick=0; }
    }
}
void Game::tick(const Input& i) {
    ++ticks; sound=Sound::None;
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
        progress.crumpDefeated=true; scene=Scene::Explore; saveRequested=true; menu=Menu::Status;
        return;
    }
    if (scene==Scene::Defeat) {
        if (i.confirm || (tap && i.touchY>=107 && i.touchY<155)) { progress.hp=10; startBattle(); }
        else if (i.cancel || tap) { progress.hp=10; scene=Scene::Explore; }
        return;
    }
    if (scene!=Scene::Explore) return;
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
    move(i);
    if(warpRequest>=0 || water.active())return;
    if((groundContact.attributes&0x200) && !fieldJump.airborne && beginWater())return;
    if (i.cancel) openDialogue(2);
    else if (i.confirm && !fieldJump.airborne) {
        if (progress.room==0 && !progress.crumpDefeated && near(CrumpPosition)) openDialogue(1);
        else if (progress.room==0 && near(NpcPosition)) openDialogue(0);
        else { jumpBase=groundHeight;fieldJump.begin();saveRequested=false;sound=Sound::Jump; }
    }
}
}
