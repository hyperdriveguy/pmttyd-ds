#include "animation_source_texture.hpp"
#include <cstring>
namespace ttyd {
namespace {
using K=AnimationTextureRefKind;using A=AnimationTextureArgument;
A scalar(uint32_t x){return {K::Scalar,x};}A guest(uint32_t x){return {K::Guest,x};}
A object(){return {K::TextureObject,0};}A palette(){return {K::PaletteObject,0};}A matrix(uint32_t x){return {K::Matrix,x};}
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
bool same(const AnimationSourceTextureInput& a,const AnimationSourceTextureInput& b){return a.count==b.count && a.ids==b.ids && a.bases==b.bases && a.states==b.states && a.descriptorCount==b.descriptorCount && a.descriptors==b.descriptors && a.resource==b.resource;}
bool same(const AnimationSourceTextureServices& a,const AnimationSourceTextureServices& b){return a.user==b.user && a.word==b.word && a.byte==b.byte && a.doubleWord==b.doubleWord && a.half==b.half && a.number==b.number && a.call==b.call && a.matrix==b.matrix;}
}
bool resumeAnimationSourceTexture(const AnimationSourceTextureInput& input,const AnimationSourceTextureServices& s,AnimationSourceTextureCursor& c,unsigned visits){
 using namespace animation_texture_source;using L=AnimationTextureLocals;
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}else if(!same(input,c.input) || !same(s,c.owner))return false;
 const auto& in=c.input;
 auto word=[&](uint32_t a,uint32_t& v){auto x=s.word?s.word(s.user,a):std::nullopt;if(!x)return false;v=*x;return true;};
 auto byte=[&](uint32_t a,uint32_t& v){auto x=s.byte?s.byte(s.user,a):std::nullopt;if(!x)return false;v=*x;return true;};
 auto half=[&](uint32_t a,uint32_t& v){auto x=s.half?s.half(s.user,a):std::nullopt;if(!x)return false;v=*x;return true;};
 auto number=[&](AnimationTextureArithmetic op,uint32_t a,uint32_t b,uint32_t d,uint32_t& out){auto x=s.number?s.number(s.user,op,{a,b,d}):std::nullopt;if(!x)return false;out=*x;return true;};
 auto call=[&](uint32_t fn,std::initializer_list<A> args){if(!c.pending){c.request={};c.request.function=fn;c.request.count=args.size();unsigned i=0;for(auto a:args)c.request.arguments[i++]=a;c.request.reals=c.reals;c.pending=true;c.result=0;}if(!s.call || !s.call(s.user,c.request,c.locals,c.result))return false;c.pending=false;return true;};
 auto sdk=[&](uint32_t fn,unsigned out,unsigned left,unsigned right,std::array<uint32_t,3> xyz,uint32_t radians=0){if(!c.matrixPending){c.matrixRequest={};auto& q=c.matrixRequest;q.function=fn;q.output={false,out};q.left={false,left};q.right={false,right};for(unsigned i=0;i<3;++i)q.xyz[i]=real(xyz[i]);q.radians=real(radians);c.matrixPending=true;}if(!s.matrix || !s.matrix(s.user,c.matrixRequest,c.locals))return false;c.matrixPending=false;return true;};
 constexpr uint32_t Translate=0x8029e230,Rotate=0x8029dff0,Concat=0x8029dd64,Scale=0x8029e2b0;
 for(unsigned visit=0;visit<visits;++visit)switch(c.phase){
 // Pass1: signed count and low32 arithmetic, without invented slot bounds.
 case 0:if(int32_t(c.j)>=int32_t(in.count)){c.j=0;c.phase=100;break;}++c.phase;break;
 case 1:if(!word(in.ids+4*(in.count-c.j-1),c.slot))return false;c.base=in.bases+BaseBytes*c.slot;c.state=in.states+StateBytes*c.slot;++c.phase;break;
 case 2:if(!word(c.base,c.index))return false;++c.phase;break;
 case 3:if(!byte(c.state,c.result))return false;c.index+=c.result;c.phase=int32_t(c.index)<0 || int32_t(c.index)>=int32_t(in.descriptorCount)?10:25;break;
 // Invalid index executes the real diagnostic before selecting descriptor0.
 case 10:c.index=0;if(!word(Work,c.work))return false;++c.phase;break;
 case 11:if(!word(c.work+0x18,c.pose))return false;++c.phase;break;
 case 12:if(!word(c.work,c.files))return false;++c.phase;break;
 case 13:if(!word(c.pose+0x10,c.resourceIndex))return false;++c.phase;break;
 case 14:if(!word(c.pose+0x14,c.animationIndex))return false;++c.phase;break;
 case 15:if(!word(c.files+16*c.resourceIndex+8,c.handle))return false;++c.phase;break;
 case 16:if(!word(c.handle+0xa0,c.handle))return false;++c.phase;break;
 case 17:if(!word(c.handle,c.payload))return false;++c.phase;break;
 case 18:if(!word(c.payload+0x1ac,c.name))return false;++c.phase;break;
 case 19:if(!call(Diagnostic,{guest(DiagnosticFormat),guest(c.payload+4),guest(c.name+64*c.animationIndex)}))return false;c.phase=25;break;
 case 25:if(!word(c.base+4,c.overrideWrap))return false;++c.phase;break;
 case 26:if(!word(in.descriptors+DescriptorBytes*c.index+8,c.kind))return false;++c.phase;break;
 case 27:if(!word(in.descriptors+DescriptorBytes*c.index+4,c.tplIndex))return false;c.phase=c.kind?90:30;break;
 case 30:case 35:if(!word(in.resource+8,c.handle))return false;++c.phase;break;
 case 31:case 36:if(!word(c.handle+0xa0,c.handle))return false;++c.phase;break;
 case 32:case 37:if(!word(c.handle,c.payload))return false;++c.phase;break;
 case 33:case 38:if(!call(Get,{guest(c.payload),scalar(c.tplIndex)}))return false;c.pair=c.result;++c.phase;break;
 case 34:if(!word(c.pair+4,c.palette))return false;c.ci=c.palette!=0;c.phase=35;break;
 case 39:if(!word(c.pair,c.image))return false;++c.phase;break;
 case 40:if(!word(c.image+0xc,c.wrapS))return false;++c.phase;break;
 case 41:if(!word(c.image+0x10,c.wrapT))return false;
  if(int32_t(c.overrideWrap)>=0){c.wrapS=c.overrideWrap&4?2:c.overrideWrap&1;c.wrapT=c.overrideWrap&8?2:(c.overrideWrap>>1)&1;}++c.phase;break;
 case 42:if(!byte(c.image+0x21,c.min))return false;c.phase=c.ci?43:44;break;
 case 43:if(!word(c.pair+4,c.palette))return false;++c.phase;break;
 case 44:if(!byte(c.image+0x22,c.max))return false;c.phase=c.ci?45:51;break;
 case 45:if(!word(c.palette+8,c.data))return false;++c.phase;break;
 case 46:if(!word(c.palette+4,c.format))return false;++c.phase;break;
 case 47:if(!half(c.palette,c.paletteCount))return false;++c.phase;break;
 case 48:if(!call(InitPalette,{palette(),guest(c.data),scalar(c.format),scalar(c.paletteCount)}))return false;++c.phase;break;
 case 49:if(!word(c.pair,c.image))return false;c.phase=51;break;
 case 51:if(!word(c.image+8,c.data))return false;++c.phase;break;
 case 52:if(!half(c.image+2,c.width))return false;++c.phase;break;
 case 53:if(!half(c.image,c.height))return false;++c.phase;break;
 case 54:if(!word(c.image+4,c.format))return false;++c.phase;break;
 case 55:
  if(c.ci){if(!call(InitCi,{object(),guest(c.data),scalar(c.width),scalar(c.height),scalar(c.format),scalar(c.wrapS),scalar(c.wrapT),scalar(c.min!=c.max),scalar(0)}))return false;}
  else if(!call(Init,{object(),guest(c.data),scalar(c.width),scalar(c.height),scalar(c.format),scalar(c.wrapS),scalar(c.wrapT),scalar(c.min!=c.max)}))return false;
  ++c.phase;break;
 // Headers and LOD conversion bias are live AFTER object initialization.
 case 56:if(!word(c.pair,c.image))return false;++c.phase;break;
 case 57:if(!byte(c.image+0x21,c.min))return false;++c.phase;break;
 case 58:if(!byte(c.image+0x22,c.max))return false;++c.phase;break;
 case 59:{auto pair=s.doubleWord?s.doubleWord(s.user,ByteBias):std::nullopt;if(!pair)return false;c.biasHi=(*pair)[0];c.biasLo=(*pair)[1];c.phase=61;break;}
 case 61:if(!number(AnimationTextureArithmetic::ByteToFloat,c.min,c.biasHi,c.biasLo,c.reals[0]))return false;++c.phase;break;
 case 62:if(!word(c.image+0x14,c.minFilter))return false;++c.phase;break;
 case 63:if(!word(c.image+0x18,c.magFilter))return false;++c.phase;break;
 case 64:if(!word(c.image+0x1c,c.reals[2]))return false;++c.phase;break;
 case 65:if(!number(AnimationTextureArithmetic::ByteToFloat,c.max,c.biasHi,c.biasLo,c.reals[1]))return false;++c.phase;break;
 case 66:if(!byte(c.image+0x20,c.edge))return false;++c.phase;break;
 case 67:if(!call(Lod,{object(),scalar(c.minFilter),scalar(c.magFilter),scalar(0),scalar(c.edge),scalar(0)}))return false;c.phase=c.ci?68:69;break;
 case 68:if(!call(LoadPalette,{palette(),scalar(0)}))return false;++c.phase;break;
 case 69:if(!call(Load,{object(),scalar(c.j)}))return false;c.phase=90;break;
 case 90:++c.j;c.phase=0;break;
 // Pass2 gates capture zero and one once, even when signed N<=0.
 case 100:if(!word(Zero,c.zero))return false;++c.phase;break;
 case 101:if(!word(One,c.one))return false;++c.phase;break;
 case 102:if(int32_t(c.j)>=int32_t(in.count)){c.active=false;return true;}++c.phase;break;
 case 103:if(!word(in.ids+4*(in.count-c.j-1),c.slot))return false;c.state=in.states+StateBytes*c.slot;c.mask=0;++c.phase;break;
 case 104:if(!word(c.state+0x14,c.transform[0]))return false;++c.phase;break;
 case 105:if(!word(c.state+0xc,c.transform[1]))return false;++c.phase;break;
 case 106:if(!word(c.state+0x10,c.transform[2]))return false;++c.phase;break;
 case 107:if(!word(c.state+4,c.transform[3]))return false;++c.phase;break;
 case 108:if(!word(c.state+8,c.transform[4]))return false;c.phase=real(c.transform[0])!=real(c.zero)?110:130;break;
 case 110:if(!word(Half,c.literal))return false;++c.phase;break;
 case 111:if(!sdk(Translate,L::Scratch,0,0,{c.literal,c.literal,c.zero}))return false;++c.phase;break;
 case 112:if(!word(Degrees,c.literal))return false;++c.phase;break;
 case 113:if(!number(AnimationTextureArithmetic::Multiply,c.literal,c.transform[0]^0x80000000u,0,c.literal))return false;++c.phase;break;
 case 114:if(!sdk(Rotate,L::Rotation,0,0,{},c.literal))return false;++c.phase;break;
 case 115:if(!sdk(Concat,L::Translation,L::Scratch,L::Rotation,{}))return false;++c.phase;break;
 case 116:if(!word(NegativeHalf,c.literal))return false;++c.phase;break;
 case 117:if(!word(Zero,c.freshZero))return false;++c.phase;break;
 case 118:if(!sdk(Translate,L::Scratch,0,0,{c.literal,c.literal,c.freshZero}))return false;++c.phase;break;
 case 119:if(!sdk(Concat,L::Rotation,L::Translation,L::Scratch,{}))return false;c.mask|=1;c.phase=130;break;
 case 130:c.phase=real(c.transform[3])!=real(c.zero) || real(c.transform[4])!=real(c.zero) || real(c.transform[2])!=real(c.zero)?131:140;break;
 case 131:if(!word(One,c.literal))return false;++c.phase;break;
 case 132:if(!word(Zero,c.freshZero))return false;++c.phase;break;
 case 133:if(!number(AnimationTextureArithmetic::Subtract,c.literal,c.transform[4],0,c.literal))return false;++c.phase;break;
 case 134:if(!number(AnimationTextureArithmetic::Subtract,c.literal,c.transform[2],0,c.literal))return false;++c.phase;break;
 case 135:if(!sdk(Translate,L::Translation,0,0,{c.transform[3],c.literal,c.freshZero}))return false;c.mask|=2;c.phase=140;break;
 case 140:c.phase=real(c.transform[1])!=real(c.one) || real(c.transform[2])!=real(c.one)?141:150;break;
 case 141:if(!word(One,c.literal))return false;++c.phase;break;
 case 142:if(!sdk(Scale,L::Scale,0,0,{c.transform[1],c.transform[2],c.literal}))return false;c.mask|=4;c.phase=150;break;
 // Dispatch through live original branch targets, not a mask-only shortcut.
 case 150:if(!word(Dispatch+4*c.mask,c.target))return false;c.phase=153;break;
 case 153:switch(c.target){case 0x8003e178:c.phase=160;break;case 0x8003e198:c.selected=L::Rotation;c.phase=170;break;case 0x8003e1a0:c.selected=L::Translation;c.phase=170;break;case 0x8003e1a8:case 0x8003e1c8:case 0x8003e1e0:case 0x8003e1f8:c.phase=151;break;case 0x8003e1c0:c.selected=L::Scale;c.phase=170;break;default:return false;}break;
 case 151:{const unsigned left=c.target==0x8003e1e0?L::Translation:L::Rotation;const unsigned right=c.target==0x8003e1c8 || c.target==0x8003e1e0?L::Scale:L::Translation;if(!sdk(Concat,L::Scratch,left,right,{}))return false;c.selected=L::Scratch;c.phase=c.target==0x8003e1f8?152:170;break;}
 case 152:if(!sdk(Concat,L::Translation,L::Scratch,L::Scale,{}))return false;c.selected=L::Translation;c.phase=170;break;
 case 160:if(!call(Generator,{scalar(c.j),scalar(1),scalar(c.j+4),scalar(60),scalar(0),scalar(125)}))return false;c.phase=180;break;
 case 170:if(!call(Generator,{scalar(c.j),scalar(1),scalar(c.j+4),scalar(30+3*c.j),scalar(0),scalar(125)}))return false;++c.phase;break;
 case 171:if(!call(LoadMatrix,{matrix(c.selected),scalar(30+3*c.j),scalar(1)}))return false;c.phase=180;break;
 case 180:++c.j;c.phase=102;break;
 default:return false;
 }
 return false;
}
}
