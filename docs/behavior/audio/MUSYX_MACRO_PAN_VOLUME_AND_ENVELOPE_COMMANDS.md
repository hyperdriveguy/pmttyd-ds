# MusyX macro panning, volume scaling and envelope commands

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdSetPanning802807a0/9c,
mcmdSetSurroundPanning8028083c/9c, mcmdScaleVolume802808d8/134,
mcmdEnvelope80280a0c/130, mcmdFadeIn80280b3c/13c, sndConvert2Ms802905a8/8,
dataGetCurve8027f238/5c and curvecmp8027f228/10hex; five opcode stubs below.
Addresses/offsets/masks hexadecimal; literals decimal. Static command/field/
arithmetic order, not original envelope/panning/audio timing. Read
[conversions](MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md),
[binary search](MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md),
[volume publication](MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md) and
[fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md).

## Common input/domain boundary

Retain voice V and command pointer C. Bn(W) means unsigned byte at bits8*n..8*n+7.
No pointer/activity/IRQ/code/field/range check or full command snapshot. Reads
listed fresh can observe child/alias/shared-cell mutation. Flags H=word114,
L=word118. All integer arithmetic wraps32 unless a narrower store/clamp is stated;
signed division truncates toward0. These bodies request neither hardware mixing
nor a synth job; consumers and measured cadence remain separate requirements.

## Complete pan/surround fields and raw-duration division

| Role | Pan | Surround |
| --- | --- | --- |
| Duration word | 18c | 190 |
| Start word | 174 | 178 |
| Target word | 184 | 188 |
| Increment word | 17c | 180 |

Read C.word0.high16 as retained raw duration D; store it in the selected duration
field, request sndConvertMs(&that FIELD). The child shifts it left8/wrap32.
Then reread C.word0 AND C.word4: start=B1(W0)<<16, store start. Delta=S8(W1.low8)
<<16. Read freshly stored start and store target=start+delta wrap32. No target
0..127 clamp or current-pan capture. Read duration field fresh: if nonzero,
store SIGNED delta/D using RETAINED UNSCALED D; otherwise store delta unchanged.
This is not delta divided by converted D<<8. Unsupported mutation could make
fresh duration nonzero with retained D0; no independent divisor guard is added.
Finally read H and store OR2000hex. No L write, endpoint normalization, explicit
wait/stop or success result. Each child request/store sequence preserves order.

## Complete curve lookup and interpolation

dataGetCurve(raw ID) stores low16 into key.half4 at8041eed0 (key base8041eecc).
Capture registry count half8041eef8; request sndBSearch(&key,803fc6e8,count,8,
curvecmp). Comparator U16(key.half4)-U16(row.half4), signed32. Store search row
pointer to global8041eec8. Nonnull returns row.word0 raw curve pointer; null
returns0. A row with null payload also yields0. No count refresh/reference
admission, extent/sorting/ID/generation check or allocation/acquisition.

The three volume commands below form curve ID=B3(W0)|(B0(W1)<<8), using their
specified fresh captures. Exact65535 bypasses lookup. Lookup0 leaves retained
candidate volume M unchanged. Nonzero curve: i=M.high16,f=M.low16. i<127:
read byte a=curve[i], then b=curve[i+1]; M=wrap32((a<<16)+f*(b-a)). i>=127:
M=curve[i]<<16. No curve length/value127 validation or post-curve saturation.
A byte255 can therefore yield ff0000, above the preceding7f0000 clamp.
No float lerp or division of the interpolation product by65536 is performed;
it already carries the candidate's fractional scale.

## Complete scale-volume:127 divisor, temporary stores and high flag

Capture C.word4 then C.word0. Factor=B1(W0); B1(captured W1)==0 selects fresh
V.word158 as source, otherwise fresh word15c. Product=wrap32(source*factor),
unsigned floor(product/127) via high-multiply02040811/subtract/shift sequence;
store quotient to V.word158.

Reread W0: add B2(W0)<<16 to fresh word158/store wrap32. Reread word158;
unsigned>7f0000 caps/stores7f0000. Then reread W0/W1, capture fresh word158 as
M, form curve ID and apply lookup/interpolation above. Final V.word158=M,
then freshly read H/store OR1000hex. Multiple temporary volume writes precede
lookup; the final retained M can overwrite a child's later volume mutation.
No envelope flag clear, word198/19c/1a0 rewrite or separate enable decision.

## Envelope/fade duration conversion and different128 scaling

Envelope captures ORIGINAL old V.word158 BEFORE duration conversion; fade does
not retain an old volume. Capture C.word4.high16 into stack D, reread word4:
bit100hex set requests sndConvertMs(&D), otherwise sndConvertTicks(&D,V).
Read converted D, request sndConvert2Ms(D). That COMPLETE leaf returns unsigned
D>>8. If returned divisor E0, replace locally with1. No stored duration field
or physical elapsed-time/tick measurement is established here.

Then capture W0 and FRESH V.word158. Candidate target T=
wrap32((wrap32(freshVolume*B1(W0)) logical>>7)+(B2(W0)<<16)); unsigned>7f0000
caps T. This128 divisor differs from scale-volume's127. Reread W1, form curve
ID using captured W0 high byte/fresh W1 low byte and apply curve to retained T.
Lookup can change other live fields; target remains the retained value unless
curve bytes themselves change. No clamp after curve, float ratio or dirty job.

## Exact signed step and publication order

Envelope: compute wrap32(T-ORIGINAL old), store target T at word19c; signed divide
that wrapped difference by E into step register. Store ORIGINAL old at word1a0,
store step at198, restore ORIGINAL old to158. Finally read L/store OR8000hex.
Thus target calculation uses fresh post-conversion volume while start/step use
captured pre-conversion old; they are not one volume snapshot.

Fade: compute signed T/E into step register; store T at19c,0 at1a0, step at198,
0 at158, then fresh L OR8000. It does not use captured old as the starting value.
Even E promoted1 or target0 still performs these stores/flag publication. High
flags are untouched in both suffixes. No step saturation, whole-envelope reset,
callback or immediate audible gain guarantee. Actual curve/value domains and
original update consumers remain open.

## Registration, evidence and remaining source/runtime

| Opcode | Complete region (end exclusive) | Child |
| --- | --- | --- |
| 13 | 80282e70..e80 | ScaleVolume |
| 14 | 80282e80..e90 | SetPanning |
| 15 | 80282e90..ea0 | Envelope |
| 20 | 80282f18..f28 | FadeIn |
| 21 | 80282f28..f38 | SetSurroundPanning |

Each stub passes V,&shared cstep8041ef08, ignores returned register and joins
with stop marker unchanged (initial0). No caller PC rewind/wait or rendering-
frame timing is added. Whole interpreter and remaining opcodes remain open.

[Manifest](../../../tools/musyx_macro_pan_volume_envelopes_evidence.json) pins
eight bodies/five regions/five linked identities/dispatch table:19 spans,210
anchors/19 calls/five registrations. Five pan/six scale/four target/four curve/
four step vectors are deductions, not original execution. Full fresh ELFs/
listings stay `/tmp`; no original bodies/binary assets committed.

Remaining: actual duration/rate/command/pan/volume/curve/registry/flag/caller
writers, valid raw-versus-converted divisor/value/curve/alias/shared-cell/IRQ/
concurrency domains and envelope/mix/job/DSP consumers; original pan/fade/volume/
audio traces, other opcodes/full native binding. EVT-01 remains OPEN.
