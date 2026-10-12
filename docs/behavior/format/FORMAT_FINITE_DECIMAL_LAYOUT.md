# Finite decimal format selection and byte layout

US G8ME01 revision0, recovered2026-10-12. B evidence is static original float2str
at0x8026abc8,size71c, following the entry/preparation/special-value contracts in
[decimal rounding](FORMAT_DECIMAL_ROUNDING.md). See [directive parser](FORMAT_DIRECTIVE_PARSER.md)
and [output runtime](FORMAT_OUTPUT_RUNTIME.md). No original execution, complete
binary-to-decimal generation or host-libc equivalence is claimed. Formats/text
remain original reused data, without normalization to avoid observed behavior.

## Finite record normalization and reverse buffer (B)

After decimal preparation, let N=unsigned digit count, E=signed halfword exponent,
digits begin at record+5, P=signed format precision word, B=bufferEnd. Rewrite
E=low16(E+N-1), interpreted signed: the exponent now describes the leading digit's
decimal order. Set C=B-1 and store NUL there. The earlier trim phase incremented
the pre-normalization exponent for each removed trailing zero; these are distinct
operations. Sign byte is retained independently of E/digits.

Dispatch normalized conversion e/E to scientific, f/F to fixed, g/G to general
selection. Other direct float2str conversion bytes return the current C (empty
substring) after this initialization, rather than a local null/error. The normal
formatter routes a/A elsewhere. This is not admission for arbitrary raw formats.

The format record here is a local copy made by __pformatter. P/conversion
mutations below affect this helper copy, not the outer formatter's record.
Integer/halfword arithmetic retains original wrap, signed tests and narrowing;
no general lower-precision bound or final allocation-length argument is supplied.

## g/G rounding before selection (B)

If N>P, round_decimal(record,P), then reload updated count/exponent. Choose
scientific when E<-4 or E>=P, using P before layout rewriting. Otherwise fixed.
The threshold tests occur after rounding, so an all-nine carry changing E can
change the chosen notation. Neither threshold is based only on the input double.

For scientific choice:

- Alternate form: P=P-1.
- Without alternate form: P=N-1.
- Change conversion g to e, G to E; then enter scientific path.

For fixed choice:

- Alternate form: P=P-(E+1).
- Without alternate form: P=max(N-(E+1),0), using signed word comparison.
- Enter fixed path without changing conversion byte.

Without alternate form, rounding's logical trailing-zero trimming reduces the
fraction budget. Alternate form preserves the requested significant-digit
budget through the precision transformation and later zero fill. There is no
second universal P<=509 check after these transformations.

## Scientific exponent and admission (B)

For e/E, if N>P+1, round_decimal(record,P+1). Reload E/count. Take exponent sign
'+' for E>=0 or '-' for E<0; negate negative signed-halfword E into a word
magnitude. Generate decimal exponent digits backward using the original signed
divide-by-ten multiplier route; emit at least two digits, including E=0.
Prepend exponent sign, then current conversion e/E.

Now C points to this suffix. If signed word `P+(B-C)>509`, return null. The
counted displacement includes the suffix and original NUL; it is not a check
of the fully constructed mantissa/sign nor a supplied buffer size. Original
arithmetic overflow and negative P can affect admission. Helper-null fallback
in the outer formatter copies the remaining format literally and stops.

## Scientific mantissa and sign (B)

With valid nonnegative P and normalized count, the byte construction is:

1. If N<P+1, prepend P+1-N zeros before the exponent suffix.
2. Prepend digits N-1 down to1 from the decimal record, in reverse storage order.
3. Prepend '.' when P!=0 or alternate form is set.
4. Prepend first record digit.
5. Prepend '-' if sign byte is nonzero; else '+' for sign policy1, space for
   policy2, otherwise nothing. Return C.

Forward substring is sign, leading digit, optional decimal point, P fractional
positions and signed exponent with at least two digits for admitted ordinary
records. This is derived from actual stores, not a claim for malformed digit
counts/negative precision. No digit decoding, locale separator, width fill or
reparse occurs in this helper. Outer formatter handles its own field padding.

## Fixed fraction budget and rounding (B)

At fixed entry compute M=max(N-E-1,0), the significant fractional positions.
If M>P, compute retained K=N-(M-P) and call round_decimal(record,K). Reload
count/exponent and recompute M=max(N-E-1,0). K can be zero or negative; the
rounding helper's zero-retention count parity/canonical-zero behavior applies,
not an assumed platform round-to-even function on the input double.

Compute integer positions Q=max(E+1,0). If signed word Q+M>509, return null.
This check does not include P-M trailing fraction zeros, decimal point, sign or
NUL. It therefore is not a complete final-output-capacity guarantee. For example,
an instruction-derived shape N=1,E=308,P=509 admits Q+M=309 even though later
requested fraction zeros exceed that displacement. This is a derived obligation,
not an executed original overrun or a supported-call-site assertion. Caller
ranges and actual generated records still require evidence.

## Fixed reverse construction (B)

Start digit source S=recordDigits+N. Build backward in this exact order:

1. If signed P-M>0, prepend that many zeros (batched eight-byte stores followed
   by remainder). These are requested trailing fraction zeros.
2. Copy backward from S while copied<M and copied<N. Advance copied and decrement
   S on every byte. These are significant fraction digits.
3. If copied<M, prepend M-copied zeros; these are leading fractional positions
   before the first significant digit when E is negative.
4. Prepend '.' if P!=0 or alternate form is set.
5. If Q=0, prepend '0'. Otherwise first prepend zeros while their count<Q-N;
   then copy Q minus that zero count digits backward from current S. This is
   the literal signed comparison in the binary; do not substitute a generic
   decimal-point-placement algorithm without checking it against record invariants.
6. Prepend sign as in scientific path and return C.

Step5's zero loop tests `zeroCount < Q-N` with signed word comparison; zero fill
supplies integer positions after the available digits. Count and pointer states
from the fraction copies remain authoritative.
With valid record shapes, this produces significant integer digits followed by
zeros, or a single0 before the decimal point for subunit values.

## Capacity and mutation summary (B)

Entry rejects signed P>509 before decimal generation. Scientific uses P+suffix
displacement; fixed uses Q+M after rounding. These checks are distinct and may
precede additional stored bytes. No destination length, heap resize or general
error recovery exists locally. Buffer sign comes from generated record, preserving
signed zero from the generator. Actual generator sign behavior is now covered
in [binary entry](FORMAT_BINARY_DECIMAL_ENTRY.md), independently of this consumer.

## Differential obligations and remaining work (P)

Compare leading-exponent normalization/trailing trim, g/G post-round boundaries
at-4/P, alternate significant-digit retention, e/E zero/signed/multidigit
exponents, carry changing mode, fixed K positive/zero/negative, fraction/whole
zero fill, literal integer-position loop, conditional decimal point, sign policy,
precision-copy mutations and all three capacity tests. These are derived cases,
not executed formatting outputs or measured gameplay.

Binary generation/record invariants are now covered in
[binary entry](FORMAT_BINARY_DECIMAL_ENTRY.md) and its linked helpers; hex layout
in [hex-float](FORMAT_HEXADECIMAL_FLOAT.md).
Remaining: floating environment, wide/memory primitives and original format/type/count
call-site corpus. Local finite float2str layout plus preparation/rounding is
documented across these files; full formatting/story/full-game equivalence
remains unfinished.
