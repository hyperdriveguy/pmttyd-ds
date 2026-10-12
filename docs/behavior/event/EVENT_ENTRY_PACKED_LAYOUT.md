# Consolidated original EventEntry packed layout

US G8ME01 revision0; consolidated2026-10-12 by documentation agent ID2.
This is the requested reviewed map for the native memory translator: original
entry stride0x1b0 (432 bytes),big-endian GameCube storage,32-bit address words.
It consolidates the delivered manager,constructors,lifecycle and all119 local
opcode contracts;message callbacks supply additional typed uses. It does not
claim all game callbacks or unknown byte producers have been recovered.

Machine-readable map: `tools/event_entry_layout.json`. Reproduce supporting
binary checks and complete byte partition with
`python3 tools/verify_event_entry_layout.py`. That verifier runs ten existing
original-ISO evidence suites and checks this map's structural accounting. It
does not execute original code or prove every possible callback/address alias.
Read [address/alias requirements](EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md)
before adapting the semantic C++ EventEntry: its host layout is not this ABI.

## All432 bytes,without invented padding semantics

Offsets below are hexadecimal relative to original entry E. Size is DECIMAL
bytes. Raw32 means preserve all bits;different readers can interpret the SAME
word as signed integer,encoded float,mask or address. “Unknown raw bytes” means
no field meaning established in the delivered scope;it does not mean unused,
reserved,always zero or safe to drop. Root/descendant memset and deletion/reset
cover these bytes along with known fields. Multiword entries show inclusive spans.

