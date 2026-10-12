# Front-floor service: upward clearance then downward point output

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
getFrontFloor0x8014b638,size0x434. Actual
[main jump](FIELD_PARTNER_JUMP_DESTINATION.md) calls it after partyGetTargetDir
and leaves that angle in f1. Inputs are(P,output V,heading f1),not merely(P,V).
Its ordinary returns write output XYZ;integer return semantics are below.
Full backend/resource/math/source domains and original trajectories remain open.

## Captured parameters and three UPWARD probes

Capture heading H. Compute step=R(R(2*P+f4)/3). Load and capture float C from
SDA+1cf0 (0x8041ec10);its local setter/census is in
[status and clearance](FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md).
Request sincosf(H) for u=sin,v=-cos. For i=0,1,2,convert i with original bias
double and request a direct hitCheckVecFilter query with null callback:

```
originX = F(float(i),R(u*step),current P.x)
originZ = F(float(i),R(v*step),current P.z)
originY = R(current P.y+current P+f0)
direction = (0,+1,0)
maximumLength = 100
```

Direction comes from0x802f4688. These clearance rays cast UPWARD from head
height. Current root/height are reread per iteration;step,H,C remain captured.
No partner Yoshi/Nokotaro callback is requested. Null callback does not remove
the backend's geometry/cull/attribute rules.

For nonnull result,read normal X/Z and compute single-result horizontal norm m
with positive frsqrte plus THREE double refinements,otherwise negative/NaN
canonical classification. This matches the local
[floor norm](FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) shape,using double constants
at0x802f4738/4740/4748 here. Ordered m<45 retains that ray's RAW pointY and
stops scanning. Null or m not ordered<45 advances i. Threshold45 applies to
HORIZONTAL NORMAL MAGNITUDE,not inclination degrees;no angle request occurs
in this selection block. First admitted probe wins,not nearest/lowest ceiling.

After three misses/rejections,use sentinel -2000 as local selectedY. It is
not returned directly as a failure coordinate.

## Downward origin and width offset

If selectedY is ordered>-2000,compute A=R(current P.y+C),B=R(selectedY-2).
Use A for ordered A<B;otherwise B. If selectedY fails that gate,use
R(current P.y+C). This ordered minimum can select B for unordered A/B;
do not substitute unspecified host-min NaN behavior.

Request sincosf(H) AGAIN. Set radius=R(.75*current P+f4),then origin
X=F(u,radius,current P.x),Z=F(v,radius,current P.z),Y=the chosen value above.
Cast direction(0,-1,0) from0x802f4694,length300,null callback. This endpoint
uses .75 width,not the upward sequence's2/3 width step. No later horizontal
movement or normal-slope acceptance is performed by this body.

## Output and nonboolean return

Final downward miss writes CURRENT P.xyz to V in XYZ order and returns0.
The caller receives a populated fallback,not untouched/uninitialized output
or the selectedY sentinel.

For nonnull G,quantize raw pointX/Y/Z independently to thousandths with
ordered-value>=0?+.5:-.5,fused1000*value+bias,fctiwz and signed-word conversion/
division. Write V.X/Y/Z,then request hitGetAttr(G) and return that attribute
mask. A successful zero-attribute hit also returns0;no boolean1 or hit-pointer
return is substituted. There is no output-pointer guard. Upward pointY and
normal calculations are not quantized by these final output stores.

Main jump ignores the integer result,reads V.y and adds10 for destination
preparation;its retained direction is saved separately before this call.
This resolves the earlier output dependency and documents the actual f1 input.
The service does not directly move P,write contacts/slopes or change motion.

## Validation and remaining scope

The live-services verifier pins the complete body,up/down templates,all three
quantizers,attribute return,actual caller and exact numeric constants. It also
checks a scoped DOL census for the captured clearance parameter. Deduced cases
include upward direction,first m<45 admission,all-rejected sentinel fallback,
selectedY=-2000,clearance minimum ordering,miss rootXYZ output,successful
attr0 return0 and original heading propagation. These are static fixtures,
not original collision observations.

Next:full backend/normal/cull/traversal,Gekko/FPSCR/int-conversion and heading/
width/height/clearance/source domains,all callers and original clearance/jump
trajectories. Full-game documentation and requirements remain incomplete.
