# Original native weapon-menu icons

Native weapon rows now draw the original icon beside the unchanged US name,
inside the existing original type0 frame with the original font and selection
hand. Target rows retain their current presentation. Physical navigation and
whole-row touch hit regions are unchanged; touch remains optional.

Jump and Hammer use the complete equipment icon leaves described in
[source bindings](../battle/WEAPON_TABLE_SOURCE_BINDINGS_AND_ICON_LEAVES.md#complete-icon-leaves-fresh-pouch-and-signed-level).
Each helper independently queries current pouch storage. Byte98/99 equal3
returns169/166;equal2 returns168/165;every other value returns167/164, including
negative byte encodings and values above3. No level clamping or row-ordinal icon
formula is introduced. The native presentation obtains the actual supplied
ProloguePouch for each rendered row. Unknown legacy pouch state does not invent
an equipment level; those rows can omit an icon until a real pouch is owned.

Partner Headbonk uses unsigned halfword weapon80393ea8+4:icon344. The command
metadata now carries the source weapon icon in PMTCMD2 (three rows, four LE32
fields and64 name-key bytes per row). Original weapon addresses, targeting words
and name keys retain their values. Old PMTCMD1 packs require rebuilding; no
made-up icon fallback is admitted. This is a private native pack, not a GameCube
format or original save.

`battle_weapon_icon_assets.py` pins the complete two original icon-helper bodies
and three complete default weapon records in the read-only US ISO. The original
icon.bin descriptors and icon.tpl cells164..169/344 are packed without resampling
into the existing512×512 RGBA8 atlas. Its GPU allocation remains1MiB. Native
screen scaling adapts presentation, without changing packed pixels. See
[assets](../../ASSETS.md).

Tests exercise source level boundaries and independent pouch queries, actual
command/partner bindings, absent-pouch admission, source fingerprints, unchanged
original cell pixels and nonoverlapping atlas rectangles. Native captures use
physical inputs and isolated emulator profiles with explicit development v8
pouch fixtures; they are not original card saves or story progression evidence.

This remains the native weapon-row adaptation. The full rotating action-class
menu, complete weapon-table builder/eligibility/upgrade moves, tutorial command
ownership, damage/turn services and lower-screen audience scene remain required.
Icon presentation alone does not prove any of those systems complete.
