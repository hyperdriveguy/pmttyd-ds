# Message resource loading, lookup and command tokenization

US G8ME01 revision0, recovered 2026-10-12. B evidence is original DOL
instructions/data and read-only ISO resource census. Names annotate services.
See [baseline](../../ORIGINAL_BEHAVIOR.md), [event runtime](../event/EVENT_RUNTIME.md) and
[announcement messages](../battle/BATTLE_ANNOUNCEMENT_MESSAGES.md). No original execution
or Switch equivalence was measured. Original text/code extracts remain ignored.
Exact message IDs, text bytes and control tokens are required story data;
this document does not authorize paraphrasing or fabricated dialogue.

## Observed US resource format (B)

msg/US contains260 files. A bounded read-only scan found13,186 complete key/text
pairs, zero unmatched keys, zero nonzero bytes after each resource terminator,
and two duplicate key occurrences within resources. There are4,366 bytes≥128
across the loader-visible strings. Do not assume UTF-8 or normalize those bytes
from this count alone; glyph mapping and decoding still need recovery.

The loader-visible stream is alternating NUL-terminated key and text strings,
ending at the first empty string. No embedded count/header is read by the loader.
Offsets and sizes refer to bytes. Runtime offsets are constructed from strlen,
not from decoded character counts. The metadata census preserves every pair's
key bytes (hex), key/text offsets, text size/hash, duplicates and literal tag
lexeme counts, without exporting dialogue text. Lexemes are not interpreted
control commands or proof of script branch/character attribution.

Reproduce with tools/message_resource_evidence.py; its output stays ignored at
artifacts/reference/behavior/message-resources.json. Bounds validation belongs
to inspection tooling; the original loader itself relies on NUL termination
rather than checking its raw file size on every string traversal.

## Slot record and resource paths (B)

Global pointer at r13−0x6e70 (US0x804160b0) references two16-byte slot records:

| Slot-relative offset | Established role |
| --- | --- |
| 0 | loaded file length word |
| 4 | raw resource buffer pointer |
| 8 | signed pair count |
| c | smart allocation handle; its first word points to pair-index storage |

msgLoad at0x8008465c,size0x2dc receives(resourceBaseName,slotIndex). It computes
slotIndex*16 and uses that record without a local index bound. Normal search
later visits only slots0/1. Do not extend invalid indices into supported gameplay.
Path format is `./%s/%s.txt`, combining language directory and base name, then
prefixed through `%s/%s` using the result of helper0x80007388 (DVD root path).
Language directories indexed by the global configuration word+0x16c are:
0 msg/JP,1 msg/US,2 msg/DE,3 msg/FR,4 msg/SP,5 msg/IT,6 msg/US.
Index6's US reuse is literal data; language display identity is not inferred.
No local language-index validation exists here.

## Loading and storage lifetime (B)

msgInit at 0x80084938, size 0xbc, creates pose `hoshi_2` with argument 2,
stores its ID at message-work+0x20, selects `HOS_C_1` with argument 1,
sets material bits 0x1800, clears 0x40 and installs msgDispKeyWait_render
(0x80083360) with argument 0. It nulls raw/index pointers in slots 0 and 1,
then calls msgLoad("global",1). It does not free previous pointers before
nulling them or explicitly zero every work field; repeated initialization and
release policy remain caller/lifecycle questions. The pose is the original
key-wait presentation dependency, not a substitute generated UI asset.

If raw pointer is null and slotIndex=1, probe the named resource in all seven
language directories. For each successful DVDMgrOpen(path,2,0), obtain length,
close handle and retain the signed maximum length. Allocate aligned32-byte size
`(maxLength+31)&~31` through __memAlloc(0,size), store raw pointer. This reserves
space across languages once; later loads with an existing raw pointer do not
repeat the max-length scan or resize this buffer locally.

Set record length0, build current-language path, open DVDMgrOpen(path,2,0).
If successful, write its length. For slotIndex0, allocate a raw buffer with
_mapAlloc(globalMapHeap,align32(length)); other indices use the existing pointer.
Read align32(length) bytes through DVDMgrRead(handle,raw,roundedLength,0), then
close. There is no local allocation/read-result check before proceeding.
Raw-buffer release/reuse outside this routine remains a lifecycle dependency.

If raw pointer is null afterward, set length and pair count0. Otherwise scan
NUL strings until the first empty string, incrementing stringCount and advancing
by strlen+1; store signed trunc(stringCount/2) as pair count. This scan is not
bounded by the stored file length. A failed current open with a retained raw
pointer can therefore rebuild from retained contents; this local path does not
unconditionally clear old bytes or report a separately computed load-success
boolean. Original caller/allocation invariants and failure policy need tracing.

If a prior smart index handle exists, smartFree(handle). Pair count0 writes
index handle null and exits. Otherwise smartAlloc(pairCount*8,0), store handle,
dereference its first word as index base. No local null-allocation fallback.

