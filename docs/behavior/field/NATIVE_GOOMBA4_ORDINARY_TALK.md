# Native original Goomba4 ordinary talk with known quest guards

The binding uses the recovered [quest and item-selection source graph](ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md)
and [ordinary NPC event ownership](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md).
Current actor visibility remains the native story0..55 envelope; this is not a
claim that original lifetime or every later scene is bound.

PMNPC008 now retains all11 original source-order story rules and the original
gor_02_030_ prefix for the exact gor_02/Goomba4 profile. The shared talk selector
first checks partial knowledge of GSW1420 and GSF5405/5406. An entry quest arm
is excluded if the known quest byte differs from its required2/13, or if its
corresponding flag is knownfalse. Both arms must be excluded before ordinary
playback. Quest/flag unknown is distinct fromzero. After a known matching quest/true
flag, a live original pouch count0 can also exclude its arm through the
[key-item service core](../message/NATIVE_ITEM_TABLE_SELECTION_CORE.md).
Positive/unknown item counts still require unbound quest/window services; no
item absence is inferred from native mushroom quantity or a legacy checkpoint.
The default selector API therefore continues to reject Goomba ordinary talk
without explicit state, even though the pack now retains its source rules.

An explicit native fresh-file opening handoff initializes these three values
from the recovered swInit clear semantics: quest0, bothflagsfalse. Room changes
retain the snapshot. Native legacy files have no such evidence and decode into
unknown. This is a partial original-switch snapshot; the full global switch bank,
other quest writers and complete original save format remain unbound. Future
area scripts must publish their actual saved-switch changes into the owning
state instead of assuming new-game zero forever. Guard admission is scoped to
room2 and original area/name profile key2b0a9f83, not reusable character art.

The ordinary print uses unchanged original US IDs/control tokens, source speaker
me and the existing source actor anchor/pose request owner. Story branches
retain the exact164/175/176 boundaries and multi-suffix message keys. Talk start
uses live actor root/yaw; paper facing remains simulation-owned and rendering
uses staged original c_kuribo_n resources through citro3d/tex3ds. No story/quest
write, substitute scene or fake item selection is generated.

## Native checkpoint v7

The port's checkpoint remains40 bytes and is not an original GameCube save.
Version7 packs the existing native mushroom word at20 as follows. A completely
unknown snapshot still encodes version6, preserving existing fixture consumers.

| Bits | v7 meaning |
| --- | --- |
| 0..7 | Native mushroom quantity0..99 |
| 8..15 | Original GSW1420 byte, zero when unknown |
| 16 | GSW1420 known |
| 17/18 | GSF5405/5406 known |
| 19/20 | GSF5405/5406 values, zero when unknown |
| 21..31 | Reservedzero |

Other fields/checksum remain unchanged. The decoder rejects reserved bits and
values without knowledge flags atomically. v1..6 decode into unknown quest state;
knownfalse survives roundtrip. This compact native adapter does not serialize
original inventory or the complete source switch bank. Fixture tool --quest,
--flag5405 and --flag5406 explicitly supply development checkpoint evidence;
omission never meanszero. Native product UI exposes no guessed quest state.

Validation and remaining work are recorded in [the handoff](../../PROGRESS.md).
Next bind original item table creation/update/free, item-window service and
exact selection/removal/flag owners for both quest arms; then complete source
switch/inventory persistence, original event cadence and later actor lifetimes.
