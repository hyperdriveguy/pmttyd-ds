#include "field_pose_pool.hpp"
#include "animation_texture.hpp"
namespace ttyd {
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
bool FieldPosePool::animationReady(const std::string& model,const std::string& animation) const {
 const auto found=resources_.find(model);if(found==resources_.end())return false;
 const auto& r=*found->second;const char* selected=r.names.front().c_str();
 for(const auto& name:r.names)if(name==animation){selected=name.c_str();break;}
 return r.bank->pose(selected)!=nullptr;
}
bool FieldPosePool::canEnter(const std::string& model,int32_t mode) const {
 if(mode<0 || mode>2)return false;
 const auto found=resources_.find(model);if(found==resources_.end())return false;
 const auto& resource=*found->second;
 bool freePose=false;for(unsigned i=0;i<256;++i){const auto* p=poses_.slot(i);if(!p || !p->word(0)){freePose=true;break;}}if(!freePose)return false;
 bool freeModel=false;for(const auto& m:poses_.models()){if(!m.flags)freeModel=true;else if(m.resource->modelName==resource.modelName)return true;}
 if(!freeModel)return false;
 for(const auto& t:poses_.textures())if(!t.flags || t.name==resource.textureName)return true;
 return false;
}
std::optional<int32_t> FieldPosePool::enter(const std::string& model,int32_t mode,BattlePoseClock clock){
 if(!canEnter(model,mode))return {};
 return poses_.enter(resources_.at(model),mode,clock,model.c_str());
}
}
