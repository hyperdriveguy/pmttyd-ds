#include "trio_battle_commands.hpp"
#include "message_assets.hpp"
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
struct Command {uint32_t address=0,admission=0,attributes=0;std::array<char,64> key{};};
std::array<Command,3> commands;
MessageResource labels;
bool ready=false;
constexpr std::array<unsigned,4> registry{{3,4,5,6}};
constexpr std::array<unsigned,7> visual{{0,0,1,3,5,4,2}};
int32_t half(const BattleCandidateBuffer& b,unsigned o) {
    const uint16_t v=uint16_t(b.bytes[o])<<8|b.bytes[o+1];return v<0x8000?v:int32_t(v)-0x10000;
}
}
bool loadTrioCommandAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    FILE* f=std::fopen((base+"/trio-battle-commands.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t count=0;std::array<Command,3> next{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMTCMD1",8) &&
        std::fread(&count,4,1,f)==1 && count==3;
    for(auto& c:next)ok=ok && std::fread(&c.address,4,1,f)==1 && std::fread(&c.admission,4,1,f)==1 &&
        std::fread(&c.attributes,4,1,f)==1 && std::fread(c.key.data(),1,64,f)==64 && c.key.back()==0;
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);
    MessageResource text;if(!ok || !text.load((base+"/messages/global.txt").c_str()))return false;
    for(auto& c:next)if(!text.find(c.key.data()))return false;
    commands=next;labels=std::move(text);ready=true;return true;
}
unsigned TrioBattleCommands::weaponIndex() const {return actor==2?2:command;}
int TrioBattleCommands::count() const {const auto v=candidates.bytes[0xa6c];return v<128?v:int(v)-256;}
std::array<int32_t,2> TrioBattleCommands::target(unsigned index) const {
    if(count()<=0 || index>=unsigned(count()))return {{-1,-1}};
    const unsigned row=4+36*candidates.bytes[0xa6d+index];return {{half(candidates,row),half(candidates,row+2)}};
}
std::array<int32_t,2> TrioBattleCommands::selected() const {return target(candidates.bytes[0xab7]);}
const char* TrioBattleCommands::commandName(unsigned i) const {
    if(!ready || i>=(actor==2?1u:2u))return "";
    return labels.find(commands[actor==2?2:i].key.data());
}
const char* TrioBattleCommands::targetName(int32_t id) const {
    if(id<3 || id>6)return "";
    const auto* name=labels.find(trioBattleUnitSpecs()[id-3].nameKey.data());return name?name:"";
}
CandidateSamplingServices TrioBattleCommands::services() {
    // Rebind views for every sampling call: a Game copy must never retain pointers
    // into its previous owner, including a cloned host fixture/native snapshot.
    for(unsigned id=1;id<7;++id) {
        positionUnits[id]={&positions[id],&positions[id],&scales[id],&facings[id],&units[id].belong,&positionParts[id]};
        positionParts[id].owner=&positionUnits[id];
        samplingParts[id].position=&positionParts[id];samplingParts[id].direction=facings[id];
        samplingUnits[id]={&units[id],int32_t(id),&partCounts[id],&samplingParts[id],&scales[id]};
    }
    CandidateSamplingServices s;s.user=this;
    s.captureWork=[](void* p)->void* {return p;};
    s.unit=[](void*,void* w,int32_t id)->SamplingUnit* {return id>0 && id<7?&static_cast<TrioBattleCommands*>(w)->samplingUnits[id]:nullptr;};
    s.part=[](void* p,int32_t id,int32_t part)->SamplingPart* {return id>0 && id<7 && part==1?&static_cast<TrioBattleCommands*>(p)->samplingParts[id]:nullptr;};
    s.status27=[](void*,SamplingUnit& u){return checkStatus(u.data,27);};
    s.worldPosition=[](void*,SamplingPart& p){JumpVector v;getPartWorldPosition(*p.position,&v.x,&v.y,&v.z);return v;};
    s.truncate=[](void*,float value)->int32_t {return static_cast<int32_t>(value);};return s;
}
bool TrioBattleCommands::begin(const ProfessorStory& scene,int32_t actorId) {
    if(!ready || (actorId!=1 && actorId!=2))return false;
    *this=TrioBattleCommands{};actor=actorId;
    for(unsigned id=1;id<7;++id) {
        partCounts[id]=1;scales[id]=1;facings[id]=id<3?1:-1;units[id].belong=id<3?0:1;
        positionParts[id].id=1;
    }
    units[1].kind=222;units[2].kind=224;
    for(unsigned i=0;i<registry.size();++i) {
        const auto& spec=trioBattleUnitSpecs()[i];const auto id=registry[i];
        units[id].kind=spec.kind;units[id].hp=units[id].maxHp=spec.hp;units[id].fp=spec.fp;units[id].attributes=spec.attributes;
        positionParts[id].attributes=spec.partAttributes;
        positionParts[id].offset={spec.partPosition[0],spec.partPosition[1],spec.partPosition[2]};
        positionParts[id].hitOffsets[0]={spec.hitOffset[0],spec.hitOffset[1],spec.hitOffset[2]};
        samplingParts[id].samplingOffsets[0]={spec.samplingOffset[0],spec.samplingOffset[1],spec.samplingOffset[2]};
        samplingParts[id].forwardBias=spec.forwardBias;
    }
    syncPositions(scene);state=State::Command;return true;
}
void TrioBattleCommands::syncPositions(const ProfessorStory& scene) {
    for(unsigned id=1;id<7;++id) {
        const auto& p=scene.actors[visual[id]].position;positions[id]={p[0],p[1],p[2]};
    }
}
void TrioBattleCommands::chooseCommand(unsigned index) {
    if(state!=State::Command || index>=(actor==2?1u:2u))return;
    command=index;
}
void TrioBattleCommands::chooseTarget(unsigned index) {
    if(state==State::Target && count()>0 && index<unsigned(count()))candidates.bytes[0xab7]=index;
}
void TrioBattleCommands::refreshTargets() {
    const auto old=selected();updateBattleCandidates(candidates,services());
    for(int i=0;i<count();++i)if(target(i)==old){candidates.bytes[0xab7]=i;break;}
}
void TrioBattleCommands::input(uint32_t triggers,uint32_t repeats) {
    if(state==State::Command) {
        const unsigned n=actor==2?1:2;
        if(repeats&0x80000)command=(command+1)%n;
        if(repeats&0x40000)command=(command+n-1)%n;
        if(triggers&0x200)return;
        if(triggers&0x100) {
            const auto& c=commands[weaponIndex()];
            sampleBattleCandidates(candidates,nullptr,actor,1,c.admission,c.attributes,1,services());
            if(count()>0)state=State::Target;
        }
    } else if(state==State::Target) {
        refreshTargets();const int n=count();
        if(n>=2) {
            if(repeats&0x80000)candidates.bytes[0xab7]=(candidates.bytes[0xab7]+1)%n;
            if(repeats&0x40000)candidates.bytes[0xab7]=(candidates.bytes[0xab7]+n-1)%n;
        }
        if(triggers&0x200){state=State::Command;return;}
        if((triggers&0x100) && n>0) {
            const auto pair=selected();attack=TrioAttackRequest{actor,pair[0],pair[1],commands[weaponIndex()].address};state=State::AttackReady;
        }
    } else if(state==State::AttackReady && (triggers&0x200)) {
        // Development return while the event consumer has not accepted dispatch.
        attack.reset();state=State::Target;
    }
}
}
