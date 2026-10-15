# Retained audience target and turn-end helpers

Implementation: [target source](../../source/battle/battle_audience_target_continuation.cpp),
[target API](../../include/battle/battle_audience_target_continuation.hpp),
[turn-end source](../../source/battle/battle_audience_turn_end_continuation.cpp),
[turn-end API](../../include/battle/battle_audience_turn_end_continuation.hpp).
These notes explain native continuation mechanics, not new measured game rules.

A cursor owns one original invocation. Retrying requires identical provider users,
callbacks and inputs. Target inputs compare float **bits**, so signed zero and NaN
payloads remain distinct. `false`/`nullopt` retains partial work; engaged numeric
zero is a completed result. Reset refuses reentry and discards native state only:
it does not undo guest writes or effects already performed by a provider. Caller
owns the lifetime of all providers; the identity checks do not cover the actor
`audienceLifecycle` member.

Original addresses are 32-bit guest tokens. Audience base is a separately captured
current global battle plus `0x2858`. Do not consolidate these captures into a
single battle pointer: a child may legitimately observe a different global owner.

| Audience offset | Reached interpretation |
| --- | --- |
| `0x13778` | Baseline float used for target scaling |
| `0x1377c` | Desired target float |
| `0x13780` | Signed offset float |
| `0x13790` | Signed word capacity for target clamps |
| `0x138cc` / `0x13904` | Phase-category list / count |
| `0x13908` | Turn-end random-event chance word |

The target parent adds bias to offset, then uses opposite-sign delta to cancel
that offset before calling Sub. Sub captures its own global owner and consumes
positive or negative chunks of at most the original unit literal. It freezes
quotient and desired target before holding on the source `fctiwz` provider. Once
acknowledged, the weighted branch divides the chunk by converted integer plus or
minus one. Preserve source wrapping, binary64 xoris/lfd conversion, single
precision operations and guest read/store order. Replacing this with host rounding
or a combined formula changes the contract. Nonfinite/nonprogressing chunk domains
enter sticky phase 7 until reset; they do not spin or manufacture progress.

Only the parent performs final desired-target and offset clamps, using its own
captured owner. Standalone Sub omits them. Source constants are guest reads,
including zero at `0x80424988`, one at `0x80424990`, negative one at `0x804249d4`,
weight step at `0x804249ac` and offset bounds at `0x804249d8/dc`.

Turn-end progresses through capture/chance update, division, target child,
threshold category, one random draw, target capture/conversion/count, and the
AddAudienceNum suffix. Chance increases by three with word wrapping and a signed
upper clamp of 100. Threshold divides twice the wrapped group-level sum by
Mario's level, with a minimum of five; exceptional division belongs to the
provider. The battle turn counter is a signed halfword. Category insertion
recaptures the global battle on every call and suppresses duplicates.

After target conversion, floor to zero precedes the signed capacity clamp.
There is no second floor. Existing population scans 200 records, stride `0x134`,
starting at audience `+0x1bc`: bit 0 required, mask `0x102` excluded, state byte
`+0x19 == 12` excluded. Repeated global reads, even unused ones, preserve source
ordering. The resulting amount is wrapped target minus count.

AddAudienceNum captures battle, audience and pouch independently, in order.
Capacity is pouch `+0x8c` **halfword**. Zero capacity/amount or battle `+0xef4`
bit `0x100` returns before requiring the suffix provider. Otherwise that provider
owns all original allocation, removal and RNG effects from `0x801a176c`, using the
frozen invocation. A hold must not replay the chance update or population prefix.
See [Trio scheduling](trio-scheduling.md) for its caller; this helper's completion
alone is not whole-battle completion.
