# Original HitN64 Entry/Main and display frontier

`hit_n64_effect.hpp/.cpp` implement the complete original effHitN64Entry
801c6344/270 and effHitMain801c61bc/188. effHitEntry8013f1b4/20 is a forwarding
wrapper, preserving kind, amount, XYZ and scale. No original dialogue, command,
timing constant, control or audience state is replaced. Calls advance in source
simulation invocations; rendering must not tick the effect a second time.

## Real record and payload ownership

Entry directly invokes the recovered `effectPoolEntry`, using supplied actual
EffectPoolServices memory. It does not mint an effect address or append a damage
notice. The source constructor reserves its real record before heap allocation,
sets name802fb0c8 (HitN64), and uses signed word arithmetic to compute
`count = trunc_signed(wrap32(amount*5+20)/3)+1`. Heap3 allocation size is
`wrap32(count*0x44)`; source count and requested bytes are never clamped. A
negative amount can produce count1 or0. Original effEntry exhaustion behavior
and valid mapped storage remain the EffectPool provider's source domain.

Neither constructor contains a null-allocation repair branch. A completed heap
return0 is still used for subsequent original payload stores. Native providers
must admit actual storage or leave the allocation unavailable; returning a
fabricated successful zero effect is not a recovered failure path. Nullopt is a
held source child. Earlier record reservation/name/count stores do not repeat
when that heap request waits.

The constructor installs callback801c61bc and writes the original root payload:
kind0, XYZ4/8/c, scale28, lifetime30, age34, animation38 and six color bytes3c..41.
Source cnt_table80419888 contains four byte13 values; its source kind indexing is
not clamped to a substitute. Kinds0,1,2/default retain their exact original
white/yellow-red/yellow-green/cyan-blue color tuples. Each child is0x44 bytes.
Original raw RNG8026c324 is required once for each particle1..count-1. Direction
uses truncating signed division of successive wrapped360 numerators by count-1;
random angle preserves the source signed mulhw remainder360, including negative
raw values. Positions, zero values, duplicate writes, bias captures and stores
retain source order. No fake RNG or guessed particle positions exist.

Retained cursors bind all provider identities and exact original arguments,
including float bits. Recursive entry refuses progress. Reset only discards the
continuation; it is **not** effect/payload deletion. The actual parent owns
allocation admission, cancellation, lifetime and release of partially initialized
records and held Main/queue work. A pool must not dispatch a partial constructor
as if its original synchronous call had returned.

## Main, lifetime and actual callback graph

Main copies root XYZ, decrements signed lifetime30 and increments word age34.
A now-negative lifetime requests whole effDelete8005ea30 and exits. Actual hard
delete frees heap3 payload before clearing record flags; a pending delete must
not decrement lifetime again. Otherwise original anim_data8041988c and
radius_data8041989c update root animation/child radius in source order, with the
original conversion biases802fb0b8/c0. Each loop condition reloads record+8;
each particle rereads the count-table byte. The source unsigned conversion,
binary32 subtraction/multiplication and source zero/one/factor words are retained.

Main next requests actual depth8001086c for its captured XYZ, then queues
`dispEntry(camera4,layer2,effHitDisp801c5d6c,effect,depth)`80010bc4. These are
execution acknowledgments, not tracing hooks. Each can hold; the completed
update and depth do not replay. The usual original kind0 life13 yields13 display
requests and deletion on invocation14, with progress255/256 on the final visible
invocation. Do not substitute a fade duration measured in renderer frames.

The focused test routes the **actual** `resumeEffectMain` update callback into
`resumeHitN64Main`, and routes deletion into actual `hardDeleteEffect` plus a
fixture heap owner. It verifies a reserved nonzero record is skipped, real record
context/callback/name/count/payload stores, held child boundaries, changed
argument/provider rejection, source signed edge angles, all four color branches,
unclamped count0/1, and complete13+1 lifetime. Its queue/depth are explicitly
fixture providers; it proves no native pixels or normal audience interruption.

## Primary source and data verification

