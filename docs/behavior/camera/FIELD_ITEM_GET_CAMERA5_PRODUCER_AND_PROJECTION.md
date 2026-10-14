# Field item-get camera5 producer and projection

US G8ME01 revision0; read-only DOL recovery2026-10-13. Finite ordinary field
item95 owner, original camInit/camMain/camEffMain/camDraw requests. Complements
[camera4 lifecycle](OVERWORLD_CAMERA_LIFECYCLE_AND_PROJECTION.md) and
[type6 draw state](../effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md).
No original execution, raster equivalence or native hardware measurement.

## Camera5 is independently initialized

camInit8000fd9c calls camEntryPersp8000fac8 at80010438, stores the returned
pointer into camera table803d95f8+14 at80010454. The callback argument is
camEffMain8000e974, name string802bf650 is `3deff_A`.
Original six float arguments, freshly read from the DOL:

| Entry argument / retained field | Literal / source word |
| --- | --- |
| lensFOV / +38 | 25 /8041f6cc=41c80000 |
| aspect / +3c | 1.2666666507720947 /8041f6d8=3fa22222 |
| near / +30 | 100 /8041f6d4=42c80000 |
| far / +34 | 10000 /8041f720=461c4000 |
| viewport nearZ / +10c | .20000000298023224 /8041f6f8=3e4ccccd |
| viewport farZ / +110 | .30000001192092896 /8041f724=3e99999a |

camEntryPersp initializes projectiontype+19c=0 and builds C_MTXPerspective
at8000fb6c. It zeroes roll+14c and view offsets+150/+154/+158,
callback+1ec=camEffMain, halfword flags0/2/4=0. ScissorXY and viewportXY
are0. ScissorW/H and viewportW/H come independently from original render-mode
framebufferWidth/EFBHeight through DEMOGetRenderModeObj; ordinary reviewed
NTSC Mario descriptors are608x480. Depth-range 0.2..0.3 is a source viewport
request, not a modified world-space near/far pair.

Constructor eye/target/up copy original SDA vectors: camPtDefault80414fb0
(0,0,1000), target80414fbc(0,0,0), camUpDefault80414fc8(0,1,0), freshly
checked read-only. Camera5 later replaces all three with camera4 vectors; up
is an explicit vector input, not inferred from a post-roll camera4 matrix.
Viewport fields are +fcX0,+100Y0,+104W608,+108H480,+10cnearZ.2,+110farZ.3
in the reviewed original NTSC mode. Scissor halfwords are +f4X0,+f6Y0,
+f8W608,+faH480. Screen dimensions remain original render-mode inputs.

Camera4 instead initializes near1/far32768, depth0..1 with the same default
lens/aspect. Sharing its final native projection/depth range with camera5
would lose these explicit original values. Camera5 has its own retained lens;
this constructor/callback does not dynamically copy camera4's altered lens.

## Same-manager-invocation vector copy and independent view

camMain8000f704 visits camera table indices0..12 ascending. For each record,
first invokes callback+1ec if nonnull at8000f770, then constructs its view.
Camera4 therefore finishes its update/view before camera5's callback begins.
The callback camEffMain8000e974 reads table+10 (camera4pointer), then copies
these exact vector words to the current camera5 record:

| Fields | Copy instructions |
| --- | --- |
| eye+0c/+10/+14 | 8000e99c..8000e9b0 |
| up+24/+28/+2c | 8000e9b4..8000e9c8 |
| target+18/+1c/+20 | 8000e9cc..8000e9e0 |

Then PSVECDistance8029eeb4(eye,target) stores distance+118 at8000e9e8.
angleABf8000703c(eyeX,eyeZ,targetX,targetZ) stores yaw+114 at8000ea00.
Finally C_MTXPerspective8029e7e8 builds projection+15c using **camera5's**
retained lens+38,aspect+3c,near+30,far+34 at8000ea18.
No camera4 viewmatrix, projectionmatrix, roll, viewport/depth range, lens,
near/far, distance or yaw word is copied; the last two are recomputed.

