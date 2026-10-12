# Bounded authored straight camera paths

US G8ME01 revision0; recovered2026-10-12, source-only. Verify
`python3 tools/verify_overworld_camera_straight_paths.py`. This pins the complete
collisionCurve/calcCamRoad bodies, branch/data anchors and five authentic camera
containers, then checks the exact bounded classification of19 road records.
Read [container](OVERWORLD_CAMERA_CONTAINER.md),
[zone/state interface](OVERWORLD_CAMERA_ROAD_STATE.md) and
[lifecycle](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md).

## Domain and actual source records

The domain is mode0 collisionCurve with constant cardinal unit XZ tangent rows,
collinear point rows progressing strictly along that tangent (no duplicate or
reversed segments), start/end indices2/3, capflag+84=1, extra-curve count+b4=0.
The verifier derives that classification from raw source fields, not road names.
There are19 eligible records out of44. These include gor_01 C_mae/C_oku/
C_mae_02, gor_02 outdoor ground paths, gor_03 C_ground_ok/C_san_rozi/
C_ground_ma and gor_04 C_mae/C_hai. Exact index/offset/full name metadata is
retained in the evidence file. Repeated names are distinct record identities.

27 records have constant cardinal/collinear geometry, but8 contain duplicate
point rows and are **not** included in this nondegenerate19-record contract.
Other records require differing tangent/curved, duplicate-point, cap/mode or
additional-curve branches. An importer should preserve all44 authentic records;
this classification controls bounded solver eligibility, not extraction.

## Equal-tangent collisionCurve branch

Let a source road's first-half rows be points Pi, second-half rows be
(Ti.x,Ui,Ti.z), with H=count/2=6 in this slice. Let Q be divided player targetXZ,
T the constant cardinal tangent, N=(-T.z,T.x), lowIndex=2, highIndex=3,
leftCap=record+48, rightCap=record+4c. Begin/end parameter bounds are U2/U3.
Source compares abs(U3-U2) with double9.999999747e-5 and, if strictly less,
replaces the low bound with U3 (8000c610–624). Physical endpoints stay P2/P3.

The finite ordinary geometry of the source branch is:

1. Initialize found=false, bestDistance=100000000 authored units.
2. Scan segments i=0..H-2 in authored order. Admit a segment only if
   dot(T,Q-Pi)>=0 and dot(T,Q-P(i+1))<=0 (8000c660–6cc).
3. Equal endpoint tangentXZ takes branch8000cf24 (comparison8000c720–72c).
   Project Q orthogonally onto the segment's infinite XZ line, obtaining R and
   fraction t. In this domain the slab admission bounds t to[0,1]. Source chooses
   the dominant segment axis for fraction reconstruction; it does not add a
   native barycentric/endpoint tolerance or silently clamp t first.
4. Compute U=Ui+t*(U(i+1)-Ui) and d=distance(Q,R), using the source squared
   distance/square-root sequence. Reject d>=bestDistance (8000d0ec–f8).
   Otherwise set bestDistance=d **before** applying any output cap. Equal
   distances retain the earlier segment; shared endpoint identity therefore
   follows this strict tie rule rather than a name/index normalization.
5. Determine signed side s=dot(N,Q-R). If s<0 use leftCap; otherwise rightCap.
   Clamp only when d>that cap, setting output flag2 on a clamp. Equality does
   not set flag2. The cap compares Euclidean distance, not s before magnitude.
6. If U<lowBound, output U=lowBound, index=2, flags|=1 and anchorXZ=P2+N*s',
   where s' has Q-R's side and magnitude min(d,applicableSideCap).
   If U>highBound, similarly use U=highBound, index=3, flags|=1 and P3+N*s'.
   At exact bound equality use the non-endcap branch.
7. Otherwise output anchorXZ=R+(Q-R)*min(1,cap/d), with the d==0 domain handled
   by the source's zero-distance branch (no invented division). Output original
   U and segment index i. In real-number geometry this is R+N*s' in the bounded
   straight/cardinal domain, but source instruction rounding differs.
8. Continue scanning; return found as integer0/1. The closest-distance selection
   uses uncapped R/d, not distance to the capped final anchor.

The output field writes are U at+0, current segment start pointY at+8, anchor
at+c/+10/+14 with anchorY=0, tangent at+18/+1c/+20 with tangentY=0,
word index+24 and word flags+28. Function integer found and the float+8 are
**different owners**; +8 is not a Boolean validity flag. Field+4 has no write
in this bounded branch and must not be cleared by an invented struct reset.
Lower/upper end-cap sites are8000d108–1a8 and8000d1ac–24c; normal side/output
sites are8000d250–2f4. The source output index for a capped endpoint becomes
2 or3 rather than preserving the original extended segment index.

These equations are a finite-domain geometry reconstruction for native binding.
Source dominant-axis solve, f32/FMA, frsqrte refinement and field stores remain
numeric boundaries; a portable vector projection must be described as a
numerical adaptation, not instruction-for-instruction parity.

## Zero-shift camera-state composition

After actual zone selection and path success, calcCamRoad reads output tangentXZ,
constructs normalized horizontal direction D=(-T.z,0,T.x), and writes anchorXZ
from the curve result. AnchorY is the divided player targetY, except the source
version101 record+20 fixed-Y gate which instead uses record+24. It takes distance
from record+58, primary pitch from+64, secondary axis angle from+70; retained
float index token is the source output index converted to float. Curve flags are
preserved in the RoadState+8 owner.

For incoming seventh camera-shift argument exactly0, branch8000bd24 skips the
large look-ahead/inertia block to8000c2f0. With extra-curve count+b4=0, this bounded
zero-shift candidate can feed the final eye/at construction already documented.
This zero is **not** a claim every ordinary moving Mario has zero dynamic camera
shift. camShiftMain can supply nonzero values through ordinary motion history;
those owners and the skipped correction/inertia branches remain required.

camRoadMain additionally compares candidate/retained states, starts/restarts
transitions and invokes calcHokanCamRoad. This contract does not remove that
transition owner or make every selected record an immediate final view. Event
blending/dolly and original lens/projection remain separate lifecycle owners.
The static straight-path closure therefore supports a bounded actual authored
candidate, while full moving-room camera fidelity still requires those remaining
curve/shift/transition domains.

Source-derived cases: centerline gives uncapped anchor; left/right side-distance
inequality uses corresponding authored cap; exact cap does not setflag2; outside
parameter range caps to P2/P3 with flag1 and endpoint index; equality preserves
noncap/index tie rule; equal closest distances retain earlier segment; zero shift
skips look-ahead correction while preserving selected source record/fields.
These are derived acceptance cases, not recordings of original gameplay.
