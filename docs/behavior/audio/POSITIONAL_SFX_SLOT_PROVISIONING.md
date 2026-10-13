# `__psndSFXOn` — positional SFX slot provisioning

**Address:** `0x800d9934`
**Size:** 1344 bytes (336 instructions)
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow) — **decoded end-to-end; two items flagged open**

## Overview

`__psndSFXOn` is the internal start routine behind all three public entry
points. It resolves a sound key to a `pssfxlist` entry, allocates one of
the 40 `pssfx` slots, and populates the record — including packing the
handle that every accessor validates.

This closes the last structural gap in the positional-SFX family. The
recovery is end-to-end; two specific questions are left open and are
flagged in place rather than guessed at.

## ABI

Per [the start-wrapper contract](BATTLE_POSITIONAL_SFX_START_WRAPPERS.md):

| Register | Name | Captured to |
|----------|------|-------------|
| `r3` | key | `r26` (and CR0 via `mr.`) |
| `r4` | arg1 (volume) | `r20` |
| `r5` | arg2 | `r21` |
| `r6` | arg3 | `r22` |
| `r7` | arg4 (position pointer) | `r23` |
| `r8` | arg5 | `r24` |
| `r9` | arg6 | `r25` |
| `r10` | arg7 | `r17` |

Stack frame: 112 bytes. Working registers: `r19` = `pssfxlist` base,
`r18` = pssfxlist index, `r31` = masked index, `r28` = pssfxlist entry,
`r29` = chosen slot, `r27` = slot index.

## 1. Name resolution (`0x800d9974`..`0x800d99b0`)

```asm
800d9944: mr.     26, 3                ; r26 = key, sets CR0
800d9974: bt      0, 0x800d9980        ; key != 0 -> scan
800d9978: mr      18, 26               ; key == 0 -> index = key (0)
800d997c: b       0x800d99b4
800d9980: li      18, 0                ; scan from index 0
800d9984: lwz     3, 0(19)             ; pssfxlist[i].word0  (char*)
800d9988: mr      4, 26                ; the key
800d998c: bl      0x8026d464           ; strcmp
800d9990: cmpwi   3, 0
800d9994: bt      2, 0x800d99a8        ; match -> stop
800d9998: addi    18, 18, 1            ; i++
800d999c: addi    19, 19, 20           ; pssfxlist += 20
800d99a0: cmpwi   18, 4242
800d99a4: bt      0, 0x800d9984        ; loop
800d99a8: cmpwi   18, 4242
800d99ac: bt      2, 0x800d99b4        ; matched -> keep index
800d99b0: li      18, 0                ; exhausted -> index 0
```

The table is scanned with **`strcmp` against `pssfxlist[i].word0`**, stride
**20**, up to **4242 entries**. A key of `0` skips the scan entirely and
resolves to index `0`. A scan that runs off the end also resolves to index
`0` — **there is no "not found" failure**.

This is notable: the "key" argument is used as a **string pointer** here,
not as the packed word the accessors compare against `slot+0x00`.

## 2. Entry availability (`0x800d99b4`..`0x800d99d8`)

```asm
800d99b4: clrlwi  31, 18, 19           ; idx = index & 0x7FFFF
800d99b8: lis     3, -32719
800d99bc: mulli   4, 31, 20
800d99c0: addi    0, 3, 27432          ; 0x80316b28 = pssfxlist
800d99c4: add     28, 0, 4             ; entry = pssfxlist + idx*20
800d99c8: lwz     0, 4(28)             ; entry->word1
800d99cc: cmplwi  0, 0
800d99d0: bf      2, 0x800d99dc
800d99d4: li      3, -1                ; word1 == 0 -> return -1
```

`pssfxlist[idx].word1 == 0` means "no sound data" and is the **only**
content-based rejection at this stage.

## 3. Global gates (`0x800d99dc`..`0x800d9a6c`)

The global control block is at **`0x803df188`** = `pssfx` (`0x803df988`)
minus `0x800`. Reached as `lis -32706; addi -2328`.

### Mute gate (`0x800d99dc`)

```asm
800d99e4: lhz     0, 86(19)            ; global[+0x56]
800d99e8: rlwinm. 0, 0, 0, 29, 29      ; bit 29
800d99ec: bt      2, 0x800d9a00        ; clear -> continue
800d99f0: cmpwi   17, 0                ; arg7
800d99f4: bf      2, 0x800d9a00        ; arg7 == 0 -> continue
800d99f8: li      3, -1                ; bit29 set AND arg7 != 0 -> fail
```

