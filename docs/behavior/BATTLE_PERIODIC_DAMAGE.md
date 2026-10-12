# Periodic Poison and Burn damage

US G8ME01 revision0, recovered 2026-10-12. B evidence is original US instructions
and data. This covers btlseqTurn states0x03000006..09 and their Burn effect and
shadow-guard helpers. The turn controller's HP/FP regeneration states and outer
entry cadence remain separate work. See [baseline](../ORIGINAL_BEHAVIOR.md),
[recovery ordering](BATTLE_TURN_RECOVERY.md) and
[direct damage](BATTLE_DAMAGE_APPLICATION.md). No original execution, measured
frame timing or Switch equivalence is claimed.

## Position in the turn sequence (B)

The verified78-entry dispatch table maps channel3 states as follows:

| State | Entry | Work |
| --- | --- | --- |
| 0x03000006 | 0x8011d898 | Poison scan |
| 0x03000007 | 0x8011c69c | shared event wait/conclusion check |
| 0x03000008 | 0x8011d978 | Burn eligibility scan |
| 0x03000009 | 0x8011da2c | Burn effect initialization, polling and event wait |

These precede status recovery states12..3e (hex suffixes). Neither periodic
scan decrements duration itself. A faithful replacement must preserve the
sequence of damage/effects, waits and later recovery instead of decrementing
all durations before applying periodic effects.

## Poison scan (`0x8011d898`–`0x8011d974`)

Scan unit slots0..63 ascending via BattleGetUnitPtr(B,slot). Skip a unit if
null, CheckStatus(unit,27) is nonzero, or signed current HP at+0x10c≤0.
Otherwise GetStatus(unit,4,&duration,&strength). Require signed strength>0;
the returned duration is not tested in this caller. Then require
BtlUnit_CheckShadowGuard(unit)=0.

For each eligible unit, mark the phase changed and call
`BattleDamageDirect(−5,unit,null,S8(strength),0,279,0,1)`.
The amount is the strength returned before this call, not a constant1. This
uses the full direct-damage path, including amount clamping, bookkeeping,
presentation, HP mutation, possible Sleep recovery and hit/death events.
There is no attack-weapon calculator or guard sampling here. Continue to the
next slot after each request; do not wait for each unit's event individually.

If none qualified, increment sequence channel3 twice and immediately recurse
into btlseqTurn, skipping state07. If any qualified, increment once and recurse
into07. The changed flag means a request was made, not proof that HP decreased.
State07's wait/conclusion protocol is documented in the recovery contract.

## Burn presence scan (`0x8011d978`–`0x8011da28`)

Scan slots0..63 with the same null/status27/positive-HP gates. GetStatus(unit,8)
and test its returned signed strength>0. Stop the scan at the first qualifying
unit. Shadow guard is **not** tested here.

If no unit qualifies, increment channel3 twice and recurse, skipping09.
Otherwise write halfword0 to B+2, increment channel3 once, and recurse into09.
B+2 is a substate here; it is not a elapsed-time value.

## Burn controller substate (`0x8011da2c`–`0x8011db5c`)

Read B+2 as unsigned halfword:

- **0:** increment B+2 to1. Scan all64 slots with the same eligibility gates
  as the presence scan. For each qualifying unit call
  BSE_TurnFirstProcessEffectEntry(unit,1). Then fall through to substate1 in
  this same invocation.
- **1:** scan all64 slots, calling BSE_TurnFirstProcessEffectMain(unit) for
  every nonnull unit. Do not repeat the status27, HP or Burn test during this
  polling scan. If any call returns nonzero, leave B+2 unchanged and exit.
  If all return0, increment B+2 and exit; do not enter the wait in this call.
- **Other values:** call BattleWaitAllActiveEvtEnd(B). If0, exit unchanged.
  Otherwise call BattleCheckConcluded(B); nonzero sets channel0=3 and
  channel7=0x07000000 then exits. Zero increments channel3 once and exits.

Polling does not stop at the first active effect: all nonnull units are updated
on each substate1 invocation. A unit's HP/status can change between entry and
final damage; the polling caller does not cancel the registered effect on that
basis. The helper's final guard test is separate below.

