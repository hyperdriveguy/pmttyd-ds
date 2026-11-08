# Original Crump opening integration requirements

This contract specifies the unfinished US G8ME01 disc0 revision0 harbor encounter,
from its field trigger through the first battle and the original postbattle
escape to Rogueport plaza. It records requirements and current implementation
boundaries as inspected on 2026-11-07. **The normal game cannot yet complete this
chain.** Available assets, portable services and fixture coverage do not establish
that the production owners execute them together.

Original dialogue, event words, actor identities, conditions and ordering remain
authoritative. No development battle, HP-based story advance, fabricated result,
replacement crowd or paraphrased dialogue satisfies this contract. Source
coordinates and simulation ticks must remain separate from rendering cadence.

## Original chain and required owners

| Requirement | Required behavior and authoritative evidence | Current production boundary |
| --- | --- | --- |
| CO-01: field entry | The original `gor_00_init_evt` selects story2 arrival, story3 encounter, story4 beforebattle or story5 afterbattle, with its original LW0 resume values and attached/independent event ownership. [Original bindings](STORY_HARBOR_OPENING_BINDINGS.md) retain init and event sites; [area initialization](../field/NATIVE_HARBOR_AREA_INITIALIZATION.md) retains the complete root. | FieldArea can import the original root, but its actual-manager fixture reaches the genuine first audio provider. Full normal Game activation and its transitive map/light/Bero/tag/audio services remain open. Normal Game uses separate staged `HarborStory` and `HarborEncounter` owners. |
| CO-02: encounter admission | Story3 polls integer-decoded Mario Z<=-100, sets GSF1179, creates/configures original actors, plays016..021 and writes story4. Story4 polls Z<=-100 AND X>=-25 on the same position read. Preserve Q1024 storage and truncation; X=-25.5 admits, Z=-99.5 does not. | `HarborStory` reproduces a staged subset of the recovered choreography; `HarborEncounter` executes original beforebattle words. This is not a single original field scheduler. |
| CO-03: beforebattle | Execute all 132 original commands at REL2639c, including LW0==1 restoration,022..024, independent subordinate moves, camera, actual control nesting, source stationary jump and conditional HP minimum5. | Original-data VM tests reach setup2 and block correctly. Movement/camera/landing are scoped native adapters. Environment, fukidashi and SFX are retained requests acknowledged without their complete presentation backends. |
| CO-04: actual field battle start | `evt_npc_set_battle_info(Crump,2)` at269dc precedes start269ec and wait269f8. Use the actual same-VM registered NPC, GP/F, F8 actor and F30 result; original stop/control/sequence requests must retain ownership through holds. | `HarborEncounter.battleServices` has no normal Game assignment. Its independent VM cannot borrow regular-event IDs from the FieldArea scheduler. Game observes `battleRequested` and starts `HarborTutorial` separately. [Same-VM binding](../field/NATIVE_FIELD_NPC_BATTLE_OWNERS.md) and [callback parents](../field/NATIVE_HARBOR_BATTLE_CALLBACKS.md) exist as composition APIs. |
| CO-05: setup2 sequence entry | Resolve numeric gor record2, consume original audience/group RNG, acquire actual stage/background/unit/message/menu/audio resources and run original battle sequence Init/Main and BattleInit, producing one actual BattleWork. | Normal main's OriginalSequence callback implements only field Exit80007390. It does not install `BattleSequenceBinding` or acknowledge battle Init. See [sequence binding](../battle/NATIVE_BATTLE_SEQUENCE_ENTRY_AND_FIELD_BINDING.md). |
| CO-06: tutorial battle control | Run original init/phase, legitimate first turn, command/menu/target/attack/action command, physical defense, damage/reaction, subsequent phases and terminal admission with the shared source BattleWork and clocks. | Normal Game now drives a bounded `NativeHarborGameplay` policy over the manual setup2 scene: physical command/menu input, selected player attack, feedback, installed Crump attack and subsequent source turn preparation. The renderer publishes class/weapon readiness after an actual submitted frame. Whole original btlseqMain/shared-BattleWork sequence integration, complete action effects, lethal-path services and result/end/export/field return remain open. |
| CO-07: result and teardown | Obtain result through original result producer, ending services and engine completion; export actual pouch/unit state, end resources/events, restore field/party/control/audio and perform original sequence return. The battle engine result must precede field wait completion. | `HarborTutorial::running()` returns `triggered`; there is no completed lifecycle predicate in this owner. `nativeGameplayConclusion` now runs original battleCheckConcluded/battleInitialEndResult and retains an optional numeric conclusion privately. It does not publish scene.result or complete End/field wait; result0 is not victory. Normal result/export/teardown/field-return remains open. [Result publication](../battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md), [engine completion](../battle/NATIVE_ENGINE_COMPLETION_AND_RETURN_SEQUENCE.md) and field callback contracts define separate required parents, not installed production completion. |
| CO-08: resumed beforebattle tail | Resume the SAME original source start/wait calls only after actual sequence/result conditions. Execute camera/actor restoration, story5 write26b24, source sailor get_reglid/DELETE_EVENT requests and attached afterbattle child26bbc. | Source words are in the broad harbor pack, but unbound actual field services prevent this tail. A second VM, synthetic current sequence2 or inferred result cannot release it. |
| CO-09: postbattle | Execute the complete 276-command `gundan1000tai_afterbattle` at REL26e84, including LW0==1 restoration, original crowd/motion/effects/party/camera/dialogue and independent event cancellation. | No normal afterbattle owner, callback catalog or playback tests exist. `tools/harbor_event_assets.py` catalogs only beforebattle callbacks. All REL sections in a pack do not make this second event executable. |
| CO-10: original exit | After033 and original waits, write GSW0=6 at280f8; BGMOff_f(512,2000), map-change fade(0,2,500,1,500), then `evt_bero_mapchange("gor_01","s_bero")` at28130. Admit the actual plaza incoming story6 path and retain party/save ownership. | No afterbattle execution reaches this transition. Game restore explicitly admits harbor story3/4, not an original story5 afterbattle resume. Existing generic map/party/fade APIs alone do not prove this exit. |

