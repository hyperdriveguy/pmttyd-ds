# MusyX resource ADSR and pitch ADSR commands

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete mcmdSetADSR802801a0/258,
mcmdSetPitchADSR80280524/27c and adsrSetup80286660/28hex; two opcode regions
below. Addresses/offsets/masks hexadecimal; scalar values decimal. Static resource/
decode/FP/envelope requests, not original ADSR/pitch/audio timing. Read
[curve lookup](MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md),
[hardware layouts/time-cent wrapper](MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md)
and [fetch](MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md).

## Lookup and little-endian resource domains

Both handlers retain V/C, capture C.word0, request dataGetCurve((word>>8).low16).
Lookup0 returns BEFORE parameter/descriptor/envelope/flag work. Nonzero pointer R
is not checked for length, alignment, type/tag, residency or valid numeric fields.
No active/IRQ/member/ID gate. The underlying lookup does not reinterpret its data
as ADSR; this caller determines how the bytes are read.

LE16(R,k)=byte[k]|byte[k+1]<<8; LE32 adds byte[k+2]<<16|byte[k+3]<<24.
Source uses separate byte reads or unsigned BE halfword plus low16 swap. Fresh
resource rereads are not a single memcpy/snapshot. Extended resource roles:
attack LE32@0, decay LE32@4, sustain LE16@8, release LE16@a,
attack coefficient LE32@c, decay coefficient LE32@10.
Sentinel coefficient EXACT80000000 skips its adjustment; other raw words are
converted as SIGNED32 below. No integer time-unit/plot interpretation inferred.

## Single-precision coefficient adjustment shared by both commands

For attack coefficient X!=80000000, read fresh V.word15c as UNSIGNED32 U;
construct double using43300000 high/U low, subtract bias4503599627370496
with SINGLE fsubs. Multiply with fmuls by original binary32 factor2^-23 at
80429194. Construct X XOR80000000 with43300000 high; subtract signed bias
4503601774854144 with SINGLE fsubs. Multiply the two single values with fmuls,
then fctiwz signed integer conversion/extract word. Add that word to decoded
attack wrap32.

Decay coefficient Y!=80000000 instead reads fresh V.byte131, converts unsigned
value with the same unsigned bias/single subtraction, fmuls factor0.0078125
(1/128) at80429198, then signed coefficient conversion/fmuls/fctiwz/add to decay.
Operation order/rounding and sentinel branches remain exact; no combined double
formula, abs coefficient, unsigned conversion helper or result clamp. Valid FP/
fctiwz overflow/exception/alias domains remain open. Missing/sentinel coefficient
is not an independent negative-infinity time-cent rule.

## Ordinary ADSR halfword layout and extended layout

mcmdSetADSR rereads C.word0.high8 AFTER lookup. Zero: read/swapped LE16 at0,2,4,6
into four consecutive stack halfwords in that order. Read fresh identityf4.low8,
request hwSetADSR(slot,&stackDescriptor,0); ignore return. No extended fields/
coefficient/table read in this route. After child read lowFlags118/store OR100hex.

Nonzero high8 selects extended resource (all nonzero values, not a specific1):

1. Read LE16@8; index=sustain>>5. Read DLS float803bc7e4[index], without index<=128
   guard (raw65535 selects2047, beyond original129 entries).
2. Read LE32@0/store stack word0. Multiply retained DLS float by original4096
   with fmuls, fctiwz/extract integer; read LE32@4/store stack word4; store that
   converted LOW16 at stack half8. Read LE16@a/store stack halfa.
3. Decode attack/decay coefficients, test sentinels and adjust the respective
   stack words using shared FP sequence above, attack first then decay.
4. Read fresh identity.low8, request hwSetADSR(slot,&stackDescriptor,1), ignored.
5. Fresh lowFlags118 OR100hex.

DLS sustain conversion is SIGNED fctiwz, not the controller command's unsigned
helper. The stack descriptor writes only words0/4 and halves8/a; no whole-record
clear or coefficient-field copy. Hardware mode1 owns subsequent time-cent,
low16, sustain-scale/cap and release rules. No highFlags write/voice-local ADSR
setup/job request here. Table/data aliases can affect later resource reads.

## Pitch command adjustment before resource field decode

mcmdSetPitchADSR has no ordinary/extended selector. After successful lookup,
read C.word4.low8 as signed A; store low16(A*256) to V.half210. Read that half
SIGNED as F, use its sign to choose arithmetic. Reread C.word4, B=S8(bits8..15).
Compute q=truncToward0(B*256/100) through signed-high-multiply51eb851f/shift/
sign-correction. Store low16(F+q) if F>=0, else low16(F-q). No absolute B,
range clamp or unsigned fraction. A127/B127 can wrap past signed half boundary.
Initial half210 publication precedes resource decode and is not rolled back.

Then decode LE32 attack/decay, LE16 sustain/release and both coefficients into
retained registers. Preserve separate source reads; apply shared attack/decay
FP adjustments in order. No DLS sustain float lookup in pitch route.

## Pitch envelope setup and post-child flag order

After coefficient handling:

1. V.byte1e8=1; V.byte20e=0.
2. Request adsrConvertTimeCents(adjusted attack); store raw returned word at1fc.
3. Request conversion(adjusted decay); store raw returned word at200.
4. Index=min(unsigned LE16 sustain>>2,1023); read scaleTable803bc3e4[index];
   store low16(193-byte) at half204.
5. Store ZERO-EXTENDED decoded LE16 release to word208.
6. Request adsrSetup(V+1e8), ignoring return.
7. Fresh highFlags114 OR200hex. Low flags untouched by this local suffix.

No time-cent conversion of release in this command, low16 truncation of attack/
decay outputs, hardware hwSetADSR call or whole voice-local envelope clear.
Conversions/initializer children may mutate later live fields. Source preserves
retained sustain/release across conversions; high flag publication is post-child.

adsrSetup(Q) stores Q.byte1=0, then requests salChangeADSRState(Q)802863f4;
returns without result handling or another local state assignment. This closes
the wrapper only; the complete state engine remains OPEN. Calling setup and
setting high200 does not establish that an envelope has advanced or is audible.

## Opcode bindings, evidence and remaining requirements

| Opcode | Complete region (end exclusive) | Child |
| --- | --- | --- |
| 12 | 80282e60..e70 | mcmdSetADSR |
| 32 | 80283168..178 | mcmdSetPitchADSR |

Shortened ends omit unchanged leading address digits. Stubs pass V,&shared cstep8041ef08, ignore
return and join with stop marker unchanged (initial0). Fetch already advanced PC;
no implicit stop/wait or rendering-frame timing. Whole interpreter remains partial.

[Manifest](../../../tools/musyx_resource_and_pitch_adsr_evidence.json) pins three
bodies/two regions/six linked children/seven original table/literal spans and dispatch table:
19 spans,164 anchors/12 direct calls/two registrations. Seven signed
adjustment/five LE decode/four sustain-index/four sentinel vectors and masks/
width/FP/store-order deductions are static checks, not original execution.
Full fresh ELFs/listings stay `/tmp`; no original bodies/binary assets committed.

Remaining: salChangeADSRState/pow/transitive math, actual resource/curve/command/
volume/base-note/coefficient/descriptor/envelope/flag/caller writers, valid length/
type/index/FP/value/alias/shared-cell/IRQ/concurrency domains and original ADSR/
pitch/controller/audio traces; other opcodes/full native binding. EVT-01 OPEN.
