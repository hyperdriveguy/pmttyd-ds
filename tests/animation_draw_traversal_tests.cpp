#include "animation_draw_traversal.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <vector>
using namespace ttyd;
struct Trace {int hold=-1;std::vector<BattleAudienceMatrixRequest> calls;
    static bool matrix(void* p,BattleAudienceMatrixRequest& r){auto& t=*static_cast<Trace*>(p);if(t.hold==int(t.calls.size()))return false;BattleAudienceAuxServices s;bindAudienceAuxMathAdapter(s);assert(s.matrix(nullptr,r));t.calls.push_back(r);return true;}
};
std::shared_ptr<AnimationDrawResource> rawFixture(bool cycle=false){
    std::vector<uint8_t> raw(0x800);auto word=[&](unsigned o,uint32_t v){for(unsigned i=0;i<4;++i)raw[o+i]=uint8_t(v>>(24-8*i));};
    std::memcpy(raw.data()+4,"model",6);std::memcpy(raw.data()+0x44,"texture",8);
    word(0xc4,1);word(0xe8,1);word(0x14c,0x1b0);word(0x13c,3);word(0x1a0,0x260);word(0x140,72);word(0x1a4,0x280);word(0x144,3);word(0x1a8,0x400);word(0x148,1);word(0x1ac,0x600);
    std::memcpy(raw.data()+0x600,"fixture",8);
    for(unsigned i=0;i<3;++i){const auto g=0x400+88*i;word(g+0x40,i==1?0:0xffffffff);word(g+0x44,i==2?1:0xffffffff);word(g+0x48,i==0?0:0xffffffff);word(g+0x4c,i);word(g+0x50,24*i);word(g+0x54,i==2?0:1);}
    if(cycle)word(0x400+0x40,2);
    FILE* f=std::tmpfile();assert(f && std::fwrite(raw.data(),raw.size(),1,f)==1);std::rewind(f);auto r=std::make_shared<AnimationDrawResource>();const bool ok=r->read(f);std::fclose(f);if(cycle){assert(!ok);return {};}assert(ok);return r;
}
int main(){
    for(const auto& spec:std::array<std::pair<const char*,unsigned>,3>{{{"romfs/trio-attack-mario.draw-model.bin",42},{"romfs/trio-attack-goombella.draw-model.bin",22},{"romfs/professor.draw-model.bin",19}}}){
        const auto r=AnimationDrawResource::load(spec.first);assert(r && r->groups()==spec.second);const auto root=r->group(r->groups()-1);assert(root.shape==-1 && root.node+24<=r->word(0x140));
    }
    rawFixture(true);
    std::array<float,24> n{};n[3]=n[4]=n[5]=1;n[0]=2;
    Trace trace;trace.hold=2;AnimationNodeCursor node;std::vector<BattleActMatrix> stack{{1,0,0,0,0,1,0,0,0,0,1,0}};
    assert(!resumeAnimationNodeDraw(n,false,{1,1,1},stack,&trace,Trace::matrix,node) && stack.size()==2 && node.pending==0x8029dd64);
    n[0]=999;stack[0][3]=1000;trace.hold=-1;
    assert(resumeAnimationNodeDraw(n,false,{1,1,1},stack,&trace,Trace::matrix,node) && stack.size()==2 && stack.back()[3]==2);
    // Literal joint Z-only scale gates, and unconditional rotation Identity.
    n={};n[3]=n[4]=1;n[5]=2;node={};stack.resize(1);stack[0]={1,0,0,0,0,1,0,0,0,0,1,0};trace.calls.clear();
    assert(resumeAnimationNodeDraw(n,true,{1,1,2},stack,&trace,Trace::matrix,node) && stack.back()[10]==1 && trace.calls.size()==3);
    // Zero pivots suppress nonzero offsets for transform nodes.
    n={};n[3]=n[4]=n[5]=1;n[18]=9;n[21]=8;node={};stack.resize(1);trace.calls.clear();
    assert(resumeAnimationNodeDraw(n,false,{1,1,1},stack,&trace,Trace::matrix,node) && stack.back()[3]==0 && trace.calls.size()==3);
    // Mixed transform pivots retain both six-call SDK graphs before dispatch.
    n={};n[0]=8;n[1]=9;n[2]=10;n[3]=2;n[4]=3;n[5]=4;n[6]=45;
    n[13]=2;n[18]=1;n[19]=7;n[20]=3;n[15]=1;n[16]=2;n[17]=3;n[21]=4;n[22]=5;n[23]=6;
    node={};stack.resize(1);trace.calls.clear();
    assert(resumeAnimationNodeDraw(n,false,{1,1,1},stack,&trace,Trace::matrix,node) && trace.calls.size()==20);
    assert(std::fabs(stack.back()[3]-12)<.00001f && std::fabs(stack.back()[7]-21)<.00001f && std::fabs(stack.back()[11]-12)<.00001f);
    n[7]=11;n[8]=23;node={};stack.resize(1);trace.calls.clear();
    assert(resumeAnimationNodeDraw(n,false,{1,1,1},stack,&trace,Trace::matrix,node) && node.count==25 && trace.calls.size()==24);
    n={};n[3]=n[4]=n[5]=1;for(unsigned i=0;i<3;++i){n[9+i]=float(i+1);n[6+i]=float(i+4);}
    node={};stack.resize(1);trace.calls.clear();assert(resumeAnimationNodeDraw(n,true,{1,1,1},stack,&trace,Trace::matrix,node));
    unsigned rotations=0;constexpr float angles[]{3,2,1,12,10,8};
    for(const auto& call:trace.calls)if(call.function==0x8029dff0){assert(call.axis==122-rotations%3 && call.radians==angles[rotations]*.01745329238474369f);++rotations;}
    assert(rotations==6 && trace.calls.size()==18);
    auto resource=std::make_shared<BattlePoseResource>();resource->draw=rawFixture();resource->modelFlags=1;
    // Enter an ordinary existing real bank, then replace only this test's
    // explicit typed source fixture; no actual game history is fabricated.
    const auto real=loadBattlePoseResource("romfs/trio-attack-mario.pose-directory.bin","romfs/trio-attack-mario.pmanim");assert(real && real->draw);
    BattlePosePool pool;const auto handle=pool.enter(real,1,{});assert(handle);auto* p=pool.pose(*handle);p->shape.visibility={1,0,1};p->shape.nodes.assign(72,0);
    for(unsigned i=0;i<3;++i)for(unsigned k=3;k<6;++k)p->shape.nodes[24*i+k]=1;
    p->shape.nodes[0]=1;p->shape.nodes[24+3]=2;p->shape.nodes[24+4]=3;p->shape.nodes[24+5]=4;p->shape.nodes[48]=10;
    BattlePoseInnerContext context;context.handle=*handle;context.resource=resource;context.current=7;context.pass=2;context.model={1,0,0,0,0,1,0,0,0,0,1,0};
    struct DrawTrace:Trace {bool holdShape=true;std::vector<int32_t> rendered;BattleActMatrix model{};} draw;
    AnimationDrawTraversalServices services;services.user=&draw;services.matrix=Trace::matrix;
    services.render=[](void* u,const AnimationShapeRequest& r,uint32_t& pending){auto& t=*static_cast<DrawTrace*>(u);assert(r.key==7 && r.pass==2);if(r.shape==0 && t.holdShape){pending=0x8000f2e4;return false;}t.rendered.push_back(r.shape);t.model=r.model;return true;};
    AnimationDrawTraversalCursor cursor;assert(!resumeAnimationDrawTraversal(pool,context,services,cursor) && cursor.pending==0x8000f2e4 && draw.rendered==std::vector<int32_t>{-1});
    const auto calls=draw.calls.size();draw.holdShape=false;assert(resumeAnimationDrawTraversal(pool,context,services,cursor));
    assert(draw.rendered==std::vector<int32_t>({-1,0}) && draw.calls.size()==calls && cursor.stack.size()==1 && draw.model[3]==11 && draw.model[0]==.5f);
    // Callback receives the actual mutable stack matrix; a held invocation
    // retains its identity while the following child reads fresh pose fields.
    struct CallbackTrace:DrawTrace {bool held=false;unsigned attempts=0;} callback;
    services.user=&callback;p->word(0x154,0x1234);p->word(0x158,0x5678);
    services.callback=[](void* u,uint32_t fn,uint32_t data,uint32_t group,BattleActMatrix& m){auto& t=*static_cast<CallbackTrace*>(u);assert(fn==0x1234 && data==0x5678 && group==2);++t.attempts;if(!t.held){m[3]+=3;t.held=true;return false;}return true;};
    cursor={};assert(!resumeAnimationDrawTraversal(pool,context,services,cursor) && cursor.pending==0x1234 && callback.attempts==1);
    p->word(0x154,0);callback.holdShape=false;
    assert(resumeAnimationDrawTraversal(pool,context,services,cursor) && callback.attempts==2 && callback.model[3]==14);
}
