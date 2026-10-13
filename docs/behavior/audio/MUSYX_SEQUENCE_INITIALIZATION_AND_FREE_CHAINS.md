# MusyX sequence initialization and free chains

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete seqInit80276770/1a8hex.
Read [lookup/stop](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md) and
[initialization caller](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).
Offsets/addresses hexadecimal; explicit counts/strides labeled decimal.
Static reset/source layout only; actual sequence/voice/resource runtime open.

## Initial roots and eight-instance publication

No input, sndActive/old-state check, IRQ or child request. First store
seqActiveRoot8041ee5c=0, then seqPausedRoot8041ee58=0.
seqInstance803ebdc0 has eight instances of1888hex bytes, totalc440hex.
For ordinal i0..7, iterate in ascending order:

- i0: store seqFreeRoot8041ee54=instance0, then instance0.previous(word4)=0.
- i>0: store preceding instance.next(word0)=current; current.previous=preceding.
- Write that ordinal's16 consecutive MIDI-priority halfwords toffff, in order,
  with the private-ID/state writes interleaved as described below.
- Store instance.byte9=i, then instance.byte8=0.

MIDI priority array seqMIDIPriority803f8200 has100hex bytes:8 rows*16 halfwords.
Within each row, stores0..1a precede private ID; half1c follows that private-ID
store; state0 follows half1c; half1e follows state0. Pointer advances20hex per row.
After all8 instances, set final instance7.next=0. No external key(wordc), note
head(e64/e68/e6c), scheduled field(efc), project/track storage or whole-instance
clear. Lookup can now rely on initialized private bytes only if later producers
preserve valid IDs; active/paused roots are empty immediately after this body.

## Note free chain and cross-record stores

seqNote803ea9c0 has1400hex bytes:256 records,20 DECIMAL bytes each.
Store noteFree8041ee4c=record0. Build32 blocks of8 consecutive records:

1. At each block's first record, store its.previous(word4)=retained preceding
   record pointer (0 for the first block).
2. For every record, if retained previous nonnull, store previous.next=current;
   then set retained previous=current.
3. For the first seven records in each block, the source ALSO stores the current
   pointer at record+18hex BEFORE advancing. Because stride is14hex, this address
   is the NEXT record's word4: it initializes that record's previous link.
   The eighth record skips this cross-record store; next block step1 supplies it.
   After the optional store, advance current by14hex.

Finally store last record255.next=0. This produces previous/next chain0..255,
head.previous0/tail.next0, wholly within the supplied array. The +18 writes are
not a separate per-note field or out-of-array payload initialization. They alias
next.previous by design. No note.word8 voice, other payload field or whole-array
zero/free. Fresh initialization/reinitialization can preserve old payloads.

Only after all chains, store seq_next_id8041ee40=0, then return without an
independent success assignment. Root/state resets do not request key-off/voice
kill, unlink old external owners or release project/sample resources. Safe reset
lifetime/concurrency and subsequent constructor ownership remain prerequisites.

## Validation and remaining source/runtime

[Manifest](../../../tools/musyx_sequence_initialization_evidence.json) pins the
complete body and88 source publication/loop/store anchors (one span/no calls).
Derived exact extents,
eight-instance chain and note boundaries are metadata. Independent simulation of
the emitted32*8 note-store pattern checks512 next/previous assertions and keeps
all writes within1400hex; this does not execute original code. Full fresh ELF/
listing stays `/tmp/inbox-seqinit.elf`; no original body/asset exported to history.

Remaining: instance/ID/active/paused/note allocation and constructor producers,
MIDI-priority consumers and payload/voice/resource ownership, valid reset/alias/
concurrency/list domains and original sequence initialization/reuse/audio traces.
One complete initializer is answered; full native/source/runtime remains OPEN.
