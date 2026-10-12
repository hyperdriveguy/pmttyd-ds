# Power large-exponent gates and near-one log pair

US G8ME01 revision0,recovered2026-10-12. Evidence B: __ieee754_pow
0x8027015c..0x80270268 within containing0x830-byte routine,original coefficients
and selected instructions. See [entry/special cases](POWER_ENTRY_AND_SPECIAL_CASES.md)
and [decimal caller](DECIMAL_TO_BINARY_APPROXIMATION.md). This covers one branch
of the finite core;general logarithm and common exponentiation/range/sign tail
remain open. No original execution or numerical-accuracy claim is made.

## High-word admission and immediate returns (B)

Reuse AX=abs-base high word and AY=abs-exponent high word from the entry contract;
HY is the signed original y high word. These comparisons ignore both low words.
If AY<=0x41e00000,skip this entire path and enter general logarithm at0x80270268.
The threshold corresponds to the high word of2^31,but the predicate is not an
exact full-double |y|>2^31 compare:all values sharing its high word skip the path.

For AY>0x41e00000,perform the following ordered high-word gates:

| Order | Condition | Return |
| --- | --- | --- |
| 1,only if AY>0x43f00000 | AX<=0x3fefffff | +infinity if HY<0,otherwise+0 |
| 2,same extreme-y block | AX>=0x43f00000 | +infinity if HY>0,otherwise+0 |
| 3,all admitted large-y cases still live | AX<0x3fefffff | +infinity if HY<0,otherwise+0 |
| 4,remaining cases | AX>0x3ff00000 | +infinity if HY>0,otherwise+0 |
| 5 | None of the above | Near-one polynomial below |

In order2,the same0x43f00000 literal is compared to **AX**,not a high word of1.
Do not replace that raw condition with an assumed fdlibm source spelling.
Both signs of zero y and infinite/NaN y were handled earlier,so the retained
ordinary finite y is nonzero. Branches load original positive zero/infinity
constants directly and return;there is no local errno write or floating
huge*huge/tiny*tiny expression on these immediate paths.

For AY in(0x41e00000,0x43f00000],the polynomial receives AX in the inclusive
high-word window[0x3fefffff,0x3ff00000]. For larger AY,order1 excludes the lower
endpoint too. This describes raw-word coverage,not measured approximation
accuracy or a full low-word mathematical neighborhood bound.

## Original-base reload and coefficients (B)

The polynomial reloads the original x double from stack+8,not fabs(x) stored
at stack+0x48. Set t=x-1 via binary64 fsub. Admitted negative bases were already
classified as integral-y domain cases by entry;the high-word window used abs
bits but this arithmetic uses the signed x. Do not silently replace it with
abs(x)-1. Exact abs(x)==1 shortcuts ran earlier;other negative near-unit bases
can still reach this local sequence depending on y and the raw gates.

Constants at0x80428e30..0x80428e58 are six original binary64 words:

| Address | Bits | Role |
| --- | --- | --- |
| 0x80428e30 | 3fe0000000000000 | half |
| 0x80428e38 | 3fd5555555555555 | third approximation |
| 0x80428e40 | 3fd0000000000000 | quarter |
| 0x80428e48 | 3ff7154760000000 | H coefficient |
| 0x80428e50 | 3e54ae0bf85ddf44 | L coefficient |
| 0x80428e58 | 3ff71547652b82fe | full coefficient |

The last three are split/full reciprocal-log constants by mathematical role;
exact bit patterns and operation boundaries are the porting evidence. Rounded
printed decimal constants or a generic log2 call are not established substitutes.
The whole48-byte region has a pinned hash in the independent verifier.

## Arithmetic sequence and high/low pair (B)

Let D(expr) denote one original binary64 arithmetic result boundary. Let FMS(a,b,c)
be fused multiply-subtract a*b-c and NMS(a,b,c) be fused negative multiply-subtract
c-a*b. Preserve this order:

```
t = D(x-1)
a = NMS(quarter, t, third)
b = NMS(t, a, half)
sq = D(t*t)
u = D(sq*b)
w = D(fullCoefficient*u)
p = FMS(Hcoefficient, t, w)
v = D(Lcoefficient*t)
combined = D(v+p)
hi = bits_to_double(double_bits(combined) & 0xffffffff00000000)
err = D(hi-v)
lo = D(p-err)
```

The clear-low-word operation is a store of the combined double followed by an
integer zero store to its low word and a double reload. It is not frsp,decimal
rounding or an exponent-dependent mantissa cap. The fused steps are original
fnmsub/fmsub instructions;independently rounding their product before addition/
subtraction changes the recovered sequence. t is also stored locally,but this
path performs no helper call.

Leave hi in stack+0x30 and lo in f0;branch to common tail0x80270450. That tail
combines the pair with y and result sign/exp/range policy,which is still open.
These intermediates are not the returned x^y value,and a log-pair label is not
an error-bound or correct-rounding proof. Original FPSCR/denormal/NaN behavior
must be established by execution.

## Caller intersection and validation scope

The ordinary decimal scanner hands __dec2num at most20 retained digits and a
signed16 record exponent. Its initial Q is signed16,then at most19 digit positions
are subtracted in chunk preparation. Hence the pow exponent magnitude used by
that scanner's base-five path is at most32787 under the recovered count domain,
well below this large-y gate. This is a derived caller bound,not measured input
frequency or a bound on every other game's pow caller. Recover the general
logarithm/common tail to close that actual decimal dependency.

Twenty additional anchors pin high-word tests,return requests,original-base
reload,fused polynomial,low-word clearing and compensated pair handoff. Prior
whole-helper identity pins the containing body;the coefficient hash checks all
six constants. Original ELF/disassembly/reports remain ignored. No executable
linking/execution,asset conversion,message rewrite or licensing change occurs.

Next: general logarithm normalization/coefficient path and common exponentiation/
range/sign tail,then helper dependencies and executed base-five/general power/
FPSCR corpus. Continue numerical/scanner composition and all uncovered text/
story/field/battle/audio/save/content before complete-game requirements. Full
objective remains incomplete;no Switch power behavior is inferred.
