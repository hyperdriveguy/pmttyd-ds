# Native original class-menu callback composition

`BattleClassNativeFrame` composes the existing whole retained queued display,
radial animation, icon/raster/footer controllers and immutable publication.
It adds no duplicate original source body. The original chain is
BattleCommandDisplay_ActMenuDisp `8012210c/40` → ActSelect_Disp `80116cb8/20`
→ DrawMainMenu `801160e4/b94`. Source pins are checked read-only by
`tools/verify_battle_class_native_frame.py`, including the existing ring/body
verifier. These are static evidence, not original hardware observations.

The callback first looks up type14 at passed B+171c. Its private output begins
unwritten and remains unused. DrawMainMenu then captures its own current-global
B/W/C; the native Trio binding separately proves passed/global B agree and the
actual W allocation is still owned. The original body owns W+14 and row/window
animation writes. Neither native publication nor renderer admission writes
source readiness. Physical controls and original X interruption are unchanged;
there are no touch-only controls or new audience rectangles.

## Actual ownership and holds

Binding copies source display/raster tables and the backend configuration,
retains explicit immutable resource/context ownership, and uses the actual
allocation token/address/current predicate. The camera bridge reserves an
explicit proven-unmapped range and requires the actual successful CameraMain
publication predicate, actual CameraMain/Draw cursors and DisplayQueue. A
published view alone is insufficient. Source execution only occurs while the
real queued callback8012210c is invoking on camera8 with its exact userdata.
The bridge verifies record, queue reset epoch, frame and producer ownership.
A held callback cannot move to a different successful Main pass. Every linked
provider boundary additionally checks the current camera epoch through the
existing pipeline's current predicate, including after an external provider
returns. Completed children and source writes never restart on a later wait.

Reentrant resume/bind/admit calls are rejected. Retirement invalidates source
and publication immediately and defers destruction until an in-flight callback
returns. A whole source batch is exposed only after camera finish succeeds;
partial or producer-invalidated work cannot become drawable. Completed copied
geometry survives ordinary queue reset after source completion, subject to
allocation and resource lifetime. Rendering reads this batch and reports exact
admission once; it never advances animation or starts another source call.

All predicates are pure, non-reentrant actual-owner checks. Resource/context
objects must remain immutable while bound. Total guest memory callbacks must
be actual admitted storage, not unavailable-as-zero adapters. If an external
provider partly completes before false, it must own its retained child state.
The backend font-string owner retains its existing atomic admission contract.
The existing observable icon sorter remains an explicit port policy: missing
inactive operands can be ignored only when visible order is uniquely proven;
missing/tied active geometry holds. This composition does not reconstruct
unwritten original stack positions or certify full source/pixel parity.

## Proposed native Trio integration

`artifacts/battle-class-native-frame-native-replacements.json` supplies exact
unapplied public/runtime changes. `classMenuFrameBinding(linked)` returns real
VM actor/cursor services, shared camera bank and predicates retaining the real
runtime. The actual UI token comes from the committed source allocation hook
and is retired before real map free. Its retained token address supplies a
native camera generation, as in HP; its number alone proves no lifetime. Checks
also compare the public Trio runtime, actual B+1c78, current-global B and exact
token. The public Trio facade itself must outlive the binding. A retained old
runtime cannot authorize callbacks after the facade replaces it.

The proposed label reader admits only the seven actual VM-mapped original
message pointers, reads their complete NUL-terminated bytes without a truncation
limit, and caches exact bytes in runtime-owned stable map nodes. Unknown or
unbacked pointers remain unavailable. The real shared BattleRandom supplies
source bounded draws in original request order. `resourceWord` reads actual
VM then GP storage; it does not fabricate a common TPL/FileEntry. The proposed
`resumeClassMenuFrame` pins the real runtime and passes its actual completed
Main epoch, Draw cursor and queue to this owner.

Normal activation and the production Renderer callback are deliberately unbound
in this batch. The caller still must bind actual original class surface/icon
GPU lease, immutable US font/message icons, original common-bank descriptor/TPL,
retained footer/project/change-position/sound providers and numeric services.
The surface lease's `current()` must participate in the allocation-current
predicate and the copied backend must carry that same actual lease. Camera
providers use `frame.camera()` only during source execution. Its reserved range
must not fall through to guest RAM on unavailable reads. No synthetic camera,
identity, zero geometry or success footer may close these dependencies. The
lower screen continues to belong to the original scene/audience presentation.

## Focused validation

Host tests use actual CameraManager Main/Draw and DisplayQueue dispatch with
explicit test source providers. They compare the original DrawMainMenu parent
trace through its icon boundary and all original readiness/store effects;
extra observable-sorter guards are identified separately. Tests hold a footer
twice, reject changed passes/reused owners/changed W/reset queues/reservation
loss, reject reentry, retire during a child, and invalidate a producer from a
provider before the next sibling. Empty rows are intentional source test data,
not synthesized native menu rows. Existing full source/raster/pipeline tests
remain required for nonempty geometry. Host/ARM checks and frozen dependency
pins are recorded in the accompanying validation artifacts; whole-project gates
are owned by the integrating root.
