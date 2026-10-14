## 2026-10-14 — Original class circle/bubble consumers and textures

Commit6a94d338 implements whole DrawMainIconCircle801191a8/0x4a0 and
DrawMainIconHukidasi80119648/0x474 local bodies. Typed requests preserve original
GX setup/texture/primitive/icon/font identities, matrix calls, shadows, circle
depth gray byte and scaling, disabled flags, icon0->403 fallback, bubble
mirroring and fresh enabled-state checks. Label width uses unsigned low16 bits,
compressing only X above90 source units. The circle icon's original rotation
then identity reset is retained. Unwritten source angle-offset cases stop at an
explicit frontier after prior effects. See
[the complete consumer contract](behavior/battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md).

Imported original battle/common/battle_common.tpl indices0(40x40 quadrant)
and1(128x80 speech bubble) after checking source constructor pathname and draw
indices. Private256x128 RGBA8 atlas uses2px transparent padding, unchanged pixels
and no resizing. Dedicated class-icon atlas remains separate. Normal assets-ready
now invokes this producer. Paths/hash/conversion/licensing are recorded in
[ASSETS](ASSETS.md); ISO, decoded pixels and compiled packs remain ignored.

Validation: make test in isolated artifacts/tutorial-class-raster-maq_jw1b,
base517fa8795ab9d58c75d0bff065088aedefbb6ba9 plus only this code increment,
passes261/261 C++ and146/146 Python checks
(artifacts/tutorial-class-raster-test.log). New checks cover exact point/quad
counts, shadow and gray colors, disabled/fallback icons, texture indices,
mirrored geometry, retained label with fresh text enable state, width truncation
and compression, atlas pixel equality, UV descriptors and padding. Read-only
source verifier passes3 body hashes,4 data spans and r2 pin
(artifacts/tutorial-class-raster-source.log). Native make all -o assets-ready,
BUILD=build-class-raster-review TARGET=ttyd-class-raster-review passes with the
prepared private packs (artifacts/tutorial-class-raster-native.log), producing
452451756-byte3DSX. Exact new bin and t3x payloads are found inside the artifact
(artifacts/tutorial-class-raster-embedding.json). Final verifier span coverage was
expanded separately after snapshot builds began, rerun successfully and copied
into the snapshot; compiled code/test/converter files match the tested copies.
No Azahar visual check was performed; bundled assets do not prove visible UI.

Next: bind these request consumers to actual native GX/SDK, palette/icon and US
font providers, load the new atlas in the renderer, and bind center/offset with
actual actor projection. Connect ring lifecycle once per simulation tick before
removing manual development class-readiness acknowledgment. Then normal tutorial
command/turn scheduling and successful Jump voice remain. Preserve upper-screen
original menus and bottom-screen lower audience continuation from both user
screenshots and mockup; optional stop-throw taps still require actual audience
telegraphs/events and a physical equivalent. Full weapon UI, audience logic and
broader ordinary battles are incomplete. The active goal is not complete.

## 2026-10-14 — Original class-ring draw lifecycle and icon routing

Commit26a1f8b8 implements complete local DrawMainMenu801160e4/0xb94 and
DrawMainIcon80119abc/0x1c0 bodies with host-testable typed providers. Entry,
stable selection, both rotation directions, wrap bookkeeping, resume translation
and growth update original work arrays and readiness bytes. Source arithmetic
uses the retained angle table, wrap32 integer products/signed division and
float/fused operations. Icon routing resolves the actual acting unit/cursor,
sorts positions through the original descending swap loop, checks visibility
and routes actual flag2 to speech-bubble rather than circle providers. Unwritten
source scratch values are explicit optionals; no invented transforms or count
clamping is supplied. See
[the draw contract](behavior/battle/ACT_SELECT_RING_DRAW_AND_ICON_ROUTING.md).

Validation: final make test in artifacts/tutorial-class-draw-7stb2j28 passes
259/259 C++ and145/145 Python checks
(artifacts/tutorial-class-draw-test-final.log). Snapshot base is
996ae10930d2f72124981347fa9aab0b4363860e plus this scoped increment; it preserves
committed concurrent fixes and excludes unrelated working edits. Final native
make all -o assets-ready BUILD=build-class-draw-review
TARGET=ttyd-class-draw-review passes using existing private converted packs
(artifacts/tutorial-class-draw-native-final.log), producing452443020-byte3DSX.
The read-only source verifier passes4 body hashes,3 data spans and r2 base pin
(artifacts/tutorial-class-draw-source.log). Earlier initial build also passed;
final run includes added sorting/routing cases and final source read ordering.
No emulator/visual run or new original asset extraction was performed.

Limitations/next: these are complete local controllers with linked providers,
not complete native rendering. Camera/projection, GetRingCenter/GetRingOffset,
SDK matrix/interpolation, DrawMainIconCircle/DrawMainIconHukidasi raster, party
button and pinch-mark bindings retain their own frontiers. The explicit native
classMenuPresented acknowledgment remains until a full presentation owner
invokes this lifecycle once per simulation tick. Bind original circle/bubble
resources and actual upper-screen rendering next, then normal tutorial command
and turn scheduling. Lower-screen audience scene continuation and optional
physical-equivalent stop-throw interaction remain required by both user
screenshots and the rough two-screen mockup. Successful Jump voice, full weapon
UI, audience events and ordinary battle scheduling are still incomplete.
The goal remains active; neither tutorial nor full remake completion is claimed.

## 2026-10-14 — Book runes/picture source cinematic crop

Completed the active inside-cover/page-text goal. User clarified the printed
runes and picture needed comparison against original Dolphin. Prior turns
corrected the green cover/paper width; this audit exposed the missing settled
source letterbox crop. Original camLetterBox8000e388 caps top49/bottom44; its
source quads expose Y49..436 (height387). Direct read-only DOL inspection is in
artifacts/book-vertical-letterbox.log. Original isolated Dolphin first-picture
border is at Y67 in reference-t110.png, hence expected native
(67-49)*240/387=11.1628. Previous native border was34; corrected Azahar border
is11. Actual-image row analysis is artifacts/book-vertical-comparison.json.
This proves the reported vertical framing correction without inventing an
independent rune/picture shift.

Commit996ae109 fills that original cinematic interval in the400x240 view with
uniform480/387 zoom and the asymmetric vertical offset. Capture textures keep
their existing193x240 composition/VRAM footprint; normal cover and paper consume
the same outer transform afterward. A source-illustration-edge fit, weighted
by opening/scale/fade, keeps the picture in view. About179px of green cover
remain visible, versus98 before the initial framing fix. Source meshes, UVs,
art, original dialogue, sequence and simulation/frame presentation gates stay
intact. Native pan/fit/left-width adaptations are explicitly documented; full
original image-camera/GX/Gekko equivalence is not implied. The final fade still
returns focus to the illustrated page.

Validation: final make test passed260/260 C++ and145/145 Python
(artifacts/book-vertical-final-tests.log); the preceding full run also passed
260/260 and145/145 (book-vertical-tests.log). Native make all with prepared
assets passed in the coherent private snapshot (book-vertical-native.log).
Both book crop/geometry checks cover the source visible bounds, asymmetric
offset, captured-front layout, hinge continuity and illustration-edge fit.
Original book/capture/camera8 verifier65 anchors passed again read-only
(book-vertical-original.log). The shared unrelated harbor regression from the
prior entry was repaired by its owner before this run; no book-test skips.

Isolated Azahar artifacts/azahar-book-vertical completed four turns and final
zoom/market/Peach handoff, exit0/no native exception. Reviewed initial green
cover and picture(t46), turned runes(t64), all subsequent pages, terminal
Rogueport(t116), final fade/zoom and Peach(t136). Main-screen picture edge11
matches the source visible crop; page/paper retain that position through turns.
Final immutable artifact artifacts/book-vertical-current/ttyd-2ds.3dsx has its
source-scope/binary SHA manifest. No user emulator profiles/saves or ISO writes;
no new extraction/conversion/source payload entered history. Source/rendered
comparison and licensing are recorded in MECHANICS/ASSETS and book contract.
Scoped whitespace/doc links checked. Physical hardware performance, full raster
parity and existing cold scene IO remain follow-up work. Full remake is open.

## 2026-10-14 — Original Trouble Center window rendering

Committed bc65e4ac. Portable full display requests and native Waku/font/scissor
backend now render original four frames/headings/all-count rows with completed
gray/active red styling, three fresh row-ID reads and independent scroll samples.
64 original US resources retained; client/contents copy verbatim21/31-byte caches,
widths prepared for immutablefont/spacing2. Matrix titles correctly ignore shared
.85; rows use proportional per-glyph scaled-pen FMA, existing shaders/arenas and
GPU clipping. No new texture/placeholder, source94 animation or acceptance write.

Reviewed tree35421e0b7b4d768ecc3ae9f40fb2423fadb704e1 passes256/256 C++ and
145/145 Python via make test -o assets-ready and normal make all -o assets-ready.
Sequential assets-ready and existing original class-icon dependency converter
passed. Logs artifacts/trouble-window-{final-assets,class-assets,final-test,
final-build}.log. Normal reviewed artifact SHA256
5c3224471812fddd35608bc63ac8bea3ee44c577503722e78eafb9662caa0a95.
Native fog enum/type and missing independent class-icon test pack failures fixed;
initial failed logs retained. Verified owned changes committed atop concurrent
work; unrelated edits preserved.

Private DEBUG_SHORT_BOOT preview renders original story8 headings/six rows using
explicit test completion/active colors; checkpoint byte-identical. Inspected
artifacts/azahar-trouble-window-preview-checked/original-trouble-window-preview.png;
smoke log trouble-window-preview-checked-smoke.log passes. Initial purewhite check
was corrected against original panelRGBA216/219/216/255; transient missing/tmp
script failure retained, final input script workspace-local. Preview SHA256
8bef578dd92e64696398060bba5f3888aa4c57cdb4940ee4ee468f40f42da388.
Private preview source restored/audited all2600 tracked files and normal-main
recompile timestamp restored. Unintended shared-root preview build artifact
unused; isolated artifact supplies evidence. User profile/saves untouched.
[Exact implementation/limitations](behavior/display/NATIVE_TROUBLE_CENTER_WINDOW_AND_US_CACHE.md).

Next: original menu icons and controller→display binding, complete saved switch
providers, case9/door/cutaway admission for the actual east board, first lecture
and confirmation/acceptance event. All30 help/confirmation table IDs are now
recovered. Then validate acceptedquest2→parcel→Goomfrey→McGoomba full chain.
Font-global/SDK/GX/matched-original pixels, GP38/pad/NDSP parity and hardware
performance remain open. Backend preview is not native board gameplay; full-game
goal remains active/incomplete and future original areas remain to be added.

## 2026-10-14 — User two-screen battle mockup

Recorded the user's subsequent rough hand-drawn layout in
[HUD/menu presentation](behavior/display/NATIVE_BATTLE_HUD_AND_SUBMENU.md):
upper screen holds HUD/stage/battle actors; narrower centered lower screen
continues into audience rows at consistent scene scale. Rough colors/shapes
are composition guidance; original ISO art and the earlier game screenshots
remain the style reference. Audience touch interaction is optional and must
retain a physical equivalent. Exact framing/gap compensation needs visual
validation when the scene continuation is implemented.

Documentation-only update; git diff --check passes. No code, original assets
or test expectations changed. Implementation and validation limitations from
the class UI handoff below remain open.

## 2026-10-14 — Original class UI ownership and physical command binding

Commit0ae3c80f implements whole original ActSelect Init80116cdc/0x1a4 and
End80116c78/0x40 with host-testable allocation/free providers, preserved unused
words, entry/resume arrays, signed source delay arithmetic and ready-byte gate.
Native harbor ActMenu setup/main/end now bind actual class/cursor/status,
position, physical pad and camera services. Class work uses tracked SDKheap0
allocations; legacy weapon projection lifetime is explicitly native. Weapon UI
End frees its current class owner before zeroing it. Physical B returns from
weapons to classes; after explicit presentation readiness, physical A selects
Jump and opens weapons again. The fixture enables Jump in its retained pouch;
source bare-pouch eligibility is unchanged. An explicit openClassMenu entry is
available, but normal Game activation is not added in this increment.

The user's two screenshot references are recorded in
[HUD/menu presentation](behavior/display/NATIVE_BATTLE_HUD_AND_SUBMENU.md):
compact original top HUD, circular icons beside Mario and a selected-command
speech bubble. The lower screen must continue the scene into the lower audience;
the existing lower-screen submenu adapter is interim. Audience stop-throw taps
remain optional and require a physical alternative plus actual original
telegraphs/events. No new original asset extraction or payload commit occurred.

Validation uses immutable snapshot artifacts/tutorial-act-ui-ziodkpyd, base
945cbe10773c328a63d4682fbe6bf86921ffbd71 plus only this code increment.
Required make test ran:256/257 C++ tests passed, including battle_act_ui and
harbor_tutorial_original; harbor_encounter_original fails at
harbor_encounter_tests.cpp:62, groundHeight/landedFloor and jumpHeight assertion,
in the separate actor-motion work. Log:artifacts/tutorial-act-ui-test.log.
Since make stops on CTest failure, Python was run separately:145/145 passed
(artifacts/tutorial-act-ui-python.log). Native make all -o assets-ready with
existing converted private packs, BUILD=build-act-ui-review and
TARGET=ttyd-act-ui-review passed (artifacts/tutorial-act-ui-native.log);
3DSX is452441496 bytes. Existing original act-menu verifier passes24 body pins,
472 instruction anchors and3 data spans
(artifacts/tutorial-act-ui-source-verification.json). No Azahar visual check
was performed for this increment; host/native success does not prove UI parity.

Limits/next: classMenuPresented is an explicit native readiness acknowledgment,
not recovered original visual cadence. Sound tokens are recorded, playback
still needs original binding. Connect Game tutorial turn ownership and original
class icon/bubble renderer, then replace the lower-screen submenu with audience
scene continuation. Successful Jump voice, full weapon UI lifecycle, Crump
scheduling, audience actors/throw response and broader ordinary battle providers
remain incomplete. Continue original US dialogue and simulation-tick action
commands; do not report the tutorial or full remake complete.

## 2026-10-14 — Turned book page width

Follow-up user report resolved in commitbc2cfe37. The native crop placement
compressed the source terminal39.9395-unit page width to120.4426px while its
49.875-unit height used249.375px. The left-folded side now uses the same5px/unit
as Y, yielding199.6975px and a narrow inside-cover border. The captured front
retains193/64px/unit and unchanged frame0 registration; placement is continuous
at authored hinge x=-32. All four original A_1 samples/UVs/materials, source rune
texture, captures, narration and180-tick presentation gates remain intact. This
piecewise native aspect adapter is documented explicitly; it is not recovered
original image-camera/GX raster equivalence. The inside-cover pan and final-fade
focus corrections from4dfec42d/2b62bdf3 remain active.

Validation: both updated book tests pass2/2 (artifacts/book-width-tests.log).
Required shared make test passes258/259 C++ checks; unrelated
harbor_encounter_original fails tests/harbor_encounter_tests.cpp:26
sawRun/sawJump/sawLockedJumpPose after concurrent actor changes
(book-width-shared-test.log). Standalone Python passes145/145
(book-width-python.log). Isolated native make -o assets-ready all passes
(book-width-native.log), using the already converted original packs and snapshot
base4dfec42d plus the scoped final-fade/width changes. A first compiler attempt
lost a temporary assembly file; rerun with snapshot-local TMPDIR passed.

Isolated Azahar normal run artifacts/azahar-book-width exits0 without native
exception, completing four turns, final zoom and market/Peach handoff. Reviewed
initial green cover, first/remaining turned pages, terminal Rogueport page(t116),
fade/zoom(t122) and Peach(t136). The left paper now occupies the inside cover
rather than leaving a broad green strip; front registration remains intact.
Final immutable playtest artifact: artifacts/book-width-current/ttyd-2ds.3dsx;
manifest records exact source scope and binary SHA. Native snapshot/captures and
source inspection remain ignored; no user emulator profile/saves or ISO writes.

Original US read-only Dolphin comparison also completed in an isolated profile
(artifacts/dolphin-book-width, book-width-dolphin.log). First illustration and
first turned rune back are observed at t110/t125; the original viewport clips
the left page and does not establish its complete width. Health-screen input and
process-relative capture labels are not a measurement of original event timing.
Observation is recorded in MECHANICS.md; source bounds remain static resource
evidence. Original cover/capture/camera anchors previously pass65 read-only and
are unchanged by this layout adapter. Asset paths/conversions/license are in
ASSETS.md. Scoped whitespace/document links checked.