Bit 29 of `global+0x56` suppresses playback, but **only** when `arg7` is
nonzero. Bit 29 is reused later as a slot-flag source.

### Special-key dispatch (`0x800d9a00`)

```asm
800d9a00: cmpwi   31, 91               ; idx <  91 ?
800d9a04: bt      2, 0x800d9a24
800d9a08: cmpwi   31, 2250             ; idx < 2250 ?
800d9a0c: bt      2, 0x800d9a24
800d9a10: addi    0, 31, -446          ; idx - 446
800d9a14: cmplwi  0, 1                 ; unsigned: 445 or 447 only
800d9a18: bf      1, 0x800d9a24
800d9a1c: cmpwi   31, 517
800d9a20: bt      2, 0x800d9a48        ; idx == 517 -> event path
```

Indices outside `< 91`, `>= 2250`, and not 445/447 take the common path at
`0x800d9a24`. Index **517** alone diverts to an event query at
`0x800d9a48`.

### Event gate, index 517 only (`0x800d9a48`)

```asm
800d9a48: lis     4, -2594             ; 0xF5DE0000
800d9a4c: li      3, 0
800d9a50: addi    4, 4, 384            ; 0xF5DE0180
800d9a54: bl      0x80031260           ; event value query
800d9a58: cmpwi   3, 2
800d9a5c: bt      1, 0x800d9a70        ; == 2 -> allowed
800d9a60: cmpwi   31, 869
800d9a64: bf      2, 0x800d9a70        ; not 869 -> allowed
800d9a68: li      3, -1                ; idx 869 and value != 2 -> fail
```

Index **869** is gated on an event flag at `0xF5DE0180` equalling `2`.

### Cooldown gate (`0x800d9a24`)

```asm
800d9a2c: lbz     0, 92(3)             ; global[+0x5c]
800d9a30: extsb.  0, 0                 ; signed
800d9a34: bt      2, 0x800d9a40        ; == 0 -> allowed
800d9a38: li      3, -1                ; nonzero -> fail
800d9a40: li      0, 6
800d9a44: stb     0, 92(3)             ; global[+0x5c] = 6
```

A **signed-byte cooldown of 6**. Nonzero rejects the start outright, so
this is a hard rate limit, not a retry. Nothing in this function decrements
it; that is `psndSFXMain`'s job.

## 4. Slot search, first pass (`0x800d9a70`..`0x800d9b50`)

Twenty slots are scanned (`li 0, 20; mtctr 0`), cursor `r17` starting at
`pssfx`, advancing by 40 each iteration.

A slot is a candidate only if **all** hold:

| Test | Instruction |
|------|-------------|
| `slot.key != -1` | `addis 0,6,1; cmplwi 0,65535; bt 2` |
| `(slot.key >> 19) == idx` | `clrlwi 0,6,19; cmplw 0,31; bf 2` |
| bit 23 of `slot.halfword6` clear | `lhz 0,6(4); rlwinm. 0,0,0,23,23; bt 2` |
| a preference value has been computed | `cmpwi 26,0; bf 2` |

The first candidate also performs the **preference bump**:

```asm
800d9ac4: lhz     6, 84(3)             ; global[+0x54]
800d9ac8: addi    0, 6, 8             ; += 8
800d9acc: sth     0, 84(3)            ; global[+0x54] = old + 8
800d9ad0: clrlwi  26, 0, 16           ; preference = low 16 bits
```

So `global+0x54` is a round-robin cursor advancing **8 per allocation**,
and its low 16 bits seed the slot search. On a candidate match:

```asm
800d9ae4: mr      29, 4               ; r29 = slot pointer
800d9ae8: mr      27, 5               ; r27 = slot index
```

Note the loop body is duplicated: the tail from `0x800d9ad4` repeats at
`0x800d9b30`. The copy differs only in reading `lwz 6, 0(4)` after `lwzu`
has already advanced the cursor, which is consistent with the unrolled
loop tail.

## 5. Slot search, second pass (`0x800d9b54`..`0x800d9c5c`)

If no same-key slot was free, **40** slots are scanned. This pass
**reclaims** slots rather than reusing them:

```asm
800d9b68: lwz     3, 0(17)            ; slot.key
800d9b6c: addis   0, 3, 1
800d9b70: cmplwi  0, 65535
800d9b74: bf      2, 0x800d9b84       ; key != -1 -> occupied
800d9b78: mr      29, 17              ; free slot -> take it
800d9b7c: mr      27, 30
800d9b80: b       0x800d9c50
```

