# Hexadecimal floating scan and direct binary64 construction

US G8ME01 revision0,recovered2026-10-12. Evidence B: hexadecimal regions of
__strtold0x8026d6d0,size0x100c. See [entry/special values](SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md),
[decimal states](SCAN_FLOAT_DECIMAL.md),[scan consumers](SCAN_FORMAT_CONSUMERS.md)
and [reader](SCAN_INPUT_RUNTIME.md). This completes the local hex regions;
__dec2num and executed floating equivalence remain open. No original code was
executed or linked into the native port.

## Entry, scratch and two state variables (B)

Decimal state2 consumes an initial0 and recognizes uppercase X in its lookahead.
It sets mode/hex-state H=1 and outer state S=32768. Current c is still X. The
outer width/EOF gate can stop here before initializing hex scratch. If admitted,
H1 clears eight bytes at stack+0x18,sets nibble index J=2,H=2,reads past X and
increments outer lookahead N. Those eight bytes are the later binary64 scratch R.
Leading integer digit count K=0,explicit exponent E=0,exponent-byte counter Z=0
and exponent-negative flag Q=0 were initialized at function entry.

The outer state remains32768 during ordinary hex handlers; H routes1,2,4,8,16,
32,64,128,256. It is also the flag selecting the hex exit instead of decimal
conversion. Error/success handlers can set outer S4096/2048,then the common gate
writes its preliminary count and unreads c. These state roles must not be
collapsed into a single generic parse-success flag.

## Mantissa scan and packed digits (B)

| H | Operation |
| --- | --- |
| 2 | If c=='0',read next,N++;no digit/count/packing update. Otherwise set4,reuse c. |
| 4 | Admit ctype mask0x20 hex digit and pack below. Otherwise on configured decimal point,read next,set8;else set16,reuse c. |
| 8 | Admit hex digit and pack below. Otherwise set16,reuse c. No leading-zero suppression here. |
| 16 | If uppercased c is P,read next,H32,Z++,N++;otherwise set outer S2048. |

Hex mask0x20 is ASCII0..9,A..F,a..f in the pinned classification map. Digit d
is uppercase(c)-55 for A..F,otherwise uppercase(c)-48. On H4/H8,when signed
J<17,read scratch byte at trunc(J/2),OR d into its high nibble if J even or low
nibble if odd,store the low byte,and J++. H4 also K++;H8 does not. Read next,N++.
When J>=17,consume/read hex digits without changing J,K or scratch.

J starts2,so admitted packing indices are2..16: fifteen nibbles. The first
fourteen occupy scratch bytes1..7; index16 accesses byte8 at stack+0x20,
adjacent to the eight-byte R region. That byte is not cleared by H1 and is ORed,
not replaced. It is not included in the later eight-byte double load. This is a
local stack access within the frame,not proof of an allocation fault; do not
silently expand the eight-byte returned mantissa to include it.

Leading integer zeros are skipped in H2 without K/J updates. Fractional zeros
in H8 are packed and advance J,with K unchanged. Digits beyond the packing limit
have no sticky-rounding or scale counter. Decimal-point consumption itself does
not change K/J. No required-digit check precedes H16 or the local raw-zero exit.
The scanner does not receive the original f/e/g/a directive byte; all those
consumer routes share this helper's prefix detection.

## Binary exponent scan (B)

H32 initially Z++. If c=='-',set Q1; '+' is also consumed through read-next.
For other c,unread it through action1,Z--,then read it again through action0.
That reread increments N even though it restores/reconsumes the same byte.
Set H64. This is a callback/count difference from decimal exponent parsing.

H64 requires an ASCII digit;otherwise outer S4096. If zero,read next,H128,Z++,N++;
otherwise H256 reuses c. H128 consumes further zeros with reads/N++ but does
not increment Z;other bytes set256,reuse c. H256 on a nondigit sets outer S2048.
On digit,set E=signed16(10*E+c-48),Z++,read next,N++.

The status test in that exponent handler compares **r28**,the separate decimal
exponent accumulator,against32767. It does not compare the newly narrowed hex
E in r22. On the inspected hex-only path r28 remains0,so that test does not flag
hex exponent overflow. E wraps at16 bits on every digit. No replacement range
check before direct bit construction repairs it. This is original local behavior,
not a claim of mathematically correct hexadecimal floating conversion.

## Common exit and raw-zero branch (B)

