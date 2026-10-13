# marioSlitChkWallAround

**Address:** `0x800882f4`
**Size:** 1500 bytes
**Namespace:** `mario_hit.o`
**Evidence class:** B (inspected US bytes/control flow)

> **Correction (2026-10-12, ID 3).** An earlier version of this document gave
> absolute addresses for the `r2`-relative `lfs` float constants (written as
> `0x800494xx`). Those were wrong: `lis` sign-extends its 16-bit immediate, and
> the arithmetic had been done without that extension. Every such address is now
> given in its authoritative `N(r2)` form, taken from the instruction bytes. The
> `lis`-derived data bases were also wrong and are corrected here — they now
> resolve to real symbols in `.data` (`0x80343b78` and `0x80343d88`).
> The three `chkfilterVec*` values are **code** addresses, not string literals.

## Overview

`marioSlitChkWallAround` probes the surroundings of the current Mario
position along a ring of 8 evenly spaced directions and decides whether
Mario is wedged in a narrow gap ("slit"). It is the enclosure test used by
the wall-crawl / slip-through family of field collision routines.

It takes no arguments; it works entirely off the global Mario pointer
returned by `marioGetPtr`.

## Structure

The function is three phases:

1. **Probe setup** — builds a vector-filter descriptor on the stack from
   Mario position fields and a static work descriptor.
2. **Direction sweep** — a loop of **8 iterations** that reads back one hit
   result per direction and stores a `0`/`1` flag into an 8-entry array on
   the stack (`r25` advancing by 4 bytes per iteration). The sweep is
   repeated a second time with a different origin offset.
3. **Triple-run verdict** — scans the collected flags and returns.

## Phase 2 — the 8-direction loop

Loop back-edge is at `0x800884a8` (`bt` to `0x80088370`), and the second
sweep's back-edge is at `0x80088790` (`bt` to `0x80088644`).

```asm
80088370: li      0, 0                  ; clear results[0]
80088374: fmr     1, 29                 ; angle base
80088378: stw     0, 0(25)              ; results[i] = 0
...
80088498: addi    24, 24, 1             ; i++
8008849c: fadds   29, 29, 31            ; angle += angleStep
800884a0: cmpwi   24, 8                 ; 8 iterations
800884a4: addi    25, 25, 4             ; results cursor += 4
800884a8: bt      0, 0x80088370          ; loop
```

`f31` is the loaded angle increment, so the eight probes are equally
spaced. The increment is one of the `lfs` constants at `-27540(r2)`
.. `-27520(r2)`.

Per-direction result classification (`0x80088448`..`0x80088494`) — three
ways to mark a direction as blocked:

| Check | Instruction | Meaning |
|-------|-------------|---------|
| bit 12 of a returned status word, **or** bit 7 of `mario[+0]` | `rlwinm. 0, 23, 0, 12, 12` / `rlwinm. 0, 0, 0, 7, 7` | solid hit |
| else bit 24 set → clear bit 0 of the status word | `rlwinm 0, 23, 0, 24, 24` then `rlwinm 0, 23, 9, 31, 31` | soft / angled hit, cleared |
| else compare `mario[+0x2e]` against 25 | `lhz 0, 46(3)` + `subfic/cntlzw/srwi` | falloff comparison |

The `subfic 0, 0, 25` / `cntlzw` / `srwi 0, 0, 5` idiom is a
branch-free `mario[+0x2e] == 25` test: it yields 1 only when the halfword
differs from 25 by enough to keep bit 23 clear after the leading-zero
count.

## Phase 3 — triple-run verdict (`0x800887a0`..`0x800888a0`)

Runs a 2-iteration loop over two 16-byte result groups on the stack
(`r30`, stride 16), with a 4-byte-stride side array indexed through `r4`:

```asm
800887a4: lwz     0, 0(30)             ; g[0]
800887a8: cmpwi   0, 0
800887ac: bf      2, 0x800887e0
800887b0: addi    0, 4, 1              ; side[1]
800887b4: rlwinm  0, 0, 2, 27, 29      ; &side[1] (byte offset)
800887b8: lwzx    0, 3, 0
800887bc: cmpwi   0, 0
800887c0: bf      2, 0x800887e0
800887c4: addi    0, 4, 2              ; side[2]
...
800887d8: li      3, 0                 ; return 0  (three in a row)
```

The same 3-in-a-row test is repeated at offsets `+0/+1/+2`,
`+1/+2/+3`, `+2/+3/+4`, and `+3/+4/+5`, then:

```asm
80088894: addi    30, 30, 16           ; next group
80088898: addi    4, 4, 4             ; next side array
8008889c: bdnz    0x800887a4
800888a0: li      3, 1                 ; return 1
```

**Return value: `0` if any three consecutive directional flags are set in
either group, otherwise `1`.** That is: three adjacent blocked directions
means Mario is enclosed on a side, so he is not in a traversable slit.

The `rlwinm 0, 0, 2, 27, 29` / `lwzx 3, 0, 0` pattern is a bounds-free
`side[i]` load — the index is biased into the array base so the mask keeps
only the scaled offset.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x8005e0c0` | `marioGetPtr` | many | fetch global Mario pointer |
| `0x800158a4` | `hitCheckVecFilter` | 3 | run the built vector filter |
| `0x80014310` | `hitGetAttr` | 3 | read filter attribute mask |

## Data addresses

| Address | Role |
|---------|------|
| `0x80343b78` | static work descriptor (`lis 3,-32724` + `addi 31,3,15224`) |
| `-27540(r2)`, `-27532(r2)`, `-27528(r2)`, `-27524(r2)`, `-27520(r2)` | `lfs` angle/probe constants |
| `0x80091434`, `0x80091504`, `0x800915e4` | filter-label **code addresses** — `chkfilterVecVivian`, `chkfilterVecRoll`, `chkfilterVec` — passed to `hitCheckVecFilter`; **not strings** |

## Porting notes

1. The verdict is a **pure function of the 8 direction flags** — the
   triple-run scan is the whole decision and is easy to reproduce exactly
   and host-test.
2. The angle sweep must stay at **8 evenly spaced directions**; the loop
   bound is a literal `cmpwi ..., 8`, not a table size.
3. The three-way per-direction classification is ordered: the solid-hit
   test is checked first, the bit-24 soft test second, and the
   `mario[+0x2e] == 25` test last. Reordering changes the flag matrix.
4. The second sweep re-reads `mario[+0x90]` (float) as its new origin
   base — it is not merely a repeat of the first.

## Remaining gaps

- Exact semantic names for the `lfs` constants at `-27540(r2)`..`80`
  (probe length, angle step, and origin offsets are not identified).
- Layout of the 16-byte result groups and the side arrays written by
  `hitCheckVecFilter`; the mapping from `side[i]` to a direction is not
  recovered.
- Which of `mario[+0x8c]`, `mario[+0x90]`, `mario[+0x94]` is position and
  which is radius/extent.
- Whether the return value feeds a wall-slide, a slip-through trigger, or
  both; callers of `marioSlitChkWallAround` are not yet audited.
- `hitCheckVecFilter` itself (3332 bytes) is undocumented.