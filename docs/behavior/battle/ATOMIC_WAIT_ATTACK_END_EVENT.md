# Original attack-event barrier

`battleWaitAttackEndEvent` implements complete US `btlevtcmd_WaitAttackEnd`
8010a154/74. The incoming EVT pointer is unused. Each invocation captures the
actual battle global, reads its signed attacker ID at +420 and uses the existing
registry helper. Null attacker or zero unit attack-event ID (+2a8) returns 2.
A nonzero ID is checked by the existing complete `checkEventId`, selecting the
real event manager through its GP battle-context flag. An active entry with that
ID returns 0 without writes. A dead ID clears the captured unit's +2a8 and
returns 2. Mutation of the global battle during the check cannot redirect this
store. A later poll starts with a fresh battle/attacker capture.

This atomic leaf requires admitted synchronous total memory and event-manager
services, as the existing actor/event helpers do. Missing services are not
interpreted as dead events. Return 0 means a genuinely executed live-event
poll; it is not an unavailable-provider response. There is no retained timeout,
victim-HP shortcut, synthetic event finish or inferred battle result.

The native journal delegates to `vm.creationServices()`, preserving actual
field/battle context selection and the original flags&1 activity test. No guest
stack is fabricated. No presentation readiness or resource lifetime changes.

Validation checks exact read/store order, closed branches without unused
providers, signed invalid IDs, manager selection, activity-bit semantics and
fresh-vs-captured ownership under callback mutation. The separate coherent
15584 lethal fixture includes the previously frozen death/StarPoint/rotation
and return-item/part-attribute leaves; its GPU admission remains TESTONLY and
provides no native rendering or whole original ending receipt.

The read-only source verifier pins complete parent, registry and evtCheckID
bodies. Parent integration must run fresh full `make test` and `make all` with
matching headers; these focused tests do not replace those gates.
