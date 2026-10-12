# Power common exponentiation, range gates and final scaling

US G8ME01 revision0,recovered2026-10-12. Evidence B: __ieee754_pow
0x80270450..0x80270694 within containing0x830-byte routine,original coefficients
and selected instructions. See [entry/sign classification](POWER_ENTRY_AND_SPECIAL_CASES.md),
[general logarithm](POWER_GENERAL_LOGARITHM.md),[large-y alternate](POWER_LARGE_EXPONENT_PATH.md)
and [ldexp service](FORMAT_BINARY_NUMERIC_SERVICES.md). This completes recovered
local power control/arithmetic flow;helper execution and numerical equivalence
remain unverified. No original code was executed or linked into the native port.

## Result sign and compensated y/log product (B)

Both log paths supply hi at stack+0x30 and lo in f0. The retained base-sign
indicator and Yclass select result multiplier S:negative base with odd-integer
Yclass1 uses original double-1;all other admitted paths use+1. This decision
follows the earlier negative/noninteger domain gate;it is not applied to every
entry shortcut that already returned.

Let D be one binary64 result boundary,FMA fused a*b+c,and HI clear the low32
bits of a stored double. Reload original exponent y and compute:

```
yh = HI(y)
yl = D(y-yh)
a = D(y*lo)
pH = D(yh*hi)
pL = FMA(hi,yl,a)
z = D(pL+pH)
```

Preserve pH at stack+0x40,pL in f12 and z high/low words ZH,ZL. This is not a
single host multiply of y by hi+lo;the distinct arithmetic and fused boundary
are part of the recovered behavior. Raw sign/exponent gates below classify z,
not the mathematical exact y*log2(x).

## Overflow and underflow requests (B)

First compare signed ZH against0x40900000,the high word of+1024:

- If ZH>=0x40900000 and z is not exactly bits0x4090000000000000,return
  D(huge*D(huge*S)).
- At exact+1024,return that overflow expression only if ordered
  D(overflowCorrection+pL)>D(z-pH). Otherwise continue to reduction.

For remaining z,if (ZH&0x7fffffff)>=0x4090cc00,the high magnitude word of1075:

- Unless bits equal0xc090cc0000000000 (exact-1075),return D(tiny*D(tiny*S)).
- At exact-1075,return that underflow expression when ordered pL<=D(z-pH).
  Otherwise continue.

Both gates use original word comparisons and equality tests. They do not merely
compare a host power result to DBL_MAX or zero. Huge/tiny requests are arithmetic,
not loading a fixed signed infinity/zero result;FPSCR/rounding/traps affect the
executed bits and flags. There is no local errno store on these gate returns.
The wrapper does not add an error policy. The later ldexp service can have its
own effects,which must be composed separately.

## Exact coefficients and constants (B)

Thirteen binary64 words at0x80428eb8..0x80428f18 are pinned as a complete region:

| Address | Bits | Role |
| --- | --- | --- |
| 0x80428eb8 | bff0000000000000 | negative result multiplier |
| 0x80428ec0 | 7e37e43c8800759c | huge (1e300) |
| 0x80428ec8 | 3c971547652b82fe | overflowCorrection |
| 0x80428ed0 | 01a56e1fc2f8f359 | tiny (1e-300) |
| 0x80428ed8 | 3fe62e4300000000 | lgH |
| 0x80428ee0 | 3fe62e42fefa39ef | lgFull |
| 0x80428ee8 | be205c610ca86c39 | lgL |
| 0x80428ef0 | 3fc555555555553e | P1 |
| 0x80428ef8 | bf66c16c16bebd93 | P2 |
| 0x80428f00 | 3f11566aaf25de2c | P3 |
| 0x80428f08 | bebbbd41c5d26bf1 | P4 |
| 0x80428f10 | 3e66376972bea4d0 | P5 |
| 0x80428f18 | 4000000000000000 | two |

Labels express arithmetic roles;no measured fit/error bound is inferred. Use
exact bits rather than rounded printed coefficients for a fidelity comparison.

