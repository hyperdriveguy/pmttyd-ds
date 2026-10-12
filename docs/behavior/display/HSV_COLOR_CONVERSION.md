# HSV byte colors to RGBA

US G8ME01 revision0; recovered2026-10-12 from HSV2RGB at0x800752fc,
size0x22c. Evidence B: static original DOL instructions and constants, checked
against the read-only ISO. Symbol names are external annotations, not behavioral
proof. See [glyph quads](../message/FONT_GLYPH_QUADS.md) for the four per-vertex callers and
[floor](../format/FLOOR_NUMERIC_SERVICE.md) for the sole linked arithmetic service.
No original execution or rendered-color comparison was performed.

## Calling contract and observable state (B)

r3 points to four bytes H,S,V,A. Incoming r4 is overwritten before use: there is
no second conversion-mode argument. H/S/V are unsigned byte loads before the
floor call; A is loaded later at0x8007549c. The input buffer is never written.
Return r3 is the big-endian packed word0xRRGGBBAA, assembled in local stack
storage with one byte per component. Returning that numeric word on a
little-endian host does not imply the same in-memory byte order.

The only external branch-and-link is floor at0x800753b0. There are no RNG, GX,
allocation or global draw-state writes in this helper. It can affect floating
status through its arithmetic and conversions; exception/trap behavior depends
on the original FPSCR and is not measured here. Invalid pointer handling is
ordinary unchecked access, not a color fallback.

## Arithmetic sequence (B)

Let SP(expr) denote one original single-precision arithmetic rounding boundary.
It is not an assertion that every target uses host-default rounding. Byte
conversion uses double bias0x4330000000000000, then fsubs, producing exact
single-precision byte values. Constants are binary32 1,1/256,255,360,60 at
0x804203e4,0x804203f4,0x804203fc,0x80420400,0x80420404 respectively.
Preserve this sequence:

```
a = SP(360 * H)
b = SP(a * (1/256))
h = SP(b / 60)
s = SP(S / 255)
v = SP(V / 255)
k = trunc_to_signed32(floor(h))
f = SP(h - SP(k))
if k is even: f = SP(1 - f)
p = SP(v * SP(1 - s))
t = fused_single(1 - s * f)
q = SP(v * t)
```

`fused_single` corresponds to fnmsubs at0x80075400: the product/subtraction is
fused before the single-precision result. Splitting it into an independently
rounded multiply and subtract can change a later byte. Signed k conversion
back to floating uses xoris and bias0x4330000080000000. No modulo operation is
performed on k.

For every valid H byte, h is exactly3H/128: 360H and division by256 are exact,
and the division by60 has an exactly representable binary32 result. Thus
0<=h<=5.9765625 and k is0..5. These bounds are derived from instructions and
representable inputs, not measurements. In particular H255 does not wrap to
H0; a port must retain division by256 rather than normalizing hue by255.

## Sector selection and byte conversion (B)

The local signed comparison tree assigns floating RGB as follows:

| k | R | G | B |
| --- | --- | --- | --- |
| 0 | v | q | p |
| 1 | q | v | p |
| 2 | p | v | q |
| 3 | p | q | v |
| 4 | q | p | v |
| 5 | v | p | q |

There is no default initialization for the three result registers when k lies
outside0..5. That route is unreachable for valid byte inputs under the recovered
floor contract; do not invent a fallback as original behavior.

Each selected component is multiplied by255 with fmuls, converted with fctiwz
(truncation toward zero), and stored as the low byte of the integer word. There
is no explicit clamp, nearest-integer rounding or gamma/sRGB conversion.
Alpha is copied unchanged; it is not premultiplied into RGB. Draw alpha scaling
is a separate [font-state contract](../message/FONT_DRAW_STATE.md).

Useful derived boundaries: H0,S255,V255 gives red255/0/0; S0 removes hue
dependence; V0 gives zero RGB. For H255,S255,V255 the last-sector residual is
3/128, so blue truncates from5.9765625 to5 and RGB is255/0/5. These examples
follow exact arithmetic in those particular cases, not an executed sample set.
Do not assume all intermediate V/255 or S/255 products round identically across
unconfigured host floating environments.

## Reimplementation and remaining evidence

Use byte-accurate inputs/output and preserve arithmetic boundaries, including
fused subtraction and truncation. A generic HSV library with a hue endpoint of1,
rounded channels, a different alpha convention or gamma transformation is not
established equivalent. The glyph caller seeds S170,V255,A255 and supplies each
vertex hue; the helper adds no RNG consumption to that path.

Fingerprints identify the whole selected routine; fourteen independent
instruction anchors plus normalization/bias constants check the cited structure.
They do not prove execution equivalence. Capture original output for all256
caller hue values and boundary saturation/value cases under the actual FPSCR,
then compare native vertex bytes and rendered glyphs. Original GX interpolation,
texture/page bindings, font resources, and caller cadence remain separate gaps.
Switch remake behavior is not inferred from this GameCube helper.
