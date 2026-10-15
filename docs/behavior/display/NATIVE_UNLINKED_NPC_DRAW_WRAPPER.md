# Native original unlinked NPC Draw wrapper

US G8ME01 disc0 revision0, static source plus native implementation; no original
execution measurement. Original `animPoseDrawMtx8003b4d8/790` SHA256:
`d029569d05cea10fa954194e91776e2a15a98cbf5fcd6d294682362c5be332fd`.
Read-only audit: ignored `artifacts/native-npc-draw-wrapper-evidence.txt` and
`artifacts/npc-main-parent-evidence/animPoseDrawMtx-review.txt`.

The [wrapper contract](ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md) now has a retained
actual-VM implementation for its initially unlinked route. This does not finish
the linked wrapper or original Inner/GPU renderer. Initial linked preparation
and a changed final link expose separate unfinished frontiers; neither is an
empty success. The full remake and ordinary Game NPC activation remain open.

## Exact source reads and dispatch

Retain raw handle/pass, copied caller matrix, angle and scale. Capture global W
through80415dc0, pool at W10, and P=pool+low32(handle*170). Read P90. Initial−1
continues; other values stop before the linked prepass. The unlinked route reads
P10 resource index, INITIAL captured W0 model table, table+16*index+8 file,
filea0 holder, holder0 MODEL. It does not reload global W for this lookup.

Pass2 first reads MODELc4 and admits bit1. If that fails, it **reloads** MODELc4
before the ordinary mask test; other passes read it once. PPC `slw(1,pass)` uses
only low6 shift bits: shift<32 gives1<<shift, otherwise0. Raw64/−64 therefore
shift0; raw32/−1 produce0. There is no pass enum, clamp or upper-bit bounds test.
Rejected masks complete without a final P90 read, Inner call or wrapper stores.

Admitted masks reread P90. Still−1 dispatches original Inner8003bc68 with the
captured P RECORD POINTER, unchanged original pass/matrix/angle/scale. The matrix
payload represents the field caller's actual stack matrix; no guest stack address
is invented. No matrix data, pose flags/mode, clock, selector or constants are
read by this wrapper branch. Inner owns its separate source reads and effects.
After the child acknowledges, this unlinked route returns with no cleanup.

If the final P90 changed to non−1, original dispatch uses incoming GPR30. The
initially unlinked path never set captured Q in that register. The implementation
retains a `CallerRegister` frontier instead of inventing Q from the new link or
calling P. A restored memory value does not undo an already captured branch.
Initial linked preparation has its own `LinkedPreparation` frontier; its source
prepass/ring/selector/matrix/cleanup work must be implemented before completion.

## Native service ownership

[The NPC lease](NATIVE_NPC_ANIMATION_PASS.md) routes Draw8003b4d8 exclusively to
this wrapper when its animation provider is installed. Legacy GPU handlers never
receive a failed native wrapper request. Exact VM, lease/pass/current predicate,
configuration and native pose generation/resource/buffer owners stay pinned.
Raw fields can still be read at the original capture points; sidecar lifetime
checks do not replace source reads. Input comparison uses exact float bits,
including signed zero and NaNs. Unsupported guest matrix-address lanes hold;
field callbacks already supply the copied semantic matrix with that lane0.

`configureNpcInner` installs a separate borrowed `NativeNpcInnerServices` context
only while quiescent. No default acknowledges Inner. A false provider retains
its original context and effects. Wrapper state blocks lease retirement and
competing animation calls. Its Inner phase may resume the actual process Main
only for the same root handle and lease pass; presentation/selection remain
excluded. A provider cannot acknowledge completion while that child is pending.
This permits the original Inner cache-miss Main without allowing another request
to steal it. The caller still owns full callback/queue completion before lease end.

The wrapper uses fixed cursor/matrix storage and existing source/native owners;
it does not build meshes, initialize another VM or allocate a new per-draw
executor. Raw source outputs remain authoritative. Older typed mesh snapshots
are not GPU completion evidence. Actual Inner/cache/material/SDK/group/shape/GPU
binding and physical hardware performance require further work.

## Verification scope

`animation_draw_wrapper_tests.cpp` pins every unlinked read/hold, fresh pass2
fallback, captured-W reuse, raw shift domains, address wrapping, final-link
frontiers, bitwise input/provider identity and held Inner without prefix replay.
`native_npc_animation_tests.cpp` uses actual cold files/VM/aliases, proves
same-pass retained late Main, source pass rejection, stale lifetime/current guards,
and complete deferred callback traversal through an explicit Inner receipt fixture.
For the original Mario resource mask1, translucent callback passes1/2/3 submit
only source-admitted pass2. Original NPC callbacks forward source scale2. Neither
fixture is a production GPU backend or proof of rendering equivalence.