## Integer exponent reduction (B)

Start integer n0. If (ZH&0x7fffffff)<=0x3fe00000,leave pH unchanged and skip
integer extraction. Again this is a high-word predicate,not an exact |z|<=0.5
comparison including ZL. Otherwise execute the original integer bit sequence:

1. e=(ZH>>20)&0x7ff. Set roundedHigh=word32(ZH+(0x00100000>>(e-1022))).
   The original uses arithmetic shifts of positive constants.
2. k=((roundedHigh&0x7fffffff)>>20)-1023.
3. integerHigh=roundedHigh & ~(0x000fffff>>k). Store it with low word0,
   forming signed integer-valued double T.
4. Extract n=((roundedHigh&0x000fffff)|0x00100000)>>(20-k),and negate n
   when signed ZH<0.
5. Set pH=D(pH-T).

These shifts operate in the already range-gated domain. Do not replace the
sequence with a rounding-mode-sensitive host round/nearbyint call or alter its
half-boundary policy. The original low word ZL does not participate in this
integer extraction. n is kept in general register r3;offset=word32(n<<20) in r4
is later used for exponent reconstruction.

## Natural-exponent pair and exponential polynomial (B)

Using reduced pH and unchanged pL:

```
t = HI(D(pL+pH))
e = D(pL-D(t-pH))
u = D(lgH*t)
v = FMA(lgFull,e,D(lgL*t))
a = D(u+v)
w = D(v-D(a-u))
sq = D(a*a)
p = FMA(P5,sq,P4)
p = FMA(sq,p,P3)
p = FMA(sq,p,P2)
p = FMA(sq,p,P1)
p = D(sq*p)
c = D(a-p)
r = D(D(a*c)/D(c-2))
correction = FMA(a,w,w)
r = D(r-correction)
result = D(1-D(r-a))
```

This is the complete local polynomial/reconstruction sequence,including fused
Horner steps and compensated correction. lgH,lgFull,lgL are different constants:
the original first low product uses lgL,not lgFull. HI is an integer zero store
into the low word,not frsp. No generic exp/exp2 call replaces this polynomial.
The recipe preserves boundaries;it is not a numerical-accuracy guarantee under
unconfigured host compiler/FPSCR semantics.

## Binary exponent insertion or ldexp and final sign (B)

Store result as binary64 with high word RH. Compute signed target exponent
indicator=(word32(RH+offset) arithmetic-shift-right20). If that indicator>0,
add offset to the stored high word directly,keeping the low word unchanged.
There is no local upper exponent clamp on this branch beyond the earlier z
gates. If indicator<=0,call ldexp0x80272150(result,n),store its returned double.
This includes subnormal/underflow reconstruction domains;the already-recovered
service's arithmetic/range/errno behavior is a dependency.

Reload the scaled result and return D(S*result). Sign application is a floating
multiply by+1/-1,not an unconditional bit OR or fneg. All local early gate returns
branch to the common epilogue;normal scaling reaches this final multiplication.
Original flags,NaN/denormal effects and exact power bits still require execution.

## Coverage and validation obligations

Entry/special,large-y,general-log and this contract now cover the local pow/
__ieee754_pow bodies. sqrt invoked by the half-exponent shortcut remains an
unrecovered helper here;ldexp has a local contract but composed execution is
unverified. Decimal pow(5,Q) depends on this general tail for larger Q,then
applies its own scaling/refinement;host-libm agreement is not presumed.

Thirty-two new anchors pin sign/product splitting,range gates,integer extraction,
fused polynomial,exponent insertion/ldexp and final multiplication. A complete
104-byte hash pins all thirteen constants. Inspection disassembly/reports remain
ignored;no original executable execution/linking,asset conversion,message rewrite
or licensing change occurs.

Next: sqrt dependency and executed pow(5,Q)/general-input/FPSCR/returned-bit
corpus,then composed scan/converter/record-distance validation. Continue all
uncovered text/story/field/battle/audio/save/content before complete-game
requirements. Full goal remains incomplete;no Switch power behavior is inferred.
