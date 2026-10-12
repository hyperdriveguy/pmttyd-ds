# Published Switch behavior and patch boundaries

These publisher descriptions establish features to recover, not a complete
static contract. They are separate from the supplied base 1.0.0 binary findings.
Sources were consulted on 2026-10-12.

## Remake features described before release

Nintendo's May 22, 2024 feature article describes:

- L opens the field Partner Ring, allowing partner changes without the Party menu.
- ZL requests exploration hints.
- Revised quick-travel pipes return to explored areas through a marked pipe room
  accessible from Rogueport after discovery.
- Ian Foomus sells Trouble Center information for coins.
- The Battle Master offers combat advice.
- Star Pieces and Shine Sprites unlock art/music galleries.
- Equipping Nostalgic Tunes selects the classic soundtrack.

Exact unlock flags, prices, input edges, selection order, music coverage and
persistence require the shipped data and consumers. The pre-release article says
tap L; the later tips article says hold L. Do not turn that wording difference
into a verified input threshold. [Nintendo: 7 new details](https://www.nintendo.com/au/news-and-articles/7-new-details-in-paper-mario-the-thousand-year-door/).

## Battle and exploration controls described by Nintendo

Nintendo's July 24, 2024 tips describe A timing for jump attacks and damage-reducing
guards; B timing can negate damage and retaliate, with stricter timing. Successful
Tattle retains enemy information for subsequent encounters. Audience reactions
can supply items and Star Power. Badges affect moves, strength and timing.

The same article describes Y to exchange Mario/partner positions in battle,
directional menu shortcuts, X for active Goombella's field commentary, Shine
Sprites exchanged with Merlon for partner upgrades, and Star Pieces exchanged
with Dazzle for badges. Numerical costs, exceptions and formulas are not supplied.
This article postdates 1.0.1 and cannot certify every base-build branch.
[Nintendo: Pack a papery punch](https://www.nintendo.com/en-ca/whatsnew/pack-a-papery-punch-with-these-tips/).

## Changes explicitly assigned to 1.0.1

Nintendo's July 9, 2024 update notes report fixes involving:

- Passing Twilight Trail's fallen tree before Tube Mode, including save-load
  repositioning for affected progress states.
- Occasional loss of field control following hammer contact or Ms. Mowz's ability.
- Battle progression/status-icon faults after extended uninterrupted play.
- A Petal Meadows grass-hidden Koopa Troopa battle failing to end correctly.
- A whole turn sometimes skipped after Double Dip or Triple Dip.
- Text corrections in English and several other languages, plus unspecified fixes.

These are version-difference test leads. Their existence does not tell us the
exact failing base-build branch or reproduce the fault. Do not silently apply
1.0.1's corrected behavior to a specification explicitly based on 1.0.0.
[Nintendo Support: update history](https://en-americas-support.nintendo.com/app/answers/detail/a_id/65608/p/988/c/950).

## Comparison policy

Existing GameCube formulas and story text are comparative hypotheses for Switch.
Keep each claim tagged with its actual build and evidence. For current static-only
work, close differences using data/bytecode/native consumer traces. Gameplay
capture is deferred until the user requests it. Do not use a walkthrough or a
mod's altered behavior as proof of the supplied executable's unmodified rules.
