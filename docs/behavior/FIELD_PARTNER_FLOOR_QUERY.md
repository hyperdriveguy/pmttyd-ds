# Partner floor query: five samples, support admission and cache ownership

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
partySearchGround0x800c3bcc,size0x804. Actual stay postpasses request it with
(P,depth=10,dropLimit=0) for [fly](FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md),and
(P,depth=15,dropLimit=-15) for [ground](FIELD_PARTNER_GROUND_POSTPASS.md).
Inputs depth/dropLimit arrive in f1/f2. This closes its local graph;full scene/
triangle traversal,called filters/math and original trajectories remain separate.

## Early return and initialization

Capture dropLimit. If P+0&0x02000000 is set,return null BEFORE resetting any
query outputs,choosing a heading or making a ray. Existing P+c4/c8/cc/e0/e4/e8
therefore survive this return. This is the same bit latched by the ground
postpass's attr0x200/byte+4c==0 route;its later clearing owner remains open.

Otherwise clear projected slope floats P+c4/c8/cc from the original zero vector
and clear inclination P+e0 to0. Compute A=R(abs(depth)+original .1). Obtain
global Mario and inspect its motion+2e. If it is26,set base ray elevation B=5.
Otherwise B=R(.75*partyGetHeight(P)). That height leaf returns P+f0 normally;
unsigned kind4 with P+0&0x100 set returns R(18.5+P+f0). Partner owner P+160
does not supply this global motion test.

Initialize P+e4 to current root Y=P+5c and P+e8 to-1. These are query caches,
not root stores. With no later selected support,e4 remains the initialized root
height even when some rays hit rejected candidates. There is no height/width
clamp or surrounding-wall width6 override in this query.

## Heading,footprint and fixed ray origins

If float P+104 compares equal to0,use
[partyToMovedir(P,P+10c)](FIELD_PARTNER_FLOOR_HEADING.md);otherwise use P+100
directly. NaN P+104 takes the direct branch;both signed zeros take the helper.
Convert selected heading H with rad=R(R(piFloat*H)/180). Request sin,round
its result,then request cos and round/negate it. Construct offsets in literal
operation order a=R(.375*R(P+f4*sinFloat)),b=R(.375*R(P+f4*(-cosFloat))).
Do not premultiply .375*width or replace the calls with a different rounding
path if numerical parity matters.

Capture root Y then X/Z after these setup calls. Let rayY=R(rootY+B),
maximumLength=R(B+A). All five rays use direction(0,-1,0),copied from the
original vector at0x802cb77c. Origin is(R(rootX+dx),rayY,R(rootZ+dz)):

| Sample | dx | dz |
| --- | --- | --- |
| 0 |0 |0 |
| 1 |a |b |
| 2 |-b |a |
| 3 |b |-a |
| 4 |-a |-b |

The center is first. All five iterations run even after a support was selected;
ray origins remain fixed throughout the pass. Output metadata does not move
root or influence a later origin. Float additions also occur for center zeros.

For EACH ray request fresh yoshiGetStatus and choose filter in order:nonzero
global status → chkfilterYoshi0x800c49b0;otherwise unsigned P kind2 and
P+0&0x100 → chkfilterNokotaro0x800c48cc;otherwise chkfilter0x800c4a9c.
The [ordered filter/status/height leaves](FIELD_PARTNER_FRONT_WALL_FILTERS.md)
are already recovered. This uses the same selector as partner front-wall work,
not the global Mario motion28/roll filters used by surrounding correction.
Callback state is checked per ray;do not cache selection for the whole pass.

## Candidate processing and slope side effects

A null ray result advances to the next sample without reading result fields.
For nonnull G,quantize HIT-POINT Y at query+28 using fused1000*y plus
ordered-y>=0?+.5:-.5,integer truncation and original conversion/division by1000.
This is not distance+3c quantization. Retain its normal from query+30/+34/+38.
The point-X/Z and distance do not become support outputs in this body.

First execute the [projected-slope and inclination pipeline](FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md).
This writes P+c4/c8/cc before testing inclination. If ordered inclination>=50,
discard G and advance,WITHOUT undoing those projected angle stores. P+e0 is
not updated by that rejected sample. Unordered inclination does not reject at
this branch under nontrapping continuation;its later height predicates still
govern admission.

For a still nonnull G,execute the complete pipeline AGAIN,including fresh
angle requests and normal norm calculation;write P+c4/c8/cc again and its
new inclination to P+e0. This second execution precedes vertical admission
and is not an optimization artifact. A native pure helper can reproduce the
values only after establishing that collapsing duplicate requests preserves
all required numeric/state effects.

## Vertical admission and highest retained support

Keep bestHeight=-10000 and bestHit=null initially. Let h be quantized pointY
and Y the captured root Y. Use ordered predicates with separately rounded
differences,as follows:

* If ordered h<Y AND abs(R(h-Y))>=2:compare this drop d against abs(dropLimit).
  If ordered d>abs(dropLimit),write P+e8=abs(R(Y-h)) and skip this candidate.
  Otherwise select it only when ordered h>bestHeight.
* All other cases:select only when ordered abs(R(h-Y))<=11 AND
  ordered h>bestHeight. This includes a below-root drop strictly smaller than2,
  level/above-root candidates and the literal unordered route to these tests.

There is no universal absolute-drop bound11 on the first route:with a larger
dropLimit,a lower floor farther than11 may be admitted if the ray finds it.
Conversely a drop below2 uses the11 test rather than dropLimit,so fly's limit0
does not reject every below-root height. Equal candidate heights keep the
earlier selected sample because replacement uses strict greater. A candidate
not greater than the -10000 sentinel is not selected even if other tests pass.

Every excessive-drop sample overwrites P+e8;this does not compute a minimum
or maximum across rejected drops. That write may occur AFTER another sample
already selected support. At loop end,if bestHit is nonnull,write bestHeight
to P+e4,clear P+e8 to0 and return bestHit. Otherwise return null,leaving e4
at its initialized root value and e8 at-1 or the LAST excessive-drop write.
P+c4/c8/cc/e0 retain the last writes described above,not necessarily those
of bestHit. No root-Y snap,contact P+138 store or motion transition occurs here;
the postpass caller owns those decisions.

## Implementation cases and verification scope

Deduced finite cases include center ties preserving center,highest permitted
edge replacing center,steep late sample overwriting only projected angles,
admitted-height miss still overwriting all angles,no selected support with
e4=rootY/e8=-1,last excessive drop overwriting e8,and any selected support
clearing a later drop write. At exact drop2,use dropLimit;at exact dropLimit
admit;at exact near-height11 admit;at inclination50 reject. These are static
acceptance cases,not original gameplay observations.

tools/verify_field_partner_ground.py pins complete floor/heading/height local
bodies,selected backend regions,independent anchors,exact constants and vector/
numeric spans plus actual postpass/lecture bindings. It does not execute rays
or establish byte-exact libm/frsqrte/GQR/FPSCR equivalence. Full filters are
separately checked by the existing front-wall evidence tool.

Remaining:full shared backend/traversal/resource domains,angle/libm/frsqrte and
platform state,all cache/flag/width/heading/drop producers,appear-position and
jump/fall/fly services,and original floor/support/slope trajectories. Complete
field/game documentation and complete-game requirements remain incomplete.
