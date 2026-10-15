# Ordered original weapon-menu packets

This is a native composition owner, not a new original-game controller. The
source parent remains the complete retained queued `80121ce8` → Disp `8011604c`
→ DrawWeaponWin `80115dac` graph described in
[the actual weapon frame contract](NATIVE_WEAPON_MENU_FRAME.md). It does not
activate that graph, set menu readiness, invent positions, or process input.
Original physical controls and optional audience interaction keep their actual
source owners.

`BattleWeaponOrderedPublication` collects a single typed vector in original
successful emission order: original font glyph packets, original common-bank
submenu2/3 packets, source icon packets with actual resident texture identity
and generation, and actual original Window packets. The global windowSetup
source owner and its native panel generation remain distinct from battle-common
menu storage; no artificial shared-owner equality is required between them. Reusing the icon residency/validation structures named
`BattleEnemyHpIconPacket` does not use the HP source pass, HP completion, or HP
publication. The owner has its own actual weapon allocation/generation/pass.
No original source function is acknowledged merely by recording its request.

The binding pins actual W allocation and source-bank ownership, native resource
contexts, the real font/submenu resources and the actual icon lookup. Once bound
it cannot rebind. `current` proves both W incarnation and source-bank lifetime;
`sourceCurrent` additionally proves the actual executing weapon callback and
current CameraMain/Draw provenance. `sourceComplete` must query the actual
retained whole parent, normally `BattleWeaponNativeFrame::sourceComplete`.
Nonzero caller counters alone prove none of these conditions. All callback
contexts and resources must stay alive through held calls and synchronous
submission. Callbacks are pure unless explicitly designated draw/commit.

`begin(pass)` reserves the bounded ordered arena before source work. Holds on
source children leave accepted entries intact. Stale pass sinks cannot append
to a later pass. Source submenu packets must have the same pass, allocation
generation and actual source-bank owner as the binding. Icon capture queries
the real resident import; missing imports remain holds. Renderer eviction must
include this owner's `retains(image)` while its building/sealed entries exist.

Font emission needs the exact unapplied journal
`artifacts/weapon-ordered-source/font-sink-hooks-journal.json`. Its optional paired
hooks reserve one ordered slot before the original atomic glyph's two RNG calls,
then commit that typed packet without allocation, callback, or failure after RNG.
An unchanged pending reservation may retry, but another packet cannot pass it.
Changed float operands, including signed zero, cannot replace the reservation.
The native atomic interval forbids resource retirement and source/reentrant work.
Violating the nonfailing commit contract aborts rather than dropping an admitted
glyph after its RNG. Existing unbound font sink consumers retain their behavior.
See [the actual font leaf](RESIDENT_BATTLE_FONT_AND_GLYPH_PACKETS.md) and
[the actual submenu leaf](ORDERED_ORIGINAL_SUBMENU_PACKETS.md).

`seal` requires actual complete source work and full typed resource/geometry
preflight. It returns an immutable batch retaining both source ownership pins
and copied leaf geometry. Subsequent `seal` calls return that same batch. The
submission adapter matches `BattleWeaponNativeSubmission`; it rejects forged
copies and aliases with a different shared ownership identity. `ready` must
preflight **every** mixed native packet without GPU mutation, import, RNG or
source execution. A false result retains the batch. After a final typed and
ownership recheck, `draw` synchronously submits every primitive in vector order,
without failure, reentry, resource changes or source execution. Successful
submission is recorded once, so caller retries never replay GPU work.

Finite source floats alone do not prove finite GPU geometry. The collector also
checks every multiply and intermediate sum for all four actual icon/submenu
clip vertices; the font leaf checks its composed transform and clip vertices.
These conservative native guards do not cull finite offscreen, transparent,
zero-area or singular geometry and do not add source RNG calls.

The long batch proof deliberately does not depend on the executing camera epoch:
copied matrices/raster state survive the legitimate `camera.finish`. Actual W,
source-bank and resident texture generations must remain valid. Once observed,
allocation loss or explicit retirement is sticky. This collector does not own
physical GPU allocations or authorize freeing them while native draw runs.

The actual [Window packet/resource leaf](ORIGINAL_CAMERA8_WINDOW_PACKETS.md)
is the fourth typed variant, never an opaque original-child acknowledgment.
`appendWindow` requires the current source pass/allocation generation, the actual
bound `BattleWindowSurfaceResource`, and complete original packet validation.
An absent optional window-resource binding holds that child without appending.
The Window packet separately retains its genuine global windowSetup/TEX/palette
source owner and actual resident source72→native128 panel lease. The ordered
owner also retains the battle W/common-bank proof; both must remain current.
A pending font reservation blocks Window append, just as every other sibling.
No held Window FIFO prefix is restarted, and every sealed batch preflight checks
its source/texture generations again, including after camera finish.

The normal renderer/source binding is still missing. Existing
`Renderer::sourceWindowFrame` is not used: it has another projection and does
not prove this source camera8/window texture generation. Genuine source
windowTexSetup/8041ea1c readiness, palette and actual TEX object producers,
source ordinary global GX table0/indirect state, actual SDK/numeric/memory and
source callback ownership remain required. Their absence is a real hold, not
an omitted Window entry. Native weapon icon capture must route actual source
GX/camera state into this owner's `appendIcon`, with its own pass instead of
borrowing the finishing/HP pass. Renderer whole mixed preflight/atomic draw,
font ordered hooks and the full actual source material graph are not activated
by this standalone module.

Focused tests cover font/submenu/icon/font ordering, pending-font barriers,
unchanged reservation retries, changed operands, source completion holds,
two GPU-admission holds, exactly one actual typed draw, post-camera copied batch
validity, new-pass/stale-sink rejection, forged ownership, missing/replaced icon
imports, font/submenu retirement, W lifetime loss and callback reentry. The
Window extension executes actual original TEX5/GX5 against real original
window0 metadata and source SDK constants, then the complete180FIFO source
Window body to append Window/font/submenu/icon/font. These explicit isolated
primitive-entry/ordinary-GX/camera fixture conditions do not certify whole
windowSetup or normal battle. Tests hold Window behind a pending font without
replaying earlier FIFO, reject missing actual object/SDK/resource providers,
retain late-captured color, allow independent global Window source ownership,
and reject retired source/native Window owners before any mixed GPU work.

Class surface extension appends typed alternative4 for original I4 circle/bubble0/1. It preserves actual TEX object/known32B lifetime independently of the resident class pair, true camera8 and common-bank/W owner. Class final FIFO waits behind pending font reservations; whole mixed preflight rejects stale source or native leases. C0 RGB and raster RGBA stay separate; C0 alpha is unused. See [source class packets](ORIGINAL_CLASS_SURFACE_PACKETS.md).
