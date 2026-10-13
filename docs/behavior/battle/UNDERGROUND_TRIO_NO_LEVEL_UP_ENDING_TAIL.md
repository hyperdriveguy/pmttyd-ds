# Underground trio: original ten-point ending tail

2026-10-13 read-only US G8ME01 source delivery. This closes the ordinary
positive10EXP/no-level-up continuation after [EXP counting](UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md).
Owners remain B=battle work, I=B2738, S=I+c, E=Bf28, P=pouch.
Evidence: complete btlseqEnd80215234/1470 listing `/tmp/trio-para-end.dis`,
complete used leaf listings `/tmp/trio-tail-*`, original jump tables/constants.
No native changes or executed original-game timing claim.

## Per-invocation state order

State numbers below are hexadecimal. Each row ends at the controller's common
display exit, unless stated otherwise. Advancing a state does not execute its
new row in that invocation. The earlier08→09 fallthrough is a separate rule.
The current ordinary path requires E0c=0, Bflags00400000 clear and P96+10<100;
level99, zero-EXP, fortune-end and upgrade branches retain their own providers.

| State / source entry | Exact ordinary continuation |
| --- | --- |
| 0c /80215b48 | Decrement signed E4. Positive waits. Expiry first writes E4=60, calls `_GetExpIcon_End(E)`, sets0d, calls effSoftDelete(E29c), then writes E4=5 when E0c=0 (60 if level-up). |
| 0d /80215bb0 | Decrement E4; positive waits. Expiry clears E29c and sets0e. No effect-live query. |
| 0e /80215be8 | Bflags00400000 clear sets10. Set branch launches original8041b8c0 priority10/flags0, writes Mario owner to event160 and ID15c to U2a8, then sets0f. |
| 0f /80215c50 | Conditional branch only: wait actual BattleWaitAllActiveEvtEnd8011ace8; ready selects10 for E8>0, otherwise19. Ordinary clear-flag path never enters0f. |
| 10 /80215c9c | Clear E30 bit1. E0c=0 sets19. Nonzero initializes original upgrade selection and goes11; it is not an ordinary ten-point branch. |
| 19 /802163a4 | Source voice gate below; always SetBodyAnim(Mario, `M_V_1`), E4=60, set1a. |
| 1a /80216488 | Decrement E4; positive waits. Expiry sets Mario body animation type42; nonnull eligible party also type42; set1b. |
| 1b /80216504 | Set1c only. |
| 1c /8021651c | Set1d first; BattleAudienceSoundSetVolAll(0,30); fbatGetPointer returned F8 nonnull NPC gets NPC140 OR100. |
| 1d /8021655c | fadeEntry(10 decimal,200 milliseconds,black RGBA000000ff at80427144), set1e, BGMOff(201 hex). |
| 1e /80216590 | Wait actual fadeIsFinish80010d94; nonzero sets1f. |
| 1f /802165b4 | Ordered unit export, end-work free, top-bank completion and field-party reconstruction below. |

The normal point counting grants P96=10 with unchanged level1. It does not
restore HP/FP or maxima. The ordinary0d five-call delay is not permission to
wait for disappearance of the point-get effect.

## Effect and body-animation ownership

effSoftDelete8005e9c0/70: null or effect flags0 does nothing. Flags&2 sets
flags4 and preserves handle/payload; otherwise frees heap3 payload+c and
clears effect flags. The actual PointGet effect has active1 and flag2, so
this requests its own leave phase. E29c is cleared after the five-call0d
delay independently of effect completion/deallocation. Effect-driver cadence
and GPU ownership remain [effect services](../effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).

State19 retains the source Mario U138 flag word. Either mask10000000 or
20000000 suppresses the voice. With a party, the source checks that retained
word again and calls party CheckStatus(27); a dead party suppresses voice.
Do not silently substitute a party U138 load for the literal cached Mario
word. Admitted voice: S4&20000000 selects string802feafc
`SFX_VOICE_MARIO_RELIEF1`; otherwise802feb14 `SFX_VOICE_MARIO_RELIEF2`.
BtlUnit_snd_se receives Mario, that key, original float sentinel f1194d80,0.
M_V_1 at80427298 is requested even when voice is suppressed.

BtlUnit_SetBodyAnim801275f4/4c finds the unit's original body-part ID and
part pointer, then btlDispChangeAnime(part,name,force0). SetBodyAnimType
80127640/48 finds the same part and delegates SetAnimType801276ac.
State1a skips party type42 only when party kind225 and signed U12b>=1.
Goombella224 is admitted; actual type42 resolves PKR_R_1 (table80393a98,
entry80393af8→string802f9108). Mario type42 resolves M_R_1. These are pose
types, not event-data keys41/42. The positive-EXP children earlier use key40;
their original M_V_2/PKR_Y_1 assets and owner U2a8 are documented separately.
No extra key41/42 event is launched by this ordinary tail.

