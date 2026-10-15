#include "battle_class_native_frame.hpp"
#include "battle_class_footer_gpu.hpp"
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Resources {
  BattleClassAssets assets;
  MessageFont font;
  Resources(){assert(assets.load("romfs/battle-class-surfaces.bin","romfs/battle-class-icons.bin"));
    assert(font.load("romfs/font.pmfont"));}
};
struct Fixture {
  static constexpr uint32_t B=0x10000,W=0x30000,C=0x40000;
  std::shared_ptr<Resources> resources=std::make_shared<Resources>();
  std::shared_ptr<const uint8_t> token=std::make_shared<const uint8_t>(0);
  std::shared_ptr<CameraManager> bank=std::make_shared<CameraManager>();
  CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;
  BattleClassNativeFrame frame;
  std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{B+0x420,1},{B+0x24,0x60000},{0x60008,222},{B+0x1bb4+8,0},{B+0x1c78,W},{W,C},{W+4,0x50000},{W+0xc,10},{W+0x18,0},{C,0},{C+8,0}};
  std::vector<std::string> trace;
  uint64_t pass=0;bool live=true,free=true,footerReady=false,reenter=false,retire=false,requireCamera=false,changeProducer=false;
  unsigned lookups=0,party=0,pinch=0,submissions=0;
  bool submitReady=false,append=false;
  BattleClassNativeSubmission submission;
  Fixture(){bank->initializeBoot(608,480);}
  BattleActorServices actors(){BattleActorServices a;a.memory.user=this;
    a.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("r"+std::to_string(at));if(!f.words.count(at)){std::fprintf(stderr,"missing%08x\n",unsigned(at));assert(false);}return f.words.at(at);};
    a.memory.storeWord=[](void* p,uint32_t at,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("w"+std::to_string(at)+":"+std::to_string(v));f.words[at]=v;};
    a.memory.storeByte=[](void* p,uint32_t at,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.trace.push_back("b"+std::to_string(at)+":"+std::to_string(v));f.words[at]=v;};return a;}
  BattleClassDisplayServices display(){BattleClassDisplayServices s;s.user=this;s.draw.user=this;s.draw.memory=actors();
    s.cursorLookup=[](void* p,uint32_t menu,uint32_t type,std::optional<uint32_t>&){auto& f=*static_cast<Fixture*>(p);assert(menu==B+0x171c && type==14);++f.lookups;f.trace.push_back("lookup");return true;};
    s.draw.camera=[](void* p,uint32_t camera){assert(camera==8);auto& f=*static_cast<Fixture*>(p);if(f.requireCamera){assert(f.frame.camera() && f.frame.camera()->camera(8));}f.trace.push_back("camera");return true;};
    s.draw.icons=[](void*,uint32_t,uint32_t,const std::vector<BattleActDrawSlot>&){return true;};
    s.draw.partyButton=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.party;
      if(f.reenter){assert(!f.frame.resume(f.pass,f.main,f.draw,f.queue,8,B));assert(!f.frame.bind({},f.resources,{},{},{}));}
      if(f.retire)f.frame.retire();
      if(f.changeProducer)++f.pass;
      if(!f.footerReady)return false;
      if(f.append){BattleClassFooterAtlasQuad packet;packet.texture=&f;packet.generation=f.token;packet.index=4;
        assert(f.frame.appendFooter(packet));}
      f.trace.push_back("party");return true;};
    s.draw.pinchMark=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.pinch;f.trace.push_back("pinch");return true;};return s;}
  bool bind(){BattleClassNativeFrameBinding b;b.allocationOwner=token;b.allocation=W;b.generation=7;b.cameras=bank;b.actors=actors();
    b.addresses={0x60000000,[this](uint32_t base,uint32_t bytes){assert(base==0x60000000 && bytes==13*0x400);return free;}};
    b.allocationCurrent=[this,expected=token]{return live && expected==token && words.at(B+0x1c78)==W;};
    b.producerCurrent=[this](BattleClassCameraEpoch e){return e.generation==7 && e.pass==pass;};
    BattleClassBackend backend;backend.assets=&resources->assets;backend.font=&resources->font;
    backend.label=[](uint32_t)->const char*{assert(false);return nullptr;};
    backend.random=[](uint32_t)->uint32_t{assert(false);return 0;};
    BattleClassRasterServices raster;raster.memory=actors();
    requireCamera=true;return frame.bind(std::move(b),resources,display(),raster,backend);}
  void produce(){CameraMainServices s;s.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};
    s.lookAt=[](void*,const CameraState& c){return cameraLookAtAdapter(c.eye,c.at,c.up);};
    assert(bank->main(false,s,main));++pass;}
  bool dispatch(bool enqueue=true){if(enqueue)assert(queue.enter(8,0,0x8012210c,B,500)==DisplayAdmission::Registered);
    CameraDrawServices s;s.user=this;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};
    s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};
    s.display.callback=[](void* p,uint32_t cam,uint32_t callback,uint32_t battle){auto& f=*static_cast<Fixture*>(p);assert(callback==0x8012210c);return f.frame.resume(f.pass,f.main,f.draw,f.queue,cam,battle,f.submission);};
    return bank->draw(queue,s,draw);}
};
}
int main(){
  Fixture f;assert(f.bind());assert(!f.frame.camera());
  assert(!f.frame.resume(1,f.main,f.draw,f.queue,8,Fixture::B));
  f.produce();f.reenter=true;assert(!f.dispatch());
  assert(f.words.at(Fixture::W+0x14)==1 && f.lookups==1 && f.party==1 && !f.pinch);
  const auto prefix=f.trace;assert(!f.dispatch(false));assert(f.trace==prefix && f.lookups==1 && f.party==2);
  assert(!f.frame.publication().batch());
  f.footerReady=true;assert(f.dispatch(false));assert(f.lookups==1 && f.party==3 && f.pinch==1);
  assert(!f.frame.camera());const auto key=f.frame.publication().key();assert(key.sequence==f.pass);
  assert(f.frame.admit(key));assert(f.frame.consumeAdmission());assert(!f.frame.consumeAdmission());
  // The parent prefix through DrawMainMenu's icon-call boundary and all
  // source stores match the original synchronous reference. The existing
  // observable sorter adds explicit guard reads within that child boundary.
  Fixture reference;reference.footerReady=true;auto services=reference.display();BattleClassDisplayCursor cursor;
  assert(resumePackedBattleClassQueuedDisplay(8,Fixture::B,services,cursor));
  assert(f.words==reference.words);
  assert(std::equal(reference.trace.begin(),reference.trace.end()-2,f.trace.begin()));
  auto writes=[](const std::vector<std::string>& t){std::vector<std::string> out;for(const auto& x:t)if(x[0]=='w' || x[0]=='b')out.push_back(x);return out;};
  assert(writes(f.trace)==writes(reference.trace));
  // A later actual producer pass advances source once; render admission never
  // invokes another source draw. Empty source rows are not invented icons.
  f.produce();assert(f.dispatch());assert(f.lookups==2 && f.pinch==2);
  for(unsigned mutation=0;mutation<5;++mutation){Fixture held;assert(held.bind());held.produce();assert(!held.dispatch());
    const auto trace=held.trace;
    if(mutation==0)held.token=std::make_shared<const uint8_t>(0);
    if(mutation==1)held.words[Fixture::B+0x1c78]=Fixture::W+4;
    if(mutation==2)held.queue.reinitialize();
    if(mutation==3)++held.pass;
    if(mutation==4)held.free=false;
    held.footerReady=true;assert(!held.dispatch(false));assert(held.trace==trace && held.pinch==0);
    assert(!held.frame.publication().batch());}
  Fixture retired;assert(retired.bind());retired.produce();retired.retire=true;retired.footerReady=true;
  assert(!retired.dispatch());assert(retired.pinch==0 && !retired.frame.publication().batch());
  assert(retired.bind()); // Deferred retirement completed after callback exit.
  Fixture changed;assert(changed.bind());changed.produce();changed.footerReady=true;changed.changeProducer=true;
  assert(!changed.dispatch());assert(changed.pinch==0 && !changed.frame.publication().batch());
  Fixture gpu;assert(gpu.bind());gpu.produce();gpu.footerReady=true;gpu.append=true;
  gpu.submission={&gpu,[](void* p,const BattleClassBackend& batch){auto& f=*static_cast<Fixture*>(p);
    assert(f.frame.camera()&&batch.quads.size()==1&&batch.quads[0].texture==BattleClassTexture::Footer);
    ++f.submissions;return f.submitReady;}};
  assert(!gpu.dispatch());const auto sourceComplete=gpu.trace;
  assert(gpu.lookups==1&&gpu.party==1&&gpu.pinch==1&&gpu.submissions==1);
  assert(!gpu.dispatch(false)&&gpu.trace==sourceComplete&&gpu.submissions==2);
  const auto originalSubmit=gpu.submission;gpu.submission.user=&f;
  assert(!gpu.dispatch(false)&&gpu.trace==sourceComplete&&gpu.submissions==2);
  gpu.submission=originalSubmit;gpu.submitReady=true;assert(gpu.dispatch(false));
  assert(gpu.trace==sourceComplete&&gpu.submissions==3&&gpu.frame.consumeAdmission());
  BattleClassFooterAtlasQuad outside;assert(!gpu.frame.appendFooter(outside));
  Fixture lostGpu;assert(lostGpu.bind());lostGpu.produce();lostGpu.footerReady=true;
  lostGpu.submission={&lostGpu,[](void* p,const BattleClassBackend&){auto& f=*static_cast<Fixture*>(p);++f.submissions;f.live=false;return true;}};
  assert(!lostGpu.dispatch()&&lostGpu.submissions==1);lostGpu.live=true;
  assert(!lostGpu.dispatch(false)&&lostGpu.submissions==1); // No revival/replayed accepted GPU.
  Fixture unavailable;unavailable.resources.reset();assert(!unavailable.frame.bind({}, {}, {}, {}, {}));
  return 0;
}
