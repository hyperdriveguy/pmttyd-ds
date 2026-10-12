# Animation Main entry,linked evaluation and frame time

US G8ME01 disc0 revision0;ID1,2026-10-12. Evidence B:complete local
animPoseMain0x8003ed2c,size0x1940. The six Main/init documents divide its local
graph,not six independent execution phases. Original execution and all called
resource/clock/helper/copy/camera/libm services remain separate.
[Partner pre-display](../partner/FIELD_PARTNER_PRE_DISPLAY.md) actually calls
Main at800b7b5c;Mario pre-display at800579f4. Draw and paper-selector consumers
can call it too:there is no local once-per-simulation-tick admission guard.

## Capture and platform writes before any animation-null gate

Load W through0x80415dc0. Capture pool B=word[W+10],resource array
J=word[W+0],R=B+0x170*inputID. Read resource index R+10 and animation index
R+14;resolve E=word[J+16*index+8],H=word[E+a0],D=word[H]. Animation descriptor
A=word[D+1ac]+64*animationIndex;T=word[A+3c]. No local ID/index/active/pointer
guard precedes these accesses. This is the loaded address layout,not the
serialized relative-offset representation.

Write hardware GQR2=0x00040004,GQR3=0x00050005,GQR4=0x00060006,
GQR5=0x00070007 via SPR914..917 at8003ed94..8003edb8. No prior GQR2..5
values are saved/restored by Main. Quantized delta loads use index4,therefore
SIGNED8 values with load scale0;index4 is not quantization TYPE4. Scalar W1
loads one element. GQR0 remains an external assumption for register saves.
The interpretation is checked against [Dolphin's GQR definition](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Gekko.h)
and [quantized-load implementation](https://raw.githubusercontent.com/dolphin-emu/dolphin/master/Source/Core/Core/PowerPC/Interpreter/Interpreter_LoadStorePaired.cpp).

Capture output vector pointers BEFORE linked recursion:if word[W+e8]!=0,
Opos=word[W+f8],Oaux=word[W+fc];otherwise Opos=word[R+4c],Oaux=word[R+54].
Global buffers and per-pose buffers are alternate outputs,not separate animation
sources. Their ownership/capacity and W+e8 writer domains remain open.

Reload linked ID R+90;if !=−1,read linked+4 through CAPTURED pool B. If mask2
CLEAR,recursively call animPoseMain(linkedID) at8003edfc;SET skips it.
There is no local visited-set,cycle test,depth limit or same-ID guard. Recursive
resource and output writes occur before evaluating the parent. Preserve captured
R,D,T and output pointers;do not silently refresh all pointers after recursion.

Only AFTER these steps,T==null returns to epilogue80040648. Null T therefore
does not skip resource resolution,GQR writes or admitted linked recursion;it
skips parent time/sample/cache/interpolation/facing work. No local fallback pose.

## Elapsed time and local rate

For nonnull T,L=word[T+24] and captured loop selector C=word[L+0]. Request
clock quotient Q using CURRENT R+c and the literal hardware divisor described in
[local-time/clock setters](ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md).
Subtract current timestamp HIGH R+18/LOW R+1c from Q with low `subc` and high
`subfe`;pass the64-bit difference to __cvt_ull_flt0x80264f24. The conversion is
an unsigned64-bit-to-float request,not signed elapsed-time clamping.

Let F be its returned floating value. Compute fmuls60*F then fdivs1000 and
store binary32 R+20. Reload R+20 and current rate R+44;fmuls and store R+20
again. Those stores and arithmetic rounding points are distinct. Constants60
and1000 are original words0x42700000/0x447a0000. They specify this conversion;
no observed renderer cadence or hardware performance is inferred.

## Span,loop fraction and selected sample time

Reload L through T+24. S=float[L+4],E=float[L+8],H=round32(E−S),
X=current float[R+20]. All arithmetic below is single;comparisons are ordered
except the explicit equality-with-zero `fcmpu`. Let Z be selected sample time:

| Condition | R+84 publication | Z |
| --- | --- | --- |
| H==0 | round32(X−S) | S |
| H!=0,C!=0,round32(X−S)<0 | positive0 | S |
| H!=0,C!=0,otherwise | V=round32(round32(X−S)/H) | FMA subtraction X−I*H |
| H!=0,C==0,X<S | positive0 | S |
| H!=0,C==0,X>E | round32(round32(X−S)/H) | E |
| H!=0,C==0,otherwise | round32(round32(X−S)/H) | X |

Loop branch I is `fctiwz` of the RELOADED R+84,then signed integer→float via
bias0x802c1070=0x4330000080000000. Z uses `fnmsubs(I,H,X)`:
one single fused subtraction X−I*H. Do not substitute fmod,integer division,
separate multiply/subtract,or a clamp of R+84 to1. R+84 is a raw ratio and can
exceed1 even for nonlooping animations. The zero-span path stores an offset,
not a ratio,and fixes Z to S;it has priority over C. NaN comparisons do not
become an invented validity failure. Integer-conversion edge domains remain open.
Finally store Z to R+24,then perform
[timeline sampling](ANIMATION_TIMELINE_SAMPLING.md).

## Validation and boundaries

`python3 tools/verify_animation_main.py` checks two complete containing bodies
(Main/init),1773 reviewed instruction words,17 constants,five data spans,two
lecture imports,65 direct DOL sites and one original serialized Mario example.
This does not prove all sites are reached in US play or validate live resources,
clock/pool/recursion invariants,FPSCR or original trajectories. Main's local
[cache replay](ANIMATION_COMPRESSED_DELTA_CACHE.md),
[interpolation](ANIMATION_OUTPUT_INTERPOLATION.md) and
[optional facing tail](ANIMATION_MAIN_CAMERA_FACING.md) are covered separately.
