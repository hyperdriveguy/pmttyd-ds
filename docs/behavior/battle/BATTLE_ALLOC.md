# BattleAlloc

**Address:** `0x800f8934`  
**Size:** 40 bytes (10 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800f8934: 94 21 ff f0  	stwu 1, -16(1)
800f8938: 7c 08 02 a6  	mflr 0
800f893c: 7c 64 1b 78  	mr 4, 3
800f8940: 90 01 00 14  	stw 0, 20(1)
800f8944: 80 6d 18 ec  	lwz 3, 6380(13)
800f8948: 4b f3 76 11  	bl 0x8002ff58
800f894c: 80 01 00 14  	lwz 0, 20(1)
800f8950: 7c 08 03 a6  	mtlr 0
800f8954: 38 21 00 10  	addi 1, 1, 16
800f8958: 4e 80 00 20  	blr
```

## Behavior

`BattleAlloc` allocates memory from the battle heap. It loads the battle
heap pointer from SDA, then calls the heap allocation function with the
requested size.

| Item | Value |
| --- | --- |
| Argument | `r3` = size in bytes to allocate |
| Battle heap | `[r13 + 0x18ec]` (heap pointer) |
| Calls | `0x8002ff58` (heap allocation function) |
| Return | `r3` = pointer to allocated memory |

## Interpretation

This function provides memory allocation for battle-related data structures.
It uses a dedicated battle heap (separate from the main game heap) to
manage memory during battles. The heap pointer is stored in SDA and
maintained by the heap system.

## Remaining gaps

- Implementation of the heap allocation function at `0x8002ff58`
- Battle heap initialization and size
- Caller graph
- Relationship to `BattleFree`
