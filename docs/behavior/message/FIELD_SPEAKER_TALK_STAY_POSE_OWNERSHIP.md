# Field speaker talk/stay pose lifecycle

US G8ME01 disc0 revision0, read-only static DOL evidence. Scoped native binding
answer for [OW-INTERACT-01](../../requests/OW-INTERACT-01.md) and
[OW-ROAD-01](../../requests/OW-ROAD-01.md). Use existing
[print speaker identity](MESSAGE_PRINT_EVENTS.md),
[General window states](MESSAGE_WINDOW_STATES.md) and
[playback records](MESSAGE_PLAYBACK.md). No original visible animation trace.

## Print creation does not activate the talk pose

_evt_msg_print0x800d28d4/0x4dc creates/configures a window only on firstCall,
stores field speaker name in D+f204, and every invocation polls windowCheckID.
It does **not** call npcSetTalkPose directly. Message event completion only
means pollflag2 cleared; retained state2 and closing states may remain active.
Rendering a speaker's talk animation for MessagePlayback.active() therefore
adds talk ownership not present in the source.

evt_msg_print_party0x800d2790 constructs exact suffixresource key on every
invocation, then calls commonprint(flags0,key,callback0,speaker="party").
Speaker ownership remains the actual party route; expanded `_kur` text does
not convert it into the explicit field NPCnamedクリスチーヌ.
evt_msg_print(0,mac_0_069_02,0,"party") uses the same party pose helper while
retaining its unsuffixed resource key. Both differ from lecture084_06's explicit
Goombella NPC speaker.

## General_Main owns a per-invocation talk countdown

Before dispatching **every General window state**, inspect cached D.f250.
If nonzero and cached D.f254==0:

1. For D.flagsbit8 battle route request BtlUnit_ChangeTalkAnim(D.f224).
2. Otherwise request npcSetTalkPose(D+f204).
3. Set cached latchD.f254=1.

Decrement cached D.f250 once. If it becomes0, reload handledata and request
the matching battle/NPC stay helper, then clear cachedD.f254=0. There is no
positive-only guard or once-per-renderframe shortcut; this runs once per admitted
General_Main invocation, including opening,closing and retained/historystates.
Clear/selection-main callbacks have different owners and omit this countdown.

msgMain generic-record route examines recordflags after callback(5,handle).
If recordbit0x10 is set, that branch has priority and does not set talkcountdown.
Otherwise if recordbit8 is set and windowtype!=3, fresh D.f250=15 is written.
This write occurs **after** the General pre-dispatch decrement, so its first
talkpose request is on a later General invocation. Ordinary nonspace glyphs
carry bit8 through the parser; waits/spaces do not continually refresh talking.
Consequently a sufficiently long wait can return a speaker to stay while the
message window still has more text to print. The exact countdown is15 calls,
not an independently measured250ms timer.

General state1's msgMain result handlers also request stay:

| Result | Source state/ownership |
| --- | --- |
| 0 | state7,request stay,draw;closing sound/fade begin nextinvocation |
| 1 | state2,clear windowpollflag2,request stay,draw;retain allocatedwindow |
| 2 | state3,inputwait,draw;no result-branch staycall |
| 3/other | retainedstate,draw;no result-branch staycall |

These result staycalls do not locally clear every countdown/latch field. A
retained nonzero countdown still participates in subsequent pre-dispatch logic.
Do not silently reset it to simplify native state transitions. Force-close/
window lifecycle cleanup has its separate source stay restoration requests in
[window lifecycle](MESSAGE_WINDOW_LIFECYCLE.md) and
[force-close](MSG_WINDOW_FORCE_CLOSE_AND_REPEAT.md).

## Exact helper routing and force/restart behavior

npcSetTalkPose0x8004979c and npcSetStayPose0x80049644 are both0x158bytes.
Both ignore empty speakername and the source ignored-name literal. Their exact
sentinel-string routes:

| Speaker name | Talk request | Stay request |
| --- | --- | --- |
| party | marioGetPartyId→partyGetPtr;partyChgPoseId(P,7) whennonnull | samequery,selector1 |
| extparty | marioGetExtraPartyId→partyGetPtr;selector7 whennonnull | samequery,selector1 |
| mario | marioChgPose(source retained talkname) | marioChgPose(source retained stayname) |
| ordinary NPCname | enablednamedNPC;requires NPC+0 mask0x200;animPoseSetAnim(pose,NPC+6c,force1) | sameguard,animPoseSetAnim(pose,NPC+4c,force1) |

NPC stringsearch scans the active context's832-byte actor records by strcmp,
checking enabledbit1. The guard is0x200, distinct from drawhide0x80 and pose
facing-snap0x8000. Source Professor setupflags0x80600 includes0x200, so it
admits these pose requests. Helpers forceNPCanimation restart1 on each actual
request; native must not repeatedly call them every drawframe or substitute
force0 solely because the name has not changed. PartyChgPoseId has its own
changed-name/selector behavior, including normalkind1 stay low-HP remapping;
its implementation is not the same as direct NPC force1.

Actual source used asset bindings:

| Actor owner | Talk/stay |
| --- | --- |
| Professor NPC,tribe0x80333014 | c_kuri_h:T_1/S_1 |
| Goombella NPC,tribe0x803316d0 | c_pkr:PKR_T_1/PKR_S_1 |
| Ordinary Goombella partnerkind1 | normalrowselectors7/1→PKR_T_1/PKR_S_1;lowHPstay policies remain ownowner |
| Temporary Professorkind11 | normalrowselectors7/1→T_1/S_1 |

Field NPC no-hit hop does not select a jump animation. Importing PKR_T_1 for
actual message requests does not justify applying it to the earlier hop or
all activewindow states. Keep NPC and partner identity/lifetimes distinct even
when both use the same c_pkr bank/pose names.

## Native binding obligations and evidence

Expose the actual General talkpose request/countdown ownership to the speaker
adapter. New glyphpublication refreshes countdown15; subsequent Generalcalls
toggle the talklatch; completion/stop request stay independently. Messagewindow
allocation/active state,sourceEventpoll completion,playerkeywait and talkpose
are separate state fields. Opening a window or merely finding its speaker
does not imply talking. Native timing/admission must retain the source order
and avoid advancing pose clocks after the selected pose request incorrectly.

The existing parser/playback/window contracts pin the containing DOL bodies.
This delivery directly disassembled bothNPCpose helpers with
tools/inspect_dol_function.py and traced actual Professor/Goombella tribe/party
pose rows. No native code,sourceasset payload or new execution trace added.
Original managercadence,callback-induced handle relocation,party lowHP/live
member switching,battle talkpose services and rendered-runtime comparison
remain dependencies beyond these local helper bindings.
