#include "trio_attack.hpp"
#include "battle_event_callbacks.hpp"
#include "event_memory_bootstrap.hpp"
#include <cmath>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include "battle_action_services.hpp"
#include "battle_timing_a.hpp"
#include "battle_weapon_cost.hpp"
#include "battle_weapon_power.hpp"
#include "battle_damage_application.hpp"
#include "event_lifecycle.hpp"
namespace ttyd {
namespace {
EventScriptAssets scripts;bool assetsReady=false;
std::array<std::array<uint32_t,3>,3> reactionUnits{};
constexpr uint32_t Battle=0x84000000,Units=0x84100000,Parts=0x84200000,Kinds=0x84300000;
constexpr std::array<unsigned,7> visual{{0,0,1,3,5,4,2}};
void required(bool ok){if(!ok)std::abort();}
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
}
bool loadTrioAttackAssets(const char* root) {
    if(!root)return false;
    EventScriptAssets next;if(!next.load((std::string(root)+"/trio-attack-events.pmevt").c_str()))return false;
    uint32_t command=0;if(!next.readWord(0x80355cf0,command) || command!=0x5005b)return false;
    FILE* f=std::fopen((std::string(root)+"/trio-attack-units.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint32_t count=0;decltype(reactionUnits) units{};
    const bool valid=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMTHIT1",8) && std::fread(&count,4,1,f)==1 && count==3 && std::fread(units.data(),12,3,f)==3 && std::fgetc(f)==EOF;
    std::fclose(f);if(!valid)return false;
    scripts=std::move(next);reactionUnits=units;assetsReady=true;return true;
}
struct TrioAttackRuntime {
    EventNativeRuntime vm;
    EventCallbackRegistry registry;
    BattleDataChildServices child;
    BattleTakeoffBackend takeoffBackend;
    BattlePackedTakeoffServices takeoff;
    BattlePackedMovePositionServices movement;
    BattleMarioJumpBackend jumpBackend;BattlePackedMarioJumpServices jump;
    BattleTriggerHistory history;BattleRandom random{0}; // Explicit native seed adapter; original seed ownership is open.
    std::array<DamageUnit,7> damageUnits{};std::array<DamagePart,7> damageParts{};
    std::array<std::array<int8_t,5>,7> defenses{},defenseAttributes{};
    std::vector<DamageEffect> damageEffects;std::vector<std::string> soundRequests;std::vector<DamageNotice> notices;
    ProfessorStory* scene=nullptr;
    uint64_t now=0;uint32_t root=0,pending=0;
    bool done=false;
    std::vector<ProfessorServiceRequest> trace;
    TrioAttackRuntime():vm(0x81000000,platform(this)) {}
    static EventNativePlatform platform(TrioAttackRuntime* self) {
        EventNativePlatform p;p.user=self;p.clock=[](void*)->uint32_t{return 243000000;};
        p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first)->int32_t{return static_cast<TrioAttackRuntime*>(p)->call(e,fn,first);};
        p.truncate=[](void*,float v)->int32_t{return int32_t(v);};p.divideWord=[](void*,int32_t a,int32_t b)->int32_t{return a/b;};
        p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
        p.signedWordFloat=[](void*,int32_t v){return float(v);};p.addFloat=[](void*,float a,float b){return a+b;};
        p.subtractFloat=[](void*,float a,float b){return a-b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};p.divideFloat=[](void*,float a,float b){return a/b;};
        p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};
        p.distanceFloat=[](void*,float a,float b,float c,float d){return std::hypot(c-a,d-b);};
        p.headingFloat=[](void*,float a,float b,float c,float d){return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};
        p.sineDouble=[](void*,double v){return std::sin(v);};p.cosineDouble=[](void*,double v){return std::cos(v);};p.roundDoubleFloat=[](void*,double v){return float(v);};
        p.addDouble=[](void*,double a,double b){return a+b;};p.subtractDouble=[](void*,double a,double b){return a-b;};p.multiplyDouble=[](void*,double a,double b){return a*b;};p.divideDouble=[](void*,double a,double b){return a/b;};
        p.fmaDouble=[](void*,double a,double b,double c){return std::fma(a,b,c);};p.fmodDouble=[](void*,double a,double b){return std::fmod(a,b);};return p;
    }
    uint32_t word(uint32_t a)const{uint32_t v=0;required(vm.memory.readWord(a,v));return v;}
    void word(uint32_t a,uint32_t v){required(vm.memory.writeWord(a,v));}
    uint8_t byte(uint32_t a)const{uint8_t v=0;required(vm.memory.readByte(a,v));return v;}
    void byte(uint32_t a,uint8_t v){required(vm.memory.writeByte(a,v));}
    int16_t half(uint32_t a)const{uint16_t v=0;required(vm.memory.readHalf(a,v));return int16_t(v);}
    void half(uint32_t a,int32_t v){required(vm.memory.writeHalf(a,uint16_t(v)));}
    void real(uint32_t a,float v){word(a,bits(v));}
    float real(uint32_t a)const{const auto v=word(a);float f;std::memcpy(&f,&v,4);return f;}
    int32_t raw(EventEntry& e,unsigned i)const{return int32_t(word(e.arguments+4*i));}
    int32_t value(EventEntry& e,unsigned i){return getEventValue(e,raw(e,i),vm.interpreterServices().operands);}
    void output(EventEntry& e,unsigned i,int32_t v){setEventValue(e,raw(e,i),v,vm.interpreterServices().operands);}
    uint32_t unit(EventEntry& e,unsigned operand=0){const int32_t id=translateBattleActor(&e,value(e,operand),nativeBattleActorServices(vm));return word(Battle+0x20+4*id);}
    std::string text(uint32_t address)const{std::string s;for(unsigned i=0;i<128;++i){const auto c=byte(address+i);if(!c)return s;s+=char(c);}return {};}
    int32_t call(EventEntry& e,uint32_t fn,int32_t first) {
        if(first){ProfessorServiceRequest r{fn,e.currentCommand,now,{}};for(unsigned i=0;i<e.argumentCount;++i)r.rawOperands.push_back(raw(e,i));trace.push_back(std::move(r));}
        int32_t result;if(registry.invoke(e,fn,first,result))return result;
        switch(fn) {
        case 0x8010ad00: { // Source fresh no-protector domain; retain both output writes.
            (void)unit(e);const int32_t target=translateBattleActor(&e,value(e,1),nativeBattleActorServices(vm));const int32_t part=value(e,2);
            for(unsigned id=1;id<7;++id)if(int32_t(word(Units+id*0x1000+0x140))==target){if(!pending)pending=fn;return 0;}
            output(e,1,target);output(e,2,part);return 2;
        }
        case 0x801059a0: { // Ordinary item0/FP0/AP0 still records its action count.
            const auto u=unit(e),weapon=uint32_t(value(e,1));
            if(word(weapon+8) || byte(weapon+0x11) || byte(weapon+0x12)){if(!pending)pending=fn;return 0;}
            uint16_t fp=0;required(vm.memory.readHalf(u+0x112,fp));required(vm.memory.writeHalf(u+0x112,fp));
            if(word(u+8)==222){auto points=byte(Battle+0x16f4b),count=byte(Battle+0x16f4c);addActionPoint(points,0);addActionCount(count);byte(Battle+0x16f4b,points);byte(Battle+0x16f4c,count);}
            return 2;
        }
        case 0x801058e0:output(e,1,byte(uint32_t(value(e,0))+0x6e));return 2;
        case 0x80107b38:{const auto u=unit(e);const int32_t base=value(e,1);byte(Battle+0x1cbc,base);byte(Battle+0x1cbd,std::clamp(base-int32_t(byte(u+0x305))+int32_t(byte(u+0x306)),int32_t(0),int32_t(6)));return 2;}
        case 0x801077ec: {
            const auto u=unit(e);const int32_t id=value(e,1),arg=value(e,3);const auto options=uint32_t(raw(e,2));
            word(Battle+0x1c90,u);word(Battle+0x1c94,options);word(Battle+0x1c9c,0);word(Battle+0x1cb4,0);word(Battle+0x1cb8,1);
            for(unsigned o:{0x1ca0u,0x1ca8u,0x1ca4u,0x1cacu})word(Battle+o,0);
            word(Battle+0x1c98,arg);word(Battle+0x1cc0,0);
            for(uint32_t r=0x80364348;word(r);r+=20)if(word(r)==uint32_t(id)) {
                word(Battle+0x1ca0,word(r+4));if(!(word(u+0x27c)&0x10))word(Battle+0x1ca8,word(r+12));
                word(Battle+0x1ca4,word(r+8));word(Battle+0x1cac,word(r+16));break;
            }
            return 2;
        }
        case 0x8010e5a8:byte(unit(e)+0x189,value(e,1));return 2;
        case 0x8010e4bc:{const auto u=unit(e);byte(u+0x189,byte(Battle+0xa+8*byte(u+0xc)));return 2;}
        case 0x8010b7c4:{const auto u=unit(e);const int32_t amount=value(e,2),sign=int8_t(byte(u+0x189));const auto x=value(e,1);output(e,1,int32_t(uint32_t(x)-uint32_t(amount)*uint32_t(sign)));return 2;}
        case 0x8010e684: {
            const auto u=unit(e);const int32_t actorPart=value(e,1),target=value(e,2),targetPart=value(e,3),mode=raw(e,4);
            if(actorPart!=-1 || (mode!=1 && mode!=16)){if(!pending)pending=fn;return 0;}
            float x=float(target);if(mode==16){const int32_t id=translateBattleActor(&e,target,nativeBattleActorServices(vm));const auto t=word(Battle+0x20+4*id),part=packedUnitPart(t,targetPart,nativeBattleActorServices(vm).memory);if(word(part+0x1ac)&0x10000000){if(!pending)pending=fn;return 0;}x=real(t+0x3c)+real(part+0x24);}
            const float delta=x-real(u+0x3c);output(e,5,delta>0?1:delta<0?-1:int8_t(byte(u+0x189)));return 2;
        }
        case 0x80106b20: {
            const auto u=unit(e);const int32_t partId=value(e,1),type=value(e,2);const auto p=packedUnitPart(u,partId,nativeBattleActorServices(vm).memory);
            uint32_t table=word(p+0x1bc),name=0;if(!table){if(!pending)pending=fn;return 0;}
            for(unsigned i=0;i<70 && word(table+8*i+4);++i)if(word(table+8*i)==uint32_t(type)){name=word(table+8*i+4);break;}
            if(!name){if(!pending)pending=fn;return 0;}changePose(u,p,text(name),false);return 2;
        }
        case 0x80106bb8:{const auto u=unit(e);const auto p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory);changePose(u,p,text(uint32_t(value(e,2))),false);return 2;}
        case 0x8010a78c: {
            const auto a=unit(e),d=unit(e,1),p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),w=uint32_t(value(e,3));const uint32_t flags=uint32_t(raw(e,4));
            if(word(d+8)!=1 || word(p+0x1b0) || w!=0x803538a8){if(!pending)pending=fn;return 0;}
            word(d+0x258,p);const int32_t result=checkGroundDamage(a,d,p,w,flags);output(e,5,result);return e.flags&1?2:255;
        }
        case 0x801064d8:{const auto u=unit(e),p=packedUnitPart(u,value(e,1),nativeBattleActorServices(vm).memory);for(unsigned o=0x118;o<=0x135;++o)if(byte(u+o)){if(!pending)pending=fn;return 0;}if(word(u+0x104)&0x20000){if(!pending)pending=fn;return 0;}const auto table=word(p+0x1bc);uint32_t name=0;for(unsigned i=0;i<28 && word(table+8*i+4);++i)if(word(table+8*i)==28){name=word(table+8*i+4);break;}if(!name){if(!pending)pending=fn;return 0;}changePose(u,p,text(name),false);word(p+0x204,(word(p+0x204)&~uint32_t(2|0x10|0x80|4))|1);return 2;}
        case 0x801141a4:word(Battle+0x1c70,0);byte(Battle+0x1c74,0);return 2;
        case 0x8010dcb0:{const auto kind=int32_t(word(unit(e)+8));output(e,1,kind==222 || (kind>=224 && kind<231));return 2;}
        case 0x801140fc:{const auto u=unit(e);if(!u)return 2;for(unsigned o:{0x2a8u,0x2b4u})for(int i=0;i<vm.battle.capacity;++i)if(word(u+o) && (vm.battle.entries[i].flags&1) && vm.battle.entries[i].id==word(u+o))return 0;return 2;}
        case 0x80105e50:{const auto u=unit(e);const int32_t type=value(e,1),duration=value(e,2);(void)value(e,3);const bool on=value(e,4)!=0;if(type!=27){if(!pending)pending=fn;return 0;}byte(u+0x135,on?duration:0);return 2;}
        case 0x80111720:{const auto u=unit(e);for(unsigned i=0;i<3;++i)real(u+0x6c+4*i,float(value(e,i+1)));return 2;} // Used fresh zero rotation.
        case 0x80106dfc: {
            if(first){const auto id=translateBattleActor(&e,value(e,0),nativeBattleActorServices(vm));word(Battle+0x271c,0);for(unsigned i=0;i<3;++i)word(Battle+0x2720+4*i,value(e,i+1));word(Battle+0x272c,id);word(Battle+0x2730,0);word(Battle+0x2734,value(e,4));}
            const auto id=int32_t(word(Battle+0x272c));const uint32_t a=id>=0 && id<64?word(Battle+0x20+4*id):0;const int32_t t=int32_t(word(Battle+0x271c)+1);word(Battle+0x271c,t);
            int32_t result=0;const int32_t lo=int32_t(word(Battle+0x2720)),hi=int32_t(word(Battle+0x2724)),end=int32_t(word(Battle+0x2728));
            if(!a || (word(a+0x27c)&0x10))result=1;
            else {const int32_t scan=int32_t(word(Battle+0x2734));if(t<=1 && scan>0)for(int32_t i=0;i<scan;++i)if(history.entries[i]&0x100){result=-1;break;}
                if(!result && ((history.entries[0]&0x100) || (byte(a+0x307) && lo<t && t<=hi)))result=t<=lo?-1:t<=hi?2:-2;
                if(!result && t>=end)result=1;
            }
            if(result){word(Battle+0x2730,result);return 1;}return 0;
        }
        case 0x80106d7c:{const auto result=int32_t(word(Battle+0x2730)),count=int32_t(word(Battle+0x271c));if(raw(e,0)!=-250000000)output(e,0,result);if(raw(e,1)!=-250000000)output(e,1,count);return 2;}
        case 0x80139898:return 2; // Original no-stylish audience request retained.
        case 0x801075c4:{const int32_t actor=value(e,0),input=value(e,1);int32_t result=std::clamp(input+int32_t(byte(Battle+0x1cbd))-1,int32_t(0),int32_t(6));if(actor!=-1 && actor!=-5 && (word(unit(e)+0x278)&255)==30)result=-1;output(e,2,result);return 2;}
        case 0x80106fa0:return 2; // Original success-effect request retained in trace.
        case 0x8010f2e8:output(e,2,(word(unit(e)+0x278)&uint32_t(raw(e,1)))?1:0);return 2;
        case 0x8010f0e8:output(e,2,word(unit(e)+0x274)==uint32_t(value(e,1)));return 2;
        case 0x8010f374:output(e,1,word(unit(e)+0x278)&255);return 2;
        case 0x8010f458:output(e,1,int8_t(byte(unit(e)+0x270)));return 2;
        case 0x8010ed8c:output(e,1,int32_t(word(unit(e)+8)));return 2;
        case 0x80105b40:{const auto u=unit(e);word(u+0x1c,word(u+0x1c)|uint32_t(value(e,1)));return 2;}
        case 0x80105ae0:{const auto u=unit(e);word(u+0x1c,word(u+0x1c)&~uint32_t(value(e,1)));return 2;}
        case 0x8010e430:output(e,2,(word(unit(e)+0x104)&uint32_t(value(e,1)))?1:0);return 2;
        case 0x8010a154: {
            const auto a=word(Battle+0x20+4*word(Battle+0x420));if(!a || !word(a+0x2a8))return 2;
            const auto id=word(a+0x2a8);for(int i=0;i<vm.battle.capacity;++i)if((vm.battle.entries[i].flags&1) && vm.battle.entries[i].id==id)return 0;
            word(a+0x2a8,0);return 2;
        }
        case 0x80107758:if(word(Battle+0x1cac)==0x8012a7c0)word(Battle+0x1c9c,1000);return 2;
        case 0x8010d208:return genericJump(e,first);
        // Feedback/effect providers retain original requests; source event waits
        // and unit/reaction state remain executing, rather than flattened timers.
        case 0x8010749c:case 0x80139af4:case 0x8010436c:case 0x801054e8:return 2;
        case 0x80113950:case 0x801138d8:output(e,1,0);return 2; // Fresh unequipped drain badges.
        case 0x8010abac: { // Bounded fresh trio precheck. Changed status/badges/fog await generic binding.
            const auto a=unit(e),d=unit(e,1),p=packedUnitPart(d,value(e,2),nativeBattleActorServices(vm).memory),w=uint32_t(value(e,3));const uint32_t flags=value(e,4);
            for(unsigned o=0x118;o<=0x135;++o)if(byte(a+o) || byte(d+o)){if(!pending)pending=fn;return 0;}
            if(byte(d+0x2e0) || byte(d+0x2e1) || byte(d+0x2e2) || word(Battle+0x182bc) || (word(d+0x104)&0x18) || (word(p+0x1ac)&(0x4000|0x40)) || (word(p+0x1b0)&2) || byte(w+0x10)!=100){if(!pending)pending=fn;return 0;}
            if(word(w+0x74)&4)word(a+0x27c,word(a+0x27c)|8);
            if(!(flags&0x100000) && !(word(w+0x74)&8))(void)random.bounded(100);
            output(e,5,1);return 2;
        }
        case 0x80106f40:{const bool on=value(e,0)!=0;const auto mask=uint32_t(raw(e,1));word(Battle+0x1f50,on?word(Battle+0x1f50)|mask:word(Battle+0x1f50)&~mask);return 2;}
        case 0x80106ef0:output(e,0,timing().successWindow(byte(Battle+0x1cbd)));return 2;
        case 0x80107708:{auto c=timing();TimingAContext context;context.difficulty=byte(Battle+0x1cbd);context.setupOptions=word(Battle+0x1c94);context.actorFlags=word(word(Battle+0x1c90)+0x27c);context.battleFlags=word(Battle+0xef4);c.evaluate(history,context,random);storeTiming(c);return c.result&1?0:2;}
        case 0x80107660:output(e,0,int32_t(word(Battle+0x1cb8)));return 2;
        case 0x80107548:output(e,0,int32_t(word(Battle+0x1cb4)));return 2;
        case 0x80107788: {
            int32_t state=int32_t(word(Battle+0x1c9c));const uint32_t actorFlags=word(word(Battle+0x1c90)+0x27c);
            ActionCommandView view;view.state=&state;view.actorFlags=&actorFlags;view.help=[](void*,int32_t){};
            ActionValueServices values;values.user=&e;values.resolve=[](void*,int32_t v){return v;};
            startActionCommand(view,value(e,0),values);word(Battle+0x1c9c,state);return 2;
        }
        // Camera provider requests retain their original operands in the trace.
        // Native stage camera stays at the existing source-scene adapter view.
        case 0x8012bfd0:case 0x8012bc70:case 0x8012ba14:case 0x8012b8cc:
        case 0x80104700:case 0x80104508:return 2; // Retained original camera/SE/voice requests; presentation providers remain open.
        default:break;
        }
        // Deliberately no success fallback. A pending owner remains suspended
        // until its callback is implemented; it cannot spend a turn or win.
        if(!pending)pending=fn;
        return 0;
    }
    bool initialize(const TrioBattleCommands& menu,ProfessorStory& s,int16_t hp) {
        scene=&s;now=s.now;
        for(auto span:std::array<std::pair<uint32_t,unsigned>,10>{{{0x82000000,0x100000},{0x8041f070,16},{0x8041e428,4},{0x8041e844,4},{0x8041eb90,4},{0x8041eb00,4},{Battle,0x19000},{Units,0x10000},{Parts,0x10000},{Kinds,0x1000}}})
            if(!vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)))return false;
        if(!vm.memory.registerRam(0x85000000,std::vector<uint8_t>(0x600)) || !vm.memory.importScripts(scripts))return false;
        auto m=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,m);
        if(eventSdkCreateHeap(start,0x82100000,m)!=0)return false;
        word(0x8041e844,0);vm.gp.refreshRate=60;vm.gp.timestamp=now*eventClockRate(243000000);vm.initializeContexts();vm.gp.battleContext=1;
        word(0x8041eb90,Battle);word(0x8041eb00,0x85000000);byte(Battle+0xa,1);byte(Battle+0x12,255);
        word(Battle+0x420,menu.actor);word(Battle+0x424,6);word(Battle+0x1c5c,0);word(Battle+0x171c+0x4b0,0);word(Battle+0x171c+0x80,menu.attack->weapon);
        for(unsigned i=0;i<menu.candidates.bytes.size();++i)byte(Battle+0x428+i,menu.candidates.bytes[i]);
        for(unsigned id=1;id<7;++id) {
            const uint32_t u=Units+id*0x1000,p=Parts+id*0x1000,k=Kinds+id*0x200;
            word(Battle+0x20+id*4,u);word(u,id);word(u+8,menu.units[id].kind);byte(u+0xc,menu.units[id].belong);
            word(u+0x140,0xffffffff);byte(u+0x21,id==6?0:1);byte(u+0x22,id==6?0:1);byte(u+0x23,id==6?0:1);word(u+0x2c,id<3?0x04000002:0x04000004);word(u+0x10,k);word(u+0x14,p);byte(k+0xb4,1);word(p+4,k+0x100);word(k+0x100,1);word(p+0x4ec,u);
            byte(u+0x189,s.actors[visual[id]].yaw>0 && s.actors[visual[id]].yaw<180?1:255);byte(p+0xbd,id<3?1:255);real(u+0x114,1);
            for(unsigned axis=0;axis<3;++axis){real(u+0x3c+4*axis,s.actors[visual[id]].position[axis]);real(u+0x30+4*axis,s.actors[visual[id]].position[axis]);}
            if(id>=3) {
                const auto& spec=trioBattleUnitSpecs()[id-3];
                if(id<=5){const auto kind=reactionUnits[id-3][0],definition=word(kind+0xb8);word(u+0x10,kind);word(u+0x18,word(kind+0xc0));word(u+0x2b0,reactionUnits[id-3][1]);word(u+0x28c,reactionUnits[id-3][2]);word(p+4,definition);word(p+0x1bc,word(definition+0x48));word(p+0x1b4,word(definition+0x38));word(p+0x1b8,word(definition+0x3c));}
                half(u+0x108,spec.hp);
                word(u+0x104,spec.attributes);word(p+0x1ac,spec.partAttributes);word(p+0x1b0,spec.counterAttributes);
                required(vm.memory.writeHalf(u+0x10c,spec.hp));required(vm.memory.writeHalf(p+0x1a0,spec.forwardBias));
                for(unsigned axis=0;axis<3;++axis){real(p+0x24+axis*4,spec.partPosition[axis]);real(p+0x170+axis*4,spec.hitOffset[axis]);real(p+0x188+axis*4,spec.samplingOffset[axis]);}
            } else {
                word(p+0x1ac,1);
                if(id==1) {
                    word(u+0x10,0x80372afc);word(u+0x104,word(0x80372afc+0xac));word(u+0x18,word(0x80372afc+0xc0));
                    const uint32_t source=word(0x80372afc+0xb8);const unsigned count=byte(0x80372afc+0xb4);
                    for(unsigned j=0;j<count;++j) {
                        const uint32_t record=source+0x4c*j,part=p+0x500*j;word(part,j+1<count?part+0x500:0);word(part+4,record);word(part+0x4ec,u);byte(part+0xbd,1);
                        for(unsigned axis=0;axis<3;++axis){real(part+0x24+axis*4,real(record+0xc+axis*4));real(part+0x170+axis*4,real(record+0x18+axis*4));real(part+0x188+axis*4,real(record+0x24+axis*4));}
                        for(unsigned offset:{0x40u,0x44u,0x38u,0x3cu,0x48u})word(part+(offset==0x40?0x1ac:offset==0x44?0x1b0:offset==0x38?0x1b4:offset==0x3c?0x1b8:0x1bc),word(record+offset));
                    }
                    required(vm.memory.writeHalf(u+0x10c,hp));half(u+0x108,10);
                }
            }
        }
        child=nativeBattleDataChildServices(vm);takeoffBackend.runtime=&vm;takeoff=nativeBattleTakeoffServices(vm,nativeBattleTakeoffCoreServices(takeoffBackend));movement=nativeBattleMovePositionServices(vm,{});
        bool ok=bindBattleJumpSettingCallback(registry,vm) && bindBattleFlagQueryCallback(registry,vm) && bindBattleSelectedTargetCallback(registry,vm) && bindBattleCommandWeaponCallback(registry,vm) &&
            bindBattleWaitGuardMoveCallback(registry,vm) && bindBattleDataChildCallback(registry,child) && bindBattleMajinaiQueryCallback(registry,vm) && bindBattleGetPosCallback(registry,vm) &&
            bindBattleGetHomePosCallback(registry,vm) && bindBattleSetPosCallback(registry,vm) && bindBattleSetMoveSpeedCallback(registry,vm) && bindBattleTakeoffCallback(registry,takeoff) &&
            bindBattleMovePositionCallback(registry,movement) && bindBattleGetMoveFrameCallback(registry,vm) && bindBattleGetHitPosCallback(registry,vm) && bindBattleMarioJumpParameterCallback(registry,vm) && bindBattleStartWaitCallback(registry,vm) && bindBattleCheckStatusCallback(registry,vm) && bindBattleCheckDataCallback(registry,vm) && bindBattleSetFallAccelCallback(registry,vm);
        jumpBackend.runtime=&vm;jumpBackend.user=this;jumpBackend.play=[](void*,uint32_t,const std::array<float,3>&)->uint32_t{return 0;};
        jump=nativeBattleMarioJumpServices(jumpBackend);ok=ok && bindBattleMarioJumpPositionCallback(registry,jump);
        if(!ok)return false;
        auto& e=createRootEvent(vm.schedule,0x80355cf0,10,0,vm.creationServices());e.owner=menu.actor;root=e.id;word(Units+menu.actor*0x1000+0x2a8,e.id);return true;
    }
    void readDamage(unsigned id) {
        auto& d=damageUnits[id];const auto u=Units+id*0x1000,p=Parts+id*0x1000;
        d.kind=int32_t(word(u+8));d.belong=int8_t(byte(u+0xc));d.hp=half(u+0x10c);d.maxHp=half(u+0x108);d.fp=half(u+0x112);d.maxFp=half(u+0x10e);
        d.flags=word(u+0x27c);d.statusFlags=word(u+0x138);d.attributes=word(u+0x104);d.damageSequence=half(u+0x260);d.attackSequence=int32_t(word(u+0x280));
        for(unsigned i=0;i<30;++i)d.statusBytes[i]=byte(u+0x118+i);
        damageParts[id].attributes=word(p+0x1ac);
        if(id>=3 && id<=5){defenses[id]=trioBattleUnitSpecs()[id-3].defense;for(unsigned i=0;i<5;++i)defenseAttributes[id][i]=int8_t(trioBattleUnitSpecs()[id-3].defenseAttributes[i]);}
        damageParts[id].defense=defenses[id].data();damageParts[id].defenseAttributes=defenseAttributes[id].data();
    }
    void writeDamage(unsigned id) {
        const auto& d=damageUnits[id];const auto u=Units+id*0x1000;
        half(u+0x10c,d.hp);word(u+0x27c,d.flags);word(u+0x208,d.hpBefore);word(u+0x20c,d.hpAfter);word(u+0x264,d.requestedHp);word(u+0x268,d.requestedFp);word(u+0x26c,d.consumedFp);
        byte(u+0x270,d.lastHp);byte(u+0x271,d.lastFp);byte(u+0x272,d.lastConsumedFp);word(u+0x274,d.pattern);half(u+0x1fe,d.damageFlags);
    }
    DamageApplicationServices application() {
        DamageApplicationServices s;s.user=this;
        s.getUnit=[](void* p,int32_t id)->DamageUnit* {return id>=1 && id<7?&static_cast<TrioAttackRuntime*>(p)->damageUnits[id]:nullptr;};
        s.bodyPart=[](void* p,DamageUnit& d)->DamagePart* {auto& r=*static_cast<TrioAttackRuntime*>(p);return &r.damageParts[&d-r.damageUnits.data()];};
        s.hitPosition=[](void* p,DamageUnit& d,DamagePart&) {auto& r=*static_cast<TrioAttackRuntime*>(p);const unsigned id=&d-r.damageUnits.data();auto xyz=packedHitPosition(Units+id*0x1000,Parts+id*0x1000,nativeBattlePartPositionServices(r.vm));return DamagePosition{xyz[0],xyz[1],xyz[2]};};
        s.effect=[](void* p,const DamageEffect& effect){static_cast<TrioAttackRuntime*>(p)->damageEffects.push_back(effect);};
        s.sound=[](void* p,const char* key,DamagePosition){if(key)static_cast<TrioAttackRuntime*>(p)->soundRequests.emplace_back(key);};
        s.notice=[](void* p,DamageUnit&,DamageNotice notice){static_cast<TrioAttackRuntime*>(p)->notices.push_back(notice);};
        s.hitEvent=[](void* p,DamageUnit& d,uint32_t code){auto& r=*static_cast<TrioAttackRuntime*>(p);const unsigned id=&d-r.damageUnits.data();r.writeDamage(id);r.hitEvent(Units+id*0x1000,code);};
        s.inventory.itemCount=[](void*,int32_t)->int32_t{return 0;};return s;
    }
    void hitEvent(uint32_t u,uint32_t code) {
        const auto script=word(u+0x2b0);if(!script)return;
        for(unsigned offset:{0x2b4u,0x290u,0x29cu,0x2a8u}){const auto id=word(u+offset);if(id)deleteEventId(id,vm.creationServices());word(u+offset,0);}
        auto& e=createRootEvent(vm.schedule,script,10,0x20,vm.creationServices());e.owner=int32_t(word(u));word(u+0x278,code);word(u+0x2b4,e.id);
    }
    int32_t checkGroundDamage(uint32_t a,uint32_t d,uint32_t,uint32_t w,uint32_t flags) {
        const auto ai=word(a),di=word(d);readDamage(ai);readDamage(di);auto& attacker=damageUnits[ai];auto& defender=damageUnits[di];auto& part=damageParts[di];
        DamageWeapon weapon=ordinaryJumpWeapon();for(unsigned i=0;i<6;++i)weapon.powerWords[i]=int32_t(word(w+0x20+4*i));
        WeaponPowerServices power;power.user=this;power.actionResult=[](void* p){return static_cast<TrioAttackRuntime*>(p)->word(Battle+0x1cb8);};power.pouchLevel=[](void*,MarioPowerKind)->uint8_t{return 1;};
        power.bonus=[](void*,const DamageUnit& u,MarioPowerKind kind){return kind==MarioPowerKind::Jump?u.jumpPowerBonus:u.hammerPowerBonus;};
        power.powerWord=[](void* p,int32_t index)->int32_t {auto& r=*static_cast<TrioAttackRuntime*>(p);return int32_t(r.word(0x803538a8+0x20+4*index));};weapon.callbackData=&power;
        DamageBattleState state;state.actionResult=word(Battle+0x1cb8);uint32_t code=0;const auto hp=calculateHpDamage(&attacker,defender,part,weapon,code,flags,state,storedDamageStatusServices());const auto fp=calculateFpDamage(&attacker,defender,part,weapon,code,flags,state);
        defender.damageSequence=int16_t(uint16_t(defender.damageSequence)+1);half(d+0x260,defender.damageSequence);
        applyDirectDamage(ai,defender,&part,hp,fp,code,byte(w+0x6d),1,random,application());
        word(a+0x27c,attacker.flags);word(a+0x280,word(a+0x280)+1);byte(d+0x273,word(a+0x280));return hp || fp?18:19;
    }
    int32_t genericJump(EventEntry& e,bool first) {
        const int32_t selector=value(e,0),x=value(e,1),y=value(e,2),z=value(e,3),requested=value(e,4);(void)value(e,5);
        const auto id=translateBattleActor(&e,selector,nativeBattleActorServices(vm));const auto u=word(Battle+0x20+4*id);
        if(first) {
            e.initialized=1;if(requested)word(u+0x16c,requested);
            setPackedMovePosition(u,true,{float(x),float(y),float(z)},nativeBattleActorServices(vm).memory);
            setPackedMovePosition(u,false,{real(u+0x3c),real(u+0x40),real(u+0x44)},nativeBattleActorServices(vm).memory);
            real(u+0x17c,vm.platform.headingFloat(this,real(u+0x3c),real(u+0x44),float(x),float(z)));real(u+0x180,std::hypot(float(x)-real(u+0x3c),float(z)-real(u+0x44)));
            const float dx=float(x)-real(u+0x3c),dy=float(y)-real(u+0x40);if(dx!=0)byte(u+0x188,dx>0?1:255);
            if(!word(u+0x16c))word(u+0x16c,uint32_t(int32_t(real(u+0x180)/real(u+0x170))));else real(u+0x170,real(u+0x180)/float(int32_t(word(u+0x16c))));
            const float n=float(int32_t(word(u+0x16c)));real(u+0x180,-std::fma(n,real(u+0x170),-real(u+0x180)));if(!word(u+0x16c))return 2;
            real(u+0x178,std::fma(real(u+0x174)*n,.5f,dy/n));real(u+0x170,real(u+0x170)+real(u+0x180)/n);
        }
        real(u+0x158,real(u+0x158)+real(u+0x178));real(u+0x178,real(u+0x178)-real(u+0x174));if(real(u+0x178)<0 && real(u+0x158)<real(u+0x164))real(u+0x158,real(u+0x164));
        integratePackedMoveHorizontal(u+0x154,u+0x15c,real(u+0x170),real(u+0x17c),movement);
        for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,real(u+0x154+4*axis));
        word(u+0x16c,word(u+0x16c)-1);
        if(int32_t(word(u+0x16c))>0)return 0;
        for(unsigned axis=0;axis<3;++axis)real(u+0x3c+4*axis,real(u+0x160+4*axis));
        return 1;
    }
    void changePose(uint32_t u,uint32_t p,const std::string& name,bool force) {
        for(unsigned i=0;i<=name.size();++i)byte(p+0x1c4+i,i==name.size()?0:unsigned(name[i]));
        auto flags=(word(p+0x204)|3)&~uint32_t(0x80|0x200|0x10|8);if(force)flags|=8;word(p+0x204,flags);
        const unsigned id=word(u);if(scene->actors[visual[id]].pose!=name){scene->actors[visual[id]].pose=name;scene->actors[visual[id]].poseAt=now;}
    }
    TimingACommand timing() const {
        TimingACommand c;c.finishCountdown=int32_t(word(Battle+0x1f54));c.state=int32_t(word(Battle+0x1c9c));c.flags=word(Battle+0x1f50);c.classification=int32_t(word(Battle+0x1ce8));c.result=word(Battle+0x1cb8);c.count=int32_t(word(Battle+0x1cb4));
        for(unsigned i=0;i<11;++i)c.scratch[i]=word(Battle+0x1f20+4*i);
        return c;
    }
    void storeTiming(const TimingACommand& c) {
        word(Battle+0x1f54,c.finishCountdown);word(Battle+0x1c9c,c.state);word(Battle+0x1f50,c.flags);word(Battle+0x1ce8,c.classification);word(Battle+0x1cb8,c.result);word(Battle+0x1cb4,c.count);
        for(unsigned i=0;i<11;++i)word(Battle+0x1f20+4*i,c.scratch[i]);
    }
    void actionManager() {if(word(Battle+0x1ca0)==0x8012aae0){auto c=timing();const int result=c.tick();storeTiming(c);if(!result)for(unsigned o:{0x1ca0u,0x1ca4u,0x1ca8u,0x1cacu})word(Battle+o,0);}}
    void tick(uint64_t time,ProfessorStory& s,uint32_t triggers){scene=&s;now=time;history.push(triggers);if(!pending && !done){vm.gp.timestamp=time*eventClockRate(243000000);actionManager();vm.runManager();done=vm.runtime.allocationCount==0;}publish(s);}
    void publish(ProfessorStory& s){for(unsigned id=1;id<7;++id){const uint32_t u=Units+id*0x1000;for(unsigned axis=0;axis<3;++axis)s.actors[visual[id]].position[axis]=real(u+0x3c+4*axis);s.actors[visual[id]].yaw=byte(u+0x189)==1?90:270;}}
};
bool TrioAttack::begin(const TrioBattleCommands& menu,ProfessorStory& scene,int16_t hp){if(runtime_ || !assetsReady || !menu.attack || menu.attack->actor!=1 || menu.attack->weapon!=0x803538a8)return false;auto r=std::make_shared<TrioAttackRuntime>();if(!r->initialize(menu,scene,hp))return false;runtime_=std::move(r);return true;}
void TrioAttack::tick(uint64_t time,ProfessorStory& scene,uint32_t triggers){if(runtime_)runtime_->tick(time,scene,triggers);}
bool TrioAttack::active()const{return runtime_ && !runtime_->done;}
bool TrioAttack::finished()const{return runtime_ && runtime_->done;}
uint32_t TrioAttack::pendingFunction()const{return runtime_?runtime_->pending:0;}
const std::vector<ProfessorServiceRequest>& TrioAttack::requests()const{static const std::vector<ProfessorServiceRequest> empty;return runtime_?runtime_->trace:empty;}
bool TrioAttack::readWord(uint32_t a,uint32_t& v)const{return runtime_ && runtime_->vm.memory.readWord(a,v);}
}
