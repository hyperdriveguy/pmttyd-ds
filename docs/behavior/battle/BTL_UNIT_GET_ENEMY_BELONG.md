# BtlUnit_GetEnemyBelong

**Address:** `0x80127454`  
**Size:** 28 bytes (7 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80127454: 88 63 00 0c  	lbz 3, 12(3)
80127458: 7c 63 07 74  	extsb 3, 3
8012745c: 2c 03 00 01  	cmpwi 3, 1
80127460: 41 82 00 18  	bt 2, 0x80127478
80127464: 4c 80 00 20  	bclr 4, 0
80127468: 2c 03 00 00  	cmpwi 3, 0
8012746c: 4d 80 00 20  	bclr 12, 0
80127470: 38 60 00 01  	li 3, 1
80127474: 4e 80 00 20  	blr
80127478: 38 60 00 00  	li 3, 0
8012747c: 4e 80 00 20  	blr
```

## Behavior

`BtlUnit_GetEnemyBelong` reads the unit's belong byte (offset 12),
sign-extends it, and returns 0 if the value is 1, or 1 otherwise.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to a battle unit structure |
| Return | `r3` = 0 if belong == 1, else 1 |
| Side effects | None |
| Calls | None |

## Interpretation

This function checks whether a unit belongs to the enemy team. The belong
byte at offset 12 uses the value 1 to indicate an enemy unit. The function
returns 0 for enemies and 1 for non-enemies (player/ally units). This is
a simple team identification check.

## Remaining gaps

- Full layout of the battle unit structure
- Meaning of other belong values (e.g., 2 for neutral)
- Caller graph
