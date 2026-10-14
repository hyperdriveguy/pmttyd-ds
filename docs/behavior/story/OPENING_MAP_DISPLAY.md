# Opening map display: used US aaa prefix

The read-only source verifier is [verify_opening_map_display.py](../../../tools/verify_opening_map_display.py), with pinned body hashes in [opening_map_source_evidence.json](../../../tools/opening_map_source_evidence.json). This is the used `evt_prologue2` prefix, not a generic event interpreter or SDK parity claim.

After original `pro_05`, aaa waits300ms, changes Luigi to `I_1`, reads his position and adds(-25,+15,-3). Item `map`, original item96/icon314, enters with mode17 and scale563/1024. After1000ms it deletes the item, changes Luigi to `S_1` and Mario to `M_L_3`, then waits1500ms. `e_mario` supplies that original Mario clip with the `a_mario` texture binding. The used item mode resolves to mode7, skips item physics, and retains its source position for its one-second lifetime. Its native billboard uses the original icon dimensions and normal 3D icon quad constants(.5,.6), original item scale and camera yaw.

At aaa:1a7c, GF0 is cleared. The persistent inline `mapdraw` callback allocates eight bytes and the original `mariost.tpl` file. Its first and every later callback add2 to the alpha counter. Only values greater than255 clamp to255 and publish GF0=1. It registers camera4/layer7/depth1000 `draw`; this callback temporarily uses camera8, renders original TPL palette3 with white RGB, zero translation and unit scale, then restores the prior projection/view ownership. No map input gate occurs in this source.

The native map is the existing original608x480 raster reflowed to304x240 in the centered source viewport. It fades over the retained original house scene rather than replacing the backdrop before alpha becomes opaque. Simulation60Hz owns the finite source waits and alpha counter. Native texture admission starts the first callback; a missing resource cannot advance this producer. The native GF0 bridge additionally requires an actual alpha255 draw followed by `C3D_FrameEnd` and `C3D_FrameSync`. This presentation barrier is a native adaptation: the original callback publishes GF0 before registering its draw in the same callback. It prevents native resource or GPU delay from fabricating successful display.

After the native GF0 publication, the event waits1000ms, requests BGMoff_f(group512,2000ms), turns off environment group512, selects source mapchange49/50, stores GSW0=2 and changes to muj_20/prologue. This slice binds the story state, the source49/50 geometric transfer and Voyage admission. `mapDisplay.environmentOffRequested` retains the actual envOff512 request for the root house-sound owner; this bounded worktree does not invent environment-sound acknowledgment.

The used group512 music adapter is described in the [audio contract](../audio/OPENING_MAP_BGM.md).

Validation uses host stalled-resource/final-frame tests, source hashes/operands and native isolated emulator captures. It does not claim Dolphin raster or audible parity.

The [display contract](../display/OPENING_MAP_TURN_CAPTURE.md) owns the original49/50 geometry, dynamic material inheritance, GPU capture and resource lifetime. Voyage runs concurrently with50, matching its used init/event prefix; presentation completion does not introduce an event wait.
