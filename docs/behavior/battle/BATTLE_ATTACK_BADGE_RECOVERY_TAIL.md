# Ordinary attack badge recovery tail: exact graph and concurrency

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete32-command event btldefaultevt_SuitoruBadgeEffect8034f2e4/1e8,
SHA256 dadf7e175f491ab9e4a12ae63411cd7806df2bd0e73ec2be0a12e0b3ba454179.
Read [amount/resource wrappers](BATTLE_BADGE_RECOVERY_SERVICES.md),
[coercion/effect requests](BATTLE_BADGE_RECOVERY_PRESENTATION.md) and
[brother lifetime](../event/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md).
Verify: `python3 tools/verify_badge_recovery_tail.py`. Static requests only;
no measured delay or original execution claim.

## Complete ordered control flow

Actor selector is raw-2 throughout. L0..L7 refer to event local work words,
encoded operands0xfe363c80+i. Opcode29 is signed resolved >=;33 closes IF.

| Site | Request/control and consequence |
| --- | --- |
| 8034f2e4 | _get_heart_suitoru_point(-2,L5) |
| 8034f2f4 | IF L5>=1; otherwise skip through END_IF8034f3d0 |
| 8034f300 | Create attached brother;owner skips its body to8034f3c8 |
| 8034f304 | Brother GetBodyId(-2,L4) |
| 8034f314 | GetStatusMg(-2,L6), using float output |
| 8034f324 | GetHeight(-2,L7), integer output |
| 8034f334 | Opcode60: MULF L7,L6, with float resolution/store |
| 8034f340 | GetPartsPos(-2,L4,L0,L1,L2), integer XYZ outputs |
| 8034f35c | Opcode53: ADD L1,L7, with INTEGER resolution/store |
| 8034f368 | evt_eff(0,"recovery",0,L0,L1,L2,L5,0,0,0,0,0,0,0,0) |
| 8034f3a8 | Brother WAIT_FRAME120 |
| 8034f3b0 | RecoverHp(-2,L4,L5) |
| 8034f3c4 | END_BROTHER: delete brother;no local copy-back |
| 8034f3c8 | Owner WAIT_FRAME125, then END_IF8034f3d0 |
| 8034f3d4 | _get_flower_suitoru_point(-2,L5), overwrites OWNER L5 |
| 8034f3e4 | IF L5>=1;otherwise skip through END_IF8034f4c0 |
| 8034f3f0 | Create attached brother;owner skips body to8034f4b8 |
| 8034f3f4 | Brother GetStatusMg(-2,L6), then GetHeight(-2,L7)8034f404 |
| 8034f414 | MULF L7,L6 |
| 8034f420 | GetBodyId(-2,L4) |
| 8034f430 | GetPartsPos(-2,L4,L0,L1,L2) |
| 8034f44c | INTEGER ADD L1,L7 |
| 8034f458 | evt_eff(0,"recovery",1,L0,L1,L2,L5,0,0,0,0,0,0,0,0) |
| 8034f498 | Brother WAIT_FRAME120 |
| 8034f4a0 | RecoverFp(-2,L4,L5) |
| 8034f4b4 | END_BROTHER: delete brother;no local copy-back |
| 8034f4b8 | Owner WAIT_FRAME125, then END_IF8034f4c0 |
| 8034f4c4/8034f4c8 | Delete current event,then terminal marker |

Effect name pointer is0x802ee900, bytes"recovery\0"; no text/dialogue
resource is involved. The HP brother resolves body before scale/height; FP
resolves it afterward. These service orders are distinct and should remain so.
The documented [presentation contract](BATTLE_BADGE_RECOVERY_PRESENTATION.md)
retains encoded float quantization and integer addition rather than a naive
unrounded height*scale Y placement.

## Ownership, waits and ordinary Jump binding

Each brother receives a copy of the owner's event locals and shares original
unit/BattleWork service state. Each copies the positive L5 amount selected
before creation. The owner does not run brother commands itself,join it with
a child call or receive its changed L0..7. Its125-frame wait is separate from
the brother's120-frame wait. The FP amount query occurs after the owner's HP
wait; it is not captured simultaneously with the HP amount. No explicit brother
completion test precedes that FP query. Actual scheduling/credit/pause can affect
relative execution; do not claim the125 wait proves completion in every state.

Ordinary Jump label99 invokes at80356be0 this event as a waiting CHILD after
ResetFaceDirection and before StartWaitEvent(-2). Its parent waits for this
child's deletion; that differs from this child's own attached brothers. Owner
or ancestor deletion can delete associated brothers before their delayed resource
writes. Preserve original deletion/copy ownership, not detached host timers.
Missing target can still reach this tail, but amount depends on retained actor
state;the graph does not substitute a fresh fixed amount or clear damage.

There is no HP/FP/fullness/actor-kind/target-alive/result check in the event graph
beyond each amount>=1. There is no local badge removal,resource pouch export,
action-command stop,audience declaration or damage-counter reset. Both qualified
branches request presentation even when recovery later caps at full resources.
With both amounts below1, the owner reaches deletion without either125 wait.
One qualifying branch requests one owner125 wait;two request two sequential waits.
These are request counts, not verified elapsed real time or rendering frames.

Acceptance cases: amounts0/1/negative; both branches/order; full resources still
request effect/wait/recovery; copied amount survives owner L5 reuse; service
ordering difference; quantized scale/product versus integer ADD; owner/brother
cancellation; child wait versus brother no-copy-back. Native simulation/event
credits, delayed actor validity, effect renderer/audio and original traces remain
open. This closes the local badge tail, not the whole attack/event system.
