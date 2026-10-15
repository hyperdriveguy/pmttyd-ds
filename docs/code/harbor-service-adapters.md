# Harbor tutorial service adapters

These native adapters connect the tutorial's packed guest work to recovered
battle helpers. They are a limited composition layer, not evidence that every
battle branch has a complete native provider. See [commands](battle-commands.md),
[weapon menus](battle-weapon-menus.md), [damage order](battle-damage-order.md),
[reports/resources](battle-reports-resources.md) and
[battle events](battle-events-rules.md) for the underlying routines.

## Address and result domains

[Damage](../../source/battle/harbor_tutorial_damage.cpp) uses separate native
`Units` and `Parts` guest regions, with `0x1000` bytes per registered slot.
Slots 1 and 2 are Mario and Crump. These addresses are 32-bit memory tokens,
never host pointers or original GameCube allocation addresses. The packed
layout offsets remain original: a native stride does not redefine those fields.
Global cells `0x8041eb90` and `0x8041eb00` hold the source BattleWork and pouch
pointers. Some callbacks reload those cells rather than using the adapter's
fixed `Battle` token; preserve that distinction.

An absent optional denotes native unavailability. An engaged zero or false can
be a completed source result. Ordinary `bool` admission returns have their own
contract; do not turn a rejected provider into a successful miss or empty menu.
Retained cursors survive visits even though stack service tables and contexts
are rebuilt. A captured stack context is valid only during that invocation.

## Damage snapshot and admission

`readDamage` decodes packed unit, kind and part tables into fixed typed arrays.
Signed halfwords use the 32768 sign boundary and 65536 modulus. Thirty status
bytes, badge/bonus counts and five elemental defenses keep their original
numeric domains. Four sound strings are copied into owned storage; the 128-byte
copy limit is native policy, not a measured maximum original key length.
`writeDamage` publishes only the supported mutable fields, including HP/FP,
request/report values and selected reaction words. A whole-structure writeback
would overwrite fields owned by other source routines.

`checkDamageCore` admits only the Mario/Crump ordinary-hit paths with the known
default, jump or hammer power callback. It rejects non-null FP callbacks,
unsupported element/mode/guard domains, nonzero weapon status payload, active
unit statuses/badge bonuses and a counter reported in work word 9. These gates
express missing native services; they are not additional original game rules.
It also rejects a base hit that reaches or exceeds defender HP because lethal
inventory/revival/event handling needs separate admission. No clamp makes that
hit appear nonlethal.

After admission, the source calculators use real packed power words and pouch
levels. Effects, sounds and notices are collected as presentation requests;
collection alone does not prove playback/rendering. A hit-event callback writes
the current defender damage state before starting source hit work. Direct
application temporarily selects the defender as sound owner, restores the prior
owner afterward and publishes the attacker's dealt-HP report. The synchronous
calculation context cannot be retained by a provider past the call.

## Aftereffect and AP report gates

[Aftereffect](../../source/battle/harbor_tutorial_aftereffect.cpp) first runs the
recovered helper against a copy of the RNG. Its admitted linked requests only
inspect complete non-mutating exits: null table, signed pouch resource gate,
already-falling flags or tutorial suppression. If the preview needs an
unsupported event-producing path, it returns unavailable before consuming live
random draws. Once admitted, the helper runs again using the real RNG. This is
an admission preview of the same draw sequence, not a search for a favorable
hazard result. Adding a mutating provider would require reconsidering this
preview policy.

[AP report](../../source/battle/harbor_tutorial_ap_report.cpp) is admitted only
when the original chance byte at `Battle+0x18ffc+0xa` is zero; the full bingo
producer remains unbound. Source RNG consumption within the admitted report
still occurs. The adapter forwards conversion, FMA, addition, multiplication
and truncation through VM platform operations, with explicit square-root and
single-rounding callbacks. Reassociating those operations can change results.
Break-slot work is read through the current global BattleWork pointer at
`B+0x1616c`, with turn word at `+0x10`.

## Audience, item and weapon-menu ownership

[Audience selection](../../source/battle/harbor_tutorial_audience.cpp) uses
`MenuRam+0x14` as private item output and the source pad trigger at `B+0xf44`.
It resumes its audience cursor rather than replaying a completed item step.
Closing releases actual class UI first; only after acknowledgement does it clear
the act-menu flag and scene menu booleans. The empty light callback corresponds
to an original `blr` leaf, not a general fallback for unimplemented services.

[Present item](../../source/battle/harbor_tutorial_present_item.cpp) keeps XYZ
scratch at `MenuRam+0x20`. Its cursor retains acknowledged recovery and pending
math/effect progress. Double subtraction followed by single rounding has a
different rounding boundary from subtracting two pre-rounded floats. Missing
math stays unavailable. The recovery effect callback is deliberately absent in
this adapter, so no fake completion clears the item after recovery.

[Weapon input](../../source/battle/harbor_tutorial_menu_input.cpp) accepts class
codes 0 jump, 1 hammer, 2 item and 4 special. Setup stores the source class
aliases but supplies native UI storage at `MenuRam+0xc00`. Its presented byte
at `+0xc14` starts zero and changes only through explicit presentation
acknowledgement. Separate private cursor words at `MenuRam+0xc` and `+0x10`
serve setup/input. Input distinguishes triggers (`B+0xf44`) from repeats
(`B+0xfbc`); source `B+0x1c78` owns the current UI pointer. Release must succeed
before clearing that pointer. Target setup only opens the scene's target view;
the actual weapon decision remains in `selectWeapon`. Row projections refresh
on an engaged source result, including zero.

These notes cover the six adapters linked above. See [composition](harbor-composition.md)
and [packed state](harbor-packed-state.md) for the tutorial owner and other
Harbor adapters. Concurrent retained-action changes and Trio integration still
need their own audit against committed code.
