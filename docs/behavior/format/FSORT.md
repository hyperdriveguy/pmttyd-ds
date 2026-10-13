# fsort

**Address:** `0x80006c3c`  
**Size:** 1548 bytes (387 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`fsort` is a file sorting function that organizes file entries in a directory
structure. It manages file ordering, allocation, and directory entry
maintenance. This is used for save file management and file system operations.

## Key behavior summary

- Receives file entry pointer and sort parameters
- Compares file entries using comparison function
- Manages file allocation and deallocation
- Updates directory entries and file ordering
- Handles file system errors and edge cases

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | File entry pointer |
| +0x04 | 4 | Sort parameters |
| +0x08 | 4 | Comparison result |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x80006680` | File comparison |
| `0x8002e218` | Sequence/operation helper |

## Porting notes

This function is critical for save file management. For porting:
1. Preserve file entry comparison logic
2. Preserve file allocation and deallocation
3. Preserve directory entry maintenance
4. Handle file system errors and edge cases

## Remaining gaps

- Full file entry structure layout
- Comparison function details
- File allocation algorithm
- Directory entry format