An occupied slot (`key != -1`) is **stopped** — this block is an inlined
copy of the `psndSFXOff` body (`0x800d95f8`), including the same
`slot+0x06 & 0x1F` gate at `0x800d9bc8`, the same `key >> 19` table index
at `0x800d9be4`, the same two-bit `SoundSSStopCh` / `SoundSongStopCh` /
`SoundEfxStop` dispatch at `0x800d9c08`..`0x800d9c28`, and the same
`key = -1` / `handle = -1` clearing at `0x800d9c2c`.

It also enforces `lhz 3, 12(17)` (slot `+0x0c`) against
`lbz 0, 16(28)` (pssfxlist `+0x10`); a mismatch skips the slot
(`cmpw 3,0; bf 1`).

If the second pass exhausts all 40 slots, the function returns `-1` at
`0x800d9c58`. **Exhaustion is a real, reachable failure.**

## 6. Volume and arg2 clamps are dead code (`0x800d9c60`..`0x800d9c7c`)

```asm
800d9c60: clrlwi  0, 20, 24           ; arg1 & 0xFF
800d9c64: cmplwi  0, 255
800d9c68: bf      2, 0x800d9c70       ; <= 255 -> skip clamp
800d9c6c: li      20, 127             ; unreachable
800d9c70: clrlwi  0, 21, 24           ; arg2 & 0xFF
800d9c74: cmplwi  0, 255
800d9c78: bf      2, 0x800d9c80       ; <= 255 -> skip
800d9c7c: lbz     21, 13(28)          ; unreachable: read pssfxlist[+0x0d]
```

Both `clrlwi` results are in `0..255`, so `cmplwi ..., 255` is never
greater and the clamps never execute. `li 20, 127` and
`lbz 21, 13(28)` are **unreachable**. Recorded as observed, not fixed.

## 7. Slot flag assembly (`0x800d9c80`..`0x800d9d24`)

`slot+0x06` is built by OR-ing a base value with conditional bits:

```asm
800d9c80: li      0, 257              ; 0x101
800d9c84: sth     0, 6(29)
800d9c88: lhz     0, 86(19)           ; global[+0x56]
800d9c8c: rlwinm. 0, 0, 0, 27, 27     ; bit 27
800d9c90: bt      2, 0x800d9ca0
  slot[+0x06] |= 16                  ; 0x10
800d9ca0: rlwinm. 0, 18, 0, 5, 5     ; index bit 5
  slot[+0x06] |= 32                  ; 0x20
800d9cb4: idx == 1304 or 1858 -> slot[+0x06] |= 640   (0x280)
800d9cd0: lbz 0, 14(28) == 1 -> slot[+0x06] |= 512    (0x200)
800d9ce8: index bit 15 -> |= 1024  (0x400)
800d9cfc: index bit 14 -> |= 2048  (0x800)
800d9d10: index bit 13 -> |= 128   (0x080)
```

The **low 5 bits** of `slot+0x06` stay `1` from the base `0x101`, which is
what the accessors test. The index-bit tests mean the channel bits are a
**pure function of the pssfxlist index** — this is how the engine routes
similar sounds to different hardware channels.

## 8. Record population (`0x800d9d24`..`0x800d9e2c`)

```asm
800d9d24: stw     18, 0(29)           ; slot[+0x00] = pssfxlist index
800d9d34: stb     20, 8(29)           ; slot[+0x08] = arg1
800d9d44: lbz     8, 12(28)           ; pssfxlist[+0x0c]
800d9d58: stb     8, 9(29)            ; slot[+0x09]
800d9d60: stb     21, 10(29)          ; slot[+0x0a] = arg2
800d9d68: lbz     7, 17(28)           ; pssfxlist[+0x11]
800d9d9c: stw     7, 12(1)            ; staged
800d9dd4: lbz     0, 15(28)
800d9dd8: stb     0, 11(29)           ; slot[+0x0b] = pssfxlist[+0x0f]
8009d9dc: lbz     0, 16(28)
800d9de0: sth     0, 12(29)           ; slot[+0x0c] = pssfxlist[+0x10]
800d9de4: lha     0, 18(28)           ; SIGNED halfword
800d9de8: sth     0, 20(29)           ; slot[+0x14] = pssfxlist[+0x12]
800d9e2c: sth     25, 18(29)          ; slot[+0x12] = arg6
800d9e30: li      0, 32000
800d9e38: sth     0, 22(29)           ; slot[+0x16] = 32000
```

