# SDK inverse-transpose and point matrix arithmetic

US G8ME01, source ISO opened rb. Static source evidence only; no original execution.
Companion private `.dis` files under `artifacts/battle-display-queue-evidence/`
are corrected with tools/gekko_disasm.py after llvm-objdump. This local source
contract supports [shape entry](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md);
exact reciprocal-estimate/exception behavior remains a numeric provider.

## Bodies

- PSMTXInvXpose8029df28:c8 raw SHA256 c96d218ee09b679c82c6a2fd9867fd5e8e4abb06f1b193c99f7f8aad2e7172d6.
- PSMTXMultVec8029e6a4:54 raw SHA256 0e86b49668bd9172bfd1256aa2842a4326032efab48cb6b8df58523a63992e78.

## PSMTXInvXpose

r3=input affine3x4, r4=output affine3x4. Input basis rows are(a,b,c),
(d,e,f),(g,h,i). Input translations are never loaded. All nine basis values
are loaded before any output write, so same input/output storage is safe in the
ordinary valid-memory/float domain. GQR0 must mean ordinary float/no scaling;
W1 psq loads synthesize1 in their second lane. No numeric constants loaded from
memory, no calls and no null/range validation.

Let R mean required single-result rounding, FS(x,y,z) the source paired-single
fused product-minus, F(x,y,z) fused product-plus. These are instruction arithmetic
contracts, not a claim that arbitrary host fma reproduces Gekko FPSCR/NaN behavior.
The first lanes and stored cofactor pairs are:

C00=FS(e,i,R(h*f)); C01=FS(f,g,R(i*d)); C02=FS(d,h,R(e*g)).
C10=FS(h,c,R(b*i)); C11=FS(i,a,R(c*g)); C12=FS(b,g,R(a*h)).
C20=FS(b,f,R(e*c)); C21=FS(c,d,R(f*a)); C22=FS(a,e,R(b*d)).

determinant=F(g,C20,F(d,C10,R(a*C00))). Zero reference Z is actually the
first lane of ps_sub f1,f1,f1, hence b-b, not an external literal. At8029df8c
ordered compare of determinant lane0 to Z; equality returns r3=0 atdf94
WITHOUT ANY output stores. Unordered compares proceed, so do not add a NaN guard.
For finite inputs Z is zero; signed-zero determinant compares equal.

Otherwise e=fres(det), then twoE=R(e+e), square=R(e*e), reciprocal is source
ps_nmsub(det,square,twoE). Finite algebra is 2e-det*e*e but both intermediate
roundings and fused negative-subtract instruction must be retained. There is
exactly ONE refinement. Replacing fres by division1/det is a different numeric
adapter. Required estimate/FPSCR/subnormal/special-value behavior remains an
explicit primitive provider unless implemented independently.

Multiply every cofactor by reciprocal first lane using ps_muls0. Output byte
store order: scalar Z at0c, scalar Z at1c, scalar Z at2c, pair(C00,C01) at00,
pair(C10,C11) at10, pair(C20,C21) at20, scalar C02 at08, set return r3=1,
scalar C12 at18, scalar C22 at28. Scaled cofactors are inverse-transpose basis;
translation output is Z. No epsilon or identity fallback.

Renderer8003c884 ignores status then requests GXLoadNrmMtxImm on its local output.
Thus singular failure leaves that output unmodified; silently initializing an
identity output does not match original source. Used nonsingular inputs can be
admitted separately without claiming singular behavior.

## PSMTXMultVec

r3=matrix, r4=inputXYZ, r5=outputXYZ. Snapshot vector(x,y) at6a4 and(z,1) at6ac.
For each matrix row i the arithmetic is:

p0=R(Mi0*x); p1=R(Mi1*y);
q0=F(Mi2,z,p0); q1=F(Mi3,1,p1);
outi=R(q0+q1).

This is paired grouping, not leftfold a*x+b*y+c*z+t. Even zero seed executes
all multiplies/FMA; translation is not directly copied. No calls, constants
loaded from memory, return status or validity checks.

Rows0/1 matrix values are loaded before X output store8029e6cc. Row2 first pair
loads8029e6d4 and second pair8029e6dc AFTER X output has been written; Y stores
8029e6e4 and Z8029e6f0. All three original vector inputs were already captured,
so inputXYZ==outputXYZ is supported. Do not promise arbitrary overlapping matrix
and output snapshot semantics: X store may overwrite not-yet-loaded row2.
The used renderer seed vector is disjoint from the modeling matrix.
