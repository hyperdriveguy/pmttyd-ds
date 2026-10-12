# Decimal converter neighbor search, distance records and ties

US G8ME01 revision0,recovered2026-10-12. Evidence B: remaining __dec2num regions
0x80266148..0x8026684c,within containing0x1080-byte routine. See
[equality/refinement entry](DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md),
[infinity boundary/sign](DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md),
[approximation](DECIMAL_TO_BINARY_APPROXIMATION.md) and
[internal decimal generator](FORMAT_BINARY_DECIMAL_ENTRY.md).
These paths complete the local containing body's recovered control flow.
[__minus_dec](DECIMAL_RECORD_SUBTRACTION.md) is recovered separately;pow and
composed numerical behavior remain open dependencies;no original execution or complete
correct-rounding/equivalence proof is asserted.

## Operand map and record comparison (B)

X is prepared private input at stack+0x1a0. C is current binary candidate at+0x30;
A is its generated decimal record at+0x148. All records here use numeric digits
and leading-digit signed16 exponents,not public ASCII decimal formatting.
Initial equality has already returned false. Initial inlined A<X chooses upward
when true,downward when false. Sign is applied only after final selection.

Each inlined strict comparison below was checked with its actual operands.
Its local LT(left,right) policy is:

- Left.first0: true iff right.first is nonzero. Otherwise right.first0: false.
- Unequal signed16 exponents decide signed leading-digit order.
- Equal exponents: compare unsigned digits through min(counts).
- If common digits match and left ended first,any nonzero remaining right byte
  makes LT true;otherwise false. If only right ended first,LT false.

Signs are ignored,leading-zero records take first-byte shortcuts,and digit/count
bounds are unchecked. The equality helper separately allows trailing numeric
zeros and ignores sign,as [recovered](DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md).
These rules are literal record contracts;mathematical magnitude interpretation
requires the canonical generator/subtraction input domain.

## Upward bracketing (B)

The admission path produced raw neighbor U=bits(C)+1 at stack+0x28. If its class
was infinity,it already returned U through shared sign exit. Otherwise
__num2dec_internal generated B at stack+0x11c from U. At0x80266254,compare B<X.
If false,stop stepping and compute distance records. If true,execute the loop:

1. Preserve old U as a double;increment U's low word by1 with addc and its high
   word by carry with adde.
2. Copy all42 bytes of the old B record into A,including unused digit storage.
3. Store old U into C,so C/A are the previous neighbor and its record.
4. Classify new U through raw exponent/fraction masks. If infinity,store new U
   into C and return via shared sign exit without distance comparison.
5. Otherwise decimalize new U into B and compare B<X again.

Thus ordinary canonical finite stepping leaves C as the lower candidate and U
as the next encoding whose generated record is not less than X. No loop counter,
maximum iteration count,binary search or floating epsilon update appears. The
raw encoding step has no branch based on exponent-boundary spacing. Generator
rounding/record precision can affect when LT stops;correct exact-value bracketing
cannot be inferred solely from these calls.

## Upward distance requests and selection (B)

On the stop route,call __minus_dec0x80266ddc twice,with explicit output and
operand pointers:

| Output record | Left request | Right request |
| --- | --- | --- |
| DL at stack+0xf0 | X | A (current lower candidate record) |
| DU at stack+0xc4 | B (upper neighbor record) | X |

These are subtraction requests by symbol/caller convention,not an established
exact-arithmetic implementation. See [subtraction](DECIMAL_RECORD_SUBTRACTION.md) for local precision,normalization,
borrow,aliasing and malformed-domain flow;exact-distance and composed execution
accuracy remain unverified.
Both calls occur before equality;there is no local float-distance subtraction.

Call __equals_dec(DL,DU). If nonzero,test current C's low-word bit0;the high word
is ANDed with zero in the parity sequence. If even,retain C. If odd,copy U into C.
Return via shared sign exit. There is no check that the input X is literally a
binary64 midpoint;the tie criterion is equality of the returned decimal records.

If equality returned0,compute LT(DL,DU) with the inlined record comparison.
True retains C;false copies U into C. In ordinary accurate-distance domains,
this corresponds to choosing the smaller distance. That interpretation is
conditional on helper output correctness,not an executed proof.

## Downward bracketing (B)

At0x802664f4,store C into lower-neighbor scratch L at stack+0x20,then decrement
its64-bit encoding by adding0xffffffff to the low word and0xffffffff plus carry
to the high word. This is word64 minus1,not a floating subtraction. Decimalize
L into D at stack+0x98. No explicit zero/underflow/nonfinite classification guard
appears in this downward admission or loop.

At0x802665b0,compare **X<D**. If false,stop and request distances. If true:

1. Preserve old L,then decrement L's raw words by1 again.
2. Copy all42 bytes of old D into A.
3. Store old L into C,so C/A are now the prior,upper-side candidate.
4. Decimalize the new L into D;compare X<D again.

The compare operand direction is reversed from upward stepping:the loop seeks
a lower record not greater than X. C remains the upper candidate at the stop
route. There is no iteration cap or no-progress detection. Under valid
nonnegative finite input this can be interpreted as stepping downward through
representable magnitudes;unsupported/malformed records are still subject to
unchecked raw decrement and generator behavior.

## Downward distances, ties and branch direction (B)

After stopping,call the same helper with these distinct operands/outputs:

| Output record | Left request | Right request |
| --- | --- | --- |
| DL at stack+0x6c | X | D (lower neighbor record) |
| DU at stack+0x40 | A (current upper candidate record) | X |

Call __equals_dec(DL,DU). On equality,current upper C's low bit0 decides:even
retains C;odd copies L into C. Return via shared sign exit. The two adjacent
nonnegative finite encodings ordinarily alternate low-bit parity,so both search
sides implement the same local even-encoding preference without checking the
record sign.

On unequal distance records,compute LT(DL,DU). Here true copies L into C and
false retains C. This is the **opposite copy/retain branch** from the upward
route,because current C occupies the opposite side of the bracket. Do not reuse
an upward "false chooses neighbor" rule for downward selection.

All selected ordinary candidates branch to0x8026684c:load private input sign
byte,if nonzero fneg saved C,then return f1. No caller-original record is written,
no count/status pointer is provided to __dec2num,and no local errno update is
performed by these selection paths. Helper effects/FPSCR remain dependencies.

## Local coverage versus executable equivalence

This document,entry/special,approximation,infinity boundary and equality/admission
contracts cover the inspected __dec2num local body. The call graph still depends
on pow/__minus_dec and on the previously recovered generator,ldexp and equality
services. Whole-body fingerprints or green source tests do not prove those
services' numerical accuracy,original FPSCR behavior,termination for all inputs
or the game's actual caller corpus.

Thirty-two additional anchors pin both loop steps,full-record copies,operand-
specific continuation branches,four subtraction requests,equality calls,parity
and asymmetric copy/retain selections. Existing fingerprints identify the
containing function and generator/equality dependencies. Original disassembly/
reports remain ignored;no original executable linking/execution,asset conversion
or story/message rewrite occurs.

Next recover __minus_dec and pow dependencies,then validate generator/distance
records,neighbor bits,iterations,tie parity and final signed output against
original execution at normal/subnormal/exponent/maximum boundaries and actual
scanner inputs. Continue text/drawing/resource callers and all uncovered
story/event/field/battle/audio/save/content before complete-game requirements.
The full objective remains unfinished;no Switch behavior is inferred.
