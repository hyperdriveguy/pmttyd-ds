# Script, message and effect asset provenance

Original event script assets, US-default disc-thread system-message resources and the shared effect texture source. Split out of [ASSETS.md](../ASSETS.md) on 2026-10-12; sections are verbatim.

## Original event script assets (2026-10-12)

`make event-assets` reads /home/carson/games/roms/G8ME01.iso read-only and uses
ignored artifacts/reference/us_symbols.csv to verify original DOL address/file
mapping. tools/pack_event_scripts.py pins the three default streams:
ordinary Jump80355cf0/f0c,power-check child80363388/8c,and badge tail8034f2e4/1e8.
Their314 commands/raw operands remain big-endian and unmodified. Four original
bytes AFTER each symbol are retained because label construction reads the
terminal operand unconditionally. Matching overlap/adjacent intervals are merged;
no inferred zero padding,dialogue generation or control-token rewriting occurs.

Ignored outputs:local-assets/events/jump.pmevt(.json),and
romfs/events.pmevt(.json) from the Make target. Both packs are4540 bytes with
SHA2565429b0beab40e4af68725a87c0ee8b2f73df9fe67986d55affcc7eb9d7042c70.
PMEVT001 is a port container with sorted original-address ranges and canonical
file extents. Native event_script_assets.cpp reads original bytes/words;addresses
are never cast to host pointers. Manifest retains per-stream/segment hashes,
addresses,annotations,source identity and explicit extraction limits. Pack and
manifest stay outside source history;local3dsx embeds the RomFS data.

Original Nintendo/Intelligent Systems game data;no redistribution rights granted.
The ISO remains unchanged and untracked. Parser/reader/tooling is authored port
code;no original executable function or ISO was committed. Default source hashes
match supplied US revision0;Jump/badge match their behavior verifier fingerprints.
This is asset import,not proof of original callback registration/entry owners or
executed gameplay. Native storage/registry/time/FPSCR/heap services remain next.

## US-default disc-thread system-message resources (ID2,2026-10-12)

Read-only source:/home/carson/games/roms/G8ME01.iso,US G8ME01 disc0/revision0.
[Presentation contract](../behavior/platform/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md)
records original DOL language table slot1 and codes10..13 at virtual803019a0,
803019f8,80301a3c,80301aac;ISO offsets31eca0,31ecf8,31ed3c,31edac;lengths
87/67/109/123 bytes including NUL. Exact source SHA256 values are in that contract
and tools/verify_event_readiness_gate.py. Text/newlines/language association
must be reused unchanged if extracted;no paraphrase or invented system notice.
No conversion/extracted message body or compiled pack was added by ID2;
inspection ELF/listings/resource metadata remain /tmp/id2-gate-*. Copyrighted
original-game/SDK data retains the existing proprietary asset/license posture;
this research grants no distribution license. Only pointers,lengths,hashes and
selected instruction/data anchors are source-versioned. SDK console-ROM font
texture/encoding is a separate runtime source,not proved present in this ISO;
font realization/reuse licensing remains unresolved and no bitmap conversion
was invented. Story dialogue continues to use exact msg/US IDs/control tokens.


Partner floor/slope/heading research by ID1 (2026-10-12):
verify_field_partner_ground.py and field_partner_ground_evidence.json pin four
DOL identities (floor query,heading adjustment and height leaf complete;shared
backend selected result-layout/filter regions only),100 instructions,28 exact
constants,four vector/numeric spans and two lecture imports. Actual stay
postpass calls are pinned. Original ELF/listings and field-partner-ground-
evidence.json remain ignored under artifacts/reference/behavior. ISO read-only;
no asset conversion,original execution,profile/save mutation or original binary/
dialogue distribution. Full scene/trig/angle/frsqrte/platform/source domains
remain separate from metadata identities.


