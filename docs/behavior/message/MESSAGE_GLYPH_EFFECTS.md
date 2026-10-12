# Normal message glyph effects, RNG order and draw-time mutation

US G8ME01 revision0, recovered2026-10-12. B evidence is static original
msgDisp0x800833e0,sizeb54, tail0x800839f4..0x80083ee0 and referenced constants.
Together with [draw admission](MESSAGE_DRAW_RECORD_ADMISSION.md), this covers
its local branches. Font/matrix/clock/numeric/trigonometric services and original
execution remain separate; this is not a complete raster or dialogue claim.
See [parser flags](MESSAGE_PARSER_RECORDS.md), [selection](MESSAGE_SELECTION.md)
and [RNG](../meta/RANDOMNESS.md). No original message text is altered.

## Priority after admission (B)

Only ordinary drawable glyphs that passed history/vertical admission and have
historyEmphasis=0 enter this tail. Let F=R.word[0], scale=R.float[10], dynamic=
R.float[14], code=R.u16[4], X/Y be admitted draw coordinates. Selection target
t is signed D.byte[f243]. Branch order is:

| First satisfied gate | Local branch |
| --- | --- |
| F&1 | Dynamic matrix glyph, optionally with shake F&2 |
| F&2 | Shake glyph |
| F&4 | Clock-driven wave glyph |
| F & originalWordShift(0x100,t) | Selected glyph wave/color/edge/rainbow requests |
| Otherwise | Plain glyph |

These are priority gates, not independent stacked effects. Dynamic can shake;
other combinations follow the first admitted branch. originalWordShift uses
PowerPC's low six count bits: bit5 set gives zero, otherwise low5 bits select
the left shift. It does not validate target line/range or use unbounded shifts.
The selected-mask branch has no separate D.selectionMode flag check locally.

## Two-draw shake requests (B)

Dynamic+shake and standalone shake each call irand(10000) twice, X first then Y.
Each result is reduced modulo3 through signed reciprocal multiplication using
0x55555556 and sign correction. With the recovered positive RNG domain, offsets
are0,1,2, added to X/Y by separately rounded fadds. No subtraction to center
them at-1..1, independent-axis seed or rejection sampling.

irand consumes one rand for each nonzero bound, so these routes consume two
explicit outer RNG draws per admitted shaking glyph per draw invocation. History
emphasis/skips, reserved controls/icons/poses, wave/selected/plain glyph branches
do not take these outer calls. The subsequently recovered [inner quad](FONT_GLYPH_QUADS.md)
also calls irand64 twice per quad unconditionally, including nonshaking glyphs;
outer shake counts are not whole-renderer totals. There is no opacity-zero suppression here. Repeated
draws can change shared RNG progression; optional/native rendering cadence must
not be assumed independent of gameplay randomness.

## Dynamic matrix path and stored relaxation (B)

Call kanjiGetWidth(code); derive half-width H=(returnedWord>>1)&0x7fff. Optional
shake occurs next, before matrix setup. Then request these operations in order:

1. PSMTXTrans(matrix,-float(H),12,0).
2. PSMTXScaleApply(matrix,matrix,dynamic,dynamic,dynamic).
3. PSMTXTransApply(matrix,matrix,float(H),-12,0).
4. PSMTXScaleApply(matrix,matrix,scale,scale,scale).
5. PSMTXTransApply(matrix,matrix,X,Y,0).
6. FontDrawCodeMtx(matrix,code).

Coordinates in step5 include any shake. Matrix composition and font raster
internals remain helper contracts; retain request order instead of guessing a
host transform order. This branch uses scale on all three axes even when F has
the scaleX flag0x10000. It does not run normal FontDrawScale/ScaleVec routing.

After drawing, load dynamic again, compute delta=f32(dynamic-1), then store
dynamic=fusedSingle(0.5*delta+1) at R+14. This is a record mutation during draw,
not parser reinitialization or simulation update. No snap/range/NaN guard. Derived
error shrinks approximately2^-n after n draws, subject to separate subtraction
and fused rounding. Skipped/history-emphasis glyphs do not relax this value.

