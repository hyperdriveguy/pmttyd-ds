# Reading the event VM implementation

The portable VM operates on recovered US GameCube script words and original
32-bit address tokens. [EventNativeRuntime](../../source/event/event_native_runtime.cpp)
composes services; [contracts](../behavior/event/EVENT_OPCODE_IMPLEMENTATION_COVERAGE.md)
record evidence and unresolved providers. Local opcode coverage is not full
native story execution or original numeric/hardware parity.

## Scheduler and continuation

[event_manager.cpp](../../source/event/event_manager.cpp) collects live entries
into a256-slot priority table paired with generation IDs. Descending pairwise
exchanges preserve the source swap order; replacing this with a generic sort
can alter ordering. IDs detect slot reuse after collection. Child/brother
constructors may append to a running pass; roots need flag0x20 to append.

Entry flags1/2/0x10/0x80 mean allocated/stopped/waiting-child/inhibited. Mask0x92
prevents blocked entries from accruing lifetime or credit. Eligible visits add
lifetime once, then timescale credit may dispatch multiple times at unchanged
lifetime. Clock conversion uses the source reciprocal multiplier0x10624dd3 to
divide `clock >> 2` by1000. A negative delta becomes zero; a delta above500 ms
becomes one integer refresh interval. Native rendering cadence is a separate
owner, not the source scheduler clock.

[dispatch](../../source/event/event_dispatch.cpp) fetches a header containing
signed high16 argument count and low8 opcode, then advances4-byte arguments.
`argumentCount` stores only low8 count bits. Two result protocols coexist:

| Handler result | Dispatcher action |
| --- | --- |
| 0 | Yield with current opcode retained |
| 1 | Clear opcode then yield |
| 2 | Clear opcode then continue fetching in the same call |
| 3 | Redispatch without clearing |
| 255 | Return -1 to manager (end this visit) |
| Negative | Return1 to manager (abort entire pass) |
| Other nonnegative | Redispatch without clearing |

The manager receives0/yield, -1/end visit,1/abort pass. Child creation uses255
to suspend the parent visit without deleting the parent. `WAIT_FRAME` captures
and decrements on the first invocation:0 continues immediately,1 completes
and yields immediately. Negative counts wrap rather than clamp. `WAIT_MSEC`
subtracts only lifetime low words, sign-extends the32-bit result, then performs
unsigned64 division. Preserve that unusual domain behavior. `USER_FUNC` caches
the resolved function once, removes its operand from the argument list and
passes firstCall1; later polls reuse the function/list with firstCall0.
See [wait protocol](../behavior/event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).

## Operand numbers and storage

[event_operands.cpp](../../source/event/event_operands.cpp) routes signed bands
in order, using operand minus base as index. M below means one million.

| Integer getter upper bound | Family | Index base / storage |
| --- | --- | --- |
| -250M | Immediate | Raw signed word |
| -220M | Encoded float | `(signed32(word+230M))/1024` |
| -200M | UF user flag | -210M / entry-supplied base |
| -180M | UW user work | -190M / entry-supplied base |
| -160M | GSW global saved byte | -170M / GP+0x578; index0 is story word+0x174 |
| -140M | LSW local saved byte | -150M / GP+0xdb8 |
| -120M | GSF global saved flag | -130M / GP+0x178 |
| -100M | LSF local saved flag | -110M / GP+0xd78 |
| -80M | GF global flag | -90M / context+0x84 |
| -60M | LF local flag | -70M / entry+0xdc |
| -40M | GW global work | -50M / context+4 |
| -20M | LW local work | -30M / entry+0x9c |
| Higher | Immediate | Raw signed word |

Flags have32 bits per4-byte word. `eventShiftMask` reproduces PowerPC `slw`:
low6 count bits select a shift, but bit5 produces zero. Negative flag indices
retain signed quotient/remainder behavior; they are not sanitized into a new
namespace. Work arithmetic wraps32 bits explicitly, avoiding host signed
integer overflow.