| Offset / span | Size | Field / local role | Access domain | Evidence contract |
| --- | --- | --- | --- | --- |
| 000..007 | 8 | lifetime | u64 as high/low BE words | [manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| 008 | 1 | flags | u8 mask | [manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| 009 | 1 | argumentCount | u8 narrowed count | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 00a | 1 | opcode | u8 cached opcode | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 00b | 1 | priority | u8 | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 00c | 1 | type | u8 mask | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 00d | 1 | initialized | u8 zero/nonzero | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 00e | 1 | loopIndex | u8 stores / s8 address index | [loop](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 00f | 1 | switchIndex | u8 stores / s8 address index | [switch](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 010 | 4 | raw10 | unknown raw bytes | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 014 | 4 | nextCommand | u32 address | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 018 | 4 | arguments | u32 address | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 01c..02b | 16 | labelIds | 16 u8 stores / s8 GOTO reads | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 02c..06b | 64 | labelAddresses | 16 u32 addresses | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 06c | 4 | waitingParent | u32 entry address | [lifetime](EVENT_DELETE_AND_COPYBACK.md) |
| 070 | 4 | waitingChild | u32 entry address | [lifetime](EVENT_DELETE_AND_COPYBACK.md) |
| 074 | 4 | brotherOwner | u32 entry address | [lifetime](EVENT_DELETE_AND_COPYBACK.md) |
| 078 | 4 | waitCount | s32 / raw word | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 07c..083 | 8 | waitStart | high/low u32 words | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 084..097 | 20 | raw84 | unknown raw bytes | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 098 | 4 | cachedFunction | u32 code address | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 09c..0db | 64 | locals | 16 raw32 words / namespace coercion | [operands](EVENT_INTEGER_OPERAND_NAMESPACES.md) |
| 0dc..0e7 | 12 | localFlags | 3 u32 masks | [operands](EVENT_INTEGER_OPERAND_NAMESPACES.md) |
| 0e8..107 | 32 | loopBodies | 8 u32 addresses | [loop](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 108..127 | 32 | loopCounts | 8 raw32 operands / signed tests | [loop](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 128..12f | 8 | switchStates | 8 u8 stores / s8 reads | [switch](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 130..14f | 32 | switchValues | 8 s32 words | [switch](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 150 | 4 | dataCursor | u32 address | [data](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 154 | 4 | userWork | u32 address | [operands](EVENT_INTEGER_OPERAND_NAMESPACES.md) |
| 158 | 4 | userFlags | u32 address | [operands](EVENT_INTEGER_OPERAND_NAMESPACES.md) |
| 15c | 4 | id | u32 identity | [manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| 160 | 4 | actor | s32 selector / raw word | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 164 | 4 | timescale | IEEE binary32 | [manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| 168 | 4 | credit | IEEE binary32 | [manager](EVENT_MANAGER_COMPLETE_LOOP.md) |
| 16c | 4 | field16c | raw32 initially -1 | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 170 | 4 | field170 | raw32 / message NPC-address domain | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 174 | 4 | raw174 | unknown raw bytes | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 178 | 4 | messageWindow | s32 handle / raw32 | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 17c | 4 | selectionWindow | s32 handle / raw32 | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 180 | 4 | messageStyle | u32 mask / raw32 | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 184 | 4 | messageX | binary32 / raw32 copy | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 188 | 4 | messageY | binary32 / raw32 copy | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 18c | 4 | messageZ | binary32 / raw32 copy | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 190 | 4 | messageLayer | raw32 / low signed16 consumer | [message](../message/MESSAGE_PRINT_EVENTS.md) |
| 194..19f | 12 | raw194 | unknown raw bytes | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 1a0 | 4 | restartScript | u32 address | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |
| 1a4 | 4 | field1a4 | raw32 debug opcode116 | [tail](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 1a8 | 4 | currentCommand | u32 address | [dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 1ac | 4 | raw1ac | unknown raw bytes | [root](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) |

The five raw spans10..13,84..97,174..177,194..19f and1ac..1af total44 bytes.
Known role spans account for388 bytes;the partition has no gaps or overlapping
field declarations. Actual memory aliases ARE possible and required by the
address rules;the nonoverlapping declaration table does not forbid them.

The array extents are the storage spans inferred from the adjacent fields and
used native valid-domain arrays:16 label IDs/pointers,16 locals,three flag words,
eight loop-body/count and eight switch-state/value entries. Original helpers
lack general bounds enforcement. A declaration's extent is not an original
clamp/error check. See the explicit overflow/underflow examples in the alias
contract rather than implementing each array as isolated checked storage.

## Width,signedness and interpretation distinctions

Lifetime00/04 is high/low big-endian64-bit raw ticks;manager arithmetic can
interpret a delta as signed64 before clamping. Wait-start7c/80 also stores
high/low,but WAIT_MSEC uses LOW-word subtraction/sign extension rather than a
full64-bit elapsed difference. Preserve both words and their actual store order.
Changing both fields to one host wall-clock scalar would lose observed stores.

Flags8,priorityb and typec are unsigned bytes. Manager mask1 is active,mask2
stopped,mask10 waiting-child andmask80 inhibited. The table builder still
includes active entries with those latter flags. Priority compares unsigned.
Fetch count9 stores low8 of a SIGNED high16 stream count,but uses the original
signed16 value to compute next14. Cached opcodea is low8 of the command opcode;
lexical scanners can read full low16. Byte narrowing does not change the stream
header's scanner/count interpretation. Markerd is zero/nonzero retained state,
not an independent physical-frame counter.

Index bytese/f startff,increment/decrement with low8 stores,and are sign-extended
for stack addresses. Label bytes are stored low8 and read signed8 by GOTO.
Switch state bytes are stored low8 and tested as signed8. Loop counts are raw
operands with separate direct-decrement versus variable-namespace policies;
switch values are signed comparison words. Local9c..d8/UW words preserve raw
encoded values;GetValue/GetFloat apply different routing/coercion on reads.
They are not permanently typed host floats or resolved integer-only registers.

Pointer words14/18,2c..68,6c/70/74,98,e8..104,150/154/158,1a0/1a8 carry original
32-bit addresses. The words have different use domains (script,data,entry,code,
user buffers),but no intrinsic tag or automatic native ownership. Actor160 is
an integer selector,not a generic pointer;ID15c is identity,not a slot/address.
Field170 is copied as a raw word and interpreted as NPC context by message
callbacks for relevant types. Field16c begins-1,without a complete producer/
consumer schema in this scope. Field1a4 is raw operand storage for opcode116;
do not infer a callback pointer from that write alone.

Message178/17c are DISTINCT print and selection window handles. Message180 is
style/mode mask;184/188/18c are float coordinate storage used by message setup.
Message190 is a word whose low signed16 is passed as layer to window creation,
then reset by the print callback. Lifecycle copies preserve all seven words:
17c,178,180,paired184/188,18c,190. The paired transfer does not make184/188 one
semantic double;they remain two independently addressable float/raw words.
[Message variants](../message/MESSAGE_PRINT_VARIANTS.md),[selection](../message/MESSAGE_SELECTION.md)
and [window lifecycle](../message/MESSAGE_WINDOW_LIFECYCLE.md) pin those consumers.

## Construction,restart,deletion and retained raw storage

Root/typed,child/brother constructors clear the entire1b0 entry AFTER selecting
its slot. This initializes unknown spans to zero for THAT construction;it does
not establish reserved-zero invariants for later raw writes. Constructors then
perform their documented field writes and label scan. The scan can overrun
nominal16-label storage;see linked alias policy. Globals,source copies,append
and run-check remain ordered operations,not a single struct assignment.

Child inherits local words/flags and message metadata;brother inherits local
words/flags but not message metadata. Both share copied user154/158 address
words. Inline and ID-output wrappers have further post-construction recopy;
plain brother110 copies150 afterward,ID brother111 does not. Root-independent
inline retains no parent/brother relationship solely because metadata was copied.

Restart changes only its listed script/opcode,speed/credit,lifetime,index and
label fields plus called run-check effects. It does NOT clear unknown bytes,
locals,flags,arrays beyond indices,user pointers,callback,args/marker,timers,
message metadata,ID or relationship words. Fetch afterward resets marker/count/
args,which is a later stage. Deletion first performs descendant/copy-back work,
then clears active and memset432;inactive deletion preserves stale storage.
Context ReInit bulk-clears selected array without per-entry deletion/copy-back.

These differences prohibit a blanket reset of all semantic EventEntry fields
for restart or a destructor-driven free policy for user buffers. See
[descendant constructors](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[restart](EVENT_RESTART_PRESERVATION.md),[run/ID wrappers](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md)
and [deletion](EVENT_DELETE_AND_COPYBACK.md) for exact write/call order.

## Context and global address domains needed beside entries

Field C0=803d8d30,battle C1=803d8dd0;each is0xa0 bytes. Its complete layout is
signed capacity word0,32 raw work words4..83,three flag words84..8f,entry-array
address90,raw unknown word94,previous timestamp high98/low9c. Init requests
256*432=110592 bytes for EACH array. Initial extent is evidence,not proof no
later capacity/pointer mutation occurs. The manager can retain one context while
an operand helper recaptures another from current GP.word14. No equality guard.

Shared globals are separate from either array:ID schedule803d8530,slot schedule
803d8930 (256 words each),manager-active8041e858,live count8041e85c,allocation
count8041e860,global ID804151e8 and global binary32 speed804151ec. GP pointer
80415fd0 supplies context selector14 and event timestamp38/3c. Saved-switch
banks are GP-owned storage,not physically inside EventEntry;GSW0 is GP174 WORD
while other GSW indices use GP578+index bytes. Do not map all names to the
entry's local arrays. [Context init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md),
[manager](EVENT_MANAGER_COMPLETE_LOOP.md) and
[saved leaves](EVENT_SAVED_SWITCH_STORAGE.md) provide source/store policy.

The native translator may bind these original addresses to host storage,but
must preserve byte/word aliasing,lifetime and read/write order. Unknown bytes,
external buffers/code/resources,full source/owner/registry/scene/platform/numeric
policy and original traces remain explicit EVT-01 requirements. This map answers
layout consolidation;it does not deliver that translator or complete game VM.
