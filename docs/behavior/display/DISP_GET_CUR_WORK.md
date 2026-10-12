# dispGetCurWork

**Address:** `0x80010864`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80010864: 80 6d 16 fc  	lwz 3, 5884(13)
80010868: 4e 80 00 20  	blr
```

## Behavior

`dispGetCurWork` is a trivial SDA global load getter. It loads a 32-bit
word from offset `0x16fc` (5884 decimal) relative to the small-data area
base register `r13`, and returns it in register 3.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` (set by `__init_registers` at `0x8000539c`) |
| Load address | `0x8041cf20 + 0x16fc = 0x8041e61c` |
| Return | `r3` = word at `0x8041e61c` (the global display work pointer) |
| Side effects | None |
| Calls | None |

## Interpretation

The function returns a pointer to the global **display work** structure.
The display system manages rendering state, display lists, and GPU-related
data. This getter provides access to the current display context without any
indirection beyond the single global variable load.

## Remaining gaps

- Exact layout of the display work structure
- Full caller graph
- Lifetime/initialization of the global pointer
