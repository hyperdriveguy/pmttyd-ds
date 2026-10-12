# Common print event identity, speakers and placement

US G8ME01 revision 0, recovered 2026-10-12. B evidence is static original DOL
instructions/constants, not executed story scenes. See [event runtime](../event/EVENT_RUNTIME.md),
[lookup](MESSAGE_RESOURCES_AND_LOOKUP.md), [lifecycle](MESSAGE_WINDOW_LIFECYCLE.md)
and [window states](MESSAGE_WINDOW_STATES.md). Original text, message IDs,
attribution and event conditions remain required reused data. This local service
contract does not establish complete story ordering or Switch equivalence.

## Wrapper operands and common-call ABI (B)

evt_msg_print at 0x800d284c,size 0x88 receives (event,firstCall). Resolve four
script operands through evtGetValue in order on every invocation, including
resumed calls. Pass them to _evt_msg_print at 0x800d28d4,size 0x4dc as
(event,firstCall,flags,keyOrRawText,callback,speakerOperand).
The final operand is an NPC name pointer on the field route and a unit operand
for BattleTransID on the battle route; do not apply field string typing globally.
The callback is a resolved function value, not an invented dialogue participant.
Return the common helper's result unchanged.

The common helper copies inputs, initializes local resolved-speaker value N=0,
and only performs creation/configuration on nonzero firstCall. Resumed calls
skip all creation, identity, speaker and placement work and poll saved window.
No local first-call failure retry or stale-event/window-index protection exists.

## Creation, identity and event ownership (B)

If input flags mask 1 is clear, msgSearch(keyOrRawText) provides text and retain
the resolved key as identity. If mask 1 is set, use input directly as text and
replace identity with static NUL string DIRECT at 0x802cc71c.

Call msgWindow_Entry(text,callback,s16(event.word[0x190])). Save returned window
index at event+0x178; reset event.word[0x190]=0. No full-pool/allocation check.
Get W and D; set D.word[0]=identity pointer and D.word[f200]=owning event pointer.
This fills the constructor's initially zero identity field. It does not copy
the identity string or transfer its storage ownership. Field f200 is distinct
from the callback pointer f1fc installed during construction.

If input flags mask 2 is clear, take field/NPC speaker route below; otherwise
take battle-unit route. Other input flag bits are not inspected locally here.
This helper neither substitutes resource placeholders nor rewrites dialogue.

## Event NPC name resolution (B)

evtNpcNameToPtr at 0x8004f5d8,size 0x168 receives(event,name). Exact strcmp
checks in order, with no null guard or case folding:

| Name | Returned value |
| --- | --- |
| me | event.word[170] |
| party | word -1 |
| (^x^)party | word -1 |
| extparty | word -2 |
| mario | word -3 |
| master | pointer at event.word[170]+32c |
| slave_0 | pointer at event.word[170]+330 |
| slave_1 | pointer at event.word[170]+334 |
| slave_2 | pointer at event.word[170]+338 |
| slave_3 | pointer at event.word[170]+33c |

Other names call npcNameToPtr(name). Relationship branches dereference event
owner without local validity checks. Return sentinels are not ordinary NPC
pointers and need the consumer's explicit handling; do not dereference -1/-2/-3
as character structures or normalize them to null.

## Field speaker choice and anchor (B)

On this route call the speaker operand speakerName. If event.byte[0x10]=6,
initialize N from event.word[0x170]; other event types
leave N=0. If speakerName is nonnull, override N with evtNpcNameToPtr(event,name).
Thus a named override takes precedence over the event's owning NPC.

For every field route, position writes are allowed only when
`event.word[0x180] & 0x1effffff == 0`. The mask is literal, not 0x1fffffff:
0x01000000 is excluded while several higher bits are included. Speaker/name
storage still occurs when that gate suppresses anchor derivation.

| N | Anchor source when gate allows | Text copied into D+f204 |
| --- | --- | --- |
| -1 | marioGetPartyId(), partyGetPtr(result); partner position+58/5c/60 and add float+f0 to Y | input speakerName |
| -2 | marioGetExtraPartyId(), partyGetPtr(result); same partner offsets/height | input speakerName |
| -3 | marioGetPtr position+8c/90/94, add float+1bc to Y | input speakerName |
| ordinary nonzero NPC pointer | NPC+8c/90/94, add float+150 to Y | NPC name at NPC+8 |
| 0 | no anchor/name assignment locally | no assignment |

