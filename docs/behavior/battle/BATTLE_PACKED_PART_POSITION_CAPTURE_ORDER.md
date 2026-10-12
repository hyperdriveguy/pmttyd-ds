# Native part lookup and world-position event capture

US G8ME01 disc0 revision0;static original inspection2026-10-12.
Read [position graph](BATTLE_UNIT_PART_POSITION_QUERIES.md) and
[part lookup](BTL_UNIT_GET_PARTS_PTR.md). No original execution claim.

GetPartsPtr reads unit14 head before checking signed requested ID. Negative ID
returns head directly. Nonnegative ID first reads head.next0 WITHOUT head-null
guard;single next0 returns head for any ID,without definition read. Otherwise
scan each part's definition4→word0 and next0;first matching word ID wins,miss0.
There is no unit/head/definition/cycle repair. BattleGetUnitPartsPtr independently
reads current battle,fetches unit by retained ID and invokes this getter.

GetPartsPos80112d98/10c retains E/A,resolves/translates actor,then resolves part ID.
Read CURRENT battle and fetch unit;discard that returned pointer. THEN call
BattleGetUnitPartsPtr,which reads another current battle/unit. Only after part
lookup capture RAW outputs A8/Ac/A10. Query full world coordinates into private
temporaries;for each X,Y,Z skip conversion/SetValue only for rawf1194d80,otherwise
fctiwz and ordered SetValue. No firstCall or null-part fallback. Anchors:initial
battle80112ddc/lookup80112de8,part helper80112df4,outputs80112df8/2e00/2e08,
world query80112e10. The first unit result is not the part's owner.

World-position leaf reads attributes1ac then owner4ec even on absolute branch.
Mask10000000 copies part18/1c/20 with ordered output stores;owner is not queried.
Relative branch preloads owner's visible3c/40/44 and part offset24/28/2c into
private temporaries,then reads signed owner189. X uses single fused
(offsetX*direction)+baseX;Y/Z use single add. Writes remain X,Y,Z. Source
unit114 scale and movement-direction188 are not substituted here. Later output
mutations do not cause relative source rereads,but absolute output/source aliases
can affect the next load.

Native arithmetic goes through explicit fma/add platform providers. This models
required operation boundaries,not all original exceptional/FPSCR behavior.
Inspection-only ELFs/listings remain `/tmp/pm-native-get-parts-pos.*` and
`pm-native-parts-world-pos.*`,outside history;reproduce with
`tools/inspect_dol_function.py` and original US symbol index. Tests cover signed/
single/multiple lookup,distinct current work captures,owner/scale distinction,
absolute alias versus relative snapshots,raw output skip and actual native
registry/event/part coordinates. Source part/unit writers,hit-offset layer,
full Jump/Game and original runtime/hardware remain separate requirements.