Next: physical New2DS XL visual/performance checks, original outer image-camera/
projection comparison and existing cold scene IO stalls. Separate harbor battle
actor/timing regression remains with its owner. Full remake stays incomplete.

## 2026-10-14 — Opened book inside-cover framing

Commits4dfec42d and2b62bdf3. User clarified that the green inside cover immediately after opening looked
small. Source cover geometry/UV dimensions were intact; the closed-cover camera
center left the opened spine at x98, clipping the left green cover to98 visible
pixels while the right side occupied216. Commit4dfec42d adds an explicit native
horizontal pan during opening, proportional to source angle/scale and hinge.
At scale1.5/rotation180 the pan is102 pixels and spine x200; each inside cover
has200 visible pixels. Closed cover/vertical FOV, original geometry, source
artwork, narration order and simulation timing remain intact. This is a native
framing adaptation, not a recovered original camera mutation.

Turning-paper placement receives the same pan. Final zoom scales the pan and
weights it by1-fade to return focus to the illustrated right page.
GPU capture moves from110/0/193/240 to212/0/193/240; it still contains the entire
193x240 crop, including five pixels beyond the visible screen for deformation.
Untilting/crop projection and previous retained pages stay registered. Existing
original assets/texture conversions/licensing remain as recorded in ASSETS.md;
no source art, extracted payload or ISO writes were introduced.

Validation: shared make test passed256/258 C++ checks, including both book tests.
Unrelated trio_attack_original failed its cameraHistoryComplete/pendingFunction
assertion at tests/trio_attack_tests.cpp:464; concurrent battle_action_class_table
failed missing original romfs/battle-class-icons.bin at line74. Standalone Python
passed145/145. Shared make all hit the concurrent uncommitted trouble-quest
renderer compile error. Isolated committed snapshot artifacts/book-inside-review
then passed make -o assets-ready all, reusing prepared private assets. Read-only
US book/capture/camera8 verifier passed65 anchors. Logs are artifacts/book-inside-
test.log, python.log, build.log, review-build.log and original.log respectively
(the shared log basename prefixes are book-inside-). Binary source commit/SHA
is retained in the snapshot manifest. No user profile/save mutations.

Final book geometry/capture tests passed2/2 after the fade adjustment; native
incremental make all passed (book-inside-final-tests.log/final-build.log).
Isolated Azahar normal runs artifacts/azahar-book-inside and
azahar-book-inside-final both completed all four turns and market/Peach handoff
with exit0/no native exception. Reviewed green cover filling200px to the spine
at native x200 (t46), turned paper and final Rogueport page, final zoom and Peach.
Native snapshots/captures are private ignored artifacts. Physical hardware and
original full raster parity remain unmeasured; existing cold IO stalls remain.
The subsequent user report about turned-page width is under investigation;
source A_1 frame180 bounds are x[-71.9395,-32] versus initial[-32,32], and the
current capture-crop scale reduces its screen width further. Compare original
runtime/projection before altering that deformation. Full remake stays open.

## 2026-10-14 — Original action-class descriptors and class-menu icon pack

Committed `9a8a9d32`. Complete shared _btlcmd_MakeActClassTable now preserves
unit/pouch/setup-definition/cursor ownership and entry order before wrapped
M-zero, raw ABI return, seven ordered admissions, live kind/level/flag checks,
all eligibility checks even after denial, reason preservation, original message
keys and source icon selection. Message providers can change kinds, levels and
flags; subsequent source reads remain fresh. Operations/Lecture are independent
tests, allowing both/neither under mutation. Disabled rows still count; padding,
unused rows, leading fields and cursor+4 remain intact. Current count publication
and signed index correction retain negative and zero-count behavior.

Native buildClassMenu exposes actual Mario descriptors from the original field
pouch/definition and unchanged US label/cache providers. Native fixture builds
these records before its existing physical menu/target/cancellation/attack flow;
HP4/7, one payment, failure/dealt1 and result0 still come from the actual failed
Jump. This API does not open class UI or auto-advance turns. Class-table/UI
registration, constructor/animation/resource ownership and Game activation remain
unfinished. No table-shaped placeholders or rewritten labels are introduced.

Imported original IDs334..343 from icon.bin/icon.tpl images339..348, one static
48x48 frame each. Existing message/HUD atlas could not admit ten extra images;
the completed importer uses a dedicated256x256 RGBA8 sheet (262144 bytes) with
2px transparent borders and no resampling. Original general atlas remains
unchanged. Added its normal assets-ready producer. Private outputs are
local-assets/battle-class-icons/ and romfs/battle-class-icons.bin/t3x/json,
using existing PMICON1 format. Original pixels/UVs/padding and portable loader
are tested; native dedicated texture binding/drawing remains pending. Licensing
and conversion choices are in ASSETS; original payloads/ISO are not committed.

Validation: isolated artifacts/tutorial-class-table-tpbgou0l, based on
f9afecc2 plus scoped changes, passed `make test`:255/255 C++ and145/145
Python, no skips. Source fixtures cover kind/level/counter signedness, absence
versus disable, mutable message providers, retained cursor, both/neither final
rows, reason/padding/count/index boundaries and provider frontiers. Native fixture
checks real class labels/types/reasons before battle continuation. Final native
`make all -o assets-ready BUILD=build-class-table-review
TARGET=ttyd-class-table-review` passed with prepared private assets. Artifact:
that snapshot's ttyd-class-table-review.3dsx (451945808 bytes); mmap verified the
actual original class .bin embedded. Logs: artifacts/tutorial-class-table-test.log,
tutorial-class-table-native.log and tutorial-class-table-assets.log. Read-only
class verifier passed4 bodies/7 spans/366 anchors (complete producer and3 linked
services). Final scoped files matched review; diff check passed. Initial atlas
capacity failure was resolved with the dedicated sheet, preserving all general
assets. No Azahar/Dolphin, new original-runtime measurements or rendered/playable
class-menu claim. Concurrent camera/book/quest/render changes were preserved.

Next: original ActSelect UI Init/End and texture/resource lifetime, class-menu
provider bindings into Harbor's existing full Input controller, source first-act/
turn/post-action/phase scheduler and physical class/weapon/target presentation in
Game. Then native rendered menu/cancellation smoke and original-runtime comparison,
successful voice/pitch/volume/follow-up hits, hazards/guard/status domains, and
second-screen scene/audience extension with optional hostile-item taps and a
physical equivalent. Tutorial/real battle/full remake goal remains active and
incomplete; descriptors/resources alone do not prove a playable original loop.

## 2026-10-14 — Original class-menu controls and physical cancellation handoff

Committed `f7b4fba1`. Complete portable ActMenu Main/Setup/End, cursor initializer
and position-change gate now preserve source request order/retained owners,
exact nine cursor resets with counts/exclusions intact, fresh flags after UI
providers, signed Mario/Party move/status gates, live class-count validation,
reason precedence, timer word wrap, all14 published type routes/default and
local0/1 result policy. Sound/UI providers can change rows/cursors/reasons;
source rereads are retained. Navigation keeps forward priority, post-sound wrap
reads and unchecked source count arithmetic. Audience and late position requests
remain ordered; the second position query does not reuse the first answer.
Native class-table/UI constructor binding and Game activation remain incomplete.
The known Display Main blr is a linked provider boundary, not a rendered menu.

Host build exposed the already committed display-footer implementation missing
from HEAD CMake. Added its real source declaration to remove unresolved symbols;
no no-op replacements. Concurrent footer tests/quest-menu/render edits preserved.
Final isolated artifacts/tutorial-act-menu-xnbc3av2, based onc59eda21 plus
scoped helpers/link fix, passed `make test`:252/252 C++ and144/144 Python (no skips).
Native `make all -o assets-ready BUILD=build-act-menu-review
TARGET=ttyd-act-menu-review` passed with prepared private assets. Artifact:
that snapshot's ttyd-act-menu-review.3dsx (451924464 bytes). Logs:
artifacts/tutorial-act-menu-test.log and tutorial-act-menu-native.log.
Read-only act-menu verifier:24 bodies/3 spans/472 instruction anchors; six complete
source bodies/18 linked identities. Scoped implementation matched review and
diff check passed. No new asset conversion, original runtime observation,
Azahar/Dolphin or playable/visual class-menu claim in this step.

The interrupted previous turn completed commit `bf7a17b7`: full source command-
pose/status/body/animation helpers and native physical target B cancellation.
Normal status selects actual original M_C_3 through type67 lookup, not guessed
M_C_1. B wins over simultaneous A, closes target UI, sets state6/help0 and source
part flag80; the following invocation rebuilds state7's weapon list. Repeated
force0 publication preserves native pose timestamp. After UI presentation,
reselection and finalization still launch one original failed Jump: HP4/7,
failure/dealt1, one payment and result0. Weapon-to-class cancellation still needs
the class providers. Native status PoseAnime work remains separate from complete
status-clip/display consumption; no unproved animation completion is synthesized.

Imported unchanged original M_C_3 from a/b_mario/a/a_mario- into the existing
private tutorial pack. Existing mesh/animation conversion and half-size LANCZOS
texture choice retained; provenance/license recorded in ASSETS. Host sampled
actual command-pose vertices under12000. That step's final isolated
artifacts/tutorial-command-pose-j1wt02is (basef19f8ed2) passed251/251 C++
and144/144 Python, and native `make all -o assets-ready
BUILD=build-command-pose-review TARGET=ttyd-command-pose-review` passed.
Artifact451902760 bytes; mmap verified the actual updated Mario .pmanim embedded.
Logs: artifacts/tutorial-command-pose-final-test.log/final-native.log (native log
basename tutorial-command-pose-final-native.log). Verifier pinned8 complete
bodies, original priority27,2,9,1,4,5,3,-1 and actual M_C_3 lookup. Initial fixture
name assumption was corrected from source evidence; an unrelated stale coin-
fanfare manifest was refreshed with its existing original asset producer before
final checks. No ISO/source-payload commit, user profile/save mutation or visual
runtime proof. These validated changes were committed before interruption;
this entry supplies their previously missing session handoff.

Next: implement original class-table admission/eligibility and UI Init/End,
register Act-menu providers in Harbor Input, activate source command/first-act/
turn/post-action/phase scheduler in Game and render/smoke physical menus. Continue
original successful voice/pitch/volume/follow-up hits, damage/guard/hazards and
second-screen scene/audience expansion with optional hostile-item taps plus a
physical-button equivalent. The tutorial/real battle/full remake goal stays
active and incomplete; these are mechanics/provider advances, not completion.

## 2026-10-14 — Original Trouble Center selection producer

Committed a1dfd26a. New retained portable menu implements original30-slot story
unlock order, including20/21 before19, no completed-task filtering, exact A/B/
repeat/page/scroll/icon/SFX requests and next-call one-based selection publication.
Required original display/provider interface keeps native resources external;
no fabricated menu or acceptance writes. Source94 remains opaque; complete
original display does not use it. Copyguard and unknown-provider admission retained.

Source recovery corrected earlier acceptance prose: LW11 is one-based quest ID;
opcode115 is debug, notincrement; acceptance writes1420=ID after nonblocking
RUN_EVT of start_event[ID-1]. Original board/counter map cases, lecture, full
selection producer and Yes/No caller are documented. Complete window display/
64 US title/client/content resource provenance recovered; original frame/clipping/
colors/width rules now available for next native backend. No new asset pack.

Final isolated tree3239633f9328ade5d0fd2e6ef77d26439424567a passes make test
-o assets-ready (252/252 C++,144/144 Python) and normal make all -o assets-ready;
sequential assets-ready passed, converters unchanged. Logs artifacts/trouble-menu-
{assets,final-test,final-build}.log. Normal reviewed artifact SHA256
31499e76d8902f39d666fe01f7fbbe6942568ced17b290a5e6d412cbe33b1f60.
Initial host missing committed footer dependency registration/native mixed-width
min failures fixed and oldlogs retained. Read-only source review passes; audit
matches and concurrent edits preserved. No new native menu/hardware capture.
[Producer validation](behavior/field/PORTABLE_TROUBLE_CENTER_MENU_PRODUCER.md),
[original board](behavior/field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md),
[original drawing/US cache](behavior/display/TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md).

Next: source window backend/icons/US cached strings and complete global switch
providers; source case9/door/cutaway ownership for actual east billboard (notNPC
or professor mask2); first lecture and confirmation/acceptance event. Consolidate
all30 help/confirmation table bindings, then validate acceptedquest2→parcel→
Goomfrey→McGoomba reward in isolated Azahar. Original GP38/pad/NDSP parity,
matched Dolphin/hardware performance and future original areas remain open.
Full-game goal remains active/incomplete; native board UI is not yet playable.

## 2026-10-14 — Native original McGoomba reward event

Committed d7b2c245. Game now admits source Goomba3 quest2/knowntrue5406 reward,
without5405/5376 admission guards. Original irai_02_02 attribution/order,
blocking coin20, quest1420clear→5376set→sys_request_finish speaker0 remain.
One outer NPC KeyOff holds through completion, then KeyOn. Reward EVT follows
camera publication and precedes current Mario; coin waits still visit Mario so
Get/Stay motions advance. Full coin service shape/post/resource readiness is
required before candidacy; copied Game cannot invoke live reward services.

Final isolated tree5c0faad74ecd588ac03b1f86848f3e2f3a8faef1 passes sequential
make assets-ready, make test -o assets-ready (250/250 C++,144/144 Python) and
normal make all -o assets-ready. Logs artifacts/reward-admission-{assets,test,
build}.log. Normal reviewed artifact SHA256
976e3b28e35d03178d877ee5be8cf409a7b2a0e2e1134576f0d9b51569bac5e1.
Read-only review passes; source audit matches and concurrent work preserved.

Isolated DEBUG_SHORT_BOOT Azahar reward capture/save passes using explicit
room0/story8/deliveredquest2, soundenabled/original100coin pouch and private DSP.
Saved v9: quest0/5376true/5406true/120coins/HP10/FP5, both checksums; further input
retains120. Inspected source dialogue, coin/window/pink effect and finish captures
under artifacts/azahar-goomba3-reward/. Log reward-admission-smoke.log. Debug
artifact SHA256046e6c6e0ed726f1a2475cf037eaef6b011749cadc9abfaa782d70769e7f14cb.
No reward instrumentation/forced transitions, new payload or user profile/save
mutation. Early startup capture black; later rendered stages supply the evidence.
Private source restored/audited; normal-main rebuild timestamp restored.
[Exact contract/evidence](behavior/field/NATIVE_GOOMBA3_REWARD_ADMISSION.md).

Next: original Trouble Center acceptance and complete acquisition→Goomfrey→
McGoomba reward validation from that accepted state. Original GP38 admitted-delta
pause clock and retained-input polling parity remain unbound; native audio tail/
matched Dolphin sound/pixels and hardware performance remain unverified. Continue
original area events/NPCs/objects using existing catalogs/providers. Full-game
goal stays active/incomplete; finite reward execution is now native playable.

## 2026-10-14 — Native retained coin presentation services

Committed50f5ceb1. Renderer now binds original coin window/403/type7/camera5,
M_I_U/Get/Stay, row219 shared NDSP and original pouchAddCoins to a Game-owned
stable FieldCoinGet. Per-frame service rebinding cannot overwrite an active
borrowed parameter. Window manager observes actual deletion then retires storage
independently of EVT return2. Overlay uses the original coin window; existing
field pass consumes original icon/effect. Readiness requires free icon, early
camera5 and exclusive shared resources, without parcel key-slot/row217 checks.
Texture/effect shader/vertex pool/atlas are reused; assets unchanged.

Camera placement uses original quantized angle table over current cam4 eye/at,
not renderer atan2 yaw. Native raw svcGetSystemTick uses source divisor units;
original GP38 admitted-delta pause/freeze publication remains unbound and this
clock is explicitly best effort. Prior audio detached-tail check remains open.

