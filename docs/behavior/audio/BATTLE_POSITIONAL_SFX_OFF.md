# psndSFXOff

**Address:** `0x800d95f8`
**Size:** 200 bytes (50 instructions)
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow — **complete body**)

## Overview

`psndSFXOff(handle)` releases one positional-SFX slot: it looks up the
40-byte slot record, stops the underlying stream, song or effect according
to the slot's channel table entry, and clears the record's key and handle
words to `-1`.

Every branch and call site is documented below. This is not a partial
summary.

## Signature

| Register | Use |
|----------|-----|
| `r3` | slot handle (composite: low 8 bits select the slot) |

Return: void. Stack frame: 16 bytes (only `r31` is spilled).

## Level 1 — handle and slot selection (`0x800d9604`..`0x800d9628`)

```asm
800d9604: addis   0, 3, 1              ; r0 = handle + 0x10000
800d9608: cmplwi  0, 65535             ; == 0xFFFF ?
800d960c: stw     31, 12(1)
800d9610: bt      2, 0x800d96ac        ; handle == -1 -> return immediately
800d9614: clrlwi  0, 3, 24             ; slot = handle & 0xFF
800d9618: lis     3, -32706            ; 0x80030000
800d961c: mulli   4, 0, 40             ; slot * 40
800d9620: addi    0, 3, -1656          ; 0x80030000 - 0x678 = 0x8002f988
800d9624: add     31, 0, 4             ; r31 = 0x8002f988 + slot*40
```

**The `-1` sentinel is rejected before any table access.** The test is
written as `handle + 0x10000 == 0xFFFF` (unsigned), which is exactly
`handle == 0xFFFFFFFF`.

Slot records live at `0x8002f988`, **stride 40 bytes**, indexed by the
**low 8 bits** of the handle. The upper bits of the handle are ignored
here.

## Level 2 — slot liveness (`0x800d9628`..`0x800d9650`)

```asm
800d9628: lwz     4, 0(31)             ; slot.key
800d962c: addis   0, 4, 1
800d9630: cmplwi  0, 65535
800d9634: bt      2, 0x800d96ac        ; key == -1 -> already free, return
800d9638: lhz     0, 6(31)             ; slot.halfword6
800d963c: clrlwi. 0, 0, 31             ; test low 5 bits
800d9640: bt      2, 0x800d9654        ; nonzero -> stop the sound
800d9644: li      0, -1
800d9648: stw     0, 0(31)             ; slot.key   = -1
800d964c: stw     0, 24(31)            ; slot.handle = -1
800d9650: b       0x800d96ac
```

Two independent exits clear the record (`+0x00` key and `+0x18` handle both
to `-1`):

1. **`slot.key == -1`** — already free. Returns **without writing**, so a
   double-stop is a genuine no-op, not a re-clear.
2. **`(slot.halfword6 & 0x1F) == 0`** — live record but no channel bits
   set. Clears the record **without calling any stop service**.

Only the second case with nonzero low-5 bits reaches the stop dispatch.

## Level 3 — channel table and stop dispatch (`0x800d9654`..`0x800d96a0`)

```asm
800d9654: lwz     3, 24(31)            ; slot.handle
800d9658: clrlwi  5, 4, 19             ; (key & 0xFFF80000)
800d965c: lis     4, -32719            ; 0x80010000
800d9660: addis   0, 3, 1
800d9664: mulli   5, 5, 20             ; * 20
800d9668: addi    4, 4, 27432          ; 0x80010000 + 0x6b28 = 0x80016b28
800d966c: cmplwi  0, 65535
800d9670: add     4, 4, 5              ; r4 = 0x80016b28 + (key>>19)*20
800d9674: bt      2, 0x800d96a0        ; handle == -1 -> skip the stop
800d9678: lwz     4, 4(4)              ; table[k].word0
800d967c: rlwinm. 0, 4, 0, 0, 0        ; bit 0
800d9680: bt      2, 0x800d968c
800d9684: bl      0x800ddff8           ; SoundSSStopCh
800d9688: b       0x800d96a0
800d968c: rlwinm. 0, 4, 0, 1, 1        ; bit 1
800d9690: bt      2, 0x800d969c
800d9694: bl      0x800e1188           ; SoundSongStopCh
800d9698: b       0x800d96a0
800d969c: bl      0x800e0c38           ; SoundEfxStop
```

The channel table is at `0x80016b28`, **stride 20 bytes**, indexed by the
**top 13 bits of `slot.key`** (`key >> 19`, with the low 19 bits masked
off before the multiply — `clrlwi 5, 4, 19` then `mulli 5, 5, 20` is
exactly `(key >> 19) * 20`).

Dispatch on two bits of `table[k].word0`:

| bit0 | bit1 | Stop service | Address |
|------|------|--------------|---------|
| 0 | — | `SoundSSStopCh` | `0x800ddff8` |
| 1 | 0 | `SoundSongStopCh` | `0x800e1188` |
| 1 | 1 | `SoundEfxStop` | `0x800e0c38` |

There is a **second** `-1` guard here, on `slot.handle`. If the handle word
is `-1` the record is cleared but no stop service runs.

Finally, unconditionally after any stop dispatch:

```asm
800d96a0: li      0, -1
800d96a4: stw     0, 0(31)             ; slot.key    = -1
800d96a8: stw     0, 24(31)            ; slot.handle = -1
```

## Slot record layout (stride 40)

| Offset | Size | Role |
|--------|------|------|
| `+0x00` | 4 | sound key word; `-1` means free |
| `+0x06` | 2 | channel bits; **low 5 bits** decide whether a stop runs |
| `+0x18` | 4 | live handle; `-1` means no stop |
| `+0x1c` | 4 | position x (written by [`psndSFX_pos`](BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md)) |
| `+0x20` | 4 | position y |
| `+0x24` | 4 | position z |

## Data addresses

| Address | Role |
|---------|------|
| `0x8002f988` | slot record array base, stride 40 |
| `0x80016b28` | channel table base, stride 20, indexed by `key >> 19` |

## Porting notes

1. `handle & 0xFF` selects the slot but the full 32-bit handle is what gets
   `-1`-checked. Do not mask before the sentinel test.
2. The already-free path returns **without** re-clearing. A port that always
   clears will also write to `+0x18`, which is observably different when a
   caller inspects the record afterwards.
3. The `& 0x1F == 0` path clears without stopping. This is a real
   behaviour: a record can be occupied yet produce no sound.
4. The channel-table index is derived from the **key**, not the handle.
   Two slots sharing a key share a channel-table row.
5. Preserve the two-bit stop selection. Collapsing "bit0 clear" into
   "not a song" would misroute stream stops.

## Remaining gaps

- Writers of `slot.key`, `slot.halfword6` and `slot.handle` other than the
  start path in [`psndSFX_pos`](BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md);
  the `__psndSFXOn` provisioning chain that fills them is open under EVT-01.
- Layout of the 20-byte channel table rows beyond `word0`.
- Whether `SoundSSStopCh`/`SoundSongStopCh`/`SoundEfxStop` receive the
  table word as an implicit channel argument; their bodies are open.
- No original execution trace of a real positional-SFX stop was taken.