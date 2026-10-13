# MusyX virtual-sample refill descriptor and cursor

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete vsUpdateBuffer80286f8c/358hex.
Addresses/offsets/masks hexadecimal; counts/indices decimal. Static source
requests/writes, not original refill throughput, buffer contents or playback
measurements. Read [updater/position](MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md)
and [start/end callbacks](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md).

## Input pointer, cursor and descriptor fields

vsUpdateBuffer(Q,P) retains the raw slot-state pointer Q; P is a raw word in r4.
Q is the updater's T+8+slot*2chex, where T=80406b00. The function itself performs
no slot lookup or index conversion. Relevant fields:

| Field | Offset/address |
| --- | --- |
| Sample type byte | Q+2 |
| Refill cursor word | Q+4 |
| Callback payload pointer | Q+10hex |
| First segment encoded offset | Q.word1c |
| First segment length | Q.word20 |
| Second segment encoded offset | Q.word24 |
| Second segment length | Q.word28 |
| Global loop size | T.word4=80406b04 |
| Callback pointer | T.wordb4c=8040764c |

The cursor Q.word4 differs from the updater's raw previous-position wordc and
remaining word8. The callback payload begins before the four descriptor words;
do not replace it with a standalone segment struct or omit earlier sample/
generation/association/extra-data payload fields.

Capture C=Q.word4 once at entry and compare C/P UNSIGNED. Equality returns with
no field store or callback, before reading the sample type. Otherwise branch
on C<P versus C>P, and in the latter branch P==0 versus P!=0. Each admitted
branch reads sample type Q.byte2; only exact5 or6 performs refill. Other types
return without descriptor/cursor writes or callbacks. No state-byte/active/
valid-generation/count/index/null/loop-size/range/IRQ check in this body.

## Exact offset encodings

All admitted branches encode their first offset from CAPTURED entry cursor C:

| Type | First encoded offset |
| --- | --- |
| 5 | floor(C/14)*8, original multiply-high/subtract/shift/add/mask sequence |
| 6 | wrap32(C<<1) |

Type5 uses constant24924925hex, multiply-high h, then
`(h + ((C-h)>>1)) & fffffff8hex`. It produces eight encoded units per complete
14 cursor units. This differs from the parent's type5 position rounding, which
produces multiples of14; retain both transformations and their separate uses.
There is no type5 remainder carry or cursor alignment guard here. Type6 shift
wraps; it is not a widened host multiplication with overflow rejection.

## Complete forward and wrap descriptor order

After storing first offset at Q.word1c, reread F=Q.word4. Entry C still determines
the branch and encoded offset; F determines first length. Store the remaining
three descriptor words in this exact order:

| Entry condition | Q.word20 first length | Q.word24 second offset | Q.word28 second length |
| --- | --- | --- | --- |
| C<P | wrap32(P-F) | 0 | 0 |
| C>P and P==0 | wrap32(fresh T.word4-F) | 0 | 0 |
| C>P and P!=0 | wrap32(fresh T.word4-F) | 0 | Original P |

For wrap branches load fresh cursor BEFORE fresh global size. No global size
read for forward descriptor construction. Type5/6 use the same lengths; only
first offset encoding differs. The nonzero wrap branch submits BOTH tail and
head segments in one descriptor/callback. It does not request two callbacks,
merge the ranges or infer an additional lap. Zero P submits a tail segment and
zero head; it does not suppress a nonempty tail.

No length clamping, bounds verification, zero-length rejection, fresh type
recheck, state change or rollback. Invalid/mutated cursor/size/input can derive
wrapped lengths; valid domain requirements belong to providers/callers.

## Six callback sites and fresh post-callback cursor advancement

Each of the three admitted range branches has separate type5/type6 source paths,
for SIX indirect call sites. All perform the same request AFTER all four stores:
read fresh T.wordb4c and invoke callback(1,Q+10hex). No callback-null check;
no defined third argument is assigned by this service. The updater's earlier
nonnull admission is not repeated here and does not prove safe reentrancy or
pointer stability through intervening children.

Let callback return raw word R. Exact R=0 returns without a parent cursor write;
callback mutations of cursor/descriptor/global size are not rolled back.
Any NONZERO R reads fresh Q.word4, THEN fresh T.word4, forms
A=wrap32(freshCursor+R), and performs original divwu/mullw/sub sequence:

`Q.word4 = wrap32(A - unsignedQuotient(A,freshLoopSize)*freshLoopSize)`.

For nonzero loop size this equals A modulo freshLoopSize. The sum wraps before
division; a host64-bit `(cursor+R)%size` can differ. There is no R<=requested
length check, minimum advancement, sign convention, descriptor reread or second
callback. Even ffffffff is a nonzero advancement word. Zero loop size is
unchecked; no valid remainder/error behavior is established for that domain,
and no invented divide-by-zero repair is added.

The cursor uses fresh values after callback, not retained entry C or the
pre-callback length cursor F. Any callback mutation of Q, loop size, descriptors
or callback pointer requires its actual implementation contract. No identity/
map/generation/slot-state revalidation after return. Parent return is incidental,
not an independent success status; vsSampleUpdates ignores it.

This callback tag1 return convention differs from start tag0: refill nonzero
advances the cursor; start callback ZERO accepts the reservation. End tag2's
return is ignored. Do not impose a shared boolean-success interpretation.

## Local source boundary and validation

The complete body writes four descriptor words and, only after nonzero callback
return, the refill cursor. It does not itself transfer sample bytes, upload ARAM,
flush cache, configure hardware, kill/break a voice, notify macro/end, mutate the
map, update remaining count or install callbacks. Actual callback work and
providers/backend consumers remain required source contracts.

[Manifest](../../../tools/musyx_virtual_sample_refill_evidence.json) pins one
complete body,141 instruction anchors including six indirect call sites, zero
direct child calls. Ten encoded-offset/13 stable descriptor/nine post-callback
vectors and all65536 low-input type5 encoding cases are source-derived checks,
not original execution. Stable examples do not prove callback/concurrency/
alias/range guarantees; the fresh-read order above remains part of the contract.
Full fresh ELF/listing stays `/tmp/inbox-vsUpdateBuffer.elf`; no original body/
asset committed or original code executed.

Remaining: actual callback installation/implementation and byte-transfer work,
slot/type/cursor/position/loop-size/result providers, valid bounds/alignment/
nonzero-size/overflow/alias/generation/IRQ/reentrancy/concurrency domains and
original contiguous/wrapped/partial/zero-return refill/audio traces. The local
refill body is answered; EVT-01 and full native/source/runtime/hardware
equivalence remain OPEN.

### Provider/callsite follow-up

[Scoped virtual table provider/DOL census](MUSYX_VIRTUAL_TABLE_PROVIDER_AND_DOL_ENTRY_CENSUS.md)
now consolidates known configuration writes and pins28 bounded literal-address
candidates/seven exact DOL entry calls. Wider indirect/REL/alias writers,
callback installation/transfer implementation and original execution remain open.
