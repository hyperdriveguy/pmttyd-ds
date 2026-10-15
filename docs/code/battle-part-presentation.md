# Part rendering, enemy HP and source screen coordinates

See [audience draw](battle-audience-draw.md) for auxiliary math adapters,
[battle camera](battle-camera.md) for framing and [render adapters](render-adapters.md)
for native presentation. These routines preserve packed guest reads and source
SDK boundaries; readiness does not establish complete native battle integration.
Offsets below are hexadecimal unless a decimal count is stated.

## Part matrices, materials and history

Ordinary part display retains 21 matrix slots and prepared scalar/material/draw
requests. Fixed providers and part are caller obligations; only part identity is
checked here. Negative pose handle finishes without drawing. Initial unit matrix
construction performs sixteen source operations before part attribute 0x20000000
reaches an explicit unimplemented continuation.

Shared unit scale +0x114 affects X/Y, while Z uses its separate source products.
Original rotations, pivots, display offsets, world translation and facing/camera
correction remain separate calls. Rotation selector 0..5 maps YZX/YXZ/XYZ/XZY/ZXY/ZYX;
invalid selectors skip the ordered products rather than choosing XYZ. Prepared
request operands remain pinned through a hold, but caller-side arguments and
later source getter reads follow the implementation's actual call sites.

The native phase adapter reads unsigned halfword phase and computes
`(phase*(floating?4:3)+270)%360`. Binary32 full-turn multiply/divide precedes double
sine; amplitude 2 or 4 feeds FMA(amplitude,sine,amplitude). It is an explicit native
adapter, not evidence of original SDK sine parity.

Material color starts from part +0x4f4. Flags 0x10/0x20 can blacken RGB. Only
initial-white RGB takes status-4 tint and unit +0x312 fade. Blue deliberately
uses already-reduced red in its own 160/255 operation; treating red/blue as
independent original-255 channels changes the result. Calls then publish event
color, FlagOn 0x40, before-callback, status-16-dependent FlagOn/Off 0x800000,
three real draw passes 1..3 and after-callback. A later missing child does not
replay acknowledged siblings.

History has ten entries of stride 0x44. Nine old slots shift backward, loading
word pairs before stores. Head holds mode, 3x4 matrix, angle, color and two late
words. A matrix-copy hold resumes after the shift, so it cannot shift twice.
History flags may clear the new head's low bit only after source publication.

## Enemy HP number and bar

Monosiri captures current B/kind. Force flag 2 bypasses switch/bank work; otherwise
it ORs switch(kind+4474), fresh unit flags and source bank bits. Flag 4 additionally
queries alternate kind from unit +4 without a second switch lookup. Bank addressing
uses signed kind/32 quotient/remainder and PPC word-shift rules: low-six-bit shifts
32..63 produce zero rather than a modulo-32 bit. Any nonzero final result becomes 1.

The retained HP draw admits camera 8 and scans 64 slots. It skips status 27,
failed Monosiri, attribute 0x02000000 and source menu/unit gates. It reads current
XYZ before battle flag 0x80 can overwrite with home XYZ, then adds signed halfword
X/Y offsets +0xd4/d6 and requires actual screen projection.

Captured unit and later slot reloads are different identities. Monosiri and HP
number queries use late slot pointers; bar drawing keeps the original unit. One
reload chooses X offset 12/16/20 for HP at least 100/10/other; another reload gets
the actual displayed HP. Number is always white. Bar color initializes only once
per invocation, so a preceding unit's fade alpha can carry to a later unit. A
next-slot transition does not reset that source register.

Gauge halfwords +0x1fc/1fe/200/202/204/206 hold mode/flags/timer/duration/fade/lifetime;
words +0x208/20c store HP snapshots and +0x210/214 ratios. Init retains separate
HP/max reads and post-store mode read; duration 20/40 and lifetime +30 are source
counts. No max-HP-zero fallback is invented. Curves are linear mode 0/cubic mode 2,
with separate float rounding; duration zero returns end and step is not clamped.
Main predecrements timer. Mode 1 uses fresh max after interpolation; mode 0 retains
max before it. Missing interpolation can follow ratio stores and prevents the
later fade/hide suffix. These counters are not measured render frames.

## Projection and stock EXP

Live screen helper acquires camera 4 before copying guest or borrowed typed XYZ.
View +0x11c then projection +0x15c transform the local in place. Only after both
acknowledge does it read original scale/zero constants and publish output. Failed
SDK admission leaves output untouched. Source scale 304/240 is half of 608x480;
it is not native viewport reflow, clamping or lower-screen pixels. A typed camera
snapshot projector has a different acquisition contract.

Stock EXP draws icons, not decimal text. Positive count clamps at 100; ones use
icon 404 at scale 0.35 and spacing 13, tens use scale 0.5 and spacing 20, both
flag 16/alpha 255. Count 100 means ten large icons. Source positions use offsets
308/-220/-100 and live globals 0x8041eb98/9c reloaded before each queued icon.
The queued request pins those operands until acknowledgement, then advances the
index; a held native icon cannot redraw earlier siblings.
