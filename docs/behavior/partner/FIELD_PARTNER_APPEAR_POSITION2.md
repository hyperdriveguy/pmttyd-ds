# Original partner appearance position2

2026-10-13 US read-only complete partyGetAppearPos2_800c6718/584. Inbox
request was native Goombella ending constructor placement; listing retained
ignored `artifacts/recovered-temp-evidence/return-appear2.dis`. No native edits,
shared PROGRESS/index edits or commit. This closes local query/write/order
policy, not source ray/trig/backend execution or full field constructors.

## Entry, outputs and precise service domains

Inputs P=partner pointer and V=output XYZ pointer. Capture M=P160. Retain
L=single(30+Pf4). Request revise360(single(180+CURRENTM1ac)) at800c6770;
then load M1ac AGAIN into retained F. The initial revise360 result is not used
as a cached heading by later branches, but its request/order remains observable.
Write current M8c,M90,M94 to V in XYZ load/store order before slot dispatch.
There is no pointer/null/range check, alias snapshot or initial return guard.

Only sign-extended M246 chooses single versus dual path. It does not test
M245, partner kind, current HP, Pword4 inhibition, floor attributes or Mario
motion mode. Fresh kind1 kuribo_init writes Pf4=26, hence L56.
Don't reuse general appearance's additional gates merely because its helper
has a similar name; [general placement](FIELD_PARTNER_GENERAL_PLACEMENT.md)
is a DIFFERENT function called later by marioFBattlePost.

## Ray ABI and classification

Every query calls hitCheckVecFilter800158a4(Q,null callback). Common Q at
frame+7c: origin+0c,direction+18,point+24,normal+30,length+3c. Horizontal
origin is (current sourceX, single(sourceY+37), current sourceZ), direction
(sin,0,-cos), lengthL. Each direction requests source heading conversion then
sincosf80005e8c. Down origin is (current Vx,single(Vy+37),Vz), direction
(0,-1,0), length38. Queries use copied source templates and ordered live reads.

**Any nonnull downward hit succeeds**: load raw Q.pointY into V.y, return1.
No hitGetAttr,0x200/800 exclusion,map-object-name scan,100-length admission,
slope/normal classifier, quantization, radius correction or minimumY exists
in this body. Null horizontal means passage clear; nonnull means blocked.
Floor-hit kind/content does not select a separate local reaction here.

## Complete single-slot path (signed M246<0)

1. Reload current M1ac; request toMovedir(single(180+value))800963fc,
   sincosf. Horizontal origin uses CURRENT V (seeded above, possibly aliased),
   lengthL. Nonnull hit immediately returns0; no secondary attempt.
2. Null hit: reload current M1ac AGAIN, request toMovedir(value); request
   movePos80005de4(Vx,Vz,distance-30,returnedAngle). This updates Vx/Vz.
3. Down query using current moved V, length38. Nonnull writes rawY/return1;
   null returns0. Failure retains moved X/Z and prior Y; no reseed/rollback.

## Complete dual-slot path (signed M246>=0)

First attempt:

1. Reseed V from current M XYZ. Request toMovedir2(F,-90)80095ff8,
   sincosf; horizontal origin uses CURRENT M XYZ withY+37, lengthL.
2. If horizontal MISS, reseed V from M XYZ again. Sign-extend P2f:
   slot==0→toMovedir2(F,+20), otherwise→toMovedir2(F,-20).
   movePos(Vx,Vz,-30,returnedAngle), then down38. Nonnull floor writes rawY/
   returns1; null returns0. **Failed down does not try the second side.**
3. Only a NONNULL first horizontal hit admits second attempt below.

Second attempt:

1. Request toMovedir2(F,+90), sincosf; horizontal origin uses current M XYZ,
   Y+37,lengthL. It does not universally reseed output before this horizontal
   query. Nonnull hit returns0 with whatever current V remains.
2. Horizontal MISS: reseed V from current M XYZ. Slot==0→toMovedir2(F,-20),
   otherwise→toMovedir2(F,+20); movePos(Vx,Vz,-30,angle), down38.
   Nonnull writes rawpointY/return1; null returns0 with moved output retained.

