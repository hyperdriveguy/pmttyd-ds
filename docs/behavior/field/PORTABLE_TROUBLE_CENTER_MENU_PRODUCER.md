# Portable original Trouble Center menu producer

Implements the recovered list/main producer in the
[original board contract](TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md).
Story comparisons retain all30 original slot identities, signed thresholds and
20/21-before19 order. Count is the number of nonzero slots; completion does not
filter or sort the list. No start/reset switch callback runs during list init.

TroubleQuestMenu retains work/cursor/top/previous/counter94/smooth scroll.
State0 emits original icon504/524/524/134/136 enter/flags/scale order, hides all
five, sets1, requests SFX42 and common display without processing current input.
State1 preserves A then B precedence, current quest1420 and completion flag
5374+one-basedID guard. AcceptedA→100/SFX18 andB→101/SFX19 still register common
display and defer LW0 publication/delete to next invocation. Exit states publish
one-based ID or-1, reset0, delete allfive in sourceorder/SFX43, return2 with no
display. Re-entering state0 preserves cursor/list after source confirmation No.
Unknown source work states still request the common display.

Sequential down/up repeat conditions run before wrap; only fresh direction
trigger permits edge wrap. Scroll thresholds7/3, ten-row L/R paging, source
positions/visibility order, counter-reset and float scroll delta/6 are preserved.
No increment owner is established for counter94; the complete original display
does not use it. Original frame/text drawing belongs to the mutable display
provider, not callback invocation/render-frame guesses.

All provider identities are required before begin. Missing native active-quest
or completion knowledge returns pending before source writes; it does not infer
zero/false from absence. Copied owner cannot invoke live services. Retained work
is an adapted finite owner; source allocation416c/string cache/mapFree and caller
EVT lifetime remain providers. No native billboard/lecture/window admission or
placeholder art/menu is introduced by this portable producer.

Tests cover source story boundaries/fullslotorder, no completed-task filtering,
active/complete/unknown-state guards, copyguard, A/B precedence, deferred quest2
publication (not3), original request ordering, both-directions-before-wrap,
repeat-edge refusal, paging and scroll/pointer geometry. Source review found no
finite producer mismatch. Native2D icon/display/SFX assets and source doorway/
case9/lecture/confirmation/acceptance chain remain next required bindings.

## Exact validation (2026-10-14)

Committed a1dfd26a. Reviewed isolated tree3239633f9328ade5d0fd2e6ef77d26439424567a
passes make test -o assets-ready:252/252 C++ and144/144 Python, and normal
make all -o assets-ready. Sequential assets-ready passed before checks;
converters/payloads are unchanged. Logs ignored artifacts/trouble-menu-assets.log,
trouble-menu-final-test.log and trouble-menu-final-build.log. Reviewed artifact
artifacts/ttyd-trouble-menu-reviewed.3dsx SHA256
31499e76d8902f39d666fe01f7fbbe6942568ced17b290a5e6d412cbe33b1f60.
Read-only source comparison found no finite producer blocker; source audit matches.

Initial host link failed because committed battle_display_footer.cpp was absent
from host target registration; the missing dependency is now registered. Initial
native compile rejected mixed int32_t/int min deduction; explicit32-bit template
arguments fixed paging. Initial logs trouble-menu-test.log/trouble-menu-build.log
remain as failed evidence. Final checks use corrected snapshot. Concurrent
source/CMake/camera edits are preserved. No new Azahar/Dolphin or hardware/menu
rendering run; portable producer compilation does not prove native board play.
