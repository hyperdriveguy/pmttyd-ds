# Complete five-pool bootstrap,bulk reset and primary map-root construction

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete memInit80030218/2a4,memClear80030154/c4,OSInitAlloc80296634/70,
OSCreateHeap802966a4/6c,OSDestroyHeap80296710/24 and three arena access leaves.
Called allocation/fill/cache bodies link to
[SDK allocation](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) and
[map allocation](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md).
Verify:`python3 tools/verify_event_heap_release_and_bootstrap.py`.
Static original DOL evidence;no arena-address/heap-capacity/runtime measurement.

## Tables and initialized five-record configuration

r13=8041cf20. Tables hold FIVE32-bit words each:

| Address | SDA offset | Meaning in these bodies |
| --- | --- | --- |
| 804151b8 | -7d68 | Five {mode,value} pairs,eight bytes each |
| 8041e830 | 1910 | Pool start words |
| 8041e81c | 18fc | Pool end words |
| 8041e844 | 1924 | Stored SDK heap handles |
| 8041e808 | 18e8 | Primary map root |
| 8041e818 | 18f8 | Requested SDK payload length for primary map region |

Initialized DOL configuration is independently pinned:

| Pool index | Mode | Value | Normal initial reservation |
| --- | --- | --- | --- |
| 0 | 1 | 7300 | Fixed7300KiB;event context arrays allocate here |
| 1 | 1 | 3472 | Fixed3472KiB;later converted into primary map region |
| 2 | 1 | 224 | Fixed224KiB |
| 3 | 1 | 128 | Fixed128KiB |
| 4 | 0 | 100 |100% of the remainder AFTER all fixed reservations |

These are original static resource choices,not New2DS memory requirements or
proof configuration never changes. Arena boundaries and bootstrap platform
reservations are runtime inputs. No repository source asset/extracted ISO is
needed in history to implement these policies;original data reuse remains the
project assumption. Port memory sizes/addresses must be explicit platform policy.

## Complete SDK arena/bootstrap helper behavior

OSGetArenaLo reads word8041e430=r13+1510;OSGetArenaHi reads8041f080=r13+2160.
OSSetArenaLo stores supplied word at8041e430,with no extra alignment/clamp.
They do not reserve or clear memory by themselves.

OSInitAlloc(start,end,count):store descriptor BASE=start to8041f070(r13+2150)
and descriptor COUNT=count to8041f074(r13+2154). Capture metadata bytes wrap32(ORIGINAL count*12) before these stores.
Loop signed indices0..<LIVE global count,with base reload on EACH iteration;
each12-byte descriptor receives size=-1,allocatedHead0,freeHead0 in that write
order. After the loop RELOAD global base and return usableStart=
wrap32(reloadedBase+retainedMetadataBytes+31)&0xffffffe0. Store aligned end=end&~31 to8041f07c
(r13+215c);store current-heap word8041e428(r13+1508)=-1;store usableStart to
8041f078(r13+2158). No null/count/extent/multiplication check. Nonpositive count
skips descriptor loop but still performs arithmetic/global writes.

OSCreateHeap(start,end):round start UP32 using word+31/mask;round end DOWN32.
Read descriptor count and base;scan ascending descriptors while signed count>0.
First descriptor with SIGNED size<0 is available. Store descriptor.size=end-start;
write free-node header at rounded start:previous0,next0,then RELOAD
descriptor.size and write that to node.size;
store descriptor.freeHead=start,allocatedHead0;return descriptor index. No extent/
minimum-header/overlap/range check. No available descriptor returns-1. A negative
created extent can still mark that descriptor available for a later scan.

OSDestroyHeap(handle):base+wrap32(handle*12);store descriptor.size=-1,
allocatedHead0,freeHead0,in order. No block-by-block free,payload clear,callbacks,
coalescing or handle validation. Stored game handle-table entries are NOT cleared
by this SDK leaf. Allocation/free of individual blocks use separate contracts.

## memInit complete region assignment and two creation passes

Read arena low L and high H in that order. Request OSInitAlloc(L,H,5);retain
its returned usable lower boundary P;OSSetArenaLo(P). Round P UP32 again and
H DOWN32. Then FIRST pass through the five config records ascending:
for exact mode1,store start[i]=P,compute span=wrap32(value<<10),store end[i]=
wrap32(P+span),advance P=end[i]. Other modes do not assign their tables here.
No fixed-reservation total-versus-H check.

