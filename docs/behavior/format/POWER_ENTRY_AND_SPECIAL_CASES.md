# Binary64 power wrapper and ordered special cases

US G8ME01 revision0,recovered2026-10-12. Evidence B: pow0x80272620,size0x20 and
entry of __ieee754_pow0x8026fe9c,size0x830 through0x8027015c. Whole-function
fingerprints identify both bodies;the finite logarithm/exponential/range core
beyond that boundary remains open. See [decimal approximation](DECIMAL_TO_BINARY_APPROXIMATION.md)
for pow(5,integer) callers. No original execution,FPSCR observation or host libm
accuracy/equivalence claim is made.

## Wrapper and raw input classification (B)

pow takes x in f1,y in f2. It saves/restores its frame/link state,calls
__ieee754_pow0x8026fe9c and returns f1 unchanged. No wrapper-side errno policy,
argument conversion,mode selection or fallback exists. Error/status behavior
comes from the underlying implementation and other helpers.

The implementation stores both inputs as binary64 high/low words. Let HX,LX and
HY,LY be those words;AX=HX&0x7fffffff,AY=HY&0x7fffffff. Order matters:

1. If AY|LY==0,return original binary64+1 immediately. Both signed exponent zeros
   take this route,even with a NaN base.
2. If either input has all-one exponent and nonzero fraction,return x+y using
   fadd. This is arithmetic NaN propagation,not a fixed canonical NaN word.
3. Classify y's integer parity when the original base sign bit is set,then
   perform exponent/base shortcuts described below.

No wrapper/input pointer is supplied. Floating arithmetic flags,traps,NaN
payload/quieting and signed-zero rounding effects remain original-environment
questions. The local raw tests themselves are not math-library classification
calls or string parsing.

## Integer parity for negative bases (B)

Initialize Yclass0. Only a signed-negative HX enters this classification.
If AY>=0x43400000,assign2 (even classification). For finite magnitudes this
threshold is2^53;special infinite y reaches this classification too but is
handled by its earlier-in-shortcut-order dedicated route later.

If AY<0x3ff00000,retain0. Otherwise e=(AY>>20)-1023:

| Exponent region | Test |
| --- | --- |
| 21<=e<53 | Shift LY right by52-e,then left again;require exact equality with LY. On equality,set Yclass=2-(shiftedLow&1). |
| 0<=e<=20 | Require LY0. Shift AY right by20-e,then left again;require equality with AY. On equality,set Yclass=2-(shiftedHigh&1). |

Thus reachable finite classes are0(noninteger),1(odd integer),2(even integer).
Magnitude/sign are considered separately;negative y can still be odd/even.
Positive x leaves Yclass0 even with an integer exponent because these flags are
used for negative-base handling. No arbitrary-precision integer conversion or
whole-y fctiw instruction is used by this classification.

## Exponent shortcuts with LY==0 (B)

The following tests run only when y's low word is zero and after y-zero/NaN
precedence. They use exact high-word patterns:

| y | Request/result |
| --- | --- |
| +/-infinity | If abs(x) is exactly1,return y-y via fsub. Otherwise abs(x)>1 gives y for positive y or+0 for negative y;abs(x)<1 gives -y for negative y or+0 for positive y. |
| +1 | Return x bits loaded as double |
| -1 | Return1/x using fdiv |
| +2 | Return x*x using fmul |
| +0.5 with signed HX>=0 | Call sqrt0x802728d0(x),return its result |

The +/-infinity route's exact abs(x)==1 test includes LX0. Its other comparison
uses AX's leading high word against0x3ff00000;cases with that high word and
nonzero LX are on the above-one side. The abs-one route produces arithmetic
infinity-minus-infinity rather than a hardcoded1 result. Source +1,+0 and
+infinity constants at0x80428e18..0x80428e30 are original binary64 values.

There is no shortcut here for -2 or -0.5. The half-exponent signed HX gate
excludes negative zero and all sign-bit-negative bases;those continue to later
base/general handling. sqrt's internal behavior is not recovered in this
contract. Shortcuts can bypass the general errno path below;do not infer one
universal domain-error policy from the public name pow.

## Exact base0/1/infinity handling (B)

After exponent shortcuts,compute abs(x) with fabs. If LX0 and AX is0,
0x3ff00000 or0x7ff00000,initialize local magnitude M=abs(x). If y's sign bit is
set,replace M with1/M via fdiv. If HX is nonnegative,return M.

For sign-bit-negative x:

- When abs(x)==1 and Yclass0,return (M-M)/(M-M),using fsub then fdiv.
- Otherwise when Yclass1,negate M with fneg.
- Otherwise retain M.

These literal branches include negative zero and negative infinity. In
particular the special-zero route does not apply the general negative-base/
noninteger error solely because its sign bit is set. The branch has no local
errno write. Reciprocal and zero/zero operations may affect floating status;
raw input classification is not an executed guarantee of every returned bit.

## General negative-base domain gate (B)

For inputs not returned by those shortcuts,combine base sign and Yclass. If
base is sign-bit-negative and Yclass0,write word33 to errno at0x8041ee28,
load original binary32 NaN word0x7fffffff from0x8041e3c8 into f1 and return.
The global is runtime BSS;its initial/current value is not read as an ISO
constant. This particular gate writes33 but does not clear errno on other entry
routes. NaN widening/FPSCR still needs original execution.

All other inputs continue at0x8027015c into exponent-size gates and finite
logarithm/exponential construction. Negative finite bases with integral y
are admitted;later parity-dependent result sign/range policy remains open.
No unconditional safe replacement for every negative-base case is established.

## Decimal base-five domain and remaining scope

The decimal converter calls this wrapper with x5 and a nonnegative exact integer
power argument:for negative scale it passes -Q,otherwise Q,then applies division/
multiplication and ldexp separately. At this entry,x is positive,finite,not1/0/
infinity. y0 returns1,y1 returns5,y2 computes25 by a binary64 multiply. Larger
powers pass the general core. These are exact local branch deductions,not a
complete base-five accuracy or conversion-equivalence proof.

Twenty-four anchors pin wrapper delegation,zero/NaN order,parity tests,exponent/
base shortcuts and errno gate. Direct checks pin three special double constants;
prior evidence pins the NaN source word. Whole-body identity does not imply
recovery of the remaining core. ELF/disassembly/reports remain ignored;no
original executable linking/execution,asset conversion or story rewrite occurs.

Next: all large-exponent/logarithm/exponential/range/sign regions,coefficient
bits and sqrt/scalbn dependencies. Compare actual pow(5,Q) outputs and composed
decimal/scanner/FPSCR behavior against original execution. Complete numerical/
layout/story/game documentation and requirements remain unfinished;no Switch
power behavior is inferred.
