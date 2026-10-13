# MusyX DSP studio command payloads and fade bindings

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Ten complete salBuildCommandList REGIONS
listed below; whole80288e00/25f8hex fingerprinted. Remaining voice/sample/budget/
command-admission regions stay open. Addresses/offsets/command IDs hexadecimal;
counts/byte lengths decimal. Read [rollover](MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md),
[studio lifecycle](MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md),
[depop/fade](MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md) and
[sorted voice iteration](MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md).
Static command/field/cache requests; DSP command meanings/audible roles unmeasured.

## Region boundaries and common publication rules

| Region | Complete source range, end exclusive |
| --- | --- |
| Command00 studio prefix | 80289020..8028906c |
| Command01 input loop | 8028910c..802891b4 |
| Command03/voice chain end/aux gate | 8028aa20..8028aa88 |
| Command04/second-aux mode gate | 8028ab28..8028abb0 |
| Command05 | 8028ac50..8028acc4 |
| Command10 | 8028ad64..8028adec |
| Command06/nine fades/outer loop | 8028ae8c..8028af94 |
| Command11/surround studio gate | 8028b034..8028b09c |
| Command09/surround loop/compressor gate | 8028b13c..8028b1a8 |
| Command12 | 8028b248..8028b298 |

S is current studio retained in r16 for the first seven regions; command globals
are in the rollover contract. P=dspCmdPtr. For each header below, store command
half at P, then listed halfword arguments at+2,+4,etc IN ORDER. Addresses split
high then low. P and source fields/frame bytes are reloaded at each source store,
not assumed one atomic address pair. Finish by rereading P and publishing P+length,
except command03's explicit ordering below. Pending fit/rollover occurs in the
separate recovered blocks; callers/gates outside these regions remain separate.
No null/index/type/alias/total-buffer validation or output initialization added.

## Command00 and complete input-record loop

Command00 appends(00,high16(S.word0),low16(fresh S.word0)), length6. Set retained
input record pointer R=S, ordinal j=0, base80407c68 for referenced studios;
add11362 decimal to retained budget register r23 with wrap32. Read fresh S.byte52;
unsigned j<count admits the next recovered rollover block (pending12).

Command01 on each admitted iteration: increment j BEFORE source reads;
add10573 decimal to budget r23; append six halves, length12:

`01, high16(Q), low16(fresh Q), R.half5a, R.half5c, R.half5e`

Q=word at80407c68+bc*U8(R.byte58)+28+4*(U8(salFrame8041efbc) XOR1).
Reference studio byte and global frame are reread independently for high/low.
After capturing final R.half5e, advance R by0c; publish pointer+12. Recheck fresh
S.byte52 against j. This is a counted input loop with a fresh bound, not a null-
terminated list. Raw frame XOR1 flips only the low bit; it does not clamp to0/1.
Reference studio byte has no bounds check. Input record extends from S+58 and
stride0c; valid S storage/count/reference/frame domains require producer evidence.
Input headers/budget additions alone do not establish actual mix contribution.

## Voice command03 and parameter-block chain tail

At8028aa20 (admission/rollover gate in preceding region), capture P; compute P+2
and publish dspCmdPtr BEFORE storing half03 through captured old P. Length2.
Then if retained last-voice PB pointer r18 nonnull, store PB.half0=0 then half2=0,
request DCFlushRangeNoSync(PB,c4). Null skips. This tail runs whether preceding
voice-produced-command flag selected command03; the gate itself remains outside
this region. No deactivation/list unlink or whole parameter block clear.

Compute next auxiliary index N=(U8(salAuxFrame8041efbb)+1) modulo3. Source uses
signed high multiply55555556hex/sign correction, multiply quotient by3/subtract;
over input byte0..255 this gives0..2 exactly. Store N only in retained r14;
global salAuxFrame is NOT advanced here. Capture/test S.wordac: nonzero admits
command04 rollover; zero skips to the following mode gate at8028ab98.

## Auxiliary/mode headers and exact gates

Command04 length10:
`04, high16(S.word(30+4*A)), low16(fresh S.word(30+4*A)), high16(S.word(30+4*N)), low16(fresh S.word(30+4*N))`.
A is fresh salAuxFrame for each current address half; N is retained next index.
After header, read S.word54. Mode0 reads S.wordb0: nonzero admits command05,
zero skips to command06. ANY nonzero mode admits command10 unconditionally,
without reading S.wordb0 as its gate. No low-byte mode normalization.

