# Original asset pipeline

## Original Mario data-child pack extension (2026-10-12)

`make event-assets` now includes subset_attack_ready_jump_event@battle_event_subset.o
at original80416bd0,size8,SHA2561e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61.
Its DELETE2/terminal1 words and four ORIGINAL trailing bytes at80416bd8 remain
byte-exact. The tiny event has real constructor/deletion/ID/copy-back effects;
no zero padding or callback-completion replacement was generated.

Current four-stream ignored local-assets/events/jump.pmevt and romfs/events.pmevt
(with JSON sidecars) are4564 bytes,SHA256
82dd17bd66ff013fa3df32d25134b01d4077f11460bab222d9da0aad4d43790d.
Prior4540-byte three-stream fingerprint below is historical/superseded. All316
command words/raw operands,original addresses and scanner extents are retained.
No new USER_FUNC site is added by this child;direct original callback census
remains72 addresses for the original three main streams. Native reader and
manager integration verify the actual extracted child without claiming actual
Mario kind/table import or complete Jump runtime. Original Nintendo/Intelligent
Systems copyright/no redistribution rights and read-only ISO/source-history
exclusion remain unchanged.

## Isolated native audio validation firmware (2026-10-12)

User supplied /home/carson/games/roms/dspfirm.cdc. Read-only source copied to
ignored artifacts/azahar/data/azahar-emu/sdmc/3ds/dspfirm.cdc;49,756 bytes,
SHA2568e213f3e71d2e3e45d1169bac6465a70eabeb22b303f1fa6d7679370ffad0f54.
Source/copy hashes match. No patch/conversion,embedding,source-history addition
or redistribution occurred;original firmware copyright/licensing remains in
force. This component is separate from GameCube/Switch game sound assets.

The prior isolated Audio unavailable finding is resolved:Settings shows Audio
service ready,private-sink PCM capture verifies nonzero music,field jump waveform
support,digital mute silence and restored output. Capture derivatives/metrics
stay ignored in artifacts/azahar/audio-verify*. No user emulator profile/saves,
firmware source or default audio device changed. Dedicated sink was removed.
No original-game audible equivalence/hardware/all-cue validation is claimed;
see AUDIO_MAPPING.md and the progress follow-up for exact measurements/logs.


## Sound mapping and GameCube native fallbacks (2026-10-12)

[Audio mapping](AUDIO_MAPPING.md) documents a reproducible catalog of3,218 named
US revision0 GameCube BGM/SFX entries,1,247 sample directory IDs and10,256 Switch
sound records. Original DOL table hashes/addresses/flags/parameters and exact
stream aliases are retained;FX→group/object→direct macro/sample and Switch
sound→FWSD note→wave archive/member dependencies are separate identifiers.
9,679 Switch wave references and577 stream references resolve to existing PCM;
28 records expose conflicting group-local variants. Name-derived categories do
not establish original story conditions,event order or ownership.

Sources:read-only /home/carson/games/roms/G8ME01.iso sound/proj/pmario.{proj,pool,
sdir,samp},DOL sound tables and selected sound/stream files;existing Switch
local-assets/switch/audio library. Full raw/catalog/CSV/PCM outputs stay ignored
in local-assets/audio-map/. Native-only conversion uses its separate native/
subdirectory and ignored romfs/audio-map.json and five effect WAVs. Authored
config/audio_bindings.json and tools/audio_map.py contain no original PCM/code.

Fallback PCM is decoded at original rates/channels,without normalization or
resampling. One-shot effects omit sidecars while retaining source-loop metadata
in catalog. Primary samples do not render macro pitch/envelope/child voices,
reverb/spatialization or original voice-priority rules. Field jump deliberately
uses audible fallback gain despite its zero-volume original entry. Outdoor
Rogueport now uses the original GOR-table stream stg_gor2_32k rather than the
CSN-table stg_gor1_32k. Win1's original whole-file loop is retained;nonlooping
source files are no longer forcibly repeated. Complete original event binding,
hardware/Dolphin audible comparisons and native NDSP playback validation remain
open. Isolated Azahar lacks a DSP component and reports Audio unavailable.

Original assets retain original copyright;local conversion grants no
redistribution rights. No ISO,original banks,PCM,dialogue or compiled packs are
committed. GCMusyXTools is a primary STARTSAMPLE layout reference;no external
implementation was copied/compiled. NintendoWare sources/licenses remain as
recorded in switch/AUDIO_ASSETS.md. All source inputs were read-only;the automated
emulator used only the project-local isolated profile and test saves.


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

## Switch 1.0.0 audio extraction (2026-10-12)

Completed the separate Switch audio library from the verified read-only
`local-assets/switch/romfs/` extraction of the supplied base NSP. Ignored
`local-assets/switch/audio/` preserves all 670 sound-tree files and 594 structured
bank/control/group extracts, and contains PCM16 WAVs for all 577 Opus streams
and 9,691 DSP bank waves. The full library is separate from GameCube assets and
native RomFS. [Switch audio provenance](switch/AUDIO_ASSETS.md) records paths,
source package identity, conversion choices, decoder build and licensing.

Manifest and archive inventory retain all 10,256 sound names/IDs, all 62 global
wave-archive records, source hashes/spans and external loop metadata. Group-local
file-ID conflicts are preserved as contextual variants rather than overwriting
global resources. WAVs preserve source rates/channels; 421 music tracks and
807 bank waves have native-format loop sidecars. PCM files total
11,385,943,684 bytes; source sound-tree files total 1,941,989,744 bytes.
Verified all 10,268 WAV formats/full lengths/hashes/loops, original FWAV header
fields for all 9,691 waves, and all 670 original/copied source hashes. All 22,453
library files remain ignored and no originals are tracked.

Decoder: external vgmstream r2117, commit7c520e8f9b078dfde2f84f1b5bcbad0c6c415806,
ISC with existing matching third_party notice. libpulsar MIT and Citric Composer
snapshots supplied format facts only; no implementation copied. Existing original
copyright and no-redistribution rights remain unchanged. No NSP/key/firmware,
source RomFS, emulator profile/save or native audio selection was modified.
These are decoded assets, not faithful runtime mixes/event playback; see the
linked provenance for source-domain, alias, codec and audit limitations.

## Ordinary head-query binding (2026-10-12)

