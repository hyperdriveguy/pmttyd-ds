# Mario camera shift targets, update gates and controls

US G8ME01 revision0; recovered2026-10-13 for
[OW-CAMERA-01](../../requests/OW-CAMERA-01.md) and
[MAINLOOP-01](../../requests/MAINLOOP-01.md). Complete local marioCamMain801a924c/
140hex, marioUpdateCam801a8f8c/270hex, camFollowYOff801a8eb4/40hex,
camFollowYOn801a8ef4/40hex, marioNoUpdateCamPos801a8f34/2chex and
marioUpdateCamPos801a8f60/2chex:290 instructions. Addresses/masks/fields hexadecimal;
counts/arguments decimal. Static deductions, not observed framing/cadence.
Read [field update graph](../overworld/MARIO_MAIN_COMPLETE_REQUEST_GRAPH.md).

## Three biased shift channels before the target gate

marioUpdateCam requests marioGetPtr8005e0c0 at801a8f9c, retaining M/r31 for its
whole body. Three channels execute in order, each independently loading floats:

| Target T | Current C | Rate R | First read |
| --- | --- | --- | --- |
| M138 | M13c | M150 | 801a8fa4 |
| M140 | M144 | M154 | 801a9018 |
| M148 | M14c | M158 | 801a908c |

For ordered C>T: single subtract(T,1.0), subtract(result,C), multiply(R,result),
add(C,result), store current. Reread stored current and target; if ordered new<T,
copy freshly read T into current. For ordered C<T use single add(1.0,T), then
subtract C, multiply R, add C, store; reread current/target; if ordered new>T,
copy T. Equal/unordered first comparison takes neither arm and does no store.
The bias1.0 bits3f800000 live80424ac4. Preserve separate single operations;
these are not a plain interpolation with a universal .07 coefficient. No local
rate/default/range/finite clamp. Ordered FP exceptions/FPSCR and concurrent
mutations remain domains, not host guarantees. NaN is not an equality fallback.

Only AFTER all three channels, read M.word4 MASK00010000. Set: exit without
camera/trig requests or target writes; channel updates already occurred.
Clear: request camGetPtr8000f2fc(4) TWICE at801a9110/18, without using the first
returned pointer locally. Keep both requests; identity/effects are child-owned.
Second returned camera.word114 float minus90.0(bits42b40000/80424ac8) becomes
f1 for sincosf80005e8c(stack+0c,stack+08) at801a9130. Name outputs A(stack+0c),
B(stack+08); angle units/output meaning/numeric approximation belong to sincosf.
Read M.word4 FRESH afterward; MASK00020000 selects target construction.

## Flag-dependent target construction, not room framing

MASK00020000 clear:

- Mec=singleFMA(A,M13c,M8c), Mf4=singleFMA(B,M13c,M94).
- Mf0=singleAdd(M144,singleAdd(M90,M9c)); both additions explicit, no FMA.
- Fresh M.motionU16+2e==25 adds10.0(bits41200000/80424acc) to freshly read Mf0.
- Copy current M14c to M64, then exit.

MASK00020000 set:

- Mec=singleFMA(A,M14c,singleFMA(A,M13c,M8c)).
- Mf4=singleFMA(B,M14c,singleFMA(B,M13c,M94)).
- Copy M14c to M64; do NOT write Mf0 or run motion25 height adjustment.

Reads occur separately in the original instruction order; do not algebraically
combine two FMAs or reuse a stable shared snapshot to erase later reads. These
are Mario-owned target fields, not direct view/projection-matrix writes. Room
limits, camMain consumers, authored camera events, actual yaw and default shift/
rate producers remain separate. No speed/window/deadzone or .07 literal here.
ABI32-byte frame/GPR31/LR; no independent scalar return inferred.

## Main update, zoom and expiration requests

