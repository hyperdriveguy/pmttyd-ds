# Recovery effect lifecycle and Maya ownership

US GameCube G8ME01 revision0, static read-only DOL analysis. This contract
covers original local ordering, not measured Dolphin timing, native graphics
parity or a completed tutorial. Whole-body evidence is retained locally under
`artifacts/recovery-effect-agent-audit/`; `symbol-identities.json` records source
identities and hashes. Original asset rights remain unchanged; see
[asset provenance](../../ASSETS.md).

| Original body | Address | Bytes, hex |
| --- | --- | --- |
| effRecoveryEntry | 8014335c | 400 |
| effRecoveryMain | 80142bc4 | 798 |
| effRecoveryMain2 | 80142978 | 24c |
| effRecoveryDisp | 801422f4 | 684 |
| effRecoveryDisp2 | 801420fc | 1f8 |
| effRecoveryDisp3 | 80141c40 | 4bc |
| effRecoveryDisp4 | 80141828 | 418 |
| effCalcMayaAnim | 8005e288 | 154 |
| effPlayMayaAnim | 8005e3dc | 8c |
| effDeleteMayaAnim | 8005e468 | 28 |
| effMayaAnimAlloc | 8005e490 | 64 |
| effCalcMayaAnimMatrix | 8005e4f4 | 3ac |

## Constructor publishes an owned effect, not a completed recovery

Entry receives kind/amount in r3/r4 and XYZ in f1/f2/f3. It first obtains an
effect through effEntry8005ec68. Original kinds2/3 replace amount with0,
kinds4/5 with20, kind6 with11; other kinds preserve the supplied amount.
It stores name802f3c00, count=amount+1, allocates heap3 count*0x44 bytes,
and publishes payload E+c. Kind6 selects Main2; other kinds select Main.
There are no local allocation-null checks. Native admission policy must not
reinterpret a fabricated null pointer as successful original execution.

All kinds initially allocate Maya descriptor80417a44, store its owner at
root38 and call Play. Root bytes0/1 hold truncated kind/normalized amount;
XYZ are floats8/c/10, signed-half counter2c starts0, word34 captures current
GP1c, word4=255, float40=-30. Immediate SFX selects signed amount-byte
thresholds<=5, <=10, else big, with kind-specific branches. HP kind0 uses
802f3c0c/24/38; FP kind1 uses802f3c4c/64/78. Kinds4/5 use the corresponding
big variant. This is a 3D sound request using retained root XYZ, not an
invented generated tone.

Kind6 then deletes the just-created Maya owner and sets root counter180.
Particles start at root+44, stride44. Source initializes specific fields only;
it does not clear the entire payload. XYZ, velocity14/18/1c, scale20/24/28,
counter2c, state byte2 and angle30 are initialized, word4=255. Kinds4/5
set particle Y=0 for indices<10, otherwise the original constant at80422e3c.
Kind6 has separate scale/alpha/delay/random-angle initialization, including
first-particle alpha0/delay0 and later delays index*8. Whole-kind support
therefore cannot be inferred from the HP/FP amount1 constructor alone.

The portable `recovery_effect.hpp` interface admits source kinds0..5,7,8;
kind6 remains explicitly unavailable before allocation pending its distinct
Main2 owner. Suspension cursors are native service adaptations, not source
GameCube event waits. Successful callbacks acknowledge real ownership/side
effects once; an absent service must retain progress instead of replaying
allocation, sound or an earlier store.

## Main advances source elapsed ticks and retains invocation locals

Main captures root, signed kind and targetY=root40, then calls Maya Calc.
It reads unsigned current GP1c and previous root34. If current>previous,
elapsed=current-previous; otherwise elapsed=1. This advances simulation by
source ticks and is independent of display/render frame count.

The launch latch and completed count initialize once per Main invocation,
not once per elapsed iteration. Each iteration freshly reads E+c, signed
root amount and XYZ. Kinds4/5 use logical amount10. Signed root counter2c
increments with 16-bit storage and reload; values>10 set the launch latch.
Particle state byte2 is sign-extended before unsigned range checking0..6.
Each state performs one arm per particle visit; transitions do not fall through.

