# Original NPC shadow, blur and separate-pose suffix

The retained controller implements npcMain8004aeac..8004b10c, composed after
[the display prefix](NATIVE_NPC_MAIN_DISPLAY_PREFIX.md). It accepts only that
prefix's Shadow/SeparatePose/History frontier and retained depth. A Pending
prefix cannot start it. Completion reaches late History only; actor traversal,
final state clear and field tail remain unfinished. It does not activate normal
Area Main or dispatch callbacks. The [whole parent map](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md)
and [child ABI](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md) remain authoritative.

## Ordinary shadow

Capture fresh mainflags once.04000000 selects Vivian even with40000; otherwise
40000 skips ordinary shadow to blur. Load descriptor28 for a null test, copy
current rootXYZ, then for nonnull descriptor load fresh yaw144, request
reviseAngle, load fresh180 and select fresh90/270 for ordered angle<180. NaN
selects270. Request sinfd, THEN reload descriptor28; the original pointer is
not reused. Offset X is single(offset30*scaleE0) then single fused with retained
sine and copied rootX. Y and Z use single fused offset34*scaleE4+copiedY and
offset38*scaleE8+copiedZ. Root changes during children do not replace this copy.

Radius follows the exact single graph: divide width14c by original3, multiply
scaleE0, multiply factor154, multiply original1.3300000429. Request shadowEntry
with ONLY four float arguments XYZ/radius. Its returned INDEX is retained,
including valid0. Fresh mainflags80000 requests shadowSetCollision(index,0).
This clears record half flags0 bit1; it does not write collision half18.

## Vivian shadow

Copy rootX/Y, load zero, one and100, then copy fresh rootZ and load-1. Request
hitCheckFilter(filter0, origin=(copiedX,single(1+copiedY),copiedZ), direction=
(zero,-1,zero)). Seven semantic output slots preserve r4..r10 identities:
pointX/Y/Z,length,normalX/Y/Z. Only length is initialized100. Other incoming
scratch is uninitialized; no fabricated guest stack pointers or zero-filled
outputs. A nonzero child result consumes ONLY written pointY into copiedY.
A null result preserves copiedY. An invalid completed hit without Y stays held
without replaying the completed child. Real query/filter/geometry ownership
remains required; explicit fixtures are not an implementation of that backend.

Fresh handle104 and the COPIED XYZ feed animPoseVivianMain. The native leaf
admits the actual same AnimationPoolMemory row, preflights a complete writable
RAM span, ORs20 into pose word0, then writes strided translations130/140/150.
No active/resource gate, animation evaluation or fake frame advancement is added.
The public contiguousRam predicate exposes no pointer and is rechecked per call.
Radius omits1.33: single(factor154*single(single(width/3)*scaleE0)). ShadowEntry
uses FRESH actorXYZ after the pose child, not the copied hit point. Fresh flags
again decide collision disable. Source sinfd is a request, not a hardcoded +/-1:
its original scalar graph is single(single(TAU*degrees)/360), double sin, rounded single.
Original libm/FPSCR numeric parity and real native math providers remain open.

## Blur, separate pose and holds

Both shadow routes and ordinary suppression join a fresh mainflags100 test.
If set, reload camera1f4 and register class2 callback8004bc3c with actual actor
and retained depth. SeparatePose prefix bypasses shadows and blur entirely.
At b080, fresh110==-1 goes directly to History; otherwise fresh alpha selects
xlu8004c234 class2 or opaque8004bf28 classes0/1/2. All separate registrations use
literal camera1. They retain branch, callback, actor and depth through holds.
No alpha/handle reload changes an already chosen opaque sequence.

Every false/empty primitive has no effects; successful reads/calls publish once.
Actor, incoming frontier, depth BITS (including NaN) and service owner remain
bound. Camera, shadow pool, queue generation and borrowed lifetimes must remain
in the same producer pass through holds and deferred dispatch. Native queue
NaN rejection is acknowledged without records; unsupported pool/queue capacity
holds. Source itself has no native64-slot capacity check. Tests use the actual
ShadowWork and DisplayQueue owners, not per-actor fake shadow records. Game's
same shared frame shadow owner and actual collision/math/callback providers
still need binding before ordinary Area activation.

Validation includes an independent ordinary trace, every primitive hold across
all source branches, fresh descriptor/root/flag mutations, valid index0,
collision flags, hit/miss/incomplete outputs, suppressed shadows with blur,
literal alternate camera, NaN depth and owner identity. A cancellation oracle
checks fused offset result2^-46 (bits28800000); separate float multiply/add
would yield0. Shared VM fixtures exercise inactive pose255, collision-to-pose
Y=-5, fresh shadow Y=0, held blur, actual pool/queue and non-replayed external
pose Y=42. Parent phase60/MainCount1 remain unchanged after suffix completion.

Static evidence is in ignored artifacts/npc-main-parent-evidence/shadow-suffix-
review-notes.txt and *-shadow-review listings. Helper hashes: Vivian80039824/38hex
3a1fa87ba9e70f529ae5f9ec36fe4df56e70582f015752fdb61ec83730085dac;
SetCollision8002bcfc/34hex
f7e40f99259af6d2823f29a3dc3df6950a4994dd2e2eba9a4cc1fe2bb10c0836;
sinfd80125cd4/34hex
2438796048394f29f154dc21c9a0620236c7dec53740e525c529cd659f80777f;
ShadowEntry8002be98/echex
06cfea58a6edbb4341716d99601d96c02f7c40ac181db95ae8d5363b5d418386;
hitCheckFilter8001577c/128hex
2449ccf33696b3a35d43431dc373741cc46c2836adf11e5cf8d4a4174388e10a.
These are static source observations; no Dolphin execution or full field visual
performance acceptance is claimed.
