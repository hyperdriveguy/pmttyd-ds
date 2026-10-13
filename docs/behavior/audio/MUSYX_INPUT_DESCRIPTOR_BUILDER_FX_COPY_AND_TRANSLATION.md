# MusyX input descriptor builder, FX copy and translation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete inpAddCtrl8028f0ac/98hex,
inpFXCopyCtrl8028f144/114hex, inpTranslateExCtrl8028ff8c/70hex,
and nine-entry jump table803be6a0/24hex. Addresses/offsets/masks hexadecimal;
counts, indices and scalar values decimal. Static source mutations, not
original controller/audio execution. Read [evaluator/r28 domain](MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md),
[cache/dirty wrappers](MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md)
and [controller/default reads](MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md).

## Complete descriptor builder ABI and reset/admission

inpAddCtrl(D,I,K,M,Variable) receives block pointer, raw source index,
coefficient word, mode word and variable flag in r3..r7. Retain D/K/M/Variable.
First test M.low8: EXACT zero stores D.byte22=0. Any nonzero low8 leaves the
count unchanged. This is not a low4 mode-zero test; flags such as10hex prevent
the reset even when the evaluator's combine mode will be0.

Then read count n=D.byte22. If n>=4, return with no descriptor stores or
translation child. A preceding zero-mode reset has already occurred, so it
admits row0 even when prior count was4 or255. Otherwise:

1. Store D.byte22=n+1 BEFORE translation or row population.
2. If RAW Variable==0, request inpTranslateExCtrl(raw I), retaining its result
   as the source index. Any nonzero Variable bypasses this request.
3. For nonzero Variable set flags=(M OR10hex).low8; for zero Variable the
   eventual byte store uses M.low8 unchanged.
4. Using captured n, store row.byte0=index.low8, then row.byte1=flags,
   then row.word4=RAW K. Row address=D+n*8.

No block-null/active/IRQ/valid-source/coefficient/enum gate, dirty-bit write,
cache reset, other-row cleanup, constructor return status or failure rollback.
Count is published before the row is populated; it is not reread after child
return. The direct translation child has no block writer, but source reentrancy/
concurrency/alias policy is not established by that observation.

The four-row cap is enforced for APPENDS here, unlike the evaluator's live
count traversal. It does not validate an existing count or remaining rows;
nonzero-mode count>=4 simply refuses. There is no row compaction/replacement
except the exact zero-mode count reset.

## What this builder proves about the first-mode requirement

A valid zero-low8-mode call resets count and produces row0 with combine mode0,
including Variable!=0 where flags become10hex. This establishes the evaluator's
interpretation flag when that row is reached. However an empty block with
M.low8=1,2 or3 also admits row0 without a reset or first-mode validation.
Thus this builder does NOT prove every descriptor's first row establishes r28.
Actual caller sequences/initialization/other writers remain a required domain.

Raw M=256 resets and stores flags0 (or16 for a variable source). Raw M=16 does
not reset, but stores combine-mode0. High mode bits are discarded only at the
specific low8 test/store; Variable is tested as a FULL word. No provisional
incoming-r28 default or universal first-row repair is added.

Variable mode feeds evaluator varGet(V,0,index) and its local/global low5 slot
selection. It is not the external-controller getter's nonzero-selector path.
For normal sources, translation changes aliases before evaluator classification;
for variable sources, original index.low8 is stored without that translation.
The coefficient is preserved raw for the evaluator's wrapped signed arithmetic.

## Complete external-controller translation

inpTranslateExCtrl(I) computes i=I.low8. For i128..136, jump-table dispatch
returns the following literal word. Otherwise return ORIGINAL raw I unchanged,
including its high bits. No memory/controller/voice read or marker write:

| i | Returned literal |
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

The normal builder stores returned low8, so unmapped high bits disappear there;
direct translator callers receive them unchanged. For example I=384 selects
alias128 and returns128, while I=256 returns256 but the builder stores0.
No unknown-index refusal, bounds repair, low5 variable mask or independent status.

This translator differs from inpGetExCtrl: the builder stores its translated
index for matrix source routing; the getter's fallback forwards the ORIGINAL
index. Normal builder alias130 becomes matrix LFO source160 with its marker
side effect, whereas the external getter has its own centered LFO return and
no matrix marker. Do not merge those separate protocols.

## Complete FX controller copy ABI, row selection and routing

inpFXCopyCtrl(I,DestinationVoice,SourceVoice) first computes i=I.low8, then reads
DestinationVoice.wordf4 and SourceVoice.wordf4, in that order. Use each ID.low8
as an FX row ordinal:

`destination = fx_ctrl8040dec8 + destinationID.low8*86hex`;
`source = fx_ctrl8040dec8 + sourceID.low8*86hex`.

No full-ID/generation match, pointer/active/bounds/IRQ check or channel/set-byte
lookup. Source/destination voice pointers are dereferenced before controller
routing; IDffffffff selects unchecked row255 rather than refusing.

| i | Copied byte offsets |
| --- | --- |
| 0..63 | i&31, then (i&31)+32 |
| 128 or129 | 128, then129 |
| 132 or133 | 132, then133 |
| All other0..255 | i alone |

For paired cases: load first source byte, store first destination byte, THEN
load second source byte, store second destination byte. No snapshot/memcpy/
atomic pair, byte-range clamp, controller assembly or value conversion. Generic
64..69 copies raw threshold bytes;96..101 copies raw bytes despite the controller
reader's constant-zero returns for that range. Indices beyond133 are unchecked
address consequences, not invented special-source copies.

No dirty/cache flag is marked, no last-note/channel-default/MIDI-set table is
copied, and no evaluation/hardware request follows. An outer caller must supply
any required invalidation; this local service does not imply that a cached voice
wrapper will recompute after the copy. Alias and source/destination lifetime
invariants remain separate from this ordered byte-copy contract. Return is
incidental, not an independent success/error result.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_input_descriptor_builder_fx_copy_translation_evidence.json)
pins three complete bodies,two ABI entry fingerprints and one jump table:
six spans,58 instruction anchors including indexed stores/jump dispatch,
three resolved calls. It records nine builder/17 translation/all256 FX-copy
routes/five full-ID→row vectors and exact builder publication order. These are
source deductions, not original execution or proof that all caller descriptors
have a valid first mode. ABI entry hashes do not describe their entire
fallthrough save/restore chains. Fresh full ELFs/listings stay
`/tmp/inbox-inpAddCtrl.elf`, `/tmp/inbox-inpFXCopyCtrl.elf`,
`/tmp/inbox-inpTranslateExCtrl.elf`; no original bodies/binary assets committed
or original code executed.

Remaining: actual builder/copy/translator callers and dirty invalidation owners,
other descriptor/count/index/mode/coefficient/variable writers, first-row/r28/
source/FX-row bounds/lifetime/alias/IRQ/concurrency domains and original build/
copy/recompute/audio traces. Three local services are answered; EVT-01 and
full native/source/runtime/hardware equivalence remain OPEN.
