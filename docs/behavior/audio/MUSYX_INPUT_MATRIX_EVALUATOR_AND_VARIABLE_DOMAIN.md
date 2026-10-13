# MusyX input matrix evaluator and variable domain

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete _GetInputValue8028f2d0/4a0hex
and varGet802821ac/68hex. Addresses/offsets/masks hexadecimal; counts/indices
and numeric constants decimal. Static original integer operations, not measured
controller/audio response. Read [controller reads](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md)
and [initial matrix stores](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md).

## ABI, live descriptor count and incoming mode residue

_GetInputValue(V,D,C,S) retains voice pointer V, descriptor block D and raw
channel/set words C/S. Set accumulator A=0, ordinal=0 and row pointer=D.
Before EACH iteration read fresh D.byte22 and compare ordinal unsigned.
Rows have stride8: source index byte0, flags/mode byte1, coefficient word4.
After processing, advance row pointer8/ordinal1. At termination store
D.half20=A.low16 and return zero-extended A.low16, including count0.

There is NO local count<=4, descriptor-null, voice-type/bounds/IRQ/active gate.
A conventional four-row block is not enforced; row4 would overlap the cache/
count fields. Child mutations of the descriptor count can alter termination.
The result cache is always stored, not only when a dirty flag is set.

Crucially, register r28 is the accumulator interpretation flag B but is NOT
initialized on entry. The ABI save helper preserves incoming registers; it does
not assign B. B==0 selects unsigned accumulation; any nonzero selects centered
signed accumulation. Mode0 establishes B for its source class; other modes
can read incoming residue first. Signed mode2 with B=0 also establishes B=1.
Unknown modes do not establish it. Do not silently initialize B=0 or infer a
valid first-row mode from a convenient example. Actual descriptor/caller
invariants must prove that incoming residue cannot affect a reachable result.

Example: first signed mode2 with scaled x=4096 and A=0 produces A=8192/B=1
if incoming B=0, but A=4096 if incoming B!=0. Both are derived source routes;
neither is an observed original register value. Empty count never reads B.

## Complete source routing and side effects

Read row.byte1 bit10hex first. If set, source is SIGNED: for V!=0 request
varGet(V,0,row.byte0), sign-extend its low16; V==0 yields0 without the child.
This override bypasses all index classifications below.

Otherwise read index i=row.byte0. The signed-source set is EXACTLY
1,10,128,131,160,161. For i160/161 and V!=0: read signed halfword at
V+(i*12-1464), store V.byte(i+312)=1, shift the signed value left1 with wrap32.
Thus160 reads half1c8/sets byte1d8;161 reads half1d4/sets byte1d9.
V==0 yields0 with no store. Other signed indices request
inpGetMidiCtrl(i,raw C,raw S), use low16 then subtract8192.

All other indices are unsigned-source routes:

| Index | Raw source X before coefficient scaling |
| --- | --- |
| 162 | V.byte131<<7, or0 for V==0 |
| 163 | V.word15c logical>>9, or0 for V==0 |
| 164 | Elapsed route below, or0 for V==0 |
| Other | inpGetMidiCtrl(i,raw C,raw S).low16 |

Index164 reads synthRealTime LOW8041eea4, V timestamp LOW94, synthRealTime
HIGH8041eea0, then V timestamp HIGH90. Subtract with borrow to form wrapped64-bit
current-minus-timestamp. Rotate/insert constructs the low32 bits of that
unsigned difference logical>>8. Interpret that word SIGNED for an upper-only
16383 cap; negative words are not clamped to0. Store V.bytea8=1. There is no
atomic clock snapshot, ordered timestamp validation or physical age measurement.

Sources/classes are selected before coefficient and combine-mode reads. Child
calls may mutate row coefficient/flags or D count before their fresh reads;
no source/voice/descriptor identity recheck. Side-effect marker stores remain
possible even for a later unsupported combine mode.

## Exact wrapped coefficient scaling

Let W=signed32(fresh row.word4). Compute H=ASR32(W,1), then P=low32(X*H),
then x=signed32(P) arithmetic>>15. No widened product, floating coefficient,
round-to-nearest or division truncated toward zero.

For SIGNED sources clamp x to[-8192,8191]. For UNSIGNED sources apply ONLY
signed upper cap16383; negative scaled results remain negative. Source163
may supply more than16 bits; source164 can already be negative. No general
finite/range/normal-controller-byte invariant is inferred from these names.

