# Positional SFX slot accessors

**Address:** `0x800d9558`, `0x800d95a0`, `0x800d96c0`, `0x800d9744`, `0x800d97d0`
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow — **all five bodies complete**)

## Overview

Five small accessors that resolve a handle to the 40-byte positional-SFX
slot record at `0x803df988` and read or write one field. Together with
[`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md) and
[`psndSFX_pos`](BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md) they complete
the accessor family; the only remaining unwritten slot fields belong to the
`__psndSFXOn` provisioning chain.

| Function | Address | Size | Access |
|----------|---------|------|--------|
| `psndSFX_get_vol` | `0x800d9558` | 72 | read volume |
| `psndSFXChk` | `0x800d95a0` | 88 | validate handle, return key overflow flag |
| `psndSFX_dist` | `0x800d96c0` | 56 | write distance |
| `psndSFX_pit` | `0x800d9744` | 140 | write pitch or call `SoundEfxSetPitch` |
| `psndSFX_vol` | `0x800d97d0` | 56 | write volume |

## The shared prologue

Every one of the five begins with the same two-step `-1` guard. First the
handle:

```asm
addis   0, 3, 1              ; r0 = handle + 0x10000
cmplwi  0, 65535             ; == 0xFFFF  ->  handle == -1
```

then, after resolving the slot, the key:

```asm
lwz     3, 0(slot)           ; slot.key
addis   0, 3, 1
cmplwi  0, 65535             ; == 0xFFFF  ->  key == -1
```

Slot resolution is always `slot = 0x803df988 + (handle & 0xFF) * 40`,
written as:

```asm
clrlwi  0, 3, 24             ; slot index = handle & 0xFF
lis     3, -32706            ; 0x803e0000
mulli   N, 0, 40
addi    0, 3, -1656          ; 0x803df988
add     N, 0, N
```

Three of the five (`psndSFX_get_vol`, `psndSFX_vol`, `psndSFX_dist`) are
**frameless leaves** that return via `bclr 12, 2` — a conditional branch to
the link register — and save nothing at all.

## `psndSFX_get_vol` — read volume (`+0x08`)

```asm
800d9558: addis   0, 3, 1
800d955c: cmplwi  0, 65535
800d9560: bf      2, 0x800d956c      ; handle != -1 -> continue
800d9564: li      3, 0               ; else return 0
800d9568: blr
...
800d9598: lbz     3, 8(4)            ; slot[+0x08]
800d959c: blr
```

Returns the **unsigned byte** at `slot+0x08`, or `0` for either `-1`
guard. Note the return is a byte widened to a word, not a signed value.

## `psndSFX_vol` — write volume (`+0x08`)

```asm
800d9800: stb     4, 8(5)           ; slot[+0x08] = (u8)volume
800d9804: blr
```

Single store, `stb`. The caller's value is **truncated to 8 bits** with no
clamping — `psndSFX_vol(handle, 300)` writes `44`. Both guards apply; the
function returns void via `bclr`.

## `psndSFX_dist` — write distance (`+0x12`)

```asm
800d96f0: sth     4, 18(5)           ; slot[+0x12] = (u16)distance
800d96f4: blr
```

Same shape as `psndSFX_vol`, halfword store, truncation to 16 bits.

## `psndSFXChk` — validate and return the key overflow flag

The only accessor with a **third** guard and a real return value.

```asm
800d95c0: rlwinm  6, 3, 24, 16, 31  ; (handle >> 8) & 0xFFFF
800d95cc: lhz     0, 4(3)           ; slot[+0x04] channel halfword
800d95d0: cmplw   6, 0
800d95d4: bt      2, 0x800d95e0      ; equal -> continue
800d95d8: li      3, -1
800d95dc: blr
800d95e0: lwz     4, 0(3)           ; slot.key
800d95e4: addi    3, 4, 1           ; r3 = key + 1
800d95e8: subfic  0, 4, -1          ; r0 = -1 - key  ==  ~key
800d95ec: nor     0, 3, 0           ; r0 = ~(key + 1)
800d95f0: srawi   3, 0, 31          ; r3 = (~(key+1)) >> 31
800d95f4: blr
```

This validates that **bits 8..23 of the handle equal the halfword at
`slot+0x04`**. A mismatch means the handle does not belong to this slot's
channel allocation, and the function returns `-1`.

The handle therefore is not a bare slot index — it packs at least three
fields:

| Handle bits | Meaning |
|-------------|---------|
| 0..7 | slot index (`0x803df988 + index*40`) |
| 8..23 | channel allocation, mirrored at `slot+0x04` |
| 24..31 | ignored by every accessor in this family |

The final sequence is a branch-free "is `key + 1` nonzero" test:
`srawi((~(key+1)), 31)` equals `(key + 1) >> 31` arithmetic, which is `0`
for `key` in `[-1, 0x7FFFFFFE]` and `1` only for `key == 0x7FFFFFFF`. For
real sound keys the result is `0`.

So `psndSFXChk` returns **`-1` for a bad handle or a dead slot**, and `0`
for a live one. This is the validation primitive the other consumers use.

## `psndSFX_pit` — pitch, with an engine handoff (`+0x14`)

```asm
800d9780: lhz     0, 6(6)           ; slot[+0x06]
800d9784: clrlwi. 0, 0, 31          ; test low 5 bits
800d9788: bt      2, 0x800d9794      ; nonzero -> engine path
800d978c: sth     4, 20(6)          ; slot[+0x14] = (u16)pitch
800d9790: b       0x800d97c0
800d9794: clrlwi  0, 3, 19          ; (key >> 19)
800d9798: lis     3, -32719         ; 0x80310000
800d979c: mulli   5, 0, 20
800d97a0: addi    0, 3, 27432       ; 0x80316b28
800d97a4: add     3, 0, 5
800d97a8: lwz     0, 4(3)           ; channelTable[key>>19].word1
800d97ac: rlwinm. 0, 0, 0, 0, 0     ; bit 0
800d97b0: bf      2, 0x800d97c0
800d97b4: bf      2, 0x800d97c0
800d97b8: lwz     3, 24(6)          ; slot[+0x18] handle
800d97bc: bl      0x800e0bbc        ; SoundEfxSetPitch
```

Two mutually exclusive pitch routes, chosen by the **low 5 bits of
`slot+0x06`** — the same field that decides whether
[`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md) stops anything:

