# Retained shared animation paper-facing branches

US G8ME01 disc0 revision0. `advanceAnimationMainFacing` implements the complete
optional Main tail80040540..80040648, source SHA256
`478452539d429ed0b4dd7eb4e04d74ed1470cee47d98d977dd0f82848c32b19a`.
`advanceAnimationInnerFacing` implements the complete Inner facing/uniform-scale
region8003be8c..8003c210, source SHA256
`00592d979b2fcbf2c694b4671860d0f01089701251b2e86624d26c13fdbd6721`.
See [Main facing](ANIMATION_MAIN_CAMERA_FACING.md),
[Inner facing](ANIMATION_INNER_DRAW_FACING.md) and
[NPC callbacks](RETAINED_NPC_ORDINARY_DRAW_CALLBACKS.md).

## Source graph and holds

Both use actual source pose/camera/constant addresses through word/store/call
services. Every acknowledged load/store/SDK primitive is retained; an unavailable
primitive has no effects. Controllers themselves may return false after child
progress, so callers retain the child rather than replaying its parent. Inputs,
provider context and source producer pass remain pinned across holds. Output
matrix publishes only after complete Inner work; successful controllers reset.

Camera4 is queried through8000f2fc, followed by camera18,c,20,14 loads and
single-precision differences/negation. Atan2 returns double and narrows BEFORE
factor/turn/bound loads. All literal reloads and ordered wrap loops remain in
source order. No reviseAngle/fmod helper, shortest-arc replacement or finite
clamp is introduced. Main reads fresh pose74 for its signed difference and
publishes exactly-1 or+1 to78 under the original asymmetric intervals.

Inner reads flag8. Clear skips camera/pose angle/reflection work, optionally
rotates incoming A and then applies common uniform scale. Set first loads
original-90, stores A+(-90) at70 BEFORE its first camera query, and reloads70
AFTER that query/atan2. Front and rear BOTH query a fresh second camera heading.
Rear adds original180 and wraps; front uses that fresh heading directly. Both
store target7c before unwrapping retained74.

Each unwrap condition reads fresh7c then fresh74; the loop body rereads74 before
its store. On the final second-loop exit, target/old/difference remain captured
across fresh flags100 and coefficient reads. Snap uses that captured target;
otherwise one SP FMA with original .1 publishes74. This recurrence runs once per
admitted Inner invocation, with no render-frame or elapsed-time multiplier.

Fresh74 is rotated, then original-1 is loaded BEFORE fresh78. Exact equality
admits Z reflection. Uniform scale follows either flag8 route and always loads
originalONE even when S==1. NaN enters source inequality gates; SDK numeric
availability remains an explicit provider. Rotation/reflection matrices are
right-concatenated to the retained input model before common scale. Copied
bounded operands preserve request order; exact Gekko/SDK numerical parity and
all original in-place arithmetic are not measured.

Wrap/unwrap backedges yield after one iteration per resume. This is native
scheduling policy, not an original total iteration bound. No successful source
loop step is repeated, and infinite/nonprogressing values remain pending rather
than being clamped or falsely completed. Owners must retain the SAME producer
pass while pumping these children, including across such yields.

## Inner integration and Main boundary

`resumeBattlePoseInner` now accepts a source pose-address admission callback and
facing services. With these supplied, phase4 executes the complete facing child
and rejoins the existing phase10. Earlier material/GX work is not replayed while
facing holds. Later Main still occurs AFTER the current reflection decision,
without refreshing previously captured resource/frame/fraction/model context.
The existing admitted unlinked typed-pool domain remains explicit. Missing
facing owners continue to hold rather than acknowledging invisible geometry.
Existing flags8-clear callers retain their prior supported path.

The Main helper is ONLY its optional facing tail. Existing `BattlePosePool::main`
still evaluates before rejecting flag8; it is not changed into a complete
retained Main by this work. A real field Main owner must retain an explicit
successful evaluation boundary before running this child, and must preserve the
earlier null-descriptor exit that skips it. Existing evaluation may itself
publish state before a later failure; blindly retrying the whole method is not
an atomic/no-effects Main provider. GQR/platform, linked/ring and broader
unsupported sampling domains remain separate.

## Actual field camera and pose binding

`AnimationFacingBinding` borrows the real VM and AnimationPoolMemory aliases,
retains the shared typed pool and semantic CameraManager, and exposes live
source-unit eye/target lanes from published field camera4. Existing
publishNativeFieldCamera is their real producer (native coordinates*100), not
yaw114 or an inferred camera heading. Camera mode2 and unpublished/nonperspective
field states are unavailable; no battle-camera bridge is substituted.

A caller supplies a native token reservation and a pure predicate proving its
ENTIRE range is unmapped by actual VM/file storage. Every boundary rechecks that
reservation, the exact field publication/pass guard and shared pool alias.
Tokens are never host or original guest pointers. Inside the reservation only
source c/14/18/20 of camera4 are readable; unavailable/unsupported reads cannot
fall through to arbitrary VM RAM. Pose address admission bounds the actual256
source slots and adds no active/resource gate; Inner's earlier typed admission
still applies independently. Atan2 and finite SDK matrix adaptation are native
numeric services, not measured original libm/FPSCR parity.

All cursors/bounded requests avoid per-invocation allocation. Bind-time shared
ownership and a stable binding address must outlive suspended children. This is
a reusable field service for future areas; normal Game has not yet attached the
full NPC Main/SetAnim/draw/GPU path.

## Validation and next work

Tests compare an explicit Main trace and every primitive hold in Main/Inner
variants, both camera samples, original bounds/reloads, snap/FMA captures,
source matrix ordering, owner/input pinning and yielding nonfinite loops. Native
fixtures use actual cold VM/shared pose aliases and the real semantic camera
publication/binding. They cover reservation collisions/no fallback, unavailable
pass, battle-mode rejection and complete Inner resumption at its later Main
boundary with material work and smoothing performed only once.

Private extraction adds original animation-local constants8041fb24+c and
8041fb6c+2c to door-data, and only the missing radians/facing ranges to the cold
animation image. Equal NPC-local literals at different addresses are not used.
Ignored read-only evidence is in `artifacts/npc-main-parent-evidence/`:
`animation-facing-exact-notes.txt`, `Main-facing-tail-review.txt` and
`Inner-facing-review.txt`. [EVT-01](../../requests/EVT-01.md) remains open.
Next retain the actual field evaluation/Main boundary, attach the shared Inner
and setter/draw services, compose real queue/GPU ownership, then activate NPCs
in normal Game. Original visual/numeric/runtime comparison and New 2DS XL
performance remain unverified.
