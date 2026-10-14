#include "battle_unit_resources.hpp"
#include <cassert>

namespace ttyd {
namespace {
int16_t half(int32_t value) {
    const uint16_t bits=static_cast<uint16_t>(value);
    return static_cast<int16_t>(bits<0x8000?bits:static_cast<int32_t>(bits)-0x10000);
}
int16_t read(const uint8_t* p,unsigned offset) {
    return half((uint32_t(p[offset])<<8)|p[offset+1]);
}
void write(uint8_t* p,unsigned offset,int16_t value) {
    const auto bits=static_cast<uint16_t>(value);
    p[offset]=bits>>8;p[offset+1]=bits;
}
uint8_t* pouch(const UnitResourceServices& s) {
    assert(s.pouch);auto* p=s.pouch(s.user);assert(p);return p;
}
bool partner(int32_t kind) {return kind>=224 && kind<=230;}
void cap(int16_t& current,int16_t maximum) {if(current>maximum)current=maximum;}
}
int32_t battlePartyIndex(int32_t kind) {
    switch(kind) {
    case 80:case 137:case 224:return 1;
    case 81:case 138:case 225:return 2;
    case 142:case 229:return 3;
    case 82:case 139:case 226:return 4;
    case 83:case 140:case 227:return 5;
    case 141:case 228:return 6;
    case 143:case 230:return 7;
    default:return 0;
    }
}
void initializeStatusChangeInfo(BattleStatusUnit& u) {
    for(auto& record:u.statusChangeInfo) {
        record.type=0xff;record.duration=record.strength=record.extra=0;
        record.value=0;
    }
}
void clearUnitStatus(BattleStatusUnit& u) {
    u.statusBytes.fill(0);setStatus(u,16,1,-99);
    initializeStatusChangeInfo(u);
}
void reviseUnitResources(DamageUnit& u,const UnitResourceServices& s) {
    auto* p=pouch(s);const int32_t kind=u.kind;
    if(kind==222) {
        u.baseHp=read(p,0x8e);
        u.maxHp=half(read(p,0x8e)+5*u.hpPlusCount);
        u.baseFp=read(p,0x90);
        u.maxFp=half(read(p,0x90)+5*u.fpPlusCount);
        cap(u.hp,u.maxHp);cap(u.fp,u.maxFp);
    } else if(partner(kind)) {
        const unsigned r=14*battlePartyIndex(kind);
        u.baseHp=read(p,r+4);u.maxHp=half(read(p,r+4)+5*u.hpPlusCount);
        u.hp=read(p,r+6);cap(u.hp,u.maxHp);
    } else {
        u.maxHp=half(u.baseHp+5*u.hpPlusCount);cap(u.hp,u.maxHp);
        u.maxFp=half(u.baseFp+5*u.fpPlusCount);cap(u.fp,u.maxFp);
    }
}
void importUnitResources(DamageUnit& u,const UnitResourceServices& s) {
    auto* snapshots=s.snapshots;auto* p=pouch(s);const int32_t kind=u.kind;
    if(kind==222) {
        u.hp=read(p,0x70);u.fp=read(p,0x74);reviseUnitResources(u,s);
    } else if(partner(kind)) {
        u.hp=read(p,14*battlePartyIndex(kind)+6);reviseUnitResources(u,s);
        assert(snapshots && partner(u.kind));
        const auto& b=(*snapshots)[u.kind-224];
        u.snapshotValue=b.value;u.flags=b.flags;u.statusBytes=b.statuses;
        u.snapshot13c=b.field28;u.snapshot13e=b.field2a;
        if(u.hp<=0)setStatus(u,27,1,1);
    }
}
void exportUnitResources(const DamageUnit& u,const UnitResourceServices& s) {
    auto* snapshots=s.snapshots;auto* p=pouch(s);const int32_t kind=u.kind;
    if(kind==222) {write(p,0x70,u.hp);write(p,0x74,u.fp);}
    else if(partner(kind)) {
        write(p,14*battlePartyIndex(kind)+6,u.hp);
        assert(snapshots && partner(u.kind));
        auto& b=(*snapshots)[u.kind-224];
        b.value=u.snapshotValue;b.flags=u.flags;b.statuses=u.statusBytes;
        b.field28=u.snapshot13c;b.field2a=u.snapshot13e;
    }
}
}
