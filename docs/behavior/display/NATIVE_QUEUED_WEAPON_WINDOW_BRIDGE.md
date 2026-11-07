# Queued weapon Help Window source bridge

`BattleWeaponWindowBridge` connects the actual Help child call to
`windowDispGX_Waku_col` (`8007b44c`, `68c` bytes), its retained
`WindowPrimitiveCursor`, and `BattleWindowPacketProvider`. It prepares one typed
Window packet at its original position in `BattleWeaponOrderedPublication`.
The queued parent must still complete, seal, preflight, and submit the entire
ordered publication before acknowledging the original display callback. A
prepared packet alone is not a display-completion receipt.

## Source and resource ownership

The binding receives the same genuine `NativeUiStartup` process owner used by
Font and Window startup. Source reads give its owned resource footprint first
refusal, including unknown cells. Other reads use the actual caller's source
memory; no constant, BSS, source-ready, or atlas fallback is supplied. The five
Window constants are already present in the original Trio/Harbor source packs.

The original current-camera query precedes the source ready-word read. A real
ready-zero invocation can therefore complete its no-draw child without an
imported atlas, completed SDK object, or GX profile. Ready-one follows all source
matrix operations and the original 180 scalar FIFO writes for nine quads.
Missing readers, math, camera, texture construction, compatible GX history,
native residency, or packet admission retain the original continuation.

At actual `GXLoadTexObj`, the bridge requires `startup.windowObject(0)` at the
requested source address. This witness contains the genuine indexed palette,
image metadata, and all 32 SDK constructor bytes produced by source startup.
The immutable packet pins that witness, the shared process lifetime, and the
actual per-load `SourceGxSetterRouting::capture(1)` snapshot. Its long current
predicate rechecks the indexed source object and source allocation; no atlas
index is substituted for an original texture object. Later unrelated GX setters
do not rewrite the captured history. Source-object mutation, process retirement,
or resource-generation loss prevents replay.

The binding supplies one actual admitted camera8 address and a copied camera
projection, viewport, and scissor proof from its live invoking source callback.
The original camera query must return that address. Camera view words are read
by the original continuation through the actual source provider. There is no
message-panel orthographic camera or default view. The same borrowed Help
request must remain stable across holds. Recursive entry and replaced request
objects are refused.

Only an engaged strict packet-provider result is observed by the GX history
router. Router metadata is never a second source acknowledgement. Final FIFO
admission can hold without repeating camera, matrix, SDK, or earlier FIFO work.
The native surface owner must outlive this helper; release retires its real
generation and the enclosing renderer context must retire before destroying it.

## Native integration

The unapplied journal in
`artifacts/weapon-window-bridge-validation/runtime-journal.json` adds the helper
to the peer's actual queued weapon runtime composition. Its `windowHelp` hook
handles only original Help function `8007b44c`; the peer's real font routes must
remain in the same dispatcher. The hook appends through the existing ordered
owner, preserving source order with font, submenu, icon, and class packets.
It does not call `Renderer::sourceWindowFrame`, activate a separate overlay,
alter physical controls, or acknowledge whole Weapon/Help completion.

No new art or conversion is introduced. The existing original descriptor0
72x72 Window surface and its verified 128x128 edge-extruded native texture are
reused with their existing provenance and rights. Original rights remain with
their holders; this bridge grants no asset license.

## Focused evidence and limits

The host test runs real Font/Window startup, actual original palette bytes and
51 SDK objects, genuine process cold GX contribution, the whole source Window
primitive, strict native-atlas import-memory validation, and actual ordered
append. Its camera and invoking queue are explicitly isolated test owners
using real camera Main/Draw math; they are not normal gameplay activation.
The test intentionally withholds whole-parent source completion and GPU submit.

Tests cover source-ready-zero with no atlas, late color capture, camera/view
and matrix continuation holds, final FIFO holds, missing source literals,
wrong camera, unknown SDK objects, invalidated GX history, source-object
mutation, native resource retirement, and process retirement. Independent
source verifiers pin the existing complete Window/WindowSetup bodies. Strict
host, owned-code ASan/UBSan, and ARM syntax checks are recorded in the frozen
manifest. Parent integration still requires a consistent whole build and a
normal queued callback proof; this package makes neither claim.
