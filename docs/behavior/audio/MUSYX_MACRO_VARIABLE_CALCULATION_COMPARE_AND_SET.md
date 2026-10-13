# MusyX macro variable calculation, comparison and controller set

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdVarCalculation80282214/220,
mcmdIfVarCompare80282434/14c and inpSetExCtrl802900f8/1e4hex; complete eight
opcode regions96..101/112..113. Addresses/offsets/masks hexadecimal; modes/opcodes/
scalar values decimal. Static memory/arithmetic/request order, not original
command execution or audio. Read [fetch/dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md),
[variable reader](MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md),
[external getter](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md) and
[controller request cores](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md).

## Variable addressing and fresh command words

Both handlers retain voice V, command pointer C and mode K.low8. Define Bn(W)
as unsigned byte at bits8*n..8*n+7, n0 least significant. Variable slot j=ID.low5:
j<16 addresses V+ac+4*j; otherwise global803f8fa4+4*(j-16). Getter paths read
full words; calculation later sign-extends low16, comparison retains full32.
No null/table-extent/active/IRQ/command/member guard or implicit initialization.
External source flags use nonzero bytes, not a single bit. Getter requests use
raw byte ID and retain only child result.low16. Command words are reread at the
specified stages, so a child/shared-cell mutation can affect later selection.

## Complete calculation source order

mcmdVarCalculation(V,C,K): capture C.W0 and C.W1 before first source request.
Left source ID=B0(W1), external iff B3(W0)!=0. External requests
inpGetExCtrl(V,ID); otherwise read variable word. Retain LEFT=S16(result.low16).

If K.low8==4, reread C.W1 and take RIGHT=S16(W1>>8), the16 bits8..23; no
second source request. Otherwise reread W1; right source ID=B2(W1), external
iff B1(W1)!=0. Resolve getter/variable as above, then RIGHT=S16(low16).

| K.low8 | Raw signed32 calculation |
| --- | --- |
| 0 | LEFT+RIGHT |
| 1 | LEFT-RIGHT |
| 2 | LEFT*RIGHT |
| 3 | RIGHT0 gives0; otherwise signed division, truncating toward0 |
| 4 | LEFT+immediate RIGHT |

These S16 operands keep sum/product/division within signed32. Division -32768/-1
produces32768 before saturation. No divide instruction executes for zero right.
K>=5 defines NO result assignment: r30 retains its incoming saved-register value,
then destination handling still occurs. Do not invent fallback0, copy-left or
no-write behavior. The actual opcode stubs below supply only valid0..4 modes;
arbitrary direct caller reachability/incoming-r30 remains separate.

Then reread C.W0 for destination ID=B2(W0), external iff B1(W0)!=0. Clamp signed
raw result to-32768..32767. External requests inpSetExCtrl(V,ID,S16(clamped));
variable route stores sign-extended clamped value as a FULL word at selected
local/global slot. No wrapping S16 result or dirty/job publication locally.
External setter can clamp further, as described below. Child return ignored;
no explicit success value or interpreter stop marker is assigned.

## Complete comparison, inversion and PC update

mcmdIfVarCompare(V,C,K) captures W0 first: left ID=B2(W0), external flag B1(W0).
Read external low16 zero-extended, or FULL variable word; retain LEFT32.
Then reread W0 and W1: right ID=B0(W1), external flag B3(fresh W0). Resolve
external low16 zero-extended or full variable word; retain RIGHT32.

K0 compares full-word equality. K1 computes SIGNED32 LEFT<RIGHT using the
source XOR/arithmetic-shift/subtract sign-bit expression. It does not
sign-extend both low16 as calculation does. For example storedffffffff is
signed-1, while controller resultffff is zero-extended65535. K>=2 defines no
boolean assignment: incoming saved r31 remains, then inversion/test still occurs.
Do not invent false or skip behavior for such direct calls.

