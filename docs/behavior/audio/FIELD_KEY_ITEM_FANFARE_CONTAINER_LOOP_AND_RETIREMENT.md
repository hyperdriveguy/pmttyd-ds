# Key-item fanfare container loop and retirement

US G8ME01 disc0 revision0; read-only recovery2026-10-14.
Closes the loop-policy question in [item95 audio routing](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).
Composes existing complete SoundSS contracts rather than replacing their
callback, buffering, IRQ and device owners with a guessed one-shot timer.

## Container restart owns looping; DSP loop flag does not

Original SoundSSPlayCh800ded7c calls SoundSSPlayChEx(...,cacheArgument0).
Its admitted fresh row reset stores streamflags1, clearing old200 as part
of that full halfword assignment. For matched successful header callback,
SoundSSPlayChEx_main800de1b8..de22c copies common header word8 to rowtotal1c,
common headerwordc to rowrestart20. Exactlyffffffff restart stores0 at20
without setting loopmask200. Every other restart ORs200. See
[header mapping](GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md).

Original key-item resource sound/stream/ff_get2_32k.stm common fields:

| Header offset | Value | Original consumer |
| --- | --- | --- |
| 0 | 02007d00 | formatbyte2,frequency32000 |
| 4 | 2 | dual-channel/type2 |
| 8 | bb40 (47936) | total encoded bytes per channel |
| c | ffffffff | **no container loop**,rowrestart normalized0 |
| 10/14 | bb40/bb40 | primary/secondary end |
| 18/1c | 0/0 | primary/secondary restart |

DSP channel subheaders nevertheless set their loopflag1 and describe
startnibble2/end17682hex. The game's loop decision above does not read those
DSP flags to set200. Thus source fanfare is nonlooping on a fresh stream row.
This corrects a converter relying solely on DSP loopflag, which would loop
this fanfare indefinitely and prevent genuine group1 vacancy.

Source format2 maps coefficients from header5c..7b and secondarybc..db;
container positions/restarts are independent of those codec tables. Preserve
original coefficient/initial-history decoding; do not zero them merely to
change loop policy. Exact hardware/MusyX restart histories, refill boundary
and scheduler timing remain separate providers.

## Generic shipped US STM export domain

A fresh readonly survey of **all146** original sound/stream/*.stm files:

- All146 DSP subheader loopflags are1.
- Common restartwordc:35 sentinelffffffff,111 non-sentinel.
- All111 non-sentinel restart offsets are divisible8.
- For those111, primary DSP startnibble==2*commonRestart+2 exactly.
- All146 commonTotal==ceil(DSPsampleCount/14)*8.
- A follow-up provenance/hash-verified local audit of both channels confirms
  DSP endnibble==2*commonTotal+2 for all146 source files. Native export refuses
  mismatched codec/container start/end relationships, rather than extending them.

The finite original format2 payload has8-byte Nintendo DSP frames producing
14 samples. For these validated source files, decoded PCM loop selection is:
sentinel→no loop; otherwise startSample=(commonRestart/8)*14,
endSample=DSPsampleCount. This agrees with DSPstartnibble→sample conversion
for the111 genuinely looped files, while preventing the35 sentinel tracks
from acquiring a synthetic loop. Preserve original complete final-frame sample
count, not ceil(count/14)*14 decoded samples; source codec metadata identifies
which samples in the last frame are valid.

A generic converter must validate this frame-aligned format2 domain instead
of pretending floor division handles arbitrary malformed/nonaligned offsets.
Other formatbytes/counts/channels, incomplete sourceheaders and uncommon
restart/end/history relationships need separate contracts. This survey proves
these146 source-resource relationships, not every conceivable STM file.

## Producer end→stream stop→group vacancy

In admitted format2 encoded-copy/decoder path, reaching common rowtotal1c
loads rowrestart20 into current position, then rereads streamflags. With200
clear, OR4 and stop the current producer span. See
[encoded producer](GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md)
and [whole producer](GAME_STREAM_PRODUCER_PCM_AND_WHOLE_COMPOSITION.md).
The second requested span can still execute; setting4 is an original pending
stop publication, not a promise all queued/device audio instantly disappeared.

SoundSSMain800dd2e0 scans three slots. Initialflags8 set/2 clear admits row;
initial4 has stop priority before fades. It clears captured4,recaptures row,
then if currentflags nonzero performs original inline stop. Store flags0
**before** requesting closeprimary,closesecondary,freeprimary and (type2)
freesecondary stream. Original IRQ counter/restore and marker writes retain
[source stop ordering](GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md).
Deferred DVD close does not delay flag0 until physical file deletion.

SoundSSGetFlagCh800dd968/18 returns the selected row's unsigned flags half.
psndBGMMain800db7cc observes actual current group's stream flag result;
zero marks group currentindex=-1. At that point psndBGMChk(group1) returns-1
and the itemget source requests conditional fieldmusic unmute/continue.
The actual itemget acknowledgment can precede this end and requests group1
fade-off itself; do not block A/B until all samples play.

Native implementation may preserve the original authored PCM as one-shot,
request retirement from actual stream progression/drain/backend ownership,
then publish genuine group vacancy. It must not fabricate vacancy at grant
or start a free-running2.6215-second timer as equivalent source proof. Source
stream-prefetch/device submission/stop cadence can affect the relation between
producer end, SoundSSMain stop and last audible sample. That exact relation
requires original traces and native hardware checks.

## Evidence and remaining boundaries

Fresh actual ff_get2 header bytes and all146 headers read directly from ISO;
full resource identity/hash in the prior routing contract. Fresh original
SoundSSPlayChEx_main disassembly pins copy/sentinel instructionsde1b8..de22c;
GetFlag leaf freshly read at800dd968/18. Existing complete producer and
SoundSSMain contracts pin their end/stop owners and source call boundaries.
No asset, converter, nativecode, sharedindex/progress, userprofile/save edited.

Source finite nonloop policy and retirement publication are now established.
Still open: native original stream export/NDSP ownership, complete source
buffering/decoder-history/device/callback cadence parity, fieldgroup secondary
volume continuation and physical New2DS XL audio validation. This closes the
container-loop ambiguity without claiming those providers already work.
