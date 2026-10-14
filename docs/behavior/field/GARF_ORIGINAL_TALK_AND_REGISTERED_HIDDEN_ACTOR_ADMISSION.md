# Garf original talk and registered versus hidden actor admission

US G8ME01 disc0 revision0; read-only recovery2026-10-14. Complements
[quest1 world actors/door copy](TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md),
[ordinary candidacy](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md), and
[NPC collision response](FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md).
Static contracts, not native/Dolphin execution or complete quest implementation.

## Complete tozokudan2_talk branch tree

gor_02 tozokudan2_talk file42c04/1e4,39 commands, is the setup talk pointer
of registered actor盗賊団２ (Garf). All prints in this local body are exactly
evt_msg_print(0,originalID,0,me). Resolve me to the actor owner, not an
unattributed message or a literal speaker name. The body has no local
KeyOff/On, toge, actor-turn, regular-event stop/start or finish-pose requests;
the ordinary NPC talk owner handles those services and control lifetime.

First test GSW1420==1. On that branch:

1. evt_pouch_check_item(49,LW0).
2. If signedLW0>=1, printirai_01_02; then
   evt_win_item_select(0,key_tbl,LW0,0). key_tbl424a4/8 is signed[49,-1].
   If returnedLW0!=-1, blocking opcode94 calls evt_unlock_npc.
   Return regardless of selection result. It does not fall through to advice.
3. Otherwise, ifGSF5406==0, printirai_01_00, await print completion, then
   SETGSF5406=1. Else printirai_01_01. Return.

Thus5406 is the original shared working flag used here, not a new flag named
for Garf or a quest-completion flag. Countunknown is not equivalent to zero.
The selected item is not additionally compared with49 in the local body;
the actual original finite item selector/provider enforces its[49,-1] table.
No source hand-in can be replaced by automatically consuming an owned key.

When1420!=1, SWITCHsignedGSW0 with first matching CASE_LESS(opcode38):

| Story test | Message ID |
| --- | --- |
| <56 | gor_02_011 |
| <164 | gor_02_011_01 |
| <215 | gor_02_011_02 |
| <261 | gor_02_011_03 |
| <339 | gor_02_011_04 |
| <373 | gor_02_011_05 |
| <401 | gor_02_011_06 |
| default | gor_02_011_07 |

There is deliberately no112 threshold on this actor. All eight exact US
messages are83 bytes and contain wait250 then keywait. Advice00 is294bytes
with original waits/keys/pages;01 is174bytes with keys/pages;02 is24bytes
ending keywait. Exact text remains original msg/US/gor_02.txt. Latch the
admitted branch while a blocking print is pending; do not reevaluate switches
to substitute a different message halfway through a live talk.

## Hand-in child is an actual world scene

evt_unlock_npc42a80/184,24commands, runs in this order:

- OR40000020 on named盗賊団２; printirai_01_03 attributed to that actor.
- Blocking evt_sub_get_coin(20),then nonblocking RUN_EVT(mario_yokeru).
- evt_npc_jump_position_nohit(actor,445,8,80,300,30); WAIT500ms.
- Move mobj_lock00 to(0,-1000,0); SETGSF1219=1.
- Get actorXYZ intoLW0..2; WAIT100ms; positionalSFX_MOBJ_LOCK_SWING1 at that
  capturedXYZ; WAIT200ms; positionalSFX_MOBJ_UNLOCK1 at the same capturedXYZ.
- evt_pouch_remove_item(49,LW0), then nonblocking RUN_EVT(tozokudan2_house_door).
- evt_npc_move_position(actor,445,-85,0,encodedF24B6A80,0).
- SET1420=0, SETGSF5375=1; LW0=door_data_06; blockingevt_door_entry.
- Clear40000020 on actor; print(0,sys_request_finish,0,0); return.