Final isolated tree037a54a0b71d081f75639085b1b96bd7a924097f passes make test
-o assets-ready (250/250 C++,144/144 Python) and normal make all -o assets-ready;
sequential assets-ready already passed, converters unchanged. Logs artifacts/
coin-provider-{assets,final-test,final-build}.log. Reviewed normal artifact
artifacts/ttyd-coin-provider-reviewed.3dsx SHA256
e5d86d111bc4e85d716c2596bcfd4619bd04a485ffb04e1fa4551ae1363a8d47.
Read-only final review passes finite resource/service scope. Source audit matches;
concurrent Game/battle edits preserved. No new emulator/hardware validation or
render/performance fidelity claim. [Exact provider scope](behavior/field/NATIVE_COIN_GET_PRESENTATION_SERVICES.md).

Next: bind original Goomba3 reward talk admission/EVT and outer NPC key hold to
these services and persistent5376, verify actual native reward presentation/save.
Implement original Trouble Center acceptance and test acquisition→Goomfrey→
McGoomba full chain. Add original GP38 admitted-delta owner and matched pause/
NDSP clock traces. Continue future-area events/NPCs/objects and hardware profiling.
Full-game goal remains active/incomplete.

## 2026-10-14 — Original reward5376 persistence

Committed b9310091. Appended optional completion5376 and native checkpoint v9
known/value bits24/25. Knownfalse and knowntrue write exactly1536 bytes using
existing dual checksums/pouch layout. Unknown state keeps previous version
selection. Versions1..8 import unknown, v8 retains strict reserved24..31 mask,
invalid value-only/reserved/payload records are rejected. Fresh swInit setsfalse.
Atomic backup flow and independent queued snapshots remain unchanged.

Source reward tests set5376 only after blocking coin return2, after quest1420
clear, and verify5376 does not become an invented admission guard. Read-only
parallel format/source review found no blockers. Final isolated tree
d810dfe0b19a7d7bab864417b809df6069b8d93d passes sequential make assets-ready,
make test -o assets-ready (250/250 C++,144/144 Python) and normal make all
-o assets-ready. Logs artifacts/reward-save-{assets,test,build}.log. Reviewed
normal artifacts/ttyd-reward-save-reviewed.3dsx SHA256
53ce6e34aa6fe0d65214bef9c9b054c7313a5c48181f86afafa6f7a429467842.
Source audit matches; concurrent changes preserved. No new assets or native
emulator/hardware evidence in this increment; previous audio-tail checks remain
unresolved. [Exact format/source contract](behavior/format/NATIVE_REWARD5376_CHECKPOINT_V9.md).

Next: wire Game reward coin/window/effect/icon/clock/audio/Mario services and
independent window deletion retirement to the new persistent5376 owner. Preserve
original reward admission/order and actual early camera5 publication. Implement
original quest acceptance, then verify acquisition→Goomfrey→McGoomba reward in
isolated Azahar. Continue original area events/NPCs/objects and hardware/render
profiling. Full-game goal remains active and incomplete.

## 2026-10-14 — Coin callback borrowed-window lifetime

Committed fcbe48db. FieldCoinGet now supports independent retirement after the
window owner confirms actual deletion in state101. Optional observation leaves
unbound providers retained; copied callbacks, early states and active closure
cannot release storage. Original state101/action query remains unchanged.
The actual CoinGetWindow integration test closes/deletes with action1 retained,
releases parameters without another grant/callback, and safely reopens the window.
Native Game/window-provider wiring remains pending; reward is not yet playable.

Reviewed isolated tree5fb57a5b3e6775acf245ffc331298914d218429f passes sequential
make assets-ready, make test -o assets-ready (250/250 C++,144/144 Python), and
normal make all -o assets-ready. Logs: artifacts/coin-lifetime-{assets,test,build}.log.
Reviewed artifact artifacts/ttyd-coin-lifetime-reviewed.3dsx SHA256
54fda8542d92c85e616fc5a3f71ade0048a9ae1950b2e3b3f9509ffe797c94fa.
Source audit matches the review tree; concurrent changes preserved. This step
uses existing assets, adds no payload and makes no Azahar/hardware timing or
rendering performance claim. Previous native audio tail checks remain unresolved.
[Lifetime contract](behavior/field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md).

Read-only parallel source/save review confirms5376 independent GSF GP+418 bit0;
reward clears1420 then sets5376 after acknowledged coin completion. It is not a
reward admission guard and quest acceptance does not clear it. Next: append
optional5376 state and native v9 checkpoint known/value bits24/25 while keeping
v8 reserved-bit validation and legacy unknown knowledge. Then bind real coin
window lifetime/audio/clock/motion services and Game reward admission, implement
original quest acceptance and verify acquisition→Goomfrey→McGoomba full chain.
Continue original events/NPCs/objects in future areas and hardware profiling;
full-game goal remains active and incomplete.

## 2026-10-14 — Complete source command Input and native target confirmation

Committed `a7557ea2`. Shared BattleCommandInput now implements all30 states and
the unsigned default with original request identities/arguments, captured B/U/M,
unit lookup before wrapped-M-zero, one private cursor output, exact setup flags,
reason-entry ID publication/current-ID waits, fallthroughs and local progress.
The pending-byte re-entry path preserves P; normal re-entry clears it. No loop
limit/one-state-per-frame yield or VM completion2 is substituted. State11 uses
the existing complete target core; state29 selects the source attack, freshly
clears live menu/battle flags, requests OffLight(0,0) and returns1 without a root,
turn consumption or battle/story result synthesis.

Native inputCommand(sample) composes the owner over existing menu providers.
Explicit confirmed-class openWeaponMenu admits state7. Weapon A progresses to
actual help key/mode1/state11 in the same call; a later target A stores29/help2;
the following invocation finalizes the original script. The root-registration
bridge consumes that selection once without selecting it again. Physical A/B/
repeat words come from the actual general pad. Target cancellation is rejected
before effects at its missing CommandAnimPose provider. In-body missing providers
freeze the native command context, and legacy root launch refuses that context,
so a later call cannot advance from a partial state write or replay lost SP08/P.
A new explicit menu build establishes a new context. The real weapon-menu B path
closes/stores1, then freezes at the unbound class Setup provider; it is not falsely
reported as a completed cancellation/menu return.

Complete native _commandRestoreRec is bound; present-item admission uses actual
source getters/retained unit lookup and only complete early exits (item>=339,
invalid target, or actual None item0). Gift/effect paths, operation payment and
populated audience entity producers remain unbound. Source light blr leaves and
whole help/cursor/sampling/attack-selector providers are reused. UI constructors/
animations remain native presentation adapters. This is an explicit native
provider bridge, not yet automatic source class/turn/menu activation in Game.

Validation: isolated artifacts/tutorial-command-8lfuey83, based on5f82f7e4 plus
scoped changes, passed final `make test`:250/250 C++ and142/142 Python (no skips).
Portable tests exercise every state, shared cursor reuse, changed type/actor
values, paired setup/fallthrough policy, reason IDs, retained owners, priority,
fresh final flags and P-preserving pending re-entry. Native fixture drives the
weapon/target/finalization sequence through one root and then actual failed Jump
completion: Crump HP4/Mario7, dealt1/failure1, one payment and result0. It checks
pre-admission target B retains UI and in-body class cancellation freezes across
calls/root-launch attempts. Final native `make all -o assets-ready
BUILD=build-command-review TARGET=ttyd-command-review` passed using prepared
private assets; artifact: that snapshot's ttyd-command-review.3dsx (451598064
bytes). Logs: artifacts/tutorial-command-final-test.log and
tutorial-command-final-native.log (same artifacts directory). Read-only Input
verifier passed39 bodies/3 spans/526 instruction anchors. Final scoped source/
fixtures matched isolated review; diff check passed. Initial fixture compile
scope error was fixed before all final checks. No new art/music conversion,
ISO writes, Azahar/Dolphin or measured/runtime visual-playability claim.
Independent audio/coin/Frankly/pose/field work was preserved by scoped commits.

Next: complete CommandAnimPose/body/status/animation providers so target B can
return to the weapon menu; original ActMenuSetup/Main/class table/UI construction
and BattleCommandInit/first-act/turn/post-action/phase owners; wire that scheduler
into Game and render/smoke the original physical menus; successful voice/pitch/
volume/follow-up hits and stage hazards; second-screen scene/audience expansion,
optional hostile-item taps and physical equivalent. Tutorial/real battle and
full remake requirements remain incomplete.

## 2026-10-14 — Native original coin fanfare219

Committed c3b72f6d. Coin row219 now uses original ff_get4 stereo32000/65352-frame
PCM and COMMON nonloop metadata through the shared retained group1 provider.
Exact readiness/profile gain, source envelopes and actual NDSP drain are reused;
preparation requires a free slot and preserves group0 field music/cursor. Asset
provenance/licensing recorded in ASSETS; no payload/binary/ISO committed.

Reviewed isolated tree17d43265e3d7d23205b213676c824d2af2128f7f passes sequential
make assets-ready,249/249 C++ and143/143 Python via make test -o assets-ready,
and normal make all -o assets-ready. Normal reviewed artifact SHA256
886742a5c7002bef7fe72e53701183ef3873c77f26f5cf2beba70f16e23714cb.
Logs: artifacts/coin-audio-{assets,test,build}.log. Read-only review passes
fresh-admission scope.

Both isolated instrumented Azahar probes FAILED detached-tail assertions.
Natural EOS/vacancy and retained group0/cursor were observed; independent
checkpoint comparison is byte-identical. A second probe used60 owner calls in
a burst and does not prove physical cadence. No source/hardware fidelity,
physical1000ms fade or tail-drain claim. Private sources restored/audited;
user emulator profile/saves untouched and concurrent changes preserved.
[Exact contract and evidence](behavior/audio/NATIVE_COIN_FANFARE_ROW219_PROVIDER.md).
AUDIO-01 requests matched clock/NDSP traces; best-effort provider remains.

Next: resolve native tail timing check, retire borrowed coin params on actual
window deletion, persist5376 and bind Game reward admission. Implement original
quest acceptance and validate complete acquisition→Goomfrey→McGoomba reward.
Continue original areas/events/objects and actual hardware profiling. Active
full-game goal remains incomplete.

## 2026-10-14 — Native original rotating coin and camera publication

Committed `200e326d`. Actual coin403 now retains source frame0/414/delay6,
cycles all four original frames every six icon-owner calls even while hidden,
and preserves phase on reveal. Queued resources resolve/advance in the same
visit; named setters/delete retain first-active duplicate behavior. Renderer
reads published frame state and reuses the existing icon shader/vertex pool.
Original40x40 pixels414..417 fit unchanged in the same512x512/1MiB atlas with
2px padding. No generated art/resampling or larger native allocation.

Native icon visit now follows Game item owners and precedes sound/windows/
effects. Game publishes camera5 immediately after camera4 before acquisition
EVT/Mario; drawing consumes this snapshot, correcting previous render-time
camera5 rebuilding. Existing up/aspect/math/shadow/global-context limits remain.

Final isolated treeff860bbe78016b77ae9cf4f9bd72b18dc7831f70 passed make test:
247/247 C++ and142/142 Python; make all passed. Logs ignored
artifacts/coin-icon-test.log and coin-icon-build.log. Normal artifact
artifacts/ttyd-coin-icon-reviewed.3dsx SHA256
d11a48afceb38a4fa482dc60fc826d2fa9d72d321ba3548ac3645a7b0e066d9f.
Read-only source review and animation/duplicate/camera snapshot checks pass.

Clean DEBUG_SHORT_BOOT parcel capture/save passes: one95,5405true/5406false,
quest2,HP10/FP5 retained, using explicit room0/story8 test state/private DSP.
Log artifacts/coin-icon-parcel-smoke.log. Private simulation-driven raster
preview shows original coin rotating with source title/pink effect, records18
phase captures and proves unchanged checkpoint bytes. Evidence:
artifacts/azahar-coin-icon-raster/ and coin-icon-raster-smoke.log. Preview adds
no reward/audio/Mario-motion/quest writes and validates no complete quest.
Private source restored; audit matches. No user emulator profile/saves touched,
matched Dolphin/hardware parity or stable60 claim; concurrent work preserved.

Next: bind distinct row219 ff_get4 PCM/NDSP, native parameter retirement on
actual window deletion,5376 persistence and Game reward admission. Complete
original task acceptance and verify acquisition→Goomfrey→McGoomba reward in
isolated Azahar. Continue original areas/events/objects and actual hardware
profiling. Goal remains active; full game remains incomplete.
[Exact source, assets, validation and limitations](behavior/display/NATIVE_COIN403_ANIMATION_AND_CAMERA_PUBLICATION.md).

## 2026-10-14 — Original TimingA display and A-button HUD icons

Committed `17b6bced`. Complete shared original battleAcDisp_TimingA8012a7cc/f8
and draw helper8012a6b0/110 now preserve source state gates, state99 interpolation
followed by unconditional30 overwrite, signed entrance/exit counter conditions,
fresh post-provider reads, separate caller/global owners and32-bit wrap. Draw
captures global B before camera8 lookup, freshly selects108/109 from B1f50 bit0,
copies original XYZ templates and requests source offsets(-200,50), scale1 and
flags16, then increments its retained owner's B1f20. This counter does not alter
TimingA result/count or Main's finish countdown.

Harbor admits its known borrowed camera8 callbacks after the event pass once per
physical simulation sample and publishes retained icon requests. Missing art or
unknown identity/owner reports actionDisplayPending before callback effects;
render retries do not mutate source display work. Native overlay consumes these
requests through the existing original-pixel atlas at centered half-size/cropped
400x240 coordinates. Native C2D rendering adapts original camera/GX; no complete
iconGX material/matrix or original-runtime visual alignment claim is made.
Physical A remains the input. Actual native Jump fixtures emit both original A
icons; failed Jump still completes, deals1 HP and produces original feedback,
and successful Jump still reaches the existing voice80104508 frontier.

Imported original icon.bin108/109 descriptors2442/2454 and icon.tpl108/109:
one non-looping48x48 frame each. Original pixels fit the existing512x512 atlas
with2px transparent padding and no resampling. Updated both ignored local and
isolated review packs, retaining newly committed coin403. Conversion/provenance
and copyright restrictions are recorded in ASSETS. No ISO or original payload
was written into source history; no generated art/tone or dialogue rewrite.

Validation: final isolated artifacts/tutorial-display-0d60usfi, refreshed to
297ba204 plus scoped changes, passed `make test`:249/249 C++ and142/142 Python
(no skips). Tests cover source callback state gates/fallthrough, mutable provider
owners/counters, missing providers, wrap, actual native icon requests, repeated/
absent input, unchanged failed-Jump feedback and original atlas pixels/UVs. Final
`make all -o assets-ready BUILD=build-display-review TARGET=ttyd-display-review`
passed with prepared private assets; artifact is that snapshot's
ttyd-display-review.3dsx (451596532 bytes).
Logs: artifacts/tutorial-display-final-test.log, tutorial-display-final-native.log
and tutorial-display-final-assets.log. Read-only display verifier passed3 whole
body hashes,5 constants,2 templates and both icon descriptors; startup r2 base
also pinned. Original listings remain in ignored tutorial-timing-display/draw/
icon artifacts. Final source/asset converter matched scoped review; diff check
passed. No Azahar/Dolphin or rendered screenshot/runtime cadence measurement.
Independent coin, shadow, pose and field edits were preserved.

Next: original class/target menu activation and turn/post-action/phase scheduler
in Game so these battle requests are reachable through normal play; rendered
cue/menu smoke fixtures and visual comparison; successful voice/pitch/volume,
follow-up hits and active stage hazards; second-screen scene/audience rendering
with optional hostile-item taps and a physical-button equivalent. Tutorial and
real battle/full-game goals remain incomplete.

## 2026-10-14 — Native tutorial action-command manager and deferred cleanup

Committed `36aee76a`. Harbor now runs the complete shared original action-command
manager once per physical simulation sample before its event-manager pass.
General input samples first; the manager owns the private-pad update, including
null-Main calls. Repeated timestamps do not shift history or tick Main again.
Missing physical samples leave pad/command work intact rather than synthesizing
no input. Native TimingA8012aae0 uses the existing source state machine: Stop's
1000 sets60 and decrements59 immediately,59 further calls reach1002, then the
next call clears main/display/result/delete in original write order. The outer
manager retains its independent zero-return main-only clear and fresh display
lookup. Unknown main identities remain an explicit provider frontier.

