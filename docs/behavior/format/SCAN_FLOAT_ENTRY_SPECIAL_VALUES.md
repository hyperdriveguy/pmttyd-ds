# Floating scan entry, leading state and special values

US G8ME01 revision0, recovered2026-10-12. Evidence B: __strtold at0x8026d6d0,
size0x100c, original initializer/token/constants and selected instructions.
**Coverage boundary:** this document recovers entry, outer gate, leading state,
infinity and NaN paths. [Finite decimal states/exits](SCAN_FLOAT_DECIMAL.md) are recovered separately;
[hexadecimal states](SCAN_FLOAT_HEXADECIMAL.md) are also recovered; __dec2num
remains open. A whole-function fingerprint identifies the containing body; it
is not a claim that the entire scanner has been recovered or executed.
See [scan consumers](SCAN_FORMAT_CONSUMERS.md), [directive grammar](SCAN_DIRECTIVE_PARSER.md)
and [reader protocol](SCAN_INPUT_RUNTIME.md). No original execution occurred.

## Entry, outputs and outer gate (B)

Arguments are width r3,readCallback r4,context r5,countOutput r6,statusOutput r7.
Return is floating f1; no long-double memory return is observed. The consumer
later stores a float for selector0 or a double for5/6. Destination pointers
are fetched/written by that consumer, not by this helper's entry.

Entry stores the width/count pointer locally, clears statusOutput through its
unchecked pointer, initializes internal counters/flags, and copies a42-byte
all-zero decimal record template from0x80303c00. A pointer loaded from
0x803bb608 supplies a first-byte decimal-point character for later finite
states; that runtime data dependency is separate from special-token recognition.
No local locale mutation or errno write occurs at entry.

Initialize state S=1,leading-whitespace count W=0,lookahead-request count N=1,
negative flag F=0,sign-present flag P=0. Perform a read callback action0,value0
**before** any width validation. Retain its full return c, then enter the gate.
Continue only when signed N<=width,c!=-1 and(S&0x1800)==0. A zero/negative width
therefore still causes the initial read and later common unread. This differs
from the integer scanners' invalid-entry/no-read route.

States2048/4096 stop via that mask. On common termination, the code replaces S
with S&0x0e2c. Nonzero writes count=word32(N+W-1); zero writes count0. Always
unread current c through callback action1 on this common path, then enter the
finite conversion tail. The tail's returned numeric value/status is not covered
here. Special successes described below bypass this common gate/count/unread.

The state-dispatch comparisons route1 to leading handling,2/4/8/16/32/64/128/
256/512/1024 to finite handlers,16384 to infinity and8192 to NaN. The numeric
state semantics and digit/exponent accumulation require the remaining analysis;
do not label them established decimal/hex grammar solely from their addresses.

## Leading whitespace, signs and token routing (B)

In S1, low8(c) classification mask0x06 recognizes TAB,LF,VT,FF,CR,space.
For whitespace, read next c,W++,leave N and S unchanged. Thus field width does
not count leading whitespace requests. Nonwhite c is uppercased through the
original table unless full c==-1, which retains EOF. ASCII lowercase/uppercase
I or N selects the following special handlers after consuming that initial
letter by a read-next request,N++,state16384/8192.

'+' reads next c,N++,sets P=1 and leaves S1. '-' additionally sets F=1 before
that read, then uses the same P write. The state remains1, so repeated signs,
and whitespace between signs or before the next token, can pass these branches.
A later plus does not clear F. P is a boolean, not a sign count; several consumed
signs still contribute only one in special-success count formulas. Other bytes
switch to S2 without consuming c, for the finite scanner to interpret.

These are literal local branches, not accepted modern floating grammar. For
example multiple signs can reach a special token if the outer width admits the
reads, but finite acceptance after such a prefix remains a separate dependency.

## Infinity handler (B)

After consuming the first I, copy original nine bytes `INFINITY` plus NUL from
0x80303c2c to local storage. Set token index K=1 and compare current c against
uppercase expected token[K]. For each match while K<8, read next c,advance
expected pointer,K++,N++. This inner loop has no width gate: once the outer loop
admits state16384, later reads can exceed the parsed field width. EOF is compared
as-1, not uppercased into a byte. Matching the final Y still reads one more c.

