# Modeling stack publication and SDK affine matrix arithmetic

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete two node
builders and PSMTXConcat0x8029dd64/0xcc,Copy0x8029dd30/0x34,
Identity0x8029dd04/0x2c,Scale0x8029e2b0/0x28,Trans0x8029e230/0x34.
Original GQR0/FPSCR/exception/denormal behavior and execution remain open.

## Push publication and final world composition

Both [joint](ANIMATION_JOINT_NODE_MATRICES.md) and
[transform](ANIMATION_TRANSFORM_NODE_MATRICES.md) read modeling level through
0x8041e89c,add1 using32-bit arithmetic,store level,then publish pointer at
0x8041e8a0=0x803d8e70+48*newLevel. Signed-low lis/addi producers are checked.
They update these globals BEFORE mask dispatch/composition helper calls.
No depth/overflow/capacity guard exists. Available symbol extent of the original
modeling stack is0x600 bytes (32 affine matrices),not an enforced bound.

At final composition RELOAD current level L. Previous pointer=base+48*(L−1),
output=base+48*L;request PSMTXConcat(previousStack,selectedLocal,output).
The function does not pop:caller group traversals decrement/repoint after visible
node work. Level0 is seeded by draw wrappers. A caller skipping a matching push
or using too-deep/cyclic data can access outside this modeled valid source domain;
do not add an alleged original guard. Stack pointer/work overrides are separate.

## PSMTXConcat snapshot and arithmetic

Inputs A r3,B r4,output C r5;3 rows x4 columns,binary32 row-major,48 bytes.
Under required GQR0 float/no-scale state,the original code loads ALL12 values
from EACH input before the FIRST C store. It supports C==A,C==B and ordinary
input/output overlap within valid memory domains. It is not implemented as
row-by-row writes with later input rereads. Stack/frame overlap or exceptions
remain unsupported domains;no runtime pointer/null checks exist.

For each row i and columns j,define R as the original single multiply result,
F as original paired-single fused multiply-add per lane. Compute:

```
u[j] = R(B[0,j]*A[i,0])
v[j] = F(B[1,j],A[i,1],u[j])
w[j] = F(B[2,j],A[i,2],v[j])
C[i,0] = w[0]
C[i,1] = w[1]
C[i,2] = F(0,A[i,3],w[2])
C[i,3] = F(1,A[i,3],w[3])
```

The final pair uses original(0,1) vector at0x8041e458. The EXTRA fused
0*translation operation for column2 remains observable in NaN/infinity/signed-
zero/exception domains;do not omit it merely because finite algebra simplifies.
Operand order/PS instruction rounding semantics remain literal;host fma is not
an original numeric oracle. Stores occur in PAIRS at byte offsets
00,10,08,18,20,28 (hex),not sequential row order. A/B snapshots precede them.

## Copy and constant writers

PSMTXCopy interleaves 8-byte pair LOAD/STORE for offsets00,08,10,18,20,28.
It does not snapshot the entire source first;partial overlap can affect later
loads. Same pointer preserves its own fields in ordinary valid float domains.
No memmove substitute or bitwise-identity guarantee for exceptional float values.

Identity,Scale,Trans write through scalar and paired float stores using original
binary32 constants1 at0x80429358 and0 at0x8042935c. Required GQR0 remains an
external platform invariant. Final matrices in ordinary float domains:

| Writer | Rows |
| --- | --- |
| Identity | (1,0,0,0);(0,1,0,0);(0,0,1,0) |
| Scale(x,y,z) | (x,0,0,0);(0,y,0,0);(0,0,z,0) |
| Trans(x,y,z) | (1,0,0,x);(0,1,0,y);(0,0,1,z) |

Preserve STORE order if alias/reentrant observers matter:
Identity pair offsets08,18,20,10,00,28;
Scale scalar00,pair04,pair0c,scalar14,pair18,pair20,scalar28,scalar2c;
Trans scalar0c,scalar1c,pair04,pair20,scalar10,scalar14,scalar18,scalar28,
scalar2c,scalar00. Offsets are hex. The original code does not assert alignment,
finite values or valid output span.

## Scope

These complete local primitive contracts close called matrix bodies for node
builders/draw wrappers that use Copy/Concat/Scale/Trans/Identity. Rotation is
specified in [SDK rotation](ANIMATION_SDK_AXIS_ROTATION.md). Group resources,
source stack invariants,render/material/GX and platform GQR0/FPSCR/libm/original
numeric/visual parity remain open. Fingerprinted instructions are static evidence.
