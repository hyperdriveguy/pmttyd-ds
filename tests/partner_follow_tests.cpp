#include "partner_follow.hpp"
#include "game.hpp"
#include <stdexcept>
#include <cstdio>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(#x);}while(0)
static PartnerFollowServices flat() {
    PartnerFollowServices s;s.free=[](CameraPoint){return true;};
    s.floor=[](CameraPoint,float lo,float hi,float& h){if(lo<=0 && hi>=0){h=0;return true;}return false;};
    return s;
}
int main() {
    auto s=flat();CameraPoint root{0,0,0};PartnerFollow f;f.reset(root,{.8f,0,0});
    for(int n=0;n<40;++n)f.tick(root,{.8f,0,0},false,.0225f,s);
    CHECK(std::fabs(root.x-.3f)<.001f && root.y==0);
    // The air edge survives stationary XYZ and starts only when its mark is read.
    f.tick(root,{.8f,0,0},true,.0225f,s);CHECK(f.motion==PartnerFollow::Motion::Jump && root.y==0);
    const int count=f.jumpCount;bool rose=false;
    for(int n=0;n<100;++n){f.tick(root,{.8f,0,0},false,.0225f,s);rose|=root.y>.1f;}
    CHECK(rose && root.y==0 && f.motion==PartnerFollow::Motion::Ground && f.jumpCount==count);
    CHECK(std::fabs(root.x-.3f)<.001f); // Stationary hops retain follower space.
    // A jump at the end of many short marks must not sit forever beyond the gap.
    root={0,0,0};f.reset(root,{.5f,0,0});
    for(int n=1;n<=50;++n)f.tick(root,{.5f+n*.02f,0,0},false,.0225f,s);
    for(int n=0;n<60;++n)f.tick(root,{1.5f,0,0},false,.0225f,s);
    f.tick(root,{1.5f,0,0},true,.0225f,s);CHECK(f.motion==PartnerFollow::Motion::Jump);
    // Air travel continues across missing support even inside the spacing gap.
    auto gap=s;gap.floor=[](CameraPoint p,float lo,float hi,float& h) {
        if(p.x>=.1f && p.x<=.4f)return false;
        if(lo<=0 && hi>=0){h=0;return true;}return false;
    };
    root={0,0,0};f.reset(root,{.8f,0,0});f.trail={{{0,0,0},true},{{.8f,0,0},false}};
    for(int n=0;n<100;++n)f.tick(root,{.8f,0,0},false,.0225f,gap);
    CHECK(root.x>.4f && root.y==0 && f.motion==PartnerFollow::Motion::Ground);
    // A ledge does not freeze horizontal motion or snap instantly to lower ground.
    auto ledge=s;ledge.floor=[](CameraPoint p,float lo,float hi,float& h) {
        float y=p.x<.2f?0.f:-.5f;if(lo<=y && hi>=y){h=y;return true;}return false;
    };
    root={0,0,0};f.reset(root,{1, -.5f,0});bool fell=false,intermediate=false;
    for(int n=0;n<110;++n){f.tick(root,{1,-.5f,0},false,.0225f,ledge);fell|=f.motion==PartnerFollow::Motion::Fall;intermediate|=root.y<0 && root.y>-.5f;}
    CHECK(fell && intermediate && root.y==-.5f && root.x>.2f);
    // Thin walls cannot be crossed, even with an oversized travel budget.
    auto wall=s;wall.free=[](CameraPoint p){return !(p.x>.4f && p.x<.6f && std::fabs(p.z)<1);};
    CHECK(PartnerFollow::move({0,0,0},{1,0,0},1,wall).x<.4f);
    root={0,0,0};f.reset(root,{0,0,2});
    f.trail={{{.5f,0,0},false},{{0,0,2},false}};
    for(int n=0;n<150;++n)f.tick(root,{0,0,2},false,.0225f,wall);
    CHECK(root.z>1.4f && root.x<.4f); // Stale blocked mark pruned to a supported route.
    // A remaining barrier never causes an arbitrary timer/capacity teleport.
    root={0,0,0};f.reset(root,{2,0,0});
    for(int n=0;n<700;++n)f.tick(root,{2,0,0},false,.0225f,wall);
    CHECK(root.x<.4f && !f.warped);
    for(int n=0;n<600;++n)f.tick(root,{2+n*.001f,0,0},false,.0225f,wall);
    CHECK(root.x<.4f && !f.warped && f.trail.size()<=512);
    // Screen recovery uses the recovered margins and commits a candidate.
    auto screen=s;screen.project=[](CameraPoint p){CameraScreenCache q;q.x=int(p.x*100);q.y=200;return q;};
    root={10,0,0};f.reset(root,{0,0,0});f.tick(root,{0,0,0},false,.0225f,screen);
    CHECK(f.warped && screen.project(root).x<=700 && root.x>=-1 && f.trail.size()==2);
    root={7,0,0};f.reset(root,{0,0,0});f.tick(root,{0,0,0},false,0,screen);CHECK(!f.warped);
    root={0,-20.1f,0};f.reset(root,{1,0,0});f.tick(root,{1,0,0},false,0,s);CHECK(f.warped && root.x==1 && root.y==0);
    // Native Game binding follows actual collision output and carries air state.
    clearCollision();Game game;Progress p;p.position={0,2};game.restore(p);
    Input input;input.confirm=true;input.jumpHeld=true;game.tick(input);
    bool partnerJump=false;
    for(int n=0;n<100;++n){game.tick({});partnerJump|=game.partnerFollow.motion==PartnerFollow::Motion::Jump;}
    CHECK(partnerJump && game.partnerHeight==0);
    std::puts("Partner following scenarios passed");
}
