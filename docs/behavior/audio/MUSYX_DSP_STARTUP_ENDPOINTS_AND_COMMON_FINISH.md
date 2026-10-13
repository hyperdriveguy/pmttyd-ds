# MusyX DSP startup endpoints and common finish

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGION
80289688..80289ad4 exclusive. Combined source coverage is audited below.
Addresses/offsets/masks hexadecimal; shifts/counts decimal. Read
[startup prefix](MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md),
[existing endpoint arithmetic](MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md),
[volume ramps](MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md) and
[envelope command loop](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
Static endpoint/field/state operations, not original playback units/DSP/audio.

## Retained base and exact encoded-index notation

V=r29,PB=r19,S=r16 remain retained. B=r24 is the format-specific base from the
prefix: type0/1/4/5 uses wrap32(V.word78<<1); type3 uses raw word78;
type2/6 uses word78 logical>>1. Unsupported prefix types can leave B as prior/
incoming register state; no default base is added by this region.

Define E(B,I)=wrap32(B+wrap32(16*Q+2+R)), with unsigned Q=floor(I/14),R=I-14*Q.
All I inputs are wrapped32 BEFORE division. Source uses the already-recovered
24924925 high-multiply/subtract/shift formula, not division instructions. This
notation retains masked/wrapped16*Q and additions. Physical encoded-address/
sample/byte interpretation and valid source domains remain separate.

## Nonzero word8c: loop flag and type-specific endpoints

Read V.word8c. Nonzero stores PB.half6e=1, then freshly reads unsigned type90.
Captured gate value can differ from the fresh word8c used in endpoint arithmetic.
Let U be V.word88, L be fresh V.word8c, A be V.word94. Source capture order and
route formulas are:

| Type | Reads/order | Loop start | Inclusive end | Extra store |
| --- | --- | --- | --- | --- |
| 0,1,4 | L then U | E(B,U) | E(B,wrap32(U+L-1)) | None |
| 5 | L then U then A | wrap32((A<<1)+2) | E(B,wrap32(U+L-1)) | byte9c=0 |
| 6 | L then A then U | A logical>>1 | wrap32(B+U+L-1) | byte9c=0 |
| 2,3,7..255 | U then L | wrap32(B+U) | wrap32(B+U+L-1) | None |

Type5 start uses fresh buffer A but end uses RETAINED prefix B; do not unify
these bases. The type5/6 latch clear occurs before endpoint publication. Other
routes do not clear it. No empty-loop/offset/count/address guard or signed-value
clamp. Even if L changes to0 after the nonzero gate, wrapped L-1 arithmetic still
executes the selected branch.

Publish start high16 then low16 to PB72/74; end high16 then low16 to PB76/78;
finally PB.halfc2=0. This c2 clear belongs ONLY to the nonzero gate path.
Endpoint halves use captured results. No cache/voice callback/physical buffer
request or word78 rewrite in this region.

## Zero word8c: fallback address and full-length end

Zero stores PB.half6e=0, then fresh type dispatch. Let N=V.word84 and
Z=dspARAMZeroBuffer8041ef50, read independently on each selected route:

| Type | Reads/order | Fallback start | Inclusive end |
| --- | --- | --- | --- |
| 0,1,4,5 | N then Z | wrap32((Z<<1)+2) | E(B,wrap32(N-1)) |
| 3 | N then Z | Z | wrap32(B+N-1) |
| 2,6 | N then Z | Z logical>>1 | wrap32(B+N-1) |
| 7..255 | No selected address initialization | Retained r15 | Retained r26 |

All branches publish current start/end halves to PB72/74/76/78 in that order.
Unsupported types fall through using uninitialized-for-this-branch/prior registers;
do not substitute zero/error addresses. N0 wraps N-1. ZeroBuffer source identity
alone does not prove memory/address/encoding validity. This path does NOT clear
PB.c2 or byte9c. Word8c admission and type dispatch are fresh independent loads.

## Common parameter copies and startup offset

After either endpoint path, copy unsigned V.halfcc to PB08 and V.halfce to PB0a.
Read V.byted4, publish PB.halfba=(byte!=0). RELOAD d4; if fresh nonzero, copy
V.halfd6 to PBbe, V.halfd8 to PBc0, then PBbc=0. Zero skips those three writes;
there is no common filter-history/coefficient clear. Capture startup slot byte
V.byteda into r27. PB.half0e=(captured byte==0), using cntlzw/rotate-low16 logic.
No clamp to0..4 or time-global reload here; r27 is retained for the startup
entry into the envelope loop. Predicate is exact across all256 byte values.

## Nine desired/host/PB level seeds and delta resets

Process first SIX rows in order; for each read unsigned desired half ONCE,
store captured value to V.host, then PB.level:

| Component | V desired | V host | PB level | PB delta |
| --- | --- | --- | --- | --- |
| A0 | 4c | 5e | 12 | 14 |
| A1 | 4e | 60 | 16 | 18 |
| A2 | 50 | 62 | 2e | 30 |
| B0 | 52 | 64 | 1a | 1c |
| B1 | 54 | 66 | 1e | 20 |
| B2 | 56 | 68 | 32 | 34 |
| C0 | 58 | 6a | 22 | 24 |
| C1 | 5a | 6c | 26 | 28 |
| C2 | 5c | 6e | 2a | 2c |

After A/B rows, PB.half0c=0. Freshly inspect A0/A1 levels; either nonzero OR3.
Inspect B0/B1; either nonzero OR30. Inspect B2; nonzero OR80.
Then copy the THREE C rows in order. Zero all nine PB deltas in table order.
No whole PB/V clear, scaling, saturation or reconstruction of host values from
previous histories. Desired/host/PB aliasing can affect later fresh reads.

Fresh S.word54 chooses final enables:

- Mode0: C0/C1 nonzero OR600; A2 nonzero OR4; B2 nonzero OR80 AGAIN;
  C2 nonzero OR1000. Each test reads the fresh PB level(s).
- Any nonzero mode: OR4000 unconditionally; C0/C1 nonzero OR600;
  B2 nonzero OR80. A2/C2 enable tests are skipped; their seeded levels/deltas
  remain. No exact mode1 requirement or normalized low byte.

Earlier A/B enables and later mode enables OR into fresh PB.half0c, preserving
previous writes after its explicit zero. No ramp-bit enable from initial nonzero
levels beyond the exact masks above. No V.word24 dirty-bit reset here.

Finally stack.word20=1, then V.bytef0=2; branch to8028a370, the startup envelope
entry. This skips ordinary prefix8028a354, so it preserves captured r27 startup
slot and the previously stored V.word20 cursor. Envelope/commands/budget work
occurs AFTER active2 publication; these stores alone do not prove audio playback.

## Composed local DSP builder coverage audit

The [manifest](../../../tools/musyx_dsp_startup_endpoints_and_finish_evidence.json)
records source ranges from the ten prior region manifests plus this region.
Their union covers ALL2419 non-ABI instruction words in80288e00..8028b3f8.
The remaining11 words are ordinary entry/exit stack/linkage/ABI-helper requests;
these are separately anchored in this manifest. Total2430 words/25f8hex bytes.
Whole-body hashing and region instruction/call verification are distinct from
this coverage audit; hashes alone would not establish semantic closure.

Read the composed contracts for local behavior:

- [Initialization/rollover/finalization](MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md).
- [Studio depop prepass](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md).
- [Studio/input/aux/surround/fade payloads](MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md).
- [Voice collection/sorting](MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md).
- [ADSR setup/update/commands](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md).
- [Startup prefix](MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md), this finish,
  and [existing voice endpoints/completion](MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md).
- [Volume-ramp consumers](MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md),
  [parameter copies](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md) and
  [cost/rejection/chain](MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md).

The complete LOCAL builder body is now answered by this composition, including
source defaults/unchecked domains and exact publication order. Transitive child/
provider/address/type/list/ABI/IRQ/concurrency validity, actual DSP program/cache/
command execution and original audio remain separate requirements. This is not
complete native audio or a measured fidelity claim; EVT-01 stays OPEN.

## New-region evidence and remaining work

New manifest pins region/whole builder: two spans,286 anchors (275 region/11 ABI),
zero calls within the new region. Eight loop/six fallback/six initial-enable
vectors, nine seed bindings and all256 startup-byte predicate cases are deductions,
not original execution or reachable-domain proofs. Synthetic unsupported-type
fixtures supply base registers explicitly rather than inventing runtime values.
Full fresh builder ELF/listing/coverage generator stays `/tmp`; no bodies/binary
assets committed. Prior region manifests are reverified for the composed audit.

Remaining: actual descriptor/address/count/offset/loop/type/zero-buffer/filter/
mode/gain/start-slot/register/flag/list/PB/callback providers and valid extent/
residency/encoding/index/overflow/alias/lifetime/ABI/IRQ/concurrency domains;
cache/DSP/command consumers, original startup/endpoints/voice/audio traces and
full native binding. Other precision/macro/stream/song/backend work and lower-
priority inbox/full-game requests remain open.
