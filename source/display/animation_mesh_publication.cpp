#include "animation_mesh_publication.hpp"
#include "animation_startup_process.hpp"
#include "event_ram_access.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {constexpr uint32_t Counts[]={0xf0,0xf8,0x13c,0x140,0x12c},Outputs[]={0x4c,0x54,0x5c,0x64,0x6c},Widths[]={12,12,1,4,24};}
AnimationMeshPublication::AnimationMeshPublication(AnimationStartupProcess& p,std::function<bool(uint64_t)> current,bool privateCapture):process_(p),pool_(p.pool_),aliases_(*p.animationMemory()),current_(std::move(current)),privateCapture_(privateCapture){}
AnimationMeshPublication::~AnimationMeshPublication()=default;
bool AnimationMeshPublication::available() const{if(fastCapture_)return true;const auto* p=pool_->slot(handle_);return active_ && current_(pass_) && (captureReady_ || process_.ready()) && process_.animationMemory()==&aliases_ && aliases_.aliases(*pool_,process_.vm().memory) && p && p->nativeGeneration==generation_ && p->resource==resource_ && p->sourceBuffers==buffers_;}
bool AnimationMeshPublication::reserve(uint32_t handle,uint64_t pass){
 if(active_)return handle==handle_ && pass==pass_ && available();
 if(!current_(pass) || !process_.ready() || handle>=256)return false;
 const auto* p=pool_->slot(handle);if(!p || !p->nativeGeneration || !p->resource || !p->resource->draw || !p->resource->bank || !p->sourceBuffers)return false;
 handle_=handle;pass_=pass;generation_=p->nativeGeneration;resource_=p->resource;buffers_=p->sourceBuffers;phase_=0;active_=true;return true;
}
bool AnimationMeshPublication::word(uint32_t a,uint32_t& v){if(!available() || !process_.vm().memory.readWord(a,v))return false;++reads_;return true;}
bool AnimationMeshPublication::real(uint32_t a,float& f){uint32_t v;if(!word(a,v))return false;std::memcpy(&f,&v,4);return std::isfinite(f);}
bool AnimationMeshPublication::byte(uint32_t a,uint8_t& v){if(!available() || !process_.vm().memory.readByte(a,v))return false;++reads_;return true;}
bool AnimationMeshPublication::domain() const{
 if(!pose_ || counts_[0]!=pose_->positions.size() || counts_[1]!=pose_->normals.size() || counts_[2]!=pose_->visibility.size() || counts_[3]!=pose_->nodes.size() || counts_[4]!=pose_->textures.size() || (!pose_->surfaces.empty() && pose_->surfaces.size()!=counts_[4]) || pose_->groups.empty() || pose_->groups.size()>128 || pose_->bindings.size()>8192)return false;
 for(unsigned c=0;c<5;++c)if(uint64_t(counts_[c])*Widths[c]>0x180000 || uint64_t(outputs_[c])+uint64_t(counts_[c])*Widths[c]>0x100000000ull)return false;
 for(const auto& g:pose_->groups)if(uint64_t(g.node)+24>counts_[3] || (!pose_->visibility.empty() && g.visible>=counts_[2]) || g.joint>1)return false;
 for(const auto& b:pose_->bindings)if(b.group>=pose_->groups.size() || b.position>=counts_[0] || b.texture>=counts_[4])return false;
 return true;
}
bool AnimationMeshPublication::prepare(){
 if(index_>=resource_->names.size())return false;
 pose_=resource_->bank->pose(resource_->names[index_].c_str());if(!domain() || !process_.files_->modelGeometryCurrent(process_.vm().memory,model_,resource_->draw))return false;
 candidate_.positions.resize(counts_[0]);candidate_.normals.resize(counts_[1]);candidate_.visibility.resize(counts_[2]);candidate_.nodes.resize(counts_[3]);candidate_.textures.resize(counts_[4]);candidate_.surfaces=pose_->surfaces;channel_=item_=lane_=0;return true;
}
bool AnimationMeshPublication::capture(unsigned& visits,unsigned budget){
 const uint32_t r=aliases_.poseAddress(handle_);
 // Only reads/current checks run here. Never hold this lease through static
 // geometry admission, assembly, source evaluation or a caller-visible yield.
 return process_.vm().memory.withRawRamAccess([&](auto& ram){
 if(!process_.ready())return false;
 captureReady_=true;
 struct ResetReady {bool& ready;bool& fast;~ResetReady(){fast=false;ready=false;}} resetReady{captureReady_,fastCapture_};
 // This slice only reads raw guest data and writes private native storage.
 // Exclude every VM alias of that storage, not only current channel sources.
 // Re-prove on each borrow; scalar fallback keeps its precise read frontier.
 uint32_t guardWork=0;
 if(phase_==20 && privateCapture_ && available() && ram.bind(0x80415dc0,4) && ram.readWord(0x80415dc0,guardWork) && ram.bind(guardWork,24) && ram.disjointStorage(this,sizeof(*this))){
  auto separate=[&](const auto& v){return ram.disjointStorage(v.data(),v.size()*sizeof(v[0]));};
  bool raw=separate(candidate_.positions) && separate(candidate_.normals) && separate(candidate_.visibility) && separate(candidate_.nodes) && separate(candidate_.textures) && separate(candidate_.surfaces);
  for(unsigned c=0;c<5 && raw;++c)if(counts_[c])raw=ram.bind(outputs_[c],counts_[c]*Widths[c]);
  if(raw){fastCapture_=true;++fastSlices_;}
 }

 for(;visits<budget;++visits){
  if(phase_==15 || phase_==30 || phase_==31)return true;
  // Each word/byte primitive checks fresh ownership. Pure cursor transitions
  // have no external effects and need no duplicate check in this read slice.
  switch(phase_){
 case 1:if(!word(0x80415dc0,work_))return false;++phase_;break;
 case 2:if(!word(work_,models_))return false;++phase_;break;
 case 3:if(!word(r+0x10,record_))return false;++phase_;break;
 case 4:if(!word(models_+record_*16u+8,entry_))return false;++phase_;break;
 case 5:if(!word(entry_+0xa0,holder_))return false;++phase_;break;
 case 6:if(!word(holder_,model_))return false;++phase_;break;
 case 7:if(!word(r+0x14,index_))return false;++phase_;break;
 case 8:if(!word(work_+0xe8,selector_))return false;channel_=0;++phase_;break;
 case 9:if(!word(model_+Counts[channel_],counts_[channel_]))return false;if(++channel_==5){channel_=0;++phase_;}break;
 case 10:if(!word(channel_<2 && selector_?work_+0xf8+4*channel_:r+Outputs[channel_],outputs_[channel_]))return false;if(++channel_==5)++phase_;break;
 case 11:{uint32_t value;if(!word(r+0x3c,value))return false;candidate_.last=value;++phase_;break;}
 case 12:{uint32_t value;if(!word(r+0x2c,value))return false;candidate_.next=value;++phase_;break;}
 case 13:{uint32_t v;if(!word(r+0x40,v))return false;std::memcpy(&candidate_.fraction,&v,4);++phase_;break;}
 case 14:{uint32_t v;if(!word(r+0x24,v))return false;std::memcpy(&candidate_.selectedTime,&v,4);++phase_;break;}
 case 20:if(channel_==5){phase_=30;break;}if(item_==counts_[channel_]){++channel_;item_=lane_=0;break;}
  if(channel_<2){auto& v=channel_?candidate_.normals:candidate_.positions;if(!real(outputs_[channel_]+12*item_+4*lane_,v[item_][lane_]))return false;if(++lane_==3){lane_=0;++item_;}}
  else if(channel_==2){if(!byte(outputs_[2]+item_,candidate_.visibility[item_]))return false;++item_;}
  else if(channel_==3){if(!real(outputs_[3]+4*item_,candidate_.nodes[item_]))return false;++item_;}
  else if(!lane_){if(!byte(outputs_[4]+24*item_,candidate_.textures[item_]))return false;lane_=1;}
  else{float value;if(!real(outputs_[4]+24*item_+4*lane_,value))return false;if(!candidate_.surfaces.empty())candidate_.surfaces[item_].transform[lane_-1]=value;if(++lane_==6){lane_=0;++item_;}}break;
 default:return false;
  }
 }return true;
 });
}
bool AnimationMeshPublication::advance(uint32_t handle,uint64_t pass,unsigned budget){
 if(!budget || !reserve(handle,pass) || !available())return false;
 if(phase_==0){if(!process_.main_ || !process_.main_->advance(handle,pass,budget))return false;phase_=1;return false;}
 unsigned visits=0;
 while(visits<budget){
  if(!available())return false;
  if(phase_==15){if(!prepare())return false;phase_=20;++visits;continue;}
 if(phase_==30){if(!domain() || !pose_->assemble(candidate_) || candidate_.vertices.size()>12000 || candidate_.colors.size()!=candidate_.vertices.size())return false;for(const auto& v:candidate_.vertices)if(!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z) || !std::isfinite(v.u) || !std::isfinite(v.v))return false;
  phase_=31;return false; // Assembly is never repeated after success.
 }
 if(phase_==31){if(!process_.files_->modelGeometryCurrent(process_.vm().memory,model_,resource_->draw))return false;auto* target=const_cast<BattlePose*>(pool_->slot(handle_));std::swap(target->shape,candidate_);++publications_;active_=false;resource_.reset();buffers_.reset();pose_=nullptr;return true;}

  if(!capture(visits,budget))return false;
 }
 return false;
}
bool AnimationStartupProcess::reservePresentation(uint32_t handle,uint64_t pass){return !entryResource_ && !selectionPending() && presentation_ && (presentationPending() || !mainPending()) && presentation_->reserve(handle,pass);}
bool AnimationStartupProcess::present(uint32_t handle,uint64_t pass,unsigned budget){return !entryResource_ && !selectionPending() && presentation_ && (presentationPending() || !mainPending()) && presentation_->advance(handle,pass,budget);}
}
