#include "battle_badge_recovery.hpp"
#include <cassert>

namespace ttyd {
namespace {
struct RecoveryArguments {DamageUnit* unit;int32_t amount;};
// Capture battle work before actor resolution. Actor-1 completes without
// part/amount evaluation; otherwise resolve discarded part BEFORE amount.
// The apparently unused getter remains observable through service callbacks.
RecoveryArguments arguments(const std::array<int32_t,3>& operands,const BadgeRecoveryEventServices& s) {
    assert(s.captureWork && s.resolve && s.translate);void* work=s.captureWork(s.user);
    const int32_t actor=s.translate(s.user,s.resolve(s.user,operands[0]));
    if(actor==-1)return {nullptr,0};
    assert(s.unit);auto* unit=s.unit(s.user,work,actor);assert(unit);
    s.resolve(s.user,operands[1]); // Discarded part, still resolved before amount.
    const int32_t amount=s.resolve(s.user,operands[2]);return {unit,amount};
}
}
// Badge query translates RAW actor (no resolve), caps hpDrain/fpDrain only
// above dealtHp and writes0 for missing unit. Output operand stays raw; negative
// dealtHp can yield negative point, so do not add an unobserved lower clamp.
int badgeRecoveryAmountEvent(BadgeRecoveryResource resource,const std::array<int32_t,2>& operands,
                             const BadgeRecoveryEventServices& s) {
    assert(s.captureWork && s.translate && s.unit && s.store);
    void* work=s.captureWork(s.user);const int32_t output=operands[1];
    const int32_t actor=s.translate(s.user,operands[0]); // Raw actor: no evtGetValue.
    auto* unit=s.unit(s.user,work,actor);int32_t point=0;
    if(unit) {
        point=resource==BadgeRecoveryResource::Hp?unit->badges.hpDrain:unit->badges.fpDrain;
        if(point>unit->dealtHp)point=unit->dealtHp;
    }
    s.store(s.user,output,point);return 2;
}
// Attribute40000000 bypasses gauge bookkeeping only. Ordinary HP recovery
// captures before/after, mirrors field202/206 to200/204 and ORs damage flag1.
// FP recovery delegates shared FP ownership instead of duplicating these HP fields.
int recoverHpEvent(const std::array<int32_t,3>& operands,const BadgeRecoveryEventServices& s) {
    const auto args=arguments(operands,s);if(!args.unit)return 2;auto& unit=*args.unit;
    if(unit.attributes&0x40000000)recoverUnitHp(unit,args.amount);
    else {
        unit.hpBefore=unit.hp;recoverUnitHp(unit,args.amount);unit.hpAfter=unit.hp;
        unit.field200=unit.field202;unit.field204=unit.field206;unit.damageFlags|=1;
    }
    return 2;
}
int recoverFpEvent(const std::array<int32_t,3>& operands,const BadgeRecoveryEventServices& s,
                   const DamageApplicationServices& resources) {
    const auto args=arguments(operands,s);if(args.unit)recoverUnitFp(*args.unit,args.amount,resources);
    return 2;
}
}
