# battleMain

**Address:** `0x800f904c`  
**Size:** 144 bytes (36 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800f904c: 94 21 ff f0  	stwu 1, -16(1)
800f9050: 7c 08 02 a6  	mflr 0
800f9054: 90 01 00 14  	stw 0, 20(1)
800f9058: 93 e1 00 0c  	stw 31, 12(1)
800f905c: 83 ed 1c 70  	lwz 31, 7280(13)
800f9060: 28 1f 00 00  	cmplwi 31, 0
800f9064: 41 82 00 1c  	bt 2, 0x800f9080
800f9068: 4b ff fd c1  	bl 0x800f8e28
800f906c: 2c 03 00 00  	cmpwi 3, 0
800f9070: 40 82 00 10  	bf 2, 0x800f9080
800f9074: 80 1f 0e f4  	lwz 0, 3828(31)
800f9078: 64 00 80 00  	oris 0, 0, 32768
800f907c: 90 1f 0e f4  	stw 0, 3828(31)
800f9080: 80 01 00 14  	lwz 0, 20(1)
800f9084: 83 e1 00 0c  	lwz 31, 12(1)
800f9088: 7c 08 03 a6  	mtlr 0
800f908c: 38 21 00 10  	addi 1, 1, 16
800f9090: 4e 80 00 20  	blr
```

## Behavior

`battleMain` is the main battle system update function, called once per
retrace from `marioStMain`. It checks if battle work exists, calls the
battle sequence dispatcher, and sets the battle-active flag.

| Item | Value |
| --- | --- |
| Battle work | `[r13 + 0x1c70]` (SDA global) |
| Null check | Returns immediately if battle work is null |
| Dispatcher | `0x800f8e28` — battle sequence dispatcher |
| Battle-active flag | Sets bit 15 (mask `0x8000`) of battle flags at offset 3828 |
| Return | None (void) |

## Interpretation

This is a thin wrapper that delegates to the battle sequence dispatcher at
`0x800f8e28`. The dispatcher returns a value; if it returns 0, the
battle-active flag is set. The battle flags field at offset 3828 (`0xef4`)
tracks various battle states including the active bit.

The function is called from the main pass order:
```
seqMain → cardMain → bgMain → shadowMain → animMain → battleMain → ...
```

## Remaining gaps

- Implementation of the battle sequence dispatcher at `0x800f8e28`
- Meaning of the return value from the dispatcher
- Full semantics of the battle flags at offset 3828
- Relationship to turn management and unit updates
