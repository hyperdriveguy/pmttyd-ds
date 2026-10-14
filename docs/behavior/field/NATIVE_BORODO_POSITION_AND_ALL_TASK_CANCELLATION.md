# Native Borodo position and original cancellation tables

Source evidence: [actual actors and door-copy owner](TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md),
[cancellation callback](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).

PMNPC009 appends an exact, NUL-padded 32-byte Shift-JIS event name and native
initialization profile to each actor. Older catalog versions remain readable.
NPC names are validated and unique within overlapping room/story/interior
registrations; map objects retain their separate ASCII source-name field.
This permits future source event packs to address actual retained actors without
renderer-specific character branches. The new atlas is reduced to512x256
RGBA8/512KiB; source UV ratios remain unchanged. A2MiB atlas exceeded the
current resident room budget in the first native capture.

The original gor_02 ボロドー２ uses c_borodo_n, original S_1/T_1/W_1/R_1 clips
and source tribe dimensions. Its init profile waits for known GSW1420; quest4
also requires known GSF5406. The source branches initialize (-475,0,-180), or
(0,-1000,0) for quest4/5406=true. Init runs once per retained field instance.
Changing a switch later does not replay it over a live event position.

The board's literal NpcPosition callback looks up the exact source name and
writes the same native body's previous/current and movement roots, in original
engine units. Subsequent simulation and drawing retain that position. Pose,
direction, flags and clocks are preserved. Missing/uninitialized/script-owned
actors remain pending; there is no synthetic actor or successful no-op. The
[retained-name provider](NATIVE_RETAINED_NPC_FLAGS_AND_DRAW_SUPPRESSION.md)
now uses active1 independently of draw800. Catalog coverage still controls
registration construction/pruning, so full original registered lifetime is open. Generic evtGetFloat,
event-me/party lookup, Garf and house05 door-copy ownership remain external.
Borodo's original talk event is recorded but is not yet executed natively.

Cancellation reads all30 list pointers from the immutable original PMEVT
irai_item_table at 0x90545cd4. Each signed list terminates at -1; malformed
pointers, missing terminators and out-of-domain IDs reject the asset load
atomically. The runtime borrows that same program's ordered list rather than
maintaining a second quest-to-item mapping. The original nonempty lists are:

| Quest | Original key item |
| --- | --- |
| 1 | 49 |
| 2 | 95 |
| 13 | 107 |
| 16 | 110 |
| 17 | 98 |
| 19 | 73 |
| 23 | 99 |
| 25 | 109 |
| 29 | 100 |

The other21 lists are genuinely empty. All30 reset GSF5405..5421 and
GSW1469..1474, then remove each listed key while a fresh original pouch count
is positive. Empty lists need no pouch service. The original caller owns its
30-coin fee, active-quest reset and independently launched end child.

Host coverage includes unknown init branches, below-map placement, explicit
position persistence, previous/current agreement, preserved presentation state,
invalid lookup/coordinates, all30 actual counter cancellation paths, repeated
key removal and preservation of other keys/resources. Native raster and timing
evidence must come from the isolated captures recorded in PROGRESS.md.