## Actual fade10 scalar completion

fadeEntry80013368/73c uses manager=r13+1700 (global8041e620), slot0=manager+8,
stridea8. Used type10 initializes slot flagsU16=1, word4=10, state8=0,
duration signed64 words10/14=(0,200), color18=000000ff, floats24/28/5c=0,
float90=1, words6c/70=-1, and manager350=0. General resource-release and other
fade slots remain the [fade dispatcher](../display/DISPLAY_FADE_ENTRY_DISPATCH.md).

fadeMain80012058/11c8 active-flag1/type10 route800121c0, only while state8=0:
`slot5c = float(slot5c + 1000.0 / signed(GP4))`; convert duration64 to float;
`slot24 = float(slot5c / duration)`; `slot28 -= 10.0`.
Only **ratio >1.0** clamps ratio1, ORs flags2 and increments state8. Equality
does not finish. Duration0 directly sets ratio1/flags2/state8++. Constants
1000=8041f804,10=8041f808,1=8041f7ac. GP4 is the original refresh-rate owner;
use its supplied value rather than hardcoding a map-transition tick count.
Type10 submits disp_texture80011a7c through dispEntry80010bc4 with (9,0,
callback,manager) and source layer float8041f7a8=0. Full GX texture/viewport
consumer and actual global fade scheduling remain rendering dependencies.

fadeIsFinish80010d94/2c reads manager350. -1 returns1. Otherwise it returns
the selected slot flags bit1 (mask2). It does not infer readiness from opacity,
native elapsed ticks, or the battle end state's timer.

## Ordered export and original field admission

State1f captures current party U8 before teardown (0 if none), translates
through BattleTransPartyId800f8444, and captures original S through I+c.
S4&40 alone admits Mario signedHP10c<=0→1. Ordinary clear flag does not heal.
BattleSetMarioParamToFieldBattle800f8a68 exports Mario then nonnull party;
see [exact export and snapshot domain](PARTY_FIELD_EXPORT_AND_SNAPSHOT_CALL_DOMAINS.md).
Then BattleFree800f8908(E), Bf28=0, top bank0=4; field Mario word4 clears1.
Captured party kind nonzero calls marioPartyEntry(translated ID), zero calls
marioPartyKill. It then obtains current field party via marioGetPartyId and
partyGetPtr, clearing word4 bit1 when nonnull. End-work display common exit
still runs with null E and follows its null gate.

marioPartyEntry801412fc/110 ID<8 requires pouch party record14*ID flags&1;
otherwise returns-1. Existing signed fieldMario245>=0 is partyKill'ed and
245=-1, then partyEntry800bea68(ID); nonnegative returned registry index
is stored245, resolves partyGetPtr and requests partyHello800bbc48.
ID>=8 uses auxiliary slot246/ext-party constructor800bddd0 instead. No
implicit party unlock is granted. marioPartyKill80141198/6c removes existing
nonnegative245 then246 via partyKill800be69c and sets each removed slot-1.
These field constructors/hello lifetimes are distinct from surviving battle
unit memory and must use the actual Progress-owned unlocked Goombella record.

Next BtlActRec_JudgeRuleKeep8018ef8c reads I18. Fresh Rule0 uses jump table
803977b0[0]→8018efe0: writes I19=6, then returns, after original Mario/party
queries. Other0..67 rule cases remain their source providers. Finally
BattleAudienceSoundStop(1)8019fd68 queries801a85bc; record word0&1 requests
psndSFXOff800d95f8(record4), then reloads flags and clears1. Sound-record
producer and actual playback remain audio dependencies.

Uppercase BattleMain topbank4 returns0; lowercase battleMain publishes
Bflags80000000; seq_battleMain advances and waits its counter **>30 calls**
before seqSetSeq(2,1,0), unless source global conditions select an alternate
sequence. The field wait requires seq2 AND Iresult!=0. Follow
[field protocol](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md) and the preserved
[161-command trio continuation](../story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md).
State1f itself does not unlock controls, publish story12 or fabricate an
immediate field return. Native event/archive/resource/audio scheduling and
postbattle restoration must be bound before claiming complete victory.

Validation: complete read-only local bodies/jump tables and constants,
`git diff --check`. Documentation-only; no native tests/builds or commit.
