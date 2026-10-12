# HP and unit-work event leaves: capture-order refinement

US G8ME01 disc0 revision0,static read-only-ISO inspection2026-10-12.
Refines [unit-query summary](BATTLE_UNIT_QUERY_EVENT_LEAVES.md);no original
execution,full-unit writer/lifetime or Game integration claim.

| Body | Address / size | Established capture order |
| --- | --- | --- |
| GetHp | 801101c4 /78 | Retain E/A;capture battle BEFORE raw actor resolution/translation;ID-1 returns2 BEFORE unit lookup;otherwise use retained battle,read signed unit10c,then RAW output A4 and SetValue |
| GetMaxHp | 801100cc /78 | Same order,read signed unit108 |
| GetUnitWork | 8010f828 /84 | Retain E/A;resolve actor→translate;resolve index A4;capture RAW output A8;ONLY THEN read current battle/lookup unit;read unit218+wrap32(index*4),SetValue |
| SetUnitWork | 8010f7a4 /84 | Retain E/A;resolve actor→translate,index A4,value A8;ONLY THEN read current battle/lookup;direct WORD store at unit218+wrap32(index*4);return2 |

The shared-pipeline summary must not imply a common early battle/unit capture.
HP's ID-1 gate precedes unit lookup;unit-work callbacks have no such gate and
no index bounds check. Negative indices can address preceding fields. Input
indices/values resolve independently;destinations remain raw event tokens.
Actor translation's own global battle read is separate from the caller's read.
All bodies ignore firstCall. No pointer/null/HP clamp or event cancellation is
introduced by these local callbacks.

Instruction anchors:HP captured battle801101dc,ID comparison801101fc,lookup
80110210,halfword80110214,raw output8011021c. Max equivalents801100e4/80110104/
80110118/8011011c/80110124. Get-work raw output8010f86c precedes battle8010f870
and lookup8010f878;load8010f888. Set-work value resolution8010f7f0 precedes
battle8010f7f8 and lookup8010f800;store8010f810. These refine ordering;they do
not close all counter/unit source domains or neighboring callback bodies.

Reproduce using `tools/inspect_dol_function.py` with original US symbol index
`artifacts/reference/us_symbols.csv`. Original inspection-only ELFs remain
`/tmp/pm-unit-get-hp.elf`,`pm-unit-get-max-hp.elf`,`pm-unit-get-work.elf` and
`pm-unit-set-work.elf`,outside history. Native tests cover retained versus fresh
battle,late/captured output tokens,signed halfwords,HP-only early-out,raw work
aliases and real event/registry memory. Unsupported native addresses are provider
domain assertions,not original graceful fallback behavior.
