# Original authored-road camera transitions

US G8ME01 revision0, static read-only DOL reconstruction. This closes
compareCamRoad, calcHokanCamRoad and their ordinary camRoadMain owner for finite
valid state/record inputs. Candidate production remains the separate
[road-state solver](OVERWORLD_CAMERA_ROAD_STATE.md), including its open curved
and nonzero-shift geometry. Source SDK math is a numerical dependency.

## Work/state ownership and initialization

Road work is r13-32632. Four96-byte camera states are:

| Work offset | Owner |
| --- | --- |
| +8 | current displayed state C |
| +68 | newly calculated candidate N |
| +d8 | retained destination/history D |
| +138 | fixed transition origin O |

Each state's+0 is active,+4 zone-selection word,+8 curveFlags,+24 record
pointer,+28/+2c/+30 anchor,+34/+38/+3c direction,+40 distance,+44 pitch,+48
secondary angle,+4c float segment token,+5c record7c parameter. These state
flags are not the camera flags in CameraEntry.

Other work fields: +c8 transition-active word,+cc anchor-blend word,+d0
progress,+d4 progress rate,+198 change mask,+1c8/+1cc prior transition timestamp.
camRoadSetup8000e104 clears **all0x210 bytes** at8000e120–124. This initializes
all states inactive, no transition, progress/rate/timestamps0. It additionally
sets comparison factors+1b4=2,+1b8=1,+1bc=1,+1c0=3,+1c4=1 at8000e194–1b8.
The constants are source floats; do not infer20 ticks from road record7c.

## compareCamRoad(A,B,rateOut)8000b270

Order matters: the leaf is asymmetric and uses A's distance for all angular
costs. B.active==0 returns0 immediately, without writing rateOut. In finite
valid-pointer inputs:

1. If record pointers differ, skip the same-record early gates. If equal,
   also skip these gates when A.zoneWord==1 and B.zoneWord==0, or when
   A.zoneWord==0 and **B.curveFlags==1 exactly**. Otherwise truncate both
   float segment tokens toward zero and let k=abs(trunc(A.segment)-
   trunc(B.segment)), span=A.record.endIndex44-startIndex40.
   k<=1 returns0; k>=span-1 returns0; otherwise k>=span-2 starts mask0x20.
   These early returns ignore changes in angles/distance/position.
2. OR mask1 for different record pointers, mask2 for A.zoneWord==1 and
   B.zoneWord==0, mask4 for A.zoneWord==0 and B.curveFlags==1 exactly,
   mask8 when abs(A.segment-B.segment)>double1 (802bf570). Segment comparison
   here uses the original float values, not their integer truncations.
3. Compute these costs, in the stated source order, retaining their maximum:
   - Direction angle: q=A.dirX*B.dirX+A.dirZ*B.dirZ, clamp only q>1 to1;
     angle=abs(acos(q)*float57.29578018). If angle>180 replace with360-angle.
     Cost=((floatTwoPi*A.distance)*angle/360)*work1b4.
   - Pitch angle: abs(A.pitch-B.pitch), if>180 replace with360-difference;
     cost=((floatTwoPi*A.distance)*difference/360)*work1b8.
   - Secondary angle: same single correction, cost factorwork1bc.
   - Distance: abs(A.distance-B.distance)*work1c0.
   floatTwoPi is word40c90fdb (8041f654), not twice the easing pi constant.
   These angle adjustments are single branches, not arbitrary modulo loops.
4. Independently compute translation t=sqrt(sum((A.anchor-B.anchor)^2)).
   If maximum angular/distance cost<.01 AND t<.3, return0 without rateOut write
   (8000b644–660), even if a mask bit was already set.
5. Include translation cost t*work1c4 in the maximum. Clamp that result
   **to20..90**, then write rateOut=1/result at8000b67c–6a8. If mask is still0,
   set mask0x10. Return the mask.

Source .01/.3 are IEEE32 words3c23d70a/3e99999a. The cost clamp is word41a00000
(20) and42b40000(90). Square root is the Gekko reciprocal-estimate/refinement
sequence; acos/trigonometry remain SDK dependencies. Do not clamp q<-1 in a
fidelity claim: source has no such branch. The bounded unit-vector domain
avoids that invalid-input case.

## camRoadMain update order8000ab48

First save incoming target/look coordinates in work1ec..200, divide all six
coordinates by10 and call calcCamRoad(N,D,...), preserving argument7.
Then perform the following owner sequence:

1. If transition-active!=0, call compare(D,N,&rate). If it returns a nonzero
   mask: store mask, transition-active=1, timestamp=currentGP+40/+44,
   progress=0; copy C to O (whole96 bytes), then N to D (whole96 bytes).
   This restarts from the **current blended view**, not the earlier origin.
   Recompute anchor-blend admission below. If comparison returns0, only
   copy N to D; retain O, progress, rate, admission and timestamp.
