# Camera letterbox and dummy draw services

US G8ME01 revision0, read-only DOL inspection2026-10-14. Completes the service
boundaries left by [camera manager](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md)
and refines [camera8 save lock](SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md).
Static source evidence; no original execution or GX/PICA raster equivalence.
Names were resolved against the existing US symbol catalog, while instructions,
constants and startup clear ranges were reread from the user's read-only ISO.

## camLetterBox8000e388/1dc admission and mutations

Always capture camera8 from camPtrTbl803d95f8+20 and request partyGetPtr(0)
800bf1b4 before any flag admission. This is registry slot0, not a lookup by
partner kind/current index. The captured pointer remains live for the later
word0 read. A null pointer is an actual result; it is not unavailable service.

Read camera8 flags unsignedhalf0. If mask200 set, bypass automatic decisions
directly to animation at8000e464. Otherwise request bero_get_BeroEXEC8007ac40;
its leaf reads retained word SDA+1af4, absolute8041ea14. Return lowbit1 also
bypasses automatic decisions. Neither bypass suppresses animation/registration.

Otherwise request seqGetSeq8002e210. If return2, request marioChkKey8005c47c.
If that returns nonzero, request marioChkCtrl8005c464; a nonzero Ctrl result
takes the disable route. If Key returns0 or Ctrl returns0, inspect the captured
party pointer: null qualifies for enable; nonnull reads its live word0 and
mask100 set takes disable. The other sequence values take disable directly.

Enable: freshly read camera8 flags; if mask100 already set, skip initialization.
Otherwise set100, request psndSetFlag800dc110(mask20), then store signedhalf
alpha1e0=255, top1e2=0,bottom1e4=0, word drawCamera1e8=8. The sound call
precedes all these initializers. It is a sound-global flag change, not an icon.

Disable: request seqGetSeq AGAIN. Result3 preserves existing100/sound state;
any other result clears100 and requests psndClearFlag800dc0f8(mask20), even if
100 was already clear. Do not collapse two sequence reads into one snapshot.
The sound setters mutate retained halfword803df73e. ClearFlag only AND-clears;
SetFlag ORs then tests the **accumulated** bits1/2, requesting BGM/environment
Off_f_d for groups0/1 when those bits are set. With both bits clear, adding20
requests no additional sound service. No music restart belongs in this helper.
See [global owner and live side-effect gates](CAMERA_LETTERBOX_GLOBAL_PRODUCERS.md)
and the [original sound routing contract](../audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).

Animation freshly reads flags0. Enabled100 stores alpha255, adds3 separately
to each signed bar, stores to halfword, reloads/sign-extends, then caps top49
and bottom44. Preserve store-before-comparison wrapping. If fresh flags400,
clear400 and store top49,bottom44 immediately.

Disabled100 reads signed alpha and GP signed word4, computes signed integer
step=2*(255/GP4), subtracts/storehalf/reloads, then clamps negative alpha0.
The original has no zero-divisor guard; an unavailable/exceptional native rate
needs an explicit arithmetic adapter/provider boundary. If fresh flags400,
clear400 and store alpha0; bar amounts are not zeroed in this disabled route.

Always request dispEntry80010bc4(drawCamera1e8,renderClass3,z+0,
callback8000e564,param0). The registration occurs even for invisible bars.
z literal8041f6b4 is00000000. Class3 and null parameter are source arguments.
Retain continuation across the sound calls/queue capacity; do not repeat flag
changes, bar increments or initializers on retry.

## camLetterBoxDraw8000e564/284

The queued callback ignores its passed camera/parameter and independently
fetches camera8. It returns before any GX request if GP.word0&8000 or its
signed alpha1e0==0. Negative alpha is not this equality early-out. Therefore
queued drawCamera and the matrices actually used can differ.

For an admitted callback, exact service order:

| Order | Request and arguments |
| --- | --- |
| 1 | GXGetViewport(out6floats), GXGetProjectionv(out7floats), GXGetScissor(out4words), in that order |
| 2 | GXSetProjection(camera8.projection15c,camera8.kind19c) |
| 3 | GXSetBlendMode(1,4,5,0); GXSetZCompLoc(1); GXSetAlphaCompare(7,0,0,7,0); GXSetZMode(0,3,0); GXSetNumChans(1) |
| 4 | GXSetChanMatColor(4,RGBA), where RGB reads retained startup word80429510 and alpha is low8 of signedhalf1e0 |
| 5 | GXSetChanCtrl(4,0,0,0,0,0,2) |
| 6 | GXSetFog(0,+0,+0,+0,+0,RGBAffffffff from8041f6b0) |
| 7 | GXSetNumTexGens(0); GXSetNumTevStages(1); GXSetTevOrder(0,255,255,4); GXSetTevOp(0,4); GXSetCullMode(0) |
| 8 | GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxAttrFmt(0,9,1,3,0) |
| 9 | GXLoadPosMtxImm(camera8.view11c,0); GXSetCurrentMtx(0); GXBegin(128,0,8) |
| 10 | Submit the two quads below, direct signed16 XYZ positions |
| 11 | GXSetViewport(saved6floats); GXSetProjectionv(saved7floats); GXSetScissor(saved4words), in that order |

Top quad: (-304,240,0),(304,240,0),(304,240-top,0),(-304,240-top,0).
Bottom quad: (-304,-240+bottom,0),(304,-240+bottom,0),(304,-240,0),
(-304,-240,0). Bars are live signedhalf reads at the individual vertices.
The callback restores only viewport/projection/scissor. It leaves material,
TEV/channel/vertex/cull/depth states as emitted; no whole-state restore is source.

