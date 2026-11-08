# Battle camera tokens and native numeric adapters

[The shared camera engine](../../source/display/battle_class_camera_bridge.cpp)
provides admitted source-style camera words and finite native math.
[Class numeric services](../../source/display/battle_class_native_numeric.cpp)
and [weapon numeric services](../../source/display/battle_weapon_native_numeric.cpp)
adapt it to existing source draw/raster tables. These providers neither run
camera Main nor acknowledge unavailable GX, font, texture or display children.

## Native address policy and original field offsets

The caller reserves thirteen native camera slots, each 0x400 bytes, under an
aligned nonzero base. This is a virtual token domain, not a recovered original
Camera struct array or host memory pointer. Wider bounds reject a reservation
crossing 2^32. A pure range predicate must prove the entire reservation unmapped,
including shared file-cache data, at bind and subsequent provider boundaries.

`owns()` retains the whole range while bound, even when unavailable. Optional
word readers must route every address in it to the bridge; undefined words
cannot fall through into guest RAM. Only cameras 4 and 8 expose these aligned
original field offsets:

| Field | Offset/span | Representation |
| --- | --- | --- |
| Yaw | +0x114 | One binary32 word |
| View | +0x11c..0x14b | Twelve row-major words, affine 3x4 |
| Projection | +0x15c..0x19b | Sixteen row-major words, 4x4 |

## Admission pins an actual queued invocation

Class, HP and weapon bindings select original callbacks 0x8012210c,
0x8011fde0 and 0x80121ce8 respectively. The engine checks the selected actual
callback; wrappers do not rewrite a class queue record to impersonate HP or
weapon work. Main must be inactive at index 13 after the actual successful
producer. Draw must be active at stage/index 8, current camera 8, with an invoking
dispatcher and matching current queue record/callback/battle userdata.

A nonzero native generation/pass also needs the caller's real producer-current
proof. An idle default cursor or matching scalar key is insufficient. A held
pass pins Main/Draw/queue object identities. Queue resetEpoch must remain the
same nonzero value, so reset followed by identical record repopulation cannot
revive borrowed dispatch state.

Admission copies actual published finite view/projection/yaw for cameras 4/8.
Every provider compares those exact bits against the bank and rechecks producer,
range and dispatch. Loss while held poisons the binding even after apparent
restoration. Successful finish ends this short lease before Draw advances;
metadata-only retire permits a fresh bind. Completed geometry's long publication
proof must remain independent of an ended short camera lease.

The [weapon wrapper](../../source/display/battle_weapon_camera_bridge.cpp) also
freezes projection kind, all six viewport lanes and scissor; signed-zero float
changes are observable. The [HP wrapper](../../source/display/battle_enemy_hp_camera_bridge.cpp)
uses a typed copied XYZ path rather than fabricating guest stack storage.
The shared engine itself freezes matrices/yaw, not all raster fields.

## Optional queries and the restricted total reader

Numeric public word/camera/project queries check currentness around work and
return unavailable on missing/stale results. Matrix methods use a private request
copy and publish output only after completion under the same epoch.

The older source raster ABI has a total word function, with no hold result.
Its Camera8 request first captures all twelve view words through optional reads;
partial or stale captures expose no camera token. The private total reader then
admits only those aligned 48 bytes inside the reservation. Undefined camera
fields, premature use, stale epochs or missing ordinary total readers violate
a programming contract and abort. They do not return zero or fall through to
unrelated guest memory. Ordinary total reads require actual mapped, stable VM
storage; arbitrary readiness queries must use the optional API.

The raster facade exposes no guest stores, byte reader or lifecycle hooks.
Actual actor halfword reads keep their original context and cannot target the
reserved camera domain. Optional class party/pinch children keep their own
userdata; the numeric draw table's user remains the numeric adapter. Those
children retain their own partial source work.

## Existing helpers own the calculations

ScreenPoint and ring helpers retain their source coordinate/constants/order.
The bridge supplies finite view/project, native libm sine/cosine and existing
SDK adapters. No fixture lens, position scale or identity camera is added.

| SDK operation | Source token |
| --- | --- |
| Translation | 0x8029e230 |
| Radians rotation, ASCII x/y/z axes 120/121/122 | 0x8029dff0 |
| Scale | 0x8029e2b0 |
| Concatenation | 0x8029dd64 |
| Requested identity | 0x8029dd04 |

Interpolation delegates to the existing camera helper; the adapter only rejects
unavailable/nonfinite results. Signed truncation admits finite [-2^31,2^31)
inputs and rounds toward zero before casting. It does not invent exceptional
PPC conversion values. Weapon FMA is one binary32 fused operation, with finite
input/output and epoch checks; replacing it with separately rounded multiply
and add changes results. These are native admission/arithmetic policies, not
measured Gekko FPSCR, libm or raster parity.

See [camera ownership](../behavior/camera/NATIVE_CLASS_CAMERA_BRIDGE.md),
[class numeric providers](../behavior/display/NATIVE_CLASS_NUMERIC_PROVIDERS.md),
[HP camera admission](../behavior/display/NATIVE_ENEMY_HP_CAMERA_BRIDGE.md)
and [publication lifetimes](battle-publications.md).
