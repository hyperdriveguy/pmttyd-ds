# Original overworld camera collision target and secondary shift

US G8ME01 revision0, read-only DOL reconstruction. This closes the ordinary
Mario producers used by the camera-road query, separately from the curved-road,
transition and nonzero-shift geometry consumers. Static source evidence is not
an original execution trace or a numerical equivalence claim.

## Retained ground-contact target

Mario+f8/+fc/+100 is a retained XYZ contact point, separate from follow target
+ec/+f0/+f4 and live root+8c/+90/+94. `marioUpdateCamCollision`801a91fc takes a
pointer to three words: copies them unchanged into f8/fc/100 at801a9214–228,
then ORs displayFlags(+4) with0x40000 at801a922c–234. It has no existing-flag
admission test, smoothing, root substitution, or extra Y offset.

The ordinary producer is `marioSearchGround`8008e694. It passes the current
ray's **quantized point XYZ** from stack+40/+44/+48 at8008efa8–fc4 and
8008f01c–038. This follows sign-aware Q1000 rounding (8008e9dc–eb0c). The
point's X/Z can be a rotated peripheral foot probe, not Mario root X/Z.
In the ordinary support domain documented in
[the support contract](OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md), a strictly higher
admitted candidate writes the contact point at the same time as winning height,
node and output-angle selection. A later equal-height probe does not replace
it. A lower/distant ray that only writes outputDelta does not replace it.
A miss, a rejected slope, and a search with no admitted candidate preserve the
previous contact point. No source root fallback is performed here.

The two admission branches are:

- Lower point F<R with R-F>=2 and R-F<=abs(V): only F>currentBest admits and
  calls the writer (8008ef24–fc8). R-F>abs(V) only writes delta and angle.
- Remaining ordinary cases require abs(F-R)<=11, then F>currentBest admits
  and calls the writer (8008efd0–f03c).

The service can run multiple times within one Mario callback. Apply writes in
service order; do not treat display40000 as a first-write-only lock.
`marioCamMain` clears that bit at801a9368–370; it does **not** clear or rewrite
the retained XYZ. Motion29 skips marioUpdateCam but still reaches this tail.
The normal manager calls camera before Mario and EVT before Mario
([lifecycle/order contract](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md)), so
the next camera callback consumes the prior callback's retained contact.

`marioInit` zeroes all0x2f8 Mario bytes at8005b5f4–600. `marioReInit` also zeroes
all0x2f8 bytes at8005b2a4–2b0; its recovered local body does not restore this
triple. Thus the local initialization value is(0,0,0), until an actual writer.
Do not infer a first-frame contact without executing the applicable ground
query. Stores to+f8 in `marioStInit` are **GP** fields, not Mario fields.

Other direct DOL callers of this writer belong to mot_jabara, mot_plane,
mot_roll and yoshi_use, outside the plain Mario domain. This direct-branch census
does not exclude arbitrary event/runtime aliases; those remain separate owners.

Camera4 mode1 copies retained f8/fc/100 into camera+ac/+b0/+b4 at8000ed78–dac,
adding exactly1 engine unit to Y. It does not inspect display40000. Subsequent
event blend B combines this with event target into camera+a0/+a4/+a8. These
three fields supply road query arguments4–6 at8000ef8c–94; arguments1–3 come
from follow target camera+88/+8c/+90. The plain event-inactive target is therefore
contact+(0,1,0), not root+(0,1,0).

## Seventh road argument is a separate state machine

With camera flag4 enabled, cam3dMain calls `camShiftMain`800f3550 with(camera,
Mario, outputFloat*) at8000ef60–78. It then passes that output as argument7 to
camRoadMain8000ab48 at8000ef98–9c. This is not Mario depthShift+64 or sideShift
+13c. camRoadMain divides arguments1–6 by10 but preserves argument7.

`camRoadGetCurrentShiftXRate`8000ab20 returns roadWork+c4 if candidateState+68
is active; otherwise0. +c4 is candidateState+5c, copied from road record+7c.
Consequently **record+7c also supplies the shift target magnitude**. Reading
only the selected incoming record would miss this retained prior-candidate
ownership: camShiftMain runs before the current camRoadMain refreshes candidate.

Shift work is the pointer at r13-25944. Fields:

