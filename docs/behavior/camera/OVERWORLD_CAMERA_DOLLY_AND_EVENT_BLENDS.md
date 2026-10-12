# Original ordinary camera dolly and event blends

US G8ME01 revision0, cam3dMain8000ea68 mode1 source reconstruction. This
composes with the retained follow/contact, authored road and transition
contracts; it does not replace their owners. Source bytes and finite derived
math are pinned; no original execution or numerical equivalence is claimed.

## Admission and retained state

Camera flags2 skips the entire callback at8000ea88/8c. Mode1 is ordinary field;
mode2/battle and other modes are separate domains. Camera flags10 suppresses
only the followY approach, not XZ, contact, timers, road or final blend.
Mario motion/recovery is not a whole camera-callback gate.

| Camera field | Meaning |
| --- | --- |
| halfword4, float8 | stateA, progressA |
| XYZ40/44/48,4c/50/54 | storedEye, storedAt |
| XYZ58/5c/60,64/68/6c | presetEye, presetAt |
| doubleword70/74,78/7c; byte80 | startTimeA,durationA,curveA |
| halfword82,float84 | stateB,progressB |
| XYZ88/8c/90,a0/a4/a8 | blendedFollow,blendedLook |
| XYZ94/98/9c,ac/b0/b4 | rawMarioFollow,rawMarioLook |
| XYZb8/bc/c0,c4/c8/cc | anchor,previousAnchor |
| doublewordd0/d4,d8/dc; bytee0 | startTimeB,durationB,curveB |
| floate4/e8/ec/f0 | dollyPercent,zeroStart,endPercent,previousPercent |

A and B timer updates run before constructing targets. Both use signed64
GP38/3c time minus their retained startTime, converted to float and divided
by signed64 duration converted to float (8000ead4–ed18). State0 retains progress.
State1 with positive duration sets p=1−elapsed/duration; only **p<0** clamps0
and clears state to0. State2/3 sets p=elapsed/duration; only **p>1** clamps1
and clears state to0. Exact endpoint equality retains state until the next
strict crossing. Nonpositive duration immediately sets state0 and p0 for
state1, p1 for2/3. No per-tick progress increment or render-count duration.

The newly updated state controls this same callback's later branch. In
particular stateA3 crossing into0 switches from preset candidate to the road
candidate while pA1 makes the final blend retain storedEye/storedAt.

## Target blend B, before the road query

Read prior Mario follow and contact through the earlier contract. Every
callback resets camerae8 to literal0 at8000ed6c–774. Let

    tB = intplGetValue(curveB, trunc(100000*progressB), 100000, 0, 1)
    iB = 1-tB

The truncation is fctiwz at8000edc4; the multiplier100000 is word47c35000
at8041f6c8. No extra clamp of interpolation output appears here.
For updated stateB0/1/2 (8000edf8–ee80):

    blendedFollow = rawMarioFollow*iB + anchor*tB
    blendedLook   = rawMarioLook*iB + anchor*tB
    dollyPercent  = zeroStart*iB + endPercent*tB

For stateB3 (8000ee88–eeec):

    blendedFollow = previousAnchor*iB + anchor*tB
    blendedLook   = blendedFollow
    dollyPercent  = previousPercent*iB + endPercent*tB

Other B states skip these stores; do not infer freshly zero outputs.
The look target remains a separate source input unless the stateB3 branch
explicitly makes it equal to follow. Applying B after road solving would select
the wrong zone/segment and miss its authored effect.

## Road candidate and post-road dolly

Build a local stored-view snapshot and literal local lens25/aspect1/far32768
at8000eef0–ef3c. Updated stateA0/1/2 selects:

- With camera flags4 set, camShiftMain at8000ef78 then camRoadMain at8000ef9c
  using blendedFollow XYZ, blendedLook XYZ and actual shift output argument7.
- With flags4 clear, the candidate is the stored view (8000f000–048).

After the road call, for each eye component only (8000efa0–eff8):

    candidateEye = roadEye + ((roadAt-roadEye)*dollyPercent)/100
    candidateAt  = roadAt

Multiply before divide, then add, matching source order. Divisor100 is
word42c80000 at8041f6d4. Dolly is a **percentage of eye-to-at distance**,
not an additive world-unit depth, follow offset or FOV change. No clamp to
0..100 occurs in this owner: negative values extend away, >100 cross the at.
At100 eye/at coincide; do not invent a source minimum distance here.

