#include "battle_enemy_hp_raster.hpp"
#include <cassert>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Event {
 bool math=false;BattleTexturePlaneRequest q;BattleAudienceMatrixRequest m;
 bool operator==(const Event& b)const{return math==b.math && q.kind==b.q.kind && q.function==b.q.function && q.integers==b.q.integers && q.reals==b.q.reals && q.matrix==b.q.matrix && m.function==b.m.function && m.left==b.m.left && m.right==b.m.right && m.xyz==b.m.xyz;}
};
struct Fixture {
 std::map<uint32_t,uint32_t> memory;
 std::vector<uint32_t> reads;std::vector<Event> events;
 int stop=-1;bool released=false,late=false;uint32_t format=14;
 BattleEnemyHpRasterRequest input;
 Fixture(){
   memory={{0x8041eb90,0x10000},{0x263f8,0x30000},{0x300a0,0x40000},{0x40000,0x50000},
     {0x8042224c,bits(1)},{0x80422240,bits(0)},{0x80422234,bits(.5f)},
     {0x802ee400,0x43300000},{0x802ee404,0}};
   BattleActMatrix view{1,0,0,10,0,1,0,20,0,0,1,30};
   for(unsigned i=0;i<12;++i)memory[0x2011c+4*i]=bits(view[i]);
 }
 bool admit(const Event& e){if(int(events.size())==stop && !released)return false;events.push_back(e);return true;}
 BattleTexturePlaneServices services(){
   BattleTexturePlaneServices s;s.user=this;s.actors.memory.user=this;
   s.actors.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.memory.at(a);};
   s.request=[](void* p,const BattleTexturePlaneRequest& q)->std::optional<uint32_t>{
     auto& f=*static_cast<Fixture*>(p);if(!f.admit({false,q,{}}))return {};
     if(q.kind!=BattleTexturePlaneKind::Call)return 0;
     switch(q.function){
       case BattleTexturePlaneCall::CurrentCamera:return 0x20000;
       case BattleTexturePlaneCall::LoadMatrix:if(f.late)f.input.integers[1]=0xaabbccdd;break;
       case BattleTexturePlaneCall::TextureObject:assert(q.integers==std::vector<uint32_t>({0x50000,17}));return 0x1234;
       case BattleTexturePlaneCall::TextureFormat:return f.format;
       case BattleTexturePlaneCall::TextureWidth:return 40;
       case BattleTexturePlaneCall::TextureHeight:return 16;
       default:break;
     }return 0;
   };
   s.matrix=[](void* p,BattleAudienceMatrixRequest& m){
     auto& f=*static_cast<Fixture*>(p);if(!f.admit({true,{},m}))return false;
     switch(m.function){
       case 0x8029e2b0:m.output={m.xyz[0],0,0,0,0,m.xyz[1],0,0,0,0,m.xyz[2],0};break;
       case 0x8029e230:m.output={1,0,0,m.xyz[0],0,1,0,m.xyz[1],0,0,1,m.xyz[2]};break;
       case 0x8029dd64:
         for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c){float value=c==3?m.left[r*4+3]:0;for(unsigned k=0;k<3;++k)value+=m.left[r*4+k]*m.right[k*4+c];m.output[r*4+c]=value;}
         break;
       default:assert(false);
     }return true;
   };return s;
 }
 bool run(BattleEnemyHpRasterCursor& c){return resumeBattleEnemyHpRaster(input,services(),c);}
 std::vector<BattleTexturePlaneRequest> fifo()const{std::vector<BattleTexturePlaneRequest> r;for(const auto& e:events)if(!e.math && e.q.kind!=BattleTexturePlaneKind::Call)r.push_back(e.q);return r;}
};
void everyHold(const BattleEnemyHpRasterRequest& input,uint32_t format=14){
 Fixture baseline;baseline.input=input;baseline.format=format;BattleEnemyHpRasterCursor base;assert(baseline.run(base) && !base.active());
 for(size_t stop=0;stop<baseline.events.size();++stop){
   Fixture f;f.input=input;f.format=format;f.stop=int(stop);BattleEnemyHpRasterCursor c;assert(!f.run(c) && c.active());
   const auto size=f.events.size(),reads=f.reads.size();assert(!f.run(c));assert(f.events.size()==size && f.reads.size()==reads);
   auto other=f.input;assert(!resumeBattleEnemyHpRaster(other,f.services(),c));
   auto wrong=f.services();wrong.user=&baseline;assert(!resumeBattleEnemyHpRaster(f.input,wrong,c));
   f.released=true;assert(f.run(c) && !c.active());assert(f.events==baseline.events && f.reads==baseline.reads);
 }
}
}
int main(){
 BattleEnemyHpRasterRequest init;init.function=0x80103b48;everyHold(init);
 Fixture i;i.input=init;BattleEnemyHpRasterCursor ic;assert(i.run(ic));assert(i.events.size()==19 && i.reads.empty());
 assert(uint32_t(i.events[1].q.function)==0x802b940c && i.events[1].q.integers==std::vector<uint32_t>{1});
 assert(uint32_t(i.events[2].q.function)==0x802b8d60 && i.events[2].q.integers==std::vector<uint32_t>({7,0,0,7,0}));
 assert(i.events[7].q.integers==std::vector<uint32_t>({4,0,1,1,0,2,2}));
 assert(i.events.back().q.integers==std::vector<uint32_t>({0,255,255,4}));
 auto noMemory=i.services();noMemory.actors.memory.word=nullptr;BattleEnemyHpRasterCursor nm;
 assert(resumeBattleEnemyHpRaster(init,noMemory,nm));
 BattleEnemyHpRasterRequest quad;quad.function=0x80103918;quad.integers={0x123,0x245,0x367,0x489};quad.reals={2,3,7,-5};everyHold(quad);
 Fixture f;f.input=quad;BattleEnemyHpRasterCursor c;assert(f.run(c));const auto fifo=f.fifo();assert(fifo.size()==28);
 const float expected[4][3]={{2,3,0},{7,3,0},{7,-5,0},{2,-5,0}};
 for(unsigned v=0;v<4;++v){for(unsigned lane=0;lane<3;++lane)assert(fifo[v*7+lane].reals==std::vector<float>{expected[v][lane]});for(unsigned lane=0;lane<4;++lane)assert(fifo[v*7+3+lane].integers==std::vector<uint32_t>{uint8_t(quad.integers[lane])});}
 assert(f.reads==std::vector<uint32_t>(4,0x80422240));
 // Each Point loads z immediately before its own held FIFO write.
 Fixture z;z.input=quad;z.stop=3;BattleEnemyHpRasterCursor zc;assert(!z.run(zc));z.memory[0x80422240]=bits(9);z.released=true;assert(z.run(zc));auto zs=z.fifo();assert(zs[2].reals==std::vector<float>{0});for(unsigned v=1;v<4;++v)assert(zs[v*7+2].reals==std::vector<float>{9});
 BattleEnemyHpRasterRequest plane;plane.function=0x801032c8;plane.integers={17,0x12345678,0};plane.reals={3,4,5,.5f,2};
 everyHold(plane,0);everyHold(plane,14);
 Fixture p;p.input=plane;p.late=true;BattleEnemyHpRasterCursor pc;assert(p.run(pc));
 unsigned mathCount=0;for(const auto& e:p.events)if(e.math)++mathCount;
 assert(mathCount==4);
 auto matrix=std::find_if(p.events.begin(),p.events.end(),[](const Event& e){return !e.math && e.q.function==BattleTexturePlaneCall::LoadMatrix;});
 assert(matrix!=p.events.end());assert(matrix->q.matrix==std::optional<BattleActMatrix>({.5f,0,0,13,0,2,0,24,0,0,1,35}));
 const auto pf=p.fifo();assert(pf.size()==36 && pf[0].reals==std::vector<float>{-20} && pf[1].reals==std::vector<float>{8});
 assert(pf[3].integers==std::vector<uint32_t>{0xaa} && pf[6].integers==std::vector<uint32_t>{0xdd});
 // Entry float registers and a held Scale's fresh source one are captured;
 // modifying caller registers or memory cannot replay that SDK operation.
 Fixture scale;scale.input=plane;scale.stop=13;BattleEnemyHpRasterCursor sc;
 assert(!scale.run(sc));const auto scaleReads=scale.reads.size();
 scale.input.reals={100,200,300,400,500};scale.memory[0x8042224c]=bits(9);
 assert(!scale.run(sc) && scale.reads.size()==scaleReads);scale.released=true;assert(scale.run(sc));
 assert((scale.events[13].math && scale.events[13].m.xyz==std::array<float,3>({.5f,2,1})));
 // Independently exposed PlainGX retains source byte-load read-ahead rather
 // than taking four early word snapshots or reading each byte at its store.
 for(unsigned heldLane:{0u,3u}){
   Fixture direct;std::array<uint32_t,4> colors{0x11223344,0x55667788,0x99aabbcc,0xddeeff00};
   BattleTexturePlaneCursor dc;
   assert(resumePackedBattleTexturePlainGX(17,colors,direct.services(),dc));
   size_t at=0;unsigned lane=0;
   while(at<direct.events.size()){
     if(direct.events[at].q.kind!=BattleTexturePlaneKind::Call){if(lane++==heldLane)break;}
     ++at;
   }
   Fixture held;held.stop=int(at);BattleTexturePlaneCursor hc;
   assert(!resumePackedBattleTexturePlainGX(17,colors,held.services(),hc));
   auto changed=colors;assert(!resumePackedBattleTexturePlainGX(17,changed,held.services(),hc));
   colors.fill(0xabcdef12);held.released=true;
   assert(resumePackedBattleTexturePlainGX(17,colors,held.services(),hc));auto h=held.fifo();
   if(heldLane==0){assert(h[3].integers[0]==0x11 && h[4].integers[0]==0x22 && h[5].integers[0]==0x33 && h[6].integers[0]==0x12);}
   else {assert(h[6].integers[0]==0x44 && h[12].integers[0]==0x55 && h[13].integers[0]==0xcd);}
 }
 BattleEnemyHpRasterRequest load;load.function=0x802ba2fc;load.integers={0};load.matrix=CameraViewMatrix{{{{1,2,3,4}},{{5,6,7,8}},{{9,10,11,12}}}};everyHold(load);
 Fixture missing;missing.input=plane;auto services=missing.services();services.request=nullptr;BattleEnemyHpRasterCursor mc;assert(!resumeBattleEnemyHpRaster(missing.input,services,mc));assert(missing.events.empty() && missing.reads.empty());
 BattleEnemyHpRasterRequest invalid;invalid.function=0x8010323c;BattleEnemyHpRasterCursor bad;assert(!resumeBattleEnemyHpRaster(invalid,missing.services(),bad) && !bad.active());
 std::cout<<"Original HP TexPlane, InitSub/Rasta, Quad/Point, late inputs and every boundary passed\n";
}