Command05 length10 has the same layout as04 but uses offsets3c+4*A and3c+4*N.
It unconditionally branches to command06's rollover afterward.

Command10 length10:
`10, high16(S.word(3c+4*F)), low16(fresh S.word(3c+4*F)), high16(S.word(3c+4*(F XOR1))), low16(fresh S.word(3c+4*(F XOR1)))`.
F is freshly reread salFrame for EACH address half. N is not used here.
No assertion that mode is exactly1 or frame a boolean. Aux current byte is raw
and unbounded, unlike retained N's modulo result; address pairing/order matters.
The regions do not clear ac/b0 or advance frame/aux globals.

## Command06, packed fade fields and outer loop

Command06 appends(06,high16(S.word(28+4*F)),low16(fresh S.word(28+4*F))), length6,
with separately reread frame bytes. Capture studio block B=S.word0 and retain
it across all child requests. Then call DoDepopFade(C,D,A) nine times in order:

| Studio accumulator A offset | B copied-word C offset | B delta-half D offset |
| --- | --- | --- |
| 04 | 00 | 04 |
| 08 | 06 | 0a |
| 0c | 0c | 10 |
| 10 | 12 | 16 |
| 14 | 18 | 1c |
| 18 | 1e | 22 |
| 1c | 24 | 28 |
| 20 | 2a | 2e |
| 24 | 30 | 34 |

This packs nine six-byte records in36hex bytes. Child uses captured X for
step selection but fresh accumulator/delta reads for copy/advance, as its complete
contract states. No child return test or B recapture between components. After
all nine, request DCFlushRangeNoSync(B,36).

Outer loop increments the stack+24 studio BYTE, then freshly reads
salMaxStudioNum8041efb9; unsigned byte ordinal<count branches to80288ebc.
Studio admission there requires byte50 EXACT1. Neither count/static-eight-studio
extent nor child/list/index/alias domains are validated by this loop.

## Surround prefix, eligible studio loop and compressor

Command11 appends(11,high16(dspSurround8041ef80),low16(fresh dspSurround)), length6.
Initialize retained ordinal0 and static studio base80407c68. At each ordinal,
select S2=base+bc*low8(ordinal); only S2.byte50==1 AND S2.byte51!=0 admits
command09 rollover/header. Other bytes skip header. No actual audible-output
admission claim follows from those storage gates.

Command09 appends(09,high16(S2.word(28+4*F)),low16(fresh S2.word(28+4*F))), length6;
frame is independently reread for high/low. Increment raw ordinal and compare
low8(ordinal)<fresh salMaxStudioNum; loop accordingly. Global frame untouched.
After loop read dspCompressorOn8041ef74: zero skips command12; any nonzero
admits its rollover/header. No low-byte boolean test or child request.

Command12 appends FIVE constant halves, length10:
`12,8000,000a,803b,ca00`.
Address803bca00 binds original compressorTable/1a40hex bytes, fingerprinted only;
its contents/algorithm/DSP interpretation remain open. Do not infer units for
8000/000a or export the table as a compiled asset. Following final command region
is already documented separately.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_studio_command_payloads_evidence.json) pins ten
complete regions and four linked fingerprints (whole builder/fade/cache/table):
14 spans,323 instruction anchors/11 calls (nine fades/two cache stores).
Six auxiliary-index/five raw-frame/seven mode/six surround-gate/one compressor-
layout vectors and nine fade bindings are deductions, not original execution or
reachable-domain proofs. Full fresh builder ELF/listing stays `/tmp`; no bodies/
binary assets committed; compressor table retained only as source hash identity.

Remaining: preceding command03 admission, full voice/sample/budget/command paths,
studio/input/count/reference/frame/aux/mode/buffer/coefficient/block/alias/
lifetime/IRQ/concurrency domains, cache/DSP/table consumers and original studio/
input/aux/surround/compressor/fade/command/audio traces. Ten local regions
answered; whole builder/full native binding and EVT-01 remain OPEN.

## 2026-10-13 — Budget/admission follow-up

[Budget/rejection/chain](MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md) closes initial/cost/rejection/PB-chain
regions and command03 admission. Remaining startup/sample/provider/value/index/
alias domains and original cost/DSP/audio execution remain open.

## 2026-10-13 — Cache-service source-name correction

[Cache range/source correction](GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
corrects earlier service-name attribution against US symbols/direct targets.
The body above now names the emitted original service; pointer/length/order
evidence stays unchanged. Hardware/handler/runtime cache effects remain open.
