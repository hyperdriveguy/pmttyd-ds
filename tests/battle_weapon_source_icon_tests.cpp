#include <memory>
#include "battle_weapon_source_icon.hpp"
#include "battle_weapon_icon_evidence.hpp"
#include "battle_audience_aux_callback.hpp"
#include "native_ui_startup.hpp"
#include "source_icon_position.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
using namespace ttyd;
namespace {
constexpr uint32_t CameraBase=0x78040000,CameraAddress=CameraBase+8*0x400,PrivateObject=0x78010000;
uint32_t bits(float f){uint32_t x;std::memcpy(&x,&f,4);return x;}
uint32_t backing(uint32_t n){uint32_t p=8;while(p<n+4)p*=2;return p;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double x){volatile float v=float(x);return v;};return p;}
struct Scope {
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>ui;std::shared_ptr<NativeSourceIconProcess>icons;
 std::shared_ptr<BattleWeaponOrderedPublication>intentOwner=std::make_shared<BattleWeaponOrderedPublication>();
 std::shared_ptr<BattleWeaponSourceIconPin>pin;std::shared_ptr<SourceGxSetterRouting>gx;
 BattleWeaponSourceIcon*child=nullptr;
 bool source=true,alive=true,resident=true,reentry=false;unsigned math=0,queries=0,appends=0,attempts=0;
 unsigned holdMath=UINT32_MAX,holdAppend=UINT32_MAX;std::vector<RetainedIconPacket>packets;std::vector<std::shared_ptr<const SourceGxInheritedSnapshot>>evidence;
 std::shared_ptr<const uint8_t>testResidentGeneration=std::make_shared<const uint8_t>(0);uint8_t testResidentStorage=0;
 bool current()const{return alive&&ui&&ui->current()&&icons&&icons->current();}
 std::optional<BattleEnemyHpIconResidency>testResidency(uint32_t id){if(reentry){assert(!child->current());assert(!child->resumeMatrix({}));}++queries;if(!resident)return {};auto p=icons->word(0x8041e630);if(!p)return {};auto table=icons->word(*p+8);if(!table)return {};auto h=icons->word(*table+id*8);if(!h)return {};auto sizes=icons->word(*h);if(!sizes)return {};
  BattleEnemyHpIconResidency r;r.texture=&testResidentStorage;r.generation=testResidentGeneration;r.image=id;r.width=uint16_t(*sizes);r.height=uint16_t(*sizes>>16);r.gpuWidth=backing(r.width);r.gpuHeight=backing(r.height);r.format=0;return r;
 }
};
struct F {
 std::shared_ptr<Scope>s=std::make_shared<Scope>();std::unique_ptr<BattleWeaponSourceIcon>child;BattleAudienceIconRequest request;
 F(){s->process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(s->process&&s->process->ready());s->ui=NativeUiStartup::create(s->process,"romfs");assert(s->ui&&s->ui->resumeIcons());s->icons=s->ui->iconProcess();assert(s->icons&&s->icons->ready());
  std::weak_ptr<Scope>w=s;s->pin=std::make_shared<BattleWeaponSourceIconPin>(s->intentOwner,s->process,[w]{auto p=w.lock();return p&&p->current();});s->gx=std::make_shared<SourceGxSetterRouting>(s->ui->gxHistory(),s->process,[w]{auto p=w.lock();return p&&p->current()&&p->source;});
  BattleWeaponSourceIconBinding b;b.textures.process=s->icons;b.textures.lifetime=s;
  b.textures.privateAddress=PrivateObject;b.textures.longCurrent=[w]{auto p=w.lock();return p&&p->current();};b.textures.sourceCurrent=[w]{auto p=w.lock();return p&&p->current()&&p->source;};
  b.textures.unmapped=[w](uint32_t a,uint32_t n){auto p=w.lock();return p&&p->current()&&!(uint64_t(a)<uint64_t(CameraBase)+13*0x400&&uint64_t(CameraBase)<uint64_t(a)+n)&&p->process->process().vm().memory.unmappedRange(a,n);};
  b.textures.truncate=[w](float v)->std::optional<int32_t>{auto p=w.lock();if(!p||!p->current())return {};auto&vm=p->process->process().vm();return vm.platform.truncate(vm.platform.user,v);};
  // TEST-only direct SDK camera fixture. This proves a leaf's source/matrix/
  // packet behavior, not actual CameraMain, UI allocation or queue dispatch.
  b.camera.lifetime=s;b.camera.user=s.get();b.camera.current=[](void*p,const BattleSubmenuCameraProof&){return static_cast<Scope*>(p)->current();};b.camera.camera=8;b.camera.generation=1;b.camera.pass=1;
  b.camera.projection=cameraOrthographic(240,-240,-304,304,0,1000);b.camera.viewport={0,0,608,480,0,1};b.camera.scissor={0,0,608,480};
  b.gx=s->gx;b.pin=s->pin;
  b.word=[w](uint32_t a)->std::optional<uint32_t>{auto p=w.lock();if(!p||!p->current())return {};if(a>=CameraAddress+0x11c&&a<CameraAddress+0x14c&&!(a&3)){const auto i=(a-CameraAddress-0x11c)/4;return bits(i==0||i==5||i==10?1.f:0.f);}if(a==CameraAddress+0x114)return bits(0);uint32_t v;if(!p->process->process().vm().memory.readWord(a,v))return {};return v;};
  b.half=[w](uint32_t a)->std::optional<uint16_t>{auto p=w.lock();if(!p||!p->current())return {};uint16_t v;if(!p->process->process().vm().memory.readHalf(a,v))return {};return v;};
  b.cameraAddress=[w](std::optional<uint32_t>index)->std::optional<uint32_t>{auto p=w.lock();return p&&p->current()&&(!index||*index==8)?std::optional<uint32_t>(CameraAddress):std::nullopt;};
  b.matrix=[w](BattleAudienceMatrixRequest&q){auto p=w.lock();if(!p||!p->current())return false;if(p->reentry){assert(!p->child->current());assert(!p->child->resumeMatrix({}));}
   if(p->math==p->holdMath&&p->attempts++<2)return false;
   BattleAudienceAuxServices adapters;bindAudienceAuxMathAdapter(adapters);if(!adapters.matrix(nullptr,q))return false;++p->math;return true;};
  // TEST-only host lease metadata; no claim of actual C3D import/residency.
  // Production must use Renderer::enemyHpIconResidency's actual import token.
  b.residency=[w](uint32_t image){auto p=w.lock();return p?p->testResidency(image):std::nullopt;};
  b.append=[w](const RetainedIconPacket&q){auto p=w.lock();if(!p||!p->current())return false;if(p->reentry){assert(!p->child->current());assert(!p->child->resumeMatrix({}));}if(p->appends==p->holdAppend&&p->attempts++<2)return false;++p->appends;p->packets.push_back(q);return true;};
  b.retainOrdinary=[w](std::shared_ptr<const SourceGxInheritedSnapshot>q){auto p=w.lock();if(!p||!p->current()||!q||!q->current())return false;p->evidence.push_back(std::move(q));return true;};
  child=BattleWeaponSourceIcon::create(std::move(b));assert(child);s->child=child.get();request.function=0x8001a4bc;request.flags=16;request.icon=445;request.rgba=0xff8030ff;request.matrix={1,0,0,37,0,1,0,-12,0,0,1,0};
 }
 void run(){for(unsigned i=0;i<1000;++i)if(child->resumeMatrix(request))return;assert(false);}
};
void ordinary(){F f;f.run();assert(f.s->packets.size()==1&&f.s->packets[0].image==465&&f.s->packets[0].camera==8&&f.s->evidence.size()==1&&f.s->evidence[0]->stages()==1);auto p=f.s->packets[0];assert(p.material.mode==NativeIconPlan::Normal&&p.material.tint==f.request.rgba&&p.pipeline.color&&p.pipeline.alpha&&!p.pipeline.depth&&!p.pipeline.depthWrite);assert(!f.child->pendingImage());
 f.request.flags=48;f.request.icon=446;f.run();assert(f.s->packets.size()==2&&f.s->packets[1].image==466&&f.s->packets[1].material.mode==NativeIconPlan::Grayscale&&f.s->evidence.back()->stages()==3);
 f.request.flags=16;f.request.icon=504;f.run();assert(f.s->packets.size()==3&&f.s->packets[2].image==533&&f.s->packets[2].material.mode==NativeIconPlan::Normal);auto evidence=f.s->evidence;f.s->source=false;for(auto&q:evidence)assert(q->current());assert(f.child->current());f.s->icons->retire();for(auto&q:evidence)assert(q->current());assert(!f.child->current());f.s->ui->retire();for(auto&q:evidence)assert(!q->current());}