Partner support/screen recovery research by ID1 (2026-10-12):
verify_field_partner_recovery.py with field_partner_recovery_evidence.json pins
seven DOL identities (six complete local bodies;shared backend selected result
regions only),105 instruction anchors,18 constants,six data spans including
12 exact signed angle words,and two lecture imports. Original appearance/ray/
reset/projection/control listings/ELFs and field-partner-recovery-evidence.json
remain ignored under artifacts/reference/behavior. ISO read-only;no original
execution,asset conversion,profile/save mutation or original binary/dialogue
distribution. Full projection/scene/placement3/domain services remain separate
from pinned identities;all reported behavior is static evidence.


Partner placement3/math/NPC research by ID1 (2026-10-12):
verify_field_partner_placement3.py and field_partner_placement3_evidence.json pin
six DOL identities (five complete local bodies;shared backend selected result
regions only),108 instructions,22 constants,nine data spans including nine exact
signed angle words and the existing named-support pointer/name identities,and
two lecture imports. Actual screen-reset caller is pinned. Original inspection
ELF/listings and field-partner-placement3-evidence.json stay ignored under
artifacts/reference/behavior. ISO remained read-only;no asset conversion,original
execution,profile/save mutation or original binary/dialogue distribution. NPC/
scene/numeric/source and original trajectory domains remain separate.


Partner motion2 jump research by ID1 (2026-10-12):
verify_field_partner_jump.py with field_partner_jump_evidence.json pins six
complete local DOL identities,136 instructions,17 constants,two data spans,
two lecture imports and four actual walkMain dispatch calls. Original handler/
index/landing ELF/listings and field-partner-jump-evidence.json stay ignored
under artifacts/reference/behavior. IDs0927/0928 are original takeoff/landing
sound REQUESTS;full pose/audio playback is not inferred or newly authored here.

