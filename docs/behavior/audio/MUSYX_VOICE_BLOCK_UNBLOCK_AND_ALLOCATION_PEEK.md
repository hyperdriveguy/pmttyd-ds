# MusyX voice block, unblock and allocation peek

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete voiceBlock802855c8/1c4,
voiceUnblock8028578c/218 and voiceAllocatePeek802850ec/78hex. Addresses/offsets/
masks hexadecimal; scalar literals decimal. Static mutation/request order,
not original reservations, replacement or audio observations. Read
[selection/accounting](MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md),
[priority/free writers](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md),
[hardware active/break](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md) and
[macro inactivity](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).

## Shared bases and validation boundary

synthVoice pointer8041ee90, stride458hex. Free records80406940, stride4:
previous byte0/next byte1/flag half2. Free root/tail bytes8041ef39/38;
Music/FX running8041ef36/37. Priority aids use
[the same domains](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md).
The wrappers add no sndActive/IRQ/null/64-slot bounds/identity-generation/member/
cycle/alias/concurrency validation. Search contributes its own idle gate and
selection policy. Raw ordinals and low-byte identity indices are distinct.

## Complete block search and accounting

voiceBlock retains raw requested priority P. Request
voiceAllocateFind(P,255,65535,1), in order (priority, group cap, group word, FX
kind); use search result S. It does not request voiceAllocate. S==ffffffff
skips list/accounting and all record/child work, returns S. The second sentinel
check uses wrap32(S+10000)==ffff, equivalent to that exact missing value.

For S!=ffffffff, free record F=freeBase+wrap32(S*4), without a low8 mask:

- F.flag==1: unlink from free list. Previous!=255 writes F.next to predecessor
  next; otherwise root=F.next. Read F.next fresh; non255 writes F.previous
  fresh to successor previous. If S equals fresh tail, tail=F.previous fresh.
  Then F.flag=0. Own links are not cleared.
- Other F.flag: read voice from fresh synthVoice+wrap32(S*458), read byte11d;
  nonzero decrements FX running low8, zero decrements Music running low8.
- After either path, freshly read/increment FX running low8.

No saturation/underflow repair. A free slot increments FX only; an allocated
Music slot transfers one count to FX; allocated FX decrements then increments
FX with separate fresh reads. Invalid non1 free flags follow the allocated path.
The wrapper does not unlink old priority membership at this point.

## Block record publication and child order

For a selected S, compute retained offset wrap32(S*458). Reload synthVoice
separately for each record operation/request below:

1. Store byte11c=1 (search suppression/protection field).
2. Store byte11d=1 (FX kind).
3. Store word100=ffffffff (group word).
4. Request vidRemoveVoiceReferences(record).
5. Store wordf4=RAW S OR ffffff00. This is not a newly generated ordinary ID,
   and the expression does not first mask S to a byte.
6. Request hwIsActive(RAW S); nonzero requests hwBreak(RAW S).
7. Request macMakeInactive(record,2).
8. Store word34=0.
9. Request voiceSetPriority(record,RAW P).

Return retained S. All child returns except the activity branch are ignored;
no streamKill, voiceKill/free, word110 metric reset or whole voice zero occurs.
Separate synthVoice reloads can target different pools under unsupported
mutation; the original body does not validate that identity/group/protection/
priority publication or accounting forms an atomic reservation. The complete
priority setter can early-return if old membership/priority already matches,
so a hardware priority refresh is not unconditional.

## Complete unblock and retained-pointer recycle

voiceUnblock receives RAW ordinal I. Exact ffffffff returns without hardware,
record or list work; there is no test of byte11c or a masked synthetic identity.
Otherwise request hwIsActive(I); nonzero requests hwBreak(I). Compute
wrap32(I*458), reload synthVoice and store record.wordf4=RAW I. Reload synthVoice
again, capture V at that offset and request macMakeInactive(V,2).

The remainder inlines the
[complete free-writer local sequence](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md):
derive priority entry from U8(fresh V.wordf4); exact membership1 unlinks voice
neighbors/head and, for the only-node case, sort neighbors/root. Sort next is
half0, previous half2; clear only membership. Then V.word34=0, V.byte10c=0.
Derive free record/slot from U8(fresh V.wordf4); only free flag0 appends, sets
flag1, updates root/tail and decrements FX if fresh V.byte11d nonzero, otherwise
Music, wrapping low8. Nonzero flag skips append/count change. Finally
V.wordf4=ffffffff.

After recycling, RELOAD synthVoice again and store byte11c=0 at the original
raw-I offset. This final store need not target retained V if the base changed.
Byte11d/group100/metric110/chainec are not cleared. The body does not request
voiceFree by call, release sample resources, remove VID references, clear stream
state or return an explicit success/failure value. Caller ordinal/protection/
identity/count domains are still needed; repeated/malformed calls are not proved
idempotent by the membership and free-flag gates.

## Complete allocation peek and output validity

Inputs (P,G,Q,F,O); retain output pointer O and forward the first four raw values
to voiceAllocateFind. Resultffffffff returns0 without writing O. Otherwise read
free record at freeBase+wrap32(S*4). Exact free flag1 also returns0 without
writing O. Every other flag value captures fresh synthVoice+wrap32(S*458), reads
record.word100, stores that word to O, and returns1. No output-pointer check,
whole ID/priority/kind/activity check, list mutation, accounting or allocation
occurs in this wrapper. Search miss and selected-free are both result0; neither
initializes an output fallback. Result1 reports the selected record's group word,
not the selected slot or a public/internal voice ID.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_voice_block_unblock_peek_evidence.json) pins
three complete bodies and six linked child identities: nine spans,180 anchors/
ten direct calls. Seven block-counter/seven
synthetic-identity/four peek vectors are mathematical deductions, not original
execution. Source byte/hash/call verification does not establish child lifetime,
valid caller arguments or DSP/audio completion. Full fresh inspection ELFs/
listings stay `/tmp`; no original binary bodies/assets committed.

Remaining: VID reference-removal semantics; actual block/unblock/peek callers,
priority/metric/kind/identity/count/synthVoice producers, valid raw ordinal/
synthetic ID/member/sorted/cycle/alias/reinit/IRQ/concurrency/accounting/rollback
and stream/resource domains; original reservation/free/replacement/audio traces.
EVT-01 remains OPEN.
