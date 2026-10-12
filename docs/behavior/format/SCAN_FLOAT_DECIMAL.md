# Floating scan finite decimal states and conversion handoff

US G8ME01 revision0, recovered2026-10-12. Evidence B: decimal regions of
__strtold0x8026d6d0,size0x100c and original constants. See
[entry/special values](SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md),
[scan consumers](SCAN_FORMAT_CONSUMERS.md) and [directive grammar](SCAN_DIRECTIVE_PARSER.md).
This recovers the decimal state machine and its local exit policy. [Hexadecimal states](SCAN_FLOAT_HEXADECIMAL.md) are recovered separately;
__dec2num internals remain open. No original execution, floating
accuracy measurement or host conversion equivalence is claimed.

## State, record and decimal-point dependency (B)

Use outer state S,current callback return c,N lookahead count,W whitespace count,
F negative flag and shared status word from the entry contract. Decimal storage
is a42-byte record initialized to zero: byte+4 retained count D,ASCII digit bytes
starting+5,halfword+2 exponent written at handoff. Scale adjustment A starts0;
explicit exponent E starts0; exponent-negative flag Q starts0. The record's sign
byte remains zero locally; final sign is applied separately.

The decimal point character is loaded once at entry through the pointer word
at0x803bb608. Its original initializer points to0x80303ba8, whose first byte is
ASCII'.'. No locale update/caller census was recovered; runtime mutation can
change the loaded character. Comparisons use that byte value rather than assuming
an unconditional literal period. Digits are ASCII0..9 under ctype mask0x10.

Every read-next below calls action0,value0, increments N and retains its result.
Transitions that reuse c do not increment N. The common gate checks width/EOF
before each handler, so trailing states at EOF or exhausted width can terminate
without executing their next transition.

## Mantissa transitions (B)

| S | Handler behavior |
| --- | --- |
| 2 | If c equals decimal point,read next,set16. Otherwise require digit or set4096. A zero reads next: if uppercase lookahead is X, set hex-mode flag and32768; otherwise set4. A nonzero digit sets8,reuses c. |
| 4 | Consume/read past each zero without storing. At other c,set8,reuse it. |
| 8 | On digit,store if D<20; otherwise A++. Read next,retain8. On decimal point,read next,set32. Other nondigits set64,reuse c. |
| 16 | Require digit after an initial decimal point; otherwise4096. On digit,set32,reuse c. |
| 32 | On digit with D<20,store unless it is a leading zero with D0; in either case A--. Read next. With D>=20,read next without storage or changing A. Nondigit sets64,reuses c. |
| 64 | If uppercase c is E,read next,set128. Otherwise set2048(success stop). |

Leading integer zeros do not spend retained-digit capacity or change A, but
remain part of input count. Fractional leading zeros decrease A without storage.
For integer digits beyond20, each discarded digit increases A. For fractional
digits beyond20, neither retention nor A changes. There is no scanner-side
rounding based on the first discarded digit or a sticky discarded-nonzero flag.
The later converter may round the **retained record**, which is a different
question from representing the full input exactly.

A zero lookahead X enters the uncovered hex path, even when the scan directive
was f/e/g; __sformatter delegates to the same helper without a conversion-letter
argument. The decimal states alone cannot establish hex grammar or acceptance.

## Explicit exponent (B)

State128 optionally consumes one '+' or '-', with '-' setting Q1, then sets256.
It does not remain in a repeated-sign state as the leading sign handler does.
State256 requires a digit, otherwise4096. On zero,read next,set512; on nonzero,
set1024 and reuse c. State512 consumes more zeros without changing E, then sets
1024 on another byte. State1024 on a nondigit sets2048; on a digit, compute
E=word32(10*E+c-48), test signed E>32767 and set status1 if true, then read next.
No saturation, immediate stop or32-bit overflow guard occurs. Status is sticky;
a later wrapped E cannot clear an earlier overflow indication. Exponent leading
zeros do not themselves set range status.

