# Retained story-to-native party callbacks

The original US Professor lecture uses native party run/stop, clear/kill and
previous-position callbacks. `NativeFieldPartyScriptCall` binds those callbacks
without copying an event into the animation VM. It also admits the original
explicit position and pouch-join leaves for subsequent story integrations.

## Ownership and ordering

The caller retains one cursor per active USER_FUNC invocation, actual EventEntry
and ID, source VM, function and original first-call value. Admission can wait for
field bindings or the source entry address without losing first=true. Once field
bindings are admitted, registry, Mario, current-index, pouch, root and native
animation/pool identities remain pinned. A missing join pouch is admitted only
at the reached join child and then pinned; a late provider does not replay
operand resolution. Disposal never dereferences providers;
it is not cancellation, successful teardown or permission to destroy an active
source entry. Callers must keep entry/VM/owners stationary until completion.

Position callbacks delegate to [the retained position event](NATIVE_PARTY_POSITION_EVENTS.md)
using the actual caller's memory and operand services. Construction waits do not
publish Mario245/246 or advance the script. Display services may become available
at their reached use. An initially first=false position call retains its source
no-op semantics; later first=false retries of a first=true admission do not.

Clear/kill uses the generic original event-removal cursor with caller-VM E18/raw
selector reads and the actual Mario245/246 byte facade. Its opaque Kill/Kill2
child uses `NativeFieldPartyLifetime::kill`, whose model/pose/file releases use
the animation owner's VM. Calling the lifetime's same-VM `.event()` with a
foreign story entry would read the wrong address space. Primary clear and kill both preserve saved kind; auxiliary clear uses Kill2
and clears its saved kind while auxiliary kill preserves it. Projection of Goombella's NPC or removal of the
Professor projection follows completed native work, including resource waits.

Join resolves operand0 through actual evtGetValue once, then ORs bit0 into the
canonical shared pouch's unsigned halfword at14*kind. Kind>=8 is a genuine no-op;
negative kinds retain an explicit unbound-domain boundary. No HP, actor, selected
index, saved kind or mode changes are inferred from a join.

Run/stop read RAW selector0, never evtGetValue: any nonzero means literal table1;
zero reads the independently published current-index word8041ead0. This is not
Mario245 or a newly calculated registry current(). Missing current publication
holds; signed negative returns null; indices>=2 remain an explicit unbound table
domain. Run changes mode only when the actor's byte34 is zero. Stop always enters
the mode wrapper. Fresh auxiliary mode2 therefore remains2 after run.

The mode wrapper saves the first old mode to35 except9/3, then reads old mode
again for inner dispatch. Final low-byte mode and fresh word8 OR8 publish only
after the child finishes. Old1/2 executes the exact SlitOff no-work gate: read
word0 bit1000 first, otherwise word4 bit100. Both clear is a genuine completed
leaf. An active branch stays held at8014c448; clearing flags later cannot turn
that already-reached branch into a no-op. Old3/4/11 remain held at800bcb98; old12
clears word4 bit100000. A weak registry entry lease is acquired before capturing the table pointer and
excludes native constructors/removals until mode work completes. Waiting for the
lease captures no actor pointer; allocation-address reuse cannot substitute a
new incarnation beneath a held capture. Identity is checked before dereference
on retries.

Entry retirement removes the invocation from the active ID map. Pure leaf/mode
prefixes have no admitted opaque child and release their lease without rolling
back prior source writes. A held constructor/resource release is quarantined,
retaining its backing and exposing its pending boundary; native cancellation is
unbound and the scene stops instead of replaying it under a reused event ID.
This is an explicit limitation, not successful source completion. Game restore
and checkpoint/warp handoff also defer while any party invocation remains active,
including admission waits before registry leases, or native construction/removal/
mode ownership is held.

## Production scope and evidence

Game supplies these bindings only for an actual native animation session, using
its shared field registry, Mario, current-index publication, canonical story
pouch, accepted asset root, Goombella bank and appearance services. The Professor
runtime binds existing original callback tags27/29/36/58/62. A held invocation
cannot become a presentation-only success if its binding disappears. Standalone
legacy scene harnesses retain their declared partial projections for the older
run/stop/clear/kill callbacks; previous-position remains a real boundary without
an owner. No dialogue, message IDs, branches or story conditions are changed.

Kind11 Professor construction, movement, direction and projected position queries
remain separate services. This bridge does not complete those services, native
battle teardown, sequence return or full prologue fidelity. Original battle
start/wait gates remain intact.

Fresh read-only original DOL evidence is saved under the ignored
`artifacts/field-party-story-evidence/`: complete evt_pouch_party_join800f0a8c,
partyJoin80140f74, pouchGetPtr800d5eb0, evt_party_run800eae70,
evt_party_stop800eaeb8, partyRun800becdc, partyStop800becb8 and
L_partyForceSlitOff8014c448 listings with hashes. See also
[callback contracts](FIELD_PARTY_EVENT_CALLBACKS.md) and
[mode initialization](FIELD_PARTY_MODE_INITIALIZATION.md).

Tests deliberately map different operands at identical guest addresses in two
VMs, retain native file waits and first-call admission, exercise canonical pouch
and raw current-index semantics, and run the original lecture through its native
kill boundary. These are local service checks, not evidence of a complete native
battle-to-field return, hardware timing or original GPU/FPSCR parity.