## Clock pipeline and phase shared by wave/selection (B/P)

Both branches independently reread global work pointer and current high/low clock
at+38/+3c, subtract D.timestamp[18/1c] with64-bit borrow arithmetic, and read the
word at0x800000f8. Set frequencyWord=thatWord>>2. Compute divisor as
high32(0x10624dd3*frequencyWord)>>6, the original unsigned reciprocal sequence.
For ordinary word inputs this is floor(frequencyWord/1000); there is no local
zero-divisor or clock-range guard.

Call __div2i0x80264ad8(elapsedHigh,elapsedLow,0,divisor), then
__cvt_sll_flt0x80264e70(returnedPair), retaining its floating return T. Signed64 division
and float-conversion helper internals, clock initialization and resulting time
unit are still open; do not label T as measured milliseconds/ticks merely from
the literal1000. Exact arguments/order are established here.

Compute product=f32(0.02*X), then phase=fusedSingle(0.01*T-product), using the
original constant bits0x3ca3d70a/0x3c23d70a. Call cos(phase)0x80271ea8, narrow
result through frsp; request movedX=fusedSingle(3.2*cosResult+X). Then call
sin(phase)0x80272410, narrow through frsp; movedY=fusedSingle(Y-3.2*sinResult).
The sin argument remains the original phase even where the register used for
phase is later reused as movedX. No RNG, stored coordinate mutation or explicit
per-frame increment in either clock-driven branch. Trig/FPSCR/paired-single
accuracy and actual clock/cadence comparisons remain unverified.

## Font scale and branch-specific drawing (B)

Shake, wave, selected and plain paths use FontDrawScaleVec((scale,1,1)) when
F&0x10000, otherwise FontDrawScale(scale). Each vector starts from its own
original three-float template before replacing the first component; the five
templates including history emphasis are verified. No scale-vector call in the
dynamic matrix path.

Standalone shake then FontDrawCode(code,jitterX,jitterY); ordinary wave then
FontDrawCode(code,movedX,movedY). Plain calls FontDrawCode(code,X,Y). None has
a local font-color save/restore or record scale mutation. Inner font services
may change shared state; these local requests do not guarantee neutral state.

Selected path uses the same phase/motion calculations, then, after scale setup:

1. FontDrawColor(pointer to0x00000050); FontDrawCode(code,movedX,movedY).
2. FontDrawEdge(); FontDrawRainbowColor().
3. FontDrawColor(pointer to0xffffffff); FontDrawCode(code,movedX-4,movedY+4).
4. FontDrawEdgeOff(); FontDrawRainbowColorOff(); FontDrawColorIDX(0).

Calls are in this exact order. Two glyph requests occur. Color index0 resets
through the font service; it is not restoration of a saved prior color. There
is no local multiplication of these literal color words by window alpha and
no explicit prior edge/rainbow state snapshot. Final appearance/alpha/index
mapping is now covered in [font state](FONT_DRAW_STATE.md), including index0's
palette and effective alpha. Inner glyph/GX appearance remains open.
All paths rejoin the exposed-record loop
without changing its key-result candidate.

## Local coverage and remaining evidence (B/P)

Admission/control/history/pose/icon/indicator behavior is specified separately;
this file completes msgDisp's local normal-glyph branch map. Derived obligations
include priority combinations, two RNG draws in X/Y order, one-sided jitter,
width truncation, matrix operation order, scaleX exclusion in dynamic, repeated
draw relaxation, independent clock reads, exact phase/trig narrowing/fused steps,
selection shift domain, double glyph/color/edge/rainbow order and index0 reset.
These are static-derived contracts, not original captures or gameplay timings.

Remaining: signed64 clock division/float conversion, trig/matrix/font bodies and
glyph tables/materials, clock/RNG seeding and all render-dependent consumers,
actual draw cadence, callback/font state lifetime, matched original image/seed
traces and story attribution/order/conditions. Full font rendering/dialogue/
complete-game equivalence remains unproven.
