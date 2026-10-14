# Native battle camera requests, frame history and depth adapters

The native trio now binds mode/speed/set-zoom/add-zoom/homing/offset/MoveTo,
horizontal shake and Y-axis callbacks to their actual packed request writes.
Ending08 executes speed2, mode17/18 reset and zero-offset writes in original
order. Returning2 from a request does not assert completed framing.

`initializeBattleCamera` clears exactly B2754..2857 and applies the original
Work+8 mode selection and default fields. It does not reset camera vectors at
victory, change Work type or invent a setup4 camera type. Native Work+8 is an
input from its setup owner; full original setup-history correctness remains
separate. The initial offset vector is read from the existing original DOL
data bank, without creating a new asset format.

`BattleCameraFrame` retains camera eye/at/up separately from packed target,
old-vector and delta fields. The scalar update implements default14..16,
mode3 timeline and victory17/18 against actual packed unit home positions.
Source axis order, bounds, relative/mirror offsets, speed1 division by15,
speed3 delta/add, old-vector freezes, EyeZ cap and baseline publication are
preserved. Mode14 reaches the actual mode3 timeline; no stationary shortcut
skips its callbacks. Mode3 decrements before six interpolation calls, rereads
its control words for each axis and restores only speed277c. Source curves use
the existing portable intpl adapter; arbitrary callback-pointer curves remain
unavailable. Action modes0/7/10/11 now follow original current/world/hit
queries and signed kind-derived width/height. Width compensation is1.5.
The complete ordinary speed branches use1.5/15/5/direct/200 divisors, with
Z-speed5 divisor100 and the separate X/Y guard rejecting5 to1.
Unimplemented modes1/2/4/5/6/8/9/12/13/19/20 return unavailable.
Unsigned modes>20 follow the source no-handler common tail. Tremor admits its
real missing provider after baseline publication, rather than clearing flags.

The trio calls this owner once per admitted native simulation time after
battle-controller work and before event-manager execution. Repeated draw/depth
queries do not advance it. This is a declared native cadence adapter to the
recovered top-level source order, not a measured original hardware frequency.
The existing development hold still freezes ordinary simulation. The native
hardware rumble-off adapter has no physical motor on New2DS XL.

`cameraHistoryComplete()` reports whether all observed native controller
callbacks were supported. Once an unsupported callback occurs it stays false;
victory does not reset history. This does not prove original setup/global
camera entry/lens/roll/translation/projection history or rendering parity.
The frame accessor exposes actual retained native controller state for the
next matrix owner; it does not replace the fixed renderer camera.

The portable depth helper implements recovered paired grouping for view rows,
projection row3 reciprocal followed by numerator multiplication and source
FMA/clamps/sign for queue priority. XYZ is in the supplied snapshot's units;
the helper never mixes source positions with an unscaled native matrix.
Its LookAt/perspective constructor is a bounded finite numerical adapter on
supplied eye/at/up/lens inputs, with explicit host/ARM normalization/libm
differences. It supplies neither missing framing history nor C3D tilted
projection. Original exceptional/FPSCR/GQR0/NaN-payload bit behavior remains
unverified even though ordered NaN/clamp branches are retained.

The trio still holds at dispCalcZ8001086c: actual camera4 lens/matrix entry
history, remaining modes/shakes and queued callback ownership are open.
Source queue admission and its distinct immediate/deferred continuations are
[documented](../display/ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md).
This step does not claim original camera4 restoration or field return.

Source authorities: [Init/default/victory](BATTLE_CAMERA_INIT_DEFAULT_AND_VICTORY_MODE17.md),
[timeline](BATTLE_CAMERA_MODE3_TIMELINE_AND_MOVETO_CAPTURE.md),
[add zoom/Y-axis ABI](BATTLE_CAMERA_ADD_ZOOM_AND_NOMOVE_Y_EVENT_ABI.md),
[requests](../battle/BATTLE_CAMERA_REQUESTS.md) and
[depth/matrix writers](../display/BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md).
The [action-mode constant audit](BATTLE_CAMERA_ACTION_MODES_0_7_10_11_AND_FIELD_RESET.md)
supersedes the earlier incorrect speed1 divisor5 attribution. Code and tests
now use the independently reread original word41700000=15 at8042275c.
Host tests verify initializer extent, home-versus-current position, recurrence,
axis freeze, timeline/counter/restoration behavior, request gates, homogeneous
division, signed-zero/NaN clamp handling and ordinary source-space projection.
