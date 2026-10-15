#include "native_field_animation.hpp"
#include "event_time_publication.hpp"
#include "field_npc_clock.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Bus=162000000,Name=0x81800000;
uint32_t word(const EventAddressSpace& m,uint32_t a){uint32_t v=0;const bool ok=m.readWord(a,v);assert(ok);(void)ok;return v;}
float real(const EventAddressSpace& m,uint32_t a){const uint32_t v=word(m,a);float f;std::memcpy(&f,&v,4);return f;}
float rounded(float x){volatile float f=x;return f;}
}
std::shared_ptr<NativeFieldAnimation> NativeFieldAnimation::create(const char* root,std::shared_ptr<NativeFileCache> files,const EventNativePlatform& platform){
 if(!root || !files || !stagePrologueAnimationFiles(*files,root) || !files->yieldProvider(nullptr,[](void*){return true;}))return {};
 auto n=std::shared_ptr<NativeFieldAnimation>(new NativeFieldAnimation);n->files_=std::move(files);n->pool_=std::make_shared<BattlePosePool>();
 n->process_=AnimationStartupProcess::cold(n->files_,n->pool_,platform,(std::string(root)+"/animation-boot/memory.pmevt").c_str(),0x81000000,0x82000000,12*1024*1024);
 if(!n->process_ || !n->process_->advance())return {};
 auto& m=n->process_->vm().memory;
 // Native platform/caller storage, never initialized ISO state or host pointers.
 if(!m.registerRam(0x800000f8,std::vector<uint8_t>(4)) || !m.writeWord(0x800000f8,Bus) || !m.registerRam(0x8041ec30,std::vector<uint8_t>(4)) || !m.registerRam(Name,std::vector<uint8_t>(256)) || word(m,0x8041fb40)!=0x41855555)return {};
 n->pool_->nativeProvider_=n.get();
 n->pool_->nativeEntry_=[](void* u,std::shared_ptr<const BattlePoseResource> r,int32_t mode,BattlePoseClock,const char* name){return static_cast<NativeFieldAnimation*>(u)->enter(std::move(r),mode,name);};
 n->pool_->nativeRelease_=[](void* u,int32_t h){return static_cast<NativeFieldAnimation*>(u)->release(h);};
 n->pool_->nativeClock_=[](const void* u,BattlePoseClock& c){return static_cast<const NativeFieldAnimation*>(u)->clock(c);};return n;
}
NativeFieldAnimation::~NativeFieldAnimation(){if(pool_ && pool_->nativeProvider_==this){pool_->nativeEntry_=nullptr;pool_->nativeClock_=nullptr;pool_->nativeRelease_=nullptr;pool_->nativeProvider_=nullptr;}}
bool NativeFieldAnimation::ready() const{return process_ && process_->ready();}
bool NativeFieldAnimation::clock(BattlePoseClock& out) const{
 if(!ready())return false;
 const auto& m=process_->vm().memory;uint32_t gp=0,bus=0,ah=0,al=0,bh=0,bl=0;
 if(!m.readWord(0x80415fd0,gp) || !gp || gp>0xffffffb8 || !m.readWord(0x800000f8,bus) || !m.readWord(gp+0x40,ah) || !m.readWord(gp+0x44,al) || !m.readWord(gp+0x38,bh) || !m.readWord(gp+0x3c,bl))return false;
 const uint32_t divisor=uint32_t((uint64_t(0x10624dd3)*(bus>>2))>>32)>>6;
 uint64_t a=0,b=0;if(!divisor || !nativeFieldNpcClockDivide(nullptr,uint64_t(ah)<<32|al,divisor,a) || !nativeFieldNpcClockDivide(nullptr,uint64_t(bh)<<32|bl,divisor,b))return false;
 out={a,b};return true;
}
bool NativeFieldAnimation::tick(bool battle,bool paused){
 if(!ready())return false;
 auto& vm=process_->vm();auto& m=vm.memory;uint32_t gp=0,bus=0,window=0;
 if(!m.readWord(0x80415fd0,gp) || gp!=0x81000000 || !m.readWord(0x800000f8,bus) || !m.readWord(0x8041ec30,window))return false;
 if(window){uint16_t flags=0;uint32_t state=0;if(window>0xffffffdc || !m.readHalf(window,flags) || (!(flags&0x1000) && !m.readWord(window+0x20,state)))return false;}
 if(bus!=Bus || !m.writeWord(gp+0x10,paused?1:0) || !m.writeWord(gp+0x14,battle?1:0))return false;
 raw_+=675000; // Exactly(162000000/4)/60, independent of render/drop/IO time.
 EventTimeServices s;s.memory=vm.memoryBinding.services();s.user=this;
 s.upper=[](void* u){return uint32_t(static_cast<NativeFieldAnimation*>(u)->raw_>>32);};
 s.lower=[](void* u){return uint32_t(static_cast<NativeFieldAnimation*>(u)->raw_);};
 publishEventRetraceTime(0,s);return true;
}
std::optional<int32_t> NativeFieldAnimation::enter(std::shared_ptr<const BattlePoseResource> resource,int32_t mode,const char* requested){
 if(!ready() || releasing_.active || mode<0 || mode>2 || !resource || !resource->draw || !resource->textures || !resource->bank || resource->names.empty())return {};
 const std::string name=requested?requested:resource->modelName;if(name.empty() || name.size()>=256 || name!=resource->modelName || !files_->contains("a/"+name,5) || !files_->contains("a/"+resource->textureName+"-",4))return {};
 // Preserve the original model-cache canonical resource, including its atlas.
 for(const auto& model:pool_->models())if(model.flags() && model.resource && model.resource->modelName==resource->modelName){resource=model.resource;break;}
 if(!entering_){name_=name;entering_=resource;mode_=mode;auto& m=process_->vm().memory;for(unsigned i=0;i<=name.size();++i)if(!m.writeByte(Name+i,i<name.size()?uint8_t(name[i]):0))return {};}
 else if(entering_!=resource || mode_!=mode || name_!=name)return {};
 uint32_t address=Name;
 for(auto pair:{std::make_pair("a_mario",0x802c18a0u),std::make_pair("a_mario_r",0x802c18a8u),std::make_pair("e_mario",0x802c18b4u),std::make_pair("c_pkr",0x804213e0u)})if(name==pair.first)address=pair.second;
 auto result=process_->enter(address,uint32_t(mode),resource);if(result){entering_.reset();name_.clear();}
 // Source exposes -2 for file failure; the typed API exposes only valid handles.
 if(result && *result<0)return {};
 return result;
}
bool NativeFieldAnimation::mario(const MarioPresentation& mario,const std::array<int32_t,3>& handles){
 sampled_=-1;if(!ready() || !mario.supported || !mario.poseUploaded)return false;
 bank_=(mario.displayFlags&0x10000000)?2:(mario.displayFlags&0x80000000)?1:0;
 const int32_t handle=handles[bank_];auto* p=pool_->pose(handle);if(!p || !p->sourceBuffers || !p->resource->bank->pose(mario.selected))return false;
 BattlePoseClock current;if(!clock(current))return false;
 if(selected_!=handle || uploaded_!=mario.poseUploadSerial){
  if(!pool_->selectName(handle,mario.selected,true,current))return false;
  // Used unlinked SetLocalTime: source float41855555 multiply, finite native
  // unsigned conversion, modulo64 subtraction, low-before-high ordinary and
  // paper stores. No cache/rate reset beyond the preceding forced selection.
  const float ms=rounded(real(process_->vm().memory,0x8041fb40)*float(mario.poseStartFrame));
  if(!std::isfinite(ms) || ms<0 || double(ms)>=18446744073709551616.0)return false;
  const uint64_t q=p->word(0xc)?current.modeNonzero:current.modeZero,epoch=q-uint64_t(double(ms));
  if(p->word(0x90)!=0xffffffff)return false;
  p->word(0x1c,uint32_t(epoch));p->word(0x18,uint32_t(epoch>>32));p->word(0x8c,p->word(0x1c));p->word(0x88,p->word(0x18));
  selected_=handle;uploaded_=mario.poseUploadSerial;
 }
 if(!pool_->main(handle,current))return false;
 sampled_=handle;return true;
}
}