void holds(){F reference;reference.run();auto packet=reference.s->packets[0];for(unsigned i=0;i<reference.s->math;++i){F f;f.s->holdMath=i;f.s->reentry=true;f.run();assert(f.s->attempts==3&&f.s->math==reference.s->math&&f.s->packets.size()==1&&f.s->packets[0].image==packet.image&&f.s->packets[0].transform==packet.transform);}
 F f;f.s->resident=false;assert(!f.child->resumeMatrix(f.request)&&f.child->pending()==0x802b7880&&f.child->pendingImage()==465&&f.s->pin->retains(465)&&f.s->packets.empty());const auto math=f.s->math;assert(!f.child->resumeMatrix(f.request)&&f.s->math==math&&f.s->evidence.size()==1);auto changed=f.request;changed.rgba=0;assert(!f.child->resumeMatrix(changed));f.s->resident=true;f.run();assert(f.s->math==math&&f.s->packets.size()==1);
 F append;append.s->holdAppend=0;append.run();assert(append.s->attempts==3&&append.s->appends==1&&append.s->evidence.size()==1&&append.s->packets.size()==1);
}
void position(){F f;SourceIconPositionCursor c;SourceIconPositionRequest q;q.flags=16;q.icon=504;q.alpha=255;q.scale=.75f;q.position={19,42,0};for(unsigned i=0;i<1000;++i)if(resumeSourceIconPosition(q,f.child->matrixServices(),c))break;assert(!c.active&&f.s->packets.size()==1&&f.s->packets[0].image==533);}
void invalidAndRetirement(){F invalid;invalid.request.matrix[0]=std::numeric_limits<float>::infinity();assert(!invalid.child->resumeMatrix(invalid.request)&&invalid.s->packets.empty());F retired;retired.s->resident=false;assert(!retired.child->resumeMatrix(retired.request));retired.s->alive=false;assert(!retired.child->current());retired.s->alive=true;retired.s->resident=true;assert(!retired.child->resumeMatrix(retired.request)&&retired.s->packets.empty());}
void missingWorld(){F f;f.request.flags=0;assert(!f.child->resumeMatrix(f.request)&&f.child->pending()==0x800208c0&&f.s->packets.empty());auto prefix=f.s->math;assert(!f.child->resumeMatrix(f.request)&&f.s->math==prefix);f.s->alive=false;assert(!f.child->resumeMatrix(f.request)&&!f.child->current());}
}
void serialProfiles(){
 F f;const auto history=f.s->ui->gxHistory();auto evidence=BattleWeaponIconEvidence::create(history,64,f.s,[s=f.s]{return s->ui->current();});assert(evidence);
 for(const auto flags:{48u,16u,48u,16u}){
  f.request.flags=flags;f.run();const auto snapshot=f.s->evidence.back();assert(history->ownsSnapshot(snapshot));assert(evidence->retain(history,snapshot));
 }
 assert(f.s->packets.size()==4&&evidence->current());const auto unique=evidence->size();
 for(unsigned i=0;i<30;++i){f.request.flags=(i&1)?16:48;f.run();assert(evidence->retain(history,f.s->evidence.back()));}
 assert(evidence->size()==unique); // complete exact evidence stays bounded across source Loads
}
int main(){ordinary();holds();position();missingWorld();invalidAndRetirement();serialProfiles();std::cout<<"source icon TEX/GX packet leaf tests PASS (TEST camera/residency, no native queue/GPU claim)\n";}
