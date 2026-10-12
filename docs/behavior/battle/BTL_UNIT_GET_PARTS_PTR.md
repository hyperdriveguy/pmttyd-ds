# BtlUnit_GetPartsPtr

**Address:** `0x80128cd8`  
**Size:** 64 bytes (16 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80128cd8: 2c 04 00 00  	cmpwi 4, 0
80128cdc: 80 63 00 14  	lwz 3, 20(3)
80128ce0: 4d 80 00 20  	bclr 12, 0
80128ce4: 80 03 00 00  	lwz 0, 0(3)
80128ce8: 28 00 00 00  	cmplwi 0, 0
80128cec: 40 82 00 20  	bf 2, 0x80128d0c
80128cf0: 4e 80 00 20  	blr
80128cf4: 48 00 00 18  	b 0x80128d0c
80128cf8: 80 a3 00 04  	lwz 5, 4(3)
80128cfc: 80 05 00 00  	lwz 0, 0(5)
80128d00: 7c 00 20 00  	cmpw 0, 4
80128d04: 4d 82 00 20  	bclr 12, 2
80128d08: 80 63 00 00  	lwz 3, 0(3)
80128d0c: 28 03 00 00  	cmplwi 3, 0
80128d10: 40 82 ff e8  	bf 2, 0x80128cf8
80128d14: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_GetPartsPtr` searches a linked list of parts for one matching
the supplied ID. Returns a pointer to the matching part, or null if not
found.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure |
| Argument | `r4` = part ID to search for (0 terminates search) |
| Return | `r3` = pointer to matching part, or 0 if not found |
| Side effects | None |
| Calls | None |

## Interpretation

This function traverses a linked list of parts associated with a battle
unit. Each part has a part ID at offset 0 and a next pointer at offset 0
(the same field, suggesting a union or the ID is stored elsewhere). The
search continues until a matching part ID is found or the list ends.
A part ID of 0 serves as a sentinel/terminator.

## Remaining gaps

- Full layout of the part structure
- Exact linked list traversal logic (the disassembly is complex)
- Caller graph
- Relationship to `BtlUnit_GetBodyPartsId`
