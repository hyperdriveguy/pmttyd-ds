# Battle after-reaction queue: init, entry and release

US G8ME01 disc0 revision0; static contracts, no execution, cadence or gameplay
claim. Closes `BattleAfterReactionQueueInit` (`0x800f7af0`, `battle.o`, size
`0x38`), `BattleAfterReactionEntry` (`0x800f7a78`, size `0x78`) and
`BattleAfterReactionRelease` (`0x800f79e4`, size `0x94`) from the
[unaddressed register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md). None of the
three has a definition in the pinned snapshot (rechecked with
`tools/reference_source_gap.py`). `BattleAfterReactionMain` (`0x800f78fc`) is
the consumer loop and is **not** covered here.

## Queue layout

All three bodies read the shared battle pointer `B = word[0x8041eb90]`
(`r13+7280`) and address a 64-record array at `B + 0x16F5C`
(`addis 1` + `addi 28508`/`0x6f5c`). Each record is 8 bytes:

| Offset | Role |
| --- | --- |
| `+0` | key, an event/entry pointer (`-1` = free) |
| `+4` | value word, accompanying payload |

Slot 63 starts at `B + 0x16F5C + 504` (`0x1f8`), matching the `504`/`508`
store offsets in the releaser.

## BattleAfterReactionQueueInit (0x800f7af0, 20 lines)

A single unrolled loop over 64 records: `key = -1` (`li 5,-1`) and
`value = 0` (`mr 4,3` with `r3 = 0`). No reads of prior contents, no gates, no
calls. Void body.

## BattleAfterReactionEntry (0x800f7a78, 36 lines)

`Entry(entry, value)`:

1. **Duplicate scan** — for each of the 64 slots, skip `key == -1`; on
   `key == entry` (`cmpw 3,0`, `bclr 12,2`) return immediately with `r3`
   unchanged, i.e. it returns the **entry pointer itself**. No second copy is
   queued.
2. **Free-slot scan** — walk the array until the first `key == -1`, keeping the
   running count in `r6`. If no free slot exists the count reaches 64.
3. **Full check** — `cmpwi 6,64` / `bclr 4,0`: when the count is not below 64
   the body returns the entry pointer unchanged; nothing is stored.
4. **Insert** — `key = entry` at slot `count` (`stwx 3,7,count*8`) and
   `value = r4` at `slot+4`, then return the record address
   `base + count*8`.

So the return value distinguishes outcomes: equal to the caller's entry pointer
means "already queued or queue full"; a different address is the new record.
There is no null check on the entry pointer, and `value` is stored unmodified.

## BattleAfterReactionRelease (0x800f79e4, 43 lines)

`Release(entry, value)` walks slots 0..63 while `index < 64`:

1. Skip `key == -1`; require `key == entry` **and** `value == r4`
   (`cmpw 3,0` and `cmpw 4,0`), otherwise advance.
2. On a match, compact: `mtctr = 63 - index`, and if `index < 63` copy each
   following record down one slot (`[i+k] = [i+k+1]` for both key and value).
   The loop is skipped when `index == 63` because there is nothing to shift.
3. Terminate: `slot 63 key = -1`, `slot 63 value = 0`.
4. Decrement `index` before the loop increment, so the slot just shifted down
   is re-examined. Consecutive matching records are therefore all removed in
   one call.

The body is void and returns nothing (`blr` with `r3` untouched from the
caller).

## Not established

- The queue's frame/tick cadence and what `value` identifies (the consumer loop
  is documented elsewhere; the callers of these three bodies are not).
- Whether entry pointers can alias each other such that a returned record
  address could equal a queued entry address; the code does not test for that.
- No caller census is included here; callers live in the same object and their
  registration semantics are undocumented.
