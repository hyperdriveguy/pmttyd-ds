# Class circles, bubbles and footer display

Read [action UI](battle-action-ui.md) for ring state and retained parent ownership,
[draw pipeline](draw-pipeline.md) for lower display services and
[TimingA/reels](battle-feedback-draw.md) for the separate break-slot draw parent.
An engaged optional scalar zero acknowledges a linked SDK/GX/font operation;
only `nullopt` means unavailable. Immediate raster/footer leaves can fail after
submitted primitives. Use retained children or complete provider admission when
that suffix can suspend.

## Source constant bank

The negative displacements in these helpers refer to original float/color bank
`0x80427440`; they are source storage identities, not arbitrary native tuning.
Some words intentionally serve multiple roles. Important groups are:

| Signed displacement(s) | Role at the point of use |
| --- | --- |
| -20544 / -20532 | Bubble / circle material color |
| -20540/-20536, -20528/-20524 | Enabled/disabled font color, icon alpha |
| -20508 / -20500 | Button-plane color / zero |
| -20496 / -20492 / -20488 | Ring angular step coefficient / pi / half turn |
| -20484 / -20480 / -20476 / -20472 | Entry spin offset / spin coefficient and font max width / full turn / degree conversion |
| -20468 / -20464 / -20460 | Selected X and footer Y offset / slide X/Y start |
| -20456 / -20452 / -20436 | Growth increment / unit scale / partner-right depth |
| -20400, -20272/-20268/-20280 | Piecewise shadow offset coefficients and circle divisor/scale intercept |
| -20396/-20376/-20372, -20384 | Pinch quad coordinates / pinch Y offset |
| -20388 / -20380 | Unit-side threshold / shade coefficient and localized pinch scale |
| -20368 / -20364 | Footer angle decrement / projected midpoint factor |
| -20292/-20264, -20276/-20256 | Bubble/circle shadow Y / depth-scale and icon-scale coefficients |
| -20260/-20252/-20236/-20232/-20228 | Circle icon Y / bubble base Y / bubble icon X / label Y / font scale |
| -20248/-20244/-20240 | Bubble left/right/body Y; -20240 also ellipse X radius |
| -20284/-20224/-20220/-20216/-20212 | Ellipse Y radius / angle phase / projected center X/Y / lower X clamp |

These roles explain the current implementation without renaming unidentified
source fields or claiming original SDK/Gekko arithmetic parity.

## Circle and bubble calls

Both draw leaves capture B, require camera 8 and use one source TEV stage for
texture color/alpha multiplied by raster color/alpha. Operation tuples encode
ADD, zero bias, scale 1, clamp, PREV; texture order is coord 0/map 0/color0A0.
These GX enum values are not native GPU registers. Palette comes through
`B+0x163f8`, owner `+0xa0` and two pointer dereferences. Texture index 0 selects
circle, 1 bubble. Camera view is a 3x4 matrix at camera `+0x11c`.

Circle gray is low-byte `(192-truncate(shadeCoefficient*depth))`, not a saturating
clamp. Depth scale uses FMA. It copies two 32-float source XY/UV templates at
`0x802ef650/0x802ef6d0`, then draws four black-alpha-128 shadow quads and four
gray-alpha-255 body quads. Begin tuple 128/0/4 means GX_QUADS, vertex format 0,
four vertices. Copies remain pinned despite later primitive callbacks changing
source templates. The icon's rotation call is deliberately acknowledged and
then replaced by an actual identity call; eliminating it changes observable SDK
ordering. Row icon zero falls back to 403. Enabled is exactly 1, choosing icon
flags 16 versus 48 and the corresponding source alpha.

Shadow offset uses two independent closed angle intervals. At half-turn both
execute and the second overwrites the first. Outside both, including unordered
values, the original local lacks a produced offset; native code holds instead
of inventing zero. The retained version keeps that frontier sticky until reset.

Bubble work `+0x1c` swaps left/right edges. Shadow/body precede icon/font. Font
width uses only unsigned low 16 bits; X compresses above source maximum while
Y/Z keep the font scale. Label is captured before width measurement, but text
color independently rereads enabled after FontStart. Icon enable and text enable
can therefore differ if an admitted provider mutates the row.

The raster cursor pins circle/bubble identity, all provider bindings and input
bits using `memcmp`, including signed-zero/NaN representations. Each prepared
request or math operand survives a hold; `*Done` phases consume results once.
Reset drops retained CPU state, not previously submitted GPU work. Missing late
font/icon/texture services cannot be acknowledged by successful earlier geometry.

## Footer buttons and pinch marks

Work `+0x120` stores spin angle. Positive angle subtracts the source decrement
without a post-subtract clamp; nonpositive becomes source zero. Item -1 resets
spin. No-party button path projects only Mario and omits the partner-path arrival
sound. With party, both projections precede item lookup; an arriving item sounds
only when angle exactly equals its reset value, then decrements and draws type 5.
Without an item, actual position-change admission may draw type 4. Button rotation
uses ASCII axis 121 (`y`), then translation adds to matrix indices 3/7/11.
TexturePlane2 requires its real palette/size/GX provider, not a fabricated quad.

Pinch uses signed HP: alive, below maximum and at most 5. HP 1 selects localized
danger art through `0x80416d30`, alternating colors over GP clock modulo 50
(0..24 versus 25..49). Other admitted HP uses `0x80416d24`. These clock units are
not measured render frames. Source world-X threshold chooses party/Mario draw
order; children separately recapture B/camera/unit. Missing party is accepted
absence, while Mario remains an admitted input. Nonzero language adds source
scale after the earlier texture/projection/transform calls.

## Queue registration and audience animation bytes

Command palette ramps G up, R down, B up, G down, R up, B down between 192/255;
initial alpha 207 stays unchanged. One retained registration steps it once,
queues camera 8/class 0 at priority 902, then seven freshly gated source mask/
callback pairs at priority 500. A pending record pins its borrowed argument and
priority across queue unavailability and is not re-gated. Each active cursor
belongs to one entrypoint and battle owner; registration does not validate that
identity, so callers must keep it fixed.

Audience hidden flag `0x20000` skips only four initial records. Window query still
runs once and can queue a window at camera 8/class 1/priority 499. Break-slot
registration uses camera 8/class 0/priority 490; queue admission is not GPU
completion. Queue and borrowed arguments must outlive invocation/execution.

The animation helper scans 200 audience records of stride `0x134`, admitting
flags containing both 1 and 4. Positive timer decrements and skips opcode work.
It handles one opcode per actor visit: -8 wait, -6/-7 loop start/end, -5 label
jump, -4 label, -3 restart and -2 frame/timer. -1 or out-of-range leaves the script
pointer unchanged. Stack depth byte `+0x40` addresses raw source loop words;
there is no invented bounded native stack. Backward loop and forward label scans
can mutate pointers before missing script reads. The cursor retains that exact
scan/frontier, including prior stores, rather than restarting the opcode.
