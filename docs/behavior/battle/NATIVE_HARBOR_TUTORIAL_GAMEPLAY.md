# Native manual Harbor tutorial gameplay driver

`NativeHarborGameplay` is an explicit port scheduling policy for the existing
manual setup2 `HarborTutorial` scene. It connects the original tutorial event
scripts, physical command input, selected attack, action-command sampling,
damage/reaction roots and phase dialogue to normal Game simulation visits.
It does not represent whole original `btlseqMain`, original source startup or End
completion. The source-owned process integration remains a separate owner.

Previously normal Game called only `HarborTutorial::tick()`. That executes init
and the opening script but never invokes command Input, selected player attack or
installed enemy attack. The original mechanics already existed as explicit APIs,
so completing unrelated source OS or display helpers is not required to call them
from this bounded native scheduling policy.

## Scheduling and identity

One visit is admitted per increasing simulation tick with nondecreasing native
milliseconds. The driver pins the actual runtime returned by
`nativeGameplayLifetime()` and rejects a different facade/runtime. `reset()` drops
only native scheduling metadata and that pin when not invoking. It does not call
source End, acknowledge a held child or reset process resources. The exact Game
journal resets policy at actual successful restore/new-scene boundaries. Game
copy/move support is retained; a copied driver does not authorize another facade
to drive the same runtime.

After actual opening completion, the bounded source setup2 phase tags must be
Mario04000002 and Crump04000004. Original channel4 ordering supplies tutorial
feedback04000003 and closing04000005. The driver dispatches the real Crump phase
hook at these boundaries and waits for actual phase/event/damage retirement.
The hook itself reads original turn/work/success/failure/HP conditions and prints
the exact original message IDs; the driver adds no dialogue or conditions.
Subsequent turn preparation reuses genuine `battleTurnFirstProcess` and its real
packed rule, move, initiative-RNG, fortune and fog providers. The actual source
signed halfword is read back, preserving its16-bit wrap rather than inventing a
native turn counter. This does not claim the rest of original Turn status passes
or every original scheduler invocation/read order.

`openClassMenu()` starts one actual native command context. Every subsequent Input
visit uses the supplied physical sample. Only actual Input completion1 can admit
`dispatchSelectedAttack()`. The driver never writes class/weapon presentation
readiness; the real native menu renderer must publish its own completed frame.
It never selects a weapon/target directly, synthesizes a timing/guard press,
converts touch/dialogue edges to physical input or completes an unknown service.
A dialogue active before or after the tick blocks menu advancement, so its
closing A edge cannot also confirm a command in that visit.
Original action timing remains in the simulation owner. Physical controls and the
existing lower scene continuation remain available.

After actual player roots finish, the original feedback phase runs, followed by
the actual installed Crump attack and its live defense sampling. Genuine
unavailable source callbacks keep those roots held. The native driver cannot
advance around such waits.

## Conclusion and teardown boundary

At idle attack boundaries, `nativeGameplayConclusion()` invokes actual
`battleCheckConcluded` and `battleInitialEndResult` with real packed actor/status
and enemy-alliance providers. A selected numeric result moves the driver to
Conclusion and prevents extra attacks. It does not write `scene.result`, invent a
victory, acknowledge End, export a pouch, delete units or release the blocked
field event. Those operations require the separate actual completion adapter.

The existing manual facade still owns its provisional native actor registry,
42-degree camera/presentation and previously declared service projections. This
change neither upgrades those to source receipts nor requires their replacement
before the existing original scripts can respond to physical input. Source voice
binding proves actual public pending-slot ownership, not audible NDSP playback.
The separate native menu policy must draw original imported art and may notify
manual presentation readiness only after a genuine complete frame; it must not
forge an original queued display completion.

## Evidence

`verify_native_harbor_gameplay.py` pins whole original Phase9c/Move690 bodies,
Mario/Crump phase tags and the complete1128-byte Crump phase EVT from the read-only
US revision-zero ISO. This is source evidence, not original execution.

The focused test runs original imported EVT, command/action/damage mechanics and
real public sound pending-slot allocation. Its explicit TEST physical sequence
selects through the menu and misses the action timing; actual source Jump changes
Crump HP5→4 and actual Crump attack changes Mario HP7→6, reaching turn2 with no
pending child. Animation acknowledgements follow evaluation of actual imported
original clip endpoints. TEST menu presentation acknowledgements are labeled
fixtures, not native framebuffer evidence. An unpresented menu stays blocked for
200 simulation visits; duplicate ticks and replaced runtimes reject, and explicit
policy reset admits a new actual runtime. Native GPU/audio and physical tutorial
round-trip validation remain required after integration.

The successful timed-input route now binds the genuine GetResultPrizeLv wrapper
to actual VM operands/actor lookup and existing source `actionPrizeLevel`. It
preserves captured-work versus fresh unit lookup, low-code30's early return and
actual unknown-byte holds. A direct registered-entry test proves the early route
does not read an unknown grade byte or manufacture a default. Successful Jump
then reaches ACSuccessEffect80106fa0 after its first actual hit when no visual
consumer is bound. The integrated [original-bank Nice owner](../display/NATIVE_HARBOR_NICE_EFFECTS.md)
and [physical stylish policy](NATIVE_HARBOR_STYLISH_WINDOW.md) now admit their
actual supported children; an actual successful source round reaches turn2.
The [bounded completion contribution](NATIVE_HARBOR_COMPLETION_POLICY.md) also
binds original fatal rotation. The next observed lethal hold is the real
`evt_eff` request for `kemuri_test` type4, before conclusion. Native framebuffer,
audible playback, complete fatal effects and normal field return still require
integration evidence; no result or story advance is inferred from HP0.
