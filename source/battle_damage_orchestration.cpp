#include "battle_damage_orchestration.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000ll);}
}
uint32_t packedBattleCheckPikkyoro(uint32_t w,uint32_t* flags,const BattleDamageOrchestrationServices& s) {
    const auto& m=s.memory.memory;assert(m.word);
    const auto battle=m.word(m.user,0x8041eb90);
    uint32_t value=flags?*flags:0;
    if(m.word(m.user,w+0x74)&0x800) {
        const uint32_t enabled=m.word(m.user,battle+0x163f4);
        unsigned count=0,index=0;std::array<uint32_t,5> candidates{};
        for(unsigned i=0;i<5;++i)if(enabled&(0x100u<<i)){candidates[i]=1;++count;index=i;}
        if(count>=2) {
            assert(s.irand);int32_t rank=s.irand(s.user,int32_t(count));index=0;
            for(unsigned i=4;i>0;--i){rank-=int32_t(candidates[i]);if(rank<0){index=i;break;}}
        }
        if(count)value|=0x01000000u<<index;
    }
    if(flags)*flags=value;
    return value;
}
int32_t checkPackedBattleDamage(uint32_t a,uint32_t d,uint32_t p,uint32_t w,uint32_t flags,const BattleDamageOrchestrationServices& s) {
    const auto& m=s.memory.memory;assert(m.word);
    auto word=[&](uint32_t at){return m.word(m.user,at);};
    auto byte=[&](uint32_t at){assert(s.memory.byte);return s.memory.byte(s.memory.user,at);};
    auto store=[&](uint32_t at,uint32_t v){assert(m.storeWord);m.storeWord(m.user,at,v);};
    const uint32_t battle=word(0x8041eb90);BattleCounterWork counter;counter.init();
    int32_t hp=0,result=-1;uint32_t statuses=0;bool reaction=false;
    if(!d)return 18;
    if(p)store(d+0x258,p);
    auto sequence=[&](){store(a+0x280,word(a+0x280)+1);assert(m.storeByte);m.storeByte(m.user,d+0x273,uint8_t(word(a+0x280)));};
    auto nice=[&](int32_t mode){
        assert(s.hitPosition && s.niceEntry);auto pos=s.hitPosition(s.user,d,p);
        pos.x-=60;pos.y+=45;pos.z-=10;
        const auto effect=s.niceEntry(s.user,mode,pos);store(word(effect+0xc)+0x1c,0x3f400000); // Source0.75 payload.
    };
    auto audience=[&](DamageAudienceRequest request,int32_t amount=0){assert(s.audience);s.audience(s.user,request,amount);};
    auto direct=[&](int32_t id,uint32_t u,uint32_t part,int32_t h,int32_t f,uint32_t code,int32_t pattern,int32_t recovery){assert(s.directDamage);s.directDamage(s.user,id,u,part,h,f,code,pattern,recovery);};
    auto event=[&](uint32_t u,uint32_t code){assert(s.hitEvent);s.hitEvent(s.user,u,code);};
    auto special=[&](uint32_t code){assert(s.emergencyRevival);s.emergencyRevival(s.user,d,code);event(d,code);};
    auto possible=[&](){assert(s.acPossible);return s.acPossible(s.user,d);};
    auto defense=[&](){assert(s.defenseResult);return s.defenseResult(s.user);};
    if(!(word(w+0x74)&0x20000) && possible() && defense()==5) {
        nice(2);flags|=0x80000;assert(s.systemSound);s.systemSound(s.user,0x802ee064);audience(DamageAudienceRequest::GuardGood);
        sequence();const uint8_t mode=byte(w+0x13);
        if(mode!=2)flags|=0x100;
        direct(signedWord(word(a)),d,p,0,0,flags|42,byte(w+0x6d),0);
        if(mode==2 || mode==3)return 21;
        if(mode==4 && word(a+0x2d0)){assert(s.hitEventDirect);s.hitEventDirect(s.user,a,311,word(a+0x2d0));}
        else direct(-5,a,0,1,0,311,0,1);
        return -1;
    }
    BattlePackedCounterServices cs;cs.memory=s.memory;cs.user=s.user;cs.status=s.status;
    (void)packedBattleCheckCounter(counter,a,d,p,w,flags,cs);
    if(counter.words[0]) {
        bool eventExit=false;
        if(flags&0x100) {
            if(word(p+0x1b0)&0x2000){event(d,1817);result=18;eventExit=true;}
            else if(word(p+0x1b0)&0x1000) {
                const auto element=byte(w+0x6c);assert(s.status);
                if(element==3 || element==4 || element==1 || ((word(w+0x74)&0x400) && s.status(s.user,a,8))) {
                    event(d,1817);result=18;eventExit=true;
                }
            }
        }
        if(!eventExit) {
            if(!(word(w+0x74)&0x20000) && possible()) {
                if(defense()>3){nice(0);flags|=0x40000;assert(s.systemSound);s.systemSound(s.user,0x802ee078);audience(DamageAudienceRequest::GuardGood);}
                else audience(DamageAudienceRequest::GuardBad);
            }
            for(uint32_t mask:{0x1000u,0x2000u})if((word(w+0x74)&mask) && (word(p+0x1ac)&mask)){reaction=true;flags|=0x200000;}
            if((word(w+0x74)&0x4000) && (word(p+0x1ac)&0x800)){reaction=true;if(flags&0x100)flags|=0x400000;}
            (void)packedBattleCheckPikkyoro(w,&flags,s);
            uint32_t code=0;assert(s.hpDamage && s.fpDamage);
            hp=s.hpDamage(s.user,a,d,p,w,code,flags);
            const int32_t fp=s.fpDamage(s.user,a,d,p,w,code,flags);
            assert(m.half && m.storeHalf);m.storeHalf(m.user,d+0x260,uint16_t(m.half(m.user,d+0x260)+1));
            if(!(flags&0x40000) && (flags&(0x40000000|0x100))){assert(s.weaponStatus);statuses=s.weaponStatus(s.user,a,d,p,w,flags);}
            if((code&255)==30 && reaction)code=flags|23;
            if((code&255)==30 && (byte(w+0x6d)==19 || byte(w+0x6d)==27))code=flags|23;
            if((code&255)==30 && statuses)code=flags|(statuses&1?23:22);
            if(statuses&4)code=flags|28;
            if(statuses&8)code=flags|29;
            if((flags&0x100) && (statuses&0x10)){store(d+0x274,0);special(0x717);result=18;eventExit=true;}
            else if((flags&0x100) && (word(w+0x74)&0x80)) {
                assert(s.status);if(s.status(s.user,d,9)){assert(s.irand);if(s.irand(s.user,100)<0){special(0x8717);result=18;eventExit=true;}}
            }
            if(!eventExit) {
                const int32_t kind=signedWord(word(d+8));
                if(!statuses && !hp && !fp && !reaction){result=19;if(kind>=1 && kind<=215 && byte(w+0x6d)!=27)audience(DamageAudienceRequest::EnemyNoDamage);}
                else {result=18;if(kind>=1 && kind<=215 && (hp>0 || statuses))audience(DamageAudienceRequest::EnemyDamage,hp);}
                int32_t pattern=byte(w+0x6d);
                if(pattern==20 && counter.words[9])pattern=0;
                if(pattern==14 || pattern==16) {
                    if(word(d+0x104)&0x4000)pattern=0;
                    assert(s.irand);const auto draw=s.irand(s.user,100);
                    if(draw>=byte(word(d+0x10)+0x8c))pattern=0;
                    if(!(flags&0x20000))pattern=0;
                }
                direct(signedWord(word(a)),d,p,hp,fp,code,pattern,statuses&0x20?0:1);
            }
        }
    }
    sequence();auto& k=counter.words;if(!k[9])return result;
    for(unsigned offset:{1u,6u,7u})if(k[offset])k[10]+=uint32_t(signedWord(uint32_t(hp)*50u)/100);
    const unsigned offsets[]={3,5,2,4};const int32_t types[]={4,9,8,6};
    for(unsigned i=0;i<4;++i)if(k[offsets[i]]){assert(s.counterStatus);uint32_t local=0;s.counterStatus(s.user,local,a,p,0,types[i],100,0,3,1);}
    if(!k[10])k[10]=1;
    if(byte(a+0xc)==0)audience(DamageAudienceRequest::Countered);
    if(k[6]) {
        assert(s.hitPosition && s.bombEntry && s.spatialSound && s.fogStop);
        const auto pos=s.hitPosition(s.user,d,p);s.bombEntry(s.user,0,pos,1);s.spatialSound(s.user,0x802ee08c,pos);s.fogStop(s.user);
    }
    assert(s.stopAction && s.helpType);s.stopAction(s.user,battle);s.helpType(s.user,0);
    direct(-5,a,0,signedWord(k[10]),0,k[9],0,1);
    if(k[11]){store(d+0x274,0);event(d,k[11]);}
    return -1;
}
}
