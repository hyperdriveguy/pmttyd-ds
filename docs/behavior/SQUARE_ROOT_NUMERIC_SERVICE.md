# Binary64 square root, digit extraction and integer rounding

US G8ME01 revision0,recovered2026-10-12. Evidence B: sqrt0x802728d0,size0x20 and
complete __ieee754_sqrt0x80272640,size0x224,original instructions/constants.
See [power half-exponent shortcut](POWER_ENTRY_AND_SPECIAL_CASES.md). No
original execution,FPSCR/error observation or host sqrt equivalence is claimed.
The implementation is integer digit extraction,not a hardware fsqrt instruction.

## Wrapper and ordered special cases (B)

sqrt receives/returns binary64 f1. Its wrapper saves frame/link state,calls
__ieee754_sqrt and returns unchanged;it adds no errno handling or conversion.
The core stores input as high word H and low word L. Order is:

| Condition | Behavior |
| --- | --- |
| Exponent bits all ones | Compute fused x*x+x in f1,write errno33,return |
| Signed H<=0 and (H&0x7fffffff)\|L==0 | Return original f1,both signed zeros preserved |
| Remaining signed H<0 | Write errno33,load binary32 NaN0x7fffffff into f1,return |
| Other positive finite inputs | Integer normalization/extraction below |

The first branch includes positive infinity,negative infinity and all NaNs.
It tests exponent without separately excluding infinity from errno. Its fmadd
may produce infinity,NaN or floating status depending on input/environment;
it is not a universal canonical NaN load. The negative-finite branch instead
loads the original single NaN word. NaN payload/quieting,trap/flag behavior and
widening must be established on the original processor.

errno is runtime BSS word0x8041ee28. These branches store33 but do not clear it
on zero/positive finite success. There is no caller buffer,allocation or helper
call inside the core. Input ABI and load/store byte order are binary64 big-endian.

## Subnormal normalization and exponent parity (B)

For positive finite input,set e=arithmetic(H>>20). When e0:

1. While H==0,OR L>>11 into H,shift L left21,subtract21 from e.
2. Set i0;while H's bit0x00100000 is unset,shift H left1 and i++.
3. OR the original current L shifted right(32-i) into H,shift L left i,
   and set e=e-(i-1).

The right-shift uses PowerPC register shift semantics;when i0 its shift count32
produces zero rather than a host masked-count shift0. Preserve that edge if the
transferred low word made H's normalization bit set immediately. The zero-input
shortcut prevents a normal zero record entering these loops.

Set E=e-1023 and remainder high A=(H&0x000fffff)|0x00100000,remainder low B=L.
If E is odd,double the two-word remainder once. Then double it unconditionally.
Each double is A=word32(2*A+(B>>31)),B=word32(2*B). E's signed arithmetic half
is retained only at final result construction;there is no floating scaling call.

## High-word extraction loop (B)

Initialize root high QH0,root low QL0,trial accumulator high SH0,low SL0.
Start r=0x00200000. While r!=0:

```
t = word32(SH+r)
if signed(t) <= signed(A):
    SH = word32(t+r)
    A = word32(A-t)
    QH = word32(QH+r)
double remainder A:B
r = logical_shift_right(r,1)
```

This makes22 iterations for masks bit21 through0. The compare is signed in the
original instructions;do not indiscriminately replace every helper comparison
with unsigned arithmetic. Each iteration doubles the current remainder after
the admission decision. No floating arithmetic or rounding instruction is used.

## Low-word extraction and carry/borrow (B)

Start r=0x80000000 and perform32 iterations until r0. For each,let TH=SH and
TL=word32(SL+r). Admit when signed SH<signed A,or SH==A and unsigned TL<=B.
On admission:

1. Set SL=word32(TL+r).
2. If TL's high bit was1 and new SL's high bit is0,increment SH.
3. Subtract saved TH from A;decrement A once more if unsigned B<TL.
4. Set B=word32(B-TL),QL=word32(QL+r).

Then double A:B and logically shift r right1. The saved TH is the accumulator
before its carry update. Carry admission uses the literal high-bit condition,
not a floating significand operation. All sums/subtractions/stores are word32.
The two loops yield root bits and a final remainder;no explicit approximation
iteration tolerance or rendering/simulation timing is involved.

## Remainder correction and result bits (B)

If A\|B==0,skip correction. Otherwise load original double+1 at0x804290f8 and
store it twice to the same temporary slot;no fadd/fsub/compare with a tiny value
appears. The value is overwritten by final result storage. Thus there is no
local floating rounding-mode probe or FPSCR read in this rounding branch.

If QL==0xffffffff,set QL0 and QH++. Otherwise set QL=word32(QL+(QL&1)). This
is the inspected integer correction rule when remainder is nonzero. Do not
infer handling of all rounding modes from a generic fdlibm source version or
a familiar algorithm name;the original compiled operations are the evidence.

Construct output words:

```
outLow = QL >> 1
if QH is odd: outLow |= 0x80000000
outHigh = word32(arithmetic(QH>>1) + 0x3fe00000)
outHigh = word32(outHigh + word32(arithmetic(E>>1)<<20))
```

Store both words and reload binary64 f1. There is no ldexp call,subnormal-result
branch,final sign insertion or output float narrowing. Ordinary finite positive
sqrt magnitudes can be interpreted from these bits,but complete numerical
accuracy/rounding remains an original execution and domain-validation task.

## Caller intersection and evidence

Power's exact+0.5 exponent shortcut calls sqrt only when signed base high word
is nonnegative. Its other special/domain paths are independent;this service's
errno writes must not be generalized to the complete power API. Negative zero
is excluded by that power shortcut even though standalone sqrt preserves it.
Other sqrt callers,finite-input census and global errno consumers remain open.

Two full-function fingerprints and24 anchors pin wrapper delegation,special/
errno/zero ordering,subnormal shifts,exponent parity,both extraction loops,
carry/borrow,remainder correction and output assembly. Direct checks pin the
loaded+1 constant;prior evidence pins the NaN source word. Static checks never
execute original instructions;ELF/disassembly/reports remain ignored. No native
code linking,asset conversion,message rewrite or licensing change occurs.

Validate signed zeros,NaNs/infinities/errno,negative values,smallest subnormals,
normalization shift boundaries,perfect squares,nonzero remainder and carry/
parity/output bits under actual FPSCR. Compare standalone helper and power's
half-exponent caller before claiming composed equivalence. Continue numerical/
scanner validation and uncovered text/story/field/battle/audio/save/content
before complete-game requirements. Full objective remains incomplete;no Switch
square-root behavior is inferred.