StateA3 bypasses the road/shift-main/dolly and uses presetEye/presetAt
(8000f04c–084). camShiftPostMain still runs afterward at8000f088 for all these
state branches. Unknown A states leave candidate locals outside this bounded
contract; native adapters should not silently invent a road candidate there.

## View blend A, after dolly

At8000f08c–144:

    tA = intplGetValue(curveA, trunc(100000*(1-progressA)),100000,0,1)
    iA = 1-tA
    finalAt  = storedAt*iA  + candidateAt*tA
    finalEye = storedEye*iA + candidateEye*tA

It stores finalAt to18/1c/20 and finalEye toc/10/14. Then VECDistance and
angleABf derive current view distance/yaw at8000f148–164; projection uses live
cameraFOV38/near30/far34. The local road metadata lens values do not replace
these live lens words. camMain subsequently constructs the view matrix and
additional retained screen/depth transforms, as documented in lifecycle.

## Fresh ordinary identity and map lifetime

camInit calls camEntryPersp for camera4 at80010404. camEntryPersp allocates
0x260 bytes and memset0 at8000fb40–54, which includes Bstate, Bprogress,
anchor and dolly fields. Fresh road load succeeds, enables flags4 and mode1,
then sets Astate0, Aprogress0, curveA11, curveB11 and Bprogress0 at
8000f418–428. It clears follow state and resets lensFOV25 separately.

Curve11 dispatch table entry8030434c+11*4 points to80006f78. It evaluates
start+(end−start)*(1−float(cos(sourcePi*step/steps)))*.5 at80006f78–fa0;
sourcePi word40490fd8 at8041f46c is3.141592025756836, .5 at8041f47c.
With finite default inputs, tB0 and tA1: blended targets retain their raw
Mario inputs, dolly0, and final view retains the complete road candidate.
The verifier checks these algebraic identity cases using explicit f32 cosine
rounding, not executing the shipped cos/Gekko body.

**camLoadRoad does not clear Bstate82, anchors, dollyEndec or previousPercentf0.**
It resets Bprogress0/curve11 but can therefore retain a stateB3 previous-anchor
branch across room history. The fresh identity conclusion requires actual
fresh/ordinary Bstate0 and no event writes; it is not a blanket reset rule on
restore/map change. Retain source global clock and event state.

## Bounded event producers and callback order

These EVT user functions return2 immediately after configuration; there is no
implicit wait for completion. They resolve integer event values and convert
millisecond duration to system ticks using hardware timebase word800000f8;
start times are current GP38/3c. Event manager runs after camMain, so a request
normally first affects the next camera callback.

- evt_cam3d_evt_set_xyz800a7d04 takes X,Y,Z,depthPercent,durationMs,curve.
  It snapshots current blendedFollow to previousAnchorc4/c8/cc, writes anchor
  b8/bc/c0 from the resolved XYZ, snapshots current dollye4 to previousPercentf0,
  writes requested depth toec, then sets Bstate3, start/duration, curveB
  (800a7dfc–ec8). It does not explicitly reset Bprogress in this leaf.
- evt_cam3d_evt_xyz_off800a7c04 takes durationMs,curve. It snapshots blendedFollow
  and current dolly, writes Bstate1/start/duration/curve at800a7c98–ce4.
  It retains anchor/endPercent. State1's target blend uses rawMario and anchor;
  the saved previousAnchor/Percent are not read by the state1 branch.
- evt_cam3d_evt_set800a89d4 takes eyeXYZ,atXYZ,durationMs,curve. It snapshots
  current actual eye/at to preset58..6c, writes requested view to stored40..54,
  then sets Astate3/start/duration/curve (800a8bb4–c6c).
- evt_cam3d_evt_off800a7f48 takes durationMs,curve; snapshots current eye/at
  to preset58..6c, retains stored40..54, and sets Astate1/start/duration/curve
  (800a7fa0–8000). Astate1 reads stored view and road candidate, not preset.

Other producers (relative/NPC/zoom/event-from-road/direct lens writes) exist;
see the source caller census. Their inputs cannot be inferred from a generic
native toggle or substituted with a fixed damping constant. This contract
closes the mode1 consumer and the named basic event leaves, not a full field
EVT adapter or unsupported interpolation callback modes.

## Validation

Run `python3 tools/verify_overworld_camera_event_blends.py`. Full source bodies,
branch/write anchors, interpolation dispatch and constants are pinned against
the read-only ISO. Derived finite cases cover fresh identities, dolly scaling,
strict timer boundaries and stateB3 target aliasing. libcos, fused operations,
Gekko float conversion and OS-time native adaptations remain numerical/service
boundaries; the checks are not original runtime tests.
