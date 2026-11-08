#include "fukidashi_event.hpp"
#include <cstring>
#include <cstdlib>
#include "event_native_runtime.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Function=0x8006c3c4,GetValue=0x80031260,GetFloat=0x80030840,Compare=0x8026d464;
constexpr uint32_t MarioGet=0x8005e0c0,NpcGet=0x800499e0,PartyGet=0x80141050,ExtraGet=0x80140fb8,PartyPointer=0x800bf1b4;
constexpr uint32_t Construct=0x80133164,SetName=0x8005ebec;
constexpr uint32_t Twice=0x80420328,Thirds=0x8042032c,Zero=0x80420330,Me=0x80420334,SpecialHeight=0x80420338,Empty=0x8042033c;
constexpr uint32_t Arguments=0x18,EventNpc=0x170,MarioHeight=0x1bc,NpcHeight=0x150,PartyHeight=0xf0,PartyKind=0x31;
constexpr uint32_t MarioAttachment=0xffffffff,PartyAttachment=0xfffffffe,ExtraAttachment=0xfffffffd;
constexpr unsigned FirstFloat=5,Duration=10;constexpr uint8_t SpecialPartyKind=5;
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool same(const FukidashiEventWorldServices& a,const FukidashiEventWorldServices& b){
 return a.user==b.user && a.compare==b.compare && a.binary==b.binary && a.mario==b.mario && a.npc==b.npc &&
 a.partyId==b.partyId && a.partyPointer==b.partyPointer && a.construct==b.construct && a.setName==b.setName;
}
bool same(const FukidashiEventServices& a,const FukidashiEventServices& b){
 return same(static_cast<const FukidashiEventWorldServices&>(a),static_cast<const FukidashiEventWorldServices&>(b)) &&
 a.word==b.word && a.byte==b.byte && a.value==b.value && a.real==b.real;
}
int32_t signedWord(uint32_t n){return n<=0x7fffffff?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
struct Guard {bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
}
bool FukidashiEventCursor::reset(){if(invoking_)return false;active_=false;phase_=Phase::Arguments;pending_=0;return true;}
std::optional<int32_t> resumeFukidashiEvent(uint32_t entry,const FukidashiEventServices& s,FukidashiEventCursor& c){
 using Phase=FukidashiEventCursor::Phase;
 if(c.invoking_ || !s.word)return {};
 if(c.active_ && (c.entry_!=entry || !same(c.owner_,s)))return {};
 Guard guard(c.invoking_);
 if(!c.active_){c.active_=true;c.entry_=entry;c.owner_=s;c.index_=0;c.effect_=0;c.request_={};c.phase_=Phase::Arguments;}
 const auto word=[&](uint32_t a){return s.word(s.user,a);};
 const auto scalar=[&](uint32_t a){return real(word(a));};
 const auto mul=[&](float a,float b){return s.binary(s.user,FukidashiEventBinary::Multiply,a,b);};
 const auto div=[&](float a,float b){return s.binary(s.user,FukidashiEventBinary::Divide,a,b);};
 const auto finish=[&](){c.active_=false;c.phase_=Phase::Arguments;c.pending_=0;return std::optional<int32_t>(2);};
 for(;;)switch(c.phase_){
 case Phase::Arguments:c.args_=word(entry+Arguments);c.phase_=Phase::Raw;break;
 case Phase::Raw:c.raw_=word(c.args_+4*c.index_);c.phase_=Phase::Decode;break;
 case Phase::Decode:{
  const auto i=c.index_;
  if(i>=FirstFloat && i<Duration){
   c.pending_=GetFloat;if(!s.real)return {};const auto n=s.real(s.user,entry,c.raw_);if(!n)return {};
   if(i<8)c.request_.position[i-FirstFloat]=*n;else if(i==8)c.request_.radius=*n;else c.request_.angle=*n;
  }else{
   c.pending_=GetValue;if(!s.value)return {};const auto n=s.value(s.user,entry,c.raw_);if(!n)return {};
   switch(i){case 0:c.mode_=*n;break;case 1:c.name_=uint32_t(*n);break;case 2:c.actorName_=uint32_t(*n);break;
    case 3:c.request_.kind=*n;break;case 4:c.request_.variant=*n;break;case Duration:c.request_.duration=*n;break;}
  }
  c.phase_=++c.index_>Duration?Phase::Dispatch:Phase::Raw;break;
 }
 case Phase::Dispatch:
  switch(c.mode_){case 0:c.phase_=Phase::Mario;break;case 1:c.phase_=Phase::CompareActor;break;
   case 2:c.request_.attachment=0;c.phase_=Phase::Construct;break;
   case 3:case 4:c.phase_=Phase::PartyId;break;default:c.phase_=Phase::Name;break;}break;
 case Phase::Mario:{
  c.pending_=MarioGet;if(!s.mario)return {};const auto n=s.mario(s.user);if(!n)return {};
  c.actor_=*n;c.request_.attachment=MarioAttachment;c.phase_=Phase::Height;break;
 }
 case Phase::CompareActor:{
  c.pending_=Compare;if(!s.compare)return {};const auto n=s.compare(s.user,c.actorName_,Me);if(!n)return {};
  if(!*n){c.actor_=word(entry+EventNpc);c.request_.attachment=c.actor_;c.phase_=Phase::Height;}else c.phase_=Phase::Npc;break;
 }
 case Phase::Npc:{
  c.pending_=NpcGet;if(!s.npc)return {};const auto n=s.npc(s.user,c.actorName_);if(!n)return {};
  c.actor_=*n;c.request_.attachment=c.actor_;c.phase_=Phase::Height;break;
 }
 case Phase::PartyId:{
  c.pending_=c.mode_==4?ExtraGet:PartyGet;if(!s.partyId)return {};const auto n=s.partyId(s.user,c.mode_==4);if(!n)return {};
  c.partyId_=*n;c.phase_=Phase::PartyPointer;break;
 }
 case Phase::PartyPointer:{
  c.pending_=PartyPointer;if(!s.partyPointer)return {};const auto n=s.partyPointer(s.user,c.partyId_);if(!n)return {};
  c.actor_=*n;c.request_.attachment=c.mode_==4?ExtraAttachment:PartyAttachment;c.phase_=*n?Phase::Height:Phase::Name;break;
 }
 case Phase::Height:{
  c.pending_=Function;if(!s.binary || (c.mode_==3 && !s.byte))return {};
  float height=0,twice=0,thirds=0,y=0,radius=0,zero=0;
  // Match the three source load/operation orders, including the ordinary
  // party's repeated divisor read and kind5 override after its first division.
  if(c.mode_==3){
   twice=scalar(Twice);height=scalar(c.actor_+PartyHeight);const auto kind=s.byte(s.user,c.actor_+PartyKind);
   const auto product=mul(twice,height);thirds=scalar(Thirds);y=div(product,thirds);
   if(kind==SpecialPartyKind)y=scalar(SpecialHeight);
   thirds=scalar(Thirds);zero=scalar(Zero);radius=div(height,thirds);
  }else{
   // NPC loads2 before height; Mario and extra party load height first.
   if(c.mode_==1)twice=scalar(Twice);
   height=scalar(c.actor_+(c.mode_==0?MarioHeight:c.mode_==1?NpcHeight:PartyHeight));
   if(c.mode_!=1)twice=scalar(Twice);
   thirds=scalar(Thirds);const auto product=mul(twice,height);radius=div(height,thirds);
   zero=scalar(Zero);y=div(product,thirds);
  }
  c.request_.position={zero,y,zero};c.request_.radius=radius;c.phase_=Phase::Construct;break;
 }
 case Phase::Construct:{
  c.pending_=Construct;if(!s.construct)return {};const auto n=s.construct(s.user,c.request_);if(!n)return {};
  c.effect_=*n;c.phase_=Phase::Name;break;
 }
 case Phase::Name:if(!c.name_)return finish();c.phase_=Phase::CompareName;break;
 case Phase::CompareName:{
  // Source still compares a nonnull name when mode is invalid, party is null,
  // or the executed constructor returned0; only SetName is effect-gated.
  c.pending_=Compare;if(!s.compare)return {};const auto n=s.compare(s.user,c.name_,Empty);if(!n)return {};
  if(!*n || !c.effect_)return finish();
  c.phase_=Phase::SetName;break;
 }
 case Phase::SetName:c.pending_=SetName;if(!s.setName || !s.setName(s.user,c.effect_,c.name_))return {};return finish();
 }
}

FukidashiEventServices NativeFukidashiEventCall::services(){
 FukidashiEventServices s;s.user=this;
 s.word=[](void* p,uint32_t a){auto& c=*static_cast<NativeFukidashiEventCall*>(p);uint32_t n;if(!c.vm_->memory.readWord(a,n))std::abort();return n;};
 s.byte=[](void* p,uint32_t a){auto& c=*static_cast<NativeFukidashiEventCall*>(p);uint8_t n;if(!c.vm_->memory.readByte(a,n))std::abort();return n;};
 s.value=[](void* p,uint32_t entry,uint32_t raw)->std::optional<int32_t>{auto& c=*static_cast<NativeFukidashiEventCall*>(p);if(entry!=c.address_ || !c.vm_->platform.truncate)return {};return getEventValue(*c.entry_,signedWord(raw),c.vm_->interpreterServices().operands);};
 s.real=[](void* p,uint32_t entry,uint32_t raw)->std::optional<float>{auto& c=*static_cast<NativeFukidashiEventCall*>(p);if(entry!=c.address_ || !c.vm_->platform.truncate)return {};return getEventFloat(*c.entry_,signedWord(raw),c.vm_->interpreterServices().operands);};
 s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.compare?w.compare(w.user,a,b):std::nullopt;};
 s.binary=[](void* p,FukidashiEventBinary op,float a,float b){auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.binary(w.user,op,a,b);};
 s.mario=[](void* p)->std::optional<uint32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.mario?w.mario(w.user):std::nullopt;};
 s.npc=[](void* p,uint32_t name)->std::optional<uint32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.npc?w.npc(w.user,name):std::nullopt;};
 s.partyId=[](void* p,bool extra)->std::optional<uint32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.partyId?w.partyId(w.user,extra):std::nullopt;};
 s.partyPointer=[](void* p,uint32_t id)->std::optional<uint32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.partyPointer?w.partyPointer(w.user,id):std::nullopt;};
 s.construct=[](void* p,const FukidashiEntryRequest& r)->std::optional<uint32_t>{auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.construct?w.construct(w.user,r):std::nullopt;};
 s.setName=[](void* p,uint32_t effect,uint32_t name){auto& w=static_cast<NativeFukidashiEventCall*>(p)->world_;return w.setName && w.setName(w.user,effect,name);};
 // A missing pure binary provider must hold before any source height reads.
 if(!world_.binary)s.binary=nullptr;
 return s;
}
bool NativeFukidashiEventCall::step(EventEntry& e,EventNativeRuntime& vm,const FukidashiEventWorldServices& world,int32_t& result){
 if(invoking_)return false;
 Guard guard(invoking_);
 if(!vm_){vm_=&vm;entry_=&e;id_=uint32_t(e.id);command_=e.currentCommand;world_=world;}
 if(vm_!=&vm || entry_!=&e || id_!=uint32_t(e.id) || command_!=e.currentCommand || !same(world_,world))return false;
 uint32_t mapped=0;if(!vm.entryAddresses.addressOf(&e,mapped) || !mapped)return false;
 if(address_ && address_!=mapped)return false;
 address_=mapped;
 if(complete_){result=2;return true;}
 const auto n=resumeFukidashiEvent(address_,services(),cursor_);if(!n)return false;
 result=*n;complete_=true;return true;
}
}
