# Weapon-menu target count and eligibility update

Complete static US G8ME01 revision0 local helpers:
`_btlcmd_CheckWeaponTargetNum`80123d00/0xb0 and
`_btlcmd_UpdateSelectWeaponTable`80123db0/0x110.
[Verifier](../../../tools/verify_weapon_menu_main.py) pins these full bodies
alongside [Main](WEAPON_MENU_MAIN_CONFIRMATION_AND_LIGHT_REQUESTS.md)/End.
[Class Main](ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md) calls the same
count helper,whereas [selection decision](WEAPON_MENU_SETUP_AND_DECISION.md)
uses the live battle target buffer. No original runtime trace is claimed.

## Count helper: guards,cost,and private sampling buffer

Arguments B,U,V are retained. V is weapon pointer. If V==0,return0 without
reading weapon fields or issuing services. Otherwise read WORD(V+0xb0);if
zero,return0 without a cost check or sampling. There is no U-null guard here.

Request BtlUnit_CheckWeaponCost(U,V). Return0 from this service makes the
helper return-1 immediately. Any nonzero value admits sampling;do not check
for exactly1. The already recovered [cost contract](BATTLE_WEAPON_COST_AND_PAYMENT.md)
explains its own scalar/resource policy;this slice preserves the call boundary.

Request BtlUnit_GetEnemyBelong(U). Retain its RAW word return E for both
index arithmetic and later argument conversion. In order,read:

1. WORD(U+0) as actorId.
2. WORD(V+0x64) as admission mask A.
3. WORD(V+0x68) as attribute mask C.
4. U8(B+wrap32(E*8)+0x0a) as directionByte.

The side-dependent address uses full raw E shifted left3,NOT S8(E).
Pass BattleSamplingEnemy(privateSP08,V,actorId,S8(E),A,C,
S8(wrap32(-directionByte))). The original unsigned byte is NEGATED before
signed-byte argument conversion. Do not substitute the unnegated direction
used by some other sampling callers or a selected battle-buffer direction.

Ignore sampling's scalar return. Read signed byte at SP+0xa74,which is
private buffer+0xa6c candidate count,and return it sign-extended. No extra
selected-byte reset or target choice occurs here. The count can be negative;
there is no normalization to zero. [Complete sampler](BATTLE_ENEMY_CANDIDATE_SAMPLING.md)
and [choice](BATTLE_ENEMY_CANDIDATE_CHOICE.md) already document underlying
local bodies,including source/platform/geometry boundaries. The private count
helper does not write the live B+0x428 target buffer in its own stores.

The stack frame is0xaf0 bytes;private buffer begins+8,while preserved registers
are near its tail. Preserve buffer ownership and typed fields when adapting
the caller,without treating literal host stack addresses as game data.

## Update helper: captured count and fresh row-pointer read

Arguments B and T are retained. Load WORD(B+0x420),request
BattleGetUnitPtr(B,actor),retain U;retain M=B+0x171c. Request
GetCursorPtr(M,T,&its OWN privateSP08);ignore return. Read cursor from this
slot and capture signed WORD(cursor+8) as N ONCE. Unlike class Main's
validation scan,this helper never rereads cursor count inside the loop.
Unsupported types leave this private output unwritten;Main's different slot
or Input's initialized slot cannot supply a fallback.

For signed J=0 while J<N,derive Q=wrap32(M+28*J). In order:

1. Read WORD(Q+0x80) as retained weapon V.
2. Store WORD(Q+0x84)=0 (disabled).
3. Store WORD(Q+0x98)=0 (reason).
4. If V==0,skip services and keep both cleared fields.
5. Otherwise request CheckWeaponTargetNum(B,retainedU,V),then classify its
   SIGNED return using the table below.

| Count-helper result | Retained T | Final enabled / reason stores |
| --- | --- | --- |
| >0 | any | enabled1,then reason0 |
| ==0 | 0 | enabled remains0;reason4 |
| ==0 | 1 | enabled remains0;reason5 |
| ==0 | all other values | enabled remains0;reason3 |
| <0 | 4 | enabled remains0;reason2 |
| <0 | all other values | enabled remains0;reason1 |

The negative branch accepts any negative result,not just-1:the sampler's
negative count also reaches it. Reason1/2 therefore cannot be labelled solely
“cost failure” based on this caller. Old reasons are cleared for EVERY row
before evaluation;this differs from class Main's preservation of nonzero reason.
A positive result writes both fields AFTER the service,overriding service-side
changes to them. Other branches write only reason after the initial clear.

Increment J by1 and byte offset by28. Captured count N controls all iterations,
even if services modify current cursor/count. Row Q,weapon V,U,and T are
retained across count services;later rows read their own fresh weapon pointers.
No descriptor-wide clear,sort,index correction,type write,or count publication
appears in this body. Other descriptor bytes and unvisited rows are preserved.
There is no explicit scalar result assignment after the loop;do not invent an
update count or success code. Main ignores this return.

## Descriptor and source requirements

Descriptor start is M+0x80+28*J:weapon at+0,enabled+4,reason+24. Fields at
+8..+23 are untouched by this update. Preserve null-weapon row clearing,
V+b0 guard before cost,negative count signedness,captured iteration count,
negated side-byte conversion,and private versus live sample buffers.

Full `_btlcmd_MakeSelectWeaponTable` production,source ItemData/weapon/menu
provisioning,remaining writers and reachable query-event invariants are still
needed. These local eligibility contracts make port implementation concrete
without supplying fabricated default weapons or proving global cursor safety.
