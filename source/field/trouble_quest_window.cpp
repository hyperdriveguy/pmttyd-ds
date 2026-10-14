#include "trouble_quest_window.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
bool drawTroubleQuestWindow(TroubleQuestMenuWork& w,const TroubleWindowServices& s) {
 if(!s.ready || !s.emit || !s.header || !s.client || !s.contents || !s.width || !s.flag || !s.activeQuest || !s.scissor || !s.ready(s.user))return false;
 if(w.count<0 || w.count>30)return false;
 auto emit=[&](TroubleWindowRequest r){return s.emit(s.user,r);};
 auto simple=[&](TroubleWindowOp op){TroubleWindowRequest r;r.op=op;if(op==TroubleWindowOp::FogOff)r.color=0xffffffff;return emit(r);};
 auto color=[&](uint32_t c){TroubleWindowRequest r;r.op=TroubleWindowOp::Color;r.color=c;return emit(r);};
 auto edge=[&](bool on){TroubleWindowRequest r;r.op=TroubleWindowOp::Edge;r.edge=on;return emit(r);};
 auto scale=[&](){TroubleWindowRequest r;r.op=TroubleWindowOp::Scale;r.scaleX=r.scaleY=r.scaleZ=.85f;return emit(r);};
 auto frame=[&](uint32_t c,float x,float y,float width,float height){TroubleWindowRequest r;r.op=TroubleWindowOp::Frame;r.color=c;r.x=x;r.y=y;r.width=width;r.height=height;return emit(r);};
 auto title=[&](unsigned index,float x,float y,float span,float cap,bool firstEdge){
  const char* text=s.header(s.user,index);if(!text)return false;
  if(firstEdge && !edge(true))return false;
  const float measured=float(uint16_t(s.width(s.user,text)));
  const float effective=measured>cap?cap:measured;
  TroubleWindowRequest r;r.op=TroubleWindowOp::MatrixString;r.text=text;
  r.x=std::fma(span-effective,.5f,x);r.y=y;r.scaleX=measured>cap?cap/measured:1.f;
  return emit(r); // FontDrawStringMtx ignores prior shared scale .85.
 };
 if(!simple(TroubleWindowOp::FogOff) || !frame(0x2ba3ffff,-280,170,135,90) || !simple(TroubleWindowOp::FontStart) || !color(0xffffffff) || !scale() ||
    !title(0,-280,152,135,115,true) || !title(1,-280,127,135,115,false) || !edge(false) ||
    !frame(0xffffffff,-129,170,410,340) || !frame(0x2ba3ffff,-103,190,120,40) || !frame(0x2ba3ffff,91,190,120,40) ||
    !simple(TroubleWindowOp::FontStart) || !color(0xffffffff) || !scale() || !title(2,-103,180,120,100,true) || !edge(false) ||
    !title(3,91,180,120,100,true) || !edge(false))return false;
 const auto saved=s.scissor(s.user);
 TroubleWindowRequest clip;clip.op=TroubleWindowOp::Scissor;clip.clip={175,98,410,304};
 if(!emit(clip) || !simple(TroubleWindowOp::FontStart) || !color(0x000000ff) || !scale())return false;
 bool success=true;
 for(int32_t i=0;i<w.count;++i) {
  if(i>=30){success=false;break;} // Native finite domain, not a source clamp.
  const auto completed=s.flag(s.user,5374+w.quests[i]);
  if(!completed || !color(*completed?0x808080ff:0x000000ff)){success=false;break;}
  const auto active=s.activeQuest(s.user);
  if(!active){success=false;break;}
  if(w.quests[i]==*active && !color(0xff0000ff)){success=false;break;}
  const int32_t id=w.quests[i];if(id<1 || id>30){success=false;break;}
  for(unsigned column=0;column<2;++column) {
   const char* text=column?s.contents(s.user,id):s.client(s.user,id);
   if(!text){success=false;break;}
   const float y=float(140-30*i)+w.scroll;
   const float measured=float(uint16_t(s.width(s.user,text))),cap=column?260.f:180.f;
   TroubleWindowRequest r;r.op=TroubleWindowOp::ScaleVec;
   r.scaleX=measured>cap?.85f*(cap/measured):.85f;r.scaleY=r.scaleZ=.85f;
   if(!emit(r)){success=false;break;}
   r.op=TroubleWindowOp::String;r.text=text;r.x=column?51.f:-119.f;r.y=y;
   if(!emit(r)){success=false;break;}
  }
  if(!success)break;
 }
 clip.clip=saved;return emit(clip) && success;
}
bool TroubleQuestWindowCache::prepare(const char* path,const MessageFont& font) {
 auto resource=std::make_shared<MessageResource>();if(!resource->load(path))return false;
 TroubleQuestWindowCache next;
 const char* keys[]={"irai_name1_win","irai_name2_win","irai_client_win","irai_contents_win"};
 for(unsigned i=0;i<4;++i){next.headers_[i]=resource->find(keys[i]);WindowFontMeasurement m;
  if(!measurePlainWindowFont(font,next.headers_[i],2,m))return false;
  next.widths_[i]=m.width;}
 for(unsigned i=0;i<30;++i)for(unsigned column=0;column<2;++column) {
  char key[32];std::snprintf(key,sizeof(key),column?"irai_contents_%02u":"irai_client_%02u",i+1);
  const char* text=resource->find(key);if(!text)return false;
  const size_t length=std::strlen(text);char* target=column?next.contents_[i].data():next.clients_[i].data();
  if(length>=(column?31u:21u))return false;
  std::memcpy(target,text,length+1);WindowFontMeasurement m;if(!measurePlainWindowFont(font,text,2,m))return false;
  next.widths_[4+2*i+column]=m.width;
 }
 next.messages_=std::move(resource);*this=std::move(next);return true;
}
uint16_t TroubleQuestWindowCache::width(const char* text) const {
 for(unsigned i=0;i<4;++i)if(text==headers_[i])return widths_[i];
 for(unsigned i=0;i<30;++i){if(text==clients_[i].data())return widths_[4+2*i];if(text==contents_[i].data())return widths_[5+2*i];}
 return 0; // Native caller admits only retained cache pointers.
}
}
