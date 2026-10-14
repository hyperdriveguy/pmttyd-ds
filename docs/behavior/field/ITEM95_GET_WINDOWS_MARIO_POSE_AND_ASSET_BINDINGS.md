# Item95 get windows, Mario pose and asset bindings

US G8ME01 disc0 revision0; read-only DOL/icon recovery2026-10-13.
Complements [finite receive sequence](GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).
These are original display/control requests, not matched GX/PICA pixels.

## Exact original window definitions

Original itemdrv_window_desc80313dc8 contains40-byte definitions. First two:

| Field word offset | Name80313dc8 | Help80313df0 |
| --- | --- | --- |
| +0,+4 | 0,0 | 0,0 |
| +8 | 8 | 8 |
| +c,+10,+14,+18 | -175,120,350,42 | -240,-130,500,80 |
| +1c color | e1d79bff | f0f0f0ff |
| +20 maincallback | NULL | winHelpMain800acdc8 |
| +24 displaycallback | winNameDisp800acf34 | winHelpDisp800acd5c |

Entry uses winMgrEntry/Open/SetParam(entrypointer); original manager parameter
is+2c,definitionpointer+28. Window rectangles live+18/+1c/+20/+24.
Manager service's constructor/frame/action semantics remain providers.
winMgrAction8023eb1c/18 returns(managerflags>>2)&1 for the requested68-byte
record: it is the **actionbit4**, not a text-finished query or elapsed timer.
Name/help displays both skip their content while this actionbit is set.

winNameDisp resolves itemDataTable record+4 through msgSearch. Item95's
name key is **in_irai_butsu**, description+8 is **msg_irai_butsu**.
It chooses **msg_window_item_get** (item95 outside badges240..338), never
fabricated NPC story text. GPword16c==0 formats original template via sprintf
with item name. Nonzero selector (including US1) copies original template then
calls unk_800d1364(buffer,itemname). This original localization expansion
service remains a dependency; do not assume US text is ordinary sprintf just
because the alternate language branch uses sprintf.

Measure FontGetMessageWidthLine(buffer,&lineCount);retain low16 widthW.
If W+20>managerwidth,write width=W+20 and X=trunc(-width/2). Set height to
originaldefinitionheight+24*lineCount. Only when namewindowaction0:
FontDrawStart,FontDrawColor(original palette word804210a8),
FontDrawMessage(trunc(-W/2),managerY-8,buffer). The callback has no separate
original story evt_msg_print. Reflow may adapt screen placement, preserving
original template/name/control tokens and source expansion semantics.

winHelpDisp checks helpwindowaction0, resolves original item record+8 via
msgSearch, stores textmanager+34 then calls winMgrHelpDraw8023e53c.
Its maincallback only delegates winMgrHelpMain8023e9b8.
Existing [help draw contract](../message/ITEM_TABLE_SELECTION_DRAW_REQUESTS.md)
owns shared scissor(0,384,608,56),two-line scrolling and arrows; it must not be
reimplemented as a different invented message bubble.

winMgrHelpMain skips when managerflags4. If retainedtext+34 nonnull,measure
FontGetMessageWidthLine and set linecount+3c=reportedlines+1. For count>2,
keyGetButtonRep(0)&100 subtracts2 from page+38 and clamps negative to0;
else keyrepeat&200 advances2 only if(trunc((linecount+1)/2)-1)>
trunc((page+1)/2). These are repeated A/B bits in this service; source
itemseq acknowledgment separately checks A/B **trigger** after action0.
Original scheduling can expose both readers in the same update; preserve
owners rather than silently substitute directional-input page controls.

## Mario get-item motion and original pose

marioChgGetItemMotion80094c90/100 special currentmotion22 calls800a15a8;
21/25 clear verticalfloat180;26 returns. Ordinary path rejects Mario mainflag
10000000. If oldmotion30 is16/17 it conditionally clears displayflags08000000
and4. Then preserve currentmotion2e into previousmotion30,set current15,
call previousmotion's table80310030+12*previous +8 exitcallback ifnonnull,
OR pendingword0c bit1. This requests motion initialization, not an immediate
pose call inside the service. Special forms/flags require their own admission.

Motion15 table row803100e4 holds mot_getItem80092a3c/58,othercallbacksNULL.
On pendingbit1:clearbit1,clear words48/44 andhalf50,clear vertical180,
marioAdjustMoveDir800965fc,then marioChgPose80058d84 with original
string80420a14 **M_I_U**. Later invocations pendingclear do nothing locally.
Original pose-bank routing remains
[ordinary selection](../overworld/OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md).
Fresh read-only ISO animation-directory inspection pins a/a_mario M_I_U
index39, one nonloop descriptor start/end0..0 and single sparse keytime0.
It is absent from a/a_mario_r and a/e_mario. This is an authored static pose,
not an inferred timed waving clip. Existing original basic-name evidence also
contains M_I_U; normal basic pose selection uses front a_mario. Do not replace
it with Goomba13's M_N_5B/M_N_C or an arbitrary same-name bank.

## Original icon and effect-resource identities

Item95 record80311780 stores icon-descriptor307 at u16+20 (0x133).
Original icon.bin offset-table half2*(307+1) points0x12f2; descriptor halfwords
(0,0,0,1,312,0). Thus original icon.tpl image312 is the field item95 visual,
through the same traced iconGX texture selector as
[original item00](../story/PROFESSOR_ITEM_POSE_JUMP_AND_TURNS.md).
Keep descriptor307 versus TPLimage312 distinct.

Source effItemGetEntry80212488/2b8 type6 allocates one60-byte payload record,
installs effItemGetMain80212188,sets effectnameItemGet,payloadkind6,root at
MarioXYZ+(0,62,0); caller sets payload+38=5. effItemGetDisp802118a0/8e8
requests effGetTexObj IDs44 and42,loaded in GXtexturemap0/1, at80211c54/6c.
Both IDs<118 route through effectmanager+8's original base texture bank;
effGetTexObj8005ed58 uses original dummy1x1 fallback whenmanager+0c==0.
Do not reinterpret these as US-bank IDs44/42: IDs>=118 use the separate
language-cache bank and subtract118. Exact manager+8 asset installer,
effect kind6 update/draw curve/TEV/colors and GXshape parity remain open;
resource IDs alone do not establish a complete effect presentation.

## Verification and remaining providers

Fresh named DOL extraction: winNameDisp,winHelpDisp,winHelpMain,winMgrHelpMain,
winMgrAction,marioChgGetItemMotion,mot_getItem,effItemGetEntry/Disp/effGetTexObj.
Raw source definition/item/motion tables and icon descriptor read from readonly
ISO; inspection files ignored under artifacts/reference/goomba3-services.
[Exact95 insertion](../unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md) explicitly
supersedes the contradictory legacy pouchGetItem pointer-lookup stub.

Still required: manager open/close animation ownership, US template-expansion
helper, font numeric/backend parity, M_I_U native resource conversion and
motion dispatcher cadence, full effect6 update/render/texture installation,
capacity-failure menu and original/native/hardware execution validation.
No prose/dialogue rewritten, assets committed,code/sharedindex/progress,
emulator profile or saves changed. Documentation-only validation.
