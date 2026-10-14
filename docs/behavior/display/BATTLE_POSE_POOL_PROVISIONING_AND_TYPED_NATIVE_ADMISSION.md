# Battle pose pool provisioning and typed native admission

2026-10-14 US G8ME01 disc0 revision0. Read-only original ISO/DOL and existing
converted animation-owner interfaces. Minimal real owner closure for battle
LocalTimeRate8004142c; no native code/shared docs/index edits or commit.
Read the [forced part consumer](../battle/FORCED_PART_POSE_DISPLAY_RATE_AND_ANIMATION_FRONTIER.md),
[original Entry admission](../partner/FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md),
[pool initializer](ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md),
[selection/restart](ANIMATION_SELECTOR_AND_RESTART.md), and
[Main/time](ANIMATION_MAIN_ENTRY_AND_TIME.md).

## Corrected fresh allocation state is source-proven zero

Mutable work pointer80415dc0 initially contains803d9470, a BSS work owner,
not serialized model data. animInit80042854/274 initializes resource capacity
64, texture capacity64, pose capacity256; allocates through __memAlloc heap0
in this order: resource64*10hex, texture64*0c, pose256*170hex, then auxiliary
1024*0c. It publishes each returned pointer through freshly read work fields
W0,W8,W10,Wec. Other work/tangent/archive/file requests remain the linked
full-init contract; there is no narrow-only wholeInit success claim.

**New transitive closure:** __memAlloc800300f0/64 reads SDK heap handle by
supplied heap index from8041e844 array, captures requested byte size, calls
OSAllocFromHeap802964ac. For nonnull result it explicitly calls
memset(pointer,0,requestedSize) at80030128, then DCFlushRange80296a54 with
that same pointer/requested size, and returns the retained pointer. Null skips
zero/flush and returnsnull. Thus successful fresh pools have zero flags and
zero reference fields. This resolves the older pool document's unestablished
allocation-zeroing boundary. It does not justify zeroing an already live pool.

Source animation manager is global: field Mario/partner/NPC poses may already
occupy the pool before battle entry. Original first-free result0 is proven only
for a truly fresh zero pool, not at every battle. A scoped native battle owner
can declare its own handle projection, but it must select from its actual
retained registry and own real slots/resources; it cannot assert original
global handle numbers without the field/global allocation history.

## Entry: first free pose, resource references, mode1 prerequisites

animPoseEntry8004176c/4ec captures requested resource string and mode. Mode1
first calls fileSetCurrentArchiveType80189d38(2); mode0 chooses1, mode2 chooses0.
Other mode values make no local archive setter call. Mode is not a pool index.

Pose scan uses current W10 pool/W14 count and returns first ascending record
whose whole WORDflags0==0. It immediately ORs active1 and stores words4/8=0.
Pool exhaustion chooses index-1/pointer0, then dereferences that pointer: not
graceful negative allocation admission. Valid native supported domain must
have a real free slot; explicit native capacity failure is an adapter boundary.

Scan64 resource records with stride10hex. Active resource flags0 nonzero:
resolve model payload through fileHandle+ a0 and compare exact payload header
name at data+4 against requested name. Match increments WORDreference4 and
reuses its resource index. Otherwise first resource flags0 record is selected,
madeactive1, refcount incremented, and fileAllocf(type5,original prefix/name
format) is requested. Null modelhandle or null loaded data clears resource
active, decrementsrefcount, marks index-2; data-null branch first fileFree.
Publish index→pose10; index-2 clears poseflags and returns-2. Unit caller does
not turn that into a safe pose-1 sentinel; retain resource failure explicitly.

New successful resource derives source texture filename from model+44, uses
exact name reuse/reference increments in64 texture records stride0c, otherwise
first free record/fileAlloc(type4). Resource/texture exhaustion has no unchecked
pointer repair. Reused resources retain real texture/model handles and counts.
The decoded loaded layout differs from source serialized relative offsets.

Successful pose stores animation index14=0, modec=inputmode. Mode1 (all nonzero
modes locally) reads current GP38/3c and originalhardware quotient to publish
timestampLOW1c thenHIGH18. Mode0 usesGP40/44 instead. It sets cache3c=-1,
FLOAT40/84=0, WORD80=0, FLOAT70/74/7c=0, linked90=-1, then calls
animPose_AllocBuffer80041ee0 with POSEPOINTER, not integerhandle.

After buffer initialization: wordsE0/E4/E8/EC=0; colorwordsF0/F4=ffffffff;
FLOATFC/F8=1; FLOAT100=0; FLOAT44 localrate=1; matrix124identity;
model-specific sort/owner fields11c/120; WORD154/158=0. The buffer helper owns
real output vectors/node/texture/shape/cache workspace; do not replace it with
an arbitrary positive handle that has no resource/evaluation storage.

## GetPtr and rate setter minimal exact ABI

