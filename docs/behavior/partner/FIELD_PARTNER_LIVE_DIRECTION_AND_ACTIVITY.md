# Live target direction and the scalar named TargetDistY

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partyGetTargetDir0x800ccf90,size0x16c,and partyGetTargetDistY0x800cca54,
size0xe0. Actual [jump preparation](FIELD_PARTNER_JUMP_DESTINATION.md) uses
them for direction and a zero/nonzero travel gate. These select live state,
not follower-history records. Names do not establish the returned quantity.

## Shared candidate test

Normalize integer q with32-bit negation/OR/sign to b=(q!=0). Request
anotherPartyGetPtr(b);its complete [other-party leaf](FIELD_PARTNER_HISTORY_WRITER.md)
maps b0 to table1 and b1 to table0. A nonnull candidate Q is unusable when
nokonokoGetStatus(Q)==3,or otherwise when SDK PSVECDistance(global Mario XYZ,
Q XYZ) is ordered>=150. Null skips both services. Status3 skips distance;
unordered distance does not take >=150 under nontrapping continuation.
The complete [status leaf](FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md) and
[SDK distance](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md) are now linked.

This is a repeated-service test,not an immutable cached target identity. No P
owner pointer is passed into these wrappers,and neither writes history/movement.

## partyGetTargetDir: q0 also tests another partner

Inputs are(q,X,Z);capture X/Z in nonvolatile floats. Obtain global Mario G0,
then normalize q/request Q0 and apply the test using G0. For null/unusable Q0,
return angleABf(inputX,inputZ,G0.x,G0.z),reading captured G0 without a fresh
fallback request.

Unlike [3D/planar target distances](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md),
there is NO q==0 shortcut to Mario direction. q0 admits usable Q0 from table1
into the repeated route. Direction and distance must not be forced to share
an endpoint policy solely because their names contain Target.

For usable Q0,request Mario G1 as fallback,then Mario G2,normalize ORIGINAL q
again and request Q1. Test Q1 using G2;replace unusable Q1 with null. Return
angleABf(inputXZ,Q1.xz when nonnull,otherwise G1.xz). Coordinates remain live
through these pointers until final loads. Do not reuse Q0/G0 or collapse G1/G2.
The second lookup can fall back after the first admitted a candidate.

## partyGetTargetDistY: activity scalar,not a coordinate difference

Input is q only. Obtain Mario G0 first. For q==0,return G0+180 when that float
is ordered>0;otherwise return G0+188. There is no partner lookup,coordinate
difference,absolute value,history read or epsilon. NaN at+180 selects+188;
positive infinity takes the positive branch if execution continues. Offsets
are original field identities,not inferred height semantics.

For q!=0,normalize q/request Q0 and test against G0. Usable Q0 returns its
float+104 directly,including any zero/negative/nonfinite value permitted by
the source domain. Null/unusable Q0 obtains FRESH Mario G1,then returns G1+180
when ordered>0,otherwise G1+188. No second partner-selection pass occurs here.

Partner+104 is the horizontal-speed field used by recovered movement handlers.
Jump preparation tests this return only for equality to0 to suppress travel;
it is not targetY-rootY. Main jump's two history branches independently request
it. Do not replace it with a vertical distance to a live entity/history record.
Complete writers/meaning of Mario+180/+188 remain their source contracts.

## Verification and remaining scope

tools/verify_field_partner_live_services.py pins complete local bodies,selected
backend/reinit evidence,independent requests/branches/loads/callers and constants.
Deduced cases include direction q0 admitting table1 while distance q0 uses
Mario,captured vs fresh fallbacks,Q0 usable/Q1 unusable rereads,scalar strict
positivity,+188 fallback and raw Q+104. These are static fixtures,not original
outputs or measured motion.

Next:target/status/ID/coordinate/scalar producers,all callers,angle/library/
Gekko domains and original target/movement traces. Full field/game
documentation and complete-game requirements remain incomplete.
