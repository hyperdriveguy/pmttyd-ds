# Original unsigned64 scan state machine and carry arithmetic

US G8ME01 revision0, recovered2026-10-12. Evidence B: __strtoull at0x8026e6dc,
size0x40c, original17-entry state table and shared classification/uppercase data.
See [unsigned32 scanner](SCAN_INTEGER32.md), [scan consumers](SCAN_FORMAT_CONSUMERS.md),
[directive grammar](SCAN_DIRECTIVE_PARSER.md), [reader](SCAN_INPUT_RUNTIME.md) and
[unsigned64 division service](FORMAT_UNSIGNED64_DIVISION.md). No original
execution or host/public strtoull equivalence was established.

## Interface and retained state (B)

Inputs r3..r9 are base,width,readCallback,context,countOutput,negativeOutput,
overflowOutput. Return r3:r4 is unsigned magnitude high:low, before applying a
sign. All three output pointers are unchecked. Initialize negative/overflow
words0; count is written at exit. Local magnitude V starts high0,low0; threshold
T starts high0,low0. State S=1, lookahead-request count N=0, skipped-whitespace
count W=0. N/W are32-bit word counters even though V/T are64-bit values.
No local errno write or signed64 limit check occurs.

The active consumer selects this helper only for descriptor selector4, then
applies a negative flag through a two-word negation and stores low word at
output+4 before high word at+0. This helper neither stores the final integer to
that destination nor rejects overflow on the consumer's behalf.

## Entry, threshold and invalid-entry difference (B)

Validate signed base:0 or2..36; signed width must be>=1. Invalid entry sets
S=64 and skips the first read. Valid entry performs action0,value0, sets N=1
and caches the full callback return c in r20. For every nonzero base, including
an invalid nonzero base, call __div2u with numerator0xffffffffffffffff and
sign-extended32-bit base as the64-bit divisor. Cache the high/low quotient in T.
Base0 defers that call until base inference. This is unsigned division despite
sign extension of the divisor; the valid bases have upper word0.

The initial current-character register r20 is **not initialized** before the
valid-entry read. Invalid entry therefore reaches the common unread with the
incoming r20 value, unlike the32-bit helper's incoming width word. This is a
callee-saved register value, not a defined input character, fixed zero, width,
or original EOF result. The division service preserves it under the observed
ABI. The unconditional unread can decrement an original string-reader cursor
before its initial input even though no read occurred. No safe validation
fallback or executed fault behavior is established.

Loop gate: continue only if signed N<=width,c!=-1 and(S&0x60)==0. If S<=16,
index the original17-entry table at0x803bb940 by S*4. Handlers exist for1,2,4
and shared8/16; unused indices return to the gate. S32/64 terminate before
indexing. Byte255 remains a real character, distinct from full return-1.

## Whitespace/sign and prefix transitions (B)

| State | Behavior |
| --- | --- |
| 1 | If low8(c) classifies whitespace under mask0x06, read next c,W++,reuse1,N unchanged. Otherwise optionally consume '+'/'-' by reading next c,N++; '-' writes negative=1 after that read. Set2. |
| 2 | For base0/16 and c=='0', set4,read next c,N++. Otherwise set8 and reuse c. |
| 4 | For c=='x'/'X', set base16,state8,read next c,N++. Otherwise infer base8 only if base0, set16,reuse c. |
| 8/16 | Infer base10 if still0; initialize T if both words are zero; evaluate digit and accumulate as below. |

There is no leading-whitespace retry after a sign, duplicate sign syntax or
binary-prefix route. Explicit8/10 treats leading zero as an ordinary digit;
only base0/16 admits the prefix handler. A standalone leading zero can terminate
successfully in state4. A consumed x/X puts the scanner in state8 and requires
another valid digit. State16 means a leading zero without x or at least one
ordinary digit has been admitted.

Whitespace requests increment W without N, so field width does not bound total
callback reads. Sign, leading-zero/prefix and accepted-digit requests can fetch
one lookahead beyond width; that last read is excluded from accepted count and
unread at exit. Prefix state changes can reuse a current byte without consuming
another width unit. These are the same inspected control rules as the32-bit
helper, not an assumed library-standard contract.

