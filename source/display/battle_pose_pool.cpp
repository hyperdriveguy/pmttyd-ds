#include "battle_pose_pool.hpp"
#include <cassert>
#include <cstring>
#include <map>

namespace ttyd {
namespace {
bool readName(FILE* file,std::string& out){
    std::array<char,64> bytes{};
    if(std::fread(bytes.data(),bytes.size(),1,file)!=1 || !bytes[0] || !std::memchr(bytes.data(),0,bytes.size()))return false;
    const auto length=std::strlen(bytes.data());
    for(size_t i=0;i<length;++i)if(static_cast<unsigned char>(bytes[i])>127)return false;
    for(size_t i=length;i<bytes.size();++i)if(bytes[i])return false;
    out.assign(bytes.data(),length);return true;
}
uint64_t clockFor(const BattlePose& pose,BattlePoseClock clock){return pose.word(0xc)?clock.modeNonzero:clock.modeZero;}
float rounded(float value){volatile float single=value;return single;}
}
std::shared_ptr<const BattlePoseResource> loadBattlePoseResource(const char* directory,const char* pack){
    if(!directory || !pack)return {};
    // RomFS packs are immutable during a native run. Renderer and battle owners
    // share one validated bank; this cache adds no independent strong lifetime.
    static std::map<std::pair<std::string,std::string>,std::weak_ptr<const BattlePoseResource>> cache;
    const auto key=std::make_pair(std::string(directory),std::string(pack));
    const auto cached=cache.find(key);if(cached!=cache.end())if(auto resource=cached->second.lock())return resource;
    FILE* file=std::fopen(directory,"rb");if(!file)return {};
    auto resource=std::make_shared<BattlePoseResource>();std::array<char,8> magic{};std::array<uint8_t,4> countBytes{};
    bool ok=std::fread(magic.data(),8,1,file)==1 && !std::memcmp(magic.data(),"PMPOSE01",8) && readName(file,resource->modelName) && readName(file,resource->textureName) && std::fread(countBytes.data(),4,1,file)==1;
    const uint32_t count=uint32_t(countBytes[0])|uint32_t(countBytes[1])<<8|uint32_t(countBytes[2])<<16|uint32_t(countBytes[3])<<24;
    // Bounded native sidecar admission; not an original model count restriction.
    ok=ok && count>0 && count<=4096;
    for(uint32_t i=0;ok && i<count;++i){std::string name;ok=readName(file,name);if(ok)resource->names.push_back(std::move(name));}
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);if(!ok)return {};
    auto bank=std::make_shared<ActorAnimationPack>();if(!bank->load(pack))return {};
    const auto* initial=bank->pose(resource->names.front().c_str());ActorAnimationSample shape;
    if(!initial || !initial->evaluate(0,shape) || shape.positions.empty() || shape.nodes.empty() || shape.world.empty())return {};
    resource->bank=std::move(bank);cache[key]=resource;return resource;
}
uint32_t BattlePose::word(unsigned offset) const {assert(offset+4<=state.size());return uint32_t(state[offset])<<24|uint32_t(state[offset+1])<<16|uint32_t(state[offset+2])<<8|state[offset+3];}
void BattlePose::word(unsigned offset,uint32_t value){assert(offset+4<=state.size());for(unsigned i=0;i<4;++i)state[offset+i]=uint8_t(value>>(24-8*i));}
float BattlePose::scalar(unsigned offset) const {const auto bits=word(offset);float value;std::memcpy(&value,&bits,4);return value;}
void BattlePose::scalar(unsigned offset,float value){uint32_t bits;std::memcpy(&bits,&value,4);word(offset,bits);}
BattlePose* BattlePosePool::pose(int32_t handle){return handle>=0 && unsigned(handle)<poses_.size() && poses_[unsigned(handle)] && poses_[unsigned(handle)]->word(0)?poses_[unsigned(handle)].get():nullptr;}
const BattlePose* BattlePosePool::pose(int32_t handle) const {return const_cast<BattlePosePool*>(this)->pose(handle);}
void BattlePosePool::restart(BattlePose& pose,BattlePoseClock clock){
    const auto timestamp=clockFor(pose,clock);pose.word(0x1c,uint32_t(timestamp));pose.word(0x18,uint32_t(timestamp>>32));
    pose.word(0x3c,0xffffffff);pose.scalar(0x40,0);pose.word(0x80,0);pose.scalar(0x84,0);
}
std::optional<int32_t> BattlePosePool::enter(std::shared_ptr<const BattlePoseResource> supplied,int32_t mode,BattlePoseClock clock){
    if(mode<0 || mode>2)return {};
    archiveType_=mode==1?2:mode==0?1:0;
    if(!supplied || supplied->modelName.empty() || supplied->textureName.empty() || supplied->names.empty() || !supplied->bank)return {};
    unsigned handle=0;while(handle<poses_.size() && poses_[handle] && poses_[handle]->word(0))++handle;if(handle==poses_.size())return {};
    unsigned model=0;while(model<models_.size() && (!models_[model].flags || models_[model].resource->modelName!=supplied->modelName))++model;
    const bool reused=model<models_.size();
    if(!reused){model=0;while(model<models_.size() && models_[model].flags)++model;if(model==models_.size())return {};}
    const auto resource=reused?models_[model].resource:std::move(supplied);
    unsigned texture=reused?models_[model].texture:0;
    bool reusedTexture=reused;
    if(!reused){
        while(texture<textures_.size() && (!textures_[texture].flags || textures_[texture].name!=resource->textureName))++texture;
        reusedTexture=texture<textures_.size();
        if(!reusedTexture){texture=0;while(texture<textures_.size() && textures_[texture].flags)++texture;if(texture==textures_.size())return {};}
    }
    const auto* initial=resource->bank->pose(resource->names.front().c_str());ActorAnimationSample shape;
    if(!initial || !initial->evaluate(0,shape) || shape.positions.empty() || shape.nodes.empty() || shape.world.empty())return {}; // Visibility does not determine buffer ownership.
    if(!poses_[handle])poses_[handle]=std::make_unique<BattlePose>();
    auto& pose=*poses_[handle];
    if(!reused){
        if(!reusedTexture){auto& t=textures_[texture];t.flags=1;t.name=resource->textureName;t.resource=resource;}
        ++textures_[texture].references;models_[model].flags=1;models_[model].texture=texture;models_[model].resource=resource;
    }
    ++models_[model].references;pose.resource=resource;pose.shape=std::move(shape);pose.archiveType=archiveType_;
    pose.word(0,1);pose.word(4,0);pose.word(8,0);pose.word(0xc,uint32_t(mode));pose.word(0x10,model);pose.word(0x14,0);restart(pose,clock);
    for(unsigned offset:{0x70u,0x74u,0x7cu})pose.scalar(offset,0);
    pose.word(0x90,0xffffffff);for(unsigned offset:{0xe0u,0xe4u,0xe8u,0xecu,0x154u,0x158u})pose.word(offset,0);
    pose.word(0xf0,0xffffffff);pose.word(0xf4,0xffffffff);pose.scalar(0xf8,1);pose.scalar(0xfc,1);pose.scalar(0x100,0);pose.scalar(0x44,1);
    for(unsigned row=0;row<3;++row)for(unsigned column=0;column<4;++column)pose.scalar(0x124+16*row+4*column,row==column?1:0);
    return int32_t(handle);
}
bool BattlePosePool::setRate(int32_t handle,float rate){auto* p=pose(handle);if(!p)return false;p->scalar(0x44,rate);return true;}
bool BattlePosePool::selectName(int32_t handle,const std::string& name,bool force,BattlePoseClock clock){
    auto* p=pose(handle);if(!p)return false;const auto& names=p->resource->names;uint32_t index=0;
    for(size_t i=0;i<names.size();++i)if(names[i]==name){index=uint32_t(i);break;}
    return selectIndex(handle,index,force,clock);
}
bool BattlePosePool::selectIndex(int32_t handle,uint32_t index,bool force,BattlePoseClock clock){
    auto* p=pose(handle);if(!p)return false;if(index>=p->resource->names.size())index=0;
    if(!force && index==p->word(0x14))return true;
    p->word(0x14,index);
    if(p->word(0x90)!=0xffffffff && (p->word(0)&2))p->word(0,p->word(0)|4);
    restart(*p,clock);return true;
}
bool BattlePosePool::evaluate(int32_t handle,BattlePoseClock clock){
    auto* p=pose(handle);if(!p)return false;
    const auto index=p->word(0x14);if(index>=p->resource->names.size())return false;
    const auto* animation=p->resource->bank->pose(p->resource->names[index].c_str());if(!animation)return false;
    const uint64_t elapsed=clockFor(*p,clock)-p->timestamp();
    const float frames=rounded(rounded(rounded(float(elapsed))*60.f)/1000.f);
    p->scalar(0x20,rounded(frames*p->scalar(0x44)));
    // CPU sampler/cache is an explicit ordinary conversion adapter, not source
    // GX/paired-single Main. Repeated calls at one clock do not add elapsed ticks.
    return animation->evaluate(p->scalar(0x20),p->shape);
}
bool BattlePosePool::clearEffect(int32_t handle){auto* p=pose(handle);if(!p || p->word(0x90)!=0xffffffff)return false;p->word(0x90,0xffffffff);p->word(0xec,p->word(0xec)&~1u);return true;}
bool BattlePosePool::release(int32_t handle){
    auto* p=pose(handle);if(!p)return false;
    if(p->word(4)==0 && (p->word(0)&0xc0))return false; // Actual smart-buffer providers are required.
    const auto modelIndex=p->word(0x10);
    if(modelIndex>=models_.size())return false;
    auto& model=models_[modelIndex];
    if(!model.flags || !model.references || model.resource!=p->resource || model.texture>=textures_.size() || !textures_[model.texture].references)return false;
    if(p->word(4)!=0){const auto remaining=p->word(8)-1;p->word(8,remaining);if(remaining<0x80000000u)return true;}
    --model.references;
    if(model.references==0){
        model.flags=0;auto& texture=textures_[model.texture];--texture.references;
        if(texture.references==0){texture.resource.reset();texture.flags=0;}
        model.resource.reset();
    }
    p->word(0,0);p->resource.reset();p->shape=ActorAnimationSample{};return true;
}
}