marioCamMain first requests marioGetPtr at801a9260, retaining entry owner M0/r31.
If M0.motionU16+2e!=29 request marioUpdateCam at801a9274; motion29 skips that
child only. Always request marioCamZoomUpLevelMain801a86f0 at801a9278, then
marioGetPtr again at801a927c for timer owner M1, which can differ from M0.

Read M1.word4 once. MASK00100000 clear: clear MASK00200000 on M1.word4, skip
timer work. MASK00100000 set/MASK00200000 clear: skip timer. Both set:
read signed M1.half5c, subtract1, store low16, test signed resulting halfword.
Positive skips expiration; nonpositive stores half5c=0 and half60=0, rereads
word4 and clears BOTH00300000. Initially -32768 wraps to32767 and therefore
skips expiration; this is not a saturating timer.

On expiration, read signed half62; D=that*10 as word, N=D*60. Compute signed
high multiply(N,10624dd3), arithmetic >>6, then add logical sign correction;
store low16 result to M1.half5e. For this s16 domain this is truncation toward
zero of N/1000. No physical millisecond/second cadence is claimed. Store D to
stack+08, literal11 tostack+0c, pointer(stack+08) tostack+48. Request
evt_cam3d_evt_off800a7f48(stack+30,1) at801a9310. Only these local argument
writes are established; child operand/event semantics and return effects remain
open. No fabricated camera event or complete event-entry initialization inferred.

## Heading comparison uses a fresh owner; final clear uses entry owner

Always request camGetPtr(4) at801a9318 and retain C/r30, then marioGetPtr at
801a9320 for M2. If M2.float194 compares unequal/unordered to0.0
(bits00000000/80424aa4), compare C.floatc against M2.float160 with fcmpu:
equal sets M2.word4 MASK20; unequal/unordered clears MASK20. If M2.float194==0,
leave that bit untouched. Always copy C.floatc to M2.float160 afterward.
Finally read entry M0.word4, clear MASK00040000, store to M0. Do not redirect
this last clear to M1/M2 merely because their pointers commonly coincide.
No local pointer/null/generation/active validation. ABI496-byte frame/GPR30..31/
LR; no independent scalar result. Children can change pointer/flags/fields.

## Four complete control leaves

| Leaf | Ordered requests/writes |
| --- | --- |
| camFollowYOff | camGetPtr(4); OR camera.half0 MASK10; then marioGetPtr; OR Mario.word4 MASK00020000 |
| camFollowYOn | camGetPtr(4); clear camera.half0 MASK10; then marioGetPtr; clear Mario.word4 MASK00020000 |
| marioNoUpdateCamPos | marioGetPtr; OR Mario.word4 MASK00010000 |
| marioUpdateCamPos | marioGetPtr; clear Mario.word4 MASK00010000 |

Each request/write occurs even if bit already has the requested value. No
reference count, saved old state, nested ownership, owner-null guard or flag
result gate. FollowY is not a universal pause: its Mario bit selects horizontal
extra shift and leaves prior target Y; NoUpdateCamPos gates target creation
AFTER shift channel evolution. Camera consumers own the meaning of camera10.

## Evidence and remaining native work

[Manifest](../../../tools/mario_camera_update_evidence.json):15 spans/294 anchors
(all290 body words/four floats),17 direct calls, no indirect calls. Eight shift/
six timer/four target-gate/nine duration mathematical vectors, complete signed
halfword timer/duration domains, channel layout, masks and original source pins
checked independently. Generic pin checks do not validate all prose/runtime.
Original ELF/listings/generator stay `/tmp`; no raw bodies/assets committed.

OW-CAMERA-01 gains these complete local functions; whole room tracking/limits/
projection request stays OPEN. Native .07 smoothing must not be relabeled as
original; actual shift/rate/default producers, camera4 ownership/consumers,
zoom/expiration child/event control, yaw/trig/FP domains, pause/cadence and
original room/height/lock/unlock execution still need recovery/binding.