## Effect registration and state storage (B)

BSE_TurnFirstProcessEffectEntry at0x80195f38,size0x10 writes halfword1 to
unit+0x338 and the low16 bits of its second argument to unit+0x33a. It does not
clear a prior effect pointer or timer and does not delete an existing effect.
Repeated registration is therefore not a general safe replacement operation.

BSE_TurnFirstProcessEffectMain at0x80195d7c,size0x1bc resolves the body part
through GetBodyPartsId and GetPartsPtr before checking whether it is active.
Fields are stage U16(+338), type U16(+33a), counter word(+33c), scale float(+340),
and effect pointer(+344). No null-unit guard exists.

If stage=0 or type=0, return0. If type≠1, return0 without clearing fields.
For type1, the stages are:

| Stage | Ordered work | Return |
| --- | --- | --- |
| 1 | stage=2; scale=1.0; get body-part world position, z+=5; effFireEntry with integer arguments(0,0), floats(x,y,z,scale); store effect pointer; counter=0 | 1 |
| 2 | dereference effect payload at effect+0xc; increment counter; compute scale below; store unit scale and payload+0x24; if signed counter≥60 increment stage to3 | 1 |
| 3 | effDelete(pointer); clear pointer and stage; check shadow guard; if false request one damage as below | 0 |
| Other nonzero stage | no stage-specific work | 0 |

Stage2 computes binary32 `scale = fma(1.6f, sinfd(float(counter*3)), 0.4f)`.
The integer multiply uses32-bit arithmetic. The original fsubs conversion and
fmadds instructions determine rounding; 1.6 and0.4 are stored binary32 values,
not exact decimal rational numbers. The sinfd implementation and Fire rendering
remain dependencies; do not substitute an assumed host sine convention.

Constants at US0x80424728,2c,30,34 are respectively1.0,5.0,0.4f,1.6f.
The original does not check effect allocation before stage2 dereferences it.
At stage3, the type/counter/scale fields are left unchanged after pointer/stage
clearing. If CheckShadowGuard(unit)=0, request
`BattleDamageDirect(−5,unit,null,1,0,280,0,1)`.
No HP/status recheck occurs locally at this point, and the damage is constant1,
not the earlier retrieved Burn strength. The guard test occurs after effDelete.

With no external mutations, a fresh registration takes62 Main invocations:
one stage1,60 stage2,one stage3. The invocation reaching counter60 still
returns1; deletion and damage happen on the following call. This is a derived
call count, not measured frame duration. Since registration falls into polling,
the first invocation is made in the same btlseqTurn call as registration.
After all effects return0, the controller advances its substate and waits on
battle events on a subsequent invocation.

## Shadow guard predicate (B)

BtlUnit_CheckShadowGuard at0x80125d08,size0x3c returns1 if unit attributes
word+0x104 has0x10. Otherwise it returns1 only when kind word+8 equals228 and
signed word unit+0x228>0. All other cases return0. It assumes a valid unit.
No status helper, random draw or state mutation occurs here. Producers of these
fields and kind228's gameplay identity need independent data/event recovery.

This predicate gates Poison's initial request, Burn's final request, and the
[Freeze expiry request](BATTLE_TURN_RECOVERY.md). It does not suppress Burn
effect registration or Freeze expiry presentation in those callers.

## Differential cases and remaining dependencies (P)

Compare multiple poisoned units and verify slot order before the shared wait;
positive strength with differing duration; suppressed damage still registering
work; no eligible Burn skipping effect state; protected Burn still running its
effect; guard changes before final damage; status/HP changes while polling;
counter59/60/next-call boundaries; all active units updated each polling call;
registration over an existing pointer; unexpected type/stage handling.
These are derived comparison cases, not executed-original tests.

Still open: sinfd numeric contract, Fire effect rendering/lifetime, turn driver
cadence, Poison/Burn-specific event scripts, shared event waits/conclusion
internals and periodic HP/FP regeneration. These contracts specify local
periodic damage behavior, not complete status or turn equivalence.
