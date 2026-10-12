#include "battle_selection.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t signedByte(uint8_t v) {return v<128?v:int32_t(v)-256;}
int32_t signedHalf(uint16_t v) {return v<0x8000?v:int32_t(v)-0x10000;}
}
int selectedTargetEvent(const std::array<int32_t,2>& outputs,const BattleSelectionServices& s) {
    assert(s.currentWork && s.byte && s.half && s.store);
    const uint32_t work=s.currentWork(s.user),buffer=work+0x428;
    const int32_t firstOutput=outputs[0],secondOutput=outputs[1];
    if(signedByte(s.byte(s.user,buffer+0xa6c))<=0) {
        s.store(s.user,firstOutput,-1);s.store(s.user,secondOutput,-1);return 2;
    }
    const int32_t cursor=signedByte(s.byte(s.user,buffer+0xab7));
    const int32_t rowIndex=signedByte(s.byte(s.user,buffer+0xa6d+static_cast<uint32_t>(cursor)));
    const uint32_t row=buffer+4+0x24u*static_cast<uint32_t>(rowIndex);
    const int32_t unit=signedHalf(s.half(s.user,row));s.store(s.user,firstOutput,unit);
    const int32_t part=signedHalf(s.half(s.user,row+2));s.store(s.user,secondOutput,part);return 2;
}
bool findCommandCursor(uint32_t menu,uint32_t type,uint32_t& cursor,const BattleSelectionServices& s) {
    assert(s.currentWork && s.word && s.unit);
    const uint32_t current=s.currentWork(s.user);
    const int32_t actorId=signedWord(s.word(s.user,current+0x420));
    auto* actor=s.unit(s.user,current,actorId);
    if(type>14)return false;
    uint32_t offset;
    switch(type) {
    case 0:offset=0x4b0;break;
    case 1:offset=0x4bc;break;
    case 2:offset=0x51c;break;
    case 3:offset=0x528;break;
    case 4:offset=0x4ec;break;
    case 5:assert(actor);offset=actor->kind==222?0x4f8:0x504;break;
    case 6:offset=0x4c8;break;
    case 7:offset=0x4d4;break;
    case 8:offset=0x4e0;break;
    case 10:offset=0x510;break;
    case 14:assert(actor);offset=actor->kind==222?0x498:0x4a4;break;
    default:return false;
    }
    cursor=menu+offset;return true;
}
std::optional<uint32_t> selectedCommandWeapon(uint32_t capturedWork,const BattleSelectionServices& s) {
    assert(s.word);const uint32_t menu=capturedWork+0x171c;
    const uint32_t type=s.word(s.user,capturedWork+0x1c5c);
    uint32_t cursor;
    if(!findCommandCursor(menu,type,cursor,s))return std::nullopt;
    const uint32_t index=s.word(s.user,cursor);
    return s.word(s.user,menu+28u*index+0x80);
}
std::optional<int> commandWeaponAddressEvent(const std::array<int32_t,2>& operands,const BattleSelectionServices& s) {
    assert(s.currentWork && s.resolve && s.translate && s.unit && s.word && s.store);
    const uint32_t captured=s.currentWork(s.user);
    const int32_t actor=s.resolve(s.user,operands[0]);const int32_t output=operands[1];
    const int32_t translated=s.translate(s.user,actor);
    const uint32_t current=s.currentWork(s.user);auto* u=s.unit(s.user,current,translated);assert(u);
    std::optional<uint32_t> weapon;
    if(u->kind==222 || (u->kind>=224 && u->kind<=230))weapon=selectedCommandWeapon(captured,s);
    else weapon=s.word(s.user,0x803108a8u+40u*u->heldItem+0x24);
    if(!weapon)return std::nullopt;
    s.store(s.user,output,signedWord(*weapon));return 2;
}
}
