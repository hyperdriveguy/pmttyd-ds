#include "battle_startup_resources.hpp"
#include <cstdio>
#include <cstring>
#include <set>
#include <memory>
namespace ttyd {
namespace {
constexpr const char* Names[]={"p_b_st","p_jyabara","p_kaiten_h","EFF_nice_us","EFF_good_us","EFF_great_us","EFF_wonderful_us","EFF_excellent_us","EFF_lucky_us"};
constexpr uint32_t Keys[]={0x802efbe4,0x802efbec,0x802efbf8,0x802f3d00,0x802f3d0c,0x802f3d18,0x802f3d28,0x802f3d3c,0x802f3d50};
uint32_t fnv(const std::vector<uint8_t>& bytes){uint32_t hash=2166136261;for(auto b:bytes)hash=(hash^b)*16777619;return hash;}
bool be(const std::vector<uint8_t>& b,size_t offset,uint32_t& value){if(offset>b.size() || b.size()-offset<4)return false;value=uint32_t(b[offset])<<24|uint32_t(b[offset+1])<<16|uint32_t(b[offset+2])<<8|b[offset+3];return true;}
bool text(const char* raw,size_t n,std::string& result){const void* end=std::memchr(raw,0,n);if(!end)return false;result.assign(raw,static_cast<const char*>(end));for(unsigned char c:result)if(c<32 || c>126)return false;return !result.empty();}
bool sourceDescriptors(const BattleStartupResources::Resource& r){
    const auto& b=r.originalBank;uint32_t count=0,directory=0;
    if(b.size()<0x1b0 || !be(b,0x148,count) || count!=r.descriptors.size() || !be(b,0x1ac,directory) || directory>b.size() || count>(b.size()-directory)/64)return false;
    std::string name,owner;
    if(!text(reinterpret_cast<const char*>(b.data()+4),64,name) || name!=r.name || !text(reinterpret_cast<const char*>(b.data()+0x44),64,owner) || owner!=r.textureOwner)return false;
    for(unsigned i=0;i<count;++i){
        const auto& p=r.descriptors[i];const size_t row=directory+i*64;uint32_t header=0,loops=0,offset=0,loop=0,start=0,end=0,keys=0;std::string pose;
        if(!text(reinterpret_cast<const char*>(b.data()+row),60,pose) || pose!=p.name || !be(b,row+60,header) ||
           !be(b,size_t(header)+4,loops) || loops!=1 || !be(b,size_t(header)+0x24,offset) ||
           !be(b,size_t(header)+offset,loop) || !be(b,size_t(header)+offset+4,start) || !be(b,size_t(header)+offset+8,end) ||
           !be(b,size_t(header)+8,keys))return false;
        float sourceStart,sourceEnd;std::memcpy(&sourceStart,&start,4);std::memcpy(&sourceEnd,&end,4);
        if(loop!=p.loop || sourceStart!=p.start || sourceEnd!=p.end || keys!=p.keys)return false;
    }
    return true;
}
}
bool BattleStartupResources::load(const char* base){
    if(!base)return false;
    const std::string directory=base;
    FILE* file=std::fopen((directory+"/battle-startup-resources.bin").c_str(),"rb");if(!file)return false;
    char magic[8];uint32_t header[3];
    bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMBRDY01",8) && std::fread(header,4,3,file)==3 && header[0]==9 && header[1]==1 && header[2]==1;
    auto next=std::make_unique<std::array<Resource,9>>();
    for(unsigned i=0;ok && i<9;++i){
        auto& r=(*next)[i];char name[64],owner[64];uint32_t values[7];float uv;
        ok=std::fread(name,64,1,file)==1 && std::fread(owner,64,1,file)==1 && std::fread(values,4,7,file)==7 && std::fread(&uv,4,1,file)==1 &&
            text(name,64,r.name) && r.name==Names[i] && text(owner,64,r.textureOwner) && values[0]==Keys[i] &&
            values[1]>=0x1b0 && values[1]<=1024*1024 && values[3]>0 && values[3]<=32 && values[4]>0 && values[4]<=values[3] &&
            values[5]>0 && values[5]<=64 && values[6]==uint32_t(i<3) && (uv==1 || uv==2 || uv==4 || uv==8);
        if(!ok)break;
        r.sourceKey=values[0];r.textures=values[5];r.needsCapture=values[6];r.uvScale=uv;
        std::set<std::string> poses;unsigned supported=0;
        for(unsigned p=0;ok && p<values[3];++p){
            char poseName[32];Pose pose;uint32_t flags[2];
            ok=std::fread(poseName,32,1,file)==1 && text(poseName,32,pose.name) && poses.insert(pose.name).second &&
                std::fread(&pose.loop,4,1,file)==1 && pose.loop<=1 && std::fread(&pose.start,4,1,file)==1 && std::fread(&pose.end,4,1,file)==1 &&
                std::isfinite(pose.start) && std::isfinite(pose.end) && pose.start<=pose.end && std::fread(flags,4,2,file)==2 && flags[0]>0 && flags[0]<=4096 && flags[1]<=1;
            if(!ok)break;
            pose.keys=flags[0];pose.supported=flags[1];supported+=pose.supported;r.descriptors.push_back(std::move(pose));
        }
        if(!ok || supported!=values[4]){ok=false;break;}
        r.originalBank.resize(values[1]);ok=std::fread(r.originalBank.data(),1,r.originalBank.size(),file)==r.originalBank.size() && fnv(r.originalBank)==values[2] && sourceDescriptors(r);
        if(!ok)break;
        ok=r.animations.load((directory+"/battle-ready-"+r.name+".pmanim").c_str());
        for(const auto& pose:r.descriptors){
            const auto* native=r.animations.pose(pose.name.c_str());
            if(!pose.supported){if(native)ok=false;continue;}
            if(!native || native->loop!=pose.loop || native->start!=pose.start || native->end!=pose.end || native->keys.size()!=pose.keys || native->bindings.empty() || native->surfaces.empty()){ok=false;break;}
            ActorAnimationSample sample;if(!native->evaluate(pose.start,sample)){ok=false;break;}
        }
    }
    ok=ok && std::fgetc(file)==EOF;std::fclose(file);
    if(ok){resources=std::move(*next);root=directory;textureAdmitted={};loaded=true;}
    return ok;
}
bool BattleStartupResources::admitTexture(unsigned index,const TextureAdmission& admission){
    if(!loaded || index>=9 || !admission)return false;
    if(textureAdmitted[index])return true;
    if(!admission(index,root+"/battle-ready-"+resources[index].name+".t3x"))return false;
    textureAdmitted[index]=true;return true;
}
bool BattleStartupResources::resourceReady(uint32_t key,int32_t context) const{
    if(!loaded || context!=1)return false;
    for(unsigned i=0;i<9;++i)if(resources[i].sourceKey==key)return textureAdmitted[i];
    return false;
}
bool BattleStartupResources::paperReady(uint32_t key,int32_t context) const{
    for(unsigned i=0;i<3;++i)if(key==Keys[i])return resourceReady(key,context);
    return false;
}
bool BattleStartupResources::niceReady(int32_t mode) const{
    if(!loaded || mode!=1)return false;
    bool ready=true;for(unsigned i=3;i<9;++i)ready=textureAdmitted[i] && ready;return ready;
}
}
