#include "game.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
namespace ttyd {
namespace {
std::vector<RoadWarp> roads;
std::vector<Rect> walls;
bool collisionReady=false;
std::vector<Vec2> wallLevels;
struct GroundTriangle { float v[9]; };
std::vector<GroundTriangle> ground;
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
const char* roomAsset(uint32_t room) {
    static const char* names[]={"map","gor_00","gor_02","gor_03"};
    return room<4?names[room]:nullptr;
}
const char* roomName(uint32_t room) {
    static const char* names[]={"Rogueport plaza","Rogueport harbor","Rogueport east side","Rogueport west side"};
    return room<4?names[room]:"Unknown area";
}
bool wallAtLevel(size_t n,float elevation) {
    return n<walls.size() && (wallLevels.empty() || (wallLevels[n].z>=elevation+.1f && wallLevels[n].x<=elevation+1.2f));
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
    Vec2 arrival{w.spawnX,w.spawnZ};
    if(!groundAt(arrival,height,height) || !walkable(arrival,height))return false;
    Progress next=progress;next.room=w.target;next.position=arrival;next.elevation=height;
    restore(next);groundHeight=height;partner=arrival;partnerHeight=height;
    followerPath.clear();warpCooldown=45;saveRequested=true;return true;
}
bool walkable(Vec2 p,float elevation) {
    constexpr float r = 0.28f;
    if (!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(elevation) || std::fabs(p.x)>4096 || std::fabs(p.z)>4096) return false;
    if(collisionReady) {
        if(ground.empty() && (p.x < -6.7f+r || p.x > 6.7f-r || p.z < -4.5f+r || p.z > 5.8f-r))return false;
        for(uint32_t n:nearby(wallIndex,p)) {
            auto wall=walls[n];
            if(!wallAtLevel(n,elevation))continue;
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
    bool levels=(count&0x80000000)!=0;count&=0x7fffffff;
    if(!ok || count>100000) {std::fclose(file);return false;}
    std::vector<Rect> data(count);std::vector<Vec2> heights(levels?count:0);
    for(uint32_t n=0;n<count;++n) {
        ok=ok && std::fread(&data[n],sizeof(Rect),1,file)==1;
        if(levels)ok=ok && std::fread(&heights[n],sizeof(Vec2),1,file)==1;
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);
    for(auto w:data) if(!std::isfinite(w.x0) || !std::isfinite(w.z0) || !std::isfinite(w.x1) || !std::isfinite(w.z1))ok=false;
    for(auto h:heights)if(!std::isfinite(h.x) || !std::isfinite(h.z) || h.x>h.z)ok=false;
    SpatialIndex next;size_t references=0;
    for(uint32_t n=0;ok && n<count;++n) {
        auto w=data[n];
        ok=indexBounds(next,n,std::min(w.x0,w.x1)-.28f,std::min(w.z0,w.z1)-.28f,
                       std::max(w.x0,w.x1)+.28f,std::max(w.z0,w.z1)+.28f,references);
    }
    if(ok) {wallLevels.swap(heights);walls.swap(data);wallIndex.swap(next);collisionReady=true;}
    return ok;
}
bool loadGround(const char* path) {
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    uint32_t count=0;bool ok=std::fread(&count,4,1,file)==1 && count>0 && count<=100000;
    std::vector<GroundTriangle> data(ok?count:0);
    if(ok)ok=std::fread(data.data(),sizeof(GroundTriangle),count,file)==count && std::fgetc(file)==EOF;
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
        ground.swap(data);groundIndex.swap(next);roomBounds=bounds;
    }
    return ok;
}
bool groundAt(Vec2 p,float reference,float& height) {
    if(!std::isfinite(p.x) || !std::isfinite(p.z) || !std::isfinite(reference) ||
       std::fabs(p.x)>4096 || std::fabs(p.z)>4096)return false;
    if(ground.empty()) {height=0;return true;}
    bool found=false;float best=-1e30f;
    for(uint32_t n:nearby(groundIndex,p)) {
        const float* v=ground[n].v;
        if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) ||
           p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
        float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);
        if(std::fabs(den)<1e-8f)continue;
        float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
        float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
        if(a<-.0001f || b<-.0001f || a+b>1.0001f)continue;
        float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
        // Stay on the current floor; do not teleport onto roofs above Mario.
        if(y<=reference+.25f && y>=reference-.5f && y>best) {best=y;found=true;}
    }
    if(found)height=best;
    return found;
}
void clearCollision() {walls.clear();wallLevels.clear();ground.clear();wallIndex.clear();groundIndex.clear();collisionReady=false;roomBounds={-9,-4.5f,9,4};}
const std::vector<Rect>& collisionWalls() {return walls;}
bool Game::near(Vec2 p, float d) const { return length(progress.position,p) < d; }
void Game::restore(const Progress& saved) {
    const Progress p=saved; // Caller may pass this game's own progress.
    *this = Game{};
    progress = p;
    partner = {p.position.x-0.8f,p.position.z};
    groundHeight=p.elevation;partnerHeight=p.elevation;
    if (!walkable(partner,p.elevation)) partner = p.position;
    groundAt(p.position,p.elevation,groundHeight);
    groundAt(partner,groundHeight,partnerHeight);
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
    float x=i.x, z=i.z, len=std::hypot(x,z);
    if (len > 1) { x/=len; z/=len; }
    Vec2 p = progress.position;
    Vec2 candidate{p.x+x*0.065f,p.z};
    float nextHeight=groundHeight;
    if (walkable(candidate,groundHeight) && groundAt(candidate,groundHeight,nextHeight)) {p=candidate;groundHeight=nextHeight;}
    candidate={p.x,p.z+z*0.065f};
    if (walkable(candidate,groundHeight) && groundAt(candidate,groundHeight,nextHeight)) {p=candidate;groundHeight=nextHeight;}
    progress.position=p;progress.elevation=groundHeight;
    if(warpCooldown>0)--warpCooldown;
    if(warpCooldown==0 && warpRequest<0 && len>.15f) {
        for(size_t n=0;n<roads.size();++n) {
            const auto& road=roads[n];
            if(road.source==progress.room && length(p,{road.x,road.z})<.7f &&
                x*(road.x-p.x)+z*(road.z-p.z)>0) {warpRequest=int(n);break;}
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
    if(remaining>.9f) {
        float budget=std::min(.065f,remaining-.9f);
        while(budget>0 && !followerPath.empty()) {
            Vec2 target=followerPath.front();float dist=length(partner,target);
            if(dist<.001f) {followerPath.erase(followerPath.begin());continue;}
            float step=std::min(budget,dist);
            Vec2 q{partner.x+(target.x-partner.x)/dist*step,partner.z+(target.z-partner.z)/dist*step};
            float h=partnerHeight;
            if(!walkable(q,partnerHeight) || !groundAt(q,partnerHeight,h))break;
            partner=q;partnerHeight=h;budget-=step;
            if(step>=dist)followerPath.erase(followerPath.begin());
        }
    }
    // Bound the trail if the partner cannot traverse an old segment.
    if(followerPath.size()>512) {partner=p;partnerHeight=groundHeight;followerPath.clear();}
    cameraX+=(p.x-cameraX)*.07f;
    cameraZ+=(p.z-cameraZ)*.07f;
    cameraHeight+=(groundHeight-cameraHeight)*.07f;
    if (jumpTick>0) { --jumpTick; jumpHeight=1.1f*std::sin(jumpTick*3.14159265f/36.f); }
    else jumpHeight=0;
}
void Game::startBattle() {
    scene=Scene::Battle; phase=Phase::Select; phaseTick=0; selection=0;
    enemyHP=BattleRules::CrumpHP; actionSuccess=false; guarded=false;
    jumpAttempted=false; hammerArmed=false; guardAttempted=false;
    jumpHeight=0; jumpTick=0;
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
    if (i.cancel) openDialogue(2);
    else if (i.confirm && jumpTick==0) {
        if (progress.room==0 && !progress.crumpDefeated && near(CrumpPosition)) openDialogue(1);
        else if (progress.room==0 && near(NpcPosition)) openDialogue(0);
        else { jumpTick=36; sound=Sound::Jump; }
    }
}
}
