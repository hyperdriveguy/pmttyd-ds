# Slit-floor source,activation,forced exit and used caller bindings

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
marioGetSlitFloorPos0x800a6258(size0x5c),marioClearSlitFloor0x800a62b4
(size0x28),partyChgMotSlitFloor0x8014c408(size0x40),
L_partyForceSlitOff0x8014c448(size0xc0). These complement
[slit phase protocol](FIELD_PARTNER_SLIT_GATE.md) and
[roll protocol](FIELD_PARTNER_ROLL_GATE.md). No original trajectory is measured.

## Global slit-floor getter and clear

GetSlitFloorPos(output V) calls marioGetPtr and captures returned global Mario G.
If word G+26c==0,return0 WITHOUT writing V. Any nonzero word takes success:
load raw G+270(X) and G+274(Y),store V.x then V.y,THEN load G+278(Z) and
store V.z. Return1. There is no null/output/range guard;output/source aliasing
must preserve the early X/Y loads and late Z load rather than copying an invented
immutable XYZ. Availability is an integer word predicate,not a ray cast.

ClearSlitFloor calls marioGetPtr and stores word0 ONLY at G+26c. It preserves
the three stored coordinate words. It ignores any incoming object pointer and
does not clear partner flags or force its phase to a completion state. Global
floor-availability/coordinate producer and full lifecycle remain open.

## partyChgMotSlitFloor(P)

If P+0 mask0x2000 already SET,return immediately. Otherwise capture owner
M=P+160 and test M+0c mask0x80000000. If clear,OR1 into P+8,then store
P+37=1. If set,skip those two writes. In EITHER case OR0x2000 into P+0.
Thus owner rejection can still leave the special slit-floor flag set,without a
new pending marker or P+37 transition. No floor/source query,position move,
phase/count change or animation request occurs here.

[Flying homing](FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md) calls this after a
second successful slit-floor getter and planar distance<=5. The getter's success
can change between its two calls;do not cache the first vector or activation.
Ground homing and another ground path also bind this leaf as listed below.

## L_partyForceSlitOff(P)

Admit if P+0 mask0x1000 SET OR P+4 mask0x100 SET. If both clear,return with
no local writes or paper/pose call. Otherwise:

1. A=revise360(round(current P+100-current P+f8)). If ordered A>=180,
   replace local A with0;otherwise preserve wrapped A,including unordered.
2. Store A to P+10c,LOAD it again and store P+110. Compute
   revise360(round(270-current P+10c)),store P+fc,LOAD that field again and
   store P+100. Do not replace this with a single immutable heading expression.
3. Clear P+4 mask0x3c0,then request
   [partyPaperOff](FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md)(P).
4. Clear P+0 masks0x3000;store byte0 to P+37;request partyChgPoseId(P,1).

This forced path does not reset phase3a,count24,rootXYZ,global slit-floor
availability or all other roll/slit transform fields. It differs from slit42's
cleanup and roll101's heading/vector reset. PaperOff's guarded ownership still
applies;do not assume the request always releases a handle. Full pose backend and
source/alias/numeric domains remain open.

## Exact direct-reference census

All seven DOL text sections contain22 linked direct references to these six
slit/roll/floor/exit functions,no direct nonlink sites in this scoped census:

| Target | Direct call sites / containing annotations |
| --- | --- |
| party_slit | 8014f238 homing_fly;8014ff50 homing_walk;801514c8 unk_8015146c;80151878 stay |
| party_roll | 8014f248 homing_fly;8014ff60 homing_walk;801514d0 unk_8015146c;80151880 stay |
| GetSlitFloorPos | 8014f538,8014f55c homing_fly;80150424,80150448 homing_walk |
| ChgMotSlitFloor | 8014f5c8 homing_fly;801504b0,80150968 homing_walk |
| ClearSlitFloor | 8013a974 mot_damageToge;8013ae70 mot_damage;8014c250 party_slit |
| ForceSlitOff | 800b716c partySetFamicomMode;800bc3d4 driveParty;800bcf14 N__partyChgRunMode;80184430 vivian_use |

The existing [driver mode11 contract](FIELD_PARTY_DRIVER_DISPATCH.md) binds its
pending force-off call;other full containing bodies remain separate. Direct
reference identity does not prove feasible runtime branches,frequency,all
indirect/REL uses or whole caller completion.

Run `python3 tools/verify_field_partner_slit_roll.py`:ten complete local hashes,
482 instruction anchors,18 constants,four original data spans,two lecture imports
and exact22-site census. Reproducible bytes/metadata do not execute original
code. Underlying collision helper,paper/pose backend,global availability producer,
all state/flag/source/alias/numeric domains and matched original form/exit traces
remain required for strong end-to-end equivalence.
