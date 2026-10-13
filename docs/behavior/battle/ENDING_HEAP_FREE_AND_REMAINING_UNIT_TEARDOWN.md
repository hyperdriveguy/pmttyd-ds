# Ending map-heap free and later remaining-unit teardown

2026-10-13 US read-only complete BattleFree800f8908/2c,
_mapFree8002fdc4/9c and BattleEnd800f8da4/84. Listings ignored under
`artifacts/recovered-temp-evidence/return-battle-{free,end}.dis` and
`return-map-free.dis`. No native changes/commit.

## BattleFree is a map allocator operation

BattleFree takes payload pointer in r3, retains it in r4 and tests null.
Null makes no allocator call. Nonnull loads WORD r13+18ec=8041e80c,
symbolR_battlemapalloc_base_ptr, into r3; calls _mapFree(base,payload).
This is **not** __memFree(heapID0/3,payload), and the loaded value is not an
SDK heap-number argument. [BattleAlloc](BATTLE_ALLOC.md) pairs this with
_mapAlloc8002ff58 using the same original map-allocation base.

No payload memset, handle/event cancellation, global-base clear or owner-field
mutation occurs in BattleFree. Ending1f subsequently clears Bf28 and changes
topbank0=4; those are caller writes. E29c PointGet is a separate effect/heap3
owner and was already soft-deleted/released by its own source phases. A native
fixed-address RAM allocator adapter may retire the E allocation without
unregistering a shared VM region; preserve that explicit adapter/domain rather
than tracing a fictitious SDK __memFree or clearing all retained bytes.

## Complete _mapFree valid-header operation

Inputs base header and payload. Null payload returns. H=payload-32;
unsigned halfword H8==0 returns without mutation (already-free header).
Capture next=H0 and size=H4. If next!=0 AND next8==0, merge the next free
block: capture next.next and add next.size+32 to size. Retain the new next.

Traverse headers from passed base using their word0 links. When current0==H
and current8==0, choose current as final header and add current4+32 to size;
this coalesces the predecessor. If current==H, current>H, or current0==0
without that predecessor admission, use current H. Otherwise move to current0
and repeat. Final writes selected header0=retainedNext,header4=combinedSize,
U16header8=0. No forward walk across multiple adjacent free blocks, allocator
destruction, payload clear, global pointer clear or OS free is requested.

Original unsigned pointer comparisons and low32 size arithmetic remain raw.
No invalid-header/cycle/address/alias guard exists locally. This closes valid
map-heap blocks only; source allocator/bootstrap/invalid memory domains remain
separate services.

## Later BattleEnd and exact remaining-owner order

BattleEnd is called by battle_exit/sequence teardown, not ending1f itself.
Capture current global B from r13+1c70. For indices0..63, request
BattleGetUnitPtr(B,index)800f88cc; nonnull calls BtlUnit_Delete80128e08.
The getter is invoked once per index against retained B; live registry changes
can affect later slots. Do not delete only the previously defeated three
enemies or infer that alive Mario/Goombella/Frankly remain indefinitely.
See [unit deletion](BTL_UNIT_DELETE.md) for ordered events/parts/registry owners.

Then call, in order:
BattleIconEnd801aa118; battleMenuDispEnd80119d74;
BattleStageEnd801390e4; BattleAudience_End801a6b10;
BattleBreakSlot_End80203688. Retained B1c78 nonnull→BattleFree(that pointer);
then BattleFree(B), finally global B=0. B1c78 is passed raw, without inferred
type or ownership reset. None of these transitive services are skipped merely
because the native audience/effect inventory is empty; bind their actual
admission/reset/release contracts or retain explicit pending providers.

Native next safe scoped step: execute BattleFree on actual E allocation and
the caller's Bf28/topbank4 writes, then actual field-Mario/party reconstruction
before Rule0/audience stop. Do not publish sequence2/field control/story12 there.
Use [field-return handshake](UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md).
Later BattleEnd requires valid battle work/live registry and ordered resource
teardown; it has no global-B null guard.

Validation: complete read-only used bodies/header fields/service identities;
`git diff --check`. Documentation-only/no native builds/tests/commit. Map-heap
bootstrap/allocator adapter, transitive remaining-unit/resources and actual
field reconstruction remain explicit native/provider boundaries.
