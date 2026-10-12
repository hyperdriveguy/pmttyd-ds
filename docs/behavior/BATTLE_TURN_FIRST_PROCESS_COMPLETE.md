# Complete local first-process turn preparation and fog-expiry request

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete btlseqTurnFirstProcess8011b1ec/f0,BtlUnit_CheckStatusFlag801277f8/1c,
OffStatusFlag801277d8/10 and BattleFogEndCheck80147c2c/78.
[Turn-rule judgment](BATTLE_ACTION_RECORD_TURN_RULES.md),
[byte updates](BATTLE_ACTION_RECORD_BYTE_UPDATES.md),
[move reset](BATTLE_MOVE_COUNTS.md),
[registry](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md) and
[RNG](BATTLE_ENEMY_CANDIDATE_CHOICE.md) close linked local helpers.
Verify:`python3 tools/verify_battle_action_records.py`.
Static original DOL behavior;full turn-controller/fog/effect/fortune/source and
original runtime cadence remain separate requirements.

## Ordered pre-unit operations

Capture EXPLICIT battle pointer B argument. Call global JudgeTurnRuleKeep()
FIRST,ignoring return. Then read SIGNED halfword B+0,add1,store low16 back;
reload signed16. If result>=2,request BattleMajinaiCheck800f7774;otherwise skip.
This called fortune engine is not recovered merely by its name. No lower/cap
check on the stored turn halfword;32767 wraps to-32768 and fails>=2.

Store WORD0 to B+18ff4 THEN B+18ff0. Call AddCount(B+16f57),using explicit B
rather than another global lookup. Its100 guard differs from halfword wrap.
No counter clear or blanket action record reset occurs in this local body.
Captured B survives later helper calls;the global rule/fog services select their
own current battle pointers independently,with no equality/stability check.

## Full64-slot unit loop

For i=0..63 ascending,call BattleGetUnitPtr(B,i). Null skips the unit. Nonnull U:

1. Read U8(U24),increment,store low8 U24. No100 cap,HP/activity/kind/belong test.
2. BtlUnit_ResetMoveStatus(U). Its Fast/Slow/sign/count policy is the complete
   linked move-reset contract. It therefore sees the ALREADY incremented U24,
   relevant to Slow parity;do not snapshot old parity or reorder these calls.
3. Read K=U.kindPointer10 and retain it. Read SIGNED K.byte89 as N0;
   irand(N0)→R. After return reread SIGNED K.byte89 as N1 and SIGNED K.byte88
   as A. Store word U28=wrap32(A+R-truncTowardZero(N1/2)). It uses the retained
   K pointer,not a new read of U10 after RNG. No source-span/priority clamp.
4. CheckStatusFlag(U,0x01000000). If returned word!=0,OffStatusFlag(U,same mask).
   Otherwise skip the clear. No callback/stopped/type filter in these leaves.

irand0 consumes no RNG;every nonzero signed bound uses the linked generator/
signed remainder policy. Priority/span fields can be reread differently after
RNG;do not cache N0 for the half term. This is per-unit signed WORD U28 ordering
state,not the event manager's unsigned8 event priority. No death/HP filter skips
non-null zero-HP units here;other controller/services own those later decisions.

CheckStatusFlag accepts null U→return0;otherwise returns RAW U.word138&mask,
not normalized1. OffStatusFlag has no null gate:read word138,and-complement mask,
store word138. No event/job flag or character attribute table is queried by these
local leaves. Clearing requested bits preserves all other word bits.

After all64 units,request BattleFogEndCheck() and IGNORE its result,then return
without specified scalar contract. Caller frequency/phase ownership is not
proved once-per-turn merely from this helper's name.

## BattleFogEndCheck complete local timer and effect requests

Independently load global battle B2. Read SIGNED WORD timer B2+182b8.
If EXACT0,return1 WITHOUT pointer read/delete;an existing nonnull effect pointer
is not cleaned up by that zero-timer path. Otherwise decrement timer with word
wrap and store;RELOAD timer. If signed new value>0,return0. If <=0,read pointer
B2+182bc;nonnull→effDelete8005e9c0,then store pointer0;null→skip request/store.
Return1. No timer normalization-to0,renderer callback,scene heap free or unit
status mutation beyond that effect request. Effect backend/ownership remains
separate. Incoming negative timers stay negative after ordinary decrement and
return1 until word wrap;minimum signed value decrements to max positive and
can take return0. These domains are not silently clamped into a native deadline.

The controller always ignores this result,so local0/1 does not become a turn
state transition here. Source timer/pointer writers,effDelete implementation,
frame/turn applicability and original fog visuals remain required.

Acceptance:global judgment before explicit counter increments,two cleared word
order,halfword wrap versus record byte cap;all64 slots/null/zero-HP,unit24 byte
wrap before Slow reset,retained kind pointer and post-RNG priority/span reads,
zero/nonzero RNG consumption,truncSigned half,raw mask getter/clear;global fog
recapture,exact0 with preserved pointer,positive1 expiry/nonnull deletion,
negative/wrapped timer and ignored result. Full general rule/controller/action/
fortune/effect/source/turn/scene/runtime ownership remains STATUS-01/DMG-02 work.


[Complete fortune producer](BATTLE_FORTUNE_PRODUCER.md) and
[initializer/call/RNG policy](BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md) now close
the local MajinaiCheck requested after the signed halfword increment>=2.
Countdown/mode/count remain distinct from battle16f57;full field assignment/
setup flag/pouch/save/turn/source and original execution remain open.
