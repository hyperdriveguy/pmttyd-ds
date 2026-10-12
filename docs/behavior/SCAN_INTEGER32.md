# Original unsigned32 scan state machine

US G8ME01 revision0, recovered2026-10-12. Evidence B: __strtoul at0x8026eae8,
size0x378, original17-entry state table and classification/uppercase tables.
See [scan consumers](SCAN_FORMAT_CONSUMERS.md), [directive widths](SCAN_DIRECTIVE_PARSER.md)
and [string reader](SCAN_INPUT_RUNTIME.md). No original execution or host strtoul
comparison was performed. This is an internal callback scanner, not the complete
public strtoul API or the64-bit helper.

## Interface and distinct outputs (B)

Arguments in r3..r9 are base,width,readCallback,context,countOutput,
negativeOutput,overflowOutput. Return r3 is a32-bit unsigned magnitude before
sign application. Initialize overflow=0 and negative=0 through their pointers;
count is written at termination. Output pointers are unchecked. There is no
local errno write, signed-limit saturation or multiplication by the sign flag.
The [consumer](SCAN_FORMAT_CONSUMERS.md) applies word negation and destination
narrowing separately, and does not reject a nonzero overflow flag locally.

Let S=1,V=0,N=0,W=0,T=0, where S is state,V accumulated magnitude,N counted
nonwhitespace lookahead requests,W skipped whitespace count and T overflow
threshold. States use numeric values, not arbitrary enum spellings:

| S | Role |
| --- | --- |
| 1 | Leading whitespace and optional sign |
| 2 | Leading-zero/prefix eligibility |
| 4 | A zero was accepted; possible following x/X |
| 8 | Digit required, including after a hexadecimal prefix |
| 16 | At least one ordinary digit accepted |
| 32 | Stop successfully on a nondigit after zero/digits |
| 64 | Stop with no accepted conversion |

The17-entry table maps1/2/4 to distinct handlers and8/16 to a shared digit
handler; other indices0..16 return to the loop test. States32/64 terminate via
the state mask and are not indexed into that table.

## Entry validation and lookahead (B)

Accept base0 or signed base2..36 and signed width>=1. Invalid base (negative,1,
above36) or width sets S=64 without initial read. Otherwise request one read
(action0,value0), set N=1 and retain its full callback result as current c.
For any nonzero base, calculate T=unsigned(0xffffffff)/base with divwu even on
the invalid-base route; base0 leaves T=0 until base inference. No caller-visible
invalid-base error code is written in this helper.

Before each state handler, continue only when signed N<=width,c!=-1 and
(S&0x60)==0. Callback return255 is distinct from-1 here. Width is not a byte
capacity or bound on total callback calls: whitespace does not increment N and
several state changes process the same lookahead without a new read. Successful
digit/sign/prefix processing can fetch one byte beyond the admitted width;
termination always unreads that last lookahead through action1.

On an invalid entry with no read, the common unread still runs with the incoming
r4 width as its value. With the original reader and clear EOF flag, this decrements
its cursor before the input start. Do not infer safe no-op validation from the
zero return/count. Actual address effects remain unexecuted.

## Whitespace, signs and base inference (B)

State1 classifies low8(c) through the original table mask0x06. On whitespace,
fetch another byte, increment W, retain S=1 and leave N unchanged. It can skip
arbitrarily many whitespace bytes despite a small field width. Once nonwhite,
'+' or '-' fetches one more byte and increments N; '-' also writes negative=1
after that read. Then set S=2. With no sign, reuse c and set S=2. There is no
second sign/whitespace pass after the first sign.

State2: only base0 or16 permits the special zero-prefix route. If c=='0', set
S=4, fetch another byte and increment N. Otherwise set S=8 without consuming c.
Other explicit bases, including8 and10, process zero as an ordinary digit.

State4: if c=='x' or'X', set base=16,S=8, fetch another byte and increment N.
Otherwise, if base was0 set base=8; set S=16 and reuse c. Thus a leading zero
can be a successful conversion even before ordinary digit accumulation, but
an x/X prefix requires a digit afterward. State4 at width/EOF termination counts
as accepted; state8 after a bare prefix does not.

