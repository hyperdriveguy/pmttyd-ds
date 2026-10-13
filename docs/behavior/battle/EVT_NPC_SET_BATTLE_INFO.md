# evt_npc_set_battle_info

**Address:** `0x8004f090`  
**Size:** 100 bytes (25 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8004f090: 94 21 ff f0  	stwu 1, -16(1)
8004f094: 7c 08 02 a6  	mflr 0
8004f098: 90 01 00 14  	stw 0, 20(1)
8004f09c: bf c1 00 08  	stmw 30, 8(1)
8004f0a0: 7c 7e 1b 78  	mr 30, 3
8004f0a4: 83 e3 00 18  	lwz 31, 24(3)
8004f0a8: 80 9f 00 00  	lwz 4, 0(31)
8004f0ac: 4b fe 21 b5  	bl 0x80031260
8004f0b0: 80 9f 00 04  	lwz 4, 4(31)
8004f0b4: 7c 7f 1b 78  	mr 31, 3
8004f0b8: 7f c3 f3 78  	mr 3, 30
8004f0bc: 4b fe 21 a5  	bl 0x80031260
8004f0c0: 7c 60 1b 78  	mr 0, 3
8004f0c4: 7f c3 f3 78  	mr 3, 30
8004f0c8: 7c 1e 03 78  	mr 30, 0
8004f0cc: 7f e4 fb 78  	mr 4, 31
8004f0d0: 48 00 05 09  	bl 0x8004f5d8
8004f0d4: 7f c4 f3 78  	mr 4, 30
8004f0d8: 4b ff 4c a9  	bl 0x80043d80
8004f0dc: bb c1 00 08  	lmw 30, 8(1)
8004f0e0: 38 60 00 02  	li 3, 2
8004f0e4: 80 01 00 14  	lwz 0, 20(1)
8004f0e8: 7c 08 03 a6  	mtlr 0
8004f0ec: 38 21 00 10  	addi 1, 1, 16
8004f0f0: 4e 80 00 20  	blr
```

## Behavior

`evt_npc_set_battle_info` is an event command that sets battle information for an NPC. It resolves two IDs, calls an info setup function, and then calls a secondary configuration function.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve ID) x2, `0x8004f5d8` (setup info), `0x80043d80` (configure) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command configures an NPC's battle information. It resolves two IDs (likely NPC type and battle config), calls a setup function to initialize the battle info, and then calls a secondary configuration function. This is used when NPCs enter battle to set up their stats, abilities, and behavior.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the called functions
- Caller graph
