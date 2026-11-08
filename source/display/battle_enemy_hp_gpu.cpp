#include "battle_enemy_hp_gpu.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
BattleEnemyHpQuadSink BattleEnemyHpGpuBatch::sink(){return {this,[](void* p,const BattleEnemyHpGpuQuad& q){static_cast<BattleEnemyHpGpuBatch*>(p)->quads.push_back(q);return true;}};}
bool BattleEnemyHpGpuResource::load(const char* root,const BattleEnemyHpTextureServices& s){
 if(ready() || !root || !s.import || !s.release)return false;
 const std::string base=root;FILE* file=std::fopen((base+"/battle-enemy-hp-surfaces.bin").c_str(),"rb");if(!file)return false;
 // PMBEHP01 little-endian ABI: 8-byte magic + count u32 + two 28-byte
 // rows (index/width/height u32, then four binary32 UVs), with exact EOF.
 uint8_t raw[68];const bool ok=std::fread(raw,1,sizeof(raw),file)==sizeof(raw) && std::fgetc(file)==EOF;std::fclose(file);
 if(!ok || std::memcmp(raw,"PMBEHP01",8))return false;
 const auto word=[&](unsigned o){return uint32_t(raw[o])|uint32_t(raw[o+1])<<8|uint32_t(raw[o+2])<<16|uint32_t(raw[o+3])<<24;};
 if(word(8)!=2)return false;
 // Original bank 17/18: 40x16 cells at (2,2)/(46,2) in a 128x32
 // native atlas. V flips once: (32-2)/32 to (32-18)/32; pitch is 44.
 std::array<BattleClassRegion,2> regions;
 for(unsigned i=0;i<2;++i){const auto at=12+28*i;if(word(at)!=17+i || word(at+4)!=40 || word(at+8)!=16)return false;
   auto& r=regions[i];r.width=40;r.height=16;float* values[]={&r.u0,&r.v0,&r.u1,&r.v1};
   for(unsigned j=0;j<4;++j){const auto n=word(at+12+4*j);std::memcpy(values[j],&n,4);}
   const float x=float(2+44*i);if(r.u0!=x/128 || r.v0!=30.f/32 || r.u1!=(x+40)/128 || r.v1!=14.f/32)return false;
 }
 const auto imported=s.import(s.user,(base+"/battle-enemy-hp-surfaces.t3x").c_str());if(!imported)return false;
 if(!imported->identity || imported->width!=128 || imported->height!=32 || imported->format!=0){if(imported->identity)s.release(s.user,imported->identity);return false;}
 // uint8_t value 0 carries no ID: shared-object identity is the reload epoch.
 regions_=regions;admission_=*imported;owner_=s;generation_=std::make_shared<const uint8_t>(0);return true;
}
// Explicit, externally serialized teardown: release runs before local fields
// clear. Callers must retire publications and must not reenter from callbacks.
void BattleEnemyHpGpuResource::retire(){if(ready())owner_.release(owner_.user,admission_.identity);admission_={};owner_={};generation_.reset();regions_={};}
const BattleClassRegion* BattleEnemyHpGpuResource::surface(uint32_t index)const{return ready() && index>=17 && index<=18?&regions_[index-17]:nullptr;}
bool BattleEnemyHpGpuResource::accepts(const BattleEnemyHpGpuQuad& q)const{
 if(!ready() || !q.generation || q.generation!=generation_)return false;
 const BattleClassRegion* region=nullptr;
 switch(q.material){
 case BattleEnemyHpMaterial::UntexturedRaster:if(q.texture || q.index)return false;break;
 case BattleEnemyHpMaterial::TexturedModulate:case BattleEnemyHpMaterial::TexturedRaster:
   region=surface(q.index);if(!region || q.texture!=identity())return false;break;
 default:return false;
 }
 for(const auto& v:q.vertices){if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z)||!std::isfinite(v.u)||!std::isfinite(v.v))return false;
   if(region){if(v.u<region->u0 || v.u>region->u1 || v.v>region->v0 || v.v<region->v1)return false;}
   else if(v.u!=0 || v.v!=0)return false;
 }return true;
}
// Apply row-major affine 3x4 XYZ, then map normalized ST into one atlas cell.
// Solid material uses index 0/null texture/zero UV but still pins this HP epoch.
bool BattleEnemyHpGpuResource::quad(uint32_t index,BattleEnemyHpMaterial material,const BattleActMatrix& matrix,
 const std::array<std::array<float,3>,4>& positions,const std::array<std::array<float,2>,4>& uv,
 const std::array<uint32_t,4>& colors,const BattleEnemyHpQuadSink& sink)const{
 if(!ready() || !sink.append)return false;
 for(auto f:matrix)if(!std::isfinite(f))return false;
 const auto* r=surface(index);const bool solid=material==BattleEnemyHpMaterial::UntexturedRaster;
 if(solid?index!=0:!r)return false;
 BattleEnemyHpGpuQuad q;q.texture=solid?nullptr:identity();q.generation=generation_;q.index=index;q.material=material;
 for(unsigned i=0;i<4;++i){for(auto f:positions[i])if(!std::isfinite(f))return false;
   auto& v=q.vertices[i];const auto& p=positions[i];
   v.x=matrix[0]*p[0]+matrix[1]*p[1]+matrix[2]*p[2]+matrix[3];v.y=matrix[4]*p[0]+matrix[5]*p[1]+matrix[6]*p[2]+matrix[7];v.z=matrix[8]*p[0]+matrix[9]*p[1]+matrix[10]*p[2]+matrix[11];v.rgba=colors[i];
   if(!solid){for(auto f:uv[i])if(!std::isfinite(f)||f<0||f>1)return false;v.u=r->u0+(r->u1-r->u0)*uv[i][0];v.v=r->v0+(r->v1-r->v0)*uv[i][1];}
 }
 return accepts(q) && sink.append(sink.user,q);
}
bool validateBattleEnemyHpBatch(const BattleEnemyHpGpuBatch& batch,const BattleEnemyHpGpuResource* resource){for(const auto& q:batch.quads)if(!resource || !resource->accepts(q))return false;return true;}
// Read the executing VM bank, not native atlas metadata. 217920 is the exact
// original common TPL byte length; uint64 bounds keep the whole bank in 32-bit VM.
std::optional<AnimationTPLImage> readBattleEnemyHpTexture(const BattleEnemyHpMemoryReader& m,uint32_t palette,uint32_t index){
 if(!m.word || index<17 || index>18 || uint64_t(palette)+217920>0x100000000ULL)return {};
 const auto read=[&](uint64_t a)->std::optional<uint32_t>{return a<=0xffffffffULL?m.word(m.user,uint32_t(a)):std::nullopt;};
 // Live B global -> B+0x163f8 FileEntry -> +0xa0 pointer cell -> palette.
 const auto b=read(0x8041eb90);if(!b || !*b)return {};
 const auto desc=read(uint64_t(*b)+0x163f8);if(!desc || !*desc)return {};
 const auto holder=read(uint64_t(*desc)+0xa0);if(!holder || !*holder)return {};
 const auto actual=read(*holder);if(!actual || *actual!=palette)return {};
 // TPL magic 0x0020af30, 97 descriptors, table at +12. Each descriptor
 // is two words; HP headers begin +0x578 at 36-byte stride, with no CLUT.
 const auto magic=read(palette),count=read(uint64_t(palette)+4),table=read(uint64_t(palette)+8);
 if(!magic || *magic!=0x0020af30 || !count || *count!=97 || !table || *table!=uint64_t(palette)+12)return {};
 const auto header=read(uint64_t(*table)+index*8),clut=read(uint64_t(*table)+index*8+4);
 if(!header || *header!=uint64_t(palette)+0x578+(index-17)*36 || !clut || *clut)return {};
 // Header words: packed H16/W40, CMPR=14, pixel pointer (+0xb100,
 // 320-byte image stride), clamp S/T=0, linear min/mag=1, bias/LOD=0,
 // and final source relocation/unpacked marker 1.
 const uint32_t expected[9]={0x00100028,14,palette+0xb100+(index-17)*320,0,0,1,1,0,1};
 for(unsigned i=0;i<9;++i){const auto n=read(uint64_t(*header)+4*i);if(!n || *n!=expected[i])return {};}
 AnimationTPLImage image;image.index=index;image.imageHeader=*header;image.width=40;image.height=16;image.format=14;image.data=expected[2];image.minFilter=1;image.magFilter=1;return image;
}
BattleEnemyHpGpuProvider::BattleEnemyHpGpuProvider(const BattleEnemyHpGpuResource& r,BattleEnemyHpQueryServices q,BattleEnemyHpQuadSink sink):resource_(r),generation_(r.generation()),query_(q),sink_(sink){}
bool BattleEnemyHpGpuProvider::ready()const{return generation_ && resource_.ready() && generation_==resource_.generation();}
std::optional<uint32_t> BattleEnemyHpGpuProvider::request(const BattleTexturePlaneRequest& q){
 if(!ready())return {};
 using K=BattleTexturePlaneKind;using F=BattleTexturePlaneCall;
 if(q.kind!=K::Call){
   if(!lanes_ || !material_ || !matrix_)return {};
   // lanes_ is one-based pending ordinal. Each vertex is XYZ (3 floats),
   // RGBA (4 bytes, high byte first), and optionally ST (2 floats).
   const unsigned ordinal=lanes_-1,vertex=ordinal/stride_,lane=ordinal%stride_;
   auto positions=positions_;auto uv=uv_;auto colors=colors_;
   if(lane>=3 && lane<=6){if(q.kind!=K::FifoByte || q.integers.size()!=1 || q.integers[0]>255 || !q.reals.empty() || q.matrix)return {};
     const unsigned shift=24-8*(lane-3);colors[vertex]=(colors[vertex]&~(255u<<shift))|(q.integers[0]<<shift);
   }else {if(q.kind!=K::FifoFloat || q.reals.size()!=1 || !std::isfinite(q.reals[0]) || !q.integers.empty() || q.matrix)return {};
     if(lane<3)positions[vertex][lane]=q.reals[0];else uv[vertex][lane-7]=q.reals[0];
   }
   // Work on private candidate arrays: a refused final append leaves every
   // prior lane and the final operand retryable, without partial publication.
   if(ordinal==4*stride_-1){const bool solid=*material_==BattleEnemyHpMaterial::UntexturedRaster;
     if(!resource_.quad(solid?0:*texture_,*material_,*matrix_,positions,uv,colors,sink_))return {};
     lanes_=0;
   }else ++lanes_;
   positions_=positions;uv_=uv;colors_=colors;return 0;
 }
 if(lanes_ || !q.reals.empty())return {};
 const auto& n=q.integers;
 if(q.function==F::CurrentCamera){if(!n.empty() || q.matrix || !query_.currentCamera)return {};return query_.currentCamera(query_.user);}
 if(q.function==F::LoadMatrix){if(n!=std::vector<uint32_t>{0} || !q.matrix)return {};for(auto f:*q.matrix)if(!std::isfinite(f))return {};matrix_=*q.matrix;return 0;}
 if(q.matrix)return {};
 if(q.function==F::TextureObject){
   if(n.size()!=2 || !resource_.surface(n[1]) || !query_.texture)return {};
   const auto image=query_.texture(query_.user,n[0],n[1]);
   if(!image || image->index!=n[1] || image->width!=40 || image->height!=16 || image->format!=14 || !image->imageHeader || !image->data || image->wrapS || image->wrapT || image->minFilter!=1 || image->magFilter!=1 || image->bias!=0 || image->minLOD || image->maxLOD || image->edge)return {};
   // Index-valued handles are valid only after this owner admits TextureObject;
   // raw source indices cannot bypass the retained descriptor query.
   objects_[n[1]-17]=*image;return n[1]; // Actual object admitted in this owner.
 }
 if(q.function==F::LoadTexture){if(n.size()!=2 || n[1] || n[0]<17 || n[0]>18 || !objects_[n[0]-17])return {};texture_=n[0];return 0;}
 if(q.function==F::TextureFormat || q.function==F::TextureWidth || q.function==F::TextureHeight){
   if(n.size()!=1 || n[0]<17 || n[0]>18 || !objects_[n[0]-17])return {};
   const auto& image=*objects_[n[0]-17];return q.function==F::TextureFormat?image.format:q.function==F::TextureWidth?image.width:image.height;
 }
 // Begin is GX_QUADS=128 (0x80), vertex format 0, count 4. fixed_=31
 // means all five InitSub setters were seen (bits 1/2/4/8/16 below).
 if(q.function==F::Begin){
   if(n!=std::vector<uint32_t>({128,0,4}) || !matrix_ || !sink_.append || fixed_!=31 || !cull_ || !channels_ || !tev_ || !colorOp_ || !alphaOp_)return {};
   // Descriptor/format masks: POS=1, CLR0=2, TEX0=4. Untextured stride
   // 7 gives 28 scalar lanes; textured stride 9 gives 36. Profile codes
   // channel/rgb/alpha/order below are local recognizer states, not GX enums.
   if(generators_==0 && descriptors_==3 && attributes_==3 && channel_==1 && rgb_==1 && alpha_==1 && order_==1){material_=BattleEnemyHpMaterial::UntexturedRaster;stride_=7;}
   else if(generators_==1 && descriptors_==7 && attributes_==7 && channel_==2 && texgen_ && texture_ && alpha_==2 && order_==2 && (rgb_==1 || rgb_==2)){material_=rgb_==1?BattleEnemyHpMaterial::TexturedRaster:BattleEnemyHpMaterial::TexturedModulate;stride_=9;}
   else return {};
   lanes_=1;positions_={};uv_={};colors_={};return 0;
 }
 // Exact accepted GX profile. Depth-location 1 is before texturing; alpha
 // compare ALWAYS(7)/0 AND(0) ALWAYS/0; blend BLEND(1), SRCALPHA(4),
 // INVSRCALPHA(5), logic NOOP(5); depth enable=1, ALWAYS=7, no Z writes.
 // CULL_NONE=0. DIRECT=1; POS=9/XYZ=1/F32=4, CLR0=11/RGBA=1/
 // RGBA8=5, TEX0=13/ST=1/F32=4, all in format 0 with fraction bits 0.
 bool valid=false;
 switch(q.function){
 case F::DepthLocation:valid=n==std::vector<uint32_t>{1};if(valid)fixed_|=1;break;
 case F::AlphaCompare:valid=n==std::vector<uint32_t>({7,0,0,7,0});if(valid)fixed_|=2;break;
 case F::BlendMode:valid=n==std::vector<uint32_t>({1,4,5,5});if(valid)fixed_|=4;break;
 case F::DepthMode:valid=n==std::vector<uint32_t>({1,7,0});if(valid)fixed_|=8;break;
 case F::Cull:valid=n==std::vector<uint32_t>{0};if(valid){cull_=true;fixed_|=16;}break;
 case F::ClearDescriptors:valid=n.empty();if(valid){descriptors_=0;attributes_=0;}break;
 case F::Descriptor:valid=n.size()==2 && n[1]==1 && (n[0]==9 || n[0]==11 || n[0]==13);if(valid)descriptors_|=n[0]==9?1:n[0]==11?2:4;break;
 case F::AttributeFormat:
   if(n==std::vector<uint32_t>({0,9,1,4,0})){valid=true;attributes_|=1;}
   else if(n==std::vector<uint32_t>({0,11,1,5,0})){valid=true;attributes_|=2;}
   else if(n==std::vector<uint32_t>({0,13,1,4,0})){valid=true;attributes_|=4;}
   break;
 case F::Channels:valid=n==std::vector<uint32_t>{1};if(valid)channels_=true;break;
 // COLOR0A0=4, lighting off; ambient VTX=1 (solid) or REG=0
 // (textured), material VTX=1, no lights, diffuse CLAMP=2, attenuation NONE=2.
 case F::ChannelControl:if(n==std::vector<uint32_t>({4,0,1,1,0,2,2})){valid=true;channel_=1;}else if(n==std::vector<uint32_t>({4,0,0,1,0,2,2})){valid=true;channel_=2;}break;
 case F::TextureGenerators:valid=n==std::vector<uint32_t>{0} || n==std::vector<uint32_t>{1};if(valid)generators_=n[0];break;
 // Texgen MATRIX2x4=1 from TEX0=4, identity=60/postidentity=125,
 // normalize=0. TEV stage 0: ADD=0, bias ZERO=0, scale 1 token=0,
 // clamp=1, PREV=0. RGB ZERO=15/RASC=10; alpha ZERO=7/RASA=5/TEXA=4.
 // Order coord/map 255 means NULL for solid, 0/0 for textured; channel=4.
 case F::TextureGenerator:valid=n==std::vector<uint32_t>({0,1,4,60,0,125});if(valid)texgen_=true;break;
 case F::TevStages:valid=n==std::vector<uint32_t>{1};if(valid)tev_=true;break;
 case F::ColorOp:valid=n==std::vector<uint32_t>({0,0,0,0,1,0});if(valid)colorOp_=true;break;
 case F::AlphaOp:valid=n==std::vector<uint32_t>({0,0,0,0,1,0});if(valid)alphaOp_=true;break;
 case F::ColorIn:valid=n==std::vector<uint32_t>({0,15,15,15,10});if(valid)rgb_=1;break;
 case F::AlphaIn:if(n==std::vector<uint32_t>({0,7,7,7,5})){valid=true;alpha_=1;}else if(n==std::vector<uint32_t>({0,7,5,4,7})){valid=true;alpha_=2;}break;
 case F::Order:if(n==std::vector<uint32_t>({0,255,255,4})){valid=true;order_=1;}else if(n==std::vector<uint32_t>({0,0,0,4})){valid=true;order_=2;}break;
 // GX_MODULATE=0 selects the local rgb/alpha profile code 2.
 case F::TevOp:valid=n==std::vector<uint32_t>({0,0});if(valid){rgb_=2;alpha_=2;}break;
 default:break;
 }
 return valid?std::optional<uint32_t>(0):std::nullopt;
}
#ifdef __3DS__
bool NativeBattleEnemyHpGpu::load(const char* root){
 BattleEnemyHpTextureServices s;s.user=this;
 s.import=[](void* p,const char* path)->std::optional<BattleEnemyHpTextureAdmission>{
   auto& o=*static_cast<NativeBattleEnemyHpGpu*>(p);if(o.imported_)return {};
   FILE* file=std::fopen(path,"rb");if(!file)return {};C3D_FrameSync();
   o.imported_=Tex3DS_TextureImportStdio(file,&o.texture_,nullptr,false);std::fclose(file);if(!o.imported_)return {};
   C3D_TexSetFilter(&o.texture_,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&o.texture_,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
   return BattleEnemyHpTextureAdmission{&o.texture_,o.texture_.width,o.texture_.height,uint32_t(o.texture_.fmt)};
 };
 s.release=[](void* p,const void* id){auto& o=*static_cast<NativeBattleEnemyHpGpu*>(p);if(id!=&o.texture_ || !o.imported_)return;
   C3D_FrameSync();Tex3DS_TextureFree(o.imported_);C3D_TexDelete(&o.texture_);o.imported_=nullptr;o.texture_={};};
 return resource_.load(root,s);
}
void NativeBattleEnemyHpGpu::release(){resource_.retire();}
bool NativeBattleEnemyHpGpu::draw(const BattleEnemyHpGpuBatch& batch,shaderProgram_s* shader,int uniform,const C3D_Mtx& projection,C3D_Tex* parked){
 if(!validateBattleEnemyHpBatch(batch,&resource_) || !shader || uniform<0 || !parked)return false;
 if(batch.quads.empty())return true;
 // Entire batch preflight precedes GPU state changes. Shader attributes are
 // XYZ/ST/RGBA; all three sampler slots are valid and six TEV stages reset.
 C3D_BindProgram(shader);auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
 AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,uniform,&projection);
 for(const auto& q:batch.quads){
   const bool solid=q.material==BattleEnemyHpMaterial::UntexturedRaster;
   C3D_TexBind(0,solid?parked:&texture_);C3D_TexBind(1,parked);C3D_TexBind(2,parked);
   for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
   auto* env=C3D_GetTexEnv(0);
   if(q.material==BattleEnemyHpMaterial::TexturedModulate){C3D_TexEnvSrc(env,C3D_RGB,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_RGB,GPU_MODULATE);}
   else {C3D_TexEnvSrc(env,C3D_RGB,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_RGB,GPU_REPLACE);}
   if(solid){C3D_TexEnvSrc(env,C3D_Alpha,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_REPLACE);}
   else {C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);}
   // HP source profile: ALWAYS depth with color writes only, no alpha reject.
   // Native blending here uses source alpha for BOTH RGB and alpha channels.
   C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);
   C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
   C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
   // Packed RGBA bytes normalize by 255; ordered four vertices form a fan.
   for(const auto& v:q.vertices){C3D_ImmSendAttrib(v.x,v.y,v.z,1);C3D_ImmSendAttrib(v.u,v.v,0,1);C3D_ImmSendAttrib(float(v.rgba>>24)/255,float((v.rgba>>16)&255)/255,float((v.rgba>>8)&255)/255,float(v.rgba&255)/255);}
   C3D_ImmDrawEnd();
 }
 return true;
}
#endif
}