2. If transition-active==0 at this point, call compare(N,D,&rate). Nonzero
   starts a transition with O=D and D=N; same mask/active/time/progress writes
   and admission calculation. Zero does not start one.
3. If transition-active is now nonzero: progress+=getV60FPS(rate,
   priorTimestamp,currentGP+40/+44). Clamp only progress>1 to1. Replace prior
   timestamp with currentGP+40/+44. Call calcHokan(anchorBlend,O,D,N,C,progress).
   Replace **C.curveFlags(+8)** with anchorBlend?1:0 afterward.
4. If progress==1 exactly, copy D to C (all96 bytes) and clear active=0.
   Otherwise preserve current metadata fields not written by calcHokan.
5. If no transition is active, copy N to D, then D to C (all96 bytes).
6. Convert C through calcCurrentCamState, then multiply output eye/at XYZ
   by10. Lens metadata remains a separate owner.

This initialization intentionally displays the first candidate directly:
initial D.active0 makes compare(N,D) return0. Later transition ticks retain
C.active/record/target metadata until completion; calcHokan does not prefill
C from N. A zero comparison during an active transition still updates the
moving destination D without restarting, allowing its anchor to track.

The transition timestamp is **GP+40/+44**, not camShift's GP+38/+3c.
getV60FPS80005a00 converts each OS timestamp independently to integer
milliseconds, then returns rate*60*millisecondDifference/1000 in the ordinary
increasing-time domain. At exact1/60-second clock intervals the progress
increment is approximatelyrate, subject to source millisecond quantization.
A native simulation-clock adapter must preserve this ownership; using render
frames or an arbitrary fixed20-tick transition changes it. On a newly started
transition this same callback's start/end timestamp is equal, so its increment
is0. The rate is updated only when comparison reaches its rateOut store.

### Anchor-blend admission

After copying O/D on a start or restart, enable anchorBlend when any is true:

- D.curveFlags(+8)!=0 or O.curveFlags(+8)!=0;
- work.version==101 and either non-null state record has word+20!=0;
- either non-null state record has float+7c!=0.

Otherwise disable it. Literal checks are work+e0 and+140 at8000ac6c–080 and
8000adec–e00, both **state+8**, not the zone-selection word+4. This correction
is essential for ordinary start/end/side cap flags. Mask+198 is retained but
is not an input to this admission or the interpolation leaf.

## calcHokanCamRoad8000b038

Arguments are(anchorBlend,O,D,N,C,progress). N is passed in r6 but **unused**
by this body. It writes only C.anchor,C.direction,C.distance,C.pitch,
C.secondaryAngle,C.parameter5c. All other C fields are retained by the caller.

The finite source weight simplifies algebraically to

    w = min(1, float(0x3f000011) * (1 - cos(float(0x40490fd8)*progress)))

The scale is **.5000010132789612**, not exactly.5. Easing pi is
3.141592025756836 (8041f648); angle wrap pi elsewhere is
3.1415927410125732 (8041f658). This is an algebraic description, not a claim
that host rearranged arithmetic matches the original instruction rounding:
the source computes cos(0) twice, uses intermediate f32 rounding and fused
operations at8000b084–0e0. Only its upper weight bound is clamped.

If anchorBlend!=0, each anchor coordinate becomes O+w*(D-O), using fmadds.
Otherwise copy the current D.anchor directly. Direction uses
oldAngle=atan2(O.dirX,O.dirZ), newAngle=atan2(D.dirX,D.dirZ), then adjusts
**oldAngle** by repeated plus/minusfloatTwoPi until oldAngle-newAngle lies
inside inclusive[-floatPi,+floatPi]. Blend angle old+w*(new-old), then write
C.dirX=sin(angle), C.dirZ=cos(angle) (positive cosine). C.dirY is a scalar
linear blend. This is not the Mario sincosf negative-cosine heading helper.
Distance, pitch, secondary angle and parameter5c each use O+w*(D-O), with
**no** angle wrap on the scalar pitch/secondary interpolation.

## Verification and remaining owners

Run `python3 tools/verify_overworld_camera_transitions.py`. It pins complete
DOL bodies, relevant branch/copy instructions and decoded constant words.
Derived finite examples validate the documented early-return and mask rules;
they do not execute the original code. Unsupported candidate geometry must
remain an explicit dependency, not fabricated interpolation inputs. Nonzero
argument7 correction/inertia and curved/duplicate paths are still separate
research tasks. Original event camera blends, lenses and SDK numeric services
are outside this ordinary authored-road transition owner.

## Complete owner/timebase supplement (2026-10-13)

[Complete manager/time helper](FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md) closes all local
camRoadMain/getV60FPS words, fresh/captured copy owners and timestamp order.
The clock helper requests the signed __div2i entry; its decreasing returned-
quotient branch uses UINT64_MAX-P+C without adding1. The increasing finite
model above does not establish that wider domain. No-transition final copy
uses retained work for source and freshly loaded work for destination. These
source details supplement the stable-owner reconstruction above; authored
selection, library/provider domains and original visual execution remain open.
