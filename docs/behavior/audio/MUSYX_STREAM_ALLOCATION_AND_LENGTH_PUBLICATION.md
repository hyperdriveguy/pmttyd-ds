# MusyX stream allocation, public ID and length publication

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete sndStreamAllocEx8027b330/4a0 and
sndStreamAllocLength8027b7d0/44hex. Offsets/addresses/masks hexadecimal;
counts/shifts decimal. Static source deductions, not original allocation/audio
execution. Read [lookup/mix](MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md),
[stream buffers](MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md),
[hardware stream bridge](MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md),
[filter coefficients](MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md) and
[stream handler](MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md).

## Raw ABI captures and first free row

Label register inputs A..H=r3..r10. Incoming stack arguments I/J are low bytes
at oldSP+b/f; K/L/M/N are words at oldSP+10/14/18/1c. The body captures all
before hwDisableIrq80292810. No parameter pointer/value/activity/system check.

Scan streamInfo803f98e8: exactly64 rows, stride68hex, eight iterations of eight
unrolled state+c reads. First EXACT0 state selects ordinal i; every nonzero
state occupies a row. No synthInfo count bound or public ID matching in this scan.
All64 occupied: select returnffffffff, skip all record/ID/resource operations,
request hwEnableIrq and return. No timeout, reclamation or voice stealing.

## Candidate IDs and collision retries

Read nextPublicID8041eea8 once into proposed candidate. Each attempt takes raw
candidate X and prepares next=wrap32(X+1). Xffffffff skips that sentinel: candidate
becomes0, next1. Scan all64 rows ascending, only nonzero state rows read public
ID word4. Any exact match retries from the already-advanced next value; it does
not reread the global counter. Inactive rows' stale IDs are ignored.

No generation/index encoding, fixed retry cap or candidate preference. Once no
active match, commit nextPublicID=next, selected row.word4=candidate and word8=K,
in that order. This commit precedes length/buffer/activation; failures do not
restore counter or row ID/flags.

## Complete wrapped byte-length leaf

Request sndStreamAllocLength(C,K). K&1 clear: bytes=wrap32(2*C). Set:
bytes=8*floor(wrap32(C+13)/14), using original24924925 quotient sequence.
Then return wrap32(bytes+31)&ffffffe0 in both cases. Both additions/shift wrap
BEFORE alignment. No minimum, maximum, zero rejection or allocation here.
Large lengths can return0; format flag uses only mask1, not a validated enum.

Retain returned byte length Q. Store selected row in order: word14=B, word18=C,
word1c=Q, word10=L, word4c=ffffffff. No cursor20/whole-record clear.

## Format and coefficient publication

K&1 set: if N!=0, sequentially load signed halfwords N+0..1e and immediately
store each low16 to row2c..4a (16 values); then store half24=8. No pointer/length/
coefficient validation or source snapshot; aliasing can alter later reads.
N0 retains old coefficients and half24. Both branches then store format byte0d=1.
K&1 clear stores format0d=0, leaving these coefficient/half24 fields untouched.

## Frequency, volumes, mode and default filter

Capture synthFlags8041ee8c BEFORE the following record fields. Store:

| Field | Retained input |
| --- | --- |
| word54 | D |
| byte60 | J |
| byte58 | low8(A) |
| byte59 | low8(E) |
| byte5c | low8(H) |
| byte5d | I |
| byte5e | low8(F), requested pan |
| byte5f | low8(G), requested surround |

Captured mode mask1 selects effective pan64/surround0; otherwise mask2 clear
uses F/0, mask2 set uses F/G. Read streamDefaults8041eeac.byte0, then store
selected low bytes5a,5b and captured default byte61, in that order. Requested
5e/5f are not overwritten. No gain127 cap or mode recapture.

Default byte0 nonzero: freshly read defaults.word4 and request hwLowPassFrqToCoef
(frequency,&row.half62,&row.half64). Zero: store half64=0 THEN half62=0.
The filter child's return is ignored; later fields are still published.

## State3 before buffer initialization, optional activation and failure

Store row.word50=M, word0=ffffffff, then state+c=3. Request hwInitStream(Q)
80291374 at8027b750. AFTER return store low8 result at byte0e; low8==ff marks
failure. Other low bytes admitted, including high-word results with low byte0.

If buffer admitted AND retained K&10000 clear, request sndStreamActivate(candidate)
8027cab4. Return0 marks failure; any nonzero preserves public candidate result.
K&10000 set skips activation, returning candidate with this local state3
publication (actual child mutations/providers remain separate).

Failure chooses returnffffffff and clears selected state+c=0. No counter/ID/flags/
source/coefficients/preferences/selector restoration; this caller issues no free/
exit request. Child cleanup semantics remain separate. State clear wins even if
activation changed that byte. Every exit requests hwEnableIrq802927d8, then
returns retained candidate/failure. No saved IRQ restore token in this caller.

Prefix allocates80 decimal stack bytes and uses _savegpr_15; suffix restores
GPR15..31 via _restgpr_15 and LR/stack. Each helper entry falls through17 ordered
stores/loads at oldSP-68 through-4 then blr: complete72-byte spans pinned, not
only the CSV's4-byte entry symbol. Valid stack/ABI/IRQ domains remain prerequisites.

## Evidence and remaining work

[Manifest](../../../tools/musyx_stream_allocation_and_length_evidence.json)
pins11 spans,349 anchors (313 body/36 helper),eight calls. Thirteen length/six ID/
four slot/five format/six completion vectors and16 coefficient-copy rows checked.
Generic verifier checks hashes/anchors/calls, not source-domain deductions or
original execution. Fresh ELFs/listings/generator remain `/tmp`; no bodies/assets
committed. Entire local allocation/length bodies answered.

Remaining: activation/deactivation/free/frequency/mix lifecycle, actual callers/
flags/count/ID/state/length/source/callback/voice/default/resource providers and
valid extent/value/encoding/alias/lifetime/IRQ/concurrency/ABI domains, transitive
ARAM/DSP consumers and original allocation/failure/stream/audio traces. EVT-01 OPEN.

## 2026-10-13 — Primary activation follow-up

[Primary activation](MUSYX_STREAM_ACTIVATION_PRIMARY_ADMISSION.md) closes initial lookup/voice/state/terminal
paths, including allocator's ordinary companion-sentinel branch. Companion
continuation/providers/domains/original activation/audio/native binding open.
