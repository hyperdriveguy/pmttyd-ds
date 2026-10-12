# dispReInit

**Address:** `0x80010c64`  
**Size:** 12 bytes (3 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80010c64: 38 00 00 00  	li 0, 0
80010c68: 90 0d 16 f8  	stw 0, 5880(13)
80010c6c: 4e 80 00 20  	blr
```

## Behavior

`dispReInit` is a trivial SDA global clear. It stores zero to offset
`0x16f8` (5880 decimal) relative to the small-data area base register
`r13`, then returns.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` |
| Store address | `0x8041cf20 + 0x16f8 = 0x8041e618` |
| Side effect | `[0x8041e618] = 0` (clears the display work pointer) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function resets the global **display work** pointer to null. It is
called during display subsystem reinitialization, typically when the
system mode changes or when the display context needs to be rebuilt from
scratch.

## Remaining gaps

- Relationship to `dispInit` (which allocates and initializes the display
  work buffers)
- Caller graph