Reread C.W1 after source requests. B1(W1)!=0 replaces the result with whether
its LOW8 equals0 (logical inversion of low8, including unsupported incoming
register values). Then test result.low8!=0. True reads V.word34 macro base and
sets V.word38 PC=wrap32(base+U16(W1.high16)*8). False leaves PC unchanged.
The source rotate19/mask13..28 yields that byte displacement. No lookup/new
macro base, range/alignment check, base-null check or stop/voice-free request.

## Exact opcode bindings and direct immediate assignment

Each call stub passes V,&shared cstep8041ef08 and the listed mode; ignores
return and joins without changing initial stop marker0. Region ends exclusive.

| Opcode | Region | Child/mode |
| --- | --- | --- |
| 96 | 802837b8..7cc | Calculation0 |
| 97 | 802837cc..7e0 | Calculation1 |
| 98 | 802837e0..7f4 | Calculation2 |
| 99 | 802837f4..808 | Calculation3 |
| 100 | 80283808..81c | Calculation4 |
| 101 | 8028381c..870 | Inline immediate assignment |
| 112 | 80283870..884 | Comparison0 |
| 113 | 80283884..894 | Comparison1 |

Shortened ends retain80283 prefix. Opcode101 reads shared W1 first and captures
S16(W1.low16), destination ID=B2(captured W0), external flag B1(captured W0).
External requests inpSetExCtrl(V,ID,value); otherwise directly stores the sign-
extended word in local/global variable slot. It does NOT add to an old variable,
use bits8..23 immediate, read a source or perform calculation saturation.
Normal fetch already advanced PC; comparison may replace it with branch target.
Whole interpreter and other opcode handlers remain open.

## Complete external setter clamp and dispatch exclusion

inpSetExCtrl(V,raw I,raw value) first takes S16(value.low16), clamps to0..16383.
Retain original raw I; i=I.low8. Only i128..136 use the nine-entry table803be6e8:
128→128,129→130,130→160,131→161,132→131,133→132,134→162,135→163,136→164.
Other i retains the original index as dispatch selector. Selector.low8==160 or
161 returns WITHOUT controller or voice LFO write. This is a local no-op setter
route, unlike the getter's two direct voice-half reads.

Other selectors read channel=V.byte121. Channel255 skips all requests. Otherwise
read set=V.byte122. The branch after this load REUSES channel comparison CR;
it does not compare set with255. Thus there is no independent set255 rejection
in this body. Raw set validity belongs to the child/caller domains.

With Q the clamped14-bit value and captured channel/set:

- ORIGINAL i<64: request inpSetMidiCtrl(i.low5,channel,set,Q>>7), then
  inpSetMidiCtrl(i.low5+32,channel,set,Q.low7).
- ORIGINAL i128/129 or132/133: even index=i&fe; request that controller with
  Q>>7, then even+1 with Q.low7.
- Other original indices: request inpSetMidiCtrl(ORIGINAL RAW I,channel,set,Q>>7).

The remapped selector controls only160/161 exclusion. Pair/fallback decisions
use original i; request fallback preserves the full original word. For example
alias133 selects132 but the pair is controllers132/133, not the getter's
remapped fallback. Both pair requests retain channel/set and Q across the first
child; no fresh channel lookup or combined14-bit atomic request. Returns ignored.
No inpSetMidiCtrl14 call, voice-matrix/dirty write or child semantics inferred:
complete downstream inpSetMidiCtrl remains a separate open provider.

## Evidence and remaining bindings

[Manifest](../../../tools/musyx_macro_variable_commands_evidence.json) pins
three bodies/eight regions/four linked ABI-child identities/two tables:17 spans,
224 anchors/24 direct calls/eight opcode registrations. Eleven arithmetic/seven
comparison/eight setter-clamp/six index vectors and rotate/comparator/address/
source-order deductions are static checks, not original execution. Fresh full
ELFs/listings stay `/tmp`; no original bodies/binary assets committed.

Remaining: downstream MIDI setter, actual command/source/variable/controller/
base/PC/incoming-register writers and reachable mode/domain invariants, shared-
cell/alias/IRQ/reentrancy/concurrency, other opcode paths and original calculation/
comparison/controller/audio traces. EVT-01 remains OPEN.
