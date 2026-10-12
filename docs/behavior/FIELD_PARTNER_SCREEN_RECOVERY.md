# Partner out-of-screen recovery: projection gates and candidate search

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
party_force_reset_outofscreen0x8014b1a8,size0x490,marioGetScreenPos0x80058960,
size0x84,and two16-byte control/key gate leaves. Actual
[stay](FIELD_PARTNER_STAY_MOTION.md) calls recovery after the ground postpass,
but returns after the mobility==1 fly postpass without this call. Other callers,
complete projection services and original screen trajectories remain open.

## Projection and initial gates

Capture M0=P+160. Request marioCtrlOffChk,then only if0 request marioKeyOffChk;
either nonzero returns before projection or mutations. The complete leaves read
the shared Mario pointer from SDA+19e0 and return sign-extended byte+38 or+39,
respectively. Negative gate values also suppress work. Neither leaf reads P.

Project a copy of current P root. marioGetScreenPos(vector,&SX,&SY,&SZ) gets
camera4,requests GXSetProjection(camera+15c,type from camera+19c),then
GXGetProjectionv and GXGetViewportv into local arrays. It requests GXProject
with the input XYZ,camera+11c modelview,those arrays and the three output
pointers. The local body does not restore a previous projection afterwards.
Projection/camera/viewport numeric and rendering-state ownership remain called
services;do not bind camera0 by convenience. Recovery ignores SZ throughout.

Next request planar distABf(M0.x,M0.z,P.x,P.z). If ordered distance<20 AND
SY>800,request partyGetAppearPos3(P,&candidate),ignore its return,copy candidate
XYZ directly to P root and finish. That called placement algorithm remains
unclosed here;its output guarantees and failure domain must be established
before assuming that an unchecked copy always has valid coordinates.

Otherwise return without further work if ordered -100<=SX<=700 AND SY<=650.
These tests have no lower SY or SZ/depth bound. Unordered comparisons do not
admit their conjunction under nontrapping continuation. This initial window
is fixed;later candidate acceptance uses different bounds and only SX.

## Footprint reset and search setup

If initial gates did not finish,reread P+0&8. BOTH branches request the same
partyClearFootmark0x800cc7c4,which ignores its input and resets bank0 from
global Mario. The set-bit branch does NOT call partyClearFootmark2. This differs
from ordinary stay's bank-selection route. Its complete
[bank0 leaf](FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md) is already recovered.

Capture fresh M1=P+160 after that reset. Copy current P root to mutable local V.
Set completed-move count k=0. Choose step and screen margin:

| Classification | XZ step | Margin m |
| --- | --- | --- |
| Unsigned mobility P+32==0 |Current P+f4 |100 |
| Nonzero mobility,unsigned kind P+31==5 |10 |70 |
| Other nonzero mobility |10 |50 |

No width6 or positive-step clamp is added. The local root remains separate from
actual P until an explicit commit below. Neither motion nor flags are reset by
this body's direct stores.

## Outer candidate loop and bounded moves

At each outer iteration,project V FIRST. If ordered -m<=SX<=600+m,copy V
XYZ to P root and return. This test ignores SY/SZ;there is no requirement that
the candidate pass the initial fixed-window vertical test. Otherwise,if k>100,
return WITHOUT committing V. The comparison comes after projection/acceptance,
so up to101 XZ moves are possible;the candidate after move101 can still commit
if its projected X qualifies. It is not a100-iteration clamp or a final unconditional
teleport on exhaustion.

If another move is allowed,request angleABf(V.x,V.z,M1.x,M1.z),then movePos
on V's X/Z by the selected step toward that angle. Set vertical probe offset
t=200,attempt count j=0,and chosenY=current V.y. The fixed V is the moved X/Z
with its current root Y;query origins do not accumulate previous vertical offsets.

## Vertical probe loop and raised retry

Each vertical attempt builds origin(V.x,R(V.y+t),V.z),direction(0,-1,0) and
length300;request [partyHitCheck](FIELD_PARTNER_VECTOR_RAY_WRAPPER.md).
For null result,set chosenY=M1.y. For nonnull with attr0x200 SET,retain the
prior chosenY. For other nonnull results,set chosenY=quantized pointY from
that wrapper. The generated null-test duplicate inside the nonnull branch is
unreachable given the immediately preceding branch;do not invent another
observable null route.

Project (V.x,chosenY,V.z). If ordered SY>100,perform the following optional
raised retry and finish this vertical loop:

* If ordered R(V.y+74)>chosenY,cast from(V.x,R(V.y+74),V.z),direction down,
  length100. Nonnull hit with attr0x200 CLEAR replaces chosenY with its
  quantized pointY. Null or attr0x200 preserves chosenY.
* Otherwise make no raised retry. Finish regardless of the retry outcome.

If SY is not ordered>100,increment j. When j>4,finish with current chosenY.
Otherwise set t=R(t-50),replace it with0 only for ordered t<0,and repeat.
Thus the ordinary offset sequence is200,150,100,50,0 with at most five primary
queries per outer move. The raised retry can occur after any primary attempt;
it is not a sixth ordinary offset. ChosenY survives excluded-attribute hits
between attempts,while their input origins continue to use V.y+t.

At vertical-loop exit,write chosenY to local V.y,increment k,and return to the
outer projection gate. No actual P root store occurs on each query or move.
Footprint bank0 reset can remain observable even if all candidate work ultimately
returns without a root commit. The body neither clears retained contacts nor
directly requests a new motion/pose at successful placement.

## Verification and outstanding dependencies

tools/verify_field_partner_recovery.py pins these complete local bodies,selected
backend evidence,actual stay/ground calls,independent gates/loop/query/root-store
anchors and exact constants/templates. It fingerprints called services only
where its stated scope permits;partyGetAppearPos3,full SDK projection,movePos/
angle/distance math and traversal are not closed by this document.

Deduced cases include signed control/key suppression,near20/SY800 strictness,
initial versus candidate screen windows,bank0 reset on either flag branch,
candidate root writes only at explicit acceptance,101 moves with acceptance
before exhaustion,null floor owner-Y fallback,attr0x200 retaining prior Y,
five offset attempts,raised74 retry and miss/excluded-hit preservation. These
are static fixtures,not measured screen recovery or emulator observations.

Next:placement3 output contract,full projection/camera/viewport state and math,
hit/source/flag/width/counter domains,all callers and original off-screen
trajectories;full field/game documentation and requirements remain incomplete.
