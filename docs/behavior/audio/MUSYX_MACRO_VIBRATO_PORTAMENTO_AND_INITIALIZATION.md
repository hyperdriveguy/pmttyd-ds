# MusyX macro vibrato, portamento and initialization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdVibrato8027fe84/17c,
mcmdPortamento80282048/164, synthInitPortamento80276990/78hex and two opcode
regions below. Addresses/offsets/masks hexadecimal; literals decimal. Static
conversion/byte/flag/controller/init operations, not original modulation/pitch/
audio timing. Read [time conversion](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[MIDI setter](MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md),
[controller getter](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md)
and [fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md).

## Conversion inputs and live command reads

Both handlers retain voice V/command C. Duration input is U16(C.word4.high16),
stored in a local stack word. Then REREAD word4: bit100hex set requests
sndConvertMs(&duration), otherwise sndConvertTicks(&duration,V). Converted word
is read after the child. These already-complete children preserve wrap/truncation/
rate-zero domains; their names do not establish physical cadence. No snapshot
of entire command, duration positivity/range/IRQ/sndActive/null validation.

## Complete vibrato type and zero-duration branches

First capture C.word0; type=(word>>24).low2. Type!=0 reads lowFlags118, stores
OR4000hex (high114 untouched). Type0 reads low then high flags, stores low with
4000 cleared then high unchanged (source ANDffffffff). These happen BEFORE
conversion, including a converted-zero duration.

Converted D0: read low then high flags, store low with2000hex cleared then high
unchanged. Prior fields144/145/148/14c are NOT reset. Nonzero D: read low flag,
store OR2000, then D to word148. Reread C.word0 for signed bytes A=S8(bits8..15),
B=S8(bits16..23). Exact branches/write order:

| Condition | Ordered parameter stores |
| --- | --- |
| A<0 | byte145=abs(B); byte144=-A; word14c=fresh word148>>1 |
| A==0,B<0 | byte145=-B; word14c=fresh word148>>1; byte144=0 |
| A>0,B<0 | byte145=(100-B).low8; word14c=0; byte144=A-1 |
| A>=0,B>=0 | byte145=B; word14c=0; byte144=A |

No amplitude clamp/float absolute or signed shift of duration. abs(-128)=128
fits the byte; B-128 in positive-A branch gives228, not100. Fresh word148 reload
for half-phase can differ from captured D under unsupported alias/concurrency.
No immediate pitch/hardware/controller/job/wait request after these stores.

## Complete portamento prefix and mode0

First read C.word0 bits16..23, store byte133 RAW (not boolean). Perform conversion
above; store converted duration to word138 even if zero. Then reread C.word0,
mode=U8(word>>8). Other than modes0/1/2, return after prefix without flag/controller
writes. Unsupported mode does not reject or undo prefix stores.

Mode0: read channel121. If not255, read set122 and request
inpSetMidiCtrl(65,channel,set,0). Ignore return. Then read low flags followed by
high flags; store low with400hex cleared, high unchanged. Fresh post-controller
flags are used. No duration/base/word140 reset or initializer call.

## Mode1 force-enable and mode2 controller query

Mode1: admitted channel!=255 requests inpSetMidiCtrl(65,channel,set,127), ignored.
Regardless of channel, freshly read high then low flags; only low400 contributes.
If clear, request synthInitPortamento(V); otherwise skip. Then read low fresh,
store OR400 even if initializer returned without writes. High flag does not
contribute to that initialization admission (source AND0).

Mode2: channel255 returns after prefix. Otherwise request
inpGetMidiCtrl(65,channel,set), take low16; STRICTLY>8064 enters the same flag/
initializer/OR400 suffix as mode1, WITHOUT controller write. <=8064 leaves
flags unchanged, including already-set400. No mode2 automatic disable/zero,
controller ownership/threshold cadence or initialization success gate inferred.

## Complete portamento initialization precondition and stores

synthInitPortamento reads high then low flags; only low20000hex contributes to
early return. Set means NO stores. Otherwise read byte133. If exactly1 and
captured low1000hex clear, store word140=0. All other cases store fresh word138
to word140. Then read byte132, store unsigned byte<<16 to word13c. No duration
conversion, activation/controller/hardware/job request, flag write, base-pointer
snapshot or note127 clamp. This body can skip while caller still publishes400.

## Registration, evidence and remaining source/runtime

| Opcode | Complete region (end exclusive) | Request |
| --- | --- | --- |
| 27 | 80283040..3050 | mcmdPortamento(V,&shared cstep8041ef08) |
| 28 | 80283050..3060 | mcmdVibrato(V,&shared cstep) |

Stubs ignore returns and join with local stop marker unchanged (initial0).
Fetch already advanced PC; no wait/stop or render-frame timing added. Only these
regions, not whole macHandleActive, are closed by this delivery.

[Manifest](../../../tools/musyx_macro_vibrato_portamento_evidence.json) pins three
bodies/two regions/four linked providers/dispatch table: ten spans,151 anchors/
ten calls/two registrations. Eight vibrato/seven portamento/
five initializer vectors are deductions under stable child inputs, not original
execution. Full fresh inspection ELFs/listings stay `/tmp`; no original bodies/
binary assets committed.

Remaining: actual command/converted duration/rate/amplitude/byte133/base132/
flags/controller/caller producers and modulation/pitch/job consumers, valid
shared-cell/alias/IRQ/concurrency/initialization domains and original vibrato/
portamento/controller/pitch/audio traces; other opcodes/full native binding.
EVT-01 remains OPEN.
