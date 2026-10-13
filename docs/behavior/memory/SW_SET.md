# swSet

**Address:** `0x800858fc`  
**Size:** 64 bytes (16 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Disassembly

```asm
800858fc: 7c 64 2e 70  	srawi 4, 3, 5
80085900: 54 60 d8 08  	slwi 0, 3, 27
80085904: 7c 84 01 94  	addze 4, 4
80085908: 54 63 0f fe  	srwi 3, 3, 31
8008590c: 7c 03 00 50  	sub 0, 0, 3
80085910: 80 cd 90 b0  	lwz 6, -28496(13)
80085914: 54 85 10 3a  	slwi 5, 4, 2
80085918: 38 80 00 01  	li 4, 1
8008591c: 7c a6 2a 14  	add 5, 6, 5
80085920: 54 00 28 3e  	rotlwi 0, 0, 5
80085924: 7c 00 1a 14  	add 0, 0, 3
80085928: 80 65 01 78  	lwz 3, 376(5)
8008592c: 7c 80 00 30  	slw 0, 4, 0
80085930: 7c 60 03 78  	or 0, 3, 0
80085934: 90 05 01 78  	stw 0, 376(5)
80085938: 4e 80 00 20  	blr
```

## Behavior

`swSet` sets a story/work switch by index. It computes a word offset and bit
position from the supplied switch index, then sets that bit in the switch bank.

| Item | Value |
| --- | --- |
| Argument | `r3` = switch index |
| Switch bank base | `[r13 + 0x90b0]` (SDA global) |
| Word index | `(index >> 5) * 4` (each word holds 32 switches) |
| Bit position | `((index & 31) * 5) & 31` (rotated) |
| Side effect | Sets bit in `[bank_base + word_index + 376]` |
| Return | None (void) |

## Interpretation

The GameCube TTYD uses a bit-packed story switch system. This function sets
an individual switch by:
1. Dividing the index by 32 to find the word offset
2. Computing the bit position within that word (with an unusual rotation)
3. OR-ing the bit into the switch bank at offset 376 from the bank base

The rotation by 5 bits (`rotlwi 0, 0, 5`) is notable — it suggests the switch
indices are packed with a specific encoding that may differ from a simple
bitfield. This could be related to how the original game organizes its
persistent story state.

## Critical for porting

`swSet` is one of the most-called functions in the game (258 direct DOL calls,
15 REL references). It is the primary mechanism for persistent story state.
For porting, the key requirements are:

1. Preserve the exact index-to-bit mapping (including the rotation)
2. The switch bank at offset 376 must be saved/loaded with the game state
3. The bank base at `[r13 + 0x90b0]` must be initialized correctly
4. All 258 callers must use consistent index values

## Remaining gaps

- Total number of switch words in the bank
- Relationship between switch indices and story events
- How the bank is saved/loaded to/from the memory card
- Whether the rotation is intentional or an artifact of the original encoding
- Full caller index mappings for all 258 call sites
