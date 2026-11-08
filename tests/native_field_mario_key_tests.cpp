#define main nativeKeyPositionFixtureMain
#include "native_field_mario_get_position_tests.cpp"
#undef main
#include "native_field_mario_key.hpp"
namespace {
struct KeyBacking {
 FieldMarioBattleState mario;const EventNativeRuntime*vm=nullptr;
 bool admitted=true,locked=false;unsigned borrows=0,stores=0,lookups=0;
 NativeFieldMarioKey*reenter=nullptr;EventEntry*entry=nullptr;
 FieldMarioPreparationWorld world(){FieldMarioPreparationWorld w;w.storage.user=this;
  w.admit=[](void*p,uint32_t fn){auto&b=*static_cast<KeyBacking*>(p);assert(fn==NativeFieldMarioKeyFunction);return b.admitted;};
  w.storage.mario=[](void*p)->uintptr_t{auto&b=*static_cast<KeyBacking*>(p);++b.lookups;return FieldMarioOwnerAddress;};
  w.storage.read=[](void*p,uintptr_t a){assert(a==FieldMarioOwnerAddress);return static_cast<KeyBacking*>(p)->mario.flags;};
  w.storage.write=[](void*p,uintptr_t a,uint32_t v){auto&b=*static_cast<KeyBacking*>(p);assert(a==FieldMarioOwnerAddress);++b.stores;b.mario.flags=v;b.locked=(v&8)!=0;if(b.reenter)assert(!b.reenter->invoke(NativeFieldMarioKeyFunction,*b.entry,false));};
  w.byte=[](void*p,uintptr_t a){assert(a==FieldMarioOwnerAddress+0x39);return static_cast<KeyBacking*>(p)->mario.keyDepth;};
  w.storeByte=[](void*p,uintptr_t a,uint8_t v){auto&b=*static_cast<KeyBacking*>(p);assert(a==FieldMarioOwnerAddress+0x39);++b.stores;b.mario.keyDepth=v;};return w;
 }
};
struct Keys {
 Fixture fixture;NativeFacadeRegistration host;std::shared_ptr<KeyBacking>backing=std::make_shared<KeyBacking>();std::unique_ptr<NativeFieldMarioKey>key;
 Keys(){backing->vm=fixture.vm.get();backing->entry=fixture.entry;fixture.entry->argumentCount=1;NativeFieldMarioKeyBinding b;b.lifetime=backing;b.host=host.proof();b.user=backing.get();b.world=backing->world();b.current=[](void*p,const EventNativeRuntime&v){auto&b=*static_cast<KeyBacking*>(p);++b.borrows;return b.vm==&v;};key=NativeFieldMarioKey::create(*fixture.vm,b);assert(key);}
 std::optional<int32_t>call(uint32_t raw){assert(fixture.vm->memory.writeWord(fixture.entry->arguments,raw));return key->invoke(NativeFieldMarioKeyFunction,*fixture.entry,true);}
};
void sourceState(){Keys f;f.backing->mario.flags=0x20;f.backing->mario.keyDepth=1;f.backing->reenter=f.key.get();assert(f.call(0)==2&&f.backing->mario.keyDepth==2&&f.backing->mario.flags==0x28&&f.backing->locked);assert(f.call(1)==2&&f.backing->mario.keyDepth==1&&f.backing->locked);assert(f.call(1)==2&&f.backing->mario.keyDepth==0&&f.backing->mario.flags==0x20&&!f.backing->locked);assert(f.call(1)==2&&f.backing->mario.keyDepth==255);assert(f.call(0)==2&&f.backing->mario.keyDepth==0&&f.backing->mario.flags==0x28);}
void admissionAndLifetime(){Keys f;f.backing->admitted=false;assert(!f.call(0)&&!f.backing->stores&&!f.backing->lookups);f.backing->admitted=true;assert(!f.call(uint32_t(-30000000))&&!f.backing->stores);const auto before=f.backing->borrows;f.host.retire();assert(!f.call(0)&&f.backing->borrows==before);Keys other;auto retained=std::move(other.key);other.fixture.getter.reset();other.fixture.vm.reset();const auto borrowed=other.backing->borrows;assert(!retained->current()&&other.backing->borrows==borrowed);}
}
int main(){sourceState();admissionAndLifetime();std::puts("Actual typed Mario key depth/flags, nested ownership, underflow/wrap, admission/reentry/lifetime checks pass");return 0;}
