#include "trouble_quest_window.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <vector>
using namespace ttyd;
struct Harness {
 TroubleQuestMenuWork* work=nullptr;
 std::vector<TroubleWindowRequest> requests;
 std::vector<int32_t> flags,clients,contents;
 TroubleWindowClip initial{1,2,3,4};
 bool mutate=false;
};
TroubleWindowServices services(Harness& h) {
 return {&h,[](void*){return true;},[](void* p,const TroubleWindowRequest& r){auto& h=*static_cast<Harness*>(p);h.requests.push_back(r);
  if(h.mutate && r.op==TroubleWindowOp::Color && r.color==0xff0000ff)h.work->quests[0]=19;
  if(h.mutate && r.op==TroubleWindowOp::String && r.x==-119){h.work->quests[0]=30;h.work->scroll=60;}
  return true;},
 [](void*,unsigned){return "header";},
 [](void* p,int32_t id){static_cast<Harness*>(p)->clients.push_back(id);return "client";},
 [](void* p,int32_t id){static_cast<Harness*>(p)->contents.push_back(id);return "contents";},
 [](void* p,const char* text){auto& h=*static_cast<Harness*>(p);
  if(!std::strcmp(text,"client")){if(h.mutate)h.work->scroll=99;return uint32_t(0x10000|360);}
  return uint32_t(!std::strcmp(text,"header")?200:520);},
 [](void* p,int32_t flag){auto& h=*static_cast<Harness*>(p);h.flags.push_back(flag);if(h.mutate)h.work->quests[0]=13;return std::optional<bool>(true);},
 [](void*){return std::optional<int32_t>(13);},
 [](void* p){return static_cast<Harness*>(p)->initial;}};
}
int main() {
 auto w=originalTroubleQuestList(401);w.top=20;w.scroll=600;w.counter94=123;
 Harness h;h.work=&w;assert(drawTroubleQuestWindow(w,services(h)) && w.counter94==123 && w.cursor==0);
 unsigned frames=0,titles=0,rows=0,clips=0;bool red=false;
 for(const auto& r:h.requests) {
  if(r.op==TroubleWindowOp::Frame)++frames;
  if(r.op==TroubleWindowOp::MatrixString){++titles;assert(r.scaleY==1);assert(std::fabs(r.scaleX-(titles<=2?.575f:.5f))<.0001f);}
  if(r.op==TroubleWindowOp::String)++rows;
  if(r.op==TroubleWindowOp::Scissor){++clips;if(clips==1)assert(r.clip.x==175 && r.clip.y==98 && r.clip.width==410 && r.clip.height==304);else assert(r.clip.x==1 && r.clip.y==2 && r.clip.width==3 && r.clip.height==4);}
  if(r.op==TroubleWindowOp::Color && r.color==0xff0000ff)red=true;
 }
 assert(frames==4 && titles==4 && rows==60 && clips==2 && red);
 assert(h.flags.size()==30 && h.flags[18]==5394 && h.flags[19]==5395 && h.flags[20]==5393);
 assert(h.clients[18]==20 && h.clients[19]==21 && h.clients[20]==19 && h.contents==h.clients);
 auto one=originalTroubleQuestList(8);one.count=1;one.quests[0]=2;one.scroll=10;
 Harness live;live.work=&one;live.mutate=true;assert(drawTroubleQuestWindow(one,services(live)));
 assert(live.flags[0]==5376 && live.clients[0]==19 && live.contents[0]==19); // Third ID captured after red publication; retained through client providers.
 std::vector<float> y;
 for(const auto& r:live.requests)if(r.op==TroubleWindowOp::String)y.push_back(r.y);
 assert(y.size()==2 && y[0]==150 && y[1]==200); // Each scroll sampled before its width call.
 MessageFont font;assert(font.load("romfs/font.pmfont"));TroubleQuestWindowCache cache;
 assert(cache.prepare("romfs/messages/gor_02.txt",font));
 MessageResource original;assert(original.load("romfs/messages/gor_02.txt"));
 for(unsigned id=1;id<=30;++id){char key[32];std::snprintf(key,sizeof(key),"irai_client_%02u",id);assert(!std::strcmp(cache.client(id),original.find(key)));
  std::snprintf(key,sizeof(key),"irai_contents_%02u",id);assert(!std::strcmp(cache.contents(id),original.find(key)));
  WindowFontMeasurement m;assert(measurePlainWindowFont(font,cache.contents(id),2,m) && cache.width(cache.contents(id))==m.width);}
 const auto* retained=cache.header(0);assert(!cache.prepare("/missing/room",font) && cache.header(0)==retained);
 std::vector<WindowFontQuad> glyphs;unsigned rng=0;
 struct FontCapture {std::vector<WindowFontQuad>* glyphs;unsigned* rng;} capture{&glyphs,&rng};
 WindowFontDrawServices draw{&capture,[](void* p,const WindowFontQuad& q){static_cast<FontCapture*>(p)->glyphs->push_back(q);},
  [](void* p,uint32_t bound){assert(bound==64);++*static_cast<FontCapture*>(p)->rng;return 0u;}};
 const char* row=cache.contents(2);WindowFontStyle style;style.color=0xff0000ff;
 assert(drawPlainWindowFontPitch(font,row,style,draw,-119,140.125f,.85f,.85f));
 float pen=-119;unsigned mapped=0;
 for(const auto* p=reinterpret_cast<const uint8_t*>(row);*p;++p){const auto code=font.mapping[*p];if(code==0xffff)continue;
  assert(glyphs[mapped].pitchX==pen && glyphs[mapped].pitchY==140.125f && glyphs[mapped].left==0 && glyphs[mapped].rgba==0xff0000ff);
  pen=std::fma(.85f,float(font.glyphs[code].width+2),pen);++mapped;}
 assert(glyphs.size()==mapped && rng==2*mapped);
 const auto before=glyphs.size();style.edge=true;assert(!drawPlainWindowFontPitch(font,row,style,draw,0,0,1,1) && glyphs.size()==before);

}
