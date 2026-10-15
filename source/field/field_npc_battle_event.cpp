#include "field_npc_battle_event.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
bool fieldNpcBattleEventFunction(uint32_t f){return f==0x8004ef54 || f==0x8004eed8 || f==0x8004f894;}
bool fieldNpcBattleEvent(uint32_t f,uint32_t entry,bool first,const BattleSequenceEntryServices& s,FieldNpcBattleEventCursor& c,int32_t& result){
 if(!fieldNpcBattleEventFunction(f))return false;
 if(!c.active){c={};c.active=true;c.function=f;c.entry=entry;c.first=first;c.owner=s.user;c.phase=f==0x8004eed8?20:0;}
 if(c.function!=f || c.entry!=entry || c.owner!=s.user)return false;
 auto load=[&](uint32_t a,uint32_t& v){c.pending=f;auto n=s.word?s.word(s.user,a):std::nullopt;if(!n)return false;v=*n;return true;};
 auto store=[&](uint32_t a,uint32_t v){c.pending=f;return s.store && s.store(s.user,a,v);};
 auto call=[&](uint32_t fn,std::array<uint32_t,5> a=std::array<uint32_t,5>{}){
  if(!c.queued){c.child={};c.child.function=fn;c.child.args=a;c.queued=true;}
  c.pending=fn;if(!s.call || !s.call(s.user,c.child))return false;c.queued=false;return true;
 };
 auto done=[&](int32_t v){result=v;c.active=false;c.pending=0;return true;};
 for(;;)switch(c.phase){
 case 0:if(!load(entry+0x18,c.args))return false;++c.phase;break;
 case 1:if(!load(c.args,c.raw))return false;++c.phase;break;
 case 2:if(!call(0x80031260,{entry,c.raw}))return false;c.name=c.child.result;c.phase=f==0x8004f894?30:3;break;
 // The second read deliberately crosses the original one-operand start record
 // into the following command header. Its resolved value is discarded.
 case 3:if(!load(c.args+4,c.raw))return false;++c.phase;break;
 case 4:if(!call(0x80031260,{entry,c.raw}))return false;++c.phase;break;
 case 5:if(!call(0x8004829c))return false;c.field=c.child.result;++c.phase;break;
 case 6:if(!call(0x8004f5d8,{entry,c.name}))return false;c.npc=c.child.result;if(!c.first)return done(2);++c.phase;break;
 case 7:if(!store(c.field+8,c.npc))return false;++c.phase;break;
 case 8:if(!call(0x8005c3cc))return false;++c.phase;break;
 case 9:if(!call(0x8005c2c4))return false;++c.phase;break;
 case 10:if(!call(0x80141050))return false;c.value=c.child.result;++c.phase;break;
 case 11:if(!call(0x800bf1b4,{c.value}))return false;c.party=c.child.result;c.phase=c.party?12:13;break;
 case 12:if(!call(0x800bed44,{c.party}))return false;++c.phase;break;
 case 13:if(!call(0x80114508,{c.field+0x20,0}))return false;++c.phase;break;
 case 14:if(!call(0x800dc110,{0x10}))return false;++c.phase;break;
 case 15:if(!call(0x8002e218,{4,0,0}))return false;return done(0);
 case 20:if(!call(0x8004829c))return false;c.field=c.child.result;++c.phase;break;
 case 21:if(!call(0x8002e210))return false;if(c.child.result!=2)return done(0);++c.phase;break;
 case 22:if(!call(0x80114500,{c.field+0x20}))return false;if(!c.child.result)return done(0);++c.phase;break;
 case 23:if(!call(0x8005c290))return false;++c.phase;break;
 case 24:if(!call(0x80141050))return false;c.value=c.child.result;++c.phase;break;
 case 25:if(!call(0x800bf1b4,{c.value}))return false;c.party=c.child.result;c.phase=c.party?26:27;break;
 case 26:if(!call(0x800bed0c,{c.party}))return false;++c.phase;break;
 case 27:if(!call(0x800dbcc4))return false;++c.phase;break;
 case 28:if(!call(0x8005c30c))return false;return done(2);
 case 30:if(!call(0x8004f5d8,{entry,c.name}))return false;c.npc=c.child.result;++c.phase;break;
 case 31:if(!load(c.npc+0x11c,c.value))return false;++c.phase;break;
 case 32:if(!load(c.args+4,c.raw))return false;++c.phase;break;
 case 33:if(!call(0x80030cfc,{entry,c.raw,c.value}))return false;return done(1);
 default:return false;
 }
}
BattleSequenceEntryServices NativeFieldNpcBattleScriptCall::services(){
 BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& c=*static_cast<NativeFieldNpcBattleScriptCall*>(p);
  if(a==c.address_+0x18 || a==c.args_ || a==c.args_+4){uint32_t v=0;if(!c.vm_->memory.readWord(a,v))return std::nullopt;if(a==c.address_+0x18)c.args_=v;return v;}
  return c.binding_.word(c.binding_.user,a);
 };
 s.store=[](void* p,uint32_t a,uint32_t v){auto& c=*static_cast<NativeFieldNpcBattleScriptCall*>(p);return c.binding_.store(c.binding_.user,a,v);};
 s.call=[](void* p,BattleSequenceCall& call){auto& c=*static_cast<NativeFieldNpcBattleScriptCall*>(p);const auto op=c.vm_->interpreterServices().operands;
  if(call.function==0x80031260){if(call.args[0]!=c.address_)return false;call.result=uint32_t(getEventValue(*c.entry_,signedWord(call.args[1]),op));return true;}
  if(call.function==0x80030cfc){if(call.args[0]!=c.address_)return false;setEventValue(*c.entry_,signedWord(call.args[1]),signedWord(call.args[2]),op);return true;}
  return c.binding_.call(c.binding_.user,call);
 };return s;
}
bool NativeFieldNpcBattleScriptCall::step(uint32_t fn,EventEntry& e,bool first,EventNativeRuntime& vm,const BattleSequenceEntryServices& b,int32_t& result){
 if(!fieldNpcBattleEventFunction(fn))return false;
 if(!vm_){vm_=&vm;entry_=&e;id_=e.id;function_=fn;first_=first;}
 if(vm_!=&vm || entry_!=&e || id_!=e.id || function_!=fn)return false;
 if(!bound_){if(!b.word || !b.store || !b.call)return false;binding_=b;bound_=true;}
 if(b.user!=binding_.user || b.word!=binding_.word || b.store!=binding_.store || b.call!=binding_.call)return false;
 if(!address_){const auto op=vm.interpreterServices().operands;if(!op.entryAddress || !(address_=op.entryAddress(op.user,e)))return false;}
 return fieldNpcBattleEvent(fn,address_,first_,services(),cursor_,result);
}
}
