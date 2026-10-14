#include "battle_audience_actor_control.hpp"
#include "battle_audience_population.hpp"
#include <array>
#include <cstring>
namespace ttyd {
namespace {
uint32_t record(int32_t slot,const BattleActorServices& a){return currentBattleAddress(a)+0x2858+0x1bc+uint32_t(slot)*0x134;}
int32_t integer(uint32_t value){int32_t n;std::memcpy(&n,&value,4);return n;}
float scalar(uint32_t address,const BattleActorServices& a){auto n=a.memory.word(a.memory.user,address);float f;std::memcpy(&f,&n,4);return f;}
void real(uint32_t address,float f,const BattleActorServices& a){uint32_t n;std::memcpy(&n,&f,4);a.memory.storeWord(a.memory.user,address,n);}
// Force the separate source single-precision instructions, including on hosts
// permitting excess precision. There are no fused operations in these bodies.
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float divide(float a,float b){volatile float v=a/b;return v;}
}
bool controlPackedBattleAudienceIdleReaction(uint32_t fn,int32_t slot,int32_t front,const BattleActorServices& a){
    if(fn!=0x80192164 && fn!=0x80192054 && fn!=0x80191f20)return false;
    const auto r=record(slot,a);const auto& m=a.memory;
    const int32_t animation=(uint8_t(front)==1?0:1)+(fn==0x80191f20?2:0);
    packedBattleAudienceSetAnimation(slot,animation,0,a);
    if(fn==0x80191f20){
        const auto phase=integer(m.word(m.user,r+4));
        if(phase<0 || phase>2)return true;
        if(phase==0){m.storeWord(m.user,r+8,a.byte(a.user,r+0x1b)==7?3:1);m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);}
        if(phase<=1){
            const bool type7=a.byte(a.user,r+0x1b)==7;
            m.storeWord(m.user,r+0xdc,m.word(m.user,type7?0x80424510:0x8042457c));
            m.storeWord(m.user,r+0xe4,m.word(m.user,type7?0x804244fc:0x80424578));
            m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        }
        if(scalar(r+0xe4,a)==scalar(0x804244e8,a)){
            m.storeWord(m.user,r+8,m.word(m.user,r+8)-1);
            if(integer(m.word(m.user,r+8))<=0)packedBattleAudienceChangeStatus(slot,0,a);
            else m.storeWord(m.user,r+4,1);
        }
        return true;
    }
    const auto timer=integer(m.word(m.user,r+0x30));
    if(fn==0x80192164){
        const auto step=scalar(0x80424584,a);
        if(timer<=15)real(r+0xa0,mul(step,float(timer)),a);
        if(timer>=30)real(r+0xa0,mul(step,float(integer(45u-uint32_t(timer)))),a);
        if(timer>45)packedBattleAudienceChangeStatus(slot,0,a);
        else m.storeWord(m.user,r+0x30,uint32_t(timer)+1);
    }else{
        const auto step=scalar(0x80424580,a);
        if(timer>=0 && timer<=10)real(r+0x6c,mul(step,float(timer)),a);
        if(timer>=20 && timer<=40)real(r+0x6c,sub(scalar(r+0x6c,a),step),a);
        if(timer>=50 && timer<=60)real(r+0x6c,add(scalar(r+0x6c,a),step),a);
        if(timer>60)packedBattleAudienceChangeStatus(slot,0,a);
        else m.storeWord(m.user,r+0x30,uint32_t(timer)+1);
    }
    return true;
}
bool controlPackedBattleAudienceReaction(uint32_t fn,int32_t slot,int32_t front,const BattleAudienceReactionServices& s){
    if(fn!=0x80191e18 && fn!=0x80191cec && fn!=0x80191bb0 && fn!=0x80191884)return false;
    const auto& a=s.actors;const auto& m=a.memory;const auto r=record(slot,a);
    if(fn==0x80191e18 || fn==0x80191884){
        if(fn==0x80191884 || uint8_t(front)==0)packedBattleAudienceSetAnimation(slot,4,0,a);
        const auto phase=integer(m.word(m.user,r+4));
        if(phase!=0 && phase!=1)return true;
        if(phase==0){
            if(!s.random)return false;
            const auto random=s.random(s.user,2);if(!random)return false;
            m.storeWord(m.user,r+0xe8,m.word(m.user,*random==0?0x804244e0:0x804244e4));
            m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        }
        const auto start=scalar(0x804244e8,a),end=scalar(0x80424524,a);
        const auto step=integer(m.word(m.user,r+8));
        if(!s.interpolate)return false;
        const auto value=s.interpolate(s.user,4,step,15,start,end);if(!value)return false;
        real(r+0x70,*value,a);m.storeWord(m.user,r+8,m.word(m.user,r+8)+1);
        if(integer(m.word(m.user,r+8))>30){
            real(r+0xc,sub(scalar(r+0xc,a),scalar(0x804244f8,a)),a);
            if(scalar(r+0xc,a)<=scalar(0x804244e8,a))packedBattleAudienceChangeStatus(slot,0,a);
            else m.storeWord(m.user,r+8,0);
        }
        return true;
    }
    packedBattleAudienceSetAnimation(slot,uint8_t(front)==1?2:3,0,a);
    const auto phase=integer(m.word(m.user,r+4));
    if(phase<0 || phase>2)return true;
    if(phase==0){
        m.storeWord(m.user,r+8,fn==0x80191cec?1:2);
        m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
    }
    if(phase<=1){
        if(!s.randomRaw)return false;
        const auto random=s.randomRaw(s.user);if(!random)return false;
        const auto fraction=divide(float(*random),scalar(0x80424554,a));
        const auto start=scalar(fn==0x80191cec?0x8042457c:0x80424574,a);
        const auto acceleration=m.word(m.user,0x80424578);
        real(r+0xdc,add(start,fraction),a);m.storeWord(m.user,r+0xe4,acceleration);
        m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
    }
    if(fn==0x80191bb0)real(r+0xa0,add(scalar(r+0xa0,a),scalar(0x8042453c,a)),a);
    if(scalar(r+0xe4,a)==scalar(0x804244e8,a)){
        m.storeWord(m.user,r+8,m.word(m.user,r+8)-1);
        if(integer(m.word(m.user,r+8))<=0)packedBattleAudienceChangeStatus(slot,0,a);
        else m.storeWord(m.user,r+4,1);
    }
    return true;
}
bool controlPackedBattleAudienceActor(int32_t slot,const BattleAudienceActorControlServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto r=record(slot,a);
    if(!s.front)return false;
    const auto front=s.front(s.user,slot);
    if(!front)return false;
    constexpr std::array<uint32_t,21> functions{{0x80192260,0x80192164,0x80192054,0x80191f20,
        0x80191e18,0x80191cec,0x80191bb0,0x80191a48,0x80191980,0x80191884,0x8019173c,
        0x80191470,0x80191290,0x80191058,0x80190d68,0x801906bc,0x80190444,0x8019024c,
        0x8019014c,0x8018ff4c,0x8018f9a8}};
    const auto state=a.byte(a.user,r+0x19);
    if(state<functions.size() && (!s.state || !s.state(s.user,functions[state],slot,*front)))return false;
    for(unsigned axis=0;axis<3;++axis){
        real(r+0x48+4*axis,add(scalar(r+0x48+4*axis,a),scalar(r+0x78+4*axis,a)),a);
        real(r+0x78+4*axis,add(scalar(r+0x78+4*axis,a),scalar(r+0x84+4*axis,a)),a);
    }
    const auto zero=scalar(0x804244e8,a),vx=scalar(r+0x78,a);
    if(vx>zero)m.storeWord(m.user,r+0xe8,m.word(m.user,0x804244e4));
    else if(vx<zero)m.storeWord(m.user,r+0xe8,m.word(m.user,0x804244e0));
    if(scalar(r+0xd0,a)<zero)for(auto offset:{0xd0u,0xdcu,0xe4u})real(r+offset,zero,a);
    if(a.byte(a.user,r+0x1b)==11){for(auto offset:{0xdcu,0xd0u,0xe4u})real(r+offset,zero,a);}
    else{
        real(r+0xd0,add(scalar(r+0xd0,a),scalar(r+0xdc,a)),a);
        real(r+0xdc,add(scalar(r+0xdc,a),scalar(r+0xe4,a)),a);
    }
    const bool hover=a.byte(a.user,r+0x1b)==2;
    if(hover){
        real(r+0x12c,add(scalar(r+0x12c,a),scalar(0x80424508,a)),a);
        const auto period=scalar(0x804245d0,a);
        if(scalar(r+0x12c,a)>=period)real(r+0x12c,sub(scalar(r+0x12c,a),period),a);
        if(!s.sine)return false;
        const auto sine=s.sine(s.user,double(scalar(r+0x12c,a)));
        if(!sine)return false;
        real(r+0x130,mul(scalar(0x80424510,a),float(*sine)),a);
    }
    real(r+0xc0,add(scalar(r+0x48,a),scalar(r+0xcc,a)),a);
    auto y=add(scalar(r+0x4c,a),scalar(r+0xd0,a));if(hover)y=add(scalar(r+0x130,a),y);
    real(r+0xc4,y,a);real(r+0xc8,add(scalar(r+0x50,a),scalar(r+0xd4,a)),a);
    return true;
}
}