Original camera8/class1/priority15 display requests now enter a runtime-owned
DisplayQueue, exposed as borrowed read-only requests for the current simulation
tick. Tests assert the actual callback8012a7cc, retained BattleWork argument and
sort key399985. The queue is not yet consumed by an action-command draw backend.
The failed original Jump still completes its attack/recovery root, applies1 HP
and produces the actual failure count/original mac_0_b_t1_002 feedback through
explicit phase dispatch; it now also cleans up all four deferred callbacks.
Repeated-timestamp and absent-input fixtures protect countdown and private pad.
Success still stops at voice80104508; battle/story result remains0.

Validation: isolated snapshot artifacts/tutorial-manager-9ae6m6zm, based on
ff9d4b64 plus the scoped changes. Final `make test` passed246/246 C++ and142/142
Python tests. Final native `make all -o assets-ready BUILD=build-manager-review
TARGET=ttyd-manager-review` passed using existing prepared private assets;
artifact: that snapshot's ttyd-manager-review.3dsx (451352076 bytes). Final logs:
artifacts/tutorial-manager-final-test.log and tutorial-manager-final-native.log.
The initial full snapshot run also passed; final rerun covers the cleanup-order
adjustment. Read-only ISO checks passed46 motion/action/facing spans and the
input-cadence verifier's11 body hashes/154 anchors/28 calls/3 data bindings.
Original complete TimingA Main listing inspected at
artifacts/tutorial-manager-timing-main.txt. Diff check passed. No new asset
conversion, ISO writes, Azahar/Dolphin, measured frame timing or rendered cue
claim. Independent field/trio/display/shadow edits were preserved by a scoped
seven-file commit.

Next: original action-command HUD display consumption and source class/target
menu activation in Game; successful voice/pitch/volume and follow-up hits;
post-action/turn/phase scheduler and system-window restoration; active stage
hazards; second-screen scene/audience rendering and optional hostile-item taps
with a physical-button equivalent. This is native manager admission, not the
complete original BattleMain or a finished tutorial/remake.

## 2026-10-14 — Original coin window/effect7 and native camera5 fix

Committed `f82d32cc`. Original coin window80415e84 loads unchanged US title,
redNUM tokens and single-pane geometry. It gates before amount/measurement,
allows text during transition4 and caches expanded text/glyphs/measurement.
Shared native frame/font path serves coin and parcel. Effect kind7 retains
original .75/pink E85BD0, sharing original42/44 and PICA shader with kind6.
Source review found previous native EffectPayload5 was incorrectly a no-op;
actual Game now writes effect camera5 and renderer supplies that selected view.
Original coin403 documentation now pins four duration6 frames, animation even
while hidden, source camera5 and bottom-anchored24x24 geometry.

Final scoped tree4dc27d8aaf31a8e55c75e46efce4bdc60194f87f passed make test:
244/244 C++ and142/142 Python; make all passed. Normal artifact
artifacts/ttyd-coin-window-reviewed.3dsx SHA256
1136ad7461f9bc70c5b901a51feea0686a7fca798b73c72fd0062007720bcaba.
Logs: ignored artifacts/coin-window-scoped-test.log and scoped-build.log.
An earlier private snapshot included incomplete concurrent camera calls and
failed; final scope excludes those calls without changing foreign work.

Clean DEBUG_SHORT_BOOT parcel capture/save checks passed twice after camera5
fix: one95,5405true/5406false,quest2,HP10/FP5 retained. Held original windows/
effect capture: artifacts/azahar-coin-window-camera5-held/parcel-open-window.png;
log artifacts/coin-window-camera5-held-smoke.log. Private fixed-pose raster
preview shows original coin title/red20 and pink effect7 in
artifacts/azahar-coin-window-raster/boot.png. It supplies no reward/icon/audio/
switch writes and proves no native reward playability. Private diagnostic code
was restored; final source audit matches. Only isolated profiles/test saves
were used. No new asset payload/conversion or matched Dolphin/hardware check.
Emulator frames are not stable60/hardware performance evidence.

Important cleanup boundary: original AutoDelete clears active1 but retains4,
so deleted window action can remain1. Native borrowed coin-parameter retirement
must observe actual deletion separately, not delay reward writes waiting for
an unmodified state101 action0. Native icon403 playback, row219 PCM/NDSP,
flag5376 save owner and Game reward admission remain next; complete original
quest acceptance and verify full acquisition→hand-in→reward chain afterward.
Continue areas/events/object coverage and real-hardware profiling. Goal active.
[Exact implementation/validation](behavior/display/NATIVE_COIN_WINDOW_AND_EFFECT7_PROVIDERS.md).

## 2026-10-14 — Failed original Jump script completion and real feedback

Committed `e2740e06`. Complete shared WeaponAftereffect preserves weighted byte
load/subtraction/reread order, force/inhibition, conditional B restoration,
ordered stage requests, three mandatory chance draws, fresh force short-circuits
and object gate/optional draw. Native admission previews with a copy of retained
RNG and only complete non-mutating source leaf exits; unsupported hazard event
producers stay pending before live draws. It never rerolls for another outcome,
skips a selected hazard or rewinds live RNG. Actual dispatch runs once after
admission. Active destruction/nozzle/fall event backends remain incomplete.

Native HP/FP Drain amount wrappers now translate the RAW actor, retain work/
output and use actual badge byte/dealt HP signed min, including null/negative
semantics. Fresh zero badge counts naturally skip recovery; they are not a
universal hardcoded0. The original failed Jump now executes aftereffect,
bounce/return and its recovery child through actual root DELETE. Mario returns
to home/M_S_1 with restored wait handle; attack/damage roots are no longer live,
Crump HP stays4, Mario dealt1/failure1 and battle/story result0 remain. Deferred
TimingA state1000/main callback are still retained for their separate manager.
No turn consumption, source victory or tutorial completion is fabricated.

A controlled original phase dispatch using the produced count then prints
original US mac_0_b_t1_002 after mac_0_b_t1_000 and sets the actual failure
once-latch, without manually setting success/failure counters. This verifies
the failed-attack/feedback script path under explicit scheduler admission;
Game does not yet automatically schedule the menu/turn/phase loop. The successful
Jump still waits at original voice80104508; backend/UI/visual timing remain open.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-aftereffect-gdofmq63, based on56e612ac plus scoped code/tests,
passed `make test`:245/245 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-aftereffect-review
TARGET=ttyd-aftereffect-review` passed with prepared private assets. Artifact:
that review folder's ttyd-aftereffect-review.3dsx. Logs: ignored
artifacts/tutorial-aftereffect-full-test.log and tutorial-aftereffect-native.log.
Read-only aftereffect verifier passed13 bodies/7 data spans/56 anchors; badge
tail verifier18 bodies/32 commands/13 spans/71 anchors. Final isolated code/tests
matched scoped files; diff check passed. No new payload/conversion, Azahar/
Dolphin, measured original timing or complete playable/visual/audio claim.
Concurrent field/camera/display work preserved in scoped commits.

Next: bind original voice/pitch/volume and successful follow-up hits, active
stage hazards, native private-pad/TimingA main/display and post-action scheduling.
Connect original class/target Input/re-entry and restored HUD window to Game,
consume original poses, render original HUD/menu with physical controls and
second-screen scene/audience, plus optional hostile-item taps with a physical
alternative. Verify actual play in isolated Azahar. Goal remains active;
full tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Original Goomba3 reward caller and finite coin service

Committed `9172923a` and `b41495e1`. Portable reward retains original
irai_02_02 attribution, blocking coin20, quest1420=0, flag5376 and original
sys_request_finish speaker0. Source reward guard reads5406, not5405. Finite
coin callback preserves per-call Mario/camera reads and polynomial placement,
strict GP-low wrapped/signed >200/>500 waits, A-only state10→100, current
amount grant, resumeA0-before-Off201 and source return2 before window retirement.
Pouch coin arithmetic stores low16 before signed clamp. Stable parameter
storage stays borrowed until explicit state101 cleanup; no native gameplay
reward is advertised before its presentation/switch providers exist.

Source documentation now pins the complete callback/window/title/icon403/
effect7 and distinct row219 ff_get4_32k stream: stereo32000Hz,65352frames,
COMMON restartFFFFFFFF/nonloop despite DSP loop flags. No key-fanfare/color/
icon substitute or rewritten dialogue is introduced. New native icon lifecycle
and reward-binding requests are in ITEM-01. Existing Goomfrey/Goomba4 quest2
hand-in is already implemented; the earlier parcel handoff's “delivery/reward
unbound” wording should be read as remaining end-to-end validation/reward
providers, not absence of that hand-in owner.

Validation: final isolated tree b5e0c2fa5e74c25aa35aa4c3482a7a0af9180b13 passed
make test:242/242 C++ and142/142 Python; make all passed. Native artifact
artifacts/ttyd-field-coin-reviewed.3dsx SHA256
eb98e3fed60da05043601c011b7108c8aadbe9f3c54d7ff49af2371b0e91b0a2.
Logs: ignored artifacts/field-coin-final-test.log and field-coin-final-build.log.
Reward-only prior snapshot passed241 C++/142 Python/native build. Source review
caught C++ argument read order; explicit locals and regression checks now pass.
Tests compose actual reward/coin owners with original message bytes, source
clock/grant boundaries and delayed parameter cleanup. Native compilation only;
no Azahar/Dolphin/hardware rendering/audio performance observation this increment.
Concurrent changes preserved through scoped snapshots/commits; no asset payload.

Next: bind native coin window, descriptor403's original four-frame icon owner,
effect7 and row219 PCM/NDSP stream with retained cleanup; add original5376 switch
persistence and actual Game admission. Complete upstream quest acceptance and
verify acquisition→Goomfrey hand-in→McGoomba reward in isolated Azahar. Continue
original areas/events/object coverage and hardware profiling. Goal stays active.
[Contracts, evidence and limits](behavior/field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md).

## 2026-10-14 — Complete AP report build and invitation cleanup

Committed `a94b41e6`. Shared ApRecoveryBuild preserves12-byte allocation/reuse/
copy, retained Mario/party lookups, all13 audience-kind queries, original source
weights/sqrt/single rounding and ordered factors/slot bonus. It captures the old
accumulator at the branch-specific source site and wraps the truncated addition.
Even empty reports consume the100-bound RNG draw; fresh chance byte controls the
separate point-increment provider. Complete caller clears its retained12-byte
record only after build readiness. Kind count uses actual presence/state/kind
gates, distinct from Waiting. No empty-audience shortcut, invented report or
immediate pouch SP grant is introduced.

Native Harbor uses its SDK heap0 as explicit allocation policy, actual source
fields/table/math/getters and a zero-chance admission preflight while bingo
PointInc remains unbound. The failed original Jump now allocates/retains its
report buffer, clears the source input record and advances to WeaponAftereffect
801054e8, retaining Crump HP4, Mario dealt1 and failure1. Success still waits at
voice80104508; deferred StopAC/main/display scheduling and story outcome remain
separate. Native report lifetime ends with its battle runtime; no resource
ownership beyond this adapter is inferred from the source allocator call.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-ap-vho_ojsu, based on270abd2b plus scoped files,
passed `make test`:241/241 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-ap-review TARGET=ttyd-ap-review` passed
with prepared private assets. Artifact: that review folder's ttyd-ap-review.3dsx.
Logs: ignored artifacts/tutorial-ap-full-test.log and tutorial-ap-native.log.
Expanded read-only ISO verifier passed46 whole motion/action/audience/report/
getter spans. Final isolated code/test/verifier matched scoped files; diff check
passed. Tests cover allocation/reuse/copy/clear, missing allocation, weighted
factors/slot bonuses, RNG/point provider and presence gates, plus original script
buffer/pending state. Initial live build hit an unfinished concurrent camera
edit; isolated committed sources validated the battle changes. No Azahar/Dolphin,
new payload/conversion, measured timing, native visuals/audio or full playable
attack/tutorial/battle claim. Concurrent edits preserved through scoped commits.

Next: bind original WeaponAftereffect and attack finish, voice/pitch/volume and
bingo point producer; compose native private-pad/TimingA main/display queue and
post-action control. Connect original class/target Input/re-entry to registration,
consume original poses and render original HUD/menu with physical controls.
Complete second-screen scene/audience and optional hostile-item taps with a
physical equivalent, then verify actual play in isolated Azahar. Goal stays
active; tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Native original Goomba3 parcel acquisition

Committed `3abf5525` and `d8efdb46`. Actual Game owners now bind original US
irai_02_00, item95 receive/registry deletion, original name/help windows, M_I_U,
Box icon, effect6 and original fanfare/retained field BGM. Camera8 bit200 stays
separate from camera4; source level2 mask3 stops the type6 talk root while item
Main continues. NPC talk/item KeyOff depths1→2→1→0 preserve separate holds.
Windows, effect and audio advance in simulation order, independently of draw.
Readiness refuses missing assets/DSP/free audio slot/full key storage before
print. Original ISO music is now the runtime default. M_I_U textures stay
prewarmed instead of being reimported per frame. Portable service injection
remains available for later area/item providers. No asset payload is committed.

Final isolated review tree d32b5f5c67c83fe34e41f2c1e5e67379bb3e8199 passed
make test:232/232 C++ and141/141 Python; make all passed. Logs are ignored
artifacts/item-native-key-test.log and item-native-key-build.log. Final native
artifact artifacts/ttyd-item-native-key-reviewed.3dsx has SHA256
4d2d9c4d8459836a9fc24d9d0e8a9fb437ea27f5c3ca466335bc7a9cf4b7f638.
DEBUG_SHORT_BOOT isolated Azahar run passed: original dialogue/item presentation,
return and save prove one95,5405true/5406false,quest2 and HP10/FP5 unchanged.
Evidence: artifacts/item-native-key-final-smoke.log and
artifacts/azahar-item-native-key-final/. Fixture supplies room0/story8/known
quest2, bypassing Trouble Center acceptance; private existing DSP firmware was
copied only into the isolated profile. No user profile/saves were touched.

Limits: source letterbox consumers/global event manager, complete audio macros,
full-storage/discard UI, quest acceptance and Goomfrey delivery/reward remain
unbound. Inline additiveX/Z/combined norm have no retained native producers.
Effect mask appearance is consistent with original42/44, but original pixel/
audio parity and New2DSXL hardware performance remain unverified. Emulator
performance varies; no stable60 claim. Native source tracing was private and
restored. Other sessions' source/index changes were preserved in scoped commits.

Next: consume ITEM-01's full-storage/shared-window and additional-area pickup
requests, complete original quest acceptance/delivery, compare unoccluded
original/native effects and profile render/asset allocations on real hardware.
[Binding and exact validation](behavior/field/NATIVE_GOOMBA3_PARCEL_ACQUISITION.md).
Goal stays active; broader original events/NPCs/objects and full-game work remain
incomplete.

## 2026-10-14 — Original tutorial feedback declaration and audience gates

Committed `76a0ed97`. Shared action declaration preserves exactly-1 bad grading,
200-byte saturation without repairing255, classification, source good-record
weapon/bytes and fused grade multiplier. Audience bad/good helpers use all200
actual waiting queries, captured base/fresh flags and original populated-case
request order. Waiting retains GetExist/GetSysCtrl/halfword/state gates and
fresh getter ownership. Native Harbor preflights zero waiting actors before
count publication; required populated-case providers remain unbound.

Capture now retains live declaration bytes across scene publication. The
original failed Jump itself produces failure1 after its impact, keeps Crump HP4
and Mario dealt1, and advances to InviteApInfoReport8010436c. Repeated ticks do
not replay the count/hit. Actual AcrobatNotry80139898 has a base getter-only
callee; AP-recovery80139af4 is literal li2/blr. Those complete local bodies now
admit their source routes without fabricated SP recovery. Success remains at
voice80104508. Story/battle result stays0; no turn/tutorial completion is claimed.

