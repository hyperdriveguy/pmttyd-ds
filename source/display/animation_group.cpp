#include "animation_group.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
bool same(const AnimationInnerTraversal& a,const AnimationInnerTraversal& b){return a.parent==b.parent && a.group==b.group && a.current==b.current && a.next==b.next && !std::memcmp(&a.fraction,&b.fraction,4);}
bool same(const AnimationGroupServices& a,const AnimationGroupServices& b){return a.user==b.user && a.word==b.word && a.byte==b.byte && a.store==b.store && a.node==b.node && a.copy==b.copy && a.material==b.material && a.vivian==b.vivian && a.callback==b.callback && a.render==b.render && a.blend==b.blend;}
void push(AnimationGroupCursor& c,const AnimationInnerTraversal& q){if(c.depth==c.frames.size())c.frames.emplace_back(std::make_unique<AnimationGroupFrame>());auto& f=*c.frames[c.depth++];f={};f.call=q;}
constexpr uint32_t Basis[]{0,4,8,0x10,0x14,0x18,0x20,0x24,0x28};
}
bool resumeAnimationGroup(const AnimationInnerTraversal& input,const AnimationGroupServices& s,AnimationGroupCursor& c,unsigned visits){
 if(!visits)return false;
 if(!c.active){c.active=true;c.input=input;c.owner=s;c.depth=0;push(c,input);}
 if(!same(input,c.input) || !same(s,c.owner))return false;
 auto read=[&](uint32_t a,uint32_t& v){auto w=s.word?s.word(s.user,a):std::nullopt;if(!w)return false;v=*w;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 for(unsigned visit=0;visit<visits;++visit){auto& f=*c.frames[c.depth-1];const auto g=f.call.group;
  switch(f.phase){
  case 0:if(g==0xffffffff){f.phase=105;break;}if(!read(0x80415dc0,f.work))return false;++f.phase;break;
  case 1:if(!read(f.work+0x20,f.model))return false;++f.phase;break;
  case 2:if(!read(f.work+0x18,f.pose))return false;++f.phase;break;
  case 3:if(!read(f.model+0x1a8,f.groups))return false;f.group=f.groups+g*88;++f.phase;break;
  case 4:if(!read(f.pose+0x5c,f.visibility))return false;++f.phase;break;
  case 5:if(!read(f.group+0x4c,f.index))return false;++f.phase;break;
  case 6:{auto b=s.byte?s.byte(s.user,f.visibility+f.index):std::nullopt;if(!b)return false;f.phase=*b?7:103;break;}
  case 7:if(!read(f.group+0x54,f.kind))return false;f.node.joint=f.kind!=0;++f.phase;break;
  case 8:if(!read(f.group+0x50,f.index))return false;++f.phase;break;
  case 9:if(!read(f.pose+0x64,f.nodes))return false;f.node.node=f.nodes+f.index*4;f.phase=!f.node.joint?20:f.call.parent==0xffffffff?11:10;break;
  case 10: f.parentGroup=f.groups+f.call.parent*88;if(!read(f.parentGroup+0x54,f.value))return false;f.phase=f.value?12:11;break;
  case 11:if(!read(0x8041fb24,f.value))return false;f.node.parent.fill(real(f.value));f.phase=20;break;
  case 12:if(!read(f.parentGroup+0x50,f.index))return false;f.pointer=f.nodes+f.index*4;++f.phase;break;
  case 13:if(!read(f.pointer+0xc,f.value))return false;f.node.parent[0]=real(f.value);++f.phase;break;
  case 14:if(!read(f.pointer+0x10,f.value))return false;f.node.parent[1]=real(f.value);++f.phase;break;
  case 15:if(!read(f.pointer+0x14,f.value))return false;f.node.parent[2]=real(f.value);f.phase=20;break;
  case 20:if(!s.node || !s.node(s.user,f.node))return false;f.phase=30;break;
  // Independent current-pose queries and literal seed loads, even though
  // translation overwrites all three local seed words before publication.
  case 30:if(!read(0x80415dc0,f.work))return false;++f.phase;break;
  case 31:if(!read(f.work+0x18,f.pose))return false;++f.phase;break;
  case 32:if(!read(f.pose,f.value))return false;f.phase=f.value&0x40?33:45;break;
  case 33:if(!read(0x8041e8a0,f.matrix))return false;++f.phase;break;
  case 34:if(!read(f.pose+0x15c,f.pointer))return false;++f.phase;break;
  case 35:if(!read(0x802c1048,f.seed))return false;++f.phase;break;
  case 36:if(!read(0x802c104c,f.seed))return false;++f.phase;break;
  case 37:if(!read(f.matrix+0xc,f.x))return false;++f.phase;break;
  case 38:if(!read(0x802c1050,f.seed))return false;++f.phase;break;
  case 39:if(!read(f.pointer,f.destination))return false;f.destination+=g*12;++f.phase;break;
  case 40:if(!read(f.matrix+0x1c,f.y))return false;++f.phase;break;
  case 41:if(!read(f.matrix+0x2c,f.z))return false;++f.phase;break;
  case 42:if(!store(f.destination,f.x))return false;++f.phase;break;
  case 43:if(!store(f.destination+4,f.y))return false;++f.phase;break;
  case 44:if(!store(f.destination+8,f.z))return false;++f.phase;break;
  case 45:if(!read(0x80415dc0,f.work))return false;++f.phase;break;
  case 46:if(!read(f.work+0x18,f.pose))return false;++f.phase;break;
  case 47:if(!read(f.pose,f.value))return false;f.phase=f.value&0x80?48:52;break;
  case 48:if(!read(f.pose+0x164,f.pointer))return false;++f.phase;break;
  case 49:if(!read(0x8041e8a0,f.matrix))return false;++f.phase;break;
  case 50:if(!read(f.pointer,f.destination))return false;f.destination+=g*48;++f.phase;break;
  case 51:if(!s.copy || !s.copy(s.user,f.matrix,f.destination))return false;++f.phase;break;
  case 52:if(!read(0x80415dc0,f.work))return false;++f.phase;break;
  case 53:if(!read(f.work+0x18,f.pose))return false;++f.phase;break;
  case 54:if(!read(f.pose,f.value))return false;f.phase=f.value&0x20?55:90;break;
  case 55:if(!read(f.pose+0x120,f.pointer))return false;++f.phase;break;
  case 56:if(!read(f.pointer,f.value))return false;f.phase=f.value==g?60:90;break;
  // Special B is the inline matrix at P+124, NOT word[P+124].
  case 60:if(!read(f.pose+0x11c,f.kind))return false;f.vivian.saved=f.pose+0x124;++f.phase;break;
  case 61:if(!read(0x8041fb28,f.value))return false;f.vivian.parameters[2]=f.vivian.parameters[3]=real(f.value);++f.phase;break;
  case 62:if(!read(f.pose+0x14,f.animation))return false;++f.phase;break;
  case 63:if(!read(0x80415db0+f.kind*4,f.pair))return false;f.pair+=f.animation*8;++f.phase;break;
  case 64:if(!read(0x8041e8a0,f.vivian.matrix))return false;f.phase=f.kind>=1 && f.kind<=3?65:67;break;
  case 65:if(!read(f.kind==1?0x8041fb98:f.kind==2?0x8041fb9c:0x8041fb6c,f.value))return false;f.vivian.parameters[2]=real(f.value);++f.phase;break;
  case 66:if(!read(0x8041fb44,f.value))return false;f.vivian.parameters[3]=real(f.value);++f.phase;break;
  case 67:if(!read(f.group+0x48,f.shape))return false;++f.phase;break;
  case 68:if(!s.material || !s.material(s.user,f.shape,f.call.current))return false;++f.phase;break;
  case 69:if(!read(f.pair,f.value))return false;f.vivian.parameters[0]=real(f.value);++f.phase;break;
  case 70:if(!read(f.pair+4,f.value))return false;f.vivian.parameters[1]=real(f.value);++f.phase;break;
  case 71:if(!s.vivian || !s.vivian(s.user,f.vivian))return false;++f.phase;break;
  case 72:if(!read(0x80415dc0,f.work))return false;++f.phase;break;
  case 73:if(!read(0x8041fba0,f.coefficient))return false;++f.phase;break;
  case 74:if(!read(f.work+0x18,f.pose))return false;++f.phase;break;
  case 75:if(!read(f.pose,f.value))return false;++f.phase;break;
  case 76:if(!store(f.pose,f.value&~0x20u))return false;f.lane=0;++f.phase;break;
  case 77:if(!read(f.vivian.matrix+Basis[f.lane],f.source))return false;++f.phase;break;
  case 78:if(!read(f.vivian.saved+Basis[f.lane],f.old))return false;++f.phase;break;
  case 79:{auto v=s.blend?s.blend(s.user,f.coefficient,f.source,f.old):std::nullopt;if(!v)return false;f.result=*v;++f.phase;break;}
  case 80:if(!store(f.vivian.saved+Basis[f.lane],f.result))return false;f.phase=++f.lane==9?100:77;break;
  case 90:if(!read(f.pose+0x154,f.callback))return false;f.phase=f.callback?91:94;break;
  case 91:if(!read(f.pose+0x158,f.data))return false;++f.phase;break;
  case 92:if(!read(0x8041e8a0,f.matrix))return false;++f.phase;break;
  case 93:if(!s.callback || !s.callback(s.user,f.callback,f.data,g,f.matrix))return false;++f.phase;break;
  case 94:if(!read(f.group+0x48,f.shape))return false;++f.phase;break;
  case 95:if(!s.render || !s.render(s.user,f.shape,f.call.current))return false;++f.phase;break;
  case 96:if(!read(f.group+0x44,f.value))return false;++f.phase;break;
  case 97:{auto q=f.call;q.parent=g;q.group=f.value;f.phase=100;push(c,q);break;}
  case 100:if(!read(0x8041e89c,f.level))return false;--f.level;++f.phase;break;
  case 101:if(!store(0x8041e89c,f.level))return false;++f.phase;break;
  case 102:if(!store(0x8041e8a0,0x803d8e70+f.level*48))return false;++f.phase;break;
  case 103:if(!read(f.group+0x40,f.value))return false;++f.phase;break;
  case 104:{auto q=f.call;q.parent=g;q.group=f.value;f.phase=105;push(c,q);break;}
  case 105:if(!--c.depth){c.active=false;return true;}break;
  default:return false;
  }
 }
 return false;
}
}
