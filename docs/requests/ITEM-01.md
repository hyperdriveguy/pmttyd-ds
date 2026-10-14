# ITEM-01 — Field inventory/pouch and field item use

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)) and the
full-game end goal. The slice is explicitly requested; no independent
research-agent ownership or completion claim is made.

## Open todos

- [ ] The original field inventory/pouch record layout, item slots and counts, as
  distinct from the battle pouch already documented.
- [ ] Field item use: which items are usable in the field, their effects and
  consumption, the full-HP/eligibility guards, and the menu/targeting flow.
- [ ] Where field and battle pouch state share or copy, and how the save block
  stores the field inventory.
- [ ] Item acquisition at the opening stage (pickup/chest/shop) and the key-item
  (key-item count) semantics the NPC gates already read.

- [ ] Complete native item95 full-storage/discard providers: recover shared
  failure-window definition/callback and discard selection, capacity/return,
  cancellation and cleanup conditions with source IDs/control tokens. The fresh
  native acquisition currently refuses full key storage before admission.
- [ ] Original item pickup/get callers in additional areas: catalog source
  setup/entry conditions, model/icon/resources and group0 BGM selection so the
  retained provider can be extended without guessed area behavior.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone + full-game end goal: the native field item menu is a fixture.
`include/runtime/game.hpp` carries `int hp = 10, mushrooms = 1;` and
`enum class Menu { Status, Map, Items, Settings }`; `source/runtime/game.cpp` heals from a
counter (`else if (menu==Menu::Items && progress.mushrooms>0 && progress.hp<10) {
--progress.mushrooms; progress.hp=std::min(10,progress.hp+5); ... }`) and refuses
at full HP, matching the README's "a mushroom cannot be used at full HP" note.
The battle pouch is separately recovered (`source/battle/battle_packed_pouch.cpp`:
0x5d4-byte pouch with 121/20/32/200/200 counts, seven 14-byte partner records;
`source/battle/battle_damage.cpp` item151 revival check), and
`progress.originalPouch->keyItemCount(95/107)` is read by field NPC gates, but the
original field inventory — item definitions, slot/count layout, field-use effects
and guards, the menu flow and save storage — is unrecovered, so the demo's items
and every later chapter's inventory are limited to a mushroom counter.
[DMG-01](DMG-01.md) and the weapon-table deliveries cover battle item rows, not
field inventory state or use. Please recover the original field inventory/pouch
and field item-use contract. No lock reserved.

## Answers and delivery notes (append-only history)

Delivered [pouch item dispatch](../behavior/battle/POUCH_GET_ITEM_DISPATCH.md)
(2026-10-12, no lock, static artifacts only): central receive entry with
verified type-id routing (coins cap 999, capped global counter, HP-ish and
flag-byte leaves, badge subroutine for 240–338, key insert over 121 slots),
slot bases +0xA0/+0x192 and 10/20 capacity cross-checked against the snapshot
`PouchData` layout, single shared pouch pointer (no separate battle pouch at
this layer), and 1/0 store/miss returns with memmove compaction. Effects,
consumption, save format, shop/chest/pickup callers, and the 0x800858fc +
0x800d4ed4 + 0x801410e8 callees remain open. No native/test/CMake/asset
change by this answer.

### 2026-10-13 — Goomba quest13 native binding and upstream recovery

