# Queued enemy HP presentation and retained original bar

2026-10-14; US G8ME01 revision0. This increment adds
`battle_enemy_hp_presentation.hpp/.cpp`, composing the existing complete
BattleDrawEnemyHP8011fde0/244 parent with the newly retained whole
BattleDrawEnemyHPBar8011fc3c/1a4. It does not replace source HP with a harness
counter, create Tattle flags, install a normal scheduler or acknowledge missing
numeric/texture/GPU services. Controls and lower audience views are unaffected.

## Queue and parent ownership

`resumeQueuedBattleEnemyHp` requires the actual `DisplayDispatchCursor` to be
active and invoking callback8011fde0 on camera8. It retains camera/userdata B and
provider identities across waits, rejecting mismatches before resuming. It calls
existing `drawBattleEnemyHp` rather than introducing a second unit scan. That
[parent contract](../battle/ENEMY_HP_DRAW_AND_NATIVE_TATTLE_CHECK.md) preserves
all64 registry slots, status27, source Tattle/flags, repeated registry reads,
world/projective coordinates, carried bar alpha and original number mode1.
The new wrapper forwards actual supplied parent services and requires them;
there is no fallback status result, projection, saved switch or number draw.

The wrapper owns one stable private XYZ/color copy for an admitted bar call.
It creates that copy once, then retains the whole bar child until completion.
A held mode1 number cannot start the bar; a held bar cannot replay the accepted
number, projection or earlier registry reads. Queue/VM/services must outlive the
call, and cursor reset/destruction is caller-owned after invocation returns;
this source cursor is not a reentrant renderer or arbitrary callback-lifetime
manager. Full queue record sorting, source display state and normal simulation
cadence remain the actual dispatcher owner's responsibilities.

## Complete original bar body

The bar borrows stable position and RGBA objects and retains U/camera/provider
ownership. It requests camera8, `btlDispGXInit2DRasta80103b48`, then loads that
camera's live+11c matrix through `GXLoadPosMtxImm802ba2fc`. Each later source
matrix load recaptures the matrix at its own site; retrying a held load retains
the captured matrix instead of repeating completed memory reads.

The first texture request is original `btlDispTexPlane801032c8` index17. Original
reads/arithmetic compute X-10, centerX=19+(X-10), Y-3, Z0, scales1/1 and a copied
RGBA word. Its private position X store occurs once before that helper call.
A wait therefore leaves the original local X mutation in place without another
subtraction or duplicated source request.

Fresh U16.1fc selects exact mode1. Only mode1 reads signed16 U200; positive
requests another InitRasta/matrix pair and black-alpha highlight through whole
linked `btlDispGXQuads2DRasta80103918`. Its coordinates are left=X+1, top=Y+2,
right=fma(34,U214,left), bottom=top-8; alpha is the fresh low RGBA byte. It then
runs the original third InitRasta/matrix pair. Mode1 nonpositive skips that
highlight/reset pair; every other mode skips the U200 query entirely.

The last texture request uses original index18 and the live U210 gauge ratio,
not currentHP/maxHP. Remainder=1-ratio, baseX=19+privateX and the fused source
negative-multiply/subtract gives X=baseX-20*remainder; Y=Y-3, Z0, Xscale=ratio,
Yscale1. Source mode1 and other-mode branches preserve their distinct ordered
RGBA/constant/ratio reads. No ratio/elapsed/HP clamp, rounding-to-integer,
substitute maximum or inferred gauge interpolation is added. Each external
request captures its original operands once; source memory reads after an
acknowledged child remain live. Numerical operators retain separate binary32
add/subtract and fused boundaries. Full Gekko FPSCR/NaN-payload equivalence is
not claimed.

## Primary evidence and resource frontier

Read-only original disassembly and hashes are private under
`artifacts/hp-*-source.*` and `hp-presentation-source-evidence.json`:

| Source | Size | SHA256 |
| --- | --- | --- |
| BattleDrawEnemyHP8011fde0 | 244 hex | 2e95f6ac07cf03189b47cd44f5648402d6020a1bbeebf726c5c9bb8e6b60ff69 |
| BattleDrawEnemyHPBar8011fc3c | 1a4 hex | a01256c6df577a96950b8f7a0d73663b8e8176c7246351e7768d3df2d90aa42c |
| Constants804225cc..804225ef | 24 hex | 74b613c1e03fee7d2597002a93d9b08e24be06d60ee134dc5c2325f24ec7950d |

That constant span is original10,19,3,0,1,2,34,8,20. These values are static
binary evidence, not measured behavior. Texture17/18 are original40x16 CMPR
cells in the already packaged common TPL. Existing class circle/bubble0/1 and
footer4..16 GPU owners do not include them and must not substitute their art.
No original asset bytes, extraction, conversion or licensing change is committed
by this source increment; [existing provenance](../../ASSETS.md) still applies.

`BattleEnemyHpRasterRequest` explicitly requests the real remaining linked
owners. InitRasta80103b48/f4 calls common Init80103c3c and configures untextured
raster RGBA. QuadRasta80103918/d4 invokes point helper801039ec four times.
Texture plane801032c8/11c differs from Plane2: it calls Init801036e4, actual
current camera, Scale/Translation and two Concats before loading its matrix,
then copies the then-current color and calls PlainGX801033e4. The existing
[retained Plane2 implementation](NATIVE_ORIGINAL_FOOTER_GPU_COMPOSITION.md)
provides related services but is not an interchangeable acknowledgement.
Number `iconNumberDispGx80019120/284`, these complete child owners, source
texture17/18 admission and actual ordered native presentation remain required.
A false/missing provider keeps the real source frontier pending.

## Focused validation

Werror host tests cover nine mode/timer combinations; fixed source-derived
coordinates/colors; positive/signed-negative timer selection; all50 external
call boundaries held twice; unchanged accepted requests and memory-read traces;
owner/unit/borrowed-local mismatch; retained background versus later live
ratio/color; wrong/inactive queue identity; held original mode1 number; and a
held final bar without replayed number/projection. HP changing to999 while
number holds preserves its captured5 and still uses actual U210=.5 for the bar.
The test's camera/projection/raster services are explicit fixtures, not native
GPU proof.

ASan/UBSan passes with leak scanning disabled because sandbox ptrace prevents
LeakSanitizer operation. An earlier compile overlapped an unrelated actor-service
header ABI change and is excluded; the accepted sanitizer build verified stable
headers throughout compilation. Actual devkitARM ARMv6K hard-float syntax also
passes. Root owns CMake registration, full matched-source `make test`/`make all`,
original-asset native captures and eventual normal tutorial/real battle binding.
