# Complete weapon-menu setup and decision requests

US G8ME01 revision0; ID2,2026-10-12. Native EVT-01/JUMP-01 source-domain request.
Complete BattleCommandDisplay_WeaponSelectMenuSetup80122068/6c and
_btlcmd_SelectWeaponDecide80123210/150. Verify
`python3 tools/verify_weapon_menu_domains.py`;read
[type writers/cursor reuse](WEAPON_MENU_TYPE_DOMAINS_AND_CURSOR_REUSE.md),
[selected weapon](BATTLE_SELECTED_TARGET_AND_WEAPON.md),
[candidate sampling](BATTLE_ENEMY_CANDIDATE_SAMPLING.md).
Full table construction/UI/input/source reachability/runtime remain separate.

## Setup retains B,T,F and its menu address before calls

Arguments B(r3),T(r4),F(r5). Retain all;compute M=B+171c with word wrap. Call
_btlcmd_MakeSelectWeaponTable(B,T),ignore return. Then call
_btlcmd_GetCursorPtr(retained M,T,&stackCursor). The stack word at newSP+08 is
NOT initialized before this cursor call;ignore helper return. Read that word
and call BattleMenuDisp_WeaponSelect_Init(B,cursor,M+80,F),ignore its return.

Finally read CURRENT WORD M04,OR01000010hex,store M04. No menu-type540 store,
T validation,cursor null/output-written check,entry count check or global battle
recapture occurs in this LOCAL body. Called table/UI services have separate
mutations/providers. Unsupported T can leave this setup's private cursor word
unwritten;do not transplant BattleCommandInput's initialized-slot reuse here.
No new scalar status/result2 is assigned on exit.

## Decision captures acting unit before rebuilding the table

Arguments B(r3),T(r4),INDEX(r5) retained. BUF=B+428. Read current B420 as actor
ID;call BattleGetUnitPtr(B,ID),retain U. Then call
_btlcmd_MakeSelectWeaponTable(B,T),ignore return. Compute word-wrap28*INDEX;
read P=WORD(B+179c+28*INDEX). This equals M+80 weapon pointer in that rebuilt row.
No index/count/null U/null weapon validation or requested cursor lookup here.

Call BtlUnit_GetEnemyBelong(retained U). Then read U0c byte;sign-extend returned
enemy-belong to word,read CURRENT B420 as actor ID,read P64 and P68 masks,
use S8(U0c)*8 to address B+0a+thatOffset and read/sign-extend its byte.
Request BattleSamplingEnemy(BUF,P,currentActor,enemyBelong,mask64,mask68,lastByte).
The initial retained U may differ from the actor now named by B420. Sampling
return is ignored;its full candidate/status/geometry/RNG source policies are
separately documented. No current-global B recapture occurs in this decision.

After sampler,store BYTE BUF.ab7=0 before reading CURRENT P68. If mask30000000
is0,skip preference scanning and return0. This default selection reset is a
store after sampling,not a presumed initial value or part of the sampler call.

## Optional selection uses signed belong/permutation/count reads

For nonzero mask30000000,call BtlUnit_GetBelong(retained U),sign-extend low8
result as OWNER_SIDE. Initialize chosen=-1,index J=0. Each condition rereads
S8(BUF.a6c) and compares signed J<count. No count snapshot/clamp. At each J,
read S8(BUF.a6d+J) as row permutation R;row=BUF+4+36*R with word wrap. Read
signed halfword row00 as candidate ID,fetch BattleGetUnitPtr(B,ID),then call
BtlUnit_GetBelong and sign-extend its low8. No candidate-null or kind check.

The flag branch rereads CURRENT P68 for EACH row:

| P68 mask10000000 | Admission | Selection order |
| --- | --- | --- |
| Set | Candidate side==OWNER_SIDE | Remember J,continue;last matching J wins |
| Clear | Candidate side!=OWNER_SIDE | Remember J,break;first differing J wins |

Both30000000 bits set take the10000000 branch. Changing flags during providers
can change the branch on later rows. Each row's permutation,ID and side remain
captured at their displayed times. If chosen>=0,store LOW8 chosen to BUF.ab7;
otherwise preserve the0 written immediately after sampler. Return scalar0 on
all paths;it is not an event completion2,chosen index or success Boolean.

## Source-domain and native acceptance requirements

Neither complete local body directly stores menu type M540. Incoming T and table
rows are source/provider domains;these callers do not prove unsupported types
are globally unreachable. Their callee mutations/alias domains remain explicit.

Check retained B/M/T/F,table-before-cursor/UI order,unwritten private setup cursor,
flags OR after UI,retained U versus later B420,wrapped row index,sampler argument
capture,post-sampler selection reset,zero mask,live count/flags/permutations,
first-different versus last-same,low8 chosen,invalid source pointers and return0.
Use original providers/storage rather than synthesizing a default weapon/cursor.

Original table/UI/input/pouch/item/source writers/callback consumers/Game,
platform/numeric and executed menu/attack traces remain EVT-01/JUMP-01 work.
These local functions are ready for porting;the full menu domain and full-game
requirements remain unproven.
