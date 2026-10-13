# Unit Monosiri (spatial sound) source lookup

**Address:** `0x8012ef20`
**Size:** 20 bytes (5 instructions)
**Namespace:** `battle_monosiri.o`
**Evidence class:** B (inspected US bytes/control flow — **complete body**)

## Overview

`battleGetUnitMonosiriPtr(kind)` returns the address of the 24-byte spatial
sound record for a battle unit kind. It is the smallest function documented
in this family and it answers the request filed in
[EVT-01](../../requests/EVT-01.md) for a complete body.

## Body

```asm
8012ef20: mulli   4, 3, 24           ; r4 = kind * 24
8012ef24: lis     3, -32713          ; r3 = 0x80070000
8012ef28: addi    0, 3, -12480        ; r0 = 0x80070000 - 0x30c0 = 0x8036cf40
8012ef2c: add     3, 0, 4             ; r3 = 0x8036cf40 + kind*24
8012ef30: blr
```

## Result

```cpp
// kind is treated as a SIGNED 32-bit int, multiplied by 24
uint8_t* battleGetUnitMonosiriPtr(int kind) {
    return (uint8_t*)(0x8036cf40 + (int)(kind * 24));
}
```

| Property | Value |
|----------|-------|
| Table base | `0x8036cf40` |
| Record stride | **24 bytes** |
| Sign handling | `mulli` is a **signed** 32-bit multiply |
| Overflow | **none** — `mulli` wraps modulo 2^32 |

## What the body does *not* do

This directly answers the open parts of the request. The function
performs **none** of the following:

- **No bounds check.** There is no compare of `kind` against any kind
  count. A kind beyond the table returns a pointer past its end.
- **No negative-kind check.** `kind = -1` yields
  `0x8036cf40 - 24 = 0x8036cf28`, a valid-looking address well below the
  table. Negative kinds are not rejected.
- **No null return.** The function has no path that yields `0`. A caller
  testing for `NULL` is testing the *contents* of a record, not this
  function's result.
- **No capture or field extraction.** It returns a bare pointer; nothing
  is read from the table.
- **No sentinel.** Unlike the positional-SFX family, there is no `-1`
  handle convention here.

### Consequence for callers

`BATTLE_ENEMY_CANDIDATE_SAMPLING.md` records the caller predicate as
"`battleGetUnitMonosiriPtr(kind) == null`, or its word `+4 == 0`". Given
the above, that null test is **not** testing this function's return value
for a failure — it is dereferencing the returned record and testing a
**field** of it. The `== null` reading in that document should be read as
"the record's null field is set", not "the lookup failed".

That caller's note that "the nonnull branch calls that lookup a second
time" therefore performs the whole function twice — including the
multiply — for the same `kind`. A port may cache the pointer between the
two calls; that is a safe simplification because the function is pure.

## Table base and symbol

The base `0x8036cf40` is the **`monosiriMessageTable`** symbol (`.data`,
`battle_monosiri.o`).

| Property | Value |
|----------|-------|
| Symbol | `monosiriMessageTable` |
| Address | `0x8036cf40` |
| Section | `.data` |
| Size | **5544 bytes** |
| Stride | 24 bytes |
| Records | **231 exactly** (5544 / 24 == 231) |

The size divides by the stride with **no remainder**, which independently
confirms the 24-byte stride recovered from the `mulli` and gives the table
extent: valid kinds are `0 .. 230`. The body still performs no bounds
check, so this bound is a property of the table, not of the lookup.

## Record layout

The 24-byte stride is established, but the field offsets are **not**. The
one known offset is `+0x04`, from the caller predicate recorded in
[BATTLE_ENEMY_CANDIDATE_SAMPLING](../battle/BATTLE_ENEMY_CANDIDATE_SAMPLING.md):
a caller reads word `+4` and treats `0` as "no spatial sound". Nothing in
this 20-byte body reads or writes the record, so the remaining offsets
must come from the record's consumers.

## Porting notes

1. Implement as a pure address computation. It has no side effects and no
   failure mode.
2. Use a **signed** 32-bit multiply. `kind * 24` for a large positive kind
   overflows to a negative offset; an unsigned multiply would differ.
3. Do not add bounds or negative-kind rejection. Callers rely on reaching
   whatever record the index names, and the original has no guard.
4. The two-call pattern in the sampling caller can be collapsed to one
   lookup by the caller without changing behaviour.

## Remaining gaps

- Field layout of the 24-byte record beyond the `+0x04` null test.
- The unit-kind enumeration and which kinds index valid records.
- Writers of the table: where the records are populated at battle-unit
  setup. `battleAcMain_MonosiriTarget` and the
  `monosiriExit` call at `8017225c` (recorded in
  [SDK_HEAP_RELEASE_AND_CALLER_CENSUS](../memory/SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md))
  are the nearest leads.
- The full call census of `battleGetUnitMonosiriPtr`. Only the
  `BATTLE_ENEMY_CANDIDATE_SAMPLING` consumer is currently documented.
- `BattlePackDmgTable` and the ordinary takeoff path are reported not to
  call it; that claim is inherited from existing docs and was not
  re-verified this session.
- No original execution trace.