## Digit admission and threshold compare (B)

Classify low8(c) using the original256-byte table. Mask0x10 chooses candidate
c-48; mask0xc0 chooses uppercase_table[low8(c)]-55. Accept when the signed
candidate is less than base. Original tables limit these branches to ASCII
0..9 and A..Z/a..z. No Unicode conversion is performed.

Rejecting a byte sets32 if prior state16, otherwise64. For each accepted d,
compare V and T as unsigned64 through chained subtract/carry instructions.
If V>T, store overflow1. This flag is sticky; the arithmetic still proceeds.
The subsequent multiply retains only low64 bits and explicitly includes
low-word product carry and cross-word products.

Let V=(H<<32)|L and base's sign-extended words be BH,BL. On the valid digit
path base2..36 means BH=0,BL=base; the actual word operations are:

```
PL = low32(L * BL)
PH = word32(high32_unsigned(L * BL) + low32(H * BL) + low32(L * BH))
P = (PH << 32) | PL
```

The first product high word is mulhwu; other products are mullw. Sign extension
also supplies d's high word DH; with original admitted ASCII digits DH=0.
Compute remaining=0xffffffffffffffff-P through subc/subfe, then compare it to
the two-word sign-extended d. If d>remaining under the chained unsigned borrow
comparison, store overflow1. This detects the addition edge after a wrapped
product, independently of V>T.

Set low V=word32(PL+d), and high V=word32(PH+DH+low-add carry) via addc/adde.
Set state16, read next c,N++, and retain the wrapped result even after overflow.
No saturation, digit truncation or early overflow stop occurs. The maximum
unsigned threshold is used, not0x7fffffffffffffff; a signed consumer can accept
and store bit patterns outside a signed64 mathematical range.

## Exit outputs and reader effects (B)

When the loop ends, if(S&0x34)==0 set V=0 and count=0; otherwise count is
word32(N+W-1). Reachable accepted terminal states are4,16,32. Success includes
skipped whitespace/sign/prefix in the count and excludes current lookahead.
Failure does not restore all provisional input bytes or clear a negative flag.
Always call readCallback(context,c,1), then return V high in r3,low in r4.
The original reader moves back one byte for a real lookahead; an EOF unread
clears its flag without moving. The consumer's later status query can therefore
observe a cleared EOF flag. The helper does not query callback status itself.

The32-bit [prefix/width examples](SCAN_INTEGER32.md) transfer to valid64-bit
entry conditions because these transitions/count gates were inspected here too.
Additional derived boundaries with ample width and base10:

| Input | Magnitude / overflow |
| --- | --- |
| `18446744073709551615` | high0xffffffff,low0xffffffff / 0 |
| `18446744073709551616` | high0,low0 / 1 |
| `-18446744073709551615` | same unsigned maximum magnitude / 0,negative1 |

These are mathematical deductions from the recovered multiply/add/threshold
operations, not executed traces. Negative application occurs afterward in the
scan consumer; the third row does not imply a signed/public API return of minus
the mathematical unsigned maximum. Longer digit strings continue accumulating
modulo2^64 while preserving overflow1.

## Evidence and remaining obligations

Whole-function fingerprint pins the selected body. Twenty-four independent
anchors cover entry/sign/prefix, both division calls, state dispatch, unsigned
compare, multiply/cross terms, add carry, width/EOF gate, count, unread and return
word order. A complete68-byte hash pins all17 state targets. Shared ctype and
uppercase tables are independently checked by the prior scan contracts. Original
ELF/disassembly/reports remain ignored; no original code is linked into the port.

Compare output words and callback sequences at2^32 and2^64 carry boundaries,
each base, whitespace/width limits, incomplete prefixes, sign failures, repeated
overflow, invalid entry and EOF unread. Include the original caller's selector4
argument/storage ABI; no inspected icon argument format in this research step
requests ll merely because the library supports it. Floating and multibyte
helpers, public-wrapper policies, actual runtime callers and executed scanner/
layout comparisons remain open. Full-game documentation/requirements remain
incomplete, and Switch behavior is not inferred from this GameCube helper.
