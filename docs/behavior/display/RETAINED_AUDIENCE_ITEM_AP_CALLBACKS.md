# Retained audience item and AP callbacks

Original US G8ME01 rev0 source callbacks DispItem801a37f0/108,
DispApSrc801a36c8/128, GXInit801a4408/168. Native local bodies now retain their
actual call order/arguments across unavailable GPU, matrix and icon services.
The original full audience Init clears A/13914 bytes; later producers retain
that same bank. Inactivity must be read from each actual bank at draw time.
An empty200-actor bank does not establish either100-entry bank inactive.

DispItem first calls GXInit (including its unused fresh BaseGetPtr), then
ItemGetPtr0 (independent current BaseGetPtr) and retains returned A+f25c. Scan
exactly100 records, stride48 hex. Load each flags word once and require both
bits1 and2. Rejected records have no item, pose, camera or icon calls. Never
bypass graphics setup because no item draws.

DispApSrc captures BaseGetPtr A, asks camera8 (return unused), calls GXInit
(which independently asks BaseGetPtr), then performs Blend(1,4,5,0),
ZCompareLocation1, AlphaCompare(7,0,0,7,0), ZMode(1,3,0). Scan100 at captured
A+10e7c, stride60 hex; bit1 alone admits, including alpha0. No current-record
queries or population-count gates exist in these scans.

GXInit issues18 setters: Cull0,ClearDesc, direct Position9/Color11/Tex013,
formatsXYZ/F32,RGBA/RGBA8,ST/F32,NumChans1,ChanCtrl(4,0,1,1,0,2,2),
NumTexGens1,TexCoordGen(0,1,4,60,0,125),NumTevStages1,ColorOp add/clamp,
ColorIn(0,15,10,8,15),AlphaOp add/clamp,AlphaIn(0,7,5,4,7),Order(0,0,0,4).
Both RGB and alpha multiply texture by vertex channel color. No material,
texture, camera/projection, blend or depth write exists in this helper. Native
shared shader owner retains this distinct equation and vertex material source;
an actual dummy resets the RGB mode. [Earlier audience setup](NATIVE_RETAINED_AUDIENCE_EMPTY_ADMISSION.md)
instead uses texture-plus-register RGB.

Admitted item reads WORD10 item ID, then captures original U16 icon at
803108a8 + 40*ID +20 (32-bit source offset arithmetic). Perform these services:

1. PSMTXTrans8029e230 M=(0,-20,0).
2. PSMTXRotRad8029dff0 Z, radians=record24*80424a14.
3. PSMTXConcat8029dd64 rotation*M into M.
4. PSMTXTransApply8029e264 M+(0,20,0).
5. PSMTXScaleApply8029e2d8 with(record20,record20,1).
6. PSMTXTransApply with fresh record14/18/1c position.
7. IconMtx8001a6e0(M,flags0,capturedIcon).

ScaleApply multiplies entire rows, including translation; it is left scale,
not right/column scale. Final finite transform is Tpos*Sxy*T0,20*Rz*T0,-20.
Original constants:80424a10=-20,80424a18=20,80424990=1,80424a14 bits3c8efa35.
Icon remains captured while later matrix/icon providers hold. Each matrix
request retains copied inputs until success, while fields for subsequent
source requests remain fresh. Native scalar/FMA/libm matrix adapters preserve
this order but do not claim original paired-single/FPSCR/SDK rounding parity.
Nonfinite input/output remains unavailable in this finite adapter.

Admitted AP constructs Scale(record38/3c/40), RotRadZ0,
Trans(record8/c,0), Concat(scale,rotation), then Concat(translation,result).
Capture source DATA RGBA template80424978=ffffff00; replace low8 with fresh
record BYTE5c. Call IconColor8001a4bc(matrix,flags16,icon442,localRGBA).
RGBA/matrix remain captured while the icon request holds. No source actor/item
memory writes, queue reset or counter changes occur in these local bodies.

The real native dispatcher guards callback/camera/userdata against the actual
invocation, binds matrix numerical adapters, and runs genuine setup/scan.
The portable sourceIcon service now runs the whole retained
[IconMtx/IconColor wrappers](ORIGINAL_MATRIX_ICON_SERVICES.md) when supplied.
[Native resource/registered-GP and published-camera binding](NATIVE_ORIGINAL_ICON_RESOURCES.md)
now reaches actual resident texture objects. Full GP Main ownership and native
material/primitive execution remain open, so active native icon draws still wait. IconMtx and IconColor first ask current
camera8000f2e4, check loaded globals
8041e638/63c, then use source metadata8041e634, low16 iconID, frame clocks,
current-camera concat and iconGX800197f8. A native cached quad is not this full
service. Source icon geometry requirements are in
[item selector icon flags](ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md) and
[retained item icon drawing](../field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md).
Unavailability does not acknowledge icon emission or skip admitted records.

Private evidence: artifacts/battle-display-footer-evidence/DispItem.dis,
DispApSrc.dis; artifacts/reference/display-class-check/AudienceGXInit.disasm.
Read-only ISO constants and ScaleApply instructions were checked directly.
Host tests cover all setup/scan calls, different owner captures, held matrix
arguments versus later fresh position, pinned icon/alpha, bits1/2 and alpha0,
row-scaled translation and real retained inactive banks after ending.
Original runtime/raster and hardware equivalence remain unverified.
