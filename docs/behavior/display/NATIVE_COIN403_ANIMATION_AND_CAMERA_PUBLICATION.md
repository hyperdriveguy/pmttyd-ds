# Native coin403 animation and early camera5 publication

Committed `200e326d`. Source contracts:
[coin animation/geometry](COIN403_ICON_ANIMATION_VISIBILITY_AND_CAMERA5.md),
[field manager order](../platform/GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md),
[camera5 publication](../camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md),
[coin window/effect](NATIVE_COIN_WINDOW_AND_EFFECT7_PROVIDERS.md).
This supplies icon providers; complete native reward admission is still open.

## Retained source animation and named ownership

FieldItemRegistry retains coin403 in its separate fixed32 icon pool, without
creating a fabricated inventory/item-entry record. Fresh ready construction is
flags1001, frame0, texture414, count4 and delay6; the caller separately ORs102.
Constructor resets the new record while the later named position setter updates
the first ACTIVE matching name. Duplicate entry still allocates another slot.
Named flags/delete also affect the first active match. Native full-pool refusal
is explicit before an event should advertise readiness; original overflow and
other-context pools remain unbound.

Each admitted advanceIcons visit requires both native resources ready. Queued
2000 resolves403 metadata, clears pending, then advances on that SAME visit.
Delay decrements; zero advances the unsigned frame, wraps4, loads texture414+
index and reloads6. Visibility2 is checked only by drawing, so hidden icons
continue cycling. Revealing does not reset their phase. Delete clears active1,
preventing any later animation/draw. These are iconMain invocation counts,
not elapsed clocks or rendering frames. Static item95 retains its existing
separate finite behavior; complete generic pending-descriptor admission remains
outside this coin increment.

Renderer consumes the published frame index, never MessageIcon::frame(global
render/tick age) for403. It verifies four loaded frames and source texture/index
consistency. Shared field icon geometry retains source40x40×.6, bottom anchor,
white color, camera5 and source depth/alpha pipeline. Material40, mirror/full-size
flags and native separate shadow are explicit unbound providers. Drawing cannot
advance the frame counter. The existing linear vertex pool and atlas are reused.

## Simulation and camera owners

Native Playing order is Game's event/Mario/NPC/item visits, icon owner, then
sound, windows and effects. The added advanceFieldIcons call occurs before
Audio's later BGM visit. The finite native icon adapter currently admits room0
outside Scene::Battle; other original GP14/field-battle/pause domains remain
separate. Native animation cadence maps one admitted owner to one60Hz simulation
visit; no physical original/hardware timing measurement is claimed.

Game now runs its existing camera4 owner before the acquisition event visit and
publishes fieldItemCamera5Frame immediately afterward, before later Mario/event
writes. Renderer consumes that retained camera5 frame for item effects/icons;
it no longer reconstructs camera5 at draw time from a potentially changed
camera4. This corrects the source publication boundary beyond the previous
payload5 camera-selection fix. Source view vector copies and independent25
lens/1..100 native clip/.2.. .3 depth stay retained. Existing native up=(0,1,0),
aspect/projection/libm/roll and GX/PICA raster adaptation limits remain.

## Original assets and validation

Message-icon converter now includes actual icon.bin403, whose original four
frames are icon.tpl414..417,40x40CMPR with duration6. Original RGBA pixels are
packed unchanged with the existing2px padding in the same512x512 RGBA8 atlas.
No resampling, generated art or larger allocation: native atlas remains1MiB.
Exact converted frame hashes are recorded in [asset provenance](../../ASSETS.md)
and ignored artifacts/coin-icon-pixel-audit.json. Raw/extracted/compiled packs
remain ignored under the original Nintendo/Intelligent Systems restrictions.

Final isolated treeff860bbe78016b77ae9cf4f9bd72b18dc7831f70 passed make test:
247/247 C++ and142/142 Python; make all passed. Logs:
artifacts/coin-icon-test.log and coin-icon-build.log. Normal native artifact
artifacts/ttyd-coin-icon-reviewed.3dsx SHA256
d11a48afceb38a4fa482dc60fc826d2fa9d72d321ba3548ac3645a7b0e066d9f.
Host checks prove hidden cycles, six-call boundaries/wrap, delayed readiness's
same-call advancement, no reveal reset, first-active duplicate/delete/pool
admission, original four-frame metadata,24x24 geometry and camera5 snapshot
isolation. Read-only source review found no blocking finite403 mismatch.

Clean DEBUG_SHORT_BOOT artifact SHA256
b7c82596626d7b80366130d3e010490d6b132bf6d19903b9f919e79e6c6a041d
passed isolated original parcel/dialogue/get/return/save after publication change.
Log: artifacts/coin-icon-parcel-smoke.log; held window image:
artifacts/azahar-coin-icon-parcel/parcel-open-window.png. Native checkpoint
proves quest2,5405true/5406false, one95 and unchanged HP10/FP5. Fixture supplies
known room0/story8/quest2; private DSP profile does not touch user settings/saves.

Private coin raster preview uses the production registry/frame/draw/simulation
owners with original coin title and effect7. Captures:
artifacts/azahar-coin-icon-raster/coin-phase-0.png through17.png; log:
artifacts/coin-icon-raster-smoke.log. Diagnostic artifact SHA256
f0efee945ef7fa18d0c5017715499407b64364e19d6b0be0ff5a013fb73a40cd.
Preview setup/reveal-at13 visits is a harness choice, not the original coin
callback/clock. It performs no reward/quest writes or row219 audio/Mario get
motion. The checkpoint remains byte-identical. Captures show rotating original
coin variants next to original title/pink effect. Private source was restored;
audit matches the review tree. Raster screenshot0 reported59 appFPS/100% speed,
13.72ms and native HUD sim2.09ms/CPU8.3ms/GPU.5ms: one software-emulator frame,
not stable60, matched Dolphin pixel/audio parity or a New2DSXL benchmark.

Next: row219 PCM/NDSP resource and envelopes, native coin parameter retirement
on actual window deletion, source5376 persistence and real Game reward admission;
then original upstream task acceptance/full chain and physical profiling. Later
areas/events/objects, shared pools/shadows/pause and broad rendering remain open.