`lha` at `0x800d9de4` is a **signed** halfword load, so a negative
pssfxlist value propagates into `slot+0x14`.

### Distance computation (`0x800d9d40`..`0x800d9dd0`)

```asm
800d9d40: lfd     4, 15456(3)         ; double const, lis -32722 base 0x800E0000
800d9d5c: lfd     6, 15464(7)         ; double const 0x800E3C68
800d9d54: lfs     1, -23412(2)        ; float const (r2-relative)
800d9d64: lwz     3, -28496(13)       ; field record (0x80415fd0)
800d9d6c: lwz     3, 4(3)             ; field->position
800d9d74: xoris   3, 3, 32768         ; negate via bit flip
800d9d84: fsubs   0, 0, 4             ; scaleX - dblA
800d9d90: fdivs   0, 1, 0             ; fConst / (...)
800d9da8: fsubs   1, 2, 6
800d9dac: fsubs   2, 3, 4
800d9db0: fsubs   3, 5, 6
800d9db4: fdivs   0, 1, 0
800d9db8: fadds   0, 2, 0
800d9dbc: fadds   0, 3, 0
800d9dc0: fctiwz  0, 0
800d9dc4: stfd    0, 40(1)
800d9dc8: lwz     3, 44(1)
800d9dcc: sth     3, 14(29)           ; slot[+0x0e] = distance
800d9dd0: sth     0, 16(29)           ; slot[+0x10] = fctiwz result
```

Two derived quantities are stored: a truncated sum at `slot+0x0e` and the
raw `fctiwz` word at `slot+0x10`. The computation divides by
`fieldScaleX - doubleConst` and sums three differences — an inverse-scale
distance estimate. The two `r2`-relative and `lfd` constant values are
**not extracted**.

### Position copy (`0x800d9dec`..`0x800d9e2c`)

```asm
800d9dec: bt      2, 0x800d9e2c       ; arg4 (r23) == 0 -> skip
800d9df0: lwz     3, 0(23)
800d9df8: lwz     0, 4(23)
800d9dfc: stw     3, 28(29)           ; slot[+0x1c] = x
800d9e00: stw     0, 32(29)           ; slot[+0x20] = y
800d9e08: stw     0, 36(29)           ; slot[+0x24] = z
800d9e10: slot[+0x06] |= 2
800d9e20: slot[+0x06] |= 4            ; only when arg5 (r24) == 0
```

Bit 2 marks the positional-SFX route. Bit 4 is set only when `arg5 == 0`
(`cmplwi 24, 0; bf 2` skips it otherwise). The position is published at
provisioning time, which is why a later `psndSFX_pos` call is an update
rather than the first write.

## 9. Handle packing and return (`0x800d9e4c`..`0x800d9e5c`)

```asm
800d9e3c: addi    4, 3, -2328         ; 0x803df188 global
800d9e40: lhz     3, 80(4)            ; global[+0x50] channel counter
800d9e44: addi    0, 3, 1
800d9e48: sth     0, 80(4)            ; increment (post-increment value kept)
800d9e4c: sth     0, 4(29)            ; slot[+0x04] = (u16)counter
800d9e50: lhz     0, 80(4)
800d9e54: slwi    0, 0, 8
800d9e58: or      27, 27, 0
800d9e5c: mr      3, 27
```

```cpp
uint32_t channel = ++pssfxGlobal[0x50];   // 16-bit, wraps
slot->channel  = (uint16_t)channel;
return (slot_index & 0xFF) | (channel << 8);
```

This **confirms the handle layout** asserted in
[`psndSFXChk`](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md): bits 0..7 are the
slot index and bits 8..23 mirror `slot+0x04`.

One asymmetry to preserve: `slot+0x04` receives the **full 16-bit**
counter, but only `counter << 8` goes into the handle. Once the counter
exceeds `0xFFFF` the stored channel and the handle bits diverge, and
`psndSFXChk`'s 16-bit comparison of `(handle >> 8) & 0xFFFF` against
`slot+0x04` will no longer match. The counter is 16-bit, so this is a real
(remote) divergence, not a theoretical one.

## Consolidated slot record layout (stride 40, `pssfx` = 40 slots)

