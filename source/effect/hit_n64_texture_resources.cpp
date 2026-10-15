#include "hit_n64_texture_resources.hpp"
#include <cstring>
#include <limits>
#ifdef __3DS__
#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#endif
namespace ttyd {
namespace {
uint32_t little(const uint8_t* b){return uint32_t(b[0])|uint32_t(b[1])<<8|uint32_t(b[2])<<16|uint32_t(b[3])<<24;}
// Native trusted-pack integrity guards; original source/product SHA256 is
// verified by the read-only converter/accepted immutable pack manifest.
uint64_t fingerprint(const uint8_t* b,size_t n){uint64_t h=14695981039346656037ULL;for(size_t i=0;i<n;++i){h^=b[i];h*=1099511628211ULL;}return h;}
}
std::shared_ptr<const HitN64TextureResources> HitN64TextureResources::load(const char* path){FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return {};auto r=std::make_shared<HitN64TextureResources>();const bool ok=r->read(f);std::fclose(f);return ok?r:nullptr;}
bool HitN64TextureResources::read(FILE* file){
    ready_=false;descriptor_={};vertices_={};rodata_={};tables_={};scalars_={};if(!file)return false;
    std::array<uint8_t,20> header{};
    if(std::fread(header.data(),header.size(),1,file)!=1 || std::memcmp(header.data(),"PMHITX01",8) || little(header.data()+8)!=74 || little(header.data()+12)!=32 || little(header.data()+16)!=128)return false;
    if(std::fread(descriptor_.data(),descriptor_.size(),1,file)!=1 || std::fread(vertices_.data(),vertices_.size(),1,file)!=1 || std::fread(rodata_.data(),rodata_.size(),1,file)!=1 || std::fread(tables_.data(),tables_.size(),1,file)!=1 || std::fread(scalars_.data(),scalars_.size(),1,file)!=1 || std::fgetc(file)!=EOF)return false;
    if(fingerprint(descriptor_.data(),descriptor_.size())!=0x8866b0f7776a717aULL || fingerprint(vertices_.data(),vertices_.size())!=0x57e8a04179b5fc5ULL)return false;
    if(fingerprint(rodata_.data(),rodata_.size())!=0x519ba9ecae384c85ULL || fingerprint(tables_.data(),tables_.size())!=0x415633d0c729781dULL || fingerprint(scalars_.data(),scalars_.size())!=0x1aa53b99889dbb6ULL)return false;
    ready_=true;return true;
}
std::optional<uint8_t> HitN64TextureResources::byte(uint32_t a)const{
    if(!ready_)return {};
    if(a>=0x803a0458 && a-0x803a0458<vertices_.size())return vertices_[a-0x803a0458];
    if(a>=0x802fb0a0 && a-0x802fb0a0<rodata_.size())return rodata_[a-0x802fb0a0];
    if(a>=0x80419888 && a-0x80419888<tables_.size())return tables_[a-0x80419888];
    if(a>=0x80425458 && a-0x80425458<scalars_.size())return scalars_[a-0x80425458];
    return {};
}
std::optional<uint32_t> HitN64TextureResources::word(uint32_t a)const{
    if(a>0xfffffffc)return {};
    uint32_t value=0;for(unsigned i=0;i<4;++i){const auto b=byte(a+i);if(!b)return {};value=value<<8|*b;}return value;
}
struct NativeHitN64Texture::State {
    std::shared_ptr<const HitN64TextureResources> resource;uint64_t generation=0;
#ifdef __3DS__
    C3D_Tex texture{};Tex3DS_Texture imported=nullptr;
#endif
};
NativeHitN64Texture::NativeHitN64Texture():state_(new State){}
NativeHitN64Texture::~NativeHitN64Texture(){release();}
bool NativeHitN64Texture::ready()const{return state_ && state_->generation && bool(state_->resource);}
uint64_t NativeHitN64Texture::generation()const{return ready()?state_->generation:0;}
std::shared_ptr<const HitN64TextureResources> NativeHitN64Texture::resource()const{return ready()?state_->resource:nullptr;}
bool NativeHitN64Texture::current(const std::shared_ptr<const HitN64TextureResources>& r,uint64_t g)const{return ready() && g && r && r==state_->resource && g==state_->generation;}
void NativeHitN64Texture::release(){
    if(!state_)return;
    state_->generation=0;state_->resource.reset();
#ifdef __3DS__
    if(state_->imported){C3D_FrameSync();Tex3DS_TextureFree(state_->imported);C3D_TexDelete(&state_->texture);state_->imported=nullptr;state_->texture={};}
#endif
}
bool NativeHitN64Texture::load(const char* metadata,const char* texture){
#ifdef __3DS__
    static uint64_t nextGeneration=1;
    auto resource=HitN64TextureResources::load(metadata);if(!resource || !texture || !nextGeneration)return false;
    FILE* f=std::fopen(texture,"rb");if(!f)return false;
    C3D_Tex image{};auto imported=Tex3DS_TextureImportStdio(f,&image,nullptr,false);std::fclose(f);if(!imported)return false;
    if(image.fmt!=GPU_RGBA8 || image.width!=32 || image.height!=128){C3D_FrameSync();Tex3DS_TextureFree(imported);C3D_TexDelete(&image);return false;}
    C3D_TexSetFilter(&image,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&image,GPU_REPEAT,GPU_REPEAT);
    release();state_->texture=image;state_->imported=imported;state_->resource=std::move(resource);state_->generation=nextGeneration;
    nextGeneration=nextGeneration==std::numeric_limits<uint64_t>::max()?0:nextGeneration+1;return true;
#else
    (void)metadata;(void)texture;return false;
#endif
}
bool NativeHitN64Texture::bind(uint32_t slot,const std::shared_ptr<const HitN64TextureResources>& r,uint64_t g){
    if(slot>1 || !current(r,g))return false;
#ifdef __3DS__
    C3D_TexBind(slot,&state_->texture);return true;
#else
    return false;
#endif
}
}
