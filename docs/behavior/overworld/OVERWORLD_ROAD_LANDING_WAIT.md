# Generic ROAD landing wait preserves ordinary airborne movement

US G8ME01 revision0; static caller/controller contract recovered2026-10-12.
Verify `python3 tools/verify_overworld_road_landing_wait.py`. Complete source
bodies and literal ordering/mutation sites are pinned. This is not a gameplay
trace, scheduler timing measurement or numerical-equivalence claim.
Read [road admission](OVERWORLD_ROGUEPORT_ROADS.md),
[key nesting](OVERWORLD_STORY_STATE_AND_ROAD_CONTROL.md),
[ordinary movement](OVERWORLD_MARIO_MOVEMENT.md),
[airborne owners](OVERWORLD_AIRBORNE_AND_LANDING.md),
[USER_FUNC waits](../event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) and
[independent inline events](../event/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md).

## Literal source event order

Once case10 admits an actual registered front contact and creates the departure
event, `bero_case_exec_road`8030dac8/12c performs:

| DOL command address | Source request |
| --- | --- |
| 8030dac8 | evt_mario_key_onoff(0) |
| 8030dad4 | RUN_CHILD_EVENT bero_case_init_check |
| 8030dadc | evt_mario_key_onoff(1) |
| 8030dae8–daf4 | If returned LW0==1, RETURN (custom/handled departure) |
| 8030dafc | evt_mario_key_onoff(0) |
| 8030db08 | Begin independent INLINE opcode107 |
| 8030db0c–db24 | LW0=directionIndex; LW0*=45; evt_mario_set_force_dir(LW0) |
| 8030db30 | evt_mario_wait_landon |
| 8030db38 | bero_get_position_normal(mode0,FLOAT150 distance) |
| 8030db48–db60 | Read BeroEX/EZ; evt_mario_mov_pos2(X,Z,FLOAT150 speed) |
| 8030db74–db84 | Set BeroEXEC10000, clear BeroEXEC1 |
| 8030db94 | End independent INLINE opcode109 |
| 8030db98 onward | Parent WAIT150ms, set BeroEXEC8, WAIT2frames, target-map check and mapchange request |

Thus input is off **before** the generic landing wait. There is a temporary
key-on after the child callback; the unhandled generic route immediately keys
off again. Do not bypass custom callback ownership or make that balanced nesting
sequence an arbitrary Boolean reset.

The wait/move commands are in an **independent inline entry**, not an attached
brother or a child that blocks the parent. Opcode107 advances the parent after
109 while copying its local work into the inline entry; the parent can continue
its own timer/mapchange sequence while the inline retains its wait/move
continuations. The parent starts WAIT150ms on its own first eligible invocation after inline
creation; this does not wait for landing, mov_pos2 completion or inline deletion.
The source WAIT_MSEC owner captures event-entry lifetime on its first poll, not
the inline travel-completion timestamp. Exact manager credits and cancellation
remain source services; these facts do not promote150ms to a universal fixed
number of native ticks.

After that timer, the parent sets BeroEXEC8, executes WAIT2frames, tests target
map against sys_01 and adds WAIT1000ms only for that match. It then calls
evt_bero_mapchange8007aad0. That leaf resolves its two target operands on each
poll. On firstCall!=0 it requests seqSet(3,targetMap,targetBero) at8007ab14–20;
it returns0 on that and all subsequent polls. It has **no** BeroEXEC, inline ID,
landing, root or travel-completion check. Later polls do not repeat seqSet. The
parent therefore requests mapchange independently of whether the inline has
landed, moved or deleted itself; the sequence/map-loading owner subsequently
controls scene replacement. Its exact fade/cancellation lifetime is a separate
contract, not an invented post-travel delay.

Beware the literal BeroEXEC wrapper: evt_bero_exec_onoff8007aa34 resolves its
arguments and uses first argument0 to OR the mask, and nonzero to AND-clear it
(8007aa68–7c). Its function name is not a normal Boolean set/clear convention.
Consequently db74(0,10000) sets10000, db84(1,1) clears1, and parent
dba0(0,8) sets8. Preserve these bits and their consumers explicitly.

## Landing-wait admission does not pin or move Mario

