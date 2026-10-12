# Display-facing selector and four interpolation branches

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyMakeDispDir0x800b7db0,size0x340. Sole direct DOL caller is
[pre-display](FIELD_PARTNER_PRE_DISPLAY.md) at800b7ad0. Fields C=P+10c and
T=P+110 are current/target display direction;P+fc travel copy andP+f8 captured
camera offset feed selection. This local algorithm writes display fields/flags,
not rootXYZ. R denotes original separately rounded single operations.

## Admission and target selection

P+4 mask0x100 SET returns with no local facing work. Otherwise if P+4 mask0x10
SET,skip target selection and go directly to interpolation. Normally compute
A=revise360(R(P+fc-P+f8)). Load T,then reread P+4 mask0x100:
if clear and T compares equal290 OR250,and A is ordered>=180,replace T=0.

If speed P+104 is not equal0(including unordered),apply ordered angle ranges:
190<=A<=350 sets T=0;otherwise10<=A<=170 sets T=180. Other A retains T,
including unordered. These ranges are tested in that order. Store T to P+110.
If P+4 mask0x20 SET,copy CURRENT P+110 to P+10c. Then compare CURRENT C/T:
equality returns;otherwise enter interpolation. The mask0x10 route skips this
selection/store/forced-copy/equality section entirely.

## Per-branch interpolation and latch

Interpolation first reads C. Ordered C<=180 selects LOWER;otherwise only
ordered C>180 selects UPPER. Unordered C returns without an interpolation.
Each branch handles target T equal0 or equal180 ONLY;every other T returns.
Let F denote CURRENT P+8 mask0x1000. Select intermediate goal G:

| Target | F clear | F set |
| --- | --- | --- |
| 0 | -40 | 20 |
| 180 | 220 | 170 |

Compute N=R(R(0.125*R(G-C))+C),with fsubs,fmuls,fadds,NOT FMA.
Store N to P+10c,then reread flag/current field for the branch's cleanup:

| Branch | After N publication |
| --- | --- |
| LOWER,target0 | F clear:if ordered N<-30,set F,then return;F set:if ordered N>=0,store C=0 and clear F |
| UPPER,target0 | Same ordered latch/clamp graph as LOWER,target0 |
| LOWER,target180 | F clear:if ordered N>210,set F,then return;F set:if ordered N<=180,store C=180 and clear F |
| UPPER,target180 | F set:RETURN after interpolation,without a local clamp;F clear:if ordered N>210,set F and return;otherwise if ordered N<=180,store C=180 and clear F |

The final UPPER,target180 branch is not identical to the lower branch. If its
interpolation moves C into the lower range,a later invocation may run the lower
cleanup. Preserve live rereads,strict latch thresholds and ordered equality clamps.
There is no repeated convergence loop in one call,angle wrap after interpolation,
shortest-arc selection,speed multiplier or generic absolute epsilon snap.

P+8 F is distinct from P+0 mask0x1000 used by slit and P+4 mask0x100/0x10
admission flags. Do not combine these same-bit-number fields into one form flag.
Caller/target/source/flag/alias/FP domains and original display-turn trajectories
remain open;constants0.125/-30/210 are recovered floats,not measured turn duration.
The pre-display verifier pins this complete graph and all original constants.
