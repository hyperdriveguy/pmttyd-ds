#include "native_field_mario_battle_transfer.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace ttyd;
namespace {
uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
struct Host {
 NativeFacadeRegistration registration,facade;
 FieldMarioBattleState mario;FieldPartyRegistry parties;
 MarioPresentation presentation;FieldMovement movement;
 uint16_t motion=0;std::array<uint32_t,3> xyz{{bits(12),bits(15),bits(-20)}};
 std::shared_ptr<const void> lifetime=std::make_shared<int>(1);
 unsigned reads=0,writes=0,borrows=0;bool alive=true,holdPosition=false;
 NativeFieldMarioBattleTransferBinding binding(){NativeFieldMarioBattleTransferBinding b;
  b.lifetime=lifetime;b.host=registration.proof();b.facade=facade.proof();b.user=this;
  b.current=[](void*p){auto&w=*static_cast<Host*>(p);++w.borrows;return w.alive;};
  b.mario=&mario;b.parties=&parties;b.presentation=&presentation;b.movement=&movement;
  b.motion=[](void*p)->std::optional<uint16_t>{return static_cast<Host*>(p)->motion;};
  b.position=[](void*p,unsigned i)->std::optional<uint32_t>{auto&w=*static_cast<Host*>(p);++w.reads;if(w.holdPosition)return {};assert(i<3);return w.xyz[i];};
  b.storePosition=[](void*p,unsigned i,uint32_t v){auto&w=*static_cast<Host*>(p);assert(i<3);w.xyz[i]=v;++w.writes;return true;};return b;
 }
};
void nullParty(){Host h;h.presentation.display=100;h.presentation.offset=10;h.mario.flags=0x80000000;
 auto owner=NativeFieldMarioBattleTransfer::create(h.binding());assert(owner);
 BattleSequenceCall call;call.function=MarioFieldBattlePrepare;assert(owner->call(call));assert(h.reads==3&&h.presentation.display==180&&h.presentation.target==180&&h.presentation.facing==100);
 assert((h.mario.flags&0x80001000)==0x80001000&&h.mario.controlDepth==0);
 call.function=0x8005c2c4;assert(owner->call(call)&&h.mario.controlDepth==1&&call.result==1);
 call.function=0x80141050;assert(owner->call(call)&&call.result==0xffffffff);
 call.function=0x800bf1b4;call.args[0]=call.result;assert(owner->call(call)&&call.result==0);
 call.function=0x8005c290;assert(owner->call(call)&&h.mario.controlDepth==0&&!(h.mario.flags&2));
 h.xyz={{bits(20),bits(30),bits(40)}};call.function=MarioFieldBattlePost;assert(owner->call(call));assert(!(h.mario.flags&0x1000)&&h.writes==0&&h.xyz[0]==bits(20));
}
void realPartySlots(){Host h;for(unsigned i=0;i<2;++i){h.parties.actors[i]=std::make_unique<FieldPartyActor>();h.parties.actors[i]->word(0,i?0x101:0x201);}h.mario.partyIndices={{255,255}};
 auto owner=NativeFieldMarioBattleTransfer::create(h.binding());assert(owner);BattleSequenceCall q;q.function=MarioFieldBattlePrepare;assert(owner->call(q));
 assert(h.parties.actors[0]->word(0)==0x20000201&&h.parties.actors[1]->word(0)==0x20000101);
 q.function=MarioFieldBattlePost;assert(owner->call(q));assert(h.parties.actors[0]->word(0)==0x201&&h.parties.actors[1]->word(0)==0x101);
 // A real nonnull current party reaches actual appearance, not a default XYZ.
 Host active;active.parties.actors[0]=std::make_unique<FieldPartyActor>();active.mario.partyIndices={{0,255}};
 auto held=NativeFieldMarioBattleTransfer::create(active.binding());q.function=MarioFieldBattlePrepare;assert(held->call(q));q.function=MarioFieldBattlePost;assert(!held->call(q)&&held->pending()==0x800c6c9c);
}
void restoreHold(){Host h;auto owner=NativeFieldMarioBattleTransfer::create(h.binding());assert(owner);BattleSequenceCall q;q.function=MarioFieldBattlePrepare;assert(owner->call(q));
 h.motion=20;h.xyz={{1,2,3}};q.function=MarioFieldBattlePost;assert(!owner->call(q)&&owner->pending()==0x8009455c);assert(h.xyz[0]==bits(12)&&h.xyz[1]==bits(15)&&h.xyz[2]==bits(-20)&&h.writes==3);
 assert(!owner->call(q)&&h.writes==3); // Source prefix is never replayed.
}
void ownerLifetime(){Host h;auto owner=NativeFieldMarioBattleTransfer::create(h.binding());assert(owner);const auto borrowed=h.borrows;h.facade.retire();BattleSequenceCall q;q.function=MarioFieldBattlePrepare;assert(!owner->call(q)&&h.borrows==borrowed&&h.reads==0);
 Host original;auto binding=original.binding();NativeFacadeRegistration copiedHost=original.registration;assert(copiedHost.proof().current());original.registration.retire();const auto before=original.borrows;assert(!NativeFieldMarioBattleTransfer::create(binding)&&original.borrows==before);
 std::unique_ptr<NativeFieldMarioBattleTransfer> retained;{auto dying=std::make_unique<Host>();retained=NativeFieldMarioBattleTransfer::create(dying->binding());assert(retained);}assert(!retained->call(q));
 Host nan;nan.presentation.offset=std::numeric_limits<float>::max();auto finite=NativeFieldMarioBattleTransfer::create(nan.binding());assert(!finite->call(q)&&finite->pending()==0x800965fc&&nan.presentation.display==0);
 Host special;special.mario.flags=0x100000;auto s=NativeFieldMarioBattleTransfer::create(special.binding());assert(!s->call(q)&&s->pending()==0x800965fc);
}
}
int main(){nullParty();realPartySlots();restoreHold();ownerLifetime();std::puts("Actual typed Mario/party effects, null-party preparation/return, special holds and proof-before-borrow pass");return 0;}
