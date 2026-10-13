# MusyX macro active/inactive lists, wait cleanup and initialization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete macMakeInactive80283e58/fc,
macMakeActive80283d50/108, macInit8028438c/68 and
synthForceLowPrecisionUpdate802787cc/44hex. Read
[voice-kill caller](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).
Offsets/addresses hexadecimal; states/job literals decimal. Static source order,
not original macro execution/audio timing.

## State and list fields

Retained voice pointer V uses word4c state; active next3c/previous40;
time-queue next44/previous48; wait pair98(high)/9c(low);
last-time paira0(high)/a4(low); flag words114/118.
Global active root8041ef1c, time-queue root8041ef18, real-time pair8041ef10/14.
No pointer/member/IRQ/active-system/generation or arbitrary-state validation.
Pairs are read/stored as separate words, not atomic64-bit snapshots.

Active unlink: read V.previous40; null sets global active root=V.next3c,
nonnull sets previous.next3c=V.next3c. Then read V.next3c fresh; nonnull writes
next.previous40=V.previous40. Own links are not cleared; neighbor aliasing may
change later fresh reads. Time unlink is analogous using44/48 and time root.
No allocation/free, hardware stop or reference release in either unlink.

## Complete inactive transition

Inputs V, RAW requested state N. Read old word4c. Exact equality returns
IMMEDIATELY, before any list/wait cleanup or state write. Otherwise old0 performs
active unlink; all other old values skip it. No general detach for unknown states.

Only N==2 admits wait cleanup. Capture words98/9c. Pair0 skips all cleanup.
Nonzero pair BOTHffffffff skips time unlink, but DOES perform remaining cleanup;
other nonzero pairs perform time unlink first. Then:

1. Store wait.low9c=0, then wait.high98=0.
2. Read realTime.high, then realTime.low; store low toa4, then high toa0.
3. Read flags118 then114; store flags118 &fffbfffb, then store114 unchanged
   (source ANDffffffff). Mask clears bit4hex and40000hex only.

Finally store RAW N to word4c, regardless of old-state/list validity. Other
requested states leave wait/time/flags unchanged. No saturation/domain repair,
job request, source reset or voice release. Local r3 remains input V unchanged,
without separate success code. VoiceKill supplies requested2, but equality with
old2 suppresses even nonzero-wait cleanup; callers must establish valid state.

## Complete activation and forced jobs

macMakeActive(V) reads old word4c; old0 returns immediately without any wait/list
mutation. Otherwise capture wait98/9c. Pair0 skips cleanup/jobs. Nonzero pair:
BOTHffffffff skips time unlink; other nonzero pairs unlink time list. Then request
synthForceLowPrecisionUpdate(V), BEFORE clearing wait or copying real time.
Ignore its return; retained V used afterward. Clear wait/copy real-time/mask flags
in exactly the same order as inactive cleanup above, with fresh post-child reads.

Prepend V to active list: capture root, store V.next3c=capturedRoot; if nonnull,
RELOAD root and store its.previous40=V. Store V.previous40=0, global root=V,
then V.state4c=0. No old active-list membership scan, general old-state detach,
wait/domain/alias guarantee or separate status result. Reset/activation does not
prove that queued jobs have run or the sound is audible.

ForceLowPrecisionUpdate retains V, requests synthAddJob(V,0,0)80278618,
then synthAddJob(V,1,0), in that order. No queue-result check, delay/time unit,
job-type interpretation, IRQ or state publication here. SynthAddJob is a linked
child, not recovered scheduler semantics or original simulation tick evidence.

## Complete macro initialization and live voice count

macInit stores active root0, time root0, realTime.low0, realTime.high0, in order.
Then ascending voice ordinals under FRESH synthInfo803f8540.byte210 before each
iteration, stride458hex. For each admitted ordinal:

- Reload synthVoice8041ee90 and store record.word34=0.
- Reload synthVoice again and store record.word4c=2.
- Reload synthVoice again and store record.unsignedHalfaa=0.

No captured count/pointer, voice-range cap, active-system/IRQ gate, whole-voice
clear, own-link/wait/flags/identity reset or old-node teardown. Count0 skips records.
Separate pointer reloads can direct these three stores to different pools under
unsupported concurrent mutation. Source bootstrap caps count64; this body does
not. No voice/resource/job release or explicit success assignment.

## Validation and remaining source/runtime

[Manifest](../../../tools/musyx_macro_active_inactive_init_evidence.json) pins four
complete bodies/one linked job body:5 spans/125 anchors/three resolved calls.
Six wait-pair/five flag-mask/seven state-route vectors and ordered job metadata
are static deductions; original functions not executed. Full fresh ELFs/listings
stay `/tmp/inbox-mac*.elf` and `/tmp/inbox-force-lowprecision.elf`.

Remaining: synthAddJob/macro interpreter/wait-time producers, state/active/time/
voice/pointer/count providers and member/alias/IRQ/concurrency domains, actual job/
macro/voice lifetime and original wait/activation/audio traces. Four complete
local bodies are answered; full native/source/runtime/hardware remains OPEN.

[Complete job insertion/start timestamps](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md) now closes the exact synthAddJob child, including distinct category duplicate rules and invalid-type precondition. Queue initialization/consumers/time/provider/original execution remain open.
