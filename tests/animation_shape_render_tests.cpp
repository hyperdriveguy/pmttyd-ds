#include "animation_shape_render.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstring>
using namespace ttyd;
struct Trace {
    CameraState camera,other;bool currentOther=false,holdVertex=true;uint32_t hold=0;unsigned cameraCalls=0,mathCalls=0;
    std::vector<AnimationShapeCall> calls;
    static const CameraState* current(void* u){auto& t=*static_cast<Trace*>(u);++t.cameraCalls;return t.currentOther?&t.other:&t.camera;}
    static const CameraState* indexed(void* u,uint32_t i){assert(i==4);return &static_cast<Trace*>(u)->camera;}
    static bool concat(void*,BattleAudienceMatrixRequest& r){BattleAudienceAuxServices s;bindAudienceAuxMathAdapter(s);return s.matrix(nullptr,r);}
    static bool math(void* u,AnimationShapeMath& r){++static_cast<Trace*>(u)->mathCalls;return animationShapeNativeMath(r);}
    static bool call(void* u,const AnimationShapeCall& r){auto& t=*static_cast<Trace*>(u);if(r.function==t.hold || (r.function==0xcc008000 && t.holdVertex))return false;t.calls.push_back(r);return true;}
};
int main(){
    AnimationShapeMath math;math.function=0x8029df28;math.input={2,0,0,10,0,3,0,20,0,0,4,30};
    assert(animationShapeNativeMath(math) && math.status==1 && std::fabs(math.output[0]-.5f)<1e-6f && std::fabs(math.output[5]-1.f/3)<1e-6f && std::fabs(math.output[10]-.25f)<1e-6f && math.output[3]==0);
    const auto prior=math.output;math.input={};assert(animationShapeNativeMath(math) && math.status==0 && math.output==prior);
    // The original inverse never reads translation, including nonfinite words.
    const float inf=std::numeric_limits<float>::infinity(),nan=std::numeric_limits<float>::quiet_NaN();
    math.input={2,0,0,inf,0,3,0,nan,0,0,4,-inf};
    assert(animationShapeNativeMath(math) && math.status==1 && math.output==prior);
    // Retain source single-precision square overflow; do not widen/refactor the
    // refinement to conceal the resulting exceptional normal matrix.
    math.input={1e-8f,0,0,0,0,1e-8f,0,0,0,0,1e-8f,0};
    assert(animationShapeNativeMath(math) && math.status==1);
    assert(std::isinf(math.output[0]) && std::signbit(math.output[0]) && std::isnan(math.output[1]) && math.output[3]==0);
    // Native isolated ending capture: Mario shape17, raw view-model words.
    // Its finite determinant is approximately -8e-24; estimate squared overflows.
    const uint32_t finishingWords[]{0xac0cbccb,0x9d682054,0xb509063c,0x417c5438,
        0x1fb1a220,0x2c0b52b0,0xbe90d0be,0xc1c566fb,
        0xa1150476,0x2a9f39e7,0x3ffd6d55,0xc3b51091};
    static_assert(sizeof(finishingWords)==sizeof(math.input));
    std::memcpy(math.input.data(),finishingWords,sizeof(finishingWords));
    assert(animationShapeNativeMath(math) && math.status==1);
    for(unsigned row=0;row<3;++row){for(unsigned col=0;col<3;++col)assert(std::isinf(math.output[4*row+col]));assert(math.output[4*row+3]==0);}
    // Unordered source comparisons proceed with stores; no finite-value gate.
    math.input={nan,0,0,0,0,1,0,0,0,0,1,0};
    assert(animationShapeNativeMath(math) && math.status==1 && std::isnan(math.output[0]));
    math.input={inf,0,0,0,0,1,0,0,0,0,1,0};
    assert(animationShapeNativeMath(math) && math.status==1 && std::isnan(math.output[0]));
    // The independent point adapter still admits only finite positions.
    math.function=0x8029e6a4;math.input={1,0,0,inf,0,1,0,0,0,0,1,0};
    assert(!animationShapeNativeMath(math));
    math.function=0x8029e6a4;math.input={2,0,0,10,0,3,0,20,0,0,4,30};math.vector={1,2,3};
    assert(animationShapeNativeMath(math) && math.vectorOutput==ActorVector({12,26,42}));
    const auto resource=loadBattlePoseResource("romfs/trio-attack-goombella.pose-directory.bin","romfs/trio-attack-goombella.pmanim");assert(resource && resource->draw);
    BattlePosePool pool;const auto h=pool.enter(resource,1,{});assert(h);auto* p=pool.pose(*h);assert(pool.main(*h,{}));
    AnimationShapeRequest input;input.shape=15;input.key=777;input.pass=999;input.model={1,0,0,7,0,1,0,8,0,0,1,9};
    Trace t;t.camera.viewPublished=true;t.camera.matrices.view={{{1,0,0,10},{0,1,0,20},{0,0,1,30}}};
    AnimationShapeServices services;services.user=&t;services.currentCamera=Trace::current;services.cameraByIndex=Trace::indexed;services.concat=Trace::concat;services.math=Trace::math;services.call=Trace::call;
    AnimationShapeCursor c;std::optional<uint32_t> pass=2;t.hold=0x800208c0;
    assert(!resumeAnimationShape(pool,*h,pass,input,services,c) && c.pending==t.hold && c.request.origin==ActorVector({7,8,9}));
    assert(t.calls.size()==4 && t.calls[0].function==0x802ba2fc && (*t.calls[0].matrix)[3]==17 && t.calls[1].function==0x802ba34c && t.calls[2].function==0x802ba39c && t.calls[3].arguments[0]==0);
    const auto oldFlags=c.request.arguments[0];p->word(0xec,123);input.model[3]=500;
    assert(!resumeAnimationShape(pool,*h,pass,input,services,c) && c.request.arguments[0]==oldFlags && t.mathCalls==2 && t.calls.size()==4);
    t.hold=0;t.currentOther=true;
    assert(!resumeAnimationShape(pool,*h,pass,input,services,c) && c.pending==0xcc008000 && c.vertex==0);
    unsigned fog=0;for(const auto& r:t.calls)if(r.function==0x800217c8)++fog;assert(!fog && t.cameraCalls==2);
    const auto calls=t.calls.size();const auto held=c.request.indices;assert(!resumeAnimationShape(pool,*h,pass,input,services,c) && t.calls.size()==calls && c.request.indices==held);
    t.holdVertex=false;assert(resumeAnimationShape(pool,*h,pass,input,services,c));
    // Independent original table reader checks every original low16 stream,
    // shape base and fan count (22 calls, source draw15/call66).
    const auto& raw=*resource->draw;const auto shape=raw.word(0x14c)+168*15,draw=raw.word(0x19c)+108*15;
    assert(raw.word(draw)==1 && raw.word(draw+0x38)==66 && raw.word(draw+0x3c)==22 && raw.word(shape+0x40)==107);
    unsigned run=0,vertex=0,total=0;uint32_t first=0,count=0;
    for(const auto& r:t.calls){
        if(r.function==0x802b5a6c){assert(vertex==count);const auto call=raw.word(0x150)+8*(66+run++);first=raw.word(call);count=uint16_t(raw.word(call+4));vertex=0;assert(r.arguments[0]==0xa0 && r.arguments[2]==count);}
        if(r.function==0xcc008000){assert(vertex<count && r.arguments[0]==4);constexpr unsigned ptr[]{0x158,0x160,0x168,0x16c};constexpr unsigned off[]{0x40,0x44,0x48,0x4c};for(unsigned j=0;j<4;++j)assert(r.indices[j]==uint16_t(raw.word(raw.word(ptr[j])+4*(first+raw.word(draw+off[j])+vertex))));++vertex;++total;}
        if(r.function==0x802b4db0 && r.arguments[0]==9)assert(r.arguments[1]==107 && r.pose==p);
        if(r.function==0x802b4db0 && r.arguments[0]==10)assert(r.arguments[1]==0);
        if(r.function==0x8003dcfc)assert(r.pose==p && r.arguments[1]==draw+0x10 && raw.word(r.arguments[1])==9);
    }
    assert(run==22 && total>0 && vertex==count);
    // Two-coordinate admission fixture: reverse selectors, independent UV
    // streams, high index halves and a zero-count GXBegin are source cases.
    std::ifstream source("romfs/trio-attack-goombella.draw-model.bin",std::ios::binary);
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(source)),{});
    auto put=[&](uint32_t o,uint32_t v){for(unsigned j=0;j<4;++j)bytes[o+j]=uint8_t(v>>(24-8*j));};
    const auto extra=uint32_t(bytes.size());bytes.resize(bytes.size()+4*raw.word(0x108));
    for(uint32_t j=0;j<raw.word(0x108);++j)put(extra+4*j,0xabcd0000);
    put(0x10c,raw.word(0x108));put(0x170,extra);put(shape+0x60,0);put(draw,2);put(draw+0x14,9);
    bytes[draw+0x30]=0;bytes[draw+0x31]=1;put(draw+0x50,raw.word(draw+0x4c));
    put(raw.word(0x150)+8*66+4,0xffff0000); // Begin0 still occurs.
    FILE* f=std::tmpfile();assert(f && std::fwrite(bytes.data(),bytes.size(),1,f)==1);std::rewind(f);
    auto changed=std::make_shared<AnimationDrawResource>();assert(changed->read(f));std::fclose(f);
    auto owner=std::make_shared<BattlePoseResource>(*resource);owner->draw=changed;p->resource=owner;
    c={};pass=2;input.model={1,0,0,0,0,1,0,0,0,0,1,0};t=Trace{};t.holdVertex=false;t.camera.viewPublished=true;t.camera.matrices.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
    assert(resumeAnimationShape(pool,*h,pass,input,services,c));unsigned begins=0,indices=0,arrays=0;
    for(const auto& r:t.calls){
        if(r.function==0x802b5a6c){if(!begins)assert(r.arguments[2]==0);++begins;}
        if(r.function==0xcc008000){assert(r.arguments[0]==5 && r.indices[3]==0);++indices;}
        if(r.function==0x802b4db0 && r.arguments[0]>=13){assert(r.arguments[1]==raw.word(0x18c)+(r.arguments[0]==13?0:8*107));++arrays;}
    }
    assert(begins==22 && indices>0 && arrays==2);p->resource=resource;
    // Pass rejection still follows all camera/normal/current-matrix calls.
    t=Trace{};t.camera.viewPublished=true;t.camera.matrices.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};c={};pass=1;
    p->word(0xf0,0xffffffff);p->word(0xf4,0xffffffff);assert(resumeAnimationShape(pool,*h,pass,input,services,c) && t.calls.size()==3);
    input.shape=-1;t.calls.clear();assert(resumeAnimationShape(pool,*h,pass,input,services,c) && t.calls.empty());
    // Exceptional inverse results reach the normal upload before pass rejection.
    // They must not leave the shape stuck at the math-service boundary.
    input.shape=15;input.model={1e-8f,0,0,0,0,1e-8f,0,0,0,0,1e-8f,0};
    c={};t=Trace{};t.camera.viewPublished=true;t.camera.matrices.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
    assert(resumeAnimationShape(pool,*h,pass,input,services,c) && t.calls.size()==3);
    assert(t.calls[1].function==0x802ba34c && t.calls[1].matrix && std::isinf((*t.calls[1].matrix)[0]));
    // Singular source math succeeds with no normal output; a real backend
    // must retain the unavailable local rather than fabricate an identity.
    input.shape=15;input.model={};c={};t.calls.clear();t.hold=0x802ba34c;
    assert(!resumeAnimationShape(pool,*h,pass,input,services,c) && c.pending==0x802ba34c && !c.request.matrix);
}
