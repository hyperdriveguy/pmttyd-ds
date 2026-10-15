# Whole PresentItem with native private XYZ ownership

US G8ME01 revision0 read-only DOL `_check_present_item80120024/1f8`.
The new `battle_present_item_native.hpp/.cpp` implements the complete source
parent using internally owned disjoint XYZ output. It leaves the [existing
retained guest-output helper](NATIVE_PRESENT_ITEM_CONSUMER.md) unchanged as a
source-based differential reference. [Gift branches](GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md)
and the existing packed HP/FP helpers supply the same original semantics.

This is an explicit native private-output adaptation. It neither maps an
invented guest stack nor claims equivalence for arbitrary output/unit aliasing
or PPC FPSCR/NaN effects. The actual source caller passes three distinct private
stack cells. The native cursor owns three raw words in that role. GetPos80128c7c/1c
loads unit3c then stores X, loads40 then stores Y, loads44 then stores Z; the
native sequence performs those same three load/store pairs. GetHeight80128414/8
loads signed halfce afterward. No missing position, unit, scale or literal is
replaced by a placeholder.

## Retained source operations

Capture B before the item getter's own fresh global capture. Preserve signed
item/target bounds, actual unit lookup before item0/negative rejection, generic
pending1/2, AllEnd mask, fresh previous type and type12/state29 writes. Generic
items return1; early exclusions return0 with their exact source query prefix.

For123/124, execute the existing original packed HP/FP recovery by1 exactly
once. FP retains actual partner resource ownership and independent Mario lookups.
Then GetPos, signed height conversion setup, bias802efe78, scaleU114 and
constant804225f8 are captured in original order. The cursor retains the actual
subtraction-double/single-round result across later child waits. Native Y, X, Z
are consumed after that conversion; fused scale*height+constant precedes the
single addition of original Y. The new Y is stored privately before invoking
actual effRecoveryEntry8014335c(kind0/1,amount1,XYZ).

The same owned effect position and arguments survive every effect hold. Missing
math or effect never repeats recovery, silently acknowledges an effect, clears
an item or changes the source return. Only actual effect acknowledgement enables
the fresh-global present-item0 store and completed source return0. Providers
must own any effects they partially acknowledge internally; the parent retains
all its own completed operations.

## Native ownership admission

A required `allocationCurrent(user)` provider proves actual storage/lifetime.
All service callbacks/users, memory functions and actor lifecycle hook identity
are frozen across holds. Recursive resume/reset and replacement owners are
rejected before source work. Lifetime is checked before/after synchronous source
operation groups and every math/effect boundary. A failed check permanently
poisons this retained invocation until explicit metadata reset; a later true
lease cannot resurrect the old captured work. Reset performs no guest cleanup.

The caller must freeze actual memory/allocation through each synchronous guest
memory/recovery group; a total memory callback cannot replace storage midway
and remain a valid source provider. An invalidation detected after a group can
leave its already executed source prefix, which is never replayed. The caller
retains callback/user storage for the cursor lifetime. Missing lease cannot start
source reads. These admission rules are native policies, not new original
battle predicates.

The unapplied native journal uses the existing actual Trio runtime allocation
token and mapped-B/current-global proof, independently of camera readiness.
It uses actual VM subtractDouble then roundDoubleFloat, fmaFloat and addFloat
providers. This binding admits the real current battle allocation; generic host
services may admit a fresh global B change where both actual allocations remain
valid. Missing effRecoveryEntry stays unavailable at8014335c. No successful
empty effect or fabricated resource owner is provided.

## Evidence and validation

Read-only source verification reuses20 whole bodies,243 instruction anchors and3
original numeric/startup spans, plus a new complete GetMaxFp80126b14/44 hash.
Fresh disassembly verifies the whole PresentItem body and exact GetPos/Height
leaves. No extracted source binary is committed.

Focused tests compare original guest-output and typed-output parents over the
admitted disjoint private-output domain, excluding only the guest private XYZ
reads/writes. They compare source memory read/write traces, final actual guest
storage, math/effect calls, positions and results. Cases include signed bounds,
generic pending branches, HP clamp/FP wrap, partner owner replacement between
fresh Mario lookups, signed height extrema, fused cancellation, GetPos mutations,
retained scale/constants, source fresh-global clear and all four child holds.
Each child holds twice without replaying guest work; output/request identity,
provider/lifecycle replacement, reentry/reset rejection, missing effect and
allocation invalidation after every provider are checked. Host, ARM GNU++17
and ASan/UBSan checks pass; these are not executed-PPC or full native gameplay
observations. Shared Trio/build integration and full validation belong to root.
