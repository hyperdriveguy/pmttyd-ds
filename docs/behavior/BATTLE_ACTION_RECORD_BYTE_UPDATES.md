# Complete action-record byte updates and concrete damage/cost bindings

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BtlActRec_AddPoint8018f96c/24 and AddCount8018f990/18;scoped direct
DamageDirect/cost/turn bindings and34-site census. Read
[turn-rule judgment](BATTLE_ACTION_RECORD_TURN_RULES.md),
[first-process caller](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md),
[direct damage](BATTLE_DAMAGE_APPLICATION.md) and
[weapon payment](BATTLE_WEAPON_COST_AND_PAYMENT.md).
Verify:`python3 tools/verify_battle_action_records.py`:twelve complete/linked
hashes,203 anchors,SDA setup and34 branch-shaped direct DOL calls.
Static original DOL evidence;no original bookkeeping/turn/RNG execution trace.

## AddCount(pointer):guard,not generic normalization

Read UNSIGNED byte old at pointer. If old>=100,return WITHOUT ANY write.
Otherwise store low8(old+1). No pointer/null/family/index/context validation,
second byte read,persistence service or scalar result contract. It does not
normalize an out-of-domain prior101..255 back to100.

Examples:99→100;100→100 with no write;255→255 with no write. Pointer denotes
exactly one byte,not a32-bit counter or resolved event destination. There is
no implicit per-unit/global action classification in this helper.

## AddPoint(pointer,deltaWord):narrow/store BEFORE cap

Read U8 old. Compute wrap32(old+deltaWord);store its LOW8 to pointer. Reload
U8 from that SAME pointer. If reloaded<=100,return;otherwise STORE byte100.
No pre-sum clipping,unsigned overflow error,negative delta rejection,null check,
family routing or disk save. The first store ALWAYS happens,including delta0;
the optional cap is a distinct second store. No scalar result contract.

Ordinary returned storage under no outside alias mutation is
`min(U8(old+deltaWord),100)`,NOT `min(old+deltaWord,100)`. Between store/reload,
there is no callback,but source/interrupt/alias behavior is not silently made
atomic. Capture storage through a byte provider retaining both writes.

Examples:99+2 stores101 then100;100+200 stores44 and does not cap;255+1 stores0;
0+(-1) stores255 then100;200+0 stores200 then100. A wide native saturating
counter would differ for legitimate low-byte overflow even when prior<=100.
The helper sees a word delta;its low8 determines arithmetic storage,but callers
may already have narrowed their inputs and their own positive guards apply.

## Damage application uses requested HP amount

Let B be the CAPTURED battle pointer retained by BattleDamageDirect,D the unit,
H its requested signed HP amount. If H<=0,no action-record calls in this block.
For H>0 and signed D.kind8==222:
AddCount(B+16f3e),THEN AddPoint(B+16f3c,U8(H)). For224<=kind<231:
AddCount(B+16f3f),THEN AddPoint(B+16f3d,U8(H)). Other kinds get neither.
These are original fields/order at800fe848..8a4,not invented owner IDs.

Bookkeeping uses requested H even when actual HP mutation is suppressed,
smaller than H,lethal or capped elsewhere. The helper input is U8(H),not a
later retained damage byte or measured loss. For H256,point input0 but count
still increments. No automatic FP action record in this local HP block. See
linked full direct application for preceding resources/effects and later Sleep/
audience/hit-event requests.

Weapon payment instead uses AddPoint THEN AddCount:kind222→B16f4b/16f4c;
224..230→B16f4d/16f4e,with U8(calculated positive FP cost) as point delta.
Its captured/reloaded owner/cost/source policy remains the linked payment
contract. Do not swap these two caller sequences to a generic “record action”
transaction or infer point counts are always actual resource loss.

The first-process caller uses AddCount(explicitB+16f57),AFTER global turn-rule
judgment and after its separate signed-halfword turn increment. Count byte,
turn halfword and unit-counter byte are distinct storage with different width/
cap/wrap rules. No uniform turn-number owner is established by these helpers.

##34-site census and remaining storage ownership

Verifier scans aligned linked-branch words in all seven DOL text sections for
AddPoint,AddCount and JudgeTurnRuleKeep:four DamageDirect calls,24 count calls
in _btlcmd_SetAttackEvent,two first-process calls (judge/count) and four payment
calls,34 total. The attack command containing body is fingerprinted ONLY as a
linked identity;its selection/classification branches are not recovered here.
REL/indirect callers/raw writes/record initialization/reset/save consumers are
not included by this scoped census. User-supplied original ISO is read-only.

These record pointers are battle-work offsets,not automatically pouch/save
fields. Helpers do no persistence request. Generic descriptions in older damage
notes that left saturation unresolved are superseded by this exact byte policy;
record source/clear/action/scene/turn/general-rule/persistence lifecycle and
original traces remain DMG-02/STATUS-01 work.

Acceptance:99/100/101/255 count and no-write behavior;point raw delta0/negative/
large,wrap-before-cap,first-store/reload/second-store distinction;requestedH and
kind guards,H256/low byte,captured battle pointer,cost reverse call order;turn
byte distinct from halfword/unit byte;all34 exact direct sites. No original
runtime/resource/turn equivalence is claimed by host arithmetic fixtures alone.
