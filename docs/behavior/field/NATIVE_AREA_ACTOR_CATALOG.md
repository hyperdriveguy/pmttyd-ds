# Native per-area actor catalog

This is an authored native adapter for the recovered static init subset in
[ordinary NPC bindings](ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[Frankly init](../story/PROFESSOR_FIRST_TALK_AND_FENCE.md). It is not the complete
original NPC driver, map setup parser or event evaluator.

`tools/field_actor_assets.py` reads original Shift-JIS `m/gor_02/s`, retaining
Chu3 position `(150,0,-400)`, direction270 and tribe. Its model is
`a/c_tyusan`; the model header selects the original texture bank. `CHU_S_1`
keeps original sparse keys, hierarchy, visibility and texture changes. Native
story0..55 admission is a narrow coverage envelope, not a recovered visibility
predicate. Missing story is not inferred. The recovered simple talk event
selects original `gor_02_031_00`..08 via the eight CASE_LESS thresholds;
the current envelope admits00 at runtime. Original message bytes/tokens and
actual actor attribution are retained, with no story write.

Professor records retain exact story8 position `(-135,0,-240)` and story10
position `(-220,0,305)`. Story9 has no visible NPC record. Story8 interior and
story10 exterior admission follow the existing native door adapter. First-talk,
party conversion, fence removal and lecture actors remain separate scene owners;
these static records do not execute or replace their events.

## Local format and ownership

PMNPC001/002/003 contain8-byte magic, uint32 model/actor counts, then model rows with
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
partial. Loading continues to accept PMNPC001/002.

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

Animation time uses simulation ticks since the first render after area load.
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

Static paper-facing converts source90/270 to the current right/left adapter.
Original NPC matrix/camera correction, `animPoseDrawMtx` facing, flags,
normal-based material lighting, translucent sorting and shadows remain open.
Emulator output establishes bounded execution, not New2DS hardware performance
or original GPU parity. Talk admission currently uses a native0.65-unit XZ
circle,0.25-unit height tolerance and grounded A before door/jump admission.
These are provisional adapters, not recovered original search/arbitration.
Talk opens the exact source room/global resource and uses original playback;
new actions and saves stay locked until closure while neutral field physics
continues. Collision, NPC search, facing-to-Mario and wandering remain open.

The original save-block S_1 retains outside0..1 UVs, discrete scrolling updates,
source binding repeat overrides, RGBA and pass2/3. Original bbox head collision is connected. Hit callback, save-choice
dispatch and card persistence remain unbound. Their recovered contracts are
[head lifecycle](SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) and
[common save graph](SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
