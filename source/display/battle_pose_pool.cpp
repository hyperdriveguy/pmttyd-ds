#include "battle_pose_pool.hpp"
#include "animation_texture.hpp"
#include "animation_sample_time.hpp"
#include <cassert>
#include <cstring>
#include <map>

namespace ttyd {
namespace {
// Native sidecar names64 bytes:nonempty ASCII<=127,NUL-terminated,zero tail.
// Directory words little-endian; retained original pose scalar state big-endian.
bool readName(FILE* file,std::string& out){
    std::array<char,64> bytes{};
    if(std::fread(bytes.data(),bytes.size(),1,file)!=1 || !bytes[0] || !std::memchr(bytes.data(),0,bytes.size()))return false;
    const auto length=std::strlen(bytes.data());
    for(size_t i=0;i<length;++i)if(static_cast<unsigned char>(bytes[i])>127)return false;
    for(size_t i=length;i<bytes.size();++i)if(bytes[i])return false;
    out.assign(bytes.data(),length);return true;
}
bool readWord(FILE* file,uint32_t& out){
    std::array<uint8_t,4> b{};if(std::fread(b.data(),4,1,file)!=1)return false;
    out=uint32_t(b[0])|uint32_t(b[1])<<8|uint32_t(b[2])<<16|uint32_t(b[3])<<24;return true;
}
uint64_t clockFor(const BattlePose& pose,BattlePoseClock clock){return pose.word(0xc)?clock.modeNonzero:clock.modeZero;}
float rounded(float value){volatile float single=value;return single;}
}
// PMPOSE01 directory identities;02 adds model flags and per-descriptor
// discrete timelines. Native limits4096 descriptors/65536 keys per timeline.
// Model TestXLU uses original MODEL+c4 mask8, not animation colors or pose flags.
std::shared_ptr<const BattlePoseResource> loadBattlePoseResource(const char* directory,const char* pack){
    if(!directory || !pack)return {};
    // RomFS packs are immutable during a native run. Renderer and battle owners
    // share one validated bank; this cache adds no independent strong lifetime.
    static std::map<std::pair<std::string,std::string>,std::weak_ptr<const BattlePoseResource>> cache;
    const auto key=std::make_pair(std::string(directory),std::string(pack));
    const auto cached=cache.find(key);if(cached!=cache.end())if(auto resource=cached->second.lock())return resource;
    FILE* file=std::fopen(directory,"rb");if(!file)return {};
    auto resource=std::make_shared<BattlePoseResource>();std::array<char,8> magic{};std::array<uint8_t,4> countBytes{};
    bool ok=std::fread(magic.data(),8,1,file)==1;
    const bool metadata=ok && !std::memcmp(magic.data(),"PMPOSE02",8);
    ok=ok && (metadata || !std::memcmp(magic.data(),"PMPOSE01",8)) && readName(file,resource->modelName) && readName(file,resource->textureName) && std::fread(countBytes.data(),4,1,file)==1;
    const uint32_t count=uint32_t(countBytes[0])|uint32_t(countBytes[1])<<8|uint32_t(countBytes[2])<<16|uint32_t(countBytes[3])<<24;
    // Bounded native sidecar admission; not an original model count restriction.
    ok=ok && count>0 && count<=4096;
    for(uint32_t i=0;ok && i<count;++i){std::string name;ok=readName(file,name);if(ok)resource->names.push_back(std::move(name));}
    if(ok && metadata){
        uint32_t flags=0;ok=readWord(file,flags);if(ok)resource->modelFlags=flags;
        for(uint32_t i=0;ok && i<count;++i){
            uint32_t keys=0;ok=readWord(file,keys) && keys<=65536;if(!ok)break;
            resource->discrete.emplace_back();auto& timeline=resource->discrete.back();
            for(uint32_t j=0;ok && j<keys;++j){
                uint32_t bits=0,value=0;ok=readWord(file,bits) && readWord(file,value);
                if(ok){float time;std::memcpy(&time,&bits,4);timeline.push_back({time,value});}
            }
        }
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);if(!ok)return {};
    auto bank=std::make_shared<ActorAnimationPack>();if(!bank->load(pack))return {};
    const auto* initial=bank->pose(resource->names.front().c_str());ActorAnimationSample shape;
    if(!initial || !initial->evaluate(0,shape) || shape.positions.empty() || shape.nodes.empty() || shape.world.empty())return {};
    resource->bank=std::move(bank);
    const std::string suffix=".pose-directory.bin",path=directory;
    if(path.size()>=suffix.size() && path.compare(path.size()-suffix.size(),suffix.size(),suffix)==0){
        const auto rawPath=path.substr(0,path.size()-suffix.size())+".draw-model.bin";
        if(FILE* raw=std::fopen(rawPath.c_str(),"rb")){
            auto draw=std::make_shared<AnimationDrawResource>();const bool valid=draw->read(raw);std::fclose(raw);
            if(!valid || draw->modelName()!=resource->modelName || draw->textureName()!=resource->textureName || draw->word(0xc4)!=resource->modelFlags || draw->word(0x148)!=resource->names.size())return {};
            for(uint32_t i=0;i<resource->names.size();++i){
                if(draw->descriptor(i)!=resource->names[i])return {};
                if(const auto* converted=resource->bank->pose(resource->names[i].c_str())){
                    if(converted->positions.size()!=draw->word(0xf0) || converted->normals.size()!=draw->word(0xf8) || converted->nodes.size()!=draw->word(0x140) || converted->visibility.size()!=draw->word(0x13c) || converted->groups.size()!=draw->groups())return {};
                }
            }
            resource->draw=std::move(draw);
        }
        const auto texturePath=path.substr(0,path.size()-suffix.size())+".draw-textures.bin";
        if(FILE* raw=std::fopen(texturePath.c_str(),"rb")){
            auto textures=std::make_shared<AnimationTextureResource>();const bool valid=textures->read(raw);std::fclose(raw);if(!valid)return {};
            if(resource->draw)for(uint32_t i=0;i<resource->draw->word(0x134);++i){const auto h=resource->draw->word(0x198)+64*i;if(resource->draw->word(h+8)==0 && resource->draw->word(h+4)>=textures->count())return {};}
            resource->textures=std::move(textures);
        }
    }
    cache[key]=resource;return resource;
}
uint32_t BattlePose::word(unsigned offset) const {assert(offset+4<=state.size());return uint32_t(state[offset])<<24|uint32_t(state[offset+1])<<16|uint32_t(state[offset+2])<<8|state[offset+3];}
void BattlePose::word(unsigned offset,uint32_t value){assert(offset+4<=state.size());for(unsigned i=0;i<4;++i)state[offset+i]=uint8_t(value>>(24-8*i));}
float BattlePose::scalar(unsigned offset) const {const auto bits=word(offset);float value;std::memcpy(&value,&bits,4);return value;}
void BattlePose::scalar(unsigned offset,float value){uint32_t bits;std::memcpy(&bits,&value,4);word(offset,bits);}
BattlePose* BattlePosePool::pose(int32_t handle){return handle>=0 && unsigned(handle)<poses_.size() && poses_[unsigned(handle)] && poses_[unsigned(handle)]->word(0)?poses_[unsigned(handle)].get():nullptr;}
const BattlePose* BattlePosePool::pose(int32_t handle) const {return const_cast<BattlePosePool*>(this)->pose(handle);}
std::optional<bool> BattlePosePool::testTranslucent(int32_t handle) const {
    const auto* p=pose(handle);if(!p || !p->resource->modelFlags)return {};
    return (*p->resource->modelFlags&8)!=0;
}
// Timestamp low then high source stores at1c/18;3c=-1 invalidates cumulative
// key cache,40 clears fraction,80/84 clears discrete value/loop ratio.
void BattlePosePool::restart(BattlePose& pose,BattlePoseClock clock){
    const auto timestamp=clockFor(pose,clock);pose.word(0x1c,uint32_t(timestamp));pose.word(0x18,uint32_t(timestamp>>32));
    pose.word(0x3c,0xffffffff);pose.scalar(0x40,0);pose.word(0x80,0);pose.scalar(0x84,0);
}
// Native source-sized pools256 poses,64 models,64 textures. Models share by
// original modelName; texture names share independently. Texture references
// count MODEL owners, model references count POSES. Mode0/1/2 selects archive
// type1/2/0; any nonzero mode uses the separate event-time clock.
std::optional<int32_t> BattlePosePool::enter(std::shared_ptr<const BattlePoseResource> supplied,int32_t mode,BattlePoseClock clock,const char* requestedModel){
    if(mode<0 || mode>2)return {};
    archiveType_=mode==1?2:mode==0?1:0;
    if(!supplied || supplied->modelName.empty() || supplied->textureName.empty() || supplied->names.empty() || !supplied->bank)return {};
    const std::string request=requestedModel?requestedModel:supplied->modelName;
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
    allocateActorAnimationCache(*initial,pose.cache);
    pose.word(0,1);pose.word(4,0);pose.word(8,0);pose.word(0xc,uint32_t(mode));pose.word(0x10,model);pose.word(0x14,0);restart(pose,clock);
    for(unsigned offset:{0x70u,0x74u,0x7cu})pose.scalar(offset,0);
    //90=-1 unlinked output/effect sentinel;f0/f4=-1 optional handles. Matrix
    //124 is3x4 identity at16-byte row/4-byte column stride. f8/fc/44=scale/
    //scale/rate1 defaults; unidentified scalar/flag words remain explicit offsets.
    pose.word(0x90,0xffffffff);for(unsigned offset:{0xe0u,0xe4u,0xe8u,0xecu})pose.word(offset,0);
    pose.word(0xf0,0xffffffff);pose.word(0xf4,0xffffffff);pose.scalar(0xfc,1);pose.scalar(0xf8,1);pose.scalar(0x100,0);pose.scalar(0x44,1);
    for(unsigned row=0;row<3;++row)for(unsigned column=0;column<4;++column)pose.scalar(0x124+16*row+4*column,row==column?1:0);
    // Source Entry uses its original request, not a substituted cache resource.
    uint32_t type=0,pointer=0x80415dc4;
    if(request=="tst_vivi"){type=1;pointer=0x80415da0;}
    else if(request=="c_vivian"){type=1;pointer=0x80415da4;}
    else if(request=="c_maririn"){type=2;pointer=0x80415da8;}
    else if(request=="c_majyorin"){type=3;pointer=0x80415dac;}
    pose.word(0x11c,type);pose.word(0x120,pointer);pose.word(0x154,0);pose.word(0x158,0);
    return int32_t(handle);
}
bool BattlePosePool::paperOn(int32_t handle){auto* p=pose(handle);if(!p)return false;p->word(0,p->word(0)|8);return true;}
bool BattlePosePool::materialLightFlagOn(int32_t handle,uint32_t flags){auto* p=pose(handle);if(!p)return false;p->word(0xec,p->word(0xec)|flags);return true;}
std::optional<int32_t> BattlePosePool::vivianType(int32_t handle) const {
    const auto* p=pose(handle);if(!p)return {};
    const auto v=p->word(0x11c);
    return v<=0x7fffffffu?int32_t(v):int32_t(int64_t(v)-0x100000000LL);
}
namespace {
std::optional<float> modelDimension(const BattlePose* p,uint32_t offset){
    if(!p || !p->resource->draw || !p->resource->draw->span(offset,1,4))return {};
    const auto raw=p->resource->draw->word(offset);
    const int32_t value=raw<=0x7fffffffu?int32_t(raw):int32_t(int64_t(raw)-0x100000000LL);
    return rounded(rounded(float(value))*2.f);
}
}
std::optional<float> BattlePosePool::height(int32_t handle) const{return modelDimension(pose(handle),0xcc);}
std::optional<float> BattlePosePool::radius(int32_t handle) const{return modelDimension(pose(handle),0xc8);}
bool BattlePosePool::setRate(int32_t handle,float rate){auto* p=pose(handle);if(!p)return false;p->scalar(0x44,rate);return true;}
// Ordinary admitted Main requires unlinked90=-1,private outputSelector0,
// genuine model flags/discrete data. Pose flag8 needs a missing facing provider;
// evaluating scalar/cache prefix alone does not certify its full Main tail.
bool BattlePosePool::main(int32_t handle,BattlePoseClock clock){
    auto* p=pose(handle);
    if(!p || p->word(0x90)!=0xffffffff || outputSelector || !p->resource->modelFlags || p->resource->discrete.size()!=p->resource->names.size())return false;
    if(!evaluate(handle,clock))return false;
    return !(p->word(0)&8); // A real optional-facing provider is required when admitted.
}
// Missing descriptor names/index overflow fall back to original descriptor0.
// Nonforced same-index selection preserves epoch; force restarts it.
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
// Timeline publications:20 elapsed frames,24 selected time,28/2c continuous
// predecessor/next,30 fraction,34/38 independent discrete predecessor/next,
//3c/40 cumulative cache history,80 discrete value,84 loop ratio,44 rate.
// Source frame bridge rounds float(elapsed)*60/1000 at each store. Failure can
// retain earlier source stores; this is a continuation prefix, not atomic sampling.
bool BattlePosePool::evaluate(int32_t handle,BattlePoseClock clock){
    auto* p=pose(handle);if(!p)return false;
    const auto index=p->word(0x14);if(index>=p->resource->names.size())return false;
    const auto* animation=p->resource->bank->pose(p->resource->names[index].c_str());if(!animation)return false;
    const uint64_t elapsed=clockFor(*p,clock)-p->timestamp();
    const float frames=rounded(rounded(rounded(float(elapsed))*60.f)/1000.f);
    p->scalar(0x20,frames);
    p->scalar(0x20,rounded(p->scalar(0x20)*p->scalar(0x44)));
    const auto plan=animationSampleTimePlan(p->scalar(0x20),animation->loop,animation->start,animation->end);
    p->scalar(0x84,plan.ratio);
    const auto selected=finishAnimationSampleTime(plan,p->scalar(0x84));if(!selected)return false;
    p->scalar(0x24,*selected);
    // Main scans the two independent authored timelines before interpolating.
    size_t continuous=0;while(continuous<animation->keys.size() && !(animation->keys[continuous].time>p->scalar(0x24)))++continuous;
    const int32_t current=int32_t(continuous)-1;
    p->word(0x28,uint32_t(current));p->word(0x2c,uint32_t(current+1));
    if(!animation->loop && continuous>=animation->keys.size())p->word(0x2c,p->word(0x28));
    if(!p->resource->discrete.empty()){
        if(index>=p->resource->discrete.size())return false;
        const auto& timeline=p->resource->discrete[index];size_t discrete=0;
        while(discrete<timeline.size() && !(timeline[discrete].time>p->scalar(0x24)))++discrete;
        p->word(0x34,uint32_t(int32_t(discrete)-1));p->word(0x38,uint32_t(discrete));
        if(!animation->loop && discrete>=timeline.size())p->word(0x38,p->word(0x34));
        if(discrete==0)return false; // Actual predecessor-1 provider remains unavailable.
        p->word(0x80,timeline[discrete-1].value);
    }
    if(current<0 || !std::isfinite(p->scalar(0x24)))return false;
    int32_t next=int32_t(p->word(0x2c));
    const float time=animation->keys[size_t(current)].time;
    float span=0;
    if(animation->loop && size_t(next)>=animation->keys.size()){
        next=0;p->word(0x2c,0);span=rounded(animation->end-time);
    }else span=rounded(animation->keys[size_t(next)].time-time);
    const float fraction=span==0?0:rounded(rounded(p->scalar(0x24)-time)/span);
    p->scalar(0x30,fraction);
    ActorAnimationCacheStores stores{p,[](void* u,int32_t v){static_cast<BattlePose*>(u)->word(0x3c,uint32_t(v));},
        [](void* u,float v){static_cast<BattlePose*>(u)->scalar(0x40,v);}};
    if(!sampleActorAnimationCache(*animation,p->cache,p->shape,current,next,fraction,int32_t(p->word(0x3c)),p->scalar(0x40),stores))return false;
    p->shape.selectedTime=p->scalar(0x24);
    return animation->assemble(p->shape);
}
bool BattlePosePool::clearEffect(int32_t handle){auto* p=pose(handle);if(!p || p->word(0x90)!=0xffffffff)return false;p->word(0x90,0xffffffff);p->word(0xec,p->word(0xec)&~1u);return true;}
// Original shared-pose word4 with count8 decrements first; nonnegative
// result keeps pose alive, underflow retires.0x80000000 tests signed sign bit.
// Smart-buffer flags0xc0 require a separate provider and hold release. Last
// model owner releases its texture reference; clear pose ownership last.
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
    p->word(0,0);p->resource.reset();p->shape=ActorAnimationSample{};p->cache=ActorAnimationCache{};return true;
}
}
