# Native finishing camera GPU adapter

[Frame frontier](NATIVE_FINISHING_DRAW_FRAME_BINDING_FRONTIER.md),
[original camera order](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md) and
[complete original dummy](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md) govern
this implementation. No original class/callback raster completion is implied.

## Admitted frame and continuation

Renderer opens an actual successful C3D_FrameBegin, binds the top target and
then calls the explicit logical-const Trio shared runtime continuation. A
stack service bridge routes scalar letterbox to its existing owner and GPU
requests to the frame-scoped renderer. The retained Draw/dispatch cursors and
sorted borrowed queue survive this handoff; simulation supplies no GPU provider.
A held pass does not repeat sort, scalar animation/registration or previous
camera requests. Renderer backend restoration is separate from source requests.

The independent existing world renderer resets viewport/scissor/depth range,
write/test and ordinary alpha-blend state afterward. This remains necessary
when a missing resource skips world rendering: C2D_Prepare does not establish
alpha blending, and the source dummy zero/one blend would hide its text. At the next frame admission, the
queued backend restores cached viewport then scissor (SetViewport disables
scissor), projection/current position transform and dummy pipeline state.
It never re-emits the dummy triangle during restoration. No original callback
or queue reset is acknowledged by the independent world/overlay renderer.

## Explicit finite matrix and rectangle adapter

The source608x480 upper-left rectangle(x,y,w,h) maps to physical240x400:
left=(480-y-h)*240/480, bottom=(608-x-w)*400/608, width=h*240/480,
height=w*400/608. Edges are scaled before forming extents. Only finite positive
in-bounds rectangles with integral transformed endpoints are admitted; an
absolute1e-7 double rounding tolerance recognizes arithmetic representation
of integer endpoints. Fractional transformed bounds remain a provider hold,
not a silent whole-pixel raster policy. Scissor maxima passed to C3D are
exclusive. Full bank rectangles map exactly to physical(0,0,240,400).

Source depth range[min,max] maps to C3D_DepthMap(true,min-max,1-max), preserving
the native reversed-depth convention. Original source and native RH matrices
both use near Z/W=-1, far0. Full projection times retained position3x4 is
rotated by rows(PV[1],-PV[0],PV[2],PV[3]); all four rows/components, including
projection-extra terms, survive. Named C3D vector x/y/z/w assignments handle
its reversed physical storage. No lens reconstruction or source near/far
success is fabricated. The initial native position basis is identity; the first
source dummy loads its actual view before any admitted vertices. Later
projection updates preserve that retained position view until another owner
loads it, matching the separate source projection/position state model.

## Real no-write dummy

The native provider uploads the actual source view/full projection transform,
configures float XYZ/UV plus the established shader color loader, and maps the
source one-stage constant to native RGBA0x81000000. Later PICA TEV stages pass
through. It emits original XYZ(0,0,0),(20,0,0),(20,20,0), each UV(0,0), via
C3D immediate triangles. Zero/one ADD blending, alphaGEQUAL128 (source
GEQUAL128 OR NEVER), disabled early depth, disabled depth test and zero write
mask implement the declared dummy state. Cull/fog are preserved; no extra
reset is called. The source-visible no-write triangle is not a success no-op.

After camera0 completes its empty queue and real dummy, camera1 receives
viewport/scissor/projection and holds at actual ColorUpdate802b9380 before
its first shadow record.21 records remain. Ordinary dispatch common/class
GPU state, shadow/part/material/stage/audience/menu/EXP/letterbox callbacks,
near/far services and further cameras remain unbound. Queue clearing and
outer Main/teardown are unavailable until that full ordered pass completes.

## Evidence and limits

Installed libcitro3d disassembly/headers confirm RH tilt/depth conventions,
SetViewport positive coefficients, scissor exclusive ends and actual public
frame boundaries. Independent review checked state order, attribute protocol,
constant/blend/alpha/write mapping and frame-only submission. Host tests verify
rectangle rejection/mapping, full-row transform preservation, provider holds,
cache snapshots/restoration and actual finishing handoff through camera1.
Native normal and explicitly instrumented harness validation is recorded in
the progress archive; the harness supplies only the missing Bero bit as a
documented resolved clear fixture and logs the real renderer frontier. It is
not production room/get_info history or original gameplay timing evidence.
Full GX/PICA numeric/raster parity, inherited state history and physical
performance remain unverified. No assets or original payloads are added.
