# MusyX DSP startup callback, ITD and format prefix

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salBuildCommandList REGION
80289288..80289688 exclusive; whole80288e00/25f8hex fingerprinted.
Remaining loop/endpoints/common startup finish stay open. Addresses/offsets/
masks hexadecimal; shifts/counts decimal. Read
[ADSR/startup admission](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md),
[hardware sample descriptor publication](MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md),
[virtual notification chain](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md),
[message/deactivation](MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md) and
[existing endpoint arithmetic](MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md).
Static layout/arithmetic/request order; physical format/playback units unmeasured.

## Admission and virtual-buffer callback handshake

Entry follows V.bytef0 EXACT1 and successful adsrSetup in the preceding contract.
Retain hardware record V=r29, parameter block PB=r19. Store V.wordec=ffffffff;
read unsigned V.byte90. Only captured types5/6:

1. V.word98=0; request salSynthSendMessage(V,2).
2. Store RAW callback-return word to V.wordec, even if0/ffffffff.
3. RELOAD V.word98. Nonzero continues startup. Zero requests
   salSynthSendMessage(V,1), then salDeactivateVoice(V), then exits this voice
   at shared reverse tail8028a964. Message1 return is ignored.

Admission depends on the freshly populated length word, not callback status.
Address/count/type/descriptor mutation by the callback can affect later startup.
Other captured types skip this handshake. No address/count upper bound, generation/
registry/ownership/IRQ check, fallback buffer or callback success normalization.

## Common clears and ITD setup before fresh format dispatch

Store halfword0 to PB.aa/ac/ae/b0/b2 in that order. Read V.wordf4 bit80000000.
Clear bit: PB.half36=0, preserving ITD offsets/history/scratch. Set bit:
request memset(fresh V.word08,0,40), reload word08, request DCFlushRange(ptr,40).
Then unsigned V.halfd0 to PB40, fresh d0 to PB3c; V.halfd2 to PB42, fresh d2
 to PB3e; PB36=1. No buffer allocation/null/size check or child-result test.

Now RELOAD V.byte90 for dispatch. This can differ from the handshake type.
The full local dispatch is:

| Raw type | Format path |
| --- | --- |
| 0,4,5 | Descriptor coefficient copy, zero logical offset |
| 1 | Descriptor coefficient copy, rounded seek offset |
| 3 | Direct byte-address-style arithmetic |
| 2,6 | Half-address-style arithmetic |
| 7..255 | Skip format initialization, fall through to shared cursor publication |

Names describe local address operations, not a verified codec taxonomy. Every
route joins80289678. There is no supported-type rejection or safe default cursor.

## Descriptor path for0/4/5

In order: PB70=0, PB9e=0; capture descriptor D=V.word7c; PB.a4=0,PB.a2=0;
PB.a0=zero-extended D.byte02. Copy SIXTEEN SIGNED descriptor halves from
D+08,0a,...,26 to PB+7e,80,...,9c, one read/store per field in increasing order.
Stored low16 preserves raw half bits; no sign-based coefficient clamp/scale.

Capture A=V.word78; V.worde0=0; retain base r24=wrap32(A<<1), cursor
r25=wrap32(base+2); V.worddc=0. RELOAD V.byte90: if fresh4 OR5, PB10=1 and
join shared cursor publication. Otherwise copy signed D.half04 to PBb8,
signed D.half06 to PBb6, unsigned D.byte03 to PBb4, then PB10=0.
The descriptor D remains captured; fresh type test does not reselect the earlier
path. Offset V.word80 is not consumed by this path, even for a nonzero value.

## Descriptor type1 rounded seek path

Store PB70=0 then PB9e=0. Capture raw O=V.word80, then descriptor D=V.word7c.
X=wrap32(O+13). Compute unsigned quotient Q=X/14 with original24924925 high-
multiply/subtract/shift formula; [all-word formula proof](MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md)
records that arithmetic. Do not widen O+13 or use mathematical ceil(O/14) when
addition wraps. Retain relative cursor=wrap32(16*Q+2); seek record address
R=wrap32(D+wrap32(6*Q)).

