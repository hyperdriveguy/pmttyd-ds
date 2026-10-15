#include "fixtures/harbor_class_frame_fixture.hpp"
#include "battle_class_native_footers.hpp"
#include "battle_class_surface_gpu.hpp"
#include "battle_audience_item_on_native.hpp"
#include <cassert>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x84000000,Base=0x60010000;
uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}
struct Fixture {
  HarborClassFrameFixture original;
  BattleClassFooterGpuResource resource;
  BattleClassSurfaceGpuResource surfaces;
  std::shared_ptr<CameraManager> cameras=std::make_shared<CameraManager>();
  CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;
  BattleClassNativeFrame frame;
  std::unique_ptr<BattleClassNativeNumeric> numeric;
  std::unique_ptr<BattleClassNativeFooters> footers;
  BattleActorServices actors;
  uint32_t work=0,mario=0,other=0,originalParty=0;
  uint64_t pass=0;bool current=true,soundReady=true,resourcesReady=true,textureReadsReady=true,reenter=false,invalidateOnSound=false;
  unsigned sounds=0,soundAccepted=0,submitAttempts=0;bool submitReady=true;
  std::vector<BattleClassQuad> submitted;
  Fixture(bool soundBound=true){
    assert(original.prepare("romfs"));actors=original.source;
    auto actual=original.scene.classMenuDrawBinding({});assert(actual);
    work=actual->allocation;mario=battleMarioUnit(B,actors);
    originalParty=battlePartyUnit(B,actors);other=battleRegistryUnit(B,2,actors);
    assert(mario && other && other!=mario);
    // Controlled source-branch fixture: keep actual VM allocation/resource
    // owners, isolate class rows, and deliberately assign existing guest actors
    // to the footer's registry fields. This never claims real party/population.
    put(word(work)+8,0);put(work+0xc,10);
    actors.memory.storeHalf(actors.memory.user,mario+0x10c,10);
    actors.memory.storeHalf(actors.memory.user,mario+0x108,20);
    actors.memory.storeHalf(actors.memory.user,other+0x10c,10);
    actors.memory.storeHalf(actors.memory.user,other+0x108,20);
    if(originalParty)put(originalParty+0x1c,word(originalParty+0x1c)|8);
    for(uint32_t i=0;i<200;++i)put(B+0x2858+0x1bc+i*0x134,0);
    BattleClassFooterTextureServices imports{this,[](void* p,const char*)->std::optional<BattleClassFooterTextureAdmission>{return BattleClassFooterTextureAdmission{p,512,256,0};},[](void*,const void*){}};
    assert(resource.load("romfs",imports));
    BattleClassSurfaceTextureServices surfaceImport{this,[](void* p,uint32_t i,const char*)->std::optional<BattleClassSurfaceImport>{
      return BattleClassSurfaceImport{static_cast<const char*>(p)+i,256,i?256u:128u,0,1,256,i?256u:128u,false,{0,1,1,0}};
    },[](void*,uint32_t,const void*){}};
    assert(surfaces.load("romfs",surfaceImport));
    cameras->initializeBoot(608,480);
    const auto sourceWord=[this](uint32_t at)->std::optional<uint32_t>{if(!textureReadsReady)return {};uint32_t v=0;return original.scene.readPackedWord(at,v)?std::optional<uint32_t>(v):std::nullopt;};
    BattleClassNativeNumericFooters callbacks{this,[](void* p){auto& f=*static_cast<Fixture*>(p);return f.footers->party();},[](void* p){auto& f=*static_cast<Fixture*>(p);return f.footers->pinch();}};
    numeric=std::make_unique<BattleClassNativeNumeric>(frame,actors,Base,sourceWord,[this]{return current;},callbacks);
    std::function<bool(uint32_t)> sound;
    if(soundBound)sound=[this](uint32_t key){assert(key==0x802ef7a4);++sounds;if(reenter){assert(!footers->party() && !footers->pinch());}if(!soundReady)return false;++soundAccepted;if(invalidateOnSound)current=false;return true;};
    footers=std::make_unique<BattleClassNativeFooters>(frame,*numeric,resource,actors,sourceWord,[this]{return current&&resourcesReady;},sound);
    BattleClassNativeFrameBinding binding;binding.allocationOwner=actual->owner;binding.allocation=work;binding.generation=uint64_t(reinterpret_cast<uintptr_t>(actual->owner.get()));binding.cameras=cameras;binding.actors=actors;
    binding.addresses={Base,[](uint32_t b,uint32_t n){return b==Base && n==13*0x400;}}; // Explicit isolated test virtual reservation.
    binding.allocationCurrent=[this,live=actual->current]{return current&&surfaces.lease().current()&&live();};
    binding.producerCurrent=[this,generation=binding.generation](BattleClassCameraEpoch e){return e.generation==generation&&e.pass==pass;};
    auto display=actual->display;display.draw=numeric->draw();
    auto backend=original.backend;backend.surfaceLease=surfaces.lease();backend.assets=backend.surfaceLease.assets.get();
    assert(frame.bind(std::move(binding),actual->owner,display,numeric->raster(),backend));
  }
  uint32_t word(uint32_t at)const{return actors.memory.word(actors.memory.user,at);}
  void put(uint32_t at,uint32_t v){actors.memory.storeWord(actors.memory.user,at,v);}
  float scalar(uint32_t at)const{const auto v=word(at);float f;std::memcpy(&f,&v,4);return f;}
  void party(bool on){if(on){put(other+8,224);put(other+0x1c,word(other+0x1c)&~8u);actors.memory.storeByte(actors.memory.user,other+0xc,0);}else put(other+0x1c,word(other+0x1c)|8);assert(bool(battlePartyUnit(B,actors))==on);}
  void item(bool on){const auto member=B+0x2858+0x1bc;
    put(member,on?1:0);actors.memory.storeHalf(actors.memory.user,member+0x1c,0);
    put(member+0x48,bits(10));put(member+0x4c,bits(20));put(member+0x50,bits(30));
    put(B+0x2858+0xf25c+4,0);}
  void allowChange(){put(B+0xef4,word(B+0xef4)&~uint32_t(0x40800000));
    for(const auto unit:{mario,other}){for(const auto off:{0x20u,0x118u,0x11au,0x122u,0x135u,0x12bu})actors.memory.storeByte(actors.memory.user,unit+off,0);
      actors.memory.storeByte(actors.memory.user,unit+0x22,1);actors.memory.storeByte(actors.memory.user,unit+0x23,1);put(unit+0x104,word(unit+0x104)&~uint32_t(0x10));}}
  void hp(uint32_t unit,int16_t value){actors.memory.storeHalf(actors.memory.user,unit+0x10c,uint16_t(value));}
  void referenceParty(float initial,int32_t expectedType,unsigned expectedSounds){
    // Differential parent-only effect check against the existing synchronous
    // original helper. Its explicit fixture projection can differ from native
    // Main; projection does not affect the compared W120/type/sound decisions.
    struct Reference{Fixture* fixture;int32_t type=-1;unsigned sounds=0;};Reference ref{this};
    const auto finalAngle=word(work+0x120);put(work+0x120,bits(initial));
    BattleClassFooterServices s;s.raster.memory=actors;s.user=&ref;s.raster.user=&ref;
    s.raster.request=[](void* p,const BattleClassRasterRequest& q)->std::optional<uint32_t>{auto& f=*static_cast<Reference*>(p)->fixture;assert(q.function==BattleClassRasterService::Camera&&q.integers==std::vector<uint32_t>{8});return f.original.Cam8;};
    s.project=[](void* p,uint32_t at){return static_cast<Reference*>(p)->fixture->original.project(at);};
    s.audienceItem=[](void* p,uint32_t)->std::optional<int32_t>{return nativeBattleAudienceItemOn(static_cast<Reference*>(p)->fixture->actors,BattleAudiencePrivateOutputs::IndexAndPosition).index;};
    s.changePosition=[](void* p,uint32_t battle){auto& f=*static_cast<Reference*>(p)->fixture;return f.original.footer.changePosition(f.original.footer.user,battle);};
    s.sound=[](void* p,uint32_t key){assert(key==0x802ef7a4);++static_cast<Reference*>(p)->sounds;return true;};
    s.button=[](void* p,int32_t type,float,float,float){static_cast<Reference*>(p)->type=type;return true;};
    assert(packedBattleClassPartyButton(s));assert(word(work+0x120)==finalAngle && ref.type==expectedType && ref.sounds==expectedSounds);
  }
  void produce(){CameraMainServices s;s.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};s.lookAt=[](void*,const CameraState& c){return cameraLookAtAdapter(c.eye,c.at,c.up);};assert(cameras->main(false,s,main));++pass;}
  bool dispatch(bool enqueue=true){if(enqueue)assert(queue.enter(8,0,0x8012210c,B,500)==DisplayAdmission::Registered);
    CameraDrawServices s;s.user=this;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};
    s.display.callback=[](void* p,uint32_t camera,uint32_t,uint32_t battle){auto& f=*static_cast<Fixture*>(p);BattleClassNativeSubmission submission{p,[](void* p,const BattleClassBackend& batch){auto& f=*static_cast<Fixture*>(p);++f.submitAttempts;if(!f.submitReady)return false;assert(validateBattleClassFooterBatch(batch,&f.resource));f.submitted=batch.quads;return true;}};
      return f.frame.resume(f.pass,f.main,f.draw,f.queue,camera,battle,submission);};return cameras->draw(queue,s,draw);}
  std::vector<uint32_t> indices()const{std::vector<uint32_t> out;for(const auto& q:submitted){assert(q.texture==BattleClassTexture::Footer&&q.footer);out.push_back(q.footer->index);}return out;}
};
}
int main(){
  assert(loadHarborTutorialAssets("romfs"));
  Fixture solo;assert(!solo.footers->party() && !solo.footers->pinch());solo.put(solo.work+0x120,bits(12));solo.produce();assert(solo.dispatch());assert(solo.scalar(solo.work+0x120)==90 && solo.indices().empty() && solo.sounds==0);solo.referenceParty(12,-1,0);
  Fixture item;item.item(true);item.put(item.work+0x120,bits(90));item.produce();assert(item.dispatch());assert(item.scalar(item.work+0x120)<90 && item.indices()==std::vector<uint32_t>{5} && item.sounds==0);item.referenceParty(90,5,0);
  Fixture arrival;arrival.party(true);arrival.item(true);arrival.put(arrival.work+0x120,bits(90));arrival.soundReady=false;arrival.reenter=true;arrival.produce();assert(!arrival.dispatch());assert(arrival.scalar(arrival.work+0x120)==90 && arrival.sounds==1 && arrival.submitAttempts==0);
  const auto cache=arrival.word(B+0x2858+0x138bc);arrival.put(B+0x2858+0x138bc,123);
  assert(!arrival.dispatch(false));assert(arrival.word(B+0x2858+0x138bc)==123 && arrival.sounds==2 && arrival.scalar(arrival.work+0x120)==90);
  arrival.soundReady=true;assert(arrival.dispatch(false));assert(arrival.sounds==3 && arrival.soundAccepted==1 && arrival.word(B+0x2858+0x138bc)==123 && cache==0 && arrival.indices()==std::vector<uint32_t>{5});
  const auto decremented=arrival.word(arrival.work+0x120);arrival.referenceParty(90,5,1);
  // Native submission hold retains completed source/footer/FIFO output.
  Fixture submit;submit.party(true);submit.item(true);submit.put(submit.work+0x120,bits(90));submit.submitReady=false;submit.produce();assert(!submit.dispatch());const auto angle=submit.word(submit.work+0x120);assert(submit.soundAccepted==1);
  assert(!submit.dispatch(false));assert(submit.word(submit.work+0x120)==angle && submit.soundAccepted==1);submit.submitReady=true;assert(submit.dispatch(false));assert(submit.indices()==std::vector<uint32_t>{5} && angle==decremented);
  Fixture lost;lost.party(true);lost.item(true);lost.put(lost.work+0x120,bits(90));lost.invalidateOnSound=true;lost.produce();assert(!lost.dispatch() && lost.soundAccepted==1 && lost.scalar(lost.work+0x120)==90 && !lost.frame.publication().batch());lost.current=true;assert(!lost.dispatch(false) && lost.soundAccepted==1);
  Fixture noSound(false);noSound.party(true);noSound.item(true);noSound.put(noSound.work+0x120,bits(90));noSound.produce();assert(!noSound.dispatch());assert(noSound.scalar(noSound.work+0x120)==90 && !noSound.frame.publication().batch());
  Fixture change;change.party(true);change.allowChange();change.produce();assert(change.dispatch());assert(change.indices()==std::vector<uint32_t>{4} && change.scalar(change.work+0x120)==90 && change.sounds==0);change.referenceParty(90,4,0);
  Fixture texture;texture.item(true);texture.put(texture.work+0x120,bits(90));texture.textureReadsReady=false;texture.produce();assert(!texture.dispatch());const auto firstAngle=texture.word(texture.work+0x120);assert(!texture.dispatch(false) && texture.word(texture.work+0x120)==firstAngle);texture.textureReadsReady=true;assert(texture.dispatch(false) && texture.indices()==std::vector<uint32_t>{5} && texture.word(texture.work+0x120)==firstAngle);
  Fixture overshoot;overshoot.item(true);const float step=overshoot.scalar(0x80427440u+uint32_t(-20368));overshoot.put(overshoot.work+0x120,bits(step*.5f));overshoot.produce();assert(overshoot.dispatch());assert(overshoot.word(overshoot.work+0x120)==bits(step*.5f-step));overshoot.produce();assert(overshoot.dispatch() && overshoot.scalar(overshoot.work+0x120)==0);
  Fixture pinch;pinch.party(true);pinch.allowChange();pinch.hp(pinch.mario,5);pinch.hp(pinch.other,1);pinch.produce();assert(pinch.dispatch());const auto indices=pinch.indices();assert(indices.size()>=2);
  const auto gp=pinch.word(0x80415fd0),lang=pinch.word(gp+0x16c);const auto danger=pinch.actors.memory.half(pinch.actors.memory.user,0x80416d24+2*lang);const auto peril=pinch.actors.memory.half(pinch.actors.memory.user,0x80416d30+2*lang);
  assert(indices.size()==3 && indices[0]==4);const bool partyFirst=pinch.scalar(pinch.mario+0x3c)>pinch.scalar(0x80427440u+uint32_t(-20388));assert(indices[1]==(partyFirst?peril:danger) && indices[2]==(partyFirst?danger:peril));
  for(const auto& q:pinch.submitted)if(q.footer->index==danger)for(const auto& v:q.footer->vertices)assert(v.rgba==0xffe940ff);
  Fixture retired;retired.resource.retire();retired.produce();assert(!retired.dispatch() && !retired.frame.publication().batch());
  Fixture unavailable;unavailable.resourcesReady=false;unavailable.produce();assert(!unavailable.dispatch());assert(unavailable.submitAttempts==0 && !unavailable.frame.publication().batch());
  return 0;
}
