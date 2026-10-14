# Native queued letterbox draw continuation

Source: [original callback](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md) and
[projection/FIFO recovery](LETTERBOX_DRAW_PROJECTION_VECTOR_AND_FIFO.md).
Implementation: camera_letterbox_draw.hpp/.cpp, native_camera_draw.cpp,
render_letterbox_draw.cpp and the actual TrioAttack camera-queue bridge.

The cursor captures actual GP identity before independently looking up camera8.
GP.word0 is read only after that lookup. Mask8000 returns before any alpha read;
otherwise signed alpha==0 returns before GPU work. Negative alpha is admitted.
Callback camera/userdata identify the actual invoking queue record but do not
replace the original independent camera8 lookup. A held read/setter keeps its
preceding successful work and captured owners; no registration implies emptiness.

Admitted work saves acknowledged logical viewport, original GX projectionvec7,
and scissor separately, in order. Camera8 projection/kind are captured at the
following source setter. Material RGB is read after NumChans; fresh signed alpha
is then captured into its lowbyte. A held material setter keeps that local word.
Word80429510 has a shared CameraManager letterboxColor owner, initialized0 once
at application-owner construction, not on room/battle entry or cameraInit. Native
reads it live; future indirect source writers must target that same owner.
Fog color is original readonly DOL wordffffffff, not authored replacement art.

All source setters run before GXBegin(QUADS,0,8). Native Begin acknowledges CPU
staging only after its actual source shader/display domain is available. Each
vertex retains source signed16XYZ; top/bottom are read separately at the original
four vertices. Arithmetic32bit is narrowed low16, including overflow/wrapping.
Held vertices do not recapture bars. The fixed eight-vertex stream accepts each
prefix once; finalvertex7 acknowledges only after actual submission. A held final
submission retries the same last vertex and captured prefix. No physical GPU
Begin remains open across a provider wait or frame, and no per-quad allocation
occurs. Two real Citro3D four-vertex fans preserve two-quad topology; they are not
one eight-vertex fan. Source has no RGBA/ST FIFO here; the native world shader
receives unused constant attributes while its material-color TEV consumes none.

Native logical shader support includes register-material disabled lighting,
GX_PASSCLR and direct XYZ/S16 format3. Shared source owners retain these updates.
Only viewport, projectionvec7 and scissor restore after submission, in that order.
Source view/model and shader/depth/blend/cull state are deliberately retained.
The original SDK's source kind plus six coefficients are distinct from a tilted
projection*view matrix. Native GX projection uploads now select those original
six coefficients and reconstruct the hardware-equivalent canonical projection;
arbitrary other Mat4 entries are not uploaded. All current projection callers
supply source kind; native kinds other than0/1 and nonfinite selected coefficients
remain unsupported. Projection-vector flag+0/-0 means perspective; all other
flags includingNaN mean orthographic, as the SDK comparison requires. Selected
coefficients are copied without division/inversion. Restore retains current view.

Host tests check genuine early-outs versus no-alpha GP gate, captured GP pointer
and matrix owners, later material alpha, held setter/Begin/FIFO/final submission,
live unequal bar reads, signed16 wrapping, source shader/domain state and ordered
save/restore with current view. Native projection tests cover retained kind,
canonical six-coefficient upload and no publication on backend hold. Synthetic
mutations are tests, not original execution evidence. Physical viewport scaling,
float24 conversion, quad diagonal/culling and source framebuffer parity remain
platform boundaries. Native runtime results/remaining work are in PROGRESS and
EVT-01; complete prologue/field restoration is not established by this callback.
