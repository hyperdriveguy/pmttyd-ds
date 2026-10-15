# Retained original weapon UI work allocation and subtype initialization

US G8ME01 revision0 read-only DOL evidence. The complete source graph is
Init80116070/74, Common8011794c/120, Common2 801178d8/74, Common3 80117860/78,
End8011600c/40, plus BattleAlloc800f8934/28 and BattleFree800f8908/2c wrappers.
`battle_weapon_ui_continuation.hpp/.cpp` owns this whole graph. It binds the real
UI children of [retained weapon controllers](RETAINED_WEAPON_MENU_CONTROLLERS.md).
It does not reuse the different class UI constructor.

## Original captures and distinct work layout

Init's passed B is unused by original source. It remains native invocation
metadata only. The parent initializes two separate private output words0, then
Common captures actual current global B once. It reads B1c78, frees an existing
work allocation, requests292 bytes and publishes the actual allocator result
back to that captured B1c78. It freshly reloads B1c78 into its first private
output; callback-visible replacement of that word is preserved. A returned
allocation0 is a real scalar0, not unavailable. Source then accesses that raw
work pointer unconditionally; production must admit the original memory domain,
rather than inventing a safe no-work return. The test-only low-memory case
observes this unchecked prefix explicitly.

The cursor owns native equivalents of the original two disjoint private words.
Each source load/store of those private cells uses these owned words, without
fabricated guest stack addresses. Common stores input weapon cursor at W0,
0 at Wc/W10, byte0 at W14 and the fresh weapon selection at W18. It then reads
captured B420 and performs the real registry lookup. Kind222 selects the second
private output B1bb4; every other kind selects B1bc0. These are class cursors,
separate from the borrowed input weapon cursor.

Load actual literal8042242c and store it to W98. For exactly seven entries,
write arrays Wa4=0, Wc0=1, Wdc=negative ordinal and Wf8=0 in source order.
No blanket work clear or guessed fields are added by this constructor. The
actual MapAlloc child owns its original rounded payload clear/flush. The parent
then writes the weapon rows pointer to W8, not the class constructor's W4.

Flags lowbit1 selects complete Common2, lowbit0 Common3. Each reads selected
index from the second private class cursor, and repeatedly reloads its signed
count. Advance index with the count captured at the loop head, write timer0,
ordinal, row flags0, then freshly OR row flag1. A separate fresh count determines
the selected last row. Common2 writes its state0 versus10; Common3 writes1
versus11. No artificial count clamp or one-render-frame yield is inserted.

## Source allocation wrappers and held requests

BattleFree's nonnull source branch captures the heap global8041e80c then calls
MapFree8002fdc4(heap,pointer). BattleAlloc independently captures that global
at its own entry then calls MapAlloc8002ff58(heap,292). A heap change in the
free provider is visible to the later allocation, while one held request keeps
its original captured heap/pointer. Typed requests retain exact function,
callsite, original caller and argument words. Void free results are ignored;
engaged0 allocation remains0; `nullopt` alone is unavailable. Free is never
replayed if a later allocation holds. Constructor free does not clear B1c78
before allocation, matching the source prefix exactly.

End uses its passed B, independently of current global B. It reads B1c78 and
returns immediately on0. Otherwise its real BattleFree/MapFree must acknowledge
before the original passed B1c78 is cleared, even if callbacks change that word
or current global. It does not substitute class UI End or fake source teardown.

The same cursor pins operation, all original input operands, callbacks/users,
memory and lifecycle hook identity. Wrong owners/operations and reentry/reset
are rejected before source work. Required allocationCurrent is checked at resume,
synchronous group and provider boundaries; invalidation poisons the retained
invocation until explicit metadata reset. The caller retains the actual owner
and freezes mapping/storage through synchronous memory groups. This is native
admission policy, not an extra original battle predicate.

The unapplied native journal invokes actual VM MapAlloc/MapFree using the
captured request heap, checks actual flushBlock/systemCall before an atomic
allocation can mutate memory, and records successful actual UI allocation tokens
in the existing shared UI map. A held free pins the exact old token and rejects
replacement/ABA. Unknown free ownership remains unavailable; it cannot become
a newly reused pointer on retry. Explicit retirement clears native cursor/pins
only, never guest fields or an invented End. Actual runtime allocation/B proof
is independent of camera readiness. No allocator output or GPU success is faked.

## Drawing gate and validation scope

This constructor genuinely leaves W14=0 and Wc=0 for both subtypes. That is not
physical-menu readiness. Original Weapon Main8011606c is itself a blr leaf.
The genuine drawing/state path must advance the work before KeyOK can admit
normal input: Disp8011604c/20 directly calls DrawWeaponWin80115dac/260, which uses
DrawSubMenuCommonProcess80116e80/79c and Sub1 8011761c/244. Those drawing
owners remain a separate frontier. No readiness bit is forced and no completed
constructor is described as normal visible-menu activation.

Read-only verifier pins ten whole bodies, their eleven original linked calls
and literal8042242c=00000000. The literal already resides in accepted Trio's
original span8041f440; no asset import or new art is needed. Source-derived
fixtures compare full read/write/request traces and final memory for both
subtypes, signed count/index matrices, Mario/other class cursors, actual input
weapon cursor separation, old/null allocations, fresh heap/global and B-pointer
mutation, count/flag rereads and End ownership. Each map boundary holds twice
without prefix replay. Identity, operand, reentry, missing lease, stale-lifetime
poisoning and source allocation0 are covered. Strict host, ARM GNU++17 and
ASan/UBSan checks pass; combined native proposal syntax passes. These are static
and host contract checks, not executed-PPC, renderer or gameplay equivalence.
Root owns shared runtime/build integration, full validation and commits.
