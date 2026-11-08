# Retained original Mario direction event

`SourceMarioSetDirection` implements complete US `evt_mario_set_dir800e6de8/1e0`,
owning complete retained `revise36080096564/34` and
`toMovedirSimple800961f8/cc` children. The exact whole integer
`sysMsec2Frame800059cc/28` is inlined at its original call boundary. Numeric
instructions remain typed source operations; no guessed heading/width or
millisecond presentation tween replaces these bodies.

## Actual arguments, state and result

Each original invocation captures E+18 argument pointer and Mario through
8041e900. It resolves requested angle with evtGetFloat80030840, duration with
evtGetValue80031260, converts duration using fresh global80415fd0→GP+4, then
resolves mode with evtGetValue. Integer conversion preserves low32 signed
multiply by source Hz, signed multiply-high by10624dd3, arithmetic shift6 and
sign correction. It is not an unbounded ms*Hz/1000 expression.

First visit loads actual270 at80421b1c, zeros event+78/+80, copies actual current
Mario+1ac to event+7c, and subtracts actual360 at80421b44 only if requested yaw
is greater than270. Original first status is captured through inner holds.
A fresh finish-latch1 at E+80 loads source zero80421adc, writes travel+1a4 and
facing+1a0 zero and returns2 before any interpolation.

For positive duration, revise360(270-target) is adjusted by minus360 if greater
than270. Source interpolates the raw target minus retained initial yaw at the
current counter/duration ratio. This is deliberately not shortest-arc motion.
Separate source single subtractions, conversions, division, multiplication and
addition remain distinct numeric requests. It normalizes yaw, writes+1ac,
reads+1ac afresh into+1b0, reads+1ac again for toMovedirSimple, writes+1a4,
reads+1a4 afresh and writes+1a0. Count+78 is then read afresh and incremented.
Incremented count greater than duration with mode0 returns1; otherwise returns0.
With nonzero mode the exceeded case first stores latch1 and returns0; the next
original invocation takes the finish-latch branch and returns2.

Nonpositive duration still computes and normalizes270-target and performs the
same fresh source angle/derived-direction reads and writes, returning2. A whole
completed result0 must be reset only before the next actual VM/simulation visit;
nullopt means an inner hold in the same invocation, with no repeated prefix.

## Complete direction children

revise360 reads source360 and zero, repeatedly adds360 below zero, reloads360
and repeatedly subtracts at or above360. Actual NaN comparison behavior is left
to the source comparison and numeric services. A native work budget yields every
4096 internal phase visits without replay; it does not pretend a nonterminating
source loop completed.

toMovedirSimple independently captures fresh Mario and reads actual270,
Mario camera-heading+19c, then performs(270-input)+heading. It chooses actual
+0.5 or-0.5, reads actual quantization1000, performs one source single fused
multiply-add and fctiwz, converts through actual double bias802c3ec0, divides by
1000 and wraps with the original loops. It may observe a different Mario from
the parent: its copied result is still stored on the parent's captured actor.
No nearest-cardinal-angle substitution is permitted.

## Native binding and source frontier

Root supplies stable actual VM memory for event fields/operands and real typed
Game fields at Mario+19c/+1a0/+1a4/+1ac/+1b0. Canonical camera-heading history
must exist; do not infer it from screen projection or visual facing. Original
read-only constants and both signed-conversion biases must be present. Numeric
services must implement the actual finite native/source domain or hold; host
tests use an explicit IEEE fixture and do not claim arbitrary Gekko FPSCR parity.
Source event get-value/get-float callbacks retain their own partial work.

The actual original beforebattle SetDir USER_FUNC is905065e4 with operands
(90,100,0);905065e0 is the preceding opcode21. Source-byte inspection shows next
USER_FUNC `evt_npc_set_ry80050988` at905065f8 with original NPC name and270,
then WAIT200 and `evt_eff_fukidashi8006c3c4` at90506610. This is static original
script evidence, not a native fresh-script completion receipt. These remaining
NPC/effect/pose families are not acknowledged here. No Game/main routing edits
or changes to the existing legacy adapter are part of this package.

## Lifetime and verification

Actual host-current is checked before event identity access and each source
step. Event pointer/address/ID/command, user/callback and full shared lifetime
identity remain fixed. Current loss poisons, recursion/reset while held rejects,
false stores mean no effects. The source services are stationary during each
single-threaded callback. Missing data stays unknown at its first actual use.

Independent immediate references compare complete traces for every external
boundary held twice, positive/nonpositive/wrapped duration, both mode branches,
first/poll visits, actual constants and late angle/global-pointer/travel mutations.
Sequential real polls verify6-frame duration completes on the seventh original
invocation, and the nonzero-mode extra cleanup visit. Strict host, ASan/UBSan,
ARM syntax and complete-body/actual-constant read-only ISO pins pass. Root owns
actual Game field admission, tick cadence and full native script validation.
