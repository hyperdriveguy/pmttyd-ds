# Hit vector raycast traversal order

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the traversal-ordering half of overworld request OW-WALL-01
(`docs/OVERWORLD_DOCUMENTATION_REQUESTS.md`): full `hitCheckVecFilter`
(0x800158a4, 839-line listing, 49 `bl` + 4 indirect calls) region/loop/call
skeleton. Triangle-interior float math stays open (see end).

Fingerprint: entry 800158a4, `.elf` 3524 bytes. No JSON row. Symbol
`hitCheckVecFilter`, hitdrv.o, comes from `us-symbols`; orientation and
normalization paths inside are contracted in [support/slopes](../overworld/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md),
which explicitly leaves special-mode branches open. Call census below is
machine-extracted (49/49 targets resolved, all in `us-symbols`):

| Target | Identity | Sites |
| --- | --- | --- |
| 0x8002b438 | `mapGetWork` (mapdrv.o) | 1 (entry; args ignored) |
| 0x800153d0 | `checkTriVec_xz` (hitdrv.o, 940) | 1 |
| 0x8029ec28 | `PSVECSquareMag` | 1 |
| 0x8029ebc8 | `PSVECScale` | 5 |
| 0x8029eb80 | `PSVECAdd` | 2 |
| 0x8029eba4 | `PSVECSubtract` | 15 |
| 0x8029ec84 | `PSVECDotProduct` | 13 |
| 0x8029eca4 | `PSVECCrossProduct` | 6 |
| 0x8029eeb4 | `PSVECDistance` | 4 |
| 0x8029ebe4 | `PSVECNormalize` | 1 (final normal) |

SDA constants read from the ISO: −31752 = 0.0, −31736 = −1.0, −31740 = 1.0,
−31732 = 0.5.

## Entry protocol

r26 = query struct (r3), r27 = callback-or-zero (r4). `mapGetWork()` result
is kept but its args are not passed. `PSVECSquareMag(query+24)`; zero
magnitude → return 0 immediately (no stores). Best-hit running state lives
in f29/f28/f30 across regions.

## Four sequential regions

Each region scans mesh chunks (outer loop: next-chunk pointer at [chunk+224],
null chunk ends the region) and triangles within a chunk (inner loop: triangle
pointer advances 84/iteration from [chunk+172] with a halfword count at
[chunk+168]; regions 2–4 compare index r31 against the count, region 1
compares r30 per the assignment above). Backward edges, verified:
(80015a64→800159c4, 80015a70→80015950),
(80015df0→80015b14, 80015dfc→80015aa0),
(80016160→80015e88, 8001616c→80015e14),
(80016508→800161ec, 80016514→80016178).

- Region 1 (~800158e4): setup scales/offsets, then the loop. Entry gate for
  the region body below is the nonzero-magnitude test above.
- Regions 2–4 (~80015aa0, ~80015e14, ~80016178): each opens with the
  callback gate — `cmplwi r27,0`, taken-equal skips a `mtctr`/`bctrl` call
  shaped (query, chunk) with nonzero check on its return — then setup and
  the same loop skeleton.
- Loop-counter asymmetry, verified by prologue diff: regions 2–4 use
  counter r31 (0,1,2…) in `cmpw` against the halfword count with triangle
  pointer r30 advancing 84/iteration from [chunk+172]; region 1 instead
  compares r30 (from [chunk+172]+1, +1/iteration) against the count while
  the body reads triangles through r31 (84,168,… absolute). The region-1
  assignment is reported exactly as written; no chunk layout is inferred
  to explain it.
- Region 4 falls through to the tail (~80016518).

## Per-triangle order (identical skeleton ×4)

1. Skip unless flag word [query+0] is nonzero (taken-equal skips the
   triangle; the flag is written 1/0 by a preceding attribute/cull test in
   each region).
2. `PSVECDotProduct` filter call + `fcmpo`/`cror` NaN-safe gate; miss
   (`li 0,0`) rejoins the miss path.
3. `checkTriVec_xz(query+12-shape, triangle)`; zero result rejoins miss.
4. Chained `PSVECSubtract`/`PSVECCrossProduct`/`PSVECDotProduct` float gates
   against 0.0, each with its own miss exit to the region miss block.
5. On a pass: either the hit record ([query+60/36/40/44] floats plus
   [48/52/56] word copy from the triangle) or the staged stack copy is
   published; region miss blocks set r0 = 0 and continue the loop.

Float exceptional boundaries: every gate uses the `fcmpu`/`fcmpo` +
`cror 2,1,2` + `bf`/`bt` NaN-safe ≤/< pattern (unordered results take the
miss branch); exact sites are enumerable by that triple. No gate uses a bare
ordered comparison.

## Return protocol

All-miss tail (~80016518): null check result → `li 3,0`, clean 0 return.
Hit path (~80016528): copies the staged stack words into
[query+24/28/44/48/52/56], normalizes the saved nearest-hit normal with
`PSVECNormalize` into +30 (per the support doc), and falls to the epilogue
with r3 unstaged — the hit return value is whatever the last helper left,
not a constructed code. Callers test it against zero (as `checkLand2`'s
loop does: null → next probe).

## Not established

Per-call float arguments, triangle/chunk table ownership and lifetimes, the
r27 callback's identity contract, scene/native registry identity, numeric
equivalence of the vector math, and any gameplay effect. Those are exactly
OW-WALL-01's remaining triangle/traversal/registry items.
