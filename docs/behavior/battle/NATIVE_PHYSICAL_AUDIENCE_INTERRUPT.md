# Native physical-X audience interruption

`battle_menu_audience_native` retains the complete original
`BattleCommandAttackAudienceCheck` (`8011f524/cc`) while replacing only its private
stack index output with a disjoint native scalar. It implements the actual
`GetItemOn(&index,0,0,0,0)` signature; it allocates no synthetic guest stack.

The original battle global is captured before trigger `0x400`. Battle flag
`0x40000000` and Mario flag `0x10` retain their original ordering. The query scans
200 original members, honors active/hidden/held-item/state gates, and writes the
original cache index and position. Independently refreshed globals and float-Z
addition remain in source order. The private result survives cache mutation.
There is no harmful-item filter: original physical X selects the first eligible
member, including a good item. Optional selected harmful-audience tapping is a
separate adaptation, described in [selected audience targeting](NATIVE_SELECTED_AUDIENCE_INTERRUPTION.md).

The actual existing SetTarget helper runs before retained ActMenuEnd. No query
or target writes replay while End waits. After End, the fresh previous type,
type11, state29 and battle flag4 are published in source order. Original
`BattleStageOffLightInCommand` is a complete return-only leaf; acknowledging that
leaf does not acknowledge a renderer operation.

Services pin all callback, memory and audience-lifecycle identities. Recursive
resume/reset is rejected. The actual allocation predicate is checked at entry,
continuation boundaries and after external children; losing it poisons the
invocation until explicit reset. The caller must freeze its actual mapped VM
allocation through synchronous memory/query/SetTarget primitives. A predicate
which accepts reused raw addresses is insufficient. Trio creates its token once
after initialization and retires it at runtime destruction; its predicate also
checks the actual mapped battle extent and live global. This is allocation
ownership, never camera, audience visibility or UI readiness.

Trio's retained class menu, weapon menu and command-input parents share this
actual backend and actual retained End child. Normal battle sequencing is still
partly the legacy harness. These bindings do not activate the original whole
battle Main, establish the selected-tap camera/frame publication, or prove native
original-menu rendering. No original story text changes.

The focused test compares every observable guest read/write with the existing
source checker and GetItemOn/SetTarget graph, excluding only disjoint private
output storage. It covers first/last/no holder, item states, member flags,
physical masks, blockers, a good first item, SetTarget cancellation, changing
globals/cache, held children, service mismatch, reentry and poisoned allocation.
`tools/verify_menu_interruptions.py` pins the read-only ISO's 20 whole bodies,
243 instruction anchors and three data spans. These are recovered source rules,
not Dolphin timing measurements or hardware/input/UI parity evidence.
