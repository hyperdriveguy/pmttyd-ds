#include "battle_submenu_surface_gpu.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#ifdef __3DS__
#include <tex3ds.h>
#endif
namespace ttyd {
namespace {
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
// Native LE PMBSUB01 catalog:two rows, original indices2/3, exact EOF.
// Atlas32x32:16x24 at(2,2),8x24 at(22,2),with2px clamp-edge gutters.
// V endpoints include the single native T flip; source pixels/UVs are fixed.
bool catalog(const std::string& path,std::array<BattleClassRegion,2>& regions){
 FILE*f=std::fopen(path.c_str(),"rb");if(!f)return false;char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1&&!std::memcmp(magic,"PMBSUB01",8)&&std::fread(&count,4,1,f)==1&&count==2;
 for(unsigned i=0;ok&&i<2;++i){uint32_t id=0;ok=std::fread(&id,4,1,f)==1&&id==i+2&&std::fread(&regions[i],sizeof(BattleClassRegion),1,f)==1;
  const auto&r=regions[i];const unsigned w=i?8:16,x=i?22:2;
  ok=ok&&r.width==w&&r.height==24&&r.u0==float(x)/32&&r.v0==1-2.f/32&&r.u1==float(x+w)/32&&r.v1==1-26.f/32;
 }
 ok=ok&&std::fgetc(f)==EOF;std::fclose(f);return ok;
}
}
bool BattleSubmenuSurfaceLease::current()const{return generation&&texture&&generation->resident_&&generation->texture_==texture;}
const BattleClassRegion* BattleSubmenuSurfaceLease::surface(uint32_t i)const{return current()&&i>=2&&i<=3?&generation->regions_[i-2]:nullptr;}
BattleSubmenuSurfaceGpuResource::~BattleSubmenuSurfaceGpuResource(){retire();}
BattleSubmenuSurfaceLease BattleSubmenuSurfaceGpuResource::lease()const{return lease_.current()?lease_:BattleSubmenuSurfaceLease{};}
bool BattleSubmenuSurfaceGpuResource::accepts(const BattleSubmenuSurfaceLease& l)const{return l.current()&&lease_.current()&&l.generation==generation_&&l.texture==lease_.texture;}
// Publish only after exact catalog AND actual one-image import admission:
// RGBA8 format0,unrotated32x32,full-image UV(0,1,1,0). Failed/retired-during-
// import candidates release their actual handle; no partial generation escapes.
bool BattleSubmenuSurfaceGpuResource::load(const char* root,const BattleClassSurfaceTextureServices&s){
 if(invoking_||lease_.current()||!root||!s.import||!s.release)return false;
 Guard guard(invoking_);const std::string base=root;std::array<BattleClassRegion,2> regions;
 if(!catalog(base+"/battle-submenu-surfaces.bin",regions))return false;
 const auto result=s.import(s.user,0,(base+"/battle-submenu-surfaces.t3x").c_str());
 if(!result){retirePending_=false;return false;}
 const auto&r=*result;
 const bool ok=!retirePending_&&r.identity&&r.width==32&&r.height==32&&r.format==0&&r.count==1&&r.subWidth==32&&r.subHeight==32&&!r.rotated&&r.uv==std::array<float,4>{0,1,1,0};
 if(!ok){if(r.identity)s.release(s.user,0,r.identity);retirePending_=false;return false;}
 auto generation=std::shared_ptr<BattleSubmenuSurfaceGeneration>(new BattleSubmenuSurfaceGeneration);generation->resident_=true;generation->texture_=r.identity;generation->regions_=regions;
 generation_=std::move(generation);lease_={generation_,r.identity};owner_=s;return true;
}
// Reentrant retirement requests cancel pending import. Ordinary retirement
// invalidates every old lease BEFORE texture release, even if addresses recur.
void BattleSubmenuSurfaceGpuResource::retire(){
 if(invoking_){retirePending_=true;return;}Guard guard(invoking_);
 if(generation_)generation_->resident_=false;
 auto old=lease_;auto owner=owner_;lease_={};generation_.reset();owner_={};
 if(old.texture&&owner.release)owner.release(owner.user,0,old.texture);
 retirePending_=false;
}
struct NativeBattleSubmenuSurfaceGpu::State {
 BattleSubmenuSurfaceGpuResource resource;
#ifdef __3DS__
 C3D_Tex image{};Tex3DS_Texture imported=nullptr;
#endif
};
NativeBattleSubmenuSurfaceGpu::NativeBattleSubmenuSurfaceGpu():state_(std::make_unique<State>()){}
NativeBattleSubmenuSurfaceGpu::~NativeBattleSubmenuSurfaceGpu(){release();}
const BattleSubmenuSurfaceGpuResource& NativeBattleSubmenuSurfaceGpu::resource()const{return state_->resource;}
void NativeBattleSubmenuSurfaceGpu::release(){state_->resource.retire();}
bool NativeBattleSubmenuSurfaceGpu::load(const char*root){
#ifdef __3DS__
 BattleClassSurfaceTextureServices s;s.user=state_.get();
 s.import=[](void*p,uint32_t slot,const char*path)->std::optional<BattleClassSurfaceImport>{
  auto&state=*static_cast<State*>(p);if(slot||state.imported)return {};FILE*f=std::fopen(path,"rb");if(!f)return {};
  C3D_FrameSync();state.imported=Tex3DS_TextureImportStdio(f,&state.image,nullptr,false);std::fclose(f);if(!state.imported)return {};
  BattleClassSurfaceImport r;r.identity=&state.image;r.width=state.image.width;r.height=state.image.height;r.format=state.image.fmt;r.count=Tex3DS_GetNumSubTextures(state.imported);
  if(r.count){const auto*sub=Tex3DS_GetSubTexture(state.imported,0);r.subWidth=sub->width;r.subHeight=sub->height;r.rotated=Tex3DS_SubTextureRotated(sub);r.uv={sub->left,sub->top,sub->right,sub->bottom};}
  // Atlas sampler adapter matches source clamp/linear endpoints using gutters;
  // this does not establish original GX-to-native pixel/filter equality.
  C3D_TexSetWrap(&state.image,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);C3D_TexSetFilter(&state.image,GPU_LINEAR,GPU_LINEAR);return r;
 };
 s.release=[](void*p,uint32_t slot,const void*id){auto&state=*static_cast<State*>(p);if(!slot&&state.imported&&id==&state.image){C3D_FrameSync();Tex3DS_TextureFree(state.imported);C3D_TexDelete(&state.image);state.imported=nullptr;state.image={};}};
 return state_->resource.load(root,s);
#else
 (void)root;return false;
#endif
}
#ifdef __3DS__
C3D_Tex* NativeBattleSubmenuSurfaceGpu::texture(const BattleSubmenuSurfaceLease&l){return state_->resource.accepts(l)?&state_->image:nullptr;}
#endif
}