Now read fresh row.byte1.low4 as mode. Modes0..3 combine;4..15 leave A and B
unchanged, after source/scaling/possible source side effects.

## Complete signed and unsigned combine tables

Every add/subtract/multiply below first keeps low32 bits. ASR is arithmetic
shift of signed32; LSR is logical shift of unsigned32. Define:

- CS(z): interpret signed32, clamp to[-8192,8191].
- CU(z): interpret signed32, clamp to[0,16383].
- UU(z): interpret unsigned32, cap at16383 (no signed lower clamp).
- M(a,b): low32(a*b).

B comparisons are zero versus any nonzero. The tables state B changes explicitly;
otherwise retain its current raw word.

| SIGNED source mode | B==0 | B!=0 |
| --- | --- | --- |
| 0 | A=x+8192; B=1 | A=x+8192; B=1 |
| 1 | A=CU(A+x) | A=CS(A+x-8192)+8192 |
| 2 | A=CS(LSR(M(x,A),13))+8192; B=1 | A=CS(ASR(M(A-8192,x),13))+8192 |
| 3 | A=CU(A-x) | A=CS(A-8192-x)+8192 |
| 4..15 | No accumulator/flag change | No accumulator/flag change |

Signed mode2's B==0 branch uses a LOGICAL shift, not the signed branch's
arithmetic shift, even for a negative product. It does not first recenter A.

| UNSIGNED source mode | B==0 | B!=0 |
| --- | --- | --- |
| 0 | A=x; B=0 | A=x; B=0 |
| 1 | A=UU(A+x) | A=CS(A+x-8192)+8192 |
| 2 | A=UU(LSR(M(A,x),14)) | A=CS(ASR(M(x,A-8192),14))+8192 |
| 3 | A=CU(A-x) | A=CS(A-8192-x)+8192 |
| 4..15 | No accumulator/flag change | No accumulator/flag change |

Unsigned modes1/2 B==0 use unsigned upper comparisons; unsigned mode3 B==0
uses signed upper/lower behavior. These are not interchangeable generic
saturating operations. B==0 unsigned mode0 can retain a negative scaled x;
final output is its low16, with no additional normalized range clamp.
No combine mode writes B into the block; it is only register state during this
invocation. The cache halfword likewise does not preserve B for a later call.

## Complete variable reader

varGet(V,K,I) compares RAW K with0. K!=0 requests inpGetExCtrl(V,raw I), then
sign-extends returned low16. That external-controller child is linked here;
its source/flag/side effects remain a required complete contract.

K==0 selects j=I.low5. For j0..15 read raw word V+achex+4*j. For j16..31
read synthGlobalVariable803f8fa4+4*(j-16). In both cases return sign-extended
LOW16 of that word; high16 do not contribute. No local V-null, slot bound,
active/IRQ/generation test, write or default value. The matrix caller supplies
K=0 and invokes it only when V!=0; direct callers need their own valid domain.
Masking means I32 aliases slot0 and I255 aliases global slot31. Do not interpret
all variable indices as external controllers or return a full signed word.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_input_matrix_evaluator_evidence.json) pins two
complete bodies/four linked fingerprints: six spans,191 instruction anchors,
six direct calls. It records all256 normal source routes, variable override,
64 combine-mode/interpretation vectors, three first-mode2 residue vectors,
eight wrapped scaling/six elapsed/six variable-index vectors. These are source
arithmetic deductions, not original execution or evidence for a specific incoming
r28 value. ABI entry hashes alone do not close complete save/restore fallthrough
chains. Full fresh inspection ELFs/listings stay `/tmp/inbox-GetInputValue.elf`
and `/tmp/inbox-varGet.elf`; no original bodies/assets committed or code executed.

Remaining: actual wrappers/callers and descriptor construction/count/first-mode/
coefficient/index/flag writers, incoming-r28/reachable-type/raw numeric invariants,
external-controller child, voice/variable/clock/marker/cache consumers and valid
pointer/count/alias/IRQ/concurrency domains, original matrix/controller/audio
traces. Two local bodies are answered; EVT-01 and full native/source/runtime/
hardware equivalence remain OPEN.

### Wrapper/external child follow-up

[Complete17 cache/aux/external-controller wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
now closes those local callers and varGet external child. Dirty clears precede
evaluation; wrappers do not initialize r28. Actual descriptor/dirty/caller
writers, first-mode/bounds domains and original matrix/audio traces remain open.
