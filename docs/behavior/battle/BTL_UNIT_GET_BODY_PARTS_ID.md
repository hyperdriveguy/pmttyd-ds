# BtlUnit_GetBodyPartsId

**Address:** `0x80128c98`  
**Size:** 64 bytes (16 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80128c98: 28 03 00 00  	cmplwi 3, 0
80128c9c: 38 80 ff ff  	li 4, -1
80128ca0: 41 82 00 30  	bt 2, 0x80128cd0
80128ca4: 80 63 00 14  	lwz 3, 20(3)
80128ca8: 48 00 00 20  	b 0x80128cc8
80128cac: 80 03 01 ac  	lwz 0, 428(3)
80128cb0: 54 00 07 ff  	clrlwi. 0, 0, 31
80128cb4: 41 82 00 10  	bt 2, 0x80128cc4
80128cb8: 80 63 00 04  	lwz 3, 4(3)
80128cbc: 80 83 00 00  	lwz 4, 0(3)
80128cc0: 48 00 00 10  	b 0x80128cd0
80128cc4: 80 63 00 00  	lwz 3, 0(3)
80128cc8: 28 03 00 00  	cmplwi 3, 0
80128ccc: 40 82 ff e0  	bf 2, 0x80128cac
80128cd0: 7c 83 23 78  	mr 3, 4
80128cd4: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_GetBodyPartsId` traverses a linked list of parts and returns
the part ID of the body part. Returns -1 if no body part is found.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure (may be null) |
| Return | `r3` = part ID of body part, or -1 if not found |
| Side effects | None |
| Calls | None |

## Interpretation

This function searches through the unit's parts linked list to find the
body part. It checks offset 428 (`0x1ac`) for a flag indicating whether
the current part is the body part. If found, it returns the part ID.
The search continues until the list ends or the body part is located.

## Remaining gaps

- Full layout of the part structure
- Meaning of the flag at offset 428
- Caller graph
- Relationship to `BtlUnit_GetPartsPtr`
