# Normal native shared smart/shadow bootstrap

Normal `Game::initializeNativeUiFieldAnimation` now runs original smart and
shadow initialization after UI/icons and effect initialization, before Mario
entries. `NativeShadowProcess` owns both source continuations in the existing
animation VM and retains that incarnation through room/checkpoint handoff.

The animation process transfers the complete 80-byte heap-global segment at
8041e808 to stable process-owned backing before original heap bootstrap.
`SourceSmartArena` borrows the existing first-allocation word at8041e810 with
physical identity checks; binding never resets its history. No subrange alias
splits the original globals. The native arena remains the existing12MiB policy;
its original fifth heap configuration100 means percent of remaining capacity.
These bounds are native policy, not measured original hardware behavior.

Original smart Init uses the actual heap4 SDK allocation. Original shadow Init
uses real heap0 allocation, fills, ramp loops and complete GX texture-object/LOD
children. Existing initialized SDK literals are reused only when byte-identical;
missing fragments are installed with explicit ownership and conditional teardown.
No live UI data or edited asset is overwritten. No new source asset is imported.

A held numeric child or instruction budget retains its source PC and completed
allocation prefix. Changed heap-global backing or lost SDK allocated-node identity
invalidates the owner. A second cold owner cannot reinitialize the same manager.
The mutable first-allocation word and projection gate remain source RAM; completed
bootstrap calls do not repair them or replace them with permanent C++ flags.

Platform cache hooks here provide CPU VM ordering only. GPU shadow residency,
actual draw synchronization, moving-file collection and full outer-display reset
scheduling remain unbound. Pressure/file/thread children remain held. This
activation establishes source initialization, not field shadow rendering or full
prologue acceptance. Camera initialization and map publication are next.

See [source shadow initialization](NATIVE_SOURCE_SHADOW_PROCESS_INITIALIZATION.md)
and [source smart arena](../memory/NATIVE_SOURCE_SMART_ARENA.md).
