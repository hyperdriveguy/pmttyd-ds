#include "hit_n64_effect_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t n){float v;std::memcpy(&v,&n,4);return v;}
int32_t sign(uint32_t n){return n<0x80000000?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float convert(int32_t n,uint64_t bias){const uint64_t bits=0x4330000000000000ULL|uint64_t(uint32_t(n)^0x80000000u);double d,b;std::memcpy(&d,&bits,8);std::memcpy(&b,&bias,8);return float(d-b);}
bool same(const HitN64DrawServices& a,const HitN64DrawServices& b){return a.user==b.user && a.memory.user==b.memory.user && a.memory.word==b.memory.word && a.byte==b.byte && a.multiply==b.multiply && a.truncate==b.truncate && a.matrix==b.matrix && a.call==b.call && a.indices==b.indices;}
struct Guard{bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
struct Op{uint32_t function;std::array<uint32_t,8> args{};};
// Ordered original GX ABI:NumChans0,two TEV stages,stage color/alpha order,
// ops and selectors. Register IDs1/2 mean GX C0/C1. TexGen matrices30/33,
// position9/color11/UV13 INDEX16(3),S16/RGBA8 formats;Cull5c8c is mode0.
// Keep literal setter order/arguments rather than inferred host material flags.
constexpr Op material[]={
 {0x802b6f3c,{0}},{0x802b8fcc,{2}},{0x802b8e30,{0,0,0,255}},
 {0x802b89e8,{0,0,0,0,1,0}},{0x802b8a50,{0,0,0,0,1,0}},
 {0x802b8960,{0,15,15,15,8}},{0x802b89a4,{0,7,7,7,4}},
 {0x802b8e30,{1,1,1,255}},{0x802b89e8,{1,0,0,0,1,0}},
 {0x802b8a50,{1,0,0,0,1,0}},{0x802b8960,{1,4,2,0,15}},
 {0x802b89a4,{1,0,4,2,7}}};
constexpr Op coordinates[]={
 {0x802b5084,{2}},{0x802b4e04,{0,1,4,30,0,125}},
 {0x802b4e04,{1,1,4,33,0,125}},{0x802b5c8c,{0}}};
constexpr Op descriptors[]={
 {0x802b4520,{}},{0x802b3c08,{9,3}},{0x802b3c08,{11,3}},{0x802b3c08,{13,3}},
 {0x802b4558,{0,9,1,3,0}},{0x802b4558,{0,11,1,5,0}},
 {0x802b4558,{0,13,1,3,5}}};
// Whole tri2 FIFO:six(position,color,UV)U16 triples for triangles012 and023.
// A false FIFO consumer consumes none;partial triangles cannot be acknowledged.
constexpr std::array<uint16_t,18> triangles={0,0,0,1,1,1,2,2,2,0,0,0,2,2,2,3,3,3};
}
// Raw reads are synchronous;callReady/mathReady pin complete external request
// payloads across holds. Child cursor owns each pass;no successful setter/SDK
// stage replays. Copied matrices replace incidental original stack addresses.
struct HitN64DrawCursor::State {
    HitN64DrawServices owner;uint32_t operation=0,effect=0,cameraId=0,phase=0,pending=0,step=0;
    uint32_t payload=0,camera=0,age=0,kind=0,rootAlpha=0,texture=0,pass=0,index=1,particle=0,vertices=0,result=0;
    int32_t frame=0,alpha=0;float progress=0,scale=0,angleFactor=0,scaleX=0,scaleY=0;uint64_t bias=0;
    std::array<uint8_t,6> colors{};std::array<BattleActMatrix,6> matrices{};
    const BattleActMatrix* borrowed=nullptr;bool callReady=false,mathReady=false;
    SourceIconGXCall request;BattleAudienceMatrixRequest math;HitN64DrawCursor child;
    uint32_t word(uint32_t a)const{return owner.memory.word(owner.memory.user,a);}
    float real(uint32_t a)const{return scalar(word(a));}
    uint8_t byte(uint32_t a)const{return owner.byte(owner.user,a);}
    uint64_t wide(uint32_t a)const{const auto hi=word(a),lo=word(a+4);return uint64_t(hi)<<32|lo;}
    std::array<float,3> xyz(uint32_t a)const{const auto x=real(a),y=real(a+4),z=real(a+8);return {x,y,z};}
    BattleActMatrix matrixAt(uint32_t a)const{BattleActMatrix m;for(unsigned i=0;i<12;++i)m[i]=real(a+4*i);return m;}
    bool call(){pending=request.function;if(!owner.call)return false;const auto r=owner.call(owner.user,request);if(!r)return false;result=*r;callReady=false;pending=0;return true;}
    bool call(uint32_t fn,std::array<uint32_t,8> args={}){if(!callReady){request={};request.function=fn;request.integers=args;callReady=true;}return call();}
    template<size_t N>bool table(const Op(&ops)[N]){while(step<N){if(!call(ops[step].function,ops[step].args))return false;++step;}step=0;return true;}
    bool matrix(unsigned output){pending=math.function;if(!owner.matrix)return false;auto q=math;if(!owner.matrix(owner.user,q))return false;matrices[output]=q.output;mathReady=false;pending=0;return true;}
    void mathStart(uint32_t fn){math={};math.function=fn;mathReady=true;}
    bool translate(unsigned output,uint32_t at){if(!mathReady){mathStart(0x8029e230);math.xyz=xyz(at);}return matrix(output);}
    bool concat(unsigned output,unsigned left,unsigned right){if(!mathReady){mathStart(0x8029dd64);math.left=matrices[left];math.right=matrices[right];}return matrix(output);}
    bool loadMatrix(uint32_t fn,std::array<uint32_t,8> args,unsigned m){if(!callReady){request={};request.function=fn;request.integers=args;request.matrix=matrices[m];callReady=true;}return call();}
};
HitN64DrawCursor::HitN64DrawCursor()=default;HitN64DrawCursor::~HitN64DrawCursor()=default;
bool HitN64DrawCursor::active()const{return bool(state_);}uint32_t HitN64DrawCursor::pending()const{return state_?state_->pending:0;}
bool HitN64DrawCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Required initial camera query(result unused) precedes progress*source5.
// fctiwz provider result>4 exits;negative frames retain source addressing.
// Billboard base=T(root)*Ry(-camera4 yaw)*S(scale);angle literals are source.
// Fractional frame*256 supplies low8 alpha;root2c is read without invented255.
bool resumeHitN64Draw(int32_t camera,uint32_t effect,const HitN64DrawServices& s,HitN64DrawCursor& cursor){
    if(cursor.invoking_ || !s.memory.word || !s.byte || !s.multiply)return false;
    if(cursor.state_ && (cursor.state_->operation!=1 || cursor.state_->cameraId!=uint32_t(camera) || cursor.state_->effect!=effect || !same(cursor.state_->owner,s)))return false;
    Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<HitN64DrawCursor::State>();cursor.state_->owner=s;cursor.state_->operation=1;cursor.state_->cameraId=uint32_t(camera);cursor.state_->effect=effect;}
    auto& c=*cursor.state_;const auto next=[&](){++c.phase;};const auto finish=[&](){cursor.state_.reset();return true;};
    for(;;)switch(c.phase){
    case 0:if(!c.call(0x8000f2fc,{uint32_t(camera)}))return false;next();break;
    case 1:
        c.payload=c.word(effect+0xc);{const auto factor=c.real(0x80425458),progress=c.real(c.payload+0x38);c.age=c.word(c.payload+0x34);c.progress=s.multiply(s.user,factor,progress);c.kind=c.word(c.payload);}next();break;
    case 2:{c.pending=0x801c5dac;if(!s.truncate)return false;const auto n=s.truncate(s.user,c.progress);if(!n)return false;c.frame=*n;c.pending=0;if(c.frame>4)return finish();c.scale=c.real(c.payload+0x28);next();break;}
    case 3:if(!c.translate(0,c.payload+4))return false;next();break;
    case 4:if(!c.call(0x8000f2fc,{4}))return false;c.camera=c.result;next();break;
    case 5:
        if(!c.mathReady){const auto yaw=c.real(c.camera+0x114),factor=c.real(0x80425470);c.mathStart(0x8029dff0);c.math.axis=121;c.math.radians=s.multiply(s.user,factor,-yaw);}if(!c.matrix(1))return false;next();break;
    case 6:
        if(!c.mathReady){c.mathStart(0x8029e2b0);c.math.xyz={c.scale,c.scale,c.scale};}if(!c.matrix(2))return false;next();break;
    case 7:if(!c.concat(0,0,1))return false;next();break;
    case 8:if(!c.concat(3,0,2))return false;next();break;
    case 9:{
        c.bias=c.wide(0x802fb0b8);const auto multiplier=c.real(0x80425474);c.rootAlpha=c.word(c.payload+0x2c);
        const auto whole=convert(c.frame,c.bias);volatile float fraction=c.progress-whole;c.scaleX=s.multiply(s.user,multiplier,fraction);next();break;}
    case 10:{c.pending=0x801c5e64;if(!s.truncate)return false;const auto n=s.truncate(s.user,c.scaleX);if(!n)return false;c.alpha=*n;c.pending=0;next();break;}
    // Kind1 fire table scales five channels by >>8,leaving first red unscaled.
    // Kind2 even-age swaps first RGB to black and copies original first RGB to
    // second;other branches copy six bytes. BSS e0/e4 reads below remain required.
    case 11:{
        if(c.kind==1){const auto fire=sign(c.age-1)<13?c.byte(0x804198ac+c.age-1):0;
            c.colors[1]=uint8_t((uint32_t(c.byte(c.payload+0x3d))*fire)>>8);c.colors[2]=uint8_t((uint32_t(c.byte(c.payload+0x3e))*fire)>>8);
            c.colors[3]=uint8_t((uint32_t(c.byte(c.payload+0x3f))*fire)>>8);c.colors[4]=uint8_t((uint32_t(c.byte(c.payload+0x40))*fire)>>8);c.colors[5]=uint8_t((uint32_t(c.byte(c.payload+0x41))*fire)>>8);c.colors[0]=c.byte(c.payload+0x3c);
        }else if(c.kind==2 && !(c.age&1)){c.colors[0]=c.colors[1]=c.colors[2]=0;for(unsigned i=0;i<3;++i)c.colors[i+3]=c.byte(c.payload+0x3c+i);}
        else for(unsigned i=0;i<6;++i)c.colors[i]=c.byte(c.payload+0x3c+i);
        next();break;}
    case 12:if(!c.table(material))return false;next();break;
    case 13:
        if(!c.callReady){(void)c.word(0x804296e0);const auto color=uint32_t(c.colors[0])<<24|uint32_t(c.colors[1])<<16|uint32_t(c.colors[2])<<8|uint8_t(c.rootAlpha);c.request={};c.request.function=0x802b8ab8;c.request.integers={1,color};c.callReady=true;}if(!c.call())return false;next();break;
    case 14:
        if(!c.callReady){(void)c.word(0x804296e4);const auto color=uint32_t(c.colors[3])<<24|uint32_t(c.colors[4])<<16|uint32_t(c.colors[5])<<8|uint8_t(c.alpha);c.request={};c.request.function=0x802b8ab8;c.request.integers={2,color};c.callReady=true;}if(!c.call())return false;next();break;
    // Actual GetTexObjN64 image74 object binds BOTH slots0/1. Three axis-vector
    // passes0/120/240 each invoke full particle child with stable borrowed base.
    // Capture preparation alone is not queued display/GPU completion.
    case 15:if(!c.call(0x801d78b0,{74}))return false;c.texture=c.result;next();break;
    case 16:if(!c.call(0x802b7880,{c.texture,0}))return false;next();break;
    case 17:if(!c.call(0x802b7880,{c.texture,1}))return false;next();break;
    case 18:if(!c.table(coordinates))return false;c.angleFactor=c.real(0x80425470);c.bias=c.wide(0x802fb0b8);next();break;
    case 19:
        if(!c.mathReady){const auto vector=c.xyz(0x802fb0ac);c.mathStart(0x8029e1c0);c.math.xyz=vector;c.math.radians=s.multiply(s.user,c.angleFactor,convert(int32_t(c.pass*120),c.bias));}if(!c.matrix(4))return false;next();break;
    case 20:if(!c.concat(5,3,4))return false;next();break;
    case 21:
        if(!resumeHitN64ParticleDraw(effect,c.matrices[5],s,c.child)){c.pending=c.child.pending();return false;}
        ++c.pass;if(c.pass==3)return finish();c.phase=19;break;
    default:return false;
    }
}
// Build both original UV matrices30/33;frame>=4 collapses second to zero.
// Vertex block803a0458+frame*56 has four stride14 rows:XYZ S16 at0,UV S16
// fraction5 at6,RGBA8 at10. Loop particles1..signed count-1,reloading count.
// Per particle Ry*Rz*T then borrowed base,then FRESH camera4 view11c.
// GXBegin144=triangles,count6;whole18-halfword FIFO follows exact descriptors.
bool resumeHitN64ParticleDraw(uint32_t effect,const BattleActMatrix& base,const HitN64DrawServices& s,HitN64DrawCursor& cursor){
    if(cursor.invoking_ || !s.memory.word || !s.byte || !s.multiply)return false;
    if(cursor.state_ && (cursor.state_->operation!=2 || cursor.state_->effect!=effect || cursor.state_->borrowed!=&base || !same(cursor.state_->owner,s)))return false;
    Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<HitN64DrawCursor::State>();cursor.state_->owner=s;cursor.state_->operation=2;cursor.state_->effect=effect;cursor.state_->borrowed=&base;}
    auto& c=*cursor.state_;const auto next=[&](){++c.phase;};const auto finish=[&](){cursor.state_.reset();return true;};
    for(;;)switch(c.phase){
    case 0:{const auto multiplier=c.real(0x80425458);c.payload=c.word(effect+0xc);const auto four=c.real(0x8042545c),progress=c.real(c.payload+0x38);c.progress=s.multiply(s.user,multiplier,progress);c.kind=c.progress>=four?1:0;next();break;}
    case 1:{c.pending=0x801c5b8c;if(!s.truncate)return false;const auto n=s.truncate(s.user,c.progress);if(!n)return false;c.frame=*n;c.pending=0;next();break;}
    case 2:
        if(!c.mathReady){c.mathStart(0x8029e2b0);c.math.xyz={c.real(0x80425460),c.real(0x80425464),c.real(0x80425468)};}if(!c.matrix(0))return false;next();break;
    case 3:
        if(!c.mathReady){const auto zero=c.real(0x80425468),height=c.real(0x8042546c);c.mathStart(0x8029e230);c.math.xyz={zero,height,zero};}if(!c.matrix(1))return false;next();break;
    case 4:if(!c.concat(0,0,1))return false;next();break;
    case 5:if(!c.loadMatrix(0x802ba3d0,{30,1},0))return false;next();break;
    case 6:
        if(!c.mathReady){c.mathStart(0x8029e2b0);c.math.xyz={c.real(0x80425460),c.real(0x80425464),c.real(0x80425468)};}if(!c.matrix(0))return false;next();break;
    case 7:if(!c.loadMatrix(0x802ba3d0,{33,1},0))return false;next();break;
    case 8:if(!c.kind){c.phase=11;break;}next();break;
    case 9:
        if(!c.mathReady){const auto zero=c.real(0x80425468);c.mathStart(0x8029e2b0);c.math.xyz={zero,zero,zero};}if(!c.matrix(0))return false;next();break;
    case 10:if(!c.loadMatrix(0x802ba3d0,{33,1},0))return false;next();break;
    case 11:c.angleFactor=c.real(0x80425470);c.particle=c.payload+0x44;c.vertices=0x803a0458+uint32_t(c.frame)*56;c.index=1;next();break;
    case 12:if(sign(c.index)>=sign(c.word(effect+8)))return finish();next();break;
    case 13:
        if(!c.mathReady){const auto angle=c.real(c.particle+0x24);c.mathStart(0x8029dff0);c.math.axis=121;c.math.radians=s.multiply(s.user,c.angleFactor,angle);}if(!c.matrix(0))return false;next();break;
    case 14:
        if(!c.mathReady){const auto angle=c.real(c.particle+0x1c);c.mathStart(0x8029dff0);c.math.axis=122;c.math.radians=s.multiply(s.user,c.angleFactor,angle);}if(!c.matrix(1))return false;next();break;
    case 15:if(!c.translate(2,c.particle+4))return false;next();break;
    case 16:if(!c.concat(1,0,1))return false;next();break;
    case 17:if(!c.concat(2,1,2))return false;next();break;
    case 18:
        if(!c.mathReady){c.mathStart(0x8029dd64);c.math.left=base;c.math.right=c.matrices[2];}if(!c.matrix(2))return false;next();break;
    case 19:if(!c.call(0x8000f2fc,{4}))return false;c.camera=c.result;next();break;
    case 20:
        if(!c.mathReady){c.mathStart(0x8029dd64);c.math.left=c.matrixAt(c.camera+0x11c);c.math.right=c.matrices[2];}if(!c.matrix(2))return false;next();break;
    case 21:if(!c.loadMatrix(0x802ba2fc,{0},2))return false;next();break;
    case 22:if(!c.call(0x802ba39c,{0}))return false;next();break;
    case 23:if(!c.table(descriptors))return false;next();break;
    case 24:if(!c.call(0x802b4db0,{9,c.vertices,14}))return false;next();break;
    case 25:if(!c.call(0x802b4db0,{11,c.vertices+10,14}))return false;next();break;
    case 26:if(!c.call(0x802b4db0,{13,c.vertices+6,14}))return false;next();break;
    case 27:if(!c.call(0x802b5a6c,{144,0,6}))return false;next();break;
    case 28:
        c.pending=0x801d76a0;if(!s.indices || !s.indices(s.user,triangles))return false;
        c.pending=0;++c.index;c.particle+=0x44;c.phase=12;break;
    default:return false;
    }
}
}
