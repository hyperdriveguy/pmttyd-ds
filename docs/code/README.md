# Port code guide

This directory explains implementation choices and how to read the port's code.
Recovered original behavior and its evidence remain in
[behavior contracts](../behavior/README.md); conversion/provenance remain in
[assets](../ASSETS.md). Start with the [source map](../../source/README.md),
[header map](../../include/README.md) and [architecture](../ARCHITECTURE.md).

| Guide | Implementation covered |
| --- | --- |
| [Audio services](audio-services.md) | Sound slots/handles, pending starts, fades, fanfares and opening adapters |
| [Wander/animation services](wander-animation-services.md) | Event continuations, cold pools/tangents and recursive scratch recovery |
| [Runtime ownership](runtime.md) | Native main loop, physical input, audio and frontend |
| [Game composition](game-composition.md) | Room collision, units, tick order, restore and follower ownership |
| [Checkpoint bytes](save-format.md) | Native save encoder, decoder and file replacement |
| [Trio public API](trio-api.md) | Fresh/reused owner policy, action readiness, borrowed projections and outer Main |
| [Trio draw bindings](trio-draw-bindings.md) | Exact callback admission, read routing, nested draw precedence and material ownership |
| [Trio scheduling](trio-scheduling.md) | Legacy turn/handoff, command pads, retained Main and camera/outer draw boundaries |
| [Trio display prefix](trio-display-prefix.md) | Unit/part mutation, pose/effect admission, depth and retained footer registration |
| [Trio events/entry/damage](trio-events.md) | Callback domains, packed initialization, damage/motion and actual event barriers |
| [Trio ending](trio-ending.md) | Reward counters/icons, source end-bank states, audio and partial field restoration |
| [Trio commands](trio-commands.md) | Native rows/registry mapping, candidate projection and retained action-manager phases |
| [Trio providers](trio-providers.md) | Partial runtime audit: assets, HP ownership, pose/shadow, stage and audience composition |
| [Harbor composition](harbor-composition.md) | Original catalog, actor identities, entry/event ownership and action/display boundaries |
| [Harbor packed state](harbor-packed-state.md) | Native guest arenas, packed fields, class/weapon tables and command-service admission |
| [Harbor service adapters](harbor-service-adapters.md) | Native damage admission, aftereffect preview, AP report and retained item/menu ownership |
| [Damage order](battle-damage-order.md) | Typed calculator modifiers, synchronous hit/counter flow and Dive arithmetic |
| [Retained battle owners](battle-retained-owners.md) | Main/sequence/stage continuations and shared field-sequence facade admission |
| [Audience drawing/touch](battle-audience-draw.md) | Immediate mutation, retained callback limits, auxiliary icons and completed-pass touch proof |
| [Part/HP presentation](battle-part-presentation.md) | Matrices/material/history, HP reloads/gauges, projection and stock EXP |
| [Reports/resources](battle-reports-resources.md) | AP/drop ordering, present recovery, menu files, curtain, status window and Majinai bytes |
| [Audience control](battle-audience.md) | Source work/population, actor reactions, target/window numerics and resource admission |
| [Audience items/identity](battle-audience-items.md) | Held/flight state, original query, lifecycle provenance and selected interruptions |
| [Audience sound](battle-audience-sound.md) | Voice records, count buckets, two envelope stages and throw lanes |
| [Action UI/ring](battle-action-ui.md) | Selection, UI allocation, row states and retained stack/icon ordering |
| [Class raster/footer](battle-class-raster.md) | Source constant bank, circle/bubble GX calls, buttons, pinch and queue/animation continuations |
| [Battle stage](battle-stage.md) | Packed stage/floor, map-object caches and spotlight ownership |
| [Battle camera](battle-camera.md) | Priority requests, framing modes, source numerics and tremor continuation |
| [Timing/reel drawing](battle-feedback-draw.md) | TimingA display and break-slot storage, idle limits and retained draw ordering |
| [Battle commands](battle-commands.md) | Input states, action callbacks, attack publication, feedback, command poses and power |
| [Weapon menus](battle-weapon-menus.md) | Class/weapon row layouts, eligibility, target decision and retained audience selection |
| [Typed battle support](typed-battle-support.md) | Semantic movement/position, candidates, facing and target-confirmation order |
| [Packed battle state](packed-battle-state.md) | Status bytes, menu/candidate records, prechecks, counters and shared recovery |
| [Packed battle motion](packed-battle-motion.md) | Position/part/hit aliases, movement, sound records, jumps and takeoff |
| [Battle controllers](battle-control.md) | Outer lifecycle, phase tables, conclusion and field handoff |
| [Battle events/rules](battle-events-rules.md) | Callback domains, child events, rules, payment, aftereffects and hit snapshots |
| [Battle entry/startup](battle-entry.md) | Packed actors, construction, pouch/resources, poses, events and sequence visits |
| [Battle core](battle-core.md) | Action commands, private input, status, resources and damage accounting |
| [Pouch/selection](pouch-selection.md) | Packed key operations, revision, source row/window sequencing and borrowed text |
| [Field session](field-session.md) | Setup/deletion preflight, actor cursors and shared pose aliases |
| [Trouble Center](trouble-center.md) | Saved-state knowledge, row order, original board VM and split UI visits |
| [Field items/encounters](field-items.md) | Item/coin pickup, borrowed windows, audio envelopes, quest delivery and packed battle information |
| [Map NPC/area lifetime](map-lifecycle.md) | Parsed tags, history/skip masks, stable registry aliases and ordered room retirement |
| [NPC/partner/object owners](npc-objects.md) | Packed registration/setup, CPU poses, talk/paper state, save blocks and map matrices |
| [Field entry/event leaves](field-entry.md) | Bero metadata, position/control callbacks, cases, map flags and door-record ownership |
| [Field motion/contact](field-motion.md) | Units, jump/idle timing, wall/NPC query order, camera clocks and entry bridges |
| [Draw pipeline](draw-pipeline.md) | MODEL/TPL records, suspended traversal, material/shadow ordering and batch construction |
| [Retained display state](display-state.md) | Logical publication, GX numeric domains, icon/letterbox continuation and replay |
| [Battle/item/window adapters](render-adapters.md) | Provider holds, icon residency, GPU declarations, UI transforms and second views |
| [Opening/frontend scenes](scene-rendering.md) | Capture completion, paper UVs, scene residency and ship handoff |
| [Native renderer](renderer.md) | GPU buffers, pack flags, residency, field masks, text reflow and frame boundaries |
| [Display services](display-services.md) | Camera/queue/shadow continuations and authored road numerics |
| [Animation data](animation-data.md) | Pose/cache/surface formats and map upload helpers |
| [Event VM](event-vm.md) | Scheduler, operand tags, opcodes, address mappings and allocator services |

