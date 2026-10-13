# Rogueport plaza save-block source binding

Read-only US G8ME01 disc0 revision0 static evidence, partial follow-on to
[ordinary NPC source catalog](ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[SAVE-01](../../requests/SAVE-01.md). No original-runtime observation or
native card-format equivalence is claimed.

## Room spawn and assets

`gor_01_init_evt` begins by setting local0xfbd3e280=0 at REL0x3c200.
Only story==341 sets Mario mode2 and this local=1 (0x3c20c..230).
At0x3c508 IF_NOT_EQUAL(local,1) admits0x3c514:
`evt_mobj_save_blk("mobj_save",185,60,-55,0,0)`. The coordinates are
original map units, not an authored native placement. Its model resources are
original `a/MOBJ_SaveBlock` and texture companion `a/MOBJ_SaveBlock-`.
Do not add a native A-button proximity rule as original save-block logic.

DOL `evt_mobj_save_blk`0x80066dd4 resolves name with evtGetValue,
XYZ with evtGetFloat, operand4 with evtGetValue, but retains operand5 raw.
It suppresses entry when GP word0 mask0x20 is set (returns2). It requests
`animGroupBaseAsync("MOBJ_SaveBlock",GP+20!=0,0)`; not-ready returns0
for retry. On readiness it enters the named map object, sets source position,
and writes update callback0x80066f58 atobject+0x1d0, resolved optional event
at+0x1d4, system event0x8030b4ec at+0x1d8, raw operand5 at+0x1e4,
state0 at+0x1dc, flag masks0x84 and0x400000, then computes its matrix and
requests original poseS_1 with restart1. Pose literal0x8042021c is read from
r2 base0x80427440 plus-29220; it is case-sensitive.

## Original display scale and idle ownership

`mobjEntry`0x80060b9c initializes object scaleXYZ+0x44/+0x48/+0x4c
from DOL0x802c1f7c/80/84=(1,1,1), rotations+0x50/+0x54/+0x58
from0x802c1f88/8c/90=(0,0,0). The save-block constructor makes no scale
or rotation override. `mobjDisp`0x80061524 calls `animPoseDraw`0x8003c450
with XYZ, rotationY+0x54 and **10*scaleX+0x44**. Float10 comes from
DOL0x804201bc, r2-relative-29316. The wrapper makes a translation matrix
and forwards rotation/scalar into `animPoseDrawMtx`; therefore the source
model draw scalar is10, not NPC scalar2 or an assumed scalar1. Native unit
conversion must preserve this distinction per entity kind/model.

Constructor restartsS_1 once. The update callback only replaces/restores its
pose following hits; ordinary idle does not restart every simulation tick.
Original animation Main owns clip sampling/local clock thereafter; exact field
cadence and runtime/backend binding remain open. Converted local geometry
uses the same original a/ animation bank format as NPCs, but its source draw
scalar is independently recovered here.

## Hit/update and original save event

`mobj_save_blk`0x80066f58 dispatches object+0x1dc states0..4. State0
waits for object flags mask8; this is a hit-state flag, whose collision owner
must be recovered before native activation. It locks services, requests
SFX numeric431 at object position and changes the animation, sets camera8
flag0x200 and advances state1. State1 waits on the original animation numeric
query, restoresS_1, runs optional+0x1d4 event when nonnull and waits in2;
otherwise proceeds3. State3 starts system+0x1d8 event and4 waits until its
ID is no longer running. Completion clears camera0x200 and object flag8,
unlocks services and resets state0. Exact numeric-animation query, optional
callback scheduling, collision, camera locks and sound macro remain owners
outside this scoped spawn record.

Original system event `mobj_save_blk_sysevt`0x8030b4ec has16 commands:
priority opcode99(0); Mario keysOFF; map-local0xfbd3e28a=0;
IF(flag0xf8405c69==0),set that flag1,print `save_first_hit` with
`evt_msg_print(0,ID,0,0)`,set map-local0xfbd3e28a=1;END_IF;
IF(story>=7),setflag0xf8405c6c=1;END_IF;
setlocal0xfe363c80=0;RUN_CHILD `unk_evt_803bac3c`;Mario keysON;
RETURN;END. The resource key is ISO-read at0x802c2280; preserve original
US payload/control tokens and first-hit switch lifetime. The common save event
and original card services still require binding; a native manual menu save
alone does not implement this original object interaction.

## Validation and limitations

Fresh `rel_event_inventory.py` room init extraction and
`dol_event_inventory.py mobj_save_blk_sysevt` read-only extraction produce
system-event SHA256a580e9894d07ae58b8cb24d35b825df91eb9b3d42e003f84cd4e5a15dcaa3752.
Fresh `inspect_dol_function.py` inspections of constructor/update and direct
ISO model/pose/message-key reads verify these local requests. Assets and
inspection outputs remain ignored or /tmp, outside source history. Original
Nintendo/Intelligent Systems rights remain; no asset redistribution is implied.
Docs-only: no make test/build claim. Full contact/hit producer, material/lighting/
shadow/update numerics/audio, common save window and card persistence are open.
