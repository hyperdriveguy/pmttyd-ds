# Actual Trio allocation and retained enemy HP callback binding

`TrioAttack::resumeEnemyHpDraw(..., TrioEnemyHpNativeServices)` composes the
[retained source frame](RETAINED_ENEMY_HP_SOURCE_FRAME.md) and
[HP camera companion](NATIVE_ENEMY_HP_CAMERA_BRIDGE.md) only during the actual
`8011fde0` invoking record in `CameraManager::draw`, camera8, with its actual
Trio battle pointer. It does not register a substitute record or advance Main.

The native allocation token is created after successful Trio initialization.
Its current predicate checks that same runtime/token, actual contiguous battle
storage and actual global battle pointer. `enemyHpAllocationOwner()` exposes a
strong token lease: external publication owners **must retain it**, alongside
checking `enemyHpAllocationCurrent`, to prevent native pointer ABA after an old
runtime dies. The token is native metadata, never an original guest address.
The Trio battle mapping is created once; subsequent event heap activity uses
its existing mapped storage.

The successful camera producer serial increments only after the actual retained
CameraMain completes. Overflow poisons admission. HP camera leases require that
serial plus the actual Main/Draw cursors, queue record and native queue reset
metadata. A native view at `60000000`, stride `400`, reserves thirteen camera
slots. Every provider boundary uses the actual address space's `unmappedRange`
proof, including external file storage. This is explicit native view policy;
no original guest camera pointer is claimed. Reserved addresses have first
refusal even when unavailable, with no VM fallback inside the reservation.

Real original status/Tattle saved-switch readers, typed source ScreenPoint,
interpolation and VM conversion providers feed the source parent. Source Number
constants/table tails and bar constants come from the imported original VM
pack. Original icon directory/TPL providers remain external. Number Matrix/GX
camera queries and SDK matrices use the HP bridge; ordinary GX/FIFO operations
must be acknowledged by the external real renderer owner. Runtime owns stable
wrapper tables and retains the explicit external `lifetime` across holds.
Replacing provider identities during a held allocation is rejected. Providers
must become unavailable before their renderer/Game resources retire.

The external begin hook is retained/atomic admission before source effects.
Source completion is retained separately from publication: a held publish does
not rerun Number or bar prefixes. External callers temporarily reactivate the
same capture pass on each retry and stop capture after each callback returns.
The camera lease finishes only after whole publication succeeds. Allocation
proof outlives the camera lease so copied completed geometry remains drawable.
No source End/guest cleanup is synthesized by native camera retirement.

Trio now binds the actual common-menu file cache before importing scripts and
runs original `Init80119da0` after setting global B. Its original format string
is read from the accepted original pack. Actual descriptor/a0/TPL relocation
bytes feed texture17/18 lookup. Runtime teardown releases only a completed
source initialization through original `End80119d74`; the source leaves its
stale B pointer bytes intact. The original Goombella DOL span excludes the live
file-manager global word `80418498`, which belongs to the shared file owner.

## Scope and validation

Normal runtime currently reaches Main/display registration and camera Draw
through the finishing sequence. Active battle ticks do not yet run that whole
source cadence; earlier camera4 pose/stage/audience providers can also hold. Surviving enemy
ids3/4/5 currently lack the original pose-resource directories bound for
ids1/2/6, and stop display registration at `801011fc`.
This binding alone does **not** prove active-enemy HP pixels or normal battle
HUD activation. No Tattle flags, visibility flags or cameras are forced for a
screenshot. Active source bank2/Main sequencing remains a prerequisite.

`battle_enemy_hp_number_bridge_tests` uses actual imported icon resources and
whole Number/Matrix/GX bodies through a real CameraManager Main/Draw bank. Its
boot-camera callback admission is a declared numerical fixture, not source
BattleMain history. It verifies camera-view reads, actual texture construction,
retained quad waits without prefix replay, source mode1 table-tail fallback,
and stale epoch/reset rejection. Host GPU operations are captured requests,
not a claim that native pixels were rendered. Actual Trio allocation/resource
initialization tests are separate from this numerical fixture.
