#include "battle_packed_selection.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattlePackedSelectionServices& s,uint32_t a) {assert(s.memory.memory.word);return s.memory.memory.word(s.memory.memory.user,a);}
uint8_t byte(const BattlePackedSelectionServices& s,uint32_t a) {assert(s.memory.byte);return s.memory.byte(s.memory.user,a);}
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
int32_t sb(uint8_t v) {return v<128?int32_t(v):int32_t(v)-256;}
int32_t half(const BattlePackedSelectionServices& s,uint32_t a) {assert(s.memory.memory.half);const uint16_t v=s.memory.memory.half(s.memory.memory.user,a);return v<32768?int32_t(v):int32_t(v)-65536;}
}
// B+428 candidate buffer; SIGNED count e94 (buffer a6c), cursor ab7 and
// rank->row bytes a6d. Rows36 bytes from+4 hold signed unit/part halves.
// Nonpositive count writes-1/-1; otherwise load second half AFTER first store.
int32_t packedSelectedTargetEvent(EventEntry& e,const BattlePackedSelectionServices& s) {
    const uint32_t battle=currentBattleAddress(s.memory),args=e.arguments;
    const int32_t count=sb(byte(s,battle+0xe94));
    const int32_t first=sw(word(s,args)),second=sw(word(s,args+4));assert(s.values.store);
    if(count<=0) {s.values.store(s.values.user,e,first,-1);s.values.store(s.values.user,e,second,-1);return 2;}
    const uint32_t buffer=battle+0x428;const int32_t cursor=sb(byte(s,buffer+0xab7)),index=sb(byte(s,buffer+0xa6d+uint32_t(cursor)));
    const uint32_t row=buffer+4+0x24u*uint32_t(index);
    s.values.store(s.values.user,e,first,half(s,row));
    s.values.store(s.values.user,e,second,half(s,row+2));return 2;
}
// Always query current global B's acting420 unit BEFORE rejecting type.
// Types0..8/10/14 map source cursor offsets below; 5/14 branch on Mario222.
// Other types are unsupported native domains, not guessed default cursors.
bool findPackedCommandCursor(uint32_t menu,uint32_t type,uint32_t& cursor,const BattlePackedSelectionServices& s) {
    const uint32_t current=currentBattleAddress(s.memory);const int32_t actor=sw(word(s,current+0x420));
    const uint32_t unit=battleRegistryUnit(current,actor,s.memory);
    if(type>14)return false;
    uint32_t offset;
    switch(type) {case 0:offset=0x4b0;break;case 1:offset=0x4bc;break;case 2:offset=0x51c;break;case 3:offset=0x528;break;case 4:offset=0x4ec;break;
    case 5:offset=sw(word(s,unit+8))==222?0x4f8:0x504;break;case 6:offset=0x4c8;break;case 7:offset=0x4d4;break;case 8:offset=0x4e0;break;case 10:offset=0x510;break;
    case 14:offset=sw(word(s,unit+8))==222?0x498:0x4a4;break;default:return false;}
    cursor=menu+offset;return true;
}
// Use captured B menu171c/type1c5c, but cursor helper independently reads
// current global actor. Selected rows have28(decimal) stride, weapon pointer80.
std::optional<uint32_t> packedSelectedCommandWeapon(uint32_t battle,const BattlePackedSelectionServices& s) {
    const uint32_t menu=battle+0x171c,type=word(s,battle+0x1c5c);uint32_t cursor;
    if(!findPackedCommandCursor(menu,type,cursor,s))return std::nullopt;
    const uint32_t index=word(s,cursor);return word(s,menu+28u*index+0x80);
}
// Capture B before actor resolution; recapture current B for translated unit.
// Mario222/partners224..230 use captured-menu path; others use held item308
// -> item table803108a8, stride40(decimal), weapon pointer24(hex). Raw output;
// nullopt denotes unsupported cursor, whereas engaged weapon0 still stores0.
std::optional<int32_t> packedCommandWeaponEvent(EventEntry& e,const BattlePackedSelectionServices& s) {
    const uint32_t args=e.arguments,captured=currentBattleAddress(s.memory);assert(s.values.resolve && s.values.store && s.translate);
    const int32_t actor=s.values.resolve(s.values.user,e,sw(word(s,args))),output=sw(word(s,args+4));
    const int32_t id=s.translate(s.user,e,actor);const uint32_t current=currentBattleAddress(s.memory),unit=battleRegistryUnit(current,id,s.memory);
    const int32_t kind=sw(word(s,unit+8));std::optional<uint32_t> weapon;
    if(kind==222 || (kind>=224 && kind<=230))weapon=packedSelectedCommandWeapon(captured,s);
    else weapon=word(s,0x803108a8+40u*word(s,unit+0x308)+0x24);
    if(!weapon)return std::nullopt;
    s.values.store(s.values.user,e,output,sw(*weapon));return 2;
}
}
