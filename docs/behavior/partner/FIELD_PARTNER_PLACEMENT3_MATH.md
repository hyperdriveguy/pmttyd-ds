# Placement3 heading conversion and normal inclination leaves

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
toMovedir2 at0x80095ff8,size0x200,and partyNrmToAngle0x800c4528,size0x124.
Actual [placement3](FIELD_PARTNER_PLACEMENT3.md) uses both. These are complete
local algorithms;called angle/libm and Gekko numeric state remain separate.

## toMovedir2: global facing override and signed offset

Inputs are base direction D in f1 and offset A in f2. Capture both,then get
global Mario G. If G+0&0x00100000 is clear,preserve D. Otherwise read G+1ac
and replace D using these ordered rules:exact280/460 →0;exact260/440 →180;
otherwise repeatedly wrap that field into[0,360),then ordered wrapped>=180
→0,else180. Incoming D is ignored in that flagged route. Unlike
[partyToMovedir](FIELD_PARTNER_FLOOR_HEADING.md),this uses global G and its
flag/facing fields,not P+0&0x1000 and P+10c.

Capture C=G+19c. Compute B=R(R(270-D)+C),then quantize to thousandths with
original fused1000*B plus ordered B>=0?+.5:-.5,fctiwz and signed-word
conversion/division by1000. Repeatedly add/subtract360 to wrap the quantized
value Q. Next compute W=R(Q-C) and wrap W independently. If ordered W>=180,
use -A;otherwise use A. Add that signed offset to Q with a single-result add,
wrap the result again and return it. The offset is NOT quantized after this
addition. Every wrap is the original ordered repeated single-result loop,
not a total modulo service for infinite/NaN/too-large inputs.

The helper makes one global-pointer request,retains camera offset in a float
temporary,and does not write G/P. With ordinary finite inputs,the algorithm
can be expressed as wrapped quantized camera-relative base plus or minus A,
but preserving the operation and wrapping order is required for numeric parity.
The original conversion bias double lives at0x802c3ec0. Heading/global-facing/
camera/FPSCR/int-conversion domains remain open.

## partyNrmToAngle: magnitude followed by angle request

Input is a pointer to three original normal floats. Compute
q=R(R(Nx*Nx)+R(Nz*Nz)). For ordered q>0,use frsqrte and three DOUBLE
Newton refinements with original double half/3,then round the double final
q*estimate to single. For nonpositive/unordered q,use the negative/NaN canonical
0x7fffffff classification path described in
[floor slope numerics](FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md);zero/subnormal/
other classifications retain q. This complete leaf uses the same local norm
sequence as the inline floor pipeline,not the SDK distance's single refinement.

Request angleABf(0,0,magnitude,-Ny) and return its float. There are no projected
angle requests,integer-degree conversion,P slope-cache writes,normal normalization
or inclination-limit tests in this leaf. The caller supplies its own >60 or other
admission predicate. No input-pointer null guard or numeric-domain sanitization
is added here. Exact angle service,estimate/FPSCR/denormal/exception state and
original output comparisons remain evidence requirements.

The placement verifier pins both complete bodies,global flag/field tests,
offset sign/wrap/quantizer branches and normal refinement/classification/angle
requests plus exact constants. No source-runtime execution or bit-equivalent
host math claim is made by hashing those bytes.