The encoded speed operand is retained here verbatim; do not invent a movement
speed or replace source jump/avoid/door work with a delay. mario_yokeru42734/1fc
has conditional Mario/party move/turn work; tozokudan2_house_door42508/22c
owns house06 original map visibility and open/close interpolation, inline
completion flags and sound children. Their complete local event inventories
are retained in ignored artifacts/trouble-garf-unlock-children.json.
This child is distinct from the quest-start update of house05's copied NPC list.
Actor, key selector, original coin presentation, party/Mario movement, no-hit
jump, map-lock object, sounds, door entry/interpolation and completion message
providers are all required before admitting a complete hand-in scene.

For a bounded ordinary talk binding, known1420!=1 admits the corresponding
story-only original print. Known1420==1 with knownitem49count0 admits the
advice branch and its post-print5406 write. A nonzero count requires the real
selector/hand-in services. Do not silently use the advice or ordinary-story
arm because the actual quest branch lacks providers.

## Registration/name lookup does not test render-hidden800

The field NPC registry uses stride340hex records and mainflags+0. Both
name helpers select field/battle work via liveGP14, scan registered capacity
in source order, require only activebit1, then strcmp(originalName,NPC+8).
npcNameToPtr_NoAssert80049934/ac returnsNULL after a full miss. Ordinary
npcNameToPtr800499e0/9c returns the matching record; its shown executable body
has no local runtime missing-name guard (a miss reaches its end-of-array
pointer), so valid names are caller invariants. Neither checks2,8,800,
camera/frustum, map interior masks, story thresholds, talk pointer or distance.
An actor hidden from drawing remains registered/addressable until actual
deletion removes activebit1 or its pool/area lifetime ends.

Source npc_hide_onoff800e8090/a4 toggles only main800 for actual matched names
from the mutable door record's NULL-terminated list. It does not delete actors,
clear active1/query2, alter positions or disable their talk/side-response gates.

| Consumer | Actual local admission related to this distinction |
| --- | --- |
| Name lookup | Active1 and exact original name;800 ignored |
| Ordinary talk scan80047874..47c98 | Query2 set,8 clear,talk128 nonnull;800 ignored; original height/radius/facing/map-ray gates still apply |
| npcMain entry80049b00..b10 | Active1 ANDquery2;800 ignored |
| Mario side-response call80049f88..fa8 | Skip if8,20 or4000;800 ignored; state10/other earlier gates remain |
| npcMain display tail8004acc4.. | Call800440f8, then freshmain800 test at8004acd0;set800 skips to previous-root snapshot8004b10c |

The800 draw skip precedes projection/frustum/material/display registration;
it does not bypass the already performed simulation/collision/talk work or
the admitted current-to-previous root snapshot. Clear800 merely permits later
draw gates; it does not guarantee an on-screen model. A closed wall can still
block talk rays physically, independently of800. Do not replace these separate
predicates with one generic native visible() test.

Native design should separate original-name registration/lifetime, known source
init ownership, mutable current/previousXYZ, actual main/secondary flags,
ordinary collision/candidacy and renderer admission. A named set-position must
find an existing active actor even when800 suppresses drawing, preserve that
bit and seed previous/current roots. Removing800 during reposition is not a
source evt_npc_set_position effect. Native interior coverage masks are not
original active-bit/name-lookup predicates.

## Evidence and remaining boundaries

Source rel/gor.rel SHA256
d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a.
Talk SHA256f1d444a6d8c5e49967640074424b26f33d30288554267d2b8793322af421a078;
unlock47e93cc006f0eb0b92432b7c3a405e6f31ce628e4ba103e961110a932eac14ef.
Ignored original event reports: artifacts/trouble-garf-talk-events.json,
trouble-garf-unlock-events.json,trouble-garf-unlock-children.json.
Fresh full DOL listings: artifacts/trouble-garf-name.txt,
trouble-garf-noassert.txt,trouble-garf-main.txt. Existing ordinary candidate
and side-response contracts establish their complete relevant source gates.

No native scene completion, original runtime timing, final hidden-actor pixels,
full map/NPC deletion lifetime or all quest1 hand-in providers are claimed.
No code, shared indexes, progress, asset records or original text/assets edited.
