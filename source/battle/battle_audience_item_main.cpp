#include "battle_audience_item_main.hpp"
#include "battle_audience_item_control.hpp"
#include "battle_audience_population.hpp"
#include "battle_command_pose.hpp"
#include "battle_packed_part.hpp"
#include "battle_packed_status.hpp"
#include "key_item_inventory.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
uint32_t actor(int32_t slot,const BattleActorServices& a){return base(a)+0x1bc+uint32_t(slot)*0x134;}
int32_t integer(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float real(uint32_t p,const BattleActorServices& a){auto v=a.memory.word(a.memory.user,p);float f;std::memcpy(&f,&v,4);return f;}
void real(uint32_t p,float f,const BattleActorServices& a){uint32_t v;std::memcpy(&v,&f,4);a.memory.storeWord(a.memory.user,p,v);}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float divide(float a,float b){volatile float v=a/b;return v;}
float c(int32_t offset,const BattleActorServices& a){return real(0x80427440u+uint32_t(offset),a);}
bool exists(int32_t slot,const BattleActorServices& a){(void)base(a);const auto r=actor(slot,a),flags=a.memory.word(a.memory.user,r);return (flags&1) && !(flags&0x102);}
bool shadowGuard(uint32_t u,const BattleActorServices& a){const auto& m=a.memory;
    if(m.word(m.user,u+0x104)&0x10)return true;
    return integer(m.word(m.user,u+8))==228 && integer(m.word(m.user,u+0x228))>0;
}
}
std::optional<int32_t> createPackedBattleAudienceItem(int32_t slot,int32_t kind,int32_t limit,const BattleAudienceItemEntryServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto owner=base(a),r=actor(slot,a);
    if(m.word(m.user,owner)&0x40000)return 0;
    const auto table=m.word(m.user,0x803b4decu+uint32_t(a.byte(a.user,r+0x1b))*32);
    if(integer(m.word(m.user,table))==0)return 0;
    const auto id=uint16_t(kind);
    if(id>=121 && id<236){
        if(!s.switchValue)return {};
        const auto flag=s.switchValue(s.user,235);if(!flag)return {};
        if(*flag==0)return 0;
    }
    const auto column=slot%20;if(column<4 || column>15)return 0;
    if(!exists(slot,a))return 0;
    switch(a.byte(a.user,r+0x19)){
    case 7:case 8:case 13:case 12:case 11:case 18:case 17:case 21:case 10:return 0;
    default:break;
    }
    if(!(m.word(m.user,r)&8))return 0;
    auto item=packedBattleAudienceItemAddress(0,a);
    for(int32_t i=0;i<limit;++i,item+=0x48){
        const auto flags=m.word(m.user,item);if(flags&1)continue;
        m.storeWord(m.user,item,flags|1);m.storeWord(m.user,item+4,0);
        m.storeWord(m.user,item+0xc,uint32_t(slot));m.storeWord(m.user,item+0x10,id);
        packedBattleAudienceChangeStatus(slot,7,a);m.storeHalf(m.user,r+0x1c,uint16_t(i));return 1;
    }
    return 0;
}
std::optional<uint32_t> detectPackedBattleAudiencePlayerTarget(const BattleAudienceItemMainServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto battle=currentBattleAddress(a);(void)base(a);
    std::array<int32_t,64> candidates{};uint32_t count=0;
    for(int32_t id=0;id<64;++id){
        const auto unit=m.word(m.user,battle+0x20+uint32_t(id)*4);if(!unit)continue;
        const auto body=packedBattleBodyPartId(unit,m);const auto part=packedRegistryPart(id,body,a);
        if(int8_t(a.byte(a.user,unit+0xc))!=0 || int16_t(m.half(m.user,unit+0x10c))<=0)continue;
        if(!part)return {}; // Original subsequent null dereference is outside the mapped domain.
        if(!(m.word(m.user,part+0x1ac)&0x10000))candidates[count++]=id;
    }
    if(count==0)return 0;
    if(!s.random)return {};
    const auto draw=s.random(s.user,int32_t(count));if(!draw)return {};
    if(*draw<0 || uint32_t(*draw)>=count)return {};
    return battleRegistryUnit(battle,candidates[*draw],a);
}
int32_t packedBattleAudienceEmptyItemSlots(const BattleActorServices& a){
    const auto& m=a.memory;const auto p=m.word(m.user,0x8041eb00);
    const auto count=packedKeyItemCount(p,1,m)?20u:10u;
    const auto current=m.word(m.user,0x8041eb00);int32_t empty=0;
    for(unsigned i=0;i<count;++i)if(int16_t(m.half(m.user,current+0x192+2*i))==0)++empty;
    return empty;
}
bool processPackedBattleAudienceItems(const BattleAudienceItemMainServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto battle=currentBattleAddress(a),owner=base(a);
    auto item=packedBattleAudienceItemAddress(0,a);
    const auto clear=[&](uint32_t item){
        m.storeWord(m.user,item,0);m.storeWord(m.user,item+8,0);m.storeWord(m.user,item+4,0);
        const auto slot=integer(m.word(m.user,item+0xc));if(slot!=-1)m.storeHalf(m.user,actor(slot,a)+0x1c,0xffff);
    };
    const auto stop=[&](){return s.stopSound && s.stopSound(s.user,10) && s.stopSound(s.user,11);};
    const auto cleanup=[&](uint32_t item){
        if(!stop())return false;
        const auto slot=integer(m.word(m.user,item+0xc));
        if(slot!=-1){(void)actor(slot,a);packedBattleAudienceChangeStatus(integer(m.word(m.user,item+0xc)),0,a);}
        clear(item);return true;
    };
    const auto integrate=[&](uint32_t item){
        for(unsigned axis=0;axis<3;++axis)real(item+0x14+4*axis,add(real(item+0x14+4*axis,a),real(item+0x28+4*axis,a)),a);
        for(unsigned axis=0;axis<3;++axis)real(item+0x28+4*axis,add(real(item+0x28+4*axis,a),real(item+0x34+4*axis,a)),a);
    };
    for(int32_t index=0;index<100;++index,item+=0x48){
        if(!(m.word(m.user,item)&1))continue;
        const auto r=actor(integer(m.word(m.user,item+0xc)),a);
        const auto state=a.byte(a.user,r+0x19),type=a.byte(a.user,r+0x1b);
        const auto species=0x803b4de4u+uint32_t(type)*32;
        if(state==13 || state==19 || state==21 || state==12 || !exists(integer(m.word(m.user,item+0xc)),a)){clear(item);continue;}
        switch(m.word(m.user,item+4)){
        case 0:{
            m.storeWord(m.user,item,m.word(m.user,item)|2);
            if(integer(m.word(m.user,item+0x10))==0){
                const auto table=m.word(m.user,species+8);uint32_t count=0,total=0;
                while(integer(m.word(m.user,table+8*count))!=0){total+=m.word(m.user,table+8*count+4);++count;}
                if(count==0){if(!cleanup(item))return false;continue;}
                if(!s.random)return false;
                const auto draw=s.random(s.user,integer(total));if(!draw)return false;
                uint32_t cumulative=0;bool selected=false;
                for(uint32_t i=0;i<count;++i){const auto weight=m.word(m.user,table+8*i+4);
                    if(*draw>=integer(cumulative) && *draw<integer(cumulative+weight)){
                        m.storeWord(m.user,item+0x10,m.word(m.user,table+8*i));selected=true;break;}
                    cumulative+=weight;
                }
                if(!selected)return false; // Outside authentic positive-weight table/RNG domain.
            }
            m.storeWord(m.user,item+8,uint32_t(index)+180);
            const auto size=c(-10928,a),zero=c(-10936,a);real(item+0x20,size,a);real(item+0x24,zero,a);
            const auto y=add(real(r+0x4c,a),real(species+0x10,a));
            const auto lower=sub(y,c(-10812,a)),z=add(size,real(r+0x50,a));
            real(item+0x14,add(real(r+0x48,a),real(species+0xc,a)),a);real(item+0x18,lower,a);real(item+0x1c,z,a);
            m.storeWord(m.user,item+4,m.word(m.user,item+4)+1);
            [[fallthrough]];
        }
        case 1:{
            const auto timer=integer(m.word(m.user,item+8));
            if(timer>0){
                const auto elapsed=integer(180u-uint32_t(timer));const auto step=elapsed<5?elapsed:5;
                const auto x=add(real(r+0xc0,a),real(species+0xc,a));
                if(!s.interpolate)return false;
                const auto lift=s.interpolate(s.user,0,step,5,c(-10708,a),c(-10936,a));if(!lift)return false;
                const auto y=add(add(real(r+0xc4,a),real(species+0x10,a)),*lift);
                const auto z=add(c(-10928,a),real(r+0xc8,a));
                real(item+0x14,x,a);real(item+0x18,y,a);real(item+0x1c,z,a);continue;
            }
            int32_t other=0;
            for(;other<100;++other)if(other!=index){
                const auto p=packedBattleAudienceItemAddress(other,a);
                if(m.word(m.user,p)&1){const auto q=packedBattleAudienceItemAddress(other,a);
                    if(integer(m.word(m.user,q+4))==1){m.storeWord(m.user,item+0x44,2);break;}}
            }
            if(other==100)m.storeWord(m.user,item+0x44,1);
            const auto target=detectPackedBattleAudiencePlayerTarget(s);if(!target)return false;
            if(*target==0){if(!cleanup(item))return false;continue;}
            m.storeWord(m.user,item+0x40,*target);
            const auto body=packedBattleBodyPartId(*target,m);const auto part=packedUnitPart(*target,body,m);
            if(!s.hitPosition)return false;
            const auto position=s.hitPosition(s.user,*target,part);if(!position)return false;
            const auto dx=sub((*position)[0],real(item+0x14,a));
            const auto duration=add(c(-10720,a),divide(float(std::fabs(dx)),c(-10716,a)));
            real(item+0x28,divide(dx,duration),a);
            if(!s.truncate)return false;
            const auto ticks=s.truncate(s.user,duration);if(!ticks)return false;
            const auto square=mul(duration,duration);real(item+0x34,c(-10936,a),a);real(item+0x2c,c(-10712,a),a);
            const auto dy=sub((*position)[1],real(item+0x18,a));
            if(!s.fma)return false;
            const auto gravity=-s.fma(s.user,real(item+0x2c,a),duration,-dy);
            real(item+0x38,divide(mul(c(-10864,a),gravity),square),a);
            real(item+0x30,divide(sub((*position)[2],real(item+0x1c,a)),duration),a);
            real(item+0x3c,c(-10936,a),a);m.storeWord(m.user,item+8,uint32_t(*ticks));m.storeWord(m.user,item+4,m.word(m.user,item+4)+1);continue;
        }
        case 2:
            if(!(m.word(m.user,battle+0xef4)&0x80))m.storeWord(m.user,item+0x44,0);
            if(integer(m.word(m.user,item+8))==20)packedBattleAudienceChangeStatus(integer(m.word(m.user,item+0xc)),0,a);
            if(integer(m.word(m.user,item+8))==0)m.storeWord(m.user,item+4,m.word(m.user,item+4)+1);
            if(integer(m.word(m.user,item+8))<=0)continue;
            integrate(item);
            if(integer(m.word(m.user,item+0x10))==239)real(item+0x24,add(real(item+0x24,a),c(-10704,a)),a);
            m.storeWord(m.user,item+8,m.word(m.user,item+8)-1);continue;
        case 3:{
            if(!(m.word(m.user,battle+0xef4)&0x80))m.storeWord(m.user,item+0x44,0);
            const auto target=m.word(m.user,item+0x40);if(!target)return false;
            if(int8_t(getPackedBattleStatus(target,18,a).duration)>=1 || shadowGuard(target,a))m.storeWord(m.user,item+0x44,0);
            if(m.word(m.user,item)&4)m.storeWord(m.user,item+0x44,0);
            auto kind=integer(m.word(m.user,item+0x10));
            if(kind>=128 && kind<236 && packedBattleAudienceEmptyItemSlots(a)<=0)m.storeWord(m.user,item+0x44,0);
            if(integer(m.word(m.user,item+0x44))!=0){
                m.storeWord(m.user,owner+0x137f8,m.word(m.user,owner+0x137f8)+1);
                if(integer(m.word(m.user,owner+0x137f8))>3)m.storeWord(m.user,owner+0x137f8,3);
                if(integer(m.word(m.user,item+0x44))==2)m.storeWord(m.user,item+4,4);
                else{
                    kind=integer(m.word(m.user,item+0x10));
                    if(kind>=121 && kind<236)m.storeWord(m.user,base(a)+0x137e8,0);
                    else if(kind>=236 && kind<240)m.storeWord(m.user,base(a)+0x137e8,1);
                    m.storeWord(m.user,base(a)+0x137ec,m.word(m.user,target));
                    kind=integer(m.word(m.user,item+0x10));
                    if(kind>=236 && kind<240){
                        const auto table=0x8039c880u+uint32_t(kind-236)*0xc0;
                        for(unsigned off=0;off<0xc0;off+=4)m.storeWord(m.user,owner+0x137fc+off,m.word(m.user,table+off));
                        m.storeWord(m.user,owner+0x1381c,m.word(m.user,owner+0x1381c)+m.word(m.user,owner+0x137f8)-1);
                    }
                    m.storeWord(m.user,base(a)+0x137e4,m.word(m.user,item+0x10));
                    const auto present=integer(m.word(m.user,base(a)+0x137e8));
                    if(present==0){m.storeWord(m.user,item,m.word(m.user,item)&~2u);m.storeWord(m.user,item+8,5);m.storeWord(m.user,item+4,7);}
                    else if(present==1){
                        if(!stop())return false;
                        if(real(item+0x28,a)<c(-10936,a))real(item+0x28,mul(real(item+0x28,a),c(-10700,a)),a);
                        real(item+0x2c,c(-10848,a),a);real(item+0x30,c(-10936,a),a);m.storeWord(m.user,item+4,4);
                    }
                    m.storeWord(m.user,owner+0x137f8,0);
                }
            }else m.storeWord(m.user,item+4,4);
            if(integer(m.word(m.user,item+4))!=4)continue;
            [[fallthrough]];
        }
        case 4:
            integrate(item);
            if(real(item+0x18,a)<=c(-10936,a)){
                if(!stop())return false;
                m.storeWord(m.user,item+4,5);real(item+0x18,c(-10840,a),a);real(item+0x2c,c(-10928,a),a);real(item+0x30,c(-10936,a),a);
            }
            continue;
        case 5:
            integrate(item);
            if(real(item+0x18,a)<=c(-10936,a)){m.storeWord(m.user,item+8,60);m.storeWord(m.user,item+4,6);real(item+0x18,c(-10936,a),a);}continue;
        case 6:{
            const auto timer=integer(m.word(m.user,item+8));
            if(timer<=0){if(!cleanup(item))return false;continue;}
            if(timer<30)m.storeWord(m.user,item,(uint32_t(timer)&3)<=1?m.word(m.user,item)|2:m.word(m.user,item)&~2u);
            m.storeWord(m.user,item+8,m.word(m.user,item+8)-1);continue;
        }
        case 7:
            m.storeWord(m.user,item+8,m.word(m.user,item+8)-1);
            if(integer(m.word(m.user,item+8))<=0 && !cleanup(item))return false;
            continue;
        default:continue;
        }
    }
    return true;
}
}
