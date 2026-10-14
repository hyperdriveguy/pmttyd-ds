#include "battle_audience_idle.hpp"
#include "battle_audience_population.hpp"
#include <array>
#include <cstring>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
uint32_t record(int32_t slot,const BattleActorServices& a){return base(a)+0x1bc+uint32_t(slot)*0x134;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float scalar(uint32_t p,const BattleActorServices& a){auto v=a.memory.word(a.memory.user,p);float f;std::memcpy(&f,&v,4);return f;}
void real(uint32_t p,float f,const BattleActorServices& a){uint32_t v;std::memcpy(&v,&f,4);a.memory.storeWord(a.memory.user,p,v);}
float add(float a,float b){volatile float v=a+b;return v;}
float divide(float a,float b){volatile float v=a/b;return v;}
float constant(int32_t offset,const BattleActorServices& a){return scalar(0x80427440u+uint32_t(offset),a);}
bool exists(int32_t slot,const BattleActorServices& a){const auto flags=a.memory.word(a.memory.user,record(slot,a));return (flags&1) && !(flags&0x102);}
}
int32_t packedBattleAudienceOffsetSeat(int32_t slot,int32_t x,int32_t row,const BattleActorServices& a){
    (void)base(a);(void)record(slot,a);
    const auto column=integer(uint32_t(slot%20)+uint32_t(x));
    if(column<0 || column>=20)return -1;
    const auto result=integer(uint32_t(slot)-uint32_t(row)*20u+uint32_t(x));
    return result>=0 && result<200?result:-1;
}
int32_t packedBattleAudienceBreakTurn(const BattleActorServices& a){return integer(a.memory.word(a.memory.user,currentBattleAddress(a)+0x1617c));}
bool controlPackedBattleAudienceIdle(int32_t slot,int32_t front,const BattleAudienceIdleServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto owner=base(a),r=record(slot,a);
    (void)m.word(m.user,0x8041eb00); // Whole pouchGetPtr800d5eb0, return unused.
    packedBattleAudienceSetAnimation(slot,uint8_t(front)==1?0:1,0,a);
    const auto c=[&](int32_t offset){return constant(offset,a);};
    const auto ratio=divide(add(scalar(owner+0x1377c,a),scalar(owner+0x13780,a)),scalar(owner+0x13778,a));
    // FPR0 is intentionally distinct from FPR29 and feeds the breathing FMA.
    float f0=c(-11960);std::optional<double> f29=s.callerF29;
    if(ratio>=f0){f0=c(-12104);f29=f0;}
    else{f0=c(-11956);if(ratio>=f0)f29=c(-12004);
        else{f0=c(-11952);if(ratio>=f0)f29=c(-11980);else f0=c(-12120);}}
    float f31,f30;
    if(ratio>=c(-11948)){f31=c(-12060);f30=c(-11980);}
    else if(ratio>=c(-11968)){f31=c(-12080);f30=c(-12004);}
    else if(ratio>=c(-11944)){f31=c(-12004);f30=c(-12104);}
    else if(ratio>=c(-12104)){f31=c(-12104);f30=c(-12120);}
    else{f31=c(-12120);f30=f31;}
    const auto multiply=[&](double x,double y)->std::optional<float>{return s.multiplySingle?s.multiplySingle(s.user,x,y):std::nullopt;};
    const auto fused=[&](double x,double y,double z)->std::optional<float>{return s.fusedSingle?s.fusedSingle(s.user,x,y,z):std::nullopt;};
    const auto changeScale=[&](int32_t coefficient,bool positive){
        // fnmsubs = -(coefficient * FPR0 - scale). Preserve the final negate,
        // including signed zero, rather than replacing it by an unfused sum.
        const auto result=fused(c(coefficient),f0,positive?scalar(r+0x64,a):-scalar(r+0x64,a));
        if(!result)return false;
        real(r+0x64,positive?*result:-*result,a);return true;
    };
    const auto type=a.byte(a.user,r+0x1b);
    if(type==4){
        const auto timer=integer(m.word(m.user,r+0x30));
        if(timer>180){
            if(timer<=188){if(!changeScale(-11940,true))return false;}
            else{
                if(!f29)return false;
                bool selected=false;
                constexpr std::array<int32_t,5> sourceLimits{{-11996,-11932,-11928,-11924,-12032}};
                for(unsigned i=0;i<sourceLimits.size();++i){
                    const auto limit=fused(c(sourceLimits[i]),*f29,c(-11936));if(!limit)return false;
                    if(float(timer)<=*limit){if(!changeScale(-11940,(i&1)!=0))return false;selected=true;break;}
                }
                if(!selected){real(r+0x64,c(-12104),a);m.storeWord(m.user,r+0x30,0);}
            }
        }
        m.storeWord(m.user,r+0x30,m.word(m.user,r+0x30)+1);
    }else if(type!=2){
        if(!f29)return false;
        int32_t reset=-11924,half=-12024,coefficient=-12020;
        if(type==6){reset=-11920;half=-11916;coefficient=-11912;}
        else if(type==7){reset=-11908;half=-12060;coefficient=-11904;}
        else if(type==9){reset=-11900;half=-11896;coefficient=-11892;}
        const auto beforeReset=integer(m.word(m.user,r+0x30));
        const auto end=multiply(c(reset),*f29);if(!end)return false;
        if(float(beforeReset)>=*end){real(r+0x64,c(-12104),a);m.storeWord(m.user,r+0x30,0);}
        const auto afterReset=integer(m.word(m.user,r+0x30));
        const auto midpoint=multiply(c(half),*f29);if(!midpoint)return false;
        if(!changeScale(coefficient,float(afterReset)<=*midpoint))return false;
        m.storeWord(m.user,r+0x30,m.word(m.user,r+0x30)+1);
    }
    const auto flags=m.word(m.user,r);
    if(flags&0x40){
        m.storeWord(m.user,r,flags&~0x60u);packedBattleAudienceChangeStatus(slot,10,a);
        auto item=base(a)+0xf25c; // BattleAudienceItemGetPtr(0), not a battle unit.
        if(integer(m.word(m.user,item+0x10))>=236){
            item=base(a)+0xf25c;
            if(integer(m.word(m.user,item+0x10))<240)return true;
        }
        const auto fresh=base(a);const auto delta=multiply(c(-11984),scalar(fresh+0x13778,a));if(!delta)return false;
        return s.addTarget && s.addTarget(s.user,*delta,c(-12120));
    }
    if(flags&0x20)return true;
    if(int16_t(m.half(m.user,r+0x1c))!=-1)return true;
    if(!s.random)return false;
    const auto random=s.random(s.user,360000);if(!random)return false;
    if(*random<10){packedBattleAudienceChangeStatus(slot,1,a);return true;}
    if(*random<20){packedBattleAudienceChangeStatus(slot,2,a);return true;}
    const auto bonus=multiply(c(-12060),f31);if(!bonus || !s.truncate)return false;
    const auto threshold=s.truncate(s.user,add(20.f,*bonus));if(!threshold)return false;
    if(*random<*threshold){packedBattleAudienceChangeStatus(slot,4,a);return true;}
    if(packedBattleAudienceBreakTurn(a)>=1 || integer(m.word(m.user,owner+0x13910))==1){
        if(*random<integer(uint32_t(*threshold)+300u) && a.byte(a.user,r+0x1b)!=11)packedBattleAudienceChangeStatus(slot,3,a);
        return true;
    }
    const auto extra=multiply(c(-12060),f30);if(!extra)return false;
    const auto count=s.truncate(s.user,*extra);if(!count)return false;
    if(*random>=integer(uint32_t(*threshold)+uint32_t(*count)))return true;
    const auto neighbour=packedBattleAudienceOffsetSeat(slot,0,1,a);if(neighbour==-1)return true;
    (void)record(neighbour,a);
    if(a.byte(a.user,r+0x1b)!=11 && exists(neighbour,a))packedBattleAudienceChangeStatus(slot,3,a);
    return true;
}
}
