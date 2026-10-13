# Native opening and file selection

The US story order recovered read-only from `dmo.rel`, `aaa.rel`, `gor.rel`
and `muj.rel` is: `dmo_00` demonstration → title → load/file menu →
`aaa_00` prologue → `gor_10` letter → `aaa_00` prologue2 → `muj_20`
prologue → `sys_01` prologue heading → `gor_10` awakening → `gor_00`
story2 arrival → story3 harbor encounter trigger.

`Frontend` is a portable native menu adaptation. It advances at most one
screen per press edge. Demo A/START leads to title; title A/START leads to
four files. B returns to title. Left/right wraps selection, and touch selects
and opens a file. Valid/recovered files request a transactional load; missing
files request the new opening. Invalid files cannot be overwritten through
this menu. Failed load returns to file selection. Opening input never reaches
field mechanics, and exiting menus must not autosave a default field checkpoint.

Slot0 keeps `progress.bin`, slots1..3 use `progress2.bin`..`progress4.bin`.
Each uses the existing independent temporary/backup writer. This is native
PM2D storage, not original GameCube card data or a reconstruction of card UI.
See [native save format](../format/NATIVE_SAVE_FORMAT.md).

Original demo `demo_evt` spans REL0x23d4..0x3b18,392 commands,
SHA256 `3e3f3b88dd5e9c8268dfdb58cf0df3120b0548814225041879830c3e637bc6c0`.
It requests `op_book_01`..`op_book_24`, then `op_00`..`op_12`, in that order.
Book `make_msg` and `print_add` are two services for each payload, not two
copies of the narration. The source `buttonCheck` permits A/START to title.
The demonstration is distinct from the new-file opening.

Original `evt_prologue` in aaa spans0xcf0..0x1814,187 commands,
SHA256 `833162e1181ca1e25faef38ecaaa34dfba08de0ad72c95cd6693f87a5461cf5f`.
Calls are `pro_00` (Parakarry), `pro_01`/`pro_02`/`pro_03` (Luigi).
`tegami_init_evt` in gor calls `pro_04` before returning to aaa/prologue2,
which calls `pro_05` (Luigi), writes GSW0=2, and enters muj/prologue.
Source `evt_mezame` calls `mac_0_012`, `mac_0_013`, `mac_0_013_01`
with literal speaker0. Arrival calls014/015, then writes3 and enables the
position-polled encounter. See [harbor bindings](STORY_HARBOR_OPENING_BINDINGS.md).

These are static source bindings, not Dolphin observations. Full scene services,
original menu animation and new-game initialization remain to be bound. This
contract does not claim full opening execution at the initial controller step.
