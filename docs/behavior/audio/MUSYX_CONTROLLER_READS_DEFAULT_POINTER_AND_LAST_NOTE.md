# MusyX controller reads, default pointers and last notes

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete inpGetMidiCtrl8028ed48/2c4hex,
inpGetChannelDefaults8028f00c/44hex, inpGetMidiLastNote8028f294/3chex.
Addresses/offsets hexadecimal; counts, controller indices and result values
decimal. Static source reads, not original controller/audio execution.
Read [initialization/reset/default tables](MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md).

## Controller ABI, channel rejection and row selection

inpGetMidiCtrl(I,C,S) receives controller index, channel and set in r3/r4/r5.
This differs from reset's (channel,set,cold) argument order. First compute
c=C.low8; exact c=255 returns0 before any controller table read. This is not
the set sentinel: s=S.low8==255 selects FX storage for admitted channels.
Raw input high bits are discarded in these selections.

For c!=255, i=I.low8 and s=S.low8 select row R:

| s | Controller row |
| --- | --- |
| 0..254 | midi_ctrl80409748+s*860hex+c*86hex |
| 255 | fx_ctrl8040dec8+c*86hex |

No sndActive, dirty-flag, valid-channel/set/count/IRQ/generation gate. Original
extents describe eight MIDI sets of16 channels or64 FX channels; the body
checks only c!=255 and does not enforce those extents. Unknown low8 inputs can
derive addresses beyond their tables. There is no repaired/clamped fallback.

## Complete range dispatch and fresh byte reads

MIDI and FX branches implement the same controller transformations:

| i | Ordered local reads and return |
| --- | --- |
| 0..63 | k=i&31; read R.bytek, then R.byte(k+32); return ((first<<7) OR second).low16 |
| 64..69 | Read R.bytei; return16383 if byte>=64, otherwise0 |
| 96..101 | Return0; no controller byte read |
| 128,129 | Read R.byte128, then R.byte129; return ((first<<7) OR second).low16 |
| 132,133 | Read R.byte132, then R.byte133; return ((first<<7) OR second).low16 |
| All other0..255 | Read R.bytei; return byte<<7 |

Controllers32..63 are aliases of the corresponding paired0..31 reads; they
are not standalone low-byte values. Both members of each adjacent128/129 or
132/133 pair return the same assembled word. Indices130/131 are single-byte
paths. The96..101 constant branch precedes generic reading even when original
reset tables contain nonzero values at those positions. No RPN helper call,
side effect or dirty-flag clear on those constant returns.

The threshold uses subfc/subfe/andc: its unsigned-byte subtraction sets carry
when byte>=64, selecting3fffhex. It is not a nonzero check, strict greater-than64
or bit6 test alone; bytes128..255 also satisfy the comparison.

Paired values use BITWISE OR, not addition, and raw bytes are not masked to
seven bits before assembly. Normal seven-bit bytes can produce0..16383; raw
255 bytes can produce32767. For example first1/second128 produces128, not256.
The generic shift likewise does not clamp byte255 to127; it yields32640.
Only the paired path applies a final low16 mask. No host signed-byte conversion,
saturation, read atomicity or generation guarantee is implied.

These bodies return raw source values; they perform no write, callback, hardware
request, event timing update or physical audio conversion. Each source byte is
read when requested; no wider snapshot/cache or tuple coherence is established.

## Direct consequences of original cold/warm tables

With a stable cold-reset row, controllers7 and11 return16256 (127 paired with0),
controller10 returns8192 (64 paired with0), both128/129 and132/133 return8192,
and64..69 return0. The96..101 constant branch returns0 even though cold defaults
at98..101 are127. These are static compositions of the original table and this
read body, not original sampled sound or standard-MIDI assumptions.

Warm reset writes11=127 and43=127, so their shared pair returns16383 under an
unmutated valid row. Warm's preserved byte7/39 and10/42 retain their prior
contents; no universal cold volume/pan read is guaranteed by warm reset.
Actual writers/caller cadence and alias/concurrency remain separate contracts.

## Complete packed channel-default pointer getter

inpGetChannelDefaults(C,S) selects low8 inputs and returns the raw record pointer:

| S.low8 | Exact pointer |
| --- | --- |
| 0..254 | inpChannelDefaults8040da48+S.low8*90hex+C.low8*9 |
| 255 | inpFXChannelDefaults80410048+C.low8*9 |

No memory read from the record, copy, initialized-flag/type/filter validation,
channel255 rejection, bounds/active/IRQ gate or normalization beyond low8 inputs.
It preserves the original packed9-byte stride. Do not reinterpret the return as
a successful initialized host struct or change it to a padded layout. Reset's
byte0/word1/word5 stores are documented in the linked contract; field consumers
must use that exact record representation.

## Complete last-note byte getter

inpGetMidiLastNote(C,S) uses MIDI pointer
midi_lastNote80410288+S.low8*10hex+C.low8 for set!=255; FX pointer
fx_lastNote80410308+C.low8 for set255. Read one byte and return zero-extended.
No missing-note/sentinel handling, channel255 rejection, bounds/generation/
dirty/active/IRQ check, mutation or callback. Raw255 is a valid returned byte,
not a new independently assigned error code. The reset service explicitly sets
that byte255 through its last-note child; inpInit alone does not initialize all
last-note entries. MIDI/FX table extents and valid caller indices remain separate.

Thus only inpGetMidiCtrl has the channel255 early return. The other getters do
not acquire that rule by sharing similarly named channel/set parameters.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_controller_reads_defaults_last_note_evidence.json)
pins three complete bodies:three spans,67 instruction anchors including both
threshold carry/mask sequences,zero direct calls. All256 controller routes,
eight pair/eight threshold/six single-byte/six destination vectors are derived
checks. Pair vectors include raw-byte overlap to distinguish OR from addition;
destination vectors retain the getters' different channel255 treatment.
Fresh full inspection ELFs/listings stay `/tmp/inbox-inpGetMidiCtrl.elf`,
`/tmp/inbox-inpGetChannelDefaults.elf`, `/tmp/inbox-inpGetMidiLastNote.elf`.
No original body/assets committed or original code executed.

Remaining: actual read/reset/write callers and controller/set/channel inputs,
_GetInputValue/matrix/dirty/filter/last-note consumers and mutation owners,
valid bounds/raw-byte/alias/IRQ/concurrency domains and original cold/warm/
controller/last-note/audio traces. Three local services are answered; EVT-01
and full native/source/runtime/hardware equivalence remain OPEN.
