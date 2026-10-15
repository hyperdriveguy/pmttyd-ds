# Map reveal, heading, waking and music adapters

Implementation: [map](../../include/story/opening_map_display.hpp),
[transfer](../../include/story/opening_map_transition.hpp),
[heading handshake](../../include/story/opening_heading.hpp),
[heading display](../../include/story/opening_heading_display.hpp),
[Stardust](../../include/story/opening_heading_stardust.hpp),
[wake](../../include/story/opening_wake.hpp),
[wake taps](../../include/story/opening_wake_effects.hpp),
[music](../../include/story/opening_map_bgm.hpp).
Evidence: [map reveal](../behavior/story/OPENING_MAP_DISPLAY.md),
[map capture](../behavior/display/OPENING_MAP_TURN_CAPTURE.md),
[heading](../behavior/display/NATIVE_OPENING_HEADING.md).

Map pose stages occur at18/78 ticks; request draw at168. Actual texture admission
starts alpha2, then alpha adds2 per callback, clamping only >255. The native bridge
requires actual opaque-frame publication before GF0, unlike source publish-before-
draw ordering. Wait another60 ticks, then request music/ENV off and complete.
There is no source map input gate. Capture/transfer resources are separate:
used49 exit span0, 50 entry span80; both require correct resource/capture/presentation
claims. Voyage proceeds alongside entry animation and does not wait entryComplete.

Heading phase order is entry acknowledgement ->3000 ms ->2000 ms ->start-fade
acknowledgement ->actual display started ->3000 ms ->A/B/START0x1300 ->exit
acknowledgement. Time alone cannot provide resource/display completion. Each
phase evaluates on its supplied invocation; same-time calls can consume already
published acknowledgement but do not fabricate one.

Fresh heading-display begin resets embedded Stardust readiness, requiring actual
fresh admission afterward. Eight source textures19,20,21,22,21,23,24,25 use
X=32*i+16-128 and delay1+20*i. Existing particles step before spawning new groups.
States1/2 advance background by2.5, wrap only >180 and cycle four palettes.
State2 starts letter phase at strictly negative delay, evaluates curve5 frames0..90
inclusive, then final letter rotates ring+.8 and reveals48 fixed units each tick,
capped0x4000 (64*256). These ticks adapt original display cadence, not measured
hardware or complete manager scheduling.

Stardust loads original quads/palette/scale curve and checks exact finite constants
before publication. Eight groups of four spawn once. Explicit native seed1 with
recurrence1103515245*x+12345 preserves local request counts, not original global
RNG history. Four draws per particle include the unused integer-truncated
acceleration draw. Life20/22/24/26 controls visibility; >20/<0 skips scale updates.
Seven scale bytes precede relaxation toward.1. Frame&7 selects original quad;
color requires started aged group and valid particle index. Draw/material/GPU
admission stays external; these helpers do not certify final raster or RNG parity.

Wake keeps the three original message gates: initial4000 ms, next first-close+
2000, second-close+300 starts effects and another7800 reaches the third, final
close+2300 completes. Depth/eyelid interpolate raw values then Q1024/divide10/
Q1024. Eyelid segments2000/100/100/100/500/2000 retain distinct curves; depth
converges in the final segment. View shift integrates native sine at each absolute
60 Hz tick with fused addition. This local math has no final-GPU acknowledgement;
outer scene admission and renderer capture/material work remain separate owners.

Wake blur uses eight UV taps, weights1/8. It preserves source quadrant reduction
and fused sine/cosine polynomials, not libm substitution; depth/100 then /100 sets
radius. Valid tap indices are0..7. The helper computes offsets only, not captures,
texture lifetime, blending or draw completion.

`OpeningMapBgm` uses ordinary source group0/mode512 with native catalog slots
0 harbor,7 house,8/9 Voyage variants. Snapshot is requested ownership/volume, not
hardware handles or audible completion. Repeated same-track polling preserves
fading, while explicit source On can replace that fading owner. Existing volume
becomes detached float tail with decrement/45 and retirement on next zero check;
primary fade is integer base*remaining/duration. Off preserves a running fade.
Native duration floor(ms*60/1000), minimum1 tick, is an adapter, not a hardware
clock claim. ENV and full stream allocator/MusyX services remain separate.
See [sequence](opening-sequence-house.md) and [Voyage/arrival](opening-voyage-arrival.md).
