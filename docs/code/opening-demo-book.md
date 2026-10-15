# Demo narration, marketplace and captured book

Implementation: [demo](../../source/story/opening_demo.cpp),
[marketplace](../../source/story/opening_demo_scene.cpp),
[book clock](../../source/story/opening_book_scene.cpp).
APIs: [demo](../../include/story/opening_demo.hpp),
[marketplace](../../include/story/opening_demo_scene.hpp),
[book](../../include/story/opening_book_scene.hpp).
Evidence: [source requests and adapters](../behavior/story/NATIVE_OPENING_DEMO_PRESENTATION.md),
[book/capture contract](../behavior/story/NATIVE_OPENING_BOOK_PRESENTATION.md),
[cover geometry](../behavior/display/NATIVE_OPENING_BOOK_COVER.md).
This guide describes current portable controllers, not complete original event,
camera, audio or GPU execution.

`OpeningDemo` retains book IDs `op_book_01..24`, then market `op_00..12`, with
original speaker tokens in their authored order. Missing/unsupported text fails,
without substituted dialogue. Individual loaders publish separately; a failed
compound load does not roll back earlier successful dependencies. Admit the whole
bundle before beginning. Public demo time is milliseconds; book/market/audio use
explicit floor(ms*60/1000) simulation ticks. These are native clock adapters.

Narration starts after the source 4000 ms brother wait. Book scaling advances
independently: integer elapsed milliseconds feeds `(600*(1-q)+1500*q)/1000`,
`q=elapsed/12000`, capped at 1.5. Third narration waits boot+780 ticks (13 s),
separate from the scale clamp at 720. Book04 starts selector-11 cover rotation
0..1800/2000 ms divided by ten, followed by 1000 ms; portable deadline is 3000 ms.
Do not turn this into a linear rotation or a new measured duration.

| Paper index | After narration | Old image | Depth |
| --- | --- | --- | --- |
| 0 | book08 | 8 | 6 |
| 1 | book12 | 9 | 5 |
| 2 | book16 | 10 | 4 |
| 3 | book21 | 11 | 3 |

Registered paper services require actual capture completion before starting the
180-tick clock and selecting next image 9+index. `presented()` then needs elapsed
180 **and** finite evaluated frame >=180 before page completion. The next 1000 ms
wait starts only after that acknowledgement. Completed old paper remains until
the following paper completes; final adjustment releases the last retained pages.
Unregistered mode preserves a provisional static-page wait and does not prove
source paper completion. A renderer must publish acknowledgements only after
actual resource/GPU work; these APIs cannot prove that external claim themselves.

After book24, market and BGM transition start alongside 180-tick book adjustment:
integer fade over 3000 ms, then scale `1.5+1.5*sin(pi/2*fade)` and Y offset
`-105*sin(pi/2*fade)`. Final paper receives separate `T(offset/10)*S(scale/1.5)`.
The portable marketplace retains request order/endpoints/pose tokens and rounds
millisecond waits upward to 60 Hz ticks. Camera/actor linear interpolation and
heading/hypot/trigonometry remain native adapters to the requested source behavior.

Marketplace slots 0/1/2 are Peach, merchant and chest, not field registry IDs.
Camera requests start parallel motion without blocking the root; actor move,
lift, wait and exact message gates block it. Point.Y carries move speed, while
actor height is preserved. Reset keeps the admitted angle table. Sparse advances
replay each missed tick; repeated same-tick calls do not repeat handoff additions.
The sixty transfer iterations retain X+=85/1024, Y+=170/1024 in order. Chest
tracking, Peach movement and merchant/root work remain separate owners.

Tribe indexes choose original Peach idle/talk/walk triplets. Merchant's source
request W_1 remains recorded, while rendering uses original descriptor-zero Z_1
fallback. Wall group bits are scene-local. Chest opening requests crystals at
XYZ+10Y, Peach reactions at 18/36 ticks and radiation at 30 ticks; effects continue
under the final title wait. No generated pose, effect or dialogue is introduced.

Exit request is not controller completion. In the current `tick()` implementation,
BGM and the book/market/effect/message clocks continue until `exit.finished()` sees
actual opaque final presentation. This method does not freeze them merely because
`requested` became true. That description is of the native adapter, not a claim
about complete original map-change scheduler behavior.

Book geometry helpers: [cover](../../include/story/opening_book_cover.hpp),
[view](../../include/story/opening_book_view.hpp),
[capture](../../include/story/opening_book_capture.hpp). Three source cover passes
retain TPL2 stationary/mirrored/back-cull/alpha, TPL1 rotating/normal/back-cull/
opaque, TPL2 rotating/mirrored/front-cull/opaque. Hinge is `-width/2+8`, followed
by negative Y rotation, local scale and offsets. Page TPL3..7 uses .95 dimensions;
illustration TPL8..12 has its own rectangle and no hinge. CPU projected winding
is admission only; actual XYZ/camera/GPU services remain external.

Native half-scale geometry is centered in 400x240, with source 608x480 content
corresponding to 304x240. Framing uses source settled bars 49/44, explicit pan to
center the opened spine and native zoom/overflow compensation. Capture is 193x240
inside a 256-square target, transformed in homogeneous clip coordinates before
dividing by W. Paper uses source 64x48 plane/crop124/0/386/480; folded-back scale
preserves portrait art, and depth reference16/inverted Z is native layering policy.
These adaptations do not rewrite source mesh/UVs or establish SDK numeric/pixel
parity. See [title/audio/effects](opening-title-effects.md) for adjacent owners.
