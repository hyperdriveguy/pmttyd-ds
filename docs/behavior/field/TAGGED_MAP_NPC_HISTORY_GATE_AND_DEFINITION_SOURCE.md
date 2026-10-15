# Tagged-map NPC history gate and definition source

US G8ME01 disc0 revision0, read-only static source recovery for
[EVT-01](../../requests/EVT-01.md). This closes the definition/history inputs
of the [tagged registration store span](TAGGED_MAP_NPC_REGISTRATION_STORE_ORDER.md).
It does not claim complete bootstrap, parser execution, save persistence or
native area initialization. Complete setupDataBase evidence is
`artifacts/tagged-map-npc-setupDataBase.txt`; source boundaries/fingerprint are
also in the [registration contract](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md).

## Actual getter and definition table

setupDataBase80084f24 captures argument0 area name and argument1 map name.
It calls fbatGetPointer8004829c at80084f44. This is a field-battle getter,
not a geometry map search: load GP through80415fd0, then return GP+168.
Capture that result F; r29=F+4c and r26=F+60 are formed immediately.

The definition table is _map_attr_data8030f668, size954, stride12.
The source lis8031 plus signed addif668 produces8030f668. Each row has
name pointer+0, group word+4, flags word+8. At84f5c the source reads each
name, stops at null or first strcmp match with the captured map name, and
otherwise advances12. Duplicate names therefore use their first occurrence
(the original table includes repeated eki names). An absent map uses the
actual terminating row; there is no fabricated fallback record.

Original table SHA256:
5c76b1a7b2073b6ddbfbeac319e786c9b44b77330e9bd7c062e0f5b8661dd4cc.
It contains198 named rows followed by8030ffb0={0,ffffffff,0}.
There are **no gor rows**, including gor_02. Consequently gor02 uses that
source terminator: group-1 and flags0. Its NPC history gate is disabled by
actual source definition data, not by assuming an empty history mask.

## Retained history records and promotion

F owns64 records beginning F+4c, stride0x14. A record has16 bytes of map
name followed by a word mask at+10. These are retained field-battle work
bytes, not GP saved-switch words or a host map keyed by room ID.
npcInit clears the actual0x580 F allocation at startup. npcReset clears
actor arrays/counts and mode but does not clear this history. See
[native registry lifetime](NATIVE_RETAINED_NPC_REGISTRY_OWNER.md).
Original save serialization or all transitive history writers are not
established by this local recovery.

At84f80 start index0. For each current slot, compare its map-name bytes
with original empty string804207a8. If empty, clear that entire20-byte slot,
strcpy the captured map name into it, and stop. Otherwise compare with the
captured map name; first equality stops. A mismatch advances one slot/index,
up to the source bound64. Strings have no local16-byte capacity guard.

For a selected index>0, capture all five words of that record into stack
scratch SP4c..5c (source84ff4..85028). Shift preceding records one slot
toward higher indices, processing backward; the generated loop is unrolled
in groups of four. Within each shifted record: load words0/4, store0/4;
load8/c, store8/c; load10, store10. At85120..44 publish scratch into head
with ordered operations: load scratch0/4, store head0, load scratch8,
store head4, load scratchc, store head8, load scratch10, store headc,
store head10. This is not an unordered host-container reordering contract.
Index0 performs no promotion copy.

The r29 gate pointer remains F+4c throughout, so the eventual gate reads
the head mask F+5c. The r26 pointer remains F+60: after promotion it names
the second history record. Do not replace that second record with an assumed
previous-map variable, especially when index0 selected the already-current
head without a move.

If all64 slots are occupied and none matches, the release source proceeds
with index64 rather than implementing a safe eviction. Its next accesses
reach outside the64-record history domain (although still within parts of
the0x580 field-work allocation). A bounded native implementation must hold
before that unsupported domain, not silently invent last-slot eviction.

## Group-driven invalidation

At85148 freshly read the originally selected definition's flags+8.
If flag1 is clear, skip directly to85200. Otherwise perform two fresh
definition scans: captured current map, then the string at retained F+60.
Read the resulting current group+4, then previous group+4 and compare.

- Different groups: clear F+4c for0x500 bytes (all64 records), then strcpy
  current map name into F+4c.
- Equal groups: freshly read the original selected definition's flags;
  flag2 set preserves history. With flag2 clear, flag4 set clears0x4d8
  bytes starting F+74, preserving the first two20-byte records. Otherwise
  preserve history.

These actual source tests do not give independent gameplay meanings to the
group number or flag bits beyond their recovered effects. For gor02 the
original flags0 bypass this entire conditional invalidation region, while
ordinary selection/promotion above still runs.

## NPC gate, ordinal and proven writer

At85200 freshly read selected definition+8, initialize ordinal0 and gate
boolean0, then capture gate boolean1 only if definition flag1 is set.
This overwrites the register that previously held F; the captured boolean
is not a fresh field-mode or GP system-flag query.

After parsing each complete Npc block and parsePopNext, at85314 test that
captured boolean. If zero, admit the registration span regardless of mask.
If enabled, take low8 bits of current ordinal, apply PowerPC slw to1, read
retained head mask at r29+10 (F+5c), AND, and skip registration when nonzero.
Ordinal increments at85560 on both admitted and skipped rows.

PowerPC slw uses the low6 shift-count bits; counts32..63 produce zero.
Thus ordinals0..31 address mask bits,32..63 do not skip,64 repeats shift0,
and the behavior is not modulo32. Preserve the byte314 publication in each
admitted actor and the original ordinal order, independently of npcEnt order.

A directly verified mask writer is _npcDead80046a2c/4c. It ORs NPC+140
with10, NPC+1d4 with4, stores its argument as halfword NPC+31c, then tests
NPC mainflag4. If clear, return. Otherwise load fresh GP, read actor ordinal
byte314, obtain F from GP+168, calculate source slw(1,ordinal), read F+5c,
OR and store F+5c at80046a70. _npcGroupDead800466d0/35c has corresponding
per-member ordinal reads/writes; it is a separate group/death owner. Ordinary
npcDelete is not a replacement for this mask writer. These are source body
facts, not a complete consumer/death cadence or runtime trace claim.

## Actual parser data and surrounding bootstrap

setupDataBase checks source database ptr8041ea64 at85218; zero skips parsing
to85578. Nonzero loads source size8041ea60 and calls parseInit80084f08.
These globals are produced by setupDataLoad80085648, which clears ptr,
formats the actual root/m/MAP/s path, tries arcOpen800782e4, then original
DVD/allocation/read services when needed. Its special muj_20 branch remains
separate; resource availability must not become an invented nonzero pointer.

The direct DOL caller is seq_mapChangeMain at800081b0: it supplies original
_next_area8041e5e8 and _next_map8041e5c8. Prior setupDataCheck8008558c and
fade/timing gates run before it. Afterward800081bc calls mapDataPtr80039278
with the next map name, reads its event field and constructs the map root.
The geometry mapEntry8002a46c/mapGetJoints8002b120 path is a separate map
resource owner; no getMapData/mapSearch call supplies this history gate.

A minimal gor02 row-admission provider may use actual definition lookup to
establish gate=false without inventing mask0. A complete upstream provider
must additionally own source F/history selection/promotion/invalidation,
actual parsed data, bootstrap ordering and root creation. Existing retained
F storage can represent history, but missing state must not be replaced with
an assumed previous map or fresh zero bank. Original save/load lifetime,
full-capacity behavior and additional area-specific flows remain explicit
frontiers until their actual owners are recovered and admitted.
