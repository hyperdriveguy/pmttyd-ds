# Battle wait/camera/status-switch leaves

US G8ME01 revision0; static contracts, no execution or gameplay claim.
Closes `btlevtcmd_StopWaitEvent`, `btl_camera_nomove_y_onoff`,
`btl_camera_nomove_z_onoff`, and `BattleStatusWindowEventOff` in
[UNADDRESSED_BINARY_FUNCTIONS.md](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

## btlevtcmd_StopWaitEvent (8010a1fc, 112 bytes, 34 lines)

sha256 `fc7b3d2b31bd…` (`evidence.json`, 4 annotated sites); `us-usage.json`:
`battle_event_cmd.o`, 3 data-word candidates, 56 REL imports, fingerprinted.
Full order:

1. Entry retained in r31; `evtGetValue([[entry+24]+0])` → `BattleTransID` →
   `BattleGetUnitPtr([r13+7280], id)` (battle word positional, as before).
2. id = word at [unit+656]; `cmpwi id, 0`; if zero, skip to the return.
3. Else `evtDeleteID(id)` (0x80036664, complete /9c in [event
   deletion](../event/EVENT_DELETE_AND_COPYBACK.md)) — called with the loaded id,
   since the load overwrote r3 — then store 0 to [unit+656].
4. Return 2.

Net effect: a nonzero wait id parked at unit+656 is deleted once and the slot
cleared; a zero slot is left untouched. No evtSetValue destination exists in
this body. Whether the parked id is always an event id (versus some other
 nonzero token) is not established by this body — the deletion call takes it
as one.

## Camera no-move switches (8012b2ac y, 8012b21c z; 56 bytes each, 20 lines)

Two bodies differing only in the mask bit. `evidence.json` rows
(`af51cb7c9d78…`, `4b552f057b0a…`, 0 branch sites — there are no calls);
`us-usage.json`: `battle_camera.o`, 2 link sites each, fingerprinted. Full
order, with the PPC `blt`-shaped guard (`cmpw r3,0` / `bclr 12,0`):

1. W = word at [r13+7280] (positional). Dead load: word at [W+10076] into r0,
   consumed by nothing — only the `cmpw 3, 0` result matters.
2. If incoming r3 < 0 (signed): return immediately, no store. The return
   value is whatever r3 held; these are plain helpers, not event callbacks
   (no entry use, no return-2).
3. Else if incoming r4 == 1 (`cmpwi`, signed): clear bit 3 (value 8, y) or
   bit 4 (value 16, z) in the word at [W+10068] (`rlwinm` keep-mask).
4. Else: set the same bit (`ori 8` / `ori 16`).

So: negative first argument preserves the word; otherwise the second
argument selects clear (==1) versus set. The mask bits (8/16) and the
[W+10068]/[W+10076] words are reported positionally — their camera meanings
are not grounded in prose.

## BattleStatusWindowEventOff (800f75f8, 44 bytes, 17 lines)

`us-usage.json`: `battle.o`, size 44, 1 link site, not fingerprinted; no
`evidence.json` row. Full body: ignore incoming registers; store 0 to
[[r13+7280]+10056]; `bl 0x800f7514` (absolute target with no annotation, no
evidence row, no prose mention — unresolved); return whatever that call
returns (no `li 3,2`, unlike every event callback in this series).

This is a plain helper, not an event callback: it takes no entry, writes one
fixed word, and forwards the callee's return. The callee's identity is the
open item, not the store.

## Not established

Wait-id provenance, camera-word meanings, the 800f7514 callee, cadence, and
any on-screen effect.
