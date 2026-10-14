#include "battle_audience_item_control.hpp"
#include "battle_audience_population.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
uint32_t record(int32_t slot,const BattleActorServices& a){return base(a)+0x1bc+uint32_t(slot)*0x134;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float scalar(uint32_t p,const BattleActorServices& a){auto v=a.memory.word(a.memory.user,p);float f;std::memcpy(&f,&v,4);return f;}
void real(uint32_t p,float f,const BattleActorServices& a){uint32_t v;std::memcpy(&v,&f,4);a.memory.storeWord(a.memory.user,p,v);}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float divide(float a,float b){volatile float v=a/b;return v;}
bool exists(int32_t slot,const BattleActorServices& a){(void)base(a);const auto r=record(slot,a),flags=a.memory.word(a.memory.user,r);return (flags&1) && !(flags&0x102);}
bool systemControl(int32_t slot,const BattleActorServices& a){
    (void)base(a);const auto r=record(slot,a);
    return exists(slot,a) && !(a.memory.word(a.memory.user,r)&0x10);
}
bool penalty(const BattleAudienceItemControlServices& s){
    const auto& a=s.actors;const auto& m=a.memory;
    auto item=packedBattleAudienceItemAddress(0,a);
    if(integer(m.word(m.user,item+0x10))>=236){
        item=packedBattleAudienceItemAddress(0,a);
        if(integer(m.word(m.user,item+0x10))<240)return true;
    }
    const auto fresh=base(a);const auto coefficient=scalar(0x80424570,a),baseline=scalar(fresh+0x13778,a);
    const auto bias=scalar(0x804244e8,a),delta=mul(coefficient,baseline);
    return s.addTarget && s.addTarget(s.user,delta,bias);
}
}
uint32_t packedBattleAudienceItemAddress(int32_t index,const BattleActorServices& a){return base(a)+0xf25c+uint32_t(index)*0x48;}
bool packedBattleAudienceEscapeAllowed(int32_t slot,const BattleActorServices& a){
    const auto owner=base(a),r=record(slot,a);const auto& m=a.memory;
    if(m.word(m.user,owner)&0x4000)return false;
    if(!systemControl(slot,a))return false;
    if(m.word(m.user,r)&0x200)return false;
    switch(a.byte(a.user,r+0x19)){
    case 12:case 13:case 14:case 15:case 17:case 18:case 21:case 22:return false;
    default:return true;
    }
}
bool controlPackedBattleAudienceItemState(uint32_t fn,int32_t slot,int32_t front,const BattleAudienceItemControlServices& s){
    if(fn!=0x80191a48 && fn!=0x80191980 && fn!=0x8019173c && fn!=0x80191290)return false;
    const auto& a=s.actors;const auto& m=a.memory;
    if(fn==0x80191a48){
        const auto battle=currentBattleAddress(a),owner=base(a),r=record(slot,a);
        packedBattleAudienceSetAnimation(slot,uint8_t(front)==1?0:5,0,a);
        const auto flags=m.word(m.user,r);
        if(flags&0x40){
            const auto item=packedBattleAudienceItemAddress(int16_t(m.half(m.user,r+0x1c)),a);
            m.storeWord(m.user,r,m.word(m.user,r)&~0x60u);m.storeHalf(m.user,r+0x1c,0xffff);m.storeWord(m.user,item,0);
            packedBattleAudienceChangeStatus(slot,10,a);return penalty(s);
        }
        if(flags&0x20)return true;
        if(!(m.word(m.user,battle+0xef4)&0x80))return true;
        const auto item=packedBattleAudienceItemAddress(int16_t(m.half(m.user,r+0x1c)),a);
        if(m.word(m.user,owner)&0x40000)return true;
        m.storeWord(m.user,item+8,m.word(m.user,item+8)-1);
        if(!(m.word(m.user,item)&1))return true;
        if(integer(m.word(m.user,item+8))>0 || integer(m.word(m.user,item+4))!=1)return true;
        packedBattleAudienceChangeStatus(slot,8,a);
        return s.throwSound && s.throwSound(s.user,0,slot);
    }
    if(fn==0x80191980){
        const auto r=record(slot,a);packedBattleAudienceSetAnimation(slot,6,0,a);
        if(packedBattleAudienceAnimationEnded(slot,a)==1)packedBattleAudienceChangeStatus(slot,0,a);
        const auto flags=m.word(m.user,r);
        if(!(flags&0x40))return true; // Source's final flag20 test has no stores/calls.
        m.storeWord(m.user,r,flags&~0x60u);packedBattleAudienceChangeStatus(slot,10,a);
        return penalty(s);
    }
    (void)base(a);const auto r=record(slot,a);
    if(fn==0x8019173c){
        const auto phase=integer(m.word(m.user,r+4));if(phase!=0 && phase!=1)return true;
        if(phase==0){
            packedBattleAudienceSetAnimation(slot,9,0,a);
            m.storeWord(m.user,r+0xdc,m.word(m.user,0x80424564));m.storeWord(m.user,r+0xe4,m.word(m.user,0x804244fc));
            m.storeWord(m.user,r+0x30,0);m.storeWord(m.user,r+0xe8,m.word(m.user,0x804244e0));
            m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        }
        if(integer(m.word(m.user,r+0x30))>=8){m.storeWord(m.user,r+0x64,m.word(m.user,0x804244f8));m.storeWord(m.user,r+0x30,0);}
        const auto scale=scalar(r+0x64,a),step=scalar(0x80424504,a);
        real(r+0x64,integer(m.word(m.user,r+0x30))<=4?sub(scale,step):add(scale,step),a);
        m.storeWord(m.user,r+0x30,m.word(m.user,r+0x30)+1);
        if(packedBattleAudienceAnimationEnded(slot,a)!=1)return true;
        m.storeWord(m.user,r+0x64,m.word(m.user,0x804244f8));m.storeWord(m.user,r+0x30,0);
        if(!packedBattleAudienceEscapeAllowed(slot,a)){packedBattleAudienceChangeStatus(slot,0,a);return true;}
        packedBattleAudienceChangeStatus(slot,12,a);
        return s.soundAt && s.soundAt(s.user,0x802f9698,r+0x48);
    }
    packedBattleAudienceSetAnimation(slot,7,0,a);
    const auto phase=integer(m.word(m.user,r+4));
    if(a.byte(a.user,r+0x1b)==11){
        if(phase==0){
            m.storeWord(m.user,r+8,m.word(m.user,r+8)-1);if(integer(m.word(m.user,r+8))>0)return true;
            m.storeWord(m.user,r+0x78,m.word(m.user,0x804244e8));m.storeWord(m.user,r+0x7c,m.word(m.user,0x804244fc));
            m.storeWord(m.user,r+0x80,m.word(m.user,0x804244e8));m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        }else if(phase==1 && scalar(r+0x4c,a)<=scalar(0x80424500,a)){
            packedBattleAudienceDelete(slot,a);m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        }
        return true;
    }
    if(phase==0){
        m.storeWord(m.user,r+8,m.word(m.user,r+8)-1);if(integer(m.word(m.user,r+8))>0)return true;
        if(!s.randomRaw)return false;
        const auto random=s.randomRaw(s.user);if(!random)return false;
        const auto scaled=mul(scalar(0x8042455c,a),float(*random));
        const auto x=scalar(r+0x48,a),divisor=scalar(0x80424560,a),one=scalar(0x804244f8,a);
        const auto xPart=divide(x,divisor),fraction=divide(scaled,scalar(0x80424554,a));
        real(r+0x78,sub(sub(fraction,one),xPart),a);
        m.storeWord(m.user,r+0x7c,m.word(m.user,0x804244e8));m.storeWord(m.user,r+0x80,m.word(m.user,0x80424564));
        m.storeWord(m.user,r+0xdc,m.word(m.user,0x804244ec));m.storeWord(m.user,r+0xe4,m.word(m.user,0x80424558));
        m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);
        return s.soundAt && s.soundAt(s.user,0x802f9684,r+0x48);
    }
    if(phase==1){if(scalar(r+0xe4,a)==scalar(0x804244e8,a))m.storeWord(m.user,r+4,m.word(m.user,r+4)+1);}
    else if(phase==2 && scalar(r+0x50,a)>=scalar(0x80424568,a))packedBattleAudienceDelete(slot,a);
    return true;
}
}
