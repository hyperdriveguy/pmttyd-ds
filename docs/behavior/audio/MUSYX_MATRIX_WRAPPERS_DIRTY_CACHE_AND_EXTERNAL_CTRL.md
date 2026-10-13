# MusyX matrix wrappers, dirty caches and external-controller dispatch

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete14 voice wrappers (each48hex bytes),
inpGetAuxA8028fb60/b4hex, inpGetAuxB8028fc14/b4hex,
inpGetExCtrl8028fffc/fchex, two dirty-mask tables and nine-entry dispatch table.
Addresses/offsets/masks hexadecimal; counts/indices decimal.
Static source cache/request semantics, not original control/audio cadence.
Read [matrix evaluator/domain](MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md),
[controller reads](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md),
[initial stores](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md)
and [aux frame consumer](MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md).

## Complete voice dirty-bit/cached-value wrappers

Each receives raw voice pointer V and reads captured flags F=V.word220.
If its mask is clear, read its cached halfword and return zero-extended without
calling the evaluator or writing flags. If set:

1. Store V.word220=F with ONLY this mask cleared, using captured F.
2. Load channel byte V+121, then set byte V+122.
3. Request _GetInputValue(V,V+descriptorOffset,channel,set).
4. Return its raw result; no wrapper postprocessing or flag restoration.

The descriptor address is derived before the flag store; channel/set are read
after it. No fresh flags reread before clear, late revalidation, delayed dirty
clear, rollback or wrapper cache store. The evaluator owns its halfword cache
store. Child/other writers can re-dirty during evaluation; the wrapper does not
clear a second time after return. No active/IRQ/null/count/domain check.

| Complete wrapper | Entry | Mask | Descriptor offset | Cached halfword |
| --- | --- | --- | --- | --- |
| inpGetVolume | 8028f770 | 1 | 224 | 244 |
| inpGetPanning | 8028f7b8 | 2 | 248 | 268 |
| inpGetSurPanning | 8028f800 | 4 | 26c | 28c |
| inpGetPitchBend | 8028f848 | 8 | 290 | 2b0 |
| inpGetDoppler | 8028f890 | 10 | 2b4 | 2d4 |
| inpGetModulation | 8028f8d8 | 20 | 2d8 | 2f8 |
| inpGetPedal | 8028f920 | 40 | 2fc | 31c |
| inpGetPreAuxA | 8028f968 | 100 | 344 | 364 |
| inpGetReverb | 8028f9b0 | 200 | 368 | 388 |
| inpGetPreAuxB | 8028f9f8 | 400 | 38c | 3ac |
| inpGetPostAuxB | 8028fa40 | 800 | 3b0 | 3d0 |
| inpGetTremolo | 8028fa88 | 1000 | 3d4 | 3f4 |
| inpGetFilterSwitch | 8028fad0 | 2000 | 3f8 | 418 |
| inpGetFilterParameter | 8028fb18 | 4000 | 41c | 43c |

Bit80hex is not consumed by these14 wrappers; do not assign it a new role from
this table. Names such as Pedal/FilterSwitch do not add a boolean conversion:
cached values and evaluator returns remain raw source results.

None of these wrappers assigns the evaluator's r28 interpretation flag.
They therefore do not repair its incoming-register precondition. Valid descriptor
first-mode/count/type/flag/coefficient construction must establish that domain.
Original initial stores provide some mode0/count defaults, but this alone is not
a proof for every actual wrapper call or later descriptor mutation.

## Complete aux A/B mask selection and cache flow

inpGetAuxA/B(T,P,C,S) receives studio, parameter, channel, set as raw r3..r6.
Compute these original low8-based addresses:

- Dirty word G at80409548+S.low8*40hex+C.low8*4.
- Mask K at tableBase+P.low8*4.
- Descriptor D at auxBase+T.low8*90hex+P.low8*24hex.

A tableBase=803be680, auxBase=803f9464.
B tableBase=803be690, auxBase=803f8fe4.
The four original masks per side are:

