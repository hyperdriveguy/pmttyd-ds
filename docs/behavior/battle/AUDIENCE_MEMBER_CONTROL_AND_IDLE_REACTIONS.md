# Audience member control and idle reactions

US G8ME01 revision0, read-only DOL inspection. Portable implementation:
[`battle_audience_actor_control.cpp`](../../../source/battle/battle_audience_actor_control.cpp).
These are source rules, not Dolphin measurements.

Whole local functions covered:

| Original | Address | Size | SHA256 |
| --- | --- | --- | --- |
| BattleAudienceCtrlProcessKinopio | 80192a0c | 2f8 | 450ef489caafb07277e673bb12098ae377d49207a75d4ad42ff75a9d2b0f4e5a |
| WaitHurimuki | 80192164 | fc | fee99e17d995a64af1d43c5cfd68e75f3016fefeb05d8067616f0687f9055bdd |
| WaitKubiKatamuke | 80192054 | 110 | 14bb5521cd31b4a38fa506e740c5d974ac933dce1de779cae386979d8f5522b8 |
| WaitJump | 80191f20 | 134 | c08dececd5198ce5672a412ac23a8d60f688d20ff457f45349b85931f02a00c7 |

The dispatcher captures the member pointer, obtains fresh GetFront, then reads
unsigned state byte19. Original table80397b20 has21 sequential case labels
80192a58..80192b48 (SHA256
963318bea362cc4398eeb41c52c908845806d7b1c74b95d8a4f6a561db882c6a).
Each case calls its own original function, forwarding the complete front result.
States outside0..20 skip the state call, but still obtain front and run motion.
An unavailable state stops before motion; no successful no-op state is supplied.

Motion integrates position48/4c/50 with velocity78/7c/80 and then those velocities
with acceleration84/88/8c, axis by axis. Each add rounds to single precision.
The updated X velocity sets anglee8 to90 or270; zero/unordered retains it.
Negative heightd0 clears height/velocitydc/acceleratione4 before integration.
Type11 clears them unconditionally. Other types integrate height then velocity.
Type2 advances phase12c by original0.05, subtracts original2*pi once when >=,
calls source sine80272410 through an explicit numeric adapter, narrows its
result and multiplies by5 into130. Finalc0/c4/c8 include offsets; type2 also
adds hover130 after the Y+height addition. Other types retain old hover storage.
No finite clamps or repeated phase normalization are added.

Turn and tilt choose animation0 for front low-byte1, otherwise1. Turn writes
anglea0=12*timer through signed timer15, holds16..29, then writes
12*wrapped(45-timer) from30 onward. It returns to state0 only after45.
Tilt writes6c=1.5*timer for0..10, subtracts1.5 on20..40 and adds1.5 on50..60;
its status resets after60. Each otherwise increments timer30 with32-bit wrap.
Status reset uses the original whole ChangeStatus helper, including its source
vectors and animation/color counters; it is not a hand-written subset.

Jump chooses animation2/3 by the same front rule. Phase0 sets count8 to3 for
type7, otherwise1, then falls into phase1. Phase1 writes vertical velocity5 and
acceleration-1 for type7, otherwise3.4 and-0.2, increments phase and falls into
phase2. Only exactly zero acceleration decrements the count, resets state0 when
count<=0, or returns to phase1. Landing is detected on a later controller tick:
this function executes before the dispatcher clamps negative height. Other
phase values only select animation and return.

The native Control provider now uses this dispatcher with actual retained
camera4 eye/at for existing GetFront, existing packed animation/status helpers,
and native double libm sine as the declared SDK adapter. It records the actual
unavailable state address. Subsequent increments implement
[Wait with explicit caller FPR29 availability](AUDIENCE_IDLE_BREATHING_AND_DECISIONS.md)
and [cheering, celebratory jumps and booing](AUDIENCE_CHEERING_JUMPS_AND_BOOING.md).
States7/8/10..20, actual voice playback and full normal battle/camera scheduling
remain unavailable. No fabricated caller register is supplied. These functions
do not establish a complete positive-audience battle loop or accepted framing.

Tests cover inclusive motion phases, jump fallthrough/repeats, all21 dispatch
addresses, unavailable-provider store ordering, negative-height/type11 clearing,
single phase wrapping, final offsets, NaN facing preservation and pack constants.
No executable original bytes are imported into native runtime or committed.