Compute R=wrap32(alignedH-P) ONCE after all fixed-mode reservations. SECOND
pass through records:for exact mode0,form wrapping64-bit product
U64(R)*S64(S32(value)),divide UNSIGNED64 by100 using __div2u802649ec,retain ONLY
quotient low32 and round that word DOWN32. Store start[i]=current P,end[i]=P+span;
advance P. Each percentage uses SAME R,not the shrinking remaining amount.
Negative/out-of-range percentages are not rejected;do not silently replace
this with signed division or a clamped host percentage allocator. Modes other
than0/1 leave corresponding start/end values unassigned by both passes.
[Unsigned division](FORMAT_UNSIGNED64_DIVISION.md) supplies the numeric leaf;
valid original configuration/domain restrictions remain separate.

Then call OSCreateHeap(start[i],end[i]) for ALL five slots ascending;store every
returned handle,including-1,to handle[i]. Set ArenaLo(alignedH),reserving the
whole considered arena from subsequent arena-low allocation. No caller error
branch or actual ArenaHi update.

Then loop ALL five indices AGAIN:DestroyHeap(stored handle[i]);CreateHeap(same
start/end);IGNORE the second CreateHeap return. For i==1,add primary-map-region
construction below. The second creation normally reuses the sole freed SDK
slot under valid five-descriptor state;the game still retains its original
stored handles without checking this identity. Other state/failure/reentrancy
must not be normalized into a hidden corrected-handle write.

## Primary map region inside SDK pool1

Both memInit's index1 second-pass branch and memClear(1) perform:

1. Load pool1 start,then end,then stored handle1 BEFORE any length store.
   Compute requested payload M=wrap32(end-start-32);store M to8041e818;
   request OSAllocFromHeap(the captured handle1,M).
2. If pointer!=0,memset(pointer,0,M),then DCFlushRange(pointer,M).
3. Reload M;publish pointer to primary root8041e808;write root.next0,
   root.size=wrap32(M-32),root.used16=0. Header writes happen EVEN on null result.

For normal aligned pool1 span3472KiB,SDK total requirement is the full pool
span;SDK header32 precedes returned payload. Map root lives at that payload,
with initial free size=poolSpan-64. _mapAlloc must also leave a32-byte remainder
header,so its largest single rounded payload is at most poolSpan-96. This is
local layout arithmetic,not a proof all original scene allocations fit.
No battle-root pointer reset,old SDK/map payload release or event-pointer
invalidation callback is supplied by this construction branch.

## memClear(index) complete bulk-reset contract

Capture index and offset=index<<2. DestroyHeap(handle[index]);CreateHeap(start[
index],end[index]);IGNORE returned replacement handle. For exact index1,rebuild
primary map region using the sequence above;for other indices,return after
heap reconstruction. No index bounds/type checks,configuration recomputation,
array/event-entry teardown,per-allocation callbacks or global event count reset.
Old pointers into the reset pool become a SOURCE ownership issue;the reset
itself does not find aliases or mark event E154/E158 invalid.

Direct _unload sites80007bf4 and7bfc request memClear(1),then memClear(2),after
its earlier event ReInit7b3c and other teardown requests. Battle_init/battle_exit
use explicit nested-pool allocation/free rather than calling memClear locally.
Full scene/REL/indirect ownership and event-user-pointer lifetime remain open.

Acceptance:five exact config records,fixed-before-percent assignment,same R
for percentage records,low quotient/down32,invalid-mode/no error recovery,
all five first handles then destroy/recreate with ignored replacement;arena
reservation,SDK descriptor count/create alignment/negative-size first-free,
primary map header sizing/null path,index1 versus other memClear and two unload
indices. Platform arena/heap/alias/size/handle/scene/source domains and original
memory/reset traces remain requirements;whole-game memory requirements are not
closed by these local policies.


[Live read/store-order addendum](SDK_HEAP_BOOTSTRAP_LIVE_READ_ORDER.md) supplies
exact InitAlloc loop/final reloads,CreateHeap post-header size read and both
primary-map start→end→handle captures. Its alias/observation acceptance cases
supersede any earlier summary implying snapshot count/base or handle-after-length.
