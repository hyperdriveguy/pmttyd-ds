# MusyX synth effect resource lookup and chained voice controls

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Eight complete local bodies: synthFXStart,
synthFXSetCtrl/14,synthSendKeyOff,dataGetFX,fxcmp,sndBSearch and the already
linked vidGetInternalId. Core macro/voice allocation, resident-resource writers,
MIDI storage and hardware execution remain boundaries. Read
[public API/voice/IRQ](MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) and
[original Hammer dependencies](HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md).

## Loaded FX registry lookup

Complete dataGetFX8027f38c/9chex retains shared scratch key80405b14 and table
base80400ce8 (`dataFXGroups`, BSS size600hex=128 records of12 decimal bytes).
Store LOW16 raw input FX ID to scratch half0 BEFORE testing group count.
Starting ordinal0, freshly read U16 dataFXGroupNum8041eef0 at each loop test.
For every admitted group record R, capture R.word8 entries pointer, R.half2
entry count, then request sndBSearch(scratch,entries,count,10,fxcmp).
First nonnull result returns immediately. Otherwise advance R by12 and ordinal
by1. Exhaustion returns0. Other group fields and scratch bytes are untouched.

No128-group capacity clamp, record/pointer/count validation or automatic group
loading. The shared scratch key is a real write; this is not a pure lookup.
Changes to count during comparisons can change subsequent admission. Actual
resource insertion/removal and registry/data lifetime remain separate owners.

fxcmp8027f37c/10hex returns U16(lookup.half0)-U16(entry.half0), signed32.
The two unsigned16 values make this difference exact without word overflow.
It compares FX IDs, not group/macro/sample IDs.

## Complete binary-search service

sndBSearch80290384/a4hex receives lookup pointer, array pointer, signed count,
raw byte stride and comparator. EXACT count0 returns0 without comparison.
Otherwise low=1, high=count, and execute at least one iteration:

- mid=arithmeticShiftRight(wrap32(low+high),1).
- Retain entry=array+wrap32(stride*(mid-1)); call comparator(lookup,entry).
- Result0 returns retained entry. Negative sets high=mid-1. Nonnegative
  nonzero sets low=mid+1; sign extraction uses rotate/mask on the returned word.
- Signed low<=high repeats; otherwise return0.

The concrete FX caller passes unsigned16 count and stride10, but this general
body only tests zero, not negative count or overflowing midpoint/stride/pointers.
It preserves pointers across comparator calls; it does not copy/re-sort entries,
validate comparator ordering or read the key itself. Resource sorting and
callback/source invariants are prerequisites for useful lookup.

## Complete synthFXStart and exact sound-start operands

synthFXStart80278d48/e4hex retains six raw arguments F,N,V,P,T,D and initializes
return-1. Call dataGetFX(F); null returns-1 WITHOUT sound start. For nonnull FX
record E, default each retained value only when its LOW8 equals255:
N→U8(E8), V→U8(E6), P→U8(E7), in that order. Other raw V/P words remain intact.
The original FX row is10 bytes: half0 FX ID, half2 object/macro ID, bytes4/5
priority/voice-count,6 velocity,7 pan,8 key and9 an additional raw parameter.
The last byte's wider semantics remain unassigned by this local body.

Request synthStartSound802772c4 with ALL16 ordered operands:

| ABI location | Original operand |
| --- | --- |
| r3 | U16(E2), object/macro ID |
| r4 | U8(E5) |
| r5 | U8(E4) |
| r6 | 0x80000000 OR U16(F) |
| r7 | U8(N OR0x80) |
| r8 | Retained/defaulted raw V |
| r9 | Retained/defaulted raw P |
| r10 | 255 |
| caller stack+8,+c,+10,+14 | 255,0,0,255 |
| caller stack+18 | U8(E9) |
| caller stack+1c,+20,+24 | 0,retained T,retained D |

This is a resource-object request, not a direct sample launch. The ordinary
Hammer FX862/861 resolves original group6/object995/994 when those loaded
records are provisioned; sample776/775 is a downstream macro dependency.
Return the sound-start word unchanged. No wrapper-level sndActive, resource
fallback, range clamp, allocation or post-start repair is supplied.

## Controller cores: matching generation and chain-stop rule

Complete synthFXSetCtrl80278ea8/e4 and synthFXSetCtrl14 at80278f8c/e4hex retain
raw controller C and raw value V; processed=0. Call vidGetInternalId(externalID).
While internal ID I!=-1:

1. Read CURRENT synthVoice pointer8041ee90 and resolve P=base+1112*(I&ff).
   Compare P.wordf4 against FULL I. Mismatch immediately RETURNS processed;
   it does not follow the stale node's next link.
2. Read P.word114 and P.word118. The source ANDs the first with0 and the
   second with2, so only P118 mask2 selects the MIDI-set byte. Preserve both
   source reads; do not reinterpret the discarded high word as another gate.
3. Select U8(P217) when mask2 set, otherwise U8(P122). Call
   inpSetMidiCtrl(C,I&ff,selectedByte,V) at8028e618, or
   inpSetMidiCtrl14 with the SAME argument roles at8028eb34.
4. Ignore that callee's scalar return. Freshly reload synthVoice, read next
   I from base+1112*(previousI&ff)+ec, set processed=1, then retest I!=-1.

Thus return1 means at least one matching-generation controller REQUEST,
not proof of an accepted controller/hardware change. No controller/value/low8
voice-slot bounds or cycle check. Next-link lookup uses a fresh base AFTER the
controller call. These cores have no sndActive gate or IRQ calls themselves;
the public wrappers supply the documented critical section.

## KeyOff core: active gate and stale-node continuation

Complete synthSendKeyOff802790f4/88hex first reads U8 sndActive8041ee60.
Zero returns0 BEFORE voice lookup. Otherwise resolve internal ID and loop using
the same current synthVoice, low8 index,1112 stride and FULL P.f4 identity test.

On match call macSetExternalKeyoff80283ab4(P), set processed=1. On mismatch
skip that request WITHOUT ending the chain. In BOTH cases freshly reload
synthVoice and read next internal ID at the prior slot+ec, then repeat until-1.
Return processed. This differs from both controller cores' early mismatch exit;
do not consolidate them into one generic chain walker with a uniform stale policy.
A matching request is not immediate sample termination or proof of voice deletion.

## Verification and remaining source chain

[Source manifest](../../../tools/musyx_synth_effect_core_evidence.json) pins
complete/linked bodies, branch/load/ABI/call anchors and resolved direct calls.
Fresh full listings stay `/tmp/inbox-synth*.elf`, `/tmp/inbox-datafx.elf` and
`/tmp/inbox-fx-bsearch.elf`, read-only ISO inspection. No source code, sample,
macro bytes or message payload is exported into history.

Remaining: FX registry insertion/removal/sorting and bank provisioning;
synthStartSound/macro/sample/voice admission and chain producers/lifetime;
MIDI controller and external-keyoff internals; valid pointer/count/generation/
slot/cycle/IRQ domains, spatial/live engine processing and original execution.
Eight local bodies are answered, while full EVT-01 audio/runtime closure stays
OPEN. No generated tone or WAV-only playback is original behavior evidence.

[Complete FX registry insert/remove](MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md) now supplies group/reference/count/pointer producers and the live byte9=31 mutation consumed by the sound-start request. Group-push/project residency, sorting/lifetime and original runtime remain open.
