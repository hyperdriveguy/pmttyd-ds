# NPC horizontal overlap scan returns the last admitted record

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
npcNearDistCheck0x800482a8,size0x170,called by
[partner placement3](FIELD_PARTNER_PLACEMENT3.md). Inputs are query X,Y,Z,
radius in f1..f4. Return is an NPC pointer or null. No original proximity trace
or full NPC lifecycle is inferred from this local scan.

## Pool,record order and admission

Read global GP+14. Zero selects NPC work at SDA+1994;nonzero selects that
work plus0x14. Capture signed count from work+4 and array base from work+c.
Nonpositive count returns null. Otherwise scan exactly count records at stride
0x340,in array order. Only word record+0 bit0 admits a record to numeric tests;
inactive records skip every distance calculation. No other NPC flags,hidden/
collision state,special kind or radius-from-record field is read here.

For an active N,load position N+8c/+90/+94. Compute single-result differences
dx=X-N.x,dz=Z-N.z,dy=Y-N.y;form dx²,dz² and qXZ=R(dx²+dz²). Obtain horizontal
distance d with ordered-positive frsqrte plus three double refinements,otherwise
the same negative/NaN canonical-classification shape as
[partner floor numerics](FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md). This body uses
its own double half/3/zero span at0x802c13c8 and shared canonical NaN at
0x8041e3c8. qXZ's initial sum/products remain single-result instructions.

Then compute qXYZ=R(dz²+F(dy,dy,dx²)) and compare it with0. The VERY NEXT
fcmpo overwrites the same CR field with comparison d versus radius;the branch
uses only that latter comparison. The qXYZ computation/comparison is not a
vertical-distance gate,though arithmetic exception/FPSCR effects remain part
of the numeric environment. Do not use that apparently three-dimensional
intermediate to invent a height filter.

If ordered d<radius,set return pointer=N. Otherwise leave the prior return
pointer unchanged. Continue scanning;do not return immediately on a match or
reduce a nearest distance. Consequently the LAST active horizontal match wins,
including among identical positions. Equality d==radius does not match,NaN
comparison does not match under nontrapping continuation,and a negative radius
has no admitted finite nonnegative distance. Vertical separation does not affect
eligibility. Array count/base/record validity remains an external domain.

## Placement consumer and validation

Placement3 receives that last qualifying NPC,then separately requests SDK
PSVECDistance from N position to the unsnapped candidate V. That later THREE-
dimensional distance can choose a saved fallback candidate;it does not change
which NPC the scan returned. Its SDK single-refinement numeric path is already
recovered in [distance numerics](FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md).

tools/verify_field_partner_placement3.py pins this complete body,selected-pool/
count/stride/active-bit order,norm and overwritten CR comparison,last-record
store and caller binding. Deduced tests include two horizontal matches returning
the later one despite different Y/3D distances,strict radius equality,active-bit
skip,nonpositive count and the alternate work bank. These are static fixtures,
not runtime observations. NPC creation/deletion/pool/GP14/source/numeric domains
and original collision/placement trajectories remain open.
