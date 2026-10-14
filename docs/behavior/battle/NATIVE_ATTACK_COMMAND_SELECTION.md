# Native original attack-command event selection

The shared packed selector now implements the original _btlcmd_SetAttackEvent
80120268/5b4, weapon classifier8011f29c/1ec and borrowed unit-data lookup
8012733c/40. It handles all14 command modes, all9 subcommands and out-of-range
words, including override, selected-weapon fallback/classification, charge,
party/position, audience and lecture-end selection. Publication never runs a
script or fabricates damage/turn completion. The service-success bool is a port
binding result, not an original scalar return.

The native trio's confirmed-command launch now calls the selector before root
creation. Its previous direct event/category stores omitted original selection
records. Default Jump therefore publishes80355cf0/category0 and increments
record16f38 before executing a source instruction. Original physical menu
navigation, target confirmation, cancellation and optional touch shortcuts are
unchanged. The original tutorial menu/command controller remains unfinished;
this shared selector is a required dependency, not a completed tutorial UI.

The captured battle owns counts even if cursor/audience services change global
ownership. Selected null event falls back to Wait without replacing the weapon
used for classification. Exact pointer8036aa84 skips the item count. Charge
branches retain the kind choice through event publication; other kind splits
use222 versus every other kind. AddCount leaves100..255 untouched without a
write. Audience normalization follows signed byte rules, then resolves source
unit data/present-item choice with fresh global capture. T11 resumes a message
only after its count when the captured battle's current flag permits; T12 does
neither. Message continuation remains an explicitly required external service.
Native trio exposes only its currently supported confirmed weapon modes, which
do not call that audience continuation branch.

Host fixtures cover every mode/subcommand, override store order, stale byte
preservation, null-event and absent-data distinctions, charge kind mutation,
captured count ownership versus audience recapture, counter99/100/255, exact
special-pointer exemption, non222 splits, signed audience normalization and
message admission after count. Classifier checks include null, HP-pointer
short circuit, signed parameter gates and negative-strength rows. Native Jump
checks its published script/count before source event execution.

Static source validation uses existing tools/verify_battle_attack_selection.py:
11 complete/linked bodies,301 instruction anchors,8 data spans and7 scoped
DOL call sites in the read-only original US ISO. No Dolphin observation or
native menu/effect/turn completeness is claimed. See
[original selector contract](BATTLE_ATTACK_EVENT_SELECTION.md),
[classifier/data/audience leaves](BATTLE_WEAPON_ATTACK_CLASSIFICATION.md), and
[ordinary Jump root registration](NORMAL_JUMP_ROOT_REGISTRATION.md).

Next: full original table/menu input and tutorial player action admission;
source state6 registration and action/turn ownership; connect original visual,
audio and audience service consumers. Keep physical controls for every action.
