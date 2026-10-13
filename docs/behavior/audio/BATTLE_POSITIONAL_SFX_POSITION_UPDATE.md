# psndSFX_pos

**Address:** `0x800d96f8`
**Size:** 76 bytes (19 instructions)
**Namespace:** `pmario_sound.o`
**Evidence class:** B (inspected US bytes/control flow — **complete body**)

## Overview

`psndSFX_pos(handle, xyz)` updates the stored 3D position of one live
positional-SFX slot. It is a **leaf function** — no stack frame, no
`mflr`/`mtlr`, and the only exit is a conditional branch to the link
register.

Every instruction is documented below.

## Signature

| Register | Use |
|----------|-----|
| `r3` | slot handle (low 8 bits select the slot) |
| `r4` | pointer to 3 consecutive `float`s (x, y, z) |

Return: void.

## Body

```asm
800d96f8: addis   0, 3, 1              ; r0 = handle + 0x10000
800d96fc: cmplwi  0, 65535             ; == 0xFFFF ?
800d9700: bclr    12, 2                ; if LT (handle == -1) -> return
800d9704: clrlwi  0, 3, 24             ; slot = handle & 0xFF
800d9708: lis     3, -32706            ; 0x803e0000
800d970c: mulli   5, 0, 40             ; slot * 40
800d9710: addi    0, 3, -1656          ; 0x803df988
800d9714: add     5, 0, 5              ; r5 = slot record
800d9718: lwz     3, 0(5)              ; slot.key
800d971c: addis   0, 3, 1
800d9720: cmplwi  0, 65535
800d9724: bclr    12, 2                ; if key == -1 -> return
800d9728: lwz     3, 0(4)              ; xyz[0]
800d972c: lwz     0, 4(4)              ; xyz[1]
800d9730: stw     3, 28(5)             ; slot[+0x1c] = x
800d9734: stw     0, 32(5)             ; slot[+0x20] = y
800d9738: lwz     0, 8(4)              ; xyz[2]
800d973c: stw     0, 36(5)             ; slot[+0x24] = z
800d9740: blr
```

## Two guards, both `-1`

1. **`handle == -1`** — detected as `handle + 0x10000 == 0xFFFF` unsigned.
   Returns without touching memory. This is the "no retained voice" case
   used by [`BtlUnit_snd_se_pos`'s callers](BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md).
2. **`slot.key == -1`** — the slot is free. Also returns without writing.

Both use `bclr 12, 2`, which branches to the link register when `CR0[LT]`
is set. There is **no stack frame at all**: the function saves nothing,
including `r3`/`r4`/`r5`, which is why every field is loaded into a
scratch register immediately before its store.

## What it writes — and what it does not

Three `stw`s, and only three:

| Source | Destination | Field |
|--------|-----------|-------|
| `xyz[0]` | `slot+0x1c` | position x |
| `xyz[1]` | `slot+0x20` | position y |
| `xyz[2]` | `slot+0x24` | position z |

It does **not** touch `slot+0x00` (key) or `slot+0x18` (handle). It is
purely a position publication; it cannot start or stop a sound, and it
does not re-trigger anything.

The values are copied as raw 32-bit words — no `lfs`/`stfs`, so this is a
bit-exact float copy with no intermediate rounding.

## Ordering

The x and y words are both loaded **before** either is stored
(`0x800d9728`, `0x800d972c`, then `0x800d9730`, `0x800d9734`), then z is
loaded and stored. A port may not re-interleave the loads with the stores
if it wants identical aliasing behaviour when the caller's vector overlaps
the slot record.

## Data addresses

| Address | Role |
|---------|------|
| `0x803df988` | slot record array base, **stride 40**, shared with [`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md) — symbol **`pssfx`** (`.bss`, 1600 bytes) |

## Porting notes

1. Both guards must precede every load from the caller-supplied pointer.
   A `-1` handle with a null `xyz` is legal input in the original.
2. Slot selection uses the **low 8 bits** of the handle only.
3. Copy with word moves, not float moves, if exact bit patterns matter.
4. The function is re-entrant with respect to the slot record only if no
   other writer races the same slot; the original has no protection.

## Remaining gaps

- Who reads `slot+0x1c..+0x24`, i.e. the audio update path that consumes
  the published position. That is inside `psndSFXMain`
  (`0x800d86e4`-area, 2964 bytes), which is undocumented.
- Whether a stale position persists for a slot between `psndSFX_pos` calls
  — nothing here resets it, and [`psndSFXOff`](BATTLE_POSITIONAL_SFX_OFF.md)
  does not clear it either.
- Original execution traces of a position update.