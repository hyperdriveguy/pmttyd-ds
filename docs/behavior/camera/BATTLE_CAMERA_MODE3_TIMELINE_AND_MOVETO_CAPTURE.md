# Battle camera mode3 timeline and set_moveto capture

2026-10-14. Original US G8ME01 disc0 revision0 read-only source evidence.
Offsets/addresses are hexadecimal. Complements
[Init/default/victory modes](BATTLE_CAMERA_INIT_DEFAULT_AND_VICTORY_MODE17.md)
and [depth matrix owner](../display/BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md).
This closes default14→mode3 finite built-in curve4 framing; arbitrary pointer
curves and other Main modes remain distinct providers.

## Event wrapper and core capture

`evt_btl_camera_set_moveto8012b670:f0` takes nine event operands, resolved in
this exact order: priority via evtGetValue80031260; eyeX/Y/Z and atX/Y/Z via
evtGetFloat80030840; duration via evtGetValue; curve via evtGetValue. It calls
`btl_camera_set_moveto8012b760:114` with r3=priority,r4=duration,r5=curve,
f1..f6=those six coordinates, then returns2 immediately. It does not wait,
change mode or narrow float coordinates to integers.

Core captures supplied inputs and global B BEFORE camGetPtr8000f2fc(4),
then checks signed supplied priority against signed B275c. Even a rejected
request still performs the getter. Rejection leaves all packed fields intact.
On acceptance the captured camera C supplies the initial values, not retained
B2780/B2798 or the supplied target:

| Ordered source operations | Field |
| --- | --- |
| Camera eye C+c/10/14 copied | B27e0/27e4/27e8 |
| Incoming eye f1/2/3 stored unchanged | B27ec/27f0/27f4 |
| Camera at C18/1c/20 copied | B27f8/27fc/2800 |
| Incoming at f4/5/6 stored unchanged | B2804/2808/280c |
| Supplied duration word stored twice | B27d8 remaining, then B27dc total |
| Supplied curve word stored | B27d4 |

Stores are8012b7dc..82c. No duration/curve validation, priority increase,
speed/axis/offset reset or mode3 assignment occurs in this core. Capture includes
any current camera shake/view history present at request time. Native provider
must supply that current typed camera snapshot and preserve this capture order.

## Mode3 per-update order

`battleCameraMain` has already copied retained eye/at to camera C and old
snapshots before dispatch. Unlike every other mode, mode3 does NOT clear
B27d8 at the common pre-dispatch gate. Its entry is8012caa0:

1. Read signed remaining B27d8. If<=0 skip all interpolation/MoveTo and go to
   diagnostic sprintf/common tail. Mode stays3; negative/zero does not call
   a default handler, reset a timeline, or force an endpoint.
2. Save signed HALF B277c in a retained register. Decrement remaining with
   wrapping32-bit subtraction and store B27d8 BEFORE interpolation.
3. Six calls to `intplGetValue80006c8c` in eyeX,Y,Z,atX,Y,Z order. For EACH
   call reread curve B27d4, total B27dc and newly remaining B27d8; step is
   wrapping(total-remaining), duration is total, floats are captured start/end
   pairs above. Returned eye values go to local stack floats, at values to
   retained FP registers. Do not reuse an earlier elapsed clock or increment
   between axes.
4. Write HALF B277c=3 then B277e=3. Call `battleCameraMoveTo8012c128(0,0)`
   with the six interpolated values and six delta-output pointers.
   This skips relative/mirror/offset and X clamps; speed3 uses direct
   target-current delta followed by single addition, and writes up(0,1,0).
5. Restore **ONLY B277c**, using the saved signed-halfword bits. B277e remains3.
   Source store8012cbbc is not a two-speed restoration.
6. Diagnostic sprintf8026d5fc with original format802f0304, then the ordinary
   common EyeZ cap, axis freezes, retained vector/delta publication, rumble and
   admitted shake tail from the preceding contract.

The first remaining120 callback decrements119 and samples step1; the last
remaining1 callback decrements0 and samples step120. Next callback skips
MoveTo and retains published baseline. Same-time repeated rendering must not
invoke this state transition; it is camera Main cadence, not draw count.
An accepted new set_moveto replaces all12 capture/target floats and all three
timeline words but still requires an explicit mode3 request to be consumed.

## Actual default14 transition

After default14's counter reaches10, source resets its counter B2758, invokes
mode setter8012c028(priority0,mode3), then invokes set_moveto(priority0,
duration120,curve4,eye0/110/750,at0/60/0). Accepted mode reset sets both speeds1
and clears B27d8 indirectly on subsequent non3 Main paths; it does not itself
invoke Main. The new timeline therefore first runs on the NEXT camera update,
not during that same mode14 handler. Current frame's common tail still executes.

Timeline start is the then-current camera eye/at. In the bounded fresh rank0,
flags0/no-offset/default14 case these already equal the same target established
by its first callback, so delta0 yields stable interpolation; this is source
data/history, not a reason to bypass capture or remaining-count writes.
Other rank/type/offset/shake history can give a nontrivial transition.
Mode3 does not automatically become mode0 or1 when completed. A later admitted
event/ending mode17 request changes the mode and new Main clears B27d8.

## Built-in curve4 and native numeric scope

The original [interpolation contract](OVERWORLD_CAMERA_INTERPOLATION_AND_TIMER_DOMAIN.md)
and [mode4 operation evidence](../battle/AUDIENCE_LIVE_SOUND_WINDOW_THROW_AND_DISPLAY_ADMISSION.md)
pin intplGetValue80006c8c and its dispatch. Duration0 returns end before mode
dispatch, though nonpositive remaining means this mode3 loop will not call it.
For the actual duration120/curve4 calls let R round each result to binary32:

```
s = R(signed step); n = R(signed total)
r = R(n-s); d = R(end-start)
squareN = R(n*n); squareR = R(r*r)
sum = R(start+d); product = R(squareR*d)
quotient = R(product/squareN)
result = R(sum-quotient)
```

No clamp/easing endpoint shortcut, millisecond conversion, cosine or callback
is used by mode4. Existing `cameraInterpolation` is a portable mathematical
adapter for this built-in curve; preserve source rounding points if claiming
the finite operation sequence. Unsupported mode words are callback addresses
in original intplGetValue, not a linear/default curve. Such capture requires an
actual callback provider or an explicit retained request.

## Cadence and source fingerprints

Original marioStMain order is animMain→lowercase battleMain→camMain→window,
map,hit→evtMgr. Thus mode3 updates once per admitted camera callback and observes
current controller requests before this frame's new event requests. Fixed
native simulation ticks may be a declared cadence adapter; no hardware frame
rate or original runtime trace is claimed. Matrix consumers read the resulting
snapshot after camMain's LookAt/roll/offset and projection publication.

Ignored listings/ELFs: endingcamera-setmoveto and endingcamera-setmoveto-event,
plus calcdepth-battleCameraMain under artifacts/recovered-temp-evidence.

| Source span | SHA256 |
| --- | --- |
| set_moveto8012b760:114 | 3b10640878924544c5c9fd1519b1ba9977450a628f6ff9128c956fb287df52d7 |
| event8012b670:f0 | 1c95c5b96fb6311d18cc5cc6e6c7edb9ccc32b8c001942af34c67de6274e2238 |
| Main mode3 span8012caa0:130 | 517114db94cdb143fd33f5cf77dcb52440d2f159aff7a051e4d07542dfea7e8c |

Documentation-only static recovery. No code, shared index/progress or assets
modified, and no commit.
