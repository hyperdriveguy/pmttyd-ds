# btl_camera_set_zoom

**Address:** `0x8012bab4`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8012bab4: 80 8d 1c 70  	lwz 4, 7280(13)
8012bab8: 80 04 27 5c  	lwz 0, 10076(4)
8012babc: 7c 03 00 00  	cmpw 3, 0
8012bac0: 4d 80 00 20  	bclr 12, 0
8012bac4: d0 24 27 78  	stfs 1, 10104(4)
8012bac8: 4e 80 00 20  	blr
```

## Behavior

`btl_camera_set_zoom` compares the supplied zoom level against the
current zoom level in the battle camera structure. If they differ, it
sets a flag indicating the zoom has changed. Then it stores the new
zoom level as a float.

| Item | Value |
| --- | --- |
| Argument | `r3` = new zoom level (integer) |
| Current zoom | `[battleWork + 10076]` (integer) |
| Changed flag | CR bit 12 set if `r3 != current` |
| Side effect | `[battleWork + 10104] = (float)r3` (float store) |
| Return | None (void) |
| Calls | None |

## Interpretation

This function sets the battle camera's zoom level. It first compares
the new value against the current value, setting a condition register
bit if they differ (which may trigger a zoom animation or interpolation).
The new value is then stored as a float at offset 10104. The `bclr`
instruction clears the condition register bit after the comparison.

## Remaining gaps

- Full layout of the battle camera structure
- Meaning of the zoom-changed flag
- Caller graph
- Relationship to camera interpolation and animation
