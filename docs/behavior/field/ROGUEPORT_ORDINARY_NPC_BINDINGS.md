# Rogueport ordinary NPC source bindings

US G8ME01 disc0 revision0; read-only tagged-map, REL and DOL inspection.
Partial answer to [OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[OW-ROAD-01](../../requests/OW-ROAD-01.md). Coordinates, directions and
requests below are source data, not Dolphin observations. Story first-match
selection is the original GSW(0) operand `0xf5de0180`.

## Source placement and registration

Original `m/gor_01/s` and `m/gor_02/s` are Shift-JIS tagged text, with
`<Npc>` blocks containing `<name>`, `<tribe>`, `<position>`, `<dir>` and
other fields; each field/block closes with the generic `</>` marker.
A standard XML parser cannot read this grammar unchanged. Names are exact
identifiers, including full-width numeric suffixes; preserve their source
bytes or decode losslessly. All four selected rows have `aitype="なし"`,
`territory_type="NPC_TERRITORY_TYPE_NOTHING"`, zero territory vectors and
search/homing ranges/angles, and `btl_setup_no="BTLNO_INVALID"`.
These fields alone do not prove an NPC has no regular event.

| Room / actor key | Tribe key | Source XYZ / direction | Setup row / init / regular / talk REL offsets |
| --- | --- | --- | --- |
| gor_01 / クリボー２ | NPCクリボー | (200,0,250),270 | 0x35118 / 0x32e08 / 0x32e24 / 0x32e34 |
| gor_01 / チューさん２ | チューさん | (240,0,-200),270 | 0x3522c / 0x33860 / null / 0x33868 |
| gor_01 / 水夫ボム兵 | 水夫ボム兵 | (185,0,-120),90 | 0x35288 / 0x33998 / null / 0x339a0 |
| gor_02 / チューさん３ | チューさん | (150,0,-400),270 | 0x452d4 / 0x43dc8 / null / 0x43dd0 |

All setup rows have flag word0x80600. Setup is unconditional at gor_01
init0x3c4c4 and gor_02 init0x4d630. `evt_npc_setup`0x8005323c iterates
92-byte records; +0 actor,+4 flags,+8 second flags,+12 init,+16 regular,
+20 talk. It finds the previously loaded actor by exact name, installs these
owners, and yields until all init events finish before starting regular events.
No placement lives in these setup records: read the tagged-map file and
then apply its init event. Goomba2 init sets NPC mask0x40000000; the other
three selected init streams only return/end.

## Banks and pose identifiers

`npcGetTribe`0x8004b828 scans 84-byte DOL rows beginning0x803316d0,
compares row+0's string, and advances by0x54 until null. Row+4 is model;
+8,+12,+16,+20,+24,+28,+32,+36 are pose pointers.

| Tribe / row address | Model bank | Poses in pointer order |
| --- | --- | --- |
| NPCクリボー / 0x8033335c | a/c_kuribo_n; companion a/c_kuribo_n- | S_1,Z_1,S_1,T_1,W_1,R_1,S_1,S_1 |
| 水夫ボム兵 / 0x8033320c | a/c_suifu_b; companion a/c_suifu_b- | S_1,Z_1,S_1,T_1,W_1,R_1,S_1,S_1 |
| チューさん / 0x80333554 | a/c_tyusan; companion a/c_tyusan- | CHU_S_1,CHU_Z_1,CHU_S_1,CHU_T_1,CHU_W_1,CHU_R_1,CHU_S_1,CHU_S_1 |

The stationary east Chu3 is the safest immediate outdoor slice: null regular,
return-only init, exact placement, and original CHU_S_1 idle. Interpret the
remaining pose roles through their owning NPC services; strings alone do not
prove full animation coverage. [Professor bindings](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md)
own his story8/9/10 overrides and different c_kuri_h bank.

## Ordinary talk and exceptions

All four talk streams switch on story and use first matching CASE_LESS
(opcode38):56,112,164,215,261,339,373,401,then default. Suffix00..08 maps
in that order; original prefixes are respectively `gor_01_027_`,
`gor_01_030_`,`gor_01_031_`,`gor_02_031_`. Every arm calls
`evt_msg_print(0,ID,0,"me")`; no other local branch or side effect exists.
The actual current actor is speaker: do not render literal "me" as a name.
All36 keys were checked directly in the read-only ISO msg/US/gor_01.txt
and gor_02.txt; each appears uniquely as a null-terminated identifier.
Use original US resource payloads, IDs and control tokens unchanged.

Do not bind all four as permanent outdoor sprites. Goomba2 regular0x32e24
RUN_CHILDs `gor_villager_urouro`0x200e8, which reads current XYZ, then calls
`urouro_init_func(me,X,Z,0xf24c0a80,0xf24af280,0)` and
`urouro_main_func(me)`. That motion driver remains unbound.
Chu2 and the sailor are tavern first-floor actors: `house03_f1_fade`
0x35f34 changes local color by +/-10 under door state0xfd050f8e, clamps
0..255, applies the map fade helper, then requests RGB(local,local,local),
alpha255 for sailor0x35fb8 and Chu2 0x35fd4 every frame. Their placement
must remain behind the tavern door/display lifecycle; no ordinary outdoor
visibility claim follows from their return-only init.

Later exceptions: story341 invokes `gor_01_koopa_evt`, which hides Goomba2
at0x39bd8; story400 named entry `minnnanokoe` invokes `minnnanokoe_evt`,
which hides Goomba2 at0x3a8a8 and deletes Chu2/sailor at0x3a980/0x3a98c.
The separate `sekai_yami` entry uses an animation TEV callback on Goomba2
at0x3a650. Neither named-entry scene is equivalent to story400 alone.
A conservative early-native envelope0..55 is a scoped implementation boundary,
not a newly recovered visibility gate. Preserve later scenes as independent
source owners before claiming complete all-story behavior.

## Display facing: local evidence and remaining owners

Ordinary `npcDisp`0x8004c50c loads NPC ry+0x144 at0x8004c530,
negates it, calls `reviseAngle`0x80007328 and retains that result as draw
facing. It calls `mtx_setup`0x8004ca80(NPC,matrix,-1), then
`animPoseDrawMtx`0x8003b4d8 with that facing, a separate SDA float,
matrix and system-selected pass1/2/3. The facing parameter is an animation-facing request, not a world-matrix yaw.
Ordinary dir270 requests90 after normalization; applying90 directly as world
yaw can turn planar XY bank geometry edge-on. Native paper-facing correction
must retain its adapter status until the complete pipeline is bound.
`reviseAngle` calls fmod with a read-only DOL double, rounds to single,
substitutes its zero constant for NaN, and adjusts negative values by its
single full-turn constant. Read-only constants are double360 at0x802bf240,
zero0 at0x8041f448 and float360 at0x8041f45c (r2 base0x80427440).
The separate DrawMtx float is2 at0x8041fd34. Matrix, NPC pre-display/facing smoothing, pose
selection, tribe scale/height, lighting and shadow owners still require their
full contracts before declaring correct native rendering.

## Reproduction and limits

Read-only ISO tagged sources: gor_01/s offset991157056,length13044,
SHA25675f6c76940df2609dcfbfcdf42bd6f108a653fd7a8d25154c6f906c24d641eb5;
gor_02/s offset993344684,length9322,
SHA256dd1dd8400fa0b94dbd6d266155dd1e9caba82176e7eec38e46acf0f3328ef2ec.
`tools/rel_event_inventory.py` re-extracted both room init streams, selected
init/talk/regular streams and all named exception scenes directly from ISO.
All selected actor direct references in annotated gor_01/02 events were
surveyed; dynamic operands, foreign scripts and runtime observation remain
separate gaps. `tools/inspect_dol_function.py` re-extracted setup, tribe lookup,
entry, display and angle normalization. Inspection artifacts stay in /tmp;
original assets remain local, ignored and subject to Nintendo/Intelligent
Systems rights. Documentation-only: no native build/test or hardware claim.
