# MusyX MIDI controller setters and notification

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete inpSetMidiCtrl8028e618/51c,
inpSetMidiCtrl14 8028eb34/124 and synthKeyStateUpdate80278810/28hex.
Addresses/offsets/masks hexadecimal; scalar values decimal. Static writes/job
requests, not original controller timing/audio. Read
[getter/default pointer](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md),
[reset/default initialization](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md),
[external setter](MUSYX_MACRO_VARIABLE_CALCULATION_COMPARE_AND_SET.md) and
[job scheduler](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md).

## Inputs, gate and exact address domains

inpSetMidiCtrl receives raw (I,C,S,V): controller, channel, set, value. Retain
these across special child calls. Channel.low8==255 returns before all work.
Otherwise c=C.low8,s=S.low8,i=I.low8,byteValue=V.low7. No set/count/controller/
active/IRQ bounds check or success assignment. Set255 is FX, not rejection.

| Domain | Address |
| --- | --- |
| MIDI byte row | 80409748+s*860hex+c*86hex |
| FX byte row | 8040dec8+c*86hex |
| Packed9-byte channel defaults | 8040da48+s*90hex+c*9 |
| Aux dirty word | 80409548+s*40hex+c*4 |
| Voice pool/count | synthVoice8041ee90 / synthInfo803f8540.byte210 |

These unchecked low-byte arguments can exceed the original8-set/16-channel/
64-FX-channel/134-controller shapes. A table address calculation does not prove
that the caller permits it. Byte stores use i even beyond133.

## Special controllers before the byte store

Both MIDI and FX branches select the same four special controllers by i:

- 6 requests inpSetRPNHi(RAW S,RAW C,RAW V), then force changed marker1.
- 96 requests inpSetRPNDec(RAW S,RAW C), then force marker1.
- 97 requests inpSetRPNInc(RAW S,RAW C), then force marker1.
- 38 handles the packed selector/default propagation below, then force marker1
  even when selector unrecognized or no voice matched.

These child returns are ignored. The source's96/97 request names/order are
retained, without importing an assumed MIDI convention. RPN child semantics
remain open. Special child work precedes final controller-byte publication;
no compare-old-byte admission for these four cases.

For i38, read byte100 then byte101 from MIDI row80409748+s*860+c*86, EVEN in
FX branch s255. Selector=byte100|(byte101<<8); no 7-bit mask. Selector7f7dhex
updates packed defaults.word1,7f7e updates word5; other selectors skip updates.
New word=(oldWord&0000fe00) | (U8(RAW V)<<2). This truncates most old bits and
can overlap preserved bit9; it is not a whole 7-bit byte replacement.
Store default word first. Scan ascending voices under fresh count before each
iteration, reload synthVoice each time, stride458. Matching byte122==s AND
byte121==c receives retained new word at voice.word1e0 or1e4. No macro-active/
identity/running-kind check, pointer snapshot,64-voice cap or dirty/job request
in this preliminary scan. In FX branch these selector/default addresses still
use set255 in MIDI/default formulas; no substitution of FX defaults. Actual
reachability/extent/preconditions remain open.

## Ordinary compare, unconditional store and MIDI notification

Other controllers read old byte from chosen row. Changed marker is old!=V.low7
(the source ORs two wrap32 differences then extracts sign). In ALL admitted
cases, special or ordinary, store V.low7 to chosen row before testing marker.
Unchanged ordinary writes perform the store but skip all notification.

Changed MIDI branch (s!=255) scans ascending ordinals under fresh count at every
loop test. Each iteration reloads synthVoice, checks byte122==s then byte121==c.
For a match, store voice.word220=32767, reload synthVoice AGAIN for that ordinal,
request synthKeyStateUpdate(record). Child return ignored; no admission on voice
activity/identity/protection. Later counts/pointers/fields may reflect child
mutation. After entire scan, store FULL word000000ff at auxDirty[s,c]. This is
replacement, not OR with old bits and not255 per individual byte. Aux dirty
publication occurs even with count0/no matching voices, but only changed marker1.

Changed FX branch similarly scans with matching byte122==255 and byte121==c,
stores word220=32767, reloads pool and requests key-state update. It does NOT
write the aux dirty table. The set255 branch does not reinterpret channel as a
voice ordinal for this scan; it matches the two voice fields explicitly.
No hardware/controller completion, rollback or coalescing guaranteed here.

## Complete key-state job wrapper

synthKeyStateUpdate(V) requests synthAddJob(V,2,0)80278618; no gate/field write/
time conversion or child-result handling. Job queue semantics belong to
[the existing contract](MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md). A request is
not proof the update ran before another controller store or audio output.

## Complete14-bit request wrapper

inpSetMidiCtrl14(I,C,S,V) rejects C.low8==255 first. Otherwise hi=(V>>7).low8,
lo=V.low7. No input clamp to16383; the hi request can contain bit7, later masked
by the byte setter. Select using i=I.low8:

- i<64: controller j=i.low5 with hi, then j+32 with lo.
- i128/129 or132/133: even controller j=i&fe with hi, then j+1 with lo.
- Other i: original RAW I with hi only.

Each request is inpSetMidiCtrl(index,RAW C,RAW S,value). Pair retains raw C/S/
original low16 value across first child; first and second request can each
trigger their own preliminary RPN/default writes and notification scans. There
is no combined atomic row update, remap table, returned-status gate, bound or
single dirty publication. This wrapper's lack of clamp differs from inpSetExCtrl,
which clamps S16 input before issuing its pair requests.

## Evidence and remaining bindings

[Manifest](../../../tools/musyx_midi_controller_setters_evidence.json) pins three
complete bodies/six linked RPN/job/ABI identities: nine spans,213 instruction
anchors/18 calls. Eight store/notification/six
parameter-mask/six14-bit-request/four address vectors are mathematical deductions,
not original execution. Full fresh ELFs/listings stay `/tmp`; no original bodies/
binary assets committed. Source identity does not prove valid extent/ownership.

Remaining: complete RPNHi/Dec/Inc, actual raw caller/controller/selector/default/
voice/count/dirty/job writers and consumers, valid table/voice/matching/alias/
IRQ/concurrency/reinit domains and original pair/publication/pedal/audio traces.
EVT-01 remains OPEN.

### 2026-10-13 — RPN child follow-up

[Complete RPN writers](MUSYX_RPN_PARAMETER_WRITERS_AND_VOICE_PROPAGATION.md)
now close the Hi/Dec/Inc children: exact selector/mask/byte/whole-word guards and
retained versus fresh default propagation. Parent store/forced notification order
remains as above. Actual caller/table/value/alias domains and original audio open.
