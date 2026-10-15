# Trio Turn, Phase, Move and Act bindings

The [Trio runtime](../../source/battle/trio_attack.cpp) binds reusable retained
controllers to actual VM memory, scheduler, packed actors and platform math.
This guide covers `turnServices`, `originalActServices`, `originalMoveServices`,
their resume wrappers and continuation metadata. See [menu bindings](trio-menu-bindings.md)
for their Input/UI children and [scheduling](trio-scheduling.md) for the separate
legacy path. Newer pinch/sequence/pose/public draw integrations remain outside
this partial runtime review.

## Captured requests and output domains

Turn, Phase/Move and Act each retain one original invocation. Their completion
allows the source parent to decide its next transition; it does not mean a turn,
attack, battle or render delay finished. The simulation owner supplies cadence.
A missing child keeps its exact operands and earlier effects. Engaged zero is a
completed result, including a barrier that reports not-ready and Input returning
zero. Distinct callers have separate barrier/consumption cursors.

Turn returns typed lanes: `value` for a source scalar, handle or guest address;
`position` for native XYZ; `duration` and `strength` for status byte outputs.
They are not a generic guest stack or interchangeable readiness flags. Act uses
an enum child plus original function/site metadata. Unsupported children stay
pending; an empty native visual scene cannot acknowledge them.

Move reuses shared Turn children through an owned `BattleTurnRequest`. It pins
the original Move request pointer, function and all eight integer operands until
an engaged result. Any mismatch refuses the held call without adopting a new
request. Only then is the original pointer released and the output scalar passed
back. This bridge prevents a temporary stack object's changing identity from
breaking a deeper retained child. It converts no arbitrary float ABI.

## Original function identities and packed fields

Addresses below are US original dispatch identities, not host function pointers.
Unit/B offsets and masks use hexadecimal; kinds/counts are decimal.

| Identity | Bound work |
| --- | --- |
| `0x8011b1ec` | Actual VM TurnFirst backend |
| `0x8011a06c` | Current global B (`0x8041eb90`), channel-zero trigger word B+`0xf44`, masked raw result |
| `0x80127814`, `0x80127f34` | Packed CheckStatus and GetStatus byte outputs |
| `0x80127890`, `0x801266d4` | Recovery-status query and move-status reset |
| `0x80126bec`, `0x80126b58` | Packed HP and FP recovery with original ownership rules |
| `0x80125d08` | ShadowGuard: U+`0x104` mask `0x10`, otherwise exact kind 228 and signed U+`0x228` >0 |
| `0x80128c7c` | Unit XYZ at U+`0x3c`, three four-byte float words |
| `0x80128414` | Signed height halfword U+`0xce`, sign-extended into raw result word |
| `0x80128c98`, `0x80128cd8`, `0x80128b10` | Body-part ID, actual part pointer and separate world-position query |
| `0x80005da0` | Runtime RNG with captured signed bound |
| `0x8011aa34`, `0x8011ace8` | Actual conclusion and retained all-event barrier |
| `0x8019f298` | Retained audience TurnEnd |
| `0x8011b0d0` | Synchronous source phase/target table builder |
| `0x800f7718`, `0x800f76d4` | Majinai Done and EndCheck |
| `0x800050b4` | Source memset helper: low-byte fill, actual store order, destination result |
| `0x80127434` | Total hit damage: signed low halfword to word U+`0x264` |
| `0x801297bc` | Actual phase-event constructor leaf |
| `0x8011c510`, `0x8011c2b4`, `0x8011b2dc` | Retained Turn, Phase and Act parents; wrapper pending fallbacks |

ShadowGuard is not a generic HP/defense predicate. Total-hit damage uses sign
bit `0x8000` and modulus 65536, preserving low-halfword narrowing rather than
saturation. Act CommandInit first stores Input B+`0x171c` =0, then ORs command
flag `0x80` into B+`0xef4`; it allocates no UI. Unit status flags U+`0x138`,
attributes U+`0x104`, action flags U+`0x27c` and belong byte U+`0xc` are separate
fields. CheckStatusFlag alone has the null-unit early zero in this adapter;
Clear requires admitted storage.

The PhaseEnd status callback receives `actors.user`, the actual EventNativeRuntime,
not the outer Trio owner. That query requires memory/status; phase-table sorting
also needs the separately bound float reader. See [Phase/Move](battle-phase-move-continuation.md),
[Act](battle-act-sequence.md) and [Turn](battle-turn.md).

## Real event construction and barriers

`createRootEvent` maps an actual guest EventEntry address. Its scheduler event
ID is a different domain: Delete/Exists take IDs, while the generic Create child
returns the entry address. The parent owns subsequent entry/unit publication.
Move's phase-event leaf reads U+`0x294` for a nonzero selector or U+`0x298` for
zero. A null script completes with zero. Otherwise it creates `(script,10,0x20)`,
sets owner from U+0, stores the scheduler ID at U+`0x29c`, and returns that ID.
This is distinct from the legacy helper's unconditional constructor.

Barriers use actual scheduler IDs and packed conclusion checks. A nonzero
reserved item requires the still-unbound inventory-removal owner; earlier event
cleanup and acknowledged reaction work remain retained while it waits. No
visual actor count substitutes for a completed source barrier. See
[event barriers](battle-event-barriers.md).

## Numeric and population admission

Retained audience TurnEnd uses platform division only for a nonzero denominator
excluding signed INT_MIN/-1. Float truncation admits finite values in
[-2^31,2^31); outside that native domain the child holds instead of inventing a
conversion. These guards describe adapter support, not full original CPU/FPSCR
behavior. RNG and platform callback absence likewise hold at their actual child.

The original population suffix remains unbound. Only the retained child's actual
capacity-zero, amount-zero or battle-flag early returns can complete without it.
The legacy `audienceTurnEnd()` prefix is not retried as this child. Display,
direct damage, Burn lifetime, Freeze/recovery effects and sound publication also
have explicit pending frontiers in Turn. Wiring these controllers does not
prove a playable or complete battle. See [retained audience](battle-retained-audience.md).
