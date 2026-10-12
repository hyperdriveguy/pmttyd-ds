# Field party driver dispatch, local updates and presentation requests

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
driveParty at0x800bbe84,size0x6f0 and15-word dispatch table0x80314240.
All addresses are DOL virtual addresses. This specifies local control flow and
ordered service requests,not the internals of every called movement/collision/
effect/pose service or original gameplay execution.

The [partyMain contract](FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md) supplies
enabled-object and Mario-flag admission. This body has no null P check. Let
M=P+0x160,mode=unsigned byte P+0x34,kind=signed byte P+0x31. A referenced
kind record is0x803143e8+kind*0x28. Its pointer is published at0x8041ead4
before the indirect calls below. Called services may change state;subsequent
loads must retain their original positions in the request order.

## Prefix and early shadow path

Every invocation first copies float M+0x19c to P+0xf8. Read signed halfword
P+0x2a and decrement/store it only if positive. Zero and negative values remain
unchanged;this is not the same byte countdown as the
[global updater](FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md).

Modes4..8 and14 skip the following early branch. Other modes,including values
above14,check P+0 mask0x20000000. If set,call vivianGetStatus at0x801865b8.
If result!=2,request vivianShadow(P) at0x801863b8. In either result case,
request partyPreDisp(P) at0x800b7a98 and return directly. This path bypasses
normal mode dispatch,local scratch clears,effect/screen tests and the common
vector assignment. These are service bindings,not verified visual outcomes.

Otherwise mirror **M+0 mask0x02000000** into **P+0 mask0x00800000**:
set the latter if the former is set,clear it otherwise. Clear five words in
order **P+0x138,+0x12c,+0x140,+0x134,+0x130**. Reread unsigned mode
and dispatch0..14 through the pinned table;values>14 go directly to the
common tail.

## Exact mode dispatch

For rows marked G,skip their local handler when P+0 has mask0x20000000
set and mask0x10000000 clear;continue to the common tail. Otherwise proceed.
No such G check applies to modes4..8,10 or14.

| Mode | Target | Local request |
| --- | --- | --- |
| 0,G | 0x800bbf7c | Pending-change setup,then kind record+8 callback(P) |
| 1,G | 0x800bc0e4 | unk_800ca884(P),at0x800ca884 |
| 2,G | 0x800bc104 | Kind record+8 callback(P) |
| 3,G | 0x800bc168 | OR0x100 into P+0,then kind record+0x0c callback(P) |
| 4,5,6,7 | 0x800bc1b8 | Pending-change entry setup,then partyHelloMain(P),at0x800bb544 |
| 8 | 0x800bc330 | Kind record+0x14 callback(P) |
| 9,G | 0x800bc148 | partyForceWalk(P),at0x800c92f4 |
| 10 | 0x800bc360 | Pending-change calls partyDokanInit;always partyDokanMain |
| 11,G | 0x800bc394 | Pending-change slit cleanup;always partyRideMain(P) |
| 12,G | 0x800bc3e4 | Pending-change local clear;always partyRideYoshiMain(P) |
| 13,G | 0x800bc42c | party_damage_return(P),at0x800b8128 |
| 14 | 0x800bc44c | Inline saved-kind/slot/pointer removal,pose release/free;return |

Indirect callbacks at+8,+0x0c,+0x14 are **not null-guarded** here. Kind1
and8 records bind+8 to partyMoveWalk,+0x0c to kuribo_use and+0x14 to
kuribo_bye. The [ordinary walk callback](FIELD_PARTNER_WALK_CALLBACK.md) and
[mode1 substate dispatcher](FIELD_PARTNER_MODE1_SUBSTATES.md) now specify their
local graphs;called final movement/ability bodies remain dependencies.
In particular,field stop changes mode to0 but **mode0 still calls record+8**;
do not implement it as an unconditional skip of all party work.

## Pending-change setup

The pending-change marker is word P+8 **mask8**,written by
[partyChgRunMode](FIELD_PARTY_MODE_INITIALIZATION.md). Consuming it clears
only that mask. The marker is tested only in the listed handler branches;
other modes may leave it intact. The following setup is conditional on it.