| Offset | Meaning in the recovered owner | Init/reset |
| --- | --- | --- |
| +0 | current exported shift C | 0 |
| +4 | desired shift T | 0 |
| +8 | acceleration blend B | 0 |
| +c | signed input accumulation I | 0 |
| +10 | acceleration enabled E | 0 |
| +18/+1c | previous callback GP timestamp | not written by init/reset |
| +20 | accumulation limit L | 300 |
| +24 | movement increment cap M | 3 |
| +28 | blend accumulation rate A | float word3b03126f (.002) |
| +2c/+30 | previous Mario rootX/rootZ | 0 |

`camShiftInit`800f3978 and `camShiftReset`800f34d0 perform identical listed
writes. `camShiftPostMain`800f3534 records GP+38/+3c into work+18/+1c after the
camera road/view selection, at8000f088, even when the ordinary road branch was
skipped. Timing for this owner comes from `getV60FPS`80005a00, not an invented
frame interpolation factor. For finite ordinary increasing timestamps its
returned increment is rate*60*(endMilliseconds-startMilliseconds)/1000.
The helper converts each source OS timestamp to integer milliseconds before
the difference; preserve that quantization when providing an equivalent clock.
Its wrap branch is outside this bounded ordinary increasing-time contract.

## Exact finite ordinary camShiftMain sequence

Let dt(rate) denote that source time increment from retained timestamp to
current GP+38/+3c. Read R=camRoadGetCurrentShiftXRate first.

1. If camera flags contain0x20: set B=1,T=0,E=1 and direction d=0. Otherwise
   read signed Mario+252; d=25 for positive input, -25 for negative, 0 for zero.
   If live rootX and rootZ both equal their retained work coordinates, force
   d=0. Store live rootX/Z into history (the disabled branch skips this write).
2. If d<0: I=dt(d) if oldI>0, otherwise I+=dt(d). If I<=-L, set E=1; if T>0
   reset B=0; set T=-R and I=-L. If d>0: I=dt(d) if oldI<0, otherwise
   I+=dt(d). If I>=L, set E=1; if T<0 reset B=0; set T=R and I=L.
   The d=0 branch preserves I/T at this stage. All comparisons use the values
   at this stage, including old T for the sign-change reset.
3. If E!=0: B+=dt(A), then clamp only the upper bound B<=1.
4. Let delta=T-C. If delta==0: clear B and E. Re-read fields, calculate
   delta=T-C and increment=B*delta. For positive delta<sourceepsilon .0001,
   use increment=delta instead; cap positive increment above M to M.
   For negative delta>-.0001, use increment=delta; cap negative increment
   below -M to -M. Constants are IEEE32 words38d1b717/b8d1b717.
5. If d!=0 OR camera flag0x20: C+=dt(increment); clamp an overshoot to T,
   using T's sign (negative T: C<T clamps; otherwise C>T clamps).
   Otherwise leave C unchanged and clear B=0; E is not cleared here.
6. Return C through outputFloat* at800f393c–944.

The clamp literally branches on **T**, loaded at800f3904 and compared to0 at
800f3908; the negative path compares C<T at3914–1c, nonnegative path C>T at
3928–30. It does not branch on direction or delta. Consequently camera20 with
T0,C>0,B1 can clamp to0 early for an elapsed multiplier.96, yet preserve a
negative overshoot for multiplier1.02. These16/17ms examples are derived finite
source-branch consequences, not an observed original frame trace.

The source increment cap applies before getV60FPS. A neutral-input callback
can preserve an already displaced C; it does not automatically approach zero.
Only the camera0x20 path sets T=0 and continues integrating toward it. The
ordinary root-unchanged test can likewise suspend C despite held horizontal
input. No transformation into the road's direction vector occurs in this leaf.

## Binding boundary and verification

A native ground-contact adapter must export the accepted ray **point** and
retain it on no write. Storing only winner height/normal/node loses necessary
XZ data. Initialization and reinitialization must be modeled separately from a
successful support query. A genuine seven-argument road input also needs the
shift work and original clock adapter; forcing argument7=0 is an explicitly
restricted solver domain, not complete ordinary moving-camera behavior.

The nonzero argument7 lookahead/inertia consumers, transitions, curved roads,
extra curves, event targets, special motion writers and source math/GX service
adaptation remain separate contracts. See
[straight-path bounded solver](OVERWORLD_CAMERA_STRAIGHT_PATHS.md) and
[road state selection](OVERWORLD_CAMERA_ROAD_STATE.md).

Run `python3 tools/verify_overworld_camera_target_shift.py`. The verifier pins
complete containing DOL bodies, relevant literal words and instruction anchors.
It checks source identity, including pinned literal bytes, not source execution.
