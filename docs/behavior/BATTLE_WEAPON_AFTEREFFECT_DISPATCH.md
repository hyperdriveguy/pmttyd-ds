# Weapon aftereffect: ordered stage decisions and random consumption

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete static btlevtcmd_WeaponAftereffect801054e8/20c. Read
[stage request leaves](BATTLE_STAGE_AFTEREFFECT_REQUESTS.md),
[shared RNG](BATTLE_ENEMY_CANDIDATE_CHOICE.md) and
[ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md).
Verify: `python3 tools/verify_weapon_aftereffect.py`. No original execution claim.

## Inputs and exact decision order

Capture shared BattleWork W BEFORE resolving the sole event operand through
evtGetValue; resolved value is weapon pointer V. Initialize local booleans
A1=A2=B=false. No actor translation, damage/result/HP/status/resource query,
weapon null check, first-call gate or retained continuation occurs locally.
The callback completes with event result2 after the following whole sequence.

Read unsigned weapon bytes in order +b6,+b5,+b7,+b4 and call
irand(their sum). All four are weights, not independent percentages or a
normalized sum100. Capture forceWord=W.word19078 AFTER that call. Its0x10
sets A1, its0x20 sets A2. Then reread V.b4/b5/b6 as subtraction proceeds:

| Draw range, with current reread weights | Additional booleans |
| --- | --- |
| draw-b4<0 | A1=true,A2=true; skip other subtractions |
| otherwise subtract b5 and result<0 | A1=true;skip b6 |
| otherwise subtract b6 and result<0 | A2=true |
| otherwise | No additional flag (b7 remainder) |

Subtractions are signed word arithmetic; initial sum cannot exceed1020.
The force booleans are additive before stage inhibition, not mutually exclusive.
The b7 byte is never reread in the classification step.

Read stageByte=U8(W+180f8). Its0x4 clears A2; its0x2 clears A1.
When this SAME retained stageByte has0x2 and lacks0x8, call irand(100), then
read V.b8. If draw<V.b8 OR freshly read W19078 has0x40, set B=true.
In that taken branch reread W180f8; if it lacks0x4,set A2=true again. This can
restore A2 that the earlier captured stage byte inhibited if a service mutated
stage state. B is never independently suppressed afterward.

Call DestroyA1 if A1, THEN DestroyA2 if A2, THEN DestroyB if B. Each stage
leaf reloads shared work; scalar returns are ignored. All three may run.
After these calls, process these three decisions in order; EACH always calls
irand(100), then rereads the chance byte, even if zero or force is set:

| Chance | Force tested only if draw>=chance | Taken request |
| --- | --- | --- |
| V.b9 | Fresh W19078&0x200 | BattleStage_NozzleDirChangeCheck() |
| V.ba | Fresh W19078&0x100 | BattleStage_NozzleWorkCheck(1) |
| V.bb | Fresh W19078&0x80 | BattleStage_IronFrameFallCheck() |

Short circuit matters: a successful chance bypasses the force-word read.
Finally reread W180f8. If its0x20 is clear, skip ALL object-fall work and RNG.
If set, reread W19078. Its0x1 directly requests ObjectFallCheck with no draw.
Otherwise call irand(100), reread V.bc, and request ObjectFallCheck only when
signed draw<chance. Its return is ignored. All comparisons are strict <.

Do not snapshot every chance/force/stage field up front, merge RNG calls or
reorder stage service callbacks. Shared work captured by this wrapper may
coexist with a freshly loaded pointer in leaves; no local identity check.

## Concrete ordinary weapons and callers

Nine unsigned bytes at weapon+b4..bc are:

| Weapon | Pointer | b4,b5,b6,b7 | b8,b9,ba,bb,bc |
| --- | --- | --- | --- |
| Default Jump | 0x803538a8 | 0,0,0,100 | 0,4,2,1,1 |
| Default Hammer | 0x80353de8 | 0,3,3,100 | 3,6,2,3,3 |

Hammer's initial sum is106. Jump's initial draw still calls irand(100) despite
its first three weights being zero. With stable stage/force state zero, Jump
selects no destruction but always consumes that first draw plus the three
later100-bound draws. Successful nozzle-direction callbacks can consume three
MORE draws; nozzle type initialization can consume another. Zero chance is
not permission to omit a mandatory draw. Initial sum0 would call irand(0),
which consumes no rand state, but does not skip the later mandatory draws.

Ordinary Jump calls WeaponAftereffect(L12) on first-pass failure and final
success return paths at0x80356668/0x803568ec, before bounce/return motion. Those two actual serialized
bindings are pinned by the verifier. Missing-target and avoidance-only paths
can bypass this service as the graph specifies. Weapon L12 comes from the
first-attack default or ordinary selected address, not always the default.
There is no outcome distinction inside this callback itself.

Acceptance cases: each weighted boundary and sum0/106; force then inhibition;
conditional B draw/force and A2 restoration; all three destruction calls/order;
mandatory draws with chance0/forced requests; force short-circuit reads; chance
100/>100; object stage gate/forced no-draw; mutable metadata and nested RNG.
Stage setup/flag writers, target event bodies, actual hazard effects, seed/call
interleaving and original traces remain open. This is not a complete stage,
audience, status, resource or save implementation.
