# Original camera interpolation modes and timer domain

US G8ME01 revision0, intplGetValue80006c8c, original gor_00–04 event requests,
viPostCallback8006f404 and camera event producers. This extends
[the dolly/event consumer](OVERWORLD_CAMERA_DOLLY_AND_EVENT_BLENDS.md).
Read-only source reconstruction and finite derived checks; no original execution.

## Actual authored camera curves

The complete pinned75-owner gor_00–04 camera census contains332 requests.
For the300 basic camera event requests whose interpolation operand is closed:

| Function | Curve counts |
| --- | --- |
| evt_cam3d_evt_set | 239: curve11×235, curve0×2, curve1×1, curve12×1 |
| evt_cam3d_evt_off | 55: all11 |
| evt_cam3d_evt_set_xyz | 5: all11 |
| evt_cam3d_evt_zoom_in | 1: curve11 |

The four non11 calls are original event instructions in rel/gor.rel:

| Owner | Relative file instruction | DurationMs | Curve |
| --- | --- | --- | --- |
| gor_00_koopa_evt, gor_00.o | 2b3d8 | 800 | 0 |
| minnnanokoe_evt, gor_01.o | 3ad9c | 2000 | 1 |
| hom_10_init_evt, gor_04.o | 5c858 | 8000 | 0 |
| evt_train, gor_04.o | 5cc30 | 2000 | 12 |

These names/requests are not authorization to execute story events early.
Literal durations range0..8000ms. Six coordinates may include original EVT
expressions; retain event resolution rather than treating raw operand words
as positions. The other32 camera requests are control/shake/letterbox/shift/
getters/road service owners, not additional basic interpolation operands.

## intplGetValue built-in finite contract

Arguments: signed32 mode/step/count and float32 start/end. If count==0, return
end immediately, even with an unsupported mode (80006cbc–ccc). Otherwise
convert signed step/count to float32 using the double integer-conversion bias
at80006cd0–d04. Unsigned mode<=13 selects dispatch table8030434c. Other modes,
including negative signed values, are **callback pointers**, not a fallback
linear curve:80006ff0–70008 invokes that address with step,count,start,end.
This indirect ABI is outside the portable built-in domain.

Define s=float(step), n=float(count), a=start, b=end, d=b−a, r=n−s.
The following finite formulas express the source branches. Source single
precision operation order, libsin/libcos and fused multiply-add remain numeric
adaptations. Do not replace them with a clamped generic easing library.

| Mode | Source expression | Entry |
| --- | --- | --- |
| 0 | a+(s*d)/n | 80006d24 |
| 1 | a+((s*s)*d)/(n*n) | 80006d38 |
| 2 | a+((s*(s*s))*d)/(n*(n*n)) | 80006d54 |
| 3 | a+((s*(s*(s*s)))*d)/(n*(n*(n*n))) | 80006d78 |
| 4 | (a+d)−((r*r)*d)/(n*n) | 80006e90 |
| 5 | (a+d)−((r*(r*r))*d)/(n*(n*n)) | 80006eb4 |
| 6 | (a+d)−((r*(r*(r*r)))*d)/(n*(n*(n*n))) | 80006ee0 |
| 7 | b−(r*(r*(d*C7)))/(n*n) | 80006da4 |
| 8 | b−(r*(r*(d*C8)))/(n*n) | 80006de4 |
| 9 | b−(s*(s*(d*C8)))/(n*n) | 80006e3c |
| 10 | b−abs((r*(r*C10))/(n*n))*d | 80006f14 |
| 11 | a+((d*(1−C11))*.5) | 80006f78 |
| 12 | a+d*S12 | 80006fa8 |
| 13 | a+d*(1−C13) | 80006fc8 |

Trigonometric outputs are explicitly rounded from double to single with frsp.
Source phase expressions (evaluate in this order):

    C7  = float(cos(4*(Pi*(s/n))))
    C8  = float(cos((4*(Pi*((s*s)/n)))/((15*n)/100)))
    C10 = float(cos((4*(Pi*((s*s)/n)))/((40*n)/100)))
    C11 = float(cos((Pi*s)/n))
    S12 = float(sin((HalfPi*s)/n))
    C13 = float(cos((HalfPi*s)/n))

Pi=word40490fd8 at8041f46c,3.141592025756836;
HalfPi=word3fc90fd8 at8041f480,1.570796012878418. They are not the different
source compass Pi3.1415927410125732. Constants4/15/100/40/.5 are source-pinned
at8041f468/470/474/478/47c.