For -1/-2, if partyGetPtr returns null, fall back to Mario position/height as in
-3. All anchors go to W floats+30/+34/+38, copied as words then one float Y add.
There is no local Mario/NPC pointer failure guard. Repeated marioGetPtr calls
read position and height separately. Name copying uses unbounded strcpy; no
field capacity is supplied. A sentinel route with null speakerName is not safely
handled by this code, even if a valid anchor can be found.

Speaker name is pose/association data, separate from the message key and literal
dialogue bytes. Actual attribution still requires the original event call graph;
having a message text alone does not establish who says it or under what condition.

## Battle speaker route (B)

Set D.flags bit 8, call BattleTransID(event,speakerOperand), store returned unit ID
at D.word[f224], then BtlUnit_GetTalkTogePos(unitID,&W+30,&W+34,&W+38).
Write W.s16[2c]=3. No local unit-ID validity/failure fallback or NPC name copy.
The common helper's local N remains zero on this route, so default placement
selector zero below does not run the field automatic-projection branch.
Battle translation/talk-position internals and unit event producers remain open.

## Placement selector and projection branches (B)

After speaker setup compute selector=`event.word[0x180]&0x1effffff`:

- 0: if N=0, leave mode unchanged. Otherwise call getScreenPoint(&W+30,out).
  If window type=1, write mode W.s16[2c]=7. For other types, if abs(projectedX)
  >= binary32 300.0 at 0x80421874, write mode2. Otherwise project Mario position
  at +8c using getScreenPoint; if speaker projectedX<Mario projectedX, mode3,
  else mode7. Equality takes mode7. These are projected coordinates, not an
  inferred left/right speaker-facing direction; camera projection remains open.
- 1: mode0; no additional anchor copy.
- 2: mode7; copy event words+184/+188/+18c to W+30/+34/+38.
- 3: mode2; same three-word anchor copy.
- Other values: leave prior mode/anchor as already set. No local selector error.

Overrides then occur in this exact order:

1. If event.word[180] has 0x80000000, mode=3. Only inside this branch, OR mode
   with4 for 0x40000000 and with8 for 0x20000000.
2. Independently, if that event word has 0x01000000, OR mode with16.
3. Reset event.word[180]=0.

No literal mode names are inferred from these bit patterns. Renderer consumers
must establish tail geometry, orientation and flags. Event+184/+188/+18c values
are not reset here, and the one-shot placement/layer words are distinct fields.

## Polling and completion (B)

Every invocation calls windowCheckID(event.word[178]), which returns W.flags&2.
Nonzero yields return0; zero returns2. This helper does not delete the window,
copy a selection result or wait for allocation flag1 to clear. A stop opcode
can clear poll flag2 while leaving message data/window allocated, so callback
completion is not equivalent to final dialogue closure. Other event wrappers,
window states and lifecycle helpers own later continuation or cleanup.

## Differential obligations and remaining work (P)

Compare operand resolution on resumed calls versus first-call-only creation,
direct/key identity, layer halfword/reset, event ownership/callback separation,
event-type6 owner versus named override, all special name/sentinel/relationship
routes, absent partner fallback, literal placement mask, suppressed-anchor name
copy, battle versus NPC state fields, projection threshold/equality, mode override
ordering, consumed versus retained event words and poll completion without delete.
These are derived obligations, not executed story observations.

Remaining: all print variants (partner/insert/add), placement/layer producers,
substitution and callback bodies, speaker helper services/camera projection,
renderer and manager cadence, identity lifetime and original story call-graph
attribution/order/branches/conditions. Common print and NPC name resolution are
locally documented; full dialogue and complete-game equivalence remain unfinished.
Partner/append variants and NPC/layer/placement setters are now specified in
[print variants](MESSAGE_PRINT_VARIANTS.md); insert substitutions/callbacks and
story call graphs remain open.
