#include "native_harbor_nice.hpp"
#include "native_harbor_nice_visual.hpp"
#include "event_manager.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
struct Fixture {
 EventScriptAssets original;EventEntry event;NativeHarborNiceServices services;
 std::shared_ptr<int>life=std::make_shared<int>(0);bool live=true;
 std::map<uint32_t,uint32_t>ram;std::vector<std::string>accepted;
 size_t hold=SIZE_MAX,visits=0;unsigned waits=0;int spawned=0,scaled=0,sounded=0,pitched=0;
 NativeHarborNiceSpawn visual;std::array<float,3>audio{};float scale=0;uint32_t sound=0;
 bool gate(const std::string&label){const auto i=accepted.size();if(i==hold&&waits++<2)return false;accepted.push_back(label);++visits;return true;}
 Fixture(int grade=2){
  assert(original.load("romfs/harbor-tutorial-events.pmevt"));
  event.flags=1;event.id=7;event.currentCommand=0x95570000;event.arguments=0x81020000;event.argumentCount=4;
  ram[0x80415fd0]=0x81000000;ram[0x8100016c]=1;
  ram[event.arguments]=uint32_t(grade);ram[event.arguments+4]=400;ram[event.arguments+8]=10;ram[event.arguments+12]=20;
  services.user=this;services.lifetime=life;services.current=[](void*p){return static_cast<Fixture*>(p)->live;};
  services.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate("word"+std::to_string(a)))return {};auto i=f.ram.find(a);if(i!=f.ram.end())return i->second;uint32_t v;if(!f.original.readWord(a,v))return {};return v;};
  services.value=[](void*p,EventEntry&,int32_t raw)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate("value"+std::to_string(raw)))return {};return raw;};
  services.spawn=[](void*p,const NativeHarborNiceSpawn&q)->std::optional<uint64_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate("spawn"))return {};++f.spawned;f.visual=q;return 0x100000001ULL;};
  services.scale=[](void*p,uint64_t id,float v){auto&f=*static_cast<Fixture*>(p);assert(id==0x100000001ULL);if(!f.gate("scale"))return false;++f.scaled;f.scale=v;return true;};
  services.sound=[](void*p,uint32_t key,const std::array<float,3>&v)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.gate("sound"))return {};++f.sounded;f.audio=v;f.sound=key;return -1;};
  services.pitch=[](void*p,int32_t handle,int32_t pitch){auto&f=*static_cast<Fixture*>(p);assert(handle==-1&&pitch==0);if(!f.gate("pitch"))return false;++f.pitched;return true;};
 }
};
int main(){
 Fixture reference;NativeHarborNiceCursor base;if(!resumeNativeHarborNice(reference.event,reference.services,base)){std::fprintf(stderr,"Nice pending %08x last%s\n",base.pending(),reference.accepted.back().c_str());return 1;}
 assert(reference.spawned==1&&reference.scaled==1&&reference.sounded==1&&reference.pitched==1);
 assert(reference.sound==0x802ee650&&reference.visual.type==0);
 assert(std::fabs(reference.scale-.9f)<.000001f&&std::fabs(reference.audio[0]-106.f)<.001f&&std::fabs(reference.visual.position[0]-52.f)<.001f);
 assert(reference.audio[0]<400&&reference.visual.position[0]<reference.audio[0]);
 assert(reference.audio[1]==10&&reference.visual.position[1]!=10&&reference.visual.position[2]==20);
 for(size_t h=0;h<reference.accepted.size();++h){Fixture f;f.hold=h;NativeHarborNiceCursor c;
  assert(!resumeNativeHarborNice(f.event,f.services,c));assert(!resumeNativeHarborNice(f.event,f.services,c));
  assert(resumeNativeHarborNice(f.event,f.services,c));assert(f.accepted==reference.accepted);
  assert(f.scale==reference.scale&&f.visual.position==reference.visual.position&&f.audio==reference.audio);
  assert(f.spawned==1&&f.scaled==1&&f.sounded==1&&f.pitched==1);
  const auto n=f.accepted.size();assert(resumeNativeHarborNice(f.event,f.services,c)&&f.accepted.size()==n);
 }
 for(int grade=0;grade<=6;++grade){Fixture f(grade);NativeHarborNiceCursor c;assert(resumeNativeHarborNice(f.event,f.services,c));assert(f.visual.type==uint32_t(grade<=2?0:grade-2));}
 for(int grade:{-1,7}){Fixture f(grade);NativeHarborNiceCursor c;assert(resumeNativeHarborNice(f.event,f.services,c));assert(!f.spawned&&!f.sounded);}
 {Fixture f;f.hold=2;NativeHarborNiceCursor c;assert(!resumeNativeHarborNice(f.event,f.services,c));auto replaced=f.services;replaced.lifetime=std::make_shared<int>(0);assert(!resumeNativeHarborNice(f.event,replaced,c));f.live=false;assert(!resumeNativeHarborNice(f.event,f.services,c));}
 BattleStartupResources resources;assert(resources.load("romfs"));
 bool consumer=true;auto life=std::make_shared<int>(0);NativeHarborNiceVisual visual(resources,life,[&]{return consumer;});
 NativeHarborNiceVisualRequest q;q.context=1;q.language=1;q.sourceKey=0x802f3d00;q.modelScale=2;q.fadeStart=30;q.milliseconds=100;
 assert(!visual.spawn(q)); // CPU catalog alone never admits a GPU consumer.
 assert(resources.admitTexture(3,[](unsigned,const std::string&){return true;})); // Explicit TEST import proof.
 auto id=visual.spawn(q);assert(id&&*id);assert(visual.scale(*id,reference.scale));
 assert(visual.advance(100));assert(visual.instances()[0].sample.selectedTime==1);
 assert(visual.advance(600));assert(visual.instances()[0].sample.selectedTime==31&&visual.instances()[0].alpha<1);
 assert(visual.advance(1000)&&!visual.instances()[0].id);assert(!visual.advance(999));
 q.milliseconds=1000;q.sourceKey=0x802f3d0c;assert(!visual.spawn(q));q.sourceKey=0x802f3d00;q.language=0;assert(!visual.spawn(q));q.language=1;
 consumer=false;assert(!visual.spawn(q));consumer=true;id=visual.spawn(q);assert(id);
 resources.releaseTexture(3);assert(!visual.advance(1001));assert(visual.instances()[0].id==*id);
 visual.retire();assert(!visual.current()&&!visual.spawn(q));
}