Original chain offsets above are **REL file offsets**, not loaded pointers.
The complete unchanged beforebattle SHA256 is
`4cc7e8fba986105d05f73b8c21bea7c79e4b69e1fae556f88f4920f77fd97bef`;
afterbattle is
`c055f9af15c96521318b2359e11a89664dafcca92556a9daefa6716ea531d5e7`.
Read-only fresh inventories are private
`artifacts/crump-opening-events-audit.json` and
`artifacts/crump-opening-afterbattle-command-audit.txt`.

## Dialogue and branches

Use unchanged `msg/US/gor_00.txt` payloads, IDs and control tokens. Its identified
59142-byte source hash and individual call sites are in
[original bindings](STORY_HARBOR_OPENING_BINDINGS.md). Actors G/C/A/B are source
keys14b28/14b40/14b5c/14b6c, not translated replacement names.

| Stream | Ordered message IDs | Source speaker |
| --- | --- | --- |
| Story3 | mac_0_016,017,018,019,020,021 | G,C,G,C,A,G |
| Beforebattle | mac_0_022,023,024 | G,C,C |
| Tutorial | mac_0_b_t1_000..005 subject to original phase/death conditions | Use original tutorial event operands; never print all six unconditionally |
| Afterbattle | mac_0_025,026,027,028,029,030,031,032,033 | C,C,C,G,G,C,C,C,C |

The field014..033 payloads contain no selection token and these five scene
streams have no msg_select/msg_continue call. This does not remove event-local
resume, loop, concurrent motion, crowd-work or area predicates. Preserve `<k>`,
`<p>`, dynamic, scale, shake and literal wait tokens. Beforebattle and afterbattle
LW0==1 branches restore source state; fresh entry must not execute their resume
placements. A story5 checkpoint must enter the original afterbattle resume path,
not replay a battle or unlock exploration without its source events.

## Resource availability and presentation prerequisites

