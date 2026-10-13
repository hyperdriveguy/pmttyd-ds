# MusyX controller initialization, reset tables and channel defaults

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete inpInit8028fcc8/2c4hex,
inpResetMidiCtrl8028ec58/f0hex, inpResetChannelDefaults8028f050/5chex,
inpSetMidiLastNote8028f258/3chex, and two original134-byte default tables.
Addresses/offsets hexadecimal; counts/scalar values decimal unless marked.
Static source writes, not original MIDI/event cadence or audible filter evidence.
Read [synthesis initializer](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md).

## Complete inpInit global path

inpInit(Q) branches on the RAW pointer word: zero selects global initialization;
any nonzero selects the per-voice stores below. There is no active/IRQ/allocation/
valid-pointer gate. For Q=0, visit32 rows per aux side, stride24hex, resetting
only row.byte22=0. A base=803f9464, B base=803f8fe4. For each ascending ordinal,
write A then B; two repetitions of16 unrolled paired stores. No full aux row/
controller clear, coefficient/value default store or callback installation.

Next store full word000000ffhex into all128 consecutive words at
inpGlobalMIDIDirtyFlags80409548..80409744. Four repetitions of32 unrolled words;
the value is255, not ffffffff or a repeated-byte memset. No controller-value
or packed channel-default/last-note table reset occurs on this global path.

Both paths then overwrite lpfLowFrequencyDefault8041efb4=80, followed by
lpfHighFrequencyDefault8041efb0=16000. These names identify source symbols;
no physical unit/filter response measurement is inferred. A per-voice inpInit
also performs these GLOBAL writes, not just writes to the supplied Q.
The synthesis initializer calls this service with0; other caller domains remain
separate. No independent success status or hardware output request.

## Complete nonzero-pointer store order

For Q!=0, the exact58 ordered stores are below. Width4 means a raw word;
width1 means a byte. Values are decimal. The matrix/controller roles of these
raw offsets require their consumers; no full voice-record clear is added.

| Ordinal | Q offset | Width | Value |
| --- | --- | --- | --- |
| 0 | 224 | 1 | 7 |
| 1 | 225 | 1 | 0 |
| 2 | 228 | 4 | 65536 |
| 3 | 22c | 1 | 11 |
| 4 | 22d | 1 | 2 |
| 5 | 230 | 4 | 65536 |
| 6 | 246 | 1 | 2 |
| 7 | 248 | 1 | 10 |
| 8 | 249 | 1 | 0 |
| 9 | 24c | 4 | 65536 |
| 10 | 26a | 1 | 1 |
| 11 | 26c | 1 | 131 |
| 12 | 26d | 1 | 0 |
| 13 | 270 | 4 | 65536 |
| 14 | 28e | 1 | 1 |
| 15 | 290 | 1 | 128 |
| 16 | 291 | 1 | 0 |
| 17 | 294 | 4 | 65536 |
| 18 | 2b2 | 1 | 1 |
| 19 | 2d8 | 1 | 1 |
| 20 | 2d9 | 1 | 0 |
| 21 | 2dc | 4 | 65536 |
| 22 | 2fa | 1 | 1 |
| 23 | 2fc | 1 | 64 |
| 24 | 2fd | 1 | 0 |
| 25 | 300 | 4 | 65536 |
| 26 | 31e | 1 | 1 |
| 27 | 320 | 1 | 65 |
| 28 | 321 | 1 | 0 |
| 29 | 324 | 4 | 65536 |
| 30 | 342 | 1 | 1 |
| 31 | 366 | 1 | 0 |
| 32 | 368 | 1 | 91 |
| 33 | 369 | 1 | 0 |
| 34 | 36c | 4 | 65536 |
| 35 | 38a | 1 | 1 |
| 36 | 3ae | 1 | 0 |
| 37 | 3b0 | 1 | 93 |
| 38 | 3b1 | 1 | 0 |
| 39 | 3b4 | 4 | 65536 |
| 40 | 3d2 | 1 | 1 |
| 41 | 2b4 | 1 | 132 |
| 42 | 2b5 | 1 | 0 |
| 43 | 2b8 | 4 | 65536 |
| 44 | 2d6 | 1 | 1 |
| 45 | 3f6 | 1 | 0 |
| 46 | 3f8 | 1 | 79 |
| 47 | 3f9 | 1 | 0 |
| 48 | 3fc | 4 | 65536 |
| 49 | 41a | 1 | 1 |
| 50 | 41c | 1 | 31 |
| 51 | 41d | 1 | 0 |
| 52 | 420 | 4 | 65536 |
| 53 | 43e | 1 | 1 |
| 54 | 220 | 4 | 32767 |
| 55 | 1d8 | 1 | 0 |
| 56 | 1d9 | 1 | 0 |
| 57 | a8 | 1 | 0 |

