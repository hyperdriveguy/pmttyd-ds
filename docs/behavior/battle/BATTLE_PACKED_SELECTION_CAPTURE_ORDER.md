# Selected-target and command-weapon native capture order

US G8ME01 disc0 revision0;static original inspection2026-10-12.
Read [complete selection contract](BATTLE_SELECTED_TARGET_AND_WEAPON.md).
No original gameplay or menu/unit producer claim.

GetSelectEnemy8010b540/a4 reads global battle BEFORE argument pointer E18,then
signed count battlee94 BEFORE RAW output A0/A4. Both outputs are captured before
writing either. Nonpositive count writes-1 twice. Otherwise signed cursor and
permutation select a retained row. Read signed row.unit,SetValue output0;ONLY
THEN read signed row.part,SetValue output1. No clamping,resampling,HP/status check
or output operand resolution. Anchors:battle8010b554,args8010b558,count8010b55c,
outputs8010b564/8010b56c,unit8010b5b4,part8010b5bc.

CommandGetWeaponAddress801057e8/a8 instead retains argument pointer BEFORE
capturing battle. Resolve actor,then capture RAW output BEFORE translation.
Translation selects its own global battle;unit lookup freshly captures another.
Kinds222/224..230 use the initially captured battle's command menu;other kinds
read their retained unit308 and wrapped40*ID ItemData weapon word. Zero weapon
pointer is a value,not an automatic fallback. Anchors:args801057fc,battle80105800,
resolve80105808,output8010580c,translate80105818,fresh battle80105820.

The menu cursor helper ALWAYS fetches the current acting unit before its
unsigned type guard;only types5/14 read its kind. Other supported modes must not
force an unnecessary unit-kind read. Unsupported9/11..13/>14 leave cursor output
untouched;original selected-weapon caller then uses an uninitialized cursor.
Native optional-domain failure/registry termination is explicit provider policy,
not an original scalar return or replacement weapon. Supported modes retain
original cursor offsets,wrapped index arithmetic and captured/current owner split.

Native tests cover signed count/cursor/permutation/halfwords,late part after
output alias,all supported and untouched cursor cases,kind branches,old-menu/
current-unit owners,raw output timing,wrapped held-item index and zero weapon.
Original instruction inspection ELFs/listings remain `/tmp/pm-native-selected-target.elf`,
`pm-native-command-weapon.elf`,`pm-native-command-cursor.elf`/`.disasm`,outside
history;reproduce with `tools/inspect_dol_function.py` and original US symbols.
Selected row/menu/ItemData/unit writers and full Jump/Game source/turn ownership
remain separate requirements.
