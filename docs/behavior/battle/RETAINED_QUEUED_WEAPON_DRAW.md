# Retained whole queued weapon-window display

`battle_weapon_draw_continuation.hpp/.cpp` owns the original queued callback
80121ce8/24, Disp8011604c/20, DrawWeaponWin80115dac/260,
DrawSubMenuCommon80116e80/79c and DrawSubMenuCommonSub1 8011761c/244.
The ISO remains read-only. `verify_battle_weapon_draw_continuation.py` pins all
five whole bodies, their 29 direct calls and six original constant/table spans.
These are static source pins, not execution of the original PowerPC binary.

## Source captures and readiness

The callback and Disp forward B; Win ignores that argument and independently
captures the actual current global B, W, acting unit, weapon cursor and selected
weapon. Common makes its own fresh global/work/unit/cursor captures. They are
not merged: a changed global between source captures remains observable.
The callback camera and forwarded B remain native invocation identity only.

Common owns all original cursor clamping, mode dispatch, class states and seven
submenu states. Sub1 owns both wrap transitions and both adjacent scroll
transitions. Original jump tables are actually read. All comparisons retain
source signedness, including negative cursor counts and timers. GP rates,
constants and bias words are read at their original groups; SDK operands remain
captured over a child suspension. State10 class Z is the original ring offset Y,
not a corrected center Z. State1 leaves its private position unwritten.

W14 is written1 by real source branches before subsequent geometry and raster
children. This is an observable prefix, **not whole callback completion**. The
caller must hold the current source display invocation and simulation boundary
until this whole controller returns true. It must not advance command input
merely because W14 changed. No reentry flag or readiness value is invented.

Each metadata row captures W98 and the cursor count before its fctiwz provider.
Cost receives the retained actual unit and weapon. After cost completion, the
source reloads the row's weapon before reading BP. The selected weapon for help
is the earlier Win capture; item/default help branches retain their distinct
fresh row and item-table reads. Zero help key genuinely skips the help child.

## Private outputs and child ownership

The cursor owns separate optional class position/matrix/parameter cells and
seven weapon matrix/label/icon/enabled/cost/BP cells. Unwritten cells stay
unknown. No fictitious guest stack addresses or identity matrices fill them.
The original parent's class private arrays have five entries; a class count
requiring a sixth is outside this typed native output domain and holds at
80116e80. This is an explicit native domain boundary, not a recovered original
bounds check. Negative counts preserve source no-row behavior.

Translation, scale, rotation, concatenation, identity, interpolation, ring
center and ring offset remain actual required providers. Scalar fctiwz and FMA
are explicit providers; native finite-domain adapters must state their own
rounding/FPSCR policy. Single source operations use float-rounded add/sub/mul/div
and original signed-word double-bias conversion. This does not claim arbitrary
PowerPC FPSCR/NaN equivalence on every host.

The typed request boundary retains actual function/callsite and scalar operands:
80126968 weapon cost, 80119abc MainIcon, 80119024 SubIcon, 80117a6c cursor/scroll,
80117c1c help. Draw requests borrow the same cursor-owned output object across
holds. Consumers must copy output needed beyond the invocation. Existing
[observable class drawing](NATIVE_CLASS_MENU_FRAME.md) may be
composed only under its documented active-row ordering policy; no missing Y is
invented. SubIcon consumers must reproduce the original row-count/flag tests
before requiring an unwritten inactive matrix or metadata cell.

Every provider must own its **whole** invocation across false/nullopt, including
any acknowledged raster prefix. Ordinary scalar0 is successful completion.
An absent provider stays unavailable. Old synchronous partial drawing must not
be blindly retried. A single actual ordered sink must retain icon/submenu/help
interleaving and publish only after whole callback completion.

## Native admission and integration limit

The required `current` predicate must prove actual UI allocation ownership,
current successful CameraMain producer epoch, camera8 Draw stage and the genuine
invoking queue record80121ce8. Check before/after each provider and each source
operation group; false permanently invalidates this cursor until explicit
metadata reset. The parent must freeze its memory mapping/camera epoch through
synchronous reads and writes. Full callback/users/memory/lifecycle service
identity, wrapper argument identity and recursive-entry/reset guards are pinned.
Borrowed contexts/assets outlive the held invocation. Reset performs no original
End or guest cleanup.

The existing `registerBattleCommandDisplay` already queues real80121ce8 on
camera8/class0/priority500 for its original flag branch. Do not queue a duplicate.
The existing class camera bridge is admitted only for8012210c: it cannot be
spoofed for this callback. A dedicated audited weapon companion and actual
retained SubIcon/cursor/help raster owners are still required. This batch does
not activate normal native drawing, acknowledge absent GPU work or replace
original menu art. Physical input remains in the existing original Weapon/Main
controller and is not replaced by touch-only controls.

## Validation

The asset-backed host fixture constructs a cold allocation through the actual
portable weapon UI constructor, reads original constant/table banks, and
compares immediate versus fail-each-child-twice read/write/provider traces.
It covers all mode values0..41, class/submenu state branches, timer boundaries,
wrap/adjacent transitions, item/special/default/empty help, actual unknown cells,
pre-conversion count mutation, post-cost weapon mutation, service/lifecycle and
argument replacement, reentry, unavailable providers and permanently stale
leases. Math/raster fixture providers are explicit abstract child contracts,
not camera/GPU/native proof. This is source-branch and retention validation,
not an independently executed PPC differential oracle.
