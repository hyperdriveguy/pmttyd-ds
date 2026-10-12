# System angleABf: quantized coefficient table,quadrants and zero vector

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete local
angleABf@system.o0x8000703c,size0x1b4. Another same-name body exists in
pmario_sound.o at0x800dcbc8;this contract binds the SYSTEM call target used by
field movement/slit normals. Namespace-filtered inspection preserved the original
annotation table and original instruction bytes.

Inputs f1/f2=(X1,Z1),f3/f4=(X2,Z2). Compute dx=round(X2-X1),dz=round(Z2-Z1)
with fsubs,then ax=fabs(dx),az=fabs(dz). Ordered ax>az selects X-dominant;
every other comparison,including equality/unordered,selects Z-dominant.
No atan/atan2/libm service is called by this leaf.

## Coefficient lookup and index conversion

X-dominant computes r=round(az/ax);Z-dominant first checks az equal0 using
fcmpu and RETURNS positive0 on equality. Otherwise r=round(ax/az).
For the selected ratio,compute t=round(45*r),then n=round(2*t). Form signed
index I with original DOUBLE arithmetic and fctiwz:

- Ordered n>=0: I=fctiwz(double0.5+n).
- Otherwise: I=32-bit negation of fctiwz(double0.5-n).

The bias double is0x802bf1f8. No bounds clamp or finite-value check occurs.
Address the table with original32-bit I<<2 at0x803041e0,load its binary32
coefficient C[I],then theta=round(t*C[I]) with fmuls. This is a table-scaled
ratio algorithm,not host atan2 followed by rounding. Reuse the original91-word
angleABTBL span(size0x16c) for a port requiring those coefficients;the verifier
pins its SHA-256 without distributing an extracted table asset.

For ordinary finite nonzero vectors,r lies in[0,1],t in[0,45],index0..90;
these ranges are a deduction from the branch/division,not executed domain proof.
Table0 and table90 are1.0;other coefficients retain their original words.
Exceptional conversion/address/NaN/FPSCR domains must follow original rules or
have source-domain proof;do not sanitize them into a guessed angle.

## Exact quadrant operations

All comparisons below are ORDERED >=0;unordered follows the false alternative.
Every plus/minus is a single-result fadds/fsubs,with no post-result wrap.

| Dominant | dx>=0 | dz>=0 | Return |
| --- | --- | --- | --- |
| X | true | true | round(90+theta) |
| X | true | false | round(90-theta) |
| X | false | true | round(270-theta) |
| X | false | false | round(270+theta) |
| Z | true | true | round(180-theta) |
| Z | false | true | round(180+theta) |
| Z | true | false | theta directly |
| Z | false | false | round(360-theta) |

Finite coincident coordinates give dx=dz=0,enter Z-dominant,then az==0 returns
positive0 WITHOUT table access. This closes the previously open zero-vector
request in [Vivian's trailing callback](FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md):
after its root copy,the ordinary coincident-domain call requests angle0 and
movePos(30,0),rather than the pre-copy direction. Underlying movePos/libm/alias
numeric domains remain separate.

There is no total zero/NaN classifier. For example,Z-dominant az==0 may also
return0 when an unordered X magnitude prevented X-dominant selection. Conversely
very small negative dx with negative dz can produce a rounded theta0 and return
360 rather than wrap it to0. Source/exception/underflow/FP domains remain open.
Those literal branches must not be erased by conventional angle normalization.

In [slit collision](FIELD_PARTNER_SLIT_COLLISION.md),this helper receives raw
normal XZ for each side,and its TWO returned representations enter an unwrapped
arithmetic-average predicate. That is why the table/quadrant/rounding contract
matters beyond visually similar motion. Static body/constant/table checks do
not establish bit-identical host arithmetic or original observed trajectories.
