# Battle camera mode, homing, speed and zoom requests

US G8ME01 revision0, inspected 2026-10-12. B evidence is original instructions
and constants. This covers the four camera callbacks used by the
[zero-gravity script](BATTLE_DEFAULT_ACTION_EVENTS.md), their setters and the
mode setter's axis-flag helpers. Camera main/update/render modes remain separate
work. See [baseline](../../ORIGINAL_BEHAVIOR.md). No original execution or Switch
comparison was performed; original disassembly remains ignored.

## State fields and shared numeric gate (B)

All routines below use global battle state B. Important relative fields:

| Offset | Established use |
| --- | --- |
| 2754 | camera flags; axis bits4/8/16 and mode-reset bit32 |
| 2758 | word cleared on accepted mode set |
| 275c | signed word admission threshold, referred to below as priority |
| 2760 | requested mode word |
| 2764 /2768 | first/second translated homing unit IDs |
| 276c /2770 | corresponding body-part IDs |
| 2778 | zoom float |
| 277c /277e | speed-level halfwords |
| 2810..2818 | first cached homing XYZ |
| 281c..2824 | second cached homing XYZ |

Offsets are hexadecimal. The setters for mode, speed and zoom reject when
signed requestPriority<B+0x275c. Equal/higher requests are admitted. None of
these setters writes that threshold word, so admission does not itself raise
priority or establish ownership. Threshold producers/reset lifetime are open.
This is a numeric protocol; do not assume the callback's first argument selects
an independent camera object. Homing's different argument handling is below.

## Mode callback/setter (B)

evt_btl_camera_set_mode at0x8012bfd0,size0x58 resolves both operands through
evtGetValue, calls btl_camera_set_mode(priority,mode), then returns2 immediately.
It does not wait for the requested view to be reached or report admission.

btl_camera_set_mode at0x8012c028,size0x84 applies the priority gate. If accepted,
perform these operations in order:

1. Write requested mode word to B+0x2760, without numeric mode validation.
2. Set zoom B+0x2778=0.0f (constant US0x8042273c).
3. Write halfword1 to B+0x277c and+0x277e.
4. Clear word B+0x2758.
5. Call btl_camera_nomove_x_onoff(0,0), then y(0,0), then z(0,0).
6. Clear bit0x20 from B+0x2754.

Steps5's zero-priority requests have their **own** gates. If stored priority
is positive, an admitted high-priority mode change can still leave axis flags
unchanged because those internal requests have priority0. The final bit0x20
clear is unconditional after admission. A rejected mode request skips every
listed write/call. No homing unit/part/vector field is reset here.

Axis helpers are at0x8012b33c (x),0x8012b2ac (y),0x8012b21c (z), each size0x38.
After their own admission check, onoff exactly1 ORs4,8 or16 respectively into
flags+2754. Any other value clears that axis bit. Nonzero values other than1
therefore clear rather than set. They do not change zoom, priority or speed.

## Speed-level callback/setter (B)

evt_btl_camera_set_moveSpeedLv at0x8012b8cc,size0x58 resolves priority and level
through evtGetValue, calls btl_camera_set_moveSpeedLv and returns2.
Setter0x8012b958,size0x3c rejects lower priority. Otherwise signed levels0..5
are stored as a halfword at+277c; all other signed values store1. Reload that
halfword signed and copy it to+277e. Thus these two fields are synchronized
on this request, including invalid-level fallback. No interpolation or speed
vector calculation occurs in this setter itself; level-to-speed mapping remains
part of camera update recovery.

## Zoom callback narrowing (B)

evt_btl_camera_set_zoom at0x8012ba14,size0x80 resolves priority via evtGetValue
and zoom via evtGetFloat. It converts the float to a signed integer word using
fctiwz, then converts that integer back to binary32 before calling the setter.
For finite in-range inputs this is truncation toward zero:200.9 becomes200.0,
−200.9 becomes−200.0. Fractional precision is discarded at the wrapper boundary.
Full float coercion and out-of-range conversion behavior require their source
contracts; do not impose invented zoom clamping on this path.

btl_camera_set_zoom at0x8012bab4,size0x18 only checks priority and stores its
float argument at B+2778. The leaf setter itself does not truncate or clamp.
Direct callers can consequently differ from event-wrapper callers. The callback
returns2 regardless of whether the setter admitted the request.

## Unit homing callback (B)

evt_btl_camera_set_homing_unit at0x8012be38,size0x198 resolves all three operands
through evtGetValue in order. The first resolved value is discarded. There is
no priority comparison or camera-index selection in this wrapper. It then
uses global B and the current event for BattleTransID.

For the first unit operand:

1. Translate ID, write B+2764.
2. BattleGetUnitPtr(B,ID), call GetBodyPartsId(unit), write B+276c.
3. BattleGetUnitPartsPtr(ID,bodyID); the returned part is not used afterward
   in this first-target path, but the call still occurs.
4. GetHomePos(unit,&x,&y,&z) and store those three float bit patterns at
   B+2810,+2814,+2818.

No missing-unit guard exists before body/home calls. The first vector uses home
position, without body-part offset, hit offset or height adjustment here.
Initial copied data words in the temporary vector are overwritten by all three
returned coordinates before the final stores; do not treat them as additions.

If the **resolved second unit operand** is exactly−1, exit this path immediately:
B+2768,+2770,+281c..2824 retain their previous contents. This is not a target
clear. Otherwise translate second ID, write B+2768, resolve unit/body part,
write B+2770, get its part pointer, and call BtlUnit_GetHitPos(unit,part).
Store hit XYZ at B+281c,+2820,+2824. The second vector is therefore hit position,
not the first target's home-position rule. Special IDs other than−1 are translated
normally, including−2 using event owner. Unit/part validity is assumed.

Return2 after the stores, without waiting for camera motion. The later mode's
use of those cached vectors and unit IDs must be recovered; this wrapper alone
does not establish continuous tracking or one-/two-target camera framing.

## Zero-gravity script consequences (B/P)

The inspected script requests mode(0,7), homing(0,−2,−1), speed(0,1), zoom(0,200)
and later mode(0,0). If stored priority≤0, these gated writes are admitted:
mode sets first reset zoom/speed/flags, homing updates first actor/home target
while preserving the old second target, speed selects1, zoom stores200; later
mode0 resets zoom to0 and speed levels to1 again. It does not restore a saved
prior mode/zoom/speed configuration. If priority>0, mode/speed/zoom reject while
homing still mutates the first target. These are derived request outcomes, not
observed camera shots. All four callbacks complete immediately locally.
The message callback between configuration and mode0 can still block separately.

## Differential obligations and remaining work (P)

Compare priorities below/equal/above the stored gate, accepted mode with axis
resets rejected internally, onoff1 versus2, level−1/0/5/6, wrapper fractional
zoom versus direct setter, first home versus second hit positions, resolved−1
preserving a stale second target, and homing updates despite rejected mode.
These are proposed original/native comparisons, not execution traces.

Remaining: priority/flag/vector producers and reset lifetime, main camera modes
including7, continuous target recomputation, level speeds/zoom/framing/rendering,
float coercion and hit-position services, other homing wrappers and actual
camera/message execution comparisons. Complete camera and complete-game
requirements still depend on those contracts.
