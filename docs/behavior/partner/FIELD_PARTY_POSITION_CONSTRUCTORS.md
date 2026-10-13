# Field party position constructors and ordered initialization

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
partyEntryMain0x800be144,size0x380;partyEntryPos0x800be84c,size0x21c;
partyEntry2Pos0x800bd6d8,size0x3d0;kuribo_init0x800c4fa0,size0x28.
This closes their local request graphs and writes. Called allocation,animation,
HP/color,record init and mode engines remain distinct dependencies.

Argument kind is an integer;position services also receive floating X,Y,Z.
The [previous-position callback](FIELD_PARTY_EVENT_CALLBACKS.md) uses integer
resolution/truncation before constructing those floating arguments. Constructors
receive already-resolved values and do not perform EVT operand resolution.

## Slot allocation and failure policy

partyEntryMain captures Mario pointer M, scans pointer-table entries0 then1
at0x8041ead8, and uses the first null entry. It calls __memAlloc(0,**0x188**),
publishes the result in that entry and requests memset(pointer,0,0x188).
Both occupied returns **-1** before construction. There is no allocation-null
test before memset and subsequent initialization. The chosen object is zeroed
again after table lookup;preserve the double-zeroing service order.

partyEntry2Pos captures M and tests **only table[1]**. Existing nonnull entry
returns -1 without replacing it. If free,allocate0x188,publish table[1],memset,
then look it up and memset again. It never chooses entry0. The same unguarded
allocation result domain applies.

partyEntryPos calls partyEntryMain(kind), then resolves its returned index:
negative becomes null, nonnegative reads the table. It **does not return early
on negative/null** before writing object fields. Thus its apparent index return
does not imply graceful failure when both slots are full. The original caller
must supply the expected free-slot/allocator domain. Do not report the service
as a safe replacement operation that destroys an existing partner.

## Common object initialization

For P in the chosen table slot i, the main and secondary constructors request
marioGetCamId and store it at **P+0x164**. They store -1 in three words
**+0x0c,+0x10,+0x14**, slot byte at **+0x2f**, owner M at **+0x160** and
kind low byte at **+0x31**. P+0 initially becomes1 in main,9 in secondary;
if i==1,OR4, giving5 or13 respectively. The constructors do not write
M+0x245+i themselves;the EVT previous-position wrapper stores their returned
index after a successful call.

Main checks signed **M+0x247** (not M+0x247+i) and, only if zero, copies
P's kind byte there. It also propagates M+0 bit0x80000000 into P+4
bit0x8000. Secondary performs neither this saved-kind write nor that Mario
flag propagation. Both publish kind record
**0x803143e8 + input-kind*0x28** at global0x8041ead4. This uses the full
input integer for the record index, while P+0x31 stores its low byte.
Out-of-range kinds are not validated.

## Animation setup and selected pose

Both constructors choose an animation resource before entering a pose:

| Condition | Resource pointer source |
| --- | --- |
| Kind byte!=4, P+4 mask0x8000 clear | Selected kind record+0 |
| Kind byte!=4, mask set | Fixed pointer0x802cb340 |
| Kind byte==4 | Pouch party-color result indexes pointers at r13-0x6c10 |
| Kind byte==4 and mask set | Override the color result with0x802cb340 |

The color query is requested before the override in the kind4 branch. Release
old pose P+0x0c if signed-nonnegative, then call animPoseEntry(resource,2),
store its result at+0x0c and call animPosePeraOff on it without a local
pose-entry failure check. Query marioAnimeId twice;each result selects a
word from M+0x22c+4*result. Copy that pose's material flags and material-light
flags onto the new party pose through their respective get/set-on services.
These are ordered resource requests,not verified rendering or asset conversion.

Pose selection begins with selector1, except P+4 bit2 retains the existing
pose-string pointer at+0x18. When bit2 is clear, signed kind<8 requests
partyGetHp(kind);returned HP<=5 changes selector to12. There is no explicit
kind>=0 gate. Choose the string pointer as follows:

| State | Pointer word address |
| --- | --- |
| P+4 bit0x8000 clear | 0x80314708 + signed-kind*0x3c + selector*4 |
| Bit set and kind!=4 | 0x80314bb8 + signed-kind*0x3c + selector*4 |
| Bit set and kind==4 | 0x80314d98 + party-color*0x3c + selector*4 |

If P+0x18 is null or strcmp(existing,selected)!=0, store selected at+0x18,
clear halfword+0x20 and OR0x2000 into word+8. Main and secondary include
a following second equality/null branch:when bit2 and bit0x8000 are both
clear,it can repeat those writes after their previous update. Preserve the
request/comparison ordering rather than reducing the body to one unconditional
pose assignment. Then clear word+0x1c and invoke the **current global record's
callback at+4** with P, without a null callback check. Reading the global
record at this point matters if called services changed it.

The pinned kind1 and8 records bind this init to kuribo_init. That callback
stores byte1 at P+0x33,byte0 at+0x32,float24 at+0xf0,float26 at+0xf4 and
clears the word at0x8041eae0 (r13+0x1bc0). It writes no Mario current-slot
byte and performs no HP/FP/pouch reset. Other kinds' init behavior is open.

## Position-specific tails

partyEntryMain returns the allocated index after the callback. partyEntryPos
then applies this local tail:

