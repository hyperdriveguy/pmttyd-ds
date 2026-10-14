# Opening map turn capture49/50

The [story contract](../story/OPENING_MAP_DISPLAY.md) owns the map event prelude and transfer conditions. Original bodies/model/dispatch are pinned by [the read-only verifier](../../../tools/verify_opening_map_display.py); [the asset exporter](../../../tools/opening_map_transition_assets.py) preserves the source geometry and material metadata.


The used transition is now bound by `OpeningMapTransition` and
`render_opening_map_transition.cpp`. Source fadeEntry table49→80013944 selects
`OFF_d_meku/Z_1/context2`;50→80013960 selects `A_1/context2`. Z_1 has one key
and span0; A_1 has45 keys and finite span80. Type50 explicitly preserves work
slot156, the old capture callback pointer. The exporter retains the original
88 triangles, hierarchy, UV, vertex colors and pass2/cull1 in source order.

Source49 allocates only img capture slot0 for `fImg_peron/fOff_peron`, full
rectangle(0,0,608,480), mode2. The raw model contains kinds[2,1]. Its draw order
is44 kind1 triangles followed by44 kind2 triangles, with no texture or
visibility changes in the45-key clip. `imgDisp` leaves capture slot1 absent;
`animSetPaperTexObj` publishes first/second pointers at anim work+48/+52 and
uses capture0 to publish both active flags+216/+220. `renderProc` kind2's null
pointer branch8003cd00..cd78 skips `GXLoadTexObj`, retaining the actual sampler
and UV matrix bound by the preceding kind1. The native used branch explicitly
retains that inherited capture0 sampler, with kind2 metadata preserved; it
neither invents nor acknowledges a second captured resource. This is a bounded
used-order bridge, not a rule for other dynamic material domains.

A512x256 VRAM texture target receives the actual opaque map rendering with an
untilted400x240 viewport. The native screen rectangle48..352×0..240 adapts the
source full608×480 capture; it is independent of Voyage letterbox/world cropping.
The completed map frame ends and synchronizes before captureReady publication.
Source Z_1 then draws from that actual texture and its final frame0 must submit
and synchronize before MapDisplay transfers to Voyage. The capture and original
pose pack stay resident through type50; original A_1 geometry draws over the
new Voyage world for80 simulation ticks, and its final frame80 also requires
actual GPU submission acknowledgment. A native image-plane projection adapts
the original64×48 geometry to304×240; this is not a measured original raster.

The source used muj init branch camera/map/control producers and
`evt_prologue` have no fadeEndwait. Voyage starts on actual new-scene admission
while type50 animates concurrently; entryComplete owns presentation/resource
lifetime and does not add a story wait. Root integration retains the independent
ship base producers during scene admission. Its later acceptedTick+240 command
33c58 requests group512/6000ms off before17/800 and16/800 mapchange requests.

Before the first Voyage frame, the bounded native provider stages its existing
world map resources through `prepareFrontWorld` outside `C3D_FrameBegin`. The
already presented map capture therefore remains on screen during cold resource
IO; the new frame is cleared only after staging finishes. This changes resource
service ordering, not source animation clocks or a completion acknowledgment.
