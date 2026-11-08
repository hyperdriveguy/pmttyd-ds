// TEST prefix: actual Entry/resource constructors and real event contexts are
// used; setting the caller's operand cursor below does not execute dialogue.
#define main nativeHarborSailorFixtureMain
#include "native_harbor_sailors_tests.cpp"
#undef main
#ifndef HARBOR_SEQUENCE_IMPLEMENTATION
#define HARBOR_SEQUENCE_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_SEQUENCE_IMPLEMENTATION
#include "native_harbor_field_sequence.hpp"
#include "native_field_mario_battle_transfer.hpp"
#include "source_sound_flags.hpp"
namespace ttyd {
struct NativeHarborCompletionFixture {
 static void admitTestPrefix(HarborTutorial&t){t.initialized=true;}
 static std::shared_ptr<NativeHarborCompletion> completeTestPrefix(HarborTutorial&t){
  // TEST source conclusion domain, not an executed fatal effect/playthrough.
  const auto r=t.runtime_;const auto enemy=r->packed->unitAddress(2);assert(enemy&&r->vm.memory.writeByte(enemy+0x135,1));
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.message=MessagePlayback{};t.pendingService=0;
  auto completion=t.beginNativeCompletion(1);assert(completion);
  NativeHarborCompletionServices s;s.lifetime=std::make_shared<int>(1);s.current=[](void*){return true;};s.retirePresentation=[](void*){return true;};s.retireMusic=[](void*){return true;};
  assert(completion->resume(s)&&completion->receipt());return completion;
 }
};
}
namespace {
struct Backend {
 NativeFacadeRegistration host,facade;
 FieldMarioBattleState mario;FieldPartyRegistry parties;MarioPresentation presentation;FieldMovement movement;
 std::shared_ptr<const void> lifetime=std::make_shared<int>(1);
 std::unique_ptr<NativeFieldMarioBattleTransfer> transfer;
 SourceSoundFlags sound;SoundFlagCursor flagCursor;
 bool held=true,bgmHeld=true;unsigned prepares=0,posts=0,bgmCalls=0;std::vector<uint32_t> accepted;
 std::weak_ptr<NativeHarborFieldSequence> sequence;EventEntry* recursive=nullptr;
 Backend(){NativeFieldMarioBattleTransferBinding b;b.lifetime=lifetime;b.host=host.proof();b.facade=facade.proof();b.user=this;b.current=[](void*){return true;};b.mario=&mario;b.parties=&parties;b.presentation=&presentation;b.movement=&movement;
  b.motion=[](void*)->std::optional<uint16_t>{return 0;};b.position=[](void*,unsigned i)->std::optional<uint32_t>{return 100+i;};b.storePosition=[](void*,unsigned,uint32_t){assert(false);return false;};transfer=NativeFieldMarioBattleTransfer::create(b);assert(transfer);}
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;s.call=[](void*p,BattleSequenceCall&q){auto&w=*static_cast<Backend*>(p);
  if(q.function==MarioFieldBattlePrepare&&w.held)return false;
  if(w.recursive){const auto parent=w.sequence.lock();assert(parent&&!parent->invokeSource(0x8004ef54,*w.recursive,true));}
  if(q.function==0x800dbcc4){if(w.bgmHeld)return false;++w.bgmCalls;w.accepted.push_back(q.function);return true;} // TEST real-audio boundary, not a production BGM acknowledgment.
  if(q.function==0x800dc110){if(!w.sound.set(q.args[0],{},w.flagCursor))return false;w.accepted.push_back(q.function);return true;}
  if(!w.transfer->call(q))return false;
  if(q.function==MarioFieldBattlePrepare)++w.prepares;
  if(q.function==MarioFieldBattlePost)++w.posts;
  w.accepted.push_back(q.function);return true;
 };return s;}
};
uint32_t operand(World&w,uint32_t function){for(uint32_t a=0x90506314;a<0x90506b44;a+=4)if(w.word(a)==function)return a+4;assert(false);return 0;}
void test(const char*tags){World w(tags);for(unsigned i=0;i<10000&&!w.sailors->ready();++i){w.animation->files()->advanceAsync(4096);w.sailors->advance();}assert(w.sailors->ready());
 // Execute the original four-actor Entry child with real retained files/poses.
 auto&constructor=createRootEvent(w.vm.schedule,0x90505208,0,0,w.vm.creationServices());w.cutsceneRoot=constructor.id;
 for(unsigned i=0;i<10000&&!w.returned;++i){w.creation->pump();if(w.creation->pending()&&!w.creation->resume())continue;w.vm.gp.timestamp=uint64_t(i)*17*eventClockRate(243000000);w.vm.runManager();}assert(w.returned);
 NativeFacadeRegistration context;Backend backend;auto sequence=NativeHarborFieldSequence::prepare(w.vm,w.owner,context.proof());assert(sequence&&sequence->attach(*w.setup,backend.services(),w.sailors->regularHistory()));
 auto&e=createRootEvent(w.vm.schedule,0x90506314,10,0,w.vm.creationServices());e.arguments=operand(w,0x8004ef54);
 auto&foreign=createRootEvent(w.vm.schedule,0x90506314,10,0,w.vm.creationServices());foreign.arguments=e.arguments;
 backend.sequence=sequence;backend.recursive=&e;
 assert(!sequence->request());assert(!sequence->invokeSource(0x8004ef54,e,true));assert(sequence->pending()==MarioFieldBattlePrepare&&!sequence->request());
 assert(!sequence->invokeSource(0x8004ef54,foreign,true));backend.held=false;
 assert(sequence->invokeSource(0x8004ef54,e,false)==0&&backend.prepares==1&&backend.mario.controlDepth==1&&backend.sound.value()==0x10);
 auto ticket=sequence->request();assert(ticket);const auto copy=*ticket;w.vm.schedule.managerActive=true;assert(!sequence->request());w.vm.schedule.managerActive=false;
 assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial tutorial;auto pouch=initializeProloguePouch("Yoshi");assert(tutorial.beginFromPouch(0,pouch,4));NativeHarborCompletionFixture::admitTestPrefix(tutorial);
 auto identity=tutorial.nativeFieldBattleIdentity();assert(identity&&sequence->entered(copy,*identity)&&sequence->entered(*ticket,*identity));
 NativeHarborBattleTicket absent;assert(!sequence->entered(absent,*identity));
 const auto actualField=sequence->endingFieldWork(copy);assert(actualField&&*actualField==w.owner->fieldWorkAddress());
 assert(!sequence->endingFieldWork(absent));
 const auto actualNpc=sequence->endingFieldWord(copy,*actualField+8);assert(actualNpc&&*actualNpc);
 const auto flagsAddress=*actualNpc+0x140;const auto flagsBefore=w.word(flagsAddress);
 assert(!sequence->endingFieldStore(copy,flagsAddress,flagsBefore|0x100));
 assert(!sequence->endingFieldWord(copy,flagsAddress+4));
 assert(sequence->endingFieldWord(copy,flagsAddress)==flagsBefore);
 // Original inline OR uses its captured load even across a held service. An
 // unrelated later mutation must not replace that already consumed source word.
 assert(w.vm.memory.writeWord(flagsAddress,flagsBefore^0x200));
 assert(!sequence->endingFieldStore(absent,flagsAddress,flagsBefore|0x100));
 assert(!sequence->endingFieldStore(copy,flagsAddress,(flagsBefore|0x100)^1));
 assert(sequence->endingFieldStore(copy,flagsAddress,flagsBefore|0x100));
 assert(sequence->endingFieldStore(copy,flagsAddress,flagsBefore|0x100)&&w.word(flagsAddress)==(flagsBefore|0x100));
 assert(sequence->invokeSource(0x8004eed8,e,true)==0&&backend.bgmCalls==0&&backend.mario.controlDepth==1);
 NativeHarborCompletionReceipt receipt;receipt.result=1;receipt.pouch=pouch;receipt.battleLifetime=identity->lifetime();
 assert(!sequence->returned(copy,receipt)); // Public fields alone prove no teardown.
 NativeHarborCompletionReceipt forged=receipt;forged.battleLifetime=std::shared_ptr<const void>(receipt.battleLifetime.get(),[](const void*){});assert(!sequence->returned(copy,forged));
 const auto completion=NativeHarborCompletionFixture::completeTestPrefix(tutorial);receipt=*completion->receipt();
 auto changedPouch=receipt;changedPouch.pouch.bytes[0]^=1;assert(!sequence->returned(copy,changedPouch));
 receipt.result=0;assert(!sequence->returned(copy,receipt));receipt.result=1;assert(sequence->returned(copy,receipt)&&sequence->returned(copy,receipt));
 assert(w.word(w.owner->fieldWorkAddress()+0x30)==1);
 assert(!sequence->endingFieldWork(copy)&&!sequence->endingFieldStore(copy,flagsAddress,flagsBefore|0x100));
 assert(!sequence->invokeSource(0x8004eed8,e,false)&&sequence->pending()==0x800dbcc4&&backend.mario.controlDepth==0);const auto prior=backend.accepted;
 assert(!sequence->invokeSource(0x8004eed8,e,false)&&backend.accepted==prior);backend.bgmHeld=false;
 assert(sequence->invokeSource(0x8004eed8,e,false)==2&&backend.posts==1&&backend.bgmCalls==1&&backend.mario.controlDepth==0&&!(backend.mario.flags&0x1002));
 const auto old=backend.accepted;context.retire();assert(!sequence->current()&&!sequence->invokeSource(0x8004eed8,e,false)&&backend.accepted==old);
 tutorial=HarborTutorial{};assert(!identity->current());
}
void vmLifetime(const char*tags){std::shared_ptr<NativeHarborFieldSequence> retained;NativeFacadeRegistration context;Backend backend;
 {World w(tags);retained=NativeHarborFieldSequence::prepare(w.vm,w.owner,context.proof());assert(retained&&retained->attach(*w.setup,backend.services(),w.sailors->regularHistory()));}
 // Intrinsic VM registration rejects before touching the retained binding's
 // borrowed VM/setup. The native registry owner may still be strongly pinned.
 assert(!retained->current()&&!retained->request());
}
}
int main(int argc,char**argv){assert(argc==2);test(argv[1]);vmLifetime(argv[1]);std::puts("Actual same-VM source start/wait, sealed native entry/return, unchanged result and retained control/Post pass");return 0;}
