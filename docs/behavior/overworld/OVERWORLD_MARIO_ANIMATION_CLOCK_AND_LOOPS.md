# Original Mario animation clock and bounded loop owner

US G8ME01 revision0, animPoseSetAnim80041170, animPoseSetLocalTime8004130c,
animPoseMain8003ed2c and original front/rear ordinary asset metadata.
This is a bounded clock/loop contract, separate from
[pose request selection](OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md).
Full interpolation/update decoding remains open; source bodies are pinned,
not claimed executed.

## Clock and restart

marioPoseInit creates ordinary front/rear handles with type2. Both SetAnim and
Main use **GP38/3c** for nonzero type; only type0 uses GP40/44. Source uses
unsigned64 division by ((hardware word800000f8>>2)/1000) to obtain **integer
milliseconds of each whole clock timestamp**, before subtracting the retained
startMs. This differs from camera event blend timers, which ratio raw OS ticks.
SetAnim chooses this clock at80041244–b4; Main at8003ee08–80.

animPoseSetAnim looks up exact animation name in the selected file's64-byte
animation directory, falling back to animation index0 if not found. force0
and identical animation index return without reset at8004120c–1c; force1 from
MarioPreDisp resets even an identical index. It stores the new index, startMs,
last-key marker3c=-1, last time40=0 and loop/counter80/84=0 (800412b4–d4).

Main computes:

    frameTime = float(nowMs-startMs) * 60 / 1000
    frameTime *= pose44.localTimeRate

It stores frameTime atpose20 (8003ee80–eb8). Source constants60/1000 are
8041fba4/a8; rate is initialized1 by pose entry, but explicit service writers
can change it. No per-render-frame increment or stick-speed scaling is implied
by these ordinary motion handlers. Current native integer/simulation-time
bridges must state their timestamp quantization and pause lifetime adaptation.

SetLocalTime receives source **frames**, multiplies by float16.66666603088379
(word41855555 at8041fb40), converts that product to unsigned64 and subtracts
it from the integer-ms current clock (80041314–3c,b8–d4). It also stores the
result to88/8c and may propagate the startMs to the linked paper pose. Ordinary
Mario requests set start-frame28=0; special ChgPoseTime provides another value.
This service does not reinterpret the start-frame argument as milliseconds.

## Source loop descriptor and asset ranges

Selected animation metadata has relative pointer+24 to its loop descriptor,
relative pointer+28 to44-byte keyframe records and count+8. The loop descriptor
is (word loopFlag,float start,float end). Runtime relocation supplies the
pointers loaded at8003eebc and8003ef84–90. Original raw file offsets remain
animation-relative; native asset parsers must not confuse them with DOL VAs.

All five ordinary front/rear pairs have these exact descriptors:

| Pose pair | Loop flag | Start | End |
| --- | --- | --- | --- |
| M_S_1 / M_S_1R | 1 | 0 | 288 |
| M_W_1 / M_W_1R | 1 | 0 | 40 |
| M_R_1 / M_R_1R | 1 | 0 | 20 |
| M_J_1B / M_J_1BR | 0 | 0 | 2 |
| M_J_1C / M_J_1CR | 1 | 0 | 8 |

Let L=end-start. At8003eebc–ef7c:

- L==0: retained cycle84=frameTime-start; selectedTime=start.
- Loop flag nonzero: if frameTime-start<0, cycle84=0 and selectedTime=start;
  otherwise cycle84=(frameTime-start)/L, selectedTime=frameTime−trunc(cycle84)*L.
- Nonloop: before start use start/cycle0; after end use end and
  cycle84=(frameTime-start)/L; within inclusive range use frameTime and same ratio.

SelectedTime goes to pose24. Exact loop end wraps to start. Nonloop exact end
is retained; no unconditional modulo on jump pose. These operations are bounded
to finite ordinary descriptors above, not NaN/negative-length/custom streams.

Main scans44-byte keyframe time fields while keyTime<=selectedTime; records
last passed index=passedCount−1 and next=passedCount at8003ef84–fc8. It separately
scans8-byte auxiliary time records at8003efcc–f010. Nonloop next indices at/end
are bounded back to the last index (8003f014–044). Later body branches handle
loop selection, update streams, caching and interpolation.

## Boundary for animated asset baking

[The sparse update/interpolation contract](OVERWORLD_MARIO_ANIMATION_SPARSE_UPDATES.md)
now closes position/node interpolation, stepped texture state and source
instant-tangent admission for these ten ordinary front/rear poses. Other
special/effect/external animation domains remain open. The provided decomp's
animPoseMain is empty; its struct comments are not authoritative evidence.
Full body hashes alone do not establish algorithm closure.

Current tools/character_bake.py applies cumulative deltas at passed keyframes
and explicitly defers interpolation. Its nonzero-frame geometry is therefore
not a verified original fractional pose evaluator. Frame0 pose geometry can be
used with that converter limitation, preserving named model root/visibility,
but a multi-frame atlas must bind the recovered interpolation or clearly label
its sampling approximation before claiming animation fidelity. Noclip's AnimGroup.ts is a separate useful format/math reference,
not a replacement for shipped caller/update semantics.

Verify with the pose selector verifier. Its raw loop/key times and finite clock/
loop-derived cases are source-pinned; they do not claim libm/Gekko parity or
Dolphin-observed animation cadence.
