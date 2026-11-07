# Native render camera and shared SDK ownership

US G8ME01 disc0 revision0; static original-source recovery and native adapters,
not measured original numeric execution. Completes the CPU/camera children of
[raw renderProc](NATIVE_ACTUAL_VM_RENDER_PARENT.md) when the native group backend
selects `nativeRenderSdk=true`. Explicit whole-render providers remain exclusive;
otherwise unselected generic render children keep their existing ownership.

## Shared matrix engine

`AnimationSdkMatrixBinding` owns the existing Node SDK implementation and its
retained operands, request identity and stores. Node's ten local matrices and
Render's two optional matrices use stable pair/store adapters; neither an eager
matrix copy nor synthetic guest stack bridges them. SDK constants/functions are
named original ABI identities in `animation_sdk`; they are not native PCs.
The bindings cannot be copied or moved while retaining internal owner pointers.

[Original affine numerics](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) and
[node binding](NATIVE_ACTUAL_VM_NODE_BUILDERS.md) remain authoritative. Concat
captures A00,B00,B08,B10,A10,live8041e458,B18,A08,A18,B20,B28,A20,A28 before any
output. Raw pairs require complete contiguous eight-byte actual VM backing.
Output guest pairs retain order00,10,08,18,20,28; local output publishes only on
completed child. The function, exact argument bits, local owner and storage
adapter are pinned across holds, including mutation of the caller request during
one invocation. Partial guest stores, physical aliases and local-store repair
retain captured operands. Semantic guest ranges never fall through to raw RAM,
including a pair crossing a reserved edge or a reached guest-output pair.
Existing finite-input/output, double trig and FMA adaptation domains are retained;
this extraction does not establish libm/GQR/FPSCR/subnormal equivalence.

## Lazy semantic camera bank

The render bridge shares the original native camera token reservation at
81910000, thirteen slots of280(hex) bytes, with the existing facing policy.
It keeps the exact CameraManager owner and producer-current guard; the full
reservation must remain unmapped at every reached boundary. No fake VM camera
allocation or host pointer is returned.

Current getter reads CameraManager.currentIndex; an unset current holds there.
By-index getter returns the actual indexed slot token, including camera4 before
its view is published. Getter success does not require a mode, projection kind
or published view. No camera4 fallback substitutes for an unset current camera.

Concat lazily resolves each pair of the captured camera token's published view
at original offset11c. Pairs may be four-byte aligned; no invented eight-byte
alignment gate applies. A later current-camera change cannot redirect the
captured token. Later pair values stay live, while completed pairs remain
captured through mapping repair. Unpublished views or unsupported token offsets
hold without delegating to another provider. Camera4/current tokens compare
equal exactly when their actual bank slot matches, preserving the fog branch.

Inner refreshes group/SDK/camera ownership only before a new Inner operation,
when no group/render cursor is pending, and only on camera-owner or main-config
changes. A held GPU upload retains the same SDK results and local identities.
The existing producer pass blocks configuration/tick/lease retirement through
completion. Getters and reads never run camera Main/Draw or publish a view.

## Normal and point children

The [original SDK graphs](ANIMATION_SDK_NORMAL_AND_POINT_MATRICES.md) are reused.
InvXpose consumes the local basis only; translations are not inputs. Its ignored
status0 leaves optional normal absent or preserves previous raw words. Status1
publishes even NaN/Inf, rather than inventing a finite/singular identity fallback.
Native1/determinant substitutes original fres; its refinement, roundpoints and
exceptional output graph remain explicit. No normal-pixel equivalence is claimed.

MultVec pins the guest modeling pointer and durable local vector references.
Input/output alias is supported. Its captures are vectorXY,M00,vectorZ,M08,M10,
M18, then X publication, then M20/M28, then Y/Z publication. A missing final-row
mapping retains X and earlier captures; later edits to that completed X are not
replayed. Its paired arithmetic row kernel is shared with typed shape math:
single products for x/y, separate fused z/one lanes, then single lane sum.
It is not a left-fold dot product or direct translation copy. Reached finite
input/result admission remains a native limitation. Point stage names identify
captures/stores; fixed scratch is reused without per-vertex allocations.

Only exact camera/Concat/InvXpose/MultVec requests select owned services. A held
owned SDK cannot become a camera/delegated/FIFO request or switch local owners.
Other material/GX/texture/printf requests and FIFO effects retain the explicit
backend; missing backends hold. This increment does not implement those GPU
consumers or replace genuine reached frontiers with acknowledgements.

## Evidence and remaining work

Original point8029e6a4/54 SHA256
`0e86b49668bd9172bfd1256aa2842a4326032efab48cb6b8df58523a63992e78`;
normal8029df28/c8 SHA256
`c96d218ee09b679c82c6a2fd9867fd5e8e4abb06f1b193c99f7f8aad2e7172d6`.
Ignored listings and review map: `artifacts/npc-main-parent-evidence/` and
`artifacts/npc-render-sdk-next-evidence.txt`. No new original data span or asset
conversion is required.

Tests cover lazy getter/view repair, bank collision and pointer identity, fresh
versus captured pairs, all thirteen shared Concat pair holds, input/local-owner
mutation, ignored translations, singular preservation/exceptional normals,
partial aliased point repair, and independent fused/cancellation/signed-zero
bit goldens. Original Mario executes real Node and Render SDK/camera work across
two camera owners, with a retained GPU-position hold and producer lease.
GPU/material/cache effects remain explicit test receipts. Normal Game scheduling,
material/texture/Citro3D indexed fans, linked/Vivian/callback services, matched
original images, numeric domains and physical hardware performance remain open.
