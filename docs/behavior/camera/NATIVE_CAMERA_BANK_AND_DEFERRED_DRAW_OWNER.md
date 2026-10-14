# Native retained camera bank and deferred draw owner

`CameraManager` retains thirteen independent records initialized from the
[original boot table](CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md).
Caller supplies render dimensions; ordinary native boot explicitly uses608x480.
Names, callbacks, lens/projection types, viewport depth ranges, scissor, zero
views/extras and distance-before-eye-replacement follow that table. Native double
trig and SDK math are declared numerical adapters. Game retains the shared bank
across checkpoint restore and supplies it to trio entry. Entry changes camera4
mode to2; it does not reconstruct/reset its lens, roll or extra. Standalone
trio fixtures without a shared bank use an explicit native boot fallback.

Main updates ascending0..12 without draw-disabled admission. It captures each
indirect callback identity, then retains the continuation through LookAt,
roll/translation, field-only zoom and one-use extra. Missing services preserve
completed earlier cameras and callback/view work. Extra follows field zoom and
never performs a second identity view multiply. Native trio mode2 uses the
supported retained controller, distance/native numeric adapter, original angle
lookup and source perspective/ortho construction. Effect cameras5/7 copy actual
camera4 vectors and rebuild their own lenses; image6 only rebuilds its lens.
All callbacks precede their own view publication.

The [source tremor tail](BATTLE_CAMERA_TREMOR_TAIL_FLAGS_1_AND_2.md) now executes
both independent axes after baseline snapshots. Phase uses bit2 of wrapping
elapsed; interpolation uses signed truncation by2. It publishes amplitude before
zero/negative timer admission, restores prior speeds and decrements live timer.
Zero clears only its own flag; negative keeps it. Tremor changes the live camera
without republishing baseline/deltas. A retained tail cursor suspends at missing
interpolation without replaying baseline or prior-axis work. Existing unsupported
mode and arbitrary function-pointer interpolation domains remain unavailable;
this cursor does not certify those earlier mode handlers' resumability.

Finishing now runs the bank after late Main publication. Explicit pose-depth
matrices alone do not supply the camera callback/lens provider. Supported native
callbacks complete all13 records and advance to the separate camDraw8000f454.
The20 borrowed records and shadow requests remain intact. Published camera4 depth
matrices survive subsequent lens-adapter calls while draw is pending. Native
field lens/vectors/normalization and earlier field roll/extra history remain an
explicit adaptation gap; cameraHistoryComplete stays false for that adapter.

The portable draw owner implements letterbox→sort once→ascending admitted camera
current-index/viewport/scissor→live near/far branch/projection→actual queue dispatch
→dummy GPU provider→final reset. Current index publishes before setup, survives
holds/reset and never changes in Main. Near/far branch selection survives a
suspended helper even if live flags change. Callback and dummy holds retain the
queue and dispatch cursor; no earlier setup/letterbox/callback is replayed.
All-disabled draw still runs letterbox/sort/reset and preserves prior current
index. The native current-index adapter reports unavailable until a camera has
actually been admitted, rather than guessing an inherited index.

Draw is NOT yet bound to the native GPU/letterbox providers. Dummy is genuine
[state/triangle work](CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md), not a no-op.
The original BeroEXEC producer is recovered [separately](../overworld/BERO_EXEC_RETAINED_PRODUCERS_AND_PROLOGUE_TRANSITIONS.md).
Queued part/material/shadow callbacks, letterbox scalar/global owners, native
GX/PICA state adaptation and ordered BattleEnd/field waiter remain next work.
Existing visible actor rendering continues through the current citro3d adapter.
Neither portable draw tests nor native Main completion prove deferred raster or
original field return.

Tests cover thirteen boot records/projections, disabled Main callbacks, captured
callback identity, view/zoom/extra hold order, letterbox-before-sort, live near/far
selection, borrowed callback/current publication, dummy hold, final reset,
zero/negative/wrapping tremor timers, both-axis effects, speed restoration and
no baseline replay. Trio integration covers explicit missing-lens hold and real
resumption into camDraw with queue/prefix preservation. Exact validation and
native capture scope belong in PROGRESS.md; no hardware/Gekko parity is claimed.
