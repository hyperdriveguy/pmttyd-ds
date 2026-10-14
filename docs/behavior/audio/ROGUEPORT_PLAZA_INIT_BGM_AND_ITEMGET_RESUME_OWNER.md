# Rogueport plaza init BGM and item-get resume owner

US G8ME01 disc0 revision0; read-only REL/DOL recovery2026-10-14.
Finite group0 owner for [Goomba3 acquisition](../field/GOOMBA3_SOURCE_SETUP_AND_TALK.md)
and [item95 audio resume](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).
Original map script conditions, not a guessed music choice by native scene enum.

## gor_01 init source branches

Original gor_01_init_evt, gor.rel file3c200/890, beginsLF0=0. IfGSW0==341,
requests Mario mode2 and setsLF0=1. IfGSW0==400,reads original entry name:
exact `sekai_yami1` or `minnnanokoe` setsLF1=1.
LF0/LF1 are event-local flags, not saved task flags. Fresh root event constructor
clears all three local flag words; see
[root initialization](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md).
Do not preserve a previous map's LF1 as a new saved story predicate.

The ordered BGM/environment branches at3c288..3c3ac:

| Condition | Source request order |
| --- | --- |
| Story402 | ENVOn(110,ENV_STG0_GOR1) only; no BGMOn in this branch |
| Otherwise LF0==0 and LF1==1; entrysekai_yami1 | BGMOn(200,BGM_EVT_STG8_WITCH),ENVOff(200) |
| Otherwise LF0==0 and LF1==1; entryminnnanokoe | BGMOff(200),ENVOff(200),LF1=1 |
| Otherwise LF0==0 and LF1!=1 | BGMOn(200,BGM_STG0_GOR1),BGMOn(a0,NULL),ENVOn(110,ENV_STG0_GOR1) |
| Otherwise LF0!=0 (story341 route) | BGMOn(200,BGM_EVT_KUPPA1),BGMScope(0,1),BGMOn(a0,NULL),ENVOn(110,ENV_STG0_GOR1) |

Each named-entry IF is independent in the LF1 branch; they are not invented
story400-only music switches detached from the exact entry names. Story402
must not be given a default On merely because it shares the same area mesh.
The Bowser route's scope write belongs to its source sound owner.

Fresh original native story0..55 coverage excludes341/400/402. Hence admitted
Goomba3 quest2 acquisition in this covered plaza takes the **ordinary** row:
BGMOn(200,BGM_STG0_GOR1),thenBGMOn(a0,NULL),thenENVOn110. Quest1420,
GSF5405/5406 and item95 quantity are not music-selection conditions in this
init prefix. No measured narrative availability of quest2 beforestory56 is
inferred; explicit native known-state fixtures remain separate from actual
Trouble Center acceptance.

## Exact source BGM table rows

Original262-row16-byte table80315ac8, source recordbyte12 volume,
byte13 pan,byte14 surround,byte15 globalpercentage bypass:

| Name / row / address | Stream filename | Volume/pan/surround/bypass |
| --- | --- | --- |
| BGM_STG0_GOR1 /67 /80315ef8 | sound/stream/stg_gor2_32k | 70/64/0/0 |
| BGM_EVT_KUPPA1 /194 /803166e8 | sound/stream/evt_kpa1_32k | 80/64/0/0 |
| BGM_EVT_STG8_WITCH /197 /80316718 | sound/stream/evt_wit2_32k | 88/64/0/1 |

All three records use streamflag80000000. Preserve originalGameCube resource
names and source channel/loop format; a Switch soundtrack arrangement does
not establish this source resource identity even when it has a similar melody.
The original `.stm` source filenames append extension under SoundSS provider.

Ordinary plaza row67 base70/pan64 retains fullgain70 on both source stereo
channels through divisor63 split. Its byte15=0 initializes groupbyte10=0,
so psndBGMMain uses current shared signed percentagebyte803df740 (psnd+58),
not forced100. The byte's actual producer/state is required for exactvolume;
do not invent a stable100 assertion solely from map/story/tableidentity.
Fanfare row217 has bypass1/groupbyte10ff, so its temporary playback uses100
while the plaza owner continues to use its own saved globalpercentage domain.
See [gain publication](FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md).

## Dialogue and acquisition retain group0 association

The actual Goomba3 kuribo3_talk source stream only prints originalquest text,
enters/forces item95 get,then writes5405; it issues no ordinary map BGMOn
or map-load call between these owners. Native Scene::Dialogue is an adapter
state, not an original new-map BGM constructor. Changing toDialogue must not
replace or rewind the retained plaza group0 stream.

Itemget's Off3800 selects existing group0 secondary attenuation state8 and
retains its table index/voice/cursor; it does not retire row67. Fanfare On211
uses separate group1. Eventual On(a0,NULL) resolves numeric0 with continue20,
uses **the current retained group0**, and requests its continue/unmute state.
No row67 lookup/name replay occurs in this NULL resume call. If another
original script genuinely replaced group0, resume must operate on that live
owner, not reconstruct BGM_STG0_GOR1 from native area number.

Group0 may already be source-muted: psndBGMChkSilent(0) thenmainflag8 avoids
itemget's Off3800 and laterresume,while still launchinggroup1fanfare. An absent
or unbound group0 is not a valid basis for fabricating an extraoriginalOn.
Original resource load/admission/room-init execution and retained voice binding
must precede this finite native playback claim.

## Evidence and remaining scope

Fresh original REL event inventory underignored artifacts/reference/
goomba3-services/events.json resolves all prefixsoundservice/name relocations.
Readonly DOL recordreads pin row67/194/197 bytes and filename pointers.
Original gor.rel SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a;
initspan hash aa2fada48b78e1eb7d6eddd114e1bd875e6dc7744a8d2590bb7cf588db1acb7e.

No other-area music rows inferred from assetnames. Next bind actual plaza
original stream and commonLoop metadata, retain group0 cursor acrossmessage/
itemget ownership, establish globalpercentagebyte producer and native audio
resourceadmission; later341/400/402 event/scope/ENV owners remain separate.
No code, assets, sharedindex/progress,userprofiles or saves edited; nooriginal
runtime, audiblemix or hardwarevalidationclaim.