`tools/hit_n64_source_evidence.py` reads the original US revision0 ISO read-only,
pins complete five-body hashes, actual small-data tables and conversion biases,
and decodes original texture74 privately. Startup80005418 bytes
`3c408042604274403da0804161adcf20` establish r2=80427440/r13=8041cf20. All absolute
addresses here use those primary bases; older semantic-inference r2 docs are not
used. Private source disassemblies/manifests live in artifacts/hit-n64-source/.

| Complete body | SHA256 |
| --- | --- |
| effHitEntry8013f1b4/20 | b3da6d977a1b38c8134696a5579c2e8dea750634e587c5ec83992f6bba273c78 |
| effHitN64Entry801c6344/270 | f3032cf8a104c1f1adb6194f71735381179aa86a6f84c2472907e9ff92b18d27 |
| effHitMain801c61bc/188 | 993d240615a64d5e8df4c31394396416d3659c7a6efefa1ced955646129cd95a |
| effHitDisp801c5d6c/450 | 3c0f065f58e9ae41835d7b79ded28c6ea2b6d43879346df11a452fa786ca4c86 |
| main_dl801c5b58/214 | 765affb2be6158fa08536cd3f8a77dfcab581389f722c59cf58834b151ce0316 |

Only Entry/Main are implemented by this batch. Display/main_dl hashes establish
concrete remaining source bodies, not their completion.

## Independently assignable display/GPU UI follow-up

Implement retained whole effHitDisp801c5d6c/450 plus main_dl801c5b58/214, keeping
original per-call camera/matrix, conversion, GX and geometry order. Display uses
actual camera4 pitch/view, source position/scale, original color/fire_data
branches, two TEV stages and three120-degree rotated passes. main_dl uploads
texture matrices30/33, reads original per-particle direction/random angle/radius,
loads actual camera4 view and submits the original five56-byte vertex blocks at
803a0458 (280bytes SHA256
8778157212c6fd114b2746aefd029272202764db44511a746d3d168530feef9b).
Its count and frame-index reads must retain signed source behavior; no invented
camera, normalized UV rectangle, triangle replacement or material success stub.

Texture provider is whole effGetTexObjN64801d78b0/60. Display requests index74
and loads that same original object into slots0 and1. Getter captures actual
state at global80419a40: state+4 nonzero selects actual palette word0 and original
texture-object helper80037a80; state+4 zero invokes the original1x1 I4 fallback
backed by actual source pixels80419a60. A native adapter may only use that
fallback for the genuine source branch, never fabricate zero state for missing
resources. Actual state/global/bank/file ownership remains required.

Original `e/us/effect_n64.tpl`: disc offset944965424,235776bytes,151 images,
SHA2563202ac8f437a2a7664d791ec5d9ee1988504f51b912c1211429708fac0a268ef.
Image74 headerf2c, tiled data110720,2048bytes; I4,32x128, no palette,
repeat S/T, linear min/mag, no mipmaps/bias/edge. Exact unscaled private decode
local-assets/hit-n64/texture74.png SHA256
c80e1cca9619ca95b6758b2d4778f2722b63705d1f512b97f919e09552be22be.
Do not reuse the unrelated shadow44 texture or clamp the repeat source sampler.
A dedicated32x128 native image is already power-of-two; a future provider must
retain actual import/generation and whole ordered GPU-batch admission, preserve
both texture slots and source TEV behavior, and retire queued consumers before
synchronized release while C3D is alive.

Original Nintendo/Intelligent Systems rights remain retained. Local extraction,
source hashes and original-pixel conversion grant no redistribution rights.
The ISO, source caches, decoded texture and compiled packs remain ignored/private;
no original payload is added to tracked history.

## Native frontier and validation

No Trio binding journal is supplied: there is presently no admitted native
process EffectPool init/context/Main/release plus HitN64 queued display/GPU owner.
Source Entry/Main portability and real-pool fixture tests do not permit the
normal audience event to acknowledge its ignored effect result prematurely.
Actual _attack_aud mode0 therefore retains its genuine effect frontier until
those owners exist. Root owns eventual original callback/heap/global binding,
normal scheduling, native visual proof and complete audience interruption.

Focused strict host, ASan/UBSan (leak scanning disabled), GNU ARMv6k syntax and
read-only original source verification pass. Full make test/make all and native
validation belong to root's separate frozen increment, not this focused claim.