Correction: earlier progress text called pending8010749c AcrobatNotry. Its
actual identity is AudienceDeclareACResult; AcrobatNotry is80139898. Execution
and assertions used the actual addresses. Native damage documentation is now
corrected. InviteApInfoReport calls actual ApRecoveryBuild801a1d0c and clears12
bytes afterward; its allocation/record/amount/backend services remain the next
boundary, rather than an invented report or immediate reward.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-feedback-a7_ps2h9, based onebf83bc6 plus scoped code/tests,
passed `make test`:240/240 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-feedback-review TARGET=ttyd-feedback-review`
passed with prepared private assets. Artifact: that review folder's
ttyd-feedback-review.3dsx. Logs: ignored artifacts/tutorial-feedback-full-test.log
and tutorial-feedback-native.log. Expanded read-only ISO verifier passed40 whole
motion/action/audience spans, including full declaration/waiting/bad/good and
literal stub bodies. Final isolated code/test/verifier matched scoped files;
diff check passed. No new payload/conversion, Azahar/Dolphin, original-runtime
measurement, native visuals/audio or playable battle/tutorial claim. Concurrent
field/camera/display work preserved through scoped commits.

Next: bind AP report construction, original voice/pitch/volume, aftereffect and
attack finish; compose native private-pad/TimingA main/display and post-action
control. Bind populated audience reactions and original class/target Input/
re-entry, consume original poses and render original HUD/menu with physical
controls. Complete second-screen scene/audience and optional hostile-item taps
with a physical equivalent, then verify actual play in isolated Azahar. Goal
remains active; full tutorial/battle systems and full game remain incomplete.

## 2026-10-14 — Original Mario impact and defender sound attribution

Committed `ecb4226c`. Harbor damage now retains attacker/defender IDs through
power, HP/FP calculation, direct application, flags/dealt HP and actual hit-event
registration. Existing Crump2→Mario1/default power remains; Mario1→Crump2 admits
original Jump/Hammer callback identities with live action bit, current pouch
level, actor bonus byte and source weapon power-table reads. No invented level,
success power or lower-index clamp is supplied. Native read-only base-power
preview supports the existing lethal preflight policy; status/counter/guard/
FP/inventory/other-unit domains remain explicitly bounded.

Damage sound metadata now identifies the actual defender and matching original
kind sound-table entry. Its synchronous owner is restored after application;
reactions no longer universally get Mario's identity and first sound key.
The source-script no-action-press Jump fixture applies1 damage: Crump HP5→4,
Mario U284=1 and real Crump U2b4 reaction. It stops at AcrobatNotry8010749c;
repeated ticks retain HP4/dealt1 without replaying the impact. Native fixture
assertions also retain original owner2 sound keys. Existing Crump impact and
retained lethal/guard checks pass. Success still waits at voice80104508 with
Crump HP5. Story/battle result remains0; no turn/tutorial completion is invented.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-hit-7pc2paby, based on5a3784ae plus the three scoped
code/test files, passed `make test`:236/236 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-player-hit-review
TARGET=ttyd-player-hit-review` passed with prepared private assets. Artifact:
that review folder's ttyd-player-hit-review.3dsx. Logs: ignored
artifacts/tutorial-player-hit-full-test.log and tutorial-player-hit-native.log.
Read-only orchestration verifier matched complete BattleCheckDamage/Pikkyoro/
presentation spans and13 anchors; return verifier retained17 ordered entries,
5 sentinels and21 native hierarchy bindings. Final snapshot matched scoped code/
tests; diff check passed. No new asset conversion/payload, Azahar/Dolphin,
original-runtime measurement or complete playable attack/battle/visual claim.
Concurrent field/display/camera work preserved through scoped commits.

Voice source inspection explains its remaining boundary: wrapper picks from
original5/10-key tables using RNG; unit sound derives position and size-status
pitch, consumes a41-bound draw, starts3D sound, sets pitch and gets/sets live
volume for its sentinel. Do not bypass those operations with a no-op or invented
volume. Listings remain ignored artifacts/tutorial-voice-source.txt,
tutorial-unit-sound-source.txt and tutorial-position-sound-source.txt.

Next: bind complete original voice/backend pitch/volume, AcrobatNotry/audience
and source hit/aftereffect/finish services. Compose private-pad/TimingA main/
display queue and post-action control, preserve deferred StopAC cleanup, and
connect original class/target Input/re-entry to registration. Render original
HUD/menu with physical controls and second-screen scene/audience, including
optional hostile-item taps with a physical alternative. Verify actual play in
isolated Azahar. Goal remains active; tutorial/battle systems and full game
remain incomplete.

## 2026-10-14 — TimingA result evaluation and deferred StopAC

Committed `a4def146`. Shared ResultAC captures work, calls its result provider or
reads null-callback result&2, then rereads the live pending bit independently;
scalar returns never substitute for that stored bit. Help0 resolves its own
current help/audience-window owners. StopAC invokes the retained delete callback
before help0 and preserves callbacks/result/count for deferred cleanup. Stored
result/count readers preserve raw destinations and signed words without any
evaluation/reset. Unknown providers remain explicit completion frontiers.

Harbor admits actual TimingA result8012a8c4 using original options, actor flags/
automatic byte, battle flags, general trigger history, difficulty, result/count/
classification and RNG. Ordinary history evaluation requires a current physical
producer; missing input is not interpreted as no press. No-pad source routes
retain their own admission. Actual delete8012a7c0 stores1000 without prematurely
removing callbacks. Complete native manager/private-pad/display ownership and
finish countdown scheduling remain unbound; this is not full TimingA cadence.

Tests prove ignored scalar return versus fresh pending bit, null fallback and
separate reread, help owner recapture, missing providers, deferred stop keeping
callbacks/count, and raw readers. Native source-script fixtures age menu A out
of general history, supply physical samples per simulation visit and establish
no-press result0/count0/class2 versus one-window-press result2/count1/class3.
Both clear help mode through source effects. With missing physical input Jump
still waits at ResultAC. With samples failure reaches CheckDamage8010a78c;
success reaches original voice80104508. HP and story result stay unchanged at
these service frontiers. These are controlled fixtures, not original measured
impact timing, a playable Game flow or completed battle/tutorial evidence.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-result-rvjxv33f, based on20b93109 plus scoped files,
passed `make test`:235/235 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-result-review TARGET=ttyd-result-review`
passed using prepared private assets, with final result-reader rebuild passing.
Artifact: that folder's ttyd-result-review.3dsx. Logs: ignored
artifacts/tutorial-result-full-test.log, tutorial-result-native.log and
 tutorial-result-native-final.log. Read-only ISO verifiers passed30 whole motion/
action/protocol spans; ordinary Jump19 bodies/268 commands/57 anchors; existing
Jump contracts10 fingerprints/55 instructions/9 constants. Final isolated source
matched scoped code/test/verifier files; diff check passed. An operand-service
type mismatch was fixed before green builds. No Azahar/Dolphin, new payload or
conversion, original-runtime timing or native visual/audio fidelity claim.
Concurrent field/display changes were preserved in scoped commits.

Next: bind original voice and Mario damage/power/source reactions, complete
native private-pad/sequence/TimingA main/display queue, then StopAC finish and
post-action control. Connect original class/target Input and re-entry to player
registration; consume original poses, render original HUD/menu with physical
controls and finish second-screen scene/audience plus optional hostile-item taps
with a physical alternative. Validate play/visuals in isolated Azahar. Goal stays
active; full tutorial/battle systems and full-game work remain incomplete.

## 2026-10-14 — StartAC/help, shared manager and Mario motion

Committed `751b81d5`. Complete source StartAC captures battle before resolving
on, stores state100 and conditionally requests help2 from the retained actor
token. Help key/display setters preserve current-owner recapture and actual
inline audience-window active byte; they do not alter crowd target/count.
Shared ActionCommandManager implements pad→main→fresh display order, null-main
display skip, exactly-zero clearing ONLY main, and camera8/class1/userB/priority15
registration. Missing services remain non-resumable provider frontiers after
prior effects. The obsolete speculative manager reference was replaced with
these inspected source rules.

Harbor now binds the existing complete MarioJumpPosition service using actual
eligibility, platform double arithmetic/FMA/fmod, and original sound key/position
requests with caller-unit metadata. Timing-option raw masks and adjusted
success-window queries are bound. The original selected Jump demonstrably moves
Mario and advances to ResultAC80107708; HP/story result and one payment count
stay unchanged. Native TimingA manager/display cadence is NOT yet automatically
invoked, and ResultAC is still an explicit frontier. No action success, hit,
rendered animation/audio playback, turn or tutorial completion is synthesized.

Validation: final isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-ac-1ptdwkli, refreshed to8418dfb2 plus scoped
code/test files, passed `make test`:234/234 C++ and142/142 Python tests. Native
`make all -o assets-ready BUILD=build-ac-review TARGET=ttyd-ac-review` passed
with prepared private assets; final rebuilt artifact is that review folder's
ttyd-ac-review.3dsx. Logs: ignored artifacts/tutorial-ac-full-test.log and
tutorial-ac-native.log. Read-only ISO verifiers passed23 whole motion/action/
help/window spans; cadence11 bodies/154 anchors/28 calls; ordinary Jump19 bodies/
268 commands/57 anchors; Mario motion5 bodies/20 constants/76 anchors. Final
review source matched scoped code/test/verifier files; diff check passed.
Initial full validation used a stale pose-directory reader after concurrent
pack-format advancement; refreshing from committed HEAD fixed all three asset
loading failures. No new payload/conversion, Azahar/Dolphin, original-runtime
measurement or complete playable-menu/battle claim. Concurrent changes preserved.

Next: compose complete native private-pad/sequence/action-manager and display
queue owners; run TimingA callbacks once per simulation tick, bind ResultAC and
StopAC help/window effects, then original hit services and post-action scheduler.
Do not double-sample pads, use render frames, or clear remaining callbacks when
main returns0. Connect original class/target Input and re-entry to player
registration, consume original poses, render original HUD/menu with physical
controls and finish second-screen scene/audience plus optional hostile-item taps
with a physical alternative. Verify in isolated Azahar. Goal remains active;
full tutorial/battle systems and full-game work remain incomplete.

## 2026-10-14 — Complete original facing and Harbor Jump continuation

Committed `59317591`. Shared CalculateFaceDirection8010e684/318 implements
all original mode gates/ordering: actor/part seeds, captured-battle side default,
movement direction, target comparison, fresh literal-X comparison and final
word inversion. It preserves argument/raw-output capture, current-global unit
lookups, retained-unit part lookups, unit-before-part byte reads, explicit
absolute versus local part positions and single arithmetic. Combined modes are
not reduced to exclusive choices. Direction-write8010e5a8/6c narrows only189.
Harbor binds both with real event/unit/part/platform services.

Tests cover mode precedence, combined17, signed seeds, exact read order,
offsets/absolute XYZ, equality/NaN, integer-to-single rounding, inversion and
low8 write preserving188. The selected original Jump now executes its facing
calculation/write and stops live at StartAC80107788. Repeated ticks preserve
HP/result and the one payment count; no animation, hit, action result or
source tutorial/story completion is invented. Automatic Game/menu activation
and the target Input/re-entry loop remain separate integration work.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-facing-se_b69nv, based on d771b764 plus scoped files,
passed final `make test`:232/232 C++ and141/141 Python tests. Native
`make all -o assets-ready BUILD=build-facing-review TARGET=ttyd-facing-review`
passed with prepared shared private assets, then rebuilt the final read-order
change successfully. Artifact: that review folder's ttyd-facing-review.3dsx.
Exact logs: ignored artifacts/tutorial-facing-full-test.log and
artifacts/tutorial-facing-native.log. Read-only ISO verifier passed19 complete
camera/action/facing/precheck spans, including both new whole wrappers. Source
snapshot matched final changed code/test/verifier files; scoped diff check
passed. A mixed-auto declaration was corrected before green builds. No new
asset conversion/payload, Azahar/Dolphin, measured timing or visual-fidelity
claim. Concurrent field/display work was preserved in scoped commits.

Next: implement complete StartAC with actual help/audience-window setter and
original TimingA manager cadence; then pose, motion/hits and action completion.
Connect original class/target Input and common re-entry to player registration,
render original HUD/menu with physical controls, and finish second-screen
scene/audience with optional hostile-item taps plus a physical alternative.
Verify actual play/visuals in isolated Azahar. Goal stays active; full tutorial/
battle systems and full-game work remain incomplete.

## 2026-10-14 — Harbor Jump camera, takeoff and original AC preparation

Committed `ba0850b3`. Complete shared explicit-parts homing resolves all five
arguments before the signed priority gate; preserves actor/part IDs and actual
home XYZ; target-1 preserves the target fields; other targets use the original
part/hit-position helper. Native Harbor binds this request plus the complete
existing takeoff, move-frame, jump-parameter, fall-acceleration and frame-to-ms
providers with runtime-owned backend/service lifetimes. Camera Main/rendering
is separate; this is not a full camera presentation claim.

Original GetWeaponActionLv, AcSetDifficulty and SetupAC now run in the selected
Jump graph. Setup preserves source reset/registration order, raw options,
actor-token display gate and fields outside its reset scope. It does not invoke
TimingA initialization/Main or clear history/parameters. Tests verify original
TimingA callback addresses, difficulty3, Mario1/part1 and Crump2/part1 camera
state, one zero-cost payment and unchanged HP/result. The original script now
stops live at CalculateFaceDirection8010e684 across repeated ticks, after the
actual takeoff calculation; no invented hit/turn/tutorial result is produced.

Validation: isolated snapshot /home/carson/Documents/blender_tests/pm-port/artifacts/tutorial-camera-000pqybf based on d413a9e9 plus changed code/test
files passed `make test`:231/231 C++ and141/141 Python tests. Native
`make all -o assets-ready BUILD=build-camera-review TARGET=ttyd-camera-review`
passed using previously prepared shared private assets. Artifact: that review
folder's ttyd-camera-review.3dsx. Exact logs: ignored
artifacts/tutorial-camera-final-test.log and tutorial-camera-final-native.log.
Read-only ISO verifier now pins17 complete camera/action/facing/precheck spans;
Jump preparation verifier passed13 bodies/5 constants/68 anchors. Source files
in the isolated snapshot match all changed code/test/verifier files. Scoped
diff check passed. A fresh unique snapshot avoided an older review directory's
symlinks; fixture offset errors were corrected before the final green run.
No Azahar/Dolphin, new payload/conversion, measured timing or playable-menu claim.

Next: implement complete original facing/pose request services, TimingA manager
cadence, Jump motion/hits and post-action scheduler. Connect source class/target
Input and common re-entry to original player registration. Preserve physical
A/B/directions and simulation-tick timing; render the original HUD/menu, then
second-screen scene/audience and optional hostile-item taps with a physical
alternative. Validate in isolated Azahar. Full tutorial/battle and full-game
work remain incomplete; goal stays active.

## 2026-10-14 — Native Mario retained sound posts

Committed `eb9155a9`. Native main now binds actual Audio idle/fall exit services
before each Playing simulation visit, including after restore. Game retains
source initial27c/280/284=-1; ordinary item-get/stay posts cancel27c then280 or
fall284 through source slot Off before clearing handles. NDSP field voices
reserve20..23; existing cue/house/map/music channels are excluded. Pending,
inactive and empty handles do no DSP work; unavailable ownership stays explicit.

Validation: isolated review tree6e163055 passed `make test`231/231 C++ and141/141
Python and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Native adapter stub tests verify actual
channel clears/order, retained handles, no generation check, pending/repeated
cases and reserved-channel refusals. Review identified and corrected the broad
channel-adoption collision. [Exact artifact/logs/scope](behavior/audio/NATIVE_MARIO_MOTION_SFX_POSTS.md).
No new assets/substitute samples, emulator capture, audible-equivalence or
physical performance claim. Idle/fall On/Main/macros/emitter/lifetime are open.

Next: register native acquisition windows/effect/icon and existing fanfare/BGM
consumers, key/system/camera and inline horizontal motion clear after Game
restore. Preserve source sound/window/effect order and borrowed close storage.
Capture actual Goomba3 acquisition with isolated Azahar and measure rendering;
finish full-storage UI and remaining event/area/audio paths. Default acquisition
still has no complete presentation provider; the broad goal remains active.

## 2026-10-14 — Original selected Mario attack in Harbor runtime

Committed `f36818c0`. Harbor's explicit confirmed-command scheduler bridge
consumes a successful original weapon decision, publishes the source event,
and registers it through complete shared state6 rules. Original Jump80355cf0
now has a real priority10 ROOT/owner1, actual U2a8, wait cancellation, status
window SystemOff and channel6 advance. Source suppression admits no invented
root. Table rebuild invalidates the native decision latch; selection sets it
and publication consumes it. The bridge is not an automatic UI/controller.

Selected target/weapon, guard wait and complete fortune query callbacks are
bound. The live64-entry no-protector declaration route preserves target/part
outputs. Shared ordinary item0/FP0/AP0 payment retains real FP access, point/
count and AddAP(0) narrowing/clamping. Other declaration/cost domains remain
explicit frontiers. Harbor event status-window requests now use source packed
arbitration; native startup EventOn/SystemOn and immediate visibility projection
are adapter policies. Source slide animation remains open. Capture preserves
live battle flags against later scene publication.

Validation: isolated source snapshot based on f1754748 plus these six code/test
files passed `make test`:230/230 C++ and141/141 Python tests. Isolated native
`make all -o assets-ready BUILD=build-player-review TARGET=ttyd-player-review`
passed with previously prepared shared private assets. Artifact:
artifacts/tutorial-player-review/ttyd-player-review.3dsx. Exact logs:
artifacts/tutorial-player-review-test.log and tutorial-player-review-native.log.
Read-only ISO verifiers passed status-window6 bodies; attack-selection11 bodies/
8 data spans/301 anchors; weapon selection/cost16 bodies/2 constants; and fortune
child15 bodies/9 data spans/240 anchors. Scoped diff check passed. Initial live
checks encountered concurrent missing audio-test wiring and a native callback
return-type mismatch; the latter was fixed and tested in the isolated snapshot.
Missing isolated private-asset links were corrected before the final green run.
No Azahar/Dolphin or new asset conversion; no playable-menu/full-battle claim.

The integration test completes the original intro, uses physical A in the
source weapon submenu, then explicitly supplies confirmed scheduler admission.
The actual Jump graph retains Crump2/body1, increments zero-cost action count
exactly once, runs data-key7/fortune child and stops live at camera homing-
unitparts8012bc70. Repeated ticks preserve HP/result/count; no invented attack
outcome. The Game still does not automatically invoke this bridge.

Next: implement the complete camera homing-unitparts request (read-only listing
artifacts/tutorial-player-camera-next.txt), then original Mario pose, AC manager/
motion/hit providers. Bind class/target menu Input and common pending/progress
re-entry to selected registration, and render original HUD/menu with physical
controls. Finish second-screen scene/audience and optional hostile-item taps
with a physical equivalent. Verify in isolated Azahar. Goal remains active;
tutorial and full-game work remain incomplete.

## 2026-10-14 — Deferred Mario item-get motion/control

Committed `7929601c`. Game can now request original motion15/stay through
finite motion-table callbacks and initialize M_I_U on the following admitted
Mario owner, before later item ControlOff. Entry resets44/48/50 and horizontal
speed180 without clearing unrelated vertical/air state; original display-sector
heading drives the front-bank static pose. Control uses retained nested byte
and main2; stay is deferred. Nonnull idle/fall post providers are required at
candidate and pending admission; native cancellation is not registered yet.

Validation: review treeb6f6a2ea passed `make test`229/229 C++ and141/141 Python
and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Tests cover deferred Game acquisition
pose/control order, missing exits, preserved motion fields, pose locks and
special/unsupported guards. Shared CMake configure initially hit an unfinished
concurrent battle test reference; review isolated the owned source changes.
[Exact validation/artifact](behavior/field/NATIVE_ITEM95_MARIO_MOTION_PROVIDER.md).
No native gameplay capture or physical performance claim; no new game payload.

Next: implement/register actual native idle/fall voice cancellation and inline
horizontal motion clear (174/17c/180/18c, not additiveY178); key/system/camera,
original windows/effect/icon and existing audio providers after Game restore.
Preserve sound/window/effect visits, borrowed close storage and once-only5405.
Capture native acquisition in isolated Azahar, measure rendering, then finish
full-storage UI and additional event/area paths. Default native acquisition
still lacks presentation providers. The broad remake goal remains active.

## 2026-10-14 — Original target confirmation and command finalization

Committed `92e366e0`. Shared Input state11 preserves the pre-refresh unit/part
pair, signed permutation/count handling, first-match remap, independent forward
and reverse Repeat gates, B-before-A cancellation, post-pose type reread,
retained type for literal multi-item cost pointers, and the audience query after
confirmation. Shared state29 uses the retained actor for attack selection before
fresh flag clears and original result1. Its integration test calls the actual
shared selector and publishes original Jump event80355cf0/category0/usage count
without fabricating an attack root, damage or tutorial completion.

Validation: `make test` passed231/231 C++ and141/141 Python tests; focused final
target-selection rerun passed1/1. `make all BUILD=build-tutorial-menu-layout
TARGET=ttyd-2ds-menu-layout` passed full asset preparation and native compilation.
Both used the project-local ignored TMPDIR. Read-only ISO command-input verifier
passed39 bodies/3 data spans/526 instruction anchors. Logs: ignored
artifacts/target-full-test.log, target-focused-test.log, target-native-build.log,
and target-source-verifier.log. Scoped diff check passed. No Azahar or Dolphin
run; no visual/playable-menu or full-battle fidelity claim. No new asset payload
or conversion; original/private packs remain outside history.

Boundary: handlers require complete invoked services. An unbound provider is an
explicit frontier after preceding effects, not a retry-safe continuation. The
native tutorial does not yet call these handlers, and storing state29 alone
does not set outer progress1 or automatically start an event. Game still needs
original class/target menu presentation, command-pose/UI/help ownership,
common pending/progress re-entry and selected-player attack registration.

Next: bind those complete native providers, connect actual selected Jump roots
and action-command services, then render the original extracted HUD/menu with
physical controls. Finish second-screen scene/audience presentation and optional
hostile-item taps with a physical-button equivalent; verify in isolated Azahar.
Tutorial/full-game work remains incomplete and the goal stays active.

## 2026-10-14 — Game Goomba3 acquisition ownership

Committed `4bd12c2f`. Game now retains the original quest2 acquisition and
registry, admits it through the actual NPC candidate/talk path when complete
presentation callbacks are registered, and schedules event before Mario/NPC
then item. Original printing ends before grant; dialogue/key hold lasts until
cleanup and the subsequent5405 poll. Copies keep presentation state but cannot
advance pointers into the live owner. Restore clears registration.

Validation: isolated tree d12eec21 passed `make test`225/225 C++ and141/141
Python tests and native `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Game tests cover real candidate/talk
admission, missing callbacks, original message, one grant, delayed5405 and
snapshot regressions. Independent review confirmed the ownership guard.
[Exact artifact, logs and limitations](behavior/field/GOOMBA3_GAME_ACQUISITION_BINDING.md).

