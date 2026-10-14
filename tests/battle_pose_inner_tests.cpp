#include "battle_pose_inner.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace ttyd;
namespace {
std::shared_ptr<const ActorAnimationPack> fixture(bool hiddenDefault=false){
    FILE* f=std::tmpfile();assert(f);
    auto put=[&](const auto& value){assert(std::fwrite(&value,sizeof(value),1,f)==1);};
    const std::array<char,8> magic{{'P','M','A','N','I','M','1',0}};put(magic);put(uint32_t(2));
    for(unsigned index=0;index<2;++index){
        std::array<char,32> name{};std::strcpy(name.data(),index?"walk":"default");put(name);put(uint32_t(index));put(0.f);put(10.f);
        put(std::array<uint32_t,6>{{3,24,1,3,1,2}});
        put(std::array<ActorVector,3>{{{{0,0,0}},{{1,0,0}},{{0,1,0}}}});
        std::array<float,24> nodes{};nodes[3]=nodes[4]=nodes[5]=1;put(nodes);
        put(ActorGroup{-1,-1,0,0,hiddenDefault && index==0?0u:1u});
        put(std::array<ActorVertexBinding,3>{{{0,0,0,0,0},{0,1,0,1,0},{0,2,0,0,1}}});
        ActorTextureBinding texture{};texture.cells.fill(0xffff);texture.cells[0]=0;put(texture);
        put(0.f);put(std::array<uint32_t,3>{{0,0,0}});
        put(10.f);put(std::array<uint32_t,3>{{1,0,0}});put(ActorSparseUpdate{0,16,0,0,0});
    }
    std::rewind(f);auto bank=std::make_shared<ActorAnimationPack>();assert(bank->read(f));std::fclose(f);return bank;
}
}
struct Trace {
    BattlePosePool pool;std::optional<uint32_t> pass;BattlePoseInnerCursor cursor;
    BattlePartDrawRequest draw;std::vector<uint32_t> calls;std::vector<BattleAudienceGXRequest> gx;
    std::vector<BattleAudienceMatrixRequest> matrices;std::vector<BattlePoseMapGroup> groups;
    unsigned mapReads=0,flushes=0,mainCalls=0,publications=0,traversals=0;int holdMapAt=-1;bool holdMap=false,holdGx=false,holdMain=false,holdFlush=false,hook=false;
    BattlePoseInnerContext context;
    Trace(){auto r=std::make_shared<BattlePoseResource>();r->modelName="model";r->textureName="texture";r->names={"default","walk"};r->bank=fixture();r->modelFlags=1;r->discrete.resize(2);
        const auto h=pool.enter(r,2,{});assert(h);draw.handle=*h;draw.pass=2;draw.scale=2;draw.angle=90;draw.matrix={1,0,0,3,0,1,0,4,0,0,1,5};
        pool.pose(*h)->word(0xf0,0xffffff80);pool.pose(*h)->word(0xe8,0x40);
    }
    BattlePoseInnerServices services(){BattlePoseInnerServices s;s.user=this;
        s.mapGroup=[](void* p)->std::optional<BattlePoseMapGroup>{auto& t=*static_cast<Trace*>(p);if(t.holdMap || int(t.mapReads)==t.holdMapAt)return {};assert(t.mapReads<t.groups.size());return t.groups[t.mapReads++];};
        s.gx=[](void* p,const BattleAudienceGXRequest& r){auto& t=*static_cast<Trace*>(p);if(t.holdGx)return false;t.gx.push_back(r);t.calls.push_back(r.function);return true;};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& t=*static_cast<Trace*>(p);BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);assert(math.matrix(nullptr,r));t.matrices.push_back(r);return true;};
        s.main=[](void* p,int32_t h){auto& t=*static_cast<Trace*>(p);++t.mainCalls;t.pool.pose(h)->word(0x28,19);t.pool.pose(h)->word(0x2c,20);t.pool.pose(h)->word(0x3c,0);return !t.holdMain;};
        s.publish=[](void* p,const BattlePoseInnerContext& c){auto& t=*static_cast<Trace*>(p);++t.publications;t.context=c;return true;};
        s.flush=[](void* p,unsigned i,const BattlePoseInnerContext&){auto& t=*static_cast<Trace*>(p);if(t.holdFlush && i==1)return false;assert(i==t.flushes);++t.flushes;t.calls.push_back(0x80296a54);return true;};
        s.invalidateVertices=[](void* p){static_cast<Trace*>(p)->calls.push_back(0x802b4df4);return true;};
        s.traverse=[](void* p,const BattlePoseInnerContext& c){auto& t=*static_cast<Trace*>(p);++t.traversals;assert(c.pass==2 && c.current==0 && c.next==0);t.calls.push_back(0x8003c4bc);return true;};
        s.callback=[](void* p,uint32_t fn,uint32_t pass){auto& t=*static_cast<Trace*>(p);assert(fn==0x1234 && pass==3);t.hook=true;return true;};return s;
    }
    bool resume(){return resumeBattlePoseInner(pool,0,pass,draw,services(),cursor);}
};
int main(){
    Trace t;t.holdGx=true;assert(!t.resume() && t.cursor.pending==0x802b93d8 && t.mapReads==0);
    t.pool.pose(t.draw.handle)->word(0xf0,0xffffffff);t.draw.angle=0;t.holdGx=false;t.holdFlush=true;
    assert(!t.resume() && t.cursor.pending==0x80296a54 && t.pass==3 && t.publications==1 && t.flushes==1 && t.traversals==0);
    assert(t.gx.size()==4 && t.gx[0].arguments[2]==0 && t.gx[1].arguments[0]==1 && t.gx[2].arguments[0]==1 && t.gx[3].arguments[0]==7);
    assert(t.matrices.size()==5 && t.context.model[3]==3 && t.context.model[7]==4 && t.context.model[11]==5);
    assert(t.context.model[2]>1.99f && t.context.model[8]<-1.99f); // pinned90-degree rotation precedes scale
    t.holdFlush=false;assert(t.resume() && t.flushes==2 && t.mainCalls==1 && t.publications==1 && t.traversals==1 && t.gx.size()==4);
    assert(t.calls[t.calls.size()-2]==0x802b4df4 && t.calls.back()==0x8003c4bc);
    Trace opaque;opaque.draw.angle=0;opaque.draw.scale=1;opaque.pool.pose(opaque.draw.handle)->word(0xf0,0xffffffff);opaque.groups={{0,255,255},{4,255,254}};
    assert(opaque.resume() && opaque.mapReads==2 && opaque.pass==3 && opaque.matrices.size()==1);
    Trace hook;hook.pass=99;hook.pool.pose(hook.draw.handle)->word(0xe0,0x1234);assert(hook.resume() && hook.hook && hook.gx.empty() && hook.pass==99);
    Trace facing;facing.pool.pose(facing.draw.handle)->word(0,9);assert(!facing.resume() && facing.cursor.pending==0x8000f2d0 && facing.matrices.empty());
    facing.pool.pose(facing.draw.handle)->word(0,1);assert(!facing.resume() && facing.cursor.pending==0x8000f2d0); // held path never reclassified
    Trace mainHold;mainHold.holdMain=true;
    assert(!mainHold.resume() && mainHold.cursor.pending==0x8003ed2c && mainHold.mainCalls==1 && mainHold.publications==0);
    mainHold.holdMain=false;assert(mainHold.resume() && mainHold.mainCalls==2 && mainHold.gx.size()==4);
    Trace ring;ring.pool.pose(ring.draw.handle)->word(4,2);
    assert(!ring.resume() && ring.cursor.pending==0x8003bc68 && ring.mainCalls==0 && ring.publications==0);
    Trace mode;mode.pool.pose(mode.draw.handle)->word(0xc,1);assert(mode.resume() && mode.gx.empty() && !mode.pass);
    // Fresh branch2 map snapshot, and a completed branch1 is not replayed.
    Trace map;auto s=map.services();BattlePoseMapCursor c;map.groups={{0,255,255},{4,255,0}};
    c.step=2;assert(battlePoseMapTranslucent(0,0xffffffff,0xffffffff,s,c)==false); // first supplied record has no mask4
    c={};map.mapReads=0;map.holdMap=true;assert(!battlePoseMapTranslucent(0,0xffffffff,0xffffffff,s,c));assert(c.step==1);
    map.holdMap=false;assert(battlePoseMapTranslucent(0,0xffffffff,0xffffffff,s,c)==true && map.mapReads==2);
    BattlePoseMapCursor second;map.mapReads=0;map.holdMapAt=1;
    assert(!battlePoseMapTranslucent(0,0xffffffff,0xffffffff,s,second) && second.step==2 && map.mapReads==1);
    map.groups[0]={2,0,0};map.holdMapAt=-1;
    assert(battlePoseMapTranslucent(0,0xffffffff,0xffffffff,s,second)==true && map.mapReads==2);
    BattlePoseMapCursor disabled;assert(battlePoseMapTranslucent(0x1800,0xffffffff,0xffffffff,{},disabled)==false);
}
