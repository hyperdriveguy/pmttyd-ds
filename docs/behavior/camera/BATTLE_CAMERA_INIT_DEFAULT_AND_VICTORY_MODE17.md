# Battle camera initialization and ordinary victory mode17

2026-10-14. Read-only US G8ME01 disc0 revision0. Packed offsets and addresses
are hexadecimal; mode numbers in prose are decimal. This closes the initialized
default mode15 and ordinary ending mode17/18 path with finite source arithmetic,
with bounded default14/16 below, not all21 handlers or every retained
tremor/priority producer.
See [request setters](../battle/BATTLE_CAMERA_REQUESTS.md) and
[depth consumer/matrix writers](../display/BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md).

## Exact initializer

`battleCameraInit8012dee8:c8` captures global B through8041eb90, then calls
`memset800050b4(B+2754,0,104)` at8012df0c. This clears exactly2754..2857,
including flags/priority/mode/homing/zoom, retained eye/at, delta vectors and
shake timers. It does not clear the whole BattleWork or camera4 record.
Then, in order:

- Write flags B2754=0 again.
- Read Work=WORD(B2738), then signed WORD(Work+8). Type0 writes mode14 to
  B2760; types1..8 write15; type9 writes16; negative or>=10 retains the zero
  mode from memset. Exact branches8012df20..58.
- Write B275c=0; B2778=FLOAT8042273c=+0; HALF B277c=1 then B277e=1.
- Copy original vector802f0240 (three zero words) to B27b0/27b4/27b8.
- Write B2758=0. Return; no cameraGetPtr, view/projection build or actor query.

Thus retained eye B2780..88 and at B2798..a0 are actually zero after Init;
successful first Main creates a view target before camMain publishes a matrix.
Do not substitute an invented initialized view or source viewport for that
required first update. Native setup4 must supply actual Work+8 rather than
assuming a mode from the setup number itself.

## Ending request history and offset ABI

Source ending08, after EXP admission, calls speed setter8012b958(0,2), then
mode setter8012c028(0,17), or18 when Bef4 mask100 set, then position-offset
setter8012b198(priority0,f1=0,f2=0,f3=0). This ordering matters: accepted mode
clears zoom, sets both speeds1, clears B2758, performs separately gated
zero-priority axis-off calls and clears flags20. The subsequent accepted offset
sets flags20 again even though XYZ are zero. See the original
[EXP sequence](../battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md).

`btl_camera_set_posoffset8012b198:2c` checks signed priority against B275c.
Rejected returns without writes. Accepted ORs flags B2754 with20, then stores
f1/f2/f3 unchanged as binary32 to B27b0/27b4/27b8. It does not truncate,
reset axes, update framing or raise the priority threshold.
Event wrapper8012b108:90 resolves priority via evtGetValue80031260, XYZ in
order via evtGetFloat80030840, calls the setter and returns2. Offset manual-off
and axis setter contracts are distinct; accepted mode already calls the axis
helpers documented in BATTLE_CAMERA_REQUESTS, so no new axis defaults are
invented here.

## Main capture and selected handlers

`battleCameraMain8012c8b0:1638` first calls camGetPtr(4), captures global B,
requests AudienceBaseGetPtr801a86a0 and BattleGetMarioPtr800f87a8. It zeroes
six local deltas. If current mode!=3, writes B27d8=0. It copies retained B2780
eye/B2798 at into camera C+c/C18, then copies those camera values into old
snapshots B278c/B27a4. Its jump table80364940 has21 entries. Mode15→8012d8ac;
mode17/18 share8012d95c; unknown unsigned mode>20 skips handlers to common tail.
There is no Main-local active-battle, initialized-camera or valid-unit guard.

**Default15:** force both speed halfwords3. Request MoveTo with flags(r3=1,
r4=1), eye(0,110,500), at(0,60,0), six local output pointers. Speed3 performs
direct target-current delta and current+delta (single-rounded, not raw target
assignment), thus initializes the six axes in a stable ordinary domain.
The relative flag1 admits the MoveTo mirroring/position-offset operations below.
Then sprintf8026d5fc copies original diagnostic string802f0434 into a local
buffer; this is not story dialogue or a new persistent camera field.

**Default16:** same speed3/MoveTo(1,1) sequence, but requested eyeX and atX
are-100, eyeY110, eyeZ500, atY60, atZ0. Entry8012d904..948, diagnostic
string802f044c. This is a separate initialized Worktype9 case, not an alias15.

**Default14:** entry8012d7a8 increments B2758 with wrapping32-bit addition.
Only new counter==1 queries pouchGetPtr800d5eb0 and signed HALF(P+88),
selecting requested eyeZ: rank0→750,1→830,2→920,3→1000; all other ranks keep
the local initial+0. It forces both speeds3 and MoveTo(1,1) with eye(0,110,Z),
at(0,60,0). Calls2..9 do not request MoveTo; they retain camera vectors copied
from B at Main entry. New counter==10 clears B2758, calls accepted mode setter
(priority0,mode3), then btl_camera_set_moveto8012b760(priority0,duration120,
curve4,eye0/110/750,at0/60/0). This latter timeline admission/phase3 provider
is a genuine next branch; do not replace default14 with permanent mode15 or
continue counting14 through10 without those calls. Diagnostic sprintf follows
every14 invocation. The mode3 timeline remains outside this narrow contract.

**Victory17/18:** call GetMarioPtr800f87a8, then GetPartyPtr800f8728. For a
nonnull party call **BtlUnit_GetHomePos801289b0** on Mario then party; average
each axis with source single add then multiply.5. If no party use Mario home
alone. These are U30/34/38 home positions, not current root or rendered pose
position. Mode18 subtracts20 from the averaged Y; mode17 keeps it.
Then force both speed halfwords1 and call MoveTo(0,1) with:

```
eye = (homeCenterX, homeCenterY + 100, homeCenterZ + 350)
at  = (homeCenterX, homeCenterY + 50, 0)
```

All additions are single operations. Mode17 ignores the supplied offset because
MoveTo's first flag is0. It does not delete that offset or clear flags20.
The ending08 speed2 request consequently does not remain2 in this handler.
The local diagnostic sprintf uses string802f0464, after MoveTo.

## MoveTo used arithmetic and services

`battleCameraMoveTo8012c128:788` captures all inputs before calling
camGetPtr(4), captures fresh global B and calls GetMarioPtr(B), even when the
relative flag0 later bypasses its Mario reads. It sanitizes signed speed277c
outside[0,5) to1, and277e outside[0,6) to1. Note its first bound rejects5,
unlike the speed setter accepting0..5; preserve these separate gates.

Only first flag exactly1 admits the relative operations: if Bef4 mask100 set,
take Mario homeZ U38, add it to requested eye/atZ, and add(-homeZ)*.25 to
requested eye/atY. Then add B27b0..b8 to both requested vectors regardless of
flags20. Flag20's name does not authorize replacing this actual flag1 admission.
Victory first flag0 skips all these adjustments.

Always compute `extent=max(0,(600-targetEyeZ)*.5)` with source subtract,
multiply and ordered negative test. If second flag exactly1, clamp requested
eyeX and atX separately to[-100-extent,100+extent] using source ordered tests.
There is no matching Y clamp in this helper.

For speed1, update eyeX,eyeY,atX,atY in that order: delta=single
(target-current)/5, store to supplied output then reload it and single-add to
camera. Also set camera up C24/28/2c=(0,1,0). The separate Z-speed1 block
does the same for eyeZ then atZ. Speed3 uses direct target-current then addition;
first speed3 still sets the same up vector. No additional service call is made
inside these arithmetic blocks. Other admitted speed levels are real branches,
not covered by a speed1 substitute; unsupported modes retain their providers.

## Main common publication and conditional tail

After handler, if ordered camera eyeZ>=1000 set it1000 (8012dae4..f8).
Flags B2754 masks4/8/10 freeze X/Y/Z respectively: restore corresponding
old eye/at B278c/B27a4 axis and write both returned local deltas0. Then publish
camera eye back to B2780..88, at back to B2798..a0, local eye deltas to
B27bc/c0/c4 and at deltas B27c8/cc/d0. Flags20 does not gate these stores.

If flags1 and2 both clear call padRumbleOff80005f28(0). Independently flags40
set calls it again. Flags1 then admits horizontal tremor; flags2 admits vertical
tremor. Those branches interpolate timers/amplitudes through intplGetValue
80006c8c, temporarily use speed3, call MoveTo(0,0) on shifted eye/at, restore
both previous speed halfwords and decrement their timers; completed timers
clear their own flag. They occur AFTER retained vector/delta publication, so
shake changes camera eye/at used by camMain without overwriting those retained
baseline snapshots afterward. Fresh Init proves flags1/2 clear; later script
shake requests can invalidate that domain. Do not clear them merely to admit
ending, nor treat zero timers as proof the whole conditional graph is skipped.

## Native binding boundary and evidence

Implement Init plus already recovered accepted request writes against the real
packed Work and surviving player registry. Mode17/18 can use actual player
home getters and finite MoveTo speed1 path, preserved prior vectors and axis
flags. Exact eye/at at ending depends on prior camera Main cadence/handlers;
calling Init at ending would discard that history and is not source behavior.
Default15 first update and its initializer can seed a real provider at battle
startup; intervening camera modes must run or remain explicitly unsupported.
Matrix publication still follows cam3dMain projection then camMain LookAt,
roll/offset composition as described in the depth contract.

Camera C14c roll and C150/154/158 view translation are not written by these
Init/default/victory bodies or the used MoveTo blocks. They belong to camera
entry/load/event services. Likewise C30/34 lens and C38 FOV remain separate
live fields. Do not clear them in battleCameraInit, which only clears B2754.
The only C24/28/2c up publication in the used speed1/3 path is(0,1,0).
Projectiontype and projection-extra flag/matrix retain their own producers.

Top-level source frame order, already recorded in
[trio source controller evidence](../battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md),
runs animMain, lowercase battleMain800f904c, then camMain before
windowMain/mapMain/hitMain and evtmgrMain. Thus camera callbacks consume that
frame's battle-controller writes and previously retained EVT requests before
that frame's new EVT requests. Updating camera after each battle event or rendering callback would
invent extra smoothing steps and consume new requests early. A native fixed
simulation tick camera callback can be a declared cadence adapter until the
original top-level loop is integrated; repeated draw/depth queries must read its
published snapshot without advancing Main. Original hardware frame cadence has
not been observed here. The call-order evidence does not justify a measured
60Hz timing claim or bypassing camera mode3 history.

Ignored evidence: calcdepth-battleCameraInit/Main and endingcamera-MoveTo,
offset,offset-event listings/ELFs in artifacts/recovered-temp-evidence.
Complete hashes: Init794c80609e31845cf680d6a0734996cb5c71fce9d13fce3b88679ced210ba137;
Main1bc682b107a1dac6a1134f51d34cd2482ecec868c805424840d35e584c6c0edd;
MoveTof7cb2637a6cc8384ec147ed92c9cceb637d3a8eafa87ab154f52952134748d6b;
offsetb54c2359e91168b29a1351604bdc390ab551a141ec0d590e142ff29fc007c568;
offset-eventfadcfe2412982c2fb8dfdd44ab3a47fd4fd5cd3b3d9a07ed582d573ad8659d64.
Static source evidence only; no original execution, native runtime change,
shared documentation edit or commit.
