#include "battle_window_packets.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {namespace {
bool camera(const BattleSubmenuCameraProof&p){return p.lifetime&&p.generation&&p.pass&&p.camera==8&&p.current&&p.current(p.user,p);}
bool finite(const BattleActMatrix&m){for(float f:m)if(!std::isfinite(f))return false;return true;}
bool texture(const BattleWindowTextureObject&t){const auto&i=t.image;return t.address&&t.lifetime&&t.current&&t.current(t.user)&&i.index==0&&i.imageHeader&&i.data&&i.width==72&&i.height==72&&i.format==14&&!i.paletteHeader&&!i.wrapS&&!i.wrapT&&i.minFilter==1&&i.magFilter==1&&i.bias==0&&!i.edge&&!i.minLOD&&!i.maxLOD&&t.bytes==std::array<uint32_t,8>{0x190,0,0xe11c47,(i.data>>5)&0x1fffff,0,14,0,0x00510002};}
bool finiteShader(const BattleWindowPacket&p){
 CameraProjectionMatrix composed{};
 for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){float sum=0;for(unsigned k=0;k<3;++k){volatile float product=p.camera.projection[r][k]*p.matrix[4*k+c];if(!std::isfinite(product))return false;volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;}if(c==3){volatile float next=sum+p.camera.projection[r][3];if(!std::isfinite(next))return false;sum=next;}composed[r][c]=sum;}
 for(const auto&vertex:p.vertices)for(unsigned row=0;row<4;++row){float sum=0;for(unsigned k=0;k<4;++k){volatile float product=composed[row][k]*(k==3?1.f:vertex[k]);if(!std::isfinite(product))return false;volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;}}
 return true; // Native tilt only exchanges/negates finite clip lanes.
}
bool identical(const WindowPrimitiveRequest&a,const WindowPrimitiveRequest&b){if(a.kind!=b.kind||a.function!=b.function||a.callsite!=b.callsite||a.integers!=b.integers||a.reals.size()!=b.reals.size()||a.matrix!=b.matrix||a.color!=b.color)return false;for(size_t i=0;i<a.reals.size();++i)if(std::memcmp(&a.reals[i],&b.reals[i],4))return false;return true;}
}
bool BattleWindowSurfaceLease::current()const{return generation&&generation->resident_&&texture&&texture==generation->texture_;}
BattleWindowSurfaceResource::~BattleWindowSurfaceResource(){retire();}
bool BattleWindowSurfaceResource::adopt(const BattleClassSurfaceImport&i,const uint8_t*data,size_t bytes){
 if(generation_||!data||bytes!=128*128*4||!i.identity||i.width!=128||i.height!=128||i.format||i.count!=1||i.subWidth!=128||i.subHeight!=128||i.rotated||i.uv!=std::array<float,4>{0,1,1,0})return false;
 // Actual C3D RGBA8 tiled ABGR memory. Hash original row-major RGBA plus ALL
 // clamp padding; source converter independently pins ISO/SHA and exact pixels.
 uint64_t hash=14695981039346656037ULL;
 for(unsigned y=0;y<128;++y)for(unsigned x=0;x<128;++x){unsigned morton=0;for(unsigned j=0;j<3;++j)morton|=((x>>j)&1)<<(2*j)|((y>>j)&1)<<(2*j+1);const auto offset=4*(((y/8)*16+x/8)*64+morton);for(unsigned j=0;j<4;++j){hash^=data[offset+3-j];hash*=1099511628211ULL;}}
 if(hash!=0xcaadb76327b7d2beULL)return false;
 generation_=std::shared_ptr<BattleWindowSurfaceGeneration>(new BattleWindowSurfaceGeneration);generation_->texture_=i.identity;generation_->resident_=true;return true;
}
void BattleWindowSurfaceResource::retire(){if(generation_)generation_->resident_=false;generation_.reset();}
BattleWindowSurfaceLease BattleWindowSurfaceResource::lease()const{return generation_?BattleWindowSurfaceLease{generation_,generation_->texture_}:BattleWindowSurfaceLease{};}
bool BattleWindowSurfaceResource::accepts(const BattleWindowSurfaceLease&l)const{return l.current()&&generation_&&l.generation==generation_;}
bool validateBattleWindowPacket(const BattleWindowPacket&p,const BattleWindowSurfaceResource&r){if(!r.accepts(p.surface)||!camera(p.camera)||!texture(p.source)||!finite(p.matrix))return false;for(const auto&row:p.camera.projection)for(float v:row)if(!std::isfinite(v))return false;if(!nativeCameraRect(p.camera.viewport[0],p.camera.viewport[1],p.camera.viewport[2],p.camera.viewport[3])||!nativeCameraRect(float(p.camera.scissor[0]),float(p.camera.scissor[1]),float(p.camera.scissor[2]),float(p.camera.scissor[3])))return false;const auto&v=p.camera.viewport;if(!std::isfinite(v[4])||!std::isfinite(v[5])||v[4]<0||v[4]>v[5]||v[5]>1)return false;for(const auto&vertex:p.vertices)for(float f:vertex)if(!std::isfinite(f))return false;return finiteShader(p);}
CameraProjectionMatrix battleWindowTransform(const BattleWindowPacket&p){CameraViewMatrix m{};for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c)m[r][c]=p.matrix[4*r+c];return nativeCameraTransform(p.camera.projection,m);}
BattleWindowPacketProvider::BattleWindowPacketProvider(const BattleWindowSurfaceResource&r,BattleSubmenuCameraProof c,BattleWindowQueryServices q,BattleWindowPacketSink sink):resource_(r),camera_(std::move(c)),query_(std::move(q)),sink_(sink){}
bool BattleWindowPacketProvider::current(bool sourceOnly){
 if(poisoned_)return false;
 if(!camera(camera_)||!query_.lifetime||!query_.current||!query_.current(query_.user)){poisoned_=true;return false;}
 if(!sourceOnly&&((lease_.generation&&(!lease_.current()||!resource_.accepts(lease_)))||(texture_&&!texture(*texture_)))){poisoned_=true;return false;}
 return true;
}
std::optional<uint32_t> BattleWindowPacketProvider::request(const WindowPrimitiveRequest&q){
 if(invoking_)return {};
 invoking_=true;struct Guard{bool&v;~Guard(){v=false;}}guard{invoking_};const bool prefix=q.kind==WindowPrimitiveRequestKind::Call&&q.function==0x8000f2e4&&!lanes_;if(!current(prefix))return {};
 if(q.kind==WindowPrimitiveRequestKind::FifoFloat){
  if(!lanes_||!q.integers.empty()||q.reals.size()!=1||q.color||q.matrix||!std::isfinite(q.reals[0]))return {};
  const auto ordinal=lanes_-1;auto vertices=vertices_;vertices[ordinal/5][ordinal%5]=q.reals[0];
  if(ordinal==179){if(!pending_){BattleWindowPacket p;p.surface=lease_;p.camera=camera_;p.source=*texture_;p.matrix=*matrix_;p.rgba=*color_;p.vertices=vertices;pending_=std::make_shared<const BattleWindowPacket>(std::move(p));last_=q;}if(!identical(q,*last_)||!validateBattleWindowPacket(*pending_,resource_)||!sink_.append)return {};const bool accepted=sink_.append(sink_.user,pending_);if(!current()||!accepted)return {};pending_.reset();last_.reset();lanes_=0;}else ++lanes_;
  vertices_=vertices;return 0;
 }
 if(lanes_||q.kind!=WindowPrimitiveRequestKind::Call)return {};
 const auto args=[&](std::initializer_list<uint32_t>a){return q.integers==std::vector<uint32_t>(a)&&q.reals.empty()&&!q.color&&!q.matrix;};
 uint32_t flag=0;bool valid=false;
 switch(q.function){
 case 0x8000f2e4:{if(!args({})||!query_.camera)return {};auto v=query_.camera(query_.user);if(!current(true)||!v)return {};lease_={};state_=descriptors_=formats_=0;matrix_.reset();color_.reset();texture_.reset();return v;}
 case 0x802b5c8c:flag=1u<<0;valid=args({0});break;
 case 0x802b940c:flag=1u<<1;valid=args({1});break;
 case 0x802b8d60:flag=1u<<2;valid=args({7,0,0,7,0});break;
 case 0x802b932c:flag=1u<<3;valid=args({1,4,5,7});break;
 case 0x802b93d8:flag=1u<<4;valid=args({1,7,0});break;
 case 0x802b8ff4:flag=1u<<5;valid=q.integers==std::vector<uint32_t>{0}&&q.reals==std::vector<float>{0,0,0,0}&&q.color&&!q.matrix;break;
 case 0x802b4520:flag=1u<<20;valid=args({});if(valid){descriptors_=formats_=0;}break;
 case 0x802b3c08:if(args({9,1})){descriptors_|=1;valid=true;}else if(args({13,1})){descriptors_|=2;valid=true;}break;
 case 0x802b4558:if(args({0,9,1,4,0})){formats_|=1;valid=true;}else if(args({0,13,1,4,0})){formats_|=2;valid=true;}break;
 case 0x802b4e04:flag=1u<<6;valid=args({0,1,4,60,0,125});break;
 case 0x802b6f3c:flag=1u<<7;valid=args({0});break;
 case 0x802b5084:flag=1u<<8;valid=args({1});break;
 case 0x802b8fcc:flag=1u<<9;valid=args({1});break;
 case 0x802b8e30:flag=1u<<10;valid=args({0,0,0,255});break;
 case 0x802b89e8:flag=1u<<11;valid=args({0,0,0,0,1,0});break;
 case 0x802b8a50:flag=1u<<12;valid=args({0,0,0,0,1,0});break;
 case 0x802b8960:flag=1u<<13;valid=args({0,15,2,8,15});break;
 case 0x802b89a4:flag=1u<<14;valid=args({0,7,1,4,7});break;
 case 0x802b8c98:flag=1u<<15;valid=args({0,0,0});break;
 case 0x802ba39c:flag=1u<<16;valid=args({0});break;
 case 0x802b8ab8:flag=1u<<17;valid=q.integers==std::vector<uint32_t>{1}&&q.color&&q.reals.empty()&&!q.matrix;if(valid)color_=*q.color;break;
 case 0x802ba2fc:flag=1u<<18;valid=q.integers==std::vector<uint32_t>{0}&&q.matrix&&finite(*q.matrix)&&q.reals.empty()&&!q.color;if(valid)matrix_=*q.matrix;break;
 case 0x802b7880:{if(!lease_.generation)lease_=resource_.lease();if(!resource_.accepts(lease_))return {};if(q.integers.size()!=2||q.integers[1]||!q.reals.empty()||q.color||q.matrix||!query_.texture)return {};auto t=query_.texture(query_.user,q.integers[0]);if(!current()||!t||t->address!=q.integers[0]||!texture(*t))return {};texture_=*t;flag=1u<<19;valid=true;break;}
 case 0x802b5a6c:if(!args({128,0,36})||state_!=0x1fffff||descriptors_!=3||formats_!=3||!texture_||!matrix_||!color_||!sink_.append)return {};lanes_=1;vertices_={};return 0;
 default:return {};
 }
 if(!valid)return {};
 state_|=flag;return current()?std::optional<uint32_t>(0):std::nullopt;
}
}