Only these fields are written on the nonzero path before the common global
frequency stores. In particular, nonzero Q does not clear the global aux/dirty
tables. Neither branch resets MIDI controller-value/last-note/channel-default
arrays merely because the function name contains initialization. Existing
unwritten payload stays as it was under a valid non-aliasing pointer domain.

## Exact table selection and controller reset

inpResetMidiCtrl(C,S,Cold) retains raw C/S; select Cold by the FULL r5 word:
zero chooses inpWarmMIDIDefaults80304010, any nonzero chooses
inpColdMIDIDefaults80303f88. Each table has134 bytes. Destination uses c=C.low8,
s=S.low8:

| s | Exact controller destination |
| --- | --- |
| 0..254 | midi_ctrl80409748+s*860hex+c*86hex |
| 255 | fx_ctrl8040dec8+c*86hex |

No channel/set bound or active/IRQ gate. Original BSS extents support eight MIDI
sets of16 channels, or64 FX channels, but the body does not enforce them.
Higher low8 indices derive out-of-table addresses; no source clamp or fallback.
High input bits are discarded for destination arithmetic, while Cold is not
truncated before its nonzero test.

Cold!=0 requests memcpy(destination,coldTable,134), then continues. Cold==0
visits all134 warm bytes ascending, in67 paired iterations. For EACH byte,
read the table byte fresh:255 SKIPS that destination store; other values overwrite
the destination byte. No blanket zeroing, special interpretation of a retained
old255, full-width controller store or dirty-flag request. Alias/memcpy child
semantics remain separate requirements.

### Complete original table values

Cold table: indices7 and11=127;10=64;98,99,100,101=127;128 and132=64.
All other indices0..133=0; there is no255 preservation sentinel in this table.

Warm table: indices1,33,64,65,66,67,69=0;11 and43=127;128=64.
All other indices0..133=255 and preserve their current destination bytes.
Thus warm reset stores only ten positions in a stable non-aliasing table;
volume/pan and other preserved entries do not become cold defaults.
These are original controller bytes, not invented MIDI standard defaults or
measured sound behavior. Table fingerprints and derived index categories are
recorded in the manifest; no original binary asset is exported to history.

AFTER either reset path, request inpSetMidiLastNote(raw C,raw S,255), even when
warm reset preserved most bytes. No channel-default reset is requested here.
Child result ignored; no independent reset status.

## Complete last-note setter

inpSetMidiLastNote(C,S,N) tests S.low8==255. Non-FX destination is
midi_lastNote80410288+S.low8*10hex+C.low8; FX destination is
fx_lastNote80410308+C.low8. Store N.low8 and return. No preceding-note read,
valid-channel/set bound, generation/dirty flag/IRQ request or status. The tables'
original extents are80hex bytes for MIDI,40hex for FX; unchecked low8 inputs
can exceed them. Controller reset's255 value is this explicit setter input,
not a guarantee that all last-note entries were reset by inpInit.

## Complete packed channel-default reset

inpResetChannelDefaults(C,S) selects low8 inputs using the same FX sentinel:

| S.low8 | Exact packed record |
| --- | --- |
| 0..254 | inpChannelDefaults8040da48+S.low8*90hex+C.low8*9 |
| 255 | inpFXChannelDefaults80410048+C.low8*9 |

Write byte0=2; load fresh lpfLowFrequencyDefault and perform word store at+1;
load fresh lpfHighFrequencyDefault and perform word store at+5, in that order.
The source uses9-byte records and those exact possibly unaligned word-store
addresses. Do not replace them with padded12-byte host records or reorder/cache
both global defaults into an earlier snapshot. There is no table-wide reset,
filter application/dirty flag request, bounds/active/IRQ gate or explicit status.
Record type byte2 and frequency values require actual consumer semantics.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_input_init_and_controller_defaults_evidence.json)
pins four complete bodies,three linked fingerprints (ABI save/restore entries,
memcpy),two original default-table spans: nine spans,223 instruction anchors,
four direct calls. It records58 decoded voice stores,64 ordered aux-byte resets,
128 ordered dirty-word resets,all134 classified bytes in each table,six
controller/channel-default/last-note destination vectors and five warm-preserve
vectors. These are source deductions, not original execution. ABI entry hashes
alone do not describe their complete fallthrough chains. Fresh inspection ELFs/
listings stay `/tmp/inbox-inpInit.elf`, `/tmp/inbox-inpResetMidiCtrl.elf`,
`/tmp/inbox-inpResetChannelDefaults.elf`, `/tmp/inbox-inpSetMidiLastNote.elf`.
No original body/binary asset committed or original code executed.

Remaining: actual reset/init callers and channel/set/pointer/cold-mode inputs,
matrix/aux/dirty/MIDI/last-note/filter consumers and other writers, memcpy and
valid bounds/alignment/alias/IRQ/concurrency/reinit domains, original cold/warm/
per-voice/global reset and controller/audio traces. Four local services are
answered; EVT-01 and full native/source/runtime/hardware equivalence remain OPEN.
