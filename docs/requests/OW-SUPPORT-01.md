# OW-SUPPORT-01 — Direction rate, slope record, support and stairs

**Status: OPEN (direction rate, support/slopes/upstairs and airborne/landing contracts delivered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Exact original triangle/math/traversal/aliasing and exceptional delta
  domains.
- [ ] SDK/camera/vertical writers.
- [ ] Dynamic surfaces/attributes/force/root matrices.
- [ ] Special modes/poses/sounds/hazards.
- [ ] Isolated source execution traces.

## Request text (verbatim)

Remaining requested dependencies: exact original triangle/math/traversal/aliasing
and exceptional delta domains, SDK/camera/vertical writers, dynamic surfaces/
attributes/force/root matrices, special modes/poses/sounds/hazards, and isolated
source execution traces. Partner ownership remains independent under ID1.
No native source, assets, emulator profile/save or original ISO mutations by
this documentation slice; inspection artifacts stay ignored or in/tmp.

## Answers and delivery notes (append-only history)

Delivered [direction movement rate](../behavior/overworld/OVERWORLD_DIRECTION_MOVEMENT_RATE.md)
with its6-body/27-anchor/eight-constant verifier, and
[support/slopes/upstairs contract](../behavior/overworld/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md)
with13-body/132-anchor/24-span verifier after the back-face normal and
partial-angle-write addendum. Both inspect US ISO read-only.

marioGetMoveRate is camera-relative directional gain1+.190000057*abs(cos),
following air cap; it does not read floor slope. MarioPreCamera publishes offset
from camera4 yaw before input. Native camera0/unit-vector rate remains an explicit
slice/numerical adaptation, not proof of original Rogueport yaw.

Recovered separate previous-angle vector reduction, query-owned slope angles,
feet-before-front sequencing, metadata/root rounding separation and deferred
upstairs target/entry. Recognized>=2engine-unit lower floor can select fall11
(drift suppressed) while all-ray miss selects10. Upstairs first two rises retain
travel; third dispatches current input/newA and resumes ground queries. Vertical
velocity remains retained after completion unless another owner clears it.

### Native support binding (2026-10-12)

Commit78eb86b binds ordinary five-foot support before front-step admission,
separate root/support metadata, last-query slope writes including steep-reject
partial writes, previous-angle axis reduction before air cap/direction gain,
recognized-lower-floor11 drift suppression and all-miss10 drift, and deferred
three-rise upstairs with retained first-two travel/current third-call input/A.
Upstairs retains its support node and vertical velocity; idle clears that
velocity. The undocumented center-rise XZ veto was removed. Back-facing ray
normals now orient toward their origin, including the downward floor service.
Host math/intersection/traversal remain adaptations; this is not full physics
or source-runtime equivalence.

Requested the documentation agent's next missing ordinary-air contract:
mot_jump/mot_fall phases and retained signed counter, pending10/11 entry,
motion3 descent, marioJump/marioFall/marioChkLandon query composition, and
landing tuple/angle/node ownership. Delivered
[ordinary airborne/landing contract](../behavior/overworld/OVERWORLD_AIRBORNE_AND_LANDING.md)
in35f82d8 with a4df746/ce29235 addenda. Native97fd570 and db71cca bind separate
motion/flags/pending/phase/countdown/counters, retained fall entry, apex versus
ceiling sequencing, authoritative D=V/V=V landing, query-owned slope writes,
late ground dispatch and post-move base damping. The old beginFall counter2
reset and invented landing ray bounds are removed. On-screen/bypass/key-control
inputs remain explicit native adapters; original projection/control writers
are not bound. LandOn clears drift mask0x80 and retains damping bypass0x20.

Native31f6cb8 binds ce29235/9869179/f305f1d's ordinary head contract: raw-normal
projected tilt writes without overall/reset writes, cone rejection, first
admitted ray contact or near-miss return, actual-V node gate and separate contact
flag/clearance/node outputs. Low-ceiling requests ground motion before response
admission, preserving root/tuple/flags/counter on that path. Next manager dispatch
owns ground entry; walk/dash retain tuple/counter and the front-step counter gate.
The head X copy is an identity in this domain; the leaf only writes local Y.

The session's next native binding, the Rogueport roads contract, is recorded
under [OW-ROAD-01](OW-ROAD-01.md).
