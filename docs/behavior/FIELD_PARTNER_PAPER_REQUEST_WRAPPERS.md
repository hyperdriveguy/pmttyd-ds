# Partner paper requests: handle ownership,name markers and guarded release

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyPaperOn0x800bf070(size0x120),partyPaperOff0x800beff4(size0x7c),
partyChgPaper0x800bed7c(size0x70),partyPaperLightOff0x800befcc(size0x28).
[Roll](FIELD_PARTNER_ROLL_GATE.md),[slit](FIELD_PARTNER_SLIT_GATE.md) and
[forced exit](FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md) bind these services.
This closes the partner wrapper protocol,not original animation/rendering/resource
engines or observed visual results. P+14 paper handle differs from P+c main pose.

## PaperOn(P,resourceName)

Capture name pointer. If signed current handle P+14>=0,request
animPaperPoseRelease(handle)0x8003b1f4,then store-1 to P+14. Negative handles
skip both. OR0x04000000 into P+4 BEFORE requesting
animPaperPoseEntry(name,2)0x80041620;store returned integer to P+14 without
success validation or rollback of the flag. The original name pointer is reused.

Compare name in order against `p_roll`,`p_plane`,`p_dokan_x`,`p_dokan_y`,
short-circuiting on equality. Boolean group argument B is0 for any of those,
otherwise1. Then compare name with `p_slit` independently:if equal,replace B
with(signed kind P+31==5),so kind5 gets1 and every other signed kind gets0.
These resource identifiers live in0x802cb360..0x802cb390.

Request animPoseSetPaperAnimGroup(current P+c,name,B)0x80040e04,then return
integer1 unconditionally. No name pointer null guard,entry result guard or
P+1c paper-animation-name publication occurs here. The caller may next request
partyChgPaper;do not merge their separate marker/handle ownership.

## ChgPaper(P,animationName)

If P+4 mask2 SET,return with no comparison/publication. Otherwise load existing
P+1c. Nonnull old name requests strcmp(old,new);equality returns without reset.
Null old name or inequality stores the NEW borrowed pointer to P+1c,clears
halfword P+20,and ORs0x4000 into P+8. No animation-engine call occurs here.
This is a pending name/phase marker protocol;
[pre-display](FIELD_PARTNER_PRE_DISPLAY.md) now specifies local marker consumption
and shared frame setup. Full animation-frame/backend cadence remains open. No owned copy/free of the name occurs.

## PaperOff(P) has a two-part admission guard

Do anything only if P+4 mask0x04000000 SET AND P+1c NONNULL. If either fails,
return WITHOUT clearing flag,name or handle. In the admitted path:

1. Request animPoseSetPaperAnimGroup(current P+c,null,0).
2. Store null to P+1c,then clear P+4 mask0x04000000.
3. Load current signed P+14. If>=0,request animPaperPoseRelease(handle),then
   store-1. Negative skips release/store.

PaperOn followed by PaperOff with P+1c still null therefore does NOT locally
release the handle or clear the flag. Resource/lifecycle domains must justify any
stronger cleanup assumption. The guard is not replaced by handle validity.

## LightOff(P)

Load current main pose ID P+c and request
animPoseSetMaterialLightFlagOff(poseID,1)0x800406d0,then return. No local P flag,
paper name,handle or counter is changed. Its name does not establish a native
lighting implementation or original appearance.

The slit/roll verifier pins all four complete wrappers,handle/name/mask requests,
resource identities and stores. Backend allocation/failure/material/animation/
resource lifetime,borrowed-name/source/alias/index and original presentation
remain dependencies;the wrappers alone do not implement the reused-data renderer.
