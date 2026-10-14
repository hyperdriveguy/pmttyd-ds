# Native field camera4 publication and battle inheritance

The native field frame now publishes into the retained shared camera4 record
before battle mode owns it. This bridges the currently visible native field
pipeline; it is not a claim that all original field camera writers/history or
SDK numerical behavior have been reproduced.

`publishNativeFieldCamera` accepts a finite, nonsingular perspective field frame.
It converts eye/at and view translation to source units by100, and converts the
six-coefficient native projection into a full source matrix with its translation
term scaled by100. FOV is unchanged; near/far convert by100. The same physical
point therefore retains normalized projected XYZ. Current field yaw/distance
are published. Mode, raw up, aspect, viewport/scissor, flags, roll, translation
and pending projection-extra ownership are retained. Unsupported/invalid input
returns without changing the record. This adapter does not consume extra matrices
or run another source callback.

`CameraState::viewPublished` is native availability metadata, not an invented
packed original field or a history-completeness assertion. Boot clears it.
CameraManager Main sets it at its actual view publication boundary; the field
adapter sets it only after accepting its complete candidate. A fresh battle
runtime inherits the existing published matrices for initial depth consumers and
marks full original history unproven. It does not construct an identity/boot view
or require a manual depth/lens hook. No view is inherited if none was published.

Field camera ticks stop while retained camera4 mode2 owns the view. The normal
battle loop no longer calls the diagnostic field-lens setter on every tick, so
battle lens changes survive. The independent native world and audience facing
prefer the retained published eye/at/up, rather than an unpublished controller
frame. The actual shared Game camera owner is also available before the attack
runtime exists, avoiding a missing-owner scene during intro presentation. Lower
scene replay still uses the same prepared geometry/projection.

Tests cover coordinate equivalence, preserved retained fields, atomic rejection,
mode2 refusal, real constructor/shared-owner inheritance and Game tick handoff.
Existing explicit missing-depth and resumed ending/queue tests remain meaningful:
a diagnostic can still deliberately remove a supplied snapshot. Initial view
availability now has a real native producer; original normal camMain/camDraw
cadence, roll/extra fidelity, field return and complete battles remain unfinished.
No source unit/model scale, control, dialogue, story grant or simulation tick is
changed. [Source entry/history](UNDERGROUND_BATTLE_CAMERA4_ENTRY_AND_INHERITED_LENS.md).
