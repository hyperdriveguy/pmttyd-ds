# MusyX stream activation primary lookup and voice admission

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamActivate REGIONS
8027cacc..8027cca0 and8027d0e0..8027d0ec, ends exclusive; ABI prefix
8027cab4..8027cacc and suffix8027d0ec..8027d10c anchored. Whole body
8027cab4/658hex fingerprinted; companion continuation remains open.
Addresses/offsets/masks hexadecimal; counts decimal. Static source deductions,
not original allocation/activation/audio execution. Read
[allocation caller](MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md),
[ID lookup](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[voice block](MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md) and
[IRQ wrappers](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md).

## Retained ID, IRQ and independent64-row lookup

Retain raw public ID X in r29, initialize result holder r28=0; request hwDisableIrq
at8027cad8. Scan streamInfo803f98e8 exactly64 rows, stride68hex, ascending ordinal
with eight iterations each unrolling eight state checks. Byte+c==0 skips ID word4;
any nonzero state reads and compares full32-bit public ID with X. First match
wins; no state1/2/3 restriction, count/global voice-capacity bound or sentinel-ID
rejection. This body inlines lookup; it does not call GetPrivateIndex.

Miss returns indexffffffff internally and branches to common primary return:
hwEnableIrq at8027d0e4, then return retained0. No record mutation/voice request.
Unknown nonzero states and active rows with IDffffffff can match under raw source
rules; valid ID/state producers remain separate.

## Fresh state3 voice request and failure-before-cursor publication

Found ordinal i: retain offset i*68 and row state-array base. Freshly read row+c.
Only EXACT3 requests voiceBlock(U8(row.byte58)) at8027cc4c. Other states skip
all local primary voice allocation/cursor/state stores and continue companion read.
No low-byte normalization of state or requirement that it remained unchanged
since lookup. No voice index/hardware activity/buffer-selector check.

After voiceBlock returns, STORE raw returned word to row.word4c BEFORE its
sentinel check. Exactlyffffffff: request hwEnableIrq at8027cc6c, assign r3=0
and branch directly to ABI epilogue8027d0ec. This path does not call the common
primary-return enable again. It does not clear row state, free the stream buffer,
restore old voice word or undo child mutations.

Any other returned word, including0/high-bit values: store cursor word20=0,
then state+c=1. These stores are unconditional after child success and use the
retained ordinal; no public-ID/state/generation/extent recapture. Child changes
to that record do not add a gate. The voice child owns allocation/steal/request
semantics; this caller's non-sentinel test does not prove audible playback.

## Fresh companion word and two terminal returns

After state3 success or other-state bypass, freshly read row.word0, replacing
r28's earlier offset/result with the companion word. Exactlyffffffff branches
to8027d0e0: set r28=1, then common hwEnableIrq at8027d0e4, return1. This path
has no companion lookup/query, immediate hardware start or producer callback.

Any other companion word enters the still-open continuation at8027cca0, with
another disable/lookup/activation sequence. It can return a different result and
perform additional enables/requests; this delivery does not collapse it to a
simple recursive helper or claim full-function success/IRQ closure.

The allocator publishes companion word0=ffffffff before state3/buffer setup;
that previously recovered caller can therefore use this primary terminal path
when its row fields remain unchanged. Other companion writers, cycles and
callback/shared-state mutations still need their own evidence.

## ABI and local scope

Function allocates32 decimal stack bytes, saves LR at frame+24 and GPR28..31.
Epilogue restores LR/registers, releases stack and returns r3. Early voice
failure sets r3 directly; common return moves retained r28 to r3. Prefix's
initial0 is the miss result, while companion sentinel chooses1. This is not a
single generic child-return forwarding wrapper. No saved prior IRQ token is
passed to enables; linked wrapper/runtime nesting semantics remain separate.

[Manifest](../../../tools/musyx_stream_activation_primary_evidence.json) pins
six spans (two regions/whole body/three children),134 anchors (120 region/14 ABI),
four call sites. Five lookup/seven primary/four slot vectors checked, including
unknown state/sentinel ID, failure voice publication, raw successful voice and
companion sentinel versus continuation. Generic verifier checks hashes/anchors/
calls, not semantic/provider assumptions or original execution. Fresh full ELF/
listing/generator stay `/tmp`; no bodies/binary assets committed.

Remaining: companion continuation/recursive activation/request/enables/return
composition, actual state/ID/priority/voice/companion/IRQ/providers and valid
value/index/extent/alias/lifetime/concurrency/ABI domains, stream deactivation/
free/frequency/mix, transitive ARAM/DSP and original activation/failure/audio
traces. Primary local portion answered; whole sndStreamActivate and EVT-01 OPEN.
