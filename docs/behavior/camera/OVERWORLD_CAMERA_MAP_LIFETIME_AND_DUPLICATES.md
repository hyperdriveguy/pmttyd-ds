# Original camera map lifetime and finite duplicate-point closure

US G8ME01 revision0, read-only static source reconstruction. This closes the
ordinary reset/caller lifecycle and the two finite noncardinal duplicate-record
domains left open by
[authored segment dispatch](OVERWORLD_CAMERA_AUTHORED_SEGMENT_DISPATCH.md).
No original execution, register-state observation or numerical parity is claimed.

## Process-global work, room reset values, retained timestamps

camShift's sdata pointer is at804169c8, source word803e1b30. Its0x38-byte work
lives in process-global BSS at803e1b30. It is not allocated per room and is not
part of GP/save-slot progress. The source BSS-clear table at800055b0 includes
this address; startup __init_data8000542c zeroes those spans. This is process
initialization, not a rule to erase global work on every map transition.

camShiftInit800f3978 and camShiftReset800f34d0 perform identical explicit writes:
current/desired shift, blend, signed accumulation, acceleration enable and
history rootXZ become0; accumulation cap300, increment cap3 and blend rate.002
are restored. **Neither writes work+18/+1c, the prior GP38/3c timestamp.**
Other unlisted bytes retain their existing values. Source camShiftPostMain
updates the timestamp after the camera callback. See
[the full leaf contract](OVERWORLD_CAMERA_COLLISION_TARGET_AND_SHIFT.md).

The ordinary map owner does reset these listed shift values:

1. _mapLoad80028f18, when mapdrv.activeGroup!=1 at800296b4–bc, loads camera4's
   authored room road through camLoadRoad8000f36c at800296dc. camLoadRoad calls
   camRoadSetup8000e104 at8000f3b4. Setup clears roadWork0x210 bytes and calls
   camShiftInit at8000e1cc **before** camera-container acquisition/validation.
   A failed road load therefore still performs this initialization.
2. _load800075e0 later initializes arrival/saved-position camera targets and
   calls camRoadReset8000aa14 followed by camShiftReset. The saved-position
   branch calls at80007768/76c; the common target tail at80007a38/3c.
   The saved-position branch also reaches that common tail, so it performs
   both pairs in source order.
   These calls are not deferred until the first held-input callback.
3. Current/desired/blend/accumulation/rootXZ history consequently reset on the
   common ordinary room transition. The source shift timestamp remains the
   previous camera callback's global-clock value until camShiftPostMain writes
   it again. The first post-reset increment can span map-load time.

camRoadReset preserves roadWork.container(+0), version(+1d0) and word+204,
clears all0x210 bytes, restores rates/weights and those three preserved words
(8000aa34–44 and8000aaf0–ab00). All road candidate/current/destination/origin
states and transition timestamps reset. It does **not** call camShiftReset;
callers explicitly pair them when required. camRoadSetup instead clears its
whole work and acquires/relocates a new container/version.

These camera reset/load owners do not reset GP38/3c or GP40/44. Maintain the
source global-clock adapter across room changes; resetting a room simulation
clock while retaining a timestamp invents negative/backwards elapsed time.
Shift uses GP38/3c; road motion/transition uses GP40/44. A native adapter can
rebase a clock and all dependent timestamps together only as an explicit clock
adaptation with equivalent elapsed intervals, not as the source reset behavior.
Full OS clock/paused-runtime producers remain separate services.

## Direct original DOL caller census

| Target | Direct caller site | Local ownership |
| --- | --- | --- |
| camShiftInit | camRoadSetup8000e1cc | Every road setup attempt |
| camShiftReset | _load8000776c | Saved-position initialization branch |
| camShiftReset | _load80007a3c | Common target initialization tail |
| camShiftReset | seq_gameOverMain800787c0 | Paired road reset after targets |
| camShiftReset | evt_bero_cam_shift_init80078cdc | Paired road reset; returns EVT2 |
| camShiftReset | bero_get_position_pipe2800796f0 | Paired road reset in pipe-position preparation |
| camShiftReset | evt_cam_shift_reset800a76e4 | Shift only; returns EVT2 |
| camShiftReset | evt_cam_road_reset2800a7824 | Paired reset after explicit XYZ target writes |
| camShiftReset | evt_cam_road_reset800a78e8 | Paired reset after Mario-root target writes |

