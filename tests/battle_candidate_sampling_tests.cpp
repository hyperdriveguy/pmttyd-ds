#include "battle_candidate_sampling.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t word(const BattleCandidateBuffer& b,unsigned o) {return uint32_t(b.bytes[o])<<24|uint32_t(b.bytes[o+1])<<16|uint32_t(b.bytes[o+2])<<8|b.bytes[o+3];}
int32_t half(const BattleCandidateBuffer& b,unsigned o) {const uint16_t v=uint16_t(b.bytes[o])<<8|b.bytes[o+1];return v<32768?v:int32_t(v)-65536;}
struct Fixture {
    BattleCandidateBuffer buffer;std::array<DamageUnit,3> data{};
    std::array<uint8_t,3> counts{{1,1,1}};std::array<float,3> scales{{1,1,1}};
    std::array<SamplingUnit,3> units{};
    std::array<std::array<BattlePositionPart,2>,3> positions{};
    std::array<std::array<SamplingPart,2>,3> parts{};
    std::array<uint32_t,2> info{{0,1}};bool noInfo=false;
    int work=1,captures=0,lookups=0,monoCalls=0,worldCalls=0;std::vector<int> choices;
    Fixture() {
        buffer.bytes.fill(0xaa);
        for(int i=0;i<3;++i) {
            data[i].kind=i==0?222:0;data[i].belong=i==0?0:1;data[i].attributes=2;
            units[i]={&data[i],i,&counts[i],&parts[i][0],&scales[i]};
            for(int j=0;j<2;++j) {
                positions[i][j].id=j+1;positions[i][j].attributes=0x10000001;
                positions[i][j].absolute={float(10+10*i),0,0};
                parts[i][j].position=&positions[i][j];parts[i][j].direction=1;
            }
            parts[i][0].next=&parts[i][1];
        }
    }
    CandidateSamplingServices services() {
        CandidateSamplingServices s;s.user=this;
        s.captureWork=[](void* p)->void* {auto& f=*static_cast<Fixture*>(p);++f.captures;return &f.work;};
        s.unit=[](void* p,void* work,int32_t id)->SamplingUnit* {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);++f.lookups;return id>=0 && id<3?&f.units[id]:nullptr;};
        s.part=[](void* p,int32_t id,int32_t part) {auto& f=*static_cast<Fixture*>(p);assert(id>=0 && id<3 && part>=1 && part<=2);return &f.parts[id][part-1];};
        s.status27=[](void*,SamplingUnit& u) {return checkStatus(u.data,27);};
        s.worldPosition=[](void* p,SamplingPart& part) {auto& f=*static_cast<Fixture*>(p);++f.worldCalls;JumpVector v;getPartWorldPosition(*part.position,&v.x,&v.y,&v.z);return v;};
        s.truncate=[](void*,float v)->int32_t {assert(std::isfinite(v) && double(v)>=-2147483648.0 && double(v)<2147483648.0);return static_cast<int32_t>(v);};
        s.monosiri=[](void* p,int32_t)->const uint32_t* {auto& f=*static_cast<Fixture*>(p);++f.monoCalls;return f.noInfo?nullptr:f.info.data();};return s;
    }
    int run(uint32_t a=0,uint32_t c=0,int dir=1) {return sampleBattleCandidates(buffer,nullptr,0,0,a,c,dir,services());}
    void single() {counts[0]=counts[2]=0;}
};
}
int main() {
    Fixture normal;assert(normal.run()==0 && normal.buffer.bytes[0xa6c]==3 && normal.buffer.bytes[0xab7]==0);
    assert(normal.buffer.bytes[0xa6d]==0 && normal.buffer.bytes[0xa6e]==1 && normal.buffer.bytes[0xa6f]==2);
    assert(normal.buffer.bytes[14]==0xaa && normal.buffer.bytes[15]==0xaa && normal.buffer.bytes[34]==0xaa && normal.buffer.bytes[35]==0xaa);
    Fixture high;assert(high.run(0x80000000)==0 && high.buffer.bytes[0xa6c]==255 && high.buffer.bytes[0xab7]==0xaa && high.buffer.bytes[4]==0xaa);
    assert(high.captures==2 && high.lookups==1 && high.worldCalls==0);
    Fixture swap;swap.data[0].statusFlags=0x10;assert(swap.run(0x200)==1 && word(swap.buffer,0xac0)==0x100 && swap.buffer.bytes[0xa6c]==2);
    const auto mask=word(swap.buffer,0xac0);updateBattleCandidates(swap.buffer,swap.services());assert(word(swap.buffer,0xac0)==mask);
    Fixture both;both.data[0].statusFlags=0x10;assert(both.run(0x300)==0 && both.buffer.bytes[0xa6c]==0);
    Fixture geometry;geometry.single();geometry.data[1].attributes=1;geometry.scales[1]=2;
    geometry.positions[1][0].absolute={100,10,10};geometry.positions[1][0].offset.y=3;
    geometry.parts[1][0].direction=-1;geometry.parts[1][0].forwardBias=7;
    geometry.parts[1][0].samplingOffsets={JumpVector{1,2,3},JumpVector{4,5,6}};geometry.run();
    assert(half(geometry.buffer,8)==90 && half(geometry.buffer,10)==30 && half(geometry.buffer,12)==28);
    assert(word(geometry.buffer,4+0xc)==393 && word(geometry.buffer,4+0x10)==16 && word(geometry.buffer,4+0x14)==15 && word(geometry.buffer,4+0x18)==293);
    assert(geometry.buffer.bytes[4+0x1c]==29 && geometry.buffer.bytes[4+0x1d]==1 && word(geometry.buffer,4+0x20)==0xffffffff);
    assert(samplingForwardLevel(-800,1,normal.services())==0 && samplingForwardLevel(5000,1,normal.services())==48 && samplingForwardLevel(-800,-1,normal.services())==48);
    struct Admission {uint32_t mask;int kind;int belong;uint32_t part;};
    for(auto t:std::vector<Admission>{{0x10000,0,1,1},{0x20000,0,1,1},{1,222,1,1},{2,224,1,1},{0x10,1,1,1},{0x40,218,1,1},{0x20,216,1,1},{0x100,0,0,1},{0x200,0,1,1},{0x2000,222,1,1},{0x4000,0,1,1},{0x100000,0,1,0},{0x100000,0,1,0x40001},{0x200000,0,1,0},{0x200000,0,1,0x11}}) {
        Fixture f;f.single();f.data[1].kind=t.kind;f.data[1].belong=t.belong;f.positions[1][0].attributes=0x10000000|t.part;f.run(t.mask);assert(f.buffer.bytes[0xa6c]==0);
    }
    Fixture self;self.single();self.units[1].id=0;self.run(0x1000);assert(self.buffer.bytes[0xa6c]==0);
    Fixture neutral;neutral.single();neutral.data[1].belong=2;neutral.run(0x200);assert(neutral.buffer.bytes[0xa6c]==1);
    struct Attribute {uint32_t mask,unit,part;};
    for(auto t:std::vector<Attribute>{{2,4,1},{4,2,1},{8,4,1},{0x10,0,1},{0x1000,2,0x100001},{0x2000,2,0x200001},{0x4000,2,0x400001},{0x8000,2,1},{0x2000000,0x20,1},{0x4000000,2,0x2000001}}) {
        Fixture f;f.single();f.data[1].attributes=t.unit;f.positions[1][0].attributes=0x10000000|t.part;f.run(0,t.mask);assert(f.buffer.bytes[0xa6c]==0);
    }
    Fixture exception;exception.single();exception.units[1].id=0;exception.positions[1][0].attributes|=0x200000;exception.run(0,0x102000);assert(exception.buffer.bytes[0xa6c]==1);
    Fixture info;info.single();info.run(0,1);assert(info.monoCalls==2 && info.buffer.bytes[0xa6c]==1);
    Fixture missing;missing.single();missing.noInfo=true;missing.run(0,1);assert(missing.monoCalls==1 && missing.buffer.bytes[0xa6c]==0);
    Fixture absentStatus;setStatus(absentStatus.data[1],27,1,0);absentStatus.data[2].attributes|=0x40;absentStatus.run();assert(absentStatus.buffer.bytes[0xa6c]==1);
    Fixture skippedPart;skippedPart.positions[1][0].attributes|=0x10000;skippedPart.run();assert(skippedPart.buffer.bytes[0xa6c]==2 && half(skippedPart.buffer,4)==0 && half(skippedPart.buffer,40)==2);
    Fixture dominant;dominant.single();dominant.counts[1]=2;dominant.positions[1][1].attributes=0x10000002;dominant.run(0x100000);assert(dominant.buffer.bytes[0xa6c]==1 && half(dominant.buffer,6)==1);
    Fixture alreadyRemoved;alreadyRemoved.single();alreadyRemoved.counts[1]=2;alreadyRemoved.positions[1][0].attributes|=0x400000;alreadyRemoved.positions[1][1].attributes=0x10000002;alreadyRemoved.run(0x100000,0x4000);assert(alreadyRemoved.buffer.bytes[0xa6c]==1 && half(alreadyRemoved.buffer,6)==2);
    Fixture occlusion;occlusion.run(0,0x1000000);assert(occlusion.buffer.bytes[0xa6c]==1 && half(occlusion.buffer,4)==0);
    Fixture reverse;reverse.run(0,0x1000000,-1);assert(reverse.buffer.bytes[0xa6c]==1 && half(reverse.buffer,4)==2);
    Fixture actorException;actorException.data[0].statusFlags=0x10;actorException.run(0,0x1000000);assert(actorException.buffer.bytes[0xa6c]==2 && half(actorException.buffer,40)==1);
    Fixture lane;lane.positions[0][0].absolute.z=-100;lane.run(0,0x1000000);assert(lane.buffer.bytes[0xa6c]==2);
    for(int direction:{1,0,-1}) {
        Fixture ties;for(auto& p:ties.positions)p[0].absolute.x=10;ties.run(0,0x1000000,direction);
        assert(ties.buffer.bytes[0xa6c]==3 && ties.buffer.bytes[0xa6d]==(direction>0?0:2) && half(ties.buffer,4)==0 && half(ties.buffer,40)==1);
    }
    Fixture biased;biased.data[1].attributes=1;biased.run();
    assert(biased.buffer.bytes[0xa6d]==0 && biased.buffer.bytes[0xa6e]==2 && biased.buffer.bytes[0xa6f]==1 && half(biased.buffer,40)==1);
    for(int z:{-35,-36}) {
        Fixture boundary;boundary.single();boundary.positions[1][0].absolute.z=z;boundary.run();
        assert(boundary.buffer.bytes[4+0x1d]==(z==-35?1:0));
    }
    Fixture takeoff;
    assert(sampleBattleCandidates(takeoff.buffer,nullptr,0,1,0x01001240,0x01000000,-1,takeoff.services())==0);
    CandidateChoiceServices choice;choice.user=&takeoff;
    choice.captureWork=[](void* p)->void* {return &static_cast<Fixture*>(p)->work;};
    choice.unit=[](void* p,void* work,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work && id>=0 && id<3);return &f.data[id];};
    choice.output=[](void* p,bool,int32_t value) {static_cast<Fixture*>(p)->choices.push_back(value);};
    choice.truncate=[](void*,float value)->int32_t {return static_cast<int32_t>(value);};
    chooseBattleCandidate(takeoff.buffer,4,choice); // No random service supplied: deterministic mode4.
    assert(takeoff.choices==std::vector<int>({-1,0,2,1,2,1}));
    std::cout<<"candidate sampling tests passed\n";
}
