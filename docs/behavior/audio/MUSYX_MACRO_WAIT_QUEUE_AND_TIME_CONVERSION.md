# MusyX macro wait, queue insertion and time conversion

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdWait8027f50c/2f0hex,
TimeQueueAdd80283cac/a4hex, sndRand802902dc/1chex,
sndConvertTicks80290560/48hex, synthGetTicksPerSecond8027695c/34hex,
and complete wait opcode stubs80282cac..80282cc0/80282d18..80282d34
(exclusive ends). Addresses/offsets/masks hexadecimal; scalar values/counts
decimal. Static source clock arithmetic, not measured original waits/audio units.
Read [scheduler/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[active/inactive lists](MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md) and
[prior complete sndConvertMs](MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md).

## Dispatch arguments and duration-zero admission

Opcode4 requests mcmdWait(V,&shared cstep8041ef08), copies its raw return to
interpreter stop marker and joins. Opcode7 FIRST stores shared cstep.byte6=1,
then makes the same request/return-to-marker. This sets the entire bits8..15
byte of command word1 to1, forcing its bit100hex without changing duration
high16 or low bit1hex. No private command copy or PC rollback; fetch already
advanced the voice PC by8. Stop return0 continues;1 stops this invocation.

mcmdWait(V,E) captures D=E.word4.high16 in a stack word. D==0 returns0 BEFORE
flag reads/writes, randomization/conversion/deadline or list work. Other values
enter staged fresh-command tests below. Each stage rereads command words;
no immutable two-word snapshot/voice identity/null/active/IRQ/count gate.

## First flag stage: command word0 bit100hex

If fresh E.word0&100hex is set, read V.word114 then118. If low118&8hex!=0
AND high114&100hex==0, return0 immediately with no writes in this stage.
Otherwise, if both those voice bits are set, write captured low118 unchanged,
then high114 OR400hex. If low bit8 clear, skip those two stores. Then reload
fresh low118, OR4hex and store.

If command bit100hex clear: read low118 then high114, store low with4hex cleared,
then high unchanged. This stage does not install a wait deadline or reset other
flags. Captured/fresh reads differ between these branches; no event interpretation
beyond the exact source gates is assigned here.

## Second flag stage: command word0 bit1000000hex

If fresh E.word0&1000000hex is set, read high114 then low118. Low20hex set
bypasses hardware admission; otherwise request hwIsActive(V.wordf4.low8).
Returned0 returns0, preserving any FIRST-stage writes. Returned nonzero, or
initial low20 set, reloads fresh low118, OR40000hex and stores it.

If the command bit is clear, read low118 then high114; store low with40000hex
cleared, then high unchanged. No post-hardware flag20 or full-ID recheck.
A return0 therefore does NOT universally mean no source state change.

## Randomization, indefinite sentinel and conversion helpers

Read fresh E.word0&10000hex. If set request sndRand, retain its low16 and replace
D by unsigned remainder random.low16 % original D. Original D is nonzero at
this point; result may be0. No RNG request on unflagged paths.

sndRand reads seed word8041e3f0, multiplies by a8351d63hex keeping low32,
stores the new seed, returns (newSeed>>6)&ffffhex. No additive term, seed repair,
reseeding or measured distribution. Zero seed remains0. Actual seed writers/
state at original execution remain open.

POST-random D==65535 writes deadline LOW V.word9c=ffffffff then HIGH98=ffffffff
and skips conversion/clock-base/due testing. Randomized original65535 cannot
retain that sentinel (remainder is at most65534). The indefinite pair is not
inserted by this body's TimeQueueAdd call, but still takes the nonzero wait
path below. Existing list/state invariants remain separate.

For other D, capture MS=bool(fresh E.word4&100hex). MS set requests
sndConvertMs(&D), whose complete body shifts the pointed word left8, wraps32
and stores. MS clear requests sndConvertTicks(&D,V). Neither name proves
physical units or original execution cadence.

Complete sndConvertTicks first requests synthGetTicksPerSecond(V), retains its
returned raw rate R, then reads pointed D. Exact operations:

`N=wrap32(D<<16); Q=unsignedDivide(N,R); D=(wrap32(Q*1000)) logical>>5`.

Division occurs BEFORE multiplication; sum/product widening or one combined
rational conversion changes truncation/overflow. No rate0/pointer/alignment/
range guard; no valid divide result is established for R=0. Positive input may
convert to0. The helper stores its output word and assigns no independent status.

synthGetTicksPerSecond reads set byte V+122:255 selects bank8, otherwise use
that raw byte. Read index byte V+123. Return fresh word at
synthTicksPerSecond803f8300+bank*40hex+index*4. Original table has nine banks/
16 words per bank; the getter does not enforce those extents or reject zero
rate. Caller V, bank/index and rate providers remain required source domains.

## Exact deadline bases and due handling

After conversion, use retained MS and fresh E.word4 bit1hex to choose:

| MS | Bit1hex | Deadline source before32-bit converted D |
| --- | --- | --- |
| Set | Set | V timestamp HIGH90/LOW94 plus D |
| Set | Clear | Fresh macro clock HIGH8041ef10/LOW8041ef14 plus D |
| Clear | Set | Absolute (HIGH0, LOW D) |
| Clear | Clear | V last-time HIGHa0/LOWa4 plus D |

For added pairs, load base LOW, converted D, then base HIGH; add low with carry,
store deadline LOW9c then HIGH98. No atomic64-bit snapshot, units normalization,
base-generation check or saturated carry. Absolute stores LOW then HIGH0.
The base-selection bit is reread AFTER conversion; retained MS is not recomputed.

Read deadline LOW, current macro-clock LOW, deadline HIGH, current HIGH;
compare unsigned64. Deadline<=current stores captured deadline LOW at V.a4,
HIGH at V.a0, then clears deadline LOW9c/HIGH98 to0. The last-time becomes the
DEADLINE, not current clock. A future deadline remains as written. No additional
random sampling, maximum elapsed clamp or PC rewind.

Finally read deadline HIGH then LOW. Both0 returns0. A nonzero pair other than
ffffffff/ffffffff requests TimeQueueAdd(V), then macMakeInactive(V,1), returns1.
The all-ones pair skips queue insertion but still requests macMakeInactive(V,1)
and returns1. Child results ignored. An already-due/converted-zero path may
have flag/deadline/last-time writes even though its final result0 continues.
No parent state/member/identity/duplicate-queue check or rollback.

## Complete ascending queue insertion and equal-deadline order

TimeQueueAdd(V) loads macTimeQueueRoot8041ef18. Traverse next44 while current
entry deadline is STRICTLY LESS than fresh V deadline, comparing unsigned64
with separate low/high reads. Stop at first entry>=new deadline or null.
Equal deadlines therefore insert the new node BEFORE existing equal nodes.
No queue-sort validation, duplicate/member/cycle/deadline0/all-ones guard or
count bound. New deadline is reread on each comparison.

Empty queue: set root=V, V.next44=0, V.previous48=0. Append: set captured
last.next44=V, V.previous48=last, V.next44=0. Insert before entry E:
store V.next44=E; read E.previous48 and store V.previous48. If nonnull reread
E.previous48 and store that predecessor.next44=V, else root=V. Finally
E.previous48=V. No link cleanup, active-list unlink/state write or resource child.
Alias/concurrency can change the fresh predecessor reread; do not replace it
with the scan's retained predecessor. Actual active transition is the later
macMakeInactive child, governed by its separate contract.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_wait_queue_and_time_conversion_evidence.json)
pins five complete bodies,two complete dispatch stubs,three prior child
fingerprints and dispatch table:11 spans,185 instruction anchors,nine calls.
Six RNG/six conversion/five rate-address/four base-mode/five deadline/five equal-
queue/five random-remainder vectors are source deductions, not execution or
physical time-unit measurements. Full fresh ELFs/listings stay
`/tmp/inbox-mcmdWait.elf`, `/tmp/inbox-TimeQueueAdd.elf`, `/tmp/inbox-sndRand.elf`,
`/tmp/inbox-sndConvertTicks.elf`, `/tmp/inbox-synthGetTicksPerSecond.elf` and the
prior macHandleActive inspection files. No original bodies/binary assets
committed or original code executed.

Remaining: actual wait commands/flags/callers, RNG seed/rate/index/clock/timestamp/
state/list writers, valid nonzero-rate/pointer/member/sorted/cycle/alias/IRQ/
concurrency/indefinite-wait domains and original wait/random/queue/event/audio
traces. Five local bodies/two stubs are answered; remaining interpreter opcodes,
EVT-01 and full native/source/runtime/hardware equivalence remain OPEN.