Maximum local queries:2(single or first dual miss) or3(first dual horizontal
hit, second miss, down). No surrounding radial search/NPC fallback. Retained
F versus later current M headings and output reseeds are deliberate source
capture boundaries; service mutation/alias domains must preserve them.
toMovedir/toMovedir2 consult their original global camera/facing state;
see [heading/movement/trig services](../field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md).
An explicitly declared host libm/raycast adapter is separate from this source
query policy, not permission to invent a fixed partner offset or floorY.

## Source constants/templates and hashes

Constants:180=804215ac,37=804215b0,30=804215c0,38=804215d0,
-90=804215d4,-20=804215d8,+20=8042159c,+90=80421598.
Six origin/direction templates at802cc34c/364/37c/394/3ac/3c4; horizontal
origin/direction starts zero then dynamic X/Y/Z/sin/cos overwrite; each down
template contains direction(0,-1,0). These are scalar templates, not hit results.

| Evidence | SHA256 |
| --- | --- |
| Complete800c6718/584 | 8943e743013b74f15bb9414739b8a38481057b2db5440c4955356446343a9151 |
| Constant span804215ac/30 | 65ceaaa12509b9a51fecb5379e64ebb31887a19b4fe49a436efc202b645fb636 |
| Six templates802cc34c/90 | bc92ecc71be9e17f65340bea1ede6ea7a30d66d40a8435b36295ee1b127fc769 |

## Direct Mario slot initialization and kind1/8 records

marioInit8005b5c8 publishes/zeros2f8 bytes, then at8005b718 writes low8(-1)
to M245, at8005b724 low8(-1) toM246. **245=246=ff/signed-1** on that direct
constructor path. No direct247/248 store exists in this body after its memset;
those bytes are0 at zeroing point. Called engines/first party constructor may
write saved kinds later (Main writes247 if it was0); do not reset them at
battle ending. Direct initializer span8005b70c/1c hash
89f020563890a1b489ef58fc1164b514bfc6b6eb820846351067b2deb458c0da.

Kind table803143e8 has40-byte records; words below are exact source data:

| Offset | Kind1 row80314410 | Kind8 row80314528 |
| --- | --- | --- |
| 0 model | 804213e0 `c_pkr` | 802cb47c `c_babyyoshi` |
| 4 init | 800c4fa0 kuribo_init | same |
| 8 ordinary update | 800ca134 partyMoveWalk | same |
| c use | 800c4bdc kuribo_use | same |
| 10 use-post | 800c4f84 kuribo_use_post | 0 |
| 14 bye | 800c4b80 kuribo_bye | same |
| 18/1c/20/24 | all0 | all0 |

Kind8 is not Frankly's kind11 and does not use c_kuri_h. Shared kuribo_init
does not imply identical model/art or party-unlock eligibility. Record hashes:
kind1 66223e5e99847e374a78c965305df7da505db3794f40503a107dbd8c5e2e0e3c;
kind8 0736c199924450fd0f59d4b42682e20fdd5086e0d8c6920f37fc3a4195aebb9d.
[Constructor contract](FIELD_PARTY_POSITION_CONSTRUCTORS.md) owns registry,
model/pose/material/callback order; this helper doesn't create/remove actors.

Validation: whole local listing, every query branch/return/output path,
constants/templates/direct slot stores and original records/hash extraction;
`git diff --check`. Documentation-only, no native code/tests/build/shared
PROGRESS/index edits or commit. Remaining: typed native ray/trig providers,
collision filtering/backend/current field context/aliases and original executed
numeric/trajectory comparisons; local appearance2 policy is now closed.

Fresh registry follow-up: MarioInit8005ba30 calls partyInit800bd6ac/2c,
whose only mutation is memset(8041ead8=r13+1bb8,0,8). Both pointer slots0/1
are zero; no actor allocation/free, saved/current-kind mutation or pouch grant.
Full partyInit SHA256 1de59625edee5097ee19b46d0c8e22fcf80c6e62fe1c9ac3ad0f621092ece667. Native empty registry/current
indices-1 are source-backed initialization, not permission to claim later
Goombella joining/constructor execution.
