# Electric and zero-gravity action events and entry services

US G8ME01 revision0, inspected 2026-10-12. B evidence is original script words,
callback instructions and constants. Names are public annotations. See
[baseline](../ORIGINAL_BEHAVIOR.md), [action controller](BATTLE_ACTION_CONTROLLER.md)
and [event runtime](EVENT_RUNTIME.md). No original execution or Switch comparison
was measured. Original script/disassembly extracts remain ignored.

## Electric movement script (B)

btldefaultevt_BiribiriMove is US0x8034e0cc,size0x74. The action controller's
Electric admission compares irand(100)<0, making that caller path unreachable
under the recovered RNG. That does not prove the script has no other callers.
Its complete local order is:

1. evt_sub_intpl_init(mode8,start0,end5,steps16).
2. Label0: evt_sub_intpl_get_value(), writing sample to LW(0) and continuation
   flag to LW(1).
3. SetDispOffset(−2,LW(0),0,0).
4. WAIT_FRAME(1).
5. If LW(1)=1, GOTO label0; otherwise continue.
6. StartWaitEvent(LW(10)); END_EVENT; END_SCRIPT terminator.

−2 resolves the bound event owner. LW(10) is a separate local: the script does
not initialize it, and a fresh action evtEntry starts it0. Therefore the final
wait restart resolves unit ID0 on that constructor path, while offset updates
use the actor owner. Preserve this distinction and inventory other constructors
before inferring the script always restarts its own actor.

No HP mutation, sound or message command occurs directly in this script.
Display offset is not unit world-position movement, and no final zero-offset
reset appears locally. Other pose/wait services may change presentation.

## Interpolation storage and stepping (B)

evt_sub_intpl_init at0x80056120,size0x98 resolves all four operands through
evtGetValue, then writes event local work words:
LW(11)=mode,LW(12)=start,LW(13)=end,LW(14)=0,LW(15)=steps. Return2.
These are shared script locals, not private callback storage. Other script use
of LW(11..15) can alter the continuation state.

evt_sub_intpl_get_value at0x80056064,size0xbc ignores command arguments. Convert
signed start/end words to single-precision floats; call
intplGetValue(mode,currentStep,steps,startFloat,endFloat). Truncate its float
result toward zero through fctiwz and store signed word LW(0).
Set LW(1)=1 iff signed currentStep<steps, otherwise0. Increment LW(14) as a word
**after** sampling and deciding continuation, even when already finished.
Return2. No self-clamping of step or automatic stop exists in this callback.

The Electric loop consequently samples indices0..16 inclusive:17 callback calls
and17 literal WAIT_FRAME(1) commands, absent external local/state changes. The
sample at16 still executes its wait before exiting the loop. These are derived
call counts; event timescale/scheduler and stalls determine actual elapsed time.

## Mode8 numerical path (B)

intplGetValue at0x80006c8c,size0x3b0 has many modes. Only its common preparation
and mode8 path are covered here; fingerprinting the whole routine does not
establish the other modes. If signed steps=0 return end immediately. Otherwise
convert currentStep t and steps n to binary32 and dispatch mode8 through table
entry US0x8030436c to0x80006de4.

The ordered binary32 operations in that branch are:

```text
q = (t*t)/n
q = PI32*q
r = (15.0f*n)/100.0f
angle = (4.0f*q)/r
c = roundToFloat(cos(angle))
a = (end-start)*c
b = n-t
num = b*a
num = b*num
den = n*n
result = end - num/den
```

Multiplications/subtractions/divisions are source single-precision instructions;
cos is called at0x80271ea8, then frsp rounds its result. No endpoint/easing
clamp occurs in this branch. PI32 is3.141592025756836 from0x8041f46c, not a host
higher-precision pi. Constants at0x8041f468,470,474 are4,15,100.
The cosine library's detailed numerical accuracy/error behavior remains open.
Do not fuse/reassociate these operations or replace mode8 with linear motion
when matching boundary truncation of displayed samples.

