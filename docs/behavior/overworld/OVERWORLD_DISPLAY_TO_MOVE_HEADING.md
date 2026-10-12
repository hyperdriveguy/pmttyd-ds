# Original display-direction to movement/probe heading

US G8ME01 revision0, toMovedir800963fc/168 and ordinary Mario callers.
Static read-only source contract; no runtime/numeric parity claim. Complements
[retained display direction](OVERWORLD_MARIO_DISPLAY_DIRECTION.md).

## Exact ordinary conversion

Input is float D supplied by the caller. Global Mario provides offset19c and
main flags. With **main00100000 clear**, D is not replaced or initially wrapped:

    a = single(single(270 - D) + Mario19c)
    bias = +.5 if a >= 0 else -.5
    q = single(trunc_signed32(fma(1000, a, bias)) / 1000)
    while q < 0: q = single(q + 360)
    while q >= 360: q = single(q - 360)
    return q

270 is word43870000 at80420a90;1000 is447a0000 at80420a94; bias words3f000000/
bf000000 at80420a88/8c. Field loads/arithmetic800964b4–51c, repeated wrap
80096520–544. Source uses fmadds followed by fctiwz, not a whole-degree heading
round or floor before normalization. Quantization happens **before** wrap.

At offset0: D0→270(−X),D180→90(+X),D26.25→243.75,D−30→300,D210→60.
The intermediate source turn angle therefore changes probe direction; replacing
it with a binary X flip or a camera-facing quad does not preserve this owner.
Use original degree convention u(H)=(sin(H),−cos(H)), retaining actual source
camera offset. Native trig/float operations remain numerical adaptations.

Only finite ordinary D/offset in the signed conversion range are closed here;
NaN/overflow/nontermination are not repaired with an invented source loop cap.

## Main00100000 branch

When this bit is set at80096418–20, the helper **ignores argument D** and reads
Mario1ac at80096428. It chooses effectiveD as follows:

- Exact source current1ac280 or460 sets effectiveD0 (80096424–44).
- Exact260 or440 sets effectiveD180 (80096448–64).
- Otherwise repeatedly wrap actual1ac into0..360; if wrapped>=180 use
  effectiveD0, else effectiveD180 (80096468–b0).

It then runs the same270−effectiveD+offset, signed Q1000 and wrap sequence.
All literals are pinned, including280/460/260/440 at80420a78/7c/80/84. This is
an explicit main-bit input; do not infer it from animation name or generic
X mirroring. Shared/global Mario fields are service inputs even when a separate
actor happens to call this helper.

## Ordinary caller binding

- marioMoveMain zero combined norm loads display1ac and calls at8005992c–930.
  It retains this H even with requested norm0; wall metadata's S0 branch does
  not update facing1a0.
- marioSearchFrontWall_staying's front-facing fallback loads display1ac at
  8008b58c then calls8008b590. A later current-support/secondary pass again
  uses1ac at8008b628–62c. Travel-heading passes precede this fallback according
  to the wall contract; this helper is not unconditional for every ray.
- marioSearchGround ordinary nonzero base180 uses
  revise360(travel1a4−offset19c) at8008e7b4–7cc; **zero base** uses
  toMovedir(display1ac) at8008e7d4–7d8. Motion19 instead passes1b4 at7e0–7e4.
- marioSearchHead nonzero base180 uses retained travel1a4; zero base loads1ac
  and calls8008c254–258. It does not substitute the combined air vector.
- marioChkFrontStep uses ordinary display1ac at8008931c–320, then applies its
  own source whole-degree preparation. Keep that caller's quantization separate
  from this helper's Q1000 output.

These explicit branches explain why zero displacement, zero base and retained
facing are distinct concepts in original standing/airborne probe geometry.
Other special/partner/event callers are outside this caller census slice.

## Verification

Run `python3 tools/verify_overworld_display_to_move.py`. It pins6 source bodies,
caller/load/wrap/round anchors and the exact56-byte literal block. Finite derived
ordinary, offset, sign/quantization and main00100000 cases validate this
reconstruction, not execution of the shipped Gekko conversion/trig pipeline.
