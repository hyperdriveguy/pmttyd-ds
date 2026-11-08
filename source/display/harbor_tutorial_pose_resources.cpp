#include "harbor_tutorial_pose_resources.hpp"
#include "animation_texture.hpp"
namespace ttyd {
namespace {
constexpr unsigned ModelFlags=0xc4, DescriptorCount=0x148;
constexpr unsigned PositionCount=0xf0, NormalCount=0xf8, NodeCount=0x140, VisibilityCount=0x13c;
constexpr unsigned TextureHeaderCount=0x134, TextureHeaders=0x198, TextureHeaderStride=64;
bool valid(const BattlePoseResource& r,const char* model,const char* texture,const char* initial){
    if(r.modelName!=model || r.textureName!=texture || !r.draw || !r.textures || !r.bank || !r.modelFlags ||
       r.names.empty() || r.names.front()!=initial || r.names.size()!=r.discrete.size())return false;
    const auto& d=*r.draw;
    if(d.modelName()!=model || d.textureName()!=texture || d.word(ModelFlags)!=*r.modelFlags || d.word(DescriptorCount)!=r.names.size())return false;
    for(size_t i=0;i<r.names.size();++i){
        if(d.descriptor(uint32_t(i))!=r.names[i])return false;
        if(const auto* p=r.bank->pose(r.names[i].c_str())){
            if(p->positions.size()!=d.word(PositionCount) || p->normals.size()!=d.word(NormalCount) ||
               p->nodes.size()!=d.word(NodeCount) || p->visibility.size()!=d.word(VisibilityCount) || p->groups.size()!=d.groups())return false;
        }
    }
    const auto* first=r.bank->pose(initial);ActorAnimationSample sample;
    if(!first || !first->evaluate(0,sample) || sample.positions.empty() || sample.nodes.empty() || sample.world.empty())return false;
    for(uint32_t i=0;i<d.word(TextureHeaderCount);++i){
        const auto header=d.word(TextureHeaders)+i*TextureHeaderStride;
        if(d.word(header+8)!=0 || !r.textures->image(d.word(header+4)))return false;
    }
    return true;
}
}
std::shared_ptr<const HarborTutorialPoseResources> HarborTutorialPoseResources::load(const char* root){
    if(!root)return {};
    const std::string prefix=root;
    auto load=[&](const char* stem){const auto path=prefix+stem;return loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());};
    return admit(load("trio-attack-mario"),load("harbor-tutorial-crump"),load("trio-attack-goombella"));
}
std::shared_ptr<const HarborTutorialPoseResources> HarborTutorialPoseResources::admit(
    std::shared_ptr<const BattlePoseResource> mario,std::shared_ptr<const BattlePoseResource> crump,std::shared_ptr<const BattlePoseResource> helper){
    if(!mario || !crump || !helper || !valid(*mario,"b_mario","a_mario","M_Z_1") ||
       !valid(*crump,"c_kanbu","c_kanbu","Z_1") || !valid(*helper,"c_pkr","c_pkr","PKR_Z_1"))return {};
    auto owner=std::shared_ptr<HarborTutorialPoseResources>(new HarborTutorialPoseResources);
    owner->resources_={std::move(mario),std::move(crump),std::move(helper)};return owner;
}
std::shared_ptr<const BattlePoseResource> HarborTutorialPoseResources::find(const std::string& model) const{
    for(const auto& resource:resources_)if(resource->modelName==model)return resource;
    return {};
}
}
