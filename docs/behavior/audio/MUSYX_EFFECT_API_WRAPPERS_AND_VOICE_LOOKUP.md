# MusyX effect API wrappers, voice lookup and IRQ nesting

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Eight complete local bodies, with core
synth/resource/hardware services explicitly separate. These extend
[effect PlayEx](SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md) and
[record controls](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md), not full MusyX
playback or original execution equivalence.

| Body | Address / hex byte size |
| --- | --- |
| sndFXStartEx | 8027a0d4/88 |
| sndFXCtrl | 80279fd4/60 |
| sndFXCtrl14 | 8027a034/60 |
| sndFXKeyOff | 8027a094/40 |
| sndFXCheck | 8027a15c/4c |
| vidGetInternalId | 80284964/50 |
| hwDisableIrq / hwEnableIrq | 80292810/38 /802927d8/38 |

## Complete effect start wrapper

Retain four raw words F,V,P,T from r3..r6 before any request. Call hwDisableIrq.
Then read one unsigned byte from803f8f94+2*(T&ff)+1, using rotate/mask
at8027a108. This is synthITDDefault, a16-byte BSS symbol; the local wrapper
has no index bound or provisioning write. Its eight2-byte record extent is
not proof that every arbitrary low8 T is valid. The normal SoundEfxPlayEx
caller supplies T0, but other callers/source producers remain separate.

Request synthFXStart80278d48 with six arguments in order:
`(F,255,V,P,T,thatUnsignedByte)`. None of F,V,P,T is narrowed by the wrapper
before this request; only the default-table index masks low8 T. Retain its
returned word across hwEnableIrq and return it. No wrapper-level resource
lookup, allocation/null/FX bounds check or returned-word classification.
The SoundEfxPlayEx caller supplies F,127,pan,0 and interprets its return as
underlying voice identity; that caller's allocation record is another object.

## Controller and KeyOff wrappers

sndFXCtrl and sndFXCtrl14 retain all three raw arguments before Disable,
then call synthFXSetCtrl80278ea8 or synthFXSetCtrl14 at80278f8c with those words
UNCHANGED. Retain the core return, Enable, return the retained value.
No byte14-bit clamp, controller-range guard, voice lookup or return translation
exists in these wrappers. Record-control leaves may narrow specific values
before calling; do not infer an extra wrapper clamp from its name.

sndFXKeyOff similarly retains the raw voice word, Disable,
synthSendKeyOff802790f4(rawVoice), retain result, Enable, return result.
None of the wrappers skips its core request solely because input voice==-1.

## sndFXCheck returns the external input identity

Retain raw external voice ID X; Disable; request vidGetInternalId(X).
Only when the returned INTERNAL word==-1, replace retained X with-1.
Enable and return retained X. A valid internal mapping does not return its
internal voice index from this wrapper. No other error code is classified.

Consequently Check(Check(X)) equals Check(X) for a stable mapping snapshot,
but the original effect pool calls Check twice with an Enable/Disable interval
between them. A voice can disappear between requests. Preserve those calls
and fresh lookup boundaries in the original ordered service contract rather
than assuming unconditional idempotence or returning an internal index.

## Complete internal voice mapping lookup

vidGetInternalId rejects X==-1 before reading root. Otherwise load linked-list
root vidRoot=word8041ef2c (r13+200c). At each nonnull node, read unsigned
word(node+8) and compare to raw X unsigned:

- Equal: return node.wordc unchanged, with no additional liveness test.
- Node key>X: stop immediately and return-1.
- Node key<X: follow node.word0 and repeat.

Null root/next returns-1. A node's other fields are not read here. This is a
linear ordered scan, not a binary search or table indexed by low8 handle.
Early termination assumes ordered-key source provisioning for successful
lookup; the leaf does not validate ordering/cycles or mutate/free nodes.
Node allocation/insertion/removal/generation lifetime remain core dependencies.

## Separate MusyX IRQ nesting

Counter=U16(8041efe4), saved state=word8041efe0, distinct from SoundEfxPlayEx's
S+20c/210 pair. Do not merge them into one synthetic counter.

Disable reads old counter, stores low16(old+1), THEN if old==0 requests
OSDisableInterrupts and stores its returned word to saved state. Enable
unconditionally subtracts1/stores low16, and only when that new value==0
requests OSRestoreInterrupts(savedState). No Enable zero guard or overflow
repair; Enable at0 wraps toffff without restore, Disable atffff wraps to0
without disabling. These are encoded invalid/nesting-domain cases, not
assertions that the original game reaches unbalanced usage.

## Source verification and remaining chain

[Metadata](../../../tools/musyx_effect_api_evidence.json) pins eight complete
bodies, linked core identities, original branches/calls/arguments and mapping/
IRQ source anchors. Re-run generic DOL verification with that manifest. Fresh
complete listings remain `/tmp/inbox-snd*.elf`, `/tmp/inbox-vidinternal.elf`
and `/tmp/inbox-musy-*.elf`, read-only ISO inspection only.

Remaining: synthFXStart resource/group/macro/sample admission, controller and
KeyOff mutations, voice-map producers/lifetime, default-table provisioning,
OS/thread/hardware services, full spatial/live main processing and original
source/execution/audio traces. This closes the local public API/lookup/IRQ
portion; parent EVT-01 and full-game audio remain OPEN.

[Complete synth effect start/control/KeyOff and FX lookup](MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md) now closes those exact core request bodies and binary search. Resource registry producers, actual sound/macro/voice/controller mutations and original runtime remain open.
