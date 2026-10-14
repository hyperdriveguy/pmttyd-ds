# Item95 US template expansion and window acknowledgment

US G8ME01 disc0 revision0, read-only DOL/msg/US recovery2026-10-13.
Complements [original get windows](../field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md)
and [finite get sequence](../field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).
This closes finite title expansion and wrapper action ownership, reusing the
already recovered [window-manager sequence](ITEM_TABLE_SELECTION_WINDOW.md#main-lifecycle-and-manager-ordering).

## Original US template is substituted, not rewritten

unk_800d1364 at800d1364/b70 reads GPword16c and dispatches original table
80315360. Selector1 uses800d13ac (selector6 shares this branch). Other
language branches are separate source grammars, not a generic printf service.
US namecallback first strcpy's msg_window_item_get into its buffer, then
calls this helper(buffer,originalitemname).

The English branch inspects **first raw itemname byte**. Original table
8031528c has53 entries for unsigned(firstSignedByte-65)<=52. Only
A,E,I,O,U,a,e,i,o,u choose original constant "an " at8042183c;
every other firstbyte chooses "a " at80421840. It does not skip leading
markup/space,inspect pronunciation or add semantic exception rules.

Finite item95 original msg/US/global.txt keys are:

| Key | Original payload |
| --- | --- |
| msg_window_item_get | You got <AN><col c00000ff><ITEM></col>! |
| in_irai_butsu | Box |
| msg_irai_butsu | The thing McGoomba asked\nyou to deliver to Goomfrey. |

The description table's `\n` denotes the actual LF. Preserve raw resource
bytes when rendering it.

For the finite title, helper searches first literal **<AN>**,copies preceding
bytes into stacktemporary,terminates it,appends chosenarticle and tail after
fourbyte token,then copies this intermediate string back into originalbuffer.
It then searches first literal **<ITEM>** in the intermediate buffer,copies
prefix into temporary,terminates,appends originalitemname,appends tail after
sixbyte token. Common tail copies temporary back into suppliedbuffer.

Thus exact item95 result retains original color tokens:
`You got a <col c00000ff>Box</col>!`.
This is source-template output, not authored replacement dialogue. Neither
article nor name is supplied by the current NPC story speaker. No fabricated
acquisition paragraph is necessary to present this original window.

If <AN> absent,the branch first searches <AN_ITEM> and replaces that first
ninebyte token with article+itemname; otherwise it searches <ITEM> alone.
This is **first match**,not global recursive markup rewriting. The finite
item95 template has both expected tokens,so those alternate/missing-token and
unbounded C-buffer domains need not be inferred as successful native input.
Original helper uses unbounded strcpy/strcat/strncpy: a native capacity-checked
adapter must preserve accepted output and explicitly refuse oversized/unbound
inputs rather than truncate source text or pretend original bounds existed.

## Open and close wrapper ordering

winMgrOpen8023ebbc/3c resolves indexed68-byte manager; inactiveflag1clear
returns without writes. Active writes in exact order:
action+4=1,clock+8=0,page+38=0,linecount+3c=0,text+34=NULL.
It does not itself set transitionflag4 or call interpolation. Actual next
winMgrSeq invocation ORs4 and executes the requested open action.

The actual item-get close calls are **winMgrCloseAutoDelete8023eb50/3c**,
not plainwinMgrClose8023eb8c/30. Active AutoDelete writesaction2,clock0,
then ORsflags8. Inactive returns withoutwrites. PlainClose setsaction2/clock0
without8; it must not replace source itemget's automatic active-slot release.

Name/help definitions both sequencekind0. Reuse the source manager contract:
openORvisible2;rotation30->0,scale0->1; alpha255;clockincrements;completewhen
clock>N,whereN=trunc(signedGPword4*8/60). Completionsetsaction0 butretains4;
**following idle Seq clears4**. Name/head text draws waitwinMgrAction0;
this queryreturns(managerflags>>2)&1,not an inferred timer/textcomplete test.

winMgrMain walks active slotsascending,Seq beforedefinitionmain callback,
then displayregistration ifvisible2. Helpmain therefore sees freshlyupdated
transitionflag in that iteration. Name hasnullmain. Actual itemMain/WinMgrMain
outerfield scheduling/cadence remains its separate owner; avoid assuming both
run atomically inside one itemget.tick solely because a host harness can.

ItemseqGetItem substate3 waits only **namewindow** action0,then A/Btrigger
mask300. It does not separately waithelp/failurewindow action0. Uponack it
requests AutoDeleteClose name,thenhelp. It may immediately restore itemget
control/pose/camera and delete nameditem entries in this same owner call
(successstatusbit1); it does **not** waitcloseanimationcompletion first.
Provider-owned manager records/borrowedparameters must survive their own close
lifecycle without turning that original success into an extra input hold.

CloseSeq0 ends by clearingvisible2; when8set,alsoactive1. Transition4 remains
until a lateridle call if invoked. Since inactive slots ordinarilyskip manager
main,retained4 does not itself mean a deletedclose must keep blocking the
alreadycompleted itemgetevent. Never poll a deletedmanager to invent an extra
completiongate absent from itemseq's caller.

## Evidence and limits

Fresh tools/inspect_dol_function.py reads of unk_800d1364,winMgrOpen,
winMgrCloseAutoDelete,winMgrMain; original language/articletables and rawUS
messagepayloads rereadreadonlyISO. Ignored inspection under
artifacts/reference/goomba3-services. Existing manager contract supplies full
Seq0 reload/interpolation/order details instead of duplicating a different
clock implementation here.

Remaining: exactnativefont/USresource/manageradapter integration,sharedsource
outercadence,remaininglocalizedgrammars,capacityfailuremenu and originalGX/
PICA/hardwarevalidation. Original effect pass0positiveSDA2RGB producer remains
unclosed; no source evidence justifies a guessedcolor. No nativecode,shared
index/progress,emulatorprofiles,saves or distributableassetcopy changed.