Read signed R.half28 to PB.a4, signed R.half2a to PB.a2, unsigned R.byte2c to
PB.a0. Then copy signed D.half04 to PBb8, signed D.half06 to PBb6, unsigned
D.byte03 to PBb4. Copy the same sixteen descriptor halves08..26 to PB7e..9c
in increasing order. No record-count/offset/type/descriptor bounds or data reads
at a repaired index. PB10 is NOT assigned by this type1 block.

Capture A=V.word78; store V.worddc=wrap32(14*Q); base=wrap32(A<<1);
cursor=wrap32(base+relativeCursor); V.worde0=0. Thus progress can exceed the
requested offset by rounding to the next14 boundary under nonoverflow inputs,
while wrapped high offsets can yield a small Q/progress. Exact physical seek/
frame/sample meaning and admitted offset domains remain open.

## Type3 and types2/6 direct paths

Type3: PB70=19hex (25 decimal), PB9e=100; store0 to sixteen PB halves7e..9c.
Capture O=V.word80, then A=V.word78; V.worddc=O; base=A;
cursor=wrap32(A+O); V.worde0=0. PB10 and descriptor state fieldsa0/a2/a4/b4/b6/b8
are not cleared by this path. No descriptor pointer is read for the coefficient
zeros or direct address arithmetic.

Types2/6: PB70=0a (10 decimal), PB9e=800; zero PB halves7e..9c. Capture
A=V.word78 then O=V.word80; base=A logical>>1; V.worddc=O;
cursor=wrap32(base+O); V.worde0=0. RELOAD type; ONLY fresh6 writes PB10=1.
Other fresh values leave PB10 untouched. Odd A floors under logical shift;
no round-up, signed divide, common address scaling or general PB reset.

## Default fallthrough and common cursor publication

Types7..255 skip all format-specific assignments to r24/r25. In this builder,
r25 has no earlier local assignment before the supported startup blocks, so the
first such unsupported startup can use incoming preserved-register state;
later iterations can retain a prior voice's cursor. No source-valid type invariant
or chosen default address can be inferred from this fallthrough.

At shared80289678, use current r25: PB.half7a=high16(cursor), PB.half7c=low16(cursor),
then V.word20=cursor. These writes occur even on default dispatch. This region
stops before word8c-based loop/endpoint selection. It does not clear all pending
flags, activate the voice, reset sample lifetime or prove audible startup.
No alias/pointer/type/domain guarantee protects refreshed fields/descriptor reads
or sequential halfword publication. Actual valid producers remain separate.

## Evidence and remaining source/runtime

[Manifest](../../../tools/musyx_dsp_startup_format_prefix_evidence.json) pins one
complete400hex-byte region and five linked fingerprints (whole builder/message/
deactivation/memset/cache): six spans,256 instruction anchors/five direct calls.
Nine dispatch/seven rounded-seek/six initial-cursor/five callback-length vectors
and sixteen descriptor-copy binding rows are source deductions, not original
execution or reachable-type/offset proofs. Full original fresh builder ELF/listing
stays `/tmp`; no bodies/binary assets committed.

Remaining: word8c/type-specific loop/endpoints and common startup finish,
actual type/callback/address/count/descriptor/offset/history/ITD/flag/record/
register-state providers and valid extent/residency/encoding/alias/lifetime/IRQ/
concurrency domains; cache/DSP consumers and original startup/seek/sample/audio
traces. One local startup prefix answered; whole builder/full native binding
and EVT-01 remain OPEN.

## 2026-10-13 — Endpoint/common finish follow-up

[Startup finish/composition](MUSYX_DSP_STARTUP_ENDPOINTS_AND_COMMON_FINISH.md) closes loop/fallback endpoints and
common startup finish, and audits complete local builder region coverage. Local
body answered; provider/domain/ABI/transitive DSP/original audio/native remain open.

## 2026-10-13 — Cache-service source-name correction

[Cache range/source correction](GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md)
corrects earlier service-name attribution against US symbols/direct targets.
The body above now names the emitted original service; pointer/length/order
evidence stays unchanged. Hardware/handler/runtime cache effects remain open.
