# Decimal-record equality and converter refinement admission

US G8ME01 revision0,recovered2026-10-12. Evidence B: complete __equals_dec at
0x802672d8,size0x10c and __dec2num region0x80265f54..0x80266138. See
[initial approximation](DECIMAL_TO_BINARY_APPROXIMATION.md),
[infinity boundary/sign exit](DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md) and
[binary-to-decimal generator](FORMAT_BINARY_DECIMAL_ENTRY.md). This covers local
equality,initial magnitude direction and first upward neighbor only;[subsequent neighbor search,distances/ties and final selection](DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md)
are recovered separately;subtraction/pow dependencies remain open. No original
execution or complete correctly-rounded conversion claim is established.

## __equals_dec interface and zero shortcuts (B)

r3/r4 point to numeric decimal records A/B. Return is0/1;neither record is
written. No helper call,allocation,FPSCR arithmetic,RNG or error reporting occurs.
Sign byte+0 is never read. First numeric digit is byte+5,signed exponent at+2,
unsigned count at+4. Pointers/counts/allocation capacities are unchecked.

First read A.first. If zero,return1 iff B.first is zero,using count-leading-zero
and shift to make a boolean. Exponent,count,remaining digits and sign are ignored.
If A.first nonzero but B.first zero,return0. These are first-byte zero conventions,
not a loop proving the whole represented magnitude is zero. Even a count0
record has its first byte read before count. A malformed record beginning numeric
zero followed by nonzero digits still takes this shortcut.

For both nonzero first bytes,unequal signed16 exponents return0. With equal
exponents,set n=min(A.count,B.count),compare unsigned byte pairs at+5+i for
all n indices. A mismatch returns0. If A's count was exhausted,scan remaining
B bytes;otherwise scan remaining A bytes. Any nonzero remaining byte returns0;
all-zero remainder or no remainder returns1. The helper does not require equal
counts,trim data,subtract ASCII'0' or validate digits0..9.

Thus records with the same leading-digit exponent and numeric sequences12 and
1200 compare equal. Different sign bytes do not change the result. Two first-
byte-zero records compare equal despite differing exponents/counts/tails.
These are instruction deductions,not executed caller samples or approval to
normalize arbitrary malformed records in a new interface.

## Candidate decimalization and equality fast exit (B)

At refinement entry0x80265f54,load stored candidate C into f1 and call
__num2dec_internal(output=stack+0x148). This is the already-recovered internal
generator,not public __num2dec:it supplies numeric digits and leading-digit
exponent,without caller-requested ASCII conversion/padding. Its generation uses
its own finite36-digit machinery;executed numerical accuracy remains unverified.
Let A be that generated candidate record and X the prepared private input at
stack+0x1a0. X's exponent was changed to E+D-1 during approximation.

Call __equals_dec(A,X). Any nonzero return branches directly to shared finite
sign exit0x8026684c with C unchanged. It does not require identical byte buffers,
count fields or input sign. Apply the saved private-input sign there and return,
as in the [sign contract](DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md).
This fast exit occurs before the inlined direction compare or any neighbor step.

No caller-original record is mutated by these local operations:the generator
writes its own output record and the converter operates on its private input
copy. Shared equality semantics must be preserved across those record domains;
comparing ASCII forms or floating C with a separately host-parsed X is not
established equivalent.

## Inlined initial direction (B)

If equality returned0,compute the strict relation A<X using the local compare
body,with these independently inspected operands:

1. A.first zero yields true iff X.first is nonzero.
2. A.first nonzero and X.first zero yields false.
3. Unequal signed16 exponents determine magnitude ordering.
4. Equal exponents compare unsigned digit bytes lexicographically through the
   shorter count;first mismatch determines ordering.
5. If A's count ended first or simultaneously,any remaining nonzero X byte
   makes A<X. All zeros/no remainder yield false.
6. If only X's shorter count ended,the predicate is false.

Exponent ordering uses xor/arithmetic-shift/and/subtract/sign extraction. Signs
are ignored;special marker records from nonfinite generated candidates are not
converted into a newly invented mathematical ordering here. For canonical
positive finite records,this is decimal magnitude ordering with implicit trailing
zero digits. Invalid records are still subject to these literal byte rules.

False branches to0x802664f4,the downward-side region not recovered here. True
enters the upward neighbor admission below. The fact that other inlined compare
sites have similar structure does not establish their operand direction or
caller/exit behavior without inspecting each site.

## First upward neighbor and infinity early exit (B)

Store C as two words at stack+0x28. Add1 to the low word with addc and add the
carry to the high word with adde. Reload that bit pattern into f1 and preserve
its bits in another local double. This is an increment of the64-bit encoding,
not a floating addition of an epsilon. For nonnegative finite binary64 encodings,
it gives the next higher representable pattern;that ordinary-domain interpretation
does not establish behavior for every malformed/nonfinite candidate.

Classify neighbor bits by exponent/fraction exactly as at the approximation
boundary:NaN1,infinity2,zero3,normal4,subnormal5. If class2,store this neighbor
as the current candidate and branch directly to shared sign exit. No local
decimal-distance/tie comparison runs before this particular infinity return.
When a nonnegative maximum-finite encoding is incremented,the local result is
positive infinity;the later sign exit can negate it.

Other classes call __num2dec_internal into a new record at stack+0x11c,then
continue to0x80266254 for further relation/search behavior. That continuation
and its eventual distance/tie selection remain open. This document does not
claim that every upward admission returns the first neighbor,or that a shared
neighbor classification proves correct rounding.

## Evidence and port obligations

A new whole-function fingerprint covers __equals_dec. Twenty-four additional
anchors cover zero/exponent/common-prefix/remainder equality,both generator/
equality calls,initial compare direction,word increment/carry and infinity exit.
Existing fingerprints cover the containing converter/internal generator;these
checks do not execute decimal or floating comparisons. No original routine is
compiled/distributed,and local ELF/disassembly/report outputs remain ignored.
No asset conversion,message rewrite or licensing change occurs.

Validate equality with trailing-zero/count/sign variants,zero conventions and
actual canonical caller records. Compare generated candidate records,equality/
direction decisions,neighbor bits and sign exits against original execution.
Recover the complete upward/downward search,difference helpers,tie/parity rules
and final candidate selection,plus pow dependencies,before numeric equivalence.
Full scanner/layout/story/game coverage and complete-game requirements remain
incomplete;Switch behavior is not inferred from these GameCube bodies.
