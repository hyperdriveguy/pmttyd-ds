# MusyX volume coordinate and phase preparation

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete salCalcVolume REGION
8028b708..8028b898 exclusive; whole8028b694/6f0hex fingerprinted but
interpolation/output branches remain open. Complete fmod802725c0/20hex wrapper;
its __ieee754_fmod child is linked only. Addresses/masks hexadecimal;
shifts/counts decimal. Read [volume caller](MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md)
and [unsigned conversion](MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md).
Static local arithmetic/request order, not measured mixing or audible output.

## Retained inputs and volume-table selection

Entry integer arguments r3=B,r4=O,r5=P,r6=K,r7=L,r8=T; floats f1=A,f2=C,f3=E.
O points to nine float outputs in the later regions. Retain floats and O/L/T.
If low8(B)==0 select musyx_vol_tab803be458; otherwise dspDLSVolTab803bc7e4.
Thus B256 takes the first table despite being nonzero as32 bits. Both original
tables have129 floats/204hex bytes. Independently retain musyx_vol_tab as the
base for later pan-table references. No table lookup/output store occurs in
this preparation region; selecting a base does not establish valid later indices.
No float clamp, finite-value test, pointer/buffer/type/IRQ gate here. The caller
has its own upper clamp; other callers and value domains remain open.

## Exact sentinel and unsigned coordinate transform

If RAW P==00800000, replace P=0 and K=007f0000, irrespective of incoming K.
No sentinel test on K or low16(P). Otherwise retain both raw words.
For each value V=P then K, the emitted subc/addze/andc sequence is:

`carry = 1 if unsigned V <= 00010000 else 0`
`incremented = wrap32(V+carry)`
`mask = wrap32(V-incremented)`
`adjusted = wrap32(V-00010000) & ~mask`

This equals max(unsigned V-65536,0) over all32-bit V. It is a lower floor
followed by subtraction, WITHOUT an upper clamp or signed-value conversion.
Values above007f0000 can produce coordinates beyond2; raw ffffffff is admitted.
The sentinel therefore yields adjusted P0 and K007e0000 rather than two zeros.

Convert each adjusted unsigned word through a double constructed with high
43300000 and the word as low, then SINGLE fsubs with original unsigned bias
4503599627370496 at80429208. SINGLE fmuls by original coefficient
2.422030149773491e-07 at80429210 yields x and y respectively. Single rounding
at both operations is part of the local order. For sentinel, x=0,y=2.
No division by65536, signed bias or duration/time conversion occurs here.

## Optional saved phases before coordinate compression

If raw T!=0, perform in this order:

1. fmod(x,original DOUBLE1.0 at80429218), round returned double with frsp;
   retain phase fxPre and independently request __cvt_fp2unsigned(x), retain ixPre.
2. SINGLE xcPre=original FLOAT2.0 at80429220 minus x; fmod(xcPre,DOUBLE1.0),
   frsp return into fxcPre; independently convert xcPre into ixcPre.

If T==0 skip all four requests; these saved fields/registers are not initialized
by this branch. They are only consumed by the later T!=0 output branch under
ordinary unmodified retained arguments. A remainder request does not replace
x/xcPre or feed the integer conversion with its returned phase.

Then if raw L!=0, compress x using three separate SINGLE operations:

`d = fsubs(x,FLOAT1.0 at804291fc)`
`m = fmuls(original FLOAT0.766040027141571 at80429224,d)`
`x = fadds(FLOAT1.0,m)`

Zero L leaves x unchanged. y is never compressed here. Saved T phases/indices
refer to PRE-compression x, while the common phases below use post-compression
x. Do not reorder these branches or recompute saved phases afterward.
No boolean low-byte normalization on L/T; any nonzero32-bit value admits them.

## Common four phase/index pairs and branch exit

In order, prepare:

| Coordinate | Fraction request | Integer request |
| --- | --- | --- |
| Post-compression x | fmod(x,DOUBLE1), then frsp | __cvt_fp2unsigned(x) |
| y | fmod(y,DOUBLE1), then frsp | __cvt_fp2unsigned(y) |
| xc=FLOAT2-x using fsubs | fmod(xc,DOUBLE1), then frsp | __cvt_fp2unsigned(xc) |
| yc=FLOAT2-y using fsubs | fmod(yc,DOUBLE1), then frsp | __cvt_fp2unsigned(yc) |

Both complements are computed after the x/y pairs, before the xc/yc requests.
Each conversion uses its coordinate independently of the fractional result.
No local index sign/range/129-table-extent checks, finite/NaN/overflow repair or
request-status checks. The unsigned helper's already-recovered semantics matter;
its returned index cannot be inferred as floor for arbitrary out-of-domain FP.
The actual IEEE remainder child/FPSCR behavior remains open, rather than being
substituted with host modulo or treating double fmod as single arithmetic.

Finally test retained raw T: nonzero branches to8028bb10, zero falls into
8028b898. These still-open output branches consume the retained floats/table
bases/phases/indices. No output write or independent scalar status is completed
by this region. The branch names must not be treated as measured channel roles.

## Complete remainder wrapper and evidence

fmod802725c0 saves/restores linkage, requests __ieee754_fmod8026f6bc with
incoming floating arguments unchanged, and returns its floating result unchanged.
No local frsp, argument clamp, errno/status adjustment or remainder algorithm.
The frsp operations above belong to the mixer after each wrapper return.
Fingerprinting the child does not close its exceptional/normal semantics.

[Manifest](../../../tools/musyx_volume_coordinate_phase_preparation_evidence.json)
pins one complete region/one complete wrapper/three linked body fingerprints,
two129-float tables/six numeric literals:13 spans,108 anchors/13 direct calls.
Seven raw-coordinate/sentinel/carry vectors, six binary32 compression vectors
and six low-byte table-selection vectors are deductions, not original execution
or complete mixer-output tests. All258 original table floats retained as numeric
metadata. Full fresh ELFs/listings remain `/tmp`; no bodies/binary assets committed.

Remaining: both complete table interpolation/output branches, original pan tables/
float literal providers, __ieee754_fmod/math/FPSCR/transitive execution, actual
coordinate/volume/table/output/alias/lifetime/IRQ/concurrency domains and original
mixer/volume/DSP/audio traces. Parameter/phase preparation and wrapper answered;
full salCalcVolume/native binding and EVT-01 remain OPEN.