At common termination, replace S with S&0x0e2c,write count=N+W-1 if nonzero or0
otherwise, and unread current c. Reachable accepted decimal states are4,8,32,
512,1024,2048. States2/16/64/128/256/4096 are not accepted if the outer gate
terminates there. Thus a retained mantissa can coexist with count0 after an
unfinished exponent; this is not rollback to the earlier mantissa token.
The consumer gates assignment on count, rather than assuming a returned numeric
value implies a valid conversion.

## Record normalization and pre-conversion range exit (B)

On decimal-mode common exit, if Q!=0 negate E with word arithmetic. Trim trailing
ASCII'0' digits from the retained record,incrementing A once per removed digit.
If all retained digits disappear or none were stored,force D1 and digit'0'.
Set E=word32(E+A). If signed E is outside[-32768,32767],set status1. This status
is shared with exponent accumulation and is not recomputed from scratch.

If status is already nonzero, return without __dec2num:

- Q!=0: return original positive binary64 zero0x0000000000000000, ignoring F.
- Q==0: load binary64 positive infinity and negate it when F!=0.

This decision uses exponent-sign flag Q, not the mathematical sign of E after
adjustment or an IEEE underflow/overflow classification. In particular the
negative-exponent pre-status route returns positive zero even with a negative
mantissa flag. Do not replace this branch with a generic signed-zero saturation.
Count/pushback have already happened before these exits.

## Converter call, range checks and final sign (B)

With status0, store low16(E) into record+2,then call __dec2num0x80265804 with the
record pointer. Save returned f1 to local double storage. That helper's decimal
interpretation/rounding/mutation and exception behavior remain unrecovered here.
The local range policy then compares its returned magnitude:

| Original constant | Encoding / role |
| --- | --- |
| Positive zero | 0x0000000000000000 |
| Minimum normal double | 0x0010000000000000 |
| Maximum finite double | 0x7fefffffffffffff |
| Positive infinity | 0x7ff0000000000000 |

If result is nonzero and ordered less than minimum normal,set status1 while
retaining the saved result. Otherwise,if ordered greater than maximum finite,
set status1 and replace saved result with positive infinity. Converter zero
alone does not set this range status. Compare/unordered/FPSCR behavior requires
original processor validation; no general NaN policy is invented from these gates.

If F!=0 and the masked acceptance S is nonzero,negate the saved result. If count
was rejected and S became0,do not apply F on this ordinary converter route.
Reload saved double and return. This differs from the pre-status infinity exit,
which applies F without that acceptance check. The helper does not clear count
or undo pushback when a range flag is set. __sformatter itself does not reject
this auxiliary status,so successful count can still lead to float/double stores.

## Derived scan/record cases and scope

With original point'.' and ample width,these are branch deductions rather than
executed numeric conversions:

| Input | Decimal scan/handoff behavior |
| --- | --- |
| `0` | State4 accepts; normalized record has one zero digit,E0 |
| `1.` | State32 accepts at EOF; retained digit1,E0 |
| `.` | State16 rejects,count0 |
| `1e` / `1e+` | State128/256 rejects,count0; prior digit1 remains in local record |
| `1e0` | State512 accepts; retained digit1,E0 |
| `12300` | Trim two zeros; retained digits123,E2 |
| `0.00120` | Retained digits12,E-4 after trimming the final zero |

A21st integer digit is consumed and increases A instead of being retained;
a21st retained-position fractional digit is consumed without changing A.
Compare callback position,count and record separately from the eventual returned
binary64 and consumer's float narrowing. Original icon arguments use f conversion,
so both the helper's double and consumer's frsp boundary matter.

Twenty-four additional anchors check decimal transitions,20-digit cap,scale,
exponent/sign/status,trim/handoff and range/final-sign branches. Direct checks
pin the decimal-point initializer binding and four numeric constants. The prior
whole-helper fingerprint provides identity,not completed hex/converter recovery.
Original inspection artifacts stay ignored; no native implementation,original
execution,asset conversion or message rewrite occurs.

Next: all hexadecimal states/exits and __dec2num,then executed callback/record/
count/status/sign/rounding corpus including long mantissas,exponent wrap,finite
normal/subnormal boundaries and actual icon inputs. Full floating scan/layout,
story coverage and complete-game requirements remain incomplete. No Switch
behavior is inferred from these GameCube branches.
