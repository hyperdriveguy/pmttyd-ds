# MusyX depop accumulation, fade and studio prepass

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete HandleDepopVoice8028889c/30c and
DoDepopFade802887f0/ac; complete salBuildCommandList REGION
80288ebc..80288f90 exclusive. Whole builder80288e00/25f8 fingerprinted;
other setup/list/sort/budget/sample/command sections remain open.
Addresses/offsets/masks hexadecimal; shifts/counts/thresholds decimal.
Read [studio/voice lifecycle](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md),
[message/deactivation leaves](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md) and
[volume-ramp region](MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md).
Static local request/arithmetic order, not measured pop suppression or audio.

## Complete accumulation prefix and nine gate bindings

HandleDepopVoice(S,V) first stores V.bytef1=0. Read V.word0 as PB1, store
PB1.half0e=0. RELOAD V.word0 as retained PB; subsequent operations use that
retained pointer, which can differ under unsupported overlap/shared mutation.
No active/studio/latch admission check inside this child.

Process rows in exactly this order. Every row freshly reads unsigned PB.half0c
and tests its own gate. Clear gate skips source/accumulator writes for that row.

| PB gate | Signed PB halfword | S accumulator word |
| --- | --- | --- |
| 1 | 52 | 04 |
| 2 | 58 | 08 |
| 4 | 5e | 0c |
| 10 | 54 | 10 |
| 20 | 5a | 14 |
| 80 | 60 | 18 |
| 200 | 56 | 1c |
| 400 | 5c | 20 |
| 1000 | 62 | 24 |

On admitted row: read unsigned PB half, sign-extend16; read accumulator,
add with wrap32; STORE wrapped sum to S.word, then RELOAD it. Compare this
fresh value SIGNED32 against upper007fffff and lowerff800001 (-8388607).
Clamp above/below bounds; otherwise retain fresh value. Store selected word
again even when already in bounds. There are two accumulator stores, not a
single widened saturating add. Extreme initial words can wrap across sign before
clamping. No valid initial accumulator range is independently enforced.
The final row's clear gate returns immediately; other rows continue to the next.

PB gate values are not cleared, signed source halves are not reset, own links/
active/f2 fields are not changed, and no fade/voice-free/cache/hardware/callback
request occurs. Shared/overlapping fields can change later fresh gates and sums;
no atomic or no-alias guarantee. No independently assigned scalar status.

## Complete fade step and residual behavior

DoDepopFade(C,D,A) receives copied-word pointer C, delta-halfword pointer D and
accumulator-word pointer A. Capture signed32 X=*A for branch selection:

| Captured X | Stored signed delta |
| --- | --- |
| X<=-3200 | +20 |
| -3200<X<=-160 | Truncate(-X/160) toward0 |
| -160<X<160 | 0 |
| 160<=X<3200 | Truncate(-X/160) toward0 |
| X>=3200 | -20 |

The interior divisions use signed high multiply66666667hex, arithmetic>>6,
add sign correction, then sign-extend16. Saturated branches avoid negating the
signed minimum. Boundaries ±160 produce ±1 toward zero; ±3200 produce capped
±20. No elapsed argument, measured frame/sample interval or variable fade rate.

After storing D, RELOAD *A and store that word to C. Then read SIGNED fresh *D,
RELOAD *A again; store wrap32(freshA+160*signedDelta) to A. This order can differ
from one calculation on captured X under aliases/concurrent writers. The copied
word is the pre-advance fresh accumulator, not the final word. Small residuals
inside(-160,160) remain unchanged; they are not snapped to0. No child call,
status assignment, source/target bounds or alias/pointer validation.

## Studio admission and first list: capture successor before children

At region entry, read outer studio byte ordinal from stack+24. Form
S=retained studio base(stack.word28)+bc*ordinal. Read S.byte50; ONLY exact1
admits the prepass. Other bytes skip to8028af78. This is not generic nonzero
admission or a fresh V.bytef3-based studio lookup.

Capture S.word48 as first-list root. For each V until null:

1. Read V.bytef1 and capture V.word0c as NEXT BEFORE any child requests.
2. If latch nonzero OR fresh V.word24 bit20 set, request HandleDepopVoice(S,V).
   Otherwise skip all remaining requests/state checks and follow captured NEXT.
3. After child, freshly read V.wordec. If notffffffff, request
   salSynthSendMessage(V,3). Sentinel test emits wrapped(ec+10000)==ffff;
   it is exactly the wordffffffff, not a16-bit comparison or generic negative.
4. Freshly read V.bytef0. If exact1, read V.bytef2; zero skips deactivation.
   Any other active byte, or exact1 with f2 nonzero, requests
   salDeactivateVoice(V), then stores V.bytef2=0.
5. Follow captured NEXT regardless of callback/list/record mutation.

HandleDepopVoice clears f1 before the subsequent message/status gates. Existing
word24 bit20 is not cleared by this region, so a retained node can be admitted
again later. The callback return is ignored. There is no rescan/new-root capture,
member/generation/cycle/count validation or active query before depop admission.

## Second list: read successor after child, then clear root

After first list completes, capture S.word4c. At every nonnull V, request
HandleDepopVoice(S,V) UNCONDITIONALLY, then freshly read V.word14 as next.
Unlike first-list traversal, this successor load occurs AFTER child mutation.
No message3, active/f2 gate, deactivation or f2 clear on this second path.
After traversal read command working/end pointers for the following region,
then store S.word4c=0. Root clearing is after traversal, not detachment first.
A root inserted/changed during traversal can therefore be overwritten by clear;
no guarantee of retained/new nodes is added. First-list root48 is not cleared
by this region. Command boundary calculation continues outside its endpoint;
no command emission/flush behavior is inferred here.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_depop_and_studio_prepass_evidence.json) pins two
complete bodies, one region and three linked fingerprints (whole builder/message/
deactivation): six spans,291 whole-body/region instruction anchors/four calls.
Seven wrapped-accumulation/clamp, thirteen fade-boundary/residual and six first-
list admission/sentinel/deactivation vectors plus nine field-binding rows are
source deductions, not original execution or list-mutation/audio experiments.
Full fresh ELFs/listings stay `/tmp`; no bodies/binary assets committed.

Remaining: fade callers/studio output/cache/DSP consumers, surrounding builder
admission/sorting/command/budget paths, actual accumulators/PB samples/gates/
latch/active/break/sentinel/root/link writers and valid studio/list/member/cycle/
index/value/alias/lifetime/IRQ/concurrency domains; original depop/fade/list/
callback/DSP/audio traces. Two bodies/one prepass region answered; whole builder/
full native binding and EVT-01 remain OPEN.
