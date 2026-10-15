# Packed weapon costs and original UI text

These retained owners keep original source reads and child order while waiting
for real providers. See [weapon menus](battle-weapon-menus.md),
[font GX](font-gx-pipeline.md) and [startup ownership](native-ui-startup.md).
Offsets/addresses/masks below are hexadecimal; counts and thresholds are decimal.

## Weapon cost

[Packed cost](../../source/battle/battle_weapon_cost_continuation.cpp) captures
weapon+8 item ID and unsigned base FP byte +`0x11`. Item zero skips the pouch
count. Nonzero item still queries the actual count even for base FP zero, then
reloads item for the original linear/exponential ID membership tests. A later
item mutation does not change the already captured base or counted item.

Linear multiplication wraps 32 bits. Exponential scaling uses original bias at
`0x802f00b8`, source power base at `0x802f00d0`, and wrapped copies-minus-one.
The actual power child precedes a fresh bias reload for converting the captured
base. Double multiplication and actual float-to-integer truncation remain
separate children. The `0x4330` double construction and sign XOR preserve source
integer conversion order; no native menu estimate replaces it.

Only a nonzero resulting cost reads unit+`0x2f5` reduction. Subtraction wraps,
signed nonpositive floors to 1, then signed values >=1000 cap to 999. Zero stays
free and skips that unit read. The cursor returns a value without writing guest
weapon storage. Every successful read/child survives a later hold; exact input,
service and shared control-block identities stay fixed. Current loss stales until
reset; reset cannot undo a child's earlier effects. See the
[cost contract](../behavior/battle/RETAINED_PACKED_WEAPON_COST.md).

## Width measurement and private bytes

[Font width](../../source/display/source_font_width.cpp) has separate EOF byte,
lead classification, actual multibyte halfword and fresh single-byte reads.
It parses commands; the centered drawing loop below does not. Normal width uses
actual BFN WID1 and fresh GP language/spacing, with 16-bit wrapping. Glyph
`0xffff` skips width. Newline compares low-16 current/maximum, then resets current.
EOF removes fresh language spacing only for a nonzero maximum.

A stable `0x140`-byte private frame preserves original aliases: command name at
`0xc0`, arguments `0x40`, icon dimensions `0x08/0x0a`, scale `0x0c`, signed
extra `0x10`, scan outputs `0x14/0x18`, numeric icon `0x1c` and name `0x20`.
Each byte is optional; zero is known data, absence is unwritten. Only child-written
bytes merge, so a later command can retain earlier argument bytes. Unknown output
holds after a completed child instead of rerunning it or zero-filling the frame.

An icon command calls original strcmp, CameraGet even though its result is unused,
numeric/name scanf, optional name lookup and icon size. Scanf's zero return is
completed but supplies no invented private outputs. Named negative lookup floors
to zero; icon ID narrows to 16 bits. Unsigned width and signed extra/current values
use distinct original biases before a later scale read. Required FMA, separate
float addition and truncation retain their own operands and rounding boundaries.

Only a command starting `R` skips the following-newline test. The original signed
byte cannot equal `0x2f72`, so `/r` follows the normal optional-newline-skip path.
Only a newline reaching the normal branch increments the count; command-skipped
newlines do not. The optional nonzero line destination gets an actual guest halfword
store, not a private output. A null command uses real null guest read providers.
See [width contract](../behavior/display/RETAINED_SOURCE_FONT_WIDTH.md).

## Centered raw drawing

[Centered strings](../../source/display/source_string_font.cpp) capture actual
camera, call whole width with null line destination, then center using its low-16
result and the original conversion/half constant. Camera view +`0x11c` and caller
base are captured only at reached SDK concat; a preceding width wait may observe
changed base bytes. After capture, both object identity and exact matrix bits
remain fixed. GX matrix index 0 loads before the signed conversion bias capture.

The drawing loop maps raw text, including `<`, newline and `0xffff` glyph results.
It does not borrow command parsing or newline layout from width/StringMatrix.
Every reached glyph still requires its genuine provider. Outline saves current
color, installs palette zero and current alpha, then emits X ±2 and Y ±2 pairs.
Two separate truncations precede those pairs; coordinates wrap through signed 16.
The main glyph has its own conversion and full signed-32 coordinate. After the
outline, saved source color is restored with freshly read alpha. A hold retains
prior writes/emissions; reset does not roll them back or restore color for free.

Per-glyph advancement rereads WID1 width, GP/language and spacing after drawing,
converts the signed wrapped sum, and rounds a separate float addition. It cannot
reuse centering width or cached native font metrics. See the
[centered contract](../behavior/display/RETAINED_CENTERED_STRING_FONT.md).
These controllers establish neither source queue completion nor native GPU or
original CPU/pixel parity. Stable owners must supply actual memory and linked
partial effects through every held invocation.
