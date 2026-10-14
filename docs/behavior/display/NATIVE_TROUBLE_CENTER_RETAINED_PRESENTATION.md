# Native retained Trouble Center menu presentation

Commitc4676af2 implements the finite TroubleQuestPresentation and binds the [portable producer](../field/PORTABLE_TROUBLE_CENTER_MENU_PRODUCER.md)
to the [cached native window](NATIVE_TROUBLE_CENTER_WINDOW_AND_US_CACHE.md)
and five exclusive named icon records. It requires original icon descriptors
504/524/134/136, known external quest/completion readers and an external source
sound-cue provider. Cache/resources are prepared before admission; no quest
acceptance write occurs here. Stable-owner guards reject copied owners.

[source retained icon recovery](TROUBLE_CENTER_RETAINED_2D_ICONS_AND_LOCAL_CLOCK.md)
defines the flags, frame metadata, geometry and separate iconMain clock. Hidden
504 ages; ordinary state0/iconMain/state1/iconMain produces first visible frame1.
The full cycle is46 admitted iconMain visits, not rendering-frame modulo.
Deletion clears active1; source No recreates icons while retaining menu cursor.
Native drawing consumes retained current frames without advancing animation.
Up flag8 negates bottom-anchored geometry Y and reverses UVs; X mirror retains
its centered horizontal anchor. The window is submitted before visible icons.

The native adapter uses existing Citro2D/Citro3D texture rendering and prepared
US window/font resources. The atlas remains512x512 RGBA8 (1MiB), with original
pixels, two-pixel transparent padding and no resampling. Exactly identical
bitmap cells share UV storage; every descriptor frame/delay remains separate.
No per-frame heap allocation, asset conversion or image decode is introduced.

## Validation

Final isolated make test passes261/261 C++ and145/145 Python tests;
make all passes for the restored normal native build. Review tree
3cf4b6bd6f7ef810e3f41d5cb54d7017659292fe, checkout
artifacts/source-checks/quest13-field. Logs: artifacts/trouble-presentation-
test-final.log and trouble-presentation-build-restored.log. Normal3DSX SHA256
537d25ef03f11b487662d3a361bacbe1db2adaa34e07aeae8d3f7b195320068c.
All2628 tracked review files were restored/audited after preview; main's normal
compile timestamp was restored and the normal binary rebuilt.

Host checks cover resource admission, original flags, hidden countdown,
first-visible frame1,46-visit wrap, resource-not-ready freeze, copied-owner
rejection, source result/deletion/reopen and source cues. Asset regression
compares every one of77 frame entries to original decoded RGBA pixels;73 unique
atlas cells retain exact aliases, rejecting partial overlap.

Private DEBUG_SHORT_BOOT preview uses original story401 list, two admitted
menu/iconMain visits (second with source R), explicit test quest0/completionfalse
and a silent test cue sink. It freezes subsequent visits to isolate drawing;
this is not gameplay or wall-clock timing evidence. All five icon records are
visible. Isolated Azahar screenshot/geometry assertions and byte-identical copied
checkpoint pass: artifacts/azahar-trouble-presentation-ready/retained-trouble-
presentation.png; artifacts/trouble-presentation-smoke-ready.log.
Preview3DSX SHA256b9ebb3634daf72369669c77ded63c99f843535befeed9879e03a4559d67a1b94.
No user emulator profile/save or ISO was changed; ignored local packs/binaries
remain outside history.

Initial atlas preparation exceeded capacity before pixel sharing. The earlier
nonoverlap test failed on intentional aliases; the final regression permits only
exact cells and verifies all source pixels. First10-second Azahar capture was
still black/loading and failed; the35-second boot capture above passes. Initial
logs remain artifacts/trouble-presentation-test.log and trouble-presentation-
smoke.log; they are not counted as passing. Software Azahar reports3 appFPS;
this does not establish New2DSXL performance or original raster/event parity.

## Integration boundaries

This is a callable presentation/backend, not admitted native board gameplay.
Full original gor_02 case9 map-object owner/geometry/lifetime, Trouble Center
door/lecture5374, all30 completion flags and working bank reset, yes/no help
and confirmation/acceptance events, source physical input/repeat and original
EVT/iconMain ordering remain separate providers. Source sound IDs42/18/19/5/43
are delivered to the required callback; their [original sample bindings](../audio/TROUBLE_CENTER_MENU_SFX_BINDINGS.md)
are documented, but native playback is not connected. No generic/generated
sound substitution or fabricated dialogue is used. Original GPU/camera/material
numerics and New2DSXL hardware performance are not established by this bridge.