## Comment conventions

Explain units, ownership, valid domains and reasons for unusual ordering at the
code that needs them. Give repeated policy values descriptive constants/enums
when that improves the API; a nearby comment/table is sufficient for binary
layout offsets, original opcode numbers and recovered masks. Preserve source
numeric ordering and bit patterns when documenting recovered routines.

Distinguish original constants from native presentation/scheduling policy and
provisional adapters. An original address is an emulated 32-bit token, not a
host pointer. Keep unidentified fields as `field<offset>` and explain the known
read/write behavior; do not invent a semantic name. Explain why a hold resumes
at a particular operation, what state survives, and which provider is missing.
Avoid comments that merely repeat the C++ statement.

## Documentation audit status

The current pass covers runtime main/input/save, the event VM implementation,
core battle action/status/resource/damage helpers, and Game/portable runtime
helper composition, portable display/camera/animation helpers, and main renderer/
platform resource ownership and current scene-specific render adapters.
Retained camera/display/icon/letterbox helpers are also covered. Audio/frontend
service notes are included in the runtime guide. Animation/material/shadow and
display batch and ordinary field motion/contact helpers now have inline notes.
Bero entry metadata, case/map overlays and position/preparation leaves are covered.
Packed NPC entry/setup, catalog/talk/paper and typed partner/object helpers are covered.
Map-NPC parsing/history/bootstrap and retained registry/area lifetime are covered.
Finite item/coin/Goomba delivery and field battle-information helpers are covered.
Trouble Center state/menu/window and board/counter event adapters are covered.
Committed setup/actor leaves, shared field pose integration and pouch/selection helpers are covered.
Packed battle actor entry/setup, resource/pouch leaves and startup controllers are covered.
Packed wander events, typed wandering, cold animation/scratch parents and audio helpers are covered.
Outer battle sequence/phase/return controllers, rules, costs and selected EVT services are covered.
Packed battle status/part/position, candidates, precheck/counter/recovery, movement and jump helpers are covered.
Typed movement/position/selection, candidate support and facing/target-confirmation helpers are covered.
Command input/action help/feedback, attack registration/selection, weapon/class tables and weapon UI are covered.
Stage/floor/object/spotlight, camera requests/framing and TimingA/break-slot display helpers are covered.
Action menu/UI/ring and immediate/retained class draw/raster/footer helpers are covered.
Audience initialization/population/control, actor reactions, item/selection lifecycles and sound helpers are covered.
Audience draw/callback/touch, AP/drop/present, ordinary part/HP and small battle resource/state leaves are covered.
Typed damage and synchronous orchestration, Dive, retained Main/stage and sequence facade owners are covered.
Harbor entry/catalog, packed actor/menu/command composition and action/display adapters are covered.
Selected Harbor damage/aftereffect/AP/audience/item/weapon-input adapters are covered.
Trio native command selection and retained action-manager helper are covered.
Trio established runtime/header/facade/draw/material bindings are annotated on their recorded baselines. Focused guides below cover subsequent menu and Turn/Phase/Move/Act provider composition; newer cadence/pinch/sequence/pose/public draw changes still need review. The runtime remains partial.
Remaining field/NPC/partner modules, battle, message, inventory, story, effect
and port tooling still
require a systematic inline numeric/behavior audit. Concurrent additions to
already covered modules also need review;
existing comments and contracts in those areas do not imply complete coverage.
Progress and exact validation are recorded in [PROGRESS](../PROGRESS.md).

