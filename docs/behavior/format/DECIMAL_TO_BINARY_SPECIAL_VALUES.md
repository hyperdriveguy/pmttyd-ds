# Decimal-record conversion entry and special binary64 values

US G8ME01 revision0,recovered2026-10-12. Evidence B: entry0x80265804..0x802659e8
of __dec2num,size0x1080,original sign constants and classification/lowercase data.
**Scope:** entry and special shortcuts only. [Finite preparation/approximation](DECIMAL_TO_BINARY_APPROXIMATION.md) is now
recovered separately;boundary/refinement/final rounding remains open. A whole-function fingerprint identifies the
containing routine,not complete recovery/execution. See
[decimal scan handoff](SCAN_FLOAT_DECIMAL.md),[float entry/special tokens](SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md)
and [copysign service](FORMAT_BINARY_NUMERIC_SERVICES.md).

## Record interface and precedence (B)

Input r3 points to a decimal record; return is binary64 in f1. Relevant fields
are sign byte+0,unsigned count byte+4 and first ASCII/special byte+5. The decimal
scanner also writes a signed exponent halfword+2,but these shortcuts do not read
it. No caller-buffer write,allocation,RNG or errno update occurs in the inspected
entry paths. The pointer/allocation length is unchecked locally.

First test count. If zero,return signed zero without reading byte+5. Otherwise
inspect only byte+5 for literal ASCII'0','I','N'. These checks precede any finite
digit conversion and do not validate remaining bytes as a token or numeral.

| Record condition | Route |
| --- | --- |
| count0 | Signed zero |
| count>0,first byte'0' | Signed zero,regardless of subsequent digits/exponent |
| count>0,first byte'I' | Signed infinity,regardless of subsequent bytes |
| count>0,first byte'N' | Payload construction below |
| Any other first byte | Finite path,unrecovered here |

The sign byte is loaded,unsigned then sign-extended,and tested for zero. Any
nonzero value selects negative;it need not equal1. Original constants at
0x80428bc8/0x80428bd0/0x80428bd8 are positive zero,+1,-1 binary64 words.
Zero/infinity shortcuts call copysign with magnitude and the selected+1/-1.
The linked [service](FORMAT_BINARY_NUMERIC_SERVICES.md) copies the sign bit of
the second operand into the first's stored binary64 representation.

The infinity magnitude is loaded from original binary32 word0x7f800000 at
0x8041e3cc. There is no scanner-token loop in this converter:'I' alone routes to
infinity if count is nonzero. The scan helper's successful INF/NAN paths return
without invoking __dec2num,so their token rules and NaN source constants must
not be conflated with these record shortcuts.

## N seed, sign and payload capacity (B)

Initialize local high word0x7ff00000 and low word0. If sign byte is nonzero,set
high0xfff00000. Thus the initial pattern is signed infinity with a zero fraction.
If count==1,OR0x00080000 into the high word and return,creating a quiet-NaN
fraction bit. No following record byte is read on that path.

Otherwise set C=min(count,14),nonzeroSeen=0,packing parity=1,output byte pointer
at local binary64 byte1. Iterate record indices i=1..C-1:read byte record+5+i,
decode it as below,and pack into the fraction. At most13 payload bytes contribute
52 nibbles;count above14 does not increase the local payload loop. The first
'N' is not a payload digit. No payload spelling,separator,exponent or full-record
length check appears.

## Payload conversion and byte packing (B)

For input byte b,ctype mask0x10 chooses d=b-48. Otherwise directly read lowercase
mapping table0x803bb408 at index b and set d=low8(lower(b)-87). The original
table changes A..Z to a..z and retains other byte values. It does not validate
A..F/a..f or reject nonhex characters. A comparison of the unsigned-loaded byte
to-1 appears before the lowercase lookup,but cannot match a byte0..255.

For every d,if low8(d)!=0 set nonzeroSeen=1. Packing starts at byte1,whose upper
nibble is already0xf0 from the infinity exponent:

- parity1: OR low8(d) into the current output byte,then advance the output byte
  pointer. This puts an ordinary0..15 digit into the current low nibble.
- parity0: store low8(d<<4) into the current output byte without advancing.
- Toggle parity by the original count-leading-zero/shift expression,which maps
  these reachable parity values1/0 to0/1.

The first digit therefore fills fraction bits48..51,the second fills the next
byte's high nibble,then low nibble,and so forth through byte7. Digits outside
0..15 can set more than the intended low nibble on OR steps;the high-nibble
store truncates their shifted value. nonzeroSeen tracks decoded bytes,not an
independent test of the final52-bit fraction.

After the loop,if nonzeroSeen==0,OR0x00080000 into the high word. Otherwise do
not force a quiet bit. Reload the two local words with lfd and return. A nonzero
payload lacking the top fraction bit may form a signaling NaN;no explicit
arithmetic operation canonicalizes it here. Original lfd/return/FPSCR behavior
requires processor validation. Invalid payload characters can even leave an
infinity bit pattern despite nonzeroSeen1;there is no final NaN assertion.

## Exact branch-derived examples

These concern constructed local words,not an executed processor NaN result:

| Count / bytes beginning+5 / sign | Constructed shortcut pattern |
| --- | --- | --- |
| 0 / unread / 0 | +0 via copysign |
| 2 / `01` / nonzero | -0;second byte ignored |
| 1 / `I` / nonzero | Negative infinity via copysign |
| 1 / `N` / 0 | 0x7ff8000000000000 |
| 2 / `N0` / 0 | All decoded digits zero,force0x7ff8000000000000 |
| 2 / `N1` / 0 | 0x7ff1000000000000;quiet bit not forced |
| 2 / `NG` / 0 | G maps to g,d16;first OR into0xf0 changes no bit,nonzeroSeen1,pattern0x7ff0000000000000 |

Malformed records are not demonstrated original gameplay inputs. These cases
show why a host "parse NaN payload" utility or universal replacement NaN is not
established equivalent. A record starting0 is not parsed as a conventional
leading-zero decimal string by this entry;the scanner's leading-zero suppression
and normalization must be composed with the converter's precedence.

## Evidence and remaining work

Twenty-four added anchors pin count/first-byte precedence,sign selection,
copysign calls,NaN seed/count cap,digit conversion,parity stores,zero-payload quiet
bit and binary64 reload. Direct checks pin the three sign/zero double constants
and the complete256-byte lowercase table. Existing evidence pins infinity and
ctype. Local ELF/disassembly/report artifacts remain ignored;no original code
is compiled/distributed or message/asset data converted by this step.

Next recover finite input copying/ASCII-to-numeric digits,initial approximation,
comparison/refinement/rounding and boundary/sign exits in the remainder of
__dec2num. Validate record/output/FPSCR and scan composition against original
execution,including short/malformed records only when within the validation
scope. Complete scanner/layout/story/game documentation and requirements remain
incomplete;no Switch numeric behavior is inferred.
