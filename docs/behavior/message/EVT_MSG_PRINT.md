# evt_msg_print

**Address:** `0x800d284c`  
**Size:** 136 bytes (34 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800d284c: 94 21 ff e0  	stwu 1, -32(1)
800d2850: 7c 08 02 a6  	mflr 0
800d2854: 90 01 00 24  	stw 0, 36(1)
800d2858: bf 61 00 0c  	stmw 27, 12(1)
800d285c: 7c 9c 23 78  	mr 28, 4
800d2860: 7c 7b 1b 78  	mr 27, 3
800d2864: 83 e3 00 18  	lwz 31, 24(3)
800d2868: 80 9f 00 00  	lwz 4, 0(31)
800d286c: 4b f5 e9 f5  	bl 0x80031260
800d2870: 80 9f 00 04  	lwz 4, 4(31)
800d2874: 7c 7e 1b 78  	mr 30, 3
800d2878: 7f 63 db 78  	mr 3, 27
800d287c: 4b f5 e9 e5  	bl 0x80031260
800d2880: 80 9f 00 08  	lwz 4, 8(31)
800d2884: 7c 7d 1b 78  	mr 29, 3
800d2888: 7f 63 db 78  	mr 3, 27
800d288c: 4b f5 e9 d5  	bl 0x80031260
800d2890: 80 9f 00 0c  	lwz 4, 12(31)
800d2894: 7c 7f 1b 78  	mr 31, 3
800d2898: 7f 63 db 78  	mr 3, 27
800d289c: 4b f5 e9 c5  	bl 0x80031260
800d28a0: 7c 60 1b 78  	mr 0, 3
800d28a4: 7f 63 db 78  	mr 3, 27
800d28a8: 7c 08 03 78  	mr 8, 0
800d28ac: 7f 84 e3 78  	mr 4, 28
800d28b0: 7f c5 f3 78  	mr 5, 30
800d28b4: 7f a6 eb 78  	mr 6, 29
800d28b8: 7f e7 fb 78  	mr 7, 31
800d28bc: 48 00 00 19  	bl 0x800d28d4
800d28c0: bb 61 00 0c  	lmw 27, 12(1)
800d28c4: 80 01 00 24  	lwz 0, 36(1)
800d28c8: 7c 08 03 a6  	mtlr 0
800d28cc: 38 21 00 20  	addi 1, 1, 32
800d28d0: 4e 80 00 20  	blr
```

## Behavior

`evt_msg_print` is the event command that displays a message in a dialogue
window. It resolves four arguments (message ID, speaker, animation, and
window type) and calls the message display function at `0x800d28d4`.

| Item | Value |
| --- | --- |
| Argument | `r3` = event argument structure |
| Argument | `r4` = (preserved for message display) |
| Source args | `[arg+0]` through `[arg+12]` — four 32-bit values |
| Calls | `0x80031260` (resolve) x4, `0x800d28d4` (display) |
| Return | `r3` = 0 (yield) |
| Calls | `0x80031260` (resolve), `0x800d28d4` (display) |

## Interpretation

This is the primary event command for displaying dialogue. It resolves four
values from the event argument structure (at offset 24) and passes them to
the message display function. The four values are:
1. Message ID — selects the text from the message table
2. Speaker — determines the speaker name display
3. Animation — controls the speaker's animation during display
4. Window type — selects the dialogue window style

The function returns 0, which in the event system means the command is
blocked (will continue on the next eligible invocation).

## Remaining gaps

- Implementation of the message display function at `0x800d28d4`
- Full message ID table and speaker definitions
- Animation and window type mappings
- Token processing and control code interpretation
