# MusyX hardware messages and sample notification order

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete local bodies:
synthHWMessageHandler80279728/d0hex, hwGetVirtualSampleID802915f4/28hex,
vsSampleEndNotify80286ee4/a8hex, macSampleEndNotify80283a08/achex,
vsSampleStartNotify80286c18/2cchex. Addresses/offsets/masks hexadecimal;
counts/indices/message tags decimal. Static request/store semantics, not
original sample timing, callback execution or physical audio evidence.
Read [installation](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md),
[voice kill](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md) and
[macro activation](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md).

## Complete installed message dispatch

Handler receives raw (message,V) in r3/r4; retain V and initialize return0.
The message comparison uses the full signed word, not its low byte:

| Exact message | Ordered path | Handler return |
| --- | --- | --- |
| 0 | Voice byte11c gate; hardware virtual ID; virtual end; fresh identity check; possible macro end | 0 |
| 1 | voiceKill(V.low8)802859a4 | 0 |
| 2 | vsSampleStartNotify(raw V) | Raw child return |
| 3 | hwGetVirtualSampleID(V.low8), then vsSampleEndNotify(returned ID) | 0 |
| All others | No child/store request | 0 |

Message0 selects Q=synthVoice pointer8041ee90+V.low8*458hex, retains that
displacement and reads Q.byte11c. Nonzero skips everything. Zero requests
hwGetVirtualSampleID(V.low8), then vsSampleEndNotify(its raw return), including
ffffffff. AFTER virtual-end return, reload synthVoice, derive Q using retained
displacement and read Q.wordf4. Only if it equals original full V request
macSampleEndNotify(Q). No pre-notification identity check, captured pointer
reuse, null/index/count/active-state check or post-macro identity recheck.

Message3 omits byte11c and full-ID checks and never invokes macro end.
Message1 passes a byte index to voiceKill, not the full-ID voiceKillSound API.
Message2 alone propagates a child result. No direct IRQ/active flag gate or
caller ownership/context repair in this installed handler.

## Complete hardware virtual-ID leaf

hwGetVirtualSampleID(I) selects P=word8041ef88+wrap32(I*f8hex), with no input
truncation or count guard of its own. Read P.bytef0: zero returnsffffffff;
nonzero returns raw P.wordec. It does not modify hardware/sample state or check
wordec generation. The message handler supplies I=V.low8; direct callers need
their own valid index/pointer invariant.

## Complete virtual end matching and callback cleanup

Virtual table T=803f6b00, slot stride2chex, mapping bytes T+b08, user callback
word T+b4c. vsSampleEndNotify(X) first rejects exactffffffff. Otherwise:

1. Load map[X.low8]. Byte255 returns without further work.
2. Select S=T+mapByte*2chex, with no slot-count/bounds/state-byte guard.
3. Compare S.half1a with (X>>8)&ffff. Mismatch returns.
4. Read fresh callback word T+b4c. If nonnull, invoke callback(2,S+18).
   Ignore callback result. No defined extra payload argument is assigned here.
5. After callback, reconstruct the SAME selected slot from retained displacement;
   store S.byte8=0. Read fresh S.byteb and clear map[S.byteb]=255.

The mapped slot and generation are not revalidated after callback. A callback
that changes byteb changes WHICH mapping byte is cleared; the original X.low8
mapping is not independently repaired. Null callback still performs cleanup.
No hardware loop-buffer clear, macro notification, payload wipe or counter
increment in this end body. Returns are incidental, not an independent status.

## Complete macro end event restoration/wakeup

macSampleEndNotify(Q) first requires Q.word4c==1. Otherwise return. With state1,
if Q.byte68!=0 AND Q.word54!=0, perform this exact order:

- Q.word38=Q.word60.
- Q.word34=fresh Q.word54.
- Q.word54=0.
- Request macMakeActive(Q)80283d50; ignore result and finish.

This path bypasses the fallback flag test. If either event condition fails,
read Q.word114 then Q.word118. The instructions mask the former with0 and the
latter with00040000hex; only word118's bit00040000 contributes. If set, request
macMakeActive(Q); otherwise return. Do not reinterpret word114 bits as another
wakeup condition or add a byte68 reset, event-pointer fallback, voice start,
scheduling tick or explicit status. The complete macro activation body is linked
above; interpreter/event/flag providers remain separate requirements.

## Complete virtual start reclaim and first-free selection

vsSampleStartNotify(raw V) retains full V and hardware voice byte v=V.low8.
Start by scanning slots ascending. At EVERY loop test read fresh T.byte0 count;
compare low8 ordinal with that count. For a slot with byte8!=0 and byteb==v:
store byte8=0, reread byteb, clear its mapping byte=255. Continue through all
slots; no end callback, generation/full-ID match or hardware request here.

