# marioStMain

**Address:** `0x8006faec`  
**Size:** 176 bytes (44 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`marioStMain` is the main field system update function, called once per
retrace. It orchestrates the complete field update pipeline including
sequence management, input processing, and all subsystem updates.

## Key behavior summary

- Calls `0x8002e210` (sequence check) — returns current sequence state
- If sequence == 2 or 4, calls `0x8002e210` again for validation
- Reads Mario state from `[r13 + 0x90b0]` (SDA global)
- Sets state flags: bit 9 (0x200) and bit 5 (0x20)
- Calls `0x8029d84c` (subsystem update)
- Stores results to `[r13 + 0x1a2c]` and `[r13 + 0x1a28]`
- Returns sequence state (0, 1, or 2)

## Called functions

| Address | Purpose |
|---------|---------|
| `0x8002e210` | Sequence state check |
| `0x8029d84c` | Subsystem update |

## Porting notes

This function orchestrates the entire field update. For porting:
1. Preserve the sequence check and validation logic
2. Preserve state flag setting (bits 9 and 5)
3. Preserve subsystem update coordination
4. Preserve result storage to SDA globals

## Remaining gaps

- Full sequence state definitions
- Subsystem update details
- State flag meanings and interactions
- Relationship to main pass order