Retained audience target/turn-end numeric and provider contracts are described in
[audience continuations](battle-retained-audience.md). Packed threshold flags,
alert selection/countdown and recovery setters are described in
[pinch and recovery](battle-pinch-recovery.md). The [Turn parent](battle-turn.md), [event barriers](battle-event-barriers.md) and
[audience attack events](battle-audience-attack-events.md) now have inline numeric/
continuation notes. Newer Trio integration still requires its separate audit.

[Message resources](message-resources.md), [parser/playback and selection](message-playback.md)
and [message window/font/frame drawing](message-window-drawing.md) explain native
formats, original token retention, finite consumer domains and presentation adapters.

[Source file services](file-services.md), [native file backing](native-file-cache.md)
and [save-block services](save-block-services.md) explain reference/relocation
semantics, borrowed VM views and ticketed SD handoff for original event data.

[Event addressing](event-addressing.md) and [event service APIs](event-service-api.md)
cover live packed aliases, scoped raw RAM, header contracts, result protocols and
separate event/storage/heap lifetimes.

[Opening demo/book](opening-demo-book.md) and [title/audio/effects](opening-title-effects.md)
explain original narration identities, capture/final-pose admission, bounded scene
requests, native clock/framing adapters and independent fade/effect owners.

[New Game sequence/house](opening-sequence-house.md), [Voyage/arrival](opening-voyage-arrival.md)
and [map/heading/wake](opening-map-heading-wake.md) explain original IDs, source
request gates, scene/audio ownership and explicit native presentation adapters.

[Harbor dialogue/events](harbor-story-events.md) and
[professor/underground events](professor-story-events.md) document the remaining
story controllers, source callback catalogs and native service boundaries.

[NPC clocks/matrices/numerics](npc-clocks-matrices-numerics.md) explains packed
clock fields, retained draw transforms, blur polynomials and scalar length graphs.

[NPC Main entry/movement](npc-main-entry-movement.md) explains source traversal,
movement/pose ordering, shared VM binding and composed cursor retirement.

[NPC detection/display/tail](npc-main-detection-display-tail.md) covers trigger
priority, eligibility, deferred registrations, raw history and original field ownership.

[Queued NPC drawing](npc-queued-drawing.md) and
[party removal ownership](party-removal-ownership.md) explain callback pass routing,
material/GPU state, source removal requests and native retirement leases.

[Party construction/events](party-construction-events.md) and
[Maya effect animation](effect-maya-animation.md) explain constructor forms,
publication gates, source clocks, quantized curves and transform ordering.

[Effect pool/HitN64](effect-pool-hit-n64.md) explains record predicates, simulation,
source draw requests, texture ownership and ordered GPU publication/submission.

[Recovery effects](recovery-effects.md) explains constructor normalization,
catch-up/state ordering, source callbacks, atomic geometry and immutable resources.

[Runtime bridges](runtime-bridges.md) explains retained outer Main/sequence state,
shared GP input bytes, native field-position/quest providers and HitN64 shader units.

[Native field admission](native-field-admission.md) explains animation adoption,
party bindings, retained key predicates and static MODEL checks.
[Source font loading](source-font-loading.md) explains lazy setup, original width
mutations, VM aliases and readable-prefix versus reserved allocation ownership.

[Font GX pipeline](font-gx-pipeline.md) explains late DrawStart captures, exact
ordinary state and native glyph replay. [Weapon/window bridges](weapon-window-bridges.md)
explains short camera epochs, long submission ownership, source FIFO and atlas leases.

[Retained class/weapon menus](battle-menu-continuations.md),
[command input](battle-command-input-continuation.md),
[Phase/Move](battle-phase-move-continuation.md) and
[weapon subicons](weapon-subicon-continuation.md) explain source row layouts,
private-output knowledge, invocation cadence and precise child/read frontiers.

[Act sequence](battle-act-sequence.md) and [ActMenu/class UI](battle-act-menu-continuation.md)
explain retained source PCs, event/status/count paths, menu subroutines, distinct
allocation layouts and audience admission.

[Queued weapon drawing](queued-weapon-draw.md) explains private slots, source
scroll/state/math and ordered child frontiers. [Native menu support](native-menu-support.md)
explains audience queries, physical X, immutable original labels, pose-copy admission
and retained PresentItem recovery/effect ownership.

[Trio menu bindings](trio-menu-bindings.md) explains runtime provider composition,
zero versus pending, shared map identities, original message ownership and
separate Input-state/event domains. The newer Trio integration audit stays partial.

[Trio sequence bindings](trio-sequence-bindings.md) explains retained Turn/Phase/
Move/Act composition, typed returns, packed offsets, event domains and numeric
admission. Newer runtime integrations still need separate review.

[Trio cadence](trio-cadence.md) documents retained pinch/action/sequence dispatch,
input snapshots and distinct native clock gates. [Trio menu frame bindings](trio-menu-frame-bindings.md)
documents actual allocation/resource/camera proofs, facade lifetime and exact
queue callback admission. Remaining runtime changes still require review.
