#include "animation_source_node.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
bool same(const AnimationGroupNode& a,const AnimationGroupNode& b){return a.node==b.node && a.joint==b.joint && !std::memcmp(a.parent.data(),b.parent.data(),12);}
bool same(const AnimationSourceNodeServices& a,const AnimationSourceNodeServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.number==b.number && a.matrix==b.matrix;}
constexpr uint32_t JointTargets[]{0x8003e5b8,0x8003e5c8,0x8003e5d0,0x8003e5d8,0x8003e5f0,0x8003e5f8,0x8003e610,0x8003e628,0x8003e650,0x8003e658,0x8003e670,0x8003e688,0x8003e6b0,0x8003e6c8,0x8003e6f0,0x8003e718};
constexpr uint32_t TransformTargets[]{0x8003ebc8,0x8003ebd8,0x8003ebe0,0x8003ebe8,0x8003ec00,0x8003ec08,0x8003ec20,0x8003ec38};
// Durable scratch: A0 R1 Q/offR2 S3 T4 negSP5 posSP6 offS7 negRP8 posRP9.
AnimationSourceMatrixCall op(uint32_t fn,uint32_t out,uint32_t a=0,uint32_t b=0){AnimationSourceMatrixCall q;q.function=fn;q.output.value=out;q.left.value=a;q.right.value=b;return q;}
void plan(AnimationSourceNodeCursor& c){
 c.count=c.step=0;auto id=[&]{c.dispatch[c.count++]=op(0x8029dd04,0);c.selected=0;};
 auto cat=[&](uint32_t a,uint32_t b,uint32_t out){c.dispatch[c.count++]=op(0x8029dd64,out,a,b);c.selected=out;};
 if(c.input.joint)switch(c.mask){
 case 0:id();break;case 1:c.selected=4;break;case 2:c.selected=2;break;case 3:cat(4,2,0);break;case 4:c.selected=3;break;case 5:cat(4,3,0);break;case 6:cat(2,3,0);break;
 case 7:cat(4,2,0);cat(0,3,4);break;case 8:c.selected=1;break;case 9:cat(4,1,0);break;case 10:cat(2,1,0);break;case 11:cat(4,2,0);cat(0,1,4);break;
 case 12:cat(1,3,0);break;case 13:cat(4,1,0);cat(0,3,4);break;case 14:cat(2,1,0);cat(0,3,4);break;case 15:cat(4,2,0);cat(0,1,4);cat(4,3,0);break;
 }else switch(c.mask){case 0:id();break;case 1:c.selected=4;break;case 2:c.selected=3;break;case 3:cat(4,3,0);break;case 4:c.selected=1;break;case 5:cat(4,1,0);break;case 6:cat(1,3,0);break;case 7:cat(4,1,0);cat(0,3,4);break;}
}
}
bool resumeAnimationSourceNode(const AnimationGroupNode& input,const AnimationSourceNodeServices& s,AnimationSourceNodeCursor& c,unsigned visits){
 if(!visits)return false;
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}
 if(!same(input,c.input) || !same(s,c.owner))return false;
 auto read=[&](uint32_t a,uint32_t& v){auto w=s.word?s.word(s.user,a):std::nullopt;if(!w)return false;v=*w;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 auto num=[&](AnimationNodeNumber kind,uint32_t a,uint32_t b,uint32_t& v){auto w=s.number?s.number(s.user,kind,a,b):std::nullopt;if(!w)return false;v=*w;return true;};
 auto triple=[&](unsigned i,bool neg=false){return std::array<float,3>{real(c.node[i]^(neg?0x80000000u:0)),real(c.node[i+1]^(neg?0x80000000u:0)),real(c.node[i+2]^(neg?0x80000000u:0))};};
 auto any=[&](unsigned i,uint32_t literal){return real(c.node[i])!=real(literal) || real(c.node[i+1])!=real(literal) || real(c.node[i+2])!=real(literal);};
 auto call=[&](AnimationSourceMatrixCall q){if(!c.pending){c.request=q;c.pending=true;}if(!s.matrix || !s.matrix(s.user,c.request,c.locals))return false;c.pending=false;return true;};
 auto xyz=[&](uint32_t fn,unsigned out,unsigned i,bool neg=false){auto q=op(fn,out);q.xyz=triple(i,neg);return call(q);};
 auto advanceRotation=[&]{if(++c.rotation==(c.input.joint?6u:3u))c.phase=c.input.joint?100:90;else c.phase=70;};
 for(unsigned visit=0;visit<visits;++visit)switch(c.phase){
 case 0:if(!read(c.input.node,c.node[0]))return false;++c.phase;break;
 case 1:if(!read(0x8041fb28,c.zero))return false;++c.phase;break;
 case 2:if(!read(0x8041fb64,c.two))return false;++c.phase;break;
 case 3:if(!read(c.input.node+0x18,c.node[6]))return false;++c.phase;break;
 case 4:if(!read(c.input.node+0x1c,c.node[7]))return false;++c.phase;break;
 case 5:if(!read(c.input.node+0x20,c.node[8]))return false;++c.phase;break;
 case 6:if(!num(AnimationNodeNumber::Multiply,c.two,c.node[6],c.node[6]))return false;++c.phase;break;
 case 7:if(!num(AnimationNodeNumber::Multiply,c.two,c.node[7],c.node[7]))return false;++c.phase;break;
 case 8:if(!read(c.input.node+4,c.node[1]))return false;++c.phase;break;
 case 9:if(!num(AnimationNodeNumber::Multiply,c.two,c.node[8],c.node[8]))return false;++c.phase;break;
 case 10:if(!read(c.input.node+8,c.node[2]))return false;++c.phase;break;
 case 11:case 12:case 13:if(!read(c.input.node+0xc+4*(c.phase-11),c.node[3+c.phase-11]))return false;if(++c.phase==14)c.capture=c.input.joint?9:12;break;
 case 14:if(!read(c.input.node+c.capture*4,c.node[c.capture]))return false;if(++c.capture==(c.input.joint?12u:24u))c.phase=20;break;
 case 20:c.phase=any(0,c.zero)?21:30;break;
 case 21:if(!xyz(0x8029e230,4,0))return false;c.mask|=1;c.phase=30;break;
 case 30:if(!read(0x8041fb24,c.literal))return false;c.phase=c.input.joint?31:50;break;
 case 31:c.phase=c.input.parent[0]!=real(c.literal) || c.input.parent[1]!=real(c.literal) || c.input.parent[2]==real(c.literal)?32:40;break;
 case 32:if(!read(0x8041fb28,c.literal))return false;c.phase=c.input.parent[0]==real(c.literal) || c.input.parent[1]==real(c.literal) || c.input.parent[2]==real(c.literal)?33:34;break;
 case 33:if(!read(0x8041fb24,c.literal))return false;c.compensation.fill(c.literal);c.phase=38;break;
 case 34:if(!read(0x8041fb24,c.literal))return false;++c.phase;break;
 case 35:case 36:case 37:if(!num(AnimationNodeNumber::Divide,c.literal,bits(c.input.parent[c.phase-35]),c.compensation[c.phase-35]))return false;++c.phase;break;
 case 38:{auto q=op(0x8029e2b0,2);q.xyz={real(c.compensation[0]),real(c.compensation[1]),real(c.compensation[2])};if(!call(q))return false;c.mask|=2;c.phase=40;break;}
 case 40:if(!read(0x8041fb24,c.literal))return false;++c.phase;break;
 case 41:c.phase=real(c.node[3])!=real(c.literal) || real(c.node[4])!=real(c.literal) || real(c.node[5])==real(c.literal)?42:63;break;
 case 42:if(!xyz(0x8029e2b0,3,3))return false;c.mask|=4;c.phase=63;break;
 case 50:c.phase=any(3,c.literal)?51:52;break;
 case 51:if(!xyz(0x8029e2b0,3,3))return false;c.mask|=2;++c.phase;break;
 case 52:if(!read(0x8041fb28,c.literal))return false;c.phase=any(15,c.literal)?(c.mask&2?55:53):63;break;
 case 53:if(!read(0x8041fb28,c.literal))return false;c.phase=any(21,c.literal)?54:63;break;
 case 54:if(!xyz(0x8029e230,3,21))return false;c.mask|=2;c.phase=63;break;
 case 55:if(!xyz(0x8029e230,5,15,true))return false;++c.phase;break;
 case 56:if(!xyz(0x8029e230,6,15))return false;++c.phase;break;
 case 57:if(!xyz(0x8029e230,7,21))return false;++c.phase;break;
 case 58:if(!call(op(0x8029dd64,0,7,6)))return false;++c.phase;break;
 case 59:if(!call(op(0x8029dd64,0,0,3)))return false;++c.phase;break;
 case 60:if(!call(op(0x8029dd64,3,0,5)))return false;c.phase=63;break;
 case 63:if(!call(op(0x8029dd04,1)))return false;c.rotation=0;c.phase=70;break;
 case 70:if(!read(0x8041fb28,c.literal))return false;c.angle=c.node[c.input.joint && c.rotation<3?11-c.rotation:8-(c.input.joint?c.rotation-3:c.rotation)];if(real(c.angle)==real(c.literal))advanceRotation();else c.phase=71;break;
 case 71:if(!read(0x8041fb2c,c.literal))return false;++c.phase;break;
 case 72:if(!num(AnimationNodeNumber::Multiply,c.literal,c.angle,c.radians))return false;++c.phase;break;
 case 73:{auto q=op(0x8029dff0,0);q.axis=122-c.rotation%3;q.radians=real(c.radians);if(!call(q))return false;++c.phase;break;}
 case 74:if(!call(op(0x8029dd64,1,1,0)))return false;c.mask|=c.input.joint?8:4;advanceRotation();break;
 case 90:if(!read(0x8041fb28,c.literal))return false;c.phase=any(12,c.literal)?(c.mask&4?93:91):100;break;
 case 91:if(!read(0x8041fb28,c.literal))return false;c.phase=any(18,c.literal)?92:100;break;
 case 92:if(!xyz(0x8029e230,1,18))return false;c.mask|=4;c.phase=100;break;
 case 93:if(!xyz(0x8029e230,8,12,true))return false;++c.phase;break;
 case 94:if(!xyz(0x8029e230,9,12))return false;++c.phase;break;
 case 95:if(!xyz(0x8029e230,2,18))return false;++c.phase;break;
 case 96:if(!call(op(0x8029dd64,0,2,9)))return false;++c.phase;break;
 case 97:if(!call(op(0x8029dd64,1,0,1)))return false;++c.phase;break;
 case 98:if(!call(op(0x8029dd64,1,1,8)))return false;c.phase=100;break;
 case 100:if(!read(0x8041e89c,c.level))return false;++c.level;++c.phase;break;
 case 101:if(!store(0x8041e89c,c.level))return false;++c.phase;break;
 case 102:if(!store(0x8041e8a0,0x803d8e70+48*c.level))return false;++c.phase;break;
 case 103:if(!read((c.input.joint?0x80306698:0x803066d8)+4*c.mask,c.target))return false;++c.phase;break;
 case 104:if(c.target!=(c.input.joint?JointTargets[c.mask]:TransformTargets[c.mask]))return false;plan(c);++c.phase;break;
 case 105:if(c.step<c.count){if(!call(c.dispatch[c.step]))return false;++c.step;}else c.phase=110;break;
 case 110:if(!read(0x8041e89c,c.level))return false;++c.phase;break;
 case 111:{auto q=op(0x8029dd64,0);q.left={true,0x803d8e70+48*(c.level-1)};q.right.value=c.selected;q.output={true,0x803d8e70+48*c.level};if(!call(q))return false;c.active=false;return true;}
 default:return false;
 }
 return false;
}
}