ID1's required make all invoked the concurrently edited existing audio-map
pipeline:3218 GameCube/0 Switch identities and five slice primary-sample WAV
fallbacks. Inputs were read-only sound/proj/pmario.proj/pool/sdir/samp from the
supplied ISO;raw/csv/json catalogs are local-assets/audio-map,packed outputs
romfs/*.wav and romfs/audio-map.json,using existing config/audio_bindings.json.
No conversion choices/config/audio code were changed by ID1. The pipeline's
current choices preserve catalog loop metadata but remove one-shot loop sidecars;
macro modulation/child voices are not rendered by those primary-sample fallbacks.
Original assets retain copyright;local derivative use grants no redistribution
rights. Catalogs/raw/audio packs were left unstaged outside this scoped research
commit. No original execution,ISO/profile/save mutation or original binary/dialogue
distribution occurred;this build-triggered conversion is explicitly recorded.


Partner live direction/activity/front-clearance research by ID1 (2026-10-12):
verify_field_partner_live_services.py with field_partner_live_services_evidence.json
pins eight DOL identities (six complete local bodies;backend/reinit selected
regions),98 instructions,16 constants,three numeric/vector spans,two lecture
imports and exact DOL census of nine SDA accesses/one linked setter call.
Original ELF/listings and field-partner-live-services-evidence.json remain ignored
under artifacts/reference/behavior. make all again invoked the existing audio-
map pipeline:3218 GameCube/0 Switch sound identities and five PCM fallbacks,
using the read-only ISO sound/proj paths and local-assets/audio-map/RomFS paths
recorded in the preceding motion2 provenance entry. Outputs remained unstaged;
ID1 changed no conversion/config/audio code or licensing policy. Original assets
retain copyright;local derivatives carry no redistribution grant. No original
execution,ISO/profile/save mutation or original binary/dialogue distribution.
Global parameter census is direct DOL evidence,not an alias/REL/runtime invariant.

## Original US shared effect texture source (ID2,2026-10-12)

Effect driver selector1 resolves `./e/us/effect_us.tpl` through the original
`%s/e/%s/effect_%s.tpl` format and US prefix. Read-only FST inspection of
/home/carson/games/roms/G8ME01.iso confirms path `e/us/effect_us.tpl`,106880 bytes,
SHA25693ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14.
`tools/verify_effect_driver.py` verifies current FST path/size/hash and original
selector/format bindings. No extraction/conversion/scaling or texture rewrite
was performed;native TPL indexing/palette/filter/resource consumers remain open.
Reuse the original source through the documented asset provider;do not commit
or distribute the ISO/extracted TPL/compiled packs. Original Nintendo/Intelligent
Systems data;this project records no redistribution licence or rights transfer.
See [driver/cache source](../behavior/effect/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).
# 2026-10-12 — Complete US message resources for native playback

`tools/message_assets.py` opens the US ISO read-only and copies every
`msg/US/*.txt` resource byte for byte into ignored `romfs/messages/`.
The local manifest records source paths, ISO offsets, byte sizes, SHA-256,
pair counts and duplicate-key metadata. All 260 resources / 13,186 pairs retain
their original IDs, high bytes and control tokens. No dialogue is committed.
No encoding normalization, text rewriting or token stripping occurs.
Original Nintendo/Intelligent Systems rights remain applicable; these local
assets have no redistribution license.

`MessageResource` builds an offset index over exact raw strings, rejecting
malformed/truncated resources atomically. `MessageAssets` queries room slot
before global slot. Missing keys return null in this native adapter; original
localized fallback binding remains open. Duplicate pairs are retained, but
ambiguous keys return null until original equal-key qsort ordering is recovered.
Only resident resource bytes/indexes need RAM; the full library resides in RomFS.
This delivers asset lookup, not original parser, fonts, story or window playback.

## 2026-10-12 — Original US font and harbor story3 playback assets

`tools/font_assets.py` reads `f/papermarioset_US.bfn` from the read-only US
ISO. Ignored `romfs/font.pmfont` contains the original byte-to-glyph map and
162 glyph rectangles/widths; `font.t3x` contains the original512x256 I4 mask
converted to white RGBA with intensity as alpha. Source WID bearing and GLY
row+2/height22 coordinates are retained. The ordered FontSetup space/replacement
width writes7 then9 leave glyph0 width9, as specified in the font contract.
Local `font.json` records source offset/hash, conversion and rights. No font
bytes, bitmap or original dialogue are committed.

`MessagePlayback` retains the original ID and raw text while emitting source
24-byte glyph/line/key/scroll records for the opening domain, including wait,
speed, scale, dynamic and shake flags. Source25 default schedule, key ordinals,
LF suppression after controls, scrolling, latched acceleration and ordinary
opening/closing fade drive playback through the fixed-tick millisecond bridge.
Unsupported commands fail atomically; selection/history/paper/icon/audio and
full window/material/RNG consumers remain open. Glyph rendering uses the
original mask/widths with adapted screen geometry; dynamic relaxation follows
the recovered draw mutation. Shake/noise and the original bubble texture remain
unbound. This is scoped native presentation, not original raster equivalence.

`tools/harbor_actor_assets.py` reads `a/c_kanbu_ibt`, its header-selected texture
bank `a/c_kanbu-`, and `a/c_zako_n`/`a/c_zako_n-`. Original setup Z_1 poses bake
to ignored `harbor-actors.t3x`, with root offsets/source scalar2 in the local
binary/manifest. Goombella reuses the original animated c_pkr pack. Crump and
subordinate animation/automatic-pose services remain unbound.

The source story3 call `bgmon(512,BGM_EVT_DANGER4)` binds catalog BGM row164
to `sound/stream/evt_kik4_32k.stm`; its original PCM16 conversion and loop data
are packaged as `harbor-danger.wav`. The local harbor manifest includes source
paths, hashes and conversion metadata. Native NDSP switches to this stream at
the trigger; original mix/envelope/environment/text SFX macro services remain
open. All these assets retain Nintendo/Intelligent Systems rights and stay
local/ignored without a redistribution license.