| Dependency | Available original data / conversion | Still required |
| --- | --- | --- |
| Harbor map and objects | Original `m/gor_00/d`, converted `gor_00` geometry/textures/collision, editable texture manifest, source map light inputs and original full init data. | Actual map object animation, light bank/bindings, original draw state, camera and physical raster validation. Source init's four persistent ship-light events must share live map/light owners. |
| Field cast and sailors | Full original MODEL/TPL banks for c_pkr32 clips, c_kanbu_ibt59, c_zako_n28, c_gara16, c_suifu_b9:144 total. `romfs/harbor-native-npc-resources.json` and `field-native-<model>.draw-model.bin/.draw-textures.bin/.pmanim/.pose-directory.bin` are present. Crump texture owner is c_kanbu, source `a/c_kanbu-`. [Resource contract](../field/HARBOR_NATIVE_RESOURCES.md). | Source Entry/SetTribe/Main/pose/GPU plus same-VM tagged sailors and real regular-event retirement history. Normal cutscene renderer still has partial harbor-specific converted banks; complete raw-bank availability is not proof those draws consume them. |
| Mario field poses | Beforebattle `harbor-mario` pack has M_S_1/M_W_1/M_R_1/M_J_1B/M_J_1C and e_mario M_I_N/M_N_H. Full field-player e_mario manifest contains afterbattle M_N_7 and M_N_2. | Bind afterbattle pose requests273a8/27560 to the actual full player pose owner; these poses are absent from the beforebattle-only harbor pack. Do not substitute ordinary idle. |
| Tutorial cast/stage | Original setup2 selects btl_un_kanbu1; original battle Mario and helper NPC Goombella packs, stg_00_0 and bti_01 converted products are present. Renderer loads `harbor-tutorial-*`. | Complete pose coverage for reached source paths, correct field/battle ownership switch and return, real source camera/material/light/texture semantics. Existing static or partial converted packs do not establish original raster parity. |
| Raw texture helper | [Actual-VM texture parent](../display/NATIVE_ACTUAL_VM_TEXTURE_PARENT.md) now preserves the seven original arguments, repeated TPL/header reads, live matrix constants and original SDK composition. Its boot data is pinned from the ISO. | Actual TPL relocation/residency, GX object/TLUT/generator/load services and native complete-fan submission are still required before this becomes a production rendering path. |
| Messages/font/menu | Original global/gor_00 messages, source font, source menu/common texture pack and command palette are available. Loaders probe022..024 and all six tutorial IDs. | Native manual class/weapon menu activation and submitted-frame readiness are available. Afterbattle playback/anchors/tokens, complete source queued display/acknowledgment/input scheduling and action completion remain required. No loader currently probes025..033 as a required afterbattle catalog. |
| Music | Original danger stream `sound/stream/evt_kik4_32k.stm` converted to `harbor-danger.wav`; source-selected `BGM_CHUBOSS_BATTLE1` converted to `harbor-tutorial-music.wav`, both present with loop sidecars. Native main selects tutorial frontend track12. | Execute original BGM/environment transitions/fades at source service times; static track selection is not the complete audio request owner. |
| Positional SFX and effects | Original event retains sound keys/operands and fukidashi requests; source audio banks remain available read-only in ISO. | Prove extraction/admission/playback for each reached key, real position/update/off and overlap, and original effect art/lifetimes. A retained request acknowledged without a consumer is not presentation completion. |
| Crowd | Original afterbattle uses 40 cached c_zako_n snapshots: S_1 at phases 0,2,..38 and R_1 at phases 0,1,..19. The extdrv path draws 400 instances as quads; full original c_zako_n art already exists. dai3_animtbl at REL20b1c is a signed-halfword template-index sequence. | Bind original snapshot capture/cache and shared instance/matrix/visibility/sort lifetime, source 192-byte member work and 32-byte ext instances. No validated 400-member scene GPU/CPU budget or production crowd owner is established. Rendering 400 ordinary animated NPC owners would change the source architecture. |
| Party helper | Source afterbattle sets party slot1 kind13 at275b4 and requests direction/run, followed later by original plaza story flow. Full c_pkr resources exist. | Use actual constructor/control/appearance owner and auxiliary-kind semantics. Do not replace this with canonical party kind1 or infer permanent Goombella joining here. |

Original rights remain with Nintendo/Intelligent Systems. Existing provenance
and conversion choices are tracked in [ASSETS](../../ASSETS.md); this contract
adds no original asset bytes, extraction or redistribution grant. New imports
must use empty destinations, preserve edits and record per-asset provenance.

## Postbattle service closure

The fresh source inventory establishes concrete prerequisites beyond a dialogue
list. Afterbattle has nine message calls, five camera-set calls,32 fukidashi
calls and13 positional SFX calls. It additionally requires:

- nannpc Entry/SetAnimTbl/FlagOnOff/SetSize and GetWork, with original indexed
  initialization and later work-dependent loops. Creation loops until index400.
- Original local callbacks `gundan1000tai_init`, `gundan1000tai_totsugeki`,
  `gundan1000tai_areacheck` and `gundan1000tai_totsuend`; their names alone do not
  specify crowd movement, completion, collision or area predicates.
