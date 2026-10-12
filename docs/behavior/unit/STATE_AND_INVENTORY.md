# Saved work, party resources and inventory mutations

US G8ME01 revision 0; updated 2026-10-12. Addresses are original US RAM
addresses. Evidence classes/identity: [baseline](../../ORIGINAL_BEHAVIOR.md).
This file documents B findings from original functions; no original gameplay
recording or save-file compatibility is claimed.

## Persistent state is not one flag array

The saved-work driver has separate global byte/flag and local byte/flag stores.
The pinned [public swdrv reconstruction](https://github.com/doldecomp/ttyd/blob/62131fc31866f85a12e4817a8dc37f0c25eb5f8c/src/driver/swdrv.c)
is present but its object is declared NonMatching. The following offsets and
clear lengths were checked in the US functions, rather than assumed from it.

| Store | US global-work offset | Size established by initialization | Access |
| --- | --- | --- | --- |
| Special global work index 0 | `gp+0x174` | One 32-bit word | `swByteGet/Set(0)` |
| Global flags | `gp+0x178` | `0x400` bytes = 8192 bits | `swGet/Set/Clear` |
| Global byte work | `gp+0x578` | `0x800` bytes | Nonzero `swByteGet/Set` indices |
| Local flags | `gp+0xd78` | `0x40` bytes = 512 bits | `_swGet/Set/Clear` |
| Local byte work | `gp+0xdb8` | `0x400` bytes | `_swByteGet/Set` |

`swByteGet`, `0x8008582c`, branches on index 0: that case loads a word from
`+0x174`; other cases load an **unsigned byte** from `+0x578+index`.
`swByteSet`, `0x80085850`, similarly stores a word for index 0 and truncates to
the low byte for nonzero indices. The array's byte at index 0 is not what the
special accessor returns. A universal byte-based story-state implementation
would truncate the special index and be incorrect.

`_swByteGet` (`0x80085744`) always loads an unsigned byte at `+0xdb8+index`;
`_swByteSet` (`0x80085754`) stores a byte. For nonnegative valid indices, flag
access uses word index `i/32` and mask `1 << (i%32)`. The getter returns 0 or 1,
not the raw bit mask (`swGet`, `0x800858b4`). No array-bound validation is present
in these leaf accessors; a port may reject malformed extracted script operands,
but must preserve valid original index and width semantics.

`swInit` (`0x80085980`) clears all four arrays using the sizes above, then zeros
the special index-0 word. `swReInit` (`0x8008593c`) clears only local flags and
local bytes. These describe the routines themselves: call sites that choose
which initialization runs at map changes, new game, loading and chapter changes
still need a full lifecycle trace. Do not equate the word "local" with a guessed
room lifetime or erase global progress when entering a room.

## Pouch resources: storage widths and asymmetric setters (B)

The pouch pointer is loaded from `r13+0x1be0`. The following relative fields
are halfwords read back as signed values by these routines:

| Relative offset | Resource |
| --- | --- |
| `0x70`, `0x72` | Mario current/max HP |
| `0x74`, `0x76` | Current/max FP |
| `0x78` | Coins |
| `0x7a`, `0x7c` | Current/max AP resource |
| `0x8e`, `0x90` | Additional max HP/FP copies written by max setters |

Do not automatically label the max copies as a particular badge-independent
quantity until the revise functions and their consumers have been traced.

`pouchSetHP` (`0x800d3d00`) stores the low 16 bits, reloads them as signed and
caps only above max HP. It does **not** clamp negatives to zero. `pouchSetFP`
(`0x800d3c68`) has the corresponding upper-only rule. By contrast,
`pouchAddHP` (`0x800d3d3c`) sign-extends its delta from 16 bits, adds to signed
current HP, stores/reloads the narrowed halfword, then clamps below zero and
above max. It returns the resulting HP.

Examples derived from these instructions, assuming max HP 10: SetHP(-1)
retains -1; AddHP(-1) from 0 returns 0; SetHP(12) stores HP 10.
These are accessor semantics, not claims that ordinary gameplay deliberately
passes negative HP to the setter. Keep legal-call behavior separate from parser
validation and corrupted-save policy.

`pouchSetMaxHP` (`0x800d3ce8`) writes max HP and its `+0x8e` copy; it neither
heals nor immediately caps current HP. `pouchSetMaxFP` (`0x800d3c50`) likewise
writes `+0x76` and `+0x90`. A higher-level level-up sequence may heal, but that
does not make healing part of these leaf functions.

`pouchSetPartyHP` (`0x800d3ca4`) addresses `pouch + 14*partyIndex`, writes HP
at record `+6`, and caps above the signed maximum at record `+2`; it has no
lower clamp on this path. Partner resources are not aliases of Mario's HP.

`pouchSetAP` (`0x800d3bc4`) and `pouchAddAP` (`0x800d3c00`) clamp after
halfword storage to zero and current maximum. The numerical display scale and
audience-driven AP economy are not established by these setters. `pouchSetCoin`
(`0x800d3dd4`) and `pouchAddCoin` (`0x800d3e18`) clamp to 0..999 after narrowing;
the latter returns the final count. Extreme overflow inputs can wrap before
clamping. Replacing this with unbounded host arithmetic would change those
edges, even though their valid-game reachability is not yet shown.

## Carried/stored capacity (B)

`pouchGetEmptyHaveItemCnt` (`0x800d5614`) calls `pouchCheckItem(1)`. A zero
result selects 10 slots; nonzero selects 20. It counts zero signed halfwords
from `pouch+0x192` over that active capacity. The condition is possession of
numeric item ID 1, not a prototype story flag. Mapping that ID to display text
belongs in the item database, not a replacement hardcoded name in the accessor.

`pouchGetEmptyKeepItemCnt` (`0x800d5570`) checks 32 halfwords starting at
`pouch+0x1ba`, implemented as four iterations with eight checks each. Empty is
exactly ID 0. Do not confuse this stored-item pool with the carried-item capacity.

## Acquiring an item (B, generic path plus selected special cases)

`pouchGetItem` (`0x800d50cc`, size `0x4a4`) includes immediate-resource and
equipment-special cases before choosing an array. Selected verified branches:

- ID 121 increments coins by one if below 999 and returns success, including
  at the cap (`0x800d50e0`–`0x800d510c`).
- ID 123 increments Mario HP by one if below max; calls the current-partner
  selector and, if nonzero, increments that partner's HP if below its max
  (`0x800d5140`–`0x800d5198`). Exact selector policy remains a separate contract.
- ID 124 increments FP by one if below max and returns success
  (`0x800d519c`–`0x800d51c4`).
- IDs 240..338 set global saved flag `itemID-112` **before** testing inventory
  capacity (`0x800d52e8`–`0x800d52fc`). This acquisition/seen side effect is not
  rolled back if later array insertion fails in this function.

Additional special cases for ID 122, equipment upgrades and other resources
are visible in the disassembly but not fully interpreted here. Do not route
every numeric ID through generic storage without accounting for those branches.

For IDs that reach generic selection (`0x800d5300`–`0x800d5394`):

| Numeric ID range | Selected array | Capacity | Companion array |
| --- | --- | --- | --- |
| 121 ≤ ID < 236 | `pouch+0x192` | 10 or 20 using item-1 possession | None |
| 1 ≤ ID < 121 | `pouch+0xa0` | 121 | None |
| 240 ≤ ID < 339 | `pouch+0x1fa` | 200 | `pouch+0x38a`, shifted alongside |
| Other | None | Rejected, return 0 | None |

This table is **after** special-case handling; for example ID 121 returns
earlier and does not become a carried entry. Gaps 236..239 are rejected by the
generic path. Display names and full usable-item categories still need database
mapping and validation against the original data.

The insertion loop scans backward from capacity-1 for a zero slot. If none
exists it returns 0. If an empty slot at index j exists, it shifts indices
0..j-1 one place to the right and writes the new ID at index 0. Where the
companion array exists, it shifts alongside and sets its new index 0 to zero
(`0x800d53a8`–`0x800d5538`). It returns 1 on successful insertion.

Therefore order is observable and cannot be replaced by an unordered set.
Holes and duplicate instances matter. The companion array's gameplay meaning
must be confirmed with equip/remove functions before using it as a native badge
schema. This function alone does not enforce a general uniqueness rule.

## Save/load boundary and required remaining work

The native 32-byte checkpoint is a slice-specific adaptation. It cannot carry
the complete original saved-work arrays, all inventory instances, equipment,
partners, mail, shops, side quests or chapter progress. This document specifies
in-memory mutations, **not** the original memory-card record layout.

Required next evidence: original new-game pouch values and party records;
revise/equip/unequip and BP accounting; removal/sort/deposit/withdraw order;
shop pricing and purchase rollback; special item branches; map reset call sites;
card-slot state machines, checksums, backup behavior, damaged/missing-card flows;
and all original persistent switch meanings tied to event writers/readers.

P conformance cases should distinguish SetHP from AddHP, max-change from healing,
ID-1 capacity transition, full-array failure, badge flag mutation before failure,
duplicate entries, and child-local state versus saved-work state. Test and observe
these before claiming equivalent progression or saves.

Reproduce selected anchors with `python3 tools/verify_behavior_contracts.py`.
Regenerate each function's inspection ELF with `tools/inspect_dol_function.py`
and the external US symbol CSV. All original function bytes stay ignored.
