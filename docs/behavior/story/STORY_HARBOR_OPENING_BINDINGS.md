# US harbor opening: event, story-state and message bindings

US G8ME01 disc0/revision0,recovered2026-10-12. Evidence B:original shipped
gor.rel and msg/US/gor_00.txt,external US symbol annotations and ADDR32 relocation
streams. This is an initial binding/control-flow inventory,not a complete scene
specification or executed tutorial trace. Do not fill gaps with invented dialogue,
actors,combat outcomes or Switch behavior.

## Original module and source boundaries

The read-only ISO contains gor.rel642,564 bytes,SHA-256
`d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a`.
The US message resource is59,142 bytes,SHA-256
`bd39efb45105781a403ca5e889ba8e9fb8dbaaaf0b2bd60af6955e38073acdcc`.
Its fresh census has328 key/text pairs,no duplicate keys,no unmatched key.
The20 referenced keys below each occur once. Full text/control tokens remain
original local data;their per-payload hashes/offsets/sizes are retained in the
ignored message census. No story prose is substituted for those bytes.

`tools/rel_event_inventory.py` reads the shipped module directly from the ISO
without extracting it. It validates section/import/relocation/event spans and
retains raw operands alongside self/module0 ADDR32 bindings. It also records
foreign module references without inventing loaded addresses. BSS and one-past-section targets retain section/addend identity without string
dereference;BSS is never interpreted as file-header offsets.
Bounded printable identifier decoding is a convenience,not a general string or
control-token decoder. Non-ASCII actor keys below retain source offsets and
symbol annotations;do not replace their bytes with translated actor names.

All offsets in this document are **REL file offsets**,not runtime VAs. Loaded
addresses and function pointers depend on actual relocations/lifetimes.

| Event annotation | File start | Size | Commands |
| --- | --- | --- | --- |
| gor_00_init_evt | 0x2f004 | 0x870 | 174 |
| enter_gorotsuki_town_init | 0x25290 | 0xe8 | 14 |
| christine_osoware_setup | 0x25378 | 0x148 | 18 |
| enter_gorotsuki_town | 0x254c0 | 0x948 | 211 |
| christine_osoware | 0x25e08 | 0x594 | 93 |
| gundan1000tai_beforebattle | 0x2639c | 0x830 | 132 |
| gundan1000tai_afterbattle | 0x26e84 | 0x12c4 | 276 |

Each whole event span is independently hash-checked against the ISO,including
original unrelocated words. A span hash alone does not resolve its event graph.

## Story-state entry selection and continuation

The init event switches encoded operand0xf5de0180 at0x2f528. Preserve this raw
namespace operand until its event-variable/persistent-storage mapping is fully
connected. Its relevant literal case values select:

| Value | Original requests in the selected case |
| --- | --- |
| 2 | Child actor init at0x2f538;spawn enter_gorotsuki_town at0x2f540 |
| 3 | Child actor init at0x2f550;set local0=1 at0x2f558;spawn christine_osoware at0x2f564 |
| 4 | Child actor init and christine_osoware_setup at0x2f574/57c;crowd init(count400)/subfunction requests;set local0=1;spawn beforebattle at0x2f5bc |
| 5 | Child actor init at0x2f5cc;crowd init(count400)/subfunction requests;set local0=1;spawn afterbattle at0x2f60c |

Here child execution is opcode94;spawn is92. These are distinct original event
requests. The existing scheduler/ownership contracts still require complete
child/local-copy/inline context validation for these scenes;do not collapse them
into ordinary synchronous calls or unconditional fire-and-forget code.

The scene chain writes the same story operand in its main stream:

| Write site | Value | Following binding |
| --- | --- | --- |
| 0x25dbc | 3 | Spawn christine_osoware at0x25df8 |
| 0x2633c | 4 | Spawn beforebattle at0x2638c |
| 0x26b24 | 5 | After reset/NPC movement requests,child afterbattle at0x26bbc |
| 0x280f8 | 6 | Later scene cleanup/continuation needs recovery |

