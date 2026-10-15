# Packed battle camera requests and frame updates

See [stage storage](battle-stage.md) for scene owners and
[display services](display-services.md) for camera/queue adapters. This guide
explains the packed CPU camera helpers; it does not establish original scheduler
cadence, prior camera history or complete native battle-camera integration.

## Work and requests

Camera work spans `B+0x2754..0x2857`. Key offsets are hexadecimal:

| Offset | Meaning in these helpers |
| --- | --- |
| 2754 | Tremor masks 1/2, axis locks 4/8/16, offset mask 0x20, rumble suppression 0x40 |
| 2758 / 275c / 2760 | Visit counter / current priority / mode |
| 2764/68, 276c/70 | Owner/target unit IDs, corresponding part IDs |
| 2778 / 277c/7e | Zoom / XY and Z speed halfwords |
| 2780 / 278c | Eye baseline / previous eye |
| 2798 / 27a4 | At baseline / previous at |
| 27b0 / 27bc / 27c8 | Relative XYZ offset / eye delta / at delta |
| 27d4/27d8/27dc | Timeline curve / remaining / total |
| 27e0/27ec, 27f8/2804 | Timeline start/end eye, start/end at |
| 2810 / 281c | Captured homing home / target hit vectors |
| 2828/2c, 2830/34, 2838/3c | Two tremor curves, totals, remaining counts |
| 2840/44, 2848/4c, 2850/54 | Tremor start/end amplitudes, current amplitudes |

Requests admit priority greater than or equal to the stored signed priority;
they do not raise it. EVT wrappers still return completion 2 when priority rejects
an operation. Mode resets zoom/counter and speeds to 1. Its internal axis unlock
has a separate priority-zero gate, then it unconditionally clears offset flag
0x20 without clearing the coordinates. Setting offset publishes that flag and
XYZ. Relative frame movement adds the stored offset irrespective of flag 0x20.

Speed requests accept 0..5 and reload XY before storing Z. Movement itself admits
XY only 0..4 but Z 0..5, resetting invalid values to 1. EVT Zoom/AddZoom truncate
the resolved float to integer before converting back to float; direct Zoom does
not. Axis requests use only `on == 1` to set their bit. Explicit-part homing honors
priority; body-part homing resolves and discards it. Both preserve discarded
part lookups. Target -1 preserves existing target fields after owner publication.

## Source framing modes and numerics

Init clears 0x104 bytes: setup type 0 selects mode 14, 1..8 selects 15, 9 selects
16; other types retain cleared mode 0. Default offsets come from `0x802f0240`.
The move-to producer captures CPU start/end vectors and timeline values but does
not select mode 3 or advance time.

| Mode | Implemented framing |
| --- | --- |
| 0 | Default eye `(0,110,750)`, at `(0,60,0)` |
| 15, 16 | Snap startup framing at eye Z500; mode 16 uses X=-100 |
| 7 | Single unit center |
| 10 | Single hit part |
| 11 | Two-part framing using stored home/hit span |
| 17, 18 | Victory home center; 18 lowers Y by 20 |
| 14 | Startup rank-depth sequence, then move-to |
| 3 | Six-component timeline interpolation |

Other modes at most 20 return unavailable after prefix stores. Values above 20
follow the ordinary tail without mode movement. Missing action unit/part resets
mode 0 but does not run default movement again in that visit. Victory requires
Mario; a missing partner leaves Mario's home as the center.

Smoothing divisors are `[1.5,15,5,1,200,100]`, with level 3 snapping. Relative
battle flag 0x100 adds Mario home Z to eye/at Z and `-homeZ/4` to Y, then the stored
offset. Both eye/at X clamp to `±(100+max((600-eyeZ)/2,0))`. These are source world
coordinates and policy coefficients, not 3DS pixels. Explicit binary32 boundaries
and FMA calls preserve the current port's ordering; they are not proof of Gekko
floating-point parity.

Action widths subtract 24 and clamp at zero; single-unit Y applies half of
`height-50` and clamps to 0..100. Two-part framing clamps span ratio to 0.5..1,
using 10 when the stored denominator is zero. Source coefficients 1.5/80/20/9
and eye/at bases 110/60/750 retain their local expression order. Victory averages
Mario/partner home, uses eye offsets Y100/Z350 and at Y50 with absolute Z0.

Mode 14 snaps on visit 1 to rank 0..3 depth `[750,830,920,1000]` (other rank: zero).
Visit 10 selects mode 3, duration 120, curve 4. Mode 3 decrements remaining before
six independently reloaded interpolation calls. On success it forces both
speeds to 3, moves, then restores **only XY**; Z remains 3. These are main-call
counts and source interpolation parameters, not observed render timing.

## Publication and tremor continuation

The ordinary prefix copies baseline eye/at into the CPU frame and previous
vectors. After mode movement it caps eye Z at 1000, then axis locks restore old
coordinates and zero corresponding deltas. Thus a lock can restore Z after that
cap. It publishes the unshaken baseline/deltas and issues two independently gated
rumble-off requests before the tremor tail.

Only this tail has retained continuation. The caller must retain B, frame and
services; the cursor does not validate their identity. Stage 0 captures speeds,
forces level 3 and computes sign phase `(total-remaining)&2`. Stage 1 can hold
amplitude interpolation, leaving temporary speeds and earlier baseline stores
visible. Step/duration use signed division by 2. Stage 2 moves only while remaining
is positive, clears its flag only at zero, restores speeds and decrements even a
zero or negative timer. CPU tremor is not republished into the baseline vectors.
The second lane follows the first, preserving earlier effects across a hold.

A local/default cursor cannot retain a tail hold across calls. Mode-3 interpolation
and other prefix failures have no retained suffix at all; restarting can repeat
stores or decrements. Do not treat every `false` as an effect-free retry.
