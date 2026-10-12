# Ordinary overworld Mario movement: state ownership and air control

US G8ME01 disc0 revision0, recovered 2026-10-12 from the read-only local ISO.
This is static binary evidence. No original gameplay execution, measured input
latency, hardware timing or numerical equivalence is claimed. The contract covers
ordinary Mario, scale1, no movement badge, no transformation, no external motion,
no conveyor/additive displacement, and ordinary eligible input. Named special
branches below remain separate dependencies, not default behavior.

Reproduce fingerprints and ordering anchors with
`python3 tools/verify_overworld_mario_movement.py`. The verifier pins nineteen
containing bodies, twenty-nine instruction anchors and six constants. A containing
body fingerprint does not imply complete recovery of every branch in that body.
Inspection-only ELF/listings are `/tmp/overworld-*.elf`/`.txt` and existing ignored
`artifacts/reference/marioMain.disasm`/`marioMoveMain.disasm`.

## Storage and cadence

The following roles are established by these bodies; names in public headers
are not evidence for field interpretation.

| Mario storage | Role in this slice |
| --- | --- |
| +0x2e, unsigned16 | Current motion; 0 idle, 1 walk, 2 dash, 3 ordinary jump, 10 fall |
| +0x30, unsigned16 | Previous motion retained by change requests |
| +0xc bit0 | Pending motion entry; dispatcher repeats while this remains set |
| +0x44/+0x48 | Motion phase and countdown |
| +0x50/+0x52, signed16 | Motion counter and separate recorded ascent transition counter |
| +0x180 | Retained base horizontal speed |
| +0x184/+0x188 | Base walk/dash speeds; ordinary Mario init writes1/2.25 original units |
| +0x18c | Computed combined movement speed, written by movement body |
| +0x190 | Jump-entry snapshot of +0x180; not the main movement speed source |
| +0x194 | Current stick magnitude, capped70 by marioGetStick |
| +0x198 | Current input heading |
| +0x1a0 | Retained heading fallback used by zero-magnitude stick query |
| +0x1a4 | Retained travel heading used with +0x180 |
| +0x228 | Mario scale multiplier |
| +0x252/+0x253, signed8 | Field stick axes |

`marioMain` ordinary control route orders key publication
(0x8005abdc), current input heading/magnitude query (0x8005abe8), then
`marioMotion` (0x8005abf0), before movement. `marioMotion` at0x8009461c invokes
current motion through the table at0x80310030 and loops while pending bit0 is set
(0x80094664). `marioChgMot` saves previous/current motion, requests previous
motion's post callback if present, then sets pending entry. There is no general
one-tick deferred motion transition here.

The ordinary grounded branch later calls `marioMove` at0x8005b0cc. Ascent calls
`marioJump` at0x8005affc, overhead check at0x8005b000, then `marioMove` at0x8005b004.
Falling calls move at0x8005b04c before its gravity/update path. Do not place all
motion arbitration after movement. System/key/action/transform gates can route
elsewhere; this ordering does not authorize applying it through stopped modes.

## Ground walk and dash

`marioChgMotAuto` uses exactly zero signed axes for idle, otherwise squared
radius strictly greater than3025 for dash when badge0x143 is absent. Equality
3025 belongs to walk. Walk's threshold branch at0x80087c38 requests motion2;
dash's inverse branch at0x80087910 requests motion1 when radius<=3025 or that
badge is present. This ordinary threshold has no additional numerical hysteresis.

Walk and dash entry blocks consume pending bit0 and initialize +0x180 only if
signed+0x4e==0. Their ordinary values are walk/dash base times scale. A background
mode query equal1 halves the selected base. In Paper-mode flag0x00200000, dash's
speed query selects walk base and halves it when squared radius<=3025. These
special paths must not be conflated with ordinary Mario's rates.

The walk/dash body keeps that base speed between entries. Current input heading
is written to travel+0x1a4 by walk at0x80087c4c or dash at0x80087934, after their
threshold early-return branches. Idle clears base+0x180, except its explicit
external-motion flag path. Same-call dispatcher repetition applies ordinary
idle→walk/dash entries before that call's horizontal movement; there is no
ordinary startup displacement delay inferred from entry-only speed writes.

Jump requests occur before some later transform/threshold logic in grounded
bodies. The walk call at0x80087c04 and idle call at0x800a7120 do not immediately
return based on the jump helper result. A later motion request can replace a
pending jump. Complete simultaneous jump+threshold-change/idle-start arbitration
requires the full called-service/flag graph and original trace; do not assume a
simple universal priority based solely on these local sites.

## Stable-motion jump entry and startup

When an ordinary jump request survives arbitration, same-call dispatcher
repetition invokes `mot_jump` at0x8009b480. Entry clears pending bit0, sets phase0
and countdown2, resets signed motion counters, sets jump flag0x00010000 and
clears fall flag0x00020000. It loads the jump tuple and copies position, then
copies retained base+0x180 to independent takeoff+0x190 at0x8009b538–53c.
It does not replace retained speed or travel heading with the current stick.