animPoseGetAnimPosePtr80039cf8/14:
current W through80415dc0, current pool W10, return pool+wrapping32(ID*170).
No local active/null/count/index check. SetLocalTimeRate8004142c/18 uses the
same address calculation and stores supplied FLOATf1→record44. No timestamp,
animation index, linked propagation, evaluation or per-frame increment.
These leaves can be implemented over a real typed pose registry without
pretending the full original GX/file/deformation backend has executed.

The typed native provider must own at least active allocation, source model
identity/mode/archive, real converted resource lifetime, selected source
animation/default0 metadata, timestamps, localrate44 and linked90 sentinel,
plus CPU shape/evaluation workspace. Check original caller validity through
actual registered handles; do not map an unregistered raw0 to whichever scene
actor happens to be first. Failure remains an explicit pending factory service.

## Existing ActorAnimationPack can supply real geometry, with bounded mapping

include/display/actor_animation.hpp owns actual converted original poses, positions,
normals, nodes, texture/visibility/surface updates and ActorAnimationSample
workspace; each ActorAnimationPose::evaluate validates/evaluates supported
ordinary animation data. The typed field owner include/field/field_party.hpp already
holds shared original packs, allocated CPU shape vectors and a registry.
Its current enterPose supports mode2 only, fixed field resource keys, and5
projected slots. It is a reuse pattern, not source mode1/256-slot battle Entry.
Its field handles must not silently alias a separately invented battle pool.

Converted packs are selected subsets in converter order, not the complete
original descriptor directory. Read-only original local model verification:
`a/b_mario`: headernameb_mario,85 descriptors, descriptor0 **M_Z_1**;
`a/c_pkr`: headernamec_pkr,32 descriptors, descriptor0 **PKR_Z_1**.
Current trio converter starts its selected lists with M_S_1/PKR_S_1 and does
not include these Z1 entries. ActorAnimationPack::firstPose therefore cannot
implement source Entry's index0 or SetAnim unknown-name fallback. Bind actual
source descriptor order/default metadata and those original assets, or retain
that specific selector/resource frontier rather than inventing idle fallback.
Exact supported requested names can still map to the existing validated pack.

## Forced selection and Main clock dependencies stay distinct

Source forced-part pending consumer chooses force1 from its whole resulting
flags word. animPoseSetAnim80041170 first resolves the retained resource and
source directory. Negative selector→strcmp ascending name slots64bytes;
missing name→descriptor0. Nonnegative selector below count→that index,
otherwise0. Force0/same selected index returns unchanged; force1 stores index,
requests source clock quotient, publishes LOW1c thenHIGH18, resets3c=-1,
FLOAT40/84=0, WORD80=0. It does not call Main or change rate44.

Main8003ed2c captures current pool/resource/index and descriptor before any
animation-null gate, establishes GQR load semantics, captures output pointers,
and processes admitted linked recursion. Source caller needs a real loaded
descriptor/timeline, not a fabricated null timeline to avoid evaluation.
For a real unlinked record90=-1 no child recursion occurs. With timeline:
read GP clock branch from recordc, subtract timestamp modulo64bits, unsigned
convert tofloat, single multiply60, divide1000→record20; then reload20,
multiply localrate44→record20. This means rate is applied to elapsed time,
not accumulated as rate-per-call. Changing rate can jump the evaluated time;
there is no local rebase to preserve an accumulated animation phase.

Source loop/start/end and timeline/cache/node evaluation then publish selected
time24 and fraction84. Original Main can be called repeatedly in one simulation
tick; it has no once-per-frame guard. A native monotonic simulation clock and
CPU converted evaluator are explicit platform/numeric adapters. Rendering
frequency must not invent additional elapsed time. Preserve source current
clock/mode/rate/restart/cache state while mapping the actual visible geometry.
Detailed dependency fields and rounding are the linked Main/time authority;
camera-facing tail/GX quantization/fullresource lifetime remain separate.

## Evidence and validation

Ignored complete listings battle-pose-owner-* under artifacts/recovered-temp-evidence/.

| Body/address/size(hex) | SHA256 |
| --- | --- |
| animInit80042854/274 | 08704a43980c73cabe9050bcea810daf766548b2117112da055ece626916c203 |
| __memAlloc800300f0/64 | 51d2010e5ae0c3265971a8dfaee60b24489d8bf8eb2ca386f77c1d0ceb98712b |
| Entry8004176c/4ec | 8d239cdb8bb0faf92f022b6f8289911fe399c160d7b769ed6cb39458a8da43c9 |
| GetPtr80039cf8/14 | 52d4ae8f83adb0da10b5a1fdda6d55dc0de62726c5af4ed01d7456c4ceeab90e |

Read-only source/raw local model/native interface inspection and scoped diff
check. No code/shared edits, native test/build/emulator or full animation-engine
equivalence claim.
