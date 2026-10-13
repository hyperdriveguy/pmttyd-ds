#include "game.hpp"
#include "wall_geometry.hpp"
#include "head_collision.hpp"
#include "floor_contact.hpp"
#include "save.hpp"
#include "visibility.hpp"
#include "uv_animation.hpp"
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unistd.h>
using namespace ttyd;
#define CHECK(x) do { if(!(x)) throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x); } while(0)
void advance(Game& g,int ticks) {while(ticks-->0)g.tick({});}
Input confirm() {Input i;i.confirm=true;return i;}
void attack(Game& g,bool success) {
    CHECK(g.phase==Phase::Select);g.selection=0;g.tick(confirm());
    advance(g,success?BattleRules::JumpImpact-1:BattleRules::JumpImpact);
    if(success)g.tick(confirm());
    CHECK(g.phase==Phase::Impact);
}
void headContactRules() {
    for(float angle:{50.f,90.f,130.f})CHECK(!headInclinationAllowed(angle));
    CHECK(headInclinationAllowed(49.99f) && headInclinationAllowed(130.01f));
    CHECK(headInclinationAllowed(60,true) && !headInclinationAllowed(75,true) && !headInclinationAllowed(105,true));
    FieldJump idle;idle.begin(1);idle.tick(true,false);float proposed=.9f;
    const float retainedHeight=idle.height,retainedV=idle.velocity;const int retainedCounter=idle.ticks;
    CHECK(!idle.prepareCeiling(proposed,0,.01f) && proposed==1);
    CHECK(idle.motion==0 && idle.pendingEntry && idle.jumping && !idle.falling);
    CHECK(idle.height==retainedHeight && idle.velocity==retainedV && idle.ticks==retainedCounter);
    FieldJump mismatch;mismatch.enterGround(2);mismatch.begin(1);mismatch.tick(true,false);proposed=.9f;
    CHECK(!mismatch.prepareCeiling(proposed,.01f,.01f) && mismatch.motion==1 && mismatch.jumping);
    FieldJump matched;matched.enterGround(1);matched.begin(1);matched.tick(true,false);proposed=.9f;
    CHECK(matched.prepareCeiling(proposed,.01f,.01f) && matched.motion==3 && proposed==1);
    FieldJump equality;equality.begin(1);proposed=1;
    CHECK(equality.prepareCeiling(proposed,0,.01f) && equality.motion==3); // Strict below, not equality.
    CHECK(headMotionResponds(3) && headMotionResponds(6));
    for(unsigned motion:{0u,1u,2u,4u,7u,9u,10u,12u})CHECK(!headMotionResponds(motion));
    CHECK(stairRiseAllowed(0,.05f,30));CHECK(stairRiseAllowed(0,.11f,0));
    CHECK(!stairRiseAllowed(0,.049f,0));CHECK(!stairRiseAllowed(0,.111f,0));
    CHECK(!stairRiseAllowed(0,.08f,30.1f));
    FieldStep step;step.begin(0,.09f);CHECK(step.active());
    step.tick();CHECK(std::fabs(step.height-.03f)<.00001f);
    step.tick();CHECK(std::fabs(step.height-.06f)<.00001f);
    step.tick();CHECK(!step.active() && step.height==.09f);
    const float rad=3.14159265358979323846f/180.f;
    CHECK(floorSlopeAllowed(std::sin(49.9f*rad),std::cos(49.9f*rad),0));
    CHECK(!floorSlopeAllowed(std::sin(50.1f*rad),std::cos(50.1f*rad),0));
    CHECK(!floorSlopeAllowed(0,-1,0)); // Original angle uses signed normalY.
    CHECK(!floorSlopeAllowed(0,0,0));CHECK(!floorSlopeAllowed(1,0,0));
    CHECK(floorCandidateReachable(0,0,-.019f));
    CHECK(!floorCandidateReachable(0,0,-.02f));
    CHECK(floorCandidateReachable(0,-.02f,-.02f));
    CHECK(!floorCandidateReachable(0,-.019f,-.02f));
    CHECK(floorCandidateReachable(0,-.18f,-.15f));
    CHECK(!floorCandidateReachable(0,-.18f,-.181f));
    CHECK(floorCandidateReachable(0,0,.11f));
    CHECK(!floorCandidateReachable(0,0,.111f));
    CHECK(headAttributesSolid(0));CHECK(!headAttributesSolid(4));
    CHECK(headAttributesSolid(0x00800004));
    CHECK(!headAttributesSolid(0x00080004));CHECK(!headAttributesSolid(0x84));
    // Engine-space example: ceiling45, contact45.18, root7.2 after clearance.
    const float contact=headContactHeight(.45f);
    CHECK(std::fabs(contact-.4518f)<.00001f);
    CHECK(!headContactReached(.081f,.01f,contact));
    CHECK(headContactReached(.082f,.01f,contact));
    CHECK(headContactReached(.082f,-.1f,contact));
    CHECK(!headContactReached(.082f,-.1001f,contact));
    CHECK(!headContactReached(.1f,std::numeric_limits<float>::quiet_NaN(),contact));
    CHECK(std::fabs(headClampedRoot(contact)-.072f)<.00001f);
    CHECK(std::fabs(headClampedRoot(-.4518f)-(-.831f))<.00001f);
}
void headGeometry() {
    clearCollision();char path[]="/tmp/ttyd-head-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);close(fd);
    auto ceiling=[&](uint32_t flags,uint32_t cull,bool reverse=false) {
        float tri[9]={-2,.45f,1,2,.45f,1,-2,.45f,4};
        if(reverse)for(int n=0;n<3;++n)std::swap(tri[3+n],tri[6+n]);
        std::ofstream out(path,std::ios::binary);uint32_t count=1,node=100;
        out.write("HEAD",4);out.write(reinterpret_cast<char*>(&count),4);out.write(reinterpret_cast<char*>(tri),36);
        out.write(reinterpret_cast<char*>(&flags),4);out.write(reinterpret_cast<char*>(&node),4);out.write(reinterpret_cast<char*>(&cull),4);
    };
    float root=99;ceiling(0,1);CHECK(loadHead(path));
    FieldSlope record;record.inclination=23;record.tiltX=9;record.tiltZ=4;
    auto miss=searchFieldHead({3,2},{1,0},.1f,.01f,.1f,&record);
    CHECK(!miss.contact && miss.clearance==-.01f && record.tiltX==9 && record.tiltZ==4);
    auto nearHead=searchFieldHead({-1,2},{1,0},.081f,.01f,.1f,&record);
    CHECK(!nearHead.contact && nearHead.clearance>0 && nearHead.clearance<.001f);
    CHECK(record.inclination==23 && record.tiltX<-179 && record.tiltZ<-179);
    auto fastHead=searchFieldHead({-1,2},{1,0},.1f,-.1001f,.1f,&record);
    CHECK(!fastHead.contact && fastHead.clearance<0); // Actual velocity gates node, not query/angle writes.
    CHECK(!headCollision({-1,2},{1,0},.081f,.01f,root));CHECK(root==99);
    CHECK(headCollision({-1,2},{1,0},.082f,.01f,root));CHECK(std::fabs(root-.072f)<.00001f);
    CHECK(headCollision({-2.08f,2},{1,0},.1f,.01f,root)); // Center misses; forward probe hits.
    CHECK(!headCollision({-2.08f,2},{1,1},.1f,.01f,root)); // Rotated footprint misses.
    CHECK(!headCollision({3,2},{1,0},.1f,.01f,root));
    ceiling(0,1,true);CHECK(loadHead(path));CHECK(!headCollision({-1,2},{1,0},.1f,.01f,root));
    ceiling(0,3,true);CHECK(loadHead(path));CHECK(headCollision({-1,2},{1,0},.1f,.01f,root));
    for(uint32_t flags:{4u,0x84u,0x80004u,0x800004u}) {
        ceiling(flags,3);CHECK(loadHead(path));
        CHECK(headCollision({-1,2},{1,0},.1f,.01f,root)==(flags==0x800004u));
    }
    ceiling(0,1);CHECK(loadHead(path));CHECK(truncate(path,47)==0);CHECK(!loadHead(path));
    CHECK(headCollision({-1,2},{1,0},.1f,.01f,root)); // Failed pack preserves geometry.
    {CollisionTransition rollback;clearCollision();CHECK(!headCollision({-1,2},{1,0},.1f,.01f,root));}
    CHECK(headCollision({-1,2},{1,0},.1f,.01f,root));
    ceiling(0,1);
    {std::fstream out(path,std::ios::binary|std::ios::in|std::ios::out);float high=.65f;
        out.seekp(24);out.write(reinterpret_cast<char*>(&high),4);}
    CHECK(loadHead(path));CHECK(headCollision({-1,2},{1,0},.14f,.01f,root));
    CHECK(std::fabs(root-.122f)<.00001f); // Analytical sloped-plane center intersection.
    ceiling(0,1);
    {std::fstream out(path,std::ios::binary|std::ios::in|std::ios::out);float low=.35f;
        for(int offset:{12,24,36}){out.seekp(offset);out.write(reinterpret_cast<char*>(&low),4);}}
    CHECK(loadHead(path));Game standing;standing.progress.room=2;standing.progress.position={-1,2};
    standing.tick({});CHECK(standing.overheadContact && !standing.airborne() && standing.groundHeight==0);
    Input walk;walk.x=1;standing.tick(walk);CHECK(standing.overheadContact && standing.progress.position.x>-1 && standing.groundHeight==0);
    Progress lowStart;lowStart.room=2;lowStart.position={-1,2};Game lowIdle;lowIdle.restore(lowStart);
    lowIdle.tick(confirm());CHECK(lowIdle.airborne() && lowIdle.fieldAirMotion().motion==0);
    CHECK(lowIdle.fieldAirMotion().jumping && lowIdle.fieldAirMotion().pendingEntry);
    CHECK(lowIdle.fieldAirMotion().ticks==1 && lowIdle.fieldAirMotion().velocity==.08f && lowIdle.jumpHeight==0);
    Input lowHeld;lowHeld.jumpHeld=true;lowIdle.tick(lowHeld);
    CHECK(!lowIdle.airborne() && !lowIdle.fieldAirMotion().hasAirFlags() && lowIdle.fieldAirMotion().velocity==0);
    Game lowWalk;lowWalk.restore(lowStart);Input lowLaunch=confirm();lowLaunch.x=.5f;
    lowWalk.tick(lowLaunch);CHECK(lowWalk.fieldAirMotion().motion==10 && lowWalk.fieldAirMotion().pendingEntry);
    CHECK(!lowWalk.fieldAirMotion().hasAirFlags() && lowWalk.fieldAirMotion().ticks==1);
    CHECK(std::fabs(lowWalk.progress.position.x-lowStart.position.x-.013461875f)<.00001f);
    clearCollision();standing.tick({});CHECK(!standing.overheadContact);
    ceiling(0,1);CHECK(loadHead(path));
    Game g;g.progress.room=2;g.progress.position={-1,2};g.tick(confirm());
    float maxY=0;Input held;held.jumpHeld=true;
    for(int n=0;n<40;++n){g.tick(held);maxY=std::max(maxY,g.groundHeight+g.jumpHeight);}
    CHECK(maxY<=.073f && maxY>.07f);CHECK(!g.airborne() && g.jumpHeight==0);
    // Retained rightward travel rotates the head probes, even while current
    // diagonal input would make the edge probe miss this original triangle.
    Game momentum;Progress launch;launch.room=2;launch.position={-2.12f,2};momentum.restore(launch);
    Input running;running.x=.5f;momentum.tick(running);running.confirm=true;momentum.tick(running);
    momentum.progress.position={-2.08f,2};momentum.tick(held);
    CHECK(momentum.overheadContact && momentum.headContact.sourceNode==0);
    CHECK(momentum.headClearance>0 && momentum.headClearance<=.05f); // Near miss sets contact flag without a node.
    momentum.progress.position={-2.08f,2};Input diagonal=held;diagonal.x=diagonal.z=1;
    momentum.tick(diagonal);CHECK(momentum.overheadContact && momentum.jumpHeight<.073f);
    clearCollision();Game falling;falling.progress.room=2;falling.progress.position={-1,2};falling.tick(confirm());
    float last=0;
    for(int n=0;n<30;++n) {
        last=falling.jumpHeight;falling.tick(held);
        if(falling.jumpHeight<last-.01f)break;
    }
    const float before=falling.jumpHeight;CHECK(before>.15f && falling.airborne());
    // Insert overhead contact at the pre-gravity head height while descending.
    Game control=falling;control.tick(held);CHECK(!control.overheadContact);
    ceiling(0,1);const float roof=before+.365f;
    {std::fstream out(path,std::ios::binary|std::ios::in|std::ios::out);
        for(int offset:{12,24,36}){out.seekp(offset);out.write(reinterpret_cast<const char*>(&roof),4);}}
    CHECK(loadHead(path));CHECK(!headCollision({-1,2},{1,0},before,-.1001f,root,.1f));
    falling.tick(held);CHECK(falling.overheadContact);
    CHECK(std::fabs(falling.jumpHeight-control.jumpHeight)<.00001f); // Motion10 has no jump response.
    for(int n=0;n<40 && falling.airborne();++n)falling.tick({});
    CHECK(!falling.airborne() && falling.jumpHeight==0);
    // Center near-miss must return before a later lateral probe's contact.
    ceiling(0,1);
    {std::fstream file(path,std::ios::binary|std::ios::in|std::ios::out);uint32_t count=2;
        file.seekp(4);file.write(reinterpret_cast<char*>(&count),4);file.seekp(0,std::ios::end);
        float lower[]={-.95f,.30f,1,2,.30f,1,-.95f,.30f,4};GroundContact owner{0,200};uint32_t cull=1;
        file.write(reinterpret_cast<char*>(lower),36);file.write(reinterpret_cast<char*>(&owner),8);file.write(reinterpret_cast<char*>(&cull),4);}
    CHECK(loadHead(path));auto first=searchFieldHead({-1,2},{1,0},.06f,0,.1f);
    CHECK(!first.contact && first.clearance>.02f && first.clearance<.03f);
    // A nearest steep ray updates projected tilts, then rejects its cone. The
    // separate +4 flag selects the narrower rejection interval on that same hit.
    ceiling(0,1);
    {std::fstream file(path,std::ios::binary|std::ios::in|std::ios::out);
        const float rise=std::sqrt(3.f)*2;
        float steep[]={-2,.3f-rise,1,2,.3f+rise,1,-2,.3f-rise,4};
        file.seekp(8);file.write(reinterpret_cast<char*>(steep),36);}
    CHECK(loadHead(path));record.inclination=17;
    auto rejected=searchFieldHead({0,2},{1,0},0,0,.1f,&record);
    CHECK(!rejected.contact && rejected.clearance==-.01f && record.inclination==17 && record.tiltX<-119);
    auto narrow=searchFieldHead({0,2},{1,0},0,0,.1f,&record,true);
    CHECK(narrow.contact && narrow.node.sourceNode==100 && record.inclination==17);
    clearCollision();std::remove(path);
}
void movement() {
    headGeometry();
    headContactRules();
    { // A low wall blocks standing movement, but must use this tick's new jump Y.
    char path[]="/tmp/ttyd-air-wall-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=0x80000001;Rect wall{-.9f,1,-.9f,3};Vec2 heights{.11f,.16f};
    CHECK(write(fd,&count,4)==4);CHECK(write(fd,&wall,sizeof(wall))==sizeof(wall));
    CHECK(write(fd,&heights,sizeof(heights))==sizeof(heights));close(fd);CHECK(loadCollision(path));
    Progress initial;initial.room=2;initial.position={-1,2};
    Input right;right.x=1;right.jumpHeld=true;
    Game standing;standing.restore(initial);standing.tick(right);CHECK(standing.progress.position.x==-1);
    Input runningJump=right;runningJump.confirm=true;
    Game jumping;jumping.restore(initial);jumping.tick(right);jumping.tick(runningJump);jumping.tick(right);
    CHECK(jumping.jumpHeight>.07f && jumping.progress.position.x>-.99f);
    clearCollision();std::remove(path);}
    FieldJump tapped;tapped.begin();for(int n=0;n<21;++n)tapped.tick(false);
    CHECK(tapped.airborne);tapped.tick(false);CHECK(!tapped.airborne);
    FieldJump held;held.begin();for(int n=0;n<4;++n)held.tick(true);
    CHECK(std::fabs(held.height-.245f)<.00001f);
    for(int n=4;n<25;++n)held.tick(true);CHECK(held.airborne);
    held.tick(true);CHECK(!held.airborne && held.height==0);
    auto displacement=[](float x,float z,int frames) {
        Game g;g.progress.position={-4,2};g.partner=g.progress.position;
        Input i;i.x=x;i.z=z;for(int n=0;n<frames;++n)g.tick(i);
        return Vec2{g.progress.position.x+4,g.progress.position.z-2};
    };
    CHECK(std::fabs(displacement(DashThreshold,0,1).x-WalkSpeed*1.190000057220459f)<.00001f);
    CHECK(std::fabs(displacement(DashThreshold+.001f,0,1).x-DashSpeed*1.190000057220459f)<.00001f);
    CHECK(std::fabs(displacement(1,0,60).x-1.354275f)<.0001f);
    const auto diagonal=displacement(1,1,1);
    CHECK(std::fabs(std::hypot(diagonal.x,diagonal.z)-DashSpeed*1.190000057220459f)<.00001f);
    CHECK(std::fabs(displacement(0,1,1).z-DashSpeed*1.190000057220459f)<.00001f);
    Game g;Input right;right.x=1;
    advance(g,1);for(int n=0;n<1000;++n)g.tick(right);
    CHECK(walkable(g.progress.position));CHECK(g.progress.position.x<=9.f-BodyRadius);
    Game obstacle;obstacle.progress.position={0,1.2f};Input north;north.z=-1;
    for(int n=0;n<120;++n)obstacle.tick(north);
    CHECK(obstacle.progress.position.z>=.6f+BodyRadius);
    CHECK(!walkable({std::numeric_limits<float>::quiet_NaN(),1}));
    CHECK(!walkable({0,0}));
    Game jump;jump.progress.position={0,2};jump.tick(confirm());advance(jump,4);CHECK(std::fabs(jump.jumpHeight-.22922f)<.00001f);advance(jump,18);CHECK(std::fabs(jump.jumpHeight)<.01f);
    Game follow;follow.progress.position={-3,3};follow.partner={-6,3};advance(follow,100);CHECK(follow.partner.x>-4.1f);CHECK(walkable(follow.partner));
}
void dialogueAndTouch() {
    Game g;g.progress.position=CrumpPosition;g.tick(confirm());CHECK(g.scene==Scene::Dialogue);
    g.tick(confirm());g.tick(confirm());CHECK(g.scene==Scene::Battle);
    Game npc;npc.progress.position=NpcPosition;npc.tick(confirm());CHECK(npc.scene==Scene::Dialogue);
    Input close;close.touchX=20;close.touchY=180;npc.tick(close);CHECK(npc.scene==Scene::Explore);
    Game tabs;Input touch;touch.touchX=240;touch.touchY=55;tabs.tick(touch);CHECK(tabs.menu==Menu::Settings);
    Input y;y.menuConfirm=true;tabs.tick(y);CHECK(!tabs.progress.sound);CHECK(tabs.saveRequested);
}
void timing() {
    // Native level snapshots use the physical producer; abstract scenarios below
    // inject already-produced GP edges directly.
    Game physical;physical.startBattle();Input raw;raw.physicalSnapshot=true;
    raw.physicalButtons=BattleButtonB;physical.tick(raw);
    CHECK(physical.phase==Phase::Select && physical.hammerAction().state==0);
    CHECK(physical.privatePad().triggers.entries[0]==BattleButtonB);
    CHECK(physical.generalPad().triggers.entries[0]==BattleButtonB && physical.generalPad(3).held.entries[0]==0);
    physical.tick(raw);CHECK(physical.privatePad().triggers.entries[0]==0 && physical.privatePad().triggers.entries[1]==BattleButtonB);
    raw.physicalButtons=0;physical.tick(raw);raw.physicalButtons=BattleButtonA;raw.confirm=true;
    physical.tick(raw);CHECK(physical.phase==Phase::Windup && physical.privatePad().triggers.entries[0]==BattleButtonA);
    CHECK(physical.actionHelpRequest()==2 && physical.jumpAction().state==100);
    raw.confirm=false;raw.physicalButtons=0;for(int n=0;n<40;++n)physical.tick(raw);
    raw.physicalButtons=BattleButtonA;physical.tick(raw);
    raw.physicalButtons=0;for(int n=0;n<7;++n)physical.tick(raw);
    CHECK(physical.actionSuccess && physical.lastDamage==1);
    CHECK(physical.actionHelpRequest()==0 && physical.jumpAction().state==1000 && physical.jumpAction().callbacksLive);
    Game continuity;Input continuityHeld;continuityHeld.physicalSnapshot=true;continuityHeld.physicalButtons=BattleButtonA;
    continuity.tick(continuityHeld);continuity.startBattle();continuity.tick(continuityHeld);
    CHECK(continuity.physicalChannel().pressed==0 && continuity.phase==Phase::Select);
    Game ignoredItem;ignoredItem.startBattle();ignoredItem.selection=2;ignoredItem.tick(confirm());
    CHECK(ignoredItem.phase==Phase::Select && ignoredItem.privatePad().triggers.entries[0]==BattleButtonA);
    for(int frame:{1,28,29,40,41,42,43,47,48}) {
        Game g;g.startBattle();g.tick(confirm());advance(g,frame-1);g.tick(confirm());
        advance(g,BattleRules::JumpImpact-frame);
        CHECK(g.lastDamage==1 && g.actionSuccess==(frame>=41));
        CHECK(g.damageCode()==23);
        CHECK(g.enemyBattleState().hp==g.enemyHP && g.playerBattleState().hp==g.progress.hp);
        CHECK(g.enemyBattleState().requestedHp==g.lastDamage && g.playerBattleState().dealtHp==g.lastDamage);
        CHECK(!g.damageRequests().empty() && g.damageRequests().back().kind==DamageServiceRequest::Kind::HitEvent);
    }
    // TimingA rejects two edges within the latest20 samples, even with one on impact.
    for(int first:{29,30,41,47}) {
        Game mash;mash.startBattle();mash.tick(confirm());advance(mash,first-1);
        mash.tick(confirm());advance(mash,47-first);mash.tick(confirm());
        CHECK(mash.lastDamage==1 && !mash.actionSuccess);
    }
    // An older edge lies outside the mash scan and cannot poison a later valid edge.
    for(int first:{1,28}) {
        Game old;old.startBattle();old.tick(confirm());advance(old,first-1);
        old.tick(confirm());advance(old,47-first);old.tick(confirm());CHECK(old.lastDamage==1 && old.actionSuccess);
    }
    // Success is evaluated at impact, not latched on the first in-window edge.
    Game delayed;delayed.startBattle();delayed.tick(confirm());advance(delayed,40);
    delayed.tick(confirm());CHECK(!delayed.actionSuccess && delayed.phase==Phase::Windup);
    advance(delayed,7);CHECK(delayed.actionSuccess && delayed.lastDamage==1);
    // Original ordinary Hammer: enter on tick1, stage5 on81, release82..106.
    for(int release:{2,80,81,82,100,106,107}) {
        Game g;g.startBattle();g.selection=1;g.tick(confirm());
        Input held;held.x=-1;
        for(int n=1;n<release;++n)g.tick(held);
        g.tick({});
        if(release==81) {
            CHECK(g.phase==Phase::Windup && g.hammerAction().stage==5);
            g.tick({}); // Stage-advance call cannot evaluate release.
        }
        CHECK(g.phase==Phase::Impact);CHECK(g.lastDamage==(release>=81 && release<=106?2:1));
        CHECK(g.playerBattleState().flags&8); // Recovered ordinary weapon Charge-use flag gate.
    }
    Game timeout;timeout.startBattle();timeout.selection=1;timeout.tick(confirm());
    advance(timeout,119);CHECK(timeout.phase==Phase::Windup);
    advance(timeout,1);CHECK(timeout.phase==Phase::Impact && timeout.lastDamage==1);
    for(float x:{-30.f/70.f,-30.f/70.f+.001f,0.f,1.f}) {
        Game threshold;threshold.startBattle();threshold.selection=1;threshold.tick(confirm());
        Input held;held.x=x;threshold.tick(held);
        CHECK(threshold.hammerAction().enteredCharge==(x<=-30.f/70.f));
    }
    Game late;late.startBattle();late.selection=1;late.tick(confirm());advance(late,10);
    Input held;held.x=-1;late.tick(held);CHECK(late.hammerAction().stage==1);
    for(int n=0;n<80;++n)late.tick(held);
    late.tick({});CHECK(late.phase==Phase::Impact && late.lastDamage==2);
    auto enemyTurn=[](Game& g) {g.startBattle();attack(g,false);advance(g,42);CHECK(g.phase==Phase::EnemyWindup);};
    for(int frame:{1,39,40,46,47,53,54}) {
        Game g;enemyTurn(g);advance(g,frame-1);g.tick(confirm());
        advance(g,BattleRules::EnemyImpact-frame);
        CHECK(g.phase==Phase::EnemyImpact && g.guarded==(frame>=47));
        CHECK(g.progress.hp==(frame>=47?10:9));
        CHECK(g.defenseResult()==(frame>=47?4:frame>=40?1:2));
        CHECK(g.damageCode()==(frame>=47?(0x40100|30):(0x10100|23)));
        CHECK(g.playerBattleState().hp==g.progress.hp);
        CHECK(g.playerBattleState().requestedHp==g.lastDamage);
    }
    for(int first:{1,39,40,47}) {
        Game g;enemyTurn(g);advance(g,first-1);g.tick(confirm());
        advance(g,53-first);g.tick(confirm());
        CHECK(g.guarded==(first<40) && g.defenseResult()==(first<40?4:1));
    }
    Game simultaneous;enemyTurn(simultaneous);advance(simultaneous,53);
    Input both=confirm();both.cancel=true;simultaneous.tick(both);
    CHECK(!simultaneous.guarded && simultaneous.defenseResult()==1);
    Game bOnly;enemyTurn(bOnly);advance(bOnly,53);Input b;b.cancel=true;bOnly.tick(b);
    CHECK(!bOnly.guarded && bOnly.defenseResult()==1); // Gate0 in this harness.
    Game level;enemyTurn(level);Input aHeld;aHeld.jumpHeld=true;
    for(int n=0;n<54;++n)level.tick(aHeld);
    CHECK(!level.guarded && level.defenseResult()==2); // Held A is not a press edge.
    Game guardPending;enemyTurn(guardPending);advance(guardPending,46);guardPending.tick(confirm());
    CHECK(!guardPending.guarded && guardPending.defenseResult()==0);
    advance(guardPending,7);CHECK(guardPending.guarded && guardPending.defenseResult()==4);
}
void battles() {
    Game g;g.startBattle();
    for(int turn=0;turn<5;++turn) {
        attack(g,true);advance(g,42);
        if(turn==4)break;
        CHECK(g.phase==Phase::EnemyWindup);advance(g,53);g.tick(confirm());CHECK(g.guarded);CHECK(g.progress.hp==10);advance(g,36);
    }
    CHECK(g.scene==Scene::Victory);g.tick(confirm());CHECK(g.scene==Scene::Explore);CHECK(g.progress.crumpDefeated);CHECK(g.saveRequested);
    Game loss;loss.startBattle();loss.progress.hp=1;attack(loss,false);advance(loss,42+54+36);CHECK(loss.scene==Scene::Defeat);
    loss.tick(confirm());CHECK(loss.scene==Scene::Battle);CHECK(loss.progress.hp==10);CHECK(loss.enemyHP==5);
    Game item;item.startBattle();item.selection=2;item.tick(confirm());CHECK(item.phase==Phase::Select);CHECK(item.progress.mushrooms==1);
    item.progress.hp=4;item.tick(confirm());CHECK(item.progress.hp==9);CHECK(item.progress.mushrooms==0);advance(item,42+54+36);CHECK(item.phase==Phase::Select);
}
void saves() {
    Progress p;p.position={2,2};p.crumpDefeated=true;p.hp=7;p.sound=false;
    auto bytes=encodeSave(p);Progress restored;CHECK(decodeSave(bytes,restored));CHECK(restored.hp==7);CHECK(restored.crumpDefeated);CHECK(!restored.sound);
    CHECK(!restored.originalStory); // Prototype victory is not original GSW0.
    for(int32_t story:{0,5,6,7,11,-1,std::numeric_limits<int32_t>::min(),std::numeric_limits<int32_t>::max()}) {
        Progress known=p;known.originalStory=story;
        CHECK(decodeSave(encodeSave(known),restored));CHECK(restored.originalStory==story);
    }
    // Version 1 plaza saves migrate without losing progress.
    auto legacy=encodeSave(p);legacy[4]=1;
    uint32_t sum=2166136261u;for(int n=0;n<28;++n){sum^=legacy[n];sum*=16777619u;}
    for(int n=0;n<4;++n)legacy[28+n]=(sum>>(8*n))&255;
    CHECK(decodeSave(legacy,restored));CHECK(restored.room==0);CHECK(restored.elevation==0);
    CHECK(!restored.originalStory);
    Progress elevated=p;elevated.room=1;elevated.elevation=1.19f;
    CHECK(decodeSave(encodeSave(elevated),restored));CHECK(restored.room==1);CHECK(std::fabs(restored.elevation-1.2f)<.001f);
    elevated.room=4;CHECK(decodeSave(encodeSave(elevated),restored));CHECK(restored.room==4);
    elevated.room=256;CHECK(!decodeSave(encodeSave(elevated),restored));
    bytes[8]^=1;CHECK(!decodeSave(bytes,restored));
    p.position.x=std::numeric_limits<float>::infinity();CHECK(!decodeSave(encodeSave(p),restored));
    p.room=5;CHECK(!decodeSave(encodeSave(p),restored));p.room=0;
    p.position={2,2};p.hp=11;CHECK(!decodeSave(encodeSave(p),restored));p.hp=7;
    char directory[]="/tmp/ttyd-tests-XXXXXX";CHECK(mkdtemp(directory));std::string path=std::string(directory)+"/progress.bin";
    CHECK(loadSave(path,restored)==LoadResult::Missing);
    {std::ofstream old(path,std::ios::binary);old.write(reinterpret_cast<const char*>(legacy.data()),32);}
    CHECK(loadSave(path,restored)==LoadResult::Loaded);CHECK(!restored.originalStory && restored.hp==7);
    CHECK(writeSave(path,p));CHECK(loadSave(path,restored)==LoadResult::Loaded);
    p.hp=4;CHECK(writeSave(path,p));CHECK(loadSave(path,restored)==LoadResult::Loaded);CHECK(restored.hp==4);
    {std::ofstream corrupt(path);corrupt<<"broken";}
    CHECK(loadSave(path,restored)==LoadResult::Recovered);CHECK(restored.hp==7);
    CHECK(!writeSave(std::string(directory)+"/missing/save",p));
    std::remove(path.c_str());std::remove((path+".bak").c_str());rmdir(directory);
}
void roadContracts() {
    RoadCaseAdmission gate;
    CHECK(roadContactMatches(123,123,true,gate));
    CHECK(!roadContactMatches(0,123,true,gate));
    CHECK(!roadContactMatches(124,123,true,gate));
    CHECK(!roadContactMatches(123,123,false,gate));
    gate.systemLevel=1;CHECK(!roadContactMatches(123,123,true,gate));gate.systemLevel=0;
    gate.window=true;CHECK(!gate.permits());gate.window=false;
    gate.globalBlocked=true;CHECK(!gate.permits());gate.globalBlocked=false;
    gate.preventMarioEvent=true;CHECK(!gate.permits());gate.preventMarioEvent=false;
    gate.caseValid=false;CHECK(!gate.permits());
    for(auto callback:{RoadCallback::HarborOut,RoadCallback::PartnerArrival,RoadCallback::CoinArrival}) {
        CHECK(roadCallbackPolicy(callback,std::nullopt)==RoadPolicy::MissingStory);
        CHECK(roadCallbackPolicy(callback,11)==RoadPolicy::Generic);
    }
    CHECK(roadCallbackPolicy(RoadCallback::HarborOut,5)==RoadPolicy::Custom);
    CHECK(roadCallbackPolicy(RoadCallback::PartnerArrival,6)==RoadPolicy::Custom);
    CHECK(roadCallbackPolicy(RoadCallback::CoinArrival,7)==RoadPolicy::Custom);
    CHECK(roadCallbackPolicy(RoadCallback::DebugArrival,std::nullopt)==RoadPolicy::Generic);
    clearCollision();float h=0;
    CHECK(roadStartGround({0,0},.123456f,h) && h==.123456f);
    CHECK(!roadStartGround({0,0},std::numeric_limits<float>::infinity(),h));
}
void collisionImport() {
    const WallTriangle ramp{0,0,0, 2,0,0, 0,2,0};
    CHECK(wallSliceBlocks(ramp,1,.5f,.05f,.1f));
    CHECK(!wallSliceBlocks(ramp,1,1.5f,.05f,.1f)); // Empty upper-right bbox region.
    CHECK(wallSliceBlocks(ramp,0,1.5f,.05f,.1f));
    CHECK(wallSliceBlocks(ramp,2,0,.05f,.1f)); // Single vertex.
    CHECK(!wallSliceBlocks(ramp,2.1f,0,.05f,.1f));
    char path[]="/tmp/ttyd-walls-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=1;Rect wall{0,2,0,3.5f};
    CHECK(write(fd,&count,4)==4);CHECK(write(fd,&wall,sizeof(wall))==sizeof(wall));close(fd);
    CHECK(loadCollision(path));CHECK(!walkable({.05f,2.5f}));CHECK(walkable({1,2.5f}));
    CHECK(!walkable({7,3}));
    auto levelWall=[&](float low,float high) {
        std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000001;
        Vec2 levels{low,high};file.write(reinterpret_cast<char*>(&tagged),4);
        file.write(reinterpret_cast<char*>(&wall),sizeof(wall));file.write(reinterpret_cast<char*>(&levels),sizeof(levels));
        file.close();CHECK(loadCollision(path));
    };
    levelWall(0,.1f);CHECK(walkable({.05f,2.5f},0)); // Original harbor riser height.
    levelWall(0,.3f);CHECK(!walkable({.05f,2.5f},0)); // Taller obstacle remains solid.
    CHECK(!walkable({.099f,2.5f},0));CHECK(walkable({.101f,2.5f},0));
    CHECK(walkable({.05f,2.5f},.3f));
    levelWall(.18f,.22f);CHECK(walkable({.05f,2.5f},0)); // Between the original rays.
    levelWall(.26f,.28f);CHECK(!walkable({.05f,2.5f},0)); // Upper ray only.
    levelWall(1.5f,2);CHECK(walkable({.05f,2.5f},0)); // Overhead geometry.
    for(uint32_t flags:{0u,4u,0x84u,0x80004u,0x800004u}) {
        std::ofstream file(path,std::ios::binary);uint32_t tagged=0xc0000001;
        Vec2 levels{0,2};GroundContact contact{flags,123};
        file.write(reinterpret_cast<char*>(&tagged),4);file.write(reinterpret_cast<char*>(&wall),sizeof(wall));
        file.write(reinterpret_cast<char*>(&levels),sizeof(levels));file.write(reinterpret_cast<char*>(&contact),sizeof(contact));file.close();
        CHECK(loadCollision(path));CHECK(walkable({.05f,2.5f},0)==(flags==4));
        {CollisionTransition rollback;levelWall(0,2);CHECK(!walkable({.05f,2.5f},0));}
        CHECK(walkable({.05f,2.5f},0)==(flags==4));
    }
    {
        std::ofstream file(path,std::ios::binary);uint32_t header=0xe0000001;
        Vec2 levels{0,2};GroundContact contact{0,123};WallTriangle tri{0,0,2,0,0,3.5f,0,2,2};
        file.write(reinterpret_cast<char*>(&header),4);file.write(reinterpret_cast<char*>(&wall),sizeof(wall));
        file.write(reinterpret_cast<char*>(&levels),sizeof(levels));file.write(reinterpret_cast<char*>(&contact),sizeof(contact));
        file.write(reinterpret_cast<char*>(tri.data()),sizeof(tri));file.close();
        CHECK(loadCollision(path));CHECK(walkable({.05f,3.3f},1));CHECK(!walkable({.05f,2.5f},1));
        std::ifstream original(path,std::ios::binary);std::string packed{std::istreambuf_iterator<char>(original),std::istreambuf_iterator<char>()};
        {CollisionTransition rollback;levelWall(0,2);CHECK(!walkable({.05f,3.3f},1));}
        CHECK(walkable({.05f,3.3f},1)); // Triangle geometry restored with the room.
        {std::ofstream restore(path,std::ios::binary);restore.write(packed.data(),packed.size());}
    }
    {std::fstream file(path,std::ios::binary|std::ios::in|std::ios::out);float wrongHeight=.5f;file.seekp(20);file.write(reinterpret_cast<char*>(&wrongHeight),4);}
    CHECK(!loadCollision(path));CHECK(walkable({.05f,3.3f},1));
    // Incomplete triangle metadata must not replace the last valid collision pack.
    CHECK(truncate(path,35)==0);CHECK(!loadCollision(path));CHECK(!walkable({.05f,2.5f},0));
    clearCollision();std::remove(path);
    CHECK(!loadCollision("/does/not/exist"));
}
void groundImport() {
    {
        char path[]="/tmp/ttyd-floor-faces-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);close(fd);
        auto writeFaces=[&](uint32_t roofCull,bool reverse=false) {
            float floor[]={-1,0,1,-1,0,4,1,0,1};
            float roof[]={-1,.2f,1,1,.2f,1,-1,.2f,4};
            if(reverse)for(int n=0;n<3;++n)std::swap(roof[3+n],roof[6+n]);
            std::ofstream out(path,std::ios::binary);uint32_t count=0xc0000002,front=1;
            out.write(reinterpret_cast<char*>(&count),4);
            GroundContact low{0,1},high{0,2};
            out.write(reinterpret_cast<char*>(floor),36);out.write(reinterpret_cast<char*>(&low),8);out.write(reinterpret_cast<char*>(&front),4);
            out.write(reinterpret_cast<char*>(roof),36);out.write(reinterpret_cast<char*>(&high),8);out.write(reinterpret_cast<char*>(&roofCull),4);
        };
        float h=99;GroundContact contact;
        writeFaces(1);CHECK(loadGround(path));CHECK(groundAt({-.5f,2},0,h,&contact));CHECK(h==0 && contact.sourceNode==1);
        CHECK(roadStartGround({-.5f,2},-.1f,h) && h==-.1f); // Ray never starts above the marker.
        CHECK(roadStartGround({-.5f,2},.25f,h) && h==0); // One-sided roof is culled.
        {CollisionTransition rollback;writeFaces(3);CHECK(loadGround(path));CHECK(groundAt({-.5f,2},0,h) && std::fabs(h-.2f)<.000001f);}
        CHECK(groundAt({-.5f,2},0,h) && h==0); // Cull metadata restored by rollback.
        writeFaces(1,true);CHECK(loadGround(path));CHECK(groundAt({-.5f,2},0,h) && std::fabs(h-.2f)<.000001f);
        writeFaces(3);CHECK(loadGround(path));CHECK(groundAt({-.5f,2},0,h) && std::fabs(h-.2f)<.000001f);
        CHECK(roadStartGround({-.5f,2},.199f,h) && h==0);
        CHECK(roadStartGround({-.5f,2},.25f,h) && std::fabs(h-.2f)<.000001f);
        writeFaces(9);CHECK(!loadGround(path));CHECK(groundAt({-.5f,2},0,h) && std::fabs(h-.2f)<.000001f);
        CHECK(truncate(path,99)==0);CHECK(!loadGround(path));CHECK(groundAt({-.5f,2},0,h) && std::fabs(h-.2f)<.000001f);
        clearCollision();std::remove(path);
    }
    {
        char slopes[]="/tmp/ttyd-slopes-XXXXXX";int fd=mkstemp(slopes);CHECK(fd>=0);close(fd);
        for(float angle:{49.7f,50.1f}) {
            float slope=std::tan(angle*3.14159265358979323846f/180.f);
            float tri[]={-1,-slope,1,1,slope,1,-1,-slope,4};uint32_t count=1;
            {std::ofstream file(slopes,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);file.write(reinterpret_cast<char*>(tri),36);}
            CHECK(loadGround(slopes));float h=99;
            CHECK(groundAt({-.5f,2},-.5f*slope,h)==(angle<50));
            FieldSlope record;
            const auto support=searchFieldSupport({-.5f,2},{1,0},-.5f*slope,0,record);
            CHECK(record.tiltX!=0 && record.tiltZ==0);
            if(angle>50)CHECK(!support.found && record.inclination==0);
            float forward=0,frontAngle=0;
            CHECK(frontStepGround({-.5f,2},{1,0},-.5f*slope,forward,frontAngle));
            CHECK(frontAngle>49); // Front has no foot query's 50-degree cone gate.
        }
        // A steep nearest surface rejects the ray; a lower floor must not win instead.
        float tris[]={-1,-1.53205f,1,1,1.93205f,1,-1,-1.53205f,4,
            -1,0,1,1,0,1,-1,0,4};uint32_t count=2;
        {std::ofstream file(slopes,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);file.write(reinterpret_cast<char*>(tris),72);}
        CHECK(loadGround(slopes));float h=99;CHECK(!groundAt({0,1.5f},0,h));CHECK(h==99);
        clearCollision();std::remove(slopes);
    }
    char path[]="/tmp/ttyd-ground-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=2;
    // Sloping floor, plus a roof that must not pull the actor upward.
    float triangles[18]={-4,0,1, 4,1,1, -4,0,4, -4,3,1, 4,3,1, -4,3,4};
    CHECK(write(fd,&count,4)==4);CHECK(write(fd,triangles,sizeof(triangles))==sizeof(triangles));close(fd);
    CHECK(loadGround(path));float h=0;
    CHECK(groundAt({-3,2},0,h));CHECK(std::fabs(h-.125f)<.001f);
    GroundContact legacy{123,456};CHECK(groundAt({-3,2},0,h,&legacy));CHECK(legacy.attributes==0 && legacy.sourceNode==0);
    CHECK(!groundAt({std::numeric_limits<float>::quiet_NaN(),2},0,h));
    CHECK(!groundAt({8,2},0,h));CHECK(!groundAt({0,2},0,h));
    CHECK(groundAt({0,2},.3f,h));CHECK(std::fabs(h-.5f)<.001f);
    CHECK(groundAt({-3,2},3,h));CHECK(h==3);
    // A falling world-Y ray finds support within this step, not a distant roof.
    GroundContact landing;
    CHECK(landingGround({-3,2},.2f,-.08f,h,&landing));CHECK(std::fabs(h-.125f)<.001f);
    CHECK(landingGround({-3,2},.1358f,-.01f,h));CHECK(std::fabs(h-.125f)<.00001f);
    // SearchGround's .1-engine-unit ray tail admits this sub-two-unit drop.
    h=99;landing={123,456};CHECK(!landingGround({-3,2},.3f,-.08f,h,&landing));
    CHECK(h==99 && landing.sourceNode==456);
    CHECK(landingGround({-3,2},2.95f,0,h));CHECK(h==3);
    CHECK(!landingGround({-3,2},2.94f,0,h));
    CHECK(!landingGround({-3,2},.2f,std::numeric_limits<float>::quiet_NaN(),h));
    Game g;g.progress.position={-3,2};g.restore(g.progress);CHECK(std::fabs(g.groundHeight-.125f)<.001f);
    Input right;right.x=1;for(int n=0;n<35;++n)g.tick(right);
    CHECK(g.progress.position.x>-2.3f);CHECK(g.groundHeight>.2f);
    {
        std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000002;
        file.write(reinterpret_cast<char*>(&tagged),4);
        for(int n=0;n<2;++n) {
            GroundContact contact{uint32_t(0x200+n),uint32_t(100+n)};
            file.write(reinterpret_cast<char*>(triangles+n*9),36);
            file.write(reinterpret_cast<char*>(&contact),8);
        }
    }
    CHECK(loadGround(path));GroundContact contact;
    CHECK(groundAt({-3,2},0,h,&contact));CHECK(contact.attributes==0x200 && contact.sourceNode==100);
    CHECK(groundAt({-3,2},3,h,&contact));CHECK(contact.attributes==0x201 && contact.sourceNode==101);
    CHECK(!groundAt({8,2},0,h,&contact));CHECK(contact.attributes==0x201 && h==3);
    {std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000002;file.write(reinterpret_cast<char*>(&tagged),4);}
    CHECK(!loadGround(path));CHECK(groundAt({-3,2},0,h,&contact));CHECK(contact.attributes==0x200 && contact.sourceNode==100);
    for(uint32_t flags:{4u,0x80004u,0x84u,0x800004u}) {
        {
            std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000001;
            GroundContact surface{flags,100};file.write(reinterpret_cast<char*>(&tagged),4);
            file.write(reinterpret_cast<char*>(triangles),36);file.write(reinterpret_cast<char*>(&surface),8);
        }
        CHECK(loadGround(path));CHECK(groundAt({-3,2},0,h)==(flags!=4));
    }
    clearCollision();std::remove(path);
}
void unequalFloorMotion() {
    char path[]="/tmp/ttyd-unequal-floor-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=0x80000004;
    const float surfaces[4][9]={{-2,1,1,0,1,1,-2,1,4},{0,1,1,0,1,4,-2,1,4},
        {0,0,1,3,0,1,0,0,4},{3,0,1,3,0,4,0,0,4}};
    CHECK(write(fd,&count,4)==4);
    for(int n=0;n<4;++n){GroundContact contact{0,uint32_t(n<2?1:2)};CHECK(write(fd,surfaces[n],36)==36);CHECK(write(fd,&contact,8)==8);}
    close(fd);CHECK(loadGround(path));
    float floor=99;GroundContact support;
    CHECK(landingFootprint({.06f,2},{1,0},1,0,floor,&support));CHECK(floor==1 && support.sourceNode==1);
    CHECK(!landingFootprint({.06f,2},{1,1},1,0,floor)); // Rotated footprint has crossed the edge.
    CHECK(!landingFootprint({.08f,2},{1,0},1,0,floor));
    CHECK(!landingFootprint({0,2},{0,0},1,0,floor));
    Progress p;p.room=2;p.position={-.08f,2};p.elevation=1;
    Input right;right.x=1;right.jumpHeld=true;Input runningJump=right;runningJump.confirm=true;
    Game jump;jump.restore(p);jump.tick(right);jump.tick(runningJump);
    for(int n=0;n<8;++n)jump.tick(right);
    CHECK(jump.progress.position.x>.075f && jump.airborne());
    CHECK(jump.groundHeight+jump.jumpHeight>1.3f); // World height does not fall with support.
    CHECK(jump.groundContact.sourceNode==1 && jump.previousGroundContact.sourceNode==1);
    // Jump/apex retains support; the active fall owner clears it before querying.
    for(int n=0;n<12;++n)jump.tick(right);
    for(int n=0;n<100 && jump.airborne();++n)jump.tick({});
    CHECK(!jump.airborne() && jump.groundHeight==0 && jump.groundContact.sourceNode==2);
    CHECK(jump.previousGroundContact.sourceNode==0 && jump.groundMetadataHeight==1);
    CHECK(jump.fieldAirMotion().pendingEntry && jump.fieldAirMotion().falling);
    CHECK(jump.fieldAirMotion().ticks==0 && jump.fieldAirMotion().velocity==0);
    jump.tick({});CHECK(!jump.fieldAirMotion().hasAirFlags() && jump.fieldAirMotion().takeoffY==0);
    Game fall;fall.restore(p);right.jumpHeld=false;
    for(int n=0;n<10;++n)fall.tick(right);
    CHECK(fall.airborne() && fall.groundHeight+fall.jumpHeight<1 && fall.groundContact.sourceNode==0);
    Input save;save.menuConfirm=true;fall.tick(save);CHECK(!fall.saveRequested);
    for(int n=0;n<100 && fall.airborne();++n)fall.tick({});
    CHECK(!fall.airborne() && fall.groundHeight==0 && fall.groundContact.sourceNode==2);
    Game belowTakeoff;belowTakeoff.restore(p);
    for(int n=0;n<40 && belowTakeoff.groundHeight+belowTakeoff.jumpHeight>.85f;++n)belowTakeoff.tick(right);
    const float rootY=belowTakeoff.groundHeight+belowTakeoff.jumpHeight;
    CHECK(belowTakeoff.airborne() && rootY<.85f && rootY>.7f);
    char headPath[]="/tmp/ttyd-low-fall-head-XXXXXX";int headFd=mkstemp(headPath);CHECK(headFd>=0);
    uint32_t heads=1,attrs=0,node=999,cull=1;float roof=rootY+.365f;
    float tri[]={-2,roof,1,3,roof,1,-2,roof,4};
    CHECK(write(headFd,"HEAD",4)==4);CHECK(write(headFd,&heads,4)==4);CHECK(write(headFd,tri,36)==36);
    CHECK(write(headFd,&attrs,4)==4);CHECK(write(headFd,&node,4)==4);CHECK(write(headFd,&cull,4)==4);
    close(headFd);Game unblocked=belowTakeoff;unblocked.tick({});
    CHECK(loadHead(headPath));belowTakeoff.tick({});CHECK(belowTakeoff.overheadContact);
    CHECK(std::fabs(belowTakeoff.jumpHeight-unblocked.jumpHeight)<.00001f);
    CHECK(belowTakeoff.groundHeight+belowTakeoff.jumpHeight<rootY); // No teleport to takeoff1.
    for(int n=0;n<100 && belowTakeoff.airborne();++n)belowTakeoff.tick({});
    CHECK(!belowTakeoff.airborne() && belowTakeoff.groundHeight==0);std::remove(headPath);
    // Remove the injected head surface before testing unrelated higher-platform motion.
    {std::ofstream empty(headPath,std::ios::binary);empty.write("HEAD",4);uint32_t zero=0;empty.write(reinterpret_cast<char*>(&zero),4);}
    CHECK(loadHead(headPath));std::remove(headPath);
    // A reachable higher platform is acquired on descent, not during takeoff.
    {
        std::ofstream file(path,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<4;++n){float triangle[9];std::copy(surfaces[n],surfaces[n]+9,triangle);
            if(n<2)for(int vertex=1;vertex<9;vertex+=3)triangle[vertex]=.2f;
            GroundContact contact{0,uint32_t(n<2?1:2)};file.write(reinterpret_cast<char*>(triangle),36);file.write(reinterpret_cast<char*>(&contact),8);}
    }
    CHECK(loadGround(path));p.position={.08f,2};p.elevation=0;
    Input left;left.x=-1;left.jumpHeld=true;runningJump=left;runningJump.confirm=true;
    Game higher;higher.restore(p);higher.tick(left);higher.tick(runningJump);
    for(int n=0;n<8;++n)higher.tick(left);
    CHECK(higher.airborne() && higher.groundHeight==0 && higher.jumpHeight>.3f);
    for(int n=0;n<8;++n)higher.tick(left);
    for(int n=0;n<100 && higher.airborne();++n)higher.tick({});
    CHECK(!higher.airborne() && std::fabs(higher.groundHeight-.2f)<.0001f && higher.groundContact.sourceNode==1);
    for(int hp:{7,1}) {
        p.position={2.99f,2};p.elevation=0;p.hp=hp;
        Game voided;voided.restore(p);right.jumpHeld=false;
        for(int n=0;n<20;++n)voided.tick(right);
        float before=0;
        for(int n=0;n<400 && !voided.water.active();++n){before=voided.groundHeight+voided.jumpHeight;voided.tick({});}
        CHECK(voided.water.active() && voided.water.kind==RecoveryKind::Void);
        CHECK(before>VoidThreshold && before<=VoidThreshold+.181f);
        CHECK(!voided.airborne() && voided.progress.hp==hp && !voided.saveRequested);
        Input ignored;ignored.x=-1;ignored.confirm=true;ignored.menuConfirm=true;
        for(int n=0;n<500 && voided.water.active();++n){voided.tick(ignored);CHECK(!voided.saveRequested);}
        CHECK(!voided.water.active() && voided.progress.hp==hp-1);
        CHECK(std::fabs(voided.progress.position.x-p.position.x)<.08f && voided.groundHeight==0);
        if(hp==1){CHECK(voided.scene==Scene::FieldDefeat);voided.tick(confirm());CHECK(voided.reloadRequested);}
        else {CHECK(voided.scene==Scene::Explore);CHECK(voided.partner.x==voided.progress.position.x && voided.partnerHeight==0);}
    }
    // Directional around correction keeps the body clear of this nearby wall;
    // the corrected position retains the upper ledge's eligible foot support.
    char wallPath[]="/tmp/ttyd-landing-wall-XXXXXX";int wallFd=mkstemp(wallPath);CHECK(wallFd>=0);
    uint32_t wallCount=0x80000001;Rect wall{.145f,1,.145f,4};Vec2 levels{0,.3f};
    CHECK(write(wallFd,&wallCount,4)==4);CHECK(write(wallFd,&wall,16)==16);CHECK(write(wallFd,&levels,8)==8);close(wallFd);
    CHECK(loadCollision(wallPath));p.position={-.08f,2};p.elevation=.2f;
    Game overlap;overlap.restore(p);right.jumpHeld=false;
    for(int n=0;n<10;++n)overlap.tick(right);
    for(int n=0;n<100 && overlap.airborne();++n)overlap.tick({});
    CHECK(!overlap.airborne() && std::fabs(overlap.groundHeight-.2f)<.00001f && overlap.groundContact.sourceNode==1);
    CHECK(overlap.progress.position.x<.0451f && overlap.progress.position.x>0);
    std::remove(wallPath);
    clearCollision();
    {std::ofstream file(path,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<4;++n){float tri[9];std::copy(surfaces[n],surfaces[n]+9,tri);
            if(n<2)for(int v=1;v<9;v+=3)tri[v]=3.14f;
            GroundContact owner{0,uint32_t(n<2?1:2)};
            file.write(reinterpret_cast<char*>(tri),36);file.write(reinterpret_cast<char*>(&owner),8);}}
    CHECK(loadGround(path));p.position={-.08f,2};p.elevation=3.14f;p.hp=7;
    Game fast;fast.restore(p);
    for(int n=0;n<10;++n)fast.tick(right);
    for(int n=0;n<100 && fast.airborne();++n)fast.tick({});
    CHECK(!fast.airborne() && fast.groundHeight==0 && fast.groundContact.sourceNode==2);
    CHECK(!fast.water.active() && fast.progress.hp==7); // Terminal-speed step reaches the lower floor.
    auto stepFloor=[&](float top) {
        std::ofstream file(path,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<4;++n){float tri[9];std::copy(surfaces[n],surfaces[n]+9,tri);
            if(n<2)for(int v=1;v<9;v+=3)tri[v]=top;
            GroundContact owner{0,uint32_t(n<2?1:2)};file.write(reinterpret_cast<char*>(tri),36);file.write(reinterpret_cast<char*>(&owner),8);}
    };
    stepFloor(.08f);CHECK(loadGround(path));p.position={.01f,2};p.elevation=0;
    float forward=99,slope=99;
    CHECK(frontStepGround({.05f,2},{-1,0},0,forward,slope));CHECK(std::fabs(forward-.08f)<.00001f && slope==0);
    CHECK(frontStepGround({.054f,2},{-1,0},0,forward,slope));CHECK(forward==0);
    CHECK(frontStepGround({.05f,2},{1,0},0,forward,slope));CHECK(forward==0);
    forward=99;slope=99;CHECK(!frontStepGround({.05f,2},{0,0},0,forward,slope));CHECK(forward==99 && slope==99);
    Progress early=p;early.position={.07f,2};Game anticipate;anticipate.restore(early);left.jumpHeld=false;anticipate.tick(left);
    CHECK(anticipate.airborne() && anticipate.progress.position.x>0 && anticipate.groundHeight==0);
    for(int n=0;n<3;++n)anticipate.tick(left);CHECK(!anticipate.airborne() && std::fabs(anticipate.groundHeight-.08f)<.00001f);
    char stairHead[]="/tmp/ttyd-stair-head-XXXXXX";int stairFd=mkstemp(stairHead);CHECK(stairFd>=0);
    uint32_t stairHeads=1,stairCull=1;GroundContact ceilingOwner{0,1000};float ceilingTri[]={-2,.35f,1,3,.35f,1,-2,.35f,4};
    CHECK(write(stairFd,"HEAD",4)==4);CHECK(write(stairFd,&stairHeads,4)==4);CHECK(write(stairFd,ceilingTri,36)==36);
    CHECK(write(stairFd,&ceilingOwner,8)==8);CHECK(write(stairFd,&stairCull,4)==4);close(stairFd);CHECK(loadHead(stairHead));
    Game stairs;stairs.restore(p);left.jumpHeld=false;stairs.tick(left);
    CHECK(stairs.airborne() && stairs.groundHeight==0);
    stairs.tick(save);CHECK(stairs.overheadContact && !stairs.saveRequested && std::fabs(stairs.groundHeight-.08f/3)<.00001f);
    stairs.tick(confirm());CHECK(stairs.airborne() && std::fabs(stairs.groundHeight-.16f/3)<.00001f);
    stairs.tick({});CHECK(!stairs.airborne() && std::fabs(stairs.groundHeight-.08f)<.00001f);
    // First two rises retain the admitted travel even on release/reversal.
    Game retain;retain.restore(p);retain.tick(left);const float entryX=retain.progress.position.x;
    const auto entryOwner=retain.groundContact.sourceNode;
    retain.tick({});CHECK(std::fabs(retain.progress.position.x-entryX+.0225f)<.00001f);
    CHECK(retain.groundContact.sourceNode==entryOwner);
    Input reverse;reverse.x=1;reverse.confirm=true;
    const float secondX=retain.progress.position.x;retain.tick(reverse);
    CHECK(std::fabs(retain.progress.position.x-secondX+.0225f)<.00001f && retain.airborne());
    const float thirdX=retain.progress.position.x;retain.tick(right);
    CHECK(!retain.airborne() && retain.progress.position.x>thirdX); // Third call reclassifies input.
    {std::ofstream empty(stairHead,std::ios::binary);empty.write("HEAD",4);uint32_t zero=0;empty.write(reinterpret_cast<char*>(&zero),4);}
    CHECK(loadHead(stairHead));std::remove(stairHead);
    Game stepJump;stepJump.restore(p);stepJump.tick(left);stepJump.tick(confirm());stepJump.tick(confirm());
    CHECK(std::fabs(stepJump.groundHeight-.16f/3)<.00001f);
    stepJump.tick(confirm());CHECK(stepJump.airborne() && stepJump.jumpHeight==0);
    stepJump.tick({});CHECK(stepJump.jumpHeight>0); // Third-call A entered jump, rather than another stair.
    stepFloor(.09f);CHECK(loadGround(path));Game walkOff;walkOff.restore(p);walkOff.tick(left);
    walkOff.tick(left);walkOff.tick(left);walkOff.tick(left);
    CHECK(!walkOff.airborne() && std::fabs(walkOff.groundHeight-.09f)<.00001f);
    Game idleOff=walkOff;
    // Isolate the retained +7c owner with a replacement support surface. This
    // is a controller fixture, not a claim about original moving platforms.
    {std::ofstream file(path,std::ios::binary);file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<4;++n){float tri[9];std::copy(surfaces[n],surfaces[n]+9,tri);
            for(int v=1;v<9;v+=3)tri[v]=.065f;
            GroundContact owner{0,uint32_t(n+1)};file.write(reinterpret_cast<char*>(tri),36);file.write(reinterpret_cast<char*>(&owner),8);}}
    CHECK(loadGround(path));walkOff.tick(left);idleOff.tick({});
    CHECK(!walkOff.airborne() && std::fabs(walkOff.groundHeight-.065f)<.00001f);
    CHECK(idleOff.airborne()); // Idle entry clears vertical velocity; walk entry retains .03.
    stepFloor(.2f);CHECK(loadGround(path));Game tall;tall.restore(p);tall.tick(left);
    CHECK(tall.progress.position.x<p.position.x && tall.groundHeight==0 && !tall.airborne());
    // This fixture has no tall-ledge wall: do not invent a center-height XZ veto.
    CHECK(!frontStepGround({.05f,2},{-1,0},0,forward,slope)); // .2 is above half-height ray origin.
    stepFloor(.08f);CHECK(loadGround(path));p.position={-.01f,2};p.elevation=.08f;
    Game down;down.restore(p);for(int n=0;n<5;++n)down.tick(right);
    CHECK(down.airborne() && std::fabs(down.groundHeight-.08f)<.00001f);
    Game lowerNeutral=down,lowerReverse=down;lowerNeutral.tick({});lowerReverse.tick(left);
    CHECK(std::fabs(lowerNeutral.progress.position.x-lowerReverse.progress.position.x)<.00001f);
    for(int n=0;n<100 && down.airborne();++n)down.tick({});CHECK(!down.airborne() && down.groundHeight==0);
    stepFloor(.02f);CHECK(loadGround(path));float supported=99;GroundContact owner;
    FieldSlope cameraSlope;
    const auto peripheral=searchFieldSupport({.06f,2},{1,0},0,0,cameraSlope);
    CHECK(peripheral.found && std::fabs(peripheral.point.x-(-.015f))<.00001f && peripheral.point.z==2);
    CHECK(std::fabs(peripheral.point.y-.02f)<.00001f); // Accepted peripheral ray, not root XYZ.
    CHECK(groundedFootprint({.06f,2},{1,0},0,supported,&owner));CHECK(std::fabs(supported-.02f)<.00001f && owner.sourceNode==1);
    CHECK(groundedFootprint({.06f,2},{1,1},0,supported,&owner));CHECK(supported==0 && owner.sourceNode==2);
    supported=99;CHECK(!groundedFootprint({.06f,2},{0,0},0,supported));CHECK(supported==99);
    p.position={.03f,2};p.elevation=0;Game edge;edge.restore(p);CHECK(edge.groundHeight==0);
    CHECK(edge.marioCameraCollision.x==0 && edge.marioCameraCollision.y==0 && edge.marioCameraCollision.z==0);
    edge.tick({});CHECK(std::fabs(edge.groundHeight-.02f)<.00001f && edge.groundContact.sourceNode==1);
    CHECK(std::fabs(edge.marioCameraCollision.x-(-4.5f))<.001f && edge.marioCameraCollision.z==200);
    CHECK(std::fabs(edge.marioCameraCollision.y-2)<.001f && !(edge.fieldCameraDisplayFlags&0x40000));
    CHECK(edge.fieldCameraCollisionTarget.x==0 && edge.fieldCameraCollisionTarget.y==1); // Camera read preceding contact.
    edge.tick({});CHECK(std::fabs(edge.fieldCameraCollisionTarget.x-(-4.5f))<.001f && std::fabs(edge.fieldCameraCollisionTarget.y-3)<.001f);
    const auto contactBeforeMiss=edge.marioCameraCollision;edge.progress.position={4,2};edge.tick({});
    CHECK(edge.marioCameraCollision.x==contactBeforeMiss.x && edge.marioCameraCollision.y==contactBeforeMiss.y && edge.marioCameraCollision.z==contactBeforeMiss.z);
    stepFloor(.049f);CHECK(loadGround(path));CHECK(groundedFootprint({.06f,2},{1,0},0,supported));CHECK(std::fabs(supported-.049f)<.00001f);
    stepFloor(.051f);CHECK(loadGround(path));CHECK(groundedFootprint({.06f,2},{1,0},0,supported));CHECK(supported==0);
    FieldSlope angles;
    auto lower=searchFieldSupport({1,2},{1,0},.03f,0,angles);
    CHECK(!lower.found && std::fabs(lower.delta-.03f)<.00001f && lower.height==.03f);
    auto nearDrop=searchFieldSupport({1,2},{1,0},.01f,0,angles);
    CHECK(nearDrop.found && nearDrop.height==0 && nearDrop.delta==0);
    CHECK(nearDrop.point.x==1 && nearDrop.point.z==2); // Equal-height later feet retain center's winning point.
    supported=99;CHECK(landingFootprint({1,2},{1,0},.0108f,-.01f,supported));CHECK(supported==0);
    auto retainedVertical=searchFieldSupport({1,2},{1,0},.03f,.04f,angles);
    CHECK(retainedVertical.found && retainedVertical.height==0);
    auto miss=searchFieldSupport({4,2},{1,0},.03f,0,angles);
    CHECK(!miss.found && miss.delta<0 && angles.inclination==0);
    auto independentDelta=searchFieldSupport({-.01f,2},{1,0},.051f,0,angles);
    CHECK(independentDelta.found && std::fabs(independentDelta.height-.051f)<.00001f);
    CHECK(independentDelta.delta>0); // Later rejected lower probe writes delta without replacing winning node.
    clearCollision();std::remove(path);
}
void visibility() {
    Point3 eye{0,1,4},actor{0,1,0};
    Point3 a{-1,0,2},b{1,0,2},c{0,2,2};
    CHECK(segmentHitsTriangle(eye,actor,a,b,c));CHECK(segmentHitsTriangle(eye,actor,c,b,a));
    CHECK(segmentHitsTriangle(actor,eye,a,b,c));
    CHECK(!segmentHitsTriangle({3,1,4},{3,1,0},a,b,c));
    CHECK(!segmentHitsTriangle(eye,{0,1,3},a,b,c));
    CHECK(!segmentHitsTriangle({0,1,2},actor,a,b,c)); // At-camera contact.
    CHECK(!segmentHitsTriangle(eye,{0,1,2},a,b,c)); // At-actor contact.
    CHECK(!segmentHitsTriangle({0,1,2},{1,1,2},a,b,c)); // Parallel.
    CHECK(!segmentHitsTriangle(eye,actor,a,a,a));
    Bounds3 bounds{{-1,0,2},{1,2,2}};
    CHECK(segmentHitsBounds(eye,actor,bounds));CHECK(segmentHitsBounds(actor,eye,bounds));
    CHECK(!segmentHitsBounds(eye,{0,1,3},bounds));
    CHECK(!segmentHitsBounds({3,1,4},{3,1,0},bounds));
    CHECK(segmentHitsBounds({0,1,2},{0,1,2},bounds));
    CHECK(!segmentHitsBounds(eye,actor,{{1,2,3},{0,1,2}}));
    auto nan=std::numeric_limits<float>::quiet_NaN();
    CHECK(!segmentHitsBounds({nan,1,4},actor,bounds));
    CHECK(!segmentHitsTriangle({nan,1,4},actor,a,b,c));
}
void collisionRollback() {
    char directory[]="/tmp/ttyd-transition-XXXXXX";CHECK(mkdtemp(directory));
    std::string source=std::string(directory)+"/source",target=std::string(directory)+"/target";
    auto writeWalls=[](const std::string& prefix,float x) {
        std::ofstream file(prefix+".walls",std::ios::binary);uint32_t count=1;Rect wall{x,1,x,3};
        file.write(reinterpret_cast<char*>(&count),4);file.write(reinterpret_cast<char*>(&wall),sizeof(wall));
    };
    auto writeFloor=[](const std::string& prefix,uint32_t attributes) {
        std::ofstream head(prefix+".head",std::ios::binary);head.write("HEAD",4);uint32_t empty=0;
        head.write(reinterpret_cast<char*>(&empty),4);
        std::ofstream file(prefix+".ground",std::ios::binary);uint32_t count=0x80000002;
        float triangles[]={-3,0,1, 3,0,1, -3,0,3, 3,0,1, 3,0,3, -3,0,3};
        file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<2;++n) {
            GroundContact contact{attributes,uint32_t(100+n)};
            file.write(reinterpret_cast<char*>(triangles+n*9),36);file.write(reinterpret_cast<char*>(&contact),8);
        }
        std::ofstream returns(prefix+".returns",std::ios::binary);returns.write("RPOS",4);uint32_t version=1,bindings=2;
        returns.write(reinterpret_cast<char*>(&version),4);returns.write(reinterpret_cast<char*>(&bindings),4);
        for(uint32_t node=100;node<102;++node) {
            ReturnPoint point{attributes/512.f,0,2};returns.write(reinterpret_cast<char*>(&node),4);
            returns.write(reinterpret_cast<char*>(&point),sizeof(point));
        }
    };
    writeWalls(source,0);writeFloor(source,0x200);writeWalls(target,2);
    {CollisionTransition tx;CHECK(tx.load(source.c_str()));tx.commit();}
    const Rect originalBounds=collisionBounds();float h=99;
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));CHECK(groundAt({1,2},0,h));CHECK(h==0);
    GroundContact contact;CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x40000200);
    ReturnPoint point;CHECK(groundReturn(contact,point));CHECK(point.x==1);
    {std::ofstream bad(target+".ground");bad<<"truncated";}
    {
        CollisionTransition tx;CHECK(!tx.load(target.c_str()));tx.commit();
        // Rollback must work even if the previous room cannot be read again.
        std::remove((source+".walls").c_str());std::remove((source+".ground").c_str());
    }
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));CHECK(groundAt({1,2},0,h));CHECK(h==0);
    CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x40000200);
    CHECK(groundReturn(contact,point));CHECK(point.x==1);
    CHECK(collisionBounds().x0==originalBounds.x0);CHECK(collisionBounds().z1==originalBounds.z1);
    writeFloor(target,0x400);
    {std::ofstream bad(target+".returns");bad<<"bad";}
    {CollisionTransition tx;CHECK(!tx.load(target.c_str()));}
    CHECK(groundAt({1,2},0,h,&contact));CHECK(groundReturn(contact,point));CHECK(point.x==1);
    writeFloor(target,0x400);
    {
        CollisionTransition tx;CHECK(tx.load(target.c_str()));CHECK(walkable({0,2}));CHECK(!walkable({2,2}));
        // Model a rejected arrival or graphics load after collision staging succeeds.
        CHECK(!groundAt({20,2},0,h));
        {CollisionTransition nested;CHECK(!nested.load((source+"-missing").c_str()));}
        CHECK(!walkable({2,2}));
    }
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));
    CHECK(groundAt({1,2},0,h,&contact));CHECK(groundReturn(contact,point));CHECK(point.x==1);
    {CollisionTransition tx;CHECK(tx.load(target.c_str()));tx.commit();}
    CHECK(walkable({0,2}));CHECK(!walkable({2,2}));CHECK(groundAt({1,2},0,h));
    CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x40000400);
    CHECK(groundReturn(contact,point));CHECK(point.x==2);
    clearCollision();std::remove((target+".walls").c_str());std::remove((target+".ground").c_str());std::remove((source+".returns").c_str());std::remove((target+".returns").c_str());std::remove((source+".head").c_str());std::remove((target+".head").c_str());rmdir(directory);
}
void uvAnimation() {
    UVTrack track;track.frames={{{1,0,0,0,1,0}},{{1,0,.5f,0,1,.25f}},{{2,0,0,0,3,0}}};
    UVBinding binding{0,.2f,.3f};
    auto a=animatedUV(track,binding,0),b=animatedUV(track,binding,1),c=animatedUV(track,binding,3);
    CHECK(std::fabs(a[0]-.2f)<.0001f && std::fabs(a[1]-.7f)<.0001f);
    CHECK(std::fabs(b[0]-.7f)<.0001f && std::fabs(b[1]-.45f)<.0001f);CHECK(a==c);
    char filename[]="/tmp/ttyd-uv-XXXXXX";int fd=mkstemp(filename);CHECK(fd>=0);close(fd);
    auto write=[&](uint32_t version,uint32_t frames,uint32_t vertex,float value,bool extra) {
        std::ofstream f(filename,std::ios::binary);f.write("UVAN",4);uint32_t count=1,bindings=1;
        f.write((char*)&version,4);f.write((char*)&count,4);f.write((char*)&frames,4);f.write((char*)&bindings,4);
        std::array<float,6> matrix{{1,0,value,0,1,0}};f.write((char*)matrix.data(),24);
        UVBinding item{vertex,.2f,.3f};f.write((char*)&item,sizeof(item));if(extra)f.put('x');
    };
    std::vector<UVTrack> loaded;
    write(1,1,0,0,false);CHECK(loadUVAnimation(filename,1,loaded));CHECK(loaded.size()==1);
    for(int mode=0;mode<5;++mode) {
        write(mode==0?2:1,mode==1?0:1,mode==2?1:0,mode==3?std::numeric_limits<float>::quiet_NaN():0,mode==4);
        CHECK(!loadUVAnimation(filename,1,loaded));CHECK(loaded.size()==1);CHECK(loaded[0].frames.size()==1);
    }
    std::remove(filename);
}
void meshAnimation() {
    MeshBinding b{0,1,2,3};MeshTrack track;
    track.frames={{{1,0,0,0,0,1,0,0,0,0,1,0}},{{0,-1,0,2,1,0,0,3,0,0,2,1}}};
    CHECK((animatedPosition(track,b,0)==std::array<float,3>{1,2,3}));
    CHECK((animatedPosition(track,b,1)==std::array<float,3>{0,4,7}));
    CHECK(animatedPosition(track,b,0)==animatedPosition(track,b,2));
}
void returnBindings() {
    char path[]="/tmp/ttyd-returns-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);close(fd);
    auto write=[&](uint32_t version,float x,bool duplicate,bool trailing) {
        std::ofstream f(path,std::ios::binary);f.write("RPOS",4);uint32_t count=duplicate?2:1,node=100;
        ReturnPoint point{x,.15f,2.2f};f.write(reinterpret_cast<char*>(&version),4);f.write(reinterpret_cast<char*>(&count),4);
        for(uint32_t n=0;n<count;++n) {f.write(reinterpret_cast<char*>(&node),4);f.write(reinterpret_cast<char*>(&point),12);}
        if(trailing)f.put('x');
    };
    write(1,.84f,false,false);CHECK(loadGroundReturns(path));GroundContact contact{0,100};ReturnPoint point;
    CHECK(groundReturn(contact,point));CHECK(point.x==.84f);
    for(int bad=0;bad<4;++bad) {
        write(bad==0?2:1,bad==1?std::numeric_limits<float>::quiet_NaN():2,bad==2,bad==3);
        CHECK(!loadGroundReturns(path));CHECK(groundReturn(contact,point));CHECK(point.x==.84f);
    }
    contact.sourceNode=999;CHECK(!groundReturn(contact,point));CHECK(point.x==.84f);
    std::remove(path);clearCollision();
}
void waterRecovery() {
    WaterRecovery w;int hp=7;w.begin({5.6f,-.1f,2.4f},{5.5f,.05f,-1.9f});
    CHECK(w.tick(hp)==WaterEvent::None);CHECK(std::fabs(w.position[1]+.16f)<.0001f);
    int elapsed=1;while(w.state!=1 && elapsed++<40)w.tick(hp);
    CHECK(w.state==1 && w.counter==40 && hp==7);
    for(int n=0;n<39;++n)w.tick(hp);CHECK(w.state==1);w.tick(hp);CHECK(w.state==10);
    while(w.tick(hp)!=WaterEvent::Landed && elapsed++<400){}
    CHECK(elapsed<400 && w.state==31 && w.counter==49);
    CHECK(std::hypot(w.position[0]-5.5f,w.position[2]+1.9f)<=.081f && std::fabs(w.position[1]-.05f)<.0001f);
    for(int n=0;n<48;++n)w.tick(hp);CHECK(hp==7);CHECK(w.tick(hp)==WaterEvent::GetUp);CHECK(hp==6 && w.state==32 && w.counter==11);
    for(int n=0;n<10;++n)w.tick(hp);CHECK(w.active());CHECK(w.tick(hp)==WaterEvent::Complete && !w.active());
    CHECK(hp==6);w.state=31;w.counter=1;hp=1;CHECK(w.tick(hp)==WaterEvent::Dead && hp==0 && !w.active());
}
void transitionFade() {
    TransitionFade fade;CHECK(!fade.active() && fade.alpha()==0 && !fade.ready());fade.startOut();
    int previous=0;
    for(int n=0;n<18;++n) {fade.tick();CHECK(fade.alpha()>=previous);previous=fade.alpha();CHECK(!fade.ready());}
    CHECK(fade.alpha()==255 && fade.active());fade.tick();CHECK(fade.alpha()==255 && !fade.ready());
    fade.presented();CHECK(fade.ready());fade.startIn();CHECK(fade.alpha()==255 && !fade.ready());
    for(int n=0;n<18;++n){fade.tick();CHECK(fade.alpha()<=previous);previous=fade.alpha();}
    CHECK(!fade.active() && fade.alpha()==0);
    Game g;g.warpRequest=0;g.fade.startOut();const auto p=g.progress.position;const auto partner=g.partner;
    Input blocked;blocked.x=1;blocked.confirm=true;blocked.menuConfirm=true;
    for(int n=0;n<20;++n)g.tick(blocked);
    CHECK(g.progress.position.x==p.x && g.progress.position.z==p.z);
    CHECK(g.partner.x==partner.x && g.partner.z==partner.z && !g.saveRequested);
    CHECK(g.scene==Scene::Explore && g.fade.alpha()==255 && !g.fade.ready());
    g.fade.presented();CHECK(g.fade.ready());
    // A rejected room load reveals the retained scene before controls resume.
    g.warpRequest=-1;g.fade.startIn();
    for(int n=0;n<18;++n)g.tick(blocked);
    CHECK(!g.fade.active() && g.progress.position.x==p.x);
    RoadMotion motion;motion.begin({0,0},{.6f,0});CHECK(motion.remaining==24);
    for(int n=0;n<23;++n){CHECK(!motion.tick());CHECK(std::fabs(motion.position[0]-(n+1)*.025f)<.00001f);}
    CHECK(motion.tick() && motion.position[0]==.6f);
}
void indexedOriginalCollision() {
    size_t queries=0;
    for(uint32_t room=0;room<5;++room) {
        std::string prefix=std::string("romfs/")+roomAsset(room);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));CHECK(loadGroundReturns((prefix+".returns").c_str()));CHECK(loadHead((prefix+".head").c_str()));
        std::ifstream vertexFile(prefix+".vertices",std::ios::binary|std::ios::ate);
        CHECK(vertexFile.good());const auto vertexBytes=vertexFile.tellg();
        CHECK(vertexBytes>0 && vertexBytes%36==0);const uint32_t vertexCount=vertexBytes/36;
        std::vector<UVTrack> uv;CHECK(loadUVAnimation((prefix+".uvanim").c_str(),vertexCount,uv));
        CHECK(uv.size()==(room==2?3u:room==1?4u:0u));
        std::vector<MeshTrack> mesh;CHECK(loadMeshAnimation((prefix+".meshanim").c_str(),vertexCount,mesh));
        CHECK(mesh.size()==(room==0?2u:room==1?3u:0u));size_t animated=0;
        for(const auto& track:mesh)for(auto binding:track.bindings) {
            auto first=animatedPosition(track,binding,0),loop=animatedPosition(track,binding,track.frames.size());
            CHECK(first==loop);CHECK(std::isfinite(first[0]) && std::isfinite(first[1]) && std::isfinite(first[2]));++animated;
        }
        CHECK(animated==(room==0?12u:room==1?4581u:0u));
        for(const auto& track:uv)for(auto binding:track.bindings) {
            auto first=animatedUV(track,binding,0),loop=animatedUV(track,binding,track.frames.size());
            CHECK(first==loop);CHECK(std::isfinite(first[0]) && std::isfinite(first[1]));
        }
        std::ifstream walls(prefix+".walls",std::ios::binary),ground(prefix+".ground",std::ios::binary);
        uint32_t nw=0,ng=0;walls.read(reinterpret_cast<char*>(&nw),4);ground.read(reinterpret_cast<char*>(&ng),4);
        CHECK((ng&0xc0000000)==0xc0000000);ng&=0x3fffffff;
        CHECK((nw&0xe0000000)==0xe0000000);nw&=0x1fffffff;
        std::vector<std::array<float,6>> segments(nw);std::vector<std::array<float,9>> triangles(ng);
        std::vector<GroundContact> contacts(ng);
        std::vector<uint32_t> floorCulls(ng);
        std::vector<GroundContact> wallContacts(nw);
        std::vector<WallTriangle> wallGeometry(nw);
        for(uint32_t n=0;n<nw;++n){walls.read(reinterpret_cast<char*>(segments[n].data()),24);walls.read(reinterpret_cast<char*>(&wallContacts[n]),8);walls.read(reinterpret_cast<char*>(wallGeometry[n].data()),36);}
        for(uint32_t n=0;n<ng;++n) {
            ground.read(reinterpret_cast<char*>(triangles[n].data()),36);
            ground.read(reinterpret_cast<char*>(&contacts[n]),8);
            ground.read(reinterpret_cast<char*>(&floorCulls[n]),4);
        }
        CHECK(walls.good());CHECK(ground.good());
        for(int x=-18;x<=18;++x)for(int z=-16;z<=16;++z)for(float level:{0.f,1.2f,3.f}) {
            Vec2 p{x*.5f,z*.5f};bool free=true,found=false;float floor=-1e30f;
            for(size_t n=0;n<segments.size();++n) {
                auto w=segments[n];auto flags=wallContacts[n].attributes;
                if(!(flags&0x00080080) && !(flags&0x00800000) && (flags&4))continue;
                if(!((w[4]<=level+.11f && w[5]>=level+.11f) || (w[4]<=level+.2664f && w[5]>=level+.2664f)))continue;
                if(wallSliceBlocks(wallGeometry[n],level+.11f,p.x,p.z,.1f) ||
                   wallSliceBlocks(wallGeometry[n],level+.2664f,p.x,p.z,.1f))free=false;
            }
            CHECK(walkable(p,level)==free);
            GroundContact expected;size_t selected=0;
            for(size_t n=0;n<triangles.size();++n) {
                const auto flags=contacts[n].attributes;
                if(!(flags&0x80080) && !(flags&0x800000) && (flags&4))continue;
                auto v=triangles[n];
                float ny=(v[5]-v[2])*(v[6]-v[0])-(v[3]-v[0])*(v[8]-v[2]);
                if(floorCulls[n]==1 && ny<=0)continue;
                if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) || p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
                float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);if(std::fabs(den)<1e-8f)continue;
                float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
                float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
                float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
                if(a>=-.0001f && b>=-.0001f && a+b<=1.0001f && y<=level+.25f && y>=level-.5f && y>floor){found=true;floor=y;expected=contacts[n];selected=n;}
            }
            if(found) {
                const auto& v=triangles[selected];
                double ax=v[3]-v[0],ay=v[4]-v[1],az=v[5]-v[2];
                double bx=v[6]-v[0],by=v[7]-v[1],bz=v[8]-v[2];
                double nx=ay*bz-az*by,ny=az*bx-ax*bz,nz=ax*by-ay*bx;
                // Independent normal-cone comparison, not the runtime atan2 helper.
                double cosine=std::cos(50*3.14159265358979323846/180);
                found=ny*ny>cosine*cosine*(nx*nx+ny*ny+nz*nz);
            }
            float actual=0;GroundContact contact;CHECK(groundAt(p,level,actual,&contact)==found);
            if(found) {ReturnPoint point;if(groundReturn(expected,point))expected.attributes|=0x40000000;CHECK(std::fabs(actual-floor)<.001f);CHECK(contact.attributes==expected.attributes);CHECK(contact.sourceNode==expected.sourceNode);}
            ++queries;
        }
    }
    clearCollision();std::printf("Indexed original collision matched %zu full-scan probes\n",queries);
}
void originalRooms() {
    CHECK(loadWarps("romfs/rooms.bin"));
    std::ifstream file("romfs/rooms.bin",std::ios::binary);char magic[4];uint32_t version=0,count=0;
    file.read(magic,4);file.read(reinterpret_cast<char*>(&version),4);file.read(reinterpret_cast<char*>(&count),4);
    CHECK(std::string(magic,4)=="ROAD" && version==1);
    CHECK(count==8);std::vector<RoadWarp> roads(count);file.read(reinterpret_cast<char*>(roads.data()),count*sizeof(RoadWarp));CHECK(file.good());
    for(size_t n=0;n<roads.size();++n) {
        const auto& road=roads[n];std::string prefix=std::string("romfs/")+roomAsset(road.target);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));CHECK(loadGroundReturns((prefix+".returns").c_str()));CHECK(loadHead((prefix+".head").c_str()));
        if(road.source==1 || (road.target==0 && road.arrivalNode==1382128) || road.target==2) {
            Game unresolved;unresolved.progress.room=road.source;unresolved.warpRequest=n;
            CHECK(!unresolved.finishWarp());CHECK(unresolved.roadPolicy==RoadPolicy::MissingStory);
            CHECK(unresolved.progress.room==road.source && unresolved.warpRequest==int(n));
            unresolved.progress.originalStory=road.source==1?5:road.target==0?6:7;
            CHECK(!unresolved.finishWarp());CHECK(unresolved.roadPolicy==RoadPolicy::Custom);
        }
        Game g;g.progress.room=road.source;g.progress.hp=7;g.progress.originalStory=11;g.warpRequest=n;
        g.fieldSystemFlags=0x240;
        CHECK(g.warpTarget()==road.target);CHECK(g.finishWarp());
        CHECK(g.progress.room==road.target);CHECK(g.progress.hp==7);CHECK(g.warpRequest==-1);
        CHECK(!g.roadCasesEnabled);CHECK(g.saveRequested);
        CHECK(g.fieldSystemFlags==(road.target==3?0x200u:0x240u));
        CHECK(g.cameraX==g.progress.position.x);CHECK(g.cameraZ==g.progress.position.z);
        CHECK(g.cameraHeight==g.groundHeight);CHECK(walkable(g.progress.position,g.groundHeight));
        CHECK(g.groundContact.attributes==g.partnerContact.attributes);
        CHECK(g.groundContact.sourceNode==g.partnerContact.sourceNode);
        CHECK(std::fabs(g.progress.position.x-road.spawnX)<.001f);CHECK(std::fabs(g.progress.position.z-road.spawnZ)<.001f);
        CHECK(decodeSave(encodeSave(g.progress),g.progress));
        Game entry=g;entry.beginRoadArrival();entry.fade.startIn();
        CHECK(entry.arriving && !entry.saveRequested && entry.roadMotion.remaining==24);
        Input blocked;blocked.x=1;blocked.confirm=true;blocked.menuConfirm=true;
        for(int frame=0;frame<23;++frame){entry.tick(blocked);CHECK(entry.arriving && !entry.saveRequested);}
        entry.tick(blocked);CHECK(!entry.arriving && entry.saveRequested && !entry.fade.active() && entry.roadCasesEnabled);
        CHECK(entry.progress.position.x==g.progress.position.x && entry.progress.position.z==g.progress.position.z);
        // Walk back toward the reciprocal opening, then simulate a failed load.
        Game departure=entry;
        Input toward; toward.x=entry.roadMotion.step[0]<0?1:entry.roadMotion.step[0]>0?-1:0;
        toward.z=entry.roadMotion.step[1]<0?1:entry.roadMotion.step[1]>0?-1:0;
        {Game probe=entry;for(int n=0;n<50 && probe.warpRequest<0;++n)probe.tick(toward);CHECK(probe.warpTarget()==road.source);}
        departure.tick(toward);
        Input jump=toward;jump.confirm=true;jump.jumpHeld=true;departure.tick(jump);
        for(int frame=0;frame<60 && departure.warpRequest<0;++frame)departure.tick(toward);
        CHECK(departure.warpRequest>=0);
        const auto origin=departure.progress.position;const float originHeight=departure.groundHeight;
        departure.tick(blocked);CHECK(std::hypot(departure.progress.position.x-origin.x,departure.progress.position.z-origin.z)>0);
        CHECK(departure.jumpHeight>0);
        for(int frame=0;frame<9;++frame){
            departure.tick(blocked);
            CHECK(!departure.fade.active() && !departure.roadMotion.active() && !departure.saveRequested);
        }
        departure.tick(blocked);CHECK(!departure.saveRequested);
        CHECK(departure.fade.active()); // Parent does not join airborne wait or travel.
        CHECK(std::hypot(departure.progress.position.x-origin.x,departure.progress.position.z-origin.z)>0);
        departure.cancelRoad();
        CHECK(departure.progress.position.x==origin.x && departure.progress.position.z==origin.z);
        CHECK(departure.groundHeight==originHeight && departure.warpRequest==-1);
        CHECK(walkable(departure.progress.position,departure.groundHeight));
        for(int frame=0;frame<10;++frame)g.tick({});CHECK(g.warpRequest==-1);
        if(road.source==0 && road.target==1) {
            // Harbor's entrance overlaps short posts and lower floor surfaces.
            // A downward floor change must not place Mario inside their walls.
            Input down;down.z=1;
            for(int frame=0;frame<60;++frame) {
                g.tick(down);CHECK(walkable(g.progress.position,g.groundHeight));
            }
            Input up;up.z=-1;
            for(int frame=0;frame<120 && g.warpRequest<0;++frame)g.tick(up);
            // Stepping off the deck now falls to the dock; the upper entrance
            // cannot be reached by walking straight back up the drop.
            CHECK(g.warpRequest==-1 && std::fabs(g.groundHeight-.15f)<.001f);
            Game stairs;stairs.progress.room=0;stairs.progress.originalStory=11;stairs.warpRequest=n;CHECK(stairs.finishWarp());stairs.beginRoadArrival();advance(stairs,24);
            auto walk=[&](float x,float z,int frames) {
                Input i;i.x=x;i.z=z;
                for(int frame=0;frame<int(std::ceil(frames*.065f/DashSpeed));++frame) {
                    stairs.tick(i);if(stairs.warpRequest>=0)break;
                    CHECK(walkable(stairs.progress.position,stairs.groundHeight));
                    CHECK(walkable(stairs.partner,stairs.partnerHeight));
                }
            };
            walk(0,1,9);walk(1,0,30);walk(1,1,50);walk(-1,0,60);walk(0,1,10);
            CHECK(std::fabs(stairs.progress.position.x)<1 && stairs.progress.position.z>-2.5f);
            CHECK(std::fabs(stairs.groundHeight-.15f)<.001f);
            CHECK(std::fabs(stairs.partnerHeight-.15f)<.001f);
            walk(0,-1,10);walk(1,0,60);walk(-1,-1,50);walk(-1,0,30);walk(0,-1,60);
            CHECK(stairs.warpTarget()==0);
        }
    }
    clearCollision();
}
void originalReturnSelection() {
    CHECK(loadCollision("romfs/gor_00.walls"));CHECK(loadGround("romfs/gor_00.ground"));CHECK(loadGroundReturns("romfs/gor_00.returns"));CHECK(loadHead("romfs/gor_00.head"));
    Game g;Progress p;p.room=1;p.position={0,-5.1f};p.elevation=1.2f;g.restore(p);
    ReturnPoint point{99,99,99};CHECK(!g.recoveryReturn(point));CHECK(point.x==99);
    auto walk=[&](float x,float z,int frames) {Input i;i.x=x;i.z=z;for(int n=0;n<int(std::ceil(frames*.065f/DashSpeed)) && !g.water.active();++n)g.tick(i);};
    walk(1,0,30);walk(1,1,80);walk(0,1,80);
    CHECK(g.returnContact.sourceNode==0xd2a30);
    if(!g.water.active()){g.tick(confirm());walk(0,1,60);} // Fall or jump into the original water floor.
    CHECK(g.groundContact.attributes&0x200);CHECK(!(g.groundContact.attributes&0x40000000));
    // This path crosses Ship_Change02 after A_yuka_04; the most recent
    // eligible original owner supplies its authored automatic return center.
    CHECK(g.returnContact.sourceNode==0xd2a30);
    CHECK(g.recoveryReturn(point));CHECK(std::fabs(point.x-5.5f)<.001f);
    CHECK(std::fabs(point.y-.05f)<.001f && std::fabs(point.z+1.9f)<.001f);
    const auto remembered=g.returnContact.sourceNode;CHECK(g.water.active());
    const auto partner=g.partner;
    const int before=g.progress.hp;
    for(int n=0;g.water.active() && n<500;++n) {
        const auto stamp=g.fieldCameraShift.previousMilliseconds;
        const auto follows=g.marioCamera.retained;const auto state=g.water.state;
        Input ignored;ignored.x=1;ignored.confirm=true;ignored.menuConfirm=true;g.tick(ignored);
        CHECK(g.fieldCameraShift.previousMilliseconds>stamp); // Manager runs despite retained follow lock.
        if(state==0 || state==1 || state==10 || state==11) {
            CHECK(g.marioCamera.retained.x==follows.x && g.marioCamera.retained.y==follows.y && g.marioCamera.retained.z==follows.z);
        }
        CHECK(g.partner.x==partner.x && g.partner.z==partner.z);CHECK(!g.saveRequested);
    }
    CHECK(!g.water.active() && g.progress.hp==before-1 && g.scene==Scene::Explore);
    CHECK(g.returnContact.sourceNode==remembered);
    CHECK(std::hypot(g.progress.position.x-5.5f,g.progress.position.z+1.9f)<=.081f);
    Game dead;p.room=1;p.position={5.6f,2.4f};p.elevation=-.1f;p.hp=1;dead.restore(p);
    dead.tick({});CHECK(dead.water.active());
    for(int n=0;dead.water.active() && n<700;++n)dead.tick({});
    CHECK(dead.scene==Scene::FieldDefeat && dead.progress.hp==0 && !dead.saveRequested);
    dead.tick(confirm());CHECK(dead.reloadRequested && dead.progress.hp==0);
    // Loading a different room resets the map-local remembered hit ID.
    CHECK(loadCollision("romfs/map.walls"));CHECK(loadGround("romfs/map.ground"));CHECK(loadGroundReturns("romfs/map.returns"));CHECK(loadHead("romfs/map.head"));
    p.room=0;p.position={1,2};p.elevation=0;g.restore(p);CHECK(g.returnContact.sourceNode==0);CHECK(!g.recoveryReturn(point));
    clearCollision();
}
void originalVoidReturn() {
    CHECK(loadCollision("romfs/map.walls"));CHECK(loadGround("romfs/map.ground"));CHECK(loadGroundReturns("romfs/map.returns"));CHECK(loadHead("romfs/map.head"));
    Progress p;p.position={.35f,6.95f};p.hp=7;p.crumpDefeated=true;
    float floor=0;CHECK(groundAt(p.position,0,floor) && walkable(p.position,floor));p.elevation=floor;
    Game g;g.restore(p);Input down;down.z=1;
    for(int n=0;n<300 && !g.water.active();++n){g.tick(down);CHECK(g.warpRequest==-1);}
    CHECK(g.water.active() && g.water.kind==RecoveryKind::Void);
    for(int n=0;n<500 && g.water.active();++n)g.tick({});
    CHECK(g.scene==Scene::Explore && g.progress.hp==6 && !g.airborne());
    CHECK(std::hypot(g.progress.position.x-p.position.x,g.progress.position.z-p.position.z)<.08f);
    CHECK(walkable(g.progress.position,g.groundHeight));clearCollision();
}
void originalHeadJump() {
    clearCollision();CollisionTransition tx;CHECK(tx.load("romfs/map"));tx.commit();
    Progress p;p.position={-6.7f,.1f};p.crumpDefeated=true;
    // Packed roof: back probe normal114.93deg is rejected; center132.97deg
    // is admitted but initially returns a near miss. These are mesh/static
    // contract checks, not measured original-game jump heights.
    const auto first=searchFieldHead(p.position,{1,0},.137f,.0648f,.0648f);
    CHECK(!first.contact && first.clearance>.17f && first.clearance<.19f);
    const auto contact=searchFieldHead(p.position,{1,0},.32f,.03f,.03f);
    CHECK(contact.contact && contact.node.sourceNode==1381592 && std::fabs(contact.correction-.307f)<.00001f);
    Game g;g.restore(p);g.tick(confirm());Input held;held.jumpHeld=true;float peak=0;
    for(int n=0;n<45;++n){g.tick(held);peak=std::max(peak,g.jumpHeight);}
    CHECK(std::fabs(peak-.307f)<.00001f);CHECK(!g.airborne() && g.jumpHeight==0);
    CHECK(g.progress.hp==p.hp && g.warpRequest==-1);clearCollision();
}
int main(int argc,char**) {
    try {if(argc>1) {indexedOriginalCollision();originalRooms();originalReturnSelection();originalVoidReturn();originalHeadJump();std::puts("Original room arrivals passed");return 0;}movement();dialogueAndTouch();timing();battles();saves();roadContracts();collisionImport();groundImport();unequalFloorMotion();collisionRollback();visibility();uvAnimation();meshAnimation();returnBindings();waterRecovery();transitionFade();std::puts("Core scenarios passed");}
    catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