- Mario/NPC position/direction/pose/flag services; original NPC move-position,
  reverse-ry and no-hit jump; Mario/party position correction, party slot1
  constructor/direction/run; camera event-off and shake.
- Original independent and ID-returning inline contexts, delayed capture,
  retained IDs and DELETE_EVENT behavior. Do not flatten independent loops into
  sequential animation receipts or borrow IDs across schedulers.
- Source SFX_VOICE_MARIO_SURPRISED2, SFX_VOICE_MARIO_HAND_UP1_3,
  SFX_STG0_3RD_TOGETHER1_2, SFX_STG0_3RD_ATTACK1 and
  SFX_STG0_3RD_QUESTION_MARK1, plus source sound-off, BGM fade and map transition.

The complete argument lists and command order are preserved in the private
inventory, including original raw integer/float namespaces and relocations.
The current story binding document does not fully specify these crowd helpers;
request their complete local bodies, transitive resource identities and consumer
semantics through [DEMO-01](../../requests/DEMO-01.md),
[EVT-01](../../requests/EVT-01.md) and
[RESULT-01](../../requests/RESULT-01.md). A best-effort adapter must state its
provisional motion/numeric domain and preserve a replacement boundary; it must
not fabricate success at a missing result, lifecycle or source predicate.

## Required end-to-end proof

Completion requires evidence from the normal binary and actual owners, alongside
portable regressions. Existing `tests/harbor_story_tests.cpp` and
`tests/harbor_encounter_tests.cpp` cover staged story3, original beforebattle
admission/resume,022..024, HP/jump and a retained battle boundary. The latter
asserts story4, result0 and pending notices. Direct
`tests/harbor_tutorial_tests.cpp` service/menu/attack calls and synthetic provider
receipts cannot support an end-to-end completion claim.

1. Start a fresh normal new-file opening and reach the harbor through original
   dialogue and gameplay movement. Record source event identities, message IDs,
   speaker keys, source control branches and asset admission. Also test story3,
   story4 and story5 resume branches without contaminating the user's saves.
2. Trace actual beforebattle start/wait into the same field NPC/F/result owner,
   retained OriginalSequence Init/Main, one BattleWork and source setup2. Check
   original RNG consumption/history and each held service without replay.
3. Complete the original tutorial through legitimate physical command/target/
   action/defense input, source phases/damage/reaction/death, original result
   publication and the full ending/teardown/field sequence. Show that missing
   late providers retain the call and cannot publish a premature result/return.
4. Resume the original field command and all tail cleanup, including both
   sailors' real regular IDs. Execute every afterbattle command/loop/inline and
   025..033 with original speakers/control tokens and required crowd/effect/SFX
   presentation. Prove auxiliary party setup and the actual area predicate.
5. Reach source GSW0=6 and actual gor_01/s_bero incoming event, with correct
   resources, party/pouch/controls/camera/music and save eligibility. No direct
   story setter, alternate development battle or forced sequence write counts.
6. Run `make test` and `make all`, use isolated Azahar profiles for scene captures
   before/inside/after battle, inspect images and retained diagnostics, and
   compare to original Dolphin observations recorded in MECHANICS. Measure CPU,
   allocation/linear-memory/VRAM, draw/triangle/texture counts and frame cadence
   on New2DS XL hardware, especially the 400-crowd scene. Citro3D/3DS acceleration
   must be demonstrated without dropping source actors, timing or material state.

No full-scene or hardware performance evidence was produced by this read-only
requirements audit. The goal remains unfinished until these requirements are
implemented and verified at their stated production scope.


### Crowd source evidence delivered during this audit

Private `artifacts/crump-crowd-resource-prerequisites.txt` and companion body/
resource manifests now identify the original 40 cached snapshots, both template
index sequences, 192-byte field member and 32-byte ext-instance layouts, source
first-call yield/allocation behavior, four RNG requests per initialized member,
and exact local gundan callbacks. Source snapshot rendering is original art,
not a placeholder. Raw source allocation arithmetic totals 124,532 bytes before
alignment, capture textures and cache allocation; it is not a native memory or
VRAM measurement. Original areacheck has a literal unreachable X<30 branch after
its earlier X predicates; preserve that order and the actual Y-offset reads.
Remaining research includes snapshot camera/copy/filter/alpha state and smart
texture cache ownership, complete ext ordering/matrix/sort dependencies, and
executed reference observations. The new data does not make production crowd
simulation, draw, cancellation or completion available automatically.


### Available effect art and recovered display lifecycles

