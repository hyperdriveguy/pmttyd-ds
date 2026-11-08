#include <memory>
#include "battle_weapon_icon_textures.hpp"
#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t PrivateObject=0x78010000;
const char*startup(){const auto*p=std::getenv("TTYD_ICON_STARTUP_PACK");return p?p:"romfs/source-icon-startup/startup.bin";}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double x){volatile float v=float(x);return v;};return p;}
struct F {
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>ui;std::shared_ptr<NativeSourceIconProcess>icons;
 struct Scope {std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>ui;BattleWeaponIconTextures*owner=nullptr;uint32_t palette=0;bool source=true,longLive=true,reenter=false;unsigned truncations=0,attempts=0,hold=UINT32_MAX;};
 std::shared_ptr<Scope>scope=std::make_shared<Scope>();
 std::unique_ptr<BattleWeaponIconTextures>textures;
 F(){process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());ui=NativeUiStartup::create(process,"romfs");assert(ui&&ui->resumeWindow());icons=NativeSourceIconProcess::create(process,"romfs/source-icons",startup());assert(icons&&icons->pump());
  scope->process=process;scope->ui=ui;scope->palette=palette();
  BattleWeaponIconTextureBinding b;b.process=icons;b.lifetime=scope;b.privateAddress=PrivateObject;b.longCurrent=[s=scope]{return s->longLive;};b.sourceCurrent=[s=scope]{return s->source;};
  b.unmapped=[s=scope](uint32_t a,uint32_t n){return s->process->process().vm().memory.unmappedRange(a,n);};
  b.truncate=[s=scope](float v)->std::optional<int32_t>{if(s->reenter){assert(!s->owner->current());assert(!s->owner->construct(s->palette,465));}
   if(s->truncations==s->hold&&s->attempts++<2)return {};
   ++s->truncations;auto&vm=s->process->process().vm();return vm.platform.truncate(vm.platform.user,v);};
  textures=BattleWeaponIconTextures::create(std::move(b));assert(textures);scope->owner=textures.get();
 }
 uint32_t palette(){auto v=icons->word(0x8041e630);assert(v);return *v;}
 uint32_t construct(uint32_t id){for(unsigned i=0;i<1000;++i){auto result=textures->construct(palette(),id);if(result)return *result;}assert(false);return 0;}
 SourceIconGXCall lod(uint32_t handle){SourceIconGXCall q;q.function=0x802b7544;q.integers={handle,1,1,0,0,0};return q;}
 void override(uint32_t handle){auto q=lod(handle);for(unsigned i=0;i<1000;++i)if(textures->lod(q))return;assert(false);}
};
void actual(){F f;std::vector<BattleSubmenuTextureObject>old;
 for(auto id:{465u,466u,533u,0u,600u}){
  const auto token=f.construct(id);assert(token==PrivateObject&&token!=id+1);assert(!f.textures->snapshot(token));f.override(token);
  auto q=f.textures->snapshot(token);assert(q&&q->handle==PrivateObject&&q->bytes&&q->image.index==id&&q->lifetime&&q->current(q->user));
  assert(f.textures->dimension(0x802b76c4,token)==q->image.width&&f.textures->dimension(0x802b76d4,token)==q->image.height);
  assert(((*q->bytes)[2]&1023u)+1==q->image.width&&(((*q->bytes)[2]>>10)&1023u)+1==q->image.height);
  assert(!f.textures->construct(f.palette(),id+1)&&!f.textures->construct(f.palette()+4,id));assert(!f.textures->dimension(0x802b76c4,id+1));
  assert(f.textures->finish(token));assert(!f.textures->snapshot(token));old.push_back(*q);
 }
 // Loaded snapshots are immutable independent of the temporary32B object,
 // subsequent source setters/reuse and short source/camera availability.
 f.scope->source=false;for(auto&q:old)assert(q.current(q.user));assert(!f.textures->construct(f.palette(),465));
 f.icons->retire();for(auto&q:old)assert(!q.current(q.user));
}
void waits(){F baseline;baseline.construct(465);baseline.override(PrivateObject);auto result=baseline.textures->snapshot(PrivateObject);assert(result);const auto calls=baseline.scope->truncations;assert(calls>0);
 for(unsigned at=0;at<calls;++at){F f;f.scope->hold=at;f.scope->reenter=true;f.construct(465);f.override(PrivateObject);auto q=f.textures->snapshot(PrivateObject);assert(q&&q->bytes==result->bytes&&q->image.width==result->image.width&&q->image.format==result->image.format&&f.scope->attempts==3&&f.scope->truncations==calls);}
 // A genuinely missing original SDK constant holds a partially written object;
 // restoring those exact source bytes resumes instead of rerunning Init.
 F f;auto&vm=f.process->process().vm();std::vector<uint8_t>bytes;assert(vm.memory.extractRam(0x80429454,4,bytes));assert(!f.textures->construct(f.palette(),465));auto pending=f.textures->pending();assert(pending);assert(!f.textures->construct(f.palette(),466));assert(vm.memory.registerRam(0x80429454,bytes));f.construct(465);f.override(PrivateObject);auto q=f.textures->snapshot(PrivateObject);assert(q&&q->bytes==result->bytes);
}
void rejection(){F f;assert(!f.textures->construct(f.palette(),613));assert(!f.textures->construct(f.palette()+4,465));f.construct(465);auto q=f.lod(PrivateObject);q.integers[1]=2;assert(f.textures->lod(q)); // Actual SDK filter domain, no guessed tuple substitution.
 auto changed=q;changed.integers[1]=1;assert(!f.textures->lod(changed));auto loaded=f.textures->snapshot(PrivateObject);assert(loaded&&loaded->current(loaded->user));f.scope->longLive=false;assert(!f.textures->current()&&!loaded->current(loaded->user));f.scope->longLive=true;assert(!f.textures->current()&&!loaded->current(loaded->user));}
}
int main(){actual();waits();rejection();std::cout<<"actual icon TEX/SDK private-object tests PASS\n";}
