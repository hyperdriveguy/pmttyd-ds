# BattleCheckCounter

**Address:** `0x800fabcc`  
**Size:** 1444 bytes (361 instructions)  
**Evidence class:** B (inspected bytes/control flow in US binary)

## Overview

`BattleCheckCounter` checks and processes battle counter conditions. It
evaluates counter values, determines if counter-based actions should trigger,
and manages counter state transitions. This is the gatekeeper for counter-
based battle mechanics.

## Key behavior summary

- Reads counter values from battle work structure
- Evaluates counter conditions (zero, nonzero, threshold)
- Determines if counter-based actions should trigger
- Manages counter state transitions
- Coordinates with action and event systems
- Handles counter reset and initialization

## Critical offsets

| Offset | Size | Purpose |
|--------|------|---------|
| +0x00 | 4 | Counter value |
| +0x04 | 4 | Counter threshold |
| +0x08 | 4 | Counter flags |

## Called functions

| Address | Purpose |
|---------|---------|
| `0x800f8a08` | Counter validation |
| `0x800f895c` | Action trigger |
| `0x8009455c` | Sequence entry |

## Porting notes

This function is critical for counter-based mechanics. For porting:
1. Preserve counter value reading and evaluation
2. Preserve counter condition checking
3. Preserve counter-based action triggering
4. Preserve counter state management

## Remaining gaps

- Full counter condition types and thresholds
- Counter-based action trigger conditions
- Counter state transition rules
- Relationship to action and event systems
