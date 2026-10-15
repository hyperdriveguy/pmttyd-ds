# Native retained map-NPC history and area-root admission

Implements the admitted original
[history/definition flow](TAGGED_MAP_NPC_HISTORY_GATE_AND_DEFINITION_SOURCE.md)
and composes it with [tagged registration](NATIVE_TAGGED_MAP_NPC_REGISTRATION.md).
Source is US G8ME01 disc0 revision0, read-only ISO; this is a portable C++
mechanics implementation with a separate original-address RAM adapter.

`preparePackedMapNpcHistory` captures the actual field-work pointer and first
matching map-definition row (or the actual null-name terminator), selects the
first matching/empty retained history record, promotes its five captured words,
and executes the original group/flag-driven clears. Backward shift reads/stores
retain pair/pair/late-word order. It captures the final flag1 gate once; each
row later reads the retained head mask afresh. `packedMapNpcOrdinalMask`
implements low8 ordinal then PPC low6 shift with bit5 producing zero. The native
admission rejects full64/no-match before source's out-of-history accesses; it
never invents eviction. It also rejects overlong map/history names, missing
source table/string/field bytes and source-input overlap with retained F before
history effects. It is bounded to the actual199-row US definition table.

F must already be actual writable retained field-battle storage. Admission
reads do not initialize history or claim original libc trace callbacks. Original
unbounded string overflow and unsupported full-history accesses are excluded
native domains. No original save-history persistence is claimed.

`FieldMapNpcBootstrap` owns the mapped parsed-input adapter and an immutable
copy of its source records. The caller retains its NPC binding and address
facade until after cursor destruction. Preparation executes history once;
advance retains original row ordinal and a captured per-row gate result across
resource holds, and never repeats an already registered row. Missing poses,
capacity, numeric/RNG service or source data hold registration. Skipped rows
still increment ordinal. Completion alone admits the following area root.
The compiled tag adapter does not execute the original parser or resource-load
opcodes; it supplies their extracted original row values explicitly.

`FieldAreaEvent::prepareMapNpcs` binds Game's retained NPC owner into the same
VM that will run the original map initializer. A cold owner runs actual npcInit
once. An existing owner needs actual room cleanup and caller GP+168 pointing
at that owner's field-work identity; active actors hold preparation. No reset,
cleanup or previous-map state is fabricated. Declaration order retires cursor,
then binding, then facade. Failed preparation retains its snapshot/binding and
blocks root creation; discard that area facade before correcting inputs.

The caller supplies the actual packed GP state, source area/map string
addresses, parsed resources and explicit native guest allocation identities.
`advanceMapNpcs` takes the existing explicit pose-clock/RNG providers rather
than converting event-counter ticks into guessed milliseconds. Original
`appendAreaInit` waits for completed preparation and the identical input GP
snapshot. It does not reapply that snapshot over npcInit's newly stored GP+168.
Legacy supplied-door continuation roots cannot bypass a requested map stage.
The prior direct-root API remains available to isolated continuation fixtures.

History tests cover actual gor02 terminator/flags0, retained masks, repeated
head, new slot, index63 promotion, equal/different groups, flag2/flag4 policy,
low6 shifts, fresh mask reads and malformed/full-table admission. Bootstrap
tests compose all15 original tag rows and real pose banks in the actual area VM,
hold at missing resources, resume without duplicate allocation, preserve
original skipped ordinals, capture an unset gate across a resource hold, retire
adapter RAM and reject an uncleared second area. These are source-data fixtures,
not Dolphin runtime traces or evidence of complete normal-game rendering.

Remaining owners: original outer map-change/load/parser ordering, actual room
cleanup and history persistence, shared later npcEnt battle0 RNG, field Main/
facing/query/display/collision, and production Game bootstrap invocation.
