# BtlUnit_snd_se_pos

**Address:** `0x80126088`
**Size:** 276 bytes (69 instructions)
**Namespace:** `battle_unit.o`
**Evidence class:** B (inspected US bytes/control flow — **complete body**)

## Overview

`BtlUnit_snd_se_pos` is the battle-unit wrapper that starts a positional
3D sound effect. It derives a pitch/variation id from two unit status bits
and a random draw, starts the sound with `psndSFXOn_3D`, applies a pitch
adjustment, and chooses a volume — either preserving the current volume or
using an explicit low byte from the caller-supplied sentinel.

It returns the **live slot handle**, not a success flag.

Every branch and call site is documented below.

## Signature

| Register | Origin | Role |
|----------|--------|------|
| `r3` | `U` | battle unit pointer (kept in `r26`) |
| `r4` | `key` | sound key word (kept in `r27`) |
| `r5` | `rawSentinel` | volume/selector word (kept in `r31`) |
| `r6` | `belong` | base offset added to the derived id (kept in `r28`) |
| `r7` | `xyz` | pointer to 3 floats (kept in `r29`) |

Return (`r3`): the handle word returned by `psndSFXOn_3D`.

Stack frame: 48 bytes. `r30` is the derived-id accumulator; it starts at 0.

## Register capture (`0x80126098`..`0x801260ac`)

```asm
80126098: mr      27, 4                ; r27 = key
8012609c: mr      26, 3                ; r26 = U
801260a0: mr      31, 5                ; r31 = rawSentinel
801260a4: mr      28, 6                ; r28 = belong
801260a8: mr      29, 7                ; r29 = xyz
801260ac: li      30, 0                ; r30 = 0
```

All five arguments are captured before any call, so the callees may clobber
the argument registers.

## Status-bit id derivation (`0x801260b0`..`0x80126128`)

Two independent `if` blocks, each guarded by `BtlUnit_CheckStatus` and each
using `BtlUnit_GetStatus` to read a **signed byte** from the stack.

### Status 11 — the `-341` step (`0x801260b0`..`0x801260e8`)

```asm
801260b0: li      4, 11
801260b4: bl      0x80127814           ; BtlUnit_CheckStatus(U, 11)
801260b8: cmpwi   3, 0
801260bc: bt      2, 0x801260ec        ; not set -> skip
801260c0: mr      3, 26                ; U
801260c4: addi    5, 1, 9              ; r5 = &stack[9]
801260c8: addi    6, 1, 8              ; r6 = &stack[8]
801260cc: li      4, 11
801260d0: bl      0x80127f34           ; BtlUnit_GetStatus(U, 11, &s9, &s8)
801260d4: lbz     0, 8(1)              ; s8
801260d8: extsb   0, 0                 ; signed byte
801260dc: slwi    3, 0, 1              ; 2 * s8
801260e0: addi    0, 3, -1             ; 2*s8 - 1
801260e4: mulli   0, 0, -341           ; * -341
801260e8: extsh   30, 0                ; r30 = (int16)(...)
```

### Status 10 — the `+341` step (`0x801260ec`..`0x80126128`)

```asm
801260ec: mr      3, 26
801260f0: li      4, 10
801260f4: bl      0x80127814           ; BtlUnit_CheckStatus(U, 10)
801260f8: cmpwi   3, 0
801260fc: bt      2, 0x8012612c        ; not set -> skip
80126100: mr      3, 26
80126104: addi    5, 1, 9
80126108: addi    6, 1, 8
8012610c: li      4, 10
80126110: bl      0x80127f34           ; BtlUnit_GetStatus(U, 10, &s9, &s8)
80126114: lbz     0, 8(1)
80126118: extsb   0, 0
8012611c: slwi    3, 0, 1
80126120: addi    0, 3, 1              ; 2*s8 + 1
80126124: mulli   0, 0, -341
80126128: extsh   30, 0
```

Both blocks read `stack[8]` (written through `r6`); `stack[9]` is passed
but never read back locally.

**Precedence:** these are sequential `if`s that each **overwrite** `r30`.
If both status 11 and status 10 are set, the **status 10** result wins,
because its block executes last. If neither is set, `r30` stays `0`.

The two forms differ only in the `±1`:

| Status | id contribution |
|--------|------------------|
| 11 | `(int16)((2 * S + 1) * -341)` where S is the signed status byte |
| 10 | `(int16)((2 * S - 1) * -341)` |
| neither | `0` |

## Combining and drawing (`0x8012612c`..`0x8012614c`)

```asm
8012612c: add     30, 28, 30           ; r30 = belong + statusOffset
80126130: li      3, 41
80126134: bl      0x80005da0           ; irand(41)
80126138: add     4, 3, 30             ; r4 = irand + belong + statusOffset
8012613c: mr      3, 27                ; r3 = key
80126140: addi    0, 4, -20            ; r0 = r4 - 20
80126144: mr      4, 29                ; r4 = xyz
80126148: extsh   30, 0                ; r30 = (int16)(irand + belong + offset - 20)
8012614c: bl      0x800d9848           ; psndSFXOn_3D(key, xyz, id)
```

