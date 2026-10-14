# Native Goomba3 parcel acquisition binding

Implementation begins at `3abf5525`; follow-up `d8efdb46` retains the outer NPC-talk
key hold independently from the inner item hold. This finite fresh item95 path
uses the [portable owner](GOOMBA3_PORTABLE_ACQUISITION_OWNER.md),
[Game admission](GOOMBA3_GAME_ACQUISITION_BINDING.md),
[source camera/key/system fields](ITEM95_CAMERA_KEY_AND_SYSTEM_LEVEL_PROVIDERS.md),
[original motion provider](NATIVE_ITEM95_MARIO_MOTION_PROVIDER.md),
[window provider](../display/NATIVE_ITEM95_WINDOWS.md), and
[original audio routing](../audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).
It does not accept an unknown quest state as a new story branch.

## Ownership and admission

Renderer binds services to the current Game/Audio before each Playing simulation
visit. Game owns the original pouch, named field-item registry, acquisition,
windows and finite effect6. These retained owners outlive individual render
frames. Their callbacks remain platform-independent; future item/area providers
can supply those services without putting 3DS APIs in the event owner.
Copied acquisition snapshots cannot advance another Game's retained pointers.

Admission requires room0, original Mario post readiness, original US window
assets/font, icon atlas, original M_I_U pose/texture, both original effect
textures/shader, actual original field/fanfare audio resources/DSP and a free
source key slot. Three native streaming slots are shared without eviction;
audio readiness requires a fresh fanfare group/tail and an available slot.
The full-storage/discard native UI is unbound, so a full key pouch refuses this
finite native path before story print/grant. Portable failure-state recovery
remains separately modeled and tested.

The native main order is Game's event/Mario/NPC/item owners, source BGM driver,
window manager, then effect tick. Item acknowledgment reads physical repeat
through the window manager. Simulation ticks own clocks; rendering only draws.
Original irai_02_00 attribution/order/control tokens, item95 name/help and
source flags are retained. The event resumes after the item owner removes its
entry, and only that later event visit writes5405. Native save v8 retains the
actual original pouch bytes; the embedded pouch is big-endian.

## Source state and rendering

Camera8 bit200 is retained separately from field camera4; no guessed viewport
crop or camera freeze is applied. Original letterbox decision/draw consumers
remain unbound. Level2 stores request2 and low GP mask3, stops the type6 talk
root, and leaves itemMain running. Level0 clears that nibble and resumes the
root for a later event visit. This is a finite owner, not a global event manager.
NPC talk and item acquisition each own KeyOff/KeyOn intervals using the source
wrapping byte and main8. ControlOff/On retains its independent byte/main2.
Sound flag80 is retained; other inherited flag1/2 audio-side consumers remain
outside this fresh path. Inline movement clear writes the native horizontal
speed; additiveX/Z and combined norm have no retained native producers yet.
Vertical/additiveY state is preserved.

Original M_I_U textures stay prewarmed while this quest is eligible, avoiding
reimport every render frame. Windows reuse cached glyph inputs. Effect6 uses
PICA vertex projection and texture-combiner stages, original44 repeating pattern
and42 alpha mask, followed by the independent original camera5 icon projection.
Camera5 copies current camera4 vectors and uses lens25, near1/far100 and source
depth .2.. .3. Unit conversion occurs once. The source initial shadow RGB is
black. A native screenshot shows a pointed mask fragment beneath the title;
the title occludes much of the star. No pixel parity or hardware raster claim.

Audio now defaults to the original ISO soundtrack, honoring the user's asset
preference. Original field PCM/buffers remain retained through Off3800 and
ResumeA0. The original key fanfare uses its independent group, source envelopes
and detached tail; presentation mute preserves source clocks and source queries.
Audible idle/fall On/Main/macros and complete original music routing remain open.

## Validation and remaining work

Initial private review tree78e91d5e3bad668e9eb3153db8a21c5801c9021c passed
make test:232/232 C++ and141/141 Python; make all passed. Normal artifact
artifacts/ttyd-item-native-reviewed.3dsx SHA256
6b3b484b1a907601814e43cb5e225cfd018c0076f7a7e176bf7107725090e52c.
Logs: artifacts/item-native-final-test.log and item-native-final-build.log.

Clean isolated Azahar validation uses DEBUG_SHORT_BOOT=1 on that reviewed
source, bypassing only frontend boot; artifact SHA256
599589e0fc5a28a2223cf6e190be35ec24838599c861bab9220d0f9fe5999e13.
The development checkpoint supplies room0/story8/quest2 and known false5405/5406;
Trouble Center acceptance is not exercised. Original dialogue, M_I_U, Box icon,
name/help windows and return are captured under artifacts/azahar-item-native-clean-pass.
The save proves quest2,5405true,5406false, one key95 and HP/max10/10 FP/max5/5.
Log: artifacts/item-native-clean-pass.log. Its isolated SDMC includes existing
project-local DSP firmware; no user emulator profile or saves are modified.
Earlier scripts failed from input/reentry and incorrect little-endian pouch
validation; corrected clean polling succeeds. Diagnostic tracing was private
and restored; no tracing code or binary/assets are committed.

Capture measurements are emulator observations, not original behavior or New
2DS XL hardware benchmarks: the earlier acquisition frame reported42 appFPS,
82% speed and19.02ms, while clean acquisition reported52 appFPS/92% speed and
10.31ms; later idle showed60 appFPS/100%. They establish neither stable60 nor
hardware performance. Cold startup remains slow. Profile allocation/texture
cost and benchmark real hardware; compare original effect/window raster and
audio output. Complete Trouble Center acceptance, Goomfrey delivery/reward,
discard/full storage, shared manager scheduling and later areas remain next.
The parent goal and ITEM-01/OW-INTERACT-01 remain open.

Final nested-key review tree d32b5f5c67c83fe34e41f2c1e5e67379bb3e8199
passed make test:232/232 C++ and141/141 Python and make all. Tests assert
outer talk depth1, item depth2, item cleanup depth1 and later event completion0.
Logs: artifacts/item-native-key-test.log and item-native-key-build.log.
Normal artifact artifacts/ttyd-item-native-key-reviewed.3dsx SHA256
4d2d9c4d8459836a9fc24d9d0e8a9fb437ea27f5c3ca466335bc7a9cf4b7f638.
Final DEBUG_SHORT_BOOT artifact artifacts/ttyd-item-native-key-debug-short.3dsx
SHA2564afde8d0e29dd10e05321c82ebcf45fd260b2e3521a2accae2b2442d3a7bc1b3.

Final isolated Azahar run passed with those nested holds, source checkpoint and
private DSP profile. Evidence: artifacts/item-native-key-final-smoke.log and
artifacts/azahar-item-native-key-final/acquisition-return.png. The native return
is Explore, ordinary talk is available again and the source pouch saves one95.
This validates the finite acquisition, not the upstream quest acceptance.
