# Retained animation Main clock and selected sample time

US G8ME01 disc 0 revision 0. `advanceAnimationMainTime` implements the complete
nonnull Main region `8003ee08..8003ef80`, ending after pose R+24 publication.
Source SHA256 `4d889513ebf55fc05bc449a9a735b264fdd344d42ae3be86f0e7ca888796025c`.
It is a retained child for the [Main entry evaluation boundary](RETAINED_ANIMATION_MAIN_ENTRY.md),
not complete Main or the remaining timeline/cache/output evaluator. Original
[entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md) remains the recovered source contract.

## Original reads and publications

Inputs retain captured Main W/B/R/MODEL/T/output pointers. This child reads R+c,
captured T+24, then control+0 to capture the loop selector BEFORE clock work.
It reads GP through 80415fd0 BEFORE hardware clock 800000f8. Zero mode selects
GP HIGH40/LOW44; every nonzero mode selects HIGH38/LOW3c. The source divisor is
high32(10624dd3*(hardware>>2))>>6. Division is signed `__div2i` with raw64 input
and a positive32 divisor, not unsigned division. A zero divisor remains an
unavailable numeric provider; repairing live hardware does not change a divisor
already captured by a suspended invocation.

Timestamp subtraction reads LOW R+1c before HIGH R+18 and subtracts modulo64.
The resulting bits go to UNSIGNED `__cvt_ull_flt`, with native binary64 then
binary32 rounding. Underflow can therefore become a large positive frame value;
there is no elapsed-time clamp. The NPC signed conversion helper is not used.

Load original 60 at 8041fba4, 1000 at 8041fba8, multiply, capture original ZERO
at 8041fb28, divide, and publish R+20. Reload R+20, load fresh R+44 rate, multiply
and publish R+20 again. A held child must not collapse those stores or reuse the
first calculated value across the intervening source reload.

Then reload T+24 and R+20. Read END control+8 BEFORE START control+4. Keep the
INITIAL loop selector and earlier ZERO even if live control/literal storage has
changed. Single-precision span/offset/ratio operations and ordered comparisons
follow the source branch graph. Zero-span has priority, stores offset at R+84
and selects start. Looping negative offset stores captured ZERO and selects
start. Other loop cases divide offset/span. Nonloop cases retain source start/
end clamps and unbounded ratio rather than capping it at one.

In the conversion branch, load original double bias802c1070 as ONE acknowledged
64-bit primitive BEFORE storing ratio at R+84. Then reload R+84 before fctiwz.
Reconstruct the original double word 43300000:(uint32(index)^80000000), subtract
the CAPTURED bias with single rounding, and execute single negated fused
subtract using captured span/frame. Do not replace this graph with float(index)
when the loaded bias differs, or with fmod/start-relative reconstruction.
Finally store selected time at R+24. The initial loop selector publishes to the
parent only after completion; later timeline sampling owns its own fresh reads.

## Signed zero and numeric scope

The shared `animationFnmsubs` helper computes fused index*span-frame, rounds to
single, and negates a non-NaN result. Exact cancellation therefore produces -0,
including ordinary frame60/span10/index6. Both this retained child and the older
`finishAnimationSampleTime` now use that graph. Reversing FMA operands into
frame-index*span gives the wrong zero sign. The source instruction order is
corroborated by [Dolphin's fnmsubs implementation](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_FloatingPoint.cpp).
This correction is static instruction evidence, not a measured original trace.

Native finite fctiwz accepts [-2^31,2^31); NaN/infinity/overflow remain explicit
unavailable conversion domains. No fabricated integer default is returned.
Nonloop NaNs follow the source comparisons and can publish NaN. Original FPSCR,
exceptions, denormals and NaN-payload/selection parity remain unmeasured; native
libm arithmetic is not claimed to emulate the entire Gekko floating-point unit.

## Live VM ownership and suspension

`AnimationMainTimeBinding` retains the actual shared pool and borrows the real
VM/AnimationPoolMemory owner. A pure current predicate proves the same producer
pass/lifetime. Every word, store, atomic double and numeric boundary rechecks
that predicate and the actual shared aliases. Wrong VM or stale aliases cannot
fall back to detached typed state. Mapped inactive pose rows remain readable/
writable without an invented active/resource gate. The outer Main still owns
its source resolution and actual invocation lifetime.

All unavailable primitive operations have no effects. Earlier acknowledged
stores may already have changed actual pose storage; the same cursor must
resume without restarting its parent. Request fields, provider identities,
loads, numeric results and branch operands remain pinned as appropriate.
Successful completion clears the cursor. Cursors and requests are bounded;
there is no per-invocation allocation. The binding and borrowed owners must
outlive suspended children. This child is not installed in normal Game yet.

## Validation and next work

Tests pin an explicit complete trace, every primitive hold across seven branch
classes and both clock banks, provider/input identity, initial-selector and
ZERO captures, fresh R20/control/ratio reloads, atomic bias ordering/capture,
changed-bias integer reconstruction, signed clock/unsigned elapsed underflow,
zero divisors, nonfinite conversion holds and negative-zero bits.
Actual cold-process tests compare the real sampler's R20/R24/R84 publications,
then suspend after the first actual pose store and resume after timestamp
mutation, proving no repeated earlier store/conversion. They cover actual
mapped inactive slot255, wrong-VM rejection and unavailable pass ownership.
The native numeric adapters and fixtures are port evidence, not original
runtime/visual/hardware parity.

Private extraction adds source literals8041fba4+8 and bias802c1070+8 to door-data;
the cold image adds only its missing rate literals and validates their exact
layout/words. Existing source bytes and all other products remain unchanged.
Ignored source notes/listing live in `artifacts/npc-main-parent-evidence/` as
`animation-main-time-prefix-notes.txt` and `animation-main-time-prefix-review.txt`.
[EVT-01](../../requests/EVT-01.md) stays open. Next retain original timeline scan,
cache replay and interpolation using the captured Main resource/output owners;
compose that continuation through Main and Inner, bind platform/linked/GPU
owners, activate source NPCs in normal Game, and verify original field visuals,
event chronology, numeric behavior and physical New 2DS XL performance.