No step/count/output clamp exists in this helper. Mode7/8 may exceed the
endpoint range; mode9 begins at b and has no general endpoint=b promise.
Mode10 takes abs of the factor before multiplying by d (80006f60–70), so
negative d is not itself made positive. Mode13 at s=n need not be exactlyb
because source HalfPi is approximate; never special-case every endpoint.
Finite overflow, NaN and indirect pointer domains are not repaired here.

The camera's ordinary nonnegative, monotonic elapsed-time input produces
count100000 and step0..100000 from its timer owner and fctiwz scale. It uses
step=trunc(100000*pB), or trunc(100000*(1−pA)). Timer completion does not change
intplGetValue's no-clamp contract. A rewind/unknown GP clock or arbitrary EVT
callback mode is an unsupported service domain, not an implied step clamp.

## Raw OS tick producers and the native millisecond bridge

OSGetTime8029d84c reads time-base upper/lower/upper and retries if upper changes.
It returns the coherent64-bit hardware tick count, not milliseconds or frames.
viPostCallback8006f404 calls it at8006f420; subtracts prior GP30/34 to GP28/2c
(retraceDeltaTime), then adds that delta to GP38/3c at8006f5ac–5c8. The DVD-error
GP10 check8006f4d0–d8 and winCheck8017eb24 call8006f4dc–e4 bypass that accumulation
when active. Other system-level clock masks gate their own counters; they do
not individually gate GP38/3c. GP40/44 is the separate field clock, additionally
gated by GP14 in-battle at8006f5d0–d8. They must not be conflated.

Basic camera event leaves snapshot **GP38/3c** into A70/74 or Bd0/d4 and turn
resolved durationMs into a duration pair via the source instructions:

    tickHz = unsignedWord(800000f8) >> 2
    ticksPerMs = tickHz / 1000  # unsigned integer quotient
    durationLow = low32(durationMs*ticksPerMs)
    durationHigh = 0

Unsigned division is the source mulhwu10624dd3 then srwi6 (e.g.800a7fe0–ff8),
not a float seconds conversion. Duration operands in this shipped bounded
census are nonnegative0..8000, avoiding wrap; arbitrary signed negatives or
large values have unsigned low-word behavior and are outside that ordinary
producer domain. The camera timer then reads the pair as signed64 and converts
to float; see the consumer's strict crossing rules.

A native integer-ms clock is a clearly labeled bridge. Ratio elapsedMs/durationMs
models the source producer for ordinary positive durations with consistent
clock rate, but discards sub-ms OS timestamps and source integer ticksPerMs
quantization. Do not round each source time to a simulation-frame count, use
60fps durationTicks, reset the retained global clock on room load, or truncate
both raw tick timestamps to integer milliseconds before computing a ratio
while claiming the source did so. The source elapsed ratio is computed from
the **64-bit tick difference**, not a count of callbacks or a millisecond
conversion leaf. Future source-time import requires the actual hardware word
and GP clock/pause lifetime; it is not a guessed45MHz constant.

## Exact camLoadRoad local reset versus retained view/lens

camLoadRoad8000f36c–450 acts on the selected existing camera entry. It clears
**flags10** at8000f3a0–a4, clears blendedFollow88/8c/90 and invokes camRoadSetup.
Success sets flags4 and mode1; failure clears flags4 and mode3. In both cases
it writes FOV38=25, roll/view-offset14c/150/154/158=0, Astate4=0, curveA80=11,
Aprogress8=0, curveBe0=11, Bprogress84=0, rawFollow94/98/9c=0.

The body makes **no writes** to storedEye40..48/storedAt4c..54,
presetEye58..60/presetAt64..6c, actualEye c..14/actualAt18..20, near30, far34,
aspect3c, projectionType19c, Bstate82, anchorsb8..cc, dollye4/e8/ec/f0,
A/Bstart/duration or blendedLooka0..a8/rawLookac..b4. They retain their actual
history. Existing docs detail camRoadSetup's separate global solver/shift reset;
that service has no camera-entry pointer argument and does not manufacture a
fresh entry. Fresh camEntryPersp memset is distinct from a room load.

FOV25 is the explicit live-lens reset; near/far/type remain retained. The next
mode1 callback resets e8=0 and reconstructs follow/look/blends according to its
retained event state. It may derive different actual view/projection afterward;
that does not turn these absent load-time writes into an implicit reset.

## Validation

Run `python3 tools/verify_overworld_camera_interpolation.py`. The verifier pins
full consumers/producers, source dispatch/phase constants and all75 original EVT
owner bodies/332 imports. It checks the300 basic request words and histogram,
then finite derived interpolation, out-of-range and timer-division examples.
Derived math is a host adaptation; original libm/Gekko/time service numerical
parity and source-runtime event execution remain unclaimed.
