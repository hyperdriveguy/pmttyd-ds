# Action-class UI allocation and native providers

2026-10-14. Static US G8ME01 revision0 source recovery; no Dolphin execution
or hardware raster parity claim. Complete bodies: ActSelect Init80116cdc/0x1a4
and End80116c78/0x40, pinned by the existing
[act-menu verifier](../../../tools/verify_act_menu_control.py). They accompany
[setup/end and position admission](ACT_MENU_SETUP_END_AND_POSITION_GATE.md).

End reads B+0x1c78, frees a non-null pointer, then clears that owner word.
Init frees any prior non-null owner, allocates0x124 bytes and publishes it at
B+0x1c78 before capturing W. It does not clear the allocation. W+0/W+4 retain
the cursor/row addresses; W+0x0c is10 when flag bit0 is set and0 otherwise;
BYTE(W+0x14)=0 gates input; W+0x18 takes the fresh cursor selection.

The resume loop initializes selected-first rows, fresh count/selection reads,
state10 for the selected row and11 for others, zero delays and accumulators,
and reverse order count+ordinal-1 with one signed wrap correction. The entry
loop visits the next row first, retains each loop-head count for its wrap,
and writes state0, ordinal order and zero accumulators. Delays use fresh
WORD(WORD(80415fd0)+4), wrap32 multiplication by10, signed division by60,
then wrap32 multiplication by ordinal and signed division by2. These are
source arithmetic constants, not measured animation durations. Untouched
words, including W+8 and unused array elements, stay intact.

The portable implementation returns readiness through typed allocation/free
providers; this bool is not the original ABI scalar result. A provider
frontier after a successful free can retain earlier effects and is not a
transactional retry boundary. No implicit memset or rollback is supplied.

The harbor adapter binds whole class setup/main/end through the existing
command-input controller, actual packed cursor/table/status/position services,
current physical pad producers and camera gates. Allocations use its SDK heap0
and tracked class ownership. The older fixed weapon UI projection has an
explicit native lifetime adapter; it is not presented as an original heap
allocation. Weapon UI End now releases the current owner before clearing it.
Original sound tokens are recorded; full original playback remains open.

`openClassMenu()` is an explicit development entry into source state0.
`classMenuPresented()` acknowledges BYTE(W+0x14)=1 after native presentation;
it is not a replacement for recovered original animation/draw lifecycle.
Original ActSelect Main is literally a return instruction and does not itself
prove that the renderer has presented anything. Game activation, circular icon
placement, speech bubble and original presentation cadence remain unbound.

Host tests cover entry/resume arrays, preserved words, signed delay arithmetic,
empty tables and owner teardown. The harbor test cancels weapons with physical B,
waits while class readiness is0, acknowledges presentation and selects Jump
with physical A, releasing the class allocation before weapon setup. Jump is
explicitly enabled in that fixture's retained pouch; bare-pouch eligibility is
not changed globally.
