# TimingA display and break-slot reels

See [battle commands](battle-commands.md) for callback ownership,
[battle core](battle-core.md) for action timing and [stage storage](battle-stage.md)
for other scene work. Typed TimingA simulation, packed TimingA display and
break-slot draw are separate owners and numeric domains.

## TimingA simulation versus display

TimingA samples general trigger history once per supplied simulation tick; its
render callback does not advance that input history. Difficulty windows, mash
rejection, finish states and LCG constants are documented beside the implementation.
`bounded(0)` does not advance RNG. The special actor/setup path precedes automatic
and battle-force routes; its strict negative draw still consumes RNG despite
being unreachable for ordinary nonnegative output. Special and automatic routes
preserve prior classification, unlike the ordinary scan and forced route.

Packed display scratch begins at `B+0x1f20`. Draw independently resolves global
B, requires camera 8 and chooses icon 109/108 from active mask 1 at `B+0x1f50`.
It loads source base XYZ from `0x802f01c8/1d4` even though X/Y are subsequently
overridden by arguments plus source offsets. Flag 16 and source scale are icon
presentation data. Scratch word zero increments only after real icon submission.

Display state 99 invokes curve 4 with duration 20 and step `20-count`, stores
its result, then deliberately overwrites X with `0x80422720` like state 100.
States 1000/1001 interpolate only when count is at least 40, using step
`count-40`; otherwise they use that same X constant. After draw, a fresh parent
count decrements if positive in 99/100, or increments if below 60 in 1000/1001.
The child may have recaptured a different B. These display counters are not
simulation action-command windows. Missing providers can follow scratch writes;
these immediate callbacks retain no cursor.

## Break-slot storage and partial idle bridge

Controller work is `B+0x1616c`, size `0x134`; three reels start at `+0x18`, stride
`0x5c`. Source category tables `0x802fe040/60/80` hold 8/8/5 words. Init preloads
all words before clearing controller storage and allocating each table separately.
An engaged null allocation is explicitly rejected before copying; earlier source
stores/allocations remain. The idle bridge does not implement active slot play.

Idle main retains repeated global/Mario/partner lookups. Controller state must be
zero. Exactly two state-2 reels with unequal first/second categories publish
controller state 1 and count zero. It then checks each reel for idle state zero,
clearing flag 1 and setting alpha byte `+0x58` to 255 only for admitted idle rows.
An active row can therefore return false after earlier publications. Free walks
three table pointers with fresh owner/reel lookups, without clearing pointers or
rolling back earlier frees. Neither immediate parent has a replay-safe cursor.

## Retained drawing

The source draw parent `0x802036d4` uses a retained phase/step cursor. Keep its
services and downstream state stable until completion. Missing optional reads
are distinct from stored zero, queued request data survive holds and phase/step
advance only after actual provider acknowledgement. Three getter passes freshly
resolve global B; a single cached owner for the entire traversal would change it.

Enabled reels in states 1..4 first draw icon 476 at `(226+42*i,122)`, flag 16,
color `0x80426884`. Direct icon submission takes precedence over the retained
source-icon fallback. First-reel state then chooses scissor `(0,82,608,32)` and
table `0x80419f30` for 1..4, otherwise `(0,176,608,128)` and `0x80419f3c`.
State 11 skips that scissor submission while selecting the latter table.
These are source 608-wide coordinates, not native screen pixels.

Each enabled reel admits the original GX blend/depth/alpha state, then thirteen
SDK matrix operations. Translate, scale, XYZ rotation and concatenation boundaries
stay separate, including fresh negative-pivot reads. Axes 120/121/122 are ASCII
x/y/z; source `0x804268a0` converts angles to radians. `queued` preserves request
construction and `step` preserves partially read XYZ. Cursor `category` counts
matrix operations here, then becomes the texture category index.

The first real texture plane uses category at reel `+0x10`, source table halfword,
color and separately loaded alpha. States above 10 or equal to 3/4 omit the
adjacent plane. Others increment ordinal with signed comparison/wrap to zero
against the live count, reload category data and translate Y by -32 for states
1..4, otherwise -128. The second plane reloads color/alpha; missing plane provider
`0x8010323c` remains a hold rather than success. Completion requires restoring
full scissor from current GP `0x80415fd0`, unsigned halfword width/height at
`+0x170/172`, before retiring active/pending state. No cleanup step is optional.
