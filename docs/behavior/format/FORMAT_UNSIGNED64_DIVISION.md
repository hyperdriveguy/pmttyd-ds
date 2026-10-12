# Unsigned64 quotient and remainder runtime

US G8ME01 revision0, recovered2026-10-12. B evidence is static original DOL:
__div2u at0x802649ec,sizeec and __mod2u at0x80264c10,sizee4. These are the
services used by [integer conversion](FORMAT_INTEGER_CONVERSION.md) and the
integer-digit stage of [binary decimal entry](FORMAT_BINARY_DECIMAL_ENTRY.md).
No original instructions are executed here; authored recurrence checks below
validate a derived contract, not original/native equivalence.

## Word ABI, leading zeros and early return (B)

Numerator N is r3:r4 (high:low); divisor D is r5:r6. Return pair is r3:r4.
Both routines are leaf functions without a local stack frame, memory access,
allocation, helper call, hardware divide instruction or errno store. They use
r0/r7..r10, CTR, CR0 and XER carry as scratch. Divisor registers are not rewritten.
Do not require these flags/scratch registers to survive a call.

For each operand, cntlzw on the high word gives its leading-zero count; when
high=0, replace it with cntlzw(low)+32. cntlzw(0)=32, so the full zero operand
has count64. Let zN and zD be the resulting counts.

If signed zN>zD, numerator has fewer significant bits: __div2u explicitly writes
high0/low0 and returns; __mod2u returns the original numerator pair immediately
through conditional bclr. This is a bit-length gate, not a complete N<D test.
Equal bit lengths continue into the loop even when N<D. A zero divisor does
not take this early return because zD=64 is maximal; there is no D==0 guard.

## Shifted numerator/remainder preparation (B)

Otherwise compute K=zD-zN+1 and S=64-K. Load CTR=K. Form remainder pair R and
the pair Q held in r3:r4 using word branches:

| Predicate | Pair construction |
| --- | --- |
| K>=32 | R.high=0; R.low=N.high logical-right-shift(K-32) |
| K<32 | R.high=N.high>>K; R.low=(N.low>>K) OR (N.high<<(32-K)) |
| S>=32 | Q.high=N.low<<(S-32); Q.low=0 |
| S<32 | Q.high=(N.high<<S) OR (N.low>>(32-S)); Q.low=N.low<<S |

All word results are32-bit. Register logical shifts use the original PowerPC
six-bit count behavior: bit5 set gives zero; otherwise low5 bits select0..31.
Do not substitute a host shift with undefined behavior at32 or a host count
masked only to five bits. This matters for K=32/64 and for zero-divisor S=-1.

For D!=0 on the admitted path, K is1..64 and S is0..63. The table is therefore
equivalent to R=N>>K (zero for K=64), Q=(N<<S) modulo2^64. K is the numerator
bit length minus divisor bit length plus1, giving the quotient-bit budget.

## Shared restoring loop and carry pipeline (B)

Set r10=0xffffffff. addic(R.high,0) initializes XER carry to0. Each iteration:

1. Double Q.low with incoming carry, then Q.high with the low-word carry.
   Double R.low with the high-Q carry, then R.high with the low-R carry.
   These four adde instructions move one numerator bit from Q into R while
   inserting the prior iteration's quotient decision into Q's low bit.
2. Subtract D.low from R.low with subc, retaining the temporary low result and
   no-borrow carry. subfe. computes the temporary high result with that borrow
   and updates CR0 from its signed32 result.
3. If the temporary high word is negative, keep the prior R pair. Otherwise
   copy the temporary pair to R and addic(0xffffffff,1), forcing carry1.
4. bdnz decrements CTR and repeats until the original K iterations complete.

For nonzero D on this path, a negative subtraction result is the rejected
trial and leaves carry0; the accepted trial sets carry1. The signed high-word
test agrees with the mathematical subtraction sign in this bounded restoring
domain. It is still the original signed result test, not an independent unsigned
pair comparator or a direct branch on XER carry.

The last quotient decision remains pending in carry. __div2u performs one final
adde doubling of low then high Q to insert it, and returns Q. __mod2u skips this
final quotient shift and copies R.low/R.high into r4/r3. The two routines therefore
share arithmetic preparation/loop but return different live pairs. A trailing
extra blr in the mod symbol is not targeted by its local control flow.

For D!=0 this establishes floor(N/D) and N modulo D, including unsigned values
above2^63. Equal-width N<D reaches the loop and returns quotient0/remainderN;
the earlier bit-length shortcut is not the only zero-quotient route.

## Zero-divisor derived behavior (B/P)

There is no trap or error branch locally for D=0. K becomes bitLength(N)+1,
with bitLength(0)=0. For N<2^63, S>=0, source bits survive preparation and each
trial subtraction of zero is accepted. Derived quotient is the low64 bits of
2^K-1, remainder N. For N>=2^63, K=65 and S=-1; the word shifts clear Q/R,
then65 accepted zero trials produce quotient0xffffffffffffffff, remainder0.
The extra iteration and source clearing follow the register shift rules above.

| Numerator with divisor0 | Derived quotient | Derived remainder | Iterations |
| --- | --- | --- | --- |
| 0 | 1 | 0 | 1 |
| 1 | 3 | 1 | 2 |
| 10 | 31 | 10 | 5 |
| 0xffffffff | 0x1ffffffff | 0xffffffff | 33 |
| 0x100000000 | 0x3ffffffff | 0x100000000 | 34 |
| 0x7fffffffffffffff | 0xffffffffffffffff | original numerator | 64 |
| 0x8000000000000000 | 0xffffffffffffffff | 0 | 65 |
| 0xffffffffffffffff | 0xffffffffffffffff | 0 | 65 |

These are static recurrence consequences, not executed-original return values
or a promise that game callers intentionally divide by zero. Do not introduce
a language exception and then describe it as this original local behavior.
Actual arbitrary-operand callers still require a census.

## Formatting integration and checks (B/P)

The documented longlong2str radix choices8/10/16 and binary decimal integer loop
divisor10 are nonzero. Numeric unsigned64 division/remainder therefore admit
the conventional quotient/remainder interpretation on those paths. The generator
appends remainders0..9 and repeatedly reduces its positive integer; a full64-bit
integer needs at most20 decimal digits, while its recovered significand<=2^53-1
needs at most16. This refines the normal multiplication scratch bound to16+36=52
digits on that finite generator path. Zero-V fallback remains a local branch.

An authored recurrence using the word-shift table and signed trial result was
checked against integer divmod on105350 nonzero-divisor pairs: all small
numerators0..255/divisors1..255, power-of-two and adjacent word/bit boundaries,
and5000 seeded64-bit pairs (set deduplicated). All matched. Nine zero-divisor
cases were derived with the same recurrence. This checks the documentation's
arithmetic interpretation, not instruction execution, ABI flags or every64-bit
pair. Metadata reports remain ignored with the original inspection outputs.

Differential obligations: zero/high-word count branches, equal-width early-test
distinction, shift32/64, carry initialization and pending quotient insertion,
signed trial rejection, unsigned high-bit operands and zero-divisor65-loop case.
Hex-float construction is now covered in [hex-float](FORMAT_HEXADECIMAL_FLOAT.md).
Remaining: original executions/caller corpus, wide/memory services and
floating environment. Full formatting, story and complete-game equivalence are
still unproven.
