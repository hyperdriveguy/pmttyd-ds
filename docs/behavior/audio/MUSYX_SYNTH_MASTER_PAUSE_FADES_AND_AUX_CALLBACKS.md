# MusyX synthesis master/pause fades and aux callbacks

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthHandle region
80278abc..80278d14 exclusive. Together with the complete
[job/voice/clock regions](MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md),
this closes the local whole synthHandle80278838/510hex request graph.
Read [sequence stop](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md).
Offsets/addresses hexadecimal; counts/indices explicitly decimal.
Static original instructions, not measured fade/audio timing.

## Zero-time-offset admission and two bitmaps

After queue drain/index advance, request hwGetTimeOffset. Returned low8 NONZERO
skips this entire section, proceeding directly to frame-done/time tail. Zero
admits it. This does not repeat the earlier synthInfo.word4 body gate.

Master records synthMasterFader803f8754 occupy600hex bytes:32 rows,30hex bytes
per row. Active bitmap=word8041ee88, pause bitmap=word8041ee84. Read active
then pause; both0 skip directly to aux callbacks. Otherwise scan row0..31,
retaining a shifting word bit starting1, with fresh per-row bitmap reads.
No active-system/IRQ, finite/range/null or elapsed-time parameter check here.
Raw input D is not used to scale these updates; it reaches the later clock tail.

## Master fade updates and completion requests

For row R whose bit is set in FRESH active bitmap:
read floats R+4 (A), +8 (B), +c (Q), in that order. Separate single operations:

`R.float0 = fsubs(A, fmuls(Q, fsubs(A,B)))`.

Then RELOAD float+c and float+10; newQ=fsubs(oldQ,R.float10). Store newQ at+c.
Ordered newQ<=0 completes; positive/unordered proceeds without snapping/clearing.
No host FMA substitute, epsilon, NaN repair or clamping of arbitrary coefficients.
On completion, reload float4 and store it at0. Read U8 R.byte2c:

| Exact completion mode | Request |
| --- | --- |
| 1 | seqStop(fresh R.word28) |
| 2 | seqPause(fresh R.word28)80273488 |
| 3 | seqMute(fresh R.word28,0,0)80273c8c |
| 0/4..255 | No sequence request |

Child returns ignored. AFTER the request, reload active bitmap, clear this bit,
store it. If result0, read pause bitmap fresh; also0 jumps to aux callbacks before
this row's pause work. Otherwise continue to pause gate. Sequence child may
mutate bitmap/record/source; do not use a pre-request snapshot for final clear.
Unknown mode still completes/snaps/clears its bit; no refusal/rollback added.

## Pause fade update and captured-bitmap clear

Read pause bitmap into retained word P; if its bit is set, load floats R+18(A),
+1c(B), +20(Q). Store R.float14=fsubs(A,fmuls(Q,fsubs(A,B))). Reload +20/+24,
compute newQ=fsubs(oldQ,R.float24); store at+20. Ordered newQ<=0 completes:
reload +18, store at+14; clear current bit from CAPTURED P and store pause bitmap.
No sequence request or live bitmap reread before this clear. If cleared word0,
read active bitmap fresh;0 jumps to aux callbacks. Otherwise continue scan.

After a surviving row, increment ordinal, shift bit left1, advance R by30hex;
stop at32 rows. A cleared active row can still process pause, and early exit
requires both bitmaps empty. No whole-record reset/free, per-physical-frame
count or original fade duration is inferred from one admitted coefficient step.

## Complete eight-studio A then B control callbacks

Always after admitted fade processing, visit EXACTLY8 studios ascending; no
salMaxStudioNum gate. For each studio i, process A then B using independent tables:

| Field | A base | B base |
| --- | --- | --- |
| MIDI association byte | 8041ee7c | 8041ee6c |
| MIDI set byte | 8041ee74 | 8041ee64 |
| User word | 803f8f14 | 803f8f54 |
| Callback word | 803f8f34 | 803f8f74 |

Association byte==ff skips that side. Otherwise for parameter j0..3, reread BOTH
association/set bytes, request inpGetAuxA/B(U8(i),U8(j),association,set) at
8028fb60/8028fc14, store low16 returned word into consecutive stack halfwords.
There is no recheck of associationff during this admitted four-parameter loop;
mutations can change the subsequent raw request inputs.

ONLY AFTER four requests load the fresh callback word and user word, invoke
callback(1,&fourStackHalfwords,user). No callback-null/generation check, result
admission, IRQ/queue drain or parameter normalization beyond halfword stores.
A uses one stack array, B another. B admission/arguments are read AFTER A's
callback, so same-studio A can alter B/table fields before their reads. Current
studio loop count and table pointers remain retained. Advance all byte/word table
positions independently; no callback/provider audio semantics inferred locally.

This callback tag1/control-halfword path is distinct from tag0/three channel
buffers in [audio aux processing](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
Do not merge their payloads, cache requests, cadence or control contracts.

## Source validation and local whole-body boundary

[Manifest](../../../tools/musyx_synth_fades_aux_callbacks_evidence.json) pins the
new region, six linked children, whole synthHandle fingerprint and original zero
literal:9 spans/150 anchors/six resolved calls. Six completion-mode/five bitmap
vectors and16 ordered studio/side callback rows are derived checks, not original
execution. Full listing/ELF reuses `/tmp/inbox-synthhandle.elf`, source reverified.
No body/asset exported to history.

With the prior complete job/voice/clock regions, the local whole synthHandle body
is now answered. Remaining: fade/bitmap/mode/coefficient providers and numeric/
alias/IRQ/concurrency invariants, seqPause/Mute and precision/mac/ADSR/aux child
semantics, callback installation, original scheduling/fade/voice/audio traces.
Full native/source/runtime/hardware equivalence and EVT-01 remain OPEN.
