# Native per-area actor catalog

This is an authored native adapter for the recovered static init subset in
[ordinary NPC bindings](ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[Frankly init](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md). It is not the complete
original NPC driver, map setup parser or event evaluator.
[Live wandering](NATIVE_LIVE_NPC_WANDER.md) now binds the recovered zero-vertical
Goomba4 profile alongside the static records.

`tools/field_actor_assets.py` reads original Shift-JIS `m/gor_02/s`, retaining
Chu3 position `(150,0,-400)`, direction270 and tribe. Its model is
`a/c_tyusan`; the model header selects the original texture bank. `CHU_S_1`
keeps original sparse keys, hierarchy, visibility and texture changes. Native
story0..55 admission is a narrow coverage envelope, not a recovered visibility
predicate. Missing story is not inferred. The recovered simple talk event
selects original `gor_02_031_00`..08 via the eight CASE_LESS thresholds;
the current envelope admits00 at runtime. Original message bytes/tokens and
actual actor attribution are retained, with no story write.

[Darkly](NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md) adds his original stationary
position(-335,0,-335),direction270,c_f_mura bank,S_1/T_1 and flags0x40080603.
His current early0..55 envelope binds only original gor_02_034_00; source175
name/flag and later threshold branches remain unbound. Relative order after
Chu follows the original registry. No omitted branch receives a fake fallback.

Original east Goomba4 uses init(-370,0,130),direction270,c_kuribo_n,
radius70/speed30 and source S_1/W_1/R_1 selection in early0..55. Its original
talk event is present for candidate pause but its quest-before-story playback
is unbound; no missing quest state is inferred. Registry records now sort by
original tagged order, including Goomba4 before Chu/Darkly.

Professor records retain exact story8 position `(-135,0,-240)` and story10
position `(-220,0,305)`. Story9 has no visible NPC record. Story8 interior and
story10 exterior admission follow the existing native door adapter. First-talk,
party conversion, fence removal and lecture actors remain separate scene owners;
these static records do not execute or replace their events.

## Local format and ownership

PMNPC001/002/003/004 contain8-byte magic, uint32 model/actor counts, then model rows with
32-byte asset stem and float UV scale. Actor rows are120 bytes: seven uint32
words(key,room,model,interior required,interior forbidden,story gate,kind), two
signed story bounds, five floats(originalXYZ,yaw,model scalar), and32-byte pose
name plus32-byte optional talk prefix. Kind0 is NPC, kind1 reserves map-object presentation. Numbers are little
endian on supported host/ARM targets. Keys are native FNV identifiers over
room/source name; they are not REL pointers. The scalar is explicit because
[original save blocks](ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md) use10, while
ordinary NPCs use2. PMNPC002 adds a uint32 system-forbidden mask before the
story bounds (124-byte rows). Gate2 excludes exactly one story value.
PMNPC003 adds original source names and model bbox metadata for
[native object head collision](NATIVE_MAP_OBJECT_HEAD_COLLISION.md).
The original plaza `mobj_save` uses scalar10, position `(185,60,-55)`,
S_1, story!=341 and system mask0x20 admission. Its presentation uses
[PMANIM4 surfaces](../display/NATIVE_ACTOR_SURFACE_PACK.md).
The constructor gates are represented; dynamic system flag producers remain
partial. PMNPC004 adds original model names for floor/wall filters.
PMNPC005 adds source dimensions/talk names and model draw masks. PMNPC006
adds original npcFlags (200-byte actor rows); versions1..6 remain readable,
with legacy missing flags retained as unbound0. PMNPC007 adds a variable
source-order talk-rule tail after each actor: count then comparison/story/fullID
records. Empty/unmatched bound rules return empty; versions1..6 retain their
legacy selector compatibility. PMNPC008 retains100-byte model rows and the
200-byte actor prefix plus PMNPC007 rules, then adds188 bytes for original talk
presence,enabled/work flags,radius/speed/run threshold,step period and five
pose/sound names. Legacy versions remain readable; enabled wander admission is
bounded to source direct-horizontal/no-gravity flags. See [live format and owners](NATIVE_LIVE_NPC_WANDER.md)
and [native per-actor rule format](NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md). [Native NPC side collision](NATIVE_NPC_SIDE_COLLISION.md)
uses source Entry/setup/constructor flags and per-instance previous roots. The catalog loader stages the original angle table atomically
with the catalog for [ordinary talk selection](NATIVE_NPC_TALK_SELECTION.md).

Loading rejects oversized counts, unsafe names, invalid model indices,
nonfinite positions/yaw/scale, unsupported UV scale, contradictory gates,
overlapping same-room actor identities, malformed strings, truncation and
trailing bytes. Failure retains the previous catalog. Namespace is model+pose;
different models may both own `S_1`. Future rooms add records and resources
without a renderer branch or a new global pose namespace.

## Renderer and performance

Only destination-area models import. Their animations and textures stage with
destination geometry. Failed imports discard staged resources after
`C3D_FrameSync` and retain the current area; successful changes synchronize
before disposing the previous area's resources. Staging peak memory includes
both areas. Model resources remain shared among their instances.

Object animation time uses simulation ticks since the first render after area load.
NPC instance clocks retain simulation milliseconds and source message requests,
as described by [NPC presentation](NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).
One persistent sample per resident model caches tick and pose; sampling another
pose invalidates that cache. Hierarchy traversal arrays retain capacity and use
allocation-free recursive dispatch after warmup. Source sparse key replay
remains CPU work. Root trigonometry is calculated once per instance. Original
timing, initial phase and pause cadence still need comparison.

Whole meshes enter the existing12000-vertex linear GPU buffer atomically.
Visible actor, vertex, sampling failure and overflow counters are available
through `Renderer::fieldActorMetrics()`. Actor overflow submits no partial mesh.
Texture groups submit citro3d spans through the existing PICA shader, depth/
alpha admission and frame services.

Original NPC matrix/camera correction, Main/Inner facing, source dimensions,
talk clocks and source shape culling/colors are bound in the scoped
[presentation service](NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md).
Full display flags, material lighting, translucent sorting and shadows remain open.
Emulator output establishes bounded execution, not New2DS hardware performance
or original GPU parity. [Ordinary talk selection](NATIVE_NPC_TALK_SELECTION.md)
uses source width/height, camera-relative width-span facing, map obstruction and
strict nearest selection. NPC updates publish a candidate after Mario; that
retained candidate can block the next Mario A jump. Selected talk starts on the
following NPC invocation. Complete encounter/door/regular-event priority is open.
Talk opens the exact source room/global resource and uses original playback;
new actions and saves stay locked until closure while neutral field physics
continues. Stationary talk uses source turn-to-Mario and restores its retained base yaw
after closure. Live wandering saves/restores current source yaw through its
candidate/event owner and preserves countdown/work while stopped. [NPC side response](NATIVE_NPC_SIDE_COLLISION.md) now binds source
dimensions/flags, previous-root rollback, polynomial response and map-wall
retry. [Live Goomba4](NATIVE_LIVE_NPC_WANDER.md) consumes prior NPC clock/touch
publications, applies direct XZ displacement, selects poses from actual speed
and publishes live paper matrices once per simulation tick for read-only
rendering. General NPC timers/motion/registry/event lifetime and other
wandering profiles remain open.

The original save-block S_1 retains outside0..1 UVs, discrete scrolling updates,
source binding repeat overrides, RGBA and pass2/3. Original bbox head, top support and ordinary Mario side collision are connected. Hit callback, save-choice
dispatch and native SD persistence are connected through
[live save objects](NATIVE_LIVE_SAVE_BLOCK.md); original card/audio services
remain unbound. Their recovered contracts are
[head lifecycle](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) and
[common save graph](SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