Stop on mismatch/EOF or K==8. Accept only K==3 or8. Therefore `INF` can succeed
when the next read does not match the next expected I; `INFI` followed by a
mismatch fails rather than rolling back to INF. K8 accepts the full spelling.
On failure set S=4096 and use the common termination path: count0,unread current
c,then finite tail. Previously consumed token letters are not rewound locally.

On success, load binary32 positive infinity0x7f800000 at0x8041e3cc into f1;
if F!=0, negate it. Store a local double copy,write count=word32(K+P+W),and
return immediately. statusOutput remains entry0 on this path. There is **no
unread**, so the final mismatching/lookahead byte has been consumed by the
reader; a terminal NUL read leaves its EOF flag set. The count excludes that
lookahead despite the cursor effect. No payload, numeric converter or exponent
logic is involved.

## NaN prefix and optional payload (B)

After consuming first N, copy five bytes `NAN(` plus NUL from0x80428c08 into
local storage. Initialize token index K=1,payload counter Q=0. While K<4,
compare current c to uppercased expected byte; each match reads next c and
increments K,N. Accept only K==3 (bare NAN when next byte is not '(') or4
(the opening parenthesis matched). Other indices set S4096 and use common
termination. As with infinity, these inner reads have no width gate.

For K4, read a payload while Q<32 and low8(c) classification has digit mask0x10
or alphabetic mask0xc0. Each admitted byte reads next c,Q++,N++. Only ASCII
letters/digits are admitted by the pinned classification table; underscore is
not admitted. Stop at a nonalphanumeric or Q32, then require current c==')'.
If not, fail through S4096/common termination. If closing ')' is present,
Q++ includes it in the count, with **no read after the closing bracket**. Empty
payload succeeds; exactly32 alphanumeric payload bytes can succeed if followed
by ')'. Missing ')' or a33rd alphanumeric fails. There is no payload numeric
interpretation, hash or copied payload retained for the returned NaN.

On either success route, load binary32 NaN word0x7fffffff at0x8041e3c8; negate
when F!=0,store a local double copy,and write count=word32(K+Q+P+W).
Return immediately without unread. Bare NAN consumed the byte/EOF lookahead
used to reject '('; parenthesized NAN consumed exactly through its closing ')'.
statusOutput remains0. The exact double NaN payload, sign/FPSCR effects of lfs
and fneg require processor execution; do not claim a canonical host NaN bit
pattern from the source word alone.

## Derived callback boundaries and scope

With an ample width and the original string reader, these are static deductions:

| Input | Special-path behavior |
| --- | --- |
| `INFz` | Infinity,count3; z is read and not pushed back |
| `INFIz` | Special token fails; common count0 and unread z; earlier letters remain consumed |
| `NANz` | NaN,count3; z is read and not pushed back |
| `NAN()` | NaN,count5; closes without reading after ')' |
| `NAN(a_b)` | Payload stops at underscore, fails; common unread restores underscore only |
| `--INFz` | Negative infinity; count4 although two sign bytes were consumed |

For `INFz` with width2, the initial I/read-next reaches the special state with
N2 and is admitted. The inner loop can finish INF and read z despite that width.
With width1, the outer gate stops before the special handler. This does not
establish the uncovered finite-tail return for that failed-width case.

Whole-body fingerprint and24 selected anchors check entry/read-before-width,
leading/sign states, token index acceptance, payload bound, signed special
loads, count stores and bypass of common unread. Direct checks pin the42-byte
zero template, both token strings and the two original binary32 special words.
Original disassembly/report artifacts remain ignored; no executable linking,
asset conversion or message rewriting occurs.

Next recover every finite state, decimal record/digit/exponent handling,
hexadecimal bit construction, __dec2num and range/sign exits. Then validate
complete callback traces, status/count/output/FPSCR and original icon float
arguments. This special-path contract is not complete floating scanner, text
layout or full-game equivalence; Switch behavior is not inferred.
