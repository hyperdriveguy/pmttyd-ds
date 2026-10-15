# Field entry metadata and ordered event leaves

This guide covers Bero (entry/exit) metadata, normal entry geometry, the retained
initial-position child, case dispatch, map flags, mutable door records and Mario/
previous-partner preparation. See [field motion](field-motion.md) for movement
and [the event VM](event-vm.md) for guest memory, operands and scheduling.
These helpers expose specific original callbacks; they do not complete room
loading, NPC teardown or the full original map-change sequence.

## Metadata and state ownership

`FieldBeroInfo` retains immutable converted original records, selected incoming
name, registered entries and switch values. Each record spans 15 words (60 bytes).
The private catalog header uses little endian fields; original event/data words
remain guest big endian. Loader limits are native admission bounds and do not
claim original code performed those checks. A candidate catalog replaces the
shared catalog only after validation, including the original initial-position
script's boundary words and actual adjacent scanner-prefetch word.

Room synchronization also checks catalog generation. Invalidation clears the
selection/registration, but deliberately keeps `currentNumber_`: original
`get_info` does not reset that BSS word. Registration validates the actual map
name/node lookup before publishing entries and resetting Bero execution bits.
Repeated original registration is separate from once-per-area initialization.

An explicit length at record offset 24 is signed engine-space distance. The -1
sentinel selects 30 for kinds 0, 3 and 0xf00, otherwise 60. Direction 20000 means
AUTO; kinds 0/1 test eight original name prefixes and use direction 8 if none
matches. Other kinds retain 10000 (unspecified). Case type -1 uses the original
kind-specific defaults; unlisted kinds retain -1.

`infoByIndex` writes LW3..14. `intoInfo` writes LW1..8, selecting fallback for an
unmatched/empty incoming name. GP bit 2 plus an empty incoming name instead
selects entry zero and publishes its actual name pointer. The ordinary incoming
pointer is native guest alias `0x85000000`, including its trailing NUL. It is
never a host pointer. The XYZ field stays a guest pointer into the selected
record; null callbacks alone acquire the original default event.

`FieldBeroExec` separates stored bits from a known-bit mask. Unknown transition
history is not a clear bit. Operation zero sets a mask, any other operation
clears it; both establish that mask as known. Only actual completed `get_info`
restores all-known zero.

## Normal position and retained child

`fieldBeroNormalPosition` uses engine units. X=100000 requests a named geometry
center and replaces all three coordinates. Zero distance bypasses trig/floor
services entirely. Otherwise, direction selects eighth-turn headings and mode 1
reverses by 180 degrees. The routine backs start up by 30, casts down 1,000,000,
advances finish by 30+distance, and puts recovery 37 above finish. An unavailable
floor service holds; an acknowledged miss keeps the existing Y. Repeated trig
calls and explicit single-precision boundaries preserve operation order.
Supplied trig/FMA adapters still determine numerical compatibility.

The start publication is not a plain integer cast. It truncates single(value*1024),
subtracts 230000000 with 32-bit wrap, then decodes according to the EVT literal
threshold -220000000. Values outside that encoded domain remain integer values.
The result can differ from the input after truncation and float rounding.
Exceptional conversion holds rather than clamping to an invented position.

`FieldBeroInitialEvent` runs the unchanged original `initial_pos` in a private
retained VM, admitting only normal kind 0. It snapshots the incoming/catalog
owner, refreshes world/geometry services each tick, and holds unsupported
callbacks. Guest GP/heap addresses are native VM allocations; the 243MHz bus
clock supplies simulation timestamps, not rendering cadence.

Normal-position publication stores six raw IEEE globals in original descending
address order, then reacquires Mario for recovery writes. The motion-25 check
uses the earlier captured Mario; a subsequent write reacquires Mario again.
Those lookups cannot be collapsed even if a normal caller returns one owner.
The final camera reset snapshots the completed local bank before event deletion
clears entry storage. Completion requires both retirement and that captured bank.
Mapped globals are not readable as results until geometry actually published them.