Word80429510 is .sbss2 camdrv.o, with no ISO file payload. Original startup
__init_data clear table800055b0 contains range80429500:size40c, which covers
this word and produces startupRGBA00000000. Thus ordinary boot RGB is black;
do not try to read uninitialized BSS from an ISO offset. Live mutation through
indirect writers is not excluded by this bounded static recovery.

## sysDummyDraw80005ad8/20c complete typed request sequence

This helper receives the current camera view3x4. It has no admission predicate,
uses no camera registry itself, and restores no state. Exact arguments/order:

| Order | Request |
| --- | --- |
| 1 | GXLoadPosMtxImm(view,0)802ba2fc; GXSetCurrentMtx(0)802ba39c; GXSetNumChans(0)802b6f3c |
| 2 | GXSetNumTexGens(1)802b5084; GXSetNumTevStages(1)802b8fcc |
| 3 | GXSetTexCoordGen2(0,1,4,60,0,125)802b4e04 |
| 4 | GXSetTevOrder(0,255,255,255)802b8e30 |
| 5 | GXSetTevColorOp(0,0,0,0,1,0)802b89e8; GXSetTevAlphaOp(0,0,0,0,1,0)802b8a50 |
| 6 | GXSetTevColorIn(0,15,15,15,2)802b8960; GXSetTevAlphaIn(0,7,7,7,1)802b89a4 |
| 7 | GXSetZMode(0,7,0)802b93d8; GXSetColorUpdate(0)802b9380; GXSetAlphaUpdate(0)802b93ac |
| 8 | GXSetBlendMode(1,0,1,7)802b932c; GXSetZCompLoc(0)802b940c |
| 9 | GXSetAlphaCompare(6,128,1,0,0)802b8d60 |
| 10 | GXSetTevColor(1,RGBA00000081)802b8ab8; value reads initialized word8041f440 |
| 11 | GXClearVtxDesc()802b4520; GXSetVtxDesc(9,1)802b3c08; GXSetVtxDesc(13,1)802b3c08 |
| 12 | GXSetVtxAttrFmt(0,9,1,4,0)802b4558; GXSetVtxAttrFmt(0,13,1,4,0)802b4558 |
| 13 | GXBegin(144,0,3)802b5a6c; direct float FIFO triangle below |

Vertex1 position(0,0,0), UV(0,0); vertex2(20,0,0),UV(0,0);
vertex3(20,20,0),UV(0,0). Each0 is original float8041f448=00000000;
20 is8041f454=41a00000. FIFO writes occur XYZ then UV per vertex. These
are actual vertices, not a screen clear or substitute fullscreen triangle.
All color/alpha/depth writes are disabled. The source does not request fog or
cull resets here. Preserve inherited state outside the listed setters.

Typed GX adaptation can implement this declared no-write triangle/state change
with citro3d/PICA, but source projection conventions, write masks, descriptor
semantics and inherited state require a real backend. A no-op falsely reports
service completion and removes its effect on following cameras.

## Current native binding boundary

Existing Game.fieldPartyState().pointer(0) and actor.word(0) can satisfy the
first letterbox registry read within the existing two-slot native domain.
FieldSaveObjects.camera8Flags owns save lock200, but needs wiring to the same
retained camera8 flags record; it is not the camera4 field-follow flag word.
The first missing automatic-decision producer after that registry read and
clear200 is retained BeroEXEC8041ea14. Do not map Scene::Dialogue/Battle or
inputBlocked to this transition-execution word. Next source owners are the
sequence-manager current ID, original Mario Key/Ctrl state, GP4 timing and
sound-global halfword. marioChkCtrl's complete leaf returns signed Mario
byte38==0, not the native generic controlsEnabled predicate. The original
Key query has its separate multi-condition contract.

With these owners supplied, all scalar mutations and registration are now
documented, including suspended-call boundaries. Existing shared display queue
can retain the real borrowed callback request, but neither that registration
nor a typed GX request list proves the actual native callback rasterized.
Dummy draw's first unavailable backend service is GXLoadPosMtxImm(view,0),
after current camera dispDraw has completed; do not skip other callbacks to
reach it. Current finishing camera admission still depends on full retained
camera/outer-loop history and actual providers.

## Evidence and validation

Complete byte-span SHA256, freshly reread from ISO:

| Span | SHA256 |
| --- | --- |
| camLetterBox8000e388:1dc | 39975eb964ae3b22ed38dff6f9687a532e35028ae03b24d0216713c10ab65832 |
| camLetterBoxDraw8000e564:284 | 9cfe7c12742dc5aee4d8a4417a1f52584a56fea387c76c6fc34361087d04f585 |
| sysDummyDraw80005ad8:20c | 6773555033b684d8a4d4cad3453d3132620af908a37921aeb0a09a2ef8f90a4d |
| camEffMain8000e974:bc | 32dd8b70585586f665c3b572734b2128bdfa1006aea4f0d2b979f766298dfb36 |
| camImgMain8000ea30:38 | 0679b8532ac8913884fb921f4b02f76e8cb858cad4e1a03e3536a9affe8ae4ce |

camEffMain has no flags admission. It copies camera4 eye0c,target18,up24 to
the passed effect camera, writes its distance118 via PSVECDistance8029eeb4
and yaw114 via angle8000703c(eyeX,eyeZ,targetX,targetZ), then rebuilds
perspective15c from that effect camera's retained FOV38,aspect3c,near30,far34.
camImgMain has no flags admission and only rebuilds perspective15c from its
own retained lens; it does not write distance or yaw. These callbacks are
separate services preceding camMain's view/roll/translation/extra publication.

Ignored listings remain artifacts/reference/camera-manager/. No original
instruction/asset payload was added to source history. Documentation-only:
no make/build/emulator/hardware result asserted. Native GX/PICA backend and
live global producers remain implementation work.
