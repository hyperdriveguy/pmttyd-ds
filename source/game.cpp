#include "game.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
namespace ttyd {
namespace {
std::vector<Rect> walls;
float length(Vec2 a, Vec2 b) { return std::hypot(a.x-b.x, a.z-b.z); }
int menuButton(const Input& i) {
    if (i.touchY >= 44 && i.touchY < 82 && i.touchX >= 8 && i.touchX < 312)
        return (i.touchX-8)/76;
    return -1;
}
}
bool walkable(Vec2 p) {
    constexpr float r = 0.28f;
    if (!std::isfinite(p.x) || !std::isfinite(p.z)) return false;
    if(!walls.empty()) {
        if(p.x < -6.7f+r || p.x > 6.7f-r || p.z < -4.5f+r || p.z > 5.8f-r)return false;
        for(auto wall:walls) {
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
    if(!ok || count==0 || count>10000) {std::fclose(file);return false;}
    std::vector<Rect> data(count);ok=std::fread(data.data(),sizeof(Rect),count,file)==count;std::fclose(file);
    for(auto w:data) if(!std::isfinite(w.x0) || !std::isfinite(w.z0) || !std::isfinite(w.x1) || !std::isfinite(w.z1))ok=false;
    if(ok) walls.swap(data);
    return ok;
}
void clearCollision() {walls.clear();}
const std::vector<Rect>& collisionWalls() {return walls;}
bool Game::near(Vec2 p, float d) const { return length(progress.position,p) < d; }
void Game::restore(const Progress& p) {
    *this = Game{};
    progress = p;
    partner = {p.position.x-0.8f,p.position.z};
    if (!walkable(partner)) partner = p.position;
    cameraX = std::clamp(p.position.x,-3.5f,3.5f);
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
    if (!progress.crumpDefeated && near(CrumpPosition)) return "A: confront Lord Crump";
    if (near(NpcPosition)) return "A: talk";
    return "Circle Pad: move   A: jump   X: partner";
}
void Game::move(const Input& i) {
    float x=i.x, z=i.z, len=std::hypot(x,z);
    if (len > 1) { x/=len; z/=len; }
    Vec2 p = progress.position;
    Vec2 candidate{p.x+x*0.065f,p.z};
    if (walkable(candidate)) p=candidate;
    candidate={p.x,p.z+z*0.065f};
    if (walkable(candidate)) p=candidate;
    progress.position=p;
    if (std::fabs(x) > .15f && (x>0 ? 1.f : -1.f) != facing) {
        facing = x>0 ? 1.f : -1.f; turnTick=10;
    }
    if (turnTick>0) { --turnTick; turnScale=std::max(.08f,std::fabs(std::cos(turnTick*3.14159265f/10.f))); }
    else turnScale=1;
    float dist=length(partner,p);
    if (dist>0.9f) {
        float speed=std::min(.065f,(dist-.9f)*.12f);
        Vec2 q{partner.x+(p.x-partner.x)/dist*speed,partner.z};
        if (walkable(q)) partner=q;
        q={partner.x,partner.z+(p.z-partner.z)/dist*speed};
        if (walkable(q)) partner=q;
    }
    cameraX += (std::clamp(p.x,-3.5f,3.5f)-cameraX)*.07f;
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
        if (!progress.crumpDefeated && near(CrumpPosition)) openDialogue(1);
        else if (near(NpcPosition)) openDialogue(0);
        else { jumpTick=36; sound=Sound::Jump; }
    }
}
}