The same invocation decrements countdown2→1 and increments counter0→1.
`marioJump` skips vertical integration while phase<10. Thus launch-call ascent
is zero, while launch-call horizontal movement already uses the jump flags and
retained base/heading. The next motion invocation decrements countdown1→0,
sets phase10, increments counter1→2, and allows first vertical integration.
The phase gate is explicitly `phase<10` at0x80093d84–38c. Counter+0x50 is
queried only later at0x80093dfc–e10 to gate released-A shortening at counter>=4;
there is no counter<3 integration gate in this ordinary body. The existing native
FieldJump `ticks<3` startup is inconsistent with this local composition and
requires a separately scoped correction/test. These are manager-invocation
counters, not an observed render-frame latency.

A semantic `begin()` whose counter1 already represents the launch invocation
must not consume another vertical tick inside that same invocation. The native
launch request must precede horizontal movement/contact policy while preserving
this two-count startup. Falling motion does not locally rewrite +0x180/+0x1a4;
a support-loss path can retain its preceding ground travel, subject to its other
owners/gates. Fall-off-ledge/full transition observation remains requested.

## Plain air horizontal movement

`marioMoveMain` at0x800594a8 computes an independent current-input drift, combines
it with retained travel, computes combined heading/norm, then invokes collision
movement with that heading and norm. In the ordinary jump/fall domain with
nonzero motion counter, scale1, flags0x80/0x00200000/0x02000000 clear, badge0x143
absent, and no additive/conveyor/slope effects:

1. Let `m=min(sqrt(sx*sx+sy*sy),70)` from the current stick query. Drift starts
   with `single(m*scale)` at0x80059658, then multiplies original float0.015
   (0x804200a4) at0x800596d4 and float0.25 (0x804200ac) at0x800596e0.
2. The original degree-to-XZ helper constructs drift along current input+0x198.
   It separately constructs retained travel from base+0x180 and travel+0x1a4,
   then combines X and Z using fused additions at0x80059798/79c.
3. Compute the combined norm and heading. If ordinary air flags0x00030000 are
   present, and Paper flag0x00200000 is clear, cap norm to
   `single(baseDash+0x188 * scale)` (0x8005994c–960).
4. The separately queried movement-rate service can further scale this norm
   (0x8005996c–974). The plain examples below assume its result1. Computed
   norm is retained at+0x18c and collision movement receives norm and heading.

Retained base is not multiplied by current stick magnitude. Stick release sets
drift0 but leaves retained travel: a running jump continues horizontally when
the stick returns to neutral. Opposite stick adds opposite drift, reducing or
turning the combined vector; it does not replace travel with an immediate opposite
walk/dash vector. +0x190 snapshot is used by another fall consumer and must remain
independent of +0x180.

At scale1, plain full stick m70 produces drift0.2625 original units. Running
base2.25 plus same-direction full input caps to2.25; opposite full input gives
1.9875 in the retained direction. Walking base1 plus same-direction full input
gives1.2625; neutral input leaves1. Native geometry scale0.01 maps these to
0.0225/0.019875/0.012625/0.01 respectively. These arithmetic examples are derived
from static operations; they are not measured distances or exact Gekko numerical
comparison results. Binary32/FMA/trigonometric/square-root services remain their
own equivalence boundary.

## Remaining dependencies

Physical SDK stick clamping/deadzone and native Circle Pad→signed-axis conversion
are separate from this field consumer. `marioUpdateKeyData` copies GP axes, negates
GP Y, applies field gating/overrides, and publishes masked held/trigger/repeat/
release halfwords. It does not locally invent a .15 radial deadzone. The source
SDK producer contract must supply the effective stick inputs.

Recover special modes, badge/background slowdown, support-derived movement-rate,
conveyors/additive motion, field lock transitions, airborne wall deflection and
full idle/jump/transform arbitration before claiming complete field equivalence.
Runtime observations remain needed for ordinary launch, release/reversal,
walk→dash+jump boundary, landing and ledge loss. Requests are tracked in
[the dedicated overworld documentation inbox](../OVERWORLD_DOCUMENTATION_REQUESTS.md).

## Five-probe orientation clarification

The search probes preserve a different heading from combined XZ displacement.
`marioSearchHead` tests base+0x180!=0 (0x8008c240–248): nonzero selects retained
travel+0x1a4 (0x8008c24c), while zero selects facing+0x1ac through `toMovedir`
(0x8008c254–258). `marioChkLandon2` uses the same nonzero-base/travel versus
zero-base/facing selection at0x8008ce7c–cea4 before constructing its five probes.
`marioSearchGround` also tests base+0x180; the nonzero path reads travel+0x1a4,
subtracts heading offset+0x19c, then calls `revise360` (0x8008e7b4–7cc). Zero uses
facing+0x1ac through `toMovedir` (0x8008e7d4–7d8). Motion19 instead reads+0x1b4.

In ordinary nonzero-base air travel with heading offset0, head/landing/ground
footprints therefore keep retained travel direction even when current stick or
combined movement points elsewhere. The native probe-direction owner should
remain separate from current input and the air helper's combined displacement.
Facing conversion, heading offset and motion19 are separate owners; this local
branch recovery does not establish their producers or full collision equivalence.
