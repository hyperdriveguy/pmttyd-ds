# Partner print variants, append and placement producers

US G8ME01 revision 0, recovered 2026-10-12. B evidence is static original
code/data; no executed dialogue scenes or Switch equivalence was measured.
See [common print](MESSAGE_PRINT_EVENTS.md), [lookup](MESSAGE_RESOURCES_AND_LOOKUP.md),
[lifecycle](MESSAGE_WINDOW_LIFECYCLE.md) and [event runtime](EVENT_RUNTIME.md).
Original keys/suffixes/text and story conditions remain reused assets. No
missing dialogue or character attribution is invented from function names.

## Event setters and mutation scope (B)

These four callbacks ignore firstCall, resolve operands every invocation and
return2 immediately. There is no local validity/failure fallback.

| Function/address/size | Exact local behavior |
| --- | --- |
| evt_msg_npc /800d1ed4/60 | Resolve operand0 as string. Use event.word[178] to get current window/data; strcpy(string) into D+f204 |
| evt_msg_pri /800d1f34/3c | Resolve operand0 as word; store event.word[190] |
| evt_msg_togelr /800d1f70/98 | Resolve operand0 as signed word; OR event.word[180] with80000000, then optional high bits below |
| evt_msg_toge /800d2008/88 | Resolve operand0 as word; replace event.word[180] with `(old&e1000000)|resolved`; then resolve operands1/2/3 with evtGetFloat and store floats at event+184/+188/+18c |

For togelr, value1 additionally ORs40000000; value2 ORs20000000; value3 ORs
60000000. Other values (including0 and negative/out-of-range) add no further
bits. It never clears previously accumulated orientation bits. Repeated calls
therefore accumulate, even value0 after value3. toge preserves precisely
80000000/40000000/20000000/01000000 before ORing its entire resolved word;
the input is not narrowed to a small placement enum. Setter order matters.

npc copies a name without evtNpcNameToPtr lookup, window movement or flag reset.
It does not clear battle speaker bit8 or assign battle unit ID. Its unbounded
strcpy assumes a valid source and field capacity. pri affects pending layer for
subsequent common print, not the current window's layer. Common print consumes/
resets event words180/190, narrows layer to signed halfword and reads the stored
anchor floats. See [common print](MESSAGE_PRINT_EVENTS.md) for that order.

## Partner key suffix tables (B)

Field suffix table has11 pointers at0x80416370, indexed by the signed byte at
current partner+31. Null partner selects index0. Values are:

| Field index | Suffix | Battle unit kind | Battle suffix |
| --- | --- | --- | --- |
| 0 | empty NUL string | e0 | _kur |
| 1 | _kur | e1 | _nok |
| 2 | _nok | e2 | _yos |
| 3 | _bom | e3 | _win |
| 4 | _yos | e4 | _viv |
| 5 | _win | e5 | _bom |
| 6 | _viv | e6 | _chu |
| 7 | _chu | — | — |
| 8 | _jug | — | — |
| 9 | _tel | — | — |
| 10 | _lui | — | — |

Battle table has seven pointers at0x8041639c. The binary indexes with a unit
kind word from unit+8 using address `0x8041639c+4*kind-0x380`, corresponding
to e0..e6 for these seven entries. It does not index this table as0..6 directly.
Neither field signed index nor battle kind has a local bounds guard. Do not
assert unsupported indices or infer Western character identity from suffix text.
Mapping these values to character data/callers remains a separate content task.

Key construction uses strcpy(baseKey) then strcat(suffix), without output
capacity checks. These are key bytes, not display text, locale substitutions,
or fallback searches across partner variants. Exact generated key is passed
to msgSearch; if missing, normal localized fallback applies.

## Field partner print versus append (B)

evt_msg_print_party at0x800d2790,sizebc:

1. Resolve only script operand0 as base key, every invocation.
2. marioGetPartyId(), then partyGetPtr(id). If nonnull use signed partner byte31;
   otherwise index0. Construct base+suffix in static64-byte buffer0x803df548.
   This construction is outside a firstCall guard and repeats on resumed calls.
3. Call _evt_msg_print(event,firstCall,flags0,keyBuffer,callback0,speaker="party")
   using original static name at0x8042186c. Return its code unchanged.

The common helper performs creation only on firstCall, but borrows keyBuffer as
identity. Rebuilding shared storage on resumed/other calls can change identity
bytes independently of already parsed text. Key/partner stability and concurrent
event ownership must be traced; do not replace it with an assumed immutable copy.

evt_msg_print_party_add at0x800d26bc,sized4 also resolves operand0 and queries
current partner/index on every invocation. Only nonzero firstCall constructs
base+suffix in its separate64-byte buffer0x803df508, msgSearch(generatedKey),
then msgWindow_Add(text,event.word[178]). Every invocation polls windowCheckID
that saved index: nonzero returns0, zero returns2. It does not reassign identity,
callback/owner/speaker, pending layer or placement through common print.
Different firstCall gates therefore cannot be unified by a generic partner-print
cache or by reparsing/adding text on every resumed callback.

## Battle partner print (B)

evt_msg_print_battle_party at0x800d25ec,sized0 resolves operand0 as base key
every invocation. BattleTransID(event,-4), then BattleGetUnitPtr(globalBattle,
translatedID). Null unit returns2 immediately, including on resumed calls;
no saved-window poll, close or deletion occurs on that path.

For nonnull unit, build base+suffix from its kind word as above in static64-byte
buffer0x803df4c8 on every invocation. Call common print with flags2, callback0,
speaker unit operand-4 and the generated key. Return common result unchanged.
The unit lookup used for suffix and the later common-helper translation are
separate service calls. No local partner-missing fallback to Mario or unsuffixed
key exists here. Shared-buffer identity lifetime remains as for field print.

## Generic append callback (B)

evt_msg_print_add at0x800d23c4,size9c resolves operands0(flags) and1(key/rawText)
in order on every invocation. On nonzero firstCall only:

- Flags mask1 clear: msgSearch(resolvedInput), then msgWindow_Add(text,savedIndex).
- Mask1 set: msgWindow_Add(resolvedInput,savedIndex) directly.

savedIndex is event.word[178]. No other flags are used locally. On all calls,
poll windowCheckID(savedIndex); nonzero returns0, zero returns2 (implemented by
word arithmetic rather than a conditional branch). No identity overwrite,
speaker/placement configuration, callback reassignment or deletion occurs.
The underlying Add refreshes playback timestamp and appends parser records;
it is not a whole-work replacement or a new-window print.

## Differential obligations and remaining work (P)

Compare setter order/bit accumulation, full-word toge flags versus preserved
high bits, float operand resolution, existing-window NPC name-only mutation,
pending versus current layer, null/valid partner suffix routing, signed field
index versus e0-based battle kind, distinct static buffers, construction before
firstCall gates, shared identity versus parsed text, resumed missing battle unit,
append identity retention, direct/key append and completion without deletion.
These are derived comparison obligations, not original execution observations.

Remaining: insert/add-insert substitutions and callbacks, suffix-to-character
data/caller census, placement producers in story graphs, partner/battle service
invariants, static key-buffer ownership, rendering/manager/input cadence and
original story attribution/order/branches/conditions. These eight local variants
and setters are documented; full dialogue/full-game equivalence remain unfinished.
