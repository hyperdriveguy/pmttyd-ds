# Unit part construction, defense bindings and deletion lifetime

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Evidence B: original BtlUnit_Entry region0x80129398..0x801295b7, complete
BtlUnit_GetPartsPtr0x80128cd8/0x40, BtlUnit_Delete0x80128e08/0x1d8,
BattleAlloc0x800f8934/0x28 and BattleFree0x800f8908/0x2c; original DOL Mario
part data and shipped gor.rel Crump kind/part/table relocations.

This closes the scoped DMG-01 part allocation/defense/lifetime request.
[Whole local unit entry](../battle/BATTLE_UNIT_ENTRY_COMPLETE.md) and its
[geometry/scalar](../battle/BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md) and
[pose/display](../battle/BATTLE_UNIT_POSE_AND_DISPLAY_INITIALIZATION.md) follow-ups now
close those local regions. All mutation/source/lifetime writers,resource/render
engines and original execution remain separate requirements. The
[entry/stat/spawn contract](UNIT_ENTRY_STATS_AND_SPAWN.md) already identifies
unit publication before parts and later asynchronous init registration.

## Allocation and pointer ownership

Let U be the allocated runtime unit and K its kind pointer U+0x10. Load
count=U8(K+0xb4), allocate count*0x500 bytes through BattleAlloc, zero the block
through memset, then store its base in U+0x14. The maximum unsigned count255
produces size0x4fb00; there is no count cap in this region. Publication of U in
the64-slot registry has already happened before this allocation.

BattleAlloc calls `_mapAlloc(sharedHeap,size)`, passing the heap pointer at
r13+0x18ec and the original requested size. Its wrapper does not add alignment,
clear memory or check failure. `_mapAlloc`'s internals remain a dependency.
BattleFree ignores null; otherwise it calls `_mapFree` on the same shared heap.
These wrappers do not identify themselves as standard malloc/free services.

The part region has **no allocation-null check and no zero-count pre-loop
guard**. Its first part initialization executes before count is decremented.
For count0 or failed allocation, do not invent successful zero-part construction
or rollback of the published unit. Valid count/nonfailure invariants must come
from kind data and allocator/runtime ownership. A port may report an unsupported
invalid state separately, rather than call that handling recovered behavior.

Load source part array S=word(K+0xb8). For each initialized part P:

| Runtime field | Initial source/value | Meaning used by recovered consumers |
| --- | --- | --- |
| P+0x4 | Current source part S | Retained kind-record pointer,not a copy |
| P+0x4ec | U | Back-reference to owner unit |
| P+0x1ac | word(S+0x40) | Part attributes read by target/damage services |
| P+0x1b0 | word(S+0x44) | Counter attributes read by counter services |
| P+0x1b4 | word(S+0x38) | Signed-byte defense table pointer |
| P+0x1b8 | word(S+0x3c) | Signed-byte defense-attribute table pointer |
| P+0x1bc | word(S+0x48) | Retained pose-table/service pointer |
| P+0x1c0 | -1 | Pose handle sentinel consumed during deletion |
| P+0x4e8 | -1 | Handle/marker reset by deletion;exact producer still open |
| P+0x8 | word(S+0x4) | Retained source field |

At0x801293f4..0x80129408, if P+0x1ac has mask1, write word(S+0) to U+0x2d4.
Every qualifying part overwrites it, so this stored ID is the **last** qualifying
part's source ID. The previously recovered BtlUnit_GetBodyPartsId scans the
linked list for the **first** qualifying part instead; do not merge these two
results when multiple parts carry mask1.

Other source fields are copied into signed-halfword and vector runtime slots,
and scale/rotation helpers are called in this region. Those presentation helpers
remain dependencies; documenting the defense construction does not claim a
complete executable/rendering part initializer.

After decrementing remaining count, continue only while signed remaining>0.
Set current P's next pointer to P+0x500, advance source S by0x4c and initialize
the next part. The last next pointer is zero. Thus valid positive counts use one
contiguous allocation with linked nodes, not independent per-part allocations.
Neither defense table is copied or separately allocated here: original shared
table and kind-record lifetimes must outlast their runtime readers.

## Exact part lookup behavior

BtlUnit_GetPartsPtr(U,requestedID) first loads head=word(U+0x14). U itself is
not null-checked. Its complete behavior is:

1. If signed requestedID<0, return head without dereferencing head.
2. For nonnegative ID, dereference head's next word. If it is zero, return head
   **without comparing the requested ID to the source part ID**.
3. Otherwise traverse starting at head. Compare word(word(P+4)+0) against the
   requested signed word; return the first matching P. Follow P+0 until null;
   if exhausted, return null.

A nonnegative request with null head dereferences null before the general scan.
There is no ID narrowing, bounds clamp, cycle guard or source pointer validation.
For a single valid part1, requested0,1 or99 all return the same head; requested-1
also returns it through the first branch. For multiple parts, a missing ID
returns null rather than the first part. Preserve these cases in a portable
semantic service; a generic dictionary lookup changes the single-part contract.

## Concrete Crump and Mario source tables

Crump's kind at gor.rel **file0x645c8** has part count1 and ADDR32 relocation
at +0xb8 to part array **file0x646cc**. Its single0x4c-byte record has ID1,
attributes9 and counter attributes0. ADDR32 relocations at source+0x38/+0x3c
bind **file0x6468c** defense and **file0x64694** defense-attribute tables.
Both are five zero bytes, interpreted as signed entries by the calculator.

