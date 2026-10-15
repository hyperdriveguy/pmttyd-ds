#include "professor_story.hpp"
#include "trio_battle_commands.hpp"
#include "message_assets.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_controls.hpp"
#include "actor_animation.hpp"
#include "message_icons.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
EventScriptAssets battleScripts;std::unordered_map<uint32_t,uint32_t> battleCallbacks;std::array<uint32_t,3> battleEntries{};std::array<TrioBattleUnitSpec,4> battleUnits{};bool battleReady=false;
MessageAssets undergroundMessages;MessageResource undergroundExtra;EventScriptAssets undergroundScripts;
std::unordered_map<uint32_t,uint32_t> undergroundCallbacks;uint32_t undergroundEntry=0;std::array<uint32_t,3> trioNames{};bool undergroundReady=false;
MessageAssets messages;EventScriptAssets scripts;MessageFont font;
uint32_t script=0,fenceScript=0,lectureScript=0,pipeScript=0,franklyName=0,goombellaName=0;std::unordered_map<uint32_t,uint32_t> callbacks;bool ready=false;
std::vector<uint32_t> fenceNodes;float fenceFrames=0;
// Guest two's-complement decoding; do not treat original addresses as host
// pointers or implementation-defined signed casts.
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
const std::vector<uint32_t>& professorFenceCollisionNodes(){return fenceNodes;}
// Scoped little-endian metadata: magic8 then u32 entry/fence/lecture/pipe,
// Frankly/Goombella name addresses and64 callback pairs. Numeric tags match
// professor_event_assets.py; PMPROG4 supersedes earlier contract pack versions.
// Commit global bank only after source font/icons, sorted collision nodes and
// nonlooping fence A_1 span0..140 validate. Pipe entry is retained but unlike
// the first three entry words is not probed by readWord in this loader.
bool loadProfessorStoryAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    EventScriptAssets p;MessageAssets m;MessageFont f;
    if(!p.load((base+"/professor-events.pmevt").c_str()) || !m.loadRoom((base+"/messages/gor_02.txt").c_str()) ||
       !m.loadGlobal((base+"/messages/global.txt").c_str()) || !f.load((base+"/font.pmfont").c_str()))return false;
    FILE* file=std::fopen((base+"/professor-program.bin").c_str(),"rb");if(!file)return false;
    char magic[8];uint32_t entry=0,fence=0,lecture=0,pipe=0,frankly=0,goombella=0,count=0;
    bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMPROG4",8) &&
        std::fread(&entry,4,1,file)==1 && std::fread(&fence,4,1,file)==1 && std::fread(&lecture,4,1,file)==1 &&
        std::fread(&pipe,4,1,file)==1 && std::fread(&frankly,4,1,file)==1 && std::fread(&goombella,4,1,file)==1 &&
        std::fread(&count,4,1,file)==1 && count==64;
    std::unordered_map<uint32_t,uint32_t> next;
    for(uint32_t i=0;ok && i<count;++i){uint32_t a=0,t=0;ok=std::fread(&a,4,1,file)==1 && std::fread(&t,4,1,file)==1 && a && t<64 && next.emplace(a,t).second;}
    uint32_t word=0;ok=ok && std::fgetc(file)==EOF && p.readWord(entry,word) && p.readWord(fence,word) && p.readWord(lecture,word);std::fclose(file);
    if(!ok)return false;
    ActorAnimationPack paper;if(!paper.load((base+"/professor-fence.pmanim").c_str()))return false;
    const auto* anim=paper.pose("A_1");if(!anim || anim->loop || anim->start!=0 || anim->end!=140)return false;
    file=std::fopen((base+"/professor-fence.bin").c_str(),"rb");if(!file)return false;
    uint32_t n=0;ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMFENCE1",8) && std::fread(&n,4,1,file)==1 && n>0 && n<1024;
    std::vector<uint32_t> nodes(ok?n:0);ok=ok && std::fread(nodes.data(),4,n,file)==n && std::fgetc(file)==EOF;std::fclose(file);
    if(!ok || !std::is_sorted(nodes.begin(),nodes.end()) || !loadMessageIcons((base+"/message-icons.bin").c_str()))return false;
    fenceNodes=std::move(nodes);fenceFrames=anim->end;
    scripts=std::move(p);messages=std::move(m);font=std::move(f);script=entry;fenceScript=fence;lectureScript=lecture;pipeScript=pipe;
    franklyName=frankly;goombellaName=goombella;callbacks=std::move(next);ready=true;return true;
}
// PMTRIO1 reuses professor service indices0..63 plus underground64..70.
// Three relocated trio names map slots3..5. tik_01 room messages precede
// tik_04 fallback; font comes from professor loading, without global text. Bounds here
// admit count<72 and tags<71; these are metadata guards, not story counters.
bool loadUndergroundStoryAssets(const char* root) {
    const std::string base=root;
    EventScriptAssets scripts;MessageAssets text;MessageResource extra;
    if(!scripts.load((base+"/underground-events.pmevt").c_str()) || !text.loadRoom((base+"/messages/tik_01.txt").c_str()) ||
       !extra.load((base+"/messages/tik_04.txt").c_str()))return false;
    FILE* f=std::fopen((base+"/underground-program.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t entry=0,count=0;std::array<uint32_t,3> names{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMTRIO1",8) && std::fread(&entry,4,1,f)==1 &&
        std::fread(names.data(),4,3,f)==3 && std::fread(&count,4,1,f)==1 && count<72;
    std::unordered_map<uint32_t,uint32_t> callbacks;
    for(uint32_t i=0;ok && i<count;++i){uint32_t a,t;ok=std::fread(&a,4,1,f)==1 && std::fread(&t,4,1,f)==1 && t<71 && callbacks.emplace(a,t).second;}
    uint32_t word;ok=ok && std::fgetc(f)==EOF && scripts.readWord(entry,word);std::fclose(f);if(!ok)return false;
    undergroundScripts=std::move(scripts);undergroundMessages=std::move(text);undergroundExtra=std::move(extra);
    undergroundCallbacks=std::move(callbacks);trioNames=names;undergroundEntry=entry;undergroundReady=true;return true;
}
const std::array<TrioBattleUnitSpec,4>& trioBattleUnitSpecs(){return battleUnits;}
// PMBTRIO1 carries three original init/entry/phase roots and tags<95;
// PMBUNIT2 carries four fieldwise unit records (u32/f32=4 bytes,i16=2,i8=1).
// Kind checks1/3/2/0xb4 bind Goomba/Spiky/Paragoomba/Frankly in source order.
// Require separate command assets before publishing. No flattening of scripts.
bool loadTrioBattleAssets(const char* root) {
    const std::string base=root;EventScriptAssets scripts;
    if(!scripts.load((base+"/trio-battle-events.pmevt").c_str()))return false;
    FILE* f=std::fopen((base+"/trio-battle-program.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t count=0;std::array<uint32_t,3> entries{};
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMBTRIO1",8) && std::fread(entries.data(),4,3,f)==3 && std::fread(&count,4,1,f)==1 && count<96;
    std::unordered_map<uint32_t,uint32_t> callbacks;
    for(uint32_t i=0;ok && i<count;++i){uint32_t a,t;ok=std::fread(&a,4,1,f)==1 && std::fread(&t,4,1,f)==1 && a && t<95 && callbacks.emplace(a,t).second;}
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
    f=std::fopen((base+"/trio-battle-units.bin").c_str(),"rb");if(!f)return false;
    std::array<TrioBattleUnitSpec,4> units{};
    ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMBUNIT2",8) && std::fread(&count,4,1,f)==1 && count==4;
    for(auto& u:units)ok=ok && std::fread(&u.kind,4,1,f)==1 && std::fread(&u.hp,2,1,f)==1 && std::fread(&u.fp,2,1,f)==1 &&
        std::fread(u.position.data(),4,3,f)==3 && std::fread(&u.attributes,4,1,f)==1 && std::fread(u.defense.data(),1,5,f)==5 &&
        std::fread(u.defenseAttributes.data(),1,5,f)==5 && std::fread(&u.partAttributes,4,1,f)==1 && std::fread(&u.counterAttributes,4,1,f)==1 && std::fread(u.partPosition.data(),4,3,f)==3 &&
        std::fread(u.hitOffset.data(),4,3,f)==3 && std::fread(u.samplingOffset.data(),4,3,f)==3 &&
        std::fread(&u.forwardBias,2,1,f)==1 && std::fread(u.nameKey.data(),1,64,f)==64 && u.nameKey.back()==0;
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok || units[0].kind!=1 || units[1].kind!=3 || units[2].kind!=2 || units[3].kind!=0xb4)return false;
    if(!loadTrioCommandAssets(root))return false;
    battleScripts=std::move(scripts);battleCallbacks=std::move(callbacks);battleEntries=entries;battleUnits=units;battleReady=true;return true;
}
struct ProfessorRuntime {
    ProfessorStory* scene=nullptr;bool doorBusy=true;uint32_t keys=0,rootId=0;
    struct Move {std::array<float,3> start{},target{};uint64_t begin=0,end=0;uint32_t owner=0;bool active=false,jump=false;float apex=0;};
    std::array<Move,6> moves{};
    struct BattleMove {std::array<float,3> target{},step{};int32_t count=0;unsigned actor=2;};
    std::unordered_map<uint32_t,BattleMove> battleMoves;
    std::unordered_map<uint32_t,uint32_t> messagePlacements;
    std::unordered_map<uint32_t,std::unique_ptr<NativeFieldPartyScriptCall>> partyCalls;
    std::vector<std::unique_ptr<NativeFieldPartyScriptCall>> retiredPartyCalls;
    uint32_t retiredPartyPending=0; // Opaque native cancellation remains unbound.
    std::unordered_map<uint32_t,uint64_t> waits;
    EventNativeRuntime vm;
    // Private authored GP base;243000000 is GameCube clock Hz, converted
    // from public ms when assigning the event timestamp. It is not3DS CPU Hz.
    ProfessorRuntime():vm(0x81000000,platform(this)) {}
    static EventNativePlatform platform(ProfessorRuntime* owner) {
        EventNativePlatform p;p.user=owner;p.clock=[](void*)->uint32_t{return 243000000;};
        p.truncate=[](void*,float v)->int32_t{assert(std::isfinite(v) && double(v)>=-2147483648. && double(v)<2147483648.);return int32_t(v);};
        p.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){}; // CPU-owned native arena, no GPU consumers.
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first){return static_cast<ProfessorRuntime*>(p)->call(e,fn,first!=0);};
        p.retiring=[](void* p,const EventEntry& e){
            auto& r=*static_cast<ProfessorRuntime*>(p);auto i=r.partyCalls.find(e.id);
            if(i==r.partyCalls.end())return;
            if(!i->second->releaseForRetirement()){
                r.retiredPartyPending=i->second->pending();
                r.retiredPartyCalls.push_back(std::move(i->second));
            }
            r.partyCalls.erase(i); // ID reuse cannot resume a retired invocation.
        };return p;
    }
    // Guest strings require NUL within256 bytes; argument slots have4-byte
    // stride. NPC name tokens select Mario0/G1/Frankly2/trio3..5; an unknown
    // name fails the scene instead of quietly selecting a different speaker.
    std::string string(uint32_t a) {
        std::string result;for(unsigned i=0;i<256;++i){uint8_t c=0;if(!vm.memory.readByte(a+i,c)){scene->failed=true;return {};}
            if(!c)return result;
            result.push_back(char(c));}scene->failed=true;return {};
    }
    int32_t operand(EventEntry& e,unsigned n) {
        uint32_t v=0;
        if(n>=e.argumentCount || !vm.memory.readWord(e.arguments+4*n,v)){scene->failed=true;return 0;}
        return signedWord(v);
    }
    int32_t value(EventEntry& e,unsigned n){return getEventValue(e,operand(e,n),vm.interpreterServices().operands);}
    float number(EventEntry& e,unsigned n){return getEventFloat(e,operand(e,n),vm.interpreterServices().operands);}
    uint32_t pointer(EventEntry& e,unsigned n){return uint32_t(value(e,n));}
    unsigned npc(EventEntry& e,unsigned n) {
        const auto p=pointer(e,n);if(p==franklyName)return 2;if(p==goombellaName)return 1;
        if(scene->underground)for(unsigned i=0;i<3;++i)if(p==trioNames[i])return i+3;
        scene->failed=true;return 2;
    }
    const char* text(const char* key) {
        if(!scene->underground)return messages.find(key);
        const char* p=undergroundMessages.find(key);return p?p:undergroundExtra.find(key);
    }
    void positionOut(EventEntry& e,unsigned first,unsigned actor) {
        const auto op=vm.interpreterServices().operands;
        for(unsigned axis=0;axis<3;++axis)setEventFloat(e,operand(e,first+axis),scene->actors[actor].position[axis],op);
    }
    // Per-entry deadline initialized once:0 waits/revisits,2 advances. Scripts
    // retain their own child/wait scheduling; this is only a service boundary.
    int delay(EventEntry& e,bool first,uint64_t duration) {
        if(first)waits[e.id]=scene->now+duration;
        if(scene->now<waits[e.id])return 0;
        waits.erase(e.id);return 2;
    }
    // Heading degrees retain paper-facing yaw90/270 only for10..170/190..350;
    // near-depth travel leaves prior yaw. (distance+1)/speed*1000 is native
    // duration policy, not original measured NPC stepping; zero span is inactive.
    int move(EventEntry& e,bool first,unsigned actor,unsigned x,unsigned z,unsigned speed) {
        auto& m=moves[actor];auto& a=scene->actors[actor];
        if(first) {
            m.owner=e.id;
            m.start=a.position;m.target={number(e,x),a.position[1],number(e,z)};
            const float heading=std::atan2(m.target[0]-m.start[0],m.start[2]-m.target[2])*57.29577951308232f;
            const float travel=heading<0?heading+360:heading;
            if(travel>=10 && travel<=170)a.yaw=90;
            else if(travel>=190 && travel<=350)a.yaw=270;
            const float distance=std::hypot(m.target[0]-m.start[0],m.target[2]-m.start[2]),rate=number(e,speed);
            if(!(rate>0)){scene->failed=true;return 0;}
            // Native linear movement bridge for the source speed request. Full
            // original NPC/party/collision/ease stepping remains a separate owner.
            m.jump=false;m.begin=scene->now;m.end=m.begin+uint64_t((distance+1)/rate*1000);m.active=m.end>m.begin;a.moving=m.active;
        }
        return m.active?0:2;
    }
    // An entry ID owns each movement: deleting it cancels motion before the
    // next interpolation. No-hit jump is y=start+t*(deltaY+4*apex-4*apex*t),
    // not a field collision/gravity service. End snaps all interpolated axes.
    void advance() {
        for(unsigned i=0;i<moves.size();++i)if(moves[i].active) {
            auto& m=moves[i];
            if(!checkEventId(m.owner,vm.creationServices())) {
                m.active=false;scene->actors[i].moving=false;scene->actors[i].jumping=false;continue;
            }
            const float t=std::min(1.f,float(scene->now-m.begin)/float(m.end-m.begin));
            for(unsigned k=0;k<3;++k)scene->actors[i].position[k]=m.start[k]+(m.target[k]-m.start[k])*t;
            if(m.jump){const float c=-4*m.apex,b=(m.target[1]-m.start[1])-c;scene->actors[i].position[1]=m.start[1]+t*(b+c*t);}
            if(t==1){m.active=false;scene->actors[i].moving=false;scene->actors[i].jumping=false;}
        }
    }
    // Party callback appends original _kur suffix. Source party/extparty names
    // select G1/Frankly2; battle intro forces Frankly. Cache root at print entry.
    // once_stop releases print while its window stays active for later append.
    // Battle placement low bits==1 chooses override0, otherwise3, then clears
    // this entry's placement word;0x1effffff excludes preserved high controls.
    int print(EventEntry& e,bool first,bool party) {
        if(first) {
            auto key=string(pointer(e,party?0:1));if(party)key+="_kur";
            if(scene->battleIntro)scene->speaker=2;
            else if(party)scene->speaker=1;
            else {const auto name=string(pointer(e,3));scene->speaker=name=="party"?1:name=="extparty"?2:npc(e,3);}
            scene->speakerRoot=scene->actors[scene->speaker].position;
            if(!scene->message.open(key.c_str(),text(key.c_str()),font,scene->now)){scene->failed=true;return 0;}
            if(scene->battleIntro){scene->message.placementOverride=(messagePlacements[e.id]&0x1effffff)==1?0:3;messagePlacements[e.id]=0;}
            scene->messageOrder.push_back(key);
        }
        return scene->message.active() && !scene->message.stopped()?0:2;
    }
    // Selection owns a separate window; signed result writes original LW0
    // (operand tag-30000000). Input advances outside the VM before this poll.
    int select(EventEntry& e,bool first) {
        if(first) {
            const auto key=string(pointer(e,1));
            if(!scene->selection.open(key.c_str(),text(key.c_str()),font,scene->now)){scene->failed=true;return 0;}
            scene->selectionOrder.push_back(key);
        }
        if(scene->selection.active())return 0;
        setEventValue(e,-30000000,scene->selection.current,vm.interpreterServices().operands);
        scene->selectionResults.push_back(scene->selection.current);return 2;
    }
    // Reuse retained original window/cursor via append, including once_stop;
    // trace the original ID rather than substituting a new conversation.
    int append(EventEntry& e,bool first) {
        if(first) {
            const auto key=string(pointer(e,1));
            if(!scene->message.append(text(key.c_str()),font,scene->now)){scene->failed=true;return 0;}
            scene->messageOrder.push_back(key);
        }
        return scene->message.active() && !scene->message.stopped()?0:2;
    }
    // Namespace precedence battle>underground>professor. Indices are converter
    // SERVICES identities, not VM opcodes; tools append catalogs without renumbering.
    // Trace raw operands once per admission, retaining pointer/tag identities.
    // Unknown function fails; admitted unhandled presentation tags return2.
    // That return records request acceptance, not backend/hardware completion.
    int32_t partyCall(EventEntry& e,uint32_t function,bool first) {
        auto& cursor=partyCalls[e.id];if(!cursor)cursor=std::make_unique<NativeFieldPartyScriptCall>();
        const auto result=cursor->step(e,first,function,vm,scene->fieldParty,scene->now);
        if(result==2){partyCalls.erase(e.id);scene->pendingService=0;}
        else scene->pendingService=cursor->pending();
        return result;
    }
    int32_t call(EventEntry& e,uint32_t function,bool first) {
        const auto& admitted=scene->battleIntro?battleCallbacks:scene->underground?undergroundCallbacks:callbacks;
        const auto found=admitted.find(function);if(found==admitted.end()){scene->failed=true;return 0;}
        const unsigned tag=found->second;
        if(first){ProfessorServiceRequest r{function,e.currentCommand,scene->now,{}};
            for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(operand(e,i));
            scene->presentationRequests.push_back(std::move(r));}
        // Explicit source placement increments generation; hidden sentinelY-1000
        // and NPC bit0x80 are independent of npcActive/lifetime. flagMask below
        // records only source-owned bits, so consumers can merge other flags.
        auto setPosition=[&](unsigned actor,unsigned index){for(unsigned i=0;i<3;++i)scene->actors[actor].position[i]=number(e,index+i);++scene->actors[actor].placementGeneration;scene->actors[actor].hidden=scene->actors[actor].position[1]==-1000 || (scene->actors[actor].flags&0x80);};
        //0/1 cameras,3 door barrier,5/7/8 item entry/pos/scale,9 map flags;
        //11 get Mario pos,12 keys,13/14 move/turn,15 temporary party,16 pose,
        //17/18 print,19 NPC flags,20 get NPC pos,21 no-hit hop,22 move,
        //23/24 pos/yaw,26 party move,28 party turn. Other presentation-only
        // indices retain their raw trace through the default acceptance path.
        switch(tag) {
        case 0:if(first)scene->cameras.push_back({{},{},value(e,0),value(e,1),true});return 2;
        case 1:if(first){ProfessorCameraRequest r;for(unsigned i=0;i<3;++i){r.eye[i]=number(e,i);r.at[i]=number(e,i+3);}r.duration=value(e,6);r.curve=value(e,7);scene->cameras.push_back(r);}return 2;
        case 3:return doorBusy?0:2;
        case 5:for(unsigned i=0;i<3;++i)scene->itemPosition[i]=number(e,2+i);scene->itemVisible=true;return 2;
        case 7:for(unsigned i=0;i<3;++i)scene->itemPosition[i]=number(e,1+i);scene->itemVisible=scene->itemPosition[1]!=-1000;return 2;
        case 8:scene->itemScale=number(e,1);return 2;
        case 9:if(scene->underground && string(pointer(e,2))=="S")scene->undergroundGroupShown=value(e,0)==1 && value(e,1)==0;
            if(string(pointer(e,2))=="S_saku04")scene->fenceHidden=value(e,0)!=0;
            return 2;
        case 11:positionOut(e,1,0);return 2;
        case 12:scene->keysEnabled=value(e,0)!=0;return 2;
        case 13:return move(e,first,0,0,1,2);
        case 14:if(first)scene->actors[0].yaw=number(e,0);return delay(e,first,uint64_t(std::max<int32_t>(0,value(e,1))));
        case 15:
            if(scene->fieldParty.bound() || partyCalls.count(e.id)){
                if(partyCall(e,function,first)!=2)return 0;
                const auto index=int8_t(scene->fieldParty.mario->partyIndices[1]);
                auto* actor=index>=0 && index<2?scene->fieldParty.parties->pointer(index):nullptr;
                scene->temporaryProfessor=actor && actor->bytes[0x31]==11;
                if(!scene->temporaryProfessor)return 2;
                for(unsigned i=0;i<3;++i)scene->actors[2].position[i]=actor->scalar(0x58+4*i);
            }else {scene->temporaryProfessor=true;for(unsigned i=0;i<3;++i)scene->actors[2].position[i]=number(e,2+i);}
            scene->actors[2].npcActive=true;++scene->actors[2].placementGeneration;
            scene->actors[2].hidden=scene->actors[2].position[1]==-1000;return 2;
        case 16:scene->actors[0].pose=string(pointer(e,0));scene->actors[0].poseAt=scene->now;return 2;
        case 17:return print(e,first,false);
        case 18:return print(e,first,true);
        case 19: {
            auto& actor=scene->actors[npc(e,1)];const auto mask=uint32_t(value(e,2));
            actor.flagMask|=mask;
            if(value(e,0)==1)actor.flags|=mask;else actor.flags&=~mask;
            actor.hidden=actor.position[1]==-1000 || (actor.flags&0x80);return 2;
        }
        case 20:positionOut(e,1,npc(e,0));return 2;
        case 21: {
            const unsigned actor=npc(e,0);auto& m=moves[actor];
            if(first) {
                m.owner=e.id;
                m.start=scene->actors[actor].position;
                for(unsigned i=0;i<3;++i)m.target[i]=number(e,1+i);
                m.begin=scene->now;m.end=m.begin+uint64_t(std::max<int32_t>(0,value(e,4)));
                m.apex=number(e,5);m.jump=true;m.active=m.end>m.begin;
                scene->actors[actor].jumping=m.active;
            }
            return m.active?0:2;
        }
        case 22:return move(e,first,npc(e,0),1,2,4);
        case 23:setPosition(npc(e,0),1);return 2;
        case 24:scene->actors[npc(e,0)].yaw=number(e,1);return 2;
        case 26:return move(e,first,1,1,2,3);
        case 28:if(first)scene->actors[1].yaw=number(e,1);return delay(e,first,uint64_t(std::max<int32_t>(0,value(e,2))));
        case 27:case 29:return (scene->fieldParty.bound() || partyCalls.count(e.id))?partyCall(e,function,first):2;
        // Battle extension71..94: registrations72..76 are traced;77 resolves
        // scoped Frankly unit6; work index0..15; actor selector-3 is Mario,
        // otherwise Frankly. These are used lecture services, not general lookup.
        case 71:return 2; // Original local lecture_init is a return2 leaf.
        case 72:case 73:case 74:case 75:case 76:return 2; // Script registrations retained in request trace.
        case 77:setEventValue(e,operand(e,1),6,vm.interpreterServices().operands);return 2;
        case 78:scene->lectureUnitId=value(e,0);return 2;
        case 79:if(value(e,1)>=0 && value(e,1)<16)scene->lectureWork[value(e,1)]=value(e,2);else scene->failed=true;return 2;
        case 80:setPosition(2,1);return 2;
        case 81:scene->actors[2].yaw=value(e,1)==1?90:270;return 2;
        case 82:scene->lectureMaxMoves=value(e,1);return 2;
        case 83:case 88:return 2; // Wait-event ownership requests; native pose manager is an adapter.
        case 84: {
            const unsigned actor=value(e,0)==-3?0:2;scene->actors[actor].pose=string(pointer(e,2));scene->actors[actor].poseAt=scene->now;return 2;
        }
        // One X/Z step per callback visit, independent of elapsed ms. Final
        // visit snaps XYZ and returns1 (advance+yield), not immediate return2.
        // Positive count is required; entry-ID map retains suspended movement.
        case 85: {
            auto& m=battleMoves[e.id];
            if(first) {
                m.actor=2;m.count=value(e,4);if(m.count<=0){scene->failed=true;return 0;}
                for(unsigned axis=0;axis<3;++axis)m.target[axis]=number(e,1+axis);
                const auto& p=scene->actors[2].position;m.step={(m.target[0]-p[0])/m.count,0,(m.target[2]-p[2])/m.count};scene->actors[2].moving=true;
            }
            auto& a=scene->actors[m.actor];a.position[0]+=m.step[0];a.position[2]+=m.step[2];
            if(--m.count>0)return 0;
            a.position=m.target;a.moving=false;battleMoves.erase(e.id);return 1; // Source advance+yield.
        }
        // Phase comparison intentionally reads RAW operand1, not getEventValue.
        // The packed phase literal must not be decoded as a variable tag.
        case 86:setEventValue(e,operand(e,0),scene->battlePhaseWord==uint32_t(operand(e,1)),vm.interpreterServices().operands);return 2;
        case 87:setEventValue(e,operand(e,0),scene->battleTurn,vm.interpreterServices().operands);return 2;
        case 89:case 90:return 2; // Source talk/stay names retained; used names are T_1/S_1.
        case 91:scene->battleStatusVisible=value(e,0)!=0;return 2;
        case 92:if(value(e,1)==0)scene->cameras.push_back({{},{},0,11,true});return 2;
        // Battle camera source frames become integer ms by *1000/60; curve
        // remains source value. msg_toge preserves high0xe1000000 controls.
        case 93: {
            ProfessorCameraRequest r;for(unsigned axis=0;axis<3;++axis){r.eye[axis]=number(e,axis+1);r.at[axis]=number(e,axis+4);}
            r.duration=value(e,7)*1000/60;r.curve=value(e,8);scene->cameras.push_back(r);return 2;
        }
        case 94:messagePlacements[e.id]=(messagePlacements[e.id]&0xe1000000u)|uint32_t(value(e,0));return 2;
        // Underground extension64..70: facing toward NPC, explicit Mario/party
        // placements, source trio run/damage pose prefixes, BGM restoration.
        // Party selector1 means secondary Frankly; other used selector is G.
        case 64:case 65: {
            const unsigned actor=tag==64?0:value(e,0)==1?2:1,target=npc(e,tag==64?0:1);
            const auto& a=scene->actors[actor].position;const auto& b=scene->actors[target].position;
            float heading=std::atan2(b[0]-a[0],a[2]-b[2])*57.29577951308232f;
            scene->actors[actor].yaw=heading<0?heading+360:heading;return 2;
        }
        case 66:setPosition(0,0);return 2;
        case 67:setPosition(value(e,0)==1?2:1,1);return 2;
        case 68:case 69: {
            const unsigned actor=npc(e,0);const char* prefix=actor==3?"KUR":actor==4?"PTK":"TGK";
            scene->actors[actor].pose=std::string(prefix)+(tag==68?"_R_1":"_D_1");scene->actors[actor].poseAt=scene->now;return 2;
        }
        case 70:scene->music=0;return 2;
        case 63:if(value(e,0)==1)scene->professorControlEnabled=value(e,1)!=0;return 2;
        // Native music projection:-1=off,1=non-null scene BGM token,0=restore;
        // full original BGM/ENV/SFX mix remains a different presentation owner.
        case 30:scene->music=-1;return 2;
        case 31:scene->music=pointer(e,1)?1:0;return 2;
        case 35:
            if(scene->fieldParty.bound() || partyCalls.count(e.id))return partyCall(e,function,first);
            positionOut(e,1,value(e,0)==1?2:1);return 2;
        case 36:
            if(scene->fieldParty.bound() || partyCalls.count(e.id)){
                if(partyCall(e,function,first)!=2)return 0;
                const auto index=int8_t(scene->fieldParty.mario->partyIndices[1]);
                const auto* actor=index>=0 && index<2?scene->fieldParty.parties->pointer(index):nullptr;
                scene->temporaryProfessor=scene->temporaryProfessor && actor && actor->bytes[0x31]==11;
            }else if(value(e,0)==1)scene->temporaryProfessor=false;
            return 2;
        // Capture admission and completion are separate. Bounding box43 holds
        // for actual renderer captureReady and writes four INTEGER outputs.
        // Animation wait50 must first yield, then requires A_1 elapsed140/60s;
        // visibility/release and A_saku_04 collision request remain independent.
        case 41:scene->captureRequested=true;return 2;
        case 43:
            if(!scene->captureReady)return 0;
            for(unsigned i=0;i<4;++i)setEventValue(e,operand(e,i+1),scene->captureBounds[i],vm.interpreterServices().operands);
            return 2;
        case 46:scene->fencePose=string(pointer(e,1));scene->fencePoseAt=scene->now;return 2;
        case 47:scene->fenceVisible=value(e,1)!=0;return 2;
        case 50:
            if(first)return 0; // Original wait does not complete on admission.
            return scene->fencePose=="A_1" && double(scene->now-scene->fencePoseAt)*60>=double(fenceFrames)*1000?2:0;
        case 52:scene->fenceVisible=false;return 2;
        case 54:
            if(string(pointer(e,0))=="A_saku_04" && value(e,1)==1 && value(e,2)==0)scene->fenceCollisionOff=true;
            return 2;
        // Eight float operands are two ordered2D segments, output operand8.
        // Preserve ordered endpoint rejection and fused cross products; >0
        // rejects same-side pairs, so endpoint/collinear cases stay inclusive.
        // Replacing with epsilon/general geometry changes source branch behavior.
        case 55: {
            float a[8];for(unsigned i=0;i<8;++i)a[i]=number(e,i);
            bool hit=true;
            for(unsigned axis=0;axis<2;++axis) {
                const float x=a[axis],y=a[axis+2],c=a[axis+4],d=a[axis+6];
                if(x>=y?((x<c && x<d)||(y>c && y>d)):((y<c && y<d)||(x>c && x>d)))hit=false;
            }
            if(hit) {
                const float u=a[1]-a[3],v=a[0]-a[2];
                const float c=std::fma(v,a[5]-a[1],u*(a[0]-a[4])),d=std::fma(v,a[7]-a[1],u*(a[0]-a[6]));
                hit=!(c*d>0);
            }
            if(hit) {
                const float u=a[5]-a[7],v=a[4]-a[6];
                const float c=std::fma(v,a[1]-a[5],u*(a[4]-a[0])),d=std::fma(v,a[3]-a[5],u*(a[4]-a[2]));
                hit=!(c*d>0);
            }
            setEventValue(e,operand(e,8),hit?1:0,vm.interpreterServices().operands);return 2;
        }
        //56 choice,57 append,58 partykill->NPC projection,59 battle info,
        //60 start/61 wait require the supplied FIELD battle owner. Missing owner
        // blocks;62 previous-party placement always retains its service barrier.
        case 56:return select(e,first);
        case 57:return append(e,first);
        case 58:
            if((scene->fieldParty.bound() || partyCalls.count(e.id)) && partyCall(e,function,first)!=2)return 0;
            scene->goombellaNpc=true;return 2;
        case 59:scene->battleSetup=value(e,1);return 2;
        case 60:{
            if(!scene->fieldBattle.work){scene->pendingService=function;return 0;}
            const auto result=startFieldBattle(int32_t(npc(e,0)),first,scene->fieldBattle);
            scene->pendingService=result==0?function:0;return result;
        }
        case 61:{
            if(!scene->fieldBattle.work){scene->pendingService=function;return 0;}
            const auto result=waitFieldBattleEnd(scene->fieldBattle);
            scene->pendingService=result==0?function:0;return result;
        }
        case 62:return partyCall(e,function,first); // Actual previous-position reconstruction owns its full native wait.
        // Source music,effects,item/paper/display/party-flag requests are retained
        // above for native consumers. They do not fabricate story/dialogue.
        default:return 2;
        }
    }
    // Private1MiB guest heap0x82000000..0x82100000;16-byte SDK descriptor
    // area0x8041f070, current heap0x8041e428, handle0x8041e844. GP+4=60Hz.
    // Seed actual GSW0 and GSF1207(departed), then priority10/flags0 root.
    // Original script writes are read back by tick; native code does not infer
    // story progression from a completed animation or a message count.
    bool initialize(uint32_t entry,int32_t initialStory) {
        if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(1024*1024)) ||
           !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) ||
           !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
           !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !vm.memory.importScripts(scene->battleIntro?battleScripts:scene->underground?undergroundScripts:scripts))return false;
        auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0)return false;
        if(!vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
        vm.gp.timestamp=scene->now*eventClockRate(243000000);vm.initializeContexts();
        const auto op=vm.interpreterServices().operands;writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,initialStory,op);
        writeSavedSwitch(SavedSwitchFamily::GlobalFlag,1207,scene->professorDeparted?1:0,op);
        auto creation=vm.creationServices();rootId=createRootEvent(vm.schedule,entry,10,0,creation).id;return true;
    }
};
// Original first-talk story8, Frankly(-135,0,-240); tick immediately with
// doorBusy=true so source placement can precede the original door-end barrier.
bool ProfessorStory::begin(uint64_t time,std::array<float,3> mario,std::array<float,3> goombella,bool departed) {
    if(!ready || triggered)return false;
    *this=ProfessorStory{};triggered=true;now=time;professorDeparted=departed;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[1].pose="PKR_S_1";
    actors[2].position={-135,0,-240};actors[2].npcActive=true;actors[2].placementGeneration=1;
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(script,8)){failed=true;return false;}tick(time,0,true);return !failed;
}
// Fence may replace a FINISHED first-talk owner only while temporary Frankly
// exists. Preserve his entire actor snapshot/departed flag through reset; fresh
// story9 VM owns the subsequent scene, not the completed first-talk VM.
bool ProfessorStory::beginFence(uint64_t time,std::array<float,3> mario,std::array<float,3> goombella) {
    if(!ready || running() || !temporaryProfessor)return false;
    const auto professor=actors[2];const bool departed=professorDeparted;
    *this=ProfessorStory{};triggered=true;fenceStarted=true;temporaryProfessor=true;story=9;now=time;
    professorDeparted=departed;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[2]=professor;
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(fenceScript,9)){failed=true;return false;}tick(time,0,true);return !failed;
}
// Story10 reload already hides fence/disables collision and restores Frankly
//(-220,0,305), then starts original passive crossing event with door free.
bool ProfessorStory::beginLecture(uint64_t time,std::array<float,3> mario,std::array<float,3> goombella,bool departed) {
    if(!ready || triggered)return false;
    *this=ProfessorStory{};triggered=true;fenceStarted=true;fenceHidden=true;fenceCollisionOff=true;story=10;now=time;
    professorDeparted=departed;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[2].position={-220,0,305};
    actors[2].npcActive=true;actors[2].placementGeneration=1;
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(lectureScript,10)){failed=true;return false;}tick(time,0,false);return !failed;
}
// Only upper story bound11 is checked here; caller owns valid lower domain.
// Departed flag supplies temporary Frankly presence. Trio spawn hidden atY-1000
// until actual event placement; actor order KUR/PTK/TGK differs from unit records.
bool ProfessorStory::beginUnderground(uint64_t time,int32_t initialStory,std::array<float,3> mario,std::array<float,3> goombella,bool departed) {
    if(!ready || !undergroundReady || triggered || initialStory>11)return false;
    *this=ProfessorStory{};underground=true;triggered=true;temporaryProfessor=departed;professorDeparted=departed;story=initialStory;now=time;
    actors[0].position=mario;actors[0].pose="M_S_1";actors[1].position=goombella;actors[1].pose="PKR_S_1";
    actors[2].position=mario;actors[2].hidden=!departed;actors[2].yaw=270;
    actors[2].npcActive=departed;actors[2].placementGeneration=1;
    for(unsigned i=0;i<3;++i){auto& a=actors[i+3];a.position={0,-1000,0};a.hidden=true;a.yaw=270;a.pose=std::string(i==0?"KUR":i==1?"PTK":"TGK")+"_S_1";}
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(undergroundEntry,initialStory)){failed=true;return false;}tick(time,0,false);return !failed;
}
// Scoped original lecture init->entry->phase chain, not general battle Main.
// Mario/G native presentation starts(-100,0,-5)/(-140,0,5); remap source unit
// order Goomba/Spiky/Para to actor3/5/4. Only entry0 starts now; tick appends1/2.
bool ProfessorStory::beginTrioBattleIntro(uint64_t time) {
    if(!ready || !undergroundReady || !battleReady || triggered)return false;
    *this=ProfessorStory{};battleIntro=true;underground=true;triggered=true;keysEnabled=false;temporaryProfessor=true;story=11;now=time;
    actors[0].position={-100,0,-5};actors[0].pose="M_S_1";actors[0].yaw=90;
    actors[1].position={-140,0,5};actors[1].pose="PKR_S_1";actors[1].yaw=90;
    actors[2].position=battleUnits[3].position;actors[2].pose="S_1";
    actors[2].npcActive=true;actors[2].placementGeneration=1;
    for(unsigned i=0;i<3;++i){const unsigned actor=i==0?3:i==1?5:4;actors[actor].position=battleUnits[i].position;actors[actor].pose=std::string(actor==3?"KUR":actor==4?"PTK":"TGK")+"_S_1";actors[actor].yaw=270;}
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(battleEntries[0],11)){failed=true;return false;}tick(time,0,false);return !failed;
}
// Execute original pipe writer synchronously for one manager visit; success
// requires no live events, no failure and actual departed flag. Does not invent
// a deferred success while its original event remains waiting.
bool ProfessorStory::departThroughPipe(uint64_t time,int32_t initialStory,bool departed) {
    if(!ready || running())return false;
    *this=ProfessorStory{};triggered=true;now=time;story=initialStory;professorDeparted=departed;
    runtime_=std::make_shared<ProfessorRuntime>();runtime_->scene=this;
    if(!runtime_->initialize(pipeScript,initialStory)){failed=true;return false;}
    tick(time,0,false);
    return finished && !failed && professorDeparted;
}
// Presentation-only story11 restore: departed hides/removes NPC atY-1000;
// otherwise source root(-220,0,305), yaw270. Bit0x40000000 is explicitly owned
// through flagMask. No interpreter is created or story dialogue replayed.
void ProfessorStory::restoreProfessor(bool departed) {
    *this=ProfessorStory{};triggered=true;finished=true;story=11;professorDeparted=departed;
    actors[2].position=departed?std::array<float,3>{0,-1000,0}:std::array<float,3>{-220,0,305};
    actors[2].hidden=departed;actors[2].yaw=270;actors[2].flags=0x40000000;
    actors[2].placementGeneration=1;
    actors[2].npcActive=!departed;actors[2].flagMask=0x40000000;
}
// Monotonic ms; move -> message -> selection -> VM -> saved-state readback.
// After event finish, text may still drain, but VM stays stopped. Completion
// counts ALL allocations/children; battleIntro then creates the next of three
// roots on a later manager pass, preserving separate source init/entry/phase.
bool ProfessorStory::nativePartyPending() const{return runtime_ && (!runtime_->partyCalls.empty() || !runtime_->retiredPartyCalls.empty());}
void ProfessorStory::tick(uint64_t time,uint32_t keys,bool doorBusy,const MessageSelectionInput& selectionInput) {
    if(failed || time<now || !runtime_)return;
    if(runtime_->retiredPartyPending){now=time;pendingService=runtime_->retiredPartyPending;finished=false;return;}
    if(!running()){now=time;if(message.active())message.tick(now,keys);return;}
    now=time;
    auto& r=*runtime_;r.scene=this;r.keys=keys;r.doorBusy=doorBusy;
    if(r.retiredPartyPending){pendingService=r.retiredPartyPending;return;}
    r.advance();
    if(message.active())message.tick(now,keys);
    if(selection.active())selection.tick(selectionInput);
    r.vm.gp.timestamp=now*eventClockRate(243000000);r.vm.runManager();
    if(r.retiredPartyPending){pendingService=r.retiredPartyPending;finished=false;return;}
    story=readSavedSwitch(SavedSwitchFamily::GlobalByte,0,r.vm.interpreterServices().operands);
    professorDeparted=readSavedSwitch(SavedSwitchFamily::GlobalFlag,1207,r.vm.interpreterServices().operands)!=0;
    finished=r.vm.runtime.allocationCount==0;
    if(finished && battleIntro && battleIntroStage<2) {
        ++battleIntroStage;auto creation=r.vm.creationServices();
        r.rootId=createRootEvent(r.vm.schedule,battleEntries[battleIntroStage],10,0,creation).id;finished=false;
    }
}
}
