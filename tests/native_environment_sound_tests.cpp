// Actual private native-state fixture for the explicitly unsupported live path;
// no production import/reset API exists to invent active environment handles.
#include "../source/audio/native_environment_sound.cpp"
#define main nativeEnvironmentPositionFixtureMain
#include "native_field_mario_get_position_tests.cpp"
#undef main
#include <cstdio>
namespace ttyd {
struct NativeEnvironmentSoundFixture {
 static void live(NativeEnvironmentSound&s,uint16_t handle){s.state_->handles[0]=handle;}
};
}
namespace {
struct EnvironmentHost {
 NativeFacadeRegistration host;std::shared_ptr<NativeEnvironmentSound>owner;
 unsigned borrows=0,reads=0;bool known=true,recursive=false;std::optional<NativeEnvironmentColdProof>proof;
 EnvironmentHost(){NativeEnvironmentSoundBinding b;b.lifetime=std::make_shared<int>(1);b.host=host.proof();b.user=this;
  b.current=[](void*p){auto&h=*static_cast<EnvironmentHost*>(p);++h.borrows;return true;};
  b.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&h=*static_cast<EnvironmentHost*>(p);assert(a==0x80416520);++h.reads;
   if(h.recursive){NativeEnvironmentOffCursor nested;assert(!h.owner->off(*h.proof,0x200,nested));}
   return h.known?std::optional<uint32_t>{10}:std::nullopt;};
  owner=NativeEnvironmentSound::prepare(b);assert(owner);
 }
 void cold(){proof=owner->initializeNativeCold();assert(proof);}
};
void initializationAndIdentity(){EnvironmentHost a,b;NativeEnvironmentOffCursor c;assert(!a.owner->off(NativeEnvironmentColdProof{},0x200,c));assert(!a.owner->initializeNativeCold());b.cold();assert(!b.owner->initializeNativeCold());EnvironmentHost foreign;foreign.cold();assert(!b.owner->current(*foreign.proof));assert(b.owner->current(*b.proof));const auto before=b.borrows;b.host.retire();assert(!b.owner->off(*b.proof,0x200,c)&&b.borrows==before);}
void heldIdentity(){EnvironmentHost h;h.cold();NativeEnvironmentOffCursor c;assert(!h.owner->off(*h.proof,0x201,c)&&c.active());assert(!h.owner->off(*h.proof,0x200,c)&&!h.reads);}
void heldExactRead(){EnvironmentHost h;h.cold();h.known=false;NativeEnvironmentOffCursor cursor;assert(!h.owner->off(*h.proof,0x200,cursor)&&cursor.pending()==0x80416520&&h.reads==1);NativeEnvironmentSoundFixture::live(*h.owner,17);h.known=true;h.recursive=true;const auto zero=h.owner->off(*h.proof,0x200,cursor);assert(zero&&*zero==0&&h.reads==2&&!cursor.active());assert(h.owner->off(*h.proof,0x200,cursor)==0&&h.reads==2);NativeEnvironmentOffCursor live;assert(!h.owner->off(*h.proof,0x200,live)&&live.pending()==0x800d7470);const auto reads=h.reads;assert(!h.owner->off(*h.proof,0x200,live)&&h.reads==reads);h.owner->invalidate();assert(!h.owner->current(*h.proof)&&!h.owner->initializeNativeCold());}
void actualRawWrapper(){Fixture vm;EnvironmentHost h;h.cold();auto child=NativeFieldEnvironmentOff::create(*vm.vm,h.owner,*h.proof);assert(child);auto&e=*vm.entry;assert(vm.vm->memory.writeWord(e.arguments,0x200));h.known=false;const auto old=e.locals;assert(!child->invoke(NativeEnvironmentOffEventFunction,e,true)&&e.locals==old);assert(vm.vm->memory.writeWord(e.arguments,0x201));h.known=true;assert(child->invoke(NativeEnvironmentOffEventFunction,e,false)==2&&e.locals==old);assert(!child->invoke(NativeEnvironmentOffEventFunction,e,true));child->retire(e);assert(!child->pending());
 assert(vm.vm->memory.writeWord(e.arguments,0x200));h.known=false;assert(!child->invoke(NativeEnvironmentOffEventFunction,e,true));const auto reads=h.reads;++e.currentCommand;h.known=true;assert(!child->invoke(NativeEnvironmentOffEventFunction,e,false)&&h.reads==reads);child->retire(e);}
}
int main(){initializationAndIdentity();heldIdentity();heldExactRead();actualRawWrapper();std::puts("Native cold environment seal, exact held handle/table order, raw wrapper/no-value-resolution and live/stale holds pass");return 0;}
