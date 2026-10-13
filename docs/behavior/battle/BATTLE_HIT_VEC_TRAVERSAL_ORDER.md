# Hit vector raycast traversal order

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers the traversal-ordering half of overworld request OW-WALL-01
(`docs/requests/OW-WALL-01.md`): full `hitCheckVecFilter`
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
- Loop counters differ only in register allocation, verified by init
  diff: region 1 uses counter r30 (from 0) with triangle pointer r31 (from
  [chunk+172], +84/iteration); regions 2–4 use counter r31 with pointer
  r30. Both compare the counter against the halfword count. An earlier
  revision misread region 1's init and claimed a pointer-size consequence;
  the inits (`lwz 31,172` / `li 30,0` at 0x800159b8–bc versus `lwz
  30,172` / `li 31,0` in the other regions) refute it.
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

## Region graph correction (verified edge census)

The "four sequential regions" framing above is refined by a machine-extracted
edge census (all branch targets resolved, fallthroughs included):

- Entry runs R1's scan loop (tops 159c4/15950). Chunk exhaustion drains to
  the tail (16518) via 15a68–15a74; R1 never falls through onward.
- R1-triangle-hit forwards out of the loop two ways: the 15a98 gate selects
  the R3 gateway (15e04) or R2 entry (15df8, via 15a9c). R1 does not resume
  after a forwarded hit — it scans to first admission, then hands off.
- R2 (outer 15aa0, triangle 15b14) drains to the tail on chunk exhaustion
  (15e00). Its triangle body contains no forward edge to R3; an earlier
  draft's "R2→R3 increment edge" was a misattribution — 15ea4→16150 lies
  inside R3 (triangle 15e88 → increment), i.e. R3's loop-continue when
  [tri+76] == 0.0.
- R3 (outer 15e14, triangle 15e88) is entered only through the
  15e04 → 15e10 → 16168 gateway chain (15e10 is fallthrough-reached, not
  branch-targeted). Its exits drain to the tail or its own outer loop.
- R4 (0x80016178–0x80016508) is statically unreachable: its only
  predecessors are its own back-edges (16514→16178, 16508→161ec); the
  would-be fallthrough at 16174 is itself dead behind 16170's unconditional
  tail branch, and no indirect jump targets the range. Its ~600 bytes
  duplicate R3's callback-gated scan shape with no live entry.
- Miss/empty paths at every stage converge on the tail (16518 hit-publish
  versus 16510/16528 miss returns per the protocol above).

Per-edge gate conditions and inter-region dataflow (what state R1-hit hands
to R2/R3) remain open; the edges above are control-flow facts, not a
dataflow contract.

## Region executability notes (verified, corrected)

- R4 (0x80016178–0x80016508) is statically unreachable: its only
  predecessors are its own back-edges, the fallthrough at 0x80016174 is
  dead behind 0x80016170's unconditional tail branch, and no indirect jump
  targets the range. An earlier draft's broader executability caveat is
  withdrawn below.
- Correction: regions do NOT differ in loop form. R1 allocates counter=r30
  (from 0) and triangle pointer=r31 (from [chunk+172]); regions 2–4 use
  counter=r31 and pointer=r30. Both compare the counter against the
  halfword count and stride the pointer 84/iteration. An earlier revision
  of this section misread R1's init (`lwz 31,172` / `li 30,0` at
  0x800159b8–bc) as the regions-2–4 shape and drew a false pointer-size
  consequence; re-reading the init blocks refutes it. No executability
  claim is made either way beyond R4.

## Not established

Per-call float arguments, triangle/chunk table ownership and lifetimes, the
r27 callback's identity contract, scene/native registry identity, numeric
equivalence of the vector math, and any gameplay effect. Those are exactly
OW-WALL-01's remaining triangle/traversal/registry items.

## Leaf: PSVECDotProduct (newly extracted)

`behavior/PSVECDotProduct.elf` (224 bytes) / `.disasm`, extracted for this
doc from `PSVECDotProduct@mtx.a` at 0x8029ec84, size 0x20 (the old
`behavior/PSVECdotProduct.disasm` placeholder is 0 bytes under a
case-mismatched name). Complete 9-instruction leaf, no calls, no frame:

```
psq_l f2, 4(r3) / psq_l f3, 4(r4) / ps_mul f2, f2, f3
psq_l f5, 0(r3) / psq_l f4, 0(r4) / ps_madd f3, f5, f4, f2
ps_sum0 f1, f3, f2, f2 / blr
```

Returns x1·x2 + y1·y2 + z1·z2 in f1 for the two vec3 pointers in r3/r4
(paired-single loads, fused multiply-adds, upper-sum reduction). This is
the exact operation behind all 13 `PSVECDotProduct` sites above; no
NaN/denormal special-casing exists in the leaf — exceptional behavior, if
any, comes from callers' gates.
