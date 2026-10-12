# Complete unit-data child event continuation and Mario key7 binding

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete btlevtcmd_RunDataEventChild8010a010/a0;original Jump call80355e18,
Mario kind/table binding and complete tiny selected script80416bd0/8 plus
its four original trailing label-scan bytes.
Verify:`python3 tools/verify_battle_data_child.py`:ten complete/linked body
identities,109 instructions,five source spans. Read
[actor translation](BATTLE_ACTOR_SELECTOR_TRANSLATION.md),
[borrowed data lookup](BATTLE_WEAPON_ATTACK_CLASSIFICATION.md),
[child construction](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) and
[Jump root/owner binding](NORMAL_JUMP_ROOT_REGISTRATION.md).
Static evidence only;no original Jump/child runtime trace.

## Full callback resolution and capture order

Input is parent event E;the second callback firstCall argument is NOT read.
This callback does not test E.markerd,store a polling ID or return0/1. On every
invocation retain E and capture A=word(E18),then perform:

1. Read RAW word A0 and resolve through evtGetValue(E,raw). Pass its result
   to BattleTransID(E,resolved);retain translated word I.
2. AFTER translation read CURRENT global battle pointer B and call
   BattleGetUnitPtr(B,I);retain returned unit U. There is no U-null guard.
3. Only NOW read RAW key word A+4 from retained A and resolve it through
   evtGetValue(E,rawKey). Argument1 is not snapshotted with argument0.
4. Call BtlUnit_GetData(retained U,resolved key). That helper reads U18 at this
   point,so a key resolver's alias mutation can affect the selected table.

GetValue can independently recapture context for each call. BattleTransID may
capture a different global battle than step2. Retained A need not equal a later
E18,retained U need not come from the context supplying key work banks,and no
pointer consistency validation exists. Unit lookup failure is not the same as
a missing data key:the later GetData dereferences U before testing its table.

## Zero pointer versus actual child creation

If GetData returns word0,return handler2 immediately. No ChildEntry,E.opcode
clear,actor copy,ID query,wait flag or fallback event is requested by this
branch. Missing table/key and found key with value0 share this branch. The
USER_FUNC dispatcher's common handler2 clears opcode and continues;that is a
later operation,not a store inside this callback.

For any NONZERO returned word script,call evtChildEntry(E,script,flags0).
The constructor itself installs E.child70 and E.flags8|10 before child clear,
then initializes inherited fields,labels,append/run-check and ID policy.
After it returns C,store E.opcodea=0 at8010a090,then store RETAINED translated
I to C.actor160 at8010a094. This overwrites the actor copied from E inside the
constructor. It is I,not a fresh unit U0 read or the unit kind. There is no
C-null/allocation/type/script validation before the actor store. Return255.

Handler255 causes dispatcher return-1 and ends the parent's remaining-credit
loop,without deleting that parent. E.next14 was already advanced by FETCH;
parent waiting-child mask10 prevents later eligibility until child deletion
clears it. No synchronous recursion into script,root registration,U2a8 update,
wait-event ID replacement,cost/damage request or native promise result occurs.

The child priority is low8(E.priority+1),type/local/flag/metadata/user-address
inheritance follows the constructor;speed captures current GLOBAL evtSpd and
credit/lifetime initialize0. Active manager appends this child unconditionally
(no root flags20 gate),at current table tail without sorting. It can be visited
later in the SAME pass. “Parent waits for child” does not imply a fixed frame
or tick delay:table position,run-check,context and credits control execution.

## Concrete ordinary Jump caller and Mario source table

Jump command80355e18 is opcode91 with callback8010a010 and raw operands(-2,7).
Its normal/non-first-attack branch reaches this AFTER PayWeaponCost and BEFORE
explicit child80363388 (MajinaiPowerUpCheck). These are TWO child requests with
different source and ownership. The first-attack branch bypasses this sequence;
see [full Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md).

For ordinary retained owner I,unit construction copies kind.c0→U18 in
[complete Entry](BATTLE_UNIT_ENTRY_COMPLETE.md). Original Mario kind
unitdata_Mario80372afc has word+c0 at80372bbc equal803729c8,data_table_mario.
The complete0x50 table is ten8-byte records in this actual order:

