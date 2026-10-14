#include "field_area_event.hpp"
#include "field_bero_info.hpp"
#include "field_bero_exec.hpp"
#include "field_map_flags.hpp"
#include "event_address_space.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace ttyd;
struct Trace {
 EventCreationServices creation;EventSchedule* schedule=nullptr;uint32_t initializer=0;bool checkMapOwner=true;
 bool audioReady=false;unsigned bgm=0,env=0;bool retained=false;uint32_t parent=0,child=0;
 std::string track,environment;std::array<int32_t,16> childBank{},parentBank{};
 static int32_t call(void* p,FieldAreaNativeCall& request) {
  auto& t=*static_cast<Trace*>(p);auto& e=request.event;
  auto value=[&](unsigned n){uint32_t raw=0;assert(request.memory.readWord(e.arguments+4*n,raw));return getEventValue(e,int32_t(raw),request.operands);};
  auto text=[&](uint32_t a){std::string out;for(unsigned n=0;n<64;++n){uint8_t b=0;assert(request.memory.readByte(a+n,b));if(!b)return out;out.push_back(char(b));}assert(false);return out;};
  if(request.function==0x801525a4){assert(value(0)==0x200);t.track=text(uint32_t(value(1)));++t.bgm;
   if(!t.audioReady){assert(request.first);return 0;}t.retained=!request.first;return 2;}
  if(request.function==0x80151e0c){assert(value(0)==0x200);++t.env;return 2;}
  if(request.function==0x80151e94){assert(value(0)==0x110);t.environment=text(uint32_t(value(1)));++t.env;return 2;}
  // This genuine world primitive remains unbound in this fixture and production core.
  assert(request.function==0x8007a154);assert(e.waitingParent && (e.waitingParent->flags&0x10) && e.waitingParent->waitingChild==&e);
  t.creation=request.creation;t.schedule=&request.schedule;t.initializer=e.waitingParent->restartScript;
  t.parent=e.waitingParent->id;t.child=e.id;t.childBank=e.locals;t.parentBank=e.waitingParent->locals;
  uint32_t id=0;assert(request.memory.readWord(0x81000164,id));if(t.checkMapOwner)assert(id==t.parent);return 0;
 }
};
FieldAreaGpState fixture(int32_t story){FieldAreaGpState image{};std::memcpy(image.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);for(unsigned n=0;n<4;++n)image.bytes[0x174-FieldAreaGpState::Begin+n]=uint8_t(uint32_t(story)>>(24-8*n));return image;}
int main(){
 assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 for(int32_t story:{18,64,120,174,221,271,347,23,402}) {
  FieldAreaEvent area;assert(area.begin(2,0));const auto id=area.appendAreaInit(fixture(story));assert(id && area.areaInitEventId()==id && area.eventAlive(*id));
  assert(!area.appendAreaInit(fixture(story)) && !area.appendDoorEntry(0,{}));
  FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);assert(info.publishIncoming("dokan",2)); // Deliberately stale external cache; live imported GP must win.
  FieldBeroExec exec;exec.onOff(0,8);
  Trace trace;FieldAreaEventServices services;services.map=&map;services.beroInfo=&info;services.beroExec=&exec;services.user=&trace;services.call=Trace::call;
  area.tick(17,services);assert(!area.failed && area.completed().empty());
  if(story!=402){assert(area.pendingFunction==0x801525a4 && area.pendingEventId==*id && trace.bgm==1 && !info.registered());trace.audioReady=true;}
  for(unsigned n=2;n<7 && area.pendingFunction!=0x8007a154;++n)area.tick(17*n,services);
  assert(!area.failed && area.pendingFunction==0x8007a154 && area.pendingEventId==trace.child && trace.child!=*id && trace.parent==*id);
  assert(area.eventAlive(*id) && area.eventAlive(trace.child) && area.completed().empty() && !area.cases()->active(0));
  assert(area.liveEventCount()==2 && info.registered() && exec.read()==0 && info.currentNumber()==0);
  assert(trace.childBank[1]==0 && trace.childBank[2]==2 && trace.childBank[4]==0 && trace.childBank[5]==6 && uint32_t(trace.childBank[3])==0x8100011c);
  for(unsigned n=1;n<16;++n)assert(trace.parentBank[n]==0); // Actual child has not returned/copy-backed.
  for(unsigned n=9;n<16;++n)assert(trace.childBank[n]==0);
  if(story==402)assert(trace.bgm==0 && trace.env==0);
  else {assert(trace.bgm==2 && trace.env==1 && trace.retained);assert(trace.track==(story==23?"BGM_STG0_GOR1":"BGM_EVT_FRANKURI1"));if(story==23)assert(trace.environment=="ENV_STG0_GOR3");}
 }
 // A retired map parent does not hide a still-pending independent event.
 {FieldAreaEvent area;assert(area.begin(2,0));const auto root=area.appendAreaInit(fixture(402));assert(root);
  FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);FieldBeroExec exec;Trace trace;
  FieldAreaEventServices s;s.map=&map;s.beroInfo=&info;s.beroExec=&exec;s.user=&trace;s.call=Trace::call;
  for(unsigned n=1;n<6 && !trace.child;++n)area.tick(17*n,s);assert(trace.child && area.liveEventCount()==2);
  auto& independent=createRootEvent(*trace.schedule,trace.initializer,0,0,trace.creation);const auto independentId=independent.id;
  deleteEventId(*root,trace.creation);assert(!area.eventAlive(*root));trace.checkMapOwner=false;
  for(unsigned n=6;n<12 && trace.parent!=independentId;++n)area.tick(17*n,s);
  assert(!area.failed && area.completed().size()==1 && !area.completed()[0].returned && area.completed()[0].locals[5]==6);
  assert(trace.parent==independentId && area.eventAlive(trace.child) && area.pendingEventId==trace.child && area.pendingFunction==0x8007a154 && area.liveEventCount()==2);
 }
 // A supplied continuation cannot later be relabeled as the original cleared root.
 FieldAreaEvent continuation;assert(continuation.begin(2,0) && continuation.appendDoorEntry(0,{}));assert(!continuation.appendAreaInit(fixture(23)));
}
