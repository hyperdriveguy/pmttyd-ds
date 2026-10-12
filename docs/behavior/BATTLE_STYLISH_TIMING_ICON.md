# Stylish timing icon: eligibility, queue and draw requests

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static complete _accrobat_timing_icon_disp80192d1c/98,
BtlUnit_GetHeight80128414/8 and iconDispGx8001a918/24, plus the locally specified
Main queue block80192e14..80192e9c. See
[timing/result contract](BATTLE_STYLISH_TIMING_AND_RESULTS.md).
Verification: `python3 tools/verify_stylish_timing.py`.
Actual draw/assets/depth services and rendered observations remain open.

## Eligibility and request order

After the actor-null/flag0x10 exits, Main checks W.word163f4&4 and signed
lower<t<=upper. Eligible calls copy the three-word zero template0x802f96b8
into a temporary XYZ, then overwrite X/Y with actor baseline floats+3c/+40,
and Z with single(actor.float44+15.0), constant0x804245d8. Call
`dispCalcZ(&XYZ)` and retain its returned floating depth in the ABI's f1.
Then call `dispEntry(camera4,layer1,depth,_accrobat_timing_icon_disp,actor)`.
Scalar registration/depth internals are separate contracts; Main ignores
registration's scalar return and keeps processing input.

This block runs on each eligible Main invocation and precedes its autoInput,
history and current-trigger evaluation. It can queue on a terminal success or
history rejection invocation. It does not allocate an icon once per start or
retain a queue handle. Queue lifetime, redraw and unit-pointer validity require
display ownership evidence. No sound request occurs in this block or the draw
callback; audience/voice calls belong to later event services.

## Complete draw callback

The callback receives the retained actor as its second argument. Request
`BtlUnit_GetPos(actor,&X,&Y,&Z)`, then GetHeight(actor), which returns
S16(actor+0xce). Add5 with word arithmetic, convert signed32 by biased double
subtraction with fsubs (single rounding), and set Y=single(Y+convertedHeight).
Set X=single(X+15.0). Z remains the GetPos result. Call
`iconDispGx(&XYZ,0,0xa2,0.6700000166893005)` using binary32 scale bits3f2b851f
at0x804245dc. iconDispGx adds alpha255 and forwards to iconDispGxAlpha8001a93c;
that renderer is not newly recovered here. Return has no specified value.

The queue depth coordinate used BASELINE+3c/+40/+44; the draw callback uses
GetPos's visible coordinate layer. They are not one snapshotted vector.
Height is signed16 and unscaled locally. There is no null/HP/status check,
subsequent eligibility recheck, coordinate clamp or adaptation to 2DS screens.
Do not replace icon ID0xa2 with a guessed generated asset; resource lookup and
presentation adaptation require the original asset binding.

Acceptance cases: exact lower exclusion/upper inclusion; absent eligibility bit;
queue before outcome; baseline depth versus later visible draw position;
negative height; single-round additions/scale; alpha255 forwarding. Equipment/
flag source writer,camera/depth/projection/queue lifetime, icon resource and
actual original presentation traces remain dependencies.