These writes and re-entry cases show distinct resume points. They do not prove
the scene is interruption-safe or define when saves are allowed. Several resume
paths pass local0=1;christine_osoware,beforebattle and afterbattle have leading
IF_EQUAL(local0,1) setup blocks. Their exact restored control/camera/actor state
must be retained rather than restarting every narrative action on re-entry.

## Actors and placement requests

Actor init at0x25290 creates four NPCs with local actor-key offsets:

| Key offset | Symbol annotation | Entry model identifier |
| --- | --- | --- |
| 0x14b28 | str_クリスチーヌ_gor_00014b28 | c_pkr |
| 0x14b40 | str_第三勢力幹部_gor_00014b40 | c_kanbu_ibt |
| 0x14b5c | str_第三勢力部下Ａ_gor_00014b5c | c_zako_n |
| 0x14b6c | str_第三勢力部下Ｂ_gor_00014b6c | c_zako_n |

It sets each tribe and places all at(0,-1000,0). The separate setup event
requests positions/facing as follows (original engine coordinates):

| Actor key | Position | Requested Y rotation |
| --- | --- | --- |
| 0x14b28 | (175,15,-300) | 90 |
| 0x14b40 | (225,15,-300) | 270 |
| 0x14b5c | (255,15,-330) | 270 |
| 0x14b6c | (280,15,-330) | 270 |

Each setup actor receives flag_onoff(1,key,0x600) followed by
flag_onoff(1,key,0x40000020). These are exact requests,not yet an interpretation
of the NPC driver flags. Actor-key matching and resource models establish more
than message-file order alone;US displayed character names and full pose/tribe
semantics still need their own data/service bindings.

## Message IDs and speaker operands

Every listed call is evt_msg_print(0,key,0,speaker). Speaker0 remains literal0;
do not invent attribution for those two calls. Nonzero speaker operands are
local actor-key relocations. Listed order is **command-address order in each
stream**,not proof that all messages execute sequentially without concurrent
motion/effects/waits. The full original message bytes and tokens must be played.

| Event | Call offset | Message ID | Speaker key offset |
| --- | --- | --- | --- |
| enter | 0x258a8 | mac_0_014 | 0 |
| enter | 0x258e0 | mac_0_015 | 0 |
| christine | 0x25f60 | mac_0_016 | 0x14b28 |
| christine | 0x2610c | mac_0_017 | 0x14b40 |
| christine | 0x26124 | mac_0_018 | 0x14b28 |
| christine | 0x2616c | mac_0_019 | 0x14b40 |
| christine | 0x26184 | mac_0_020 | 0x14b5c |
| christine | 0x26314 | mac_0_021 | 0x14b28 |
| beforebattle | 0x2673c | mac_0_022 | 0x14b28 |
| beforebattle | 0x26784 | mac_0_023 | 0x14b40 |
| beforebattle | 0x26888 | mac_0_024 | 0x14b40 |
| afterbattle | 0x27090 | mac_0_025 | 0x14b40 |
| afterbattle | 0x270d8 | mac_0_026 | 0x14b40 |
| afterbattle | 0x27390 | mac_0_027 | 0x14b40 |
| afterbattle | 0x274b4 | mac_0_028 | 0x14b28 |
| afterbattle | 0x27514 | mac_0_029 | 0x14b28 |
| afterbattle | 0x276d4 | mac_0_030 | 0x14b40 |
| afterbattle | 0x276f4 | mac_0_031 | 0x14b40 |
| afterbattle | 0x27868 | mac_0_032 | 0x14b40 |
| afterbattle | 0x280d8 | mac_0_033 | 0x14b40 |

See [message print ABI](../message/MESSAGE_PRINT_EVENTS.md) for how speaker/window ownership
is consumed. Story fidelity requires call-site attribution and conditions
alongside IDs;an ID census alone is insufficient. No transcript or paraphrase
is committed here.

## Tutorial battle boundary and remaining closure

