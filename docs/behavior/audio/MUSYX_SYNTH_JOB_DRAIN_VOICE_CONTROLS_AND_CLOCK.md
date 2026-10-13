# MusyX synth job drain, voice controls and clock tail

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthHandle REGIONS
8027884c..80278abc and80278d14..80278d30 exclusive; whole function80278838/510
is fingerprinted but its master-fade/aux middle is NOT closed here.
Complete hwFrameDone802915bc/4hex. Read
[job insertion](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md),
[audio frame caller](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md),
[hardware active flags](MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md).
Offsets/addresses hexadecimal; numeric thresholds/counts explicitly decimal.
Static queue/clock request order, not original physical cadence/audio evidence.

## Complete admission and retained slot

Retain raw input D in local register; read synthInfo803f8540.word4. Zero skips
EVERY request/queue/index/clock mutation and returns. Nonzero requests
macHandle802838b4 with raw input r3 still D. No sndActive/IRQ/nesting guard.
Then read U8 synthJobTableIndex8041ee61 and select slot record at
803f8d94+12 decimal*index. Index is NOT masked before this lookup, unlike insertion;
valid initial index0..31 remains a source invariant. Retain this slot pointer
across all child requests and queue phases below.

## Category0 then category2 then category1

First process retained slot.word0 (category0), next word4(category2), finally
word8(category1). Each phase captures its root ONCE at phase entry. At every node:
capture node.next(word0) BEFORE mutation/calls; store node.slotByte9=ff;
read node.voiceByte8 and fresh synthVoice8041ee90, stride458hex. No node/member/
identity/generation/index/cycle check or node free/link clear.

Category0: read selected voice.byte11c; zero requests
LowPrecisionHandler(voiceIndex)80277724, nonzero skips handler. In BOTH cases
follow captured next until null, then store retained slot.word0=0.

Category2: same stream-byte gate, but zero executes voice controls below;
nonzero skips them INCLUDING the dirty-byte tail. Follow captured next, then
store retained slot.word4=0.

Category1: same stream-byte gate; zero requests
ZeroOffsetHandler(voiceIndex)80277e68, nonzero skips. Follow captured next, then
store retained slot.word8=0.

Clearing the root occurs AFTER traversal, not a root detach before processing.
A handler can see its markerff before the old root is cleared; current-slot
insertions/root changes can be overwritten by the later clear. Captured successor
is still followed after handler mutation. No rescan/new-job guarantee is added.
Node previous links remain; complete insertion/consumer/list invariants are needed.

After all phases, RELOAD global index, store U8((freshIndex+1)&31). This can
advance a different global value than the slot ordinal captured before handlers.
Three roots remain independent; actual handler work is not inferred from names.

## Category2 retained voice and pedal admission

For nonstream node, retain V from fresh synthVoice and captured byte index I.
Request hwIsActive(I). If return0 AND V.word34==0, skip pedal/start/keyOff/ADSR
but still reach dirty-byte reset below. Otherwise request inpGetPedal(V)8028f920;
keep low16 return p. Request macSetPedalState(V,p>8064 decimal)80283ba4.
The emitted unsigned subtract/sign test is exact over all16-bit p. No normalized
boolean from nonzero pedal, fresh V pointer or independent voice-validity check.
Children may mutate fields; following gates load flags freshly as noted.

## Start/key-off/ADSR flags and requests

Read V.word114/118. If word118 bit20 set: store118 with20 cleared, store114
unchanged, then RELOAD118, OR10 and store. Request hwStart(I,V.byte11f)80290c48.
No returned-state check or resource/start-success classification.

Freshly read flags114/118. Only when (114&100)==0 AND (118&90)==90:
store118 with90 cleared, store114 unchanged; request hwKeyOff(I)80290c84.
Then read flags again. If114 bit200 set, request adsrRelease(V+1e8)802867d8.
Nonzero child return freshly stores118 unchanged and clears114 bit200; zero
leaves it. ADSR request is ONLY inside the admitted key-off branch, not a global
flag handler. No current-voice/flag recheck substitutes for these exact gates.

Dirty tail: read V.bytea8. Any nonzero clears bytea8 and stores word220=7fff;
zero skips. Runs even when hardware-active/macro gate skipped earlier requests,
but not when stream byte11c skipped the entire category2 body. No whole-record
clear, time/status normalization or pointer bounds guard.

## Complete frame-done and real-time tail

After the still-open master-fade/aux middle, request hwFrameDone802915bc.
That complete child is ONLY blr: no hardware/queue/IRQ/flag notification.
Then load synthRealTime8041eea0.low(word+4), high(word0). Add RAW retained D to
low using addc, add carry to high with addze. Store new low then new high.
Word arithmetic wraps at64 bits. D is not shifted/truncated to job-slot units;
no elapsed OS time, physical sample or rendering frame conversion occurs here.
No independent scalar success assignment. Concurrent pair readers may observe
partial publication absent source/IRQ guarantees; invocation timing remains open.

## Validation and remaining local/runtime work

[Manifest](../../../tools/musyx_synth_job_consumer_evidence.json) pins the two
regions, hwFrameDone, nine linked children and whole synthHandle fingerprint:
13 spans/163 anchors/10 resolved direct calls. Three queue-order/five index/five
clock vectors and all65536 pedal threshold values are derived checks, not original
execution. Full fresh ELF/listing stays `/tmp/inbox-synthhandle.elf`; no body/asset
exported. Whole-body fingerprint is not semantic closure of its remaining middle.

Remaining local synthHandle: zero-time-offset gate, master fade/pause/stop paths
and auxiliary controller callbacks. Other open work: mac/precision/zero/pedal/
ADSR/hardware child internals, queue/index/voice/flags/time providers and valid
member/null/alias/IRQ/concurrency domains, original scheduling/voice/audio traces.
Two local regions and no-op leaf are answered; full native/source/runtime OPEN.

[Complete master/pause fades and aux callback region](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md) now closes the remaining local synthHandle section. The composed local whole body is answered; child/provider/numeric/concurrency/original execution remain open.
