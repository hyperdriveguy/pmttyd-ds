# Retained original pose-buffer allocation

The native animation buffer machine now implements the complete
animPose_AllocBuffer80041ee0/93c parent, including its recursive recovery.
Original full listing and hashes are privately retained under
artifacts/animation-startup-owner-evidence/; fresh source inspection and static
implementation review are artifacts/shared-animation-startup-next/.
This extends the [refresh machine](NATIVE_ANIMATION_REFRESH_AND_SCRATCH_RECOVERY.md)
and [fresh shared pose owner](NATIVE_SHARED_FIELD_AND_BATTLE_POSES.md).

Input is the captured source pose pointer, not a numeric pose handle. Each of
its eight blocks reloads W from80415dc0, P10 and W0; captures old W108 BEFORE
file/holder/MODEL/count reads; then separately reloads that SAME W108 to reserve.
Model chain is W0+16*P10, file8, holder at file+a0, then *holder. Counts are
12*MODELf0 twice,12*MODELf8 twice, MODEL13c,4*MODEL140 twice,24*MODEL12c.
Multiplication and32-byte size rounding wrap uint32. Old cursor is not aligned.
Reservation publishes current cursor+aligned size; fresh W/base/cursor then
checks unsigned cursor < wrap(base+180000). Exact end is overflow.

First overflow calls initTestHeap, then scans current signed W14/current W10
with whole-word active flags and the original mode filter: workmode0 skips
mode1; nonzero skips mode0; mode2 participates in both. Each admitted pose runs
this SAME allocator recursively, followed by caller cache3c=-1. The retry uses
the terminating scan's retained W, captures old cursor, aligns retained size
again and reserves. It does not rederive size from changed MODEL data.
Second overflow runs Refresh, then testAlloc with that aligned size. This is
not testAlloc's first-overflow inline Rebuild path. Replacing each block with
a generic testAlloc call changes the source recovery graph.

Successful pointers publish P48/4c/50/54/58/60/64/68 in order. After P58 and
P68, reread the actual published word before copying to P5c/P6c. This parent
performs exactly one final cache3c=-1. Its recursive Restore caller has its own
additional invalidation. No vector clearing or base-data copy is performed.
Retained child/read/store cursors preserve captures and completed prefixes
across native provider holds. The existing64-frame native recursion bound
continues to hold unsupported/nonterminating source graphs; it is not an
original limit, successful null allocation, or modulo-ring fallback.

Restore inside testAlloc now executes this actual child. BattleSequenceBinding
already nests the refresh machine over caller VM services, so overflow reaches
the same implementation and genuine heap boundary without a substitute provider
acknowledgment. Missing W/model/file/holder/scratch storage still holds.

Tests exercise exact requests, early cursor capture, aliases, mode2 in both
contexts, actual recursive Restore, second-overflow Refresh/testAlloc, allocation
holds and bounded nontermination. Original-resource fixture maps the same real
typed pose bytes into the VM and uses original MODEL counts; W/file/holder and
zeroed scratch are explicit fixture views. CPU evaluation consumes the actual
invalidated cache word afterward. This is not proof of cold animInit, loaded
file relocation/lifetime, global W startup, CPU-vector/guest-scratch aliasing,
authored encounter chronology, GPU pixels or full prologue completion. Those
owners remain open in the [request inbox](../../requests/EVT-01.md).
