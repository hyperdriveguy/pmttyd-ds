# SDK axis rotation:trig wrappers,case folding and output layout

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete
PSMTXRotRad0x8029dff0/0x70,PSMTXRotTrig0x8029e060/0xb0,
sinf0x80272888/0x24,cosf0x802728ac/0x24. No original execution.
[Node builders](ANIMATION_JOINT_NODE_MATRICES.md) pass radians already SINGLE
scaled by0x3c8efa35;this SDK wrapper does not itself multiply degrees or wrap.

## Request order and round points

PSMTXRotRad(output r3,axis r4,radians f1) captures those inputs. Call sinf
FIRST with original radians,retain its return;call cosf SECOND with the same
original radians. Then call PSMTXRotTrig(output,axis,f1=sinReturn,f2=cosReturn).
No axis validation before BOTH trig calls. sinf calls original double sin
0x80272410 and frsp return;cosf calls double cos0x80271ea8 and frsp return.
Those underlying libm bodies/numeric domains remain open. No invented hardware
sincos instruction or proof of equivalence with a host trig library.

PSMTXRotTrig FIRST frsp's both incoming sine and cosine again;loads original
float0/1 constants,then case-folds RAW axis word by bitwise OR0x20. It accepts
only exact resulting unsigned word'x'(120),'y'(121),'z'(122). Typical uppercase
X/Y/Z therefore work,but high bits/other values remain invalid. Invalid axis
returns WITHOUT matrix stores,after those conversions/constant loads. The
Rad wrapper already performed both trig calls even for an invalid axis.

## Output matrices and paired stores

Let s=rounded sine,c=rounded cosine. Under required GQR0 float/no-scale state:

| Axis | Affine rows |
| --- | --- |
| x | (1,0,0,0);(0,c,−s,0);(0,s,c,0) |
| y | (c,0,s,0);(0,1,0,0);(−s,0,c,0) |
| z | (c,−s,0,0);(s,c,0,0);(0,0,1,0) |

Negation is original paired ps_neg;merge/store instructions preserve lane order.
There is no local sin²+cos² normalization or determinant correction. Output
covers48 bytes;no null/alignment/finite checks. Literal store offsets (hex):
x:scalar00,pair04,pair0c,pair1c,scalar2c,pair24,pair14;
y:pair18,pair00,pair28,pair10,pair08,pair20;
z:pair08,pair18,pair20,pair10,pair00,pair28.
Constant loads and register-only work precede these stores;the function does not
read existing output values. Original paired-single/FPSCR/GQR0 exception/denormal
and memory behavior remain external requirements.

## Reimplementation boundaries

Node requests append individual axes to an accumulated matrix with original
[paired concat arithmetic](ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md).
Skipping zero angles occurs in the node builders,not in this wrapper. Double
libm plus explicit single round points are part of the request graph;the docs
have not measured original images or promised bit equality for host sin/cos.
`verify_animation_node_matrices.py` pins complete caller/SDK/wrapper bodies,
constant and dispatch spans,address producers and direct builder bindings.
