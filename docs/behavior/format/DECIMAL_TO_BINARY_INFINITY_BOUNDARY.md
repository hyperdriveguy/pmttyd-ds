# Decimal converter infinity candidate boundary

US G8ME01 revision0,recovered2026-10-12. Evidence B: __dec2num
0x80265d3c..0x80265f54 and shared finite sign exit0x8026684c..0x80266868,
within containing routine0x80265804,size0x1080. See
[approximation/classification](DECIMAL_TO_BINARY_APPROXIMATION.md) and
[entry shortcuts](DECIMAL_TO_BINARY_SPECIAL_VALUES.md). This covers the infinity-
candidate branch;general finite refinement/ties remain open. No original
execution,correct-rounding measurement or Switch behavior is asserted.

## Trigger and boundary record (B)

After the initial candidate's raw-bit classification,only class2 (exponent all
ones,fraction zero) enters this branch. It does not independently prove the
input's mathematical magnitude is out of range:pow/ldexp may produce an infinity
candidate before the exact local comparison decides whether to keep it.

Build a local numeric decimal record at stack+0x174. Set sign byte0,exponent
halfword308,count initially0. Read the original NUL-terminated ASCII seed at
0x80303ac8,copy at most36 bytes into digits+5 after subtracting48,and store the
count. The inspected seed is exactly36 decimal digits followed by NUL;its
SHA256,excluding NUL,is
`d919e2c8d08e6b560f5ec6798495d6096505c7545339dd17d6c553652d55f407`.
The first byte is1 and the last three ASCII bytes are420. The full original
seed remains local data rather than a replacement handwritten port constant.

There is a tail-rounding branch if the seed continues after those36 digits:
NUL skips rounding;otherwise sign-extend the next **ASCII byte** and compare it
to numeric5 without subtracting48. Less than5 retains;greater than5 rounds up.
Exactly5 scans the remaining bytes for ASCII'0',then uses last retained numeric
digit parity for an all-zero tail tie. Any nonzero tail byte rounds up.
Round-up increments the last numeric digit below9;otherwise clear carried digits
to0 moving left. At the first digit on a complete carry,set1 and increment the
exponent halfword. The retained count is unchanged.

For the pinned seed,the next byte is NUL,so the tail branch is inactive. The
ASCII-versus5 comparison must not be silently described as a normal decimal
rounding helper;its generic meaning differs from subtracting48 first. This is
a local fixed-seed path,not a recovered user-selectable threshold generator.

## Comparison against prepared input (B)

Let T be the boundary record and X the private prepared input at stack+0x1a0.
X already has numeric digits and leading-digit exponent E+D-1,as documented in
[preparation](DECIMAL_TO_BINARY_APPROXIMATION.md). Compare magnitude only;input
sign is ignored until the shared sign exit. The local decision computes whether
T<X,using this order:

1. If T's first numeric digit is0,return true iff X's first digit is nonzero.
2. Otherwise if X's first numeric digit is0,return false.
3. Compare signed16 exponents;unequal exponents decide T<X.
4. With equal exponents,compare unsigned numeric digit bytes lexicographically
   across min(T.count,X.count). First unequal byte decides T<X.
5. If all common digits match and T's count was exhausted,scan any remaining
   X digits. Any nonzero byte makes T<X;otherwise false.
6. If only X's shorter count was exhausted,return false. Equal represented
   values also return false;the relation is strict.

Exponent ordering uses the original xor/arithmetic-shift/and/subtract/sign-bit
expression rather than a library comparator. The signed16 domain makes its
intended ordering relevant;do not replace the byte/count checks with a string
comparison or require equal stored digit counts for equality. Extra zero digits
in X do not make it larger. The loop does not validate digits0..9,record capacity
or canonical leading zeros locally.

This comparison body is inlined here. Other similar compare sites in the later
refinement body are not claimed covered merely because their instructions look
similar;each caller/operand/direction still needs inspection.

## Infinity retention versus finite recovery (B)

When T<X is true,branch directly to the shared finite sign exit with the existing
infinity candidate retained. When false,load original binary64 maximum finite
word0x7fefffffffffffff from0x8041e3d0 into the candidate storage,then continue at
0x80265f54 into ordinary refinement. Equality therefore substitutes maximum
finite and continues;it is not a final-return-at-threshold shortcut.

The shared sign exit reads the private record sign byte,sign-extends/tests zero,
and for any nonzero value performs fneg on the stored candidate. Reload and
return binary64 f1. The infinity-retention branch therefore returns negative
infinity for a nonzero input sign. Refined final candidates can also reach this
exit,but their selection remains uncovered. No local count/status pointer or
errno is supplied to __dec2num;scanner range status is a later consumer policy.

## Boundary identity versus mathematical threshold

The seed/exponent are original evidence,not a measured universal overflow rule.
For canonical positive digits and exponent309 versus this boundary308,the
strict comparison selects infinity retention;for exponent307 it selects maximum-
finite substitution and refinement. Same-exponent decisions depend on digits.
Those are branch deductions,not executed final numeric results for the continued
refinement route.

A system requirement for faithful conversion must preserve the original boundary
resource,comparison direction,equality behavior and later refinement. A host
"clamp at DBL_MAX" or generic decimal overflow detection is not established
as equivalent by this branch. Scanner-retained20-digit inputs and other public
record producers may have different coverage near the36-digit boundary;actual
caller/domain evidence remains necessary.

## Evidence and next work

Twenty-four added instruction anchors pin record preparation,36-byte cap,
original tail/parity/carry branch,first-digit/exponent/common-prefix/trailing
checks,strict retention,maximum-finite substitution and sign exit. Read-only
checks pin the complete seed via hash/NUL and maximum-finite bits. Prior
whole-body identity pins the containing converter. Inspection disassembly/
reports remain ignored;no original executable or seed payload is linked into
the native code,no asset conversion or story rewriting occurs.

Next: general candidate decimalization/equality and refinement direction,
representable-neighbor construction,difference/tie rounding and final candidate
selection;pow and numerical dependencies. Validate boundaries/private records/
final bits/FPSCR against original execution. Complete numeric/layout/story/game
coverage and complete-game requirements remain unfinished.
