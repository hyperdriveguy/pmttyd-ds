# Pinch audience adapters and original threshold bindings

Scope: US G8ME01 revision 0 static DOL evidence. Complete audience adapters
`BattleAudience_Case_MarioDanger` at 8019f440/0x38 and
`BattleAudience_Case_MarioPinch` at 8019f478/0x38. Together with the complete
[threshold writer](BATTLE_PINCH_THRESHOLD_FLAGS_AND_TRANSITIONS.md) and
[all-unit warning scan](BATTLE_PINCH_ALL_UNIT_SCAN_AND_WARNING_CADENCE.md),
these provide an implementable local port contract. No runtime observations,
seconds-based cadence or audience-core completion are implied.

## Adapter service and float order

Each adapter calls `BattleAudienceBaseGetPtr` (801a86a0/0xc) independently.
That existing leaf freshly obtains the global BattleWork pointer from 8041eb90
and returns A=B+0x2858. The adapter therefore does not reuse a manager's retained
B or receive a unit pointer. It loads a binary32 source at A+0x13778
(B+0x15fd0), multiplies it using a single-precision `fmuls`, and calls
`BattleAudienceAddTargetNum` at 801a1a74 with f1=result and f2=+0.0.

| Adapter | Source multiplier address | Exact binary32 word | Numeric value |
| --- | --- | --- | --- |
| MarioDanger | 804249a0 | 3e199999 | 0.14999999105930328 |
| MarioPinch | 80424994 | 3d4ccccc | 0.04999999701976776 |
| Both, f2 | 80424988 | 00000000 | +0.0 |

Startup establishes r2=80427440 at 80005418..1f. The adapter loads multiplier,
source, then f2 before multiplying. No null, clamp, finite-value, time or side
check intervenes. Preserve binary32 rounding and source/global capture boundaries.
There is no defined new integer result assignment after the callee. The callee's
0x180-byte identity is pinned, but its accumulator logic, meaning of this source
float, admission policy and presentation are still open. Avoid claiming that
these adapters themselves add a fixed number of visible audience members.

## Original kind heads

The verifier pins the first 16 bytes of each source definition. In every listed
head D+0x0c is 5 and D+0x0d is 1. The writer accesses the unit's live definition
pointer, not this lookup table; these are source examples, not a rule covering
all enemy definitions or mutations.

| Source actor | Kind | Definition address | Threshold +0x0c / +0x0d |
| --- | --- | --- | --- |
| Mario | 222 | 80372afc | 5 / 1 |
| Goombella (Christine) | 224 | 80393bec | 5 / 1 |
| Koops (Nokotarou) | 225 | 8038ef34 | 5 / 1 |
| Yoshi | 226 | 8037c79c | 5 / 1 |
| Flurrie (Clauda) | 227 | 803791f4 | 5 / 1 |
| Vivian | 228 | 8038b89c | 5 / 1 |
| Bobbery (Sanders) | 229 | 8038780c | 5 / 1 |
| Ms. Mowz (Chuchurina) | 230 | 803818c0 | 5 / 1 |

## Reproducible evidence and implementation checks

Run `python3 tools/verify_battle_pinch_status.py` with the read-only original ISO
(default path) or `--iso PATH`; optional `--output PATH` emits an evidence-count
report and rejects overwriting the ISO. It pins 12 whole-body SHA-256 identities
(4 complete and 8 linked), all 225 instructions in the four complete bodies,
and 14 original data/startup spans. Linked status/audio/getter/audience-core/
manager hashes are identity evidence, not claims of complete semantic recovery.

Port checks should cover entry/exit flags, suppressed transition persistence,
zero/negative/full HP, first-callback mutation before the second test, dead or
wrong-kind actor shadowing, unsigned-byte decrement/signed-byte reread, sound
gate after timer reset, group-switch timer reuse, and no-actor Mario reset.
The verifier checks original evidence rather than testing a native implementation.
ISO extraction is inspection-only outside source history; no ISO or extracted
assets are added to the repository. Full-game requirements, audio internals,
status policy, audience accumulation and actual scheduling remain open requests.
