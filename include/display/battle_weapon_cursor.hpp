#pragma once
#include <memory>
#include "battle_actor.hpp"
#include "source_icon_position.hpp"
namespace ttyd {
struct BattleWeaponCursorServices {
    BattleActorServices actors;
    std::shared_ptr<const void> lifetime;
    void* user=nullptr;
    // Prove actual UI allocation, queued callback/pass/camera and linked
    // resource/context ownership. False permanently retires a held call.
    bool (*current)(void*)=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t mode,int32_t step,int32_t duration,float start,float end)=nullptr;
    // Binary32 fused a*b+c. Source fnmsubs negates the rounded result;
    // the parent passes c=-sourceY, then negates the returned result.
    std::optional<float> (*fma)(void*,float,float,float)=nullptr;
    SourceIconMatrixServices icons;
};
class BattleWeaponCursor {
public:
    BattleWeaponCursor();~BattleWeaponCursor();
    BattleWeaponCursor(const BattleWeaponCursor&)=delete;
    BattleWeaponCursor& operator=(const BattleWeaponCursor&)=delete;
    bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend bool resumePackedBattleWeaponCursor(const std::array<uint32_t,3>&,const BattleWeaponCursorServices&,BattleWeaponCursor&);
};
// Whole original DrawCursor80117a6c/1b0. The source caller's copied XYZ bits
// must remain at this exact borrowed address through completion. Fresh X/Y
// reads occur separately for each reached icon; Z is never read by source.
// Timer mutation is source draw work, not a rendering-frame clock. The outer
// queued owner must finish linked actual GPU admission before display ack.
bool resumePackedBattleWeaponCursor(const std::array<uint32_t,3>& window,
    const BattleWeaponCursorServices&,BattleWeaponCursor&);
}
