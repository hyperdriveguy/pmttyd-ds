# Original battle HUD proportions in the native reflow

The native HUD uses the original icon catalog and actual executing resource
owners. Its former icon helper passed scale0.5 into `messageIconImage`, which
already converts source dimensions to native pixels by0.5 on both axes. This
made the HP/FP banners42x8 instead of84x16, large numerals8x8 instead of16x16,
and Mario's portrait13x13 instead of26x26. It also added a rendered image height
to the bottom coordinate, placing the artwork's top at the intended baseline.

The bounded native layout now passes source scale1 and the real bottom anchor.
The existing source-half HP/FP coordinates, numeral pitches and native horizontal
reflow remain intact. The top resource baseline is20: original source row y40
at half scale. Banners therefore occupy native y4..20. Mario's original52x52
portrait occupies y-6..20; the viewport clips its top6 pixels at this retained
baseline. The layout does not invent a translation to expose the clipped cap.
The green counter banner now shares bottom20 with its original source row and
its star/coin symbols. No image is resized, repainted or re-imported.

Original `StatusWinDisp8013cb24/91c` and its13-row8036edd0 table specify the
following AP group. The existing native HP/FP horizontal offset48 remains an
explicit port reflow, not a measured original screen position.

| Element | Original source placement | Native placement |
| --- | --- | --- |
| Banner474 | x360, bottom76, scale1 | center228, bottom38, scale1 |
| Star442 | x220, bottom80, scale0.9 | center158, bottom40, scale0.9 |
| Gauge origin | x260, bottom83 | center178, bottom41.5 |
| Full-star offsets | x32 per star, y12 | x16 per star, bottom minus6 |

The gauge origin follows actual call8013cf10 and constants80422c28..30;
original full-star offset12 is80422c58. The former compact AP banner began at
x197.8 while its first star ended at194.5, leaving that star wholly outside its
background. Restoring the pinned placement puts all eight24px original star
images inside the native148x16 banner. The original48px partial-fill/glow image
retains its larger footprint and intentionally extends beyond the banner in Y.
Capacity, fill selection, partner portrait and every counter still come from
the same real status reader. The layout never infers AP from a maxAP getter or
creates a missing partner/count. Prologue zero-capacity AP remains absent.

`planNativeBattleHud` is an immediate, fixed-capacity native icon plan. The
maximum73 records derives from three resource pairs, nine possible gauge
records, AP markers and two six-digit counters. It has no allocator, renderer,
GPU receipt, source writes or retained callback. The renderer's complete VM,
unit, pouch and status reads and original visibility gates are unchanged.
Unsupported gauge lengths reject the plan; negative counters retain the
existing unavailable-number policy. All original metadata/texture resources
and per-asset licensing remain under their existing imports. No asset product
or editable source changes are required.

Tests load the actual original compiled icon catalog and verify source scale,
bottom anchors, portrait clipping, banner/numeral bounds, all eight AP stars,
partial glow, unknown inputs and bounded decimal edges. These tests do not
acknowledge original `StatusWinMain/Disp`, slide/blink choreography or native GPU
submission. Parent integration owns whole build and isolated physical capture
receipts. Physical controls and the lower audience scene are unchanged.
