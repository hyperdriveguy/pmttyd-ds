# weaponGetPowerDefault

**Address:** `0x80189d30`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80189d30: 80 64 00 20  	lwz 3, 32(4)
80189d34: 4e 80 00 20  	blr
```

## Behavior

`weaponGetPowerDefault` is a trivial tail-load getter. It receives a
pointer argument in register 4, loads the 32-bit word at byte offset 32 from
that pointer into register 3, and returns immediately via `blr`.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a weapon record (or similar structure) |
| Return | `r3` = word at `[argument + 32]` (the weapon's default power value) |
| Side effects | None |
| Calls | None |

## Interpretation

The function reads a field at offset `0x20` from the supplied weapon data
pointer. In the battle system, this is the weapon's **default power** — the
base attack value used when no Mario level or badge modifier is applied.
The raw word is returned unmodified; no clamping, sign extension, or other
transformation occurs.

## Callers

Not yet traced. This is a leaf utility; future recovery should identify
which weapon power callback passes the default weapon record to this function
and confirm the semantic meaning of the `+0x20` field.

## Remaining gaps

- Exact semantic meaning of the `+0x20` field (presumed: default power)
- Full caller graph
- Relationship to `weaponGetPowerFromMarioJumpLv` and
  `weaponGetPowerFromMarioHammerLv` (the level-dependent variants)
