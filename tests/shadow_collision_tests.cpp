#include "shadow_collision.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
using namespace ttyd;
struct Fixture {
    ShadowWork* work=nullptr;
    unsigned cameras=0,calls=0;
    bool cameraReady=false,rayReady=false,append=false;
    ShadowRayResult result=ShadowRayResult::Hit;
    ShadowCollisionHit hit{{10,20,30},{.5f,-.75f,.25f}};
    std::vector<ShadowCollisionRay> rays;
    ShadowCollisionServices services(){return {this,
        [](void* p){auto& f=*static_cast<Fixture*>(p);++f.cameras;return f.cameraReady;},
        [](void* p,const ShadowCollisionRay& ray,ShadowCollisionHit& hit){
            auto& f=*static_cast<Fixture*>(p);++f.calls;f.rays.push_back(ray);
            if(!f.rayReady)return ShadowRayResult::Unavailable;
            hit=f.hit;
            if(f.append){f.append=false;f.work->collisionEnabled=0;assert(f.work->enter({4,5,6},3));}
            return f.result;
        }};}
};
int main(){
    ShadowWork work;ShadowCollisionCursor cursor;Fixture f;f.work=&work;
    assert(work.enter({1,2,3},4));auto& r=*work.record(0);r.matrix.fill(91);
    assert(!shadowPrepareCollision(work,cursor,f.services()));assert(f.cameras==1 && f.calls==0 && cursor.pool==work.record(0));
    f.cameraReady=true;
    assert(!shadowPrepareCollision(work,cursor,f.services()));assert(f.cameras==2 && f.calls==1 && cursor.rayPending);
    const auto original=f.rays[0];assert((original.origin==std::array<float,3>{1,3,3}));
    assert((original.direction==std::array<float,3>{0,-1,0}) && original.length==150 && original.filter==0x8002d380);
    // Hold inside the query pins origin/admission. Hit uses fresh radius,
    // height and flags even when those and position/type changed during hold.
    r.position={99,100,101};r.radius=8;r.height=12;r.flags=0x10;r.type=3;
    f.rayReady=true;f.append=true;
    assert(shadowPrepareCollision(work,cursor,f.services()));assert(f.cameras==2 && f.calls==2 && !cursor.active);
    assert(f.rays[1].origin==original.origin && r.flags==0x12);
    const std::array<float,12> expected{-5, -3, 1, 10.5f, 4, -9, 2, 19.25f, 0, 2, 6, 30.25f};
    assert(r.matrix==expected);
    // Count is reread after the query; its newly entered bypass record is
    // processed without another ray and with all off-diagonal bytes reset.
    const auto& appended=*work.record(1);
    assert(appended.flags==2 && (appended.matrix==std::array<float,12>{3,0,0,4,0,20,0,6.5f,0,0,3,6}));
    // Miss preserves every matrix byte/flags; the next callback can retry it.
    ShadowWork misses;assert(misses.enter({0,0,0},1));misses.record(0)->matrix.fill(77);
    Fixture miss;miss.cameraReady=true;miss.rayReady=true;miss.result=ShadowRayResult::Miss;
    ShadowCollisionCursor mc;assert(shadowPrepareCollision(misses,mc,miss.services()));
    assert(misses.record(0)->flags==1 && misses.record(0)->matrix[0]==77 && miss.calls==1);
    assert(shadowPrepareCollision(misses,mc,miss.services()));assert(miss.calls==2);
    // Literal negate/multiply/subtract preserves source signed zero lanes.
    ShadowWork zeroNormal;assert(zeroNormal.enter({0,0,0},2));
    ShadowCollisionCursor zc;Fixture zf;zf.cameraReady=true;zf.rayReady=true;zf.hit={{0,0,0},{0,0,0}};
    assert(shadowPrepareCollision(zeroNormal,zc,zf.services()));
    const auto& zm=zeroNormal.record(0)->matrix;
    assert(zm[0]==0 && std::signbit(zm[0]) && zm[10]==0 && std::signbit(zm[10]) && !std::signbit(zm[8]));
    // Zero (+/-), prepared, and bypass/type3 gates; no ray service required.
    ShadowWork gates;gates.collisionEnabled=0;
    assert(gates.enter({0,0,0},-0.f));assert(gates.enter({1,2,3},2));
    assert(gates.enter({4,5,6},5));gates.record(2)->flags=3;
    gates.type=3;gates.collisionEnabled=1;assert(gates.enter({7,8,9},-2));
    ShadowCollisionCursor gc;ShadowCollisionServices onlyCamera{nullptr,[](void*){return true;},nullptr};
    assert(shadowPrepareCollision(gates,gc,onlyCamera));
    assert(gates.record(0)->flags==0 && gates.record(1)->flags==2 && gates.record(2)->matrix[0]==0 && gates.record(3)->flags==3 && gates.record(3)->matrix[0]==-2);
    // Source unordered radius comparison admits NaN; do not filter it to zero.
    ShadowWork nan;assert(nan.enter({0,0,0},std::numeric_limits<float>::quiet_NaN()));
    ShadowCollisionCursor nc;Fixture nf;nf.cameraReady=true;
    assert(!shadowPrepareCollision(nan,nc,nf.services()) && nf.calls==1);
    ShadowWork different;assert(!shadowPrepareCollision(different,nc,nf.services()));
    ShadowWork empty;ShadowCollisionCursor ec;
    assert(!shadowPrepareCollision(empty,ec,{}));assert(shadowPrepareCollision(empty,ec,onlyCamera));
}
