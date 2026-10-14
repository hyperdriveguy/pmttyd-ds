# Ordinary battle part callback: matrix, material, passes and history

2026-10-14, original US G8ME01 disc0 revision0, read-only DOL evidence.
EVT-01 callback frontier after the retained queue invokes80101774. The complete
body is btlUnitPartsDisp80101774:d7c. Existing
[trio matrix contract](../battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md#final-used-ordinary-goomba-death-draw-path)
closes much of the ordinary algebra; this contract corrects its depth-bias
placement and adds mandatory material/pass/history operations. No original
execution, native callback completion or raster equivalence is claimed.
Addresses/offsets/masks are hexadecimal, matrix indices decimal.

## Used finite admission and first actual provider

Callback r3=camera,r4=borrowed source part P. Incoming camera is not retained
as the ordinary matrix camera. Read signed P1c0; negative branches epilogue
801024ac before matrix/material/history work. Capture global B and ownerU=P4ec
before that branch. Nonnegative enters real SDK matrix providers; first at
801017ec is PSMTXTrans8029e230(stack220,−U78,−U7c,−U80).

The admitted original prologue body domain has part P1ac20000000clear,
P4fc0 (rotation order), ordinary valid pose owner, and owner U10404000000clear.
Actual hidden Mario extras already suppress their queue entry; their absence
from this callback is an original flag gate, not deletion of their pose owners.
Nonzero P4fc, special part20000000 and owner04000000 admit other source paths;
retain those as explicit providers rather than substitute ordinary algebra.
The body does not redo animation Main or status-pose selection.

## Ordinary matrix construction and live facing

Initial SDK requests build unit translations T(−unitPivot),T(+unitPivot),
T(unitDisplay),T(unitWorld), unit rotations Rx/Ry/Rz and scale. ScaleX is
R(U114*R(U9c*U90)), ScaleY=R(U114*R(Ua0*U94)), ScaleZ=R(Ua4*U98).
Initial left concat order is scale,−pivot,Ry,Rz,Rx,+pivot,display, yielding
UnitM=T(display)*T(pivot)*Rx*Rz*Ry*T(−pivot)*S.
UnitWorld is built separately from U3c/40/44; it is not a substitute for the
later live part-world construction. No zero-pivot/angle/scale shortcut is local.
Use [SDK operation order](BATTLE_CAMERA4_MATRIX_PUBLICATION_ARITHMETIC.md).

At ordinary branch80101c48 choose d=270 if signed U189<0, else90. Relative X
direction uses Pbd: positive→+1,negative→−1,zero→signed source side-byte from
captured B+8*S8(Uc)+a. Compute worldX=FMA(P24,direction,U3c).
Call GetFloatDispOffset80100ed0(P), retain result; call
GetGravityDispOffset80100ffc(P). Then worldY is successive rounded additions
`((U40+P28)+gravity)+float`; worldZ=R(U44+P2c). Phase getter operands are live
P1a8/P1a6, not a cached constructor zero. They use the original sine-degree
service80125cd4, which stays a declared numeric provider if not executed.

Ordinary owner04000000clear keeps cameraCorrection+0. Build part pivot
translations, rotations from base+current part angles (Y includes correction),
part scale R(P6c*P60)/R(P70*P64)/R(P74*P68), world translation and part-display
translation. Incoming animation angle A is reviseAngle(FMS(−1,d,90)).
When owner U1c01000000clear, revise the ordered sum
`A+U70+P4c+U64+P40`; call camGetCurPtr8000f2e4 then angleABf8000703c on
live camera eye/target XZ, add/revise. Inclusive90..270 gate rebuilds the unit
matrix with NEGATIVE Z scale; X/Y retain their scales. This current camera
owner is distinct from the fixed camera4 queue-depth getter.

For actual P4fc0 the final ordinary DRAW matrix is:

```
Mdraw = T(partDisplay)*T(worldPart)*T(partPivot)
        *RxPart*RzPart*RyPart*T(−partPivot)*Sp*UnitM
```

Source part concat order is Sp,−pivot,Ry,Rz,Rx,+pivot, then world and display.
All math requests and their retained arguments must complete before material
publication. Numerical adapters may represent SDK matrices with native arrays,
but generic reordered algebra is not proof of original rounding/alias behavior.

## Correction: −5 belongs to history, not ordinary DrawMtx

8010229c writes worldPart*partMatrix into stack520;801022ac writes
partDisplay*that into stack520. At801022b8 **PSMTXCopy(stack520,stack4c0)**.
801022cc builds T(0,0,−5), then801022dc writes that*stack4c0 into stack4c0.
All three animPoseDrawMtx sites use **stack520** as r4, specifically80102434,
8010244c,80102464. Final history request uses **stack4c0** as r5 at80102498.

Therefore ordinary Mdraw has NO added−5; Mhistory=T(0,0,−5)*Mdraw. Older
trio prose describing−5 as part of the final animation draw matrix is contradicted
by the actual operand addresses. Do not add it to the rendered character model
or collapse these two separately retained matrices. Special20000000 path is
outside this correction's ordinary admission and retains its own operations.

## Mandatory material publication and pass requests

After matrix construction801022e0 copies WORD P4f4→P218 (full RGBA).
Fresh P204 masks10 then20 each independently zero bytes P218/219/21a;
alphaP21b is untouched. If all three resulting RGB bytes equal255, call
CheckStatus80127814(U,4). Nonzero then:

1. R=integer(R*160/255), store P218;
2. RELOAD new R, B=integer(R*160/255), store P21a;
3. Leave G unchanged.

Thus source white becomes(160,255,100), not(160,255,160). In that same
initial-all-white branch, regardless of status4 result, scale each RGB byte
independently by freshly reloaded U312, dividing255 and storing R,G,B in order.
If RGB was not allwhite at initial gate, skip this entire status/fade block.
Part alpha from Main is preserved rather than multiplied by U312 here.

Copy current P218 word to local color; call animPoseSetMaterialColor800406b4
(handle,&color) at801023cc. Its real local body writes raw word to poseF0.
Call animPoseMaterialFlagOn80040730(handle,40) at801023d8; it ORs poseE8,
not poseEC. These leaf operations reload the global pose-pool pointer and
perform no ID/active/null validation. A typed native bound pose owner should
publish these actual scalar fields, not only cache a color for a sprite API.

Fresh P210!=0 invokes its actual callback(P,0) at801023f4. Then
CheckStatus(U,16): nonzero OR poseE8 mask800000 through80040730;
zero clears only that mask through80040710. No invented status/material reset.
Next call animPoseDrawMtx8003b4d8 in order passes1,2,3, each with freshly read
P1c0, SAME Mdraw, retained A, scale literal2.0. Wrapper resource/link/pass gates
are in [DrawMtx admission](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md).
MODELwordc4=1 admits pass2 by its special rule and rejects passes1/3, but use
actual retained model bits per resource, not a universal class1 shortcut.
Admitted inner geometry/material/GX services remain required.

Fresh P210 is checked AGAIN after the three returns; nonnull invokes(P,1).
Do not cache its pre-call pointer for post-call admission. Even when all passes
reject, local material stores/callbacks/history still occur in source order.

## Mandatory ten-record history publication

After the optional post callback, copy current P218 word and call
801014f4(P,0,Mhistory,&copiedColor,A) at801024a8. This wrapper reads P1ac
mask04000000; set forwards mode1, clear mode0 to80101428. It always copies
provided color into a local word and invokes the inner history owner.

Let H=P21c. H0/4/8 are header words; ten68-byte records begin at Hc + i*44,
i=0..9. Inner80101428 shifts records8→9 down through0→1, in descending
order. Each copy performs eight ordered pairs of word loads/stores plus a final
word, not memcpy over an unrelated rendered pose sample. Then publish record0:

| Offset from H | Stored value |
| --- | --- |
| c | supplied mode0/1 |
| 10..3f | PSMTXCopy(Mhistory)48 bytes, actual service8029dd30 |
| 40 | retained angleA |
| 44 | provided copiedRGBA |
| 48 | fresh header H4 |
| 4c | fresh header H8 |

Finally if H0 bit1 set AND H50 bit1 set (the previous record0's mode now in
record1), clear ONLY bit1 in new record0 modeHc. This header/history condition
is not permission to skip copying. The occupied part range ends immediately
before P4d0, the separate pose-sound table. No geometry/GX dispatch occurs
inside this history leaf. Its retained CPU owner is still a real required
service before the callback can return and queue traversal can advance.

## Evidence and native binding plan

Private original disassembly: artifacts/battle-display-queue-evidence.
Complete callback SHA25680101774:d7c:
b547dee7f1b57c54d12268e328f52a02d0fe0f6c223b05da62aeb93dbfd7e87a.
Wrapper801014f4:54 SHA256:
fe009e6b4b191577b3b2ecfa161c34ec324accf07ba941a43916b82007113b97.
Original material leaves800406b4:1c,80040710:20 and80040730:20 were reread;
retained offsetsF0/E8 follow their actual instructions.

A source-backed finite native callback can retain P/U/B, ordered matrices/A,
material stages, three wrapper-pass cursors, optional callback providers and
history copy cursor. Use actual converted pose outputs/resources for admitted
rendering and actual current-camera services; keep unsupported special/linked/
inner material/geometry domains explicit. The legacy scene billboard cannot
acknowledge these stores/providers by drawing a visually plausible model.
Neither an empty audience nor a completed queue registration closes this part
callback, battle return or full prologue fidelity.
