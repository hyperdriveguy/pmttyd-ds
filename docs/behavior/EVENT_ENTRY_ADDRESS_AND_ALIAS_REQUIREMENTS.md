# EventEntry address translation and alias requirements

US G8ME01 revision0; consolidated2026-10-12 by documentation agent ID2.
This implements the documentation request for alias domains accompanying the
[complete432-byte map](EVENT_ENTRY_PACKED_LAYOUT.md). Requirements derive from
linked original local contracts;their instruction fingerprints are reproducible
with `tools/verify_event_entry_layout.py`. Proposed native architecture choices
below are identified as implementation choices,not recovered original code.
Original callback/source/scene/numeric/invalid-domain execution remains open.

## Original address words and host objects must remain distinct

Original pointer storage is32-bit big-endian,even when the host uses64-bit
pointers. Preserve original addresses as words in entry/script/buffer storage.
Translate only at the service boundary needing an entry,data view,code callback
or resource. Never store a host pointer into an original word or cast an
original address directly to a native C++ object. The semantic EventEntry in
include/event_manager.hpp has different field order,size,pointers and padding;
`sizeof`,`offsetof` and reinterpret_cast cannot supply the original ABI.

A host adapter needs bidirectional entry address mapping: E=context.array+
wrap32(slot*0x1b0) and relationship word→mapped entry. Entry ID15c is independent
of slot/address and can change on reuse;relationships store addresses rather
than IDs. Scheduled visits compare retained ID against the current entry ID.
Replacing pointer relationships with a permanent ID lookup can change deletion,
copy-back and reused-slot behavior. Capacity,alignment and mapped lifetime are
provider preconditions,not validations supplied by every original function.

All arithmetic follows32-bit wrapping where specified. Context/base selection
must occur at the original read point. Manager/root constructor can capture one
context while GetWork or a called constructor selects current GP again. Helpers
also reread pointers/fields after callbacks. A single pass-level owner snapshot
or cached copy of the whole entry/banks does not reproduce that behavior.

## One coherent byte store per mapped region

Implement readByte/writeByte/readWord/writeWord on the SAME addressable bytes.
Big-endian word at A consists of bytes A..A+3;writing byte at A+1 changes that
word's second-most-significant byte. Writes must be visible immediately to later
reads through any field,array,namespace or address alias. Preserve signedness
only at the consuming instruction/API;storage itself is bits.

An implementation choice is a byte-backed original-address arena with typed
field accessors. Another choice is semantic fields plus a complete byte-level
translation layer that preserves raw spans and cross-field access. Either needs
coherence for partial writes,external buffers,stale/raw words and all copied
spans. A cache that deserializes entry on callback start and writes the full
object afterward can overwrite concurrent/aliased changes and unknown bytes.
The original contracts do not provide such transactional snapshot/commit behavior.

Bulk memset covers every byte of the original range. Restart preserves unlisted
bytes. Individual stores/copy-back follow the documented sequence;they are not
whole-entry snapshots. Paired metadata184/188 reads BOTH source words before
writing either destination word. Preserve that snapshot width and subsequent
store order;it does not authorize reading the entire message/local bank first.
An adapter observing individual stores must still see the defined sequence.

## Locals,flags,stacks and labels are physical neighbors

The following examples are original address arithmetic,not claims valid source
scripts deliberately take them. They expose why separate host arrays cannot
silently clamp,wrap within their own array or ignore adjacent fields:

| Computed access | Original address alias |
| --- | --- |
| LW index-1: E+9c+4*(-1) | E98 cached-function word |
| LW index16 | Edc first local-flag word |
| LF bit index96 | Ee8 first loop-body word,using q=3 |
| Loop-body index-1 | Ee4 third local-flag word |
| Loop-count index-1 | E104 last loop-body word |
| Loop-body index8 | E108 first loop-count word |
| Loop-count index8 | E128 four switch-state bytes as one word |
| Switch-state index-1 | E127 last byte of final loop-count word |
| Switch-value index-1 | E12c four switch-state bytes as one word |
| Switch-state index8 | E130 first byte of switch-value0 |
| Switch-value index8 | E150 data-cursor word |
| Label-ID output index16 | E2c first byte of label-pointer0 |
| Label-pointer output index16 | E6c waiting-parent address word |

Loop/switch address indices are signed8 after low8 stores;initialff is-1.
The original stack helpers supply no universal depth guard. Label scanner has
no effective16-label cap and stores label ID before the corresponding pointer
in each iteration. Overflow can therefore change later pointer/relationship
fields. A native valid-depth/label invariant may reject an unsupported script,
but must report that domain policy rather than claim original clamping/recovery.

