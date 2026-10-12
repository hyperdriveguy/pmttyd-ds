# Home/visible position event capture and sentinel asymmetry

US G8ME01 disc0 revision0;static read-only-ISO inspection2026-10-12.
Read [position leaves](BATTLE_UNIT_PART_POSITION_QUERIES.md) and
[visible storage](BATTLE_JUMP_EVENT_CALLBACKS.md). No original execution claim.

GetHomePos801124f8/c4 and GetPos80113620/e8 retain E/A,resolvetranslate actor,
then fetch unit using CURRENT battle. After lookup,capture RAW outputs A4/A8/Ac
in order. Call their respective leaf with three private float temporaries before
any conversion/output. For each axis X,Y,Z,convert via fctiwz immediately before
SetValue;do not convert all axes first or reread unit position after output calls.
There is no actor-1/unit-null/firstCall guard in either wrapper.

GetPos alone skips an axis whose RAW output word equalsf1194d80. This skips its
conversion AND SetValue. GetHomePos has NO corresponding sentinel test:it still
converts/stores through normal destination routing. Do not resolve the output
word before this comparison or infer identical sentinel behavior from names.

Leaf Home uses unit30/34/38;visible uses3c/40/44. Each loads/stores X,then loads/
stores Y,then Z. Aliased leaf output pointers can therefore affect later source
reads;event-private temporaries instead retain all three coordinates before the
first output setter can mutate the unit. Original leaf output pointers are not
optional and have no null guard. Numeric exceptional/FPSCR behavior remains an
explicit platform/source domain;native raw-word copy tests do not prove it.

Instruction anchors:Home outputs80112534/3c/44 precede leaf8011254c and ordered
conversions8011255c/78/94. Visible outputs8011365c/64/6c precede leaf80113674;
raw sentinel tests80113678/6a0/6c8 precede each conversion. Native bindings key by
801124f8/80113620 and forward conversion to the required platform provider.

Original inspection ELFs remain `/tmp/pm-native-get-home-pos.elf` and
`pm-native-get-pos.elf`,outside history. Reproduce with
`tools/inspect_dol_function.py` and original US symbol index. Tests cover raw
sentinels,ordered aliasing leaf copies,snapshotted event floats,captured outputs,
home/visible separation and actual native registry/unit/event bytes. Source unit
writers,GetHitPos/motion ownership,full Jump/Game composition and original
runtime/hardware trajectories remain separate requirements.
