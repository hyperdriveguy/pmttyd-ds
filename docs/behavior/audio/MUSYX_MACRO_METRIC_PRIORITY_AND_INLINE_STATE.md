# MusyX macro allocation metric, priority and inline state

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete ten inline macHandleActive regions
80283428..802835e8 (exclusive end), registered by original opcodes48..57.
Addresses/offsets/masks hexadecimal; opcode/scalar literals decimal. Static
command arithmetic/store/request order, not measured age, audio or scheduling.
Read [fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[allocation selection](MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md),
[priority/hardware storage](MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md) and
[variable domains](MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md).

## Command capture and registration

V is retained interpreter voice pointer. W0 is the command word already captured
in r26 after the shared fetch; W1 reads shared cstep8041ef08.word4 where needed.
Several handlers reread shared W0 instead of reusing r26; nested/shared writers
can make them differ. PC already advanced8 before dispatch. Each listed region
branches to common join80283894 without changing local stop marker (initial0),
so normally continues to the next attempt. No independent count/PC/activity/IRQ/
command-domain check, return-based stop, PC rewind or renderer timing added.

| Opcode | Complete region | Local operation |
| --- | --- | --- |
| 48 | 80283428..494 | Signed delta to metric, clamp, hardware publication |
| 49 | 80283494..4bc | Set metric, hardware publication |
| 50 | 802834bc..4d4 | Global variable byte to full word |
| 51 | 802834d4..4ec | Two aux bytes |
| 52 | 802834ec..504 | Two tremolo bytes |
| 53 | 80283504..518 | Set low flag10000hex |
| 54 | 80283518..528 | Priority setter request |
| 55 | 80283528..568 | Signed16 sum/clamped priority setter request |
| 56 | 80283568..594 | Metric/divisor rate halfword |
| 57 | 80283594..5e8 | Scaled-byte metric, clamp, hardware publication |

Range ends exclusive; shortened end addresses retain the80283 prefix.
Only these ten regions are closed here; whole macHandleActive remains partial.

## Metric addition/set and hardware publication

Let M=unsigned V.word110 and D=S16(W0.high16).
Opcode48 computes X=(M>>15)+D using signed result comparison. The operand
ranges fit signed32. X<0 stores M=0; X>65535 stores7fff8000; otherwise stores
X<<15. Low15 prior bits are discarded, even delta0. A raw priorffffffff shifts
to131071 and therefore clamps to65535 on delta0. Do not impose a prior valid-
metric range or interpret delta as elapsed simulation/rendering frames.

Opcode49 stores U16(W0.high16)<<15 directly at V.word110 (the source rotate/
mask is equivalent). It does not clamp to60000 or preserve prior low15 bits.

After either store, and after opcode57 below, read fresh V.word110, then fresh
identityf4, then priority byte10c; request
hwSetPriority(U8(identity), (priority<<24)|((metric>>15)&00ffffff)). Return ignored;
no priority-list mutation or call to voiceSetPriority in these three handlers.
Hardware leaf publishes a memory word, not immediate DSP/audio completion.

## Scaled-byte metric and rate halfword

Opcode57 captures V.word158 first, then W1 shared; define byte B=U8(word158>>16),
scale S=U16(W1), base A=U16(captured W0.high16). Compute product=S*B, arithmetic
shift7, add A; these nonnegative operands fit signed32 so the shift equals
floor(product/128). Unsigned total>60000 selects75300000hex (=60000<<15),
otherwise total<<15. Store V.word110 then perform fresh hardware publication.
No controller query, rounding-to-nearest, negative input or float conversion.
Word158 byte semantics/producers and actual command values remain separate.

Opcode56 reads full unsigned shared W1 divisor Q. Q!=0 reads fresh metric110,
computes unsigned (metric>>8)/Q, stores low16 quotient at V.half10e. Q==0 stores
half10e=0 without reading metric. No saturation; large quotients truncate to
low16. This handler does not publish hardware priority, change metric or establish
an elapsed-time/decay unit, frequency or downstream update policy.

## Priority set and signed16 sum before clamp

Opcode54 requests voiceSetPriority(V,U8(W0>>8)); return ignored.
Opcode55 captures current priority byte10c and signed delta=S16(W0.high16),
adds them, THEN sign-extends the sum's low16. Clamp that signed16 result to0..255,
request voiceSetPriority(V,clamped.low8), return ignored.

The second sign-extension matters: old255 + delta32767 =33022, low16 interpreted
as-32514, hence requested0. It is not a wide signed sum clamped directly to255.
Both handlers inherit setter membership/unchanged-priority early-return policy;
no direct metric/free/counter write or unconditional hardware refresh here.

## Complete inline variable, aux, tremolo and flag stores

Opcode50 reads index=U8(W0>>8), value=U8(W0>>16). Store value as a full zero-
extended WORD at global variable table803f8fa4+index*4. The source permits
all256 indices locally; no table-extent/domain validation or signed-byte store.
It does not use voice-variable/local-versus-global threshold logic of varGet.

Opcode51 stores captured U8(W0>>16) to V.byte1da. Then reread shared cstep.word0,
extract its U8(word>>8), store to V.byte1db. Opcode52 stores captured U8(W0>>8)
to V.byte194; reread shared W0, extract U8(word>>16), store to V.byte195.
No clamp, evaluator/job request, dirty-matrix write, combined atomic pair or
single captured-W0 guarantee. These overwrite individual fields only.

Opcode53 reads V.lowFlags118, ORs10000hex and stores lowFlags118. HighFlags114
is untouched. No clear/toggle, key-off/hardware/job request or semantic flag
interpretation derived solely from the mask.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_macro_metric_priority_inline_evidence.json) pins
ten complete regions, two linked priority/hardware identities and the114-entry
dispatch table:13 spans,112 instruction anchors/five direct calls/ten exact
opcode registrations. Seven metric-add/six priority-add/six scaled-metric/six
rate vectors check mathematical deductions, not original execution. Fresh full
macHandleActive ELF/listing stays `/tmp`; no bodies/binary assets committed.

Remaining: actual command assets/reachability, metric/word158/rate/priority/global
variable/aux/tremolo/flag producers and consumers, valid metric/table/ID/member/
shared-cell/alias/IRQ/concurrency domains, other opcode paths and original
allocation-metric/priority/audio traces. The parent EVT-01 remains OPEN.
