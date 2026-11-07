# Retained original HUD value producer

US G8ME01 revision0, original `statusGetValue8013c9ac/178`. The whole body
SHA256 is `bc743f6eb6f985fd41b52dd17d76d542b1939081d4de853b7d0b73c003d473fd`.
`tools/verify_source_status_values.py` pins all20 actual linked calls. This is
static original-code evidence, not a Dolphin observation or a native HUD receipt.

`resumeSourceStatusValues` retains the whole function independently of the
status window's animation/display controllers. Output offsets identify the
original argument record's cells. Its owner supplies actual guest storage or
the actual parent's private cells, without inventing a guest stack address.
Each admitted store performs one atomic STH; a missing read/child/store holds
without replaying an earlier output write or acknowledged child.

| Output offset | Value |
| --- | --- |
| 00/02 | Mario HP/maxHP |
| 04/06 | FP/maxFP |
| 08 | original partner ID |
| 0a/0c | partner HP/maxHP |
| 0e | Star Points |
| 10 | coins |
| 12/14 | AP/maxAP |

The function first reads actual GP through80415fd0, then GP+14. Whole-word
zero selects field mode; any nonzero selects battle mode. It never infers mode
from a native Scene, HUD visibility, selected actor or a guessed partner.

Field mode calls the four separate pouch HP/maxHP/FP/maxFP getters, each followed
by its original STH. Then AP, maxAP, Star Points and coins use their own actual
children in that order. `marioBgmodeChk8005c164` selects the partner query:
zero calls `marioGetParty801410e8`; nonzero calls `marioGetPtr8005e0c0`, reads
pointer+247 and extends that byte with sign before storing output08. The
ordinary partner result's low16 bits likewise survive. There is no local clamp.

The field path calls `pouchGetPtr800d5eb0` twice. After each call it freshly reads
the signed halfword at output08, calculates the wrapping32-bit index14*ID and
reads pouch+index+6 for HP, then the second returned pouch+index+2 for maxHP.
Neither the first pouch pointer nor the earlier partner ID may replace those
fresh values. Unknown/negative/out-of-range source indices do not become a
fabricated absent partner: the actual source memory read must succeed or hold.

Battle mode captures W through8041eb90 and passes that same captured W first to
`BattleGetMarioPtr800f87a8`, then `BattleGetPartyPtr800f8728`. Both children
complete before Mario's actual signed halfwords10c/108/112/10e are read and
stored to00/02/04/06. AP/maxAP/Star Points/coin getters still run in their
original order. A real null partner result performs the original three zero
STHs to08/0a/0c. A nonnull result reads kind WORD(partner+8), calls original
`BattleTransPartyId800f8444`, stores that returned ID, then reads partner
HP10c/maxHP108. No native actor-index or portrait heuristic supplies this ID.

All source words, halfwords, bytes and child returns are optional. Engaged zero
is a completed original result; unavailable remains a hold. Output08 is also
an optional actual read, preserving unwritten/private aliases and late source
changes. The parent does not implement or acknowledge any child by its name.
Original source getters and unit/party ownership must be bound explicitly.

Retained provider identity includes callbacks, userdata, strong lifetime control
blocks and output ownership. The pure current predicate must prove both actual
source and output storage for the serialized invocation. Stale ownership
latches; retrying cannot revive it. Reentry and reset during execution reject.
Explicit cursor reset discards continuation only and cannot undo prior stores.

Tests cover field, background-partner, battle, genuine-null-party branches,
all reached read/child/store boundaries held twice, captured W versus fresh
Mario reads, both fresh field pouch/output reads, signed byte/halfword wrapping,
unknown source bytes, same-address/different-lifetime identity and stale/reentry
behavior. Child returns in these tests are explicit fixtures, not production
getter implementations or game state. The native HUD must publish only a
completed result from its actual owner, then retain the original presentation
and [visibility arbitration](../battle/BATTLE_STATUS_WINDOW_ARBITRATION.md).

This producer does not yet activate the remaining partner/AP groups in
[the existing native HUD](NATIVE_BATTLE_HUD_STATUS_ELEMENTS.md), run statusWinMain,
animate slide/blink state, draw a single icon or change a battle resource.
Original pixels, fonts, music, dialogue and licensing remain unchanged.
