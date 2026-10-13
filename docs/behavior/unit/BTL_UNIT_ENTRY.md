# BtlUnit_Entry

**Address:** `0x80129644`  
**Size:** 1716 bytes (429 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BtlUnit_Entry` is the primary battle unit entry and initialization function.
It sets up a new battle unit with all associated data including stats,
resources, events, and display information. This is the main unit creation
path for battles.

## Key behavior summary

- Allocates and initializes unit structure
- Sets up unit stats (HP, FP, attack, defense)
- Initializes unit resources and inventory
- Sets up event scripts and callbacks
- Initializes display and pose data
- Coordinates with pouch and status systems
- Handles unit type and category setup

## Critical offsets (unit structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Unit flags |
| +0x04 | 4 | Unit type |
| +0x08 | 4 | Current HP |
| +0x0c | 4 | Max HP |
| +0x10 | 4 | Current FP |
| +0x14 | 4 | Max FP |
| +0x18 | 4 | Attack value |
| +0x1c | 4 | Defense value |
| +0x20 | 4 | Event script pointer |
| +0x24 | 4 | Display data pointer |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800374ac` | Resource allocation |
| `0x80057878` | State cleanup |
| `0x8007ad50` | Unit initialization |
| `0x800d3d30` | pouchAddHP |
| `0x8002e218` | seqMain (battle transition) |
| `0x8012984c` | Event trigger |
| `0x8011ace8` | State update |

## Porting notes

This function is critical for battle unit creation. For porting:
1. Preserve unit structure layout and initialization
2. Preserve stats setup (HP, FP, attack, defense)
3. Preserve resource and inventory initialization
4. Preserve event script and callback setup
5. Preserve display and pose data initialization
6. Preserve pouch and status system coordination

## Remaining gaps

- Full unit structure layout and field meanings
- Stats calculation and initialization rules
- Resource and inventory setup details
- Event script and callback registration
- Display and pose data format
- Pouch and status system integration
