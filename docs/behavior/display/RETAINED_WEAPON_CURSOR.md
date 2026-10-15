# Retained original weapon cursor

`resumePackedBattleWeaponCursor` implements the complete US revision-0
`DrawCursor80117a6c/1b0`. Its caller is the original weapon window's
`80115f48` request, with the three copied `802ef750` words retained in
`BattleWeaponDrawRequest::cursorWindow`. It does not activate the weapon
window, bypass source readiness, or introduce touch controls.

## Primary evidence and resources

`tools/verify_battle_weapon_cursor.py` opens `G8ME01.iso` read-only, maps its
original DOL sections, and checks the whole cursor body, the complete
`iconDispGx8001a918/24` alpha wrapper, caller XYZ, signed-conversion bias,
all nine scalar literals, and startup r2 assignment `80427440`. The primary
body SHA-256 is
`2964e46448e17f1514d4a97b5656a96de4b69e92128c864511f14e4d7a128ac7`.
The caller XYZ is `{-60,35,0}`; these are original source values, not a new
layout. The function reads X/Y at the original load sites and never reads the
caller's Z. This implementation does not bake in that particular caller XYZ.

The scroll arrows are original icon IDs 445/446, flags 16, scale .75; the
selection hand is ID 504, flags 20, scale 1. Each call retains the existing
complete `resumeSourceIconPosition` implementation with alpha 255, genuine
icon metadata/animation, actual camera, matrix services, and either the
actual linked draw provider or original icon GX helper. No new bitmap,
conversion or asset license is introduced. Original ISO artwork remains
copyrighted by its original rights holders; provenance grants no license.
Existing source-icon RomFS packs remain the resource dependency.

## Exact retained behavior

The entry captures global B, W at `B+1c78`, C at `W+0`, timer `W+11c`,
and original interpolation literals 0/10 before calling interpolation
`80006c8c(mode4,step,duration6,0,10)`. After that call completes it reloads
and increments the live timer, then reads GP, timer, GP+4 in that order.
The reset threshold is signed `(wrap32(rate*12))/60`. Rates and timers
are not clamped or assumed positive.

The upper arrow runs only when signed `C+4 > 0`. The lower arrow runs only
when signed `C+4 < wrap32(C+8-6)`. C's fields are read freshly after preceding
children complete, but the captured C pointer is retained. The hand's
signed wrapped `(C+0)-(C+4)` is clamped to 0..5 by the original branches.
The original 64-bit bias is read as two ordered source words; conversion
and float intermediate rounding precede the original `fnmsubs`. The FMA
provider returns fused binary32 `24*index-sourceY`; the controller negates
the rounded result. This preserves the source negative-zero ordering.
The hand scale is read only after that operation completes. Exceptional
PowerPC floating-point/FPSCR behavior is not claimed for the finite native
provider policy.

A held interpolation, FMA, camera, source resource read, matrix, or icon
submission resumes that exact child without replaying completed timer stores
or earlier icons. Borrowed window identity, strong lifetime owner/control
block, service table and optional GX table are fixed across waits. Each
external child provider is guarded before and after invocation. Retirement
is sticky; reentry and reset during a provider call are rejected.
Synchronous actor memory providers must admit actual mapped source spans.
No missing provider is translated into a successful icon. The source's
actual initial zero resource-pointer branch still returns without drawing,
as the linked original helper requires.

## Integration boundary

Keep one `BattleWeaponCursor` and stable `BattleWeaponCursorServices` in
the actual weapon callback/frame owner. Route only request function
`80117a6c` and require its `cursorWindow` value. Pass the borrowed optional's
contained array directly; do not copy it into a temporary per resume.
Use the parent's actual memory/lifetime/current predicate, real interpolation
and finite FMA policy, and existing original source-icon services. When this
void child completes, the request adapter may return ignored ABI value zero.
The outer display owner must still finish whole ordered GPU admission and
submission before acknowledging the queued original callback. A captured
vector alone does not complete native presentation.

This batch supplies no normal-runtime registration, camera fixture, GPU
success fallback, weapon help child, or source weapon-window readiness write.
The original weapon parent/common/SubIcon/help and native ordered frame
owners remain separate dependencies.

## Validation

Focused strict host tests exercise original icon metadata and actual linked
position/matrix children using the private source-icon pack. They hold every
optional nested camera/read/matrix/draw service boundary in turn and verify
unchanged final icon order/transforms, single timer mutation, fresh reads,
signed reset/wrap/clamp cases, held FMA/late scale and negative zero,
resource-null source return, missing services, reentry and permanent lifetime
loss. These are host fixture tests, not GPU or original framebuffer evidence.
Strict ARM syntax and ASan/UBSan supplement the read-only ISO verifier. Root
integration owns full `make test` and `make all` over one consistent snapshot.
