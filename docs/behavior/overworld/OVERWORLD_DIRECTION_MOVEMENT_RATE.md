# Mario direction-dependent movement rate

US G8ME01 disc0 revision0, recovered2026-10-12 from the read-only ISO.
Static evidence, not original gameplay timing, numerical comparison or a measured
Rogueport camera yaw. Verify six containing bodies,27 instruction anchors and
eight constants with `python3 tools/verify_overworld_direction_rate.py`.

## Movement equation and caller ordering

`marioGetMoveRate`0x800969a4 takes a heading float H. It fetches Mario+19c offsetO,
then performs separate binary32 subtraction, multiplication and division:

```
delta = single(H-O)
radians = single(single(pi*delta)/180)
c = single(original_cos(radians))
rate = fma(0.19000005722045898, abs(c), 1)
```

Constants pi=0x80420a9c (binary32),180=80420a70,1=80420a74,
coefficient=80420aa0 (word3e428f60). Subtract at800969c8, multiply/divide at
969d0/d4, cosine request969d8, frsp969dc, abs969e8 and fused result969ec.
The helper neither reads support/slope nor clamps/normalizes its H argument.

`marioMoveMain` caps ordinary air norm to dash-speed*scale at8005994c–960.
Then, unless main MarioFlags&0x01000000, it loads RETAINED travel heading+1a4,
requests rate at80059970 and multiplies the already combined/capped norm at
80059974. It stores norm+18c at80059988 before wall collision. This is neither
an input-magnitude gain nor a rate based on combined air-displacement heading.
Special external movement routes can bypass this whole normal path separately.

The rate has no gate requiring nonzero retained base+180. A zero-base jump whose
current stick creates drift still uses retained heading+1a4 as the rate input;
keep that retained heading independently of base speed and current input.

At offset0 and original unit heading u(H)=(sin(H),-cos(H)), the ideal directional
expression is1+0.19000005722045898*abs(u.z). Depth H0/180 gains approximately19%;
X directions H90/270 approximately1;45-degree diagonal gains approximately13.44%.
Original cosine rounding can leave a tiny residual at90 degrees; vector-based
native absZ is an explicit trigonometric/numerical adaptation.

For plain depth dash base2.25, applying rate AFTER the dash cap yields
approximately2.6775 original units (.026775 native scale.01). Reapplying dash
cap after this gain changes the source ordering and cancels the intended gain.
These are source-derived arithmetic examples, not observed original distances.

## Heading-offset producer

`marioMain` requests `marioPreCamera`801a938c at8005a9dc, before key publication,
stick query and motion dispatch. The leaf fetches camera index4 and reads its
+114 heading/yaw twice. First read chooses +.5 for ordered nonnegative values,
-.5 otherwise; the second supplies the value being rounded. It computes
`float(fctiwz(fma(1000, cameraYaw, signHalf)))/1000`, calls revise36080096564,
then fetches Mario and stores result+19c at801a9418. Camera changes between those
reads belong to the camera service, not an invented single-read cache contract.

`marioGetStick` separately adds this offset to its current input angle at
80095c54–c5c and wraps it. Thus retained field heading can be in world axes while
movement rate compares it with camera-owned offset. Offset is not a support
normal or stored floor slope. The direct DOL stfs+19c census found this Mario
owner; other same-offset stores belong to camera, battle, UI or effect records.
That census does not exclude pointer/bulk/REL writers of Mario storage.

Native camera offset0 is a reasonable explicitly selected slice domain, but
this local recovery does NOT prove original Rogueport camera yaw is constantly0
or close its camera-zone/script producers. Bind actual camera ownership before
claiming the gain follows all original camera rotations.

## Native binding boundary

A portable service should take retainedHeading and current camera offset,
perform the ordered original-service math, and return the rate without changing
travel/base/camera. The native plain offset0 unit-vector adapter can apply
`fma(coefficient, abs(retainedHeadingZ), 1)` to full displacement after air cap
and before the directional wall solver. Preserve geometry unit scale consistently.

Slope reductions earlier in marioMoveMain, support/front-step records and
motion12 retained travel are separate dependencies, being documented through
the continued overworld request. Tests of this gain should cover axial/diagonal
motion, retained travel versus differing current air input, zero-base drift,
post-cap ordering and the explicit roll bypass. Original trig/Gekko equivalence,
camera defaults/writers and runtime traversal comparison remain open.
