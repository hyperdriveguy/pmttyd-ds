#include "field_actors.hpp"
#include "npc_presentation.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
FieldActorAssets catalog;NpcAngleTable npcAngles;std::string assetRoot;uint64_t generation=0;
const FieldActorTalkRule* talkRule(const FieldActorRecord& actor,int32_t story) {
    for(const auto& rule:actor.talkRules)if(rule.comparison==3 ||
        (rule.comparison==0 && story<rule.story) || (rule.comparison==1 && story==rule.story) ||
        (rule.comparison==2 && story>=rule.story))return &rule;
    return nullptr;
}
bool ordinaryTalkGuard(const FieldActorRecord& actor,const FieldActorTalkState& state,const FieldActorTalkInventory& items) {
    // Original gor_02/Goomba4 profile, independent from reusable c_kuribo_n art.
    if(actor.room!=2 || actor.key!=0x2b0a9f83)return true;
    const auto excludes=[&](uint8_t quest,const std::optional<bool>& flag,const std::optional<int32_t>& count) {
        return (state.quest1420 && *state.quest1420!=quest) || (flag && !*flag) || (flag && *flag && count && *count<=0);
    };
    // Positive/unknown item domains still require the unbound quest playback.
    return excludes(2,state.flag5405,items.item95) && excludes(13,state.flag5406,items.item107);
}
}
bool loadFieldActorCatalog(const char* root) {
    if(!root)return false;
    FieldActorAssets next;if(!next.load((std::string(root)+"/field-actors.pmnpc").c_str()))return false;
    NpcAngleTable nextAngles;if(!nextAngles.load((std::string(root)+"/npc-angle.bin").c_str()))return false;
    catalog=std::move(next);npcAngles=nextAngles;assetRoot=root;++generation;return true;
}
const FieldActorAssets& fieldActorCatalog(){return catalog;}
uint64_t fieldActorCatalogGeneration(){return generation;}
const NpcAngleTable& fieldNpcAngles(){return npcAngles;}
std::string fieldActorTalkKey(const FieldActorRecord& actor,int32_t story,const FieldActorTalkState& state,const FieldActorTalkInventory& items) {
    // Original plaza Goomba3 quest arm precedes its ordinary story switch.
    // Acquisition/reward require their own event services; only the unchanged
    // waiting print is complete here. Unknown quest/flags do not imply zero.
    if(actor.room==0 && actor.key==0x42ff5f1f) {
        if(!state.quest1420)return {};
        if(*state.quest1420==2)
            return state.flag5406==false && state.flag5405==true?"irai_02_01":std::string{};
    }
    if(!ordinaryTalkGuard(actor,state,items))return {};
    if(actor.talkRulesBound) {
        const auto* rule=talkRule(actor,story);return rule?rule->message:std::string{};
    }
    if(actor.talkPrefix.empty())return {};
    constexpr int32_t limits[]={56,112,164,215,261,339,373,401};
    unsigned branch=0;while(branch<8 && story>=limits[branch])++branch;
    return actor.talkPrefix+"0"+char('0'+branch);
}
bool fieldActorTalkAvailable(const FieldActorRecord& actor,int32_t story,const FieldActorTalkState& state,const FieldActorTalkInventory& items) {
    if(actor.room==0 && actor.key==0x42ff5f1f) {
        if(!state.quest1420)return false;
        if(*state.quest1420==2)return state.flag5406==false && state.flag5405==true;
    }
    if(!ordinaryTalkGuard(actor,state,items))return false;
    return actor.talkRulesBound?talkRule(actor,story)!=nullptr:!actor.talkPrefix.empty();
}
bool FieldActorTalk::begin(const FieldActorRecord& source,int32_t story,const MessageFont& font,uint64_t now,const FieldActorTalkState& state,const FieldActorTalkInventory& items) {
    const auto key=fieldActorTalkKey(source,story,state,items);if(key.empty())return false;
    return print(source,key.c_str(),font,now);
}
bool FieldActorTalk::canPrint(const FieldActorRecord& source,const char* id,const MessageFont& font,uint64_t now) const {
    if(!id || assetRoot.empty() || source.talkPrefix.size()<6)return false;
    MessageAssets probe;
    if(!probe.loadGlobal((assetRoot+"/messages/global.txt").c_str()) ||
       !probe.loadRoom((assetRoot+"/messages/"+source.talkPrefix.substr(0,6)+".txt").c_str()))return false;
    const auto* text=probe.find(id);MessagePlayback playback;return text && playback.open(id,text,font,now);
}
bool FieldActorTalk::print(const FieldActorRecord& source,const char* id,const MessageFont& font,uint64_t now) {
    if(message.active() || !id || assetRoot.empty() || source.talkPrefix.size()<6)return false;
    MessageAssets next;
    if(!next.loadGlobal((assetRoot+"/messages/global.txt").c_str()) ||
       !next.loadRoom((assetRoot+"/messages/"+source.talkPrefix.substr(0,6)+".txt").c_str()))return false;
    const auto* text=next.find(id);if(!text)return false;
    if(!message.open(id,text,font,now))return false;
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
    const bool version8=!std::memcmp(magic.data(),"PMNPC008",8);
    const bool version7=version8 || !std::memcmp(magic.data(),"PMNPC007",8);
    const bool version6=version7 || !std::memcmp(magic.data(),"PMNPC006",8);
    const bool version5=version6 || !std::memcmp(magic.data(),"PMNPC005",8);
    const bool version4=version5 || !std::memcmp(magic.data(),"PMNPC004",8);
    const bool version3=version4 || !std::memcmp(magic.data(),"PMNPC003",8);
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
        if(version4 && !string(m.originalName))return false;
        if(version5 && !scalar(m.drawMask))return false;
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
        if(version5) {
            if(!scalar(a.npcWidth) || !scalar(a.npcHeight) || !string(a.talkPose,true))return false;
            if(!std::isfinite(a.npcWidth) || !std::isfinite(a.npcHeight) || a.npcWidth<0 || a.npcHeight<0 ||
               a.npcWidth>10000 || a.npcHeight>10000 || (a.kind==1 && (a.npcWidth || a.npcHeight || !a.talkPose.empty())))return false;
        }
        if(models[a.model].hitMode && a.kind!=1)return false;
        if(version6 && (!scalar(a.npcFlags) || (a.kind==1 && a.npcFlags)))return false;
        if(version7) {
            uint32_t count=0;if(!scalar(count) || count>64 || (a.kind==1 && count))return false;
            a.talkRulesBound=true;a.talkRules.resize(count);
            for(auto& rule:a.talkRules)if(!scalar(rule.comparison) || rule.comparison>3 ||
                !scalar(rule.story) || !string(rule.message) || rule.message.size()<7 ||
                rule.message[6]!='_' || a.talkPrefix.empty() || rule.message.substr(0,6)!=a.talkPrefix.substr(0,6))return false;
        }
        if(version8) {
            auto& w=a.wander;
            if(!scalar(a.sourceTalkPresent) || a.sourceTalkPresent>1 || !scalar(w.enabled) || w.enabled>1 || !scalar(w.flags) ||
               !scalar(w.radius) || !scalar(w.speed) || !scalar(w.runThreshold) || !scalar(w.stepPeriod) ||
               !string(w.idle,!w.enabled) || !string(w.walk,!w.enabled) || !string(w.run,!w.enabled) ||
               !string(w.soundLeft,true) || !string(w.soundRight,true))return false;
            if(!std::isfinite(w.radius) || !std::isfinite(w.speed) || !std::isfinite(w.runThreshold) ||
               (w.enabled && (a.kind!=0 || (a.npcFlags&(0x100000|0x20000|0x08000000|0x10)) ||
                (!(a.npcFlags&0x40000000) && (a.npcFlags&(0x2000|0x10000))) ||
                w.radius<=0 || w.radius>10000 || w.speed<=0 || w.speed>10000 || w.runThreshold<=0 || w.runThreshold>10000 || w.stepPeriod>65535)) ||
               (!w.enabled && (w.flags || w.radius || w.speed || w.runThreshold || w.stepPeriod || !w.idle.empty() || !w.walk.empty() || !w.run.empty() || !w.soundLeft.empty() || !w.soundRight.empty())) ||
               (a.kind && a.sourceTalkPresent) || (!a.sourceTalkPresent && !a.talkRules.empty()))return false;
        } else a.sourceTalkPresent=!a.talkPrefix.empty();
        if(a.storyGate==2 && a.storyLow!=a.storyHigh)return false;
        if(!a.talkPrefix.empty() && (a.kind!=0 || a.talkPrefix.size()!=11 ||
           (version7?a.talkPrefix[3]!='_':a.talkPrefix.substr(0,4)!="gor_") ||
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
