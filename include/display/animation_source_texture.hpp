#pragma once
#include "animation_source_node.hpp"
namespace ttyd {
namespace animation_texture_source {
constexpr uint32_t Function=0x8003dcfc,Get=0x80037b5c,Diagnostic=0x8026a2a8;
constexpr uint32_t Init=0x802b72b0,InitCi=0x802b74fc,Lod=0x802b7544,Load=0x802b7880;
constexpr uint32_t InitPalette=0x802b78d4,LoadPalette=0x802b790c,Generator=0x802b4e04,LoadMatrix=0x802ba3d0;
constexpr uint32_t Zero=0x8041fb28,One=0x8041fb24,Degrees=0x8041fb2c,Half=0x8041fb44,NegativeHalf=0x8041fb68;
constexpr uint32_t ByteBias=0x802c1060,Dispatch=0x80306678,Work=0x80415dc0,DiagnosticFormat=0x802c10c4;
constexpr uint32_t StateBytes=24,BaseBytes=8,DescriptorBytes=64;
}
// Seven independently captured source register arguments, in r3..r9 order.
struct AnimationSourceTextureInput {
 uint32_t count=0,ids=0,bases=0,states=0,descriptorCount=0,descriptors=0,resource=0;
};
enum class AnimationTextureRefKind {Scalar,Guest,Matrix,TextureObject,PaletteObject};
struct AnimationTextureArgument {AnimationTextureRefKind kind=AnimationTextureRefKind::Scalar;uint32_t value=0;};
struct AnimationTextureCall {
 uint32_t function=0,count=0;std::array<AnimationTextureArgument,9> arguments{};
 std::array<uint32_t,3> reals{}; // Original f1/f2/f3 binary32 ABI bits.
};
// Durable source stack identities; no invented guest stack addresses. Child
// object implementations retain their own partial local stores while false.
struct AnimationTextureLocals {
 enum Matrix {Scratch,Scale,Translation,Rotation};
 AnimationNodeLocals matrices{}; // Shared SDK adapter; first four slots used.
 std::array<bool,4> matrixAvailable{};
 std::array<uint8_t,32> object{};std::array<uint8_t,12> palette{};
};
enum class AnimationTextureArithmetic {Multiply,Subtract,ByteToFloat};
struct AnimationSourceTextureServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 // One complete source lfd; no independently retained high/low prefix.
 std::optional<std::array<uint32_t,2>> (*doubleWord)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
 // ByteToFloat receives byte and actual double-bias high/low words. Other
 // operations receive binary32 operands. No source finite guard is implied.
 std::optional<uint32_t> (*number)(void*,AnimationTextureArithmetic,std::array<uint32_t,3>)=nullptr;
 bool (*call)(void*,const AnimationTextureCall&,AnimationTextureLocals&,uint32_t&)=nullptr;
 bool (*matrix)(void*,const AnimationSourceMatrixCall&,AnimationTextureLocals&)=nullptr;
};
struct AnimationSourceTextureCursor {
 bool active=false,pending=false,matrixPending=false,ci=false;
 AnimationSourceTextureInput input;AnimationSourceTextureServices owner;
 uint32_t phase=0,j=0,slot=0,base=0,state=0,index=0,overrideWrap=0,kind=0,tplIndex=0;
 uint32_t handle=0,payload=0,pair=0,image=0,palette=0,wrapS=0,wrapT=0,min=0,max=0;
 uint32_t data=0,format=0,width=0,height=0,paletteCount=0,minFilter=0,magFilter=0,bias=0,edge=0;
 uint32_t biasHi=0,biasLo=0,zero=0,one=0,mask=0,literal=0,freshZero=0,target=0,selected=0,result=0;
 uint32_t work=0,pose=0,files=0,resourceIndex=0,animationIndex=0,name=0;
 std::array<uint32_t,5> transform{}; // angle,sx,sy,tx,ty in source read order.
 std::array<uint32_t,3> reals{};
 AnimationTextureLocals locals;AnimationTextureCall request;AnimationSourceMatrixCall matrixRequest;
};
// Complete raw animSetMaterial_Texture8003dcfc. Work budget counts native
// switch visits, not simulation ticks or source loop iterations.
bool resumeAnimationSourceTexture(const AnimationSourceTextureInput&,const AnimationSourceTextureServices&,
 AnimationSourceTextureCursor&,unsigned visits=128);
}
