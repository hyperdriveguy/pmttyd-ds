# windowGetPointer

**Address:** `0x8007ad40`  
**Size:** 16 bytes (4 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8007ad40: 1c 03 00 48  	mulli 0, 3, 72
8007ad44: 80 6d 1b 08  	lwz 3, 6920(13)
8007ad48: 7c 63 02 14  	add 3, 3, 0
8007ad4c: 4e 80 00 20  	blr
```

## Behavior

`windowGetPointer` computes the address of a window record by multiplying
the supplied index by 72 (the size of each window record) and adding it
to the base of the window array.

| Item | Value |
| --- | --- |
| Argument | `r3` = window index |
| Return | `r3` = `windowArray + index * 72` |
| Side effects | None |
| Calls | None |

## Interpretation

The window system manages an array of 7 window records, each 72 bytes in
size. This getter provides indexed access to a specific window record.
The window array base is stored at SDA offset `0x1b08` from `r13`.

## Remaining gaps

- Full layout of the 72-byte window record structure
- Caller graph
- Relationship to other window management functions
