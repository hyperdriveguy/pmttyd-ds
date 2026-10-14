# Native retained explicit-table selector owner

The portable owner in `include/item_selection_runtime.hpp` and
`source/item_selection_runtime.cpp` binds the recovered
[three-window lifecycle](ITEM_TABLE_SELECTION_WINDOW.md) and
[draw requests](ITEM_TABLE_SELECTION_DRAW_REQUESTS.md). It is independent of
area/NPC identity and 3DS services. Native renderer/event/inventory integration
is still required; this contract does not claim a quest selector is live.

## Ownership and traversal

`enter` retains one template0 instance with list/header/help windows. Explicit
rows retain unsigned item IDs, duplicates and source order, with zero row flags
and second argument0. The list's source height shrinks below eight rows; header
and help retain their original rectangles. Initial sequence flags are active1,
not visible2, and all clocks/transforms/help state are clear. Main state0 owns
the open request and its source sound/status calls.

Each simulation invocation traverses slots0,1,2. For each active slot, sequence
runs first, followed by its main callback, then deferred display registration
when visible2 is set. Only slot0 runs selection main. Slot2 runs the supplied
description main. As a consequence, the first invocation requests all opens
from list main after list sequence has run, while header/help see the request
in that same invocation. Header immediate sequence2 and list/help sequence0
retain their different clocks. Close cleanup in list main deactivates all slots
before later header/help invocation. No renderer frame advances this owner.

Sound/status calls are service requests with unchanged original IDs. Refresh
and interpolation are supplied services; a constant60 service and native
`cameraInterpolation` in tests are adapter inputs, not source GP production,
original libsin rounding or measured timing. Existing sequence/main helpers
retain the source local calculation and key namespace rules.

`poll` returns0 before ready1000, -1 for canceled2000, otherwise the selected
unsigned item ID. Rows and index remain retained after ready for the event's
index getter-before-deletion ordering. `release` accepts only ready instances,
then releases rows/context. It does not restore Mario keys or write event
destinations. The caller binds those through the existing original event wrapper.
Item0 remains a zero result rather than becoming a cancel sentinel.

Native admission rejects empty lists and overlapping instance entry without
modifying an existing owner. This is a bounded native safety policy, not an
original empty-list definition. The row vector reserves n+2 entries instead of
emulating original heap memory writes; source packed allocation/effect order
remains in `fillPackedExplicitItemRows`. Tokens0/1/2 represent this retained
template, not allocation from the game's shared manager-slot pool. Full pool
allocation/lifetimes and effectful reentrant callbacks remain integration work.

## Source-space drawing and help

Plans retain every row for the source scissor, rather than dropping rows based
on the selected eight-row range while displacement eases. Item icon anchors,
name anchors/width and name black/disabled-gray colors remain separate from
the icon's white/gray service. Cursor504 uses retained floats and draw flags20 (including X mirror);
list arrows use source fused X, original blink bits, draw flags16 and scale.6.
Second argument0 does not request stripe447.

Heading retains capped metric centering, the source low16 zero-extension,
white color, width120 and outline request. Header retains X-only scale fitting,
LF-dependent vertical centering and low16 X offset. `HeaderAfterDraw` applies
the source post-draw rectangle mutation using template0 baseline Y120/height45,
and only when its active flag is set. It must run after text submission, not
before computing that invocation's text matrix.

`tickItemSelectionHelp` checks transition4 first, measures the previous display's
non-null text and publishes LFcount+1. Null text preserves prior line count.
For more than two lines, horizontal trigger100 decrements page2 with zero floor
and precedes trigger200. Forward paging uses the exact signed quotient bound
`(lines+1)/2-1 > (page+1)/2`; it is not a pixel-height or wrap calculation.
Help anchors retain fused28*page-144, arrow/page gates and separate icon120.
Header/help draw colors require the font service's mutable palette0; initial
1e1414ff is not substituted for later palette changes.

`registerDisplay` queues later rendering. A synchronous draw from inside this
callback would change prior-display help publications and header mutation
ordering. The display service accesses its retained slot through `displayWindow`
for the original help-text/header writes. Font measurement/markup, font outline,
scissor/GX/PICA state and actual icon resources are distinct required services.
Do not replace them with message-bubble parsing, reveal waits, atlas-cell widths
or authored item strings.

## Validation and remaining work

Tests cover opening-slot asymmetry, transition input suppression, original
duplicate row identity, A-before-B, cancel prohibition, close-before-ready,
inactive later-slot suppression, unsigned65535 result retention and ready-only
release. Draw checks cover all-row clipping/easing anchors, negative low16
heading center, header post-draw mutation/active gate, mutable palette, list
blink/page gates, help icon120 and source horizontal paging against prior text.
These are contract-based host checks, not original execution or GPU captures.

Next: bind original window-font scanner/metrics/draw services, native deferred
rendering, source pad/GP clock/status/sound owners, shared manager allocation,
event key control and original pouch/switch persistence. Then execute the
Goomba4 quest graphs with unchanged message IDs/speakers/branches and verify
source/runtime/New2DS observations. No full-selector, quest, original raster or
hardware performance completion claim is made here.