After callback return camMain constructs C_MTXLookAt(camera5+11c,
eye+0c,up+24,target+18) at8000f784. It left-concats rollRz and view-offsetT
at8000f7bc/8000f7cc. For constructor-default roll/offsets0, the mathematical
view is just LookAt of camera4's copied vectors. Original helper rounding
and identity-concat instructions still execute; native bitwise equivalence
is unproved.

Only camera index4 enters camMain's additional Mario zoom/screen correction
block8000f7d0..8000fa50. Camera5 skips it completely. Thus even though ordinary
zero-offset/no-zoom views can mathematically match, copying camera4's **finished
viewmatrix** is not the original producer and diverges when that correction runs.
Camera4 road selection/target/follow changes are visible through the copied
vectors; full camera4 road-selection parity remains the existing frontier.

camMain's flags8 projection-extra concat block applies to any camera only when
that flag is set. Camera5 constructor flags0=0; finite default has no such
request. External camera event writes, lens/roll/offset changes and persistent
projection customization remain independent owners; do not assume defaults
for arbitrary scenes or saves merely from this finite constructor evidence.

## Field item95 does not rewrite camera5

Original [itemseq_GetItem](../field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md)
800ab930/1324 was freshly reread. Finite mode16/main1003 selects normal kind95
presentation, retains the type6 effect and overwrites effectpayload+38=5.
This selects draw camera5; it does not set camera5's projection or view.
The sequence's camGetPtr calls at800abca0,800ac460,800ac5ec,800ac964,
800acb1c all use ID8 to set/clear its flag200. The successful finite path sets
camera8 at800abca0 and clears it at800ac460; no camera4/5 state rewrite
occurs through these calls. Changing ordinary icon cameraID to4 at teardown
is a separate icon/request owner, not a camera-projection rewrite.

The item owner runs after field camMain in the existing simulation order.
An effect newly admitted later in that tick consumes the already-prepared
camera5 when display runs; invoking effect draws must not advance/rebuild
camera clocks independently. Paused/special outer scheduling still requires
its own producer; this contract does not invent that timing.

## Projection/viewport request and native boundary

camDraw8000f454 visits active camera slots ascending. For camera5 it requests:
viewport(camera5+fc..110), scissor(camera5+f4..fa), then
GXSetProjection(camera5+15c,camera5+19c) at8000f504 before dispDraw(5).
Conditional flags40/type0 can call camEvalNearFar and rebuild perspective,
but constructor flags0 leave this branch unselected in the finite default.
Effect callback consumes camera5yaw+114 and view+11c; it does not install a
scene projection. Its separate LightOrtho remains exclusively projected ST.

A native finite camera5 adapter should therefore consume source camera4
prepared eye/up/target vectors, rebuild source LookAt and its own perspective
lens25/aspect0x3fa22222/near100/far10000, and adapt the explicit viewport 0.2..0.3
through the native depth convention. Source viewport and NTSC dimensions can
be adapted for the3DS screen presentation, retaining a documented projection/
aspect choice; arbitrary existing native field near/far/depth is not proof of
this source owner. Restore the following native consumer's camera/depth state
because source camDraw would independently install each camera's settings.

Source SDK perspective/LookAt/angle/numeric behavior, all external camera5
writers, flags40 near/far evaluation, original camera4 road math, GX-to-PICA
clip/depth conversion and raster/hardware comparison remain bounded unknowns.
The admitted camera5 producer/defaults are recovered; full shared scene parity
is not established by this document.

Fresh ignored artifacts under artifacts/reference/goomba3-services/:
camInit-item95,camEntryPersp-item95,camMain-item95,camEffMain-item95,
itemseq_GetItem-camera-review and camDraw. ISO remains read-only, no original
assets/code/binary history additions, no emulator profiles or save changes.
