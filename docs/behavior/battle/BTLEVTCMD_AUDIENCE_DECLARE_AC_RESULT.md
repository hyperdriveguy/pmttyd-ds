# btlevtcmd_AudienceDeclareACResult

**Address:** `0x8010749c`  
**Size:** 172 bytes (43 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
8010749c: 94 21 ff e0  	stwu 1, -32(1)
801074a0: 7c 08 02 a6  	mflr 0
801074a4: 90 01 00 24  	stw 0, 36(1)
801074a8: bf a1 00 14  	stmw 29, 20(1)
801074ac: 7c 7d 1b 78  	mr 29, 3
801074b0: 83 c3 00 18  	lwz 30, 24(3)
801074b4: 83 ed 1c 70  	lwz 31, 7280(13)
801074b8: 80 9e 00 00  	lwz 4, 0(30)
801074bc: 4b f2 9d a5  	bl 0x80031260
801074c0: 80 9e 00 04  	lwz 4, 4(30)
801074c4: 7c 7e 1b 78  	mr 30, 3
801074c8: 7f a3 eb 78  	mr 3, 29
801074cc: 4b f2 9d 95  	bl 0x80031260
801074d0: 7c 65 1b 78  	mr 5, 3
801074d4: 3c 9f 00 01  	addis 4, 31, 1
801074d8: 2c 05 ff ff  	cmpwi 5, -1
801074dc: 41 82 00 28  	bt 2, 0x80107504
801074e0: 88 64 6f 58  	lbz 3, 28504(4)
801074e4: 28 03 00 c8  	cmplwi 3, 200
801074e8: 40 80 00 0c  	bf 0, 0x801074f4
801074ec: 38 03 00 01  	addi 0, 3, 1
801074f0: 98 04 6f 58  	stb 0, 28504(4)
801074f4: 3c 7f 00 02  	addis 3, 31, 2
801074f8: 38 00 00 01  	li 0, 1
801074fc: 98 03 90 74  	stb 0, -28556(3)
80107500: 48 00 00 24  	b 0x80107524
80107504: 88 64 6f 59  	lbz 3, 28505(4)
80107508: 28 03 00 c8  	cmplwi 3, 200
8010750c: 40 80 00 0c  	bf 0, 0x80107518
80107510: 38 03 00 01  	addi 0, 3, 1
80107514: 98 04 6f 59  	stb 0, 28505(4)
80107518: 3c 7f 00 02  	addis 3, 31, 2
8010751c: 38 00 00 00  	li 0, 0
80107520: 98 03 90 74  	stb 0, -28556(3)
80107524: 7f e3 fb 78  	mr 3, 31
80107528: 7f c4 f3 78  	mr 4, 30
8010752c: 4b ff 2f c5  	bl 0x800fa4f0
80107530: bb a1 00 14  	lmw 29, 20(1)
80107534: 38 60 00 02  	li 3, 2
80107538: 80 01 00 24  	lwz 0, 36(1)
8010753c: 7c 08 03 a6  	mtlr 0
80107540: 38 21 00 20  	addi 1, 1, 32
80107544: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_AudienceDeclareACResult` is an event command that declares
an action command result to the audience system. It resolves two unit
IDs, checks audience counters, and calls the result declaration function.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x80031260` (resolve unit ID) x2, `0x800fa4f0` (declare result) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command declares an action command result to the audience
system. It resolves two unit IDs, checks audience counters (capped at
200), and calls the result declaration function. The audience system
tracks how many times each unit has been targeted for scoring or
reward purposes.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the result declaration function at `0x800fa4f0`
- Caller graph
- Meaning of the audience counters at offsets 28504 and 28505
