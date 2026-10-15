#include "field_party_lifetime.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Table=0x8041ead8,Definition=0x8041ead4,Data=0x803143e8;
constexpr uint32_t Kill=0x800be69c,Kill2=0x800be594,Mario=0x8005e0c0;
uint32_t signedByte(uint32_t v){return v<128?v:v|0xffffff00u;}
bool negative(uint32_t v){return (v&0x80000000u)!=0;}
bool same(const BattleSequenceEntryServices& a,const BattleSequenceEntryServices& b){
    return a.user==b.user && a.word==b.word && a.byte==b.byte &&
        a.store==b.store && a.storeByte==b.storeByte && a.call==b.call;
}
template<class Cursor> struct Operations {
    Cursor& c;
    bool word(uint32_t a,uint32_t& v){auto n=c.owner.word?c.owner.word(c.owner.user,a):std::nullopt;if(!n)return false;v=*n;return true;}
    bool byte(uint32_t a,uint32_t& v){auto n=c.owner.byte?c.owner.byte(c.owner.user,a):std::nullopt;if(!n)return false;v=*n;return true;}
    bool store(uint32_t a,uint32_t v){return c.owner.store && c.owner.store(c.owner.user,a,v);}
    bool storeByte(uint32_t a,uint8_t v){return c.owner.storeByte && c.owner.storeByte(c.owner.user,a,v);}
    bool call(uint32_t function,uint32_t a=0,uint32_t b=0){
        if(!c.queued){c.call={};c.call.function=function;c.call.args[0]=a;c.call.args[1]=b;c.queued=true;}
        c.pending=c.call.function;
        if(!c.owner.call || !c.owner.call(c.owner.user,c.call))return false;
        c.queued=false;return true;
    }
};
}
bool removeFieldParty(int32_t index,bool forget,const BattleSequenceEntryServices& s,FieldPartyRemovalCursor& c){
    if(!c.active){c={};c.active=true;c.index=uint32_t(index);c.forget=forget;c.owner=s;}
    else if(c.index!=uint32_t(index) || c.forget!=forget || !same(c.owner,s))return false;
    Operations<FieldPartyRemovalCursor> m{c};
    for(;;){c.pending=c.queued?c.call.function:(forget?Kill2:Kill);switch(c.phase){
    case 0:if(negative(c.index)){c.phase=30;break;}if(!m.word(Table+c.index*4,c.actor))return false;c.phase=c.actor?1:30;break;
    case 1:if(!m.word(c.actor+0x160,c.mario))return false;c.slot=c.mario+c.index;++c.phase;break;
    case 2:if(!m.byte(c.slot+0x245,c.linkedIndex))return false;c.linkedIndex=signedByte(c.linkedIndex);++c.phase;break;
    case 3:if(negative(c.linkedIndex))c.linked=0;else if(!m.word(Table+c.linkedIndex*4,c.linked))return false;++c.phase;break;
    // The source dereferences linked+31 even for a negative index/null pointer.
    case 4:if(!m.byte(c.linked+0x31,c.value))return false;c.definition=Data+signedByte(c.value)*0x28;++c.phase;break;
    case 5:if(!m.store(Definition,c.definition))return false;++c.phase;break;
    case 6:if(!m.word(c.definition+0x24,c.hook))return false;c.phase=c.hook?7:9;break;
    case 7:if(!m.byte(c.actor+0x34,c.value))return false;c.phase=c.value==11?9:8;break;
    case 8:if(!m.call(c.hook,c.actor))return false;++c.phase;break;
    case 9:if(forget){c.value=0;c.phase=14;break;}if(!m.byte(c.slot+0x245,c.linkedIndex))return false;c.linkedIndex=signedByte(c.linkedIndex);++c.phase;break;
    case 10:if(negative(c.linkedIndex))c.linked=0;else if(!m.word(Table+c.linkedIndex*4,c.linked))return false;c.phase=c.linked?11:13;break;
    // Distinct second load: a provider/hook can change the table between reads.
    case 11:if(!m.word(Table+c.linkedIndex*4,c.linked))return false;++c.phase;break;
    case 12:if(!m.byte(c.linked+0x31,c.value))return false;c.phase=14;break;
    case 13:c.value=0;++c.phase;break;
    case 14:if(!m.storeByte(c.slot+0x247,uint8_t(c.value)))return false;++c.phase;break;
    case 15:if(!m.storeByte(c.slot+0x245,255))return false;++c.phase;break;
    case 16:if(!m.store(Table+c.index*4,0))return false;++c.phase;break;
    // The captured actor must survive table clear and both child continuations.
    case 17:if(!m.word(c.actor+0xc,c.value))return false;++c.phase;break;
    case 18:if(!m.call(0x8003b2d0,c.value))return false;++c.phase;break;
    case 19:if(!m.call(0x800300c4,0,c.actor))return false;c.phase=30;break;
    case 30:c={};return true;
    default:return false;
    }}
}
bool removeFieldPartyEvent(uint32_t entry,bool clear,const BattleSequenceEntryServices& s,FieldPartyRemovalEventCursor& c,int32_t& result){
    const uint32_t parent=clear?0x800e2da0:0x800e2e0c;
    if(!c.active){c={};c.active=true;c.entry=entry;c.clear=clear;c.owner=s;}
    else if(c.entry!=entry || c.clear!=clear || !same(c.owner,s))return false;
    Operations<FieldPartyRemovalEventCursor> m{c};
    for(;;){c.pending=c.queued?c.call.function:parent;switch(c.phase){
    case 0:if(!m.call(Mario))return false;c.mario=c.call.result;++c.phase;break;
    case 1:if(!m.word(c.entry+0x18,c.args))return false;++c.phase;break;
    case 2:if(!m.word(c.args,c.slot))return false;++c.phase;break;
    case 3:if(!m.byte(c.mario+c.slot+0x245,c.index))return false;c.index=signedByte(c.index);c.phase=negative(c.index)?5:4;break;
    case 4:if(!m.call(c.slot && clear?Kill2:Kill,c.slot?1:c.index))return false;++c.phase;break;
    case 5:result=2;c={};return true;
    default:return false;
    }}
}
bool removeMarioParties(const BattleSequenceEntryServices& s,FieldMarioPartyRemovalCursor& c){
    if(!c.active){c={};c.active=true;c.owner=s;}
    else if(!same(c.owner,s))return false;
    Operations<FieldMarioPartyRemovalCursor> m{c};
    for(;;){c.pending=c.queued?c.call.function:0x80141198;switch(c.phase){
    case 0:case 4:if(!m.call(Mario))return false;c.mario=c.call.result;++c.phase;break;
    case 1:case 5:if(!m.byte(c.mario+(c.phase==1?0x245:0x246),c.index))return false;c.index=signedByte(c.index);c.phase+=negative(c.index)?3:1;break;
    case 2:case 6:if(!m.call(Kill,c.index))return false;++c.phase;break;
    case 3:case 7:if(!m.storeByte(c.mario+(c.phase==3?0x245:0x246),255))return false;++c.phase;break;
    case 8:c={};return true;
    default:return false;
    }}
}
}
