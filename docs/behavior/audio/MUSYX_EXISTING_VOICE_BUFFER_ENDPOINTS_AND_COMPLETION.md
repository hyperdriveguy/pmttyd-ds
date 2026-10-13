# MusyX existing-voice buffer endpoints and completion

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGION
80289ad4..80289bf4 exclusive; whole80288e00/25f8hex fingerprinted.
Startup/sample regions remain open. Addresses/offsets/masks hexadecimal;
shifts/counts/divisors decimal. Read [voice admission/setup](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md),
[hardware buffer writers](MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md),
[virtual sample/position queries](MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md)
and [host position leaf](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
Static local address/gate/request order; original playback units/audio unmeasured.

## Admission context and type4/5 field copy

V=r29 and parameter block PB=r19 are retained by preceding builder code.
[Admission](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md) sends nonzero V.bytef0
OTHER THAN1 into this region; exact1 takes the separate startup path. This local
region itself does not normalize/revalidate active state, pointers or descriptor.
Read V.byte90 as raw type t. If t==4 OR t==5:

1. Read unsigned V.bytea0 and store it as halfword to PB.b4.
2. RELOAD V.byte90. If it is not5, skip pending endpoint handling and proceed
   directly to completion gate80289bc0.
3. For fresh5, read V.byte9c; nonzero skips pending update. Zero reads unsigned
   PB.halfc2; zero also skips. Only latch0 AND c2!=0 admits type5 update.

Thus initial type4 can take type5 handling if its fresh type changes, while
initial4/5 followed by fresh6 does not dispatch into the type6 block. Do not
replace captured/fresh dispatch with one normalized type switch under mutation.

For initial t other than4/5: only EXACT6 enters the separate type6 pending gate.
Other initial types skip field copy/endpoint writes and still run completion.
No source-valid type set, member/generation/IRQ/alias guarantee inferred.

## Complete type5 wrapped endpoint calculation

Capture N=V.word98, then A=V.word94; these names denote raw local roles, not
validated sample/byte/frame units. X=wrap32(N-1). Compute:

`H = unsignedHighMultiply(24924925hex,X)`
`M = wrap32((wrap32(X-H) logical>>1)+H)`
`Q = M logical>>3`
`R = wrap32(X-14*Q)`

For unsigned32 X, Q/R equal quotient/remainder14. Proof:24924925hex equals
(2^32+3)/7. Write X=14*q+r,0<=r<14. Then H=2*q+h, where
h=floor(r/7+3*X/(7*2^32));0<=r+h<=15. The subtraction/add construction
gives M=floor((X+H)/2) without its own word overflow, so Q=floor((X+H)/16)=q.
R is r. There is no divwu instruction or floor repair on N0. Build:

`Z = wrap32(wrap32(A<<1) + wrap32(((M<<1)&fffffff0)+2+R))`

The middle masked shift equals wrapped16*Q; it discards low phase bits and high
word overflow before remaining additions. No widened address, saturation or
N>0 guard. N0 wraps X toffffffff and follows ordinary quotient/remainder logic.

Store captured raw A to V.word78 BEFORE completing PB endpoint stores. Store
high16(Z) to PB.half76, then low16(Z) to PB.half78; finally V.byte9c=1. Endpoint
halves derive from the same captured Z. The latch publishes after both halves.
No PB.c2 clear, buffer upload/cache/ownership request or whole record reset.

## Complete type6 wrapped endpoint calculation

For initial type6, read byte9c and PB.halfc2 with the same latch0/c2!=0 admission.
Capture A=V.word94, then N=V.word98. Compute Z=wrap32(N+(A logical>>1)-1).
Store raw A to V.word78, then Z high/low halves to PB76/78, then byte9c=1.
Unlike type5, A shifts RIGHT1 and count uses no divide14/encoded offset. Do not
infer equivalent units, reuse type5 mapping, round odd A upward or add a common
zero-length/address guard. Initial type6 does not update PB.b4 in this region.

Both admitted paths can wrap and can write end values outside a presumed buffer;
actual address/count/encoding/residency/extent invariants require writer evidence.
Existing hardware leaves store raw loop address/count; their local bodies alone
do not prove these endpoint domains. A nonzero latch/c2zero skips endpoint and
word78/latch writes, preserving existing values.

## Independent completion gate and request order

After ALL type/skip/update paths converge, freshly read V.word8c. Any nonzero
skips completion and reaches80289bf4, the separately documented ADSR setup gate.
Zero: read V.worddc, then V.word84. If unsigned dc<84, continue to ADSR gate.
If dc>=84 (including equality/zero threshold): request salSynthSendMessage(V,0),
then salDeactivateVoice(V), then branch directly to shared loop tail8028a964.
Ignore callback return. Message request precedes deactivation, unlike the
[budget rejection cascade](MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md).

The completion gate is not conditional on admitted type or endpoint update,
PB.c2, latch state, a current hardware query or signed-negative value. It does
not test sample-end DSP output or clear stored flags/resources. Source dc is
compared as an unsigned word; word8c chooses gate, not a normalized boolean.
The host-position helper advances dc/e0 only for its own word8c!=0 gate; that
separate body does not establish all producers of dc on this word8c==0 path.
No measured length/progress units or complete voice lifetime inferred.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_existing_voice_buffer_completion_evidence.json)
pins one complete region and three linked fingerprints (whole builder/message/
deactivation): four spans,72 instruction anchors/two calls. Eight type5/type6
endpoint, seven dispatch/latch/c2 and seven completion vectors are deductions.
The original high-multiply quotient/remainder formula matched mathematical
unsigned divide14 for all65536 low values, six32-bit edges and4096 deterministic
32-bit fixtures:69638 cases. This is not original instruction execution or a
reachable-length proof. Full fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed.

Remaining: startup/sample-format bodies, actual type/latch/PB.c2/bytea0/address/
count/word78/word8c/progress/length/callback/record writers and valid encoding/
index/overflow/residency/alias/lifetime/IRQ/concurrency domains; cache/DSP consumers
and original endpoint/completion/voice/audio traces. One local existing-voice
region answered; whole builder/full native binding and EVT-01 remain OPEN.

## 2026-10-13 — Startup prefix follow-up

[Startup format prefix](MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md) closes the preceding startup callback/
ITD/type0..6 descriptor/direct/seek/cursor region. Loop/endpoints/finish, actual
type/descriptor/offset/register domains and original sample/DSP/audio remain open.
