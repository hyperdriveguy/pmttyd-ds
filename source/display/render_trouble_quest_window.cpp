#include "platform.hpp"
#include "trouble_quest_presentation.hpp"
namespace ttyd {
// Stack bridge borrows cache/state only during this synchronous draw. Source
// request order mutates local font/scale/scissor state; GPU success is returned
// to the retained window continuation. Missing providers cannot be acknowledged.
bool Renderer::troubleQuestWindow(TroubleQuestWindowCache& cache,TroubleQuestMenuWork& work,const TroubleWindowServices& state) {
 if(!cache.ready() || !messagePanel.imported || !fontTexture.imported || !windowFrameVertices || !windowFontVertices ||
    !state.ready || !state.flag || !state.activeQuest || !state.ready(state.user))return false;
 struct Context {Renderer* r;TroubleQuestWindowCache* cache;const TroubleWindowServices* state;
  TroubleWindowClip clip;WindowFontStyle font;float scaleX=1,scaleY=1;} c{this,&cache,&state};
 TroubleWindowServices s;s.user=&c;
 s.ready=[](void*){return true;};
 s.header=[](void* p,unsigned i){return static_cast<Context*>(p)->cache->header(i);};
 s.client=[](void* p,int32_t id){return static_cast<Context*>(p)->cache->client(id);};
 s.contents=[](void* p,int32_t id){return static_cast<Context*>(p)->cache->contents(id);};
 s.width=[](void* p,const char* text){return uint32_t(static_cast<Context*>(p)->cache->width(text));};
 s.flag=[](void* p,int32_t flag){auto& c=*static_cast<Context*>(p);return c.state->flag(c.state->user,flag);};
 s.activeQuest=[](void* p){auto& c=*static_cast<Context*>(p);return c.state->activeQuest(c.state->user);};
 s.scissor=[](void* p){return static_cast<Context*>(p)->clip;};
 s.emit=[](void* p,const TroubleWindowRequest& q){auto& c=*static_cast<Context*>(p);auto& r=*c.r;
  switch(q.op) {
  case TroubleWindowOp::FogOff:C3D_FogGasMode(GPU_NO_FOG,GPU_PLAIN_DENSITY,false);C3D_FogColor(0xffffff);return true;
  // Type0 frame border 12 is in original UI units. Color is source RGBA;
  // font consumers distinguish per-string matrix scale from retained pitch scale.
  case TroubleWindowOp::Frame:return r.sourceWindowFrame(type0WindowFrame(q.x,q.y,q.width,q.height,12,
      {uint8_t(q.color>>24),uint8_t(q.color>>16),uint8_t(q.color>>8),uint8_t(q.color)}));
  case TroubleWindowOp::FontStart:c.font={};c.font.edge=false;c.scaleX=c.scaleY=1;return true;
  case TroubleWindowOp::Color:c.font.color=q.color;return true;
  case TroubleWindowOp::Scale:case TroubleWindowOp::ScaleVec:c.scaleX=q.scaleX;c.scaleY=q.scaleY;return true;
  case TroubleWindowOp::Edge:c.font.edge=q.edge;return true;
  case TroubleWindowOp::Scissor:{c.clip=q.clip;ItemSelectionClip clip{int32_t(q.clip.x),int32_t(q.clip.y),int32_t(q.clip.width),int32_t(q.clip.height)};r.selectorClip(&clip);return true;}
  case TroubleWindowOp::MatrixString:case TroubleWindowOp::String:{
   ItemSelectionTextDraw text; text.x=q.x;text.y=q.y;
   const bool matrix=q.op==TroubleWindowOp::MatrixString;
   text.scaleX=matrix?q.scaleX:c.scaleX;text.scaleY=matrix?q.scaleY:c.scaleY;
   ItemSelectionClip clip{int32_t(c.clip.x),int32_t(c.clip.y),int32_t(c.clip.width),int32_t(c.clip.height)};
   return r.sourceWindowFont(q.text,matrix?WindowFontConsumer::StringMatrix:WindowFontConsumer::StringPitch,text,c.font,&clip);
  }
  }
  return false;
 };
 const bool success=drawTroubleQuestWindow(work,s);
 selectorClip(nullptr); // Native pass owns a full-screen entry/exit scissor.
 return success;
}
bool Renderer::troubleQuestMenu(TroubleQuestPresentation& p) {
 auto* work=p.displayWork();if(!work)return false;
 const auto& source=p.stateServices();TroubleWindowServices state;
 state.user=source.user;state.ready=source.ready;state.flag=source.flag;state.activeQuest=source.activeQuest;
 bool success=troubleQuestWindow(p.cache(),*work,state);
 for(const auto& icon:p.icons())if(icon.visible())
  success=selectorIconFrame(icon.resource->frames.at(icon.frame),icon.x,icon.y,icon.scale,icon.flags) && success;
 return success;
}

}