Delivered [native quest13 hand-in](../behavior/field/NATIVE_GOOMBA4_QUEST13_HAND_IN.md),
[native pouch checkpoint v8](../behavior/format/NATIVE_POUCH_CHECKPOINT_V8.md),
and [Trouble Center writers/acquisition](../behavior/field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
First/continuing source dialogue, duplicate item selector, original Mario poses,
5407 write and checkpoint persistence are bound for explicit known state.
Native Trouble Center acceptance, quest2 item-entry/GetItem presentation and
quest13 acquisition remain pending; parent request stays open.

### 2026-10-13 — Plaza Goomba3 and original task-service recovery

Delivered [Goomba3 setup/talk](../behavior/field/GOOMBA3_SOURCE_SETUP_AND_TALK.md),
[item-get/coin frontiers](../behavior/field/GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md),
and [native binding/services](../behavior/field/NATIVE_GOOMBA3_TALK_AND_TROUBLE_SERVICES.md).
Known nonquest2 ordinary talk and quest2 waiting branch bind unchanged US text.
Portable task start/cancel preserve source17-flag/six-byte resets and low-level
all-copy removal for recovered task2/13 tables. Full acquisition/itemseq, coin
acknowledgment, task menu filtering/callers, switch persistence and Goomba3
side/gravity wander remain pending. Parent request remains open.

### 2026-10-13 — Goomba3 source map-side movement delivered and bound

Delivered [original NPC map-side/zero-vertical contract](../behavior/field/FIELD_NPC_MAP_SIDE_MOVEMENT_AND_GOOMBA3_ZERO_VERTICAL.md)
and [native map-side/wander binding](../behavior/field/NATIVE_NPC_MAP_SIDE_AND_GOOMBA3_WANDER.md).
Goomba3 now uses original100/30 shared wander, three ordered null-filter rays,
half slide, first-contact publication before second pass and source zero-vertical
no-op. No invented40000000, floor snap or gravity. Original SDK/ray traversal,
global scheduler/RNG and later vertical/reflection/filter domains remain open.
Full item95 get-item sequence recovery continues; parent request remains open.

Delivered [item95 normal get sequence](../behavior/field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md), including entry mode16/runtime flag distinction, deferred grant, original windows and acknowledgment/cleanup. Native acquisition/presentation and full-storage UI remain pending.

### 2026-10-13 — Portable original item95 receive and source presentation providers

Delivered [portable receive owner](../behavior/field/PORTABLE_ITEM95_RECEIVE_SEQUENCE.md),
[exact packed insertion](../behavior/unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md),
[original windows/pose/icon](../behavior/field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md)
and [type6 effect bank/update/draw](../behavior/effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).
Actual low-level95 insertion and receive service order are implemented/tested;
original windows precede grant, acknowledgment precedes cleanup, failure holds
at actual discard sub5. No task completion or native presentation is fabricated.
Corrected the legacy pouchGetItem pointer-lookup stub. Native window/text/effect/
audio/registry/control bindings, discard continuation and upstream quest callers
remain open; parent request stays open.

2026-10-13 follow-up: original US first-byte article/template expansion and
window acknowledgment/close gates recovered in
[message contract](../behavior/message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md).
Portable text expansion and source effect6 clock/two-pass draw plan implemented;
original M_I_U and actual base-bank42/43/44 packs prepared with native lazy pose
bank binding. [Native scope](../behavior/display/NATIVE_ITEM_GET_VISUAL_RESOURCES.md).
Quad correction is64x32; startup SDA2 color is zero within the bounded fresh
source state, with indirect/REL writers still unresolved. Native windows,
GX-to-PICA effect raster, camera/audio/control/name-registry integration and
failed-receive discard continuation remain open. Do not enable acquisition or
claim complete item-get fidelity on the strength of these resource providers.

2026-10-13 follow-up: finite original item95 name/help manager, US direct-color
font consumer and native window renderer implemented; source definitions,
frame-before-resize ordering, help prior-display publication, button-repeat
paging and AutoDelete parameter lifetime retain original ownership.
[Native window scope](../behavior/display/NATIVE_ITEM95_WINDOWS.md) and
[source display/color review](../behavior/message/ITEM95_GET_WINDOW_DISPLAY_ORDER_AND_FONT_COLORS.md).
Original color streams compile at preparation, leaving raw tokens retained;
rendering reuses existing font/frame resources and batches glyphs through C3D.
Global manager slot sharing, failure/discard window, native item/effect/audio/
control integration and original outer cadence remain open. This provider alone
does not enable quest acquisition; the parent request remains open.

2026-10-13 follow-up: read-only source recovery pins projected image44 POS
coordinates, direct image42 UV, distinct repeat/mirror wrapping, exact draw
state and actual camera5/class2 dispatch. Camera5 copies camera4 eye/up/target
but owns lens/clip/depth and excludes camera4 final zoom correction.
[Source effect](../behavior/effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md),
[camera producer](../behavior/camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md),
[native shader/camera scope](../behavior/display/NATIVE_ITEM_GET_EFFECT6.md).
Portable projected matrix and finite camera5 frame implemented; native PICA
consumes original masks with two passes, explicit projection/viewport, no render
clock or tint bake. Source camera4 prepared-vector supply/display sorting,
icon307 binding, original audio/control/name teardown and native quest acquisition
remain open; full-storage discard remains pending. Parent request stays open.

### 2026-10-14 — Original item95 name registry, icon and BGM query

Implemented independent32-slot field-item/icon ownership, source stale-name and
active-duplicate behavior, force/poll/name-wide deletion and the second active
check before icon synchronization. The original parcel307/TPL312 now has a
bounded camera5 PICA consumer with shared textures and source alpha/depth states.
See [entry/icon contract](../behavior/field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md),
[map blend/shadow admission](../behavior/display/ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md),
[native consumer](../behavior/display/NATIVE_ITEM95_REGISTRY_AND_ICON.md), and
[audio routing/query](../behavior/audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).
The portable get owner now accepts the raw source `psndBGMChk` result (-1 vacant,
0 assigned), with a test for assigned-to-vacant resume and once-only restoration.
Isolated native graphics captures show original parcel/name/help/effect and
teardown at60FPS; they do not run an actual acquisition or grant95/write5405.
Native owner dispatch/control/audio, original stream retirement, source camera4
queues/shadows, deferred icon metadata and map40 material remain open. Parent
requests remain open; do not substitute a guessed fanfare timer or missing service.

### 2026-10-14 — Fanfare loop and retirement resolved

[Original container-loop/retirement contract](../behavior/audio/FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md)
resolves the prior DSP-header ambiguity: the key-item fanfare plays once from
COMMON restartffffffff. Producer end publishes stop, stream main clears flags0,
then BGM main publishes group1 vacancy. All146 original stream headers were
surveyed (111 loops/35 one-shots); converter now follows these container markers,
validates frame offsets/extents and removes stale loop sidecars. Original fanfare
PCM was checked at stereo32000Hz/83888 frames with no loop sidecar.
No synthetic duration timer is needed. Native original audio routing, retained
field-music attenuation/resume and actual stream retirement still need binding;
parent acquisition/service requests remain open.

### 2026-10-14 — Original native key-item fanfare provider

Pack now includes original key-item fanfare PCM/provenance, preserving the
source COMMON one-shot policy. Native fresh-group playback shares the existing
three streaming slots, publishes source stereo gain before queueing and exposes
-1 vacant/0 assigned after actual backend drain, with bounded duplicate/full-pool
admission. Explicit backend release clears buffers without claiming sourceOff201.
[Native scope](../behavior/audio/NATIVE_KEY_ITEM_FANFARE_PROVIDER.md) and
[source gain/fade recovery](../behavior/audio/FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md)
record boundaries. Original Off201 fade/detached100ms tail and group0 retained-
cursor attenuation/resume remain native bindings; full Goomba3 pickup event and
item-main/control/motion/window/pouch/5405 ownership still need integration.
Parent requests remain open.

### 2026-10-14 — Original audio envelopes, tail and retained field resume

Portable original primary Off201/secondary3800/A0 counters and clean floating
SoundSS tail now have native fanfare/field-stream consumers. Logical group vacancy
is separate from retained physical tail; per-invocation volume publication precedes
counter updates. On_f_d synchronous BGMMain and group0-before1 order are retained.
Native original-mode test preserved field producer position90112 across NULLresume,
with silent8→false and successful fanfare fade/tail reuse. Both code steps passed
211 CTests/135 Python tests and native builds.
[Control/provider scope](../behavior/audio/PORTABLE_FIELD_BGM_ENVELOPES_AND_NATIVE_ITEM_AUDIO.md),
[secondary/tail boundaries](../behavior/audio/FIELD_BGM_SECONDARY_BOUNDARIES_AND_DETACHED_TAIL.md),
[plaza row67](../behavior/audio/ROGUEPORT_PLAZA_INIT_BGM_AND_ITEMGET_RESUME_OWNER.md),
and [source global percentage](../behavior/audio/FIELD_BGM_GLOBAL_PERCENTAGE_INITIALIZATION_AND_SLEW.md).
Full source sound flags/arbitration/global slew, source device scheduling and
physical performance remain unbound/unverified. Native event/Mario/window/item-main
and once-only pouch/5405 acquisition integration are next; parent requests stay open.

### 2026-10-14 — Portable Goomba3 acquisition event ownership

Committed `3b71d849`: original `irai_02_00` printing now has a separate
portable event/item owner, actual pouch receive and name-wide registry cleanup.
The event writes5405 only on a later poll after item cleanup. Duplicate-name
initial XYZ and full key-storage pending behavior are tested. Isolated snapshot
passed225 CTests/141 Python tests and native build; shared working-tree test
failed a separate battle pose assertion. See
[owner/validation](../behavior/field/GOOMBA3_PORTABLE_ACQUISITION_OWNER.md).
Native Game/NPC, deferred Mario motion15, camera/control/window/effect/audio
binding and full discard UI remain pending. Parent requests remain open.

### 2026-10-14 — Game NPC acquisition and deferred motion contract

Committed `4bd12c2f`: Game owns the item registry/acquisition, admits original
Goomba3 quest2 through registered providers and advances event/Mario/NPC/item
visits separately. Dialogue/key hold survives message completion and item
cleanup; the later event poll writes5405 and releases the talk wait. Active
copied/moved acquisition snapshots cannot advance retained source pointers.
Actual Game fixtures cover missing callbacks, print/grant/cleanup/5405 and
copies in both printing and getting. Isolated225 C++/141 Python tests and
native build passed. [Game scope](../behavior/field/GOOMBA3_GAME_ACQUISITION_BINDING.md).
Recovered [motion15/stay](../behavior/field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md)
pins pending entry, heading, front M_I_U and control-off dispatch gate.
Native main/renderer/audio registration, motion/control/camera providers,
original windows/effect consumers and full discard UI remain pending. Parent
requests stay open; host callback fixtures do not establish native playability.

### 2026-10-14 — Deferred original Mario item-get and control binding

Committed `7929601c`: finite motion request/entry uses existing Game motion,
movement and pose owners; required original idle/fall exits are injected and
checked before admission. Pending15 is consumed by the later Mario owner,
resets44/48/50 and horizontal180 while retaining unrelated vertical/air state,
AdjustMoveDir then original front-bank M_I_U. ControlOff/On use retained nested
byte/main2; stay is deferred. Game fixtures check actual acquisition order.
Isolated229 C++/141 Python tests and native build passed.
[Provider/validation](../behavior/field/NATIVE_ITEM95_MARIO_MOTION_PROVIDER.md).
Fresh source corrected prior vertical180 wording: inline clear174/17c/180/18c
is additiveX/Z, base horizontal speed and combined norm; additiveY178 stays.
[Source exit/clear contract](../behavior/field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md).
Actual native idle/fall sound cancellation and other main/renderer/audio
providers still need registration. Full discard/special forms and native
render/performance validation remain open; parent requests stay open.

### 2026-10-14 — Native Mario retained sound-post binding

Committed `eb9155a9`: Game retains original27c/280/284 initial-1 handles;
native main binds the Audio post provider before each Playing visit/after
restore. Idle/fall posts cancel source slots in order before clearing handles.
The actual NDSP stop adapter accepts owned field-effect channels20..23,
excluding existing cue/house/map/music owners. Pending/inactive/-1 slots skip
DSP; source Off uses low8 without generation validation. Readiness/frontier
and shutdown behavior are explicit. Isolated231 C++/141 Python tests and native
build passed, including actual adapter compilation with NDSP stop assertions.
[Native scope](../behavior/audio/NATIVE_MARIO_MOTION_SFX_POSTS.md),
[fresh original handle evidence](../behavior/audio/MARIO_INITIAL_SOUND_HANDLES_AND_ITEMGET_POST_STOP.md).
Idle/fall On/Main/macros/audible/position/lifetime services are still unbound;
no sample substitutes or audible equivalence are claimed. Native acquisition
window/effect/icon/camera/key/system/audio registration and captures remain
next. Parent requests remain open.

### 2026-10-14 — Native original parcel acquisition

Committed `3abf5525`: native original US dialogue/get windows, M_I_U, Box icon,
effect6, original fanfare/retained field BGM and source key/camera/system fields
are bound to the actual Game owners. Follow-up `d8efdb46` retains the separate
NPC talk/item key nesting and passes the same full checks. Readiness refuses missing DSP/assets/free audio slot/full key pouch
before story print. Isolated232 C++/141 Python tests and make all passed.
Clean isolated Azahar fixture shows original presentation and saves one95,
5405true/5406false, HP10/FP5 unchanged. This supplies known quest2 state; original
Trouble Center acceptance, Goomfrey delivery/reward, full discard UI, global
managers, letterbox consumers, audio parity and hardware performance remain
open. Original ISO music is the runtime default. No payload is committed.
[Binding, exact validation and limits](../behavior/field/NATIVE_GOOMBA3_PARCEL_ACQUISITION.md),
[source field owners](../behavior/field/ITEM95_CAMERA_KEY_AND_SYSTEM_LEVEL_PROVIDERS.md).
Parent requests remain open.