## Position and preparation callbacks

Mario position lives at source offsets +0x8c/+0x90/+0x94, partner position at
+0x58/+0x5c/+0x60. Each axis fetches its operand, resolves it and stores its IEEE
bits before the next axis. A setter can mutate subsequent inputs. Party selector
is raw: zero chooses the current partner, any nonzero chooses literal slot 1.
A real null actor completes without reading position operands; unavailable native
providers hold before the source effects they cannot support.

`evt_mario_get_pos` captures Mario before mode resolution. Nonzero mode returns
Y-1, reading Y before its destination operand; zero mode reads destination first.
Each output goes through `evtSetFloat`. Harbor story gates then deliberately use
integer `evtGetValue` comparisons on LW2 (Z<=-100) and, before battle, LW0
(X>=-25). Comparing unquantized world floats would change those original IFs.

Camera-road reset uses slot 4, writes the target, copies it to the second vector,
and copies unshifted Mario XYZ into both previous roots. Its target Y adds the
original single-precision .01. Road reset runs before shift reset.

Key suppression is a wrapping byte depth at Mario+0x39 plus main flag 8. KeyOff
increments; KeyOn decrements and clears the flag only at exact zero. The leaves
return signed depth, while their event wrapper returns completion status 2.
There is no saturation or automatic underflow repair. Background mode and
camera-follow flags remain separate: background main mask 0x2000000, camera
halfword 0x10, Mario display mask 0x20000. Form-dependent field writes retain
original IEEE values and owner lookup order; they are not native render tuning.

Previous-party recovery has a retained cursor, bound to the world user token.
Raw selectors 0/1 address signed current-index bytes +0x245/+0x246 and previous
kind bytes +0x247/+0x248. Original first phase constructs only for negative
index and nonzero kind. Native provider holds preserve the captured state and
must not repeat a completed constructor. Negative real constructor results
complete; nonnegative results narrow to byte and are re-read signed. An engaged
lookup optional containing zero is an acknowledged null and completes; a missing
optional holds. The area owner retires the cursor with the event.

## Cases, map flags and mutable door records

The case pool has 64 source-layout 160-byte records. Flags 1/2/4 mean active,
marked for second-pass dispatch, and descriptor type-0x8000 state. Case deletion
clears active without erasing payload or implicitly deleting its event. Explicit
delete mode zero first deletes the live event; other modes clear only the case.
Dispatch checks stale IDs, creates missing events with original ownership, then
clears the mark. Capacity/provider holds preserve the mark. Bero priority 20
requires NPC stop after creation; execution belongs to the later event manager.
Registration captures locals before publishing the pool index, and freshly reads
the destination after admission. Its number argument strips EVT namespace
encoding without resolving a variable's current value.

Map draw flags and hit flags are independent overlays on original hierarchy.
Map mask 1 hides vertices. Hit masks 1/0x80 disable collision; only 0x80 excludes
name lookup. Descendant updates traverse parent links, not every later node.
Duplicate names fail as ambiguous. Triangle vertices share an owner, allowing
lazy maximal visible ranges without breaking primitives. Generation rejects
stale catalog state; epoch lets dependent room prefixes replay after invalidation.

Door records are private 72-byte copies with original guest pointers retained.
Word 13 (+0x34) holds the NPC-name list. Known quest1420 selects alternate names
on equality; unknown quest holds. All room copies stage before publication.
The native guest arena advances monotonically across room invalidation; clearing
copies does not reclaim earlier addresses. Selector zero replaces that list;
other selectors succeed without touching a record. Room 2/work 11 setup hides
its map object before disabling its hit object. A hold resumes at the incomplete
phase; a changed map epoch restarts the prefix. These records do not implement
original heap aliasing or the rest of door/NPC lifecycle.
