#include "trio_enemy_pose_resources.hpp"
#include "animation_texture.hpp"
#include <algorithm>
namespace ttyd {
std::shared_ptr<const TrioEnemyPoseResources> TrioEnemyPoseResources::load(const char* directory,const char* pack){
    return admit(loadBattlePoseResource(directory,pack));
}
std::shared_ptr<const TrioEnemyPoseResources> TrioEnemyPoseResources::admit(std::shared_ptr<const BattlePoseResource> r){
    if(!r || r->modelName!="c_kuribo" || r->textureName!="c_kuribo" || !r->bank || !r->draw || !r->textures || !r->modelFlags)return {};
    const auto& d=*r->draw;
    if(d.modelName()!=r->modelName || d.textureName()!=r->textureName || d.word(0xc4)!=*r->modelFlags || r->names.empty() || d.word(0x148)!=r->names.size() || r->discrete.size()!=r->names.size())return {};
    // Full source descriptor coverage is required, including original index0.
    // Positions/normals/nodes/visibility/groups remain original source domains.
    for(size_t i=0;i<r->names.size();++i){
        const auto* p=r->bank->pose(r->names[i].c_str());
        if(d.descriptor(uint32_t(i))!=r->names[i] || !p || p->positions.size()!=d.word(0xf0) || p->normals.size()!=d.word(0xf8) || p->nodes.size()!=d.word(0x140) || p->visibility.size()!=d.word(0x13c) || p->groups.size()!=d.groups())return {};
    }
    for(uint32_t i=0;i<d.word(0x134);++i){
        const auto header=d.word(0x198)+64*i;
        if(d.word(header+8)!=0 || !r->textures->image(d.word(header+4)))return {};
    }
    auto result=std::shared_ptr<TrioEnemyPoseResources>(new TrioEnemyPoseResources);
    result->resource_=std::move(r);return result;
}
std::shared_ptr<const BattlePoseResource> TrioEnemyPoseResources::find(const std::string& model,const std::string& initialName) const{
    if(model!=resource_->modelName || std::find(resource_->names.begin(),resource_->names.end(),initialName)==resource_->names.end())return {};
    return resource_;
}
}