`evt_mario_wait_landon`800e41a0/c8 reads support HitObj Mario+1e8. If null it
returns USER_FUNC result0. With nonnull support, ordinary motion0/1/2 returns2;
other ordinary motion3/10/11 returns0 even if the jump retained its takeoff
support pointer. Additional accepted special states are motion22/21/25 when
signed counter+50==0, and motion26 unconditionally with nonnull support.
Those are separate domains, not reasons to accept ordinary jump ascent.

Result0 preserves the USER_FUNC continuation for polling. Result2 permits next
command in the same interpreter invocation. The callback makes **no** Mario root,
motion, tuple, retained speed/heading, input or control writes. It does not
snapshot a root position, compare landing against an event-trigger root, or
repeatedly restore X/Z. Subsequent floor contact/motion admission is owned by
ordinary movement/landing, not manufactured by the wait callback.

The preceding evt_mario_set_force_dir800e6230 checks signed counter+50>0 before
setting main flag40 and float heading+1a8 from the supplied integer degrees.
For counter<=0 it performs neither write. It directly changes neither rootXYZ,
retained speed+180 nor retained travel heading+1a4. Do not replace the existing
travel vector with the road direction merely because this wrapper requests it.

## Key-off suppresses live input while air momentum still runs

`marioKeyOff`8005c1f4 sets **main flag8** and increments nesting byte+39; it
does not set control-off main flag2, clear +180/+1a4, zero the vertical tuple,
change motion or hold rootXYZ. Its matching key-on decrements the nesting byte
and clears flag8 only when the signed result reaches zero.

The ordinary Mario main control-route decision at8005abc0–d8 tests main flag2.
With only key-off flag8, Mario still executes marioUpdateKeyData at8005abdc,
marioGetStick at8005abe8 and marioMotion at8005abf0. marioUpdateKeyData detects
main flags masked0x0e at80095de8–dec and zeroes all eight stick bytes+252..259.
With ordinary +290==0 it also zeroes held/trigger/release/cache buttons+24a..
250. Plain marioGetStick consequently publishes zero current magnitude.
The distinct +290/button/special-input domains retain their source gates and
are not generalized here.

For existing ordinary jump/fall, zero input does **not** imply zero retained
travel. Mario's ordinary motion owners preserve +180/+1a4 while airborne.
The source still dispatches jump vertical/head then marioMove (8005affc–b004),
or marioMove before fall integration (8005b04c onward). marioMoveMain's early
movement gate also tests main flag2 at80059514–1c, not flag8. It reaches the
retained-base addition +180*u(+1a4) at80059718 and8005977c–79c and the existing
speed/collision/support pipeline. Neutral input removes the live stick drift;
it does not remove that retained component or floor/wall/head interactions.
Button suppression can also shorten a held ordinary jump through the existing
released-A/counter gate; disabling input is not a vertical freeze.

This bounds plain Mario with no independent system/control stops, forced-motion,
transform, map/badge override or additive-motion service. Such separate flags
can alter dispatch, but the generic ROAD wait/key-off sequence does not itself
supply a root-position hold. Freezing native XZ throughout an airborne road wait
therefore contradicts this ordinary source composition.

## Source-derived integration cases

- Trigger during jump with nonzero retained+180: input becomes neutral; ordinary
  horizontal momentum and collision continue while wait result remains0.
- Trigger during motion3 with retained takeoff support: support alone must not
  complete the wait. The ordinary motion must become0/1/2 as well.
- Trigger during falling with null support: continue movement/gravity/landing;
  do not force support or a landing Y to satisfy the wait.
- Once support and ordinary ground motion satisfy the wait, the inline may start
  the documented Bero-position and scripted mov_pos2 owner. That is the first
  movement service in this local inline sequence; no previous XZ hold exists.
- Keep the parent150ms/2frame/mapchange continuation parallel to the inline
  landing/travel continuation. Even an extended airborne wait does not move the
  parent timer start to landing or gate the seqSet request on inline deletion.
- A custom handled return takes its own source path and bypasses the generic
  inline; key nesting and any callback-owned control changes must survive.

These are acceptance cases derived from source ordering/fields, not recordings
of original emulator behavior or a claim the current harness implements the
complete concurrent EVT/mapchange runtime.