Native31f6cb8 binds the read-only US DOL contract in
docs/behavior/OVERWORLD_AIRBORNE_AND_LANDING.md with ce29235/9869179/f305f1d
addenda. Its verifier checks26 bodies,185 anchors and19 data spans in
/home/carson/games/roms/G8ME01.iso without mutation. No asset conversion or
pack format changed: existing ignored romfs/*.head retains original triangle
winding/cull/attributes at scale.01. Raw ray normals feed head's projected angles
without support's Q1000 normal rounding. Host geometry/trig/scaled arithmetic
remain numerical adaptations. Mesh inspection at plaza(-6.7,.1) found rear roof
inclination114.93 degrees (rejected) versus center132.97 (admitted); the former
native peak fixture had accepted the rejected face. Updated native regression
uses these static cone/probe decisions;this is not original gameplay measurement.
Original copyrighted assets,ISO,compiled packs and emulator captures remain
outside source history;this work grants no distribution license.

## Complete GameCube SFX sample extraction (2026-10-12)

The earlier loop-only conversion is now extended to **all 1,247 samples** from
`sound/proj/pmario.sdir` and `sound/proj/pmario.samp` in the read-only US
`/home/carson/games/roms/G8ME01.iso`. `make audio-assets` refreshes all six bank
resources and converts every indexed sample into ignored
`local-assets/audio/pcm/samples/pmario-<four-digit-hex-ID>.wav`. The 1,035
nonlooped samples have no sidecar; the 212 looped samples retain their
`.wav.loop` intervals. All preserve original sample IDs, mono PCM16 data,
rates and lengths without resampling, normalization or generated replacements.

Sample WAVs total 39,383,724 bytes. Library schema 2 now records 1,393 outputs
(146 music tracks + 1,247 bank samples), totaling 930,759,732 WAV bytes.
Verified all WAV formats/full data lengths/hashes/sidecar bounds, exact coverage
of every directory ID, and all 152 raw-resource hashes. All 2,057 library files
are Git-ignored and none is tracked. Unchanged music can be reused only after
source/decoder fingerprints, WAV metadata, loop sidecars and PCM hash validation;
corrupt music is reconverted. The complete bank directory and original
`pool/proj/etbl/slib` resources remain preserved with extraction provenance.

This completes sample extraction/conversion, not MusyX effect rendering:
macro sequences, envelopes, pitch, layered voices and game event-to-effect
bindings remain unimplemented. Original copyright/no redistribution rights and
existing format-reference licensing remain unchanged; no additional third-party
implementation was copied or linked. Assets remain outside RomFS and source
history. No original-game listening or hardware audio equivalence is claimed.

## Ordinary airborne/landing binding (2026-10-12)

Native97fd570 and db71cca use the US DOL contract in
docs/behavior/OVERWORLD_AIRBORNE_AND_LANDING.md, recovered in35f82d8 with
a4df746/ce29235 addenda. tools/verify_overworld_airborne_and_landing.py reads
/home/carson/games/roms/G8ME01.iso read-only to verify original bodies,
instruction anchors and literals. Inspection does not mutate the ISO or link
original executable code. No asset extraction/conversion choice or pack ABI
changed in this native slice: existing ignored scale.01 original *.ground/
*.head provide winding/cull/attributes to the shared support service. Returned
normal/point quantization and host trig/intersection/scaled float arithmetic
remain adaptations. Original copyrighted assets and compiled/capture outputs
remain outside source history; this work grants no distribution license.

## Full loopable audio library (2026-10-12)

`make audio-assets` / `tools/extract_audio.py` reads
`/home/carson/games/roms/G8ME01.iso` read-only. The completed local library is
under ignored `local-assets/audio/`, separate from the current RomFS pack:

- `raw/sound/stream/*.stm`: all146 original streams with extraction provenance.
- `raw/sound/proj/pmario.{sdir,samp,pool,proj,etbl,slib}`: six original MusyX
  resources. `sample-directory.json` preserves all1,247 indexed sample IDs,
  offsets, coefficients, rates, counts and loop bounds.
- `pcm/streams/<original-stem>.wav`:146 PCM16 tracks, retaining source32,000Hz
  and mono/stereo channels. All146 STM headers enable looping.
- `pcm/samples/pmario-<four-digit-hex-ID>.wav`:212 mono looped bank samples,
  retaining original rates and exact IDs. Nonlooped samples remain available
  in the preserved bank and can be converted with `tools/audio_convert.py`.
- `<file>.wav.loop`: two little-endian uint32 PCM frame indices `[start,end)`;
  indices apply before playback resampling, count frames rather than individual
  stereo samples, and preserve the intro before repeating the loop region.
- `manifest.json`:152 source records with ISO paths/offsets/sizes/SHA-256,
  358 output records with formats/loops/PCM hashes, and converter fingerprints.

Conversion uses the existing DSP ADPCM decoder; no normalization, resampling,
crossfade, generated tones or inferred event names. STM inclusive nibble ends
become exclusive PCM frame ends and are capped at source sample count, following
the existing converter. MusyX uses source start+length; lengths0/1 are nonloops.
Every output was checked for WAV format, full data length, frame count, valid
loop bounds and exact sidecar bytes. A separate post-conversion pass verified all
358 outputs/hashes and152 raw hashes. Rogueport, battle and victory PCM/loops
match the existing RomFS files byte-for-byte. PCM WAV payload files total
903,557,248 bytes; original resources total271,317,240 bytes. The full library
is not embedded into the3DS executable; the early slice retains three tracks.

These are local original-game derivatives with original copyright and no new
redistribution rights. ISO, raw files, WAVs, sidecars, indexes and manifests stay
ignored; only authored tools/tests/docs are committed. Existing vgmstream ISC
format-reference notices and MusyX research provenance above/below still apply;
no new third-party implementation was copied or linked. MusyX macro/envelope/
pitch/event rendering remains unbound. PCM repetition has not been audibly
compared with original ADPCM loop-history execution; no Dolphin/hardware audio
equivalence or seam-free playback claim is made.

## Ordinary support/slopes/upstairs binding (2026-10-12)

Native commit78eb86b implements the bounded US support contract in
docs/behavior/OVERWORLD_SUPPORT_SLOPES_AND_STEPS.md, with documentation
addendum b110e63. Its verifier reads /home/carson/games/roms/G8ME01.iso
read-only:13 DOL bodies,132 instruction anchors,24 data spans. No original
executable was linked or copied into the port, and no extracted or converted
asset format changed. Existing ignored romfs/*.ground and *.head retain
original source winding/cull/attributes at scale.01; native ray hits orient
back-facing normals toward their origin before support-angle interpretation.
Host trig, geometry intersections and scaled arithmetic are numerical
adaptations, not original SDK execution. ISO/converted packs retain original
copyright and remain outside source history; this work grants no distribution
license. Emulator captures and isolated saves remain ignored in artifacts/azahar.

## Behavior research provenance (2026-10-12)

`tools/behavior_evidence.py` inventories the read-only ISO and hashes its DOL
span, `rel/gor.rel`, and all 260 `msg/US/` files without exporting their contents.
Output is ignored `artifacts/reference/behavior/evidence.json`; function ELFs
and disassembly also stay in that directory. There is no conversion or native
linking of original executable code in this research step. Binary identity,
source URLs and inference boundaries are in [ORIGINAL_BEHAVIOR.md](ORIGINAL_BEHAVIOR.md).

Public doldecomp/ttyd reference source and its MIT LICENSE were downloaded at
revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c` into that ignored directory.
Its declared baseline is Japanese G8MJ01 revision 0; selected findings were
checked against US original bytes before documenting US behavior. No upstream
implementation was copied into the native game. The existing external US symbol
CSV remains research metadata from GPL-3.0 ttyd-utils. Extracted original assets
remain subject to their original rights; neither reference license grants rights
to distribute the ISO, messages, executable or converted asset packs.

## Existing conversion pipeline

Additional 2026-10-12 behavior research: the pinned public source snapshot now
includes src/include/config and rels under ignored
`artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c/`.
`decomp_coverage.py` parses its configuration without executing it; report is
ignored `artifacts/reference/behavior/decomp-coverage.json`. The binary report
now fingerprints 359 selected US functions and includes two seven-byte defense
window tables. Independent verifier checks 1916 instruction anchors plus those
tables, the binary32 accuracy multiplier and all twelve counter jump-table
targets directly in the read-only ISO. Direct-damage research additionally
verifies nine presentation float constants and seven sound-key bindings.
Status storage research checks all81 getter/setter/recovery jump-table targets.
Infliction/presentation research fingerprints ten additional inspected case
tables,254 entries total, and checks five status effect position constants.
The application tail documents sound-key bindings, payload writes and completion
semantics; effect and announcement service implementations remain open.
Orchestration research adds five float and three sound bindings plus the
Pikkyoro zero candidate initializer; its guard/selection helpers are fingerprinted.
Turn recovery research checks the78-entry sequence-table hash,23 recovery
calls, two sound keys and Freeze scale. The report inventories direct branches
to the recovery helper across DOL text; indirect/REL/script callers are excluded.
Periodic damage research fingerprints Burn effect scheduling and shadow guard,
with four binary32 effect constants checked directly against the ISO.
Regeneration research adds HP recovery helper identity and the small original
regeneration wait-event encoding check; no event-script extracts are committed.
Recovery-event research fingerprints three complete script spans and13 additional
US callbacks/helpers. Script semantics are documented as prose; original bytecode
remains ignored. Local-work operands and owner fields are preserved separately.
Battle barrier/queue/conclusion research adds eight helper fingerprints and
local contracts, including reservation consumption order and group-result writes.
Inventory removal/reservation research adds three routine fingerprints and two
refund constants, preserving parallel-array compaction and deferred consumption.
Move-count research adds six function fingerprints and a direct DOL reset-call
inventory, excluding indirect/REL/script callers. Signed-byte narrowing and
status predicate boundaries are documented explicitly.
Phase actor/event table research adds eight function fingerprints, the15-entry
move dispatch-table hash and two position-sort constants. The complete local move-controller state map is now specified; action-controller
local state map is now specified as well, with its callee/script dependencies open.
Action-controller research additionally checks the15-entry dispatch-table hash
and camera-speed threshold. Retained Electric RNG draws and the two announcement
drain phases are documented without a dynamic-equivalence claim.
Default action event research adds two script fingerprints, seven helper
identities, mode8 interpolation target/constants and original zero-gravity
message-ID binding. No original dialogue or bytecode extract is committed.
Camera request research adds ten callback/setter fingerprints and the mode-reset
zoom constant, preserving independent priority gates and homing vector sources.
Announcement research adds five service/draw fingerprints, four floats and
window color. Duplicate local symbols use an explicit namespace qualifier in
the metadata tool; original dialogue/text buffers remain uncommitted.
Message lookup/loading/tokenizer research adds five fingerprints. Read-only
message_resource_evidence.py inventories260 US files and13,186 pairs, with
byte offsets/text hashes and two duplicate keys; metadata output stays ignored.
No dialogue extracts or encoding normalization are committed.
Parser research adds msgAnalize/msgInit fingerprints, two command/format block
hashes, layout constants and original key-wait pose bindings. Prose records local
display record construction; original parser code and message text stay ignored.
Embedded-object research adds msgIconStr2ID, all18 US name/ID bindings and
geometry constants. Census schema2 adds selected control argument shapes without
exporting dialogue or annotation text; code-generated/non-US input remains open.
Playback adds msgMain/keyGetButtonTrg fingerprints, all14 dispatch targets,
original scroll binary32 coefficient and seven paper pose/group bindings.
Timing contracts are derived from clock arithmetic, not measured gameplay.
Window lifecycle adds12 service/event fingerprints and default/small geometry
constants; original pool/heap failure behavior and caller identity remain open.
Selection adds seven input/window/event fingerprints, display-order constant
and original direct-input identity. Input repeat cadence remains unmeasured.
General/clear window states add two fingerprints, all12 dispatch targets,
24 sound table values and the original history-scroll coefficient. Fade and
countdown arithmetic are static contracts, not observed durations.
Common print research adds three fingerprints, projection threshold and ten
original special NPC names. Speaker/identity contracts do not prove story order.
Print variants add eight fingerprints, all18 original partner suffix bindings
and the field speaker literal. Shared key buffers remain original-runtime lifetime
dependencies; no generated dialogue or extracted resource text is committed.
Insert formatting adds two fingerprints. Census schema3 records per-pair and
aggregate printf-like lexemes (93 pairs/119 matches), explicitly not execution
or a format-call census. Original format/dialogue bytes remain ignored assets.
Format output/ABI research adds five runtime fingerprints and the original narrow
null-string sentinel. No original library implementation is copied into source.
Directive parser adds one namespace-qualified printf fingerprint (distinct from
scanf), both dispatch-table hashes and byte-classification hash/mask checks.
Integer conversion adds two fingerprints and both33-entry dispatch-table hashes;
the documented prefix/capacity stages are static evidence, not executed outputs.
Decimal rounding/preparation adds two fingerprints, the original special-value
string-block hash, sign-comparison zero and case-classification checks.
Binary-to-decimal generation is covered by the subsequent entry/helper research.
Finite layout research extends the existing float2str fingerprint's prose and
anchors to local scientific/fixed/general paths. Actual call ranges and executed
layout/capacity behavior still need recovery/comparison.
Binary decimal entry adds two fingerprints and its separate input-zero constant;
numeric/ASCII phases and generator request boundaries are documented. Runtime
floating environment and actual caller ranges remain explicit dependencies.
Numeric decimal multiplication adds one fingerprint and local convolution,
scratch, 36-digit retention and rounding contracts. Actual caller domains and
executed numeric behavior remain unverified; no original helper body is
incorporated into the native runtime.
Decimal power research adds one fingerprint, the73-entry dispatch identity and
184-byte seed-only block identity; the21 seed bindings are independently checked.
Original numeric strings/instructions remain in the ignored read-only research
outputs. Contracts record the original ASCII comparison and recursive rounding;
no original numeric service is compiled into the prototype.
Binary numeric services add five fingerprints and six binary64 constants.
Contracts retain raw special-value/signed-zero branches, scaling arithmetic and
word-conversion saturation; derived finite generator power bounds are not
executed-original observations. No upstream numeric implementation is copied.
Unsigned64 quotient/remainder add two fingerprints and local restoring-loop
contracts. An authored recurrence check and zero-divisor derivations remain
ignored metadata, explicitly separate from original instruction execution.
Hexadecimal float research adds one fingerprint; special strings reuse the
already verified original block. Documentation preserves raw exponent/fraction,
truncation and extended-precision stack-address calculations. No original
formatter body is compiled into the prototype or distributed as source.
Window manager/selection drawing add seven fingerprints, source path bindings
font/msgWindow.tpl and %s/f/msgWindow.tpl, and selection color/geometry/smoothing
constants. Public Japanese windowdrv.c supplies structural corroboration; US
static instructions establish the contracts. No palette/GX objects are extracted
or converted in this step, and no original renderer is compiled into the prototype.
Display queue/dispatch adds eight fingerprints (namespace-qualified comparator),
all eleven offset floats despite the symbol's undersized annotation, eleven GX
dispatch targets and five depth/common-state constants. Japanese source provides
structural corroboration; original US instructions/tables establish the contracts.
Sorting internals are covered below; outer rendering cadence remains open.
Sort implementation adds two fingerprints. Pinned Japanese system.c provides a
complete structural guide checked against original US partition/cycle branches;
no upstream implementation is compiled into the prototype. Authored model reports
for3549 sorting cases,5914 cycle permutations and8072 bit-loop counts remain
ignored and are not original execution evidence.
General window drawing adds three fingerprints, twelve type targets, fifteen
geometry/indicator floats, mode0 interpolation target and unsigned32 conversion
constants. Cursor pose pointer binding is recorded; original renderer/animation
services and visual assets are not copied into the native runtime in this step.
Draw record admission adds three fingerprints and reserved-control dispatch,
pose orientation/icon/glyph constants and emphasis color/vector bindings.
Only the admitted/control/embedded paths of msgDisp are locally documented in
that step; its normal glyph effect tail is covered below. Original
glyphs/pose/icon data remain reused assets outside source history.
Glyph effects extend the existing msgDisp fingerprint's recovered local coverage
with priority/RNG/clock/matrix/font requests and draw-time record relaxation.
Effect literals and all five scale-vector templates are independently verified;
numeric/trig/font helper implementations and executed images remain open.
Font state adds fourteen fingerprints, original three-color palette/default-scale
and noise texture-matrix constants. Original window palette object50 is a request
binding; no descriptor/image conversion or replacement noise asset is generated.
Byte-alpha arithmetic checks are authored derivations, not original execution.
Font glyph/setup requests add three fingerprints and original texture-matrix
mode constants. Static font texture object0x8041e97c is a request binding;
its glyph/page/image ownership is still unrecovered. No font assets or original
raster implementation are added to native code/source history in this step.
Inner glyph quads add two fingerprints and original HSV/bias/two-unit constants.
Font block/page assets remain unextracted in this step; local FIFO/quad requests
and mandatory two-draw RNG behavior are documented independently of measured
GPU images. No original raster implementation is compiled into the prototype.
Hit-precheck and damage-calculator research adds prose contracts and metadata;
no executable conversion or third-party implementation is incorporated.
No full original routines, source extracts,
dialogue or assets were added to history. The detailed behavior documents explain
the scope and do not claim the original binary has been executed or recompiled.

The user's latest preference supersedes the initial hybrid-placeholder plan:
use assets very similar to the original game, preferably original geometry and
textures; scale textures/reduce vertices/adjust lighting for performance.
Preserve original sounds and music. Do not reintroduce the earlier vector redraws
or synthesized sound effects.

`tools/iso_assets.py` reads the FST from the legally acquired `G8ME01` ISO. It
validates bounds and filenames, inventories all files and extracts exact selected
paths. Extracted files receive SHA-256 provenance sidecars. The source ISO is
opened read-only. Original/converted content belongs under ignored local-assets,
romfs or artifacts directories, never source history.

`make` invokes `tools/build_assets.py`. Set TTYD_ISO to change the input path.
Selected original asset sources:

- `a/a_mario-`: complete Mario pose texture 0.
- `a/c_pkr` and `a/c_pkr-`: Goombella, animation PKR_Z_1 at frame 0.
- `a/c_kanbu` and `a/c_kanbu-`: Crump, animation Z_1 at frame 0.
- `a/c_kinopio` and `a/c_kinopio-`: Toad, animation KNP_Z_1 at frame 0.
- `m/gor_01/d` and `m/gor_01/t`: original plaza mesh, scene graph and textures.
- `b/gor_01.tpl`: room backdrop.
- `m/stg_01/d` and `m/stg_01/t`: original common battle stage.
- `sound/stream/stg_gor1_32k.stm`, `btl_zak1_32k.stm`, `btl_win1_32k.stm`:
  original Rogueport, normal-battle and victory music. Confirm encounter-specific
  track choice in Dolphin; do not assume normal battle music is the tutorial's.
- `sound/proj/pmario.{sdir,samp,pool,proj,etbl,slib}`: preserved original sound bank.

GX decoding covers I4/I8/IA4/IA8/RGB565/RGB5A3/RGBA8, indexed palettes and CMPR.
Only the base mip level is used. Map texture dimensions are capped at 128 pixels
and normalized to powers of two for native repeat sampling. Original character
poses are rasterized from source geometry to 128-pixel billboards. This is an
intermediate optimization; proper original animations and direction variants
must replace the idle-only display. No animation timing is inferred from the
atlas.

The map converter flattens original S subtree transforms and triangle topology
at scale 0.1, preserves texture coordinates, material colors, wrap modes and
alpha-layer grouping. The current plaza has 40,714 triangles and 79 batches; the stage has 10,801
triangles and 14 batches before static visibility overrides. The mask, closed
upper curtain and inactive switch props are suppressed for this playable view;
verify these inferred visibility rules against the original battle scene. Exact
current counts are in romfs/manifest.json. Near-ground original collision triangles are reduced
to 711 planar wall segments. The walkable subset still needs original floor,
slope, trigger and visibility semantics.
Geometry decimation, dynamic visibility and original map animations are pending.
Materials with multiple samplers are explicitly reported rather than assumed
correct. The selected room currently has none.

STM conversion preserves rate/channels and original loop points, clamped to
valid source sample bounds. Runtime streams PCM; it does not load the entire
music track into RAM. MusyX directory indexing exposes 1,247 original samples;
macro-to-game-event resolution is still needed before faithful effects play.
Unresolved effects remain silent, not replaced with generated tones.

Format research used noclip.website (MIT) and vgmstream (ISC); corresponding
license notices are in third_party. Nisto/musyx-extract was read for format facts;
its unlicensed source is not incorporated. No Paper Mario decompilation code is
currently incorporated. Record licenses before adding any external code.

Ground surfaces are exported from the original collision subtree into
map.ground (little-endian uint32 count, then nine float32 coordinates per
triangle). Slopes with absolute normalized Y normal >= .65 are retained. This
is a provisional traversal filter, not an observed original engine threshold.
Original rel/gor.rel is extracted read-only with provenance for exit/event
analysis; no original executable code is run by the remake.

Original overworld packs now include m/gor_00, m/gor_02 and m/gor_03.
rooms.bin contains a uint32 link count followed by records (source room uint32,
target room uint32, exit X/Z, spawn X/Z/Y float32), 28 bytes each. Road IDs
0/1/2/3 map to gor_01/gor_00/gor_02/gor_03. The manifest records original REL
offsets, marker-derived coordinates and target/arrival identifiers. Wall record
count high bit now indicates min/max Y following each XZ segment (six floats).
Unknown story-dependent exits are not activated by this road-only parser.

Wall deduplication includes the original height interval in addition to X/Z,
so stacked surfaces do not overwrite one another. Floor triangles also supply
per-room footprint bounds for touch-map scaling; runtime uses a spatial index
without modifying original geometry.

Map batches now reserve bits 8–23 of their layer word for an exterior-wall
cutaway group ID (0 means ordinary geometry). Original layer remains in the
low byte. Group owners are listed as cutaway_nodes in the manifest; S_gai_kabe
subtrees provide the classification, and drawable descendants inherit an
already selected owner. Geometry positions, UVs, colors and textures remain
original. Group-based batching increases draw calls without duplicating triangles.
This classification is a rendering adaptation, not recovered original visibility
event rules. The upstream viewer's runtime flag handling was inspected at
https://github.com/magcius/noclip.website/blob/master/src/PaperMarioTTYD/render.ts
for further visibility/event work; no code from that file was copied here.

Rogueport station (`m/gor_04/d`, `m/gor_04/t`) now converts through the same
original-map pipeline: 41,398 triangles, 50 batches, textures capped at 128.
No exterior-wall classification matches this room (zero cutaway groups).
Original `rel/gor.rel` records at 0x4de80 (west n_bero) and 0x5d53c
(station s_bero) establish the reciprocal road. Station's other geometric
n_bero marker is not an additional inferred road; train/event travel is unfinished.
Arrival elevation uses the marker base, not its volume midpoint.

Original per-node culling is read from the draw-mode pointer at node+0x58,
byte+1. Converter retains GX indices 0–3, rejects invalid drawable-node values,
and records cull_triangles counts. Hierarchy-only nodes can have a null pointer.
Format reference: noclip.website src/PaperMarioTTYD/world.ts (MIT), already
used by this converter. No visual guesswork is used to classify mesh sidedness.
Batches now split by sidedness as well as material/cutaway owner. Geometry
counts are unchanged; plaza/harbor/east/west/station batches 116/79/111/127/54.
This does not recover event-controlled visibility, train animation or multilayer
materials. Those remain necessary for original presentation.

Map mesh animation tracks are now preserved in ignored local conversion sidecars
`local-assets/<room>/map.animations.json` (plaza uses `map`, battle `stage`).
The original .d source remains authoritative, including the 12 unknown components
per mesh keyframe not interpreted here. Map summary/manifest lists names, duration,
mesh-track counts and material-table offsets. Mesh sampling uses original
translation/rotation offsets, scale divisors, step flags and Hermite tangents.
Format reference: existing MIT noclip.website/world.ts. Event activation and
loop modes are not inferred. Material animation tracks are not decoded yet.
Station has shadanki, n_1_door_open/close mesh tracks; train-body mesh animation
is absent from that table. Its visual defects still require material/scene/event
inspection. Harbor contains duplicate fune_b animation names; explicit name
selection rejects ambiguity rather than choosing an arbitrary duplicate.

Material animation tracks are now decoded into local map.animations.json: five
channels (S/T translation, S/T scale, rotation), centers, texgen index, key times,
Hermite tangents and step flags. Offline preview binds texgen0 by original
material names (including _v/_x-normalized aliases). Higher texgens are reported
as unsupported, not silently accepted. Shared texture_matrix supplies both bind
and animated texture transforms; no runtime animation is enabled automatically.

Downloaded read-only public US symbol metadata for event inspection from
https://github.com/jdaster64/ttyd-utils/blob/master/resources/us_symbols.csv
to ignored artifacts/reference/us_symbols.csv. Upstream repository is GPL-3.0;
no upstream code or table is copied into native runtime or source history.
Independently written map_event_inventory.py resolves original REL local/DOL
relocations, uses externally supplied symbol names/ranges, and inventories the
original init command stream to ignored local-assets/map-init-events.json.
It does not execute scripts or evaluate story conditions.

Station road-entry conversion now recursively suppresses gondora_2 and hako_1.
Original gor_04_init_evt has IF_STR_EQUAL(entry, hikousen_return) at0x604d0,
ELSE at0x604e4 and END_IF at0x60518; ELSE sets group hidden bit1 at0x604e8
and0x60500. The currently supported station entry is s_bero, so ELSE applies.
Both groups are hierarchy-only owners; their314 child triangles are omitted,
leaving41,084 triangles/54 batches. Original collision is unchanged.

inspect_dol_function.py creates inspection-only ELF files from named functions
in the read-only US ISO, using external US symbol ranges and llvm-objdump.
No original executable code is run or linked into the native game. Original
function bytes/disassemblies stay ignored in artifacts/reference. Primary
reference evt_cmd.h was read from jdaster64/ttyd-tot for string conditional
opcodes; doldecomp/ttyd mapdrv sources inspected but do not implement these
flag functions. Semantics below derive from the user's original DOL bytes.

East gor_02 now packages mizu/noname/ie_01_light texture-zero ambient tracks
identified at original init offsets0x4d694/0x4d6a8/0x4d6bc, outside enclosing
conditional blocks. The source starts each with mode1/clock0. Actual US DOL
mapPlayAnimationLv0x80029de4 sets loop flag0x4 for mode1 and speed1; mapMain
0x80026bb8 wraps against the full authored duration (not duration-1). It
calculates frames from elapsed milliseconds times gp->framerate/1000. US
marioStInit 0x8006fb3c (symbol range; see local disassembly) writes60 to offset4
for VIGetTvFormat()==0 and50 otherwise; NTSC US input selects60. Primary
field reference: doldecomp/ttyd/include/mario/mariost.h (framerate at0x4).
No original executable implementation is copied into the port.

UVAN tables bake original Hermite/step curves at integer60Hz frames. Binding UVs
come from inverse original bind texture matrices, preserving existing vertex
layout and material batching. gor_02.uvanim is41,856 bytes, updates369 vertices
(135/144/90) in tracks with durations999/159/400. All other room/stage files
contain valid zero-track headers. Manifest records clip IDs and bound counts.
East water texgen1 is still missing; do not claim complete water reproduction.

Original ambient mesh packs now include plaza m/gor_01/d hae (duration399,
2 owner tracks,12 vertices,38,524 bytes) and harbor m/gor_00/d nami
(duration979,3 owner tracks,4,581 vertices,214,308 bytes). Harbor UVAN enables
nami/nami_02/hiki_nami (4 bound tracks,5,451 vertices,143,144 bytes). Original
init playanim calls hae0x3c798 and harbor0x2f31c/0x2f330/0x2f344 are outside
conditional blocks; loop/clock/NTSC60 semantics follow the prior DOL inspection.
All rooms and stage package mandatory MSAN headers; unused rooms have zero
tracks. Original files remain read-only and generated packs remain ignored.

Harbor nami references fune_b_lambert83, absent from this map material table;
that channel has no visible geometry binding and is recorded in limitations.
Original two-sampler harbor materials still render texture zero only. Neither
missing channels nor conditional boat animations are claimed faithful.
Mesh verifier compares five native baked poses against explicit offline original
clip samples for both rooms, including ancestor transforms, with unchanged
collision/batch files. It does not prove original event or camera behavior.

Floor packs now preserve original drawmode+8 attributes and map-local source-node
IDs. Independent verify_ground_attributes.py checks all3,576 floor records in
the five original room files directly against raw extracted node bytes. Counts
are plaza1010/harbor463/east878/west1037/station188. Harbor A_mizu contributes
16 floors carrying0x200. Source-node names/attributes are indexed in room
summaries and runtime manifest; unknown bits are retained unchanged. Geometry
and normal filtering remain unchanged; tagged records add8 bytes per triangle.

Original US DOL _hitEnt0x8001896c:80018aa8 loads joint+0x58,80018ab4 loads
drawmode+8,80018ab8 stores hit-entry+4 (null drawmode stores0). hitGetAttr
0x80014310 reads that word or returns0 for null. These original read-only byte
inspections substantiate raw static attribute mapping; implementation is native
and does not link/copy original executable functions. Inspection artifacts stay
ignored. Effective event mutations remain separate unfinished work.

room_convert.damage_returns now reads original US evt_hit_damage_return_set
imports (8007458c), validates two-argument USER_FUNC event headers, follows
self-REL relocations and reads bounded16-byte name-pointer/XYZ tables. It rejects
missing relocations, nonfinite values and malformed termination. Source call and
table/entry offsets, ordering, names, coordinates and Y=-1000 auto-position
markers are preserved. No external symbol CSV is required to build these tables.

Build produces ignored local-assets/damage-return-tables.json and records both
tables in ignored RomFS manifest provenance. These are metadata for recovery
implementation; runtime does not yet consume them. Verify with
`python3 tools/verify_damage_returns.py`. Original function-address/layout evidence
is retained in mechanics notes; no original executable implementation imported.

Resolved original init return packs: gor_00 has5 node bindings (92 bytes),
gor_02 has16 inherited bindings (268 bytes); other room files are12-byte empty
RPOS headers. Original tables still retain raw positions/sentinels and call
offsets; manifest additionally records resolved positions and descendant IDs.
Harbor automatic A_yuka center is approximately(.85,.15,2.2). Literal positions
now correctly use engine-to-native0.01, substantiated by original _mapLoad's10x
model matrix. All original collision owners/parents/centers are inventoried,
not just mesh owners. Source floor geometry/raw attributes remain unchanged.

Original wall collision attributes are now preserved alongside geometry in
32-byte records (six floats,two uint32 words); tagged count uses0xc0000000.
Source-owner IDs and flags come from original node/drawmode records, with no
attribute edits. `python3 tools/verify_wall_attributes.py` independently compares
all5,264 records against source bytes across gor_01/00/02/03/04. No bit31
special-case owners occur in these packs. Collision mesh extraction remains
approximate planar wall segmentation; original flags now control solidity.

Wall packs now additionally retain all9 triangle coordinates, unrounded, after
the geometry/attribute prefix:68 bytes per record, count tag0xe0000000.
Distinct triangles belonging to the same owner are no longer collapsed merely
because their projected segment and min/max heights match. Five rooms export
7,830 retained triangles;92 records are non-solid under normal-Mario flags.
`verify_wall_attributes.py` validates the new layout and original owner flags.
The existing wall selection threshold(abs(normalY)/length<=0.2) is unchanged;
coverage of other angled surfaces and dynamic hit meshes remains unfinished.

## Complete static head collision mesh

Offline conversion emits .head for map/gor_00/gor_02/gor_03/gor_04 from original
m/<room>/d. Format HEAD magic+uint32count; each48-byte little-endian record is
9float32 vertices, uint32attributes, uint32sourceNode(relative to map base32),
uint32drawmodeByte1. Source hitCalcVtxPosition strip winding is preserved
separately from render triangles; every collision strip triple is retained,
including steep/vertical surfaces and degenerates. Conversion does not filter
by floor slope. Same original-ISO provenance, no replacement artwork/code.
Packs stay ignored in RomFS/local-assets; no ISO/runtime original extraction.
verify_head_mesh.py validates every record owner/attribute/cull against original
read-only node bytes; it does not independently validate converted positions.
Room triangle counts3153/1604/3450/3504/548 (12,259 total). Largest geometry
vector168,192 bytes plus spatial-index overhead; only current room is active
except temporary staging/rollback. Hardware memory/performance unverified.

## Floor slope selection moved to runtime

Floor converter keeps all nondegenerate triangles with nonzero XZ projection
(|normalY|/length>1e-6), replacing.65 cutoff. Native nearest-hit query then
rejects ordinary slopes>=50degrees. This preserves valid near-limit slopes
and steep nearest blockers. Pack remains tagged44-byte attributes/source-node
format; source cull/winding metadata is not added yet. New floor counts
1337/792/1285/1332/200 (4,946 total), verified original attributes/source IDs.
Ground vector payload217,624 bytes across all packs, plus runtime struct/index
overhead; room-specific loading still applies. No art/music conversion change.
Original-derived packs remain ignored; hardware performance unverified.

## Source-facing floor pack (2026-10-12)

map/gor_00/gor_02/gor_03/gor_04.ground now use header bits31+30 followed by
48-byte little-endian records:9float32 vertices, uint32attributes, uint32source
node relative to map base32, uint32drawmode byte1. Geometry comes from the
same source hit_topology strip order as the complete head mesh, not GX render
triangles. Preserve all nondegenerate projected surfaces; source slope/cull
selection happens at runtime. Legacy untagged36-byte and bit31-tagged44-byte
packs remain readable as two-sided, with their prior winding unchanged.

Source is locally converted m/<room>/d from read-only US G8ME01 ISO; no external
code/art/license dependency introduced. ISO/originals/packs remain ignored and
are not distributed or required at runtime. Node attributes and drawmode are
checked against original raw node bytes by verify_ground_attributes.py; source
strip order follows hitCalcVtxPosition evidence. This verifier does not prove
independent original-coordinate fidelity. Counts remain4,946 records,237,408
bytes total record payload;1,867 are one-sided downward backfaces now rejected
by floor queries. Only current room is active except transactional staging.
Hardware memory/performance unverified.

## Switch 1.0.0 static research (2026-10-12)

The user selected the Switch remake as the mechanics reference and subsequently
restricted work to static analysis. Original GameCube conversions and their
licensing/provenance above remain separate. No Switch asset was converted into
or linked into the native 3DS runtime by this research.

Read-only source:
`/home/carson/games/roms/Paper Mario The Thousand-Year Door [BASE][0100ECD018EBE000][US][v0].nsp`.
SHA256 `04fba23ab27de88276d2a04eaf69619cd09044acb41de62c1cb2f89a8de110e4`;
CNMT application0100ecd018ebe000/version0, NACP1.0.0. Detailed member identity,
content checks and reproduction are in [Switch baseline](switch/BINARY_BASELINE.md).

Local extraction paths (all ignored):

- `local-assets/switch/nca/`: original PFS0 members, including private ticket.
- `local-assets/switch/exefs/`, `romfs/`, `meta/`, `control/`, `other/`:
  decrypted archive sections; no original input overwritten.
- `local-assets/switch/decoded/`: selected Zstandard-expanded data ELFs and
  uncompressed main NSO; `main.elf` is an inspection-only segment wrapper.
- `artifacts/reference/switch/`: hashes, table/relocation reports, integrity logs,
  disassembly and an isolated private key mapping. Keys are never published.
- `artifacts/reference/hactool/` and `Unsimplifier/`: pinned public source tools;
  exact revisions/licenses in the baseline document. No public editor UI or
  arbitrary mod was used to mutate the reference game.

Original game code, text, models, music and other assets remain the property of
their rights holders; possession and extraction do not grant a redistribution
license. Do not commit/distribute the NSP, extracted originals, keys, firmware
or complete story text. Research commits contain prose, independently authored
inspection tools and tests, not game payloads. hactool is ISC; Unsimplifier is
Unlicense. Public layout labels are distinguished from verified native semantics.

The user's local prod.keys was used for read-only decryption. The supplied
 title.keys lacked the game's rights entry; the archive's own common ticket
provided an isolated mapping. Original key files were not altered. Before the
static-only instruction, isolated Ryujinx files/captures were written under
`artifacts/ryujinx/`; no gameplay was reached and the normal user profile/saves
were not changed. Firmware.23.0.1.zip was supplied, but successful installation
was not established. No firmware is required for continued static recovery.

HSV conversion and its floor dependency add two US fingerprints and24 instruction
anchors. Read-only checks cover255/360/60 binary32 normalization constants and
floor's binary64 1e300/zero probe. Authored contracts describe byte order,
single-precision/fused operation boundaries and exponent/mantissa branches;
no original executable is linked, distributed or executed. Local disassemblies
and reports remain ignored; no asset conversion or licensing change occurs.

Font setup/mapping research adds four US function fingerprints and24 anchors,
seven original basename pointers and five name/path bindings. Read-only complete
BFN hash checks cover f/papermarioset_US.bfn(66560 bytes),
f/papermarioset_EU.bfn(66560) and f/papermarioset_JPN.bfn(264896). Local descriptor
metadata is ignored artifacts/reference/behavior/font-data.json; no font payload
was exported or converted by this step. Authored contracts record source paths,
block/texture dimensions and setup's two ordered width mutations, including the
US/EU fallback alias. No asset redistribution, license change or executable
linking occurs; original fonts remain subject to original rights.

Message-width/icon-dimension research adds four US function fingerprints and24
anchors. Checks pin all seven US character-spacing words (the pinned Japanese
source declares a shorter table), three icon name/scanf bindings and twelve
halfword remaps. Authored contracts cover byte scanning, wrapping/max width,
LF suppression/count, fused icon arithmetic and directory/TPL dimension lookup.
No original code or icon payload is exported/linked, no conversion or asset
license change occurs; inspection ELFs/disassemblies/reports remain ignored.

Original scan-runtime research adds three US function fingerprints and28 anchors
for sscanf, __StringRead and __sformatter. Read-only checks hash the complete
256-byte classification table and check its six whitespace codes. Authored
contracts cover ABI, EOF/pushback, counters, output storage and string loops;
scanf grammar/numeric/multibyte internals are explicitly open. No original code
is executed, compiled or exported into source history; local ELF/disassembly/
metadata reports remain ignored. No message/asset conversion or license change.

Scanf-specific parse_format adds one namespace-qualified US fingerprint and24
anchors. Read-only checks pin its40-byte initializer, all52 dispatch targets via
a complete table hash and ten width-digit classification codes. Authored grammar
covers suppression, wrapped widths, storage selectors and acceptance bitmaps;
no original parser implementation/format payload is copied into source. Its ELF,
filtered symbol CSV and disassembly stay ignored. No asset conversion, original
execution or license change occurs.

Unsigned32 scan research adds __strtoul identity and20 anchors. Read-only checks
hash all17 state targets and the complete256-byte uppercase table. Authored
contracts record signs/base inference, lookahead/width/count, sticky overflow
with wrapped magnitude and unconditional unread. No public strtoul or executed
conversion claim is implied. Original disassembly/reports remain ignored; no
asset conversion, original executable linking or license change occurs.

Unsigned64 scan research adds __strtoull identity and24 anchors, with a full
hash of its separate17-entry state table. Authored contracts record division
threshold, word products/carries, overflow/count/sign outputs and invalid-entry
uninitialized-register unread. Local ELF/disassembly/report outputs stay ignored;
no original execution, asset conversion, native linking or license change occurs.

Multibyte runtime research adds mbtowc and is_utf8_complete fingerprints and24
anchors. Authored contracts record permissive continuation-bit checks, exact
halfword masks, incomplete/error routing and size1 scan-consumer composition.
No message bytes are reinterpreted/rewritten, original code linked or asset
converted; original ELF/disassembly/report artifacts remain ignored. Licensing
is unchanged and static checks do not establish executed decoder equivalence.

Floating scan entry/special-value research adds the containing __strtold
fingerprint and24 selected anchors,42-byte zero initializer, two token bindings
and two special binary32 words. Its finite states/conversion tail remain open;
whole-body identity is not complete behavioral recovery. Authored contracts
record callback/width/sign/count and no-unread special exits. No original code
linking/execution, data conversion, message rewrite or license change occurs;
local disassemblies/reports remain ignored.

Decimal floating scan adds24 anchors within the already-fingerprinted __strtold,
initial decimal-point pointer/byte and four binary64 range constants. Authored
contracts cover retention/scale/exponent/count/handoff and local range/sign exits;
hex states and __dec2num remain open. Original disassembly/reports stay ignored;
no executable linking/execution,asset conversion,message rewrite or license change.

Hexadecimal floating scan adds32 anchors within the existing __strtold identity
and checks all22 hex digit classification codes. Authored contracts cover nibble
packing,exponent/count/normalization and direct binary64/range/sign construction,
including static uninitialized/nontermination edge deductions. No original code
execution/linking,asset conversion or message rewrite occurs; local disassembly/
reports remain ignored. Numeric execution equivalence and licensing are unchanged.

Decimal-to-binary entry research adds containing __dec2num identity and24 anchors,
three binary64 sign/zero constants and complete lowercase-table hash. Entry/
special-record contracts are authored;finite approximation/refinement remains
open despite whole-body identity. No original code execution/linking,asset
conversion or message rewrite occurs;local ELF/disassembly/reports stay ignored.
Original rights/licensing remain unchanged.

Finite decimal preparation/approximation adds20 anchors within existing
__dec2num identity,eight binary64 chunk powers and three base/bias constants.
Authored contracts distinguish private record mutation and approximate candidate
from still-open refinement/final rounding. No original code execution/linking,
asset conversion or message rewrite;ignored inspection artifacts and original
rights remain unchanged.

Decimal infinity-candidate research adds24 anchors,complete36-digit boundary seed
hash/NUL and maximum-finite fallback bits. Authored contracts cover local record/
compare/retention/sign flow;general refinement/ties remain open. Original seed
payload is retained in the read-only ISO;no native linking,original execution,
asset conversion,message rewrite or license change occurs. Inspection reports
and disassembly remain ignored.

Decimal equality/refinement admission adds __equals_dec identity and24 anchors
for complete equality,initial converter comparison and first upward neighbor.
Authored contracts distinguish sign-ignored/trailing-zero equality and raw-word
neighbor from still-open later search/ties. No original routine execution/linking,
asset conversion,message rewrite or license change;inspection artifacts remain
ignored and whole-body identity is not complete conversion equivalence.

Decimal neighbor/refinement research adds32 anchors within existing __dec2num
identity. Authored contracts cover both search loops,four subtraction requests,
record equality/parity ties and asymmetric selection. Local containing-body
recovery is distinguished from open __minus_dec/pow and execution equivalence.
No original code execution/linking,asset conversion,message rewrite or license
change;ignored inspection artifacts remain local.

Decimal subtraction adds __minus_dec fingerprint and24 anchors. Authored
contracts record complete copy/alignment/borrow/correction/normalization flow,
including pointer-dependent tail correction and unchecked zero/alias edges.
No exact-distance/execution equivalence is inferred. Original disassembly/report
artifacts stay ignored;no native linking,asset conversion,message rewrite or
license change occurs.

Power entry/special research adds pow and containing __ieee754_pow fingerprints,
24 selected anchors and three special doubles. General finite/large-exponent
core remains open;identity is not full behavior coverage. Authored contracts
record parity/shortcut/errno order and decimal base-five caller domain. Original
ELF/disassembly/reports stay ignored;no original execution/linking,asset conversion,
message rewrite or licensing change occurs.

Power large-exponent research adds20 anchors within existing __ieee754_pow
identity and a full48-byte/six-coefficient hash. Authored contracts retain raw
high-word thresholds,original signed-base reload,fused operations and low-word
split;general logarithm/common tail remain open. No original execution/linking,
asset conversion,message rewrite or licensing change;local inspection reports/
disassembly remain ignored.

Power general-log research adds24 anchors within existing helper identity,
complete reference/correction and scale/coefficient region hashes plus signed
conversion bias. Authored arithmetic preserves fused boundaries and integer
low-word splits. Common exponentiation/range tail remains open;no accuracy or
execution equivalence is inferred. Original inspection artifacts stay ignored;
no executable linking,asset conversion,message rewrite or license change.

Power common-tail research adds32 anchors within existing helper identity and
full104-byte/thirteen-constant hash. Authored contracts cover complete local
product/range/reduction/polynomial/scaling/sign sequence;helper and composed
numerical execution equivalence remain open. No original code linking/execution,
asset conversion,message rewrite or license change;local artifacts stay ignored.

Square-root research adds sqrt/__ieee754_sqrt identities and24 anchors plus
remainder-path one constant. Authored contracts record complete local special/
errno,integer normalization/extraction/correction and output bit flow,without
executed numerical/FPSCR equivalence. Original ELF/disassembly/reports stay
ignored;no executable linking,asset conversion,message rewrite or license change.

String-pitch drawing adds three US fingerprints and24 anchors,two zero-vector
wrapper templates and half/default pitch constants. Authored contracts preserve
byte/tag versus LF behavior,matrix requests,pitch/advance and outline/raw-color
restoration with known inner RNG counts. No original execution/linking,asset
conversion,message rewrite or license change;inspection artifacts remain ignored.

Affine matrix research adds three US helper identities and24 instruction anchors,
with positive-zero and Unit01 constant words checked read-only. Paired-single
instruction semantics were cross-checked against Dolphin's public GPL-2.0-or-later
interpreter source;no interpreter implementation was copied or linked. The
behavior document cites that reference and distinguishes original words from
emulator interpretation. Original ELFs/disassembly/reports remain ignored.

US completion/usage audit reads original DOL text/data and all29 shipped REL
module0 import streams read-only. Ignored us-usage.json contains annotations,
counts,relocation locations and20 command registration entries,not executable
or dialogue bytes. Its addresses/names derive from the existing GPL-3.0 symbol
metadata;no upstream implementation or original code is linked or distributed.
No asset conversion or license change. Static references are not execution proof.

Gekko disassembly correction (2026-10-12):independent tools/gekko_disasm.py
repairs LLVM paired-single mislabels and is integrated into function inspection.
354 instruction rows in47 ignored original listings repaired;386 inspection
ELFs unchanged. No ISO write,asset conversion,original execution or story edit.
Encoding facts consulted GNU opcodes and Dolphin primary references;links and
upstream licenses are in GEKKO_DISASSEMBLY.md. No upstream implementation copied
or linked;original text/ELFs and metadata repair report remain ignored.

Ordinary attack research adds six function identities,32 anchors,seven TimingA
threshold/window tables and pointer list,registration/two floats,and independent
Jump/Hammer-core event hashes/four setup records. dol_event_inventory.py reads
US ISO DOL data read-only;ignored reports retain raw operands/annotations,not
dialogue contents. No original code linked/distributed,no asset conversion or
story rewrite;existing original rights and reference licensing remain unchanged.

Hammer ID3 research adds eleven function identities and48 anchors,seven release
allowances,registration,two display floats/two sound keys and eight parameter/two
flag records in previously hashed US Hammer event data. No new extraction or
asset conversion,ISO write,game execution,native linking or story rewrite.
Original reports/disassembly remain ignored;existing licensing is unchanged.

Hammer weapon-branch research adds six function identities and40 anchors,used
VM targets/condition/case operands,three weapon difficulty/event references and
two wrapper streams. Existing read-only DOL inventory outputs retain raw data
only locally;no original code/dialogue distributed or linked,no asset conversion.
Interpretation remains static;public names are annotations and licensing unchanged.

Harbor story binding research reads rel/gor.rel directly from the read-only ISO
through rel_event_inventory.py,without writing an extracted module. Local reports
retain raw commands,self/module0 ADDR32 bindings,foreign/BSS identities and
resource identifiers;no loaded VA inference or event execution. Seven span hashes
and original msg/US/gor_00.txt identity checked;fresh message census preserves
text/token payload hashes without exporting dialogue. Only IDs/offsets/actor
bindings/authored behavior notes committed. No asset conversion,original code
linking/distribution or licensing change;symbol annotations retain existing
reference provenance. Full original story/transcript/conditions still incomplete.

Field/NPC battle research adds ten function identities and48 anchors;seven
harbor configuration/prolog spans and actual setup/group/unit relocation chain
plus zero DOL item table are checked. No new extraction/asset conversion,ISO
write,emulator execution,native linking or story edit. Registration pointers and
unit stat interpretations remain bounded by documented consumers;original files
retain existing rights and all inspection artifacts stay ignored.

Harbor boss tutorial research adds five DOL state-service identities/24 anchors,
seven whole event spans and two60-byte REL feedback helper hashes/relocations,
plus exact six US message-key uniqueness and enemy defense/power bindings. REL
inspection ELFs/listings are ignored and use file offsets explicitly;raw branch
targets are unrelocated,annotated with original relocation destinations. No
original executable linking/distribution,ISO write,asset conversion or story
rewrite;existing original/reference provenance and rights remain unchanged.

Feedback writer/stage-gate research adds four identities/48 anchors,original
quarter/half/signed bias and four ordinary attack declaration records. Connects
REL tutorial readers to DOL declaration counts and one proven battle flag
consumer. No original execution,ISO/profile write,asset conversion,executable
linking/distribution or story rewrite;all original artifacts remain ignored.

Unit entry/stat/spawn research adds five containing-body identities and32 anchors,
original boss HP5/FP0 data and tutorial getter/DIV/helper-spawn bindings. Large
entry body coverage is explicitly partial despite whole-body fingerprint. No
new extraction/asset conversion,ISO write,original execution/native linking or
dialogue rewrite;artifacts ignored and provenance/rights unchanged.

Additional target/attack research (2026-10-12): seven original DOL wrapper/helper
fingerprints establish first-attack pointer pairing,body-part ID lookup,signed
side inversion and sampling interfaces. Forty selected instruction anchors
include their local contracts and relevant GOTO/CASE_OR/CASE_END regions;the
latter regions do not constitute complete new VM helper coverage. Direct harbor
REL import/result/label checks pin the normal-attack request graph and absence
of label99/END_SWITCH in its95-command span. Fresh report remains ignored at
artifacts/reference/behavior/evidence.json;fresh event inventory was generated
read-only into /tmp/pm-boss-normal-attack.json. Original executable bytes/poses/
sound resources were not converted,linked,committed or distributed. No original
execution or new licensing rights are inferred from these contracts.

Battle jump research by agent ID1 (2026-10-12) uses a separate read-only verifier,
tools/verify_battle_jump_contracts.py,while general research tooling is shared
with agent ID2. Its ignored battle-jump-evidence.json records ten body identities,
55 instructions,nine initialized constants and concrete harbor jump imports/
operands. Nine local bodies are documented completely;evtGetFloat is documented
only for used literal routes. Inspection ELF/disassembly artifacts remain under
artifacts/reference/behavior and use corrected Gekko paired-single mnemonics.
No original executable linking/distribution,asset conversion,ISO/profile/save
mutation or story/dialogue rewrite. Encoding/behavior facts do not grant rights
to distribute original resources;existing provenance and rights still apply.

Physical input/cadence research by agent ID1 (2026-10-12) documents six new full
local bodies and checked ordering/snapshot regions in prior helpers. Original
DEMOPad is BSS;no live input state was extracted or inferred from initialized
file bytes. tools/verify_input_cadence_contracts.py pins ten containing bodies
(including four prior identities),64 instructions,28 direct calls,reset-mask data,
AC display depth and the event-credit unit. Report remains ignored at
artifacts/reference/behavior/input-cadence-evidence.json;listing/inspection ELFs
remain in the same ignored research directory. SDK hardware and actual input
poll/impact timing remain dependencies. No asset conversion,original execution,
ISO/profile/save mutation,native linking or original text rewrite. Existing
original/reference provenance and distribution rights are unchanged.

Input ownership/control research by ID1 (2026-10-12):input_ownership_audit.py
reuses the read-only shipped-reference census and adds containing-owner labels,
six small DOL body checks,30 selected instructions,two data checks and the gor
_config_key_record helper hash/HA/LO/REL24 bindings. One162-command lecture event
hash and its selected injection cases/defense imports are verified;full event
semantics remain open. Ignored report is artifacts/reference/behavior/
input-ownership-audit.json;temporary unrelocated REL inspection ELFs/listings and
lecture inventory are in /tmp. REL file offsets,not provisional loaded addresses,
identify these bodies. No original executable linking/distribution,dialogue
rewrite,asset conversion,ISO/profile/save mutation or original execution. Existing
rights/provenance remain unchanged;metadata/reference licenses do not grant
original-resource distribution rights.

Helper-role/coordinator research by ID1 (2026-10-12):the read-only verifier
verify_lecture_helper_roles.py pins eight full data records,three relocated REL
leaf bodies,ten event spans/counts,23 local/ten service bindings and concrete
work/selector writes. Distinct NPC/lecture kinds are preserved even when visual
assets are reused. Twenty coordinator message/select/append sites bind16 exact
US keys in msg/US/gor_02.txt (128,972 bytes,SHA-256
1958e3d22bb27b01332e08b67f279a33f21e0e8a3aba2b947ad8c2a446144f16).
Payload hashes/speaker operands are metadata;original text/control tokens remain
in the read-only ISO/local assets and were not paraphrased or committed. Ignored
reports are christine-helper-events.json,frankli-lecture-coordinator.json,
lecture-counter-event.json and lecture-helper-roles-evidence.json under
artifacts/reference/behavior. Temporary REL inspection ELFs/listings remain in
/tmp;unrelocated targets are distinguished from original import bindings. No
original execution,ISO/profile/save mutation,asset conversion,executable linking/
distribution or story rewrite. Existing provenance and rights remain unchanged.

Lecture player/window research by ID1 (2026-10-12):verify_lecture_player_flow.py
pins seven DOL containing bodies(four window/two timing services complete,large
menu builder partial),31 instructions,three whole403-command event spans and
three weapon/professor-table spans. Default-record substitution and mode/feedback/
AC parameter pointers are checked separately from menu availability/runtime.
Six exact US keys001/002/008/009/010/024 bind gor_02 payload hashes and original
speaker operands. No original dialogue rewrite or text extraction into history.
Ignored report is lecture-player-flow-evidence.json and original event inventory
lecture-player-events.json under artifacts/reference/behavior;inspection ELFs/
listings remain ignored. No asset conversion,original execution,ISO/profile/save
mutation or original executable linking/distribution. Existing rights/provenance
remain unchanged.

Lecture field research by ID1 (2026-10-12): tools/verify_lecture_field_flow.py
and its lecture_field_evidence.json manifest pin four original gor event spans,
346 commands,332 scoped ADDR32 relocations,22 operands,four entry/NPC links and
11 message sites in msg/US/gor_02.txt. Only identifiers,offsets,operands and
payload hashes enter source history;original dialogue/control tokens remain in
the read-only resource and must be reused unchanged. Reports lecture-field-entry,
lecture-field-callers and lecture-field-flow-evidence.json remain ignored under
artifacts/reference/behavior. No conversion,ISO modification,original execution,
emulator profile/save mutation or executable/asset distribution occurred.

Field lecture callback research by ID1 (2026-10-12):verify_field_lecture_callbacks.py
and field_lecture_callback_evidence.json pin15 DOL containing bodies,54 selected
instructions,six constants and10 actual gor imports. Twelve local bodies are
fully specified;three general variable services close only used local-coordinate
branches. Original Gekko ELF/listings and field-lecture-callbacks-evidence.json
remain ignored under artifacts/reference/behavior. Metadata and authored behavior
only are versioned. No asset conversion,original dialogue rewrite,ISO mutation,
original execution,emulator profile/save changes or original code distribution.

Field party lifetime research by ID1 (2026-10-12):verify_field_party_lifetime.py
with field_party_lifetime_evidence.json pins six original DOL bodies,43
instructions,three numeric constants,five data-span hashes and three actual
field bindings. Kind1/8 init/hook and ordinary pose-row metadata are pinned;
no animation asset conversion or resource/playback execution occurred. Original
ELFs/listings and field-party-lifetime-evidence.json stay ignored under
artifacts/reference/behavior. General field verifier accepts a manifest and
checks optional data hashes;existing callback checks retain their scope. No ISO/
profile/save mutation,original binary/dialogue distribution or rewritten story.

Field party mode research by ID1 (2026-10-12):verify_field_party_modes.py and
field_party_mode_evidence.json pin eight whole US DOL bodies,44 instructions,
six constants,three data-span hashes,two used lecture imports and original BSS
span. The scoped DOL text integer-store census found no direct r13-based stores
overlapping control-index8041ead0;alias/indexed/floating/REL/bulk writes remain
excluded. Original ELF/listings and field-party-mode-evidence.json remain ignored
under artifacts/reference/behavior. No resource conversion,original dialogue
rewrite,ISO/profile/save mutation,original execution or binary distribution.

Field driver research by ID1 (2026-10-12):verify_field_party_drive.py with
field_party_drive_evidence.json pins four whole DOL bodies,58 instructions,one
numeric constant,four data-span hashes and two used lecture imports. The15-word
mode table is additionally checked as explicit address metadata;the reusable
field verifier now validates optional data words. Kind1/8 records and four
vectors are hashed,not copied as asset payloads. Original ELF/listings and
field-party-drive-evidence.json remain ignored under artifacts/reference/behavior.
No asset conversion,original execution,ISO/profile/save mutation,original
binary/dialogue distribution or rewritten story occurred.

### Complete collision mesh reused for directional wall movement (2026-10-12)

Player wall rays now reuse the complete source collision-strip geometry already
converted into ignored romfs/map.head and romfs/gor_00.head, gor_02.head,
gor_03.head, gor_04.head. These are local derivatives of the read-only US
G8ME01 ISO room maps documented above; no new extraction, conversion, asset
publication or original data commit was made. Original node attributes, source
owner IDs, source collision winding and cull bytes feed a native finite 3D ray
adapter. Unlike legacy near-vertical .walls rows, this mesh retains oblique wall
surfaces. The native policy operates in original engine units and converts to/
from geometry scale.01; triangle math/traversal remain an adaptation requiring
original comparisons. Verified current mesh counts3153/1604/3450/3504/548;
none contains high-bit NPC-owner records. Dynamic named NPC filters require their
own provider; admission of such unsupported records asserts instead of inventing
owner names. Existing original asset ownership/licensing and private-use/no-
redistribution restrictions still apply. All binary packs/captures stay ignored.

Ordinary partner movement research by ID1 (2026-10-12):verify_field_party_follow.py
and field_party_follow_evidence.json pin seven whole US DOL bodies,66 selected
instructions,ten numeric constants,two kind-record data hashes and two lecture
imports. Authored documents specify callbacks/history/read helpers and hit
attribute query,not final terrain/movement engines. Reports and corrected Gekko
ELF/listings remain ignored under artifacts/reference/behavior. No asset
conversion,original execution,ISO/profile/save mutation,original binary/dialogue
distribution or rewritten story.

Partner walk executor research by ID1 (2026-10-12):verify_field_partner_executor.py
with field_partner_executor_evidence.json pins two complete US local bodies,66
instructions,nine numeric constants,five data-span hashes and two actual lecture
imports. Eight substate dispatch words are explicit address metadata. Map keys
aji_02/aji_05/aji_12 and pose keyPCH_A2_1 identify original resource spans;
no story payload is rewritten or copied. Corrected Gekko ELF/listings and
field-partner-executor-evidence.json remain ignored under artifacts/reference/
behavior. No asset conversion,original execution,ISO/profile/save mutation or
original binary/dialogue distribution occurred.

Partner direction/speed research by ID1 (2026-10-12):verify_field_partner_direction.py
with field_partner_direction_evidence.json pins three whole US DOL bodies,64
instructions,19 exact scalar constants and two actual lecture imports. The
correction helper is partyMarioHosei,not a target-count getter;artifacts use the
original annotation. Corrected Gekko ELF/listings and field-partner-direction-
evidence.json remain ignored under artifacts/reference/behavior. No original
execution,numeric seed measurement,asset conversion,ISO/profile/save mutation,
original binary/dialogue distribution or rewritten story occurred.

Partner target/footmark research by ID1 (2026-10-12):verify_field_partner_targets.py
with field_partner_target_evidence.json pins six whole US DOL bodies,55
instructions,three constants and two lecture imports. Live-target wrappers,
clear/ID and SDK paired distance are local contracts;source/domain and original
execution remain separate. Original ELF/listings and field-partner-target-
evidence.json remain ignored under artifacts/reference/behavior. Dolphin paired
interpreter source was read as a public operation-semantic cross-check (link in
SDK distance doc);GPL-2.0-or-later implementation was not copied into native code.
No asset conversion,original execution,ISO/profile/save mutation or original
binary/dialogue distribution occurred.

Partner front-wall research by ID1 (2026-10-12):verify_field_partner_front_wall.py
with field_partner_front_wall_evidence.json pins eight containing DOL hashes;
seven local bodies complete,the backend only selected layout/filter regions.
It checks62 instructions,16 constants,four data-span hashes and two lecture
imports. Original normal/point ABI was corrected from concrete backend stores;
hidden-block keys remain exact identifiers and hashed spans. Original ELF/listings
and field-partner-front-wall-evidence.json stay ignored under artifacts/reference/
behavior. No asset conversion,original execution,ISO/profile/save mutation or
original binary/dialogue distribution occurred.


Partner surrounding-wall research by ID1 (2026-10-12):
verify_field_partner_around.py and field_partner_around_evidence.json pin seven
complete local US DOL bodies,83 instructions,13 constants,five data-span hashes
and two lecture imports. Bodies are the actual partner wrapper,shared surrounding
core,global view leaf and four SDK vector leaves. Original listings/ELFs and
field-partner-around-evidence.json remain ignored under artifacts/reference/
behavior. Shared dot admission and pre-cap saved-vector claims corrected from
original bytes. The supplied ISO stayed read-only;no asset conversion,original
execution,profile/save mutation or original binary/dialogue distribution occurred.
Metadata contains identities/selected anchors,not complete original bodies.


Partner stay postpass research by ID1 (2026-10-12):
verify_field_partner_postpass.py with field_partner_postpass_evidence.json checks
six containing DOL identities (four complete local bodies,two query bodies only
selected entry/output regions),78 instruction anchors,12 constants,three data
spans and two lecture imports. Resource evidence pins las_10 and the exact26-word
named-support pointer table plus25-name containing span;these are resource keys,
not story dialogue. Original ELF/listings and field-partner-postpass-evidence.json
remain ignored under artifacts/reference/behavior. ISO read-only;no asset
conversion,original execution,profile/save mutation or original binary/dialogue
distribution occurred. Full ceiling/floor geometry and runtime domain remain open.


Partner ceiling/scalar ray research by ID1 (2026-10-12):
verify_field_partner_head.py and field_partner_head_evidence.json pin four DOL
identities (head query,scalar wrapper and sincosf complete;shared backend selected
callback/result-layout regions only),89 instruction anchors,six constants,two
zero-template spans and two lecture imports. Actual ground/fly call sites are
pinned. Inspection ELF/listings and field-partner-head-evidence.json stay ignored
under artifacts/reference/behavior. ISO remained read-only;no asset conversion,
original execution,profile/save mutation or original binary/dialogue distribution.
Full backend geometry/domain/runtime behavior remains separate from identities.


## US-default disc-thread system-message resources (ID2,2026-10-12)

Read-only source:/home/carson/games/roms/G8ME01.iso,US G8ME01 disc0/revision0.
[Presentation contract](behavior/DISC_THREAD_SYSTEM_MESSAGE_PRESENTATION.md)
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


## Original ROAD metadata conversion (2026-10-12)

The existing read-only G8ME01 ISO map/REL conversion now resolves the eight
reciprocal gor_00..gor_04 ROAD names against original collision mesh owners,
using their transformed local-bounding-box centers (hitObjGetPos), source-node
identities and AUTO cardinal directions. Vertex-average marker positions are
retired for this table. local-assets/room-links.json and the asset manifest retain
source/arrival node IDs, directions, REL offsets and center/spawn coordinates.
romfs/rooms.bin is a versioned ROAD1 pack:12-byte header plus eight48-byte
records (396bytes). Target placement performs its actual source-height downward
query at runtime; manifest spawnY remains marker provenance, not an assumed floor.
Original triangle winding/cull/draw attributes remain packed unchanged. The
ROAD registration OR00800000 is a runtime overlay, included in transactional
collision rollback. No disc bytes, converted assets or compiled packs were
staged; local-assets/ and romfs/ remain ignored. Original Nintendo/Intelligent
Systems assets retain their copyright; local conversion conveys no right to
redistribute the original or derivative pack. No music/art substitutions were
introduced by this road change.


Partner fall/facing research by ID1 (2026-10-12):
verify_field_partner_fall.py with field_partner_fall_evidence.json pins six
complete local DOL hashes,101 instructions,21 constants,one conversion-bias
span,two lecture imports and five actual walkMain dispatches. Original ELF/
listings and field-partner-fall-evidence.json remain ignored under artifacts/
reference/behavior. make all invoked existing3218-identity/five-fallback PCM
packing using the same read-only ISO sound/proj and local-assets/audio-map/
RomFS paths recorded above;asset/config/audio conversion choices were not edited
by ID1. Local raw/catalog/WAV packs stayed unstaged;original assets retain
copyright and no redistribution grant. No original execution,ISO/profile/save
mutation or original binary/dialogue distribution occurred. Initial native link
failures were unrelated to the verified original instruction evidence.


## Camera lifecycle evidence and unchanged visual assets (2026-10-12)

Camera commit dff51f1 changes portable target/heading/projection ownership only;
no map/art/audio conversion choices or asset pack ABI changed. Original camera
containers remain read-only source evidence,not newly compiled camera-road data.
The original map/hit loader's parent scale10 confirms current authored-map*.1
conversion corresponds to engine*.01 native mechanics (5d86268). Source evidence
verifiers read G8ME01.iso read-only;no original executable/assets or generated
captures were staged. Isolated camera captures remain under ignored
artifacts/azahar-camera-ceiling/ and artifacts/azahar-camera-roads/. Original
assets retain Nintendo/Intelligent Systems copyright;local derivative use grants
no redistribution license. Camera-road view/curve/lens binding remains unfinished.

ID1 placement4 evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied the DOL
partyGetAppearPos4 and selected fall caller instructions,angle data
0x802cc430..0x802cc51c,query templates,bias,map identifier and existing named
support table. Only addresses/hashes/constants/analysis metadata were added to
source history;original ELF/disassembly/report remain ignored under
artifacts/reference/behavior. No story text extraction or new conversion policy
was introduced. make all invoked the existing sound/proj/pmario.proj,pool,sdir,
samp audio-map pipeline:3218 GameCube identities,0 Switch,5 slice primary-PCM
fallbacks packed. Existing local-assets/audio-map raw/catalog outputs and RomFS
WAV/audio-map.json derivatives remain unstaged. Original copyright/ownership
remains with its owners;local possession grants no redistribution license.
Exact build log:/tmp/id1-placement4-build.log.

ID1 general-placement evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
partyGetAppearPos and listed movement/trig/plane/query leaves,selected
placement4 caller,and sixteen zero-origin/direction query templates at
0x802cc1cc..0x802cc34c. Source-history evidence is addresses,hashes,constants,
word metadata and analysis;original ELF/listing/reports remain ignored under
artifacts/reference/behavior. No dialogue extraction or new ID1 conversion
policy was introduced. make all invoked existing sound/proj/pmario.proj,pool,
sdir,samp pipeline:3218 GameCube identities,0 Switch,5 slice primary-PCM
fallbacks. Ignored local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json
derivatives stay unstaged. Original rights remain with their owners;this local
analysis/conversion provides no redistribution license.
Exact build log:/tmp/id1-general-placement-build.log.


## Original camera container pack (2026-10-12)

Camera import2ea1c21 reads /home/carson/games/roms/G8ME01.iso read-only and
extracts m/gor_01/c to local-assets/gor_01.c and romfs/map.camera;
m/gor_00/c,m/gor_02/c,m/gor_03/c,m/gor_04/c go to corresponding
local-assets/gor_XX.c and romfs/gor_XX.camera. File sizes/record counts are gor_00:2048/3,gor_01:7700/11,gor_02:11316/14,
gor_03:11392/14,and gor_04:1304/2. All five packed files match their ISO
extents byte-for-byte. Per-file .provenance.json records original disc path,
FST offset,size and hash;romfs/manifest.json retains those extraction records
and camera_containers source-path mappings. No geometry/field endian conversion
or coordinate scaling is applied to these immutable files. Portable native
readers interpret big-endian fields and rawfile offsets on demand;authored
coordinates convert only at documented runtime boundaries. Both absent pointer
sentinels and every unresolved record/field remain preserved.

The asset fingerprint regeneration also reran existing map/character/texture/
original-music and sound-bank conversion with their established choices;no art
or audio substitute was introduced by this camera change. Build output reported
3218 GameCube identities and5 existing slice fallbacks from the concurrent audio
pipeline;its code/config was not changed in these camera commits. Camera solver
22968ed changes portable behavior only. Captures stay ignored under
artifacts/azahar-camera-assets/. local-assets/,romfs/ and compiled packs were
not staged. Original assets remain copyrighted by Nintendo/Intelligent Systems;
local extraction/conversion conveys no right to distribute these assets or packs.
No ISO or original executable bytes were modified or committed.

ID1 flying behavior/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied flyMain@
party_move.o0x800c7ff8,two flying motion handlers/two speed writers and selected
partyFlyMain caller tail,dispatch0x80315050,map IDs0x802f47b0..0x802f47d0,
zero-vector/bias/numeric constants. Two flyMain names exist in the annotation
CSV;inspection used a temporary namespace-filtered /tmp/id1-party-fly-symbols.csv
rather than choosing Mario's mot_plane.o function. Initial unqualified inspection
rejected ambiguity and produced an empty ignored listing;no binary was changed.
Only metadata/analysis was added to source history;original ELF/listing/reports
remain ignored under artifacts/reference/behavior. No new ID1 art/audio conversion
or story text was introduced. make all invoked existing sound/proj/pmario.proj,
pool,sdir,samp audio-map pipeline:3218 GameCube identities,0 Switch,5 primary-PCM
slice fallbacks. Existing local-assets/audio-map raw/catalog and RomFS WAV/
audio-map.json derivatives remain unstaged. Original rights remain with their
owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-flying-build.log.

ID1 flying history/source/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
partyFlyMain/partyMoveFlyInit,Cloud/Vivian ordinary callbacks,three source leaves
and kind5/6 forty-byte records at0x803144b0/0x803144d8. Only addresses,hashes,
constant/word metadata and analysis were added to source history;original ELF/
listing/report remain ignored under artifacts/reference/behavior. Shared history
buffers are runtime/BSS state,not extracted art or invented source assets.
No original dialogue or new ID1 conversion policy was introduced. make all ran
existing sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube
identities,0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map
raw/catalog and RomFS WAV/audio-map.json derivatives remain unstaged. Original
rights remain with their owners;local analysis/conversion grants no redistribution
license. Exact build log:/tmp/id1-fly-history-build.log.

ID1 slit/roll/paper evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
party_slit/party_roll,floor/activation/forced-exit leaves and paper wrappers.
Original resource/animation IDs at0x802f4750..0x802f47b8 and paper-group IDs
0x802cb360..0x802cb390 are preserved as short interface identifiers,not authored
story dialogue. Restore vectors/double bias/constants are pinned as metadata.
Original ELF/listing/report remain ignored under artifacts/reference/behavior;
source history contains only analysis/addresses/hashes/constant metadata. No new
ID1 asset conversion policy or story text was introduced. make all invoked
existing sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube
identities,0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map
raw/catalog and RomFS WAV/audio-map.json derivatives stay unstaged. Original
rights remain with their owners;local analysis/conversion grants no redistribution
license. Exact build log:/tmp/id1-slit-roll-build.log.


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
See [driver/cache source](behavior/EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md).

## 2026-10-12 — Contact/shift and full camera geometry native binding

Native86150f2/19c5104/93b2817 add camera mechanics without changing original
asset conversion, raw `.camera` bytes, mesh/audio pack formats or source ISO
access. Existing five original camera files remain ignored local RomFS assets;
all44 record identities and494 zone triangles stay authored and unflattened.
No original geometry, texture, dialogue, music or sound bytes were committed.
Ownership/licensing and local-only use remain as recorded above. The actor render
contract now records source model scale/root transforms; no actor rebake or
thumbnail/root conversion is claimed by this step.

ID1 slit-collision/math and message-census provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
unk_800c2010,toMovedirSimple and angleABf@system.o,existing filter/backend
identities,zero-vector templates0x802cb830,bias constants and91-word angleABTBL
0x803041e0(size0x16c). A temporary /tmp/id1-system-angle-symbols.csv selected
system.o rather than same-name pmario_sound.o;initial unqualified inspection
rejected ambiguity. Table remains original reusable data;only span hash/addresses
and short numeric metadata were committed,not an extracted table pack. Original
ELF/listing/report remain ignored under artifacts/reference/behavior. Read-only
message_resource_evidence report confirmed260 msg/US resources/13186 key/text
pairs;only gor_00 key/offset/size/hash metadata is documented,no dialogue prose.
No new ID1 conversion policy was introduced. make all invoked existing
sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube identities,
0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map raw/catalog
and RomFS WAV/audio-map.json derivatives remain unstaged. Original rights remain
with their owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-slit-collision-build.log.

## 2026-10-12 — Authored camera pipeline consumes retained original containers

Native64008d5 uses original camera road/path/zone/lens-metadata identities in
RomFS to calculate the view. Raw camera conversion and all asset packs remain
unchanged; no original bytes or compiled artifacts were committed. File lens
metadata drives the source moving-shift calculation, while live default lens
remains a separate owner. Capture review identified existing cropped actor atlas
root/scale loss; no actor rebake is claimed yet. Original licensing/local-only
provenance remains unchanged.

## 2026-10-12 — Recovery projection and actor-bake preflight

Native8ee75c4 changes recovery mechanics/camera publication without changing
RomFS conversions or original pack bytes. Read-only actor preflight extracted
ISO a/a_mario and a/a_mario- to /tmp/pm-mario-root-bake/a_mario and a_mario.tpl;
character_bake produced temporary idle.png from M_Z_1 frame0 (102 triangles),
matching documented model bounds. This scratch bake was inspected only; atlas
root/scale metadata and native quads still need conversion. Scratch/original
bytes stay outside Git. Existing source licensing/local-use attribution applies;
no generated replacement art, music or sounds were introduced.

ID1 partner pre-display/pose/draw evidence/build provenance (2026-10-12):
read-only /home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
local pre-display/draw/facing/pose and HP/color/Vivian-tail predicate bodies.
Original pose-name tables0x80314708(1200 bytes),0x80314bb8(480),
0x80314d98(420) and adjacent color7 access span0x80314f3c(60) are pinned by
hash/address;no eighth color row or replacement pose string was invented.
Original matrices/pose names remain reused local data;underlying anim/GX/frame
services are not implemented by these docs. Map resource ID bom_01 at0x802f89b0,
ray vector,bias and32 binary32 constants are metadata,not story dialogue.
Original ELF/listing/report remain ignored under artifacts/reference/behavior.
No new ID1 art/audio conversion policy was introduced. make all ran existing
sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube identities,
0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map raw/catalog
and RomFS WAV/audio-map.json derivatives remain unstaged. Original rights remain
with their owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-predisplay-build.log.

ID1 animation setter evidence/build provenance (2026-10-12):US G8ME01 ISO
opened read-only. Five original local bodies and their299 direct DOL references
are pinned as metadata. Original binary32 zero0x8041fb28 and factor
16.66666603088379 at0x8041fb40;initial work/global pointers0x80415dc0/
0x80415fd0 are hashed,not represented as live state. Loaded names/resources are
reused original data;no pose/message/art asset or runtime source was exported
into tracked docs. Inspection ELF/listings/report remain ignored under
artifacts/reference/behavior. make all ran the existing sound/proj/pmario.proj,
pool,sdir,samp pipeline:3218 GameCube identities,0 Switch,5 primary-PCM slice
fallbacks. Existing local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json
outputs unstaged;no ID1 conversion policy change. Original rights remain with
owners;local analysis grants no redistribution license. Exact log:
/tmp/id1-animation-build.log.