| Record address | Key | Value / original annotation |
| --- | --- | --- |
| 803729c8 | 5 | 80373304,mario_appeal_event |
| 803729d0 | 6 | 8037337c,attack_audience_event |
| 803729d8 | 48 | 80416b38,btldefaultevt_Dummy |
| 803729e0 | 51 | 803730fc,mario_emergency_revival_event |
| 803729e8 | 49 | 80372ff0,mario_dead_event |
| 803729f0 | 64 | 80417c50,mario_win_reaction |
| 803729f8 | 7 | 80416bd0,subset_attack_ready_jump_event |
| 80372a00 | 65 | 80372ce4,mario_ride_btl_entry_event |
| 80372a08 | 47 | 8034f4cc,btldefaultevt_avoid_counter_event |
| 80372a10 | 0 | 0,terminator |

GetData returns first matching nonzero key's VALUE,including0;key0 stops before
comparison. The table is borrowed;no copy/free occurs. This source chain selects
80416bd0 only when actual unit kind/table pointer still follows these values.
Later U18/table mutation,other unit data keys,duplicate keys and source reachability
remain open. Do not assign this table to every semantic kind222 without proving
which original kind pointer and later overrides were imported.

## Tiny child still has lifecycle effects and source-read requirements

subset_attack_ready_jump_event at80416bd0,size8,SHA256
1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61
contains DELETE opcode2 at80416bd0 then terminal opcode1 at80416bd4. There is
no USER_FUNC,wait,pose,motion,cost,status or damage command. Once dispatched,
opcode2 calls evtDelete(C) and returns terminal255;the dispatcher does not
execute the following opcode1. The constructor label scanner DOES traverse
opcode2 and stops at opcode1,reading terminal's first operand at80416bd8
unconditionally. That trailing word belongs to the next original symbol.
Preserve the original bytes for this scanner access;do not append invented0.

Deletion clears parent's waiting flag/link and copies child's locals/flags and
message metadata back before clearing child432 bytes and decrementing allocation
count. ID allocation/increment,child priority/speed initialization,append/run-
check,parent marker/opcode continuation and copy-back therefore still happen.
Replacing this callback with handler2 because the child “does nothing” skips
those original lifecycle effects and can alter same-pass scheduling.

Current tools/pack_event_scripts.py selects Jump,power-check and badge-tail.
That pack does NOT include80416bd0 plus its trailing scanner word. The native
source owner needs this additional original-address span and actual kind/table
binding,or an explicitly unbound source state. The existing72-callback census
needs no new USER_FUNC address for this tiny child,but source/VM coverage DOES
expand. Source extraction stays ignored/read-only;ID2 changes no asset packer.

## Implementation acceptance and remaining scope

Check actor then live key query order,context/global recapture and retained A/U/I;
missing/null table/found-zero value handler2 with no callback-side opcode write;
nonzero script ChildEntry flags0,post-return parent opcode clear BEFORE actor
write,translated actor override versus inherited owner,handler255 parent survival,
same-pass append and constructor global speed;Mario c0/table/key7 actual pointer;
both tiny opcodes plus trailing read;full deletion copy-back and next-command
resumption;and existing first-attack bypass/two distinct ordinary child requests.

This closes the local service and static Mario key7 source. Actual kind/table/
actor/menu/scene writers,other dynamic values,packed address/heap/numeric/clock/
callback binding,full script domains and original execution remain EVT-01/JUMP-01.


## Reproducible additional source-span extraction

Read-only reproduction using the existing packer,with output kept outside source
history (all four names are required because explicit names replace defaults):

```sh
python3 tools/pack_event_scripts.py --output /tmp/jump-with-key7.pmevt \
  marioAttackEvent_NormalJump marioAttackEvent_MajinaiPowerUpCheck \
  btldefaultevt_SuitoruBadgeEffect subset_attack_ready_jump_event
```

ID2 ran this to /tmp/id2-datachild-jump-with-key7.pmevt;the companion JSON retains
original source fingerprints/addresses and each symbol's four trailing bytes.
This is source packaging evidence,not native dispatch/callback execution. The
repository's default selection and native source table binding remain unchanged.
