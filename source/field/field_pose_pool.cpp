#include "field_pose_pool.hpp"
#include "animation_texture.hpp"
namespace ttyd {
// MODEL header span1b0(hex):+c4 flags,+148 descriptor count,+134 texture
// record count,+198 table of64-byte records. Type0 texture records use TPL
// indices. Descriptor0 must actually evaluate with positions/nodes/world data.
// A repeated model name is accepted only for the same shared resource owner.
bool FieldPosePool::install(std::shared_ptr<const BattlePoseResource> r){
 if(!r || r->modelName.empty() || r->textureName.empty() || !r->bank || !r->draw || !r->textures || !r->modelFlags || r->names.empty() || r->discrete.size()!=r->names.size())return false;
 if(!r->draw->span(0,1,0x1b0))return false;
 if(r->draw->modelName()!=r->modelName || r->draw->textureName()!=r->textureName || r->draw->word(0xc4)!=r->modelFlags || r->draw->word(0x148)!=r->names.size())return false;
 for(unsigned i=0;i<r->names.size();++i)if(r->draw->descriptor(i)!=r->names[i])return false;
 const auto* initial=r->bank->pose(r->names.front().c_str());ActorAnimationSample shape;
 if(!initial || !initial->evaluate(0,shape) || shape.positions.empty() || shape.nodes.empty() || shape.world.empty())return false;
 for(uint32_t i=0;i<r->draw->word(0x134);++i){const auto h=r->draw->word(0x198)+64*i;if(r->draw->word(h+8)==0 && r->draw->word(h+4)>=r->textures->count())return false;}
 const auto existing=resources_.find(r->modelName);if(existing!=resources_.end())return existing->second==r;
 const auto name=r->modelName;resources_.emplace(name,std::move(r));return true;
}
bool FieldPosePool::load(const char* directory,const char* pack){return install(loadBattlePoseResource(directory,pack));}
// Unknown animation falls back to descriptor0, matching selection; readiness
// is not proof that the requested name was present in the original bank.
bool FieldPosePool::animationReady(const std::string& model,const std::string& animation) const {
 const auto found=resources_.find(model);if(found==resources_.end())return false;
 const auto& r=*found->second;const char* selected=r.names.front().c_str();
 for(const auto& name:r.names)if(name==animation){selected=name.c_str();break;}
 return r.bank->pose(selected)!=nullptr;
}
// Modes0..2 only; shared BattlePosePool has256 pose slots. Existing model
// already owns its texture, so its free-pose path needs no new model/TPL slot.
bool FieldPosePool::canEnter(const std::string& model,int32_t mode) const {
 if(!poses_->cacheMetadataReady() || mode<0 || mode>2)return false;
 const auto found=resources_.find(model);if(found==resources_.end())return false;
 const auto& resource=*found->second;
 bool freePose=false;for(unsigned i=0;i<256;++i){const auto* p=poses_->slot(i);if(!p || !p->word(0)){freePose=true;break;}}if(!freePose)return false;
 bool freeModel=false;for(const auto& m:poses_->models()){if(!m.flags())freeModel=true;else if(m.resource->modelName==resource.modelName)return m.references()<0x7fffffff;}
 if(!freeModel)return false;
 bool freeTexture=false;for(const auto& t:poses_->textures()){if(!t.flags())freeTexture=true;else if(t.name==resource.textureName)return t.references()<0x7fffffff;}
 return freeTexture;
}
// Returned handle belongs to injected global namespace when supplied. Other
// roles consume its256 slots too; do not treat it as field-local actor index.
std::optional<int32_t> FieldPosePool::enter(const std::string& model,int32_t mode,BattlePoseClock clock){
 if(!canEnter(model,mode))return {};
 return poses_->enter(resources_.at(model),mode,clock,model.c_str());
}
}
