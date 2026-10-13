# Public decompilation audit

Collection-level audit moved out of the [behavior register](../README.md) on 2026-10-12 to keep that index small. Content is verbatim.


Pinned source: [doldecomp/ttyd](https://github.com/doldecomp/ttyd/tree/62131fc31866f85a12e4817a8dc37f0c25eb5f8c),
revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`, declared Japanese G8MJ01
revision 0 support. Source existence, build matching and US behavior coverage
are three separate properties.

`tools/decomp_coverage.py` parses `configure.py` with Python AST, without
executing upstream code. It selects explicit `progress_category="game"`
libraries and the configured Rel helper calls; SDK/middleware helpers are
excluded. It records each object declaration, path, source presence/size/hash
and config line. This is an object census, not a percentage of decompiled
functions, instructions, bytes or game behavior.

Results for the pinned snapshot: **427 game-category object declarations**,
421 NonMatching and 6 Matching. **134 have source files; 293 do not**. A present
file may be partial or a stub. The six Matching declarations were not rebuilt
or independently verified against the Japanese game, and say nothing by
themselves about US equivalence.

| Config library/group | Declared objects | Source files present |
| --- | --- | --- |
| battle | 40 | 30 |
| battle_ac | 21 | 2 |
| battle_sac | 8 | 1 |
| data | 4 | 3 |
| driver | 24 | 24 |
| effect / effect_n64 | 76 / 103 | 6 / 2 |
| event | 45 | 15 |
| manager | 8 | 8 |
| mario / motion / party | 10 / 13 / 11 | 6 / 2 / 3 |
| sequence | 10 | 7 |
| unit / unit_party | 6 / 7 | 0 / 1 |
| window | 7 | 1 |
| library named sdk, categorized game by upstream | 4 | 3 |
| MarioSt | 25 | 15 |
| Explicit REL objects: dig / mri / pik / tou2 | 2 / 1 / 1 / 1 | 2 / 1 / 1 / 1 |

The five configured REL source objects are not a complete census of the 29
US ISO modules. The Japanese config even names a module not present in the
US REL inventory (`dig`); do not assign US content by matching names alone.
Cross-region REL coverage and shipped event streams need their own census.

Concrete reviewed incompleteness: the public defense checker is a constant
return stub; `BattleCheckDamage` contains only an initial fragment; most
action-command objects have no source. Conversely, substantial source exists
for the event manager and saved-work accessors despite NonMatching labels.
Research priority should be informed by those distinctions, not by treating
all 421 declarations as equally unknown.

Reproduction (snapshot and reports stay ignored):

```sh
python3 tools/decomp_coverage.py --snapshot artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c --output artifacts/reference/behavior/decomp-coverage.json
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/verify_behavior_contracts.py
```
