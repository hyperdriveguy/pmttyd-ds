#include "field_party_native_entry.hpp"
namespace ttyd {
std::unique_ptr<NativeFieldPartyEntry> NativeFieldPartyEntry::create(const char* root,FieldPartyRegistry& p,FieldMarioBattleState& m,std::shared_ptr<ProloguePouch> q,Trace t){
 if(!root || !q)return {};
 auto n=std::unique_ptr<NativeFieldPartyEntry>(new NativeFieldPartyEntry(p,m,std::move(q),std::move(t)));
 if(!n->data_.load((std::string(root)+"/field-party-entry.pmevt").c_str()) || n->data_.segments().size()!=6)return {};
 constexpr uint32_t addresses[]={0x802cb178,0x802cb4dc,0x802cb52c,0x803141c4,0x80314744,0x80421584};
 constexpr uint32_t sizes[]={12,8,8,124,60,8};
 for(unsigned i=0;i<6;++i)if(n->data_.segments()[i].address!=addresses[i] || n->data_.segments()[i].size!=sizes[i])return {};
 n->native_=p.nativeAnimation;n->pool_=p.originalPool;
 n->lifetime_=NativeFieldPartyLifetime::create(root,p,m);if(!n->lifetime_)return {};
 if(p.nativeAnimation){
  auto& memory=p.nativeAnimation->process().vm().memory;
  // Preserve already mapped live bytes. Partial source spans are an unavailable
  // ownership domain; do not overwrite an earlier consumer's source state.
  for(const auto& s:n->data_.segments()){
   unsigned count=0;uint8_t b=0;for(unsigned i=0;i<s.size;++i)count+=memory.readByte(s.address+i,b)?1:0;
   if(count && count!=s.size)return {};
  }
  for(const auto& s:n->data_.segments()){
   uint8_t b=0;if(memory.readByte(s.address,b))continue;
   std::vector<uint8_t> bytes(s.size);for(unsigned i=0;i<s.size;++i)if(!n->data_.readByte(s.address+i,bytes[i]))return {};
   if(!memory.registerRam(s.address,std::move(bytes)))return {};
  }
 }
 return n;
}
void NativeFieldPartyEntry::request(uint32_t f,std::initializer_list<int32_t> args){if(!issued_){issued_=true;if(trace_)trace_(f,args);}}
bool NativeFieldPartyEntry::word(uint32_t a,uint32_t& v) const {
 if(parties_.nativeAnimation)return parties_.nativeAnimation->process().vm().memory.readWord(a,v);
 if(data_.readWord(a,v))return true;
 // Detached legacy workspace admits only this unchanged original record.
 if(a==0x80314410){v=0x804213e0;return true;}if(a==0x80314414){v=0x800c4fa0;return true;}return false;
}
bool NativeFieldPartyEntry::byte(uint32_t a,uint8_t& v) const {
 return parties_.nativeAnimation?parties_.nativeAnimation->process().vm().memory.readByte(a,v):data_.readByte(a,v);
}
FieldPartyPose* NativeFieldPartyEntry::pose(int32_t h) const {
 if(parties_.originalPool && !parties_.originalPool->pose(h))return nullptr;
 return parties_.pose(h);
}
void NativeFieldPartyEntry::updateName(){actor_->word(0x18,selected_);actor_->bytes[0x20]=actor_->bytes[0x21]=0;actor_->word(8,actor_->word(8)|0x2000);}
bool NativeFieldPartyEntry::select(){
 if(!issued_){
  if(int8_t(actor_->bytes[0x31])!=1){pending_=0x800be144;return false;}
  request(0x80140ea4,{1});selector_=pouch_->half(20)<=5?12:1;
 }
 if((actor_->word(4)&0x8000) || int8_t(actor_->bytes[0x31])!=1){pending_=0x800be144;return false;}
 if(!word(0x80314744+4*uint32_t(selector_),selected_)){pending_=0x800be144;return false;}return true;
}
bool NativeFieldPartyEntry::compare(bool second){
 if(!issued_){
  oldName_=actor_->word(0x18);
  if(!oldName_){if(!second || !(actor_->word(4)&0x8000))updateName();return true;}
  request(0x8026d464,{int32_t(oldName_),int32_t(selected_)});
 }
 // Actual NUL-terminated byte comparison, including distinct aliases with the
 // same contents. Missing source strings hold the already captured operands.
 for(uint32_t i=0;i<0x10000;++i){
  uint8_t a=0,b=0;if(!byte(oldName_+i,a) || !byte(selected_+i,b)){pending_=0x8026d464;return false;}
  if(a!=b){if(!second || !(actor_->word(4)&0x8000))updateName();return true;}
  if(!a)return true;
 }
 pending_=0x8026d464;return false; // Native finite-string admission, never force equality.
}
bool NativeFieldPartyEntry::step(int32_t kind,std::shared_ptr<const ActorAnimationPack> bank,uint64_t now,const FieldPartyAppearanceServices& appearance,int32_t& result){
 pending_=0;
 if(parties_.nativeAnimation!=native_ || parties_.originalPool!=pool_){pending_=0x801412fc;return false;}
 if(kind_>=0 && kind!=kind_){pending_=0x801412fc;return false;}
 if(actor_ && phase_!=Phase::Done && parties_.pointer(slot_)!=actor_){pending_=0x800be144;return false;}
 for(;;)switch(phase_){
 case Phase::Caller:
  if(!parties_.nativeEntryOwner.expired() || !parties_.nativeRemovalOwner.expired()){pending_=0x801412fc;return false;}
  if(kind!=1){pending_=0x801412fc;return false;}
  kind_=kind;request(0x801412fc,{kind});if(trace_)trace_(0x8005e0c0,{});
  if(!(pouch_->half(14)&1)){result_=-1;next(Phase::Done);break;}
  oldIndex_=int8_t(mario_.partyIndices[0]);
  if(oldIndex_>=0){if(trace_)trace_(0x8005e0c0,{});oldIndex_=int8_t(mario_.partyIndices[0]);}
  next(oldIndex_>=0?Phase::Kill:Phase::Allocate);break;
 case Phase::Kill:
  request(0x800be69c,{oldIndex_});
  if(!lifetime_->kill(oldIndex_)){pending_=lifetime_->pending();if(!pending_)pending_=0x800be69c;return false;}
  mario_.partyIndices[0]=255;next(Phase::Allocate);break;
 case Phase::Allocate:
  if(!issued_){request(0x800bea68,{kind_});if(trace_){trace_(0x800be144,{kind_});trace_(0x8005e0c0,{});}}
  slot_=parties_.allocateFirst();if(slot_<0){pending_=0x800bea68;return false;}
  actor_=parties_.pointer(slot_);parties_.nativeEntryOwner=lease_;
  if(trace_)trace_(0x801a94f0,{});
  parties_.initializeBeforePose(slot_,uint8_t(kind_),mario_);next(Phase::Resource);break;
 case Phase::Resource:
  if(actor_->bytes[0x31]!=1){pending_=0x800be144;return false;}
  if(actor_->word(4)&0x8000)resource_=0x802cb340;
  else if(!word(parties_.currentDefinition,resource_)){pending_=0x800be144;return false;}
  next(Phase::Release);break;
 case Phase::Release:
  if(!issued_){handle_=int32_t(actor_->word(0xc));if(handle_>=0)request(0x8003b2d0,{handle_});}
  if(handle_>=0){
   if(!parties_.nativeAnimation || unsigned(handle_)>=parties_.poses.size()){pending_=0x8003b2d0;return false;}
   const bool done=parties_.originalPool->release(handle_);
   if(!parties_.originalPool->pose(handle_))parties_.poses[unsigned(handle_)].reset();
   if(!done){pending_=0x8003b2d0;return false;}
  }
  next(Phase::Enter);break;
 case Phase::Enter:
  request(0x8004176c,{int32_t(resource_),2});
  handle_=parties_.enterPose(resource_,2,bank,now);
  if(handle_<0){pending_=0x8004176c;return false;}
  actor_->word(0xc,uint32_t(handle_));next(Phase::Paper);break;
 case Phase::Paper:
  if(!issued_)handle_=int32_t(actor_->word(0xc));
  request(0x800414b0,{handle_});
  if(auto* p=pose(handle_))p->paperOff();else {pending_=0x800414b0;return false;}
  next(Phase::Owner);break;
 case Phase::Owner:
  if(actor_->word(0x160)!=FieldMarioOwnerAddress){pending_=0x800be144;return false;}
  capturedMario_=&mario_;next(Phase::MaterialSelect);break;
 case Phase::MaterialSelect:
  request(0x800593fc);childHandle_=capturedMario_->poseHandles[unsigned(fieldMarioAnimationId(*capturedMario_))];next(Phase::MaterialGet);break;
 case Phase::MaterialGet:
  request(0x8004069c,{childHandle_});if(auto* p=pose(childHandle_))mask_=uint32_t(p->materialFlags);else {pending_=0x8004069c;return false;}
  next(Phase::MaterialOn);break;
 case Phase::MaterialOn:
  if(!issued_)handle_=int32_t(actor_->word(0xc));
  request(0x80040730,{handle_,int32_t(mask_)});
  if(auto* p=pose(handle_))p->materialFlags|=mask_;else {pending_=0x80040730;return false;}
  next(Phase::LightSelect);break;
 case Phase::LightSelect:
  request(0x800593fc);childHandle_=capturedMario_->poseHandles[unsigned(fieldMarioAnimationId(*capturedMario_))];next(Phase::LightGet);break;
 case Phase::LightGet:
  request(0x80040684,{childHandle_});if(auto* p=pose(childHandle_))mask_=uint32_t(p->materialLightFlags);else {pending_=0x80040684;return false;}
  next(Phase::LightOn);break;
 case Phase::LightOn:
  if(!issued_)handle_=int32_t(actor_->word(0xc));
  request(0x800406f0,{handle_,int32_t(mask_)});
  if(auto* p=pose(handle_))p->materialLightFlags|=mask_;else {pending_=0x800406f0;return false;}
  next(Phase::MainSelect);break;
 case Phase::MainSelect:
  if(!issued_ && (actor_->word(4)&2)){selected_=actor_->word(0x18);next(Phase::MainAgain);break;}
  if(!select())return false;
  next(Phase::MainCompare);break;
 case Phase::MainCompare:
  if(!compare())return false;
  next(Phase::MainAgain);break;
 case Phase::MainAgain:
  if(!issued_ && (actor_->word(4)&2)){next(Phase::MainFinish);break;}
  if(!compare(true))return false;
  next(Phase::MainFinish);break;
 case Phase::MainFinish:
  actor_->word(0x1c,0);definition_=parties_.currentDefinition;next(Phase::InitRead);break;
 case Phase::InitRead:
  if(!word(definition_+4,init_)){pending_=0x800be144;return false;}
  next(Phase::Init);break;
 case Phase::Init:
  request(init_,{slot_});if(init_!=0x800c4fa0){pending_=init_?init_:0x800be144;return false;}
  actor_->bytes[0x33]=1;next(Phase::InitWidth);break;
 case Phase::InitWidth:
  if(!word(0x80421584,width_)){pending_=init_;return false;}
  actor_->bytes[0x32]=0;next(Phase::InitHeight);break;
 case Phase::InitHeight:
  if(!word(0x80421588,height_)){pending_=init_;return false;}
  actor_->word(0xf0,width_);actor_->word(0xf4,height_);parties_.goombellaInitWork=0;
  next(Phase::Scale);break;
 case Phase::Scale:{
  uint32_t x=0,y=0;if(!word(0x802cb178,x) || !word(0x802cb17c,y)){pending_=0x800bea68;return false;}
  actor_->word(0x70,x);actor_->word(0x74,y);next(Phase::ScaleZ);break;}
 case Phase::ScaleZ:{
  uint32_t z=0;if(!word(0x802cb180,z)){pending_=0x800bea68;return false;}
  actor_->word(0x78,z);actor_->bytes[0x38]=10;next(Phase::ScaleOwner);break;}
 case Phase::ScaleOwner:
  if(actor_->word(0x160)!=FieldMarioOwnerAddress){pending_=0x800bea68;return false;}
  capturedMario_=&mario_;next(Phase::DisplayFirst);break;
 case Phase::DisplayFirst:
  if(!appearance.display){pending_=0x800bea68;return false;}actor_->scalar(0x110,appearance.display(appearance.user));next(Phase::DisplaySecond);break;
 case Phase::DisplaySecond:
  if(!appearance.display){pending_=0x800bea68;return false;}actor_->scalar(0x10c,appearance.display(appearance.user));next(Phase::Place);break;
 case Phase::Place:{
  request(0x800c6718,{slot_});if(!appearance.ready()){pending_=0x800c6718;return false;}
  FieldWallVector position;fieldPartyAppearPosition2(actor_->scalar(0xf4),slot_,position,appearance);
  actor_->scalar(0x58,position.x);actor_->scalar(0x5c,position.y);actor_->scalar(0x60,position.z);
  const uint32_t x=actor_->word(0x58),y=actor_->word(0x5c);actor_->word(0xb8,x);actor_->word(0xbc,y);actor_->word(0xc0,actor_->word(0x60));
  next(Phase::PlaceSelect);break;}
 case Phase::PlaceSelect:
  if(!issued_ && (actor_->word(4)&2)){next(Phase::Publish);break;}
  if(!select())return false;
  next(Phase::PlaceCompare);break;
 case Phase::PlaceCompare:
  if(!compare())return false;
  next(Phase::Publish);break;
 case Phase::Publish:
  actor_->bytes[0x34]=1;
  if(hello_){result_=slot_;lease_.reset();next(Phase::Done);break;}
  mario_.partyIndices[0]=uint8_t(slot_);request(0x800bf1b4,{slot_});next(Phase::Hello);break;
 case Phase::Hello:
  request(0x800bbc48,{slot_});if(actor_->word(8)&8){pending_=0x800bbc48;return false;}next(Phase::HelloMain);break;
 case Phase::HelloMain:{
  request(0x800bb544,{slot_});const uint8_t state=actor_->bytes[0x38];const auto owner=actor_->word(0x160);(void)owner;
  uint32_t dispatch=0;if(state!=10 || !word(0x803141c4+4*state,dispatch) || dispatch!=0x800bb9ec){pending_=0x800bb544;return false;}
  hello_=true;next(Phase::Place);break;}
 case Phase::Done:result=result_;return true;
 }
}
}
