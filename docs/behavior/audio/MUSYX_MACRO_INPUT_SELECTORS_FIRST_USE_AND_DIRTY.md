# MusyX macro input selectors, first use and dirty publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete15 voice selector bodies (each104hex
bytes), mcmdAuxAFXSelect80281d50/17chex, mcmdAuxBFXSelect80281ecc/17chex,
inpSetGlobalMIDIDirtyFlag8028dfcc/24hex and four original aux mask tables.
Addresses/offsets/masks hexadecimal; counts and scalar constants decimal.
Static source requests/writes, not original interpreter/audio execution.
Read [builder/translation](MUSYX_INPUT_DESCRIPTOR_BUILDER_FX_COPY_AND_TRANSLATION.md),
[evaluator/r28 domain](MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md) and
[dirty/cache wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md).

## Complete voice selector first-use protocol

Each selector receives voice pointer V and two-word command pointer E.
Capture the two voice flag words114/118. Only word118's selector mask M below
contributes to the first-use test; the word114 contribution is masked with0.
Initialize builder mode0. If captured word118&M==0, store word118=old OR M,
then rewrite captured word114 unchanged; retain builder mode0. Otherwise read
E.word4 and use its low8 as builder mode. The first-use flag publication
PRECEDES coefficient decoding and builder request; no rollback/active/IRQ/
voice identity/null/command-size check or enum validation is added.

| Complete selector | Entry | First-use word118 mask | Descriptor V offset | Dirty word220 mask |
| --- | --- | --- | --- | --- |
| mcmdVolumeSelect | 80280e14 | 80000 | 224 | 1 |
| mcmdPanningSelect | 80280f18 | 100000 | 248 | 2 |
| mcmdPitchWheelSelect | 8028101c | 200000 | 290 | 8 |
| mcmdModWheelSelect | 80281120 | 400000 | 2d8 | 20 |
| mcmdPedalSelect | 80281224 | 2000000 | 2fc | 40 |
| mcmdPortamentoSelect | 80281328 | 1000000 | 320 | 80 |
| mcmdReverbSelect | 8028142c | 800000 | 368 | 200 |
| mcmdPreAuxASelect | 80281530 | 20000000 | 344 | 100 |
| mcmdPreAuxBSelect | 80281634 | 40000000 | 38c | 400 |
| mcmdPostAuxBSelect | 80281738 | 80000000 | 3b0 | 800 |
| mcmdSurroundPanningSelect | 8028183c | 4000000 | 26c | 4 |
| mcmdDopplerSelect | 80281940 | 8000000 | 2b4 | 10 |
| mcmdTremoloSelect | 80281a44 | 10000000 | 3d4 | 1000 |
| mcmdFilterSwitchSelect | 80281b48 | 40 | 3f8 | 2000 |
| mcmdFilterParameterSelect | 80281c4c | 800 | 41c | 4000 |

Portamento's bit80hex and descriptor320 explain the gap in the prior14 cached
voice wrappers; this is a source selector/schema, not a new portamento consumer
contract. First-use masks and result-dirty masks are separate fields/protocols.
Filter masks are low word118 bits40/800hex, not inferred sequential high bits.

After the shared decoding below, request inpAddCtrl(V+descriptor,I,K,mode,
variableBoolean). Ignore any incidental child result. Then load FRESH V.word220,
OR this selector's dirty mask and store. Dirty publication occurs even if the
builder refused an append because count>=4. No cached halfword update, immediate
evaluator/hardware request or first-use flag restoration follows.

## Exact shared command decoding and coefficient arithmetic

Read E.word0 into retained W0. Its upper16 bits, with lower16 cleared, are
interpreted signed32 and divided by100 using signed multiply-high by51eb851fhex,
arithmetic shift5 and sign correction (truncation toward zero). Call this B.
Read fresh E.word4, extract bits16..23 as signed8, shift left8 and apply the SAME
signed division sequence; call this F. If B<0, K=B-F; otherwise K=B+F.
No unsigned reinterpretation, floating coefficient, rounding-to-nearest,
absolute fraction repair, clamp or combined one-step division. Fraction sign
is independent of base sign. Recorded exhaustive component checks cover every
possible high16 and signed fraction byte; this does not assign original units.

Finally read E.word4 again. VariableBoolean is1 when its bits8..15 byte is
nonzero, otherwise0. I=(captured W0>>8)&255. Pass raw coefficient K and selected
mode to the builder. A variable boolean requests its variable-source bit;
it does not itself read a variable or evaluate a controller.

Mode on existing selectors, fraction and variable byte can come from distinct
E.word4 reads. No atomic command snapshot, later mask/type/source recheck or
construction of invented command fields. New-selector mode0 is imposed
independently of the command's low mode byte.

