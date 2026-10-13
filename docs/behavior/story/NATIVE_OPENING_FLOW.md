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

## Sequencing adapter implementation

`OpeningDemo` now plays24 book payloads then13 market payloads, retaining raw
bytes and timed dkey/speed/page controls. `OpeningSequence` plays four aaa
prologue payloads, one gor letter payload, aaa pro_05, a voyage input gate,
original prologue raster heading, three awakening payloads and two arrival
payloads. Diary parsing sets source window type7. Native bottom-screen layout
uses the original font mask and retains source glyph positions, scale, colors
and scroll; the dedicated diary border and full glyph effects remain pending.

Scene snapshots use original maps and actor idle clips. Their cutaway, actor
placement/pose scheduling, camera interpolation, book/page transforms, ship
movement, title curtains, environmental sounds/effects and fade owners are
unfinished. The adapter's inter-scene waits and snapshots are authored native
presentation, NOT measured original behavior or complete REL execution. In
particular the voyage gate follows the source request but its travel scheduling
is still an adapter. The story3 harbor handoff is the source final explicit
Mario request(0,15,225), not the story3-resume(0,15,250). The ordinary position
poll must then trigger016; it must not be skipped directly into a battle.

The playtest path is explicitly marked as development sequencing. Completing
faithful cutscenes requires binding the original finite/parallel event graph,
not treating this ordered-message adapter as full story-service closure.
`make debug-short` produces a separate build/target with TTYD_DEBUG_SHORT_BOOT,
so a short development boot cannot replace the normal build's front end.

Map presentation now retains the original aaa `draw` palette3 and `mapdraw`
+4 counter(+2 each call, clamp255 and WW0=1 on exceeding255):128 native
simulation updates precede GSW0=2/voyage. Original caller cadence/draw scaling
remain unbound. The letter requests palette4 from gor `draw` at REL0x116b8/
0x116e8 after `tegami` loads `%s/mariost.tpl`; the map requests palette3 at
aaa0x3c0/0x3f0. Both use original raster art. House snapshot visibility now
follows the outside S_naka exclusion versus interior S_aaa_00/S_ie_door
exclusions. Outside translated trees are omitted after their source move
endpoint; the actual translation/camera transition remains pending. Luigi's
original L_5/L_7 and L_8/L_9 idle/talk pairs replace the single idle snapshot.

The House phase now exposes a portable source-request presentation controller
with authored delivery/return/approach waypoints and original walking/carrying
clips. It gates each message on the appropriate scene stage; see
[house presentation scope](NATIVE_OPENING_HOUSE_PRESENTATION.md). This remains
a bounded native bridge rather than full REL graph execution.
