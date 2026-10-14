# Native trio pose depth and display queue binding

The finishing display prefix now retains its captured source part, camera4,
draw class and depth XYZ after pose Main/alpha/TestXLU. It calculates
[original queue depth](BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md) from
an explicitly supplied snapshot and registers callback80101774 with borrowed
part identity. The real [queue owner](NATIVE_DISPLAY_QUEUE_OWNER.md) retains
that record; registration does not execute the deferred callback.

Missing matrices preserve a continuation immediately before depth calculation.
Supplying a snapshot resumes that continuation without running pose Main,
status/effect updates or the preceding engine pass again. Capacity admission
failure retains the calculated priority. NaN rejection completes the source
Entry return without a record and still follows the immediate continuation.
A held later service does not duplicate registration on later native ticks.

## Explicit native camera adaptation

Game supplies its retained field FOV and near/far (native units times100), plus
source608/480 aspect. The runtime builds source-space matrices from its retained
battle-controller eye/at/up after each admitted camera update. This is the
existing SDK numerical LookAt/perspective adapter, with supplied source lens
values. It does not import original field roll, view translation, projection
extra, orthographic type or prove field history/controller/cadence equivalence.
The API also accepts complete explicit matrices, replacing the lens adapter.
Original camera request/history validity remains separately reported.

Renderer still uses its existing handheld battle view; queue depth matrices
have not been substituted for the GPU camera. Actual global camera4 publication
must retain the complete source state and operation order in the
[matrix producer contract](BATTLE_CAMERA4_MATRIX_PUBLICATION_ARITHMETIC.md).

## Immediate continuation and next provider

After ordinary Entry, actual source branches are checked before another fresh
PartsWorldPos query. The fresh non-midpoint domain calls the existing original
packed stage-floor consumer, retains original returned worldY for the factor,
and calculates shadowY=floor+10 and radius in the exact multiplication order
from the [registration continuation](ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md).
The exposed shadow request retains these values and holds at8002be98; no
synthetic ellipse or completed shadow owner is claimed. The partner midpoint,
secondary registration and alternate suppressed-shadow branches remain bounded
at their missing continuation. The queue owns borrowed identities, with the
current runtime retaining live unit/part/pose storage until destruction.

Regression checks drive a real finishing Mario pose, remove the matrix provider,
resume with explicit matrices, compare the queue record/key and verify pose
cache history/Main count do not replay. A later held tick preserves one record.
The native Game adapter also runs in ordinary regression playthroughs. No actual
sort/draw/reset cadence, shadow pool, deferred matrix/material/GPU callback,
complete display footer or ordered battle teardown/field return is established.

The subsequent [shadow/live-display step](NATIVE_SHADOW_WORK_AND_TRIO_LIVE_DISPLAY.md)
now completes ordinary shadow Entry and the no-table tail, follows linked parts
and registers Goombella too. The earlier8002be98 hold described above is
superseded for that admitted domain; actual shadow preparation/GPU work remains
open. The next exercised constructor provider is801011fc.
