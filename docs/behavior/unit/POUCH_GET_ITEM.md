# pouchGetItem

**Address:** `0x800d5520`  
**Size:** 1188 bytes (297 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`pouchGetItem` retrieves item data from the pouch inventory system. It searches
the item table by ID and returns the item structure. This is the primary item
lookup function for inventory management.

## Key behavior summary

- Receives item ID as argument
- Searches item table for matching ID
- Returns item structure pointer if found
- Returns null/sentinel if not found
- Handles special items (key items, badges, etc.)
- Coordinates with pouch state for inventory tracking

## Critical offsets (pouch item structure)

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Item ID |
| +0x04 | 4 | Item flags |
| +0x08 | 4 | Item quantity |
| +0x0c | 4 | Item category |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800d5eb0` | pouchGetPtr |
| `0x80094020` | State integration |

## Porting notes

This function is critical for inventory management. For porting:
1. Preserve item table structure and indexing
2. Preserve item ID mapping and lookup
3. Preserve special item handling (key items, badges)
4. Preserve pouch state coordination
5. Preserve null/sentinel return values

## Remaining gaps

- Full item table structure and size
- Item ID assignment and mapping
- Special item category definitions
- Item structure field meanings
- Relationship to equip and inventory systems
