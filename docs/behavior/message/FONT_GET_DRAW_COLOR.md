# FontGetDrawColor

**Address:** `0x8007776c`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007776c: 80 6d 1a 8c  	lwz 3, 6796(13)
80077770: 4e 80 00 20  	blr
```

## Behavior

`FontGetDrawColor` is a trivial SDA global load getter. It loads a 32-bit
word from offset `0x1a8c` (6796 decimal) relative to the small-data area
base register `r13`, and returns it in register 3.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` (set by `__init_registers` at `0x8000539c`) |
| Load address | `0x8041cf20 + 0x1a8c = 0x8041e9ac` |
| Return | `r3` = word at `0x8041e9ac` (the global font draw color) |
| Side effects | None |
| Calls | None |

## Interpretation

The function returns the current **font draw color** as a 32-bit word. In
the GameCube's GX graphics system, colors are typically stored as RGBA
packed into a single 32-bit value (8 bits per channel). This getter provides
the color that the font rendering system will use when drawing text glyphs.

## Remaining gaps

- Exact color format (likely RGBA8, but unconfirmed)
- Full caller graph
- Relationship to font draw state and glyph rendering
