# Rogueport authored camera segment dispatch and general straight geometry

US G8ME01 revision0, read-only static source reconstruction. This extends
[the19 exact-cardinal straight records](OVERWORLD_CAMERA_STRAIGHT_PATHS.md)
without misclassifying tiny authored tangent differences as circular segments.
All44 gor_00–04 records contain six points plus six tangent/parameter rows.
All220 consecutive pairs take collisionCurve's straight branch after its
original near-normal collapse. A true differing-normal circular branch exists
in the engine but is **not required by these shipped pairs**. This does not
make degenerate zero-chord cases automatically defined.

## Actual dispatch order

For segment i, points Pi/Pj, raw horizontal tangents Ti/Tj and query Q:

1. Apply halfspaces using **raw** Ti/Tj: reject Ti·(Q-Pi)<0 or
   (-Tj)·(Q-Pj)<0 (8000c660–6cc). Equality admits. Do not replace those tangents
   with the chord or normalize them before this test.
2. Compute endpoint normals Ni=(-Ti.z,Ti.x), Nj=(-Tj.z,Tj.x).
   If BOTH abs(Ni.x-Nj.x)<epsilon AND abs(Ni.z-Nj.z)<epsilon, replace both
   locals with .5*(Ni+Nj) at8000c6d0–71c.
   Epsilon is **double representing float.01**: bytes3f847ae140000000 at
   802bf5a0, value.009999999776482582. It is not an invented epsilon and is
   distinct from the parameter-bound merge threshold.
3. Exact equality of both normal coordinates selects straight branch8000cf24
   (8000c720–72c). Otherwise the circular-normal-intersection branch starts
   8000c730. The collapse rewrites locals, not the authored rows.

Raw tiny tangent-coordinate differences in indoor records therefore do not
force the engine into circular interpolation. The source inventory verifier
checks all220 pairs' decoded raw floats and this collapse admission.

## Finite nonzero-chord straight branch

This contract includes all34 records without duplicate XZ points, even when
raw tangents are not exact cardinal or perfectly aligned with the chord.
Require finite inputs, a nonzero chord H=PjXZ-PiXZ and finite projection math.
The original endpoints/normals need not be replaced by idealized axis values.

After halfspace and straight admission, project Q orthogonally onto the
**infinite chord line** to obtain R. Source8000cf24–fa8 chooses a branch based
on abs(H.z)>abs(H.x), solves the line/normal intersection using f32 division
and fused negative multiply/subtract instructions, then reconstructs R.
This is mathematically the orthogonal projection; a host dot-product division
is a numerical adaptation. It does not clamp R to the segment first.

Fraction t is (R.x-Pi.x)/H.x when abs(H.x)>abs(H.z); otherwise if H.z!=0,
t=(R.z-Pi.z)/H.z. Source comparisons are strict. For noncardinal/nonaligned
raw tangents, the halfspaces do **not** imply t is necessarily within[0,1].
Source does not clamp t. Parameter U=Ui+t*(Uj-Ui). Retain these distinctions
when extending a native cardinal-only provider.

Let e=Q-R, distance=|e| and side=H.x*e.z-H.z*e.x. Reject distance>=currentBest;
otherwise write currentBest=distance before output caps. First best sentinel
is100000000, exact ties retain the earlier accepted segment.
Choose leftCap=record48 for side<0, otherwise rightCap=record4c. Compare
Euclidean distance to cap, with strict greater setting side flag2.

For ordinary record84!=0:

- U<lowBound: output parameter=lowBound, index=startIndex40, flag1; choose
  signedMagnitude=+min(distance,leftCap) if side<0, otherwise
  -min(distance,rightCap). Anchor=Pstart+(Tstart.z,-Tstart.x)*signedMagnitude.
  Output tangent is **raw Tstart**, not normalized or averaged.
- U>highBound: analogous authored end point/raw end tangent/index44/flag1.
- Exact boundary equality is uncapped by the end branch.

For an interior result (or record84==0), preserve R+e if within side cap.
Otherwise scale e by cap/distance, setting flag2, and output R+scaled(e).
Output tangent is **raw Ti+t*(Tj-Ti)** at8000d2c0–2e8; the dispatch collapse
is not used for this tangent interpolation. Output tangentY=0. No normalization
occurs inside this output branch; calcCamRoad separately normalizes its derived
camera direction. Parameter U and segment index i are retained.

All branches write pointY0, float output+8=Pi.y, flags, index and parameter;
output+4 is unwritten. The returned integer found is separate from Pi.y.
Bounds and point-index access remain as in
[the camera container contract](OVERWORLD_CAMERA_CONTAINER.md). Parameter
bounds merge when abs(Uend-Ustart)<doublefloat.0001 at8000c610–624; this changes
only lowBound, not physical endpoints.

## Duplicate-point dependency and bounded safe subset

Ten records have exactly one duplicated XZ segment, always segment2, with
Ui==Uj and Ti==Tj. Eight are exact constant-cardinal/collinear records:

gor_00 index1 C_kakusi; gor_02 indices4,5,6,7,10,11; gor_03 index1 curve7.
Exact full names/record offsets are in the evidence inventory. Two remaining
noncardinal duplicate records are gor_01 indices2 C_ie_01 and5 Cc_ie_04.

The zero-chord straight path **does not assign fraction FPR28**:
8000cfa4 gives the projection coefficient0; R becomes Pi;8000cfe0 jumps tocfec
when H.z==0, leaving FPR28 retained. This register is initialized only by a
prior admitted nonzero-chord segment's fraction stores cfc8/cfe8. If no such
segment ran, it is the callee-saved incoming FPR28. The function has no fraction
initialization for a newly started loop, nor a "skip duplicate" branch.
Do not invent t=0 as a full instruction-owner claim.

For the eight exact-cardinal/collinear records and finite query arithmetic,
a duplicate segment can pass both halfspaces only when Q lies on its shared
endpoint's perpendicular plane. The preceding nonzero segment also admits;
its projection is exactly that shared endpoint and sets a finite fraction1.
It wins distance before the duplicate. Duplicate R equals that point and has
an equal distance, so the strict distance comparison rejects the duplicate
before writing output. The finite retained fraction is therefore irrelevant
to that duplicate's output ownership. This real-number/cardinal proof permits
a bounded native adapter to ignore that duplicate **after preserving prior
segment evaluation and tie order**; it is not a source general skip rule.

The two noncardinal duplicate records need source arithmetic and incoming/prior
fraction provenance at exact-plane boundaries before a general skip or t=0
binding can be claimed. The near-normal collapse alone does not close that
zero-chord owner. Keep this dependency explicit. Negative/nonfinite caller
register values and source SDK exceptional arithmetic are outside the bounded
cardinal proof; no original execution is asserted.

## Inventory and verification

Run `python3 tools/verify_overworld_camera_segment_dispatch.py`. It pins the
source body, branch/field writes and both dispatch/bound epsilons, hashes the
five source containers read-only, and checks44 metadata rows/220 segment pairs,
34 nonduplicate records and10 duplicate records (eight bounded cardinal).
Raw containers, points and ISO assets are not committed. The metadata records
source identity/classification, not an invented camera scene.

This closes the ordinary shipped straight-dispatch geometry beyond the initial
19-record restriction. The engine's unused-in-this-slice circular-normal
branch, extra curves, two noncardinal degenerate boundary domains, source numeric
parity and original camera event lens/target services remain separate work.
