#include "hit_n64_gpu.hpp"
#include <cmath>
#include <cstring>
#include <limits>
#ifdef __3DS__
#include <citro2d.h>
#endif
namespace ttyd {
namespace {
using Args=std::array<uint32_t,8>;
bool args(const SourceIconGXCall&q,std::initializer_list<uint32_t> a){Args v{};size_t i=0;for(auto n:a)v[i++]=n;return q.integers==v;}
bool finite(const BattleActMatrix&m){for(auto x:m)if(!std::isfinite(x))return false;return true;}
struct Op{uint32_t f;Args a;};
constexpr Op material[]={
 {0x802b6f3c,{0}},{0x802b8fcc,{2}},{0x802b8e30,{0,0,0,255}},
 {0x802b89e8,{0,0,0,0,1,0}},{0x802b8a50,{0,0,0,0,1,0}},
 {0x802b8960,{0,15,15,15,8}},{0x802b89a4,{0,7,7,7,4}},
 {0x802b8e30,{1,1,1,255}},{0x802b89e8,{1,0,0,0,1,0}},
 {0x802b8a50,{1,0,0,0,1,0}},{0x802b8960,{1,4,2,0,15}},
 {0x802b89a4,{1,0,4,2,7}},
};
constexpr Op coordinates[]={ {0x802b5084,{2}},{0x802b4e04,{0,1,4,30,0,125}}, {0x802b4e04,{1,1,4,33,0,125}},{0x802b5c8c,{0}} };
constexpr Op desc[]={ {0x802b4520,{}},{0x802b3c08,{9,3}},{0x802b3c08,{11,3}},{0x802b3c08,{13,3}}, {0x802b4558,{0,9,1,3,0}},{0x802b4558,{0,11,1,5,0}}, {0x802b4558,{0,13,1,3,5}} };
bool same(const HitN64DrawServices&a,const HitN64DrawServices&b){return a.user==b.user&&a.memory.user==b.memory.user&&a.memory.word==b.memory.word&&a.byte==b.byte&&a.multiply==b.multiply&&a.truncate==b.truncate&&a.matrix==b.matrix&&a.call==b.call&&a.indices==b.indices;}
float signedHalf(uint8_t a,uint8_t b){uint32_t v=uint32_t(a)<<8|b;return float(v<32768?int32_t(v):int32_t(v)-65536);}
}
bool HitN64GpuBatch::current()const{
    if(retired_)return false;
    const bool valid=resource_&&resource_->ready()&&generation_&&lifetime_.owner&&lifetime_.current&&lifetime_.textureCurrent&&lifetime_.current(lifetime_.user)&&lifetime_.textureCurrent(lifetime_.user,resource_,generation_);
    if(!valid)retired_=true;
    return valid;
}
struct HitN64GpuCapture::State {
    std::shared_ptr<HitN64GpuBatch> batch=std::make_shared<HitN64GpuBatch>();
    std::shared_ptr<const HitN64GpuBatch> published;
    HitN64DrawServices source,wrapped;HitN64DrawCursor cursor;
    bool bound=false,invoking=false,complete=false;int32_t camera=0;uint32_t effect=0;
    unsigned step=0,stage=0,descriptor=0;uint32_t object=0,vertexBase=0;
    bool matrixReady=false,awaiting=false;std::array<bool,2> texReady{};
    BattleActMatrix matrix{};std::array<BattleActMatrix,2> tex{};
    uint32_t colors[2]{};
    bool live()const{return batch->current();}
    static State& self(void*p){return *static_cast<State*>(p);}
    static uint8_t byte(void*p,uint32_t a){auto&s=self(p);return s.source.byte(s.source.user,a);}
    static float multiply(void*p,float a,float b){auto&s=self(p);return s.source.multiply(s.source.user,a,b);}
    static std::optional<int32_t> truncate(void*p,float a){auto&s=self(p);if(!s.live()||!s.source.truncate)return {};auto r=s.source.truncate(s.source.user,a);return s.live()?r:std::optional<int32_t>{};}
    static bool math(void*p,BattleAudienceMatrixRequest&q){auto&s=self(p);return s.live()&&s.source.matrix&&s.source.matrix(s.source.user,q)&&s.live();}
    static std::optional<uint32_t> call(void*p,const SourceIconGXCall&q){return self(p).call(q);}
    static bool indices(void*p,const std::array<uint16_t,18>&q){return self(p).indices(q);}
    std::optional<uint32_t> call(const SourceIconGXCall&q){
        if(!live()||awaiting)return {};
        if(q.function==0x8000f2fc){if(!args(q,{q.integers[0]})||!source.call)return {};auto r=source.call(source.user,q);return live()?r:std::optional<uint32_t>{};}
        if(stage==0){if(step>=12||q.function!=material[step].f||q.integers!=material[step].a)return {};if(++step==12){stage=1;step=0;}return 0;}
        if(stage==1){if(q.function!=0x802b8ab8||!args(q,{step+1,q.integers[1]}))return {};colors[step]=q.integers[1];if(++step==2){stage=2;step=0;}return 0;}
        if(stage==2){if(q.function!=0x801d78b0||!args(q,{74})||!source.call)return {};auto t=source.call(source.user,q);if(!t||!live())return {};object=*t;stage=3;return t;}
        if(stage==3){if(q.function!=0x802b7880||!args(q,{object,step}))return {};if(++step==2){stage=4;step=0;}return 0;}
        if(stage==4){if(q.function!=coordinates[step].f||q.integers!=coordinates[step].a)return {};if(++step==4){stage=5;step=0;}return 0;}
        if(q.function==0x802ba3d0){
            if(!finite(q.matrix))return {};
            const unsigned slot=q.integers[0]==30?0:q.integers[0]==33?1:2;
            if(slot==2||!args(q,{q.integers[0],1}))return {};
            // Original two 2x4 UV transforms; unknown third-coordinate
            // dependence cannot be supplied by this two-component source array.
            if(q.matrix[2]!=0||q.matrix[6]!=0)return {};
            tex[slot]=q.matrix;texReady[slot]=true;return 0;
        }
        if(q.function==0x802ba2fc){if(!args(q,{0})||!finite(q.matrix))return {};matrix=q.matrix;matrixReady=true;return 0;}
        if(q.function==0x802ba39c){if(!matrixReady||!args(q,{0}))return {};return 0;}
        if(descriptor<7){if(q.function!=desc[descriptor].f||q.integers!=desc[descriptor].a)return {}; ++descriptor;return 0;}
        if(descriptor<10){
            unsigned i=descriptor-7;uint32_t at=q.integers[1];
            if(q.function!=0x802b4db0||!args(q,{i==0?9u:i==1?11u:13u,at,14}))return {};
            if(i==0){if(at<0x803a0458||at>0x803a0458+224||(at-0x803a0458)%56)return {};vertexBase=at;}
            else if(at!=vertexBase+(i==1?10:6))return {};
            ++descriptor;return 0;
        }
        if(q.function!=0x802b5a6c||!args(q,{144,0,6})||!matrixReady||!texReady[0]||!texReady[1])return {};
        awaiting=true;return 0;
    }
    bool indices(const std::array<uint16_t,18>&q){
        static constexpr std::array<uint16_t,18> expected={0,0,0,1,1,1,2,2,2,0,0,0,2,2,2,3,3,3};
        if(!live()||!awaiting||q!=expected)return false;
        // Source pointers are admitted only against the original immutable
        // vertex owner. Read the actual source bytes too: alias changes hold.
        std::array<uint8_t,56> bytes{};
        for(unsigned i=0;i<56;++i){auto b=batch->resource_->byte(vertexBase+i);if(!b)return false;bytes[i]=source.byte(source.user,vertexBase+i);if(bytes[i]!=*b)return false;}
        HitN64GpuPacket out;out.matrix=matrix;out.color0=colors[0];out.color1=colors[1];
        for(unsigned i=0;i<6;++i){const auto*b=bytes.data()+14*q[3*i];auto&v=out.vertices[i];
            for(unsigned n=0;n<3;++n)v.position[n]=signedHalf(b[2*n],b[2*n+1]);
            const float u=signedHalf(b[6],b[7])/32.f,t=signedHalf(b[8],b[9])/32.f;
            v.sourceColor=uint32_t(b[10])<<24|uint32_t(b[11])<<16|uint32_t(b[12])<<8|b[13];
            for(unsigned n=0;n<2;++n){const auto&m=tex[n];const float s=m[0]*u+m[1]*t+m[3],v0=m[4]*u+m[5]*t+m[7];if(!std::isfinite(s)||!std::isfinite(v0))return false;(n?v.uv1:v.uv0)={s,1-v0};}
        }
        if(!live())return false;
        batch->packets_.push_back(out);awaiting=false;descriptor=0;matrixReady=false;return true;
    }
};
HitN64GpuCapture::HitN64GpuCapture()=default;HitN64GpuCapture::~HitN64GpuCapture()=default;
bool HitN64GpuCapture::begin(std::shared_ptr<const HitN64TextureResources>r,uint64_t g,const HitN64GpuLifetime&l){
    if(state_&&(state_->invoking||(!state_->complete&&state_->bound)))return false;
    auto s=std::make_unique<State>();s->batch->resource_=std::move(r);s->batch->generation_=g;s->batch->lifetime_=l;
    if(!s->live())return false;
    state_=std::move(s);return true;
}
bool HitN64GpuCapture::resume(int32_t camera,uint32_t effect,const HitN64DrawServices&s){
    if(!state_||state_->invoking||!state_->live())return false;
    auto&c=*state_;if(c.complete)return camera==c.camera&&effect==c.effect&&same(s,c.source);
    if(!c.bound){if(!s.memory.word||!s.byte||!s.multiply)return false;c.source=s;c.camera=camera;c.effect=effect;c.bound=true;c.wrapped=s;c.wrapped.user=&c;c.wrapped.byte=State::byte;c.wrapped.multiply=State::multiply;c.wrapped.truncate=State::truncate;c.wrapped.matrix=State::math;c.wrapped.call=State::call;c.wrapped.indices=State::indices;}
    else if(camera!=c.camera||effect!=c.effect||!same(s,c.source))return false;
    c.invoking=true;const bool done=resumeHitN64Draw(camera,effect,c.wrapped,c.cursor);c.invoking=false;
    if(!done||!c.live()||c.awaiting)return false;
    c.complete=true;c.published=c.batch;return true;
}
std::shared_ptr<const HitN64GpuBatch> HitN64GpuCapture::publication()const{return state_&&state_->complete&&state_->live()?state_->published:nullptr;}
void HitN64GpuCapture::discard(){if(!state_||!state_->invoking)state_.reset();}
uint32_t HitN64GpuCapture::pending()const{return state_?state_->cursor.pending():0;}
struct NativeHitN64Gpu::State {
#ifdef __3DS__
    struct Vertex{float x,y,z,u0,v0,u1,v1,r,g,b,a;};
    DVLB_s* shader=nullptr;shaderProgram_s program{};bool initialized=false;
    int transform=-1;Vertex* vertices=nullptr;size_t capacity=0,used=0;bool submitting=false;
#endif
};
NativeHitN64Gpu::NativeHitN64Gpu():state_(std::make_unique<State>()){}
NativeHitN64Gpu::~NativeHitN64Gpu(){release();}
bool NativeHitN64Gpu::prepare(const uint32_t* shader,uint32_t bytes,size_t maximumPackets){
#ifdef __3DS__
    if(!shader||!bytes||!maximumPackets||maximumPackets>std::numeric_limits<size_t>::max()/6/sizeof(State::Vertex))return false;
    // Never retire storage while an earlier submitted frame references it.
    release();auto& s=*state_;
    s.shader=DVLB_ParseFile(const_cast<uint32_t*>(shader),bytes);
    if(!s.shader||s.shader->numDVLE!=1){release();return false;}
    const auto& dvle=s.shader->DVLE[0];unsigned outputs=0;
    if(dvle.type!=VERTEX_SHDR||dvle.outTableSize!=4){release();return false;}
    for(unsigned n=0;n<dvle.outTableSize;++n){const auto&o=dvle.outTableData[n];
        unsigned bit=o.type==RESULT_POSITION?1:o.type==RESULT_COLOR?2:o.type==RESULT_TEXCOORD0?4:o.type==RESULT_TEXCOORD1?8:0;
        if(!bit||(outputs&bit)){release();return false;}outputs|=bit;
    }
    if(outputs!=15){release();return false;}
    shaderProgramInit(&s.program);s.initialized=true;
    shaderProgramSetVsh(&s.program,&s.shader->DVLE[0]);
    s.transform=shaderInstanceGetUniformLocation(s.program.vertexShader,"transform");
    if(s.transform<0){release();return false;}
    s.vertices=static_cast<State::Vertex*>(linearAlloc(maximumPackets*6*sizeof(State::Vertex)));
    if(!s.vertices){release();return false;}s.capacity=maximumPackets;return true;
#else
    (void)shader;(void)bytes;(void)maximumPackets;return false;
#endif
}
bool NativeHitN64Gpu::beginFrame(){
#ifdef __3DS__
    auto&s=*state_;if(!s.initialized||!s.vertices||s.submitting)return false;
    C3D_FrameSync();s.used=0;return true;
#else
    return false;
#endif
}
bool NativeHitN64Gpu::preflight(const HitN64GpuBatch&b,const NativeHitN64Texture&t,const HitN64NativeRaster&r)const{
#ifdef __3DS__
    const auto&s=*state_;
    if(!s.initialized||s.transform<0||!s.vertices||s.used>s.capacity||b.packets().size()>s.capacity-s.used||!b.current()||!t.current(b.resource(),b.generation())||!r.preflight||!r.apply)return false;
    for(float x:r.projection)if(!std::isfinite(x))return false;
    for(const auto&p:b.packets()){
        if(!finite(p.matrix))return false;
        for(const auto&v:p.vertices){for(float x:v.position)if(!std::isfinite(x))return false;for(float x:v.uv0)if(!std::isfinite(x))return false;for(float x:v.uv1)if(!std::isfinite(x))return false;}
    }
    return r.preflight(r.user)&&b.current()&&t.current(b.resource(),b.generation());
#else
    (void)b;(void)t;(void)r;return false;
#endif
}
bool NativeHitN64Gpu::draw(const HitN64GpuBatch&b,NativeHitN64Texture&t,const HitN64NativeRaster&r){
#ifdef __3DS__
    auto&s=*state_;if(s.submitting)return false;
    struct Guard{bool&flag;explicit Guard(bool&f):flag(f){flag=true;}~Guard(){flag=false;}} guard(s.submitting);
    if(!preflight(b,t,r))return false;
    const size_t first=s.used;size_t at=first*6;
    for(const auto&p:b.packets())for(const auto&v:p.vertices){
        // Source matrix is already actual camera4 view * original model.
        // Uniform supplies the caller's genuine view-to-native-clip mapping.
        const auto&m=p.matrix;const auto&x=v.position;
        auto&out=s.vertices[at++];
        out={m[0]*x[0]+m[1]*x[1]+m[2]*x[2]+m[3],m[4]*x[0]+m[5]*x[1]+m[6]*x[2]+m[7],m[8]*x[0]+m[9]*x[1]+m[10]*x[2]+m[11],v.uv0[0],v.uv0[1],v.uv1[0],v.uv1[1],float(p.color0>>24)/255.f,float((p.color0>>16)&255)/255.f,float((p.color0>>8)&255)/255.f,float(p.color0&255)/255.f};
        if(!std::isfinite(out.x)||!std::isfinite(out.y)||!std::isfinite(out.z))return false;
    }
    // No GPU state/effect before complete validation and CPU conversion.
    // Single-threaded submission is required: owners cannot retire in apply.
    if(!b.current()||!t.current(b.resource(),b.generation()))return false;
    C2D_Flush();r.apply(r.user);
    C3D_CullFace(GPU_CULL_NONE); // Original GXSetCullMode(0), not inherited.
    C3D_BindProgram(&s.program);
    auto*attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,3,GPU_FLOAT,4);
    auto*buf=C3D_GetBufInfo();BufInfo_Init(buf);BufInfo_Add(buf,s.vertices,sizeof(State::Vertex),4,0x3210);
    C3D_Mtx projection;for(unsigned row=0;row<4;++row)projection.r[row]=FVec4_New(r.projection[row*4],r.projection[row*4+1],r.projection[row*4+2],r.projection[row*4+3]);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,s.transform,&projection);
    // Preflight pins both aliases to the same actual imported image74 owner.
    t.bind(0,b.resource(),b.generation());t.bind(1,b.resource(),b.generation());
    GSPGPU_FlushDataCache(s.vertices+first*6,(at-first*6)*sizeof(State::Vertex));
    for(unsigned n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto*env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0);C3D_TexEnvFunc(env,C3D_Both,GPU_REPLACE);
    env=C3D_GetTexEnv(1);
    // GX C1*(1-PREV)+C0*PREV; GX alpha PREV*(1-A1)+TEX1*A1.
    // PICA interpolate is arg0*arg2 + arg1*(1-arg2).
    C3D_TexEnvSrc(env,C3D_RGB,GPU_PRIMARY_COLOR,GPU_CONSTANT,GPU_PREVIOUS);
    C3D_TexEnvFunc(env,C3D_RGB,GPU_INTERPOLATE);
    C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE1,GPU_PREVIOUS,GPU_CONSTANT);
    C3D_TexEnvFunc(env,C3D_Alpha,GPU_INTERPOLATE);
    for(size_t n=0;n<b.packets().size();++n){const uint32_t c=b.packets()[n].color1;C3D_TexEnvColor(env,C2D_Color32(c>>24,(c>>16)&255,(c>>8)&255,c&255));C3D_DrawArrays(GPU_TRIANGLES,int((first+n)*6),6);}
    s.used+=b.packets().size();return true;
#else
    (void)b;(void)t;(void)r;return false;
#endif
}
void NativeHitN64Gpu::release(){
#ifdef __3DS__
    auto&s=*state_;if(s.shader||s.vertices||s.initialized)C3D_FrameSync();
    if(s.vertices)linearFree(s.vertices);
    if(s.initialized)shaderProgramFree(&s.program);
    if(s.shader)DVLB_Free(s.shader);
    s=State{};
#endif
}
}