At a digit-handler entry, base0 becomes10. If T==0, set T=0xffffffff/base.
A leading0 selected base8 before this point; a consumed0x selected16. No binary
0b prefix or locale-specific base rule appears locally.

## Digits and overflow (B)

Use classification of low8(c). If mask0x10 is set, candidate digit=c-48;
accept when signed digit<base. Otherwise, when mask0xc0 marks alphabetic input,
look up uppercase byte through table0x803bb508, set digit=upper(c)-55, and accept
when signed digit<base. The original tables classify ASCII digits and letters;
uppercase maps a..z to A..Z and retains other bytes. There is no Unicode digit
or alternate locale alphabet handling in this local path.

A rejected digit/nondigit sets S=32 if previous S==16, otherwise64. This preserves
a successful leading zero or accumulated digits; it rejects the first required
digit. For an accepted digit d:

```
if unsigned(V) > T: overflow = 1
P = word32(V * base)
if unsigned(d) > unsigned(0xffffffff - P): overflow = 1
V = word32(P + d)
S = 16
fetch next c with action0
N = word32(N + 1)
```

The overflow flag is sticky: no later valid digit clears it. Multiplication and
addition still wrap, and scanning continues rather than returning0xffffffff or
stopping at the first overflowing digit. T is only the quotient threshold; the
second compare handles the product-plus-digit edge. Signed/unsigned comparisons
and word arithmetic are distinct and should be preserved in a port.

## Termination, count and reader position (B)

At any terminal gate, if (S&0x34)==0, set V=0 and count=0. Otherwise count is
word32(N+W-1). The accepted states reachable here are4,16,32. A count includes
skipped whitespace, sign and accepted prefix/digits, but excludes the final
lookahead read. Failure does not rewind all tentative bytes: it only unreads
current c. Earlier whitespace, sign or prefix requests remain consumed even
when count=0. negative can remain1 on a failed conversion.

Always call readCallback(context,c,1), then return V. At a real-byte lookahead,
the original reader decrements cursor; at EOF it clears its EOF flag without
moving cursor. It does not implement a full transactional rollback. This can
change the scan consumer's final EOF-vs-assignment return decision.

Useful exact branch deductions with the original reader:

| Input / base / width | Local result and stopping behavior |
| --- | --- |
| `0` / 0 / ample | V0,count1, accepted state4; EOF unread clears flag |
| `0x` / 0 / ample | V0,count0, state8; prefix remains consumed |
| `0x1` / 0 / 1 | V0,count1,state4; unread x, leaving it as next input |
| `0x1` / 0 / 2 | V0,count0,state8; unread1, leaving consumed0x |
| `+1` / 10 / 1 | V0,count0,state2; unread1, leaving consumed plus |
| `09` / 0 / ample | V0,count1; base8 rejects9 after accepted leading zero and unreads9 |
| `4294967296` / 10 / ample | V0,overflow1; digit sequence remains a successful conversion |

These are derived cases, not execution samples or universal public-API promises.
For example the consumer can store negated wrapped magnitude despite overflow,
because its assignment gate is count rather than this flag.

## Evidence and port obligations

The evidence report fingerprints the whole selected helper. Twenty independent
anchors pin entry bounds, sign/prefix states, threshold, digit route, overflow,
width/EOF termination, count and unconditional unread. Complete hashes pin the
17-entry state table and256-byte uppercase table; the classification table is
already checked by scan-runtime evidence. No executable is compiled/distributed
in this step, and disassembly/report artifacts remain ignored.

Validate callback traces and all output words, not just final integers: leading
whitespace versus width, signs, base0/16 prefixes, invalid bases/widths, empty or
incomplete conversions, threshold boundary and repeated overflow, high bytes and
EOF unread. Then compose with destination stores, parsed wrapped widths and the
actual original icon argument corpus. The64-bit and floating/multibyte helpers,
public wrappers/callers, executed layout comparisons and complete-game systems
remain open. No Switch behavior or full-game equivalence is asserted.
