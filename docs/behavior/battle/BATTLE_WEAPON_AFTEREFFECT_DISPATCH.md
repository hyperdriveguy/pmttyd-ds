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

## Shared dispatch and Harbor completion (2026-10-14)

[`battle_weapon_aftereffect.cpp`](../../../source/battle/battle_weapon_aftereffect.cpp)
implements the complete local dispatch against retained source work and live
weapon metadata. It preserves ordered weight loads, signed subtraction/rereads,
force capture after the initial draw, retained inhibition, conditional B draw,
fresh restoration, ordered destruction, three mandatory chance draws, force
short-circuiting and fresh object gate/forced no-draw. All stage requests require
an explicit complete provider; missing services return no completion after prior
effects. The shared API is not a retry-safe continuation.

Native Harbor uses an RNG-copy admission preview and only the exact source leaf
exits already supported: null nozzle-direction table, nonpositive nozzle pouch
halfword, already-falling frame flags and the original boss object-fall mask1000.
The preview performs no live draws or source field writes, searches no alternate
roll and never converts a selected hazard into no hazard. Unsupported spawning/
replacement paths remain pending before live dispatch. After successful preview,
the same dispatch consumes the actual retained RNG exactly once. This is native
provider-admission policy, not an original extra invocation or RNG rewind.

The amount wrappers for HP/FP Drain now translate the RAW actor, retain work and
output, fetch that unit and return signed min(U8(badge),S32(dealt HP)), including
null-unit/negative values. They do not fabricate zero, reset damage, recover
resources or resolve the raw actor first. Native fresh zero badges therefore
follow the original recovery-tail skips naturally.

The original failed Jump now runs aftereffect, bounce/return and recovery child
through its actual DELETE. Native fixture checks no live attack/damage root,
Mario's original M_S_1/home position and restored wait handle, Crump HP4, dealt1,
failure1 and battle/story result0. Deferred TimingA state1000/main callback remain;
root deletion does not imply command-manager cleanup or turn consumption.
An explicit controlled phase dispatch then runs original mac_0_b_t1_002 with
its actual once-latch from that produced failure count, retaining speaker/helper
and US dialogue. The Game does not automatically schedule these phases/menus yet.

Tests cover weighted boundaries, sum0, force/inhibition, conditional B restoration,
request order, mandatory/optional draw counts, changed stage/object force and
unbound providers. Success still waits at original voice80104508. Native active
hazards, complete RNG/backend interleaving, main/display/turn ownership, original
menu/HUD and second-screen audience/item input remain open. No Azahar/Dolphin
or complete playable battle/visual/audio fidelity claim is made from host fixtures.
