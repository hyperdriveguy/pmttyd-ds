# btlevtcmd_CheckPhase

**Address:** `0x80113708`  
**Size:** 112 bytes (28 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80113708: 94 21 ff e0  	stwu 1, -32(1)
8011370c: 7c 08 02 a6  	mflr 0
80113710: 38 80 00 04  	li 4, 4
80113714: 90 01 00 24  	stw 0, 36(1)
80113718: bf a1 00 14  	stmw 29, 20(1)
8011371c: 7c 7d 1b 78  	mr 29, 3
80113720: 80 bd 00 18  	lwz 5, 24(29)
80113724: 80 6d 1c 70  	lwz 3, 7280(13)
80113728: 83 e5 00 00  	lwz 31, 0(5)
8011372c: 83 c5 00 04  	lwz 30, 4(5)
80113730: 4b fe 52 71  	bl 0x800f89a0
80113734: 7c 1e 18 40  	cmplw 30, 3
80113738: 41 82 00 18  	bt 2, 0x80113750
8011373c: 7f a3 eb 78  	mr 3, 29
80113740: 7f e4 fb 78  	mr 4, 31
80113744: 38 a0 00 00  	li 5, 0
80113748: 4b f1 d5 b5  	bl 0x80030cfc
8011374c: 48 00 00 14  	b 0x80113760
80113750: 7f a3 eb 78  	mr 3, 29
80113754: 7f e4 fb 78  	mr 4, 31
80113758: 38 a0 00 01  	li 5, 1
8011375c: 4b f1 d5 a1  	bl 0x80030cfc
80113760: bb a1 00 14  	lmw 29, 20(1)
80113764: 38 60 00 02  	li 3, 2
80113768: 80 01 00 24  	lwz 0, 36(1)
8011376c: 7c 08 03 a6  	mtlr 0
80113770: 38 21 00 20  	addi 1, 1, 32
80113774: 4e 80 00 20  	blr
```

## Behavior

`btlevtcmd_CheckPhase` is an event command that checks the current battle phase. It calls a phase check function and stores the result (0 or 1) in an event variable.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Calls | `0x800f89a0` (check phase), `0x80030cfc` (set variable) |
| Return | `r3` = 2 (command complete) |

## Interpretation

This event command checks the current battle phase and stores the result in an event variable. The phase check function returns a value that is compared against a threshold, and the result (0 or 1) is stored. This is used in battle event scripts for phase-dependent logic.

## Remaining gaps

- Full layout of the event argument structure
- Implementation of the phase check function at `0x800f89a0`
- Caller graph