| Offset | Size | Written by | Source |
|--------|------|-----------|--------|
| `+0x00` | 4 | `__psndSFXOn` | pssfxlist index (`-1` on free) |
| `+0x04` | 2 | `__psndSFXOn` | channel counter, mirrored in handle bits 8..23 |
| `+0x06` | 2 | `__psndSFXOn` | base `0x101` plus conditional channel bits |
| `+0x08` | 1 | `__psndSFXOn` / `psndSFX_vol` | arg1, then runtime volume |
| `+0x09` | 1 | `__psndSFXOn` | pssfxlist `+0x0c` |
| `+0x0a` | 1 | `__psndSFXOn` | arg2 |
| `+0x0b` | 1 | `__psndSFXOn` | pssfxlist `+0x0f` |
| `+0x0c` | 2 | `__psndSFXOn` | pssfxlist `+0x10`; also compared on reclaim |
| `+0x0e` | 2 | `__psndSFXOn` | computed distance |
| `+0x10` | 2 | `__psndSFXOn` | `fctiwz` distance word |
| `+0x12` | 2 | `__psndSFXOn` / `psndSFX_dist` | arg6, then runtime distance |
| `+0x14` | 2 | `__psndSFXOn` / `psndSFX_pit` | pssfxlist `+0x12` (signed), then local pitch |
| `+0x16` | 2 | `__psndSFXOn` | literal `32000` |
| `+0x18` | 4 | `__psndSFXOn` / `psndSFXOff` | handle (`-1` on free) |
| `+0x1c` | 4 | `__psndSFXOn` / `psndSFX_pos` | position x |
| `+0x20` | 4 | `__psndSFXOn` / `psndSFX_pos` | position y |
| `+0x24` | 4 | `__psndSFXOn` / `psndSFX_pos` | position z |

## Global control block at `0x803df188`

| Offset | Size | Role |
|--------|------|------|
| `+0x50` | 2 | channel counter, incremented per start |
| `+0x54` | 2 | preference cursor, `+8` per same-key reuse |
| `+0x56` | 2 | flags: bit 27 forces `slot+0x06 \|= 16`; bit 29 is the mute gate |
| `+0x5c` | 1 | signed cooldown; set to `6` on start, rejects while nonzero |

## Porting notes

1. **Exhaustion returns `-1`.** With 40 slots and a reclaim pass, this is
   reachable under many simultaneous sounds.
2. **`pssfxGlobal[0x5c]` is never decremented here.** A port must run the
   decrement in the update function (`psndSFXMain`), or the cooldown of 6
   will stop all further starts permanently.
3. Reclaiming **stops** the evicted sound. Sound stealing is audible and
   must be preserved; do not silently overwrite.
4. The reclaim pass enforces `slot+0x0c == pssfxlist[+0x10]`, so slots are
   only stolen from a compatible entry.
5. Both volume clamps at `0x800d9c60` are unreachable. Do not "fix" them
   into live clamps — that would change behavior from the original.
6. The first-pass loop tail is duplicated in the binary; the duplication is
   benign and is recorded rather than normalised.
7. `slot+0x06` low 5 bits are always `1`. Any code that assumes they can
   be cleared post-allocation will be wrong.

## Remaining gaps

- **The `slot+0x00` semantics are inconsistent between the two families and
  this is unresolved.** Here `slot+0x00` is written as the **pssfxlist index**
  (0..4241). But [`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md) and
  [`psndSFX_pit`](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) derive their
  `pssfxlist` row from `(slot+0x00) >> 19`, which for a value of 0..4241 is
  always **0**. Either the accessors always resolve `pssfxlist[0]`, or the
  slot field is meant to be a packed key rather than the bare index. The
  bytes say `stw 18, 0(29)` unambiguously; the intended relationship is not
  established and is **not** guessed here. This should be resolved before
  relying on either side.
- The two `lfd` doubles at `0x800E3C60` / `0x800E3C68` and the `lfs` float at
  `-23412(r2)` used by the distance formula.
- The event flag at `0xF5DE0180` and the `strcmp`-based name resolution mean
  callers pass a **string**, but the accessors treat `slot+0x00` as a word.
  The two conventions coexisting is the same open item as above.
- `pssfxlist` entry fields beyond `+0x00`, `+0x0c`, `+0x0d` (dead),
  `+0x0f`, `+0x10`, `+0x11`, `+0x12` are not decoded.
- What decrements `global+0x5c`, and what decrements or expires `global+0x54`.
  Both live in `psndSFXMain` (2964 bytes), still undocumented.
- No original execution trace.