| Parameter | A mask | B mask |
| --- | --- | --- |
| 0 | 80000001 | 80000010 |
| 1 | 80000002 | 80000020 |
| 2 | 80000004 | 80000040 |
| 3 | 80000008 | 80000080 |

First load K, then captured G. Compute changed=(G&K)!=0 via neg/or/bit extraction.
If changed, store captured G&~K to that same dirty word BEFORE selecting D and
requesting _GetInputValue(0,D,raw C,raw S). No voice pointer or byte-normalized
C/S is substituted into that request. If unchanged, select D and return its
zero-extended halfword20. No evaluator/flag write on that cached path.

The shared high bit80000000 can admit ANY parameter and is cleared along with
that parameter's mask when admitted. No per-side private dirty word, captured
flag restoration, clear of all parameter bits or post-child recheck. A request
can affect admission of a later A/B parameter through this shared word.

No count/studio/parameter/channel/set bounds, association255 check, active/IRQ
or callback validation. Original shapes are eight studios/four parameter rows,
eight sets/16 dirty-word channels and four mask words per side; these wrappers
do not enforce those domains. Out-of-range low8 inputs derive unchecked reads/
writes. The caller's association-byte admission belongs to its own contract.
No full descriptor initialization or input-result clamp is requested here.

## Complete external-controller dispatch and fallback index

inpGetExCtrl(V,I) retains ORIGINAL raw I separately, selects i=I.low8 and uses
the nine-entry table803be6c4 only for128..136. Its dispatch remaps are:

| i | Dispatch selector |
| --- | --- |
| 128 | 128 |
| 129 | 130 |
| 130 | 160 |
| 131 | 161 |
| 132 | 131 |
| 133 | 132 |
| 134 | 162 |
| 135 | 163 |
| 136 | 164 |

Other i uses itself. Dispatch selector160 reads signed V.half1c8;
selector161 reads signed V.half1d4. Return wrap16((signedHalf<<1)+8192).
These two paths do NOT set matrix source marker bytes1d8/1d9. No V-null check
or result-range saturation beyond low16. Thus aliases130/131 and direct160/161
use the corresponding LFO field; no matrix coefficient/mode/cache request.

ALL OTHER selectors use fallback: read V.byte121 channel. If255, return0;
otherwise read V.byte122 set and request
inpGetMidiCtrl(ORIGINAL raw I,channel,set), returning its low16. The dispatch
selector is NOT forwarded. For example alias129 dispatches130 but fallback
reads controller129; alias134 dispatches162 but does not read voice.byte131,
instead forwarding original controller134. Do not replace fallback with the
matrix's special162/163/164 sources or feed a remapped index to the MIDI getter.

No dirty/cache mutation, active/IRQ/valid-pointer/bounds gate. Controller ranges
beyond133 remain unchecked in the downstream generic byte read. The complete
varGet(V,K!=0,I) caller sign-extends this getter's low16; that changes presentation
of the returned word, not this service's source selection or marker behavior.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_matrix_wrappers_and_external_controller_evidence.json)
pins17 complete bodies,two previously-complete child fingerprints and three
tables:22 spans,224 instruction anchors,17 resolved calls. It records14 decoded
voice mask/descriptor/cache schemas,eight aux masks,nine exact jump targets,
nine external dispatch/fallback vectors,16 captured dirty-word vectors,five aux
address/five signed-half vectors. These are source deductions, not execution or
proof of caller register/descriptor invariants. Fresh full inspection ELFs stay
`/tmp/inbox-inpGetVolume.elf` through corresponding named wrapper ELF files;
14 later wrappers also have `/tmp/inbox-<name>.listing` files. No original body/
binary asset committed or original code executed.

Remaining: actual wrapper/dirty/descriptor/index/channel/set callers and writers,
incoming-r28/first-mode/count/numeric domains, cache/aux/filter/voice consumers,
valid pointer/mask/studio/parameter/bounds/alias/IRQ/concurrency policies and
original cache/recompute/aux/external-controller/audio traces.17 local bodies
are answered; EVT-01 and full native/source/runtime/hardware equivalence remain OPEN.