The start id is
`(int16)(irand(41) + belong + statusOffset - 20)`.

The `- 20` centers the draw: `irand(41)` yields 0..40, so without the bias
the range is `belong + offset .. belong + offset + 40`. Subtracting 20 gives
`belong + offset - 20 .. belong + offset + 20`.

The `extsh` truncation is load-bearing — a `belong`/`offset` combination
outside int16 range wraps rather than saturating.

## Pitch and volume (`0x80126150`..`0x80126184`)

```asm
80126150: mr      4, 30                ; r4 = derived id
80126154: mr      30, 3                ; r30 = handle
80126158: bl      0x800d9744           ; psndSFX_pit(handle, id)
8012615c: addis   0, 31, 3815          ; r0 = (rawSentinel & 0xFFFF0000) + 0xE7700000
80126160: cmplwi  0, 19840             ; 19840 == 0x4D80
80126164: bf      2, 0x80126178        ; r0 < 0x4D80 -> use raw low byte
80126168: mr      3, 30                ; handle
8012616c: bl      0x800d9558           ; psndSFX_get_vol(handle)
80126170: mr      4, 3                 ; r4 = current volume
80126174: b       0x8012617c
80126178: clrlwi  4, 31, 24            ; r4 = rawSentinel & 0xFF
8012617c: mr      3, 30                ; handle
80126180: bl      0x800d97d0           ; psndSFX_vol(handle, volume)
80126184: mr      3, 30                ; return handle
```

`r30` holds the handle from `psndSFXOn_3D` across both calls, and that same
handle is the return value.

### The volume decision, stated exactly

Let `T = rawSentinel`. Compute

```
r0 = (T & 0xFFFF0000) + 0xE7700000        (mod 2^32)
```

then compare `r0` **unsigned** against `0x4D80`:

| Condition | Volume used | Source |
|-----------|-------------|--------|
| `r0 < 0x4D80` | `T & 0xFF` | low byte of the sentinel — an **explicit** volume |
| `r0 >= 0x4D80` | return of `psndSFX_get_vol(handle)` | the **current** volume — i.e. unchanged |

The `r0 < 0x4D80` branch is only reachable when
`(T & 0xFFFF0000)` lies in `0xE7700000 .. 0xE7704D80`, i.e. when
`T` is in `0xE7700000 .. 0xE7704D80`.

Worked case for the value used by the movement-sound callers,
`T = 250000000 = 0x0EE6B280`:

```
T & 0xFFFF0000 = 0x0EE60000
r0 = 0x0EE60000 + 0xE7700000 = 0xF65C0000
0xF65C0000 >= 0x4D80   ->  preserve current volume
```

So the packed-Jump / movement-sound sentinel `250000000` selects the
**preserve-current-volume** path, not the explicit-low-byte path. This is a
derived value from the compared words, not an observed game trace.

## Called functions

| Address | Symbol | Count | Role |
|---------|--------|-------|------|
| `0x80127814` | `BtlUnit_CheckStatus` | 2 | status bits 11 and 10 |
| `0x80127f34` | `BtlUnit_GetStatus` | 2 | signed status byte via out-pointers |
| `0x80005da0` | `irand` | 1 | `irand(41)` |
| `0x800d9848` | `psndSFXOn_3D` | 1 | start; returns the handle |
| `0x800d9744` | `psndSFX_pit` | 1 | pitch/parameter from the derived id |
| `0x800d9558` | `psndSFX_get_vol` | 1 | read current volume |
| `0x800d97d0` | `psndSFX_vol` | 1 | apply volume |

## Porting notes

1. The return value is the **handle**, and callers store it as such. It is
   not a success flag — a failed start returns whatever
   `psndSFXOn_3D` returned, and the movement-sound caller does not
   classify it. See
   [BATTLE_MOVEMENT_SOUND_CONTROL](../battle/BATTLE_MOVEMENT_SOUND_CONTROL.md).
2. Status 10 overrides status 11 when both are set. Preserving the source
   order matters.
3. The `extsb` / `extsh` narrowings at both derivations and at the start id
   are part of the contract.
4. The volume branch is unsigned and its predicate is `r0 < 0x4D80`, not
   `r0 >= 0x4D80`. Inverting it swaps "preserve" and "explicit", which is
   silent and easily missed.
5. `psndSFX_get_vol` is called **before** `psndSFX_vol`, and the handle in
   `r30` is used for both plus the return.

## Remaining gaps

- `BtlUnit_CheckStatus` and `BtlUnit_GetStatus` internals, including what
  status bits 10 and 11 mean and the ownership of `stack[8]`/`stack[9]`.
- `psndSFXOn_3D` / `__psndSFXOn` resource, macro and voice provisioning —
  the chain that actually allocates a slot. Still open under EVT-01.
- `psndSFX_pit` semantics for the derived id.
- `irand(41)` distribution: the wrapper is at `0x80005da0`; whether it is
  inclusive of 41 and how it is seeded is not established here.
- No original execution trace of a real `BtlUnit_snd_se_pos` call.