## Index building and comparison (B)

For each pair in source order, write two32-bit offsets: key start, then text
start. Advance over key NUL and text NUL using strlen. The raw text/key bytes
are retained; the loader does not rewrite dialogue, replace tokens or reflow.
Set global comparator base pointer at r13+0x1b14 to this raw buffer and call
qsort(indexBase,pairCount,elementSize8,msg_compare).

msg_compare at0x80081c6c,size0x48 reads each entry's key offset, adds the shared
raw-buffer base, strcmp keys, and returns−1/0/1 by the comparison sign.
It does not compare text, original index or key length. The qsort implementation's
equal-key ordering still needs recovery. Duplicate keys must be retained until
that ordering and search behavior are established; an assumed dictionary
last-write-wins rule is not equivalent evidence.
Global comparator state also makes concurrent index builds a scheduling concern;
the loader itself establishes no per-sort independent comparator context.

## Lookup order and binary-search bounds (B)

msgSearch at0x80081b8c,size0xe0 receives a key pointer, with no local null guard.
Search slots0 then1. Skip a slot if its smart index handle is null.
Otherwise use its handle's first word as sorted pair array and raw pointer as
string base. Initialize low=0, high=pairCount (not count−1). While high≥low:

1. Compute signed trunc((low+high)/2) with original word arithmetic.
2. If midpoint≥pairCount, abandon this slot before reading its entry.
3. strcmp(requestedKey,raw+entry.keyOffset).
4. Equal: return raw+entry.textOffset directly, not a copied/decoded string.
5. Negative: high=midpoint−1. Positive: low=midpoint+1.

First matching slot wins, irrespective of whether slot1 also contains the key.
Comparison is exact byte-string strcmp, not locale-aware/case-insensitive search.
Returned pointer lifetime depends on raw-slot replacement; no ownership transfer
or new allocation occurs here. Do not hold it across unknown load/release events
without tracing caller lifetimes.

If neither slot finds a match, select a fallback pointer from the seven-entry
table at US0x804160b4 using the same configuration word+0x16c. This is an original
localized fallback message, not null, the requested key or a newly invented
error string. Original fallback bytes/control tokens remain reused assets.
There is no local index bounds check or story-state branching in msgSearch.

## Command name/argument split (B)

msgGetCommand at0x80081aa0,size0xa0 receives(input,nameOutput,argumentOutput).
Caller supplies the command-content start; this function does not require or
consume an initial '<'. Process at most128 input bytes with a local state
initially "name":

- Byte '>': write NUL at the current name output when still in name state,
  otherwise at the current argument output; return pointer after '>'.
- First ASCII space: terminate name, enter argument state, consume the space
  without copying it.
- Later spaces: copy literally into argument output.
- Other bytes: copy to current output and advance that output pointer.

If all128 bytes are consumed without '>', return null. It does not stop on
input NUL or insert final terminators on that failure path. There is no output
capacity parameter. Tabs, newlines, slashes, numbers and high bytes are copied
as ordinary bytes here; later argument parsing/control semantics are separate.
This is not an XML parser and does not support quoting/escaping merely because
control markup uses angle brackets. Caller scanner/nesting/command dispatch
in msgAnalize is partly recovered in [parser records](MESSAGE_PARSER_RECORDS.md).

## Multibyte lead predicate (B)

_ismbblead at0x80081b40,size0x4c checks configuration word+0x16c first.
If nonzero, return0 for every input. If0, narrow input to unsigned byte and
return1 for0x81..0x9f or0xe0..0xfc, otherwise0. No trail-byte validation,
conversion or glyph lookup occurs here. These ranges match a Japanese lead-byte
classification but do not establish the complete game's encoding contract.
US configuration1 follows the single-byte path in this predicate; high-byte
font/control handling still requires its callers and tables.

## Differential obligations and remaining work (P)

Compare slot precedence, empty slots/count0, exact case/non-ASCII key bytes,
duplicates after original qsort, first-empty terminator, failed load with retained
raw buffer, slot1 max-language capacity, pointer reuse, missing-key locale fallback,
command first/later spaces, '>' at byte128, NUL before delimiter, no delimiter,
and Japanese lead-byte boundaries versus US mode. These are derived cases, not
executed-original observations.

Remaining: release and all initialization/load callers, root/DVD/heap failure
semantics, qsort equal-key ordering, msgAnalize commands/variables/branches,
font byte-to-glyph/control decoding and story event attribution/order/conditions.
The resource census and local lookup/tokenization contracts are documented;
full dialogue and full-game equivalence remain unfinished. Parser-side glyph,
wait/style/window/selection behavior is specified in
[parser records](MESSAGE_PARSER_RECORDS.md), with remaining record paths explicit.