[Original fukidashi effects](../effect/CRUMP_FUKIDASHI_EFFECTS.md) now specify all
33 scene requests, constructor cues, attachment/driver lifetime and complete
source matrix/FIFO/material order. All four original textures4..7 are required;
operand4 is not a background-disable flag. Unscaled editable PNGs and the normal
256x128 atlas overlay are available under `assets/original/effects/fukidashi/`.
Native dispatch/material/audio/display and retirement remain requirements.

[Crowd snapshot lifecycle](../display/CRUMP_CROWD_SNAPSHOT_LIFECYCLE.md) recovers
actual camera0 capture, three source passes, RGB5A3 copy, stable movable smart
headers, ext matrices/sort/preload and reset. Snapshot camera/filter/EFB
inheritance and original observations remain open. Tag1 release ownership
is verified in field-map `_unload`: `extReset` precedes both heap clears and
`smartAutoFree(1)`; native execution and draining remain unproven. Neither new contract proves production afterbattle completion.


### Portable effect update available

The original seven-kind fukidashi Main and its original animation/constant pack
are now implemented and host tested; see the
[effect implementation boundary](../effect/CRUMP_FUKIDASHI_EFFECTS.md#portable-main-implementation-2026-11-07).
This supplies one reusable dependency for CO-03/CO-09. Actual event construction,
actor attachment, GPU draw, constructor sound and pool/display retirement are
still required. No change to the uncompleted CO-04..CO-10 production chain is
claimed by this adapter.


### Effect construction available; scalar requirements corrected

The original constructor now admits a real pool record and heap3 payload,
retains position/actor/audio children and source constructor cues. The
[effect contract](../effect/CRUMP_FUKIDASHI_EFFECTS.md#portable-constructor-and-corrected-scalar-evidence-2026-11-07)
corrects prior scalar-address errors: stored Y+3; position angle subtract20;
auxiliary FMA(0.5,radius/12,0.5); kind6 spin begins-450. This supplies another
CO-03/CO-09 dependency. Actual EVT routing, attachment math, native drawing,
sound and resource retirement still need production composition. These
changes do not close the battle/result/field-return chain or afterbattle.


### Original attachment calculation available

The [effect position implementation](../effect/CRUMP_FUKIDASHI_EFFECTS.md#portable-attachment-calculation-2026-11-07)
now supplies complete calc_pos with original initialized data and retained
camera/SDK/actor ownership. Actual constructor-to-position composition is host
tested using the explicit finite native SDK adapter; source yaw, normalization,
FMA and live-read boundaries are preserved. This advances CO-03/CO-09 without
closing actual EVT/draw/audio/retirement or the complete Crump battle/result/
field-return/afterbattle production chain.


The retained whole fukidashi event wrapper is now available in
`fukidashi_event.hpp/.cpp`, with all modes and an actual caller-VM decoder/memory
adapter. It composes with the committed Entry/Position/Main services and original
six-span wrapper data pack. This advances CO-03's prerequisites but does not
change its production boundary: the normal reaction callback still needs real
shared-process effect/heap/camera/actor/audio/draw providers. Test constructors
and successful source decoding do not establish that scene presentation.


The [native effect process](../effect/NATIVE_EFFECT_PROCESS.md) now supplies the
original source bank/heap/manager/cache owner in normal Game startup and retains
it across room/checkpoint transitions. This advances CO-03 prerequisites. Actual
reaction EVT binding, primary textures, callback updates, draw/audio consumers
and retirement remain required; the full into-battle and out-of-battle story
chain is still open. Startup and fixture success do not close that acceptance.


[Whole primary/N64 effect texture setup](../effect/NATIVE_EFFECT_TEXTURE_SETUP.md)
is now available on the actual shared native process with original banks, real
source heap allocations and independent DVD/unpack/completion ownership. It
includes the mandatory N64 child. Normal original sequence activation, reaction
EVT/Main/draw/audio binding and full battle/result/return remain open. Raw bank
availability and callback fixture success do not close CO-03/CO-09 presentation.


[Original environment startup](../display/SOURCE_ENVIRONMENT_TEXTURE_STARTUP.md)
is now available over the same actual VM/heap as effect textures. Separate envInit,
source setup and real DVD/unpack/callback completion preserve the original phase,
read length and readiness rules. This closes another resource-owner prerequisite;
normal source activation still needs the preceding fade owner, and actual reaction
EVT/Main/draw/audio plus full battle/result/return remain unverified.
