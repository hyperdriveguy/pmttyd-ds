# Native source stage-floor lookup and jump recovery

The shared host-testable consumer implements original US
BattleGetFloorHeight800f7b84/50 and the complete three-operand event wrapper
TransStageFloorPosition8010510c/c0. It follows the recovered
[jump/floor contract](BATTLE_JUMP_EVENT_CALLBACKS.md#used-counter-recovery-stage-floor-wrapper-and-original-initialization).
`verify_battle_stage_floor.py` checks four complete original function bodies and
two constant spans in the read-only ISO. Static source evidence is distinct from
an original-runtime trace.

The wrapper resolves signed X, converts it to binary32, captures the Y operand
as a raw output token, then resolves/converts signed Z. It captures global battle,
queries floor, truncates toward zero and writes only the captured Y destination.
Y is never resolved as an input; no actor/part translation or event wait occurs.
The return code is2. Numeric conversion/truncation remains platform-owned.

The floor helper reloads global battle through BattleStageGetPtr, even when the
caller passed a different captured battle. It reads embedded stage+b2c and returns
original positive zero when Z<=threshold, otherwise stage+b34. Unordered NaN
comparisons take the latter branch. X/Y and the passed battle are unused. Actual
mutable stage fields are read each time; there is no unconditional ground-zero
fallback or per-query initialization.

Fresh tutorial and trio provision now copy the two original BattleStageInit
constant words into battle16f28/16f30:90.0 and-50.0. This function initializes only
those floor fields. It does not claim the full stage initializer, object/transitive
services, all later floor writers or map-specific physical collision are bound.
Both constants already reside in private imported DOL segments, so no additional
source payload extraction or scaling is needed. See [assets](../../ASSETS.md).

Tutorial JumpContinue now owns a real packed stage-floor provider. Its original
strict below-floor test remains: equality yields and the subsequent integration
can descend, clamp the visible root and return1. Ordinary trio counter-recovery
uses the same complete wrapper instead of its duplicated scoped calculation.

Tests cover global ownership reload between capture and query, output-token
capture after X conversion, never resolving Y, mutable thresholds/floors, equality,
NaN comparison, negative fractional truncation, preservation of adjacent fields,
and real JumpContinue equality/clamping on both source tiers. Original tutorial
fixtures check the two packed initialization words. The unrelated max-moves
fixture now supplies the verified original constant bytes required by provision.

This binding does not release unfinished source CheckDamage, select an attack,
advance turns, create feedback declarations, or fabricate a battle result.
Tutorial command/turn/damage ownership and audience/lower-screen rendering remain
required next work.
