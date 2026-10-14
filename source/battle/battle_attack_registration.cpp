#include "battle_attack_registration.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const BattleAttackRegistrationServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
void write(const BattleAttackRegistrationServices& s,uint32_t a,uint32_t v) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,a,v);}
int32_t status(const BattleAttackRegistrationServices& s,uint32_t u,int32_t id) {assert(s.status);return s.status(s.user,u,id);}
EventEntry* create(const BattleAttackRegistrationServices& s,uint32_t script) {
    if(s.create)return s.create(s.user,script,10,0);
    assert(s.schedule);return &createRootEvent(*s.schedule,script,10,0,s.events);
}
EventEntry* select(uint32_t battle,uint32_t u,const BattleAttackRegistrationServices& s) {
    if(read(s,battle+0xef4)&0x40000000u)if(!(read(s,u+0x104)&0x2000)) {
        const uint32_t category=read(s,u+0x2ac);if(!(category&8) && !(category&0x10))return create(s,0x8034f234);
    }
    const uint32_t category=read(s,u+0x2ac);
    if(category&0x10) {const uint32_t script=read(s,u+0x2a0);return script?create(s,script):nullptr;}
    if(category&2)return create(s,read(s,u+0x2a0));
    if(status(s,u,1) || status(s,u,2) || status(s,u,9))return nullptr;
    if(status(s,u,27) && !(read(s,u+0x104)&0x10000))return nullptr;
    if(status(s,u,6)) {assert(s.irand);if(s.irand(s.user,100)<0)return create(s,0x8034e0cc);}
    if(status(s,u,5)) {
        assert(s.irand);if(s.irand(s.user,100)<50) {write(s,u+0x27c,read(s,u+0x27c)|0x10);return create(s,read(s,u+0x2a4));}
    }
    return create(s,read(s,u+0x2a0));
}
}
void registerBattleAttackState6(uint32_t battle,const BattleAttackRegistrationServices& s) {
    assert(s.unit);uint32_t u=s.unit(s.user,battle,sw(read(s,battle+0x420)));
    if(!(read(s,u+0x2ac)&1)) {assert(s.windowOff);s.windowOff(s.user);}
    u=s.unit(s.user,battle,sw(read(s,battle+0x420)));auto* e=select(battle,u,s);
    if(e) {
        write(s,u+0x2a8,e->id);e->owner=sw(read(s,u));
        const uint32_t wait=read(s,u+0x290);if(wait) {deleteEventId(wait,s.events);write(s,u+0x290,0);}
    }
    assert(s.advance);s.advance(s.user,battle,6);
}
}