Get/set integer and float APIs have different band tests. Float reads do not
have separate UF/GSF/LSF branches; float writes omit saved-switch branches too.
Do not consolidate these into the integer routing. Integer word/byte writes
return the old value, while flag writes return the requested value. Float work
stores quantize by1024, truncate through the numeric provider, subtract230M,
and return the decoded old word. Event quantized floats differ from the IEEE
bits used by data cursor and absolute memory float operations.
See [integer routing](../behavior/event/EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[float routing](../behavior/event/EVENT_FLOAT_OPERAND_ROUTING.md).

## Opcode families

| Codes | Source implementation / meanings |
| --- | --- |
| 0 | Dispatcher fetch |
| 1..3 | END, RETURN/delete, LABEL |
| 4..8 | GOTO, DO, WHILE, DO_BREAK, DO_CONTINUE |
| 9..11 | WAIT_FRAME, WAIT_MSEC, WAIT_WHILE |
| 12..17 | String EQ/NE/LT/GT/LE/GE |
| 18..23 | Float comparisons in the same order |
| 24..29 | Integer comparisons in the same order |
| 30..33 | Mask-set/clear IF, ELSE, END_IF |
| 34..49 | SWITCH/raw SWITCH, comparisons, default, OR/AND groups, mask, group-end, range, break, end |
| 50..61 | Resolved/raw/float assignment; integer and float arithmetic |
| 62..73 | Integer/float data cursor setup,1..4 lanes, indexed read |
| 74..82 | User work/flags setup, allocation, AND/OR masks, ms/tick conversion |
| 83..90 | Absolute and indirect integer/float memory access |
| 91 | USER_FUNC |
| 92,93 | RUN_EVT, RUN_EVT_ID |
| 94..99 | RUN_CHILD_EVT, DELETE_EVT, RESTART_EVT, priority/speed/type setters |
| 100..105 | Stop/start all, others, or by ID |
| 106 | CHECK_EVT ID output |
| 107..109 | Independent inline block, ID variant, terminator |
| 110..112 | Attached brother block, ID variant, terminator |
| 113..118 | Diagnostic/no-op operations, register formatting, raw field store, yielding diagnostic scan |

Nested scanners preserve source quirks: label/inline scans are flat; SWITCH
scanning nests34 but not35. GOTO uses strict `< -270M` for a raw address, then
signed-byte labels otherwise. DO raw count0 is infinite; variable counts resolve
on each WHILE. Empty loop/switch index0xff is signed -1; stacks have8 levels,
labels16 entries. Group switch states255/254 mean signed -1/-2. Float IF20..23
uses negated opposite comparisons, accepting unordered values. MOD adds .5f
then truncates both operands before computing remainder. These are intentional
translations, not invitations to replace them with ordinary host operators.

## Original address aliases and lifetimes

[event_entry_addresses.cpp](../../source/event/event_entry_addresses.cpp) maps
packed0x1b0-byte entries to semantic `EventEntry` fields, with byte/word aliasing
and retained unknown bytes. [Packed layout](../behavior/event/EVENT_ENTRY_PACKED_LAYOUT.md)
is the offset reference. Never use `sizeof(EventEntry)` for original heap bytes.
Field/battle contexts at0x803d8d30/0x803d8dd0 are0xa0 bytes each. GP root word
0x80415fd0 selects the live GP mapping; GP+4 is refresh rate,+0x14 context
selector,+0x38/3c event timestamp. Runtime schedule arrays at0x803d8530 and
0x803d8930 contain256 four-byte IDs and slots respectively.

Big-endian byte lanes use `24-8*(address&3)`; `~3` aligns a word. The exclusive
32-bit address ceiling is0x100000000. Original pointers remain tokens until
mapped, including partially overwritten pointers. Unresolved link bits0/1/2
refer to parent/child/brother: null host pointers with set bits must not become
original null links. Registration resolves tokens when possible; retirement
converts borrowed pointers back to unresolved tokens before freeing objects.
`EventEntryStorage` replaces allocated RAM spans with stable semantic entries,
then restores packed bytes on retirement. Retiring that facade is separate
from freeing the source heap allocation.

Child deletion copies locals/flags/message state back and wakes the parent;
brothers do not use that child copyback. Stop/start traversal preserves
compiler-inlined recursion and repeated context reads so callback/alias
changes stay observable. Restart resets execution progress while retaining
identity, relationships and local state. Required unmapped memory, callbacks
and numeric/cache providers are domain failures, not successful no-ops.

## Allocator and retrace numbers

The map allocator has singly-linked32-byte headers: next+0,size+4,used halfword+8.
Sizes exclude headers; coalescing adds32 per reclaimed header. It uses best fit.
SDK heaps have12-byte descriptors (size/free-head/allocated-head) and doubly-linked
32-byte nodes (previous+0,next+4,total span+8). SDK sizes include headers; first-fit
allocation uses `(bytes+63)&~31`, refusing to split a remainder below64 bytes.
Both use32-byte alignment/cache lines, but their header arithmetic differs.
The nested battle map pool0x64000 bytes is400 KiB. Heap bootstrap globals and
five8-byte configuration rows are explained beside their uses in
[event_memory_bootstrap.cpp](../../source/event/event_memory_bootstrap.cpp).

Retrace publication retries upper/lower/upper raw-time reads across rollover.
64-bit writes publish low then high in source order. GP+0x28 holds delta,+0x30
previous raw time,+0x20 total,+0x38 event time,+0x40 nonbattle time. Inhibit
bits8/4/2/1 gate banks+0x60/58/50/48. Requested system level0..4 is separate
from the live low-nibble clock masks0/1/3/7/15 and event type stop masks.
See [time publication](../behavior/platform/EVENT_RETRACE_TIME_PUBLICATION.md).
