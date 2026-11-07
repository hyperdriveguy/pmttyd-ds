#include "battle_weapon_source_memory.hpp"
#include "source_font_state.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
namespace {
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float v){assert(std::isfinite(v)&&double(v)>=-2147483648.&&double(v)<2147483648.);return int32_t(v);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};return p;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>startup;
 std::shared_ptr<EventGpLanguageAlias>alias;
 std::shared_ptr<EventNativeRuntime>vm=std::make_shared<EventNativeRuntime>(0x81100000,platform());
 std::shared_ptr<BattleWeaponSourceMemory>memory;bool live=true,reenter=false;uint32_t sourceGp=0;
 Fixture(){process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());startup=NativeUiStartup::create(process,"romfs");assert(startup&&startup->resumeFont());assert(process->process().vm().memory.readWord(0x80415fd0,sourceGp));
  alias=std::make_shared<EventGpLanguageAlias>(process->process().vm().gpAddresses,sourceGp,startup->lifetime(),[u=startup]{return u->current();});
  memory=BattleWeaponSourceMemory::create(*vm,startup,alias,vm,[this]{if(reenter&&memory){assert(!memory->current()&&!memory->word(0x8041e978)&&!memory->store(0x8041e9ac,4,0));}return live;},"romfs/source-ui-startup/memory.pmevt");assert(memory);
 }
};
void actualOwnership(){Fixture f;f.reenter=true;assert(f.memory->current());const auto font=f.startup->fontMemory();const auto object=f.startup->fontObject();assert(font&&object);
 uint32_t absent;assert(!f.vm->memory.readWord(0x8041e978,absent)); // no injected state/VM receipt.
 assert(f.memory->word(0x8041e978)==object->font&&f.memory->word(object->font)==0x464f4e54);
 const auto old=f.memory->word(0x8041e9ac);assert(old&&f.memory->store(0x8041e9ac,4,0x10203040)&&f.startup->word(0x8041e9ac)==0x10203040);
 assert(f.process->process().vm().memory.readWord(0x8041e9ac,absent)&&absent==0x10203040);assert(f.memory->store(0x8041e9ac,4,*old));
 // These are real source bytes from the pinned immutable data image, never
 // a seeded VM mapping or a native ready flag.
 const auto literal=f.memory->word(0x802c2b6c);assert(literal&&f.memory->word(0x804203e4));assert(f.memory->word(0x8041f494)==f.startup->sourceResourceWord(0x8041f494)&&f.memory->word(0x8041f494));
 assert(f.vm->memory.registerRam(0x802c2b6c,{0xab}));assert(f.memory->word(0x802c2b6c)==literal);assert(f.vm->memory.unregisterRam(0x802c2b6c));
 assert(f.vm->memory.registerRam(0x71000000,{0xab}));assert(f.memory->byte(0x71000000)==0xab&&!f.memory->half(0x71000000)&&!f.memory->word(0x71000000));
 // Actual unknown font capacity owns routing even when an unrelated VM RAM
 // alias could otherwise provide apparently readable zero/initialized bytes.
 auto backing=std::make_shared<std::vector<uint8_t>>(32,0xff);assert(font->adoptAllocation(0x70000000,backing->data(),32,backing));assert(f.vm->memory.registerRam(0x70000000,std::vector<uint8_t>(32,0x5a)));
 assert(!f.memory->byte(0x70000000)&&!f.memory->half(0x6fffffff)&&!f.memory->word(0x70000000)&&!f.memory->store(0x70000000,1,2));
 uint8_t original=0;assert(f.vm->memory.readByte(0x70000000,original)&&original==0x5a);
 assert(!f.memory->word(0xfffffffe)&&!f.memory->store(0xffffffff,2,0));
 // Language mutations are live both directions and preserve all other GP
 // storage. A new wrapper reuses only this exact actual installed alias.
 assert(f.vm->memory.writeWord(0x8110016c,2)&&f.startup->language()==2);assert(f.process->process().vm().memory.writeWord(f.sourceGp+0x16c,3)&&f.memory->word(0x8110016c)==3);
 auto again=BattleWeaponSourceMemory::create(*f.vm,f.startup,f.alias,f.vm,[&]{return f.live;},"romfs/source-ui-startup/memory.pmevt");assert(again&&again->word(0x8110016c)==3);
 auto other=std::make_shared<EventGpLanguageAlias>(f.process->process().vm().gpAddresses,f.sourceGp,f.startup->lifetime(),[u=f.startup]{return u->current();});assert(!BattleWeaponSourceMemory::create(*f.vm,f.startup,other,f.vm,[&]{return f.live;},"romfs/source-ui-startup/memory.pmevt"));
 assert(f.memory->store(0x8041e9b0,2,0x1234)&&font->half(0x8041e9b0)==0x1234);
 f.live=false;assert(!f.memory->word(0x8041e978));f.live=true;assert(!f.memory->current()&&!f.memory->word(0x8041e978));
}
void actualSharedWindowOwnership(){Fixture f;
 assert(f.startup->beginWindow()&&!f.startup->windowReady());
 // The actual reached original Setup has produced Ready0 and the source heap.
 // Unrelated destination VM aliases must not hide that shared source history.
 assert(f.vm->memory.registerRam(0x8041ea1c,{0x5a,0x5a,0x5a,0x5a}));
 assert(f.memory->owns(0x8041ea1c,4)&&f.memory->word(0x8041ea1c)==0);
 const auto palette=f.startup->sourceResourceWord(0x8041ea18);assert(palette&&*palette);
 assert(f.vm->memory.registerRam(*palette,std::vector<uint8_t>(4,0xa5)));assert(f.memory->word(*palette)==0);
 assert(f.startup->pumpWindow()&&f.startup->windowReady());
 assert(f.memory->word(0x8041ea1c)==1&&f.memory->word(*palette)==0x0020af30);
 const auto object=f.startup->windowObject(0);assert(object&&f.memory->word(object->address+20)==14);
 uint32_t unrelated=0;assert(f.vm->memory.readWord(0x8041ea1c,unrelated)&&unrelated==0x5a5a5a5a);
 // Indexed currentness follows actual object bytes rather than Ready alone.
 const auto old=f.memory->word(object->address);assert(old&&f.startup->windowMemory()->storeWord(object->address,*old^1));
 assert(f.startup->windowReady()&&!f.startup->windowObject(0));
 assert(f.startup->windowMemory()->storeWord(object->address,*old)&&f.startup->windowObject(0));
 f.startup->retire();assert(f.memory->owns(0x8041ea1c,4)&&!f.memory->word(0x8041ea1c)&&!f.memory->word(*palette));
}
void retainedOriginalFontPrefix(){Fixture f;auto a=f.memory->actors();SourceFontStateServices s;s.actors=a;s.user=f.memory.get();s.lifetime=f.memory;s.current=[](void*p){return static_cast<BattleWeaponSourceMemory*>(p)->current();};
 uint32_t color=0x11223344;SourceFontStateInput input;input.function=0x800777e0;input.privateColor=&color;SourceFontStateCursor c;
 assert(!resumeSourceFontState(input,s,c)&&c.pending()==0x802b8ab8);assert(f.startup->word(0x8041e9ac)==0x11223344);
 // Prefix stores completed before the actual missing GX material child.
 // A retry resumes there; it must not overwrite later real shared mutations.
 assert(f.memory->store(0x8041e9ac,4,0x55667788));assert(!resumeSourceFontState(input,s,c)&&f.startup->word(0x8041e9ac)==0x55667788);
 f.alias->retire();assert(!f.memory->word(0x8110016c)&&!f.memory->current()&&!resumeSourceFontState(input,s,c));
}
}
int main(){actualOwnership();actualSharedWindowOwnership();retainedOriginalFontPrefix();return 0;}
