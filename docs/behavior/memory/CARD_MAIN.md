# cardMain

**Address:** `0x800b2508`  
**Size:** 1692 bytes (423 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`cardMain` is the save/load state machine for the GameCube memory card. It
manages save file operations including reading, writing, verifying, and
recovering save data. This is the persistent state management system.

## Key behavior summary

- Manages memory card slot state and file operations
- Handles save file creation, reading, writing, and deletion
- Manages backup save files for recovery
- Handles card errors, corruption, and recovery
- Coordinates with the sequence system for save/load transitions
- Manages checksum validation for save integrity

## Critical offsets (card work structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Current state |
| +0x04 | 4 | Slot ID |
| +0x08 | 4 | File handle |
| +0x0c | 4 | Operation result |
| +0x10 | 4 | Checksum |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x802ac744` | Memory card operation |
| `0x800d3d30` | pouchAddHP (HP check) |
| `0x80094020` | State integration |

## Porting notes

This function is critical for persistent state. For porting:
1. Preserve save file format and structure
2. Preserve checksum validation algorithm
3. Preserve backup file recovery mechanism
4. Preserve error handling and recovery paths
5. Preserve card operation state machine
6. Preserve slot management and file handles

## Remaining gaps

- Full save file format and layout
- Checksum algorithm details
- Error recovery state machine
- Card operation timing and coordination
- Multi-slot management
- Save file corruption detection and repair