| State | Original action |
| --- | --- |
| 0 | Once launch-ready, initialize circular outward velocity, counter0, state1. |
| 1 | For counter<10 damp velocity before adding it to XYZ; set scale1 and increment. Otherwise set counter10/state2. |
| 2 | Decrement signed-half counter; at<=0 write0/state3. |
| 3 | For counter<40 increment, interpolate mode11 over40, compute distance/orbit and update X/Z. Otherwise consume rand(), store signed remainder by20 and enter4. |
| 4 | Decrement signed-half counter; at<=0 write0/state5. |
| 5 | For counter<16 increment and interpolate X to0 over16, Y to captured targetY over16, Z to0 over20, and scale1 to0 over16. Otherwise enter6. |
| 6 | Increment invocation-local completed count. |

The completed count accumulates across the elapsed loop, including repeated
visits to particles already in state6. It is not recomputed from terminal
states at the end. Trigonometric returns undergo original `frsp`, followed
by separate single-precision multiplies/stores. Preserve explicit binary32
operation order, signed conversions, IEEE comparisons and fused operations
where present; algebraic simplification or host `sin` substitution is not
independent source evidence.

Only kinds0/1,4/5,7/8 perform the completion comparison. Once completed>=logical
amount, kinds other than7/8 spawn stardust kind2/count2/life30 at
(rootX,rootY-20,rootZ), additional operands30/30, then issue the source
kind/amount-dependent SHINE sound. All admitted completion branches delete
Maya then delete the effect. The terminal child is a real effect request:
effStardustEntry80141808/20 delegates to effStardust2N64Entry801ecc1c/5c0.
A missing child owner cannot be replaced with a successful no-op.

If not deleted, Main freshly reads GP1c and writes root34 after all elapsed
iterations. It computes source depth and submits camera4/class1 Disp.
Kinds0/1/7/8 independently compute depth again and submit camera4/class2
Disp2; kinds4/5 instead independently compute depth and submit class2 Disp3.
Each depth call and display submission is a separate observable boundary.

## Main2 is a distinct kind6 lifecycle

Main2 copies root XYZ, decrements signed root counter only when it is<1000,
then writes alpha=counter<<3 when the reloaded counter<32. A negative counter
spawns kind2/count6/life30 stardust at root XYZ, then deletes the effect and
returns without display. Main2 does not calculate Maya, update root34 or run
Main's elapsed GP loop.

For each particle, nonzero word3c decrements and skips its remaining work.
An admitted particle adds velocity to XYZ before damping velocity. Particle1
adds alpha5 and clamps signed alpha above128. Other particles subtract8;
negative alpha resets XYZ, recomputes X/Y velocity from retained angle and
writes alpha255. The path ends with fresh depth and camera4/class2 Disp4.
Do not route kind6 through Main merely to obtain an executable effect.

## Maya ownership, timing, layout and numeric boundary

Alloc follows descriptor+8 to the channel table, reads signed count there,
allocates heap3 size18+count*34 and clears that whole allocation. Layout
offsets/sizes in this paragraph are hexadecimal (matrix30 is48 decimal bytes). It stores
the descriptor at A10. Channel matrices start A14 with stride34; the related
scalar output starts A44 with the same stride. Each matrix occupies30 bytes,
followed by that four-byte scalar; the next matrix starts A48. The descriptor
and first matrix also have their original adjacency (A10 then A14). Keep the
original addresses and callback publication order rather than flattening this
packed layout into a guessed host structure.

Play chooses GP38/3c when GP14!=0, otherwise GP40/44. These are source 64-bit
timestamps. It divides by the unsigned divisor derived from hardware word
800000f8 (`((word>>2)*0x10624dd3)>>38`), stores the quotient high/low at A8/c,
sets frame A4=0 and ORs flags bit1 without clearing bit2. Calc runs only when
bit1 is set and bit2 clear. It selects the same current timestamp branch,
converts its quotient-minus-start 64-bit bits as unsigned through the original
helper (whose symbol is named `__cvt_sll_flt`),
then separately multiplies by signed GP4 converted to binary32 and divides
by source constant8042019c. Replacing this with GP1c elapsed ticks loses
original semantics. Original unsigned-to-binary64-to-binary32 conversion and arithmetic helper semantics
must remain explicit until implemented or admitted.

