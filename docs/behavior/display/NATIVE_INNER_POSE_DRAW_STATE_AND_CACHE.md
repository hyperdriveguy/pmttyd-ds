# Retained inner pose draw state and native cache submission

2026-10-14. Implements the ordinary unlinked, flags8-clear prefix of
[inner draw8003bc68](ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md), using actual
native pose/resource owners. Shared-ring cache misses, other facing and linked branches remain explicit
providers. This does not acknowledge original model traversal or establish
raster equivalence.

The cursor captures descriptor/resource identity and sample current/next/fraction
before Main, applies both source mode gates and original-pass admission, copies
secondary then primary material color, and runs mapTestXLU predicates in order.
Each map branch queries a fresh owner; a held second query never repeats the
first. Captured colors and call arguments stay pinned while their source service
is unavailable. An unavailable converted descriptor is a provider hold, not
proof of a source null descriptor or empty model.

Effective translucent pass3 changes GX state only. It never replaces the
original traversal pass. A nonzero material hook skips built-in GX and work-pass
publication. Otherwise depth/blend/location/alpha requests execute in original
order and only then publish the work pass. Ordinary incoming angle rotation
precedes uniform scale. Late Main follows these matrices and cannot refresh the
captured traversal descriptor/sample values. A held Main remains admitted even
if it publishes its cache key before a later provider completes. Modeling base Copy precedes typed
context publication; two separately resolved flushes precede vertex cache
invalidation and the real dispProc8003c4bc request. Holds do not replay completed
stores/requests. Special flag8 facing holds at its camera4 query boundary.

Native battle work mode is1, replaying battle_init80073920 →
animPoseBattleInit80041c58 → W10c=1 at80041c6c. The cold animInit value0 must
not be used during retained battle drawing: it would reject actual mode1 poses.
Original autoRelease(1) resets the field later; battle teardown remains pending.

The map material owner is the selected battle bank1 projection. Its producer
precedes stage initialization: seq_battleMain rank0 at80073d48 calls
bmapLoad8002892c(bti_01,stg_00_1), which selects/clears bank1 before battle_init,
BattleInit and BattleStageInit. The native stage setup replays this preceding
lifecycle once. Loader writes affect version/fog flags, leaving material bits2/4
clear and raw colors zero. This is retained initialized state, not a constant
opaque classifier. Real blend setters must store RGBA then set the corresponding
material bit; unimplemented EVT callbacks suspend rather than silently mutate.
The selected trio module imports no blend event in its reviewed event objects;
other module/global lifetimes are outside this finite projection. Bank1 stays
selected until real battle_exit/bmapUnLoad restores bank0.

Native matrix math uses the existing explicit SDK adapters. Actual depth/blend/
alpha setters queue Citro3D state. Selector0 cache flushes use the current pose's
mutable evaluated `shape.positions` and `shape.normals`, with current resource
counts re-resolved separately. PMANIM3/4 retain original Df0/Df8 arrays without
filtering; buffers must match those counts. `GSPGPU_FlushDataCache` executes even
for zero lengths, retaining the source unconditional-call boundary. Other output
selectors remain pending. The vertex-cache adapter queues
`GPUCMD_AddWrite(GPUREG_VTX_FUNC,1)` within the active command frame, the same
post-vertex-cache clear used by official Citro3D
[DrawArrays](https://github.com/devkitPro/citro3d/blob/master/source/drawArrays.c),
[DrawElements](https://github.com/devkitPro/citro3d/blob/master/source/drawElements.c)
and [ImmDrawEnd](https://github.com/devkitPro/citro3d/blob/master/source/immediate.c).
This adapts cache services; hardware cache organization and SDK/trig bit equality
are not asserted.

[Native ordinary traversal](NATIVE_ORIGINAL_MODEL_GROUP_TRAVERSAL.md) now advances
from dispProc8003c4bc through source node SDK operations and shape−1 returns to
an actual nonempty renderProc8003c884 hold after real state/matrix/cache work. No idle bake, empty-resource assumption, queue
acknowledgement or history publication replaces its indexed model/material graph.
Continue [group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md), node pushes,
current-camera shape entry, texture/light/material requests and indexed streams.

Private read-only original evidence is under
`artifacts/battle-display-queue-evidence/`: mapInit, mapBattleEnter, mapLoad,
bmapUnLoad, seq_battleMain, battleInitPrefix, mapMaterialColorLeaves,
animPoseBattleInit, animationModeFieldReset and innerFlush `.dis`/`.elf` pairs.
Flush source is8003c210:240, raw SHA256
`c3c1fecfcb4efbeb5bb8a889d853ab15addc62221d802eac3a65be5309152485`;
battle-mode producer80041c58:2c hash
`60626314ab10668a1607b4c15ae4b9d0c740a5e8a149b312bf6eb4c8869be81b`.
