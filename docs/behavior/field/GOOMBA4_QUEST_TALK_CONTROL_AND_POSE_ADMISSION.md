# Goomba4 quest talk control and pose admission

US G8ME01 revision0; read-only static DOL and original msg/US evidence,
recovered2026-10-13. Supplements [quest graph](ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md),
[talk dispatcher](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) and
[common print](../message/MESSAGE_PRINT_EVENTS.md). No original execution,
native quest playback or hardware parity is claimed.

## Talk, selector and message control are separate owners

Fresh fbatTalkMode800470dc/1a0 disassembly confirms stage0 calls StopID on
actor regular event, applies optional actor facing, then marioKeyOff8005c1f4
and partyKeyOff800b7688 **before** constructing talk event. Ordinary fieldstate2
constructs actor+128 script with evtEntry(script,0,0), sets event.byte10=6,
uses marioChgTalkMotion80094d90, publishes event+170=actor and retains its ID.
The common evt_msg_print helper performs no Mario key-off/on calls.

Stage1 waits for evtCheckID(retainedID)==0, not only completion of the current
message window. It conditionally resumes the regular event/restores actor yaw,
restores actor pose, then calls marioKeyOn8005c1c0, partyKeyOn800b768c and
marioChgStayMotion80094e48. It finally returns fieldstate1 and clears candidate.
Both partyKeyOff and partyKeyOn are exactly one blr instruction in this revision;
assigning a partner lock mutation to them would invent leaf behavior.

marioKeyOff sets mainflags bit8, increments byte39 modulo256 and returns its
signed byte. marioKeyOn decrements byte39 modulo256, clears mainflag8 only if
resulting signed byte==0, and returns the signed result. Thus ordinary talk
entry depth1, item-selector entry depth2, selector deletion depth1, and talk
**event death** depth0. Replacing selector keyOn with a boolean unlock releases
Mario too early. Underflow/overflow are original low8 arithmetic; no source
saturation or zero-depth guard occurs in these leaves. The native control-depth
adapter and visual dialogue state remain distinct services.

The selector wrapper's firstCall independently calls keyOff after entry;
ready completion deletes selector then calls keyOn before writing result.
Every quest13 selector iteration owns that same balanced nested interval.
No symbol npcWaitKeyOn/npcWaitKeyOff is called by these source paths. A native
wait callback must not be attributed to such a source service without evidence.

## Numeric speaker0 retains the talk-event actor

Quest2 prints with explicit speaker me. Quest13 prints with numeric speaker0.
The common print field route first initializes N=event+170 when event.byte10=6;
a nonnull speaker-name operand overrides N, while numeric0 does not.
Consequently both calls resolve the owning Goomba4 through this ordinary NPC
talk-event context. Numeric0 here is not a narrator/no-speaker override.
Original operands must nevertheless remain distinct. If a different caller
executes the script outside type6 actor context, speaker resolution can differ.

## Original message availability

Fresh ISO reads establish all seven keys in msg/US/gor_02.txt; native
romfs/messages/gor_02.txt currently matches that entire resource byte-for-byte.
No dialogue prose is copied into this contract.

| Key | Payload bytes | Control-token obligations |
| --- | --- | --- |
| irai_02_03 | 5 | k |
| irai_02_04 | 505 | wait250,k,p,dkey,wait500,/dkey |
| irai_13_001 | 87 | wait250,k |
| irai_13_002 | 128 | wait200,wait250,k |
| irai_13_003 | 20 | k |
| irai_13_004 | 140 | wait250,k,p |
| irai_13_005 | 32 | wait250,k |

Token column lists vocabulary, not token order/count. Preserve original bytes,
including the tiny first quest2 payload, page order and nested disabled-key
region; do not substitute native explanatory prose.

## Mario poses and fresh asset binding

Read-only local extracted original resources and current portable animation
parser establish:

| Source pose | Original model | Loop | Time range | Current field characters.pmanim |
| --- | --- | --- | --- | --- |
| M_N_5B | a/e_mario | false | 0..32 | absent |
| M_N_C | a/e_mario | false | 0..70 | absent |
| M_S_1 | a/a_mario | true | 0..288 | present |

M_N_5B and M_N_C both satisfy character_animation_data.animation's existing
single-loop-descriptor/stream bounds domain. The current build_assets.py already
imports e_mario using original a_mario texture for M_I_5,M_N_5A,M_F_1;
adding these two exact poses to that existing model group extends the existing
mesh/animation pack rather than requiring a fabricated pose or substitute.
The quest event still requests each pose explicitly, then its original500ms
waits; model animation duration is not the wait condition. Pose availability
does not establish original Mario motion/idle suppression and rendering parity.

## Saved GSF5407 ownership

Current FieldActorTalkState owns optional quest1420 and flags5405/5406 only.
Current encode/decodeSave uses versions6/7, with version7 known/value fields
in packed word20; bits21..31 are reserved and rejected. Thus neither current
pouch ownership nor a known quest13 proves GSF5407. Its original location is
GP+418 bit31, cleared by explicit swInit and retained by swReInit/card import.
A new persistence owner must include optional known/value5407 and versioned
migration that leaves older saves unknown, plus the first-loop unconditional
write5407=1 after that loop exits (including cancellation). Fresh reproduced
swInit may explicitly establish false. No story-threshold inference is sound.
Changing the reserved mask alone without retaining unknown-vs-false semantics
would manufacture legacy state. Full pouch/card serialization remains separate.

Evidence files /tmp/quest-fbatTalkMode.elf/.asm,
/tmp/quest-marioKeyOff.elf,/tmp/quest-marioKeyOn.elf,
/tmp/quest-partyKeyOff.elf,/tmp/quest-partyKeyOn.elf remain outside history.
Only this contract was added; no implementation,index,PROGRESS,asset pack,
original dialogue or emulator state was edited by this worker.
