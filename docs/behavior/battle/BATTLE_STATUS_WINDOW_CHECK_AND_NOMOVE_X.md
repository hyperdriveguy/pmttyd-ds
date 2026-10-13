# Status-window checkpoint and the X-axis camera no-move switch

US G8ME01 disc0 revision0; static contracts, no execution, cadence or gameplay
claim. Closes `BattleStatusWindowCheck` (`0x800f7514`, `battle.o`, size `0xb8`,
52 lines) and `btl_camera_nomove_x_onoff` (`0x8012b33c`, `battle_camera.o`,
size `0x38`, 20 lines). Neither has a definition in the pinned snapshot
(rechecked with `tools/reference_source_gap.py`). The Y/Z switches and
`BattleStatusWindowEventOff` are covered in
[BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md](BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md).

## BattleStatusWindowCheck (0x800f7514, 52 lines)

This is the callee that
[BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md](BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md)
listed as an unresolved `bl 0x800f7514`; it is a full local body, not an
external service. It reads the shared battle pointer `B = word[0x8041eb90]`
(`r13+7280`) and returns a boolean in `r31`/`r3`.

1. If `word[B+10044]` (`+0x273C`) is nonzero → result `0`.
2. Else if `word[B+10048]` (`+0x2740`) is nonzero → result `0`.
3. Else if `word[B+10060]` (`+0x274C`) is nonzero → result `1` (the `li 31,1`
   at `0x800f755c` is the only path that keeps the initial 1).
4. Else if `word[B+10056]` (`+0x2748`) is nonzero → result `0`.
5. Else if `word[B+10052]` (`+0x2744`) is nonzero → result `0`.
6. Otherwise → result `1`.

Then, inside the same body:

- If the result is nonzero (`0x800f7590`): call `statusWinCheck`
  (`0x8013c1f0`, statuswindow.o); if it returns 0, call `statusWinOpen`
  (`0x8013c3b8`, statuswindow.o).
- If the result is zero (`0x800f75a4`): call `statusWinCheck` again; only when
  it returns nonzero call `0x8013c390` (statuswindow.o, the close-side leaf).
- Return the boolean in `r3`.

The eight battle-word offsets are positional; their meanings are not grounded
in prose. Note the asymmetry: the blocked result opens the status window when
it is not already open, and the free result calls the close-side leaf.

## btl_camera_nomove_x_onoff (0x8012b33c, 20 lines)

Same shape as the Y/Z bodies, differing only in the mask bit and the guard's
comparison, which the Y/Z contract mis-recorded.

1. `W = word[0x8041eb90]` (shared battle pointer, positional).
2. `lwz 0, 10076(5)` loads `word[W+10076]` into `r0`, but the branch
   `cmpw 3,0` / `bclr 12,0` compares that word against the incoming `r3`
   (the first argument), **not** against zero. When they are equal the body
   returns immediately and leaves `word[W+10068]` untouched; when they differ
   it falls through. There is no signed-negative guard, and the return value is
   whatever `r3` held, because this is a plain helper rather than an event
   callback.
3. `cmpwi 4,1`: if the second argument equals 1, `ori 0,0,4` **sets** bit 2 in
   `word[W+10068]`; otherwise `rlwinm 0,0,0,30,28` clears bit 2
   (`MB > ME` wraps, so the mask is `0xFFFFFFFB`). Bit 2 is the X-axis
   no-move flag; Y sets/clears bit 3 and Z sets/clears bit 4.

The event wrapper `evt_btl_camera_nomove_x_onoff` (`0x8012b2e4`, size `0x58`)
resolves both operands through `evtGetValue` (`0x80031260`) in order — the
second operand word is resolved after the first — calls this helper with
`r3 = first`, `r4 = second`, and returns 2.

## Not established

- The meanings of `word[W+10068]`/`word[W+10076]` beyond the bit positions
  above, and of the eight checkpoint words.
- `statusWinCheck`/`statusWinOpen`/`0x8013c390` internals, caller cadence and
  any on-screen effect.
