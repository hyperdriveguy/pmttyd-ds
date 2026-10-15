# Pouch bytes, inventory services and item windows

This guide covers portable pouch/key-item mechanics, revision tables, explicit
item selection and item/coin window presentation. [Field items](field-items.md)
connects the pickup and quest callers. [Field session](field-session.md) covers
the NPC setup bridge audited alongside these dependencies. Source contracts
remain in [the behavior index](../behavior/README.md).

## Retained pouch and ordered key operations

`ProloguePouch` owns the original 0x5d4-byte block with big endian signed halfword
access. Inline offsets identify current/max/base HP/FP, coins, BP, keys and
200 equipment slots. Legacy checkpoints may omit this ownership; initialization
belongs to explicit fresh creation rather than restore or generic healing.

The constructor provider uses a temporary guest identity and captures the original
pointer publication. It does not install a runtime RAM alias at that identity.
It grants 6,0x32,9 in source order, including preceding scalar writes, and sets
100 coins. Its limited fresh-unequipped revision is not the general badge path.
Original `name_party3` comes from the US message bank; absent resources cannot
supply a replacement partner name.

Coin addition narrows the sum to low16, stores it, re-reads signed16, then clamps
0..999. Saturating the full sum first changes large-addition behavior. Key count
checks all121 slots, including gaps. Removal clears the first matching slot and
interleaves each shift with source-slot zeroing. Insertion finds the highest zero
slot, shifts that prefix and inserts at the front; it does not choose the first
hole, compact or sort the array.

Count and low-level insert/remove are separate from event status. The remove
wrapper only revises after low-level1, calling Mario then party revision before
reading its output destination. Raw output operands remain fresh after callbacks.

| Result owner | Meaning |
| --- | --- |
| Low-level key remove/insert | 1 success,0 failure |
| Remove event result | 0 success,-1 failure; callback itself returns2 |
| Revised native helper | Missing optional means admission unavailable; contained0/-1 are real results |
| Selector poll | 0 pending,-1 cancel,positive item ID selected |

Revision requires the original340 signed badge costs and32 partner HP cells,
200 admitted equipment IDs and kind1..7 ranks0..3 before any mutation. It rebuilds
Mario max HP/FP, applies source badge increments, upper-clamps current resources,
then recalculates free BP from every slot's cost. Partner HP follows kind/rank and
HP-Plus-P count. Per-update signed16 wrapping is retained; no lower clamp or heal
is invented. Failed preflight cannot partly remove inventory.

## Catalog and explicit table rows

The private PMITEM01 header pins340 direct IDs, original table0x803108a8 and
stride40. Variable key strings are original `msgSearch` keys, not rendered text
or debug names. Null pointers require zero key length. Candidate loading requires
all rows and exact consumption before publishing the catalog.

The packed explicit selector branch uses a high-bit guest table address. Counting
includes its -1 terminator, then allocates one more row: N items require N+2
four-byte rows. Optional second item precedes the table entries. Rows hold flags
and a U16 item, not quantity. Source condition and subsequent item reads remain
separate, as do fresh context count/row-pointer reads.

`itemSelectionEvent` resolves mode/table every visit, retains context at entry
+0x78, and requests keyOff after creation. Completion reads result/index before
deletion and keyOn, then freshly reads output destinations. Mode0 omits index
publication. Native runtime admission covers positive nonempty explicit tables
with second argument0; unsupported empty/other-definition behavior is not guessed.

## Three windows and source cadence

| Slot | Role | Source rectangle | Sequence |
| --- | --- | --- | --- |
| 0 | List | -24,116,300,240 | Animated0 |
| 1 | Header | -280,120,240,45 | Instant2 |
| 2 | Description | -240,-130,500,80 | Animated0 |

Flag1 is active,2 drawable,4 busy,8 AutoDelete. Action0/1/2 means idle/open/close.
Busy is set before action work and clears on a later idle visit, even for instant
sequence2. Animated duration uses signed32(GP-rate<<3)/60 and source mode12;
repeated rate/clock reads and strict incremented-clock endpoint remain observable.

Manager order is sequence,own main,display registration for each slot. Slot0 can
change sibling actions before their visits. Its main runs once for the list,
not once per window. Description main uses the prior display's text/line data.
Ready result and retained allocation differ until explicit release.

List input uses source repeat/trigger masks, edge-trigger wrapping, eight-row
scrolling and one-sixth easing. Down-scroll uses absolute selected>5; up-scroll
uses selected-firstVisible<3. Gray and unselectable are separate row bits. A is
considered before B, and cancel requires the source allow-cancel flag. Source
sound IDs and help-reset ordering are retained beside the code.

Drawing retains all easing rows for scissor clipping. Coordinates remain source
608x480 UI values. Width/centering intentionally narrows to U16 at specific
steps; replacing those casts with generic signed centering changes the result.
Header geometry mutates after its frame snapshot. List/help arrows blink from
the supplied GP counter's low5 bits, not time since entry or render FPS. Help
pages advance two lines, using a different trigger namespace from list repeats.
Original text/font/icon resources are admitted before owner entry; rendering
reads only this visit's registered display slots.

## Item and coin text/window lifetime

US template expansion replaces first AN then first ITEM only. The article test
uses the original first-byte vowel membership. Coin text replaces first NUM/S,
with signed decimal and singular only for amount1. Other bytes/control tokens
remain unchanged; this is not a general localization or replacement engine.

The finite direct-color parser accepts col/red commands, one-to-eight hex digits
and bounded tags. It validates the whole input before glyph/RNG requests. Closing
a color command selects palette0, not a previous nested color. Exactly one
following LF is suppressed. Per-glyph64-bit markers distinguish original style,
palette reset and literal RGBA. For outlined glyphs, only the fifth/main quad's
color changes; edge quads retain their original style. Alpha multiplies/truncates
in low-byte RRGGBBAA order.

Item95 owns two supported windows: main0x80313dc8 and help0x80313df0. Failure
0x80313e18 and full discard selection remain separate unfinished paths. Entry
resets specified fields while preserving old scale/rotation/alpha. Close requests
AutoDelete. `action()` reports busy bit4, not the action enum; a deleted slot may
retain that bit, so it cannot prove parameter lifetime ended.

Item display snapshots the frame before text changes geometry. Main title can
grow its manager rectangle while busy; help waits until not busy before publishing
text. The description main therefore observes an earlier display. Item help uses
repeat100/200 rather than selector-help's direction triggers.

Coin window0x80415e84 borrows [amount,showText] through AutoDelete. It tests
showText before reading amount, measuring or resizing. Coin text lacks the item95
busy gate. A changed amount recompiles original template text; frame uses the
pre-mutation rectangle. Manager deletion separately clears the borrowed pointer.
The coin event may already have returned2 while that pointer remains retained.
Original global manager allocation, complete window/IRQ behavior and broader
inventory/effect/story ownership remain outside these finite adapters.
