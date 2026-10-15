# Action selection and animated class ring

See [battle commands](battle-commands.md) for the outer input dispatcher,
[weapon menus](battle-weapon-menus.md) for class/weapon row formats and
[class raster/footer](battle-class-raster.md) for the linked draw leaves. These
helpers use original 32-bit guest addresses and borrowed storage. Native callback
readiness is distinct from the scalar returned by an admitted original call.

## Selection and UI allocation

`menu = B+0x171c`. Class rows begin at `menu+8`, with decimal stride 20. Setup
builds the class table, obtains type-14 cursor and initializes actual UI before
publishing menu flags `0x01000001` and resetting idle timer `+0x560`. Flag zero
with marker `menu+0x558` clear resets the nine listed cursor index/scroll pairs;
it preserves their count words and omitted slots. End runs actual UI destruction
before clearing active bit 1.

UI storage at `B+0x1c78` has size `0x124` and is not blanket-cleared. Constructor
releases old storage, allocates and publishes new storage, then captures it. An
unavailable allocation after release can leave the old pointer bytes published;
these immediate constructors/destructors have no replay-safe suffix cursor.

| Work offset | Meaning |
| --- | --- |
| 0 / 4 | Borrowed cursor / row pointers |
| c / 14 / 18 | UI mode / key-admission byte / prior selection |
| 1c | Bubble mirroring word, reset by draw main |
| 20 / 34 / 48 | Per-row flags / state / timer arrays, word stride |
| 70 / 84 | Old / current cyclic order arrays |
| 120 | Footer button spin angle |

All table offsets are hexadecimal. Immediate-entry flag bit 1 selects UI mode 10,
selected row state 10 and other row state 11, with zero timers and reverse cyclic
orders. Animated entry begins after the selected index; each row enters state 0
with delay `signed(ordinal * (signed(rate*10)/60))/2`, preserving word wrapping and
fresh count/rate reads. `rate` comes from GP+4, not physical render FPS.

Main first checks position trigger `0x800`, before key admission and forced
Jump/Hammer-row refresh. Switching requires both Mario and partner, state zero,
positive signed moves `+0x23` equal to `+0x22`, and statuses 1/2/9/27 clear.
Battle flags `0x00800000/0x40000000`, Mario attribute `0x10` and partner byte
`+0x12b` independently deny it. The successful position route sounds 2520,
ends UI, publishes type 9/menu 29 and submits display.

Forced Jump/Hammer rows sample actual first-weapon eligibility; zero/negative
count disables with reason 4/5 only if no earlier reason exists. Held-key query
`0xffffffff` resets idle timer and requests camera mode 0. No key increments the
wrapped timer; signed values at least 900 request mode 6/speed 5 on every visit.
These are main-call counts, not an observed duration.

Confirm `0x100` or a fresh audience marker selects enabled rows. Types
0/1/4/6/7/8 choose menu 5, operation 5 chooses 19 and special 13 chooses 29.
Item type 2 can change menu and end UI while still returning zero; state-zero
multi-item flags instead choose menu 14 and result 1. A denied row may publish
menu 3/reason and result 1. All normal returns submit display. Missing providers
can follow earlier sound/state/UI effects and do not roll them back.

Forward repeat `0x80000` takes precedence over reverse `0x40000`. In-range
index stores precede sound, but wrap sound precedes a fresh cursor lookup/store.
Audience interruption is checked before the second position-trigger poll. Those
repeated reads/calls remain observable through providers.

## Ring geometry and state visits

Projection starts from the acting unit's position `+0x3c`, applies source X/Y
offsets, overwrites Z and lower-clamps X. Ellipse angle is a source phase minus
theta. Cosine/sine receive the promoted double angle; X is written before sine
admission, so a missing sine can leave only X updated. These are linked numeric
providers, not permission to substitute guessed trigonometry or projection.

UI mode 0/2 waits for every row state 3 before switching to mode 1. Mode 10
publishes mode 1 and key readiness; mode 1 detects adjacent/cyclic index changes,
starts row state 4/5 and disables keys until stable. Row meanings are:

| State | Source local work |
| --- | --- |
| 0 | Decrement entry delay; leave this invocation's stack slot unwritten |
| 1 | Enter ring with angle/spin and selected X offset |
| 3 | Rest at ring angle |
| 4 / 5 | Forward/reverse rotation between old/current order |
| 10 | Slide selected row with X/Y curve 4 and parameter curve 0 |
| 11 | Grow the other rows |

Other states, including values above 11, also leave geometry unwritten. Optional
positions/matrices/parameters preserve this; no default identity or zero geometry
is manufactured. Source angle rows use `0x803635d8 + count*20 + order*4 - 60`.
Full-turn endpoints preserve cyclic wrapping. Durations use signed wrapped
`rate*n/60`, with n=10 for entry spin and n=12 for slide. State 4/5 can store
state 3/timer -1 before the later timer increment. State 11 updates state based
on growth before required scale/concat callbacks.

Center and offset are separate calls. Offset Y is added to screen Y *and replaces
stack depth Z*. Later icon ordering uses slot Y; circle shading/scaling uses Z.
The immediate draw performs rows, icons, party button, then pinch marks. Missing
providers can follow state writes or earlier submitted geometry.

## Retained invocation boundaries

`BattleActDrawCursor` owns one invocation's copied stack slots. Fixed provider
identity is required across holds. Each phase preserves completed SDK calls and
operand captures, while later rate/timer/constants remain fresh at their source
read points. Resume a held invocation before advancing the next simulation visit;
a second screen or GPU retry is not another draw tick.

The icon cursor borrows the slots **vector object** by identity. It first captures
all Y depths before filtering flags, then performs descending strict-`<` swaps.
Do not describe this as a stable sort. It rereads count during sorting; growth
beyond captured depth storage reaches a sticky unavailable frontier. Later rows
read flags and current borrowed slots freshly; each selected child then pins its
matrix/parameter/position until that child acknowledges. Flag 1 draws, flag 2
selects bubble instead of circle. Missing source stack data stays unavailable
until explicit reset; filling guessed slots cannot repair an existing invocation.

Queued class display uses passed B only for one type-14 cursor lookup. Its
unconsumed stack output may remain unwritten. The subsequent wrapper ignores
passed B and lets draw main capture current global B. A later draw hold does not
repeat the acknowledged lookup. Outer continuation alone cannot retain a child
raster/footer suffix: those providers must preserve their own partial calls.