## Integer display-offset wrapper and sentinel (B)

btlevtcmd_SetDispOffset at0x80112978,size0x128 resolves/translates unit operand,
then resolves x/y/z through evtGetValue, converting the resulting signed words
to binary32. It does not use evtGetFloat for these three coordinates. Fetch
unit and call BtlUnit_SetDispOffset(unit,x,y,z), then return2.

BtlUnit_SetDispOffset at0x8012832c,size0x34 compares each coordinate against
binary32 sentinel−250000000 at0x804226c4. Equal coordinates preserve that axis;
otherwise write float x/y/z to unit+0x54/+0x58/+0x5c respectively. There is no
unit-null guard. Thus the Electric script overwrites y/z with0 on every sample
and x with its integer-truncated interpolation result. Source fcmpu unordered
values do not take the equality-preserve branch.

The display renderer's interpretation of those offsets and the wait animation's
later mutations still require separate contracts.

## Zero-gravity blocked-action script (B)

btldefaultevt_CantMoveZeroGravity is US0x8034f234,size0xb0. Its complete local
sequence is:

1. GetBodyId(−2,LW(0)).
2. AnimeChangePoseType(−2,LW(0),64).
3. evt_btl_camera_set_mode(0,7).
4. evt_btl_camera_set_homing_unit(0,−2,−1).
5. evt_btl_camera_set_moveSpeedLv(0,1).
6. evt_btl_camera_set_zoom(0,200).
7. AnnounceMessage(0,0,0,messageID,120), with pointer0x802ee8e0 to original ID
   `btl_msg_cant_move_zero_gravity`.
8. evt_btl_camera_set_mode(0,0).
9. WAIT_FRAME(20).
10. StartWaitEvent(−2), END_EVENT, END_SCRIPT terminator.

These are literal callback arguments, not yet a reconstructed camera zoom/
mode/announcement-duration API. USER_FUNC propagates callback completion/block/
yield results according to the event runtime; do not assume every call returns
immediately. Pose and wait wrappers are specified in
[recovery events](BATTLE_RECOVERY_EVENTS.md). No dialogue is paraphrased or
invented; original US message ID/data must be resolved by the message service.
There is no explicit StopWaitEvent, offset reset or resource mutation here.

## Command initialization and camera-speed gate (B)

BattleCommandInit at0x80125380,size0x18 writes word0 at supplied battle+0x171c,
then ORs0x80 into battle+0xef4. It does not create a menu or select an action
in this leaf routine. CommandInput and downstream field consumers remain open.

battleCameraGetPosMoveSpeed at0x8012c0e8,size0x40 uses **global** battle state.
Read floats x/y/z from+0x27bc/+0x27c0/+0x27c4. Compute binary32 y*y, then
fma(x,x,that), then fma(z,z,that). Call sqrt at0x802728d0 on the result, round
its return to binary32 and return the float. It does not read a caller-supplied
battle pointer or compute distance from actor to target.

The action controller waits while this returned speed is strictly greater than
2.0f, then advances. Camera vector producers and square-root library semantics
remain separate contracts; the helper name alone does not explain their units.

## Differential obligations and remaining work (P)

Compare samples0/15/16, continuation after the endpoint, local-word collision,
step0/steps0, negative/out-of-range inputs under original valid-call assumptions,
mode8 operation order and integer truncation, axis sentinel preservation,
final LW(10) wait target versus owner, literal20/1 waits under timescale, camera/
message completion before mode restoration, and speed threshold equality.
These are derived comparison obligations, not original execution captures.

Remaining: interpolation modes other than8 and cosine numeric contract, default
script callers, display-offset renderer/pose reset, camera callbacks/vector
producers, announcement message/control handling, full CommandInput and library
square-root behavior. The local script/service contracts add implementation
requirements, while full battle/game equivalence remains unfinished.

The four zero-gravity camera callbacks and their setters are now specified in
[camera requests](BATTLE_CAMERA_REQUESTS.md); camera update modes and message
callback semantics remain open.