Integer operand bands are not capacity checks. Negative bit index uses
truncation toward zero for word quotient and PPC shift-mask rules,not Python
floor division or normalized modulo. The flag helper can read/store a word
even when mask0. Local words can store raw encoded floats or pointer words;
GetValue/GetFloat coercion differs. Keep all namespace thresholds and return-old
semantics in the [integer](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[float](EVENT_FLOAT_OPERAND_ROUTING.md) and
[saved-bank](EVENT_SAVED_SWITCH_STORAGE.md) contracts.

## User buffers,data cursors and indirect memory

E154 and E158 are address WORDS pointing to shared user work/flag storage.
Constructors copy those addresses;they do not copy the pointed allocation.
Two descendants can therefore observe each other's UW/UF changes while their
local9c..d8/dc..e4 banks remain value copies. Nothing requires user buffers to
be distinct from entry/context/script memory at the local namespace API level.
An address provider must retain supported aliases or explicitly state a source
domain limitation. No automatic private clone or per-entry free is recovered.

E150 is a separate data-cursor address. Sequential word/float readers advance
its stored pointer before or between their documented reads/output callbacks;
indexed readers have different coercion and query order. E150 can point into
mapped entry or buffer storage;raw outputs may themselves alter E150 or later
operands. A host data iterator detached from the pointer word would lose that
alias. The same applies to raw indirect word commands74..90:they use original
address operations,not a separate safe local-variable language.

Read [data cursor](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md),
[multiple readers](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md),
[indirect memory/time](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) and
[user buffers](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md). Counts and shifts,
allocator root/context,zeroing and output order remain exact service requests.
Bounds/null/failure policy and freeing are not supplied by those pointer names.

## Script addresses,USER_FUNC and callback-visible entry writes

E14/E18/E1a0/E1a8,label pointers and loop-body pointers are original stream
addresses,not indices into only one currently running script. FETCH publishes
current-command1a8,loads header,sets arguments18,next14 and marker/count in
order. Scanners use signed high16 count and full low16 opcode,including reads
outside a nominal command's argument count. Original script packs must preserve
required trailing label-scan bytes;do not repair original stream headers.

USER_FUNC initially resolves operand0 as a word,stores original callback address
E98,advances E18 by4,decrements count9 and sets markerd. It then calls the
registry-bound callback with firstCall1;polls use cachedE98 and firstCall0 without
re-resolving the function or restoring argument pointer/count. Registry mapping
must key by original code address and supply the actual callback result. Do not
convert all callbacks to completion2 or add a render-frame callback clock.

Callbacks may mutate their own entry,other entries,local/user banks,relationships,
context/GP or stream-backed operands. Those writes must be visible at the next
original read point. Preserve handler result0/1/2/3/255/signed-negative effects
and cached state across yields. EventEntry semantic field references and raw
address operand access must see one coherent state,including partial byte writes.
[Dispatch/waits](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) and
[all119 coverage](EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md) define local opcode
and return behavior. Full required callback registry/NormalJump registration
and source-owned actor/type/priority/context paths remain open requests.

## Message aliases and lifecycle copy-back

E178 print window and E17c selection window are separate handles. Coordinates
E184/188/18c are floats for message presentation and raw words during inheritance/
copy-back. E180 mask,E190 layer and E170 NPC context are shared entry storage,
not an independent private message-state object. Message callbacks can update
these words and local outputs during repeated USER_FUNC polling. A child receives
copied metadata and later returns it to its waiting parent through deletion;
an attached brother does not get equivalent metadata/copy-back ownership.

Preserve live descendant→parent writes before the ancestor performs its own
copy-back. Do not deserialize parent metadata once at deletion start. Copy-back
is independent of “successful completion”;ancestor cancellation can copy too.
The metadata pair184/188 reads before its two stores,while other listed fields
are transferred in their documented order. Parent active/link/ID validation is
not added by the original deletion body. See [deletion](EVENT_DELETE_AND_COPYBACK.md)
and [message prints](MESSAGE_PRINT_EVENTS.md).

## Reviewable translator acceptance and open scope

The native owner can implement the map with fixtures that exercise:

- Big-endian byte/word round trips,partial lane writes,all432 bytes including
  unknown spans,root clear versus restart preservation,and host pointer sizes.
- Original-address↔entry mapping,relationships versusIDs,slot reuse/live ID checks,
  context recapture and copied user pointers versus copied local values.
- Every physical alias above,including label outputs into pointers and stack
  underflow/overflow;explicit domain rejection where unsupported.
- Pointer-containing UW/LW words,raw versus encoded-float interpretation,
  sequential cursor/output alias,indirect read/write order and flag zero-mask.
- USER_FUNC initial/poll state,registry lookup by original address,callback writes
  visible before the next dispatcher/manager query,and original handler results.
- Message fields with raw/float views,distinct handles,paired source reads,
  child copy-back chains and cancellation;stale unknown bytes preserved when
  no bulk clear occurs.

These are acceptance requirements,not tests already passed by a native translator.
Existing C++ semantic tests prove their scoped mechanics;they do not establish
packed ABI/alias compatibility. Original alignment/invalid-address exceptions,
source reachability,full callback/domain schemas,scene/free ownership,FPSCR and
measured original execution remain unresolved. Layout consolidation is delivered;
EVT-01 remains open until those binding and fidelity requirements are satisfied.
