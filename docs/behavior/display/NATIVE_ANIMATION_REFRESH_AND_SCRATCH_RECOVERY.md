# Native animation refresh and scratch recovery

The retained animation buffer machine implements complete parent control flow
for animPoseBattleInit80041c58, animPoseRefresh80041c84, testAlloc80042ac8 and
initTestHeap80042e5c. Read-only original listings, byte hashes and static review
are private under artifacts/animation-startup-owner-evidence/. The
[pool contract](ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md) and
[typed admission](BATTLE_POSE_POOL_PROVISIONING_AND_TYPED_NATIVE_ADMISSION.md)
still apply. Static source recovery is distinct from original execution evidence.

Battle Init stores current W10c=1, then runs complete Refresh. Refresh conditionally
requests real heap0 allocation180000 when captured W104 is null; allocation
return publishes to freshly reloaded W104. It reloads W again, captures W104
and resets that W108 before scanning. Signed loop count and pool are live
reads through current80415dc0. Every nonzero pose flag word is active for this
scan. Nonzero work mode skips pose mode0; zero work mode skips mode1. Mode2 is
included in either context. This cannot be reduced to scanning field NPCs alone.

A selected pose retains its actual P address across children. Its first buffer
uses the loop-condition W capture; later buffers reload W. Each buffer reloads
P10, W0 resource table, resource+8 file, file+a0 holder, *holder MODEL, then its
count. In order: two12*MODELf0 positions, two12*MODELf8 normals, MODEL13c
visibility, two4*MODEL140 node values and24*MODEL12c auxiliary bytes. Source
multiplications wrap32. Every request invokes actual testAlloc. P58 and P68
are freshly reread before aliases P5c/P6c; cache P3c=-1 is written twice.
Read/store/child holds retain captures and do not replay completed prefixes.

Scratch allocation rounds size upward to32 with wrapped uint32 arithmetic,
captures old cursor, separately reloads cursor for reservation, then reloads
current W104/108 for unsigned NEWcursor < wrap(base+180000). Exact end fails;
zero and wrapped-to-zero sizes still follow source cursor/comparison behavior.
A failure first conditionally allocates scratch when the captured base is null,
resets cursor and performs the full selected-pose buffer rebuild inline. The
post-loop reservation uses that final captured W, without an invented root read.
Second failure runs actual initTestHeap and selected animPose_AllocBuffer(P),
then invalidates each cache once. Third failure calls Refresh and recursively
retries the original aligned request. Retained child cursors preserve all of
this recursion. Heap allocation remains a real provider; the complete
[pose-buffer allocator](NATIVE_POSE_BUFFER_ALLOCATION_AND_RECURSIVE_RECOVERY.md)
now runs as the actual retained child;
there is no ring overwrite, fake success or replacement file/model pointer.
Native stack admission holds beyond64 retained nested machine frames. That is
a port resource bound, not a measured original recursion limit or source return.
A nonterminating/unsupported original request never becomes completed allocation.

The existing battle sequence binding now runs this machine for battle_init's
animation child over the same caller VM and sound/backend services, and exposes
the deepest unavailable child. It never publishes a substitute W, forces a zero
pose count, or manufactures a loaded file. Missing W remains unavailable before
mode effects; missing heap or overflow rebuild remains pending at its real call.
All subsequent NPC release/context/party/lower BattleInit requirements remain.

Fresh native sessions now use the
[shared field/NPC/battle pose namespace](NATIVE_SHARED_FIELD_AND_BATTLE_POSES.md).
They still require genuine W, loaded model/file/holder and scratch/output
startup ownership before source-address refresh can run normally. The typed
evaluator reads the same cache3c word, but its separate native shape arrays
are not claimed to alias every published guest scratch range. Legacy colliding
handles cannot be adopted by copying records or declaring the pool empty.
Full cold animInit/tangent/file history remains required.

Validation fixtures deliberately separate branch tests from original-resource
binding. The latter uses actual registered NPC/pose CPU records and original
MODEL header counts with explicit native test guest file/holder views. Its
extra mode2 typed pose proves selected-mode refresh; it does not claim Mario/
partner namespace migration or authored encounter startup. Fixture W/count,
heap block, field-start words and dispatcher position remain test supplied.
See [progress](../../PROGRESS.md) and the [request inbox](../../requests/EVT-01.md).
