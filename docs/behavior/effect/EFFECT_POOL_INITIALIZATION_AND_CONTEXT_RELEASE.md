# Effect pool initialization,context release and coupled teardown requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effInit8005ef20/88,effAutoRelease8005ecd0/88,effInit64801d7a20/38.
Verification:`python3 tools/verify_effect_driver.py`. Read
[driver/cache](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md),
[record acquisition](EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md),
[hard/soft leaves](EFFECT_SOFT_AND_HARD_DELETION.md),
[SDK heap](../event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) and
[scene memory reset](../memory/MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md).
Static evidence only;no original reset/free/scene/pool-capacity execution trace.

## Manager storage and original512-record capacity

Initialized DOL pointer80415ee0 points to BSS manager8041e908,size18 hex=24
bytes. Its local fields are signed record count0,array pointer4,raw words8/c,
file-handle cache10 and cached selector14. Init explicitly assigns512 to count;
record stride40 gives request512*40=20480 bytes (20KiB) in SDK heap0. This
is original initial allocation provenance,not a New2DS chosen limit or proof
later count/array writes cannot alter the bank. Pool slots themselves are not
individually heap allocations;effect payloads typically allocate heap3 separately.

## effInit exact live-pointer/count/publication order

Perform these operations with distinct current-manager reloads:

1. M0=global manager;store M0.count0=512.
2. RELOAD M1;read its current count;compute wrap32(count*40). Request
   __memAlloc(heapIndex0,that byte count).
3. RELOAD M2 AFTER allocation;store returned array pointer into M2.array4.
4. RELOAD M3;read its current count FIRST,then current array4. Request
   memset(current array,0,wrap32(current count*40)). This clear uses the LIVE
   array pointer,not a retained allocation result as event Init does.
5. RELOAD M4;store cache10=0. RELOAD M5;store cached selector14=ffffffff.
6. RELOAD M6;store raw word8=0. RELOAD M7;store raw wordc=0.
7. Call effInit64 and return with no scalar result contract.

No allocation-null/count/extent guard,reuse of old bank,old-array free,fileFree
of discarded cache,per-record payload cleanup,update callback,pose release or
save operation. Repeated Init can abandon earlier storage/handles if source
owners did not release them. It does not perform AutoRelease before memset.
The allocation wrapper's own zero/cache behavior and Init's subsequent memset
are separate services. Allocation0 still publishes0 and requests a positive
clear under ordinary live count512;native provider validity must be explicit.

Manager/base/count can change across the allocation/memset callbacks. Do not
collapse all globals into one captured semantic manager or assert fresh bank
is the array cleared. Record bulk zeroing initializes names/flags/pointers but
is not the partial record acquisition contract used during ordinary slot reuse.
Raw manager8/c meanings remain unresolved beyond explicit reset assignment.

## effInit64 clears its independently selected eight-byte state

Read pointer at80419a40 (r13-34e0),request memset(pointer,0,8). RELOAD pointer
at80419a40 after that call;store WORD(pointer+4)=0 AGAIN. Initialized pointer
word is8041ecd0. No allocation/free/type/resource load/input-argument read or
per-effect callback. The second pointer can differ from the first under a
mutating provider. Names/default BSS zero do not add an undocumented N64 engine
schema;this closes its used reset leaf,not the library's texture/draw behavior.

## effAutoRelease(contextWord):active-bit/context admission,hard free inline

Retain full supplied word S. Capture manager,current signed count0 and array4
once. Scan indices0..<captured count in ascending40-byte steps. For each E:

1. Read WORD F=E.flags0;require numeric active01. Missing01 skips immediately,
   even when F is nonzero and therefore name-visible/occupied elsewhere.
2. Read WORD E.context4;require exact equality to S. No bool normalization,
   current GP14 query or replacement of every nonzero S by1.
3. The code checks the same captured E nonnull and F nonzero again;there is
   no flags reread. Read payload E.c;request __memFree(heapIndex3,payload).
4. Store E.flags0=0 AFTER free,then continue scan.

No payload-null guard or scalar result contract. It does NOT call effSoftDelete,
effDelete,E10 update callback or an effect-specific finalizer. Mask02/04 has no
special meaning here once active01/context admits the record. Only flags0 is
cleared;name/resource/callback/payload/count/context remain stale. No pose release,
map08/colour restore,display queue invalidation,owner-event/audio cancellation,
file-cache free,record-bank free or manager count reset occurs in this body.

Thus bulk context release can bypass Majinai Main's pose/map cleanup. Source
teardown must own the separate animation/map/display services. A native
per-effect destructor that automatically runs all those actions is not proven
by this routine. Record callbacks/raw writes can alter later slots;count/array
are captured,fields are read live as reached. Invalid cycles/context storage,
concurrency and retired-array domains remain separate platform/source policy.

## Four exact direct DOL source bindings

| Target | Site | Containing caller / scoped argument |
| --- | --- | --- |
| effMain8005ea7c | 8006f9a4 | marioStMain,ordinary update request32 |
| effInit8005ef20 | 8006fdf4 | marioStInit,boot initialization request |
| effAutoRelease8005ecd0 | 80007b30 | _unload,supplied context0 |
| effAutoRelease8005ecd0 | 80073770 | battle_exit,supplied context1 |

Verifier recomputes the four-site set in seven DOL text sections. REL/indirect
calls and other raw reset/source writers are excluded. Init caller body/source
boot ownership is not newly completed merely by the direct site.

The two release caller regions preserve this exact local sequence:
_unload calls AutoRelease(0) at80007b30,then animPoseAutoRelease(0) at80007b38,
then evtmgrReInit at80007b3c. battle_exit calls AutoRelease(1) at80073770,then
animPoseAutoRelease(1) at80073778,then evtmgrReInit at8007377c. These are
separate effect-payload,animation-pose and event-array services,not one implicit
object destructor. Complete caller teardown/scene/GP changes,animation release
body and downstream map/heap ownership remain linked/open requirements.

## Reviewable implementation checks and remaining scope

Check512 store before live count allocation,heap0/20KiB ordinary request,array
publication after allocation,live count-before-array clear,multiple manager
captures,cache/selector8/c reset order,allocation0 requests,no old-state cleanup,
N64 eight-byte clear plus fresh pointer/word4 clear. Release fixtures should cover
all flag01/02/04 combinations,context0/1/2/-1 exact words,null payload requests,
free-before-flags0,all stale metadata,live later slots versus captured count/
array and no pose/map/callback/display/event cleanup. Caller fixtures need exact
0/1 arguments and effect→animation→event request order.

Driver/pool/helper local contracts are delivered. Actual animation/file/TPL/
texture/display/memory/context/scene writers,field/battle transition ordering,
source capacities/invalid domains,cache/failure policy and original runtime/
resource/frame traces remain EVT-01/JUMP-01/DMG-02 requirements. No full-game
memory/rendering requirement or native fidelity verdict follows from this bank.