Beforebattle0x269dc requests evt_npc_set_battle_info(actor0x14b40,literal2),then
0x269ec requests evt_npc_battle_start(actor0x14b40),then0x269f8 waits through
evt_npc_wait_battle_end. Following this are camera,Mario/actor pose/position and
facing requests,story write5,and the afterbattle child. The callback operand2 is concrete;[entry/setup recovery](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md)
now establishes its numeric index and [the one-unit configuration](HARBOR_TUTORIAL_CONFIGURATION.md).
Unit/weapon runtime data,loss/escape handling and tutorial overrides remain open.
Do not import provisional prototype HP/damage or ordinary Hammer availability.

Other motion/camera/input/sound/wait/inline/crowd commands in these spans are
not yet fully specified. The initial cutscene trigger/movement conditions,full
NPC and battle services,persistent operand resolution,defeat/reload behavior,
message branches/control tokens and original timing traces remain open. The
broader gor_00 init includes unrelated later-story cases;the seven inspected
spans do not constitute complete Rogueport content or full-game requirements.

Reproduce local metadata with rel_event_inventory.py naming the seven events
above,`--module gor --symbols artifacts/reference/us_symbols.csv --output`
an ignored reference path. Message resource hashes/offsets are reproducible with
message_resource_evidence.py. Preserve the ISO read-only and original assets
outside source history.

## First encounter trigger and message-branch refinement (2026-10-12)

Independent read-only re-extraction of the five scene streams above establishes
the following scoped predicates. This supplements the initial inventory; it
does not establish camera/movement/event timing equivalence or the complete
overworld interaction arbiter.

The story operand `0xf5de0180` is signed `-170000000`, exactly **GSW(0)** in
the [integer operand namespaces](../event/EVENT_INTEGER_OPERAND_NAMESPACES.md).
It reads/writes the global saved byte bank via `swByteGet(0)`/`swByteSet(0, value)`.
Its on-disk save ownership remains a separate requirement.

The `christine_osoware` main stream begins with `IF_EQUAL(LW0, 1)` at
`0x25e08`. The admitted resume block disables key/control, sets Mario to
`(0,15,250)` and direction arguments `(90,0,0)`, waits through `evt_seq_wait(2)`,
then enables control/key; `END_IF` is at `0x25e78`. Both fresh and resumed
streams then execute the same poll:

| Site | Original command/request |
| --- | --- |
| `0x25e7c` | DO literal 0 (endless loop) |
| `0x25e84` | `evt_mario_get_pos(0, LW0, LW1, LW2)` |
| `0x25e9c` | IF integer `LW2 <= -100` |
| `0x25ea8` | BREAK_LOOP |
| `0x25eac` | END_IF |
| `0x25eb0` | WAIT_FRAME(1) |
| `0x25eb8` | NEXT |

After admission the stream writes `0xf840601b = 1` at `0x25ebc`, which is
**GSF(1179)** (`-129998821 - (-130000000) = 1179`). It configures the
harbor NPC/sound requests, runs `christine_osoware_setup` as child at
`0x25f4c`, disables Mario key at `0x25f54`, then prints `mac_0_016` at
`0x25f60`. There is no A/X/B button or actor-proximity test in this poll.
After the six message calls through `mac_0_021`, it writes GSW(0)=4,
enables Mario key and spawns the beforebattle stream as already tabulated.

`gundan1000tai_beforebattle` has an independent LW0==1 resume block ending
at `0x26540`. It sets Mario to `(-220,15,90)` and restores actor positions:
Goombella `(55,15,-300)`, Crump `(105,15,-300)`, subordinate A
`(135,15,-330)`, subordinate B `(160,15,-330)`; facing requests are 90 for
Goombella and 270 for the other three. Its subsequent poll is:

