# Unit sound-mode loading and the Mario jump playback request

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete BtlUnit_LoadSeMode0x80126588/0x14c and the concrete
[Mario jump caller](BATTLE_MARIO_JUMP_MOTION.md). psndSFXOn_3D0x800d9848/0x3c
is inspected/fingerprinted;its called playback core is not recovered here.
No original audio output,cadence,macro decoding or sample licensing grant is
asserted. Original resources must be resolved/reused by their actual owners.

## Four arguments and unconditional record writes

The loader accepts `(type,mode,source,destination)`,with type/mode signed32
words and pointers in r5/r6. It has no local pointer validation or allocation,
no event first-call/polling protocol and no RNG. It does not access inventory,
shared battle work or unit-kind data itself.

Before dispatching mode,write destination halfword+0x14=0,then word+0x18=-1.
These writes occur even for mode-2,which preserves most previous fields.
Retain this ordering and each later source load/store;source/destination aliasing
is not handled by a temporary whole-record copy. Field names beyond their actual
use require producer/consumer closure;do not infer duration/pitch/handle units.

The subsequent [movement sound core contract](BATTLE_MOVEMENT_SOUND_CONTROL.md)
now identifies initial/remaining/alternating counters and retained voice word18
through concrete readers/writers. Sound source/key/macro/audio meanings remain
unclosed;loader dispatch and partial-field behavior below are unchanged.

## Mode-1: load by type

Mode-1 selects the following source fields:

| Type | Destination +0/+4 | Destination +0xc/+0xe/+0x10 |
| --- | --- | --- |
| 0 | Source words+8/+0xc | Signed source halfwords+0x10/+0x12/+0x14 |
| 1 | Source words+0x18/+0x1c | Signed source halfwords+0x20/+0x22/+0x24 |
| 2 | Source words+0/+4 | All three zero |
| 3 | Source words+0x28/+0x2c | Signed source halfwords+0x30/+0x32/+0x34 |
| Every other signed type | Zero words | Zero halfwords |

Within each source-loading route,copy first word,second word,then the three
halfwords in order. Type2 does not read halfword parameters from source.
Finally read signed halfword destination+0xc and store its low16 bits at+0x12.
Source kind/sound-table initialization and meaning of these records remain
separate dependencies. The type number alone does not identify an audio macro.

## Mode-2 and other modes

For mode-2,after the unconditional+0x14/+0x18 resets,read signed destination+
0xc and copy it to+0x12. Do not clear existing destination words+0/+4 or
halfwords+0xc/+0xe/+0x10,and do not read the source/type for this route.

Every mode other than-1/-2 clears destination words+0/+4 and halfwords+0xc/
0xe/+0x10,then copies now-zero+0xc to+0x12. This includes0,positive values
and negative values<=-3. No provided raw mode number is stored as a playback
request or interpreted as a voice ID here.

All routes leave destination bytes+8..+0xb untouched. They also leave bytes
beyond+0x1b untouched. The loader is a partial-field initializer,not memset(28)
or a full source-record copy. Alias and retained-gap cases matter for reuse.

## Exact MarioJumpPosition binding

During normal first-call setup,MarioJumpPosition calls:

`LoadSeMode(type2,resolvedSoundMode,source=U+0x1c4,destination=U+0x18c)`

The two ordinary Jump sites supply raw-1,which is resolved by evtGetValue.
This route copies source words at U1c4/U1c8 to U18c/U190,zeros destination
halfwords U198/U19a/U19c,sets U19e/U1a0 to0,and sets U1a4=-1.
Destination gap U194..U197 is untouched. These are unit record fields,not
body-part fields with the same offset numbers.

After loading,MarioJumpPosition reads **word U18c**. If nonzero,pass that value
as the first argument to psndSFXOn_3D with a temporary copy of baseline XYZ.
It does not request playback of word U190 on this path,and does not store the
playback routine's return value. This establishes the used playback **input**,
not that U18c/U190 are already allocated voice handles.

The inspected psndSFXOn_3D wrapper passes its two inputs to __psndSFXOn0x800d9934:
first input unchanged,255,255,0,positionPointer,0,0x10,0. The downstream core's
identifier/string/macro/resource and voice lifetime semantics remain open;
do not rename these raw wrapper arguments as measured mix parameters. The
jump body does not later update this voice position or stop it on landing.

Mode1 initialization skips this entire loader/playback region. A normal setup
that derives n0 returns2 before loading sound. Mode-2 retains the first word
and can therefore request it after the unconditional record reset. Other modes
clear first word and omit playback. Conditional request order is documented in
the motion contract;asset playback must not be connected unconditionally.

## Implementation requirements and remaining bindings

| ID | Requirement | Derived acceptance cases |
| --- | --- | --- |
| SE-MODE-01 | Preserve partial fields,reset order and mode dispatch | Gap+8..b survives every mode;+14/+18 reset even for-2;unknown mode clears words/parameter halfwords |
| SE-MODE-02 | Preserve source type maps and raw ordered copies | Type2 copies only two words;type0/1/3 use their own signed-halfword source ranges;unknown type reads no source |
| SE-MODE-03 | Bind conditional concrete jump input and position copy | Ordinary-1 uses type2;only nonzero U18c plays once at setup;mode1/n0 omit request;return is not stored |

Remaining JUMP-01/DMG-02 work:all source/destination writers and sound-key/ID
schema,__psndSFXOn lookup/macro/voice/effect semantics,original US banks/
samples and event-loop lifetime,actual jump/landing audiovisual traces and
other sound loader callers. This contract does not supply invented replacement
samples or identify a source key merely by inspecting a symbol annotation.

tools/verify_mario_jump_motion.py pins this whole body,selected record-write
instructions and the actual Mario caller body/operands. Original ISO/resource
reads are read-only;inspection artifacts remain under /tmp or ignored paths.
