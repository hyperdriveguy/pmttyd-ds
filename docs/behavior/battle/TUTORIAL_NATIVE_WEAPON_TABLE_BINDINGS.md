# Tutorial native weapon-table providers and field-pouch ownership

The tutorial runtime now binds the shared source builder, eligibility updater
and private target-count helper for Mario domains0/1/2/4. It uses actual packed
unit/part/weapon data, pouch/equipped-badge queries, source icon leaves,
original US msg/US labels, FP/AP cost checks and the complete packed sampler.
Labels are copied unchanged into mapped native message RAM and cached by their
original key pointer; no generated command text or fallback weapon is supplied.

A separate private RAM region contains builder/updater cursor words, the count
word, private target buffer and label storage. The count sampler never writes
the live Battle+428 selection buffer. Presentation rows use the source cursor's
published count, including preserved cursor state when builder return is0.
Row weapon, name address, icon, enabled/reason, ordinal/item and text are exposed
for the future original menu controller. Building a table neither confirms a
command nor creates an attack, advances a turn or changes the story.

The public buildWeaponMenu bridge requires completed intro and no phase/attack/
reaction root. Party domain6 stays explicitly unsupported in this tutorial:
helper3 is the source NPC role, not a fabricated playable Goombella. Full class/
submenu/input/decision and player action ownership remain unbound. The Game
still displays its development action-service notice after the intro.

The binding exposed a real setup defect: beginFresh used bare pouchInit, whose
Jump level0 legitimately emits no ordinary row. The Game now enters through
beginFromPouch when original field storage exists. It copies all entry bytes
without rerunning new-game grants or clearing inventory/equipment. The existing
original unit-resource import/revision supplies Mario HP/FP/base/max fields
before his original init event. Legacy saves without storage retain the old
explicit bare-pouch harness; complete inventory cannot be inferred from HP.
Later tutorial battle export/teardown and general equipment import remain open.

Fresh source field-pouch tests obtain enabled Normal Jump803538a8/icon167 and
original Hammer80353de8, a disabled source empty-item row80363e60/reason3, and
no Special rows before unlock. They check HP7/FP2, equipment levels and entry
pouch bytes, unchanged resources/result/attack roots, and independence from
later mutations of the caller's field storage. Name lookup retains source text
and control tokens. Cost queries use live unit FP/current pouch AP; out-of-range
FP-cost numeric conversion retains an unsupported binding rather than publishing
an affordability result. Native label storage has an explicit16KiB-region budget;
this is port allocation policy, not measured original capacity.

Validation uses host execution of actual source data; no original Dolphin or
native menu rendering/playability is inferred. A relocated map-visibility test
include was corrected after subsystem reorganization. See
[shared table production](NATIVE_WEAPON_TABLE_PRODUCTION.md),
[source table layout](WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md),
[cost contract](BATTLE_WEAPON_COST_AND_PAYMENT.md), and
[source registry](HARBOR_NATIVE_REGISTRY_AND_VISUAL_PROJECTION.md).
Next: original class/submenu input and decision, actual selected-target/action
registration, timing and player impact; preserve original visual style and
physical controls alongside optional lower-screen audience taps.
