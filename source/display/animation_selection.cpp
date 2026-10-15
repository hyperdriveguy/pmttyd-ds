#include "animation_selection.hpp"
namespace ttyd {
namespace {
bool less(uint32_t a,uint32_t b){return (a^0x80000000u)<(b^0x80000000u);}
bool same(const AnimationSelectionServices& a,const AnimationSelectionServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.compare==b.compare && a.divide==b.divide;}
}
bool selectAnimation(uint32_t handle,uint32_t selector,uint32_t force,const AnimationSelectionServices& s,AnimationSelectionCursor& c,unsigned visits){
 if(!visits)return false;
 if(!c.active){c={};c.active=true;c.handle=handle;c.selector=selector;c.force=force;c.owner=s;}
 else if(c.handle!=handle || c.selector!=selector || c.force!=force || !same(c.owner,s))return false;
 auto word=[&](uint32_t a,uint32_t& v){if(!s.word)return false;auto x=s.word(s.user,a);if(!x)return false;v=*x;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 for(unsigned visit=0;visit<visits;++visit)switch(c.phase){
 case 0:if(!word(0x80415dc0,c.work))return false;++c.phase;break;
 case 1:if(!word(c.work+0x10,c.pool))return false;c.pose=c.pool+handle*0x170u;++c.phase;break;
 case 2:if(!word(c.work,c.models))return false;++c.phase;break;
 case 3:if(!word(c.pose+0x10,c.index))return false;++c.phase;break;
 case 4:if(!word(c.models+16*c.index+8,c.entry))return false;++c.phase;break;
 case 5:if(!word(c.entry+0xa0,c.holder))return false;++c.phase;break;
 case 6:if(!word(c.holder,c.model))return false;++c.phase;break;
 case 7:if(!word(c.model+0x148,c.count))return false;++c.phase;break;
 case 8:c.selected=selector;if(less(selector,0)){c.selected=0;c.phase=9;}else{if(!less(selector,c.count))c.selected=0;c.phase=12;}break;
 case 9:if(less(c.selected,c.count))c.phase=10;else{c.selected=0;c.phase=12;}break;
 case 10:if(!word(c.model+0x1ac,c.directory))return false;++c.phase;break;
 case 11:{int32_t result=0;if(!s.compare || !s.compare(s.user,c.directory+64*c.selected,selector,result))return false;if(!result)c.phase=12;else{++c.selected;c.phase=9;}break;}
 case 12:c.phase=force?14:13;break;
 case 13:{uint32_t current;if(!word(c.pose+0x14,current))return false;if(current==c.selected){c={};return true;}++c.phase;break;}
 case 14:if(!store(c.pose+0x14,c.selected))return false;++c.phase;break;
 case 15:if(!word(c.pose+0x90,c.linked))return false;c.phase=c.linked==0xffffffffu?18:16;break;
 case 16:if(!word(c.pose,c.flags))return false;c.phase=(c.flags&2)?17:18;break;
 case 17:if(!store(c.pose,c.flags|4))return false;++c.phase;break;
 case 18:if(!word(c.pose+0xc,c.mode))return false;++c.phase;break;
 case 19:if(!word(0x800000f8,c.bus))return false;++c.phase;break;
 case 20:if(!word(0x80415fd0,c.gp))return false;++c.phase;break;
 case 21:if(!word(c.gp+(c.mode?0x38:0x40),c.hi))return false;++c.phase;break;
 case 22:if(!word(c.gp+(c.mode?0x3c:0x44),c.lo))return false;++c.phase;break;
 case 23:{const auto divisor=uint32_t((uint64_t(0x10624dd3)*(c.bus>>2))>>32)>>6;if(!s.divide || !s.divide(s.user,uint64_t(c.hi)<<32|c.lo,divisor,c.quotient))return false;++c.phase;break;}
 case 24:if(!store(c.pose+0x1c,uint32_t(c.quotient)))return false;++c.phase;break;
 case 25:if(!word(0x8041fb28,c.zero))return false;++c.phase;break;
 case 26:if(!store(c.pose+0x18,uint32_t(c.quotient>>32)))return false;++c.phase;break;
 case 27:if(!store(c.pose+0x3c,0xffffffffu))return false;++c.phase;break;
 case 28:if(!store(c.pose+0x40,c.zero))return false;++c.phase;break;
 case 29:if(!store(c.pose+0x80,0))return false;++c.phase;break;
 case 30:if(!store(c.pose+0x84,c.zero))return false;c={};return true;
 default:return false;
 }
 return false;
}
}
