# MusyX stream kill/init and hardware break/active flags

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete streamKill8027af80/98,
streamInit8027a544/c8, hwBreak802909c8/50 and hwIsActive802907d8/20hex.
Read [voice-kill caller](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md),
[bootstrap](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md) and
[frame time offsets](MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md).
Offsets/addresses hexadecimal; explicit decimal counts/strides labeled.
Static record/request behavior only; actual stream/DSP/audio execution open.

## Complete stream kill: first match and callback

Input RAW internal voice ordinal I. Scan EXACTLY64 streamInfo records starting
803f98e8, stride68hex (104 decimal), total1a00hex. No sndActive gate, IRQ,
configured voice-count restriction, pointer/list/null check or initial sentinel
rejection. At each record R read state bytec: only1/2 admit comparison of R.word4c
to raw I. Other state or unequal ordinal advances to the next record.

FIRST match ends all search. If captured state2, request voiceUnblock(R.word4c)
8028578c; its result is ignored. Then store state bytec=3, load FRESH callback
R.word10 and user word50, invoke callback(0,0,0,0,userWord) with r3..r7.
No callback-null guard, parameter snapshot before state publication, return check,
record clear/free, ARAM release, voice-ID rewrite or later matching-record removal.
State1 skips Unblock but still publishes3 and invokes callback. No independent
success result; match returns after callback, full miss returns without request.
Actual callback-provider/Unblock behavior and stream-state meanings remain open.

## Complete stream initialization and live-tail count

Read U8 synthInfo803f8540.byte210 as initial count N. Store streamCallCnt8041eeb5=0,
then streamCallDelay8041eeb4=3. N0 skips all stream state stores. Otherwise:

- Initial N>8: clear state bytec in ascending eight-record blocks, number of
  bulk records8*floor((N-1)/8). Each block writes its eight bytes then advances.
- At tail, RELOAD synthInfo.byte210 as M; starting at current ordinal J, clear
  state bytec for ordinals J..M-1 if J<M. The source does not recapture/reset J.
- Initial1..8 goes directly to the fresh tail-count read; no bulk stores.

Stable admitted N clears exactly N records. No count<=64 check or IRQ; changing
count between reads can shorten/extend tail writes. Initial N>64 can exceed the
supplied64-record array. Source bootstrap caps voice byte64, but later writers/
concurrency and valid reinit invariants remain separate.

Finally store nextPublicID8041eea8=0, streamDefaults8041eeac.byte0=0,
streamDefaults.word4=0, in order. Bytes1..3 are untouched. No other stream fields,
callback/voice/ARAM pointers or whole-array clear. No existing stream callback/
Unblock/voice stop or resource release. Unlike initialization's prefix, streamKill
always scans64; stale state above a later smaller configured count can remain.
No independent status assignment or active-system admission check.

## Complete hardware break record publication

Input RAW voice index I. Compute wrap32(I*f8hex), read dspVoice8041ef88 and form
captured record V. Read V.bytef0. If EXACT1, read salTimeOffset8041efb8; if that
byte0, store V.bytef2=1. Other bytef0 values or nonzero offset skip this store.
No flag clear/reversal in the skipped branches.

Then RELOAD dspVoice and salTimeOffset; form record from the original index
offset plus4*U8(currentTimeOffset), read word24 and store it OR20hex.
The event-word record can differ from the earlier captured V if globals change.
No active-system gate, index/count/buffer bound check, queue/mail/IRQ or actual
hardware request, voice/resource free or completion wait. It publishes an event
bit even if initial active byte0. Caller source must establish time-offset domain;
ordinary frame handler supplies0..4, but this leaf does not clamp other bytes.

## Complete hardware active-record query

hwIsActive(I) computes the same raw ordinal offset, reads current dspVoice,
loads record.bytef0 and returns0 for zero,1 for ANY nonzero byte. No hardware
playback read, IRQ, sample/voice generation/map query or mutation. This differs
from Break's f2 condition, which requires active byte EXACT1. Query truth and
local break flags do not prove audible playback or completed DSP shutdown.

## Verification and remaining source/runtime

[Manifest](../../../tools/musyx_stream_kill_hw_break_evidence.json) pins four
complete bodies and linked Unblock identity:5 spans/78 anchors/one direct call.
Derived10 initialization counts,
10 kill-state/match cases and16 break active/offset cases are source deductions,
not original execution. Full fresh ELFs/listings stay `/tmp/inbox-stream*.elf`
and `/tmp/inbox-hwbreak.elf`; `/tmp/inbox-hwactive.elf`.

Remaining: voiceUnblock/stream callback constructors and state/count/voice/DSP
providers, valid index/offset/reinit/concurrency domains, streamHandle/command
consumer/actual DSP/hardware behavior and original kill/init/audio traces. Four
complete local bodies are answered; full native/source/runtime remains OPEN.
