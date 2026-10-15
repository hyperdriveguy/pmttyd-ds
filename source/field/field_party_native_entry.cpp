#include "field_party_native_entry.hpp"
namespace ttyd {
// Twelve unchanged source spans:three scale/vector passes and overwritten
// seeds(b178/b184/b190/b19c/b220),names(b4dc/b52c),Hello dispatch3141c4,
// normal/aux pose rows314744/314a14,unit421200,dimensions421584.
// Validate exact address/size; import only absent complete spans into actual VM.
// Loader layout admission is separate from converter's original-byte hashes.
std::unique_ptr<NativeFieldPartyEntry> NativeFieldPartyEntry::create(const char* root,FieldPartyRegistry& p,FieldMarioBattleState& m,std::shared_ptr<ProloguePouch> q,Trace t){
 if(!root)return {};
 auto n=std::unique_ptr<NativeFieldPartyEntry>(new NativeFieldPartyEntry(p,m,std::move(q),std::move(t)));
 if(!n->data_.load((std::string(root)+"/field-party-entry.pmevt").c_str()) || !n->definitions_.load((std::string(root)+"/field-party-definitions.pmevt").c_str()) || n->data_.segments().size()!=12)return {};
 constexpr uint32_t addresses[]={0x802cb178,0x802cb184,0x802cb190,0x802cb19c,0x802cb220,0x802cb4dc,0x802cb52c,0x803141c4,0x80314744,0x80314a14,0x80421200,0x80421584};
 constexpr uint32_t sizes[]={12,12,12,12,12,8,8,124,60,60,4,8};
 for(unsigned i=0;i<12;++i)if(n->data_.segments()[i].address!=addresses[i] || n->data_.segments()[i].size!=sizes[i])return {};
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
// issued_ latches first request/trace per phase; next() clears it. It is not
// a generic success bit:child availability and captured operands remain separate.
void NativeFieldPartyEntry::request(uint32_t f,std::initializer_list<int32_t> args){if(!issued_){issued_=true;if(trace_)trace_(f,args);}}
bool NativeFieldPartyEntry::word(uint32_t a,uint32_t& v) const {
 if(parties_.nativeAnimation)return parties_.nativeAnimation->process().vm().memory.readWord(a,v);
 if(data_.readWord(a,v))return true;
 return definitions_.readWord(a,v); // Legacy workspace reads the same unchanged original records.
}
bool NativeFieldPartyEntry::byte(uint32_t a,uint8_t& v) const {
 return parties_.nativeAnimation?parties_.nativeAnimation->process().vm().memory.readByte(a,v):data_.readByte(a,v);
}
FieldPartyPose* NativeFieldPartyEntry::pose(int32_t h) const {
 if(parties_.originalPool && !parties_.originalPool->pose(h))return nullptr;
 return parties_.pose(h);
}
// Original actor name18,clear bytes20/21,OR word8 mask2000. seed() reads
// three overwritten source stack-seed words for ordering,without publishing them.
void NativeFieldPartyEntry::updateName(){actor_->word(0x18,selected_);actor_->bytes[0x20]=actor_->bytes[0x21]=0;actor_->word(8,actor_->word(8)|0x2000);}
bool NativeFieldPartyEntry::seed(uint32_t a) const {uint32_t v=0;return word(a,v) && word(a+4,v) && word(a+8,v);}
// Scoped kinds ordinary1/aux13 only. Ordinary live signed HP(pouch half20)
// <=5 selects row12,otherwise1;aux13 row1 skips HP. Actor word4 mask8000
// is the unbound d_mario domain. Do not turn aux13 into canonical join kind1.
bool NativeFieldPartyEntry::select(){
 const int32_t admitted=form_==Form::Secondary?13:1;
 if(!issued_){
  if(int8_t(actor_->bytes[0x31])!=admitted){pending_=0x800be144;return false;}
  selector_=1;
  if(admitted<8){if(!pouch_){pending_=0x80140ea4;return false;}request(0x80140ea4,{admitted});selector_=pouch_->half(20)<=5?12:1;}
  else issued_=true; // Source auxiliary13 does not query HP.
 }
 if((actor_->word(4)&0x8000) || int8_t(actor_->bytes[0x31])!=admitted){pending_=0x800be144;return false;}
 const uint32_t row=form_==Form::Secondary?0x80314a14:0x80314744;
 if(!word(row+4*uint32_t(selector_),selected_)){pending_=0x800be144;return false;}return true;
}
// Capture old/selected string identities once per comparison; actual guest
// bytes decide equality,including distinct aliases. Bound10000hex is native
// finite-string admission. Main's second comparison still runs with fresh8000,
// suppressing only its eventual name write. It is not a duplicated no-op.
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
 if(started_ && form_!=Form::Ordinary){pending_=entryFunction();return false;}started_=true;
 return advance(kind,std::move(bank),now,appearance,result);
}
// Retain raw float words,form and kind across loading holds. A later call
// cannot retarget coordinates/form or a different native animation/pool owner.
bool NativeFieldPartyEntry::position(int32_t kind,bool secondary,const std::array<float,3>& xyz,std::shared_ptr<const ActorAnimationPack> bank,uint64_t now,const FieldPartyAppearanceServices& appearance,int32_t& result){
 std::array<uint32_t,3> bits{};for(unsigned i=0;i<3;++i)std::memcpy(&bits[i],&xyz[i],4);
 const auto form=secondary?Form::Secondary:Form::Position;
 if(started_ && (form_!=form || position_!=bits)){pending_=entryFunction();return false;}
 started_=true;form_=form;position_=bits;return advance(kind,std::move(bank),now,appearance,result);
}
bool NativeFieldPartyEntry::advance(int32_t kind,std::shared_ptr<const ActorAnimationPack> bank,uint64_t now,const FieldPartyAppearanceServices& appearance,int32_t& result){
 pending_=0;
 if(parties_.nativeAnimation!=native_ || parties_.originalPool!=pool_){pending_=entryFunction();return false;}
 if(kind_>=0 && kind!=kind_){pending_=entryFunction();return false;}
 if(actor_ && phase_!=Phase::Done && parties_.pointer(slot_)!=actor_){pending_=0x800be144;return false;}
 for(;;)switch(phase_){
 // Ordinary caller requires canonical pouch join mask1 at half14 BEFORE
 // any Kill/allocation;failure returns-1 terminal. Nonnegative primary245 is
 // re-queried before Kill. Position/Secondary skip this join/removal sequence.
 case Phase::Caller:
  if(!parties_.nativeEntryOwner.expired() || !parties_.nativeRemovalOwner.expired()){pending_=entryFunction();return false;}
  if(kind!=(form_==Form::Secondary?13:1) || (form_==Form::Ordinary && !pouch_)){pending_=entryFunction();return false;}
  kind_=kind;request(entryFunction(),{kind});
  if(form_==Form::Secondary){if(trace_)trace_(0x8005e0c0,{});next(Phase::AuxAllocate);break;}
  if(form_==Form::Position){next(Phase::Allocate);break;}
  if(trace_)trace_(0x8005e0c0,{});
  if(!(pouch_->half(14)&1)){result_=-1;next(Phase::Done);break;}
  oldIndex_=int8_t(mario_.partyIndices[0]);
  if(oldIndex_>=0){if(trace_)trace_(0x8005e0c0,{});oldIndex_=int8_t(mario_.partyIndices[0]);}
  next(oldIndex_>=0?Phase::Kill:Phase::Allocate);break;
 case Phase::Kill:
  request(0x800be69c,{oldIndex_});
  if(!lifetime_->kill(oldIndex_)){pending_=lifetime_->pending();if(!pending_)pending_=0x800be69c;return false;}
  mario_.partyIndices[0]=255;next(Phase::Allocate);break;
 // First-free ordinary record188hex is published once,then entry lease
 // excludes competing allocation/removal. Aux tests ONLY slot1:occupied returns
 // genuine-1,never substitutes free0. Source flags9 OR4,kind13,slot byte2f,
 // owner160,camera164 and absent pose IDs c/10/14 remain distinct fields.
 case Phase::Allocate:
  if(!issued_){
   if(form_==Form::Ordinary){request(0x800bea68,{kind_});if(trace_)trace_(0x800be144,{kind_});}
   else request(0x800be144,{kind_});
   if(trace_)trace_(0x8005e0c0,{});
  }
  slot_=parties_.allocateFirst();if(slot_<0){pending_=0x800bea68;return false;}
  actor_=parties_.pointer(slot_);parties_.nativeEntryOwner=lease_;
  if(trace_)trace_(0x801a94f0,{});
  parties_.initializeBeforePose(slot_,uint8_t(kind_),mario_);next(Phase::Resource);break;
 case Phase::AuxAllocate:
  if(parties_.actors[1]){result_=-1;next(Phase::Done);break;}
  // The source tests only slot1. Never substitute first-free slot0.
  if(!parties_.nativeEntryOwner.expired() || !parties_.nativeRemovalOwner.expired()){pending_=0x800bd6d8;return false;}
  slot_=1;parties_.actors[1]=std::make_unique<FieldPartyActor>();actor_=parties_.pointer(1);actor_->bytes.fill(0);parties_.nativeEntryOwner=lease_;
  if(trace_)trace_(0x801a94f0,{});
  actor_->word(0x164,mario_.cameraId);actor_->word(0xc,0xffffffff);actor_->word(0x10,0xffffffff);actor_->word(0x14,0xffffffff);
  actor_->word(0,9);actor_->bytes[0x2f]=1;actor_->word(0x160,FieldMarioOwnerAddress);actor_->bytes[0x31]=uint8_t(kind_);actor_->word(0,actor_->word(0)|4);
  definition_=0x803143e8+uint32_t(kind_)*0x28;parties_.currentDefinition=definition_;next(Phase::Resource);break;
 // Secondary retains local definition;Main rereads currentDefinition for
 // model,then BOTH reread global before init. word4 mask8000 selects d_mario
 // token2cb340 but real alternate resource/name providers remain unavailable.
 case Phase::Resource:
  if(actor_->bytes[0x31]!=(form_==Form::Secondary?13:1)){pending_=0x800be144;return false;}
  if(actor_->word(4)&0x8000){resource_=0x802cb340;next(Phase::Release);break;}
  // Secondary loads through its retained local definition; Main rereads global.
  if(form_!=Form::Secondary)definition_=parties_.currentDefinition;
  next(Phase::ResourceRead);break;
 case Phase::ResourceRead:
  if(!word(definition_,resource_)){pending_=0x800be144;return false;}
  next(Phase::Release);break;
 // Capture reached old pose once. Actual source release can invalidate its
 // typed wrapper before a file wait finishes;retire wrapper then,not on handle
 // reuse. Enter4176c uses source resource/mode2 before actor posec publication.
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
 // Post-paperOff capture actor160 once. Two independent marioAnimeId593fc
 // calls choose current owner pose for material flags then light flags; each
 // getter/mask stays captured across setter holds,with fresh partner posec.
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
 // Clear1c,then capture fresh current definition/init+4. Only original
 // kuribo_init c4fa0 is bound:bytes33=1,32=0,source width/height f0/f4,
 // shared Goombella init work0. Unknown initializer holds rather than completes.
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
  next(form_==Form::Secondary?Phase::AuxMode:form_==Form::Position?Phase::PosVector:Phase::Scale);break;
 // Mode wrapper saves prior34 to35 except9/3,then freshly rereads old mode.
 // Inner old0->2 is genuine no-work;other cleanup domains hold. Finish sets
 // mode2/word8 OR8,reads source unit1/seed,then XYZ58 and previousXYZb8 plus
 // unit scale70/74/78. No appearance/Hello/display/Mario246 publication here.
 case Phase::AuxMode:
  request(0x800bcb2c,{slot_,2});oldMode_=actor_->bytes[0x34];
  if(oldMode_!=9 && oldMode_!=3)actor_->bytes[0x35]=uint8_t(oldMode_);
  oldMode_=actor_->bytes[0x34];next(Phase::AuxModeInner);break;
 case Phase::AuxModeInner:
  request(0x800bcb98,{slot_,oldMode_,2});
  if(oldMode_!=0){pending_=0x800bcb98;return false;} // Fresh old0 inner is a real no-work leaf; other cleanup graphs need providers.
  next(Phase::AuxModeFinish);break;
 case Phase::AuxModeFinish:
  actor_->bytes[0x34]=2;actor_->word(8,actor_->word(8)|8);next(Phase::AuxUnit);break;
 case Phase::AuxUnit:
  if(!word(0x80421200,unit_)){pending_=0x800bd6d8;return false;}next(Phase::AuxSeed);break;
 case Phase::AuxSeed:
  if(!seed(0x802cb220)){pending_=0x800bd6d8;return false;}next(Phase::AuxPosition);break;
 case Phase::AuxPosition:{
  actor_->word(0x58,position_[0]);actor_->word(0x5c,position_[1]);actor_->word(0x60,position_[2]);
  const uint32_t x=actor_->word(0x58),y=actor_->word(0x5c);actor_->word(0xb8,x);actor_->word(0xbc,y);actor_->word(0xc0,actor_->word(0x60));
  actor_->word(0x70,unit_);actor_->word(0x74,unit_);actor_->word(0x78,unit_);result_=slot_;lease_.reset();next(Phase::Done);break;}
 // Ordinary Position tail:initial vector b184,overwritten seed b190,XYZ58,
 // capture owner160,final vector b19c,then TWO display calls and fresh pose
 // choice. Finish mode0;no previousXYZ,appearance/Hello or Mario245 write.
 case Phase::PosVector:{
  uint32_t x=0,y=0;if(!word(0x802cb184,x) || !word(0x802cb188,y)){pending_=0x800be84c;return false;}
  actor_->word(0x70,x);actor_->word(0x74,y);next(Phase::PosVectorZ);break;}
 case Phase::PosVectorZ:{
  uint32_t z=0;if(!word(0x802cb18c,z)){pending_=0x800be84c;return false;}actor_->word(0x78,z);next(Phase::PosSeed);break;}
 case Phase::PosSeed:
  if(!seed(0x802cb190)){pending_=0x800be84c;return false;}next(Phase::PosPosition);break;
 case Phase::PosPosition:
  actor_->word(0x58,position_[0]);actor_->word(0x5c,position_[1]);actor_->word(0x60,position_[2]);next(Phase::PosOwner);break;
 case Phase::PosOwner:
  if(actor_->word(0x160)!=FieldMarioOwnerAddress){pending_=0x800be84c;return false;}capturedMario_=&mario_;next(Phase::PosFinalVector);break;
 case Phase::PosFinalVector:{
  uint32_t x=0,y=0;if(!word(0x802cb19c,x) || !word(0x802cb1a0,y)){pending_=0x800be84c;return false;}
  actor_->word(0x70,x);actor_->word(0x74,y);next(Phase::PosFinalZ);break;}
 case Phase::PosFinalZ:{
  uint32_t z=0;if(!word(0x802cb1a4,z)){pending_=0x800be84c;return false;}actor_->word(0x78,z);next(Phase::DisplayFirst);break;}
 case Phase::PosSelect:
  if(!issued_ && (actor_->word(4)&2)){next(Phase::PosFinish);break;}
  if(!select())return false;
  next(Phase::PosCompare);break;
 case Phase::PosCompare:
  if(!compare())return false;
  next(Phase::PosFinish);break;
 case Phase::PosFinish:
  actor_->bytes[0x34]=0;result_=slot_;lease_.reset();next(Phase::Done);break;

 // Ordinary appearance vector b178,Hello state38=10,capture owner,then two
 // DISTINCT display queries:actor110 first,10c second. Their results are not
 // collapsed even when provider returns the same angle.
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
  if(!appearance.display){pending_=0x800bea68;return false;}actor_->scalar(0x10c,appearance.display(appearance.user));next(form_==Form::Position?Phase::PosSelect:Phase::Place);break;
 // Source appearance2 c6718 requires real value-output providers;original
 // boolean return is ignored. Publish resulting XYZ/previousXYZ after the child.
 // Intermediate reentrant output-alias observations are outside this adapter.
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
 // First appearance/pose/mode1 precedes Mario primary publication. Fresh
 // Hello clears through only bound state10->dispatch bb9ec;second placement
 // and live pose selection run before terminal result. No construction/material
 // or first display work replays while a later Hello/string child is held.
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
