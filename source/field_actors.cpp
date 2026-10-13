#include "field_actors.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {FieldActorAssets catalog;std::string assetRoot;}
bool loadFieldActorCatalog(const char* root) {
    if(!root)return false;
    FieldActorAssets next;if(!next.load((std::string(root)+"/field-actors.pmnpc").c_str()))return false;
    catalog=std::move(next);assetRoot=root;return true;
}
const FieldActorAssets& fieldActorCatalog(){return catalog;}
std::string fieldActorTalkKey(const FieldActorRecord& actor,int32_t story) {
    if(actor.talkPrefix.empty())return {};
    constexpr int32_t limits[]={56,112,164,215,261,339,373,401};
    unsigned branch=0;while(branch<8 && story>=limits[branch])++branch;
    return actor.talkPrefix+"0"+char('0'+branch);
}
bool FieldActorTalk::begin(const FieldActorRecord& source,int32_t story,const MessageFont& font,uint64_t now) {
    if(message.active() || assetRoot.empty() || source.talkPrefix.size()<6)return false;
    const auto key=fieldActorTalkKey(source,story);if(key.empty())return false;
    MessageAssets next;
    if(!next.loadGlobal((assetRoot+"/messages/global.txt").c_str()) ||
       !next.loadRoom((assetRoot+"/messages/"+source.talkPrefix.substr(0,6)+".txt").c_str()))return false;
    const auto* text=next.find(key.c_str());if(!text)return false;
    if(!message.open(key.c_str(),text,font,now))return false;
    messages=std::move(next);actor=source;return true;
}
bool FieldActorAssets::load(const char* path) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    const bool ok=read(f);std::fclose(f);return ok;
}
bool FieldActorAssets::read(FILE* f) {
    if(!f)return false;
    auto scalar=[&](auto& v){return std::fread(&v,sizeof(v),1,f)==1;};
    auto string=[&](std::string& v,bool empty=false){
        std::array<char,32> bytes{};
        if(!scalar(bytes) || (!empty && !bytes[0]) || !std::memchr(bytes.data(),0,bytes.size()))return false;
        v=bytes.data();for(char c:v)if(!((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='_' || c=='-'))return false;
        return true;
    };
    std::array<char,8> magic{};uint32_t modelCount=0,actorCount=0;
    if(!scalar(magic))return false;
    const bool version3=!std::memcmp(magic.data(),"PMNPC003",8);
    const bool version2=version3 || !std::memcmp(magic.data(),"PMNPC002",8);
    if((!version2 && std::memcmp(magic.data(),"PMNPC001",8)) || !scalar(modelCount) || !scalar(actorCount) ||
       modelCount>64 || actorCount>1024 || (!modelCount && actorCount))return false;
    std::vector<FieldActorModel> models(modelCount);
    for(uint32_t i=0;i<modelCount;++i) {
        auto& m=models[i];if(!string(m.asset) || !scalar(m.uvScale) ||
           !(m.uvScale==1 || m.uvScale==2 || m.uvScale==4 || m.uvScale==8))return false;
        if(version3) {
            if(!scalar(m.hitMode) || m.hitMode>1 || !scalar(m.hitBounds))return false;
            for(float value:m.hitBounds)if(!std::isfinite(value) || std::fabs(value)>10000 || (!m.hitMode && value!=0))return false;
            if(m.hitMode)for(unsigned axis=0;axis<3;++axis)if(m.hitBounds[axis]>=m.hitBounds[axis+3])return false;
            if(m.hitMode && m.hitBounds[4]<=0)return false;
        }
        for(uint32_t j=0;j<i;++j)if(models[j].asset==m.asset)return false;
    }
    std::vector<FieldActorRecord> actors(actorCount);
    for(uint32_t i=0;i<actorCount;++i) {
        auto& a=actors[i];
        if(!scalar(a.key) || !scalar(a.room) || !scalar(a.model) || !scalar(a.interiorRequired) || !scalar(a.interiorForbidden) ||
           !scalar(a.storyGate) || !scalar(a.kind) || (version2 && !scalar(a.systemForbidden)) || !scalar(a.storyLow) || !scalar(a.storyHigh) || !scalar(a.position) || !scalar(a.yaw) || !scalar(a.scale) || !string(a.pose) || !string(a.talkPrefix,true) ||
           !a.key || a.room>65535 || a.model>=modelCount || (a.interiorRequired&a.interiorForbidden) || a.storyGate>(version2?2u:1u) ||
           (a.storyGate?a.storyLow>a.storyHigh:a.storyLow!=0 || a.storyHigh!=0) || !std::isfinite(a.yaw) || a.kind>1 ||
           !std::isfinite(a.scale) || a.scale<=0 || a.scale>100)return false;
        if(version3 && !string(a.sourceName,a.kind==0))return false;
        if(models[a.model].hitMode && a.kind!=1)return false;
        if(a.storyGate==2 && a.storyLow!=a.storyHigh)return false;
        if(!a.talkPrefix.empty() && (a.kind!=0 || a.talkPrefix.size()!=11 || a.talkPrefix.substr(0,4)!="gor_" ||
           a.talkPrefix[6]!='_' || a.talkPrefix.back()!='_'))return false;
        for(float v:a.position)if(!std::isfinite(v) || std::fabs(v)>100000)return false;
        for(uint32_t j=0;j<i;++j) {
            const auto& b=actors[j];if(a.key!=b.key || a.room!=b.room)continue;
            const bool distinctInterior=(a.interiorRequired&b.interiorForbidden) || (b.interiorRequired&a.interiorForbidden);
            const bool distinctStory=(a.storyGate==1 && b.storyGate==1 && (a.storyHigh<b.storyLow || b.storyHigh<a.storyLow)) ||
                (a.storyGate==2 && b.storyGate==1 && b.storyLow==b.storyHigh && b.storyLow==a.storyLow) ||
                (b.storyGate==2 && a.storyGate==1 && a.storyLow==a.storyHigh && a.storyLow==b.storyLow);
            if(!distinctInterior && !distinctStory)return false;
        }
    }
    if(std::fgetc(f)!=EOF)return false;
    models_=std::move(models);actors_=std::move(actors);return true;
}
}
