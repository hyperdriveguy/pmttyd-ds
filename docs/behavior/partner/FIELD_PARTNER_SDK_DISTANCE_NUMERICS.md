# SDK paired-single 3D distance used by partner target selection

US G8ME01 disc0 revision0, ID1, recovered 2026-10-12. Evidence B: complete
PSVECDistance at0x8029eeb4,size0x54,called by
[target selection](FIELD_PARTNER_LIVE_TARGET_DISTANCES.md),footmark clearing
and other recovered partner source decisions. The Gekko mnemonics come from
corrected original instructions,not newer PowerPC interpretations.

## Paired lane accumulation

Inputs A and B point to three contiguous floats. Under the required GQR0
floating-load configuration,the psq_l instructions first load(Y,Z) from+4,
subtract the pairs and square them. Then load(X,Y) from+0,subtract them and
perform paired multiply-add. Its lane0 is dx²+previous dy². ps_sum0 adds
that lane0 to the previous squared-difference lane1,dz². The other result lane
contains the squared dz and is not the returned distance.

For ordinary finite values,the arithmetic dependency is:

```
dx,dy,dz = single-result coordinate differences
y2 = paired single-result multiply(dy,dy)
z2 = paired single-result multiply(dz,dz)
xy = paired fused multiply-add(dx,dx,y2)
q = paired single-result add(xy,z2)
```

Preserve the paired-single operation/environment rules,not a host double
sum-of-three-squares. As a public semantic cross-check,
[Dolphin's paired interpreter](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_Paired.cpp)
implements ps_sum0 as its A lane0 plus B lane1,and models paired multiplication
with operand precision and FPSCR-dependent single-result behavior. This source
was checked2026-10-12;it is supporting emulator evidence,not a TTYD execution
trace or proof that every hardware/numeric domain has been reproduced. No
Dolphin implementation was copied into native game code.

## Zero branch and single refinement

Load binary32 half0.5 at0x80429394 and derive zero by fsubs(half,half).
If fcmpu classifies q equal to that zero,return q directly. NaN is unordered
and does not take the equality return. Unlike the separately recovered
[direction-query norm](FIELD_PARTNER_SPEED_NUMERICS.md),this body has no
ordered-positive admission or canonical-NaN classification branch.

Otherwise load3.0 at0x80429398 and execute:

```
r = frsqrte(q)
u = single-result multiply(r,r)
v = single-result multiply(r,0.5)
w = single fused negative multiply-subtract(u,q,3.0)  // 3.0-u*q
r1 = single-result multiply(w,v)
result = single-result multiply(q,r1)
```

There is **one single-precision refinement**,not three double refinements.
The initial estimate uses the hardware instruction;its bit-exact seed and
GQR0/FPSCR/exception/denormal state remain platform evidence requirements.
Negative,NaN and infinite inputs are not sanitized by additional local branches.
Replacing the routine with host sqrt,or sharing the different direction-query
normalization policy,would change the documented numeric path.

## Verification and remaining work

tools/verify_field_partner_targets.py pins the complete84-byte body,paired loads/
subtraction/multiply/madd/sum,the zero branch,estimate/refinement and exact
0.5/3.0 constants. Whole-byte checks identify evidence;they do not run the
original CPU or validate a chosen emulator numeric backend. The tool also
checks five target/footmark bodies and actual caller anchors.

Next:GQR/FPSCR startup/writers,frsqrte seed and supported numeric domains;
planar distance/angle/trig helpers and original captured target-distance outputs.
These remaining requirements prevent a claim of bit-exact numeric equivalence.
