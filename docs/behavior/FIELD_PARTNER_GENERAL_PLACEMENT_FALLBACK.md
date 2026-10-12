# General placement: fallback dispatch,asymmetric probes and failure output

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
partyGetAppearPos0x800c6c9c,size0x135c. This continues
[entry/primary/admission](FIELD_PARTNER_GENERAL_PLACEMENT.md). All six fallback
blocks are unrolled code,not a uniform radial loop. Retained F is the entry
facing;L is entry R(30+P+f4). Every heading/offset service request consults its
own documented global state. Neither the initial primary result nor the original
input output vector is restored on failure.

## Dispatch after rejected/skipped primary

At0x800c71d0 reread signed M+245;negative returns0 immediately. Otherwise
reread signed M+246. Negative selects chain A(single-slot),nonnegative chain
B(dual-slot). This is a fresh dispatch even if primary chose the other chain.
No local branch re-enters this dispatch once a chain is selected. Within a chain,
try three blocks in order;each success returns1. Final rejection returns0 at
0x800c7fd0. No additional general placement call or guaranteed fallback ownerY
assignment occurs there.

## Chain A: current M+246 negative

| Block / horizontal call | Probe angle request | Move angle request after horizontal miss | Signed move distance | Downward call / next rejection |
| --- | --- | --- | --- | --- | --- |
| A1 / 800c7298 | toMovedir2(F,150) | toMovedir2(F,90) | -30 | 800c7358 / A2 |
| A2 / 800c74dc | toMovedir2(F,90) | toMovedir2(F,150) | -30 | 800c759c / A3 |
| A3 / 800c7720 | toMovedir2(F,0) | toMovedir2(F,0),requested AGAIN | +30 | 800c77dc / return0 |

Each horizontal ray originates at CURRENT captured-owner M.xyz with Y+37,
NOT at accumulated V,using direction(sin,0,-cos) and length L. A nonnull
horizontal result rejects that block without a local V write. On a horizontal
miss,copy fresh M.xyz to V in XYZ load/store order,then make the listed move
request on V.x/V.z. That reseed occurs BEFORE the move-angle request. Cast down
from the moved V with Y+37,length150,direction(0,-1,0),and apply the complete
attribute/name/length<=100/raw-Y gate from the primary document.

Rejected downward/matching-name/attribute/length paths leave the moved output
in place. A later horizontal HIT does not overwrite it. A3's positive distance
is intentional in the compiled body;normalizing all distances to -30 changes
its candidate and failure output. Probe and movement directions differ in
A1/A2,so a horizontal miss does not by itself validate the displacement segment.

## Chain B: current M+246 nonnegative

| Block / horizontal call | Probe angle request | Move offset when signed P+2f==0 | Move offset for EVERY other signed value | Signed move distance | Downward call / next rejection |
| --- | --- | --- | --- | --- | --- | --- |
| B1 / 800c7978 | toMovedir2(F,150) | -30 | 70 | -30 | 800c7a4c / B2 |
| B2 / 800c7bd0 | toMovedir2(F,90) | 110 | -110 | -30 | 800c7ca4 / B3 |
| B3 / 800c7e28 | toMovedir2(F,0) | -30 | -70 | -30 | 800c7efc / return0 |

Each move angle is toMovedir2(F,selectedOffset). P+2f is sign-extended and
reread after EACH successful horizontal miss;only equality0 is special.
There is no validated range restriction to0/1 in this body. Negative and other
nonzero byte values take the nonzero route.

B1 first copies fresh M.xyz to V BEFORE requesting its probe angle and
horizontal ray. B2/B3 do not do that pre-probe V reseed. All horizontal rays
still originate from fresh M.xyz,Y+37,not from V,with captured length L.
On each horizontal miss,copy fresh M.xyz to V AGAIN in XYZ load/store order,
then read P+2f,request the selected move angle and move -30. Downward query and
admission are identical to chain A. B1's first reseed thus occurs even when its
horizontal ray hits,unlike A1. Replacing all six blocks with one uniform helper
must preserve this observable distinction and each callback/read/write order.

## Output and caller composition

On valid-pointer entry,initial seed guarantees that V has been written even
if current M+245 is negative and no ray is cast. Further outcomes depend on the
actual path:seeded coordinates,B1 pre-probe reseed,or the last horizontally
admitted moved coordinates. Only accepted downward results replace V.y with
raw pointY;all rejected floors leave the corresponding seedY. Callback/source
aliases can affect later live reads. Return0 preserves neither original input
V nor necessarily initial owner coordinates. No common terminal root/Y reset,
rollback,slope cache/contact publication or maximum-distance candidate save
exists in the local body.

[Placement4](FIELD_PARTNER_PLACEMENT4.md) exhaustion calls this function with
its existing V,ignores this function's0/1,then optionally enforces V.y=M.y+5
when mobility P+32==1 and V.y<M.y. Placement4 returns0 regardless. In
[fall](FIELD_PARTNER_FALL_AIR.md),V=&P+58;fall ignores placement4's result
and adds37 to the then-current rootY before motion0 and its horizontal tail.
The general writer's reseeds therefore replace prior placement4 accumulation
on the ordinary nonalias owner/root domain,even if the general writer returns0.
Preserve that composed output policy;do not treat all three integers as one
success/failure transaction.

This closes the local general-writer graph. Feasible slot transitions,all
source/owner/output aliases,query backend,libm/FPSCR,GQR,other caller bindings
and matched original recovery trajectories remain necessary for strong
end-to-end equivalence. Static byte/census checks do not resolve those domains.