The outer gate/count/unread executes as in the entry document. Ordinary hex
S32768 masked with0x0e2c becomes0,while explicit success S2048 remains nonzero.
After unread,nonzero H selects the hex tail and loads R as a double. Compare it
to positive zero. If equal,write positive zero back and return,without applying
F or rewriting the preliminary count. Thus a raw-zero exit need not share the
nonzero mantissa count/sign policy.

A width/EOF stop immediately after recognizing0x can occur before H1 clears R.
The tail then reads uninitialized local scratch. No defined numeric fallback
is established for that route. Do not infer safe count0/zero output just from
the preliminary count gate; the tail still runs.

## Right-shift normalization and exponent insertion (B)

For nonzero R,if Q!=0 set E=signed16(-E). Repeatedly test the first unsigned
halfword of R: continue shifting while (halfword&0x00f0)!=0x0010. Each iteration
logically shifts the entire64-bit R right one bit,via two words/rotates/insert,
and increments E as a word. Once the target nibble is1,retain only the low
four bits of that first halfword (clear the leading normalization bit and other
header bits),then set E=signed16(E+4*(K-1)).

Insert low12 bits of E+1023 into the halfword's upper12 positions,retaining its
low4 fraction bits. Reload R as binary64. There is no clamp of E before bit
insertion,no floating multiply/ldexp call,and no round-to-nearest stage for bits
discarded by right shifts or packing. Exponent wrap can affect the sign/exponent
field before the later F sign step.

The normalization has no zero/iteration-limit test inside its loop. If R is a
nonzero bit pattern below2^52,logical right shifts can never make bit52 set;
after reaching zero,the test remains false forever. H8 can produce such a
pattern: a leading fractional zero followed by a nonzero digit packs the first
nonzero below that target. For example0x.01p0 yields a nonzero R below2^52 under
these instructions and,if the initial floating comparison takes the nonzero route,is a derived
nonterminating normalization case. Original denormal/FPSCR handling must be
verified before claiming it occurs on the console. This is
static reachability reasoning,not an observed gameplay hang; actual caller
coverage and original execution remain unverified. No host workaround should
be described as original behavior.

## Count rewrite, numeric bounds and sign (B)

After exponent insertion,write count=word32(P+J+1+Z+W),where P is the leading
sign-present boolean and W the skipped-whitespace count. This overwrites the
common count regardless of its masked acceptance result. It uses packing index
J and exponent counter Z,not total read count N. Integer leading zeros,ignored
mantissa digits,repeated signs,and repeated exponent-leading zeros can therefore
produce a different count from actual consumed input. Failure/EOF after a
nonzero mantissa can still reach this positive-count rewrite; consumer assignment
is based on that final count.

If constructed R is nonzero and ordered below the original minimum-normal double,
set status1 and replace R with positive zero. Unlike the decimal post-converter
subnormal branch,this hex branch does not retain that tiny result. Otherwise if
ordered above maximum finite,set status1 and replace with positive infinity.
Zero alone does not trigger those comparisons. NaN/unordered and FPSCR results
require execution; the local status flag does not prevent a consumer assignment.

If F!=0,OR0x8000 into R's first halfword,without testing masked acceptance S.
Return the resulting double. This can apply sign to a range-flushed zero;the
separate initial raw-zero exit instead returned positive zero. The sign step
is bit insertion,not fneg. It does not clear a sign bit already inserted through
wrapped exponent construction.

## Evidence and remaining work

Thirty-two additional anchors pin scratch clearing,nibble limits/parity,
integer/fraction count updates,exponent reread/narrowing/wrong-register test,
logical normalization,exponent insertion,count rewrite,range flush and sign.
Read-only classification checks pin all22 hex digit codes. Prior fingerprints
and numeric constants identify the containing helper/data. Inspection artifacts
remain ignored;no assets/messages are converted or rewritten.

Together with entry/special and decimal documents,these establish the inspected
local __strtold body; its __dec2num dependency remains unrecovered. Validate
callbacks/raw scratch/count/status/results for short prefixes,zero/nonzero,
capacity boundaries,leading zeros,exponent wrapping,normalization termination
and range/sign behavior before claiming executable equivalence. Compare actual
original icon input corpus;the library paths are not proof that malformed/hex
inputs occur in gameplay. Full text/layout/story and game-system coverage remain
incomplete,and no Switch behavior is inferred.
