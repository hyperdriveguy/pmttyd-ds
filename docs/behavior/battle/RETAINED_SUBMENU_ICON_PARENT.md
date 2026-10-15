# Retained original DrawSubIcon row parent

`battle_sub_icon_continuation.hpp/.cpp` owns whole DrawSubIcon80119024/184.
`verify_battle_sub_icon_continuation.py` pins its entire read-only US G8ME01
revision0 DOL body, four direct calls and five actual constant/bias spans.
The parent is the real linked child of
[queued weapon drawing](RETAINED_QUEUED_WEAPON_DRAW.md), not an empty raster
acknowledgment. DrawSubIconSub8011847c/ba8 remains a distinct required child.

## Source order

Capture actual current global B, read acting ID and perform the original unit
lookup even though the result is unused. Capture W98, fctiwz that initial value,
then read the signed-word double bias, row spacing and W0 cursor. Compute the
initial fractional scroll displacement from the captured float and converted
integer. Do not recompute it during later row visits.

For each of exactly seven rows, freshly read W98 and cursor count before a
separate fctiwz. Signed comparison of converted scroll+row against that captured
count comes before the row's Wa4 bit1 gate. Both gates precede all private matrix
and metadata consumption. Thus inactive cold rows can legitimately complete
without any populated matrix, label, icon, enabled, FP cost or BP cell.

For a visible row read the original bias and spacing/base/X/Z constants in
source order, evaluate the source FNMSUBS through the explicit FMA provider,
then add the previously captured fractional displacement. Request actual SDK
translation followed by Concat(translation, row matrix). Only after successful
Concat consume label, icon, enabled, cost and BP and invoke the real whole
DrawSubIconSub child. Its matrix and all scalar operands are captured in one
stable cursor-owned request through every hold. Both original flag operands
remain distinct and unchanged. Ordinary return0 is successful completion;
missing providers are unavailable.

## Native admission and private output policy

`current` must prove the actual parent allocation/pass/camera/queue lease.
It is checked before every source group and after every child boundary; false
permanently poisons this cursor until explicit metadata reset. Callers freeze
mapping and camera epochs through synchronous source reads. Every callback,
context, actor-memory/lifecycle identity and both flags are pinned. Reentrant
resume/reset fails without changing progress. Reset never writes guest state
or invokes original End.

The caller's `BattleWeaponDrawOutput` identity and bitwise cell values are
immutable during the invocation. The cursor copies that actual private output
without filling unknown cells. This is a native ownership policy: it prevents
replacement or late fabrication of the original stack storage through a hold.
An unknown active matrix holds at Concat; unknown active metadata holds at
SubIconSub. Filling that originally unknown cell does not authorize the held
invocation; retire metadata and obtain a genuine new source invocation instead.
Unknown inactive cells do not block. No fabricated guest stack is mapped.

Finite native float conversion/FMA/SDK policies remain caller-owned; this code
does not claim arbitrary FPSCR or exceptional-NaN equivalence. No camera,
texture, glyph or GPU request is stubbed. The linked raster child must retain
its whole invocation and the ordered publication remains private until the
entire queued weapon callback completes.

## Validation and integration

Asset-backed tests use actual original constant words, an independent immediate
straight-line transcription of this body, and fail-each-child-twice comparison
of every guest read/provider call. They cover signed counts and scrolls, seven
row visibility gates, genuine cold unknown rows, missing active operands,
count/scroll mutation during conversion, callback/output/lifecycle replacement,
reentry/reset and stale leases. Abstract math/raster test providers are explicit
child contracts, not native camera or GPU proof. Source pins are static evidence,
not execution of original PowerPC code.

Integration routes original function80119024 to one persistent
`BattleSubIconCursor`, using q.arguments[0] W, q.output and the two following
flags from `BattleWeaponDrawRequest`. Retain the same output reference/context
while held. Reuse actual SDK math adapters; bind `request` to a complete
retained8011847c owner. No queue registration changes are needed. Normal native
activation remains unavailable until those original raster providers and the
weapon-specific actual80121ce8 camera admission are bound.
