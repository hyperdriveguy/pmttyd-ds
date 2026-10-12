# camGetCurPtr

**Address:** `0x8000f2e4`  
**Size:** 24 bytes (6 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8000f2e4: 80 0d 16 e8  	lwz 0, 5864(13)
8000f2e8: 3c 60 80 3d  	lis 3, -32707
8000f2ec: 38 63 95 f8  	addi 3, 3, -27144
8000f2f0: 54 00 10 3a  	slwi 0, 0, 2
8000f2f4: 7c 63 00 2e  	lwzx 3, 3, 0
8000f2f8: 4e 80 00 20  	blr
```

## Behavior

`camGetCurPtr` loads a camera index from SDA offset `0x16e8`, shifts it
left by 2 (multiply by 4), and uses it to index into a pointer table at
`0x803d95f8`.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` |
| Index load | `0` = `[r13 + 0x16e8]` (camera index) |
| Table base | `0x803d95f8` (lis 0x803d + addi -27144) |
| Return | `r3` = `table[index]` (pointer to current camera state) |
| Side effects | None |
| Calls | None |

## Interpretation

The camera system maintains a table of camera state pointers. This getter
selects the current camera by index and returns its state pointer. The
index is stored as a global variable in SDA.

## Remaining gaps

- Number of entries in the camera pointer table
- Layout of the camera state structure
- Caller graph
