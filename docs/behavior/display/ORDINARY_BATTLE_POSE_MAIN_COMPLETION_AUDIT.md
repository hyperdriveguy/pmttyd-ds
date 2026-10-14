# Ordinary battle pose Main: scalar closure and next service

2026-10-14. Read-only US G8ME01 disc0 revision0 source audit of
`animPoseMain8003ed2c`, length1940 hex. This is a completion checklist for
the fresh unlinked mode1 battle-player domain, not a claim that native Main
or original rendering is already complete. Source addresses and record offsets
below are hexadecimal.

## Admitted history and actual outstanding providers

The [pool/Entry history](BATTLE_POSE_POOL_PROVISIONING_AND_TYPED_NATIVE_ADMISSION.md)
establishes a real pose/resource, mode1, linked ID R90=-1 and flags0=1.
`animInit80042854` explicitly writes work W+e8=0 before allocating shared
storage. Thus the initial ordinary output pointers are private R4c/R54;
zero work is not merely inferred from allocation. A later paper/draw publisher
can set W+e8, so this proof covers the initialized ordinary history, not every
possible reused global work state. A native projected work owner must retain
that selector history instead of silently ignoring a future alternate output.

Linked=-1 skips the only recursive Main call at8003edfc. Flags0 mask8 clear
skips the entire optional facing tail at80040548. Neither animation descriptor
M_R_1 nor the ordinary time/rate/selection/cache path sets this bit. The
[post-Main contract](../battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md)
records the original producer and camera qualifications.

In this admitted domain the complete direct-called service inventory is:
clock quotient `__div2u80264ad8`, unsigned64-to-float
`__cvt_ull_flt80264f24`, `memcpy_as480005634`, and `memcpy8000519c`.
The only other direct calls in the whole body are recursive Main, camera4
`cameraGetPtr8000f2fc`, and `atan2802725a0`, all gated out here. There is no
extra material, display, event, allocation, callback or GX service hidden after
cache interpolation. GQR writes at8003ed94..8003edb8 are platform state,
not engine-service calls.

Native ARM integer/float conversion, binary32 rounding/FMA, GQR signed-byte
decoding, tangent-table generation and typed copy/buffer operations are explicit
numerical/platform adapters. They need not be represented as fictitious engine
requests. Original FPSCR exceptional domains, GQR0 save semantics, malformed
indices, aliasing and original libm bit parity remain limits. Fresh finite
authored M_R_1 output does not establish those general domains.

## Complete scalar publication checklist

The following are all direct stores to the retained record R in this body;
ordinary output/cache writes through other pointers are separate.

| Order | Source lane and required operation | Source anchor |
| --- | --- | --- |
| 1 | R20 elapsed frames, then reload and multiply current R44 and store R20 again | 8003eea8 / 8003eeb8 |
| 2 | R84 offset/ratio, including publish/reload before loop integer conversion | 8003eedc..8003ef78 |
| 3 | R24 selected sample time | 8003ef7c |
| 4 | R28 continuous predecessor, then R2c successor; R34 independent discrete predecessor, then R38 successor | 8003efbc / 8003efc8 / 8003f004 / 8003f010 |
| 5 | Nonloop separately clamps successors; R80 is actual discrete current-row word | 8003f02c / 8003f044 / 8003f060 |
| 6 | Loop continuous successor wraps to zero when required; R30 is selected-time interval fraction or positive0 for exact zero interval | 8003f094 / 8003f0e4 / 8003f0f0 |
| 7 | Rebuild copies all five bases in source order; reread R3c and, if still -1, store0 before replaying key0..current | 8003f148..8003f1cc |
| 8 | Same-current path alone: if ordered old R40 > retained fraction, store positive0 R40 before output interpolation | comparison80040074, store80040080 |
| 9 | After all output copying/interpolation, store retained current index R3c, then retained fraction R40 | 80040538 / 8004053c |
| 10 | Only flags0 mask8 set admits R78=-1/+1 facing publication; otherwise preserve R78 | 80040540..80040644 |

No timestamp, selector, rate, flags0, linked ID, node/time reset or output
pointer publication is added by Main. In particular final R3c/R40 stores happen
even when signed current>next skips fractional output, or cubic byte3 rejects
a tangent update. They must not depend on whether visible vertices were emitted.

The retained five-channel cache must remain distinct from fractional output:
position R48 versus output R4c; auxiliary R50 versus R54; byte state R58;
node R60 versus R64; texture records R68. Rebuild/forward/equal-current
admissions and copy order follow
[cache replay](ANIMATION_COMPRESSED_DELTA_CACHE.md). Output current==next copies
position/aux/node; current>next preserves output; current<next copies current
then applies next sparse deltas and node cubic. Byte/texture stay current-key
state. See [output interpolation](ANIMATION_OUTPUT_INTERPOLATION.md).

## Native audit and concrete continuation

`source/display/battle_pose_pool.cpp` now owns retained five-channel cache
storage, original replay branches and intermediate/final3c/40 publication.
Its Main entry admits complete source metadata, private output selector0,
unlinked poses and flags8clear. The output selector is explicit native work
state initialized from animInit's actual zero writer; a later nonzero selector
remains unavailable. Optional-facing admission follows actual record bit8
after sampling. The converted assembly of hierarchy/vertices is additional
native work; Main itself only publishes sampled buffers for later draw
consumers. This fresh domain returns through declared ARM/GQR/copy/tangent
adapters without claiming general linked/paper/facing or complete GX drawing.

The next source engine call in `btlDispMain`, after local depth/alpha setup,
is `CheckStatus80127814(U,18)` at80102c00. It is already a portable packed status
query. Opaque original b_mario MODEL+c4=1 makes subsequent
`animPoseTestXLU80039c0c(handle)` false. With P204 mask100 clear the next actual
external display provider is `dispCalcZ8001086c(&depthXYZ)`, whose first call
requires camera4's real view/projection matrices. Queueing then uses
`dispEntry80010bc4` and callback `btlUnitPartsDisp80101774`; these remain
separate source display owners. Main completion does not complete them, the
automatic extra-part pose branch, the unit loop, or the footer.

## Evidence

`artifacts/recovered-temp-evidence/ordinary-main-closure.dis/.elf` contains the
complete read-only listing. Complete body SHA256:
`9083ebe5e58f3c2bd56914adfa318296291b5ca0ddc779d2bca7e6e3cbeebd26`.
`python3 tools/verify_animation_main.py` passed: two containing bodies,
1773 reviewed instructions,17 constants,five spans,two imports,65 direct caller
sites and one original serialized resource example. This verifies static source
evidence; it does not execute original Main or validate native rendering.
