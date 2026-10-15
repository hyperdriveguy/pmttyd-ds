# Trio public API and runtime ownership

This guide covers the public [TrioAttack interface](../../include/battle/trio_attack.hpp)
and [facade methods](../../source/battle/trio_attack.cpp). Read
[draw bindings](trio-draw-bindings.md), [scheduling](trio-scheduling.md),
[provider ownership](trio-providers.md) and [ending](trio-ending.md) alongside it.
The established runtime/facade is annotated on its recorded committed baseline;
newer source Turn/audience/pinch/sequence/enemy-resource integrations still need
separate review. A matching file hash alone does not complete that live audit.

## Entry and command readiness

Fresh `begin` admits Mario actor 1 with original Jump `0x803538a8` or Hammer
`0x80353de8`, or Goombella actor 2 with default `0x80393ea8`. It constructs an
actual runtime with supplied shared/borrowed resource owners. Explicit native
fallbacks include RNG seed 1, 608×480 camera boot context and camera-4 mode 2;
these do not reconstruct original process history. Initialization must complete
before publishing the runtime and fresh HP allocation token.

An existing runtime reuses only when native `done` and `readyActor` match the
command owner. It then launches against retained resources. Fresh-only pouch,
field/camera/pose/sound arguments do not rebind an existing runtime. Shared
arguments preserve backing storage; raw field/input/service pointers remain
borrowed and need their real owner to outlive use.

`active`/`finished` expose `!done`/`done`, the native action boundary. They do not
mean all original battle, reward, teardown or field-return work completed. Before
entry, both are false and status-window visibility defaults true. Pending tokens
and trace entries also do not prove completed audio/GPU or full scheduler work.

`commandActor` additionally requires no pending work and phase `0x04000002`.
It uses retained ready actor when available; otherwise actor bank `0x05000008`
and the original phase table supply an eligible ID. Signed move count, action
mode, status byte and source attributes stay separate gates. `prepareCommands`
mutates source actor preparation before native menu creation can fail. On
success it synchronizes actual registry presence, damage and part state. Failure
is not a rollback of earlier source selector writes.

## Borrowed resources and projections

Most getters return pointers/references into the current runtime. Trace getters
return stable empty sentinels before entry, then borrowed vectors. Process Mario
animation/file-cache getters can refer to a newer catalog than an existing
runtime's pinned file owner; they are not allocation/pass admission proofs.

`battlePose` requires the live packed part handle at `+0x1c0` to equal its native
binding, then queries the actual pool. This does not independently pin a generation
or prove current draw admission. Audience incarnation metadata concerns source
lifecycle; source icon pass combines owner token, sequence and completed draw.
Neither alone establishes scene visibility, touch rectangles or source selection.

Reward and field accessors expose different milestones. Active PointGet,
ending-owned icon records, fade alpha and canonical exported pouch are separate.
An exported pouch preserves actual resource writes but does not prove field
return. BattleHitRegistry invalidation explicitly reopens unknown collision/
context writers; a previously fetched registry pointer does not freeze validity.

## Camera and outer Main

`setCameraDepthLensAdapter` changes actual camera-4 fov/aspect/near/far, builds a
numerical snapshot from current vectors and marks source history incomplete.
A direct `setCameraDepthSnapshot` disables that live lens adapter without changing
camera lens or restoring history. While finishing Draw is pending, the getter
prefers the actual camera-4 published matrices. Numeric projection data is not
recovered original camera history or a completed GPU pass.

`setPhysicalInputOwner` binds a borrowed actual producer through current VM GP.
`makeOuterKey` is admitted only at the retained outer Main key stage (index 0,
phase 3). It reloads GP through source `0x80415fd0`, rate at GP+4 and motor gate
at `+0x1294`, then updates all four raw pads. Call cadence remains caller-owned;
there is no extra once-per-pass guard in this method. Native motor output is a
hardware no-op on New 2DS XL.

`resumeOuterMain` substitutes VM word/byte/store services but forwards external
request/time/divide/sequence callbacks through their original user. Stack bridge
objects exist only during this call; retained state belongs to runtime cursors.
Whole Main completion sets `outerMainCompleted` and waits on `0x8029d864` for
profiling/end-render ownership. `outerMainPending` remains true for that boundary,
while another completed `resumeOuterMain` is refused. It is not a loop that can
finish all later outer services or tear down battle.
