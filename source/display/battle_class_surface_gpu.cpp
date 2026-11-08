#include "battle_class_surface_gpu.hpp"
#include "battle_class_backend.hpp"
#include <string>
#ifdef __3DS__
#include <tex3ds.h>
#endif
namespace ttyd {
namespace {
struct Guard{bool& value;explicit Guard(bool&v):value(v){value=true;}~Guard(){value=false;}};
// Atlas V endpoints include the Tex3DS top/bottom flip exactly once.
bool region(const BattleClassRegion*r,uint32_t w,uint32_t h,uint32_t x,uint32_t y,uint32_t aw,uint32_t ah){
    return r&&r->width==w&&r->height==h&&r->u0==float(x)/aw&&r->v0==1-float(y)/ah&&r->u1==float(x+w)/aw&&r->v1==1-float(y+h)/ah;
}
// Exact native package layout: surfaces 0/1 are 40x40 at (2,2) and
// 128x80 at (46,2) in 256x128. Original class icons 334..343 are 48x48
// in a 256x256 atlas: four columns, 52-pixel pitch, two-pixel inset.
bool catalog(const BattleClassAssets&a){
    if(!a.ready()||!region(a.surface(0),40,40,2,2,256,128)||!region(a.surface(1),128,80,46,2,256,128))return false;
    for(unsigned i=0;i<10;++i)if(!region(a.icon(334+i),48,48,(i%4)*52+2,(i/4)*52+2,256,256))return false;
    return true;
}
// Slot 0 is the surface atlas; slot 1 is the icon atlas. Require one
// unrotated full-image RGBA8 subtexture, not merely the expected allocation size.
bool admitted(unsigned i,const BattleClassSurfaceImport&r){
    const unsigned h=i?256:128;
    return r.identity&&r.width==256&&r.height==h&&r.format==0&&r.count==1&&r.subWidth==256&&r.subHeight==h&&!r.rotated&&r.uv==std::array<float,4>{0,1,1,0};
}
}
bool BattleClassSurfaceLease::current()const{return assets&&assets->ready()&&generation&&generation->resident_&&generation->assets_==assets&&generation->textures_==textures&&textures[0]&&textures[1];}
BattleClassSurfaceGpuResource::~BattleClassSurfaceGpuResource(){retire();}
bool BattleClassSurfaceGpuResource::ready()const{return lease_.current();}
BattleClassSurfaceLease BattleClassSurfaceGpuResource::lease()const{return ready()?lease_:BattleClassSurfaceLease{};}
bool BattleClassSurfaceGpuResource::accepts(const BattleClassSurfaceLease&l)const{
    return ready()&&l.current()&&l.assets==lease_.assets&&l.generation==generation_&&l.textures==lease_.textures;
}
// No generation escapes until BOTH imports pass. Reentrant load is refused;
// retire during either callback marks the candidate for rollback at admission.
bool BattleClassSurfaceGpuResource::load(const char*root,const BattleClassSurfaceTextureServices&s){
    if(invoking_||ready()||!root||!s.import||!s.release)return false;
    Guard guard(invoking_);const std::string base=root;
    auto assets=std::make_shared<BattleClassAssets>();
    if(!assets->load((base+"/battle-class-surfaces.bin").c_str(),(base+"/battle-class-icons.bin").c_str())||!catalog(*assets))return false;
    std::array<const void*,2> acquired{};
    const char* files[]={"/battle-class-surfaces.t3x","/battle-class-icons.t3x"};
    for(unsigned i=0;i<2;++i){
        const auto r=s.import(s.user,i,(base+files[i]).c_str());
        if(r)acquired[i]=r->identity;
        if(retirePending_||!r||!admitted(i,*r)||(i&&acquired[0]==acquired[1])){
            // Aliased image handles are not a valid two-import owner. Each
            // successful import is one acquisition, including rejection.
            for(unsigned j=0;j<=i;++j)if(acquired[j])s.release(s.user,j,acquired[j]);
            retirePending_=false;return false;
        }
    }
    auto generation=std::shared_ptr<BattleClassSurfaceGeneration>(new BattleClassSurfaceGeneration);generation->resident_=true;generation->assets_=assets;generation->textures_=acquired;
    // A new identity object separates reload epochs even if native handles recur.
    lease_={std::move(assets),generation,acquired};generation_=std::move(generation);services_=s;return true;
}
void BattleClassSurfaceGpuResource::retire(){
    if(invoking_){retirePending_=true;return;}
    Guard guard(invoking_);
    // Publish invalidation before invoking any external native destruction.
    if(generation_)generation_->resident_=false;
    auto old=lease_;auto services=services_;lease_={};generation_.reset();services_={};
    for(unsigned i=0;i<2;++i)if(old.textures[i]&&services.release)services.release(services.user,i,old.textures[i]);
    retirePending_=false;
}
struct NativeBattleClassSurfaceGpu::State {
    BattleClassSurfaceGpuResource resource;
#ifdef __3DS__
    struct Slot{C3D_Tex image{};Tex3DS_Texture imported=nullptr;};
    std::array<Slot,2> slots{};
#endif
};
NativeBattleClassSurfaceGpu::NativeBattleClassSurfaceGpu():state_(std::make_unique<State>()){}
NativeBattleClassSurfaceGpu::~NativeBattleClassSurfaceGpu(){release();}
const BattleClassSurfaceGpuResource& NativeBattleClassSurfaceGpu::resource()const{return state_->resource;}
void NativeBattleClassSurfaceGpu::release(){state_->resource.retire();}
bool NativeBattleClassSurfaceGpu::load(const char*root){
#ifdef __3DS__
    BattleClassSurfaceTextureServices s;s.user=state_.get();
    s.import=[](void*p,uint32_t i,const char*path)->std::optional<BattleClassSurfaceImport>{
        auto&state=*static_cast<State*>(p);if(i>=2||state.slots[i].imported)return {};
        auto&slot=state.slots[i];FILE*f=std::fopen(path,"rb");if(!f)return {};
        // Import owns Tex3DS metadata and a separate C3D pixel allocation.
        // Both import and destruction synchronize outside FrameBegin while C3D lives.
        C3D_FrameSync();slot.imported=Tex3DS_TextureImportStdio(f,&slot.image,nullptr,false);std::fclose(f);
        if(!slot.imported)return {};
        BattleClassSurfaceImport r;r.identity=&slot.image;r.width=slot.image.width;r.height=slot.image.height;r.format=slot.image.fmt;
        r.count=Tex3DS_GetNumSubTextures(slot.imported);
        if(r.count){const auto*sub=Tex3DS_GetSubTexture(slot.imported,0);r.subWidth=sub->width;r.subHeight=sub->height;r.rotated=Tex3DS_SubTextureRotated(sub);r.uv={sub->left,sub->top,sub->right,sub->bottom};}
        C3D_TexSetWrap(&slot.image,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);C3D_TexSetFilter(&slot.image,GPU_LINEAR,GPU_LINEAR);
        return r;
    };
    s.release=[](void*p,uint32_t i,const void*identity){
        auto&state=*static_cast<State*>(p);if(i>=2)return;auto&slot=state.slots[i];
        if(slot.imported&&identity==&slot.image){C3D_FrameSync();Tex3DS_TextureFree(slot.imported);C3D_TexDelete(&slot.image);slot={};}
    };
    return state_->resource.load(root,s);
#else
    (void)root;return false;
#endif
}
#ifdef __3DS__
C3D_Tex* NativeBattleClassSurfaceGpu::texture(uint32_t i,const BattleClassSurfaceLease&l){return i<2&&state_->resource.accepts(l)?&state_->slots[i].image:nullptr;}
#endif
}
