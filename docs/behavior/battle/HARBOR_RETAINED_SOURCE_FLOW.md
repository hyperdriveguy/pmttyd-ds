# Retained Harbor startup and battle controller flow

This package closes source controller ownership for the normal setup-2 tutorial.
It does not turn the earlier manual intro/attack APIs into an original battle
scheduler. Actual cold BattleInit, UnitSetup/UnitEntry, field/process ownership,
physical input, presentation and result/field return remain separate providers.

## Startup source parents

`resumePackedBattleSequenceInit` implements whole btlseqInit `8011ee40/198`.
It captures one invocation's channel-1 dispatch and retains the source
same-call state0→1 path. Field Mario's flag write, party query and party flag
write happen before the retained whole UnitSetup request. UnitSetup cannot
repeat those completed prefixes when one of its children waits. Init-event
readiness, three ordered paper probes, Nice and the two status calls retain
original branching and return points. Source resource return0 completes that
invocation without advancement; native unavailability is distinct nullopt.
State4 yields after writing5; state5 owns the actual FirstAct handoff.

Important correction to older prose: MarioGetPartyId at `8011eec4` returns r3,
which PartyGetPtr at `8011eec8` consumes without an intervening assignment.
The complete `800bf1b4/20` getter compares that signed index and indexes its
source pointer table. This is not an ignored return or a readiness predicate.
The retained request captures it in `integers[0]`. The older synchronous helper
already passed it correctly. An unapplied exact documentation correction is
recorded with the validation artifacts.

`resumePackedBattleFirstAct` implements whole `8011e5c0/7f8`, all15 states and
the complete nonzero first-strike entry. It captures first-attack mode before
sequence dispatch, retains the camera/64-unit entry scan, source NPC exclusions,
field-motion26/data65 selection and event allocation/owner/ID stores. Event
addresses are actual mapped source entries supplied by the VM constructor.
It never substitutes registry IDs for pointers or fabricates guest stack cells.

The first-strike helper's two initially unknown output words are private
cursor-owned optionals. Only that original child's request exposes their stable
addresses; it may retain partial writes through its own wait and must stop
borrowing them when it completes. Unknown outputs hold at their first source
use without recalling the helper. Distinct output cells remain distinct. All
nonzero-mode damage/weapon/status/audience/attack/companion branches and exact
source flag/event fields are retained, including signed mode comparisons.

The remaining states own the decrement-before-expiry timer, captured fade
color/type23/raw200, source curtain sound/voice sentinel, independent repeated
fade/event probes, conclusion, status-message/announce order, reserve items,
audience/slot reactions, wall event, luck scan/root/counter, nozzle and actual
Turn handoff. Native partial waits preserve successful operations. A completed
original poll0 returns from this invocation; a later simulation invocation is
required to poll again. No rendering frame count advances these controllers.

Services pin actor callbacks, audience lifecycle identity, strong lifetime,
provider user/current/call and passed battle. Reentry/reset from a provider is
refused. Invalid currentness latches unavailable. Original void completion is
represented by an engaged ignored scalar; source pointers/results retain their
original domains, including genuine0. Missing children never succeed by default.

## Concrete shared-VM flow

`HarborSourceFlow` composes the retained Main, sequence dispatch, Init, FirstAct,
Pinch, Turn, Phase/Move and Act owners. Its VM adapters supply actual status,
recovery, unit data/registry, turn-first processing, phase tables/move state,
source RNG, real event construction/deletion/existence, conclusion and retained
barrier/reserve children. Source-born phase and attack event ownership stays in
the source controllers. The shared Move→Turn child bridge retains its converted
request address and all operands through waits. It does not try an alternative
provider after an unavailable child.

The original reached Input `801229d8` delegates to the persistent command UI
owner. It is not manual openClassMenu, weaponPresented or dispatchSelectedAttack.
Original command initialization, source Act progression and selected attack event
registration remain in the actual controller graph. End delegates to its actual
result/teardown owner and cannot be inferred from HP or an empty visual roster.

The caller supplies actual Main leaves for broken units, pad, action command,
stage objects, icon, stage, action help, audience, break slot and display. The
flow supplies Sequence internally in its correct position. The eleven original
Main requests remain ordered and retained. No other Main leaf is acknowledged
because a native UI atlas is resident or a debug fixture appears empty.

BattleMain contains **no EVT manager**. `80203aa0` is BattleBreakSlot_Main.
This module samples no input, advances no timestamps, runs no event manager and
invents no simulation cadence. The real outer process must execute its actual
EVT manager once at the admitted scheduling point and invoke Main once per
admitted simulation pass. A held Main resumes its captured child/sample rather
than starting a generic retry. Caller must also prevent the legacy scene
projection from overwriting source sequence/state fields across a held call.

The VM is borrowed. Actual allocation/process lifetime and a current predicate
must protect every call; callbacks execute serially and cannot free VM storage
inside an admitted operation. Use independent ownership tokens/weak runtime
callbacks when the runtime owns this flow, to avoid a runtime→flow→runtime cycle.
Short camera availability is not long allocation currentness. The flow performs
no guest End/cleanup on retirement and no source bootstrap writes on creation.
Total VM reads require admitted mapped original memory; missing source tables
must be provided before entering that finite domain, never read as zero.

## Evidence and practical frontier

The verifier pins both complete parents, their complete direct-call census, the
party getter and50 linked original sequence/startup/helper spans from the
read-only US revision-0 ISO. The Init tests compare eight state cases ×16
policies against the existing source reference. FirstAct tests compare all15
states ×11 branch sets and exercise nonzero modes -1,1,3,4,6,7,8,9,10. Every
reached child is held twice, with reentry, stable-request and lifetime checks.
The flow test uses actual EventNativeRuntime memory and real retained parent
composition; its external Main/startup leaves are explicitly test providers.
It proves controller/retention order, not native battle presentation or entry.

The previous fresh native adapter's first observed permanent wait was actual
Crump WaitAnimationEnd `80106980`, event command `95544a58`, owner2, after the
pose request at `95544a44`. This trace supplied no synthetic animation completion.
The previous tick path also never ran original Main after its manual intro.
Those are independent presentation and scheduling gaps. Successful Jump's voice
`80104508` and the previous explicit lethal-damage refusal require their real
owners too. Neither this source graph nor focused tests claim the initial battle
is playable until those actual providers, bootstrap and normal entry are bound
and verified together.
