# Bero placement and front-contact lifetime

US G8ME01 revision0; bounded original source contract recovered2026-10-12.
Verify `python3 tools/verify_overworld_road_position.py`. Read
[shipped roads](OVERWORLD_ROGUEPORT_ROADS.md) and
[wall policy](OVERWORLD_WALL_MOVEMENT.md). Static byte pins establish source
ownership, not execution or native numerical equivalence.

## Original initial-position ray

`bero_get_position_normal` 80079b74/310 reads mode with evtGetValue and distance
with evtGetFloat, obtains the Bero record's integer XYZ and signed direction
index, and converts XYZ to single precision. If integer centerX is exactly
100000, it replaces the center with hitObjGetPos(hitName); otherwise the supplied
integer center is used. Distance equal to zero skips the direction/floor-offset
branch altogether and retains the two initially copied XYZ tuples.

For nonzero distance, let C be the resolved center; H=directionIndex*360*.125,
plus 180 if mode equals1, then radians H*twoPi/360. Constants are literal IEEE32
360, .125, 180, twoPi=0x40c90fdb and radius30. With u(H)=(sin(H),-cos(H)):

- Initial point P has XZ=C.XZ-30*u(H), and Y=C.Y. These source instructions use
  rounded sin/cos results and fused negative multiply-subtracts.
- Call hitCheckFilter (8001577c) with **NULL filter**, origin exactly P, direction
  (0,-1,0), input length1,000,000. There is no originY addition or quantization in
  this caller (80079d28–80079d78). Outputs point XYZ, length and normal XYZ are
  separate stack locations.
- If returned HitObj is nonnull, assign P.Y=raw hit-point Y (80079d84–88). If
  null, preserve C.Y; do not choose an invented default floor or require a hit.
- Copy P to final tuple F, then F.XZ=P.XZ+(30+distance)*u(H), preserving Y. The
  source re-evaluates sin/cos and uses fused multiply-adds for F.

`hitCheckFilter` passes the input length to its query+3c and calls
hitCheckVecFilter. A miss returns null at80015850 without writing caller output
pointers. A hit copies raw query point+24 and normal+30 to those pointers; the
Bero caller does not Q1000 either. For exact direction(0,-1,0), hitCheckVecFilter
uses its downward specialization. The runtime node's authored cull byte equal1
selects one-sided handling; otherwise it selects two-sided handling. In the
one-sided downward branch it rejects triangle normalY<=0 (80015b20–30),
equivalent to dot(normal,ray)>=0. Other cull values do not create a blanket
upward-normal requirement. NULL filter means there is no chkfilterVec attribute
predicate in this call; preserve the complete runtime hit-object query domain
and source triangle tests, including broad-phase/search ordering. Reusing a
filtered player footprint query would change this admission policy.

## Center and matrix ownership

`hitObjGetPos`8001449c/150 looks up a matching HitObj by name among map entries,
skipping runtime HitObj flag0x80; a missing/null name writes the literal zero
vector. It initializes sum/count, calls hitObjGetPosSub(hit,sum,count,0), and
returns sum*(1/count) using source vector scale.

`_hitEnt`8001896c/48c creates each HitObj's world matrix+0c from parent and
local transform. If source map node+5c has geometry parts, it stores the local
bbox midpoint .5*(sourceBBoxMin+sourceBBoxMax) at HitObj+9c (80018ad0–aec).
Otherwise it stores the source local translation there (80018af4–b08).
HitObj+9c is a local vector; HitObj+c0 is its separately computed world image.

For a leaf (no child+ d8), hitObjGetPosSub transforms that local+9c with its
world matrix+0c using PSMTXMultVec, adds the result to the sum and increments
count (8001488c–b4). A queried hierarchy averages the world images of its
**descendant leaf centers**, following child then sibling order; it does not
return the midpoint of the hierarchy's aggregate bounds. The top call has
sibling traversal flag0, so it excludes siblings of the requested root;
recursive calls use flag1 and visit sibling subtrees. A leaf's packed converted
world bbox-center is therefore a suitable numerical adaptation only after its
source identity and leaf status are checked. Do not reinterpret it as a universal
hierarchy-center service or omit event-driven runtime matrix changes.

## Ordinary contact lifetime is owned by marioMoveMain

`marioCheckWall`8008b790 does **not** clear Mario+1f0 on entry. It stores an
admitted pass-through forward hit there at8008b914, or a mature qualifying
blocking contact at8008b9b4; main movement source details are in the wall policy.
The staying helper has no direct +1f0 writer. A direct wrapper invocation alone
therefore does not imply a fresh front-contact cache.

For ordinary non-ship motion, `marioMoveMain`800594a8 first clears +1e4, +1e0,
+1f0 and +1f4 at80059500–510, before its movement gates and eventual moving or
staying wrapper call. Motion25 skips that block at800594e8. Thus the ordinary
per-move case contact starts null; a no-contact pass cannot reuse a preceding
move's road/front pointer. A cache must represent the current source pass and
remain available for later caseMain, which reads +1f0 at8006e61c.

Separate special-motion marioMain paths also clear these fields; this contract
bounds the ordinary Mario path and does not claim identical lifetime for ship,
roll, partner or transformations. The road caller must consume the specific
front+1f0 identity under its registered case/story/event gates; +1f4 and other
wall/support caches are different owners.
