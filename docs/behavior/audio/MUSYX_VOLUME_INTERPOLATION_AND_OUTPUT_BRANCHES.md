# MusyX volume interpolation and output branches

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salCalcVolume REGIONS
8028b898..8028bb10 and8028bb10..8028bd0c, ends exclusive. Together with
[coordinate/phase preparation](MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md)
and ordinary ABI save/restore, these close the local salCalcVolume8028b694/6f0
body. Math children/FPSCR/input/alias/runtime domains remain separate.
Offsets/addresses hexadecimal; indices/shifts decimal. Read
[hardware volume consumer](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md)
and [unsigned conversion](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md).
Source arithmetic, not original execution or measured/audible channel roles.

## Inputs and exact arithmetic notation

Retained gains A/C/E are the original f1/f2/f3. O is output pointer; G is the
selected129-float volume table. x/y are prepared coordinates; xc/yc are their
single-rounded2-x/2-y complements. The preparation contract records each unsigned
index i and fractional phase f separately, using conversion of the coordinate
and frsp of fmod(coordinate,DOUBLE1). They are NOT recomputed with host floor.
For nonzero T, xPre/xcPre indices/phases were saved before optional x compression.

Use add/sub/mul to mean a SINGLE fadds/fsubs/fmuls operation with its rounding;
never fused multiply-add or widened/reassociated products. Loads are original
binary32 values. A displayed formula describes value flow under stable,
nonoverlapping inputs/tables/output; table loads still occur at their original
sites and are not globally cached or made atomic by the notation.

Pan table P=pan_tab803be65c: four floats
0,0.7078999876976013,1,1. Extra table D=pan_tab_dpl2803be66c:
0.574999988079071,0.7078999876976013,1,1. Both are10hex bytes.
For a retained coordinate's index i and phase f define:

`H(i,f) = add(mul(sub(1,f),P[i]),mul(f,P[i+1]))`

This applies separately to x,y,xc,yc. Integer index offsets use wrap32(i<<2),
then pointer addition; the next sample is loaded at+4. No local index/table-extent
or finite/range guard. Do not infer safe lookup for arbitrary conversion results.

## Volume interpolation shared by both branches

For admitted gain g, compute z=mul(original127.0 at804291f8,g). Request
__cvt_fp2unsigned(z) to get raw i. Build unsigned double43300000/i, SINGLE
subtract bias4503599627370496 at80429208 to produce single ci. Fraction
f=sub(z,ci), complement sub(original1.0 at804291fc,f). Read G[i] and G[i+1];
value Vg=add(mul(complement,G[i]),mul(f,G[i+1])). Separate rounding at every
step, no final clamp or fctiwz conversion here. i is captured child return,
not an assumed integer floor. Binary32 conversion of large i can affect f.

The selected G is musyx_vol_tab803be458 or dspDLSVolTab803bc7e4, each129 floats.
Volume interpolation always reads a next sample; table index128 alone is not
sufficient for a valid pair. The hardware caller upper-clamps gains to1 but
retains negative/unordered values; valid lower/FP/index domains remain open.
No local table/domain repair follows from the complete mixer body.

## Complete T==0 branch: all three gains and nine stores

Convert/interpolate A, then C, then E. C's scaled127*C is captured before the
first output group; E's scaled127*E before the second. The later conversion
calls use those captured scaled values, independent of prior output stores.
For g=A,C,E with group byte base b=0,0c,18 respectively:

1. Store O[b+08]=mul(original0.7078999876976013 at80429200,mul(Vg,H(y))).
2. Form W=mul(Vg,H(yc)); store O[b+04]=mul(W,H(x)).
3. Store O[b+00]=mul(W,H(xc)).

Store order:08,04,00,14,10,0c,20,1c,18. W is retained across the second/third
stores, not recomputed with a fresh Vg. Pan y samples are loaded before the first
store of each group; yc/x samples afterward; xc samples after the second store.
Fraction complements are retained across groups, but pan samples are freshly
loaded for each group. Output/table aliasing can therefore change later reads;
these formulas do not authorize caching one H per call under arbitrary aliases.
No output integer conversion or normalization beyond stfs publication.
After the last store, branch to the shared epilogue at8028bd0c.

## Complete T!=0 branch: two gains and cross-table extra output

Convert/interpolate A first; derive U=mul(VA,H(y)), W=mul(VA,H(yc)).
In order:

1. O[04]=mul(W,H(x)); O[00]=mul(W,H(xc)).
2. O[1c]=mul(U,J(xPre)); O[18]=mul(U,J(xcPre)), with J below.
3. Convert/interpolate C and write O[14]=mul(original0.7078999876976013,
   mul(VC,H(y))); O[10]=mul(mul(VC,H(yc)),H(x));
   O[0c]=mul(mul(VC,H(yc)),H(xc)).
4. Store original float0 at80429228 to O[08], then O[20].

Exact store order:04,00,1c,18,14,10,0c,08,20. There is NO third gain E volume
conversion or contribution in this branch. Slots18/1c derive from A, rather
than E; slots08/20 are always explicitly zero. These offsets describe original
output data flow, not independently measured speaker/aux labels.

For saved pre-compression coordinate index i/phase f:

`J(i,f) = add(mul(sub(1,f),D[i]),mul(f,P[i+1]))`

The LOWER sample comes from D, but the UPPER sample comes from normal P+4,
not D+4. At8028bc10/2c, r3 addresses D; the other load at8028bc14/30 uses
retained r21=P+4. Preserve this cross-table pairing, even if another interpolation
rule appears more conventional. The corresponding complements use saved phases;
post-compression H(x) and pre-compression J(xPre) can differ in the same call.

H(y)/H(yc)/H(x) samples for the A group load before O04. H(xc) loads after O04;
J(xPre) after O00; J(xcPre) after O1c. C's scaled127*C is captured before O04; its conversion/interpolation starts
after O18. Its pan sample/write sequence follows the ordinary group above.
No output/table alias, concurrent table or child mutation guarantee is added.

## Composed body, evidence and remaining source/runtime

Both branches join the ordinary floating/integer register-restoration epilogue;
there is no independently assigned scalar status. The local body is now composed
from complete parameter/phase and interpolation/output regions, plus ABI
save/restore. This does not close fmod's IEEE child, arbitrary conversion/FP
exceptions, physical update cadence or audible output.

[Manifest](../../../tools/musyx_volume_interpolation_outputs_evidence.json) pins
whole salCalcVolume, two output regions, four linked children, four numeric tables
and five literals:16 spans,337 anchors/19 calls. Two129-entry volume and two4-entry
pan tables retained. Eight binary32 formula vectors cover both tables/branches,
zero/half/full gains, boundary/interior synthetic prepared coordinates and paired
E changes showing the nonzero branch ignores E. These are deductions for stable,
nonaliasing finite fixtures; they are not original instruction execution, reachable
coordinate proofs or complete mixer integration tests. Store-order metadata uses
byte offsets. Full fresh ELF/listing remains `/tmp`; no bodies/binary assets committed.

Remaining: IEEE remainder/math/FPSCR/transitive execution, original gain/
coordinate/index/table/output/alias/lifetime/IRQ/concurrency domains, DSP ramp/
command consumers and original mixer/volume/DSP/audio traces. Local mixer body
answered; full native/runtime binding and EVT-01 remain OPEN.
