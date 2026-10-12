# Battle flag query and frame conversion: complete local read order

US G8ME01 disc0 revision0; native integration inspection2026-10-12.
Static original DOL instructions,not original execution or measured timing.
Read [Hammer gate](BATTLE_HAMMER_WEAPON_BRANCHES.md) and
[frame conversion](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md).

| Body | Address / size | SHA256 |
| --- | --- | --- |
| btlevtcmd_check_battleflag | 8011384c /50 | fa413d675aa6da54b0bf62d16727bb23e972a8daa2f8c9f48ba06617a92f4a60 |
| btlevtcmd_ftomsec | 80103dc0 /50 | 07c78b37f8dcb74cc80c27f09d4786a60ac41001f7ee82906d8cfa2620bcc431 |

Inspection used `tools/inspect_dol_function.py` with the original US symbol index
and read-only ISO. Inspection-only ELFs remain `/tmp/pm-native-check-battleflag.elf`
and `/tmp/pm-native-ftomsec.elf`,outside source history. General behavior evidence
already pins flag mask/flag reads and SetValue sites;the Majinai verifier pins
ftomsec's complete hash. These names identify the original functions,not host
callbacks or a source-engine execution claim.

The battle-flag wrapper's exact order is:

1. Retain argument pointer E18 at80113858.
2. Capture current battle8041eb90 at8011385c.
3. Read RAW mask A+4 at80113860,then battle.wordef4 at80113864.
4. Read RAW destination A0 at80113868;test numeric flag AND mask.
5. SetValue(E,raw destination,boolean1/0) and return2.

Neither operand goes through GetValue. This refines the older summary's operand
listing:destination is read AFTER mask/flag,not before. Later pointer/argument
mutations cannot replace retained A/B,but a flag-read alias can affect the later
raw output token. No firstCall gate or flag mutation occurs locally.

ftomsec retains E/A,reads RAW input A0,then calls GetValue. Multiply resolved word
by1000 with32-bit wrap at80103de0. Read RAW output A+4 at80103de8 AFTER resolution
and multiplication. Signed-divide the wrapped product by60 at80103df0,truncating
toward0,then SetValue and return2. Denominator is a fixed original literal;
GP refresh/OS clock/render cadence are not queried. Existing sound requests29,
200,90,80,5 yield483,3333,1500,1333,83ms operands respectively. This arithmetic
does not prove original audible wall-clock timing or complete sound playback.

Native callbacks use original addresses8011384c/80103dc0 and shared event memory.
Tests exercise read-order aliases,raw masks,late output tokens,signed wrap,
independent firstCall invocations and original packed Jump's mask2 operands.
Unit/battle writers,source domains,concurrency,hardware and full graph execution
remain separate requirements.