Calc stores the frame, compares it with descriptor duration+4, clamps and
ORs flag2 on ordered >=. It loops freshly through descriptor+8/channel count;
on entering the body, channel table+4 supplies curve descriptors stride10.
It invokes Matrix with matrix=A14+i*34 and scalar=A44+i*34. Matrix scans
adjacent0xb2-byte key records, compares signed-half key times, decodes the
original quantized signed channels, evaluates22 channels including Hermite
terms with fused operations, builds the original transform composition and
clamps the final scalar. Non-interpolating channels select the original prior
sample. Fewer than2 keys leave outputs unchanged. Exact curve decoding,
SDK matrix/trigonometric math and rounding are independent services: an
available lifecycle wrapper does not prove those services or pixels complete.

Embedded recovery descriptor80417a44/0c is
`{0,79.0,80417a3c}`; table80417a3c/8 is `{1,80417a2c}`.
The channel descriptor80417a2c/10 references curve80371fa8/6f4 and face data
80417a28→804179fc/2a; color data is804179d8/24. These are DOL resources,
not an ISO pathname named Recovery. Inspection copies remain ignored caches.
Editable imports must follow the current assets/manifest workflow, use an
empty destination and retain per-asset provenance/license.

## Immutable addressed resources are not effect admission

`tools/recovery_effect_assets.py` imports an editable source JSON into an empty
destination and compiles its values into the bounded `PMREC001` addressed pack.
The private extraction cache and compiled pack stay outside source history;
editable sources have the current assets/manifest and per-asset provenance rules.
Nine original spans cover conversion bias, recovery vectors/SFX names, Maya keys,
dispatch tables, colors, face data, descriptor graph, Maya constants and recovery
constants. Original big-endian payload bytes and guest addresses are retained;
the small pack directory uses an explicit little-endian header. No executable
DOL body, ISO or runtime GP/clock state is fabricated by this resource pack.

`RecoveryEffectResources` validates sorted non-overlapping spans, bounded total
size, exact payload length and no trailing data. Its byte/half/word readers
resolve immutable original addresses and decode big-endian values. Loading a
valid pack supplies data only: it does not admit the Maya curve evaluator,
SDK matrix operations, effect-pool driver, texture resources, display callbacks,
sound macros or child effects. Matrix remains an explicit lifecycle callback in
`effect_maya_animation.hpp`. The complete portable evaluator, source pool and
retained continuations are now implemented by the linked
[curve/pool contract](MAYA_QUANTIZED_CURVES_AND_EFFECT_POOL_CONTINUATIONS.md).
Original-data ownership tests exercise source curves through the explicitly
installed rendering affine adapter; full Gekko numeric and native effect
admission remain separate. The portable unsigned divide rejects denominator0;
actual native clock admission must provide the original valid hardware-derived
divisor rather than relying on a guessed divide-by-zero result.

## Graphics and native integration remain separately owned

Whole portable Disp/Disp2 now preserve original texture/material requests,
geometry, matrix ordering and retained service waits; see
[draw callbacks](RECOVERY_DRAW_CALLBACKS_AND_NUMBER_TRANSFORMS.md).
Actual GPU/texture-cache/number rendering and normal-game admission remain open.

The original base bank `e/us/effect.tpl` contains118 images. Recovery Disp
uses18/20 for HP and19/21 for FP. Its original multi-texture GX material is
not an ordinary HUD icon. Disp2 constructs a Maya-transformed number matrix
and calls effNumberGX80257950 with HP style0/FP style1, amount, alpha255,
mode1; this branch uses base-bank image108. The terminal stardust renderer
uses N64 image32, from its separate bank. Original texture-cache/fallback
admission is described by the
[effect driver contract](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).

Scene4 submission can feed the top view and lower scene continuation into
the audience; it does not authorize changing original camera/material/depth
semantics. Resource ownership, camera billboarding/view, display sorting,
original sound macros, terminal child rendering, native scheduling and device
performance require their own integration/evidence. Portable service tests
prove only their covered local behavior. Native HP/FP gifts must remain held
at effect admission until an owned effect survives the constructor and has
actual admitted driver, Maya, SFX, original resources and display owners.
The complete battle/tutorial flow and optional lower-screen audience tapping
remain unfinished; physical controls must remain available.
