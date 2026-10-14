#include "battle_display_footer.hpp"
#include <cassert>
namespace ttyd {
void BattleCommandPalette::step(){
    if(state<0 || state>5)return;
    static constexpr uint8_t channel[]{1,0,2,1,0,2},target[]{255,192,255,192,255,192};
    const auto index=uint8_t(state);rgba[channel[index]]=uint8_t(rgba[channel[index]]+((index%2)?-1:1));
    if(rgba[channel[index]]==target[index])state=int8_t((index+1)%6);
}
namespace {
bool enter(DisplayQueue& q,BattleFooterCursor& c,BattleFooterRecord request){
    if(!c.pending)c.pending=request;
    const auto& r=*c.pending;
    if(q.enter(r.camera,r.drawClass,r.callback,r.borrowed,r.priority)==DisplayAdmission::Unavailable)return false;
    c.pending.reset();return true;
}
void start(BattleFooterCursor& c){if(!c.active){c={};c.active=true;}}
}
bool registerBattleCommandDisplay(uint32_t battle,BattleCommandPalette& palette,DisplayQueue& queue,BattleFooterCursor& c,const BattleActorServices& a){
    assert(a.memory.word);start(c);
    if(uint32_t(battle+0x171c)==0){c.active=false;return true;}
    if(c.phase==0){palette.step();c.phase=1;}
    static constexpr uint32_t masks[]{1,0x10,0x100,0x1000,0x20,0x10000,0x100000};
    static constexpr uint32_t callbacks[]{0x8012210c,0x80121ce8,0x80121638,0x80121074,0x80121330,0x80120a44,0x80120844};
    while(c.phase<=8){
        const auto i=c.phase;
        if(i>1 && !c.pending && !(a.memory.word(a.memory.user,battle+0x1720)&masks[i-2])){++c.phase;continue;}
        if(!enter(queue,c,{8,0,i==1?0x8011fde0:callbacks[i-2],battle,i==1?902.f:500.f}))return false;
        ++c.phase;
    }
    c.active=false;return true;
}
bool registerBattleAudienceDisplay(uint32_t flags,const BattleAudienceWindowServices& window,DisplayQueue& queue,BattleFooterCursor& c){
    assert(window.window);
    if(!c.active){start(c);c.hidden=(flags&0x20000)!=0;}
    static constexpr uint32_t cameras[]{4,4,4,8},callbacks[]{0x801a38f8,0x801a38f8,0x801a37f0,0x801a36c8};
    static constexpr int32_t classes[]{1,2,1,0};
    while(c.index<4){
        const auto i=c.index;if(c.hidden){++c.index;continue;}
        if(!enter(queue,c,{cameras[i],classes[i],callbacks[i],0,i<3?0.f:499.f}))return false;
        ++c.index;
    }
    if(c.phase==0){const auto admitted=window.window(window.user);if(!admitted)return false;c.window=*admitted;c.phase=1;}
    if(c.window && !enter(queue,c,{8,1,0x801a35d0,0,499}))return false;
    c.active=false;return true;
}
bool registerBattleBreakSlotDisplay(DisplayQueue& queue,BattleFooterCursor& c){
    start(c);if(!enter(queue,c,{8,0,0x802036d4,0,490}))return false;c.active=false;return true;
}
bool processPackedBattleAudienceAnimations(uint32_t first,const BattleAudienceAnimationServices& s,BattleAudienceAnimationCursor& c){
    const auto& m=s.actors.memory;assert(m.word && m.storeWord && m.storeByte && s.actors.byte && s.scriptWord);
    if(!c.active){c={};c.active=true;c.first=first;}
    const auto next=[&](){++c.index;c.phase=0;};
    const auto read=[&](uint32_t address){return s.scriptWord(s.user,address);};
    while(c.index<200){
        const uint32_t r=c.first+c.index*0x134;
        if(c.phase==0){
            const auto flags=m.word(m.user,r);if((flags&5)!=5){next();continue;}
            const auto timer=m.word(m.user,r+0x2c);c.script=m.word(m.user,r+0x28);
            if(int32_t(timer)>0){m.storeWord(m.user,r+0x2c,timer-1u);next();continue;}c.phase=1;
        }
        if(c.phase==1){
            const auto opcode=read(c.script);if(!opcode)return false;
            if(uint32_t(*opcode+8u)>7u || int32_t(*opcode)==-1){next();continue;}
            switch(int32_t(*opcode)){
            case -8:c.phase=2;break;
            case -6:c.destination=r+0x34+4u*s.actors.byte(s.actors.user,r+0x40);c.phase=3;break;
            case -7:{
                const auto address=r+0x30+4u*s.actors.byte(s.actors.user,r+0x40);
                m.storeWord(m.user,address,m.word(m.user,address)-1u);
                const auto depth=s.actors.byte(s.actors.user,r+0x40);
                if(int32_t(m.word(m.user,r+0x30+4u*depth))<=0){m.storeByte(m.user,r+0x40,uint8_t(depth-1));m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+4u);next();continue;}
                c.nesting=1;c.phase=4;break;
            }
            case -5:c.phase=6;break;
            case -4:m.storeWord(m.user,r+0x28,c.script+8u);next();continue;
            case -3:m.storeWord(m.user,r+0x28,m.word(m.user,r+0x24));next();continue;
            case -2:c.phase=9;break;
            }
        }
        if(c.phase==2){const auto value=read(c.script+4u);if(!value)return false;m.storeWord(m.user,r+0x2c,*value);m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+8u);next();continue;}
        if(c.phase==3){const auto value=read(c.script+4u);if(!value)return false;m.storeWord(m.user,c.destination,*value);m.storeByte(m.user,r+0x40,uint8_t(s.actors.byte(s.actors.user,r+0x40)+1));m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+8u);next();continue;}
        if(c.phase==4){m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)-4u);c.scan=m.word(m.user,r+0x28);c.phase=5;}
        if(c.phase==5){
            const auto value=read(c.scan);if(!value)return false;
            if(int32_t(*value)==-6)--c.nesting;if(int32_t(*value)==-7)++c.nesting;
            if(int32_t(c.nesting)>0){c.phase=4;continue;}
            m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+8u);next();continue;
        }
        if(c.phase==6){const auto value=read(c.script+4u);if(!value)return false;c.label=*value;m.storeWord(m.user,r+0x28,m.word(m.user,r+0x24));c.phase=7;}
        if(c.phase==7){
            c.scan=m.word(m.user,r+0x28);const auto value=read(c.scan);if(!value)return false;
            if(int32_t(*value)==-4)c.phase=8;else{m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+4u);continue;}
        }
        if(c.phase==8){const auto value=read(c.scan+4u);if(!value)return false;if(*value==c.label){next();continue;}m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+4u);c.phase=7;continue;}
        if(c.phase==9){const auto value=read(c.script+4u);if(!value)return false;m.storeByte(m.user,r+0x1a,uint8_t(*value));c.phase=10;}
        if(c.phase==10){const auto value=read(c.script+8u);if(!value)return false;m.storeWord(m.user,r+0x2c,*value);m.storeWord(m.user,r+0x28,m.word(m.user,r+0x28)+12u);next();continue;}
    }
    c.active=false;return true;
}
}