Next: register native main/renderer/audio providers after each Game restore;
bind motion15 pending entry and [source control-off gate](behavior/field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md),
control/system/camera state, camera5 icon, original M_I_U/effect/windows/audio.
Preserve sound/window/effect order and closing-window borrowed storage. Capture
in an isolated Azahar profile, measure rendering and finish full-storage UI.
Default native acquisition still lacks providers; this is not a playable
acquisition/render validation or full-game completion. Goal stays active.

## 2026-10-14 — Original weapon submenu controls and physical tutorial bridge

Committed `012b38b2`: complete shared WeaponSelect Main/End and source key gate,
plus complete audience-interruption writer with explicit provider readiness.
Confirmation/pending/reason order, interior Repeat vs boundary Trigger,
post-request cursor rereads, forward priority, special Item cancel states,
UI-before-flags End and result0 audience ownership follow source contracts.
Native general pad is provisioned/sampled separately from private defense;
tutorial submenu API confirms with physical A and cancels with B only after
its explicit menu-presented signal. Actual Crump targets come from the source
decision; no HP/result/attack root is fabricated. Cursor/target/reason and raw
sound IDs/keys are exposed for original presentation consumers.

Native open/End/target setup are immediate UI projections, not complete original
UI constructors/animation. Game still lacks automatic original class/target
controller activation; the submenu is not yet visible/playable in the complete
tutorial flow. Host acknowledgements are controlled fixtures, not GPU evidence.
Audience trigger0 uses its source early return; an admitted X interruption
retains the unbound item producer instead of synthesizing item0/-1 or success.
Exact light/UI Main blr leaves remain no-ops; original sounds are queued without
generated tones. Cost/updater bindings were factored for reuse without rebuilding
the table in Main.

Validation: shared make test passed229/229 C++ and141/141 Python tests. Final
focused Main/tutorial rerun passed2/2 after a fixture global-pointer correction
and cursor-reread check. Native make all BUILD=build-tutorial-menu-layout
TARGET=ttyd-2ds-menu-layout passed; final rebuild after cleanup passed with
-o assets-ready (full preparation had completed). Existing source verifiers
passed menu20 bodies/344 anchors and interruption20 bodies/243 anchors. No
Azahar/Dolphin run; no native menu rendering or full battle fidelity claim.
No new game payload/conversion; scoped commits preserve concurrent work.

Next: original class/target controllers and UI activation, selected target/type/
event ownership, Mario action timing/impact and complete turns; original camera,
window/effect/audio and crowd item/throw providers. Preserve original menu/HUD
art and physical routes; audience taps on the lower screen remain optional.
The full tutorial/battle goal remains active.

## 2026-10-14 — Goomba3 portable parcel acquisition

Committed `3b71d849`. Original quest2 acquisition now has a host-testable
print → item entry/force → item service → cleanup → later5405 owner using the
real pouch and registry. Both windows precede the once-only grant; full key
storage stays pending. Duplicate source-name lookup retains initial XYZ and
cleanup deletes all matching entries. The owner is compiled into the native
program; ordinary Game/NPC acquisition and presentation are still unbound.

Validation: isolated review tree41bc4a45 passed `make test`225/225 C++ and141/141
Python tests and `make all BUILD=build-goomba-acquisition-review
TARGET=ttyd-goomba-acquisition-review`. Shared working-tree test failed the
separate trio_attack_original pose assertion (227/228 passed). No new gameplay
render capture or physical performance claim. [Exact validation and artifact](behavior/field/GOOMBA3_PORTABLE_ACQUISITION_OWNER.md).

Next: bind stable Game-owned pouch/switch/talk/registry providers; preserve
source event→Mario→NPC→item→sound→window→effect order and next-Mario-owner
motion15 initialization; connect original camera5 icon, M_I_U, effect/window
and existing native audio/control services; capture acquisition in an isolated
Azahar profile and measure rendering. Full-storage UI and other areas remain
open. The full-game goal remains active.

## 2026-10-14 — Original weapon decision and live tutorial targets

Committed `16baaf3d`: complete shared original setup80122068/6c and decision
80123210/150 wrappers, with typed port service readiness. Decision captures the
actor before rebuild, uses current actor for sampling, preserves signed owner/
direction semantics, writes the live target buffer, resets selection after
sampling, and implements current-count/flags/permutation preference scanning.
Setup preserves table/cursor/UI ordering and final flags OR. Neither body
publishes menu type, creates an attack, consumes resources or advances story.

The tutorial selectWeapon API now rebuilds its real original rows, invokes the
actual packed sampler and exposes target unit/part pairs. Normal Jump selects
Crump2/body1, with HP and roots unchanged. Unavailable rows/overlapping roots
are refused by the admission bridge. Class/submenu/target physical input, type
publication, action registration/timing/impact and turn ownership remain needed;
this target-state binding is not a complete native tutorial or displayed menu.

Validation: shared make test passed226/226 C++ and141/141 Python tests. Final
focused decision fixture passed after adding live count/flag mutation checks.
Native TMPDIR=artifacts/tutorial-admission-tmp make all
BUILD=build-tutorial-menu-layout TARGET=ttyd-2ds-menu-layout passed/rebuilt 3DSX.
Read-only US verifier passed15 bodies,234 anchors,2 spans and16 scoped calls.
Tests cover actor recapture, signed direction, selection reset, first-different/
last-same, live flags/count, permutation position, unresolved services and UI
flag mutation, plus real tutorial Crump target sampling. No Azahar/Dolphin or
menu-render/playability claim; no new game payload/conversion. Scoped commits
preserve concurrent field/render work.

Next: original class/submenu/target input and menu-type publication; source
selected event/root ownership; player command timing, impact and full turns;
original camera/window/effect/audio/audience presentation. Preserve original
art/menu style and physical controls, with optional lower-screen audience taps.
The full tutorial/battle goal remains active.

## 2026-10-14 — Tutorial native weapon providers and field-pouch entry

Implementation is committed in `452a29cd`; contract/asset record in `aff81380`.
The tutorial now binds the complete shared table/eligibility/private-target
helpers to original packed units/weapons, current pouch/equipped queries, icon
leaves, FP/AP costs, actual packed sampling and original US labels. Mapped label
storage copies text/control tokens unchanged and caches original key pointers.
The API exposes source descriptors only after intro/no active phase/attack/hit;
it does not confirm a command, create an attack or advance story/turn state.

The first live menu check exposed bare pouchInit level0, which correctly emits
no ordinary Jump row. Game tutorial entry now uses beginFromPouch when original
field storage exists, copying every entry byte without new-game grants/reset.
Original Mario resource import/revision carries HP/FP/base/max values before
his source init. Tests preserve HP7/FP2 and boots/hammer levels, enabled original
Normal Jump803538a8/icon167 and Hammer80353de8, disabled empty-item row/reason3,
empty locked Special domain, unchanged combat state and entry-copy independence.
Party NPC helper3 remains separate; tutorial party delegation is unbound. Legacy
storage-less saves keep the prior bare-pouch harness. Battle export, full gear
import and class/submenu/input/player action/turn ownership remain incomplete.

Validation: corrected shared make test passed224/224 C++ and141/141 Python tests.
The first Python pass failed a relocated map_visibility include path, then
passed after its correction. Default native make all hit stale pre-migration
shader dependency paths; isolated TMPDIR=artifacts/tutorial-admission-tmp make
all BUILD=build-tutorial-menu-layout TARGET=ttyd-2ds-menu-layout passed and built
ttyd-2ds-menu-layout.3dsx from the new subsystem layout. No Azahar/Dolphin run or
native menu visual/playability claim. No new game payload/conversion. Runtime
changes landed during concurrent commits; current source/history was inspected
and the required bindings/CMake source registration are present in HEAD.

Next: original tutorial class/submenu/input and decision, selected target and
source action registration, action timing/player impact/turns; camera/window/
effect/audio/audience consumers with original assets and physical input. Keep
lower-screen audience taps optional. The full tutorial/battle goal stays active.

# Project progress and handoff

Updated 2026-10-14. The dated implementation record is preserved in
[PROGRESS_ARCHIVE.md](PROGRESS_ARCHIVE.md). This page is the concise current
handoff; consult the archive when exact historical validation or commit context
is needed.

## Documentation and repository organization

Added subsystem maps at `source/README.md`, `include/README.md`,
`tests/README.md`, and `tools/README.md`; they explain the existing filename
prefixes and connect code/test searches to behavior contracts. The runtime
source and public headers now live in subsystem directories while retaining
module basenames. CMake and devkitARM discovery include the grouped directories;
text references were updated to the current source paths. Tests and utilities
remain filename-grouped and their indexes document that convention. The local
generated asset tree is grouped by room/feature; its layout and privacy rules
are indexed in `docs/ASSETS.md`.

The dated asset ledger is preserved in `docs/ASSETS_ARCHIVE.md`; the current
`docs/ASSETS.md` is the short source/storage/license guide. The speculative
render proposal inbox is retired; `docs/OPTIMIZATION_REQUESTS.md` points to
completed evidence and the current unmeasured-candidate audit. The active
behavior research inbox remains `docs/DOCUMENTATION_REQUESTS.md`.

Validation: fresh `make BUILD=build-structure-check all` passed and produced the
native 3DSX; `cmake -S . -B /tmp/pm-port-structure-cmake
-DCMAKE_CXX_COMPILER=/usr/bin/g++` configured successfully and
`cmake --build /tmp/pm-port-structure-cmake --target ttyd_core -j4` built the
host library. `python3 tools/check_doc_links.py` checked 1130 Markdown files
with 0 broken links; `git diff --check` passed. A first build in the reused
default directory failed on stale dependency paths from the old layout; the
fresh build passed after Makefile directory discovery was anchored to TOPDIR.
`make test` was not run. No source behavior or asset conversions changed; build
preparation regenerated/reused ignored packs only.

## Current objective

Build the native New 2DS XL remake toward the D1 Rogueport opening demo and the
G1 full-game goal in [milestones](MILESTONES.md). The remake is unfinished.
The current native slice has an original-content opening through Rogueport
arrival and connected outdoor rooms, but the original Crump tutorial battle
still lacks complete action, display, teardown, field-return and persistence
ownership. Do not treat harness completion as original-game completion.

## Current state

