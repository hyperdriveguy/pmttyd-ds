# Field partner mode1 substate dispatch, splash countdown and hazard request

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
unk_800ca884 at0x800ca884,size0x1e8 and hitGetAttr at0x80014310,size0x18.
The unknown annotation is retained without inventing an original name.
[driveParty](FIELD_PARTY_DRIVER_DISPATCH.md) mode1 calls this body directly.
It makes no call to partyMoveWalk or partyWalkMain in its local request graph.

## Marker and substate branches

If word P+8 **mask4** is set,consume it,request partyChgPoseId(P,1),clear
P+0 mask0x100 and,if unsigned kind byte P+0x31 equals5,request
unk_80187d74(P) at0x80187d74. This marker is distinct from mode-change
mask8. Then read unsigned substate byte P+0x36 and dispatch:

| Substate | Request/writes |
| --- | --- |
| 0 |Call unk_8015146c(P),at0x8015146c;then reread kind byte,and if5 call unk_80187d10(P),at0x80187d10 |
| 1,2 or6 |Clear substate byte to0 and OR2 into P+8 |
| 3 |Call party_motion_fall(P),at0x8014cbb8 |
| Others |No substate service |

The1/2/6 arm contains an additional test of the cached selected substate
against3,with three flag-clear writes if true. Under its own local dispatch
that test cannot match:substate3 took a different arm,and there is no call
between selection and this cached test. Preserve the original branch evidence
without claiming it provides reachable3→0 cleanup. The called bodies may change
state elsewhere;this particular arm does not reread the substate before testing.

## Angle copy and splash marker

After any substate branch,copy float P+0x100 to P+0xfc. If P+8
**mask0x01000000** is set,consume it,copy P+0x58/+0x5c/+0x60 into a stack
vector,and request **marioSetSplash(0,&vector)** at0x8013b6e8. Write byte20
to P+0x4c. The effect service's visual/audio behavior remains open.

Every call then reads unsigned byte+0x4c,subtracts1,stores the low byte and
sign-extends it. Negative signed result clamps the byte to0;zero is retained.
Thus a newly requested20 becomes19 in the same invocation. Raw0 becomes255
then clamps0;raw128 becomes127 and is retained;raw255 becomes254 then clamps0.
These literals and arithmetic do not establish measured splash duration or
rendering cadence. This byte has a different expiry rule from the
[global action countdown](FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md).

## Ordered hazard-pointer checks and mode13 request

If the **reread** mode byte P+0x34 equals13,skip the following hazard tests.
Otherwise test P+0x138,then+0x12c,then+0x140 in that priority order.
For each nonnull pointer H,call hitGetAttr(H). Its complete body returns
word H+4 if nonnull,otherwise0. If the returned word has **mask0x800**,
request [partyChgRunMode(P,13)](FIELD_PARTY_MODE_INITIALIZATION.md) and
skip remaining pointer checks. A null pointer or clear attribute continues
to the next pointer. No local position,HP/FP or saved-kind write accompanies
that request.

Finally request **vivianShadow(P)** at0x801863b8 in every path,including
when mode was already13 or has just changed to13,and return. The routine
name does not make this call exclusive to a Vivian-kind object. Its body
must establish actual applicability/effects. The driver subsequently continues
its common effect/screen/presentation tail with the potentially changed mode.

## Verification and remaining work

tools/verify_field_party_follow.py pins seven complete local bodies,66
instructions,ten numeric constants,two kind-record hashes and two actual
lecture imports. It checks the selected mode1 call and hazard requests as
separate anchors;whole hashes do not execute called movement/effect services.
Reports and corrected Gekko listings remain ignored.

Next:substate engines and kind5 cloud helpers,fall/terrain producers and
vivianShadow/splash bodies;marker/countdown/mode writers,driver composition,
original stop/run/fall/hazard trajectories and field/battle teardown. This
dispatch closure is not complete partner movement or full-game equivalence.
