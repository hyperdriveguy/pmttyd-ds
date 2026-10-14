# Native original Trouble Center window and US cache

Code commit bc65e4ac implements the [original display](TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md)
through portable drawTroubleQuestWindow and the native Renderer adapter.
Four style0/radius12 Waku frames, outlined headings, all-count row submission,
source scissor175/98/410/304 and restored source clip retain request order.
Completion5374+ID sets gray; fresh active1420 overrides red. Three row IDs are
sampled: completion lookup, active comparison, then cached-string identity after
color publication. The third ID remains captured for both columns. Each column
samples scroll before its width/draw calls; loop count remains live.

Header FontDrawScale.85 is retained state but does not scale StringMtx geometry:
cap/width X and Y1 follow the actual direct DOL callee. Rows use .85 scales and
proportional per-glyph physical pen FMA(scaleX,width+spacing,pen). The new plain
StringPitch path retains floating origins outside captured glyph scale; existing
matrix/message consumers retain their behavior. Two RNG requests per mapped
glyph remain even for off-clip quads. Source command/multibyte/LF/edge/fixed-pitch/
mutable-per-glyph scale and complete original SDK/GX arithmetic remain separate
providers; the native row path admits nonoutlined single-line original US text.

Cache owns the original gor_02 resource/header pointers and verbatim21/31-byte
client/contents slots for all30 tasks. Missing/oversized/unbound text is rejected,
never truncated or rewritten. Widths are prepared for immutable supplied font
and spacing2; changed fonts require re-preparation. Failed loads preserve prior
resources. No per-draw string parsing/heap allocation or new texture atlas.
Native uses existing original panel/font textures, Waku/glyph shader and vertex
arenas, PICA scissor, batched draw arrays and GSP cache flush. Source rows still
request all glyphs while whole off-clip quads avoid GPU allocation. Native pass
assumes full-screen entry/exit scissor, rather than querying arbitrary GPU state.
Native palette/alpha/font-global/SDK numerical equivalence remains unverified.

## Exact validation and rendering evidence

Reviewed tree35421e0b7b4d768ecc3ae9f40fb2423fadb704e1 passed make test
-o assets-ready:256/256 C++ and145/145 Python, and normal make all -o assets-ready.
Sequential make assets-ready passed; existing tools/battle_class_icon_assets.py
also prepared its original ignored pack because a newly committed independent
class-table test required it. Logs: artifacts/trouble-window-final-assets.log,
trouble-window-class-assets.log, trouble-window-final-test.log and
trouble-window-final-build.log. Normal artifacts/ttyd-trouble-window-reviewed.3dsx
SHA2565c3224471812fddd35608bc63ac8bea3ee44c577503722e78eafb9662caa0a95.
Tests verify source requests, low16 widths, red override, fresh mutable IDs/scroll,
all60 exact cached rows and per-glyph floating pen/RNG. Source review caught and
corrected the initial matrix-row consumer substitution before final validation.
Initial native fog enum mismatch and missing independent asset-pack test failure
remain recorded in trouble-window-build.log/trouble-window-test.log.

Private DEBUG_SHORT_BOOT drawing preview supplied original story8 list, test
completedquest1 and activequest2 colors; it made no quest/event/inventory writes.
Original headings and all six rows render in artifacts/azahar-trouble-window-
preview-checked/original-trouble-window-preview.png. Final smoke log
trouble-window-preview-checked-smoke.log passes frame-color checks and byte-
identical checkpoint. Preview SHA256
8bef578dd92e64696398060bba5f3888aa4c57cdb4940ee4ee468f40f42da388.
Initial check incorrectly expected purewhite: original panel centerRGBA is
(216,219,216,255), and source C0*TEXC modulation produces that observed body and
cyan(36,140,216). Corrected check uses actual source texture; failed initial log
is retained. Another input run failed because its temporary script was absent;
final script resides under ignored artifacts. No failed check is counted passed.

Private preview source restored: all2600 tracked reviewed files match the tree;
main timestamp forces the next normal-mode recompile. An unintended shared-root
preview build completed and its artifact was not used; the isolated binary above
supplies evidence. No user emulator profile/save or original payload committed.
No native icons/controller/lecture/case9/door/confirmation/acceptance admission
was added by this drawing preview. It proves the backend executes, not matched
Dolphin pixels/audio, original pause/pad parity, stable60 or hardware performance.
[Original menu producer](../field/PORTABLE_TROUBLE_CENTER_MENU_PRODUCER.md) remains
the controller foundation for the next full native board integration.
