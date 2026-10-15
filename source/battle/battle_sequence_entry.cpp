#include "battle_sequence_entry.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
float real(uint32_t v){float n;std::memcpy(&n,&v,4);return n;}
struct Operations {
    const BattleSequenceEntryServices& s;BattleSequenceEntryCursor& c;
    std::optional<uint32_t> word(uint32_t a){return s.word?s.word(s.user,a):std::nullopt;}
    bool load(uint32_t a,uint32_t& v){auto n=word(a);if(!n)return false;v=*n;return true;}
    bool store(uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);}
    bool call(uint32_t fn,std::array<uint32_t,5> args={}){
        if(!c.queued){c.call.function=fn;c.call.args=args;c.call.result=0;c.queued=true;}
        c.pending=fn;if(!s.call || !s.call(s.user,c.call))return false;c.queued=false;return true;
    }
    bool gp(){return load(0x80415fd0,c.gp);}
    bool field(){
        if(!c.getterCaptured){if(!gp())return false;c.getterCaptured=true;}
        if(!load(c.gp+0x168,c.field))return false;
        c.getterCaptured=false;return true;
    }
    bool done(){c.active=false;c.pending=0;return true;}
};
}
bool initializeBattleSequenceEntry(const BattleSequenceEntryServices& s,BattleSequenceEntryCursor& c){
    if(!c.active){c={};c.active=true;c.pending=0x80073f20;}Operations m{s,c};
    for(;;)switch(c.phase){
    case 0:if(!m.gp())return false;++c.phase;break;
    case 1:if(!m.load(c.gp+0xc,c.value))return false;++c.phase;break;
    case 2:if(!m.store(0x8041e968,c.value))return false;++c.phase;break;
    case 3:c.pending=0x8004829c;if(!m.field())return false;++c.phase;break;
    case 4:if(!m.load(c.field+8,c.npc))return false;++c.phase;break;
    case 5:if(!m.call(0x80031260,{0,0xf5de0180}))return false;c.phase=signedWord(c.call.result)<109?7:6;break;
    case 6:if(!m.call(0x80031260,{0,0xf5de0180}))return false;c.phase=signedWord(c.call.result)<111?11:7;break;
    case 7:if(!m.call(0x80031260,{0,0xf5de0180}))return false;c.phase=c.call.result==402?11:8;break;
    case 8:if(!m.call(0x800daefc,{1024}))return false;++c.phase;break;
    case 9:if(!m.load(c.npc+0x2f0,c.value))return false;++c.phase;break;
    case 10:if(!m.call(0x800db7a0,{1,c.value}))return false;++c.phase;break;
    case 11:if(!m.call(0x800d76b8,{1024}))return false;++c.phase;break;
    case 12:if(!m.call(0x80005f28,{0}))return false;++c.phase;break;
    case 13:if(!m.call(0x80035c68,{4}))return false;++c.phase;break;
    case 14:if(!m.call(0x800dc110,{0x80}))return false;++c.phase;break;
    case 15:c.pending=0x8004829c;if(!m.field())return false;++c.phase;break;
    case 16:if(!m.load(c.field+0x10,c.value))return false;if(c.value!=0x800000 && c.value!=0x1000000)return m.done();++c.phase;break;
    case 17:if(!m.load(0x8041e968,c.value))return false;if(c.value)return m.done();++c.phase;break;
    case 18:if(!m.call(0x8005c2c4))return false;++c.phase;break;
    case 19:if(!m.call(0x800bed44))return false;++c.phase;break;
    case 20:if(!m.call(0x80005f28,{0}))return false;return m.done();
    default:return false;
    }
}
bool updateBattleSequenceEntry(OriginalSequenceWork& w,const BattleSequenceEntryServices& s,BattleSequenceEntryCursor& c){
    if(!c.active){c={};c.active=true;c.pending=0x80073b24;}Operations m{s,c};
    for(;;)switch(c.phase){
    case 0:c.state=w[1];switch(c.state){case 0:case 10:c.phase=1;break;case 1:c.phase=10;break;case 2:c.phase=20;break;case 3:c.phase=30;break;case 4:c.phase=50;break;case 5:c.phase=60;break;case 6:c.phase=80;break;case 11:c.phase=90;break;default:return m.done();}break;
    case 1:if(!m.load(c.state==0?0x804203b0:0x804203b4,c.call.rgba))return false;++c.phase;break;
    case 2:if(!m.call(0x80013368,{22,c.state==0?1000u:200u}))return false;++w[1];if(c.state==0)w[4]=0;return m.done();
    case 10:if(!m.call(0x80007388))return false;c.root=c.call.result;++c.phase;break;
    case 11:if(!m.call(0x8018a0e8,{4,0,0x804203bc,c.root,0x802c2ae4}))return false;if(!c.call.result)return m.done();++c.phase;break;
    case 12:if(!m.call(0x80007388))return false;c.root=c.call.result;++c.phase;break;
    case 13:if(!m.call(0x8018a0e8,{4,0,0x804203bc,c.root,0x802c2b08}))return false;if(!c.call.result)return m.done();++w[4];++c.phase;break;
    case 14:if(!m.call(0x80010d94))return false;if(c.call.result)++w[1];return m.done();
    case 20:if(!m.call(0x800d5eb0))return false;++c.phase;break;
    case 21:c.pending=0x8004829c;if(!m.field())return false;++c.phase;break;
    case 22:if(!m.load(c.field+0x10,c.type))return false;++c.phase;break;
    case 23:if(!m.load(0x8041e968,c.value))return false;if(c.value){c.phase=28;break;}c.phase=c.type==0x800000 || c.type==0x1000000?26:24;break;
    case 24:if(!m.call(0x8005c2c4))return false;++c.phase;break;
    case 25:if(!m.call(0x800bed44))return false;++c.phase;break;
    case 26:if(!m.call(0x8005e0c0))return false;c.value=c.call.result;c.phase=100;break;
    case 100:if(!m.load(c.value,c.flags))return false;++c.phase;break;
    case 101:if(!m.store(c.value,c.flags&~1u))return false;c.phase=c.type==0x800000 || c.type==0x1000000?28:27;break;
    case 27:if(!m.call(0x80005f28,{0}))return false;++c.phase;break;
    case 28:c.pending=0x8004829c;if(!m.field())return false;++c.phase;break;
    case 29:if(!m.store(c.field+0x570,0))return false;++w[1];c.phase=30;break;
    case 30:c.pending=0x8004829c;if(!m.field())return false;++c.phase;break;
    case 31:if(!m.load(c.field+8,c.npc))return false;++c.phase;break;
    case 32:if(!m.load(c.npc+0x230,c.descriptor))return false;++c.phase;break;
    case 33:if(!m.call(0x800d5eb0))return false;c.pouch=c.call.result;++c.phase;break;
    case 34:if(!m.load(c.field+0x550,c.value))return false;c.phase=c.value?35:40;break;
    case 35:if(!m.load(c.field+0x558,c.value))return false;c.phase=signedWord(c.value)>=1 && signedWord(c.value)<=2?36:40;break;
    case 36:if(!m.load(c.field+0x554,c.value))return false;c.phase=c.value?37:40;break;
    case 37:if(!m.load(c.field+0x570,c.value))return false;++c.phase;break;
    case 38:if(!m.store(c.field+0x570,c.value+1))return false;++c.phase;break;
    case 39:if(!m.load(c.field+0x570,c.value))return false;if(signedWord(c.value)<200)return m.done();c.phase=40;break;
    case 40:{const auto rank=s.half?s.half(s.user,c.pouch+0x88):std::nullopt;if(!rank)return false;c.root=0x802c2adc;if(*rank<=2)c.root=0x802c2ac4+8*uint32_t(*rank);++c.phase;break;}
    case 41:if(!m.load(c.descriptor+4,c.value))return false;++c.phase;break;
    case 42:if(!m.call(0x8002892c,{c.root,c.value}))return false;++w[1];c.phase=50;break;
    case 50:if(!m.call(0x800738ac))return false;if(c.call.result)return m.done();++w[1];c.phase=60;break;
    case 60:if(!m.call(0x800f903c))return false;if(!c.call.result){c.phase=64;break;}w[4]=0;++w[1];++c.phase;break;
    case 61:if(!m.gp())return false;++c.phase;break;
    case 62:if(!m.load(c.gp+0xc,c.value))return false;if(c.value)w[1]=11;c.phase=64;break;
    case 64:if(!m.call(0x8005e0c0))return false;++c.phase;break;
    case 65:if(!m.call(0x8000f2fc,{4}))return false;++c.phase;break;
    case 66:if(!m.load(0x804203c4,c.value))return false;c.call.position[0]=c.call.position[1]=real(c.value);++c.phase;break;
    case 67:if(!m.load(0x804203c8,c.value))return false;c.call.position[2]=real(c.value);++c.phase;break;
    case 68:if(!m.load(0x8041eb90,c.value))return false;++c.phase;break;
    case 69:if(!m.load(c.value+0xef4,c.flags))return false;c.phase=c.flags&0x100?70:71;break;
    case 70:if(!m.load(0x804203cc,c.value))return false;{volatile float z=c.call.position[2]+real(c.value);c.call.position[2]=z;}++c.phase;break;
    case 71:if(!m.call(0x8000f2fc,{4}))return false;c.value=c.call.result;++c.phase;break;
    case 72:if(!m.load(c.value+0x114,c.value))return false;c.call.yaw=real(c.value);++c.phase;break;
    case 73:if(!m.call(0x800dc380))return false;return m.done();
    case 80:++w[4];if(signedWord(w[4])<=30)return m.done();++c.phase;break;
    case 81:if(!m.gp())return false;++c.phase;break;
    case 82:if(!m.load(c.gp,c.flags))return false;++c.phase;break;
    case 83:if(!m.call(0x8002e218,{(c.flags&0x3000)==0x3000?5u:2u,1,0}))return false;return m.done();
    case 90:if(!m.call(0x80010d94))return false;if(!c.call.result)return m.done();++c.phase;break;
    case 91:if(!m.gp())return false;++c.phase;break;
    case 92:if(!m.store(c.gp+0xc,0))return false;++c.phase;break;
    case 93:if(!m.call(0x8002e218,{5,1,0}))return false;return m.done();
    default:return false;
    }
}
}