The latest full host-test counts are preserved in the archive. This session's
combined native and host-library build results are recorded below; all
organization and follow-up changes are committed.

- The source-based opening reaches the original arrival checkpoint. Original US
  messages, actors, music selections and assets are retained where bound; full
  renderer/audio parity is unverified.
- The battle harness has original trio attack selection/execution, damage and
  reward work, source state6 registration, status-window arbitration, slot
  initialization, HP gauge/display prefix, original weapon rows and a partial
  tutorial weapon-menu builder. Tutorial input/selection and full encounter
  flow remain incomplete.
- The latest battle boundary is surviving-unit display and pose admission, with
  source default pose identity and non-overlapping native part records retained.
  Original field/battle pose-pool provisioning, remaining status/icon/part
  consumers, battle teardown, field restoration and save/result owners remain
  open. See [HP gauge/display scope](behavior/battle/NATIVE_HP_GAUGE_HISTORY_AND_DISPLAY_PREFIX.md)
  and the [US completion register](behavior/meta/US_COMPLETION_AND_PRIORITIES.md).
- The latest optimization pass implements actor metadata copy filtering,
  single-bin surface clipping equivalence, bounded book workspace and queued
  house-audio cleanup. Hardware performance remains unmeasured; see the
  [optimization audit](optimization/FURTHER_OPTIMIZATION_AUDIT.md).

## Next work

1. Complete original pose-pool/resource admission and the live per-unit,
   queued-display and status/icon/part consumers.
2. Bind ordered battle teardown and field return, then the real result and save
   owners required by D1.
3. Continue the original tutorial menu/input/action sequence and validate the
   complete opening chain without development dialogue or injected outcomes.
4. Profile whole-loop performance on New 2DS XL before making hardware claims.

The active research backlog is indexed in
[DOCUMENTATION_REQUESTS.md](DOCUMENTATION_REQUESTS.md). Recovered subsystem
contracts are indexed in [behavior/README.md](behavior/README.md). Asset source,
conversion, and licensing notes remain in [ASSETS.md](ASSETS.md).

## Latest recorded code validation

The combined reorganized checkout passed `make BUILD=build-structure-check all`,
CMake configure and `ttyd_core` build, the 1130-file Markdown link audit, and
`git diff --check`. `make test` was not run this session. The most recent full
test counts remain the dated results in the archive; they predate this combined
reorganization. No hardware behavior is claimed. Check the archive before
relying on older test counts or emulator captures.

The repository reorganization preserves runtime behavior. Separate feature
commits in this session add the opening-book layout correction, battle part and
pose identity handling, and tutorial weapon-menu construction; none is a claim
of full original battle completion.

## 2026-10-14 — Item-get audio providers

Committed original primary/secondary BGM envelopes, detached fanfare tail and
retained field-music resume (`1a4a5ee`, `ec3ee1f`). Both steps passed isolated
211 CTests/135 Python tests and native builds. Original-mode Azahar confirmed
logical vacancy before tail release and cursor90112→90112 across NULLresume.
The final capture observed Azahar57FPS/native36FPS in its interval; physical
performance and steady integration throughput remain unverified. Native pickup
print/entry/item-main/control/windows/pouch/5405 integration is next.
[Exact validation, artifacts, limitations and next work](behavior/audio/NATIVE_ITEM_GET_AUDIO_VALIDATION_2026_10_14.md)
retain the detailed record while this handoff/archive reorganization proceeds.
The combined reorganized checkout was subsequently rebuilt in this session; the
audio measurements above remain the earlier isolated results.

## Session commits (2026-10-14)

Repository organization is committed in `8e70c746`. Separate follow-up commits
cover the opening-book layout (`b97fa9c6`), battle-part/pose identity
(`83402d8a`), CMake registrations (`b3455044`), message-asset order
(`7f5245f6`), tutorial weapon menu and pouch import (`452a29cd`), recovered
behavior contracts (`9390b332`), generated-build ignores (`b43651bf`), and the
renderer notice (`de2dd009`). Earlier independent agent changes are preserved
in their own commits.

## 2026-10-14 — Battle part isolation and original pose defaults

Feature commit `83402d8a` preserves three distinct original Mario battle parts
and Goombella's separate primary record. Original `M_Z_1`/`PKR_Z_1` defaults are
converted; private sidecars retain all85/32 source descriptor names and original
model/texture identity. See the [storage/directory contract](behavior/display/NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md).

Final combined `make test`:224/224 C++ and141/141 Python passed. Native
`make -o assets-ready all BUILD=build-pose-constructor TARGET=ttyd-pose-constructor -j4`
passed after the migration/menu fixes. Isolated immutable Azahar check reached
the original10 Star Point reward/victory and existing field-restoration hold;
no detected exception. Exact baseline, logs and limitations are recorded in
the [archive](PROGRESS_ARCHIVE.md#2026-10-14--battle-part-isolation-and-original-pose-defaults).

Next: retained real battle pose/resource/texture ownership and per-part entry,
then original selection/restart/rate/effect/release services. Display remains
pending at animPoseSetLocalTimeRate8004142c; teardown and field return remain
unbound. No completed-prologue or hardware-parity claim.

## 2026-10-14 — Book page capture alignment and native aspect

Corrected the observed book-page corruption/shift by replacing CPU framebuffer
readback/rotation/Morton repack with direct GPU render-to-texture. Each page
repeats the same old desktop/cover/page/illustration/retained-paper composition
into a256-square VRAM texture with an untilted crop projection matching native
110/0/193/240 (original124/0/386/480). Temporary target deletion after FrameEnd
waits for GPU completion before source captureComplete/background replacement.
The original372-vertex/124-triangle A_1 deformation, four independent owners,
source rune backs,180-tick waits and narration order remain intact.

Book cover/papers now use the entire400x240 top screen. Original vertical FOV/
geometry scale remains; desktop fills at its own aspect with vertical cropping.
Paper placement applies the original capture-rectangle matrix before global
ImageBase T(offset)*S(scale), correcting final zoom alignment. No arbitrary
compensating page shift, new art, dialogue or invented timing was introduced.
Initial aspect/helper work was retained during the concurrent subsystem move;
GPU correction commit4f719013 uses the new include/story and source/display
paths. Original US init was checked read-only; its book offsets remain0.

Validation: isolated frozen frame0 native GPU replacement compares against the
live original page with all193x240 crop pixels within2 RGB levels, mean absolute
channel error0.0003742084 (book-alignment-verified/crop-comparison.json).
Original CPU/RGB/display-readback probes reproduced malformed captured pixels;
those private diagnostic builds are not deliverables. Direct GPU flat probe
(azahar-book-rtt-flat) fixes it. Production animation run
artifacts/azahar-book-four-turns completes all four turns and zoom/Peach handoff;
reviewed settled pages and transition, typical60FPS. No native exception.

Current reorganized source was then freshly compiled with
make -o assets-ready all BUILD=build-book-verified TARGET=ttyd-book-verified
(book-current-clean-all.log), reusing the already converted original packs.
Its actual normal binary artifacts/book-alignment-current/ttyd-2ds.3dsx passes
another isolated full four-turn/zoom/market run (azahar-book-current,
book-current-smoke.log). Reviewed all pages and final transition. Current cold
market admission samples0FPS at t108 and26FPS at t110, then60FPS market; this is
still synchronous scene IO/performance work, not accepted as hitch-free. Page
registration/art remains correct. Coherent prior baseline plus owned book
changes also builds normal and DEBUG_SHORT_BOOT (book-final-native-all.log,
book-debug-short.log). Normal deliverable and hashes are project-local ignored
artifacts; no diagnostic frame0 hold is enabled in production.

Host checks: coherent snapshot make test209/209 CTest and133/133 Python pass
(book-rtt-tests.log). Current standalone Python141/141 pass
(book-current-python-tests.log). Required shared make test was attempted:
first CMake encountered the directory migration mid-edit; existing host cache
then produced24 failures/225 including stale-layout segfaults. Fresh host build
artifacts/book-host-clean removes those segfaults, passing227/228 CTest, with
only unrelated trio_attack_original assertion at tests/trio_attack_tests.cpp:410
(pose word0x14==74/scalar0x44==1) remaining (book-clean-tests.log). Both book
geometry/capture tests pass. First shared make all encountered stale shader
source dependencies after migration; fresh native build above passes. Original
book/capture/camera8 verifier65 anchors pass read-only (book-original-anchors.log).
Scoped whitespace and documentation-link checks pass; logs retained.

No user emulator profile/saves or ISO writes. Remaining: physical New2DS XL
raster/performance comparison; original image-camera/TEV/shadow parity beyond
this documented adapter; cold market admission stall and separate trio test
failure. Full remake/prologue battle progression remains separate unfinished
work. This correction does not revalidate New Game beyond the previous arrival
route or alter its story endpoint.

## 2026-10-14 — Real player battle pose ownership

Committed `3a8194ec`: real mode1 handles and CPU workspaces for all three Mario
parts and Goombella, complete source descriptor selection/restart/rate history,
separate model/texture references and immutable bank sharing with the renderer.
Native forced display now consumes source names/mode gates and ordinary
counter/zero-effect services before best-effort CPU output. The complete source
Main8003ed2c boundary remains pending. Hidden defaults retain real buffer
ownership; clock branch names follow GP40/44 versus GP38/3c.
[Implementation scope](behavior/display/NATIVE_BATTLE_POSE_RESOURCE_OWNER.md).

Combined make test passed229/229 C++ and141/141 Python checks; native make all
passed. The final isolated source snapshot, including hidden-default refinement,
passed226/226 C++ and141/141 Python tests and native compilation. Azahar on the
first committed owner build reached the original10 Star Point reward/victory
and existing field-restoration hold with no detected exception. The final
hidden-default/clock-name refinement was host/native verified, not rerun in
Azahar. Exact artifacts, earlier failures and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--real-player-battle-pose-ownership).

Next: full animation Main/facing/GX publication, automatic/status-pattern part
consumers and enemy/Frankly factories, remaining queued display and real ordered
teardown/field/result/save ownership. Original global field/battle allocation
history, full cadence and physical hardware parity remain unproven; the full
playable original-fidelity prologue goal stays active.


## 2026-10-14 — Source battle animation time publication

Native battle poses now publish original elapsed/rate, ratio, selected time and
continuous sample fields before using the converted CPU sampler. Source
zero-span, early-time, nonloop end and fused-loop branches are retained;
selected time is sampled without a second wrap. Conversion edge domains stay
explicitly unavailable. The documentation inbox also records the recovered
[ordinary Mario post-Main draw queue](behavior/battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md).

Isolated make test passed232/232 C++ and141/141 Python checks; isolated and
shared native make all passed. Shared host run passed234/235, with a concurrent
battle_facing test failure outside this change. Exact logs/source/binary hashes
and limits are in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--source-battle-animation-time-publication).
No Azahar or hardware revalidation for this step.

Next: retain independent source discrete timeline/cache state, complete Main
publication and bind queued draw depth to original camera4. Enemy/Frankly
factories and ordered battle teardown/field return remain open; the playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Original battle discrete state and model translucency

Private PMPOSE02 sidecars now retain original modelwordc4 and each descriptor's
independent discrete timeline. Native poses publish34/38 and actual value80
from that timeline, with original nonloop clamping and explicit predecessor-1
unavailability. Typed TestXLU reads model bit8; it does not infer translucency
from animation or part colors. [Native format](behavior/format/NATIVE_BATTLE_POSE_DIRECTORY.md).

Shared make test passed236/236 C++ and142/142 Python checks; isolated source
passed233/233 C++ and142/142 Python checks. Shared and isolated native make all
passed. Exact manifests/logs and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-battle-discrete-state-and-model-translucency).
No Azahar/hardware revalidation or complete Main claim for this step.

Next: real compressed cache3c/40/output ownership, full Main and camera4 depth
queue, then ordered battle teardown/result/field return. The full playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Retained pose cache and ordinary Main completion

Native battle poses now retain cumulative five-channel state separately from
fractional output, implement original rebuild/forward/same-key rules and cubic
ordering, preserve loop-seam output and publish actual3c/40 cache history.
Ordinary Main completes for real metadata, unlinked/private-output poses with
facing bit8clear. The trio continues through source alpha and TestXLU, then
retains camera4/class/part/depth at actual dispCalcZ8001086c.
[Scope and limits](behavior/display/NATIVE_ANIMATION_CACHE_AND_ORDINARY_MAIN.md).

Shared make test passed237/237 C++ and142/142 Python checks; isolated snapshot
passed235/235 C++ and142/142 Python checks. Both native builds passed. Azahar
on the immutable isolated build reached original10 Star Points/victory and
the existing field-restoration hold without a detected exception. Exact logs,
source/binary manifests and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--retained-pose-cache-and-ordinary-main-completion).

Next: original battle-camera framing and live camera4 matrices, homogeneous
depth/queue callbacks, remaining pose factories/parts and real ordered battle
teardown/result/field return. General linked/paper/facing, exceptional numeric
domains, original libm bit parity and hardware performance remain unverified.
The full original-fidelity playable prologue goal stays active.


## 2026-10-14 — Original battle camera request and frame ownership

Native camera callbacks now apply original packed writes, including homing,
zoom, offset, MoveTo and Y freezes. Victory requests execute in source order.
A retained camera owner implements Init/default14..16, mode3 timeline and
victory17/18 using actual unit home positions, source interpolation order and
axis/delta history. Camera updates occur once per native time before events;
unsupported prior mode/shake history remains recorded. Explicit-matrix SDK
view/project/depth helpers now preserve homogeneous divide and source clamps.
[Scope](behavior/camera/NATIVE_BATTLE_CAMERA_FRAME_OWNER.md).

Final isolated make test passed238/238 C++ and142/142 Python checks and native
make all passed. Earlier shared builds passed240/240 C++ and142/142 Python
checks plus native compilation before the final event-wrapper additions.
Exact logs/manifests and limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-battle-camera-request-and-frame-ownership).
No Azahar/hardware run for this step.

Next: original camera4 entry/lens/matrix lifecycle, remaining controller modes
and shakes, real display queue/part callbacks, then ordered battle teardown
and field return. The trio still holds at dispCalcZ8001086c; fixed renderer
framing has not been relabeled as original camera4. Full prologue parity
remains unfinished and the goal stays active.


## 2026-10-14 — Native display queue and original action camera framing

Added a host-testable native display queue with original record layout,
class/depth keys, source sort branches and deferred dispatch continuation.
Implemented camera modes0/7/10/11 and remaining MoveTo speed branches;
corrected the earlier speed1 divisor from5 to the ISO-confirmed15. Unit kind
dimensions and part radius/height, scale, rotation and pivot defaults now
come from original initializer stores.
[Scope](behavior/display/NATIVE_DISPLAY_QUEUE_OWNER.md).

Final isolated make test passed242/242 C++ and142/142 Python tests; native
make all passed. Exact logs, snapshot and binary manifests are recorded in
the [archive](PROGRESS_ARCHIVE.md#2026-10-14--native-display-queue-and-original-action-camera-framing).
No Azahar or hardware run for this step.

Next: retain actual field-to-battle camera4 lens/offset/matrix history, bind
the queue to original draw callbacks and native GPU services, and implement
ordered teardown/field return. The trio still holds at dispCalcZ8001086c;
the queue is not yet connected to rendered battle parts. Remaining controller
modes/shakes and full prologue parity stay open; the goal remains active.


## 2026-10-14 — Trio pose depth and borrowed display registration

Bound the finishing display prefix to the native queue using explicit camera
snapshots. Game supplies a declared field-lens numerical adapter in engine
units; original camera4 roll/offset/extra history remains open. A retained
depth continuation resumes missing-provider waits without repeating pose Main
or duplicating Entry. The ordinary immediate continuation now computes the
original stage floor and shadow position/radius, holding at shadowEntry8002be98.
[Scope and limits](behavior/display/NATIVE_TRIO_POSE_QUEUE_BINDING.md).

Shared make test passed247/247 C++ and142/142 Python checks. Final isolated
make test passed244/244 C++ and142/142 Python checks, and native make all
passed. Exact snapshot/logs/manifests are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--trio-pose-depth-and-borrowed-display-registration).
No Azahar or hardware run for this step.

Next: shared original shadow pool and preparation/draw/reset cadence, remaining
part/unit display tails, and actual deferred part matrices/material/GPU draw.
Keep full inherited camera4 publication and ordered battle teardown/field return
open. Existing handheld battle view is unchanged; queue registration alone is
not rendered parity. Full original-fidelity prologue goal remains active.


