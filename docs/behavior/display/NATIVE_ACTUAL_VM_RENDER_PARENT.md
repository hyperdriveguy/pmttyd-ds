# Actual-VM retained renderProc parent

US G8ME01 disc0 revision0; static read-only source audit, not original execution.
Complete renderProc8003c884/d88 SHA256
`7baffc246c77e795af059e7af55d8dea59e9781c74d9b5f18b052d025517cc46`.
Ignored ordered listing: `artifacts/npc-main-parent-evidence/renderProc-inner-abi.txt`.
Reviewed implementation map: `artifacts/npc-renderProc-next-evidence.txt`.

`resumeAnimationSourceRender` implements the complete local parent over actual
word/byte/halfword VM reads. [Entry/defaults](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md),
[overrides](ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md),
[last stage/rebind](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md) and
[arrays/FIFO](ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md) remain source contracts.
It does not use the restricted typed shape snapshot as the actual VM renderer.
The only consumed input is raw shape index; the caller's current index and pass
are not additional renderProc inputs. uint32 address/counter arithmetic wraps;
source signed loop bounds and signed-byte selectors remain distinct.

## Entry and retained children

Shape ffffffff returns before any provider, camera, RAM or GPU effect. Other
indices reach current-camera getter, modeling pointer, Concat(camera+11c,M),
position upload, InvXpose, normal upload and current-matrix selection before
shape admission. Camera tokens are opaque guest references whose view is read
by the actual SDK child when reached. No camera4/projection substitution occurs.
InvXpose's returned status is ignored. Local matrices are optional so singular
no-store does not manufacture an identity or initialize unknown normal scratch.

`AnimationRenderCall` names the original function and ordered typed arguments.
Scalar, guest, local matrix, local vector and local color references retain
durable identities. Seed input/output aliases vector0, then a separate vector1
copy is supplied to Light. Colors are raw words, captured C2 then C1. Local
references do not invent guest stack addresses. SDK, camera, map material,
texture, GX and printf children own any partial effects through false; request
identity and return storage survive unchanged until the child completes.

Missing children hold at their actual calls. There are no successful default
receipts, implicit identity matrices, texture substitutions or state rollback.
A provider must outlive its pending request and implement the original child;
this parent alone does not provide any production SDK/material/GPU operation.

## Source capture and material order

Entry W/D/shape/draw metadata and position/normal/color bases precede pass reads.
Mode0 can read the same captured W+e0 twice. Alpha f3 short-circuits f7. Cull
selector reads the actual table without a source clamp. The eight UV fields are
captured in order58,60,68,70,80,78,88,90, interleaved with fresh W and D/UVbase.
Nonpositive draw counts still reach this work; subsequent draws use the retained
count, metadata and UV bases, while each draw reloads its own source globals.

Default slots are captured before the default guard and survive later children.
Texture ABI fields retain their precise primitive order. Each raw C2/C1/flags
capture reloads P through the same W independently. Fog compares a fresh camera4
getter with another current-camera getter, rather than reusing entry camera.

Primary kind1, then kind2 only if no kind1 match, uses reversed slots and unsigned
frame-extension bytes. A match with a null object still prevents kind2. The
warning call receives format802c1078 and the captured unsigned frame byte in r4,
then descriptor0 is read; it does not use traversal-current or the summed index
as the warning argument. Reached out-of-range descriptors still require that
real printf child. smartTexObj's returned pointer feeds the ordinary LoadTexObj;
the extra-object call discards its return. Extra/rebind uses fresh selected
object/matrix globals, including an unguarded zero selected matrix when reached.
Independent flag10 rebind runs even when P4 disabled both primary searches.
No default/material state is restored at the parent return.

## Arrays and individual FIFO effects

Selector0 uses current pose arrays; nonzero selector uses current global f8/fc.
Attribute setup precedes each signed selector byte read. The native durable UV
local table supports indices0..7. An escaped selector retains a frontier after
its captured byte and before SetArray; rewriting that byte cannot undo capture.
This is an implementation admission limit, not an original bounds branch.

Single UV run pointers are captured before Begin and remain fixed across vertex
writes. Raw run counts truncate to low16; even zero count calls Begin. Multi UV
captures W once after Begin, even for zero vertices, and reads D through that
retained W separately for each coordinate. Root mutation cannot redirect later
coordinates; mutation of that W's D pointer can. Exact four-coordinate bulk
read order is D/selector for lane0, selector/D for lanes1/2/3; every remainder
reads D/selector. UV first/base/index loads remain separate and live. A loaded
word contributes only its low16; high bits need not be zero.

Position, normal, color and UV components each alternate a read and one retained
FIFO halfword store to cc008000. A held store does not reread its input or replay
prior components. No GXEnd, stack pop, draw-publication or lease retirement is
invented. Fixed cursor/scratch storage is reused without per-vertex allocation.
No physical hardware timing or measured performance claim follows from this.

## Same-process ownership and validation

`AnimationRenderBinding` reads through the same EventAddressSpace and guards
all reads/calls/FIFO with the producer's current predicate. Group's explicit
legacy render provider remains exclusive when supplied, including on false.
Otherwise the owned parent uses `NativeNpcGroupBackend.renderChildren`; absent
children retain real call frontiers. The producer remains pending until the
last FIFO effect and the surrounding group/Inner/wrapper complete.

Host and isolated ARM fixtures cover every primitive hold across all material
routes and signed slot cases, independent source call/ABI goldens, pass/alpha
short circuits, UV load chronology, camera changes, null-match priority,
warning arguments, current extra objects/matrices, draw-entry slot capture,
multiple draws/runs, count truncation/zero count, partial FIFO and provider/input
identity. Original Mario tests run the real CPU nodes and raw render parent over
original VM tables/output/index streams under a retained producer lease.
Camera/shape SDK/GPU/cache/material effects in those tests are explicit receipts;
they do not establish production rendering or normal Game activation.

Next bind original current-camera view tokens, reusable same-VM SDK matrices,
material/texture ownership and indexed Citro3D fan submission. Complete
materialProc/Vivian/linked/callback services and normal scheduling remain open,
as do original numeric domains, matched GPU images and hardware performance.

Read-only next bridge map: `artifacts/npc-render-sdk-next-evidence.txt`. Reuse
Node SDK capture/math through a public owner rather than copying Concat. The
original MultVec child publishes local X before reading its final matrix row;
a raw-row backing hold must retain that partial store. An atomic typed point
math receipt alone does not cover those original VM boundaries.
