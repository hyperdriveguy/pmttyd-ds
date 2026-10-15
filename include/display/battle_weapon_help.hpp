#pragma once
#include <memory>
#include <array>
#include <map>
#include <optional>
#include <vector>
#include "battle_actor.hpp"
namespace ttyd {
// Original stack buffer, NOT an initialized C string. Unknown bytes stay
// unknown. Only a genuine source-frame owner can admit escaped stack writes.
struct BattleWeaponHelpBytes {
 std::array<std::optional<uint8_t>,192> cells{};
 std::map<uint32_t,uint8_t> escaped;
 std::optional<uint8_t> byte(uint32_t offset)const;
};
struct BattleWeaponHelpCall {
 uint32_t function=0,callsite=0;
 std::vector<uint32_t> integers;
 std::vector<float> reals;
 std::optional<uint32_t> privateColor;
 // Exactly one text domain for FontDrawString800773d0. A child encountering
 // an unknown private byte must wait for actual stack evidence, never add NUL.
 const BattleWeaponHelpBytes* privateText=nullptr;
 uint32_t privateOffset=0;
 std::optional<uint32_t> guestText;
};
struct BattleWeaponHelpServices {
 BattleActorServices actors;
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t guestAddress)=nullptr;
 // Whole window/font/message children, including side effects and retained
 // glyph/control parsing. A request or captured batch alone is not display ack.
 std::optional<uint32_t> (*request)(void*,const BattleWeaponHelpCall&)=nullptr;
 // Source has no buffer bound check. Escaping 192 bytes needs a genuine
 // source stack-frame alias owner; missing owner holds the original store.
 bool (*stackStore)(void*,uint32_t sourceFrameOffset,uint8_t)=nullptr;
};
class BattleWeaponHelpCursor {
public:
 BattleWeaponHelpCursor();~BattleWeaponHelpCursor();
 BattleWeaponHelpCursor(const BattleWeaponHelpCursor&)=delete;
 BattleWeaponHelpCursor&operator=(const BattleWeaponHelpCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
 const BattleWeaponHelpBytes* privateBytes()const;
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumePackedBattleWeaponHelp(uint32_t,float,float,const BattleWeaponHelpServices&,BattleWeaponHelpCursor&);
};
// Complete DrawHelp80117c1c/1d4, with original bytes and three row draws.
// Inputs must be the actual source arguments, not guessed parent stack values.
bool resumePackedBattleWeaponHelp(uint32_t key,float x,float y,
 const BattleWeaponHelpServices&,BattleWeaponHelpCursor&);
}