1. Copy vector0x802cb184 into P+0x70..0x78.
2. Store supplied X,Y,Z at **P+0x58,+0x5c,+0x60**.
3. Copy vector0x802cb19c into P+0x70..0x78,overwriting the earlier vector.
   The pinned final vector is **(1,1,1)**.
4. Load owner M from P+0x160;copy float **M+0x1ac** into **P+0x110**,then
   **P+0x10c**,with two original reads.
5. If P+4 bit2 is clear,run the same selector1/12 and string-table selection
   described above,with the single null/strcmp change test and pose writes.
6. Store mode byte **0** at P+0x34 and return the main constructor's index.

The secondary constructor has a different tail after the init callback:
call partyChgRunMode(P,**2**);store supplied X,Y,Z at+0x58..0x60;copy
them to **+0xb8,+0xbc,+0xc0**;store **1.0** separately at+0x70,+0x74,+0x78;
return index1. Its copy of an initial stack vector at0x802cb220 is overwritten
by supplied coordinates before the object position stores. It has no tail
copy from M+0x1ac like ordinary EntryPos. The final literal1 is not zero
velocity inferred from a vector name;its later interpretation belongs to
the party movement engine.

The lecture uses ordinary previous-position slot0,so its callback requests
partyEntryPos(saved kind,X,Y,Z),writes the returned index to Mario+0x245
and later calls run(0). That run callback only changes mode0 to1 after
current-index lookup. Whether global current index and owner-slot state agree
through the field/battle transition remains an independent invariant.

## Verification and remaining work

tools/verify_field_party_lifetime.py pins six whole body hashes,43 selected
instructions,three numeric constants,five data spans (kind1/8 records,their
normal pose rows,and final ordinary vector),and three actual field callback
bindings. Reports and original corrected Gekko listings remain ignored.
See [destruction](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md) for saved-kind
and clear-versus-kill lifetime behavior.

Open:all-kind record/animation tables and init callbacks;allocation/pose-driver
internals;HP/color readers and resource persistence;mode initialization and
global-index writers;owner-slot invariants,complete battle teardown and original
lesson transition traces. These local constructor graphs do not establish a
complete functioning party system or full-game equivalence.

## 2026-10-13 battle1f ordinary Goombella constructor composition

Read-only used partyEntry800bea68/1b8, partyHello800bbc48/194 and
partyHelloMain800bb544/704 listings in ignored artifacts/recovered-temp-evidence.
Existing Main/kuribo_init authority above and
[general appearance](FIELD_PARTNER_GENERAL_PLACEMENT.md) with
[fallback](FIELD_PARTNER_GENERAL_PLACEMENT_FALLBACK.md) already close their
local bodies; their native service binding remains separate.

OriginalmarioPartyEntry first requires pouchrow1flags&1, removes current
fieldMario245 registered actor, thenpartyEntry(1),storesreturnedindex245,
resolvespointer,requestsHello. Slot1Frankly staysindependent. Main's firstfree
slotdomain/allocator/publication and doublememset mustrespectthatactualregistry,
notreusebattleactorbytes. Main kind1 record80314410→string804213e0 `c_pkr`;
ordinaryselector1@80314748→802cb4dc `PKR_S_1`; HP<=5 selector12@80314774
→802cb52c `PKR_D_2`. ResourcebranchdependsactualMario80000000→P48000,
sooriginaldotappearance remainsseparate ifthatflagisset. NoHPheal/unlock here.

Main kuribo_init setsP f0=24,f4=26,33=1,32=0. Thusoriginalappearance
horizontalquerylength30+f4 is56forfreshGoombella, notinvented30. partyEntry
setsvector70=(1,1,1),38=10,angles110/10c fromowner1ac,requests
partyGetAppearPos2(P,&P58)800c6718,duplicatespositiontoPb8..c0,ordered
pose1/12queriesandsetsP34=1. Thiswrapperhasnounsafeconstructorfailure
repair: allocatedfreeslot/validowner/animation domain must alreadyhold.

SourcefreshP8doesnotmask8,soHello skipspendingentrysetupandcallsHelloMain.
Byte38=10 dispatchestable803141c4[10]→800bb9ec:requestsGetAppearPos2 AGAIN
intoP58,duplicatesXYZ toPb8..c0,originalpose1/12/null/strcmp update,then
setsP34=1 andreturns1. Byte38retains10. Do notlaunchsubstate0's16-count
jump/scale pathorbroadenotherHelloMainstates asusedfreshrecovery.

GetAppearPos2 complete584-bytebody startsownerM=P160; length=30+Pf4,
retainsfacingM1ac;writesM8c/90/94tooutput beforegating. SignedM246negative
selectssingle-slotray path; nonnegativeselectsdual-slotpath. Itisnotan
unconditionalfixed30-offset return. Wholelocalbodyoriginalray/trig/placement
providers stillneedscopedtypedbinding (andfullfunctioncontract ifnotelsewhere
closed). LatermarioFBattlePostinsteadusesGENERALappearance800c6c9c,whose
localprimary/fallback/admission contracts alreadyexist; withFranklyM246>=0
itsdualslotheading/attribute/downray policy applies. Don'tconfusethetwohelpers.

Nextboundedrequest: GetAppearPos2_800c6718 complete typedray/classification/
output/caller domains; actualfieldregistry/allocator/model/HelloMainfresh10
composition; sourcenumericcollision/provider mapping andretainedpausedfield
VM. Read-onlysource/nativeaudit; git diff --check; no native edits/build/commit.
