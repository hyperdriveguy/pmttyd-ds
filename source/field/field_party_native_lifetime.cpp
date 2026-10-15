#include "field_party_native_lifetime.hpp"
namespace ttyd {
namespace {
// Source currentDefinition/table globals. Records are unchanged kind1(c_pkr),
//8(c_babyyoshi),13(auxiliary c_pkr); model tokens identify source strings.
// Sharing init800c4fa0 does not canonicalize kinds or share their lifetimes.
constexpr uint32_t Table=0x8041ead8,Definition=0x8041ead4;
constexpr uint32_t Records[]={0x80314410,0x80314528,0x803145f0};
constexpr uint32_t ModelTokens[]={0x804213e0,0x802cb47c,0x804213e0};
}
// Require three40-byte records with exact address/model/init/zero exit+24.
// In native VM import only when cold; existing complete mappings stay mutable.
// No adapter recreation restores a callback changed by a prior consumer.
std::unique_ptr<NativeFieldPartyLifetime> NativeFieldPartyLifetime::create(const char* root,FieldPartyRegistry& parties,FieldMarioBattleState& mario){
 if(!root)return {};
 auto n=std::unique_ptr<NativeFieldPartyLifetime>(new NativeFieldPartyLifetime(parties,mario));
 if(!n->definitions_.load((std::string(root)+"/field-party-definitions.pmevt").c_str()) || n->definitions_.segments().size()!=3)return {};
 for(unsigned i=0;i<3;++i){const auto& s=n->definitions_.segments()[i];uint32_t model=0,init=0,exit=1;
  if(s.address!=Records[i] || s.size!=0x28 || !n->definitions_.readWord(s.address,model) || model!=ModelTokens[i] || !n->definitions_.readWord(s.address+4,init) || init!=0x800c4fa0 || !n->definitions_.readWord(s.address+0x24,exit) || exit)return {};
 }
 n->native_=parties.nativeAnimation;n->pool_=parties.originalPool;
 if(n->native_){
  if(!n->nativeOwner())return {};
  auto& m=n->native_->process().vm().memory;unsigned mapped=0;
  for(auto a:Records){bool full=true;for(unsigned offset=0;offset<0x28;offset+=4){uint32_t value=0;if(!m.readWord(a+offset,value))full=false;}if(full)++mapped;}
  // Cold import once. Already mapped records remain live mutable source data;
  // a new adapter must not restore a callback changed by an earlier consumer.
  if((mapped && mapped!=3) || (!mapped && !m.importScripts(n->definitions_)))return {};
 }
 return n;
}
bool NativeFieldPartyLifetime::nativeOwner() const{return native_ && native_->ready() && parties_.nativeAnimation==native_ && pool_ && parties_.originalPool==pool_ && native_->pool()==pool_;}
// Two native actor tokens with offset+width bounded by188hex. After source
// table publication,retiring_ supplies captured bytes although public slot is
// null. Original heap address space is not fabricated for unrelated VM users.
FieldPartyActor* NativeFieldPartyLifetime::actor(uint32_t address,unsigned bytes,unsigned& offset){
 if(address<ActorBase)return nullptr;
 const uint32_t delta=address-ActorBase,index=delta/ActorStride;offset=delta%ActorStride;
 if(index>=2 || uint64_t(offset)+bytes>0x188)return nullptr;
 if(retiring_ && ActorBase+index*ActorStride==capturedAddress_)return retiring_.get();
 return parties_.actors[index].get();
}
// Table/definition and actor fields alias actual typed storage. Native source
// data falls through to SAME live process VM; detached definitions/event memory
// are read-only admission for empty paths,not replacement live animation owners.
std::optional<uint32_t> NativeFieldPartyLifetime::word(uint32_t a){
 if(a==Definition)return parties_.currentDefinition;
 if(a==Table || a==Table+4){const auto i=(a-Table)/4;return parties_.actors[i]?ActorBase+i*ActorStride:0;}
 unsigned offset=0;if(auto* p=actor(a,4,offset))return p->word(offset);
 uint32_t v=0;
 if(native_){if(!nativeOwner())return {};return native_->process().vm().memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;}
 if(definitions_.readWord(a,v))return v;
 return eventMemory_ && eventMemory_->readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;
}
// Original Mario245/246 index and247/248 saved-kind bytes alias typed arrays.
// Other byte reads use backed actor or actual native VM; no zero defaults.
std::optional<uint8_t> NativeFieldPartyLifetime::byte(uint32_t a){
 if(a>=FieldMarioOwnerAddress+0x245 && a<=FieldMarioOwnerAddress+0x246)return mario_.partyIndices[a-FieldMarioOwnerAddress-0x245];
 if(a>=FieldMarioOwnerAddress+0x247 && a<=FieldMarioOwnerAddress+0x248)return mario_.savedPartyKinds[a-FieldMarioOwnerAddress-0x247];
 unsigned offset=0;if(auto* p=actor(a,1,offset))return p->bytes[offset];
 uint8_t v=0;return nativeOwner() && native_->process().vm().memory.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;
}
// Source table-clear store moves unique_ptr into retiring_;it does not free.
// Only zero publication for the exact captured slot is admitted. Definition
// publication remains a separate registry word,not reset on successful Kill.
bool NativeFieldPartyLifetime::store(uint32_t a,uint32_t v){
 if(a==Definition){parties_.currentDefinition=v;return true;}
 if(a==Table || a==Table+4){const auto i=(a-Table)/4;
  if(v || retiring_ || !captured_ || parties_.actors[i].get()!=captured_ || capturedAddress_!=ActorBase+i*ActorStride)return false;
  retiring_=std::move(parties_.actors[i]);return true;
 }
 return false;
}
bool NativeFieldPartyLifetime::storeByte(uint32_t a,uint8_t v){
 if(a>=FieldMarioOwnerAddress+0x245 && a<=FieldMarioOwnerAddress+0x246){mario_.partyIndices[a-FieldMarioOwnerAddress-0x245]=v;return true;}
 if(a>=FieldMarioOwnerAddress+0x247 && a<=FieldMarioOwnerAddress+0x248){mario_.savedPartyKinds[a-FieldMarioOwnerAddress-0x247]=v;return true;}
 return false;
}
// Built-in Mario/Kill/Kill2/Release/memFree identities only. Real nonzero
// source exit hooks remain unavailable until a provider exists; mode11 bypass
// belongs to retained source logic,not a successful native hook substitute.
bool NativeFieldPartyLifetime::call(BattleSequenceCall& c){
 if(c.function==0x8005e0c0){c.result=FieldMarioOwnerAddress;return true;}
 if(c.function==0x800be69c || c.function==0x800be594){
  if(!killImpl(int32_t(c.args[0]),c.function==0x800be594))return false;c.result=0;return true;
 }
 if(c.function==0x8003b2d0){
  if(!nativeOwner() || !retiring_)return false;
  if(!releaseCaptured_){releasePose_=int32_t(c.args[0]);releaseCaptured_=true;}
  if(releasePose_!=int32_t(c.args[0]))return false;
  const bool done=native_->release(releasePose_);
  // A borrowed early return leaves a live pose and its typed proxy intact.
  // During actual teardown, model flags can invalidate the pose before a
  // file child finishes; retire the proxy then. On terminal success this
  // runs in the same native call, before any entry can reuse the handle.
  if(releasePose_>=0 && unsigned(releasePose_)<parties_.poses.size() && !pool_->pose(releasePose_))parties_.poses[unsigned(releasePose_)].reset();
  return done;
 }
 if(c.function==0x800300c4){
  if(c.args[0] || c.args[1]!=capturedAddress_ || !retiring_ || !releaseCaptured_)return false;
  retiring_.reset();captured_=nullptr;return true; // Native heap adapter; actor allocation is unique_ptr too.
 }
 return false; // Genuine nonzero original exit hook requires its real provider.
}
BattleSequenceEntryServices NativeFieldPartyLifetime::services(){
 BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* u,uint32_t a){return static_cast<NativeFieldPartyLifetime*>(u)->word(a);};
 s.byte=[](void* u,uint32_t a){return static_cast<NativeFieldPartyLifetime*>(u)->byte(a);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<NativeFieldPartyLifetime*>(u)->store(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<NativeFieldPartyLifetime*>(u)->storeByte(a,v);};
 s.call=[](void* u,BattleSequenceCall& c){return static_cast<NativeFieldPartyLifetime*>(u)->call(c);};return s;
}
bool NativeFieldPartyLifetime::kill(int32_t index,bool forget){
 if(outer_.active || event_.active)return false;
 return killImpl(index,forget);
}
// Exclusion is native lease policy:entry/removal consumers cannot interleave.
// Typed index domain negative/0/1 differs from source unchecked controller.
// Capture valid live pose and reject another Release BEFORE table publication;
// later source teardown resumes even as typed pose validity changes.
bool NativeFieldPartyLifetime::killImpl(int32_t index,bool forget){
 if(!parties_.nativeEntryOwner.expired())return false;
 if(!parties_.nativeRemovalOwner.expired() && parties_.nativeRemovalOwner.lock().get()!=removalLease_.get())return false;
 if(index>=2)return false; // Explicit typed domain; source controller has no upper guard.
 if(!removal_.active){
  if(index>=0 && parties_.actors[unsigned(index)]){
   if(!nativeOwner())return false; // No legacy cache-only retirement masquerades as source fileFree.
   const auto handle=int32_t(parties_.actors[unsigned(index)]->word(0xc));
   // Serialize against a different pool consumer already releasing a pose.
   // Initial typed admission never publishes actor teardown into an invalid
   // namespace; an admitted removal itself resumes raw source state thereafter.
   if(native_->releasePending() || !pool_->pose(handle))return false;
   captured_=parties_.actors[unsigned(index)].get();capturedAddress_=ActorBase+uint32_t(index)*ActorStride;
  }
  releaseCaptured_=false;releasePose_=-1;parties_.nativeRemovalOwner=removalLease_;
 }
 if(!removeFieldParty(index,forget,services(),removal_))return false;
 parties_.nativeRemovalOwner.reset();captured_=nullptr;capturedAddress_=0;return true;
}
bool NativeFieldPartyLifetime::removeAll(){
 if(!parties_.nativeEntryOwner.expired())return false;
 if(event_.active || (removal_.active && !outer_.active) || (!parties_.nativeRemovalOwner.expired() && parties_.nativeRemovalOwner.lock().get()!=removalLease_.get()))return false;
 return removeMarioParties(services(),outer_);
}
// Native preflight admits raw slot0/1 in the actual EventEntry/args mapping;
// retained wrapper then performs original read order. Same VM/entry/clear mode
// across holds; eventMemory_ stays pinned until success,with no result2 early.
// Address alone is not a generation token:cancellation/reuse is caller-owned.
bool NativeFieldPartyLifetime::event(uint32_t entry,bool clear,EventAddressSpace& memory,int32_t& result){
 if(!parties_.nativeEntryOwner.expired())return false;
 if(outer_.active || (removal_.active && !event_.active) || (eventMemory_ && eventMemory_!=&memory) || (native_ && &native_->process().vm().memory!=&memory))return false;
 if(!parties_.nativeRemovalOwner.expired() && parties_.nativeRemovalOwner.lock().get()!=removalLease_.get())return false;
 if(!event_.active){uint32_t args=0,slot=0;if(!memory.readWord(entry+0x18,args) || !memory.readWord(args,slot) || slot>1)return false;}
 eventMemory_=&memory;
 const bool done=removeFieldPartyEvent(entry,clear,services(),event_,result);
 if(done)eventMemory_=nullptr;return done;
}
// Diagnostic priority follows deepest retained removal,then event,then outer.
// Lease expiry on whole-context destruction is not original Kill completion
// and does not roll back public table/owner stores already acknowledged.
uint32_t NativeFieldPartyLifetime::pending() const{return removal_.active?removal_.pending:event_.active?event_.pending:outer_.pending;}
}
