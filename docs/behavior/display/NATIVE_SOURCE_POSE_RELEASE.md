# Native original source pose retirement

US G8ME01 revision0. `releaseAnimationPose` implements the complete local
animPoseRelease8003b2d0/194 graph, SHA256
32d91c22e37a382d32a667e138d66e29f3d42fb2287b963bdc390154457863b6.
See [source release/cache order](BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md),
[fileFree](../file/FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md) and
[party destruction](../partner/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md).
Original listings/provenance are retained privately under
`artifacts/animation-field-release-evidence/`; no executable bytes are committed.

## Retained complete local graph

Capture current W and pose array, then pose = array + wrap32(ID*0x170). Source
has no ID/active/count validation. Captured pose remains the same across children
and subsequent W reloads. Nonzero pose4 decrements pose8, stores and RELOADS it;
signed nonnegative returns without retiring anything. Negative skips smartFree
and enters the resource suffix. Ordinary pose4==0 tests current flags40, calls
smartFree(pose15c) if set, then rereads flags80 and calls smartFree(pose164).

Both retirement branches then reload W, pose10 and that W's model table in
source order. Capture model, decrement/store/reload model.ref4; signed positive
skips teardown. Otherwise clear model.flags0 FIRST, load captured model.c,
reload W and its texture table, and capture texture. Decrement/store/reload
texture.ref4; signed nonpositive calls fileFree(current texture.file8), THEN
clears texture.flags0. Both texture branches read captured model.file8 freshly
after that child/clear and call fileFree. Pose.flags0 clears last. Other raw
pose/cache words, pointers and file handles remain stale source bytes.

Every acknowledged word read, store and child is retained. False/empty primitive
results have no effects; the parent can already have published effects before a
hold. A captured child argument does not change because a sibling edits the
original source lane. Keep the same ID and provider functions/context across
holds; changed requests reject without progress. No extra per-frame counter,
refcount clamp or speculative rollback is introduced. Native services must keep
the same live owners and resume the retained invocation.

## Actual native factory and file binding

`NativeFieldAnimation` installs the production release callback in its SAME
`BattlePosePool` as source Entry/Main. `FieldPosePool::release` and production
Trio runtime destruction already forward to that pool. No copied pose namespace
or typed-only refcount substitution is used. New Entry is unavailable while a
release is pending, and an incomplete Entry cannot be interrupted by Release.

Initial native admission requires a live handle/source-buffer lifetime and
matching current pool/cache metadata. Capture its native generation once.
After model.flags0 clears, typed `pose()`/`cacheMetadataReady()` can legitimately
fail while the source parent is still pending; retries therefore use retained
raw addresses and the initially admitted lifetime rather than rerunning those
typed checks. The real file child calls `NativeFileCache::release`, which runs
original fileFree against actual shared BE file entries. FileFree neither frees
payloads nor runs garbage collection. Source file/array owners remain stable.

Only completed pose.flags0 clearing retires native source-buffer/resource/
shape/cache sidecars. Inactive MODEL/TPL rows drop their native immutable-resource
references then; live shared rows remain. No other source fields are cleared and
the source scratch arena is not individually freed. Borrowed early completion
preserves sidecars. Independent escaped readers reject a retired/reused handle
through current pose/generation identity. Detached pool copies do not inherit
production callbacks. Direct FieldPartyPose scalar proxies still require their
owner to retire the wrapper before reusing the underlying handle.

Real smart-header ownership remains unavailable in this native factory: a true
smartFree request stays pending at 8002f878, including its captured argument.
Changing flags after that capture cannot bypass it. The generic complete parent
supports such children through its service interface. No successful smartFree,
collector or complete linked/effect lifetime is inferred from ordinary tests.
GPU texture residency remains the renderer's separate synchronized ownership;
fileFree does not delete those resources.

## Native enemy resource gap closed

The production source file catalog now stages unchanged `a/c_kuribo` type5 MODEL
and `a/c_kuribo-` type4 TPL from the already accepted full original 45-descriptor
Trio enemy pack. Previously the shared source factory could not admit those
new battle resources even though detached CPU pools could. No original source
asset or conversion policy is changed. Eight battle poses now enter the actual
source allocator with three ordinary Mario owners already resident; all eight
retire through the production destructor, including shared b_mario and c_kuribo
model references. Ordinary field Mario and its shared texture references survive.
This fixture does not run the whole authored field/battle/result story.

## Evidence and limits

Tests pin the full ordinary read/store/call trace and reject each primitive once
across ordinary, shared, signed-wrap, borrowed and child-mutating cases. They
check fresh counter/flag/root/file reads, captured arguments and request-owner
pinning. Actual source fixtures cover c_pkr double-entry, last-file hold after
model.flags clears, recovery without repeated decrements,40 warm Entry/sample/
Release cycles, original relocation preservation, distinct MODEL/shared TPL,
retired reader rejection, detached copies, real missing smartFree, and production
Trio creation/destruction with retained field owners. Source hash evidence and
review are static/host/native checks, not original Dolphin execution traces.

Full validation and remaining work are in [progress](../../PROGRESS.md).
Party Kill/Kill2, auxiliary-kind13 construction and authored Goombella conversion,
NPC room deletion/reset, broader linked/smart/collector owners, original frame/
camera/draw/event scheduling and physical New 2DS XL verification remain separate.
No story/dialogue writes or early canonical join are added by this work.
