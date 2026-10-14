# Native original battle HUD art and button-accessible submenu

2026-10-13 native presentation increment. Source scope is US G8ME01 revision0.
This does not implement the complete status-window controller, rotating action
class menu, tutorial command scheduler or audience throw events.

`tools/battle_hud_assets.py` reads the ISO with `rb`, verifies the13 original
status icon records at8036edd0 (SHA256
`a44bcbdc5e0be34e351fd345b6b66a914ca77e16a6436c5d2a910d919ac10fbc`)
and20 numeral IDs at80415084 (SHA256
`54b8acd42d57db411d64d29bb6eb6f2741bd2ecadea16b20bd0752c36e693137`).
The statusWinDisp8013cb24 loop reads signed XY, iconU16 and scale from each
12-byte record; iconNumberDispGx80019120 selects the two10-entry numeral
arrays. These are static binary observations, not measured Dolphin behavior.
Private inspection listings and ELF outputs remain outside source history.

The shared icon converter imports all those records through original icon.bin
frame descriptors and icon.tpl pixels. Rectangle packing preserves all frame
IDs, order, delays, dimensions and RGBA pixels in the existing512x512 RGBA8
atlas (1MiB). Two-pixel transparent padding surrounds each cell. Failure to fit
fails conversion; no silent resize, substitution or truncation is allowed.
Original resources, decoded images, compiled atlas and detailed manifest remain
ignored under local-assets/message-icons/ and romfs/.

The live renderer draws Mario's original badge, HP bar, flower, FP bar,
slashes and original digit sprites. The layout is a native half-size reflow.
Harbor's explicit statusVisible gate suppresses the entire HUD. Before a trio
attack owner exists, the retained source pouch supplies current/max HP and FP;
when an executing packed Mario exists, its current HP10c/maxHP108/currentFP112/
maxFP10e win over that field snapshot. No enemy HP, coin, AP or Star Point
value is inferred. Partner HP, original slide/blink/controller cadence, AP bar
fill and broader status routing remain unbound.

Trio command/target lists use the original type0 frame, unchanged US glyph
mapping/WID1 widths/spacing2 and original selection hand504. This is a weapon
submenu presentation adapter; the full original rotating action-class menu is
still required. Plain labels come from the existing original message lookup;
no tagged story stream is flattened or rewritten. The lower-screen frame
backend shares original geometry and GPU restoration with the top-screen
backend, selecting320-wide projection/160-center and restoring the actual
lower target after each native frame submission.

The shared BattleMenuLayout defines visible lower-screen rows and optional tap
hit regions. D-pad/Circle Pad repeats still go through the original native
physical producer; A confirms and B cancels with existing priority. Taps outside
the list do not confirm. Required actions have physical alternatives. The old
horizontal touch regions are retired for this original-script trio submenu;
the independent legacy Crump development harness remains separately scoped.

Validation checks source table pins, original pixel equality, every atlas cell's
bounds/nonoverlap, list edges/row mapping and existing physical navigation,
confirmation and cancellation. Native captures must be inspected separately;
passing host tests does not prove original-runtime or hardware raster parity.

Next: bind the original tutorial player-command scheduler and full rotating
menu, then original audience actors/throw telegraphs/targeting before adding an
optional lower-screen tap shortcut with a physical stop-throw alternative.
The current lower screen remains a submenu surface, not a fabricated audience.
