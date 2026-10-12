# Partner slit gate: admission,entry,travel,slit-floor rise and exit phases

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
party_slit0x8014be2c,size0x504. Used homing/stay callers test this BEFORE
[roll](FIELD_PARTNER_ROLL_GATE.md);return0 blocks their later movement.
Capture owner M=P+160 at prefix,even on inactive paths. Phase is unsigned
P+3a,count signed word P+24. R below is the default integer return1;single-result
operation rounding is denoted round(). Called paper/pose/collision/source/math
semantics remain separate unless linked to their own complete local contracts.

## Admission,entry and phases0/2

If P+37!=1,test CAPTURED owner M+0 mask0x00100000. Clear→return1.
Set plus M+0c mask0x80000000 SET→return1. Set plus latter CLEAR→OR1 into
P+8,store P+37=1,then enter setup. Existing P+37==1 skips those owner gates.

If P+8 bit0 set,consume it,OR0x100 into P+4,request
[partyPaperOn](FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md)(P,`p_slit`),then
partyChgPaper(P,`PM_S_1A`). Store count30 and request immediate speed writer0.
Compute A=revise360(round(P+100-current M+19c)). Ordered A<=180 clears
P+4 mask0x200 and sets phase0;otherwise(including unordered)set that mask and
phase2. The owner pointer remains captured but these fields are live loads.

For phase0 OR phase2,decrement count using addic./32-bit wrap,store it,and
continue without phase change while the result is signed>=0. Thus this transition
uses strict NEGATIVE,unlike roll's<=0 boundary. On negative:
OR0x1000 into P+0;set P+10c=270,LOAD it again and store P+110;request
`PM_S_1B`. If P+0 mask0x2000 clear,set phase10 and R=0;otherwise set phase20
and keep R=1. The later phase groups are checked in the SAME invocation;
phase20 can start its rise setup immediately. Entry phase10 returns0 only on
this transition;ordinary later phase10 calls retain default1.

## Phase10: pose,direction and no-crossing exit request

If current speed104 compares equal0,request `PM_S_1B`,set P+10c=270,and skip
the moving pose tests. Nonzero/unordered speed takes the moving branch:

1. A=revise360(round(P+100-M+19c)). Ordered A>=original DOUBLE180 sets
   P+4 mask0x200;otherwise clears it. Equality here SETS the bit,where entry
   A<=180 cleared it. This repeated angle request is not cached.
2. Set P+10c=280 when P+4 mask0x200 set,else260.
3. Recompute A with another revise360 request. Request `PM_S_1E` if ordered
   A>=315 OR A<=45 OR(135<=A<=225);otherwise `PM_S_1D`. Unordered takes D.

Both stopped/moving branches copy current rootXYZ to a local vector and request
unk_800c2010(P,&vector)0x800c2010. Its full0x7b0-byte collision/body remains open.
Only result0 proceeds to the exit eligibility tests:if captured M+0 mask
0x00100000 clear,request hitGetAttr(P+12c),then ONLY if its0x4000 clear request
hitGetAttr(P+134). No pointer-null guard is inserted before those calls;the
attribute helper owns its own null/result behavior. If both masks clear,set
phase40. Otherwise retain current phase. No root vector is published here.

## Phase group20/21/22: rise and global slit-floor clear

After earlier work,reread phase. Phase20 requests `PM_S_1H`,ORs0x02000000
then0x20 into P+0 in separate writes,stores count20,requests immediate speed0,
and stores phase21. It jumps to the exit group rather than executing phase21's
rise in this same call;R remains1.

Phase21 computes A=revise360(round(P+100-P+f8)). Ordered A<=180 selects
P+10c=260,otherwise280. Add0.5 to rootY with fadds. Decrement count32-bit;
if signed<=0,store phase22;otherwise retain21. The newly stored22 does not
execute the clear below until a later invocation because this branch skips it.

Phase22 sets phase10,clears P+0 mask0x02000000,and requests
[marioClearSlitFloor](FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md).
It does not clear P+0 mask0x20 here. Global availability and stored coordinates
are separate;the leaf clears only availability. Other phases skip this group.

## Exit group40/41/42

Reread phase again. Phase40 requests `PM_S_1C`,stores count30 and phase41.
Compute A=revise360(round(P+100-current M+19c)). If ordered A>=180,replace
local A with0;otherwise keep the wrapped A,including unordered. Store this
local float to P+10c,LOAD it again and store P+110. FALL THROUGH into phase41:
the new30 is decremented immediately to29. This differs from20→21 above.

Phase41 decrements/stores count32-bit. Signed>0 retains phase41;otherwise store
phase42 and skip its cleanup until a later invocation. R remains1 throughout
this exit group;this local function does not block locomotion for a timed phase41.

Phase42 clears P+4 mask0x3c0,requests partyPaperOff(P),then clears P+0 masks
0x3000 and stores P+37=0. Phase42 itself is retained. A future call may reenter
slit when the owner admission flags remain set. No ordinary pose1,history clear,
rootXZ publication or travel-heading reset is added to this completion path.

Unhandled phase values preserve defaultR=1 without phase-specific work;the
admission/setup path may still have run. All four dispatch groups are ordered
as written,not a loop that processes new phases until stable. Constants30/20
and phase IDs are recovered literals,not original observed durations.
Source/phase/flag/alias/numeric domains,the collision helper,pose backend and
original slit/form traces remain open after the complete local byte checks pass.