- **low-5 clear** — a slot with no channel bits. The pitch is stored
  locally at `slot+0x14` and **no engine call is made**. This is the
  software-only slot case.
- **low-5 set** — a live channel. The pitch is *not* stored in the record;
  it is handed to `SoundEfxSetPitch` using the handle at `slot+0x18`.

The channel table is the same `0x80316b28`, stride 20, indexed by
`key >> 19` used by `psndSFXOff`, but this function reads **`word1`**
(offset `+0x04`) where `psndSFXOff` reads **`word0`** (offset `+0x00`).
Bit 0 of `word1` is tested; if clear, the function returns without setting
pitch.

### Observed duplicate branch

`0x800d97b0` and `0x800d97b4` are two consecutive `bf 2` to the same
target (`0x800d97c0`), differing only in the 16-bit displacement field.
This redundancy is present in the US binary bytes. It is recorded rather
than explained: no original-execution observation distinguishes the two
encodings, and a port should not "clean it up" without a reason.

## Consolidated slot record layout (stride 40, base `0x803df988` (symbol **`pssfx`**, `.bss`))

| Offset | Size | Field | Written by |
|--------|------|-------|------------|
| `+0x00` | 4 | key word (`-1` = free) | `__psndSFXOn`; cleared to `-1` by `psndSFXOff` |
| `+0x04` | 2 | channel allocation, mirrored in handle bits 8..23 | `__psndSFXOn` |
| `+0x06` | 2 | channel bits; **low 5** select local vs engine route | `__psndSFXOn` |
| `+0x08` | 1 | volume | `psndSFX_vol` (write), `psndSFX_get_vol` (read) |
| `+0x12` | 2 | distance | `psndSFX_dist` |
| `+0x14` | 2 | pitch, **local-route only** | `psndSFX_pit` |
| `+0x18` | 4 | live handle (`-1` = none) | `__psndSFXOn`; cleared by `psndSFXOff` |
| `+0x1c` | 4 | position x | `psndSFX_pos` |
| `+0x20` | 4 | position y | `psndSFX_pos` |
| `+0x24` | 4 | position z | `psndSFX_pos` |

## Handle encoding

```cpp
// derived from psndSFXChk 0x800d95a0 and the shared prologue
uint32_t handle;
uint8_t  slot_index  = handle & 0xFF;
uint16_t channel     = (handle >> 8) & 0xFFFF;   // must equal slot[+0x04]
// bits 24..31 unused by this accessor family
```

## Porting notes

1. `psndSFX_vol` and `psndSFX_dist` **truncate silently** — byte and
   halfword stores with no clamp. A port that clamps changes behaviour for
   out-of-range callers.
2. `psndSFX_pit` chooses between a local store and an engine call using the
   low 5 bits of `+0x06`, the same gate `psndSFXOff` uses. Keeping the two
   consistent matters: a slot that `psndSFXOff` will not stop is also a slot
   whose pitch is never sent to the engine.
3. `psndSFXChk`'s `slot+0x04` comparison is the only accessor that reads
   handle bits 8..23. Any handle produced elsewhere must pack them
   consistently or every check fails with `-1`.
4. The three frameless leaves save nothing. In C, do not rely on
   callee-saved registers for `psndSFX_pos`/`psndSFX_vol`/`psndSFX_dist`
   inputs across the call.

## Remaining gaps

- Who **reads** `slot+0x08` (volume), `+0x12` (distance) and `+0x14`
  (local pitch). Those consumers are inside `psndSFXMain` (2964 bytes),
  still undocumented.
- Writers of `slot+0x00`, `+0x04`, `+0x06`, `+0x18`: the `__psndSFXOn`
  provisioning chain.
- `SoundEfxSetPitch` (`0x800e0bbc`, 124 bytes) argument interpretation.
- Layout of channel-table `word1` beyond bit 0.
- No original execution trace of any accessor.