# Message window force-close and repeat leaves

US G8ME01 disc0 revision0; static contracts, no execution, cadence or rendering
claim. Closes `msgWindow_ForceClose` (`0x80081424`, `msgdrv.o`, size `0xd0`, 58
lines), `msgWindow_Repeat` (`0x800814f4`, size `0x78`, 36 lines),
`evt_msg_close` (`0x800d2178`, `evt_msg.o`, 18 lines) and `evt_msg_repeat`
(`0x800d21a8`, size `0x54`, 27 lines) from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). None has a
definition in the pinned snapshot (rechecked with `tools/reference_source_gap.py`).
The window manager, playback and parser contracts remain the primary
message-system documents.

Common facts: `windowGetPointer(slot)` (`0x8007ad40`, `windowdrv.o`, 8 bytes)
returns the slot record, and the seven message slots are numbered 0..6.

## msgWindow_ForceClose(0x80081424, 58 lines)

`arg` (`r29`) is matched by name. The body loops `slot = 0..6`; the same three
early exits apply to each slot and continue to the next slot:

- `lhz [win+2]` (flags halfword) == 0 → skip.
- `lhz [win+0]` (state halfword) == 0 → skip.
- `lwz [win+40]` (handle) == 0 → skip.
- `lwz [[handle]]` dereferenced twice == 0 → skip.
- `strcmp([[handle]], arg)` (`0x8026d464`) != 0 → skip.

On a match:

1. `state = 7` is stored at `[win+0]` — the forced-close state.
2. `obj = [[handle]]`; `flags = word[obj+4]`; the bit selected by
   `rlwinm. 0,0,0,28,28` is `mask 0x8`.
3. If bit 3 is **clear**: `BtlUnit_ChangeStayAnim(word[obj+0xF224])`
   (`0x80127564`, `battle_unit.o`).
4. If bit 3 is **set**: `npcSetStayPose(obj+0xF204)` (`0x80049644`,
   `npcdrv.o`).

Only the first matching slot per pass is closed; the loop continues so a later
slot with the same name can also be closed. No value is returned.

## msgWindow_Repeat(0x800814f4, 36 lines)

Always operates on slot 0 (`windowGetPointer(0)`):

1. `state = 1` at `[win+0]` (`sth 1`).
2. `flags |= 2` at `[win+2]` (`lhz`/`ori 2`/`sth`).
3. `rec = [[win+40]]`; `pair = (word[0x80415fb0+0x38], word[0x80415fb0+0x3C])`
   is written to `[rec+0x30]`/`[rec+0x34]` and again to `[rec+0x18]`/`[rec+0x1C]`.
   The global word is at `r13−28496` (`0x80415fb0`).
4. `[rec+0x20]`, `[rec+0x24]`, `[rec+0x28]` and `[rec+0x10]` are zeroed.

## evt_msg_close (0x800d2178, 18 lines)

`args = word[entry+24]`; `r3 = evtGetValue(args[0])`; `msgWindow_ForceClose(r3)`;
return 2. The single operand is resolved, so the event selects the window by
its resolved name/id.

## evt_msg_repeat (0x800d21a8, 27 lines)

1. `entry` in `r31`. When the incoming `r4` (call-instance argument) is nonzero,
   request `msgWindow_Repeat(word[entry+376])` — the slot stored at `+0x178`.
2. `r3 = windowCheckID(word[entry+376])` (`0x8007ad50`, `windowdrv.o`).
3. Return 2 when that result is nonzero, otherwise 0.

The nonzero-return convention means the repeat is still pending; 2 means it
completed in this pass.

## Not established

- Caller cadence for all four bodies, the seven-slot allocation's owners, and
  the on-screen consequences of state 7.
- The identity of the global at `0x80415fb0` and of the `[rec+…]` fields beyond
  their offsets.
- Whether `strcmp` compares names, keys or tags.
