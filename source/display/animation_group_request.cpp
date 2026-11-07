#include "animation_group_request.hpp"
namespace ttyd {
namespace {
bool same(const AnimationGroupRequestServices& a,const AnimationGroupRequestServices& b){return a.user==b.user && a.word==b.word && a.archive==b.archive && a.compare==b.compare && a.request==b.request;}
// Compare wrapped guest offset/count as signed32 without host signed overflow.
int64_t signedWord(uint32_t v){return v<0x80000000?int64_t(v):int64_t(v)-0x100000000LL;}
}
// Retain each source request/lookup boundary. False means native hold, while
// completed source result 0 is a legitimate poll result and resets the cursor.
// Visits are scheduling policy; there is no elapsed-time/readiness estimate.
bool requestAnimationGroup(uint32_t name,uint32_t mode,uint32_t completion,const AnimationGroupRequestServices& s,AnimationGroupRequestCursor& c,uint32_t& result,unsigned visits){
 if(!visits)return false;
 if(c.phase && (c.name!=name || c.mode!=mode || c.completion!=completion || !same(c.owner,s)))return false;
 if(!c.phase){c.owner=s;c.name=name;c.mode=mode;c.completion=completion;c.phase=1;}
 auto read=[&](uint32_t a,uint32_t& v){auto x=s.word?s.word(s.user,a):std::nullopt;if(!x)return false;v=*x;return true;};
 for(unsigned n=0;n<visits;++n)switch(c.phase){
 // Archive modes 0/1/2 select 1/2/0; other modes skip the setter. Type 5 MODEL
 // uses format8041fb34 "%s/%s", prefix8041fb3c "a" and caller completion.
 case 1:if(mode<3 && (!s.archive || !s.archive(s.user,mode==0?1:mode==1?2:0)))return false;c.phase=2;break;
 case 2:{if(!s.request)return false;auto x=s.request(s.user,5,completion,0x8041fb34,0x8041fb3c,name);if(!x)return false;c.modelResult=*x;c.phase=3;break;}
 // Read LIVE W100 FileEntry+a0 holder only after MODEL request. Holder[0]
 // is ag2tg bytes,holder[4] signed byte length. Walk 32-byte name rows; first
 // strcmp equality selects FOLLOWING row. No native row-validity clamp here.
 case 3:if(!read(0x80415dc0,c.work))return false;c.phase=4;break;
 case 4:if(!read(c.work+0x100,c.handle))return false;c.phase=5;break;
 case 5:if(!read(c.handle+0xa0,c.holder))return false;c.phase=6;break;
 case 6:if(!read(c.holder+4,c.count))return false;c.phase=7;break;
 case 7:if(!read(c.holder,c.pointer))return false;c.offset=0;c.phase=8;break;
 case 8:if(signedWord(c.offset)<signedWord(c.count))c.phase=9;else{c.texture=0x8041fb30;c.phase=10;}break;
 case 9:{if(!s.compare)return false;auto x=s.compare(s.user,c.pointer,name);if(!x)return false;if(!*x){c.texture=c.pointer+32;c.phase=10;}else{c.pointer+=32;c.offset+=32;c.phase=8;}break;}
 // Miss uses empty string8041fb30; type 4 TPL format802c1068 "%s/%s-"
 // therefore requests a/-. Texture request is unconditional, completion 0.
 case 10:{if(!s.request)return false;auto x=s.request(s.user,4,0,0x802c1068,0x8041fb3c,c.texture);if(!x)return false;c.textureResult=*x;c.phase=11;break;}
 // Readiness is chosen AFTER both requests: MODEL -1 => 1, 0 => 0,
 // otherwise TEXTURE != 0 (including -1). Missing provider != source result 0.
 case 11:result=c.modelResult==0xffffffff?1:c.modelResult==0?0:c.textureResult!=0;c={};return true;
 default:return false;
 }
 return false;
}
}
