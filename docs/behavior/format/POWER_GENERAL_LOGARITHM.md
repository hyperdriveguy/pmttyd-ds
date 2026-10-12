# Power general logarithm normalization and compensated pair

US G8ME01 revision0,recovered2026-10-12. Evidence B: __ieee754_pow
0x80270268..0x80270450 within containing0x830-byte routine,original tables and
coefficients. See [entry](POWER_ENTRY_AND_SPECIAL_CASES.md),
[large-exponent alternate](POWER_LARGE_EXPONENT_PATH.md) and
[decimal caller](DECIMAL_TO_BINARY_APPROXIMATION.md). This covers the complete
local general-log branch;shared exponentiation/range/sign tail remains open.
No original execution or logarithm error-bound/equivalence claim is made.

## Absolute magnitude and subnormal adjustment (B)

This path uses fabs(x) stored earlier at stack+0x48. Let AX be its sign-cleared
high word. Start integer n0. If AX<0x00100000,initialize n=-53,multiply the stored
magnitude by original binary64 2^53 at0x80428e60,and reload its high word.
Otherwise retain original high word. Entry already handled exact zero/special
bases;no independent zero fallback is implemented in this region.

Let j=high&0x000fffff and e=high>>20. Set normalized high h=j|0x3ff00000,
and n+=e-1023. Preserve the magnitude's current low word. Choose reference
index k with signed comparisons of the nonnegative j:

| Fraction high bits | k / extra operation |
| --- | --- |
| j<=0x3988e | k0 |
| 0x3988e<j<0xbb67a | k1 |
| j>=0xbb67a | k0;h-=0x00100000,n++ |

Store h as the magnitude's high word and reload normalized a. Selection ignores
the low-word fraction. The last case uses a half-sized normalized exponent and
adjusts n separately;do not replace these raw boundaries with a floating
nearest-reference choice. Subnormal multiply/rounding/FPSCR behavior requires
original execution before numerical equivalence is claimed.

## Reference and coefficient identities (B)

Six doubles at0x80303c48 form three two-entry tables:

| Table / index | Bits |
| --- | --- |
| b0,b1 | 3ff0000000000000,3ff8000000000000 (1,1.5) |
| dpH0,dpH1 | 0000000000000000,3fe2b80340000000 |
| dpL0,dpL1 | 0000000000000000,3e4cfdeb43cfd006 |

Original contiguous coefficients at0x80428e60..0x80428eb0 are:

| Address | Bits | Local label |
| --- | --- | --- |
| 0x80428e60 | 4340000000000000 | subnormal scale2^53 |
| 0x80428e68 | 3fe3333333333303 | L1 |
| 0x80428e70 | 3fdb6db6db6fabff | L2 |
| 0x80428e78 | 3fd55555518f264d | L3 |
| 0x80428e80 | 3fd17460a91d4101 | L4 |
| 0x80428e88 | 3fcd864a93c9db65 | L5 |
| 0x80428e90 | 3fca7e284a454eef | L6 |
| 0x80428e98 | 4008000000000000 | three |
| 0x80428ea0 | 3feec709e0000000 | cpH |
| 0x80428ea8 | be3e2fe0145b01f5 | cpL |
| 0x80428eb0 | 3feec709dc3a03fd | cpFull |

Labels express the local arithmetic roles,not independent measured constants.
Use the exact words when comparing a reimplementation. Signed n converts to
double via xoris/bias0x4330000080000000 at0x80428f20 and fsub. No frsp or integer
floating-conversion instruction narrows that value. Complete table/coefficient
hashes pin these reused binary data regions.

## Ratio and low-word compensation (B)

Let D(expr) be one binary64 arithmetic result,FMA(a,b,c) a fused a*b+c,and
NMS(a,b,c) a fused c-a*b. Let HI(z) clear the stored double's low32 bits and
reload it as a double. HI is not a single-precision conversion. Preserve the
following operation order with b=reference[k]:

```
den = D(a+b)
u = D(a-b)
recip = D(1/den)
s = D(u*recip)
sh = HI(s)
dhBitsHigh = ((h >> 1) | 0x20000000) + 0x00080000 + (k << 18)
dh = double_from_words(word32(dhBitsHigh), 0)
dl = D(a-D(dh-b))
r = NMS(sh,dh,u)
r = NMS(sh,dl,r)
sl = D(recip*r)
```

The synthetic denominator high word is constructed by integer operations;its
low word starts0 through a stored original positive-zero double. It is not the
result of HI(den) as an interchangeable mathematical assumption. h is the
normalized high word after any exponent reduction. Fused negative operations
retain product/subtraction compensation;host compiler reassociation can change
this sequence.

## Polynomial and product pair (B)

Continue with the original fused Horner order:

```
s2 = D(s*s)
p = FMA(L6,s2,L5)
p = FMA(s2,p,L4)
p = FMA(s2,p,L3)
p = FMA(s2,p,L2)
p = FMA(s2,p,L1)
p = D(D(s2*s2)*p)
p = FMA(sl,D(sh+s),p)
sh2 = D(sh*sh)
th = HI(D(D(3+sh2)+p))
tl = D(p-D(D(th-3)-sh2))
prodH = D(sh*th)
prodL = FMA(sl,th,D(s*tl))
ph = HI(D(prodH+prodL))
pl = D(prodL-D(ph-prodH))
```

No standard log/log2 call occurs in this local branch. All additions/products
shown as D are separate original instructions;the indicated FMA/NMS boundaries
are fused. The scratch high/low splits use integer zero stores into low words.
This arithmetic contract is not a proof that a rounded host polynomial or
another fdlibm build produces identical results under the target FPSCR.

## Final logarithm pair and tail handoff (B)

Use the selected table corrections and signed-integer n double:

```
zH = D(cpH*ph)
zL = FMA(cpL,ph,D(cpFull*pl))
zL = D(dpL[k]+zL)
total = D(zH+zL)
total = D(total+dpH[k])
total = D(n+total)
hi = HI(total)
r = D(hi-n)
r = D(r-dpH[k])
r = D(r-zH)
lo = D(zL-r)
```

Leave hi at stack+0x30 and lo in f0,then enter common tail0x80270450. That tail
also accepts the alternate large-exponent pair,combines y/sign and constructs
the returned power. hi/lo are intermediate log-related values,not x^y. The
local path contains no helper call or errno write;underlying arithmetic status
and subsequent helpers are separate obligations.

## Evidence and remaining scope

Twenty-four anchors pin subnormal scaling,reference thresholds,synthetic high
word,ratio/fused compensation,polynomial evaluation and final splits. Read-only
checks pin all six table entries,eleven scale/coefficient doubles and the signed
conversion bias. Prior whole-helper fingerprint identifies this body;inspection
ELF/disassembly/reports remain ignored. No original executable linking/execution,
asset conversion,message rewrite or licensing change occurs.

Next: common y/log multiplication,overflow/underflow gates,exponential polynomial,
exponent reconstruction/sign and helper dependencies;executed pow(5,Q)/general
power/FPSCR and composed scanner/converter comparisons. Continue all uncovered
text/story/field/battle/audio/save/content before complete-game requirements.
Full goal remains incomplete;no Switch power behavior is inferred.
