# Original Rogueport camera-zone triangle service

US G8ME01 revision0, static read-only DOL/container reconstruction. This extends
[road-state selection](OVERWORLD_CAMERA_ROAD_STATE.md) from horizontal triangles
to the actual shipped finite general geometry. All494 fan triangles are finite,
but234 are nonhorizontal and8 are exactly degenerate in3D. Some additional
triangles have nonzero3D normal but zero projected XZ area. A flat-height-only
selector cannot claim the full shipped room domain.

| Map | Fan triangles | Nonhorizontal | Exact zero3D area |
| --- | --- | --- | --- |
| gor_00 | 18 | 3 | 0 |
| gor_01 | 106 | 50 | 4 |
| gor_02 | 216 | 85 | 3 |
| gor_03 | 149 | 96 | 0 |
| gor_04 | 5 | 0 | 1 |

Most Y differences are small exported floating-point differences, but gor_00
zone0 polygon2 triangle1 has heights.5/.2000000477/.5 authored units. Preserve
authentic vertex data; flattening every triangle changes a genuine sloped zone.
Inventory counts include every zone, not only a guessed current player region.

## Original collisionTri_simple8000d3b4 interface

Arguments are(Q*,A*,B*,C*,bestHeight*), passed by calcCamRoad at8000b8a4–92c.
Q is the divided **look/contact target**, not player follow target. The three
vertices are polygon fan first, current, next. Polygon/zone indexing is defined
in [the camera container contract](OVERWORLD_CAMERA_CONTAINER.md).

For finite inputs/intermediates use these vectors in source order:

    e0 = A-B
    e1 = C-A
    e2 = B-C
    N = cross(e0,e2)
    D = (0,-1,0)
    S = dot(N,Q-A)
    V = dot(N,D) = -N.y

N is conventional cross(B-A,C-A). It is unnormalized. Source
cross components are constructed at8000d530–574; all-three-zero rejection
is8000d57c–5ac. D is literal vector00000000,bf800000,00000000 at802bf564.
No normal normalization, geometric epsilon, nearest-foot filter or slope angle
is used by this camera helper.

After zero-normal rejection:

- If S>0: reject when V>=0 (8000d670–6e0).
- Otherwise S<=0: reject when V<=0 (8000da20–84).

The first branch has cror(eq,gt,eq) and proceeds only on its **false** result;
the second has cror(eq,lt,eq) and likewise proceeds only when false. A sign
mistake here would reject the ordinary above-plane zone query. These comparisons
exclude V==0 in both branches, so vertical/projected-degenerate triangles
always reject before division, even if N itself is nonzero.

For the admitted branch compute, in source order:

    K0 = dot(cross(e1,D),Q-A)
    K1 = dot(cross(e0,D),Q-B)
    K2 = dot(cross(e2,D),Q-C)

If S>0, reject any K>0 (8000d7e0–ec,d8f8–904,da10–1c).
If S<=0, reject any K<0 (8000db84–90,dc9c–a8,ddb4–c0).
Zero is admitted, including edges/vertices. These tests are equivalent finite
XZ triangle containment with winding chosen by the branch, but source evaluates
full3D cross/dot with f32/FMA; a host barycentric/2D orientation test is an
explicit numerical adaptation and must preserve the source strict boundaries.

Then negate S at8000ddc8 and solve t=-S/V, hit=Q+t*D at8000ddc4–dec0. The relevant scalar is
hitY=Q.y+S/V. If hitY<*bestHeight reject, otherwise write hitY to bestHeight and
return1 (8000dec8–dee4). **Equal height is accepted**, so later admitted fan
triangles/roads can replace earlier selection. Any rejected branch returns0
and preserves bestHeight. The helper outputs no node/normal/contact point.

For an above-plane query, the finite test admits the downward intersection;
for a below-plane query it rejects the direction sign. Exact coplanar S0
follows the S<=0 branch and only admits V>0. Thus coplanar admission depends
on winding: conventional downward normal gives N.y<0,V>0 and can admit;
reversing the triangle gives V<0 and rejects. Do not invent unconditional
coplanar rejection or silently switch that branch to S>=0.

## Caller selection ownership

calcCamRoad scans authored roads, zones, polygons and fan triangles in source
order. Shared best height begins-100000000. On helper return1 it replaces
selected record at8000b930–93c; the helper already changed the shared height.
It does not select merely from a zone bbox or assume polygon height=A.y.
The zone-count0 fallback has its separate height<=lookY and strict height>best
rule, so its equal-height behavior differs from this fan service. Version101
record-disable gates and subsequent path-query failure/retained-history handling
remain the separate [road-state contract](OVERWORLD_CAMERA_ROAD_STATE.md).

In the ordinary finite domain a native implementation can reject N0 and Ny0,
solve the true sloped plane, preserve above/coplanar direction admission and
inclusive edges, and apply the >=height tie rule. Source-coordinate scaling
is unchanged: geometry and Q here are authored units (engine/10).

## Validation and boundary

Run `python3 tools/verify_overworld_camera_zone_triangles.py`. It pins complete
containing DOL bodies, all admission/return literals and the downward vector;
hashes five source containers; and checks494 source fan triangles' finite,
nonhorizontal, zero3D-area and zeroXZ-area classifications. Derived triangle
cases check reversed winding, coplanar asymmetry, sloped height, vertical/
zero-area rejection and equal-height admission. These are source reconstruction
checks, not original execution or Gekko numerical parity.

This closes the shipped finite general triangle domain. Nonfinite inputs or
floating-point overflow from arbitrary out-of-map queries, SDK/Gekko rounding
parity and source event camera services remain explicit numeric/runtime
boundaries. No authored vertices are distributed or committed as evidence.
