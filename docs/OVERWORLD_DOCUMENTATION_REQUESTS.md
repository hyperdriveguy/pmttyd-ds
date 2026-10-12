# Overworld documentation requests

Requested by the native overworld implementation session, 2026-10-12.
This dedicated inbox avoids concurrent edits to DOCUMENTATION_REQUESTS.md.
The documentation slice was explicitly requested; no independent research-agent
ownership or full-game completion claim is made.

## OW-MOVE-01 — Ordinary Mario movement state and current input

Partial answer: [ordinary movement contract](behavior/OVERWORLD_MARIO_MOVEMENT.md).
Read-only US ISO pins sixteen containing bodies, eighteen instruction anchors and
six constants through `tools/verify_overworld_mario_movement.py`. Delivered exact
ordinary key→stick-query→motion→movement ordering, same-call motion dispatch,
strict3025 walk/dash threshold, entry-owned retained speed, jump-entry snapshot,
launch phase/countdown ownership, and plain-air retained travel plus current
input drift/cap. Corrects native immediate air walk/dash selection and neutral
stick freeze; launch contact policy must begin before movement without adding a
vertical startup tick.

Remaining requested documentation:

- Complete idle/walk/dash jump/transform/threshold arbitration, including multiple
  surviving motion requests in one dispatcher invocation. Breadcrumbs: mot_stay
  800a7120→800a71a8; mot_walk80087bf8→80087c4c; marioChkJump80092814;
  marioChkTransform80092624; marioMotion8009461c/marioChgMot8009455c.
- Complete input producer stick clamp/deadzone and native signed-axis adapter
  domain. Breadcrumbs: marioUpdateKeyData80095cb0, marioGetStick80095af8 and
  GP+13b8/13bc, physical PAD service documentation. Native .15 radial deadzone
  is provisional and is not documented by these field consumers.
- Airborne wall deflection and floor/slope movement-rate composition. Breadcrumbs:
  marioMoveMain800594a8 calls marioGetMoveRate800969a4 then marioCheckWall8008b790;
  special flag paths and angle/speed writes need all owners.
- Full ledge-loss, landing/reentry, external motion, movement badge0x143, Paper/
  background modes, conveyors and additive velocity producers. Local motion
  leaves alone do not establish all persistent storage owners.
- Original isolated Dolphin traces: stable walk/dash launch, neutral release,
  opposite/orthogonal air input, idle launch, threshold crossing+jump, wall hit,
  slope and ledge loss. Record original revision/controller/input timestamps and
  motion/state/position fields; distinguish manager invocations from video frames.

No ISO writes, original asset redistribution, emulator profile/save mutation or
story/dialogue replacement occurred. Inspection outputs remain ignored or in/tmp.
