# Original atomic unit rotation-offset event

This owner implements complete btlevtcmd_SetRotateOffset80110f9c/128 using the
existing complete BtlUnit_SetRotateOffset80128604/34 implementation in
setPackedUnitVector. Source pins include both bodies, complete unit registry
800f88cc/3c, signed conversion bias802ee608/8 and sentinel804226c4/4.
Original code/data bytes remain outside source history.

The wrapper resolves argument0 with integer evtGetValue and translates its actor
selector. It resolves X, captures raw Y, converts X; resolves captured Y,
captures raw Z, converts Y; resolves captured Z, captures Battle globally, then
converts Z and calls the actual unit registry using that captured Battle.
No evtGetFloat, early null-unit success, angle normalization or rendering occurs.
The original private4330/xoris conversion consumes the actual fresh bias and
platform double subtraction/float rounding. A tiny additive shared-owner alias
exposes that already implemented conversion; no second numeric implementation
is introduced.

The existing leaf compares each supplied float independently against a fresh
read of the original float sentinel. Unequal values write unit+78/+7c/+80 in
axis order. A store callback which legitimately changes the source sentinel
affects later axes; no captured common sentinel replaces those reads. No unit
position/height or current rotation field is implicitly modified.

This is an atomic admitted-domain source helper, matching the existing rotation
owners. Complete actual EVT, memory and numeric domains must remain valid for
the whole call. There are no async callbacks or partially acknowledged service
results to retry. Native Packed checks both actual double-subtract and float-
round providers before calling the wrapper; missing providers return unavailable
before any source argument access/write. Admitted Packed memory is its existing
required mapping invariant, never a missing-read-as-zero policy.

Tests check the exact argument/read/conversion/registry/store trace, source raw
operand captures across numeric callbacks which mutate later operands, Battle
capture before final conversion, sentinel mutation between stores and signed
integer float rounding. An actual EventNativeRuntime/HarborTutorialPacked fixture
with an unmapped command domain proves missing math holds before entering the
body, including either required provider missing independently. Strict host,
ASan/UBSan and ARM syntax cover the new owner/test and both journaled old callers.
Only the existing Packed TU's baseline indentation warning is demoted. Actual
lethal source continuation is tested separately with coherent15584 headers/core
plus frozen Smoke/Coin/EXP/StarOffset/StarPoint overlays; native GPU/presentation
proofs there remain explicitly TESTONLY. Root fresh make test/make all and native
checks remain required. No victory, pouch reward or field return is fabricated.
