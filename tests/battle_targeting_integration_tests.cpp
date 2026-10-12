#include "battle_jump_takeoff.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
    int work=1,depth=0;bool nest=false,nested=false;int choiceCalls=0;
    std::array<DamageUnit,3> data{};
    std::array<JumpVector,3> home{{{0,7.9f,0},{0,0,0},{0,0,0}}},visible{};
    std::array<float,3> scales{{1,1,1}};std::array<int8_t,3> faces{{1,1,1}},sides{{0,1,1}};
    std::array<uint8_t,3> counts{{1,1,1}};
    std::array<BattlePositionUnit,3> positions{};
    std::array<BattlePositionPart,3> parts{};
    std::array<SamplingPart,3> sampleParts{};
    std::array<SamplingUnit,3> sampleUnits{};
    BattleCandidateBuffer shared;
    std::vector<int32_t> values,outputDepths;
    struct ChoiceContext {Fixture* fixture;} choiceContext{this};
    Fixture() {
        shared.bytes.fill(0x55);
        for(int i=0;i<3;++i) {
            data[i].kind=i==0?222:0;data[i].attributes=2;data[i].belong=sides[i];
            positions[i]={&home[i],&visible[i],&scales[i],&faces[i],&sides[i],&parts[i]};
            parts[i].id=1;parts[i].attributes=0x10000001;
            parts[i].absolute=i==1?JumpVector{100.9f,30,40.9f}:JumpVector{20.9f,60,80};
            sampleParts[i].position=&parts[i];sampleParts[i].direction=1;
            sampleUnits[i]={&data[i],i,&counts[i],&sampleParts[i],&scales[i]};
        }
        sampleParts[2].forwardBias=100; // Sort coordinate differs from hit X.
    }
    static int32_t truncate(float v) {assert(std::isfinite(v));return static_cast<int32_t>(v);}
    TakeoffServices takeoff() {
        TakeoffServices s;s.user=this;
        s.captureWork=[](void* p)->void* {return &static_cast<Fixture*>(p)->work;};
        s.resolve=[](void*,int32_t raw)->int32_t {assert(raw>=0 && raw<=2);return raw==0?-2:1;};
        s.translate=[](void*,int32_t id)->int32_t {return id==-2?0:id;};
        s.unit=[](void* p,void* work,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work && id>=0 && id<3);return &f.positions[id];};
        s.enemyBelong=[](void*,BattlePositionUnit&)->int32_t {return 1;};
        s.sideByte=[](void* p,void* work,int8_t side)->uint8_t {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work && side==1);return 1;};
        // s.sample/choose stay null: composed binding must use recovered cores.
        s.truncate=[](void*,float v)->int32_t {return truncate(v);};
        s.store=[](void* p,int32_t destination,int32_t value) {auto& f=*static_cast<Fixture*>(p);assert(destination>=10 && destination<=12);f.values.push_back(value);f.outputDepths.push_back(f.depth);};return s;
    }
    BattleHitPositionServices hit() {
        BattleHitPositionServices s;s.sideFactor=[](void*,int8_t)->int8_t {return 1;};return s;
    }
    CandidateSamplingServices sampling() {
        CandidateSamplingServices s;s.user=this;
        s.captureWork=[](void* p)->void* {return &static_cast<Fixture*>(p)->work;};
        s.unit=[](void* p,void* work,int32_t id)->SamplingUnit* {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);return id>=0 && id<3?&f.sampleUnits[id]:nullptr;};
        s.part=[](void* p,int32_t id,int32_t part) {auto& f=*static_cast<Fixture*>(p);assert(id>=0 && id<3 && part==1);return &f.sampleParts[id];};
        s.status27=[](void*,SamplingUnit& u) {return checkStatus(u.data,27);};
        s.worldPosition=[](void*,SamplingPart& part) {JumpVector v;getPartWorldPosition(*part.position,&v.x,&v.y,&v.z);return v;};
        s.truncate=[](void*,float v)->int32_t {return truncate(v);};return s;
    }
    CandidateChoiceServices choice() {
        CandidateChoiceServices s;s.user=&choiceContext;
        s.captureWork=[](void* p)->void* {return &static_cast<ChoiceContext*>(p)->fixture->work;};
        s.unit=[](void* p,void* work,int32_t id) {
            auto& f=*static_cast<ChoiceContext*>(p)->fixture;assert(work==&f.work && id>=0 && id<3);++f.choiceCalls;
            if(f.nest && !f.nested && f.depth==0) {
                f.nested=true;f.depth=1;f.sampleParts[2].forwardBias=-100;
                f.run();f.sampleParts[2].forwardBias=100;f.depth=0;
            }
            return &f.data[id];
        };
        s.truncate=[](void* p,float v)->int32_t {assert(static_cast<ChoiceContext*>(p)->fixture);return truncate(v);};
        // s.output and RNG stay null: independent local output sink and mode4.
        return s;
    }
    void run() {assert(jumpTakeoffPositionEvent({{0,1,2,10,11,12}},takeoff(),hit(),sampling(),choice())==2);}
};
}
int main() {
    Fixture full;const auto sharedBefore=full.shared.bytes;full.run();
    assert(full.values==std::vector<int32_t>({50,7,40}) && full.choiceCalls==2 && full.shared.bytes==sharedBefore);
    Fixture flipped;flipped.data[0].statusFlags=0x10;flipped.data[2].belong=0;flipped.sides[2]=0;flipped.run();
    assert(flipped.values==std::vector<int32_t>({-9,7,40}));
    Fixture empty;empty.counts[1]=empty.counts[2]=0;empty.run();
    assert(empty.values==std::vector<int32_t>({160,7,40}) && empty.choiceCalls==0);
    Fixture emptyFlip;emptyFlip.data[0].statusFlags=0x10;emptyFlip.counts[1]=emptyFlip.counts[2]=0;emptyFlip.run();
    assert(emptyFlip.values==std::vector<int32_t>({160,7,40})); // Empty branch ignores Q.
    Fixture nesting;nesting.nest=true;nesting.run();
    assert(nesting.values==std::vector<int32_t>({160,7,40,50,7,40}));
    assert(nesting.outputDepths==std::vector<int32_t>({1,1,1,0,0,0}));
    assert(nesting.shared.bytes==sharedBefore && nesting.nested);
    std::cout<<"targeting integration tests passed\n";
}