## 2026-10-14 — Shared shadow requests and live player display traversal

Added the original-layout64-record native shadow request owner, retained settings
and Main registration/reset order. Game shares it with the trio and preserves
it across restores. Finishing display now follows live part links with retained
unit/part continuation, automatic status/name/rate processing and source unit-
tail gates. Mario's extra parts run descriptor0 Main, then original attributes
suppress their drawing/shadows; Mario and Goombella submit two pose and two
shadow records. [Scope](behavior/display/NATIVE_SHADOW_WORK_AND_TRIO_LIVE_DISPLAY.md).

Final isolated make test passed247/247 C++ and142/142 Python checks; native
make all passed. Exact snapshot, logs, failed intermediate checks and immutable
manifests are in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--shared-shadow-requests-and-live-player-display-traversal).
No Azahar/hardware run for this step.

Next: replace the surviving Frankly synthetic part with the
[original constructor](behavior/battle/SETUP4_SURVIVING_FRANKLY_BATTLE_POSE_CONSTRUCTOR.md)
and produce its genuine battle metadata sidecar. Current finishing pass holds
at that pose-constructor provider801011fc. Shadow collision/image/cylinder/GPU
services, full outer-loop and deferred display cadence, footer providers, complete
camera4 history and ordered field return remain open. Full prologue goal stays
active. No rendered parity is claimed by request registration alone.


## 2026-10-14 — Original Frankly battle bank and first display footers

Provisioned surviving Frankly from the actual kind/part/pose table and a
distinct mode1 owner. The Professor converter now imports all17 original
animations, including D_2, and emits genuine PMPOSE02 metadata. The finishing
loop registers his part/shadow, then implements original StockExp counter/EXP
registration and both stage display records. Original display-init bit2 is
retained. [Scope](behavior/display/NATIVE_FRANKLY_BANK_AND_FIRST_DISPLAY_FOOTERS.md).

Shared make test passed250/250 C++ and143/143 Python checks. Isolated make
test passed249/249 C++ and143/143 Python checks; native make all passed.
Isolated Azahar on the immutable build reached original10-Star-Point victory
and the existing field-restoration hold without a detected native exception.
Exact logs/manifests/capture limits are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-frankly-battle-bank-and-first-display-footers).
Asset paths/conversion/licensing are in [assets](ASSETS.md#2026-10-14--complete-frankly-animation-bank-and-battle-metadata).

Next: CommandDisplay's retained palette mutation80119e24, audience animation
interpreter/window gating and slot footer registration; then full borrowed
display dispatch/current-camera/GPU and ordered teardown/field return. Original
actor/update chronology, shadow preparation, full camera4 history and physical
performance remain unverified. The current finishing queue retains14 records
and three unprepared shadows, then holds at the palette provider. Full playable
original-fidelity prologue goal remains active.


## 2026-10-14 — Display footer mutations and Main result publication

Implemented the retained boot-lifetime command palette, all command registration
gates, full200-record audience scalar animation interpreter, independent
audience/window queues and unconditional slot registration. Missing script
reads retain prior stores. The finishing display now clears its original flags
and completes Main's late result/publication without replaying earlier services.
Its actual20 records remain queued; bank4 publishes end bit80000000 and waits
at next outer camMain8000f704.
[Scope](behavior/display/NATIVE_DISPLAY_FOOTER_COMPLETION.md).

Final isolated make test passed251/251 C++ and144/144 Python checks; native
make all passed. Shared C++ checks passed252/252; its Python pass exposed stale
private item-get metadata, refreshed with the existing read-only ISO converter
before final isolated validation. Exact logs/snapshot/manifests are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--display-footer-mutations-and-main-result-publication).
No Azahar/hardware run for this step.

Next: global camera publication and real sorted/ascending-camera deferred draw
with live palette, part/material/GPU and shadow providers. Then bind the separate
sequence-manager exit, ordered BattleEnd/resource teardown and field waiter.
End-bit publication alone does not restore the field. Existing rendering/cadence
adapters and original actor/history/performance limits remain open. Full playable
original-fidelity prologue goal remains active.

## 2026-10-14 — Matrix publication and visible trio camera

Added source-ordered roll/translation and one-use extra projection publication,
including exact perspective float-store ordering with declared native numeric
adapters. The handheld trio view now follows its retained controller eye/target/up;
its world actor-facing reads use that same view. Restored the footer host source
registration missing from latest committed CMake. Recovered all13 original camera
boot/update/draw records without claiming their native implementation complete.
[Scope](behavior/camera/NATIVE_CAMERA_MATRIX_PUBLICATION_AND_TRIO_VIEW.md).

Final isolated make test passed252/252 C++ and144/144 Python checks. Native make
all DEBUG_SHORT_BOOT=1 passed. Isolated Azahar completed actual trio attacks,
original10-Star-Point victory and the existing field-restoration hold without a
detected exception; reviewed captures show wide command/close action/victory
framing. Exact results and immutable build/source manifests are recorded in the [archive](PROGRESS_ARCHIVE.md#2026-10-14--matrix-publication-and-visible-trio-camera).

Next: retain the full13-camera bank and resumable callback/zoom publication;
bind sorted ascending-camera draw/current index, actual queued part/material/shadow
and dummy GPU providers, then ordered sequence-manager exit/BattleEnd/field return.
The finishing camMain8000f704 hold remains. Gekko math, inherited field roll/extra
history, complete draw cadence and hardware performance remain unverified. Full
playable original-fidelity prologue goal stays active.

## 2026-10-14 — Retained camera bank and original tremor consumption

Added the13-camera retained bank, resumable Main and source-ordered portable
Draw owner. Game preserves its shared bank through restore/battle entry. Both
original tremor axes now interpolate, shift the live camera, restore speeds
and consume timers without republishing baseline or replaying earlier work.
Finishing native Main completes all13 camera updates and advances to actual
camDraw8000f454, preserving its20 borrowed records. Explicit missing-lens
callbacks still hold and resume without recollection.
[Scope](behavior/camera/NATIVE_CAMERA_BANK_AND_DEFERRED_DRAW_OWNER.md).

Final isolated make test passed255/255 C++ and144/144 Python checks; native
make all DEBUG_SHORT_BOOT=1 passed. Isolated Azahar completed the actual trio
attack/victory/fade sequence without a detected native exception and reached
the existing field-restoration hold. Exact logs/snapshot/manifests, initial
failed tremor integration check and reviewed captures are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--retained-camera-bank-and-original-tremor-consumption).

Next: bind recovered BeroEXEC/letterbox global/scalar owners into the shared
camera8, then actual ascending-camera viewport/projection/borrowed GPU callbacks,
shadow preparation and genuine dummy draw. Native Draw is not certified by
portable tests; camDraw remains the finishing wait. Ordered sequence-manager
exit/BattleEnd/field return, full field-camera history, source Gekko numerics and
physical performance remain open. Full playable original-fidelity prologue goal
stays active.

## 2026-10-14 — Crump opening facing, original animations and airborne Mario

Commits `945cbe10` and `948c0672` replace fixed Crump/X-Naut Z_1 rasters with
original idle/talk/walk/run geometry and texture clips. Source tribe profiles
provide Goombella/Crump/X-Naut run thresholds20/50/50. Independent simulation
pose clocks and camera-relative NPC presentation now drive all four actors.
Original movement retains event yaw: Goombella retreats facing Crump, while
beforebattle's explicit270/90 turns remain in order. The old adapter incorrectly
turned her toward travel. Mario's scripted movement selects walk/run, keeps
ordinary midair physics and jump/fall poses under key-off, preserves explicit
source poses, and retains M_I_N during the authored displayflag2 surprised hop.
Original dialogue/control tokens, speakers, branches and story order are unchanged.
[Behavior scope](behavior/story/NATIVE_HARBOR_ACTOR_MOTION_AND_FACING.md) and
[asset provenance](ASSETS.md#2026-10-14--animated-crump-opening-field-actors).

Final frozen source `artifacts/crump-validation` passes **256/256 C++ and
145/145 Python** through `make test` (`crump-host-accepted.log`). Tests include
retained Goombella/enemy yaw, source run/walk threshold boundaries, pose clock
continuity, actual original moving geometry, midjump admission/real dry-floor
landing, authored hop pose lock and unchanged three-message battle admission.
Native `make all DEBUG_SHORT_BOOT=1` passes (`crump-native-accepted.log`), as does
normal `make all BUILD=build-normal TARGET=ttyd-2ds-normal`
(`crump-native-normal.log`). Native builds reuse verified original packs using
`-o assets-ready`; source converters ran read-only in this session. Only main.cpp
uses DEBUG_SHORT_BOOT; normal main was explicitly rebuilt without that define.
Verified normal binary is also copied to ignored `ttyd-crump-event.3dsx`.
Exact source/binary hashes are in `artifacts/crump-validation-manifest.json`.
Behavior-contract verification and scoped whitespace/Python syntax checks pass.

Isolated final Azahar capture `artifacts/azahar-crump-opening-accepted`, using
story3-entry and `tests/azahar_crump_opening_motion.py`, reaches source beforebattle
and tutorial entry without detected native exception. Reviewed captures show
Goombella facing Crump through retreat, enemy animated geometry and beforebattle
Mario/Goombella facing. Initial cold captures are black/0FPS; field samples later
reach60FPS, with35FPS at one scene sample and15FPS at tutorial entry. This does
not establish hardware performance or measured original execution parity.

Earlier shared `make all` hit a stale pre-migration shader dependency; fresh
native build passed. Shared host tests hit concurrent tutorial provider edits.
The first isolated /tmp build exhausted the shared tmpfs; final scratch/builds
use project artifacts. Two airborne test attempts assumed floor support at an
invalid harbor water fixture; the final test uses validated dry harbor ground
and verifies actual landing support. The older Azahar story script demanded a
story4 save despite active-event save suppression; its failing assertion was
replaced by an explicit capture script and host progression checks. Those initial
logs remain under `artifacts/crump-*` and are not counted as passing checks.
No ISO, user emulator profile or user saves were modified; original asset packs
remain excluded from source history.

Next: source NPC move-position easing/collision, complete force-move tick and
Mario direction-set timing, original crowd/effect/SFX services, and full tutorial/
postbattle progression. Existing story3 timing and movement bridges remain
adapters; no complete original-fidelity opening or full remake claim is made.

## 2026-10-14 — Original letterbox scalar continuation

Implemented original letterbox decisions, fresh sequence/Key/Ctrl/party reads,
sound-provider order, signed bar/alpha animation and always-registered borrowed
callback. Holds retain flag/scalar work and captured pointers. Native finishing
now enters the actual Draw owner, binds slot0, then waits at missing BeroEXEC
8007ac40; no scene-derived global values are fabricated. Explicit integration
providers resume through registration/sort to real GX viewport802ba56c while
preserving prefix work. [Scope](behavior/camera/NATIVE_LETTERBOX_SCALAR_CONTINUATION.md).

Final isolated make test passed257/257 C++ and145/145 Python checks; native
make all DEBUG_SHORT_BOOT=1 passed. Validation includes the harbor owner's two
fixture-coordinate corrections, independently proved necessary on unchanged
base; those changes are outside this camera commit. Exact snapshot/logs, initial
failed checks and provenance are in the
[archive](PROGRESS_ARCHIVE.md#2026-10-14--original-letterbox-scalar-continuation).
No Azahar/hardware run for this step.

Next: bind original BeroEXEC reset/mask writers, actual sequence-current lifecycle
and other retained letterbox globals, then real native viewport/projection,
queued part/material/shadow/letterbox and dummy GPU work. Native fieldSequence2
is not original current4 for trio. Sound SetFlag accumulated1/2 gates are now
correctly documented. Ordered outer Main/sequence exit/BattleEnd/field return,
original numeric/history parity and hardware performance remain open. Full
playable original-fidelity prologue goal stays active.

## 2026-10-14 — Retained Trouble Center menu/icons

Implemented finite stable-owner TroubleQuestPresentation and native renderer
binding: source local animation/hidden aging/deletion/reopen, exact five named
icons and original paging art, callback source cue IDs and optional state reads.
Corrected original Up Y-mirror geometry as well as UV reversal. Pixel-identical
atlas storage retains original descriptor frames/timing and512x5121MiB budget.
[Scope](behavior/display/NATIVE_TROUBLE_CENTER_RETAINED_PRESENTATION.md).

Validation: final isolated make test261/261 C++ and145/145 Python; restored
normal make all passes. Azahar original paged-menu preview passes both arrow
anchor checks and byte-identical copied checkpoint. Asset check verifies77
original frame entries in73 exact atlas cells. Tree/source audit and exact binary
hashes/logs are recorded in the linked contract. Initial nonoverlap test rejected
intentional aliases; regression now verifies every frame and rejects partial
cell overlap. First10-second preview was still loading;35-second boot passed.
These failures remain archived. Preview providers/visits are explicit fixtures;
no live board/acceptance/audio or hardware parity is claimed.

Next: native original gor_02 case9/door/lecture/global bank/save and original
help/confirmation/acceptance EVT, input/local-clock admission and matching sample
playback. Hardware/raster/event parity remains open. This step provides a callable
backend, not native board gameplay or a completed remake. Goal remains active.


## 2026-10-14 — Port code documentation: runtime, event VM and battle core

Commits `517fa879`, `b4635a79` and `ecde3ebb` add the
[port code guide](code/README.md), runtime ownership/timing, current native
v1..9 checkpoint byte/knowledge-bit tables, event VM implementation walkthrough
and core battle action/status/resource/damage guide. Source/header maps and the
project README link to this collection. Inline comments in **48 C++ files**
explain original masks, opcode/state/result numbers, packed offsets, signed
narrowing/wrap, callback counts, provider ownership and native policy values.
This pass uses the requested nearby-comment option for recovered binary values;
it does not rewrite numeric ABI constants or assign invented names to unknown
fields. In particular, it documents retained PAD samples after transfer errors,
first-poll wait decrement, WAIT_MSEC low-word signed/unsigned division, independent
handler/manager result protocols, flat/nested scan asymmetries, unresolved pointer
aliases, distinct map/SDK allocator headers, optional save-state knowledge,
private defense/general action input and requested-versus-consumed damage.

Final frozen baseline `99943ec006d7fbaf4dd7a0af6e03e90d1c390861` plus exactly
this documentation pass passes **260/260 C++ and 145/145 Python** through
`CMAKE_BUILD_PARALLEL_LEVEL=4 make test`, and normal native
`make all -o assets-ready -j4` passes. Exact logs are
`artifacts/code-documentation-validation/host-final.log` and `native-final.log`;
`manifest.json` records baseline, source hashes, unchanged executable-token
checks for all48 touched C++ files, and ELF/3DSX hashes. Live files match the
frozen source hashes. Code-guide local links and scoped `git diff --check` pass.
The isolated native build reuses existing private packs via `romfs`/`local-assets`
symlinks and skips regeneration; it does not independently freeze/verify every
private asset. No new emulator or hardware validation was needed/performed for
these comment-only executable changes. ISO and user emulator profiles/saves
were not modified. Existing compiler warnings remain outside this pass.

Initial shared `make test` failed because concurrent CMake edits referenced a
not-yet-present `tests/battle_class_raster_tests.cpp`; initial shared `make all`
failed on a stale pre-migration shader dependency. Those attempts are recorded
in `artifacts/code-documentation-test.log` and `code-documentation-native.log`,
and are not counted as passing checks. A fresh isolated source/build avoids
those unrelated mutations; other working-tree changes were excluded from the
documentation commits and validation snapshot. The first isolated runtime/event
pass also passed260/260 C++ and145/145 Python plus native build (`host.log`,
`native.log`), before adding battle-core comments and final validation.

Next documentation tasks, in order: audit the large `source/runtime/game.cpp`
and remaining runtime headers (especially units, collision pack flags and tick
ownership), renderer/animation/camera helpers, field/NPC/partner mechanics,
remaining battle packed/event/menu/display helpers, then message/inventory/story
modules and shaders. Review existing comments rather than blanket-inserting
generic summaries; retain exact dialogue/assets and original/provisional scope.
Continue incremental commits and required validation. The user's **port-code
documentation goal remains active**: this is a substantial first pass, not
complete coverage of the port, and not a completed remake claim.
