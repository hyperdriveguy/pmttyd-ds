#include <memory>
#include "battle_weapon_native_frame.hpp"
#include "fixtures/battle_weapon_source_draw_fixture.hpp"
struct NativeWeaponFixture {
 WeaponSourceFixture source;
 std::shared_ptr<CameraManager> bank=std::make_shared<CameraManager>();
 std::shared_ptr<const uint8_t> token=std::make_shared<const uint8_t>(0);
 std::shared_ptr<const int> resource=std::make_shared<const int>(1),batch=std::make_shared<const int>(2);
 CameraMainCursor main;CameraDrawCursor draw;DisplayQueue queue;
 BattleWeaponNativeFrame frame;
 uint64_t pass=0;bool live=true,resourcesLive=true,free=true,beginReady=true,sealReady=true,submitReady=true,batchLive=true;
 bool reenter=false,retire=false,mutateAfterSubmit=false;
 unsigned begins=0,seals=0,submits=0;
 NativeWeaponFixture(){bank->initializeBoot(608,480);}
 BattleWeaponNativeSubmission submission(){return {this,
 [](void*p,uint64_t pass){auto&f=*static_cast<NativeWeaponFixture*>(p);assert(pass==f.pass&&f.frame.camera());++f.begins;return f.beginReady;},
 [](void*p,uint64_t pass)->std::shared_ptr<const void>{auto&f=*static_cast<NativeWeaponFixture*>(p);assert(f.frame.sourceComplete(pass)&&f.frame.camera());++f.seals;if(f.reenter){assert(!f.frame.resume(f.pass,f.main,f.draw,f.queue,8,WeaponSourceFixture::B));assert(!f.frame.bind({},f.resource,{},{}));}if(f.retire)f.frame.retire();return f.sealReady?f.batch:nullptr;},
 [](void*p,uint64_t pass,const std::shared_ptr<const void>&batch){auto&f=*static_cast<NativeWeaponFixture*>(p);assert(f.frame.sourceComplete(pass)&&batch==f.batch&&f.frame.camera());++f.submits;const auto view=f.frame.camera()->view();assert(view&&view->viewport==f.bank->camera(8)->viewport);const auto token=f.frame.camera()->camera(8);assert(token&&f.frame.camera()->owns(*token)&&f.frame.camera()->word(*token+0x15c)==WeaponSourceFixture::bits(view->matrices.projection[0][0]));assert(!f.frame.camera()->word(*token));assert(!f.frame.publication());if(f.mutateAfterSubmit)f.bank->camera(8)->viewport[0]+=1;return f.submitReady;},
 [](void*p,const std::shared_ptr<const void>&batch){auto&f=*static_cast<NativeWeaponFixture*>(p);return f.batchLive&&batch==f.batch;}};}
 bool bind(){BattleWeaponNativeFrameBinding b;b.allocationOwner=token;b.sourceResourceOwner=resource;b.sourceResourceCurrent=[this]{return resourcesLive;};b.allocation=WeaponSourceFixture::W;b.battle=WeaponSourceFixture::B;b.generation=9;b.cameras=bank;b.actors=source.actors();
 b.addresses={0x60000000,[this](uint32_t,uint32_t){return free;}};
 b.allocationCurrent=[this,expected=token]{return live&&expected==token&&source.words[WeaponSourceFixture::B+0x1c78]==WeaponSourceFixture::W;};
 b.producerCurrent=[this](BattleClassCameraEpoch e){return e.generation==9&&e.pass==pass;};
 return frame.bind(std::move(b),resource,source.services(),submission());}
 void produce(){CameraMainServices s;s.callback=[](void*,uint32_t,uint32_t,CameraState&){return true;};s.lookAt=[](void*,const CameraState&c){return cameraLookAtAdapter(c.eye,c.at,c.up);};assert(bank->main(false,s,main));++pass;}
 bool dispatch(bool enter=true,uint32_t callback=0x80121ce8){if(enter)assert(queue.enter(8,0,callback,WeaponSourceFixture::B,500)==DisplayAdmission::Registered);
 CameraDrawServices s;s.user=this;s.letterbox=[](void*,DisplayQueue&){return true;};s.request=[](void*,const CameraDrawRequest&){return true;};s.display.user=this;s.display.state=[](void*,const DisplayStateRequest&){return true;};s.display.callback=[](void*p,uint32_t c,uint32_t,uint32_t b){auto&f=*static_cast<NativeWeaponFixture*>(p);return f.frame.resume(f.pass,f.main,f.draw,f.queue,c,b);};return bank->draw(queue,s,draw);}
};
int main(){
 NativeWeaponFixture f;assert(f.bind());assert(!f.frame.camera());assert(!f.frame.resume(1,f.main,f.draw,f.queue,8,WeaponSourceFixture::B));
 f.produce();f.source.hold=0;assert(!f.dispatch());assert(f.begins==1&&!f.seals&&!f.frame.publication());auto ops=f.source.ops;assert(!f.dispatch(false)&&f.source.ops==ops);assert(f.dispatch(false));assert(f.begins==1&&f.seals==1&&f.submits==1&&f.frame.publication()==f.batch&&!f.frame.camera());
 const auto completed=f.source.events;assert(!f.frame.resume(f.pass,f.main,f.draw,f.queue,8,WeaponSourceFixture::B)&&f.source.events==completed);
 WeaponSourceFixture reference;assert(reference.run());assert(f.source.events==reference.events&&f.source.words==reference.words&&f.source.bytes==reference.bytes);
 f.queue.reinitialize();f.produce();assert(f.dispatch());assert(f.begins==2&&f.submits==2&&f.frame.publishedPass()==f.pass);
 for(unsigned hold=0;hold<3;++hold){NativeWeaponFixture wait;assert(wait.bind());wait.produce();wait.beginReady=hold!=0;wait.sealReady=hold!=1;wait.submitReady=hold!=2;assert(!wait.dispatch());auto source=wait.source.events;assert(!wait.frame.publication());assert(!wait.dispatch(false));if(hold)assert(wait.source.events==source);else assert(source.empty());wait.beginReady=wait.sealReady=wait.submitReady=true;assert(wait.dispatch(false));assert(wait.submits==(hold==2?3u:1u));assert(wait.frame.publication());}
 for(unsigned mutation=0;mutation<10;++mutation){NativeWeaponFixture wait;assert(wait.bind());wait.produce();wait.sealReady=false;assert(!wait.dispatch());const auto prefix=wait.source.events;
 if(mutation==0)wait.token=std::make_shared<const uint8_t>(0);
 if(mutation==1)wait.source.words[WeaponSourceFixture::B+0x1c78]=WeaponSourceFixture::W+4;
 if(mutation==2){wait.queue.reinitialize();assert(wait.queue.enter(8,0,0x80121ce8,WeaponSourceFixture::B,500)==DisplayAdmission::Registered);}
 if(mutation==3)++wait.pass;
 if(mutation==4)wait.free=false;
 if(mutation==5)wait.bank->camera(8)->viewport[0]+=1;
 if(mutation==6)wait.bank->camera(8)->scissor[0]+=1;
 if(mutation==7)wait.bank->camera(8)->projectionKind^=1;
 if(mutation==8)wait.bank->camera(4)->matrices.view[0][0]+=1;
 if(mutation==9)wait.resourcesLive=false;
 wait.sealReady=true;assert(!wait.dispatch(false));assert(wait.source.events==prefix&&!wait.submits&&!wait.frame.publication());}
 for(unsigned gate=0;gate<reference.ops.size();++gate){NativeWeaponFixture held;assert(held.bind());held.produce();held.source.hold=int(gate);assert(!held.dispatch());auto prefix=held.source.events;assert(!held.dispatch(false)&&held.source.events==prefix);assert(held.dispatch(false));assert(held.source.events==reference.events&&held.begins==1&&held.submits==1);}
 NativeWeaponFixture classCall;assert(classCall.bind());classCall.produce();assert(!classCall.dispatch(true,0x8012210c)&&!classCall.begins&&classCall.source.events.empty());
 NativeWeaponFixture noProducer;assert(noProducer.bind());assert(!noProducer.dispatch()&&!noProducer.begins);
 NativeWeaponFixture retired;assert(retired.bind());retired.produce();retired.retire=true;assert(!retired.dispatch());assert(!retired.submits&&!retired.frame.publication());assert(retired.bind());
 NativeWeaponFixture recursive;assert(recursive.bind());recursive.produce();recursive.reenter=true;assert(recursive.dispatch());
 NativeWeaponFixture accepted;assert(accepted.bind());accepted.produce();accepted.mutateAfterSubmit=true;assert(!accepted.dispatch()&&accepted.submits==1);assert(!accepted.dispatch(false)&&accepted.submits==1&&!accepted.frame.publication());
 NativeWeaponFixture dead;assert(dead.bind());dead.produce();assert(dead.dispatch());dead.batchLive=false;assert(!dead.frame.publication());dead.batchLive=true;assert(!dead.frame.publication());
 NativeWeaponFixture rebound;assert(rebound.bind());rebound.produce();rebound.submitReady=false;assert(!rebound.dispatch());rebound.frame.retire();rebound.token=std::make_shared<const uint8_t>(0);rebound.draw={};rebound.queue.reinitialize();// source graph completed before the prior submission hold.
 rebound.submitReady=true;assert(rebound.bind());rebound.produce();assert(rebound.dispatch());
 std::cout<<"Real weapon queued callback/camera admission, retained whole source, immutable submission and lifetime tests passed\n";
}
