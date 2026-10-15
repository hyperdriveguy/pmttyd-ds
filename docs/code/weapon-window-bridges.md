# Weapon frame, window FIFO and submenu resources

These helpers connect original queued draw code to explicit camera and external
batch owners. They add no scheduler, menu activation or fabricated source readiness.
See [weapon menus](battle-weapon-menus.md) for source UI state and
[font GX pipeline](font-gx-pipeline.md) for the font leaf.

## Short camera epoch and long publication

[The weapon camera bridge](../../source/display/battle_weapon_camera_bridge.cpp)
binds original callback 0x80121ce8 through the shared camera engine. Camera 8,
completed Main producer, actual CameraDraw/DisplayQueue record and reset epoch
must be admitted by that engine. Class or HP callbacks cannot substitute. The
weapon companion additionally snapshots actual matrices, six viewport lanes,
four scissor words and projection kind. Availability rechecks the base engine
and freezes those raster fields; viewport lanes must remain finite and bitwise
identical, including signed zero. A detected change latches stale. Successful
finish removes this short view; a long batch must retain its own copied state.

[BattleWeaponNativeFrame](../../source/display/battle_weapon_native_frame.cpp)
pins a real allocation token, source-resource owner and linked service context.
External pure predicates prove allocation/resource history and the successful
Main generation/pass. These predicates must reflect actual owners; nonzero
metadata alone supplies no original allocation/readiness producer. Each resume
requires camera 8, actual invoking callback 0x80121ce8, matching B and the same
current queue record. Passes are monotonic native metadata, not render frames.

| Stage | What survives a hold |
| --- | --- |
| External begin | That provider's partial batch setup; successful begin is latched |
| Whole retained source draw | Source cursor and already completed children; success latches source pass |
| External immutable seal | Provider's partial work; no source draw replay while waiting |
| Actual complete submit | Immutable resource-owned batch; successful submit is latched before post-call guards |
| Camera finish | Separate short-epoch/queue barrier before publication |

Source completion alone is observable separately from publication. Publication
is exposed only after submit and camera finish with a current long resource
proof. A new pass hides the previous batch while its replacement builds. Neither
inspection nor submission retries start another source draw or repeat glyph RNG.
The immutable batch must pin every actual surface/font/icon and copied state.
The live camera accessor is available only inside the owner's current resume.

Observed source/allocation/resource/batch loss invalidates the binding. Retirement
inside a callback invalidates immediately, with destruction deferred until resume
unwinds; no guest End/free is run. Reentry fails. See the
[native weapon-frame contract](../behavior/display/NATIVE_WEAPON_MENU_FRAME.md).

## Window source instruction cursor

[The window primitive](../../source/display/window_primitive_continuation.cpp)
retains original PCs, thirty-two binary32 register slots, input bits and borrowed
color identity. Entry registers f30/f31 hold x/y, f27/f28 width/height and f29
border. Right/bottom and inner edges are computed at their source instructions,
then registers may be reused as scratch. Inputs compare by bits across holds,
including signed zero and NaN payloads. Provider replacement fails; observed
owner loss poisons the invocation until reset. Terminal completion remains cached.

Actual camera lookup precedes completion word 0x8041ea1c. A real zero returns
no-draw success; an unavailable read holds. Low-16 nonzero type becomes zero.
The source texture offset remains `(type << 5) & 0x001fffe0`, added to the fresh
0x8041ea24 object-array pointer. Each original GX object occupies 32 bytes.
This is neither a native atlas selector nor proof of texture readiness.

The source sets its own material: direct POS/UV with VAT-0 float XY/ST, texgen
matrix 60, one stage, constant C0 times texture color/alpha. It differs from font
TEV state. Caller color is read late at 0x8007b64c and retained through TEV-color
waits. SDK identity and concat use the originally captured camera's twelve view
words at +0x11c; the matrix provider owns real partial SDK work.

Begin requests QUADS 128, format 0 and 36 vertices. All nine cells, including the
center, emit 180 individual XYZ/UV float stores. The cursor keeps each original
store site and pending FIFO token 0xcc008000. An engaged return zero acknowledges
a store; `nullopt` holds it. Constants at 0x80420538/540/544/548 are freshly read
at every original load: zero, one-third, two-thirds and one. Moving these loads
ahead of holds or replacing the stream with a prebuilt mesh changes observable
behavior. No border clamp, alternate tessellation or original SDK numeric parity
is inferred. See the
[window/submenu contract](../behavior/display/RETAINED_SOURCE_WINDOW_AND_SUBMENU_SURFACES.md).

## Submenu atlas lifetime

[The submenu resource](../../source/display/battle_submenu_surface_gpu.cpp)
accepts only native little-endian PMBSUB01 with two exact original-index records
and EOF. Original index 2 is 16x24 at (2,2); index 3 is 8x24 at (22,2), inside
a 32x32 RGBA8 atlas with two-pixel clamp-edge gutters. Catalog V coordinates
already include the native T flip. Actual import must report format 0, one
unrotated 32x32 image and full-image UV `(0,1,1,0)`.

Only successful catalog and actual texture admission publish a generation.
Failure or retirement requested during import releases the candidate handle.
Every lease pins the immutable catalog and exact resident generation; texture
address reuse cannot revive an old lease. Retirement invalidates the generation
before native release. The native wrapper synchronizes C3D around real Tex3DS
import/release and uses clamp/linear atlas sampling. Prepare outside FrameBegin;
lease access imports nothing. The host wrapper stays unavailable, with explicit
host descriptor services used for policy tests. Source pixel/filter equivalence,
full mixed-packet submission and normal menu activation remain separate work.
