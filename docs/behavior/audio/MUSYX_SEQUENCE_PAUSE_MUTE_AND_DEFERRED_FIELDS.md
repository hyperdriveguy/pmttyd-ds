# MusyX sequence pause, mute and deferred field publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete seqPause80273488/27c,
seqMute80273c8c/bc and sndSeqMute80279e54/58hex. Read
[private-ID/stop](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md),
[fade callers](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md) and
[note voice-kill child](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
Offsets/addresses hexadecimal; states decimal. Static local service behavior,
not original pause/mute/audio timing or sequencer-consumer execution.

## Both cores inline external-to-private lookup

Retain input external sequence word X; search low31(X) against RAW node.wordc,
first active root8041ee5c, then freshly loaded paused root8041ee58.
First match composes zero-extended node.byte9 with ORIGINAL X bit80000000.
Total miss-1 returns without instance mutation. No delegated seqGetPrivateId,
initial X-1/sndActive/IRQ guard or member/cycle/duplicate/ID range check.
The separate reader contract describes the identical ordering/field roles.

On match, clear tag for array address: seqInstance803ebdc0+
wrap32(privateOrdinal*1888hex). Supplied eight-instance extent is not checked
here; initialized private bytes0..7 need later writer/source guarantees.
Original INPUT tag selects each core's branch, not a tag inferred from node state.

## Untagged pause moves only state1 and preserves instance identity

SeqPause without tag reads selected instance Q.byte8. EXACT1 admits; every other
state returns without list/note writes. No generic unknown-state transition.
For state1, detach active in this order: read Q.previous4; nonnull writes
previous.next=Q.next, otherwise activeRoot=Q.next. Read Q.next freshly;
nonnull writes next.previous=Q.previous. Retain Q across the remaining requests.

Capture pausedRoot and store Q.next=capturedRoot; nonnull root.previous=Q.
Store Q.previous=0; pausedRoot=Q; Q.stateByte8=2, in that order. State2/list
publication occurs BEFORE any note-voice shutdown requests. No sequence-free-list
insertion, external key/private-byte erase or project/resource free.

Then traverse note heads Q.e64, Q.e68, Q.e6c in order. At each node N request
voiceKillSound(N.word8), ignore result, and read N.next ONLY AFTER return.
No captured-next guard; child mutation affects traversal. After all kill scans,
revisit those same three heads in order, walk each nonnull list to tail T;
if noteFree8041ee4c nonnull: tail.next=oldFree, freshly loaded oldFree.previous=tail.
Reload head, store noteFree=head, clear that Q head word0. No head.previous reset,
whole-note erase/CPU free/hardware completion wait. Exact note recycle order
matches the state1 stop path, while this instance remains on paused list.

Own key/private/project/time/other fields remain. Note/list/cycle/alias/concurrency
invariants and actual voice shutdown belong to source/child evidence.

## Tagged pause: deferred byte only

With INPUT high bit set, read Q.stateByte8. Zero returns without stores;
any nonzero reads Q.byteefa, OR8, stores it, then returns. No active/paused
unlink, state2 assignment, note kill/recycle or frame/time request. The byte's
consumer determines eventual action; this body only publishes that bit.
No independent success code, rollback, status/voice/resource mutation.

## Complete mute: raw immediate versus tagged fields

seqMute inputs X, raw words A(r4), B(r5). After lookup:

- Untagged: store A to Q.word11c, THEN B to Q.word120. No state-byte gate,
  bit mask/normalization, MIDI/voice callback, note kill or list mutation.
- Tagged: read Q.byteefa, OR10hex, store it; store A to Q.wordef0,
  THEN B to Q.wordef4. No state-byte gate, even when state0. No consumer call.

Raw words are not reduced to booleans/channel bytes here. Parameter/field
meanings and actual playback/mute application remain reader/source contracts.
Fade mode3 supplies(0,0); these local stores alone do not establish the time or
scope of silence. Tagged pause and mute use distinct bits8/10 in the same byte;
existing other bits survive. Whole operation is not an atomic field snapshot.

sndSeqMute retains all THREE raw input words across hwDisableIrq, calls
seqMute(X,A,B), then hwEnableIrq. No validity precheck/active-system gate/input
conversion, independent status assignment or retained core return after Enable.
Core functions themselves request no IRQ service; the fade caller uses its
separate audio frame critical context. Actual caller/OS ownership remains open.

## Validation and remaining providers/consumers

[Manifest](../../../tools/musyx_sequence_pause_mute_evidence.json) pins three
complete bodies/three linked voice-kill/IRQ bodies:6 spans/160 anchors/five
resolved direct calls.10 pause/6 mute/5 preserved-flag vectors and exact three-head
order are derived checks, not original execution. Fresh full ELFs/listings stay
`/tmp/inbox-seqpause.elf`, `/tmp/inbox-seqmute.elf`, `/tmp/inbox-publicseqmute.elf`.
No original body/asset exported to history.

Remaining: private-ID/state/list/note providers and valid index/member/cycle/
alias/IRQ/concurrency domains, deferred byte/word and mute-field consumers,
resume/sequence/controller/voice scheduling and original pause/mute/audio traces.
Three complete local services are answered; full native/source/runtime OPEN.
