# FIELD-RECOVERY-01 — Water/void recovery stages, pose domains and pipe paper (field/AC-03)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

- [ ] Original `mot_damage` water-motion31 numeric stages (sink/rise/arc/travel/return counters, velocities, accelerations, jerk rates, reach heights, HP-penalty tick, phase transitions) and `mot_bottomless` motion33/34 placement/fall parameters, so the source-derived constants in `include/water_recovery.hpp` can be replaced before any recovery-fidelity claim.
- [ ] Original `marioChgPose`/`marioChgEvtPose` table slice for pose domains 4..9, 14, 15, 16..17, 18..20, 21..28, 30, 32, 34, 35, 36 and 37..40 (names, entry conditions, ordering), plus the original pipe paper (`p_dokan_x`) asset decoding/link deformation, exit/unlink, transform and movement lifecycle.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-12),
field/AC-03:source/water_recovery.cpp implements ordinary-Mario water/void
return-fall (mot_damage water motion31 and mot_bottomless motion33) as portable
fixed-tick motion with stages0/1/10/11/15/20/33 and authored parameters:sink
velocity-0.06 to root-1.6,40-tick rise hold with+0.003,two-tick sway then lift
arc+0.2 decaying by-0.01 to minimum0.1 until root+5,travel step distance/0.08
at stage15,drop-back fallVelocity-0.001/fallAcceleration-0.002/fallJerk+0.00024/
fallJerkRate-0.00004,and followDelta=(returnY-admittedRootY)/travelTicks;the HP
penalty is applied on landing. include/water_recovery.hpp marks these counters
as source-derived,not Dolphin observations. Pose ownership/timings are recovered
in OVERWORLD_MARIO_RECOVERY_POSES and bottomless detection/return in
MECHANICS.md(lines536-545:threshold-2000,+37 Y,300-unit probe,return index);the
numeric stage contract itself is NOT. Please recover original mot_damage
water-motion31 numeric stages(sink/rise/arc/travel/return counters,velocities,
accelerations,jerk rates,reach heights,HP-penalty tick and phase transitions)
and mot_bottomless motion33/34 placement/fall parameters so the portable
constants replace source-derived values before any recovery-fidelity claim.
No lock reserved;no original trace or game/hardware claim exists today.

Native implementation source audit / documentation request (2026-10-12),
field/AC-03:OVERWORLD_MARIO_RECOVERY_POSES documents water/void/pipe POSE
ownership and leaves original pose-table owners open for domains4..9(NPC/
switch/stand/small jump),14 talk,15 item,16..17 hip,18..20 hammer/jabara,
21..28 paper/partner transformations,30 grasp,32 spike damage,34 reset,
35 shadow,36 party use and37..40 Bowser/dummy,next to the already-closed
ordinary0/1/2/3/10 entries. The native game.cpp recovery scheduler selects
ordinary poses only and needs the original marioChgPose/marioChgEvtPose table
slice for those domains(pose names,entry conditions,ordering). Separately,the
same doc requires completing original pipe recovery paper(p_dokan_x) asset
decoding/link deformation,exit/unlink,transform and movement;native bindings
currently use M_Z_1 as a static stand-in and must not be treated as the paper
geometry. Please recover the pose-table slice and the pipe paper lifecycle;
leave further field presentation/sound/source/format claims unproven. No lock
reserved.
