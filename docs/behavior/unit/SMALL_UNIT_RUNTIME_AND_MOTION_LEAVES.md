# Small remaining leaves: guard wrapper, act-status publisher, party-motion setter and float→unsigned conversion

US G8ME01 disc0 revision0; static contracts, no execution, cadence or gameplay
claim. Closes four more bodies that have no definition in the pinned snapshot
(rechecked with `tools/reference_source_gap.py`):
`BtlUnit_CanGuardStatus` (`0x801272dc`, `battle_unit.o`, size `0x20`, 14 lines),
`btlevtcmd_CheckActStatus` (`0x80105cb4`, `battle_event_cmd.o`, size `0x84`,
39 lines), `unk_80151d20` (`0x80151d20`, `party_motion.o`, size `0x0c`, 9 lines)
and `__cvt_fp2unsigned` (`0x80264860`, `Runtime.PPCEABI.H.a` `runtime.c`, size
`0x5c`, 29 lines).

## BtlUnit_CanGuardStatus (0x801272dc, 20 bytes)

A frame-setting wrapper whose only action is `bl 0x8012724c`
(`BtlUnit_CanActStatus`, `battle_unit.o`, size `0x90`). It forwards `r3` and
returns that callee's result unchanged, so
`BtlUnit_CanGuardStatus(unit) == BtlUnit_CanActStatus(unit)` with one extra
stack frame; no local state, constants or branches exist. This matters only for
call-site disassembly: a `bl 801272dc` is a `bl 8012724c`.

## btlevtcmd_CheckActStatus (0x80105cb4, 39 lines)

`args = word[entry+24]`.

1. `id = evtGetValue(args[0])` (`0x80031260`).
2. `dest = args[4]` — the **raw** second operand word, kept in `r31` and used
   unchanged as the `evtSetValue` destination later.
3. `unit = BattleGetUnitPtr(word[0x8041eb90], BattleTransID(entry, id))`.
4. `r3 = BtlUnit_CanActStatus(unit)` (`0x8012724c`).
5. If nonzero: `evtSetValue(entry, dest, 1)`; else `evtSetValue(entry, dest, 0)`.
6. Return 2.

Both outcome paths publish a fixed literal, so the event variable receives a
clean boolean rather than the callee's raw value. There is no null-unit check
and no first-call gate; this is the leaf used by scripts that park a
"can this unit act" flag for later branches.

## unk_80151d20 (0x80151d20, 12 bytes)

`stfs` of the constant at `0x8042318c` (`r2−17076`, word `42700000` = `60.0`)
into the small-data word at `r13+7408` (`0x8041ec10`), then `blr`. A complete
global setter with no arguments, no reads and no gate: it publishes the single
value `60.0` to that small-data slot. The slot is in the initialised small-data
range, so the value is present from load; this body is the only in-game writer
of it that the direct-`bl` census in this pass found.

## __cvt_fp2unsigned (0x80264860, 92 bytes, 29 lines)

Metrowerks runtime double→unsigned32 conversion, driven by three
binary64 constants at `0x80303a20/28/30`:

| Address | Value |
| --- | --- |
| `0x80303a20` | `0.0` |
| `0x80303a28` | `4294967296.0` (2³²) |
| `0x80303a30` | `2147483648.0` (2³¹) |

Order:

1. `r3 = 0`; `f0 = 0.0`, `f3 = 2^32`, `f4 = 2^31`.
2. `fcmpu 0,1,0` then `bt 0`: when `f1 < 0.0` the body returns `r3` unchanged,
   i.e. **0**. (Unordered/NaN input clears the LT bit, so it does not take this
   exit.)
3. `fcmpu 6,1,3` then `bf 24`: when `f1 <= 2^32` the body returns `r3 = -1`,
   i.e. **`0xFFFFFFFF`**. NaN reaches this exit because the GT bit is clear.
4. Only for `f1 > 2^32` does execution continue: `fcmpu 7,1,4` /
   `bt 28` branches on `f1 > 2^31`, which is always true in this branch, so
   the `fsub f2,f1,2^31` at `0x8026489c` and the `0x80000000` OR at
   `0x802648b0` are **both dead** here. The word read from the stack is simply
   the `fctiwz` truncation of `f1`.

So the observable mapping is: `f1 < 0.0 → 0`; `0.0 <= f1 <= 2^32 → 0xFFFFFFFF`;
`f1 > 2^32 → trunc(f1)`; NaN → `0xFFFFFFFF`. The 2³¹ constant is loaded but
never used on any path. Rounding follows the current FPSCR setting; this body
does not modify it.

## Not established

- Callers of all four bodies and their cadence.
- The meaning of the small-data slot at `0x8041ec10` and of the value 60.0.
- `BtlUnit_CanActStatus` internals and the guard branch's gameplay effect.
- Whether the game's own call sites ever pass the out-of-domain inputs listed
  above; no caller corpus was checked in this pass.
