# MusyX active macro initialization and last-started voices

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete macHandleActive initialization
region80282920..80282b94 exclusive, voiceSetLastStarted80286070/54hex and
voiceInitLastStarted80286138/144hex. Other interpreter opcodes remain open.
Addresses/offsets/masks hexadecimal; counts/scalar values decimal.
Static source ordering, not original voice start/restart or audio measurements.
Read [scheduler/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[controller init/defaults](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md),
[controller read services](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md),
[selectors/first use](MUSYX_MACRO_INPUT_SELECTORS_FIRST_USE_AND_DIRTY.md) and
[job-start timestamps](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).

## Admission and optional hardware break

Retain raw voice pointer V after ABI saves. Read V.word118, then V.word114.
Only word118&3hex contributes; word114 is masked with0. Result0 skips ALL this
initialization, continuing directly to dispatch's debug-counter reset.
Any low bit0/1 set admits initialization; no state-byte/active/ID/null/IRQ gate.

If CAPTURED word118 bit1hex is set: store captured low&fffffffehex at118,
then captured high unchanged at114; read fresh V.wordf4.low8 and request
hwBreak(index). Ignore result. If bit1hex clear but bit2hex set, skip the break
and proceed. No flag reread to cancel admission after hardware return. This
initial clear is distinct from the later fresh flags reset; no whole voice kill,
allocation/free or success/rollback protocol is added.

## Exact pending-field copies before input initialization

Read byte215, shift left16 and store the same retained value at word184 then174.
Store word188=0 then178=0. Read byte214, shift left16 and store word158.
Store byte196=0; reread word158 and store word15c. Then perform ascending source
byte reads and destination stores:

| Pending source | Destination |
| --- | --- |
| byte216 | byte121 |
| byte217 | byte122 |
| byte218 | byte123 |
| byte219 | byte120 |
| byte21c | byte197 |

Store byte106=0, half154=0, half170=0, in that order. Request inpInit(V).
That complete child installs its58 matrix/voice defaults AND global low/high
frequency defaults. There is no whole-record memset here before those copies,
no coefficient/unit normalization or pending-source generation validation.
Unwritten fields remain governed by earlier initialization/other owners.

## Previous/current note and last-started publication

After inpInit, load fresh channel V.byte121 then set V.byte122, request
inpGetMidiLastNote(channel,set). If returned low8!=255, store it at V.byte132;
otherwise read fresh V.byte131 and store it at132. This is the exact255 fallback;
no error/refusal on missing previous note.

Then reload channel, set and current-note byte131, request
inpSetMidiLastNote(channel,set,currentNote), ignoring result. Request
voiceSetLastStarted(V) next. These are separate global table updates, not a
captured pair/atomic note history. Caller mutations can affect later fresh inputs.

Copy fresh pending byte21a to byte11e, then21b to11f. Set word140=0,
word138=25600, byte133=0. Read channel byte121:255 stores half134=0 without
controller call; otherwise read set byte122, request inpGetMidiCtrl(65,channel,
set) and store returned low16 at half134. No descriptor evaluation is used for
this local controller65 query. The packed defaults getter still runs afterward
for either channel value; it does not acquire this channel255 rejection rule.

## Packed channel-default import and exact reset suffix

Reload channel and set; request inpGetChannelDefaults(channel,set), retaining
returned record pointer R with no null/type/bounds/success check. Read R.byte0,
store V.byte1da. Read R.byte0 AGAIN, store V.byte1db. Read R.word1/store V.word1e0;
read R.word5/store V.word1e4. Preserve packed9-byte record offsets and fresh
separate reads; do not pad the record or reuse the first byte read for both stores.

Then perform the following ordered raw constant writes. Width4 is word, width2
halfword, width1 byte; values decimal:

| Ordinal | V offset | Width | Value |
| --- | --- | --- | --- |
| 0 | 194 | 1 | 128 |
| 1 | 195 | 1 | 0 |
| 2 | aa | 2 | 0 |
| 3 | 1bc | 1 | 0 |
| 4 | 1bd | 1 | 0 |
| 5 | 1a4 | 4 | 0 |
| 6 | 1a8 | 4 | 0 |
| 7 | 1c4 | 4 | 0 |
| 8 | 1c8 | 2 | 0 |
| 9 | 1ca | 2 | 32767 |
| 10 | 1d0 | 4 | 0 |
| 11 | 1d4 | 2 | 0 |
| 12 | 1d6 | 2 | 32767 |
| 13 | 50 | 4 | 0 |
| 14 | 54 | 4 | 0 |
| 15 | 58 | 4 | 0 |
| 16 | 68 | 1 | 0 |
| 17 | 124 | 4 | 4294967295 |
| 18 | 128 | 4 | 4294967295 |
| 19 | 12c | 2 | 65535 |
| 20 | 1dc | 2 | 8192 |
| 21 | 454 | 2 | 0 |

The event pointer words50/54/58 and enable byte68 are cleared without event/
resource release or a corresponding callback. Other fields are not implicitly
reset because these local constants exist.

## Fresh flags, local variables, clock snapshots and job start

After the suffix, read fresh V.word118 then114. Store118=low&8hex,
store114=high&0 (zero), reread114, OR3000hex and store114. Final own flags are
low's retained bit8hex and high3000hex; there is no preservation of earlier
first-use masks, init bits or arbitrary high flags. Child/concurrency domain
still determines later changes. Known selector first-use bits are clear after
this local reset, before subsequent selector commands.

Request memset(V+achex,0,64), clearing16 full local-variable words. This happens
AFTER the flag stores; it does not clear global variables, the whole voice or
event/controller tables. No byte-copy/alias/failure policy is added to the child.

Read macRealTime HIGH8041ef10 then LOW8041ef14; store LOW at V.worda4 then
HIGH at V.worda0. Reread HIGH then LOW independently; store LOW at V.word94,
HIGH at V.word90. These two pairs can represent different source snapshots;
no atomic64-bit copy or common retained timestamp. The first pair can overwrite
an outer scheduler's earlier deadline publication if this initialization is
admitted. No physical clock/tick units or generation equality inferred.

Finally request synthStartSynthJobHandling(V), ignoring result, then enter the
existing dispatch fetch region at80282b94. The complete job-start child owns
its insertion/timestamps. No voice PC/command-pointer reset, sndActive transition,
original audible-start guarantee or independent status assignment is made by
this initialization region.

## Complete last-started setter and table initialization

voiceSetLastStarted(V) reads full ID=V.wordf4. Exactffffffff returns before any
table write. Otherwise read channel byte121; exact255 returns. Then read set
byte122 and index=ID.low8. Set255 writes byte index to table80406a40+index;
other sets write byte index to80406a80+set*10hex+channel. The FX address uses
ID.low8 as BOTH index and stored value, not channel as its table index.

No full-ID/generation recheck, active/IRQ/bounds/null guard or dirty flag. Original
FX table has64 bytes; MIDI table128 bytes (eight sets/16 channels). The body
enforces neither shape beyond its two sentinel returns. A non-missing ID with
low8=255 can still derive an out-of-table write. Return is incidental.

voiceInitLastStarted fills the MIDI128 bytes at80406a80..80406aff with255
ascending (four groups of32), then FX64 bytes at80406a40..80406a7f with255
ascending (two groups of32). No runtime count, voice/table scan, setter call,
ID reset or last-note/controller mutation. It is called separately by synthesis
initialization, not by every active macro initialization.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_active_initialization_evidence.json) pins
two complete bodies,one complete initialization region,ten linked fingerprints
including whole macHandleActive:13 spans,244 instruction anchors,ten direct
calls. It records six stable initial-flag/five pending-byte/four previous-note/
six last-started vectors,128+64 ordered table-reset addresses and22 suffix
field writes. Stable examples do not model child/concurrency mutations; prose
retains fresh-read order. Whole macHandleActive hash is not closure of its
remaining opcode semantics. Full fresh inspection ELFs/listings stay
`/tmp/inbox-macHandleActive.elf`, `/tmp/inbox-macHandleActive.listing`,
`/tmp/inbox-voiceSetLastStarted.elf`, `/tmp/inbox-voiceInitLastStarted.elf`.
No original bodies/binary assets committed or original code executed.

Remaining: other interpreter opcode paths, actual pending/ID/note/channel/set/
flag/PC/command/clock writers and assets, last-started consumers and
memset behavior, valid table/pointer/bounds/alignment/alias/IRQ/concurrency/
reinit/first-mode/r28 domains and original initialize/previous-note/selection/
job/audio traces. Region/two child bodies are answered; EVT-01 and full native/
source/runtime/hardware equivalence remain OPEN.
