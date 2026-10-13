# Battle ending allocation: original wrapper, arena and header composition

2026-10-13 read-only US used-source closure for ending05's allocation.
Original call802154dc loads684/2ac bytes;802154e0 branches to
**BattleAlloc800f8934/28**, not800f88d8. Original result stores Bf28 then
calls btlseqEnd_DispInit802151b4. Small wrapper and complete _mapAlloc8002ff58/f0
listings inspected; ignored evidence `artifacts/recovered-temp-evidence/map-*`.
No native changes/commit. Reuse the existing complete source authorities:
[map allocation/fill/cache](../event/EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md),
[battle subpool](MAP_BUFFER_FREE_AND_BATTLE_POOL.md),
[five-pool bootstrap](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md),
[capture order](SDK_HEAP_BOOTSTRAP_LIVE_READ_ORDER.md).

## Wrapper and full valid-input allocation algorithm

BattleAlloc takes size r3, moves it into r4; loads root WORD8041e80c
(r13+18ec), then calls _mapAlloc(root,size). Return is the actual allocator
pointer, including0. It does not call __memAlloc(heapID0), create a12-byte
SDK-adapter header, clear Bf28 itself or infer a new object constructor.

_mapAlloc rounds `P=wrap32(size+31)&ffffffe0`. P0 returns0 before rootread.
Otherwise required span T=wrap32(P+32). No root-null check. Scan headers via
word0 next link in list order. Header0=next,4=size,8 unsigned16 used; header
span32. Used!=0 skips. Free size<unsignedT skips. Select candidate iff first
(bestSize0) OR unsignedbestSize>candidateSize. Equal-size ties keep the FIRST
selected header. Capture candidate next at selection. No candidate returns0
without mutation/fill/flush.

Always split selectedH, even when remainder is0:

1. H0=H+T; A=H+32.
2. H4=P, U16H8=1.
3. NewheaderN=H+T: N0=capturedOldNext, N4=oldSize-T, U16N8=0.
4. memset(A,0,P), then DCFlushRange80296a54(A,P), returnA.

Other header bytes are retained. Allocation failure/overflow/invalid list/
cycle/address/alias domains are not repaired by this local body. A free block
that fits P but lacks room for its mandatory32-byte new header is not admitted.
The allocated header size excludes its own header; its free predecessor size
before selection follows the existing root-size convention. Do not merge this
algorithm with the SDK allocator's32-byte TOTAL-cell convention.

For E2ac: P2c0=704,T2e0=736. **All704 payload bytes are zeroed/flushed** before
source ending initialization; don't clear only684. Minimum admitted free size
736. On a fresh inner rootR with size63fe0: returnR+20, occupied size2c0;
tailR+2e0 has next0,size63d00,used0. Later BattleFree using THIS inner root
restores root's free size63fe0/next0/used0 through coalescing. Source later
allocations/fragmentation can choose different addresses; this worked example
is not a claim E is the first original battle allocation.

## Actual nested400KiB battle arena

N_battleMapAlloc80030078/4c stores length64000=409600 to8041e814, calls
_mapAlloc(primaryRoot8041e808,64000), reloads storedlength AFTERreturn,
stores returned payload/root to8041e80c. Write root.next0,root.size=length-32
(63fe0),root.usedU16=0. There is no returned-null guard before those header
stores or prior battle-pool free. The primary allocation already zeroes/flushes
its409600-byte payload before these nested-root header writes.

OuterprimarySDK allocation/header, primary map allocation/header and inner
battle allocation/header are distinct layers. BattleAlloc/Free take inner
root8041e80c. N_battleMapFree80030048/30 calls _mapFree(primaryRoot,battleRoot)
then clears8041e80c; it does not free individual inner records or reset size.
Source battle_init80073914 creates this pool before explicit GP14=1 and event
reinitialization; battle_exit80073700 releases it after BattleEnd and before
the event/field context restoration chain.

Primary pool bootstrap is already complete in linked contracts. Original
fixed SDKpool1 span3472KiB; requested primary payload=span-32, initialized map
free size=span-64. Runtime OSarena boundaries and native address/memory policy
remain platform providers. Original arena static addresses are not native3DS
allocation requirements. For native E scope, supply aligned mapped storage,
source64000 extent/32-byte header and original allocator operations; keep SDK
cache/backend adaptation explicit.

## Native domain and pairing obligations

Current ending initially used an explicitly declared SDKstorage adapter.
Migrating E must pair source BattleAlloc with source BattleFree and keep its
actual root/allocation header address. Native preallocated battle/unit/part
records elsewhere are a separate current storage adapter; an E-only fresh
arena does not reproduce original prior allocation history or numeric pointer
identity. Preserve all live mappings/registered RAM until their actual owners
release them; don't unregister shared packed work when only E is freed.

_mapFree's early used0 check is not generic double-free safety: if a free
predecessor absorbs H, stale originalH8 can remain1. Do not rerelease stale
payloads or reset header bytes to manufacture idempotence. Use actual original
ownership/caller order. No automatic actor/event/pouch/story/sequence write
belongs to allocation/free; ending1f caller writes Bf28=0/topbank4 separately.

Validation: complete read-only _mapAlloc and actual ending/wrapper/bootstrap
calls, independent existing allocator/root authorities, worked valid-header
arithmetic and `git diff --check`. Docs only/no native tests/build/commit.
Full prior battle allocation history/arena/SDK/numeric hardware execution remain
distinct provider boundaries; ordinary E allocation/free source is closed.

## 2026-10-13 native E arena binding

The trio supplies aligned virtual primary root85600000 with declared E-only
reservation64040 (rootfree64020), invokes the actual nested pool bootstrap and
uses original allocate/free mechanics. Fresh nested root85600020 returns
E85600040; tail85600300, size63d00. Correct1f free restores rootnext0/free63fe0
and clears Bf28/topbank4 through caller writes. SDK user/event work remains
a separate native owner; no original full prior allocation history is claimed.