Consequently, the recovered entry stores those table pointers at runtime
P+0x1b4/+0x1b8 and initial flags9/0 at P+0x1ac/+0x1b0. The mask1 test writes
U+0x2d4=1 and the first-body helper also returns1. Base defense and defense
attribute for elements0..4 are0 from these unchanged source tables. This is
initial construction, not proof that later events never change pointers/flags
or that all calculated damage equals weapon power.

Mario's annotated DOL array unitpartsdata_Mario at0x80372a18 has three0x4c-byte
records, IDs1,2,3. Their attributes are9,0x13010000,0x13010000 respectively;
all three counter-attribute words are0. All three records point to shared
defense0x80417ba0 and defense-attribute0x80417ba8, each five zero bytes. The
array alone does not establish the actor's count. The subsequent
[badge/setup contract](UNIT_BADGE_COUNT_IMPORT.md) now pins entryunit_mario to
unitdata_Mario with count3 and this array,establishing the concrete initial
Mario construction. Later mutations and executed runtime state remain open.

The [damage calculator](../battle/BATTLE_DAMAGE_CALCULATION.md) reads signed bytes from
the runtime table pointers indexed by cached element, then applies badges,
statuses, guard and other modifiers. It does not validate an arbitrary element
index against the five-element data. Do not treat zero source defense as a
guarantee of zero effective defense, or source counter0 as full counter-policy
closure. See [counter selection](../battle/BATTLE_COUNTERS.md).

## Unit deletion: request order and part release

The complete local BtlUnit_Delete body accepts a unit pointer. Null returns0.
It scans registry slots0..63 for pointer equality and returns0 if absent. It
uses the matched slot for the eventual registry clear, not a trusted unit ID
as the slot index. Registered units execute the following ordered operations:

1. For each nonzero handle U+0x290,+0x29c,+0x2b4, call evtDeleteID and clear
   that field, in that order. No local explicit U+0x288 init-handle deletion is
   present; broader event teardown/child ownership remains unresolved.
2. In all64 ordering words at BattleWork+0x120, replace matches to word(U+0)
   with-1; leave other entries untouched. This is removal with holes, not
   compaction or registry-ID reassignment.
3. If word(U+0x1c) has mask0x40000000, read setup pointer BattleWork+0x2738
   and its +0xc group pointer. If S8(U+0xe)>=0, write word(U+0x308) into
   group+0x5c+4*that index. These intermediate pointers are not null-checked
   locally; the group field's downstream meaning remains open.
4. Call BattleStatusEffectDelete(U), then BattleStatusIconDelete(U).
5. If U+0x314 is nonnull and U+0x318 is nonnull, call the latter as callback(U).
   If the extra pointer is nonnull but the callback is null, BattleFree the
   extra pointer. With null extra pointer, no custom callback is invoked.
6. If U+0x31c is nonnull, call smartFree on it and clear it.
7. Traverse the part list. Replace P+0x4e8 with-1 if needed. For pose handle
   P+0x1c0 !=-1, call animPoseSetEffect(handle,0,0), then reload the handle and
   call animPoseRelease. Follow each next pointer; no per-node BattleFree.
8. BattleFree the block at U+0x14 once, then BattleFree(U), then clear the
   matched registry slot through BattleSetUnitPtr(work,slot,null). Return1.

Part source records and defense tables are not freed by this body. Other services
and the optional custom callback may have their own mutations; their internals
are dependencies. Registry clearing occurs **after** both block/unit frees,
so callbacks earlier in deletion still see the registered pointer. Do not claim
atomic unregister-first behavior or add omitted event deletions as original
semantics. Sound/effect shutdown and eventual REL unloading require their owners.

## Scoped implementation requirements and remaining work

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| PART-01 | Preserve kind/count/record/table ownership and contiguous linked construction | Positive count1/3 uses0x500/0xf00 block,0x4c source stride,terminal null and retained shared table pointers |
| PART-02 | Retain independent attribute/counter words and signed defense tables | Crump initial ID1,flags9/0 and both zero tables;distinct negative synthetic defense/attribute entries reach calculator unchanged |
| PART-03 | Preserve exceptional part lookup | Single part accepts nonmatching nonnegative ID;negative ID returns head;multiple parts match first source ID or return null |
| PART-04 | Preserve ordered lifetime/service calls | Unregistered/null delete returns0;registered delete leaves ordering holes,releases poses then block/unit once,and clears registry last |

Derived acceptance cases are not original execution tests. Full DMG-01 still
needs unit-kind/part construction for every actor, all mutation/reset writers,
pouch/badge/status initialization, effect/pose/heap internals, damage dispatch
and original hit/death/reload traces. Verify valid nonzero counts and table/REL
lifetime in real callers. Do not turn raw initializer data into tutorial timing
or fabricate a graceful original zero-part/failure branch.

Reproduce the named complete functions with tools/inspect_dol_function.py;
BtlUnit_Entry inspection contains the scoped region above. Evidence reports
retain their function fingerprints;tools/verify_behavior_contracts.py pins the
construction region hash, complete lookup/delete/allocator wrapper hashes,
40 new instruction anchors, Mario array/table values and Crump count/part/table
relocations. All ISO access is read-only;extracted code/data remain ignored or
under /tmp. No new asset conversion or licensing grant is implied.
