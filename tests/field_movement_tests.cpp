#include "field_movement.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace ttyd;
#define CHECK(x) do {if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
bool close(float a,float b){return std::fabs(a-b)<.000001f;}
int main() {
    FieldJump tapped,held;tapped.begin();held.begin();
    CHECK(tapped.ticks==1 && tapped.height==0);
    tapped.tick(false);held.tick(true);
    CHECK(tapped.ticks==2 && close(tapped.height,.0722f) && close(tapped.height,held.height));
    tapped.tick(false);held.tick(true);
    CHECK(tapped.ticks==3 && close(tapped.height,.137f) && close(tapped.height,held.height));
    tapped.tick(false);held.tick(true);
    CHECK(tapped.ticks==4 && close(held.height-tapped.height,.00526f));
    FieldMovement m;
    CHECK(close(m.airborne(1,0).x,.002625f)); // Standing launch: drift only.
    CHECK(close(m.airborne(.5f,0).x,.0013125f));
    CHECK(close(m.airborne(2,0).x,.002625f)); // Original stick cap70.
    m.grounded(.5f,0);CHECK(close(m.speed,.01f));
    CHECK(close(m.airborne(0,0).x,.01f));
    CHECK(close(m.airborne(1,0).x,.012625f)); // No fresh dash entry in the air.
    CHECK(close(m.airborne(-1,0).x,.007375f));
    CHECK(close(m.airborne(0,1).x,.01f) && close(m.airborne(0,1).z,.002625f));
    CHECK(close(m.speed,.01f) && close(m.headingX,1)); // Drift doesn't rewrite retained travel.
    m.grounded(1,0);CHECK(close(m.airborne(0,0).x,.0225f));
    CHECK(close(m.airborne(1,0).x,.0225f));
    auto diagonal=m.airborne(0,1);CHECK(close(std::hypot(diagonal.x,diagonal.z),.0225f));
    CHECK(diagonal.x>0 && diagonal.z>0);
    m.grounded(0,-1);CHECK(close(m.airborne(0,0).z,-.0225f));
    m.grounded(0,0);CHECK(m.speed==0 && m.airborne(0,0).z==0);
    m.grounded(FieldMovement::DashStick,0);CHECK(close(m.speed,.01f));
    m.grounded(FieldMovement::DashStick+.001f,0);CHECK(close(m.speed,.0225f));
    m.groundedStick(33,44);CHECK(m.speed==FieldMovement::Walk);
    m.groundedStick(34,44);CHECK(m.speed==FieldMovement::Dash);
    m.groundedStick(-55,0);CHECK(m.speed==FieldMovement::Walk && m.headingX==-1);
    clearCollision();Progress p;p.room=2;p.position={-4,2};
    auto launch=[&](Game& g,float x) {
        g.restore(p);Input i;i.x=x;g.tick(i);i.confirm=true;i.jumpHeld=true;g.tick(i);CHECK(g.airborne());
        CHECK(g.jumpHeight==0); // Launch-call horizontal policy, no extra vertical tick.
    };
    Game walk;launch(walk,.5f);float before=walk.progress.position.x;
    Input released;released.jumpHeld=true;walk.tick(released);
    CHECK(close(walk.progress.position.x-before,.01f));
    CHECK(close(walk.jumpHeight,.0722f)); // First subsequent manager update integrates.
    Input stronger=released;stronger.x=1;before=walk.progress.position.x;walk.tick(stronger);
    CHECK(close(walk.progress.position.x-before,.012625f));
    Game dash;launch(dash,1);before=dash.progress.position.x;dash.tick(released);
    CHECK(close(dash.progress.position.x-before,.0225f));
    Game standing;launch(standing,0);before=standing.progress.position.x;standing.tick(stronger);
    CHECK(close(standing.progress.position.x-before,.002625f));
    // Current input drift participates in the launch invocation itself.
    Game launchDrift;launchDrift.restore(p);Input launchInput;launchInput.confirm=true;launchInput.x=.5f;
    launchDrift.tick(launchInput);CHECK(launchDrift.airborne() && launchDrift.jumpHeight==0);
    CHECK(close(launchDrift.progress.position.x-p.position.x,.0113125f));
    // Threshold replacement in the same dispatcher call retries the unchanged
    // A trigger and establishes the newly selected speed before jump entry.
    Game threshold;threshold.restore(p);Input weak;weak.x=.5f;threshold.tick(weak);
    Input strong;strong.x=1;strong.confirm=true;before=threshold.progress.position.x;threshold.tick(strong);
    CHECK(threshold.airborne() && close(threshold.progress.position.x-before,.0225f));
    Game stop;stop.restore(p);stop.tick(strong); // Strong idle launch selects dash.
    for(int n=0;n<35;++n)stop.tick({});CHECK(!stop.airborne());
    stop.tick(weak);Input neutralJump;neutralJump.confirm=true;before=stop.progress.position.x;stop.tick(neutralJump);
    CHECK(stop.airborne() && stop.progress.position.x==before); // idle entry clears base.
    Game landing;landing.restore(p);landing.tick(neutralJump);
    for(int n=0;n<21;++n)landing.tick({});CHECK(landing.airborne());
    landing.tick(neutralJump);CHECK(!landing.airborne() && landing.jumpHeight==0);
    landing.tick({});landing.tick(neutralJump);CHECK(landing.airborne()); // New grounded trigger is admitted.
    for(float prior:{0.f,.5f,1.f})for(float current:{0.f,.5f,1.f}) {
        Game transition;transition.restore(p);Input start;start.x=prior;transition.tick(start);
        Input launch;launch.x=current;launch.confirm=true;before=transition.progress.position.x;
        transition.tick(launch);
        const float expected=current==0?0:current==.5f?.0113125f:.0225f;
        CHECK(transition.airborne() && transition.jumpHeight==0);
        CHECK(close(transition.progress.position.x-before,expected));
    }
    Game turn;turn.restore(p);turn.tick(weak);Input changed;changed.z=.5f;changed.confirm=true;
    const auto oldPosition=turn.progress.position;turn.tick(changed);
    CHECK(turn.progress.position.x==oldPosition.x && close(turn.progress.position.z-oldPosition.z,.0113125f));
    // The field sees published signed bytes and A edges, not stale HID fractions
    // or independently supplied abstract confirm/held bits.
    Game physical;physical.restore(p);Input pad;pad.physicalSnapshot=true;pad.x=.79f;
    before=physical.progress.position.x;physical.tick(pad);
    CHECK(physical.physicalChannel().analog.mainX==55 && close(physical.progress.position.x-before,.01f));
    pad.x=1;pad.physicalButtons=BattleButtonA;before=physical.progress.position.x;physical.tick(pad);
    CHECK(physical.airborne() && close(physical.progress.position.x-before,.0225f));
    pad.confirm=true; // An independently retained HID edge cannot override GP.
    for(int n=0;n<35;++n)physical.tick(pad);CHECK(!physical.airborne());
    Game cleared;cleared.restore(p);pad.physicalButtons=0x1c00;before=cleared.progress.position.x;
    cleared.tick(pad);CHECK(cleared.progress.position.x==before && !cleared.airborne());
    // Resetting the game must not carry speed from a previous room/save.
    dash.restore(p);dash.tick(Input{});Input jump;jump.confirm=true;dash.tick(jump);
    before=dash.progress.position.x;dash.tick(released);CHECK(dash.progress.position.x==before);
    std::cout<<"Ordinary field air movement passed\n";
}
