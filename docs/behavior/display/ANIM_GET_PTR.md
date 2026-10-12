# animGetPtr

**Address:** `0x80042f30`  
**Size:** 8 bytes (2 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
80042f30: 80 6d 8e a0  	lwz 3, -29024(13)
80042f34: 4e 80 00 20  	blr
```

## Behavior

`animGetPtr` is a trivial SDA global load getter. It loads a 32-bit word
from offset `-29024` (`-0x8e80`, unsigned: `0xffff7180`) relative to the
small-data area base register `r13`, and returns it in register 3.

| Item | Value |
| --- | --- |
| SDA base (`r13`) | `0x8041cf20` (set by `__init_registers` at `0x8000539c`) |
| Load address | `0x8041cf20 + (-0x8e80) = 0x804140a0` |
| Return | `r3` = word at `0x804140a0` (the global animation state pointer) |
| Side effects | None |
| Calls | None |

## Interpretation

The function returns a pointer to the global **animation state** structure.
The animation system manages skeletal animations, keyframe interpolation,
and pose data for characters and objects. This getter provides access to
the shared animation context.

## Remaining gaps

- Exact layout of the animation state structure
- Full caller graph
- Lifetime/initialization of the global pointer
