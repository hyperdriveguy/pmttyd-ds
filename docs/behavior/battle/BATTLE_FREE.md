# BattleFree

**Address:** `0x800f8908`  
**Size:** 44 bytes (11 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800f8908: 94 21 ff f0  	stwu 1, -16(1)
800f890c: 7c 08 02 a6  	mflr 0
800f8910: 7c 64 1b 79  	mr. 4, 3
800f8914: 90 01 00 14  	stw 0, 20(1)
800f8918: 41 82 00 0c  	bt 2, 0x800f8924
800f891c: 80 6d 18 ec  	lwz 3, 6380(13)
800f8920: 4b f3 74 a5  	bl 0x8002fdc4
800f8924: 80 01 00 14  	lwz 0, 20(1)
800f8928: 7c 08 03 a6  	mtlr 0
800f892c: 38 21 00 10  	addi 1, 1, 16
800f8930: 4e 80 00 20  	blr
```

## Behavior

`BattleFree` deallocates memory from the battle heap. It checks for a
null pointer, loads the battle heap, and calls the heap free function.

| Item | Value |
| --- | --- |
| Argument | `r3` = pointer to memory to free (may be null) |
| Null check | Returns immediately if `r3 == 0` |
| Battle heap | `[r13 + 0x18ec]` (heap pointer) |
| Calls | `0x8002fdc4` (heap free function) |
| Return | None (void) |

## Interpretation

This function provides memory deallocation for battle-related data
structures. It uses the same battle heap as `BattleAlloc` and includes
a null pointer check for safety. The heap free function manages the
memory pool.

## Remaining gaps

- Implementation of the heap free function at `0x8002fdc4`
- Battle heap initialization and size
- Caller graph
- Relationship to `BattleAlloc`
