# Partner floor slope pipeline and partial metadata writes

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
local pipelines inside partySearchGround0x800c3bcc. First block0x800c3f00–40d4,
second0x800c40e0–42a8. Full floor orchestration is in
[floor query](FIELD_PARTNER_FLOOR_QUERY.md). Called angleABf0x8000703c and
CPU/library numerical environments remain separate contracts.

Let original normal be(Nx,Ny,Nz),R a single-result rounding,T the original
fctiwz signed-word conversion followed by integer-to-float conversion. Each
pipeline performs these ordered requests/stores:

1. Request angleABf(0,0,R(100*Nz),R(100*Ny));compute R(result-180),convert
   with T,and store that float at P+c4.
2. Store original float0 at P+c8.
3. Request angleABf(0,0,R(100*Nx),R(100*Ny));compute R(result-180),convert
   with T,and store that float at P+cc. Input scales are separate single
   multiplies;no wrapped-angle normalization is added here.
4. Compute q=R(R(Nx*Nx)+R(Nz*Nz));obtain local horizontal magnitude m with
   the rule below. Request angleABf(0,0,m,-Ny). This is inclination I.

These fields are angle metadata,not an XYZ normal or Euler transform reconstructed
from the retained support. First execution publishes projected angles even
when I is rejected at ordered>=50,leaving P+e0 unchanged. For a candidate
that passes that first test,the second execution repeats all three angle
requests and publishes P+e0=its returned I before vertical acceptance. There
is no second inclination>=50 rejection. Missed rays write no slope metadata.
An early flag return bypasses the entry clear too. Bind every field's actual
writer rather than copying one retained support record into all four fields.

## Horizontal magnitude: three double refinements

For ordered q>0,use frsqrte(q),then three double-precision Newton refinements
with exact double half at0x802cb880 and3 at0x802cb888. Each refinement squares
the estimate,multiplies it by half,uses fused negative multiply-subtract for
3-q*r²,and multiplies by the half-estimate. The final multiply q*estimate is
double,then frsp produces m. This is not the SDK paired distance routine's
single refinement,not host hypot,and not a fused single sum of normal squares.

For the nonpositive/unordered route,compare q with original double zero at
0x802cb890. Ordered negative q loads canonical binary32 NaN0x7fffffff from
0x8041e3c8. Otherwise classify the binary32 exponent/fraction:NaN loads that
same canonical value;zero,subnormal and other classifications retain q. No
positive-infinity sanitization precedes the ordered-positive path. This describes
literal local decisions,not proof that invalid normals occur in valid gameplay.
frsqrte seed,FPSCR rounding/exception/denormal state and angle helper domains
remain required evidence for bit parity.

## Height quantization is separate

The floor query's quantizer reads pointY,not normal or ray distance. Use original
1000 factor,ordered-y>=0 selects+.5 and otherwise-.5,one fused single-result
multiply-add,fctiwz,and original signed-word conversion/division by1000.
The bias double0x4330000080000000 lives at0x802cb878. Negative and NaN conversion
semantics must follow the target CPU;do not silently clamp or substitute a
language round function. The slope T conversion instead truncates result-180
to a whole signed integer and stores its float representation,with no1000 scale.

tools/verify_field_partner_ground.py pins both pipeline entry/call/store/reject
and refinement/classification regions plus the exact constant spans. Whole-body
identity is not an execution oracle. Original normal/point/slope outputs and
supported numeric domains remain open,as do full scene traversal and cache
consumers. Deduced tests should distinguish first-only steep partial writes,
two executions for admitted normals,and slope ownership from support ownership.
