# Numeric decimal multiplication and 36-digit rounding

US G8ME01 revision0, recovered2026-10-12. B evidence is the original static
DOL: __timesdec at0x80268b68,size278. This establishes the local multiplication
routine used by [binary decimal entry](FORMAT_BINARY_DECIMAL_ENTRY.md).
Power generation is covered in [decimal powers](FORMAT_DECIMAL_POWERS.md).
Numeric helper edge behavior and executed formatting remain open. No original
code is executed by this investigation.

## Record domain and writes (B)

Inputs are(output,left,right). Each operand has unsigned byte count at+4,
numeric digit bytes at+5, and signed halfword leading-digit exponent at+2.
These digits are values0..9, unlike the ASCII records accepted by
[round_decimal](FORMAT_DECIMAL_ROUNDING.md). There is no local digit validation,
special 'I'/'N' marker handling, canonical-zero branch or sign multiplication.

Read both counts before writing output sign0. All input digits are consumed
into stack scratch before output digits are copied. Read both input exponents,
sum and store their low16 bits into output exponent. An extra leading carry
increments that stored exponent with another halfword write. Output count and
digits are then written; unused digit storage/padding bytes are not cleared and
no NUL is appended. The binary entry caller later restores the raw double sign.

## Decimal convolution and carry (B)

Let n=left.count, m=right.count, digits A[0..n-1], B[0..m-1], and C=0.
Scratch begins at stack+8; its exclusive end is stack+8+n+m. Start a write
cursor at that end and set k=n+m-1. While signed k>0:

1. Set j=m-1, i=k-j-1. If i<0, set i=0 and j=k-1.
2. Set terms=min(j+1,n-i). If terms>0, accumulate into C the products
   A[i+t]*B[j-t] for t=0..terms-1. Loads are unsigned bytes, products use
   low32 bits and addition is32-bit. The binary unrolls groups of8, then
   processes the remaining terms; order stays ascending A/descending B.
3. Compute unsigned quotient Q=floor(C/10) through high-word multiplication
   by0xcccccccd followed by a3-bit shift. Predecrement scratch cursor and write
   low8(C-10*Q); retain C=Q. Decrement k.

For valid decimal operands this visits the product's least significant
coefficient first, carrying toward the leading coefficient. The index relation
i+j=k-1 and each byte/quotient update are binary facts; replacing this with a
host floating multiplication would not preserve these intermediate records.

After the loop, sum input exponents as above. If C!=0, predecrement cursor,
write low8(C) as the additional leading digit and increment output exponent.
For positive counts and ordinary decimal digits, scratch therefore holds
n+m-1 digits without this extra carry, or n+m with it. The routine does not
strip leading/trailing zeros to shorten that scratch result.

Derived examples, not executed traces: records for12 and34, both exponent1,
yield numeric408 with exponent2/count3; records9 and9, both exponent0, yield
numeric81 with exponent1/count2. A zero operand can leave multiple numeric
zero digits and a nonzero exponent; multiplication does not canonicalize it.

## Copy budget and discarded tail (B)

Copy forward from scratch cursor into output+5 until either36 bytes are copied
or cursor reaches the original exclusive end. Store copied count as a byte.
If no scratch bytes remain, return without rounding or trimming. Otherwise:

| First discarded numeric byte | Local decision |
| --- | --- |
| <5 | Return with retained digits unchanged |
| >5 | Round up |
| =5, any later scratch byte nonzero | Round up |
| =5, all later bytes zero | Round up only if the last retained scratch byte is odd |

The tie parity load is cursor-1 in scratch, whose digit has just been copied;
it is not an output count-byte parity case. The rest-of-tail scan ends at the
original scratch end. This is a numeric decimal ties-to-even decision over
the entire discarded scratch tail, not an unconditional increment on5.

## Retained carry (B)

For round up, start at output+5+count-1 and walk backward. If unsigned digit<9,
increment and return. Otherwise, if this is not the first output digit, write
numeric0 and continue left. At the first digit, write numeric1 and increment
output exponent with halfword storage. Count remains the copied count, including
on all-retained-digit carry overflow. Lower carried-through digits stay explicit
zeros; there is no trailing-zero trimming or exponent subtraction by count here.

For valid operands, discarded bytes exist only after the36-digit copy budget is
exhausted, so this retained carry acts on36 digits. This local invariant depends
on the scratch/count domain; it is not a guard accepting arbitrary malformed
records. A byte>=9 follows the carry route; inputs beyond0..9 are not rejected.

## Capacity and integration boundaries (B/P)

The frame is112 bytes, scratch starts at+8 and saved registers start at+92.
Scratch end is computed from the two byte counts, without a capacity check.
Counts<=36 each place the scratch end no farther than+80; the output36-digit
cap alone does not protect scratch for larger arbitrary input counts. Zero
counts, malformed digits and arbitrary overlapping ranges have no local
validation. Do not infer a general memory-safe multiplication API from its
normal generator call path.

The integer record constructed by __num2dec_internal is at most20 decimal
digits for a valid unsigned64 value. The power record's count/digit bounds and
recursive rounding are now covered in [decimal powers](FORMAT_DECIMAL_POWERS.md),
establishing a combined scratch budget of at most56 digits for valid integer
records on this path. The use of leading
exponents, numeric bytes, retained36-digit rounding and restored sign is now
established locally; this does not yet establish complete finite conversion.

Differential obligations: coefficient order, carry quotient/remainder, extra
leading digit/exponent, all scratch-tail tie tests, retained parity, all9 carry
with unchanged count, absence of trimming/zero normalization, sign0 and final
caller sign restoration. Original executions and supported caller records are
still needed. Remaining dependencies include finite-input power range, runtime
numeric helpers, hex-float formatting and the original caller corpus.