## Complete aux FX selector tables and targets

Aux A/B read initial E.word4 and parameter P=that word's top byte. Read
studio=V.byte11f and derive D=auxBase+studio*90hex+P*24hex, with A base803f9464,
B base803f8fe4. Parameter P also indexes four pairs of first-use masks at
803bbc38 (A) or803bbc68 (B), stride8, and dirty masks at803bbc58/803bbc88,
stride4. No P<4/studio<8 bounds gate or association/channel validation.

| P | A first-use (word114,word118) | B first-use (word114,word118) | A dirty | B dirty |
| --- | --- | --- | --- | --- |
| 0 | (1,0) | (10,0) | 80000001 | 80000010 |
| 1 | (2,0) | (20,0) | 80000002 | 80000020 |
| 2 | (4,0) | (40,0) | 80000004 | 80000040 |
| 3 | (8,0) | (80,0) | 80000008 | 80000080 |

Capture word114 then word118 and test their masked OR. Both masked results0
means first use: store word118=old OR lowMask, then word114=old OR highMask;
builder mode remains0. Any masked bit present uses initial E.word4.low8 as mode.
This differs from the voice selectors' later mode-word read. Retain D and dirty
mask across the same coefficient/index/variable decoding and builder request.

After inpAddCtrl(D,I,K,mode,variableBoolean), test retained dirty mask bit31.
If set, load FRESH V.byte121 then V.byte122 and request
inpSetGlobalMIDIDirtyFlag(channel,set,dirtyMask). If clear, read fresh V.word220,
OR retained dirty mask and store. All valid table masks have bit31 set, so valid
parameters take the global path; unchecked table reads are not normalized into
those four valid masks. No direct evaluation/cache write or child refusal check.

The first-use flag lives on this voice even though the aux descriptor is shared
by studio/parameter. These bodies do not prove unique voice ownership of that
shared descriptor or restore flags after another voice changes it. Original
shared ownership/alias/concurrency and actual macro caller invariants remain open.

## Complete global dirty setter

inpSetGlobalMIDIDirtyFlag(C,S,F) computes address
80409548+S.low8*40hex+C.low8*4, loads the word, ORs RAW full F and stores it.
No set255 FX special path, channel255 rejection, bounds/active/IRQ/atomicity
check, mask clamp or independent status. It is a read/OR/write, not a captured
voice-cache clear or callback installation. Source tables support eight sets/
16 channels; the setter does not enforce them. Aux macros request it after the
builder using fresh voice channel/set bytes.

## Conditional first-row/r28 consequence and remaining scope

For valid D, a clear first-use mask forces builder mode0, which resets count
and publishes row0 mode0; the evaluator establishes its interpretation flag when
that row is reached. This closes that conditional local caller chain. It does
not prove every evaluation has such a first row: an already-set first-use flag
uses the command mode, other writers can reset counts/rows, and aux descriptors
are shared. No default incoming-r28 assignment or global first-mode guarantee.

The DOL direct-entry scan found17 inpAddCtrl callsites in these17 selector bodies;
indirect/REL calls and interpreter opcode dispatch/reachability remain separate
requirements. Do not treat local selector completion as the entire macro engine
or original audio binding.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_macro_input_selectors_evidence.json) pins18
complete bodies,one prior builder fingerprint,four aux mask tables:23 spans,
480 instruction anchors,19 resolved calls. It records15 selector schemas,
four first-use/four field/four global-dirty/nine coefficient vectors. Common
coefficient/decode/tail regions were compared across all15 voice bodies.
All65536 high-half/all256 fraction-byte division cases match the signed source
sequence. These are deductions, not original execution/first-use observations.
Fresh full inspection ELFs stay `/tmp/inbox-<selector-name>.elf`; eleven voice
selectors also have corresponding `/tmp/inbox-<name>.listing` files. No original
body/binary asset committed or code executed.

Remaining: original interpreter/command/voice/first-use-flag lifecycle and actual
selector calls, shared aux ownership, other descriptor/dirty writers, reachable
first-row/r28/count/index/coefficient/parameter/studio/channel/set/bounds/alias/
IRQ/concurrency domains and original selection/recompute/audio traces.18 local
bodies are answered; EVT-01 and full native/source/runtime/hardware equivalence
remain OPEN.

### Interpreter registration follow-up

[Complete outer scheduler/scoped dispatch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md)
now pins wait/event/active traversal, macro-clock tail, command-budget/PC order
and exact17 selector opcode registrations. Remaining interpreter initialization/
opcodes, actual command/flag/list/time writers and original audio remain open.