| Site | Original command/request |
| --- | --- |
| `0x26544` | DO literal 0 |
| `0x2654c` | `evt_mario_get_pos(0, LW0, LW1, LW2)` |
| `0x26564` | IF integer `LW2 <= -100` |
| `0x26570` | IF integer `LW0 >= -25` (nested) |
| `0x2657c` | BREAK_LOOP |
| `0x26580` / `0x26584` | END_IF / END_IF |
| `0x26588` | WAIT_FRAME(1) |
| `0x26590` | NEXT |

Therefore both conditions must admit on the same position read. It then
requests environment off and disables Mario key at `0x265a0`; the three
beforebattle message calls and battle index2 start/wait follow. This poll
also has no A/X/B gate. See [loop opcodes](../event/EVENT_GOTO_AND_LOOP_OPCODES.md)
and [integer IF predicates](../event/EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md).

These are **integer predicates over float-writing locals**, not raw float
comparisons. The [position callback](../field/FIELD_SEGMENT_INTERSECTION.md)
writes 1/1024-quantized encoded floats; `evtGetValue` decodes them then uses
`fctiwz` (truncation toward zero). Thus, in the finite ordinary domain, decoded
X=-25.5 admits the second poll's X test (integer -25), whereas decoded Z=-99.5
fails both Z tests (integer -99). Preserve the conversion chain rather than
changing opcodes28/29 into float IF opcodes22/23. This is static composition,
not an observed original trace.

The 20 displayed payloads `mac_0_014` through `mac_0_033` contain no `<select>`
lexeme. The five streams contain no `evt_msg_select`/`evt_msg_continue` calls.
In particular the six `christine_osoware` messages and the three beforebattle
messages are unconditional in their main stream after their position polls;
they have no reply-choice branch. The prior table retains exact speaker
operands and order. This does not prove general message-system choice behavior
or make speaker0 an attributed NPC.

All 20 payloads use `<k>`. Original paging `<p>` occurs in 014/015/017/019/
023/024/025; all other payloads in this range have none. `<dynamic 3>` occurs
in 016/022/024/026/027/031; 024 also has `</dynamic>`. Scale tokens are 026
`<scale 2.2>`, 027 `<scale 2>`, 031 `<scale 2.5>`; 026 and 031 also contain
`<shake>`. Literal `<wait N>` counts/values and every original byte remain
authoritative in `msg/US/gor_00.txt`; these inventories do not authorize
discarding tokens, combining messages or rewriting text.

The exact distinct token lexemes across this range are `</dynamic>`,
`<dynamic 3>`, `<k>`, `<p>`, `<scale 2.2>`, `<scale 2.5>`, `<scale 2>`,
`<shake>`, `<wait 100>`, `<wait 200>` and `<wait 250>`. The 20 payloads
contain **zero bytes >=128**. This is a resource census, not a general
US character-encoding contract.

Story2's `enter_gorotsuki_town` disables key/control at `0x254d8`/`0x254e4`
and sets Mario to `(0,-1000,0)` at `0x254f0`. It enables control at
`0x25854`, then sets Mario to `(0,15,225)` at `0x25860` and requests
direction `(90,0,0)` at `0x25874`, before printing 014/015. There are no
later Mario position/movement commands in this annotated stream. At its end
it writes story3 at `0x25dbc`, requests
`N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos` at
`0x25dc8`, enables key at `0x25dd0` and spawns the story3 stream at
`0x25df8` with LW0=0. `(0,15,225)` is therefore the **last explicit position
request before key restoration**, not a runtime position trace or proof that
other services cannot move Mario. The story3 resume position `(0,15,250)`
belongs only to LW0==1 and must not overwrite this fresh intro position.

Validation: re-extracted five streams with `rel_event_inventory.py` to
`/tmp/harbor-dialogue-events.json`; direct ISO assertions checked 13 command
records, two `evt_mario_get_pos` imports/operand lists, all 20 ordered IDs,
the two resource SHA-256 values and absence of `<select>`; existing
`tools/verify_behavior_contracts.py` passes. Full key/control service semantics,
postbattle actor/crowd wait predicates, original execution and runtime fidelity
remain open. See [OW-INTERACT-01](../../requests/OW-INTERACT-01.md).
