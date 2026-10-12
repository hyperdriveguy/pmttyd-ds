# pouchGetPtr

**Address:** `0x800d5eb0`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800d5eb0: 80 6d 1b e0  	lwz 3, 7136(13)
800d5eb4: 4e 80 00 20  	blr
```

## Behavior

`pouchGetPtr` is a trivial SDA global load getter. It loads a 32-bit word
from offset `0x1be0` (7136 decimal) relative to the small-data area base
register `r13`, and returns it in register 3.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` (set by `__init_registers` at `0x8000539c`) |
| Load address | `0x8041cf20 + 0x1be0 = 0x8041eae0` |
| Return | `r3` = word at `0x8041eae0` (the global pouch state pointer) |
| Side effects | None |
| Calls | None |

## Interpretation

The function returns a pointer to the global **pouch state** structure. The
pouch system manages HP, FP, items, badges, coins, and star points for the
player party. This getter provides access to that shared state without any
indirection beyond the single global variable load.

The pouch structure is used throughout the battle and field systems — any
function that needs to read or modify player resources (HP, FP, items, etc.)
will call `pouchGetPtr` first to obtain the base pointer.

## Callers

Not yet traced. Future recovery should identify which functions call
`pouchGetPtr` and how they use the returned pointer (e.g., `pouchGetHP`,
`pouchAddHP`, `pouchSetMaxHP`, etc.).

## Remaining gaps

- Exact layout of the pouch structure pointed to by `0x8041eae0`
- Full caller graph
- Lifetime/initialization of the global pointer