The first-free scan then uses the count retained from the previous terminal
loop test. It does not refresh that count. Scan ascending for byte8==0. FIRST
free slot gets byte8=1, then wordc=0, and ends the search. If none, selected
ordinal=255. Publish map[v]=selected.low8 in either case, BEFORE subsequent
hardware calls. No reservation rollback before this publication or count/bounds
validation. Zero count reaches the no-slot path.

No-slot path requests hwSetVirtualSampleLoopBuffer(v,0,0)80290bc4 and returns
ffffffff. If a slot was reserved, retain its ordinal and:

1. Read fresh map[v], request aramGetStreamBufferAddress(map[v],0)80292290.
2. Read fresh T.word4, request hwSetVirtualSampleLoopBuffer(v,address,T.word4).
3. Request hwGetSampleID(v)80290c0c; store returned low16 at S.half18.

The stream address request uses fresh mapping, not retained selected ordinal.
The slot reservation and mapping already exist while these children run;
byteb/generation/public-ID/extra/type payloads are not yet populated.
No child success/identity/mapping/count recheck is added.

## Generation selection and remaining start payload

For each candidate attempt, read fresh T.byte0 count and T.halfb48 counter g;
store counter=(g+1).low16, but retain OLD g as candidate. Scan the captured
count ascending, checking slots whose byte8!=0 for half1a==g. Any match retries
with a fresh count/counter. No match admits g. The newly reserved slot itself
is included and can still contain an earlier half1a. No special skip of that
slot, reserved token or finite retry limit. Candidate65535 is allowed; the
stored counter wraps to0. This is source selection, not a proof of global
uniqueness under concurrency or arbitrary table contents.

Store S.half1a=g. Request vidGetPublicId(original full V)802849b4 and store its
raw return at S.word1c. If return!=ffffffff, request
seqGetInstanceForVoice(fresh S.word1c)80279f30 and store its return at S.word20;
otherwise store S.word20=ffffffff. Then request hwGetSampleExtraData(v)80290c20,
store S.word24=return; request hwGetSampleType(v)80290bf8, store S.bytea=return.low8;
store S.byteb=v. No record-wide clear or aliasing/source repair.

## Start callback acceptance versus cleanup

Read fresh callback word T+b4c. If nonnull invoke callback(0,S+18). A returned
ZERO accepts; freshly read S.half1a AFTER callback and encode:

`return = (freshGeneration<<8) | retained original v`.

No generation/mapping/state revalidation after callback. Return uses retained v,
not fresh S.byteb, so mutations of that byte do not alter encoded low8.
A null callback or any nonzero return REFUSES:

1. Request hwSetVirtualSampleLoopBuffer(v,0,0).
2. Reconstruct retained selected S and store byte8=0.
3. Read fresh S.byteb; clear map[S.byteb]=255.
4. Returnffffffff.

Generation increment and payload writes are not rolled back. No refusal/end
callback or macro/voiceKill request follows. A callback's mutation of byteb can
redirect mapping cleanup as in end. Acceptance is precisely callback return0;
do not reverse it using generic success conventions. The start result propagates
only through message2; other message paths return0 regardless of child results.

## Validation and remaining contracts

[Manifest](../../../tools/musyx_hw_message_and_sample_notifications_evidence.json)
pins five complete bodies/11 linked fingerprints:16 spans,201 instruction
anchors,20 direct calls. Nine message/eight ID/15 generation/eight macro-wakeup/
four callback-admission/five stable-slot vectors are source-derived checks.
Stable examples do not model child mutations, concurrent writers or original
execution; the prose retains the fresh-read/request order above. ABI entry
hashes are not complete fallthrough contracts. Fresh full ELFs/listings stay
`/tmp/inbox-synthHWMessageHandler.elf`, `/tmp/inbox-hwGetVirtualSampleID.elf`,
`/tmp/inbox-vsSampleEndNotify.elf`, `/tmp/inbox-macSampleEndNotify.elf`,
`/tmp/inbox-vsSampleStartNotify.elf`. No original bodies/assets committed or
original code executed.

Remaining: virtual table/count/counter/map/callback and voice/event/flag/ID
providers, stream-buffer/hardware/VID/sequence-instance children, actual callback
implementations and mutation contracts, pointer/index/count/alias/generation/
IRQ/reentrancy/concurrency/loop-termination invariants and original start/end/
refusal/reuse/message/audio traces. Five local services are answered; EVT-01 and
full native/source/runtime/hardware equivalence remain OPEN.