Mode0 consumes mask8,then,if P+4 mask2 is clear,selects the ordinary
selector1/12 pose using the [constructor tables](FIELD_PARTY_POSITION_CONSTRUCTORS.md).
Null existing string or strcmp inequality updates+0x18,clears halfword+0x20
and ORs0x2000 into word+8. Next write float0 to P+0x104,clear mask0x100
in P+0 and request partyChgMot(P,0),at0x800cb584. Then,with or without
pending setup,publish the kind record and invoke its+8 callback.

Modes4..7 consume mask8 and inspect owner unsigned halfword **M+0x2e**.
If it equals22,24 or25,write byte30 to P+0x38 and skip the vector/angle/
position selection below. Otherwise reread P mode:

| Reread mode | Vector→P+0x70..0x78 | Byte P+0x38 | Other writes |
| --- | --- | --- | --- |
| 4 or5 |0x802cb250=(0,0,0)|0| OR0x10 into P+4;copy M+0x1ac twice to P+0x110 then+0x10c;copy M+0x8c..0x94 to P+0x58..0x60,then duplicate to+0xb8..0xc0 |
| 7 |0x802cb25c=(1,1,1)|20| Copy M+0x1ac twice to P+0x110 then+0x10c;do not copy position here |
| Others,including6 |0x802cb268=(1,1,1)|10| Same angle copies and owner-position/duplicate copies as4/5,without their OR0x10 |

All pending branches then clear P+0 mask0x100 before requesting
partyHelloMain(P). The byte literals0/10/20/30 are local writes,not measured
durations or proof of the called entry engine's time units.

Mode10 consumes mask8,requests partyDokanInit(P) at0x800ba56c and then
clears P+0 mask0x100. With or without pending setup,request partyDokanMain(P)
at0x800ba05c. The flag clear occurs after the init service.

Mode11's pending setup consumes mask8,clears byte+0x38,clears P+0
mask0x100 and calls L_partyForceSlitOff(P),then requests partyRideMain(P)
at0x800b8d38. Mode12 performs the same local clears without slit cleanup,
then requests partyRideYoshiMain(P) at0x800b865c. Their G gate can skip the
entire local handler,including pending consumption.

## Mode14 destruction differs from partyKill

After optionally consuming pending mask8,mode14 sign-extends P+0x2f as
slot i. Using the owner M captured in the prefix,it:

1. Copies raw P+0x31 kind into **M+0x247+i**.
2. Stores0xff into **M+0x245+i**.
3. Stores null in **table[i]** at0x8041ead8+4*i,without a slot bound check.
4. Requests animPoseRelease(P+0x0c),then __memFree(0,P).
5. Returns directly;there is no effect/screen/common presentation tail afterward.

This route captures P's own kind,unlike
[partyKill's post-hook linked-kind lookup](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md).
It performs no kind exit-hook request and no global record publication. Do not
replace it with partyKill or continue using the freed P in the common tail.

## Common tail and screen flag

All normal handlers,including G-skipped handlers and mode>14,request
party_kemuri(P) at0x80134f38. Copy current P+0x58..0x60 into a stack vector,
then call marioGetScreenPos(vector,&screenX,&screenY,&depth) at0x80058960.
Convert screen X then Y with **fctiwz**,and request
marioChkInScreen(integerX,integerY) at0x80058928. The depth output is not
used by this local tail. The position copy occurs after the effect service.

Nonzero screen-check result clears **P+4 mask0x00800000**;zero sets it.
This screen flag lives in **P+4**,separate from the prefix's mirrored flag in
**P+0**. Copy vector0x802cb244=(0,0,0) into P+0xac,+0xb0,+0xb4,and
request partyPreDisp(P). Original projection,screen acceptance,effect emission
and presentation/rendering remain dependencies;the driver makes no local EVT
stop or HP/FP export request.

## Verification and remaining work

tools/verify_field_party_drive.py checks four whole DOL bodies,58 selected
instructions,one constant,four data spans (15 explicit dispatch words,kind1/8
records,four vectors) and two actual lecture stop/run imports. The full body
hash pins all local bytes but is not proof of called service semantics or
original execution. Metadata reports and corrected Gekko listings stay ignored.

Next:kind callbacks and mode1/entry/ride/pipe/force-walk/damage bodies;
effect,projection,presentation and pose-driver contracts;callers,flag/slot/index
writers,complete field/battle teardown and original stop/run/transition traces.
These dependencies still prevent complete party-system or full-game equivalence.
