# Runtime sequence, input and field bridges

These notes cover the small runtime owners around `Game`, rather than the full
native main loop. See [runtime ownership](runtime.md), [field entry](field-entry.md)
and [Trouble Center](trouble-center.md) for their callers.

## Outer Main and original sequences

[Outer Main](../../source/runtime/game_outer_main.cpp) retains its phase, child
index and partial reads across a hold. Its
[33-entry table](../../include/runtime/game_outer_main.hpp) contains original
callee identities, not callable native pointers. Before visiting the table it
reloads GP and yields while GP+0x10 is nonzero. After a child acknowledges
completion, the index advances once. Missing services hold the same operation;
acknowledging the parent only certifies the services actually supplied.

The idle tail first tests GP flag 0x1000. It scans four channels for held
direction/button words and six analog byte arrays. Any nonzero value refreshes
the time baseline, storing the low word before the high word. All-idle handling
reads time, baseline low, hardware clock and baseline high in that order. The
wrapped 64-bit difference is interpreted as signed and divided by bus-clock/4.
Only a quotient strictly greater than 120 reaches the fresh sequence queries.
Sequence 2, or a separate fresh query returning 4, permits OR-ing GP flag 0x2000.
The routine supplies no fallback for an unavailable/zero-clock division.

[OriginalSequence](../../source/runtime/original_sequence.cpp) owns original
sequence IDs independently of the native `Scene` enum. Its eight-row table has
Init, Main and Exit columns. A request changes the requested ID and arguments;
it does not immediately replace current state. A transition waits for draw sync,
selects/runs the old Exit, records previous, clears work, then reloads the live
request. Exit may therefore change the upcoming transition. The new current ID
is published before Init admission. An invalid captured Init enters a sticky
unavailable stage, rather than retargeting a later request. A valid Init gets
work `[id, 0, p0, p1, 0, 0, 0, 0]`, then Main runs as its own child. An unchanged
request still invokes Main. Native field checkpoint import installs ID 2 without
executing original Init; callers must retire old owners separately.

Neither owner checks provider identity on resume. Keep the same service context
and nested child ownership throughout a held invocation. These interfaces retain
progress; they do not turn a missing implementation into a completed subsystem.
See the [outer Main contract](../behavior/platform/NATIVE_OUTER_MAIN_CONTINUATION_AND_SHARED_INPUT.md)
and [sequence contract](../behavior/platform/NATIVE_ORIGINAL_SEQUENCE_OWNER.md).

## Physical GP byte view

[The input facade](../../source/runtime/physical_input_gp.cpp) aliases the shared
`PhysicalInputProducer`; reads and writes never create a detached snapshot or
advance its repeat clock. GP+0x1324 is the ready word. From 0x1328 to 0x13b7,
nine arrays each contain four 32-bit channel words: held, pressed, repeat,
button countdown, released, direction, direction pressed, direction repeat,
and direction countdown. Byte accesses use big-endian shifts and read/modify/
write the same word. Countdown words retain signed bit patterns.

Force-motor bytes occupy 0x1310..0x1313; profiling offset 0x1314 is outside this
facade. The eight four-byte arrays at 0x13b8..0x13d7 hold main XY, sub XY,
trigger LR, motor mode and previous motor mode. Axes sign-extend 128..255 to
-128..-1; triggers and motor bytes remain unsigned. Unmapped offsets fail.

## Native position and registry ownership

[Registry binding](../../source/runtime/game_field_npc_registry.cpp) publishes
Game's exact NPC pool alias only after owner initialization succeeds. Binding is
not actor activation or permission to acknowledge later setup callbacks.

[Position services](../../source/runtime/field_position_native.cpp) expose Mario
and camera through typed semantic tokens; party addresses refer to actual native
party records. They are not interchangeable original heap addresses. Mario root
coordinates are native positions times 100; Y combines floor and jump height.
Stores multiply by .01 and preserve the current floor. This round trip can lose
source float bits. Known packed fields include body flags/size, retained camera
and collision roots, and optional recovery/reload words. Unknown fields remain
unavailable instead of reading allocated zeros.

Key suppression combines mask 8 with the separate Goomba acquisition/reward
locks. Preparation helpers admit their stated form/key-depth domains; camera
low-half updates retain the existing high half. The legacy no-constructor world
uses its actual current-party state. Constructor-backed worlds require the
current-index owner and real member lookups, including actual null members.

The finite Bero prefix builds stack-local synthetic argument addresses 0x100/
0x200 and event address 0x1000 solely for the shared callback interface. Encoded
LW10..12 references use the existing -30000000 namespace and Q1024 float decoder.
It runs Mario, party selector 0, selector 1, then camera reset once per arrival.
The retained original initial-position VM is a separate owner. Its pending state
cannot advance through a copied Game handle (`use_count != 1`); known entry,
registry and position providers must be admitted first. Simulation ticks become
milliseconds via `ticks * 1000 / 60`. Native floor/libm/FMA services remain finite
adapters. Completion publishes the returned positions; it does not certify a
complete original area-init parent. See
[position/camera boundaries](../behavior/field/NATIVE_POSITION_CALLBACKS_AND_ENTRY_CAMERA_RESET.md)
and [retained initial-position ownership](../behavior/field/NATIVE_RETAINED_BERO_INITIAL_POSITION.md).

## Trouble Center native providers

[The board/counter adapter](../../source/runtime/trouble_board_native.cpp) maps
physical channel 0 A/B to masks 0x100/0x200, vertical analog thresholds +/-0.5 to
0x1000/0x2000, and touch confirmation to the same UI input. Contact admission
checks room 2, interior-mask bit 4, story >=23 and the exact board/counter node identities.
A true contact return consumes the interaction even if loading or starting its
event failed; it is not proof of event completion. Pending work is tied to its
owning Game, and GW11 is published before the EVT visit.

Saved-state synchronization covers only the explicit quest domain: GSW1420,
GSF5374..5421 and bytes1469..1474. It is not a general save-memory mirror. The
turn adapter supports the actual finite Mario/party argument tuples, retains its
initial display/member selection, and uses native 60 Hz interpolation for a
200 ms turn (12 intervals). The endpoint invocation erases its retained turn and returns 1 (advance/yield). Missing party returns the original null result; unknown
services hold. Coins use the original signed pouch halfword at 0x78. Original
message/event data remain unchanged. See the
[provider boundary contract](../behavior/field/TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md)
for lecture, acceptance and cancellation ordering.

## HitN64 vertex shader

[The shader](../../source/runtime/hit_n64.v.pica) accepts already source-view XYZ,
two independently transformed/native-oriented UV sets, and GX register-0 RGBA.
Four row uniforms produce native clip coordinates using homogeneous W=1. CPU
packets own model/view conversion and the single native T flip. Repeating or
clamping UVs here would alter the original texture behavior. The color attribute
is register 0, not the original indexed vertex-color channel. Resource and draw
ownership are described in [Effect pool/HitN64](effect-pool-hit-n64.md).