The census scans original DOL text function bodies for direct branch targets;
indirect calls and other modules' runtime aliases are not a universal negative
proof. Full direct containing owners and all nine sites are source-pinned.

The two evt_cam_road_reset wrappers also write Mario+f8/fc/100 directly,
without invoking marioUpdateCamCollision or setting display40000. The no-arg
wrapper copies current Mario root at800a78cc–e0; reset2 copies explicit event
XYZ at800a77fc–81c. They also write follow ec/f0/f4, camera follow and collision
triples and call both resets. These are additional genuine event producers for
the look target; a direct-call census of marioUpdateCamCollision cannot find
these alias writes. Running applicable authored events remains a separate native
binding obligation; no generic root fallback should emulate them silently.

## Finite duplicate-point outputs

All ten duplicate segments in the shipped44 records have PiXZ==PjXZ,
Ui==Uj and TiXZ==TjXZ exactly. This includes previously unresolved gor_01
record2 C_ie_01 and record5 Cc_ie_04. All take the straight branch after
near-normal collapse. At zero chord, source projection coefficient is0 and
R=Pi. Source does **not** assign fraction FPR28 for this segment.

For any finite retained fraction f, source formulas are:

    U = Ui + f*(Uj-Ui) = Ui
    tangent = Ti + f*(Tj-Ti) = Ti

Both differences are exact0 in these records. Consequently the fraction does
not affect their finite parameter or raw tangent outputs. All other source
outputs use R, Q, distance, authored end bounds/caps, current segment index and
Pi.y. The general finite duplicate adapter may use fraction=0 **as an output-equivalent
adaptation for these exact rows**, while still evaluating this point segment,
source halfspaces, strict closest-distance test and caps in original order.
This is not a source fraction initialization rule.

Do **not** skip the two noncardinal duplicates merely because the preceding
segment usually wins. Its source projection can round differently, and source
distance ownership still decides which index/cap/tangent wins. On an admitted
zero chord, side test H.x*e.z-H.z*e.x is0, selecting rightCap; parameter equals
both duplicate start/end bounds, so strict endpoint comparisons do not set
end-cap flag1. A side clamp can still set flag2. An earlier equally distant
candidate retains ownership; a strictly closer point segment replaces it.

## FPR28 caller provenance and finite boundary

collisionCurve saves/restores caller FPR28 at8000c4a8–ac/8000d328–2c. Within
its loop the only fraction assignments are8000cfc8/cfe8; a duplicate uses the
most recent such fraction, or the caller's incoming value if none ran.

Normal first-query path is main→marioStMain→camMain(camera4 callback)→
cam3dMain→camRoadMain→calcCamRoad→collisionCurve. main, marioStMain,
cam3dMain and camRoadMain have no FPR28 assignment. camMain saves its incoming
value and its only FPR28 assignment at8000f8ac occurs **after camera4's callback**;
it restores at8000faa4–a8. calcCamRoad saves incoming FPR28 and does not assign
it before the initial query8000b9d4. Ordinary additional shift queries inherit
finite tangentX from calcCamRoad's fneg FPR28,FPR29 at8000bd60.

Startup __OSFPRInit802951d8 assigns FPR28 from ZeroF at80295278/2e8 and paired
lane from ZeroPS at802951f8/268. Both are .sbss fields8041f040/048. **Do not
claim a literal runtime0 solely from an ISO file byte:** those are BSS, and
__start calls hardware/FPR initialization before __init_data clears BSS.
An initial zero value relies on the source DOL loader's BSS initialization.
The normal finite cold-boot/ABI-preserving domain is sufficient for the output
independence above; an exact register word is unnecessary to that adapter.

The contract requires finite caller and computed fractions and finite query
math. NaN/infinite register context or projection overflow can make0*f nonzero
exceptional/NaN and remains outside this bounded ordinary contract. No debug
hook, arbitrary source ABI violation or exceptional OS context is simulated.
The finite closure covers all44 shipped record geometries; SDK rounding and
source exceptional numeric domains remain explicit adaptations.

## Verification

Run `python3 tools/verify_overworld_camera_map_lifetime.py` and
`python3 tools/verify_overworld_camera_segment_dispatch.py`. The first pins
reset/load/event/register owner bodies, source pointer/BSS table and caller
sites. The second additionally verifies all ten duplicates' exact zero deltas,
so the finite fraction-independence statement is data-backed. Dedicated evidence
contains metadata/hashes only. Native/shared progress/assets files are untouched.
