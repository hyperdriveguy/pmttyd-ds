# Complete-game behavior documentation work register

**Version scope:** This directory documents the GameCube US binary. Current
Switch 1.0.0 research lives in [the separate Switch collection](../switch/README.md).
Do not import these contracts as established Switch behavior.

Objective: thoroughly document behavior not established by available
decompilation, then produce a system requirements document for the **complete
game**, assuming original models, textures, messages, sound and other data are
reused. No requirement is satisfied merely by listing a subsystem or its symbols.
This collection is in progress as of 2026-10-12.

The [current US completion assessment and priorities](meta/US_COMPLETION_AND_PRIORITIES.md)
supersede older next-task lists: prioritize shipped event/command bindings and
complete gameplay sequences before further general-library edge-case recovery.

## Subsystem folders

Documents are grouped by owning subsystem. New files must be added to the folder
for their subsystem, with links written relative to that location; keep only this
`README.md` index at the collection root. Raw per-function disassembly dumps
live beside the contract for the subsystem they serve, and derived-reference
documents generated from one `.elf`/`.disasm` pair use a `..._REFERENCE.md`
suffix.

| Folder | Subsystem |
| --- | --- |
| [`meta/`](meta/) | Collection-level audits, completion register and RNG baseline |
| [`platform/`](platform/) | Boot, retrace/main loop, disc-thread and clock publication |
| [`event/`](event/) | Event VM, opcodes, constructors, scheduling and dispatcher |
| [`battle/`](battle/) | Battle/turn engine, action commands, weapons and tutorial encounters |
| [`input/`](input/) | Physical input producer and battle input cadence/snapshots |
| [`unit/`](unit/) | Unit/part/pouch/inventory records and stats |
| [`partner/`](partner/) | Party, partner and NPC motion, forms and presentation |
| [`field/`](field/) | Field entry, intersections and non-partner field services |
| [`overworld/`](overworld/) | Overworld Mario movement, roads and motion arbitration |
| [`camera/`](camera/) | Camera containers, paths, follow and collision targets |
| [`effect/`](effect/) | Effect drivers, pools, Majinai and effect lifetime |
| [`display/`](display/) | Display queue/sort, animation and matrix/colour services |
| [`message/`](message/) | Message resources, windows, fonts, icons and print events |
| [`audio/`](audio/) | Audio request and disc-cover state services |
| [`story/`](story/) | Story, lecture and harbor event bindings |
| [`file/`](file/) | DVD/file requests, cache and handle ownership |
| [`memory/`](memory/) | SDK/smart heaps, pools and buffer release |
| [`format/`](format/) | printf/scanf, decimal, power and numeric library services |
| [`binary/`](binary/) | Binary-artifact inventories and unaddressed-function register |

## Detailed documents and evidence boundaries

| Document | Concrete coverage | Still incomplete |
| --- | --- | --- |
| [Original Voyage sky light](display/OPENING_VOYAGE_SKY_LIGHT.md) | Actual named pointLight9 loader/intensity/no-falloff coefficients, S8/64 normals/inverse transpose, alpha byte producers/actual callback targets and authored day light-track publication | Native consumer validation, original GX/hardware numerical parity |
| [Native original prologue heading](display/NATIVE_OPENING_HEADING.md) | Original glyph/background palette, subtitle GPU cache/ring, source letter clock, real curtain/exit presentation acknowledgments | Projective rainbow TEV, original RNG/queue continuity, original/hardware raster parity |
| [Trio field-return handshake](battle/UNDERGROUND_TRIO_FIELD_RETURN_HANDSHAKE.md) | Exact sequence>30/callback teardown/retained fade presets, authored Goombella join, nested controls and original field wait/161-command continuation | Native BattleEnd/context release/appearance/party constructor/sequence/pouch-save lifetimes and original execution |
| [Ending map-heap free and remaining-unit teardown](battle/ENDING_HEAP_FREE_AND_REMAINING_UNIT_TEARDOWN.md) | Exact BattleFree/_mapFree header/coalescing/null behavior and later ordered BattleEnd64-unit/resources/free/global clear | Native map allocator/bootstrap/field constructors and transitive teardown/context lifetime |
| [Battle ending original map allocation](memory/BATTLE_END_MAP_ALLOCATION_BINDING.md) | Actual BattleAlloc8934,32-byte best-fit/split/fill/cache, nested400KiB arena/bootstrap and correct E alloc/free pairing | Native prior allocation history/arena/SDK/cache/adapted addresses and original execution |
| [Battle-ending fade10 texture consumer](display/BATTLE_END_FADE10_TEXTURE_CONSUMER.md) | Embedded1x1I4FF texel, bytealpha/truncation, exact608-square camera8 quad/material and endingNPC140 flag namespace | Native camera/clip/GX restoration and original raster comparison; other fade resources |
| [Underground trio ordinary EXP ending tail](battle/UNDERGROUND_TRIO_NO_LEVEL_UP_ENDING_TAIL.md) | Exact0c..1f timers, independent effect softdelete, actual body/voice gates, fade10 completion, Rule0 and ordered pouch/field export | Native effect/GX/fade/audio/field reconstruction cadence; level-up/zeroEXP/special branches |
| [EXP icon clock and original window frame](display/EXP_ICON_CLOCK_AND_TYPE0_WINDOW_FRAME.md) | Per-record404 creation/reset/six-call delay, same-pass outer cadence, original nine-slice36vertices/UV/material and72pxCMPR frame | Native icon inner draw, source camera/GX/palette scheduling and original raster comparison |
| [US PointGet layout and lifecycle](effect/POINT_GET_US_LAYOUT_AND_LIFECYCLE.md) | Original phrase/digit widths, cap, ordered shadow/foreground matrices/quads/UV, local effect timer/softdelete and shared assets | Native source camera/display-state/GX texture sampling, numeric/raster comparison and effect-pool scheduling |
| [Normal Jump used pose/facing/declaration/AC setup](battle/NORMAL_JUMP_USED_POSE_FACING_DECLARE_AND_AC_SETUP.md) | Exact AC setup reset scope and separate TimingA init, M_R_1/M_W_1 types, modes1/16 and facing fields, fresh protector scan, datakey7 and bounce/floor requests | Original manager/pad/event cadence, rendering and transitive motion/hit services |
| [Trio targets, turns and victory](battle/UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md) | Exact default masks/candidates/permutation, selectable spike/precheck1 versus later counter-1, geometry fields, source move counts and result1 producer | Actual menu/death/turn/end provider composition and original runtime |
| [Setup4 introduction, phase, stage and player commands](battle/UNDERGROUND_SETUP4_INTRO_PHASE_STAGE_AND_PLAYER_COMMANDS.md) | Definition/part offsets, source fresh IDs, concurrent entry and selector0 phase gate, stage resources/hashes and Jump/Hammer/Headbonk source power/action bindings | Used phase/target/stage producer execution, complete player callback clocks and runtime parity |
| [Native original Jump runtime](battle/NATIVE_ORIGINAL_JUMP_RUNTIME_BINDING.md) | Unchanged script through approach, first airborne hit and TimingA with source battle poses | Damage/reaction/counter/second-hit/turn/result and original cadence |
| [Native trio command/target binding](battle/NATIVE_TRIO_COMMAND_TARGET_BINDING.md) | Original weapon/part sampling, retained-pair refresh, native controls and dispatch tokens | Attack consumer, turn/death/reaction/result and original comparison |
| [Underground trio setup4 and return](battle/UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md) | Four-unit group with FranklyB4, unit stats/weapons/variant flags, init/firstturn/counter instruction graphs and exact message metadata | Native battle composition, impact/pad clocks and outcome routing/runtime |
| [Rogueport underground pipe route](overworld/ROGUEPORT_UNDERGROUND_PIPE_ROUTE.md) | Reciprocal records, exact case6/10 gates,60unit/20unit position math, both-party entry and Frankly outcallback | Contact producer, paper/camera/audio/runtime |
| [First underground arrival and Goomba trio](story/UNDERGROUND_FIRST_ARRIVAL_AND_GOOMBA_TRIO.md) | Full161command encounter/init/arrival, original NPC assets/visibility and ten message bindings including tik_04 resource | Actual setup4 battle backend, other map services and runtime parity |
| [SDK DVD error categorization/recovery](file/SDK_DVD_ERROR_CATEGORY_AND_RECOVERY_DISPATCH.md) | Complete category/getting-error bodies; retry/last-error/high-byte/cancel/resume/seek dispatch | Recovery children/providers/original IO/audio |
| [SDK DVD error/timeout/fatal bridge](file/SDK_DVD_ERROR_CALLBACK_TIMEOUT_AND_FATAL_BRIDGE.md) | Complete error callback/timeout/fatal wrapper; failed state/reentry/handler/resnapshot/cancel/ready | Error/reset/handler/providers/original IO/audio |
| [SDK DVD busy callback error/whole composition](file/SDK_DVD_BUSY_CALLBACK_ERROR_AND_WHOLE_COMPOSITION.md) | Complete error tail/all406-word callback; inquiry/fresh command/equality/cancel/error fetch | Error/recovery children/providers/original IO/audio |
| [SDK DVD busy callback success/audio](file/SDK_DVD_BUSY_CALLBACK_SUCCESS_COMPLETION_AND_AUDIO.md) | Complete success region; motor/cancel/DMA equality/immediate/audio/reissue/complete order | Error tail/whole callback/providers/original IO/audio |
| [SDK DVD busy callback stop/progress/cancel prefix](file/SDK_DVD_BUSY_CALLBACK_STOP_PROGRESS_AND_CANCEL_PREFIX.md) | Complete prefix/return ABI; exact16/stop/DMA/cancel callback/ready ordering | Whole success/error/children/providers/original IO |
| [SDK DVD busy dispatch/read chunks](file/SDK_DVD_STATE_BUSY_COMMAND_DISPATCH_AND_READ_CHUNKS.md) | Complete stateBusy/17-entry table; MMIO/read cap/zero callback/audio/optional ABI | Completion/low-level/providers/original IO/audio |
| [SDK DVD ready/resume/recovery alarm](file/SDK_DVD_STATE_READY_DISPATCH_RESUME_AND_ALARM.md) | Complete stateReady; check/pause/pop/fatal/resume/motor/dummy/callback/recursion/reset/alarm | Busy/low-level/callback/providers/original IO |
| [SDK DVD queue pop/check/dequeue/coverage](file/SDK_DVD_QUEUE_POP_CHECK_DEQUEUE_AND_LOCAL_COVERAGE.md) | Complete consumers/all5 local queue functions; split IRQ/fresh pop/post-restore clears/stale links | Command/cancel/providers/original IO |
| [SDK DVD async submission/queue append](file/SDK_DVD_ASYNC_SUBMISSION_AND_QUEUE_APPEND.md) | Complete submit/push/clear; command/cache/IRQ/state/ordered links/ready/raw return | Queue consumers/command engine/providers/original IO |
| [SDK DVD read bounds/wait/wake/close](file/SDK_DVD_READ_BOUNDS_WAIT_WAKE_AND_CLOSE.md) | Complete read/close/read-sync callback; bounds/async admission/state/IRQ/wait/cancel return | Command/cancel/thread/providers/original IO/audio |
| [DVD manager init/cancel/worker](file/DVD_MANAGER_INITIALIZATION_CANCELLATION_AND_WORKER.md) | Complete Init/Delete/proc_main; queue/thread publication/failure spin/cancel/main-yield | SDK/providers/original lifecycle/IO/audio |
| [DVD manager sorted service/results/close](file/DVD_MANAGER_SORTED_SERVICE_RESULTS_AND_CLOSE.md) | Complete main/DVD comparator; snapshot/cap/result/requested advancement/callback/close/scan | Initialization/SDK/providers/original IO/audio |
| [Native frontend PCM refill](audio/NATIVE_FRONTEND_PCM_REFILL.md) | Independent refill worker, initial envelope mix, retained VRAM Title owners and original book loop selection | Physical audio/profile and full sound-engine parity |
| [Stream DPCM markers/predictor/boundaries](audio/GAME_STREAM_DPCM_MARKERS_PREDICTOR_AND_BOUNDARIES.md) | Complete decoder/16-scale table; marker/nibble/saturation/count/buffer/loop/return | Providers/deferred manager/original decoded audio |
| [Stream producer PCM/whole composition](audio/GAME_STREAM_PRODUCER_PCM_AND_WHOLE_COMPOSITION.md) | Complete PCM/all521-word callback; per-copy count/boundary-before-position/retry/early stop | Decoder/providers/original refill/audio |
| [Stream producer live encoded/decode calls](audio/GAME_STREAM_PRODUCER_LIVE_ENCODED_COPY_AND_DECODE_CALLS.md) | Complete region; tag/decode-call protocol/rounded byte copy/boundary/stop/return | PCM/helper/whole callback/providers/original audio |
| [Stream producer dispatch/silent-fill prefix](audio/GAME_STREAM_PRODUCER_DISPATCH_AND_SILENT_FILL_PREFIX.md) | Complete prefix/return ABI; low8/all-zero/gates/PCM count/ADPCM block zeroing | Whole live refill/helpers/providers/original audio |
| [Stream header allocation/cache/composition](audio/GAME_STREAM_HEADER_ALLOCATION_CACHE_AND_COMPOSITION.md) | Complete tail/whole627-word callback; zeroing/raw allocation/fade/mix/cache order | _sscallback/manager/providers/original startup/audio |
| [Stream header initial reads/pending](audio/GAME_STREAM_HEADER_INITIAL_READS_AND_PENDING_PUBLICATION.md) | Complete initial-read region; storage gate/chunk/fresh type/request-before-count/pending | Whole allocation/cache continuation/providers/original IO/audio |
| [Stream header match/fields/format prefix](audio/GAME_STREAM_HEADER_MATCH_FIELDS_AND_FORMAT_PREFIX.md) | Complete prefix/exit ABI; unmatched third row/field narrowing/sentinel/format/copies/reset | Whole callback continuation/providers/original header playback |
| [Game stream play wrappers/slots/header request](audio/GAME_STREAM_PLAY_WRAPPERS_REPLACE_SLOT_AND_HEADER_READ.md) | Complete PlayCh/Ex/root leaf; replacement/slot/defaults/path/two opens/header dispatch | Header callback/providers/manager/original playback |
| [Game stream main ordered fades/stops](audio/GAME_STREAM_MAIN_ORDERED_FADES_AND_INLINE_STOPS.md) | Complete SoundSSMain; three rows/stop priority/sequential40-20-10/three mix/two stop blocks | Providers/FP domains and original cadence/fade/stop/audio |
| [Game stream cache callbacks/initial reads](audio/GAME_STREAM_CACHE_CALLBACK_TRANSFER_AND_INITIAL_READS.md) | Complete cache-next/cache-ARAM/length leaf; fixed transfer/owner/progress/next-read/reset/initial IO order | Providers/SDK domains and original cache/DMA/startup/audio |
| [Game stream completion match/cursor/startup](audio/GAME_STREAM_COMPLETION_MATCH_CURSOR_AND_STARTUP.md) | Complete active-check callback; count/terminal flags/latch/readiness/activation/mix/flag8/counter order | Providers/SDK/FP domains and original callback/IRQ/audio |
| [Game stream interrupt refill/copy/latch](audio/GAME_STREAM_INTERRUPT_REFILL_COPY_AND_LATCH.md) | Complete SoundSSMainInt/DVDMgrReadAsync/cache_flush; chunk/IO/pending/copy/latch order | Callback/manager/providers/SDK domains and original IRQ/IO/audio |
| [Game stream volume cache/requests](audio/GAME_STREAM_VOLUME_CACHE_AND_REQUESTS.md) | Complete setter; low8/float10-before-floatc/recaptured pool/inline pan and dual requests | Providers/FP domains and original volume/audio |
| [Game stream pan/surround cache/requests](audio/GAME_STREAM_PAN_SURROUND_CACHE_AND_REQUESTS.md) | Complete two setters; low8 cache/raw mono pan/63 divisor/dual requests/pool recapture | Providers/FP domains and original pan/surround/audio |
| [Game stream frequency cache/requests](audio/GAME_STREAM_FREQUENCY_CACHE_AND_REQUESTS.md) | Complete halfword cache/fresh flag8/shared-counter/dual-ID request wrapper | Engine/providers/IRQ domains and original frequency/audio |
| [Game stream fade/stop/deferred close](audio/GAME_STREAM_FADE_STOP_AND_DEFERRED_CLOSE.md) | Four complete fade/time/stop/DVD-close request bodies | Consumers/GP/counter/IO/provider/FP domains and original audio |
| [Game stream mute/check/volume/continue](audio/GAME_STREAM_MUTE_CHECK_VOLUME_AND_CONTINUE.md) | Five complete SoundSS flag/query/volume/continue leaves | Engine/writers/callbacks/provider/FP domains and original audio |
| [Stream mix/local module inventory](audio/MUSYX_STREAM_MIX_AND_LOCAL_MODULE_COVERAGE.md) | Complete mix body and audited17 tagged stream.c local functions | Providers/IRQ/FP/chain/transitive ARAM/DSP and original audio |
| [Stream frequency/companion chain](audio/MUSYX_STREAM_FREQUENCY_AND_COMPANION_CHAIN.md) | Complete frequency write/state2 pitch/four-stage recursion/IRQ body | Mix/providers/FP/chain/IRQ domains and original audio |
| [Stream free/reverse state clear](audio/MUSYX_STREAM_FREE_AND_REVERSE_STATE_CLEAR.md) | Complete five-stage deactivate/buffer-exit/companion/recursive/reverse-clear body | Frequency/mix/resource/chain/IRQ domains and original audio |
| [Stream deactivation/companion chain](audio/MUSYX_STREAM_DEACTIVATION_AND_COMPANION_CHAIN.md) | Complete five-stage lookup/unblock/state3/recursion/IRQ body | Free/lifecycle/chain/IRQ/provider domains and original audio |
| [Stream activation companions/composition](audio/MUSYX_STREAM_ACTIVATION_COMPANIONS_AND_COMPOSITION.md) | Complete companion stages/recursive request/unwinds and composed whole local activation | Chain/IRQ/provider domains and original activation/audio |
| [Stream activation primary admission](audio/MUSYX_STREAM_ACTIVATION_PRIMARY_ADMISSION.md) | Complete primary lookup/voice/state/terminal-return regions and ABI | Companion continuation/providers/IRQ/domains and original audio |
| [Stream allocation/length publication](audio/MUSYX_STREAM_ALLOCATION_AND_LENGTH_PUBLICATION.md) | Complete AllocEx/AllocLength/ID/record/buffer/deferred activation/failure bodies | Activation/lifecycle/providers/domains and original audio |
| [Stream footer/whole-body composition](audio/MUSYX_STREAM_LOOP_FOOTER_AND_COMPOSITION.md) | Complete footer/loop-PS/ABI helpers and composed local streamHandle | Lifecycle/providers/domains/transitive ARAM/DSP and original audio |
| [Stream nonzero-position wrapped refill](audio/MUSYX_STREAM_NONZERO_POSITION_WRAPPED_REFILL.md) | Complete two-span callback/strict split/eight upload/publication paths | Footer/ABI/providers/domains and original audio |
| [Stream zero-position wrapped refill](audio/MUSYX_STREAM_ZERO_POSITION_WRAPPED_REFILL.md) | Complete zero-position callbacks/modulo/four flush/publication branches | Nonzero wrap/footer/ABI/providers/domains and original audio |
| [Stream position/forward refill](audio/MUSYX_STREAM_POSITION_AND_FORWARD_REFILL.md) | Complete position normalization/forward callbacks/modulo/flush publication region | Wrapped refill/footer/ABI/providers/domains and original audio |
| [Stream throttle/dispatch/startup](audio/MUSYX_STREAM_THROTTLE_DISPATCH_AND_STARTUP.md) | Complete throttle/state1 descriptor/hardware/start/flush and row tail regions | Refill/ABI/providers/domains and original stream/audio |
| [Game/stream output-mode propagation](audio/MUSYX_STREAM_OUTPUT_MODE_PROPAGATION.md) | Complete game/public/stream mode and HRTF-disable bodies | UI/providers/IRQ/domains/transitive DSP and original audio |
| [Stream lookup/output mode/mix](audio/MUSYX_STREAM_ID_LOOKUP_OUTPUT_MODE_AND_MIX.md) | Five complete public-ID lookup/mode/volume/hardware-mix bodies | Stream engine/lifecycle/providers/domains and original audio |
| [Low-precision final pitch/composition](audio/MUSYX_LOW_PRECISION_FINAL_PITCH_AND_COMPOSITION.md) | Complete final output/requeue/ABI helpers and composed whole local handler | Providers/domains/transitive consumers and original audio |
| [Low-precision portamento](audio/MUSYX_LOW_PRECISION_PORTAMENTO_TRANSITIONS_AND_INTERPOLATION.md) | Complete controller/cache/flag transitions and wrapped interpolation | Final pitch/requeue/providers/domains and original audio |
| [Low-precision pitch preparation](audio/MUSYX_LOW_PRECISION_PITCH_PREPARATION.md) | Complete note/cents/cached-query-skip bend/modulation region | Portamento/final pitch/requeue/providers/domains and original audio |
| [Integer sine lookup](audio/MUSYX_SINE_QUARTER_WAVE_LOOKUP.md) | Complete sndSin/original1024 numeric coefficients/two oscillator bindings | Runtime table owners/caller domains and original audio |
| [Low-precision modulation/pan ramps](audio/MUSYX_LOW_PRECISION_MODULATION_AND_PAN_RAMPS.md) | Complete oscillator/periodic/pan ramp region | Sine/remaining handler/providers/domains and original audio |
| [Zero-offset filter/requeue/composition](audio/MUSYX_ZERO_OFFSET_FILTER_REQUEUE_AND_COMPOSITION.md) | Complete finish and composed whole local ZeroOffsetHandler body | Providers/domains/transitive consumers and original voice/audio |
| [Zero-offset volume/aux output](audio/MUSYX_ZERO_OFFSET_VOLUME_AND_AUX_OUTPUT.md) | Complete dirty admission/main-aux arithmetic/snapshot/hardware request region | Filter/requeue/epilogue, providers/domains and original audio |
| [Zero-offset pan/surround](audio/MUSYX_ZERO_OFFSET_PAN_AND_SURROUND.md) | Complete mode/dirty/request/wrapped clamp region | Volume/aux/filter/output/requeue, providers/domains and original audio |
| [Zero-offset gain/attenuation](audio/MUSYX_ZERO_OFFSET_GAIN_AND_ATTENUATION.md) | Complete live fader/track/base gain and modulation/tremolo attenuation region | Pan/aux/output/requeue, providers/domains and original audio |
| [Zero-offset admission/ramp](audio/MUSYX_ZERO_OFFSET_ADMISSION_AND_RAMP.md) | Complete admission/timestamp/wrapped ramp region | Remaining gain/controller/output/requeue, providers/domains and original audio |
| [Cache syscall vector/installation](audio/GEKKO_CACHE_SYSCALL_VECTOR_AND_INSTALLATION.md) | Complete vector/installer/IC range/memcpy bodies and direct OSInit call binding | Boot/vector residency/exception/SPR/cache domains and original syscall/DSP/audio |
| [Cache range services/audio source corrections](audio/GEKKO_CACHE_RANGE_SERVICES_AND_AUDIO_CALL_CORRECTIONS.md) | Five complete range/count/opcode/sc bodies; six contracts/four child pins corrected against US targets | Syscall/hardware/address/IRQ domains and original cache/DSP/audio |
| [MusyX DSP startup endpoints/finish and composed builder](audio/MUSYX_DSP_STARTUP_ENDPOINTS_AND_COMMON_FINISH.md) | Complete startup loop/fallback/seed/active publication; audited complete local builder region coverage | Providers/domains/transitive cache/DSP/commands and original audio/native binding |
| [MusyX DSP startup callback/ITD/format prefix](audio/MUSYX_DSP_STARTUP_CALLBACK_ITD_AND_FORMAT_PREFIX.md) | Complete buffer handshake/ITD/descriptor/direct format dispatch and cursor publication | Loop/endpoints/startup finish, type/descriptor/alias domains and original sample/audio |
| [MusyX existing voice endpoints/completion](audio/MUSYX_EXISTING_VOICE_BUFFER_ENDPOINTS_AND_COMPLETION.md) | Complete type4/5/6 field/pending endpoint and unsigned completion/callback region | Startup/provider/encoding/alias domains and original sample/DSP/audio |
| [MusyX DSP budget/rejection/voice chain](audio/MUSYX_DSP_BUDGET_REJECTION_AND_VOICE_CHAIN.md) | Complete initial/cost/table/unsigned rejection cascade/PB chain/command03 gate regions | Startup/sample/provider/index/alias domains and original performance/DSP/audio |
| [MusyX DSP studio payloads/fade bindings](audio/MUSYX_DSP_STUDIO_COMMAND_PAYLOADS_AND_FADE_BINDINGS.md) | Ten complete input/aux/mode/surround/compressor/chain/fade/loop regions | Remaining voice/budget/admission, pointer/index domains and original DSP/audio |
| [MusyX DSP command rollover/finalization](audio/MUSYX_DSP_COMMAND_ROLLOVER_AND_FINALIZATION.md) | Initialization/twelve rollover blocks/deferred link-size/flush/final command publication | Other payload/admission/budget/cache/DSP domains and original command/audio |
| [MusyX DSP voice sorting/collection](audio/MUSYX_DSP_VOICE_SORT_AND_COLLECTION.md) | Complete unsigned pointer partition sorter/list collector/reverse iteration | Priority/list/index/alias domains, remaining budget/admission and original DSP/audio |
| [MusyX depop/fade/studio prepass](audio/MUSYX_DEPOP_ACCUMULATION_FADE_AND_STUDIO_PREPASS.md) | Complete wrapped accumulation/clamp/fade bodies and captured-versus-fresh successor list prepass | Fade/DSP consumers/list providers/domains and original depop/audio |
| [MusyX paired cosine/square-root helpers](audio/MUSYX_PAIRED_COSINE_AND_SQRT_HELPERS.md) | Complete paired reduction/fused polynomial and estimate-before-zero/three-double-refinement bodies | GQR/FPSCR/estimate/caller domains and original math/filter/audio |
| [MusyX filter coefficients/DSP parameters](audio/MUSYX_FILTER_COEFFICIENTS_AND_DSP_PARAMETERS.md) | Complete filter setter/coefficient wrapper and five parameter-copy gates | Math/FP/caller domains, surrounding DSP commands and original filter/audio |
| [MusyX DSP volume-ramp consumers](audio/MUSYX_DSP_VOLUME_RAMP_CONSUMERS.md) | Complete three-triplet prepare/correct/mask region including studio-mode cross-triplet paths | Surrounding builder/providers/domains, command execution and original DSP/audio |
| [Original IEEE remainder used by MusyX](audio/IEEE_REMAINDER_USED_BY_MUSYX.md) | Complete exceptional gate, finite normalization/subtract/shift/reconstruction and signed-zero body | FPSCR/NaN/trap/runtime domains and original mixer/audio |
| [MusyX volume interpolation/output branches](audio/MUSYX_VOLUME_INTERPOLATION_AND_OUTPUT_BRANCHES.md) | Both output regions compose the complete local mixer; exact single interpolation/table pairing/nine-store order | IEEE math/FP/index/alias/runtime domains, DSP consumers and original audio |
| [MusyX volume coordinate/phase preparation](audio/MUSYX_VOLUME_COORDINATE_AND_PHASE_PREPARATION.md) | Complete mixer table/sentinel/unsigned coordinate/compression/phase region and remainder wrapper | Interpolation/output branches, IEEE math/domains and original mixer/audio |
| [MusyX hardware pitch/volume/DSP ramps](audio/MUSYX_HARDWARE_PITCH_VOLUME_AND_DSP_RAMPS.md) | Four complete duplicate/dirty publication and signed ramp/residual correction bodies | Mixer/ramp callers/consumers, valid FP/index/alias domains and original audio |
| [MusyX DSP initialization/activation](audio/MUSYX_DSP_INITIALIZATION_AND_ACTIVATION.md) | Six complete allocation/record/studio/HRTF/list/start/key-off bodies | Allocator/cache/DSP children, domains and original init/start/audio |
| [MusyX DSP ADSR setup/command loop](audio/MUSYX_DSP_ADSR_SETUP_AND_COMMAND_LOOP.md) | Four setup/start/update-command regions and complete host-position/message/deactivation leaves | Other DSP builder/producers/consumers/domains and original cadence/audio |
| [MusyX ADSR low-precision caller/census](audio/MUSYX_ADSR_LOW_PRECISION_CALLERS_AND_DOL_CENSUS.md) | Four admission/clock/envelope/pitch/dirty regions and fourteen direct DOL calls to six ADSR services | Remaining precision/DSP regions, indirect callers/domains and original cadence/audio |
| [MusyX ADSR advancement/wrapper](audio/MUSYX_ADSR_ADVANCEMENT_AND_LOW_PRECISION.md) | Complete old-level publication, two-domain advancement, count wrap/transition and fifteen-call wrapper | Caller cadence/value/index/alias domains, consumers and original envelope/audio |
| [MusyX ADSR transitions/release](audio/MUSYX_ADSR_STATE_TRANSITIONS_AND_RELEASE.md) | Complete two-type zero-stage fallthrough/count/domain/step/table transitions and explicit/stored release conversion | Advancement/caller/value/table/float domains, consumers and original envelope/audio |
| [MusyX resource/pitch ADSR commands](audio/MUSYX_RESOURCE_AND_PITCH_ADSR_COMMANDS.md) | Complete LE layouts/sentinel FP coefficients, signed pitch adjustment, voice envelope/setup wrapper and opcodes12/32 | State engine/pow, actual resource/index/FP/alias domains and original ADSR/pitch/audio |
| [MusyX controller ADSR/hardware layouts](audio/MUSYX_CONTROLLER_ADSR_AND_HARDWARE_LAYOUTS.md) | Complete four-query descriptor construction, three hardware layouts/unsupported flag and time-cent conversion wrapper | Pow/transitive math, actual controller/table/descriptor/index/float domains and original ADSR/audio |
| [MusyX macro pan/volume/envelope commands](audio/MUSYX_MACRO_PAN_VOLUME_AND_ENVELOPE_COMMANDS.md) | Complete pan raw/converted duration,127-versus128 scaling, curve lookup/interpolation and envelope/fade signed step/stores | Actual command/rate/curve/divisor/value/alias domains and mix/DSP/original audio consumers |
| [MusyX random notes and pitch lookup](audio/MUSYX_RANDOM_NOTE_AND_PITCH_LOOKUP.md) | Complete randomized interval/cents/RNG/shared rewrite/wait, single-precision pitch tables/unsigned conversion and one-step helper | Actual seed/command/note/rate/table/float domains, consumers and original random/pitch/audio |
| [MusyX vibrato/portamento initialization](audio/MUSYX_MACRO_VIBRATO_PORTAMENTO_AND_INITIALIZATION.md) | Complete signed-byte vibrato/converted duration/phase, portamento modes/controller gates and initializer flag/state stores | Actual rate/amplitude/flags/command/shared-cell domains, consumers and original modulation/audio |
| [MusyX hardware playback/pitch conversion](audio/MUSYX_HARDWARE_SAMPLE_PLAYBACK_AND_PITCH.md) | Complete inclusive slot reset/full descriptor/padding copy, SRC/filter/ITD leaves and unsigned/table pitch arithmetic | Actual slot/descriptor/pitch/table/divisor/alias domains and DSP/original audio consumers |
| [MusyX macro sample start/resource lookup](audio/MUSYX_MACRO_SAMPLE_START_AND_RESOURCE_LOOKUP.md) | Complete referenced-row lookup/descriptor output and opcode16 offset/hardware/pitch/job request/publication order | Hardware/pitch children, residency/descriptor/shared-cell/domain and original sample/audio |
| [MusyX macro subroutines/note commands](audio/MUSYX_MACRO_SUBROUTINES_AND_NOTE_COMMANDS.md) | Complete gosub/return ring and note/add-key/last-started predicate/wait registration | Actual ring/note/table/command/caller domains and original subroutine/note/audio |
| [MusyX external key-off/pedal/event traps](audio/MUSYX_MACRO_KEYOFF_PEDAL_AND_EVENT_TRAPS.md) | Complete external low-flag/defer/event/wakeup, full-word pedal/release clears and opcodes40/41 trap install/clear | Actual flag/event/index/command/caller/shared-cell domains and original pedal/audio |
| [MusyX macro messages/VID/key groups](audio/MUSYX_MACRO_MESSAGES_VID_AND_KEY_GROUP.md) | Complete send/callback/macro-versus-VID routes, four-entry enqueue/pop/trap activation, full-ID copy and key-group requests | External key-off/callback provider, queue/trap/ID/group/caller domains and original message/audio |
| [MusyX RPN writers and voice propagation](audio/MUSYX_RPN_PARAMETER_WRITERS_AND_VOICE_PROPAGATION.md) | Complete selector/high-byte mask/byte bounds/full-word equality guards and distinct captured/fresh voice propagation | Actual selector/default/caller/table/value/alias domains and original controller/audio |
| [MusyX MIDI controller setters/notification](audio/MUSYX_MIDI_CONTROLLER_SETTERS_AND_NOTIFICATION.md) | Complete byte/14-bit store/pair order, special RPN/default propagation and matching voice/key-job/aux dirty publication | RPN children, caller/table/count/job domains and original pair/pedal/audio |
| [MusyX macro variable calculation/compare/set](audio/MUSYX_MACRO_VARIABLE_CALCULATION_COMPARE_AND_SET.md) | Complete opcodes96..101/112..113, S16 arithmetic/saturation versus signed32 comparison/PC and external setter request order | Downstream MIDI setter, commands/incoming register/reachability/shared-cell domains and original execution/audio |
| [MusyX macro metric/priority and inline state](audio/MUSYX_MACRO_METRIC_PRIORITY_AND_INLINE_STATE.md) | Complete opcodes48..57 metric arithmetic/hardware publication, priority/variable/aux/tremolo/flag/rate writes | Actual command/field/table/ID/shared-cell domains, remaining opcodes and original allocation/audio |
| [MusyX VID lifecycle and last-started reset](audio/MUSYX_VID_LIFECYCLE_AND_LAST_STARTED_RESET.md) | Complete112-node init, chain detach/promotion/terminal node recycle, root/public lookup and conditional table reset | Actual chain/node/caller/identity/member/null/alias domains and original lifetime/audio |
| [MusyX voice block/unblock and allocation peek](audio/MUSYX_VOICE_BLOCK_UNBLOCK_AND_ALLOCATION_PEEK.md) | Complete FX reservation accounting/protection/synthetic identity, raw unblock recycle and peek output admission | VID removal/callers/raw ordinal/identity/accounting domains and original reservation/audio |
| [MusyX priority-list and voice-free writers](audio/MUSYX_PRIORITY_LIST_AND_VOICE_FREE_WRITERS.md) | Complete priority unlink/prepend/sort insertion, free append/accounting and hardware priority leaf | Actual caller/identity/metric/list domains, hardware consumers and original allocation/free/audio |
| [MusyX allocation-aid initialization](audio/MUSYX_ALLOCATION_AID_INITIALIZATION.md) | Complete free-link/endpoints, priority membership/head and ordered counter/root reset | Actual count/caller/writer domains, priority/free lifecycle and original initialization/audio |
| [MusyX voice selection/allocation bookkeeping](audio/MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md) | Complete limit/group-count/priority/free/eligibility selection and free-record/wrapping counter updates | Aid/priority/free writers, actual argument/limit/metric domains and original allocation/audio |
| [MusyX macro start/priority/VID constructor](audio/MUSYX_MACRO_START_PRIORITY_AND_VID_CONSTRUCTION.md) | Complete allocation request/state cleanup/pending ID/publication/failure and public/internal VID construction | Allocator/priority/resource/free providers, lifecycle/domain/rollback and original activation/audio |
| [MusyX macro spawn/links/FX clone](audio/MUSYX_MACRO_SPAWN_LINKS_AND_FX_CONTROLLER_CLONE.md) | Complete raw spawn arguments, suppression/result/public-ID/chain publication and limited FX clone | macStart/allocation/VID providers, lifetime/invalidation domains and original spawn/audio |
| [MusyX macro loop/jumps/lookup](audio/MUSYX_MACRO_LOOP_JUMP_AND_REGISTRY_LOOKUP.md) | Complete shared-counter loop, macro/binary lookup and termination/conditional/direct jump regions | Registry/resource/free providers, command/count/PC domains and original control-flow/audio |
| [MusyX macro wait/queue/time conversion](audio/MUSYX_MACRO_WAIT_QUEUE_AND_TIME_CONVERSION.md) | Complete wait flags/random/sentinel/base/deadline, equal-before queue insertion and conversion helpers/stubs | Wait/seed/rate/clock/list writers, valid time/member domains and original execution/audio |
| [MusyX active macro init/last-started](audio/MUSYX_MACRO_ACTIVE_INITIALIZATION_AND_LAST_STARTED.md) | Complete admission/pending copies/note-default import/reset/clock/job-start region and last-started setter/init | Other opcodes, pending/ID/flag/clock writers, table domains and original initialization/audio |
| [MusyX macro scheduler/scoped dispatch](audio/MUSYX_MACRO_SCHEDULER_AND_SCOPED_DISPATCH.md) | Complete wait/active scans and clock tail; bounded fetch/PC/selector opcode/join regions | Other initialization/opcodes, command/list/clock writers and original wait/event/audio traces |
| [MusyX macro selectors/first-use/dirty](audio/MUSYX_MACRO_INPUT_SELECTORS_FIRST_USE_AND_DIRTY.md) | Complete15 voice/two aux macro selectors, conditional mode0 construction and dirty setter/tables | Interpreter/flag lifecycle/shared ownership, global first-row/r28 domains and original audio |
| [MusyX descriptor builder/FX copy/translation](audio/MUSYX_INPUT_DESCRIPTOR_BUILDER_FX_COPY_AND_TRANSLATION.md) | Complete reset/count-before-row publication, translation and ordered raw FX byte copying | Actual callers/dirty invalidation, first-row/r28/FX bounds and original build-copy/audio |
| [MusyX matrix wrappers/dirty/cache/external](audio/MUSYX_MATRIX_WRAPPERS_DIRTY_CACHE_AND_EXTERNAL_CTRL.md) | Complete14 voice/two aux cache wrappers, masks and external-controller dispatch/fallback | Descriptor/dirty/caller writers, incoming-r28/bounds domains and original recompute/audio |
| [MusyX input matrix/variable domain](audio/MUSYX_INPUT_MATRIX_EVALUATOR_AND_VARIABLE_DOMAIN.md) | Complete source routing, wrapped coefficient/combine modes, cache and variable reads; incoming-r28 precondition | Wrapper/descriptor/clock/variable writers, first-mode/r28 domains and original matrix/audio traces |
| [MusyX controller/default/last-note reads](audio/MUSYX_CONTROLLER_READS_DEFAULT_POINTER_AND_LAST_NOTE.md) | Complete controller range/OR/threshold/constant-zero reads, packed default pointer and last-note getter | Actual callers/matrix/filter/dirty consumers, mutation/bounds domains and original audio |
| [MusyX controller init/reset/defaults](audio/MUSYX_INPUT_INITIALIZATION_CONTROLLER_RESET_AND_DEFAULTS.md) | Complete global/per-voice writes, cold/warm controller tables, last-note and packed channel-default reset | Actual callers, controller/filter/dirty consumers and valid bounds/alias/runtime domains |
| [MusyX virtual table provider/DOL census](audio/MUSYX_VIRTUAL_TABLE_PROVIDER_AND_DOL_ENTRY_CENSUS.md) | Known configuration writers,28 bounded literal-address candidates and seven exact DOL entry calls | Indirect/REL/alias provider and callback closure, actual transfer contracts/runtime |
| [MusyX virtual sample refill](audio/MUSYX_VIRTUAL_SAMPLE_REFILL_DESCRIPTOR_AND_CURSOR.md) | Complete type5/6 one/two-segment descriptors, six tag1 callback sites and fresh cursor remainder | Actual callback/transfer implementation, field/result domains and original refill/audio traces |
| [MusyX virtual sample init/updates/position](audio/MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md) | Complete partial table reset,64-entry updater, identity/remaining/cleanup order and hardware position/state leaves | Refill child, actual table/callback/hardware providers and original underrun/playback traces |
| [MusyX sample identity/hardware fields](audio/MUSYX_SAMPLE_IDENTITY_AND_HARDWARE_FIELDS.md) | Complete active-only repeated sequence/note lookup, public-ID voice chains and hardware sample/loop fields | List/ID/sample field providers, backend consumers and original association/playback traces |
| [MusyX hardware messages/sample notifications](audio/MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md) | Complete four-message dispatch, virtual start/end allocation/generation/callback cleanup and macro sample-end wakeup | Table/callback/voice providers, hardware/ID children and original message/audio traces |
| [MusyX synthesis initialization](audio/MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md) | Complete voice/fader/job/aux initial fields, row-type setter, public volume and callback installation | Child services, other writers/caller bounds/reinit domains and original initialization/audio |
| [MusyX synth volume/fader producer](audio/MUSYX_SYNTH_VOLUME_SELECTORS_AND_FADE_PUBLICATION.md) | Complete selector routes, raw duration conversion, numeric literals and ordered zero/nonzero fade/bitmap publication | Fader initialization/writers, direct group bounds/domain and original duration/audio traces |
| [MusyX sequence speed/volume](audio/MUSYX_SEQUENCE_SPEED_VOLUME_AND_GROUP_FORWARDING.md) | Complete track speed/deferred writes, volume modes/64-entry group forwarding and public IRQ wrappers | synthVolume/deferred consumers, caller ID/group domains and original tempo/volume/audio |
| [MusyX sequence continue](audio/MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md) | Complete paused→active resume, tagged flag clear and public IRQ wrapper; exact unguarded missing-ID address | Caller lookup-success/private-ID domain, track/deferred consumers and original resume/audio traces |
| [MusyX sequence pause/mute](audio/MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md) | Complete active→paused state/note recycling, raw mute/tagged deferred stores and public mute IRQ wrapper | ID/state/list/note/deferred-field consumers/resume/domain and original audio |
| [MusyX synth fades/aux callbacks](audio/MUSYX_SYNTH_MASTER_PAUSE_FADES_AND_AUX_CALLBACKS.md) | Complete zero-offset bitmap fades/completion requests and8-studio four-parameter callbacks; local whole synthHandle composed | Coefficient/mode/providers/PPC/children/callback installation and original scheduling/audio |
| [MusyX job drain/voice controls/clock](audio/MUSYX_SYNTH_JOB_DRAIN_VOICE_CONTROLS_AND_CLOCK.md) | Complete scoped category0→2→1 drain, flags/pedal/dirty order and real-time tail; hwFrameDone no-op | Remaining synthHandle fade/aux middle, child handlers/providers/concurrency/original audio |
| [MusyX job insertion/start time](audio/MUSYX_SYNTH_JOB_INSERTION_AND_START_TIMESTAMPS.md) | Complete32-slot/category nodes, duplicate/unlink/prepend and ordered timestamp/two-job start | Queue initialization/consumers/type/member/time/IRQ and original scheduling/audio |
| [MusyX macro active/inactive/init](audio/MUSYX_MACRO_ACTIVE_INACTIVE_WAIT_AND_INIT.md) | Complete state/list transitions, two-word wait cleanup, exact flags/time reset and forced-job requests | Job/interpreter/wait/state/list/providers/IRQ and original macro/audio execution |
| [MusyX stream kill/init/hardware flags](audio/MUSYX_STREAM_KILL_INITIALIZATION_AND_HW_BREAK.md) | Complete64-record first-match kill/callback, voice-count prefix reset and DSP break/active record publication | Unblock/callback/state/count/DSP providers/consumers/reinit and original audio |
| [MusyX sound/voice kill recycling](audio/MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md) | Complete external chain identity/retained-next kill and internal priority/free-list/count recycle plus hardware requests | Map/macro/stream/hardware children, allocator/source/concurrency and original lifetime/audio |
| [MusyX sequence free-chain init](audio/MUSYX_SEQUENCE_INITIALIZATION_AND_FREE_CHAINS.md) | Complete8-instance/private-ID/MIDI-priority and256-note free-chain reset; exact cross-record writes | Constructors/payload/voice/resources/priority consumers/reset/runtime |
| [MusyX sequence validity/stop](audio/MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md) | Complete active/paused ID lookup, valid wrapper, tagged-field clear and state/note/instance stop recycling | Private-index/state/list/note writers, voice/resource lifetime and original audio |
| [MusyX listener/cross/inverse](audio/MUSYX_LISTENER_UPDATE_CROSS_AND_MATRIX_INVERSE.md) | Complete four-vector copies, cross/matrix stack layout, ordered inverse/translation and volume | Listener/basis/source/alias/singular/PPC/consumer domains and original audio |
| [Game effect/listener/completion main](audio/GAME_EFFECT_MAIN_LISTENER_EMITTER_AND_COMPLETION.md) | Complete listener/vector setup, emitter update/completion passes and outer SoundMain/Int order | Listener/providers/pool/sequence/SS/DVD/controller/voice/concurrency and original audio |
| [MusyX emitter update/3D reset](audio/MUSYX_EMITTER_VECTOR_VOLUME_UPDATE_AND_3D_RESET.md) | Complete two-vector/low-byte target/current-volume writes and ordered root/studio/flag reset | Construction/consumers/caller providers/alias/finite/reset domains and original audio |
| [MusyX emitter removal/unload](audio/MUSYX_EMITTER_REMOVE_ALL_AND_FX_LIFETIME.md) | Complete individual/all/FX-array unlink, high-half flag clear, key-off requests and9 direct callers | Emitter/list/FX/member/IRQ writers, key-off effects and original unload/audio |
| [Effect spatial controls/emitter lookup](audio/SOUND_EFFECT_SPATIAL_CONTROLS_AND_EMITTER_LOOKUP.md) | Complete pan/surround early publication, dual controllers, stored volume read and active/voice flag queries | Pool/emitter writers/ordinal/lifetime domains and actual controller/audio execution |
| [Positional SFX live/fade/mute](audio/POSITIONAL_SFX_LIVE_SPATIAL_FADE_AND_MUTE.md) | Complete remaining global/live regions; composed whole-local SFXMain graph, fresh controls/mute/fade/stop ordering | Backend/writers/flags/fade/domain/lifetime and original audio |
| [Game listener/camera bindings](audio/GAME_LISTENER_SETTER_AND_SEQUENCE_CAMERA_BINDINGS.md) | Complete setter/camera lookup; two scoped field/battle position/heading argument regions and direct census | Camera/flag/state writers, sequence gates/cadence, indirect routes and original audio |
| [Game calc3D/angle table](audio/GAME_CALC3D_SPATIAL_RESULT_AND_ANGLE_TABLE.md) | Complete position-flag precedence, table angle, pan/surround/distance/volume and retained packed result byte | Listener/shared writers, PPC/libm/finite domains, live SFX and original audio |
| [MusyX frame/aux callback order](audio/MUSYX_AUDIO_FRAME_AND_AUX_CALLBACK_ORDER.md) | Complete DSP-before-updates frame graph, five slices, live voice clears/aux callbacks and direct critical/time leaves | Child engines/providers/format/IRQ/reentrancy and original audio cadence |
| [MusyX DSP task/callback/mail](audio/MUSYX_DSP_TASK_CALLBACKS_AND_COMMAND_MAIL.md) | Complete task publication/init wait, three callbacks, command mail and raw clock arithmetic | DSP program/SDK/builder/state/thread/source/concurrency and original audio |
| [MusyX AI buffers/callback/IRQ init](audio/MUSYX_AI_BUFFERS_CALLBACK_AND_IRQ_INITIALIZATION.md) | Complete zeroed four-slot allocation, initial/rotating DMA requests, logic gates, CPU destination, IRQ init and compressor flag | Actual SDK/DSP/frame callback/writers/alias/concurrency/reinit and original audio timing |
| [Game allocator/MusyX bootstrap](audio/GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md) | Four complete provider/sndInit/hwInit bodies; original hook table and scoped SoundInit prefix | Whole game init/child hardware/synth/stream/source/failure/runtime |
| [MusyX cache/reset/hook producers](audio/MUSYX_DEFAULT_CACHE_RESOURCE_RESET_AND_HOOK_INSTALLATION.md) | Complete default stack/cache publication, six registry counts/macro table reset, allocator hooks and upload forwarding | Actual setup callers/providers, alias/reset/residency domains and original runtime |
| [MusyX ARAM upload/stream bridge](audio/MUSYX_ARAM_UPLOAD_AND_HARDWARE_STREAM_BRIDGE.md) | Complete raw generic upload, high/low queue and aligned DCStoreRange stream flush/init/exit/address;25 direct DOL callers | Stream caller/providers/extent/lifetime and SDK/cache/DMA/original execution |
| [MusyX ARAM initialization/hooks](audio/MUSYX_ARAM_INITIALIZATION_AND_ALLOCATOR_HOOKS.md) | Complete zero-buffer allocation/fill/post/wait/free, limits/reset ordering and indirect hooks | Hook/provider/SDK/queue reinit/concurrency and original DMA/runtime |
| [MusyX ARAM stream lists](audio/MUSYX_ARAM_STREAM_BUFFER_LISTS_AND_LIFETIME.md) | Complete64-descriptor init, cached/downward allocation, getter/default pointer and free traversal | List/predecessor/order/membership domains, cache producers and actual stream/voice lifetime |
| [MusyX ARAM sample store](audio/MUSYX_ARAM_SAMPLE_STORE_AND_CURSOR_REWIND.md) | Complete direct/chunk upload, capacity/32-byte alignment, cursor-only removal, callback configuration and address leaves | Initialization/cache/stream/source/caller order and SDK/DMA/original playback |
| [MusyX ARAM queue completion](audio/MUSYX_ARAM_QUEUE_COMPLETION_AND_SAMPLE_REQUESTS.md) | Complete callback and low-queue sync; sample producer post/count/index ordering | Other producers/index/count/concurrency invariants and SDK/hardware/runtime |
| [MusyX sample-directory lifetime](audio/MUSYX_SAMPLE_DIRECTORY_REGISTRATION_AND_REFERENCES.md) | Four complete registration/acquire/release/removal bodies; mutable refs/addresses and preferred-directory precondition | Actual directory/sample-bank/cache/voice providers, valid domains and original runtime |
| [MusyX sample hardware bridge](audio/MUSYX_SAMPLE_STORAGE_HARDWARE_BRIDGE.md) | Four complete save/remove/identity/sync bodies; encoded byte-count proof and1247 original records | ARAM/cache/DMA/allocation/decoder services and original playback |
| [MusyX group lifecycle/resource lists](audio/MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md) | Complete push/pop/InsertData stack and ordered typed/ranged requests;12 original groups/2998 IDs and Hammer dependencies | Child sample/pool/stop/memory/voice/source/runtime residency |
| [Game sound group loader](audio/GAME_SOUND_GROUP_WARMUP_AND_LOADER_REQUESTS.md) | Complete busy/index/ready warm-up, two bank-push/count modes and temporary-buffer free | Actual file/resource/init/bank/source/runtime residency and original traces |
| [MusyX FX registry insert/remove](audio/MUSYX_FX_GROUP_REGISTRY_INSERT_AND_REMOVE.md) | Complete duplicate/new registration,128 cap, references/live entry byte9=31 and ordered compaction | Project/group residency, sorting/pointer/unload/voice/IRQ domains and original runtime |
| [MusyX synth resource/control cores](audio/MUSYX_SYNTH_EFFECT_RESOURCE_AND_CONTROL_CORES.md) | Complete loaded-FX query/bsearch/comparator/start dispatch and controller/KeyOff voice-chain requests | Registry/voice/macro/MIDI producers and actual hardware/runtime; linked bodies do not imply completion |
| [MusyX effect API/voice/IRQ](audio/MUSYX_EFFECT_API_WRAPPERS_AND_VOICE_LOOKUP.md) | Eight complete public wrapper/voice-map/IRQ bodies, external-ID returns and separate nesting | Core synth/resource/map/default-table provisioning and OS/hardware/original playback |
| [SFX pending startup](audio/POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md) | Complete800da010..398 delay/start/three-backend request/aux/pending-clear and same-pass live entry | Full calc3D/live/fade/stream/song/MusyX/resource/runtime |
| [Effect pool PlayEx](audio/SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md) | Complete40-record first-free/priority-steal/start/interrupt/publish/fresh-pool/liveness body | Pool/other writers, core MusyX/emitter/resource/hardware/original execution |
| [Effect record controls](audio/SOUND_EFFECT_RECORD_CONTROL_LEAVES.md) | Complete Check/Volume/Pitch/Aux1/Stop ordinal→voice rules, controller widths/order and clear policy | Core voice/emitter/controller semantics and source/runtime lifetime |
| [Switch storage/encoding](memory/SW_SET.md) | Complete12-body access/init/reset, global256/local16-word flags, byte banks and full-word global index0; modulo32 correction | Card record/load/recovery, full story/namespace writers and original persistence traces |
| [Monosiri flag groups/storage](battle/MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md) | Complete group-aware persistent4474+kind/battle bitmap setter, policy/original-kind checker and48 static groups | Other writers/source domains, complete Tattle/log/AC/scene/card lifetime/runtime |
| [Monosiri table/consumers](battle/MONOSIRI_TABLE_SOURCE_AND_CONSUMERS.md) | Initialized231-row Tattle/message/model table, six field-use regions, complete message-key event leaf,17 DOL calls/2 REL imports | Table mutation/alias/kind domains, dynamic callers and full log/Tattle/AC/persistence/runtime |
| [Hammer charge cue resources](audio/HAMMER_CHARGE_CUE_RESOURCE_BINDINGS.md) | Two actual original callers and table→FX→group/macro→sample identities, rates/lengths/loop fields | Full command/display/macro/backend execution and original charge/audio traces |
| [SFX update/corrections](audio/POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md) | Complete psndMain cooldown owner; scoped SFXMain completion sweep/pending transition/preference tail; corrected source masks/ABI/dispatch | Full SFXMain middle/spatial/start/backend/resource and original execution |
| [Audio frame driver and SFX update](audio/AUDIO_FRAME_DRIVER_AND_SFX_UPDATE.md) | Complete psndMain (init_f gate, signed-byte cooldown decrement, fixed 4-service order) plus psndSFXMain structure: literal 40-slot loop bound, SoundSSCheck reaping, slot+0x06 clear with slot+0x0E countdown, live count to G5B, preference reset; records the clrlwi low-13 and counter-reload corrections | Per-channel dispatch conditions, calc3D, psndENV/BGM interaction, psndInit globals, original traces |
| [Small-data r2 base and float pools](binary/SMALL_DATA_R2_BASE_AND_FLOAT_POOLS.md) | r2 = 0x8042B038 verified by the pi/4 eight-direction anchor and a pi/radian audio pool; 46 field and 21 audio constants ISO-read and tabulated | Constants outside the two windows, pool contiguity beyond them, and the -23384 zero-divisor branch |
| [SFX backend dispatch](audio/POSITIONAL_SFX_BACKEND_DISPATCH.md) | Stream/song/effect selected by row.word1 bits 0-1; 3 stream channels with stale-sweep, 4 song channels, unpaged effects; resource ids word2 / word1&0xF / word1&0xFFFF; bit 30 of slot+0x06 gates calc3D | Writer of slot+0x06 bit 30 and the later per-frame volume/pan/pitch update half |
| [Positional attenuation and pan](audio/POSITIONAL_SFX_ATTENUATION_AND_PAN.md) | calc3D structure: flags bit7 add/bit6 replace listener, distance minus global+0x3C, three-iteration frsqrte Newton refinement with frsp, NaN at zero distance, pan window [64,196] to 0..127, and the PAN record at 0x8041EB18; all seven lfd/lfs constants ISO-read and named | Distance helper 0x800DCBC8, eight r2-relative scales, classifier case semantics, out0/out1 use |
| [DMG-01 initialization/count binding](battle/DMG_01_INITIALIZATION_AND_JUMP_COUNT_BINDINGS.md) | Full local Entry/UnitSetup composition, signed HP/FP copies, original Jump shared count reset/success stores/readout | Other counters/loadout/mutations, full consumer graph and original multihit traces |
| [EVT-01 service consolidation](event/EVT_01_SERVICE_CONTRACT_AND_REMAINING_BINDINGS.md) | Scheduler/constructor/wait composition, corrected leaf capture, conditional Jump root/registry and local menu writers | Original traces, query-specific cursor domains, dynamic/global/platform/numeric bindings |
| [Shape entry/defaults](display/ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md) | Complete two-body matrix/pass/cull/default request order | Texture/map/GX/SDK/source/lifetime/runtime |
| [Texture override scans](display/ANIMATION_RENDER_TEXTURE_OVERRIDE_SELECTION.md) | Complete reversed slot/index fallback/kind priority/null/alpha differences | Override/resource/texture object/coordinate/GX/source |
| [Last-stage/rebind](display/ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md) | Complete smart return nonuse/state/extra-stage/fresh rebind order | Smart texture/map TEV/GX/matrix/source/lifetime |
| [Arrays/vertex FIFO](display/ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md) | Complete captures/arrays/formats/low16 fan/index/multiple-UV emission | GX/cache/resource/numeric/original geometry |
| [Render source/boundaries](display/ANIMATION_RENDER_RESOURCE_EXAMPLE_AND_PORT_BOUNDARIES.md) | Original24 shapes/draws/150 kind0 descriptors/70 raw fans/three caller sites | Visibility/source/other resource paths/backend/original frames |
| [Joint node matrix](display/ANIMATION_JOINT_NODE_MATRICES.md) | Complete scale asymmetry/parent fallback/six rotations/16 routes | Node/source/stack/FPSCR/libm/render/runtime |
| [Transform node matrix](display/ANIMATION_TRANSFORM_NODE_MATRICES.md) | Complete primary-only rotation/pivot-offset gates/8 routes | Resource/source/numeric/alias/render/runtime |
| [Matrix stack/affine](display/ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md) | Complete push publication/Concat snapshot/FMA/copy/writer store order | GQR0/FPSCR/memory/stack/source/visual domains |
| [SDK axis rotation](display/ANIMATION_SDK_AXIS_ROTATION.md) | Complete trig wrapper order/case-fold/matrix layout/invalid-axis stores | Original sin/cos/GQR0/FPSCR/numeric domains |
| [Node layout and used bindings](display/ANIMATION_NODE_LAYOUT_AND_USED_BINDINGS.md) | Used four-site census/original13 joint29 transform metadata/native conversion request | Loaded source/lifetime/render/material/original visuals |
| [Draw wrapper/admission](display/ANIMATION_DRAW_WRAPPER_AND_ADMISSION.md) | Complete parent/link/filter/context/cleanup request order | Resource/link/inner backend/source/runtime |
| [Draw shared ring/paper selector](display/ANIMATION_DRAW_SHARED_BUFFERS_AND_PAPER_SELECTION.md) | Complete ring/oversize/index/stamp/reset/Main requests | Pool/clock/source/alias/lifetime |
| [Paper draw matrices](display/ANIMATION_DRAW_PAPER_MATRICES.md) | Complete prepass/override/world matrix requests | SDK alias/numeric/override consumers/source |
| [Inner GX/flush](display/ANIMATION_INNER_DRAW_STATE_AND_BUFFERS.md) | Complete admission/translucency/GX callback/flush/retained traversal requests | GX/render/material/cache/resource/runtime |
| [Inner facing](display/ANIMATION_INNER_DRAW_FACING.md) | Complete camera rereads/smoothing/rotation/reflection request order | Camera/libm/SDK/flags/numeric/visuals |
| [Group traversal](display/ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md) | Complete visibility/node push/cache/child/sibling/special/ordinary requests | Node matrix/render/material/Vivian/pool/lifetime/runtime |
| [Animation Main entry/time](display/ANIMATION_MAIN_ENTRY_AND_TIME.md) | Complete captures,linked recursion/GQR/null gate and time/span/loop stores | Resource/pool/clock/helper/platform/source/runtime |
| [Animation samples](display/ANIMATION_TIMELINE_SAMPLING.md) | Complete continuous/discrete scans,predecessors/fraction and original M_Z_1 metadata | Loaded-pointer/count/time domains and original traces |
| [Native pose sidecar](format/NATIVE_BATTLE_POSE_DIRECTORY.md) | Original descriptor identity, model flags and independent discrete timeline conversion | Native format; full Main/cache/backend ownership remains open |
| [Animation delta cache](display/ANIMATION_COMPRESSED_DELTA_CACHE.md) | Complete base/rebuild/forward/same-key and five compressed relative-index streams | Copy/alias/capacity/resource/FPSCR/consumers |
| [Ordinary Main completion audit](display/ORDINARY_BATTLE_POSE_MAIN_COMPLETION_AUDIT.md) | Complete scalar/store and engine-call census for fresh unlinked private-output poses | ARM numeric adapters, alternate/linked/facing ownership and queued camera4 draw |
| [Native cache and ordinary Main](display/NATIVE_ANIMATION_CACHE_AND_ORDINARY_MAIN.md) | Retained five-channel native replay/output and scoped Main completion | General linked/paper/facing, camera depth/queue and original numeric/hardware parity |
| [Battle pose camera4 depth](display/BATTLE_POSE_DEPTH_CAMERA4_MATRICES_AND_CALCZ.md) | Original view/homogeneous projection, depth priority and actual matrix writer chain | Battle-camera framing, live matrix ownership and queued draws |
| [Battle camera Init/default/victory](camera/BATTLE_CAMERA_INIT_DEFAULT_AND_VICTORY_MODE17.md) | Source Init, default14..16, victory17/18 and MoveTo/common publication | Other modes/shakes, camera entry/lens and original cadence |
| [Battle camera mode3 timeline](camera/BATTLE_CAMERA_MODE3_TIMELINE_AND_MOVETO_CAPTURE.md) | Captured MoveTo targets, decrement/sampling order and speed restoration | Pointer curves, remaining controller modes and actual frame history |
| [Battle camera add zoom/Y-axis](camera/BATTLE_CAMERA_ADD_ZOOM_AND_NOMOVE_Y_EVENT_ABI.md) | Exact operand/truncation/gate ABI and packed writes | Framing and exceptional integer conversion |
| [Native battle camera owner](camera/NATIVE_BATTLE_CAMERA_FRAME_OWNER.md) | Real request writes, retained supported-mode frame updates and SDK depth math | Complete camera4 history/matrices, remaining modes/shakes and queued callbacks |
| [Battle camera action modes](camera/BATTLE_CAMERA_ACTION_MODES_0_7_10_11_AND_FIELD_RESET.md) | Source0/7/10/11 formulas, verified constants and kind dimension writers | Remaining modes/shakes and inherited camera-extra history |
| [Inherited battle camera4 lens](camera/UNDERGROUND_BATTLE_CAMERA4_ENTRY_AND_INHERITED_LENS.md) | Boot Entry and mode2-only battle transition with retained lens/offset | Reachable field/event writer continuity and actual matrices |
| [Camera manager boot/update/draw](camera/CAMERA_MANAGER_BOOT_UPDATE_AND_DRAW_PUBLICATION.md) | All13 boot records, callback/LookAt order and ascending draw/current-index/reset | Full native bank, live callback history and GPU providers |
| [Native matrix publication and trio view](camera/NATIVE_CAMERA_MATRIX_PUBLICATION_AND_TRIO_VIEW.md) | Source roll/translation/extra and perspective ordering; live native trio camera | Numeric parity, inherited extras and complete queued GPU draw |
| [Native camera bank/draw owner](camera/NATIVE_CAMERA_BANK_AND_DEFERRED_DRAW_OWNER.md) | Retained13 records, resumable Main/Draw and finishing callback bridge | Actual deferred GPU/letterbox/shadow and field-return bindings |
| [Battle camera tremor tail](camera/BATTLE_CAMERA_TREMOR_TAIL_FLAGS_1_AND_2.md) | Complete independent flags1/2 interpolation/speed/phase/timer and baseline ordering | Native numeric/cadence and original trace fidelity |
| [Letterbox/dummy draw services](camera/CAMERA_LETTERBOX_AND_DUMMY_DRAW_SERVICES.md) | Exact automatic predicates/scalars, callbacks and genuine GX triangle/state sequence | Native retained globals, queued raster and GX/PICA adaptation |
| [Letterbox global producers](camera/CAMERA_LETTERBOX_GLOBAL_PRODUCERS.md) | Current/next sequence, GP4, distinct Key/Ctrl and sound side-effect producers | Native producer/history binding; trio sequence mismatch remains explicit |
| [Native letterbox scalar continuation](camera/NATIVE_LETTERBOX_SCALAR_CONTINUATION.md) | Source decisions/animation/registration with native slot0 and real Draw resume | Bero/current globals and actual viewport/callback/GPU completion |
| [BeroEXEC retained producers](overworld/BERO_EXEC_RETAINED_PRODUCERS_AND_PROLOGUE_TRANSITIONS.md) | Boot/reset and resolved-mask writers with30 source event requests | Original road/arrival/reset publication into shared native owner |
| [Native BeroEXEC owner/road adapter](overworld/NATIVE_BERO_EXEC_OWNER_AND_ROAD_ADAPTER.md) | Original resolved masks, explicit provenance and scoped road/letterbox binding; Key39 correction | Full room-info/child/history, actual current sequence and deferred GPU/field return |
| [Part Entry defaults](battle/TRIO_PART_ENTRY_SCALE_RADIUS_AND_ROTATION_DEFAULTS.md) | Original descriptor radius/height, unit scales and zero rotation/pivot stores | Complete unit/part factories and draw/shadow consumers |
| [Native display queue](display/NATIVE_DISPLAY_QUEUE_OWNER.md) | Actual borrowed records, source sort and resumable deferred dispatch | GX/PICA, camera/cadence, callback and userdata lifetime owners |
| [Native trio pose queue binding](display/NATIVE_TRIO_POSE_QUEUE_BINDING.md) | Explicit camera adapter, retained depth continuation, live part registration and shadow values | Actual camera4 history, shadow owner and deferred GPU draw/cadence |
| [Native shadow pool and live display](display/NATIVE_SHADOW_WORK_AND_TRIO_LIVE_DISPLAY.md) | Shared request owner, Main registration/reset and linked player traversal | Remaining unit constructors, shadow preparation, GPU and footer providers |
| [Surviving Frankly pose constructor](battle/SETUP4_SURVIVING_FRANKLY_BATTLE_POSE_CONSTRUCTOR.md) | Actual setup4 kind180, part/table/model/default and available private assets | Native original part binding and battle pose metadata sidecar |
| [Native Frankly bank and first footers](display/NATIVE_FRANKLY_BANK_AND_FIRST_DISPLAY_FOOTERS.md) | Original part/default and full17-pose bank; actual EXP/stage queue providers | Palette/audience/slot footer, GPU dispatch and original cadence/return |
| [Original display footer registrations](display/BATTLE_DISPLAY_FOOTER_REGISTRATION_AND_MUTATION.md) | Exact stage/command/audience/slot requests and palette/animation mutations | Native remaining mutation owners and deferred consumers |
| [Native display footer completion](display/NATIVE_DISPLAY_FOOTER_COMPLETION.md) | Retained palette, audience interpreter/gates, slot queue and Main result publication | Global camera/draw cadence, GPU consumers and ordered field exit |
| [Original shadow pool/cadence](display/BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md) | Exact request writes, deferred preparation lifetime and sound-tail admission | Collision/image/cylinder backend and nonempty sound tables |
| [Original live display loop/footer](display/BATTLE_DISPLAY_LIVE_PART_LOOP_UNIT_TAIL_AND_FOOTER.md) | Live links, automatic hidden extras, unit-tail gates and footer order | Remaining constructors and deferred callbacks/footer implementations |
| [Camera4 matrix producer arithmetic](display/BATTLE_CAMERA4_MATRIX_PUBLICATION_ARITHMETIC.md) | Original concat, roll, translation and perspective/orthographic instruction order | Full LookAt/libm/platform and persistent matrix publication |
| [Ordinary battle queue admission](display/ORDINARY_BATTLE_DISPLAY_QUEUE_ADMISSION.md) | Borrowed queue registration and distinct immediate/deferred providers | Actual native queue/GX/callback/cadence and userdata lifetime |
| [Animation interpolation](display/ANIMATION_OUTPUT_INTERPOLATION.md) | Complete linear/cubic/tangent guard/end-seam and final cache writes | Renderer/numeric/lifetime/original animation |
| [Animation facing](display/ANIMATION_MAIN_CAMERA_FACING.md) | Complete camera/atan2/two wraps/sign tail | Camera/libm/flags/renderer/runtime |
| [Mario post-Main draw queue](battle/ORDINARY_MARIO_POST_POSE_MAIN_ALPHA_AND_DRAW_QUEUE.md) | Original alpha products, model TestXLU, facing admission and draw gates | Camera4 view/projection and queued callback |
| [Animation initialization](display/ANIMATION_POOL_AND_TANGENT_INITIALIZATION.md) | Complete counts/allocation/partial clear/tan writer/file requests | Heap/file/copy/libm/reentry/lifetime |
| [Jump takeoff position](battle/BATTLE_JUMP_TAKEOFF_POSITION.md) | Complete mixed coordinate/sample/closest-X/side-offset/output contract and two actual callers | Candidate cores/RNG/domain/source geometry and original position traces |
| [MoveFrame/MovePosition](battle/BATTLE_MOVE_FRAME_AND_POSITION.md) | Complete count-query versus velocity-adjusted motion,first-call integration,XYZ snap/yield and actual callers | Numeric/source/sound/scheduler composition and original timing |
| [Unit/part position queries](battle/BATTLE_UNIT_PART_POSITION_QUERIES.md) | Complete home/offset/world/scaled hit leaves,registry-part helper and event integer/sentinel wrapper | Actual source/scale/world/lifetime writers and original damage/position traces |
| [Movement sound control](battle/BATTLE_MOVEMENT_SOUND_CONTROL.md) | Complete retained voice/position,alternating signed periods and stop/core wrappers | Sound trigger/playback/resources/source lifetime and original audio traces |
| [Interpolation/party leaves](event/EVENT_INTERPOLATION_AND_PARTY_RUN_STOP_LEAVES.md) | Complete evt_sub_intpl_init/get_value state layout and intplGetValue call shape, evt_party_run/stop index selection and null-skip | intplGetValue internals, caller census and cadence |
| [GOTO and loop opcodes](event/EVENT_GOTO_AND_LOOP_OPCODES.md) | Complete4..8/raw counters,lexical scans/stack updates and NEXT | Other opcodes,script/storage/numeric domains and original traces |
| [Assignment/arithmetic/data cursor](event/EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) | Complete50..64 read modes/order,MOD conversions and cursor publication | Other readers,numeric/alias/source domains and original traces |
| [Integer operand namespaces](event/EVENT_INTEGER_OPERAND_NAMESPACES.md) | Full signed tag routing,storage and integer read/write/return semantics | Storage/source/numeric domains,persistence and original traces |
| [Float operand routing](event/EVENT_FLOAT_OPERAND_ROUTING.md) | Full API-asymmetric bands,quantized words,flag truth and old-value return | Numeric/storage domains and original execution |
| [Saved switch leaves](event/EVENT_SAVED_SWITCH_STORAGE.md) | Complete RAM byte/flag leaves,GSW0 word exception and GP reload | Save/source/capacity lifecycle and original traces |
| [Event deletion and copy-back](event/EVENT_DELETE_AND_COPYBACK.md) | Complete child-first/brother scan,parent copy-back,clear/counter and DeleteID scan | Opcode/domain/source lifetime and original traces |
| [Stop/start traversal](event/EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md) | Complete descendant-first type masks,inlined context captures and All scans | Pause/UI/domain/source and original runtime traces |
| [Run and ID-output variants](event/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) | Complete92/93/108/111 copy/order/ownership and output semantics | Lifetime/control/domain and original traces |
| [Control opcodes and ID scans](event/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) | Child yield,95..106 wrappers,setters,other/ID scan and liveness policies | Full stop/start/delete traversal,other opcodes and original traces |
| [Child/brother/inline construction](event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) | Full copy/reset/ownership maps and append,plain107/110 wrappers | ID variants,remaining controls/domain and original traces |
| [Restart preservation](event/EVENT_RESTART_PRESERVATION.md) | Exact execution reset,label rebuild and preserved locals/links/identity | Caller/control/deletion domains and original restart traces |
| [Root constructors and append](event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) | Complete root/typed slot/init/ID/speed/credit and conditional live append | Child/brother/inline/restart/domain and original traces |
| [Label scan and entry run check](event/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) | Full root label scanner and all-event system-level stop dispatch | Other constructor/source/lifetime and original pause traces |
| [Complete event manager loop](event/EVENT_MANAGER_COMPLETE_LOOP.md) | Complete context/time delta, priority ordering, eligibility/identity and multicredit/return loop | Constructors/append/initial state,time sources and original traces |
| [Dispatch and wait continuations](event/EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) | Exact fetch/common results, frame/msec waits and USER_FUNC first-call ownership | Other opcodes,numeric/domain/source and original traces |
| [Attack badge recovery tail](battle/BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md) | Complete32-command HP/FP branch/brother/wait/deletion graph | Scheduling/delayed validity and original traces |
| [Badge recovery services](battle/BATTLE_BADGE_RECOVERY_SERVICES.md) | Amount/resource/geometry wrappers, raw actor and narrowing/bookkeeping semantics | Source/lifetime/domain and original traces |
| [Badge recovery presentation](battle/BATTLE_BADGE_RECOVERY_PRESENTATION.md) | Scoped quantized local floats/integer ADD and recovery catalog/request binding | Full namespaces/renderer/audio and original traces |
| [Weapon aftereffect](battle/BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md) | Complete weighted destruction and ordered forced/chance hazard dispatch, actual weapon bytes | Flag/seed/interleaving sources and executed hazards |
| [Stage request leaves](battle/BATTLE_STAGE_AFTEREFFECT_REQUESTS.md) | Complete destruction/nozzle/frame/object event ownership and local helpers | Queued events, source/config/lifetime domains and original traces |
| [Stylish timing/results](battle/BATTLE_STYLISH_TIMING_AND_RESULTS.md) | Complete shared counter, event first-call/polling, input bounds and result readers | Source/scheduler/collision ownership, audience and original traces |
| [Stylish timing icon](battle/BATTLE_STYLISH_TIMING_ICON.md) | Exact eligibility, baseline queue depth and visible-position icon draw requests | Assets/display lifetime/projection and original presentation |
| [Enemy candidate sampling](battle/BATTLE_ENEMY_CANDIDATE_SAMPLING.md) | Complete row geometry, admission/compaction/occlusion, count and permutation ordering | Source/helper/numeric/domain and original traces |
| [Candidate choice and RNG](battle/BATTLE_ENEMY_CANDIDATE_CHOICE.md) | Complete flags/weapon weights, deterministic mode4, tie/draw boundary and shared RNG leaves | Seed/interleaving, helper/domain and original traces |
| [Selected target and weapon](battle/BATTLE_SELECTED_TARGET_AND_WEAPON.md) | Retained selection query, complete cursor dispatch and command/item weapon lookup | Menu/selection producers and original execution |
| [Weapon cost and payment](battle/BATTLE_WEAPON_COST_AND_PAYMENT.md) | FP scaling/reduction, shared owner, AP query/payment and byte action-record arithmetic | Menu cadence, numeric extremes, persistence and original execution |
| [Mario jump motion](battle/BATTLE_MARIO_JUMP_MOTION.md) | Complete local first/resumed-call phases,baseline/visible integration,zero/exact-count landing and sound/AC request ownership | Numeric/audio helper/domain/caller composition and original trajectory/input traces |
| [Mario jump numerics](battle/BATTLE_MARIO_JUMP_NUMERICS.md) | Ordered single/double/fused formulas,mode division/height/phase branches,constants and complete local reviseAngle wrapper | Original sin/fmod/angle/distance/FPSCR execution and composed comparisons |
| [Unit sound-mode loader](battle/BATTLE_UNIT_SOUND_MODE.md) | Complete type/mode partial record initialization,retained gaps and concrete Mario setup playback input | Source/key/macro/voice writers,playback core/resources and original audio traces |
| [Positional SFX stop](audio/BATTLE_POSITIONAL_SFX_OFF.md) | Corrected complete body: key low13 row, pending bit0 set skips stop, row+4 high-bit SS/song/effect dispatch, ordered clear | Slot writers, table rows, stop-service channel arguments and traces |
| [Positional SFX position update](audio/BATTLE_POSITIONAL_SFX_POSITION_UPDATE.md) | Complete leaf body: handle and key -1 guards via bclr, three word stores to slot+0x1C..0x24, load/store ordering | Reader of the published position (psndSFXMain) and stale-position lifetime |
| [Unit positional SFX trigger](audio/BATTLE_UNIT_POSITIONAL_SFX_TRIGGER.md) | Complete body: status 11/10 signed-byte id steps with 10 overriding 11, belong + irand(41) - 20 start id, sentinel-driven preserve-vs-explicit volume, handle return | Check/GetStatus internals, __psndSFXOn slot provisioning, psndSFX_pit and irand domain |
| [Positional SFX slot accessors](audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) | Corrected five bodies: generation/key Chk, truncating setters, pending-bit local pitch versus descriptor-filtered backend pitch | Readers of volume/distance/pitch, __psndSFXOn writers, SoundEfxSetPitch arguments |
| [Positional SFX start wrappers](audio/BATTLE_POSITIONAL_SFX_START_WRAPPERS.md) | Three complete bodies and the derived eight-argument __psndSFXOn ABI, with arg5/arg7 always 0 and psndSFXOn_3D discarding caller r5 | __psndSFXOn body, arg names, psndSFXOnEx_3D callers |
| [Unit Monosiri source lookup](audio/BATTLE_UNIT_MONOSIRI_LOOKUP.md) | Corrected20-byte wrapping address leaf: initialized231-row Tattle table; actual sampling pointer test/relookup, invalid-word zero address | Table mutation/aliases/source-kind domains and full log/Tattle/AC/runtime |
| [Positional SFX slot provisioning](audio/POSITIONAL_SFX_SLOT_PROVISIONING.md) | Corrected complete body: numeric/name key ABI, real gates/defaults,40-slot last-free scan/strict-priority reclaim, ordered delay/fields/coherent generation | Full startup/spatial/backend/macro/resource/source/runtime binding; earlier key conflict resolved |
| [Mario Jump parameter](battle/BATTLE_MARIO_JUMP_PARAMETER.md) | Complete integer coordinate resolution,distance correction,raw count writeback and updated stylish caller operands | MarioJumpPosition body,original numeric execution and caller/timing traces |
| [Ordinary Jump event flow](battle/BATTLE_NORMAL_JUMP_EVENT_FLOW.md) | All268 request commands/branches,per-hit flags/count loop,first-attack setup,stylish/feedback and cleanup ownership | Mario-specific motion/service internals,per-hit writers,actual tutorial/cadence and original traces |
| [Jump result/stop services](battle/BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md) | Complete result/count/window/mask/grade wrappers,prize code/byte clamp and deferred TimingA stop | Grade/option/code producers,help UI and actual callers/traces |
| [Jump VM/concurrency](event/EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md) | Signed comparison/raw mask bodies,CASE>=,attached brother versus independent inline creation/end rules | Full constructor/other opcode/arithmetic/scheduler/pause behavior and original traces |
| [Unit badge/count import](unit/UNIT_BADGE_COUNT_IMPORT.md) | Complete import/reset/scan and numeric byte maps,wrapped special thresholds,scoped Mario/partner/enemy setup bindings | Inventory equip/new-game rules,other callers and executed guard/resource comparisons |
| [Unit status/pouch initialization](unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) | Complete status clear,pouch resource import/revision,party mapping and partial six-record reset with entry callers | Badge/move/snapshot producers,turn/presentation/save lifetime and original traces |
| [Small unit/runtime leaves](unit/SMALL_UNIT_RUNTIME_AND_MOTION_LEAVES.md) | Complete CanGuardStatus forwarding wrapper, CheckActStatus boolean publish with raw destination, the 60.0 small-data setter and the double-to-unsigned32 range/fctiwz mapping | Callers, guard semantics, slot meaning and FPSCR behaviour |
| [Unit part defense/lifetime](unit/UNIT_PART_DEFENSE_AND_LIFETIME.md) | Scoped contiguous part construction/pointer copies,Crump/Mario signed source tables,complete part lookup and deletion request order | Remaining entry/vector/heap/pose services,mutation and original actor lifetime traces |
| [Weapon power callbacks](battle/BATTLE_WEAPON_POWER_CALLBACKS.md) | Complete default/Mario Jump/Hammer power bodies,signed level/index/bonus ordering,ordinary six-word tables and boss base1 | Level/bonus/AC producers,weapon lifetime,unit/part construction and executed hit totals |
| [US completion/priorities](meta/US_COMPLETION_AND_PRIORITIES.md) | Current completion audit,shipped reference census,registered commands and mechanic-first order | Full event feasibility/original execution and all-system closure |
| [Unaddressed binary functions](binary/UNADDRESSED_BINARY_FUNCTIONS.md) | Static register of 43 reference functions named nowhere in prose, with entry addresses, branch census, JSON/symbol anchors and duplicate-extraction notes | All 43 behavior contracts, shared-target identities and zero-byte re-extraction |
| [Reference files inventory](binary/REFERENCE_FILES_INVENTORY.md) | Verified census of every artifacts/reference file/directory, binary classes by magic, binary↔explainer mapping, licensing ledger and handling rules | Live-corpus drift (counts are timestamped snapshots; refresh before reuse) |
| [DOL function slices](binary/REFERENCE_DOL_FUNCTION_SLICES.md) | Full88-slice top-level pairing table, listing formats, behavior/ corpus structure and symbol/address cross-checks | Semantics of individual slices (covered by behavior contracts), unlisted-slice regeneration |
| [Tool and asset trees](binary/REFERENCE_TOOL_TREES.md) | Pinned commits/remotes for the five clones, host-build evidence, Windows binaries, Switch extraction metadata and key-handling rules | Upstream histories beyond pinned commits and third-party SDK tool internals |
| [Unit visible-position writer](battle/BATTLE_UNIT_SET_POS.md) | Complete 4-instruction BtlUnit_SetPos leaf, U3c/40/44 triple shared with visible GetPos, entry-adjacent GetPos pairing | Pointer/value producers, call cadence, persistence and effects |
| [Phase/turn/flag event leaves](battle/BATTLE_PHASE_TURN_FLAG_EVENT_LEAVES.md) | Complete CheckPhase/get_turn/onoff_battleflag bodies, BattleGetSeq/evtGet/evtSetValue call order, OR/ANDC flag and 0/1 compare writes, all return 2 | Source-word identities, arg provenance, cadence and gameplay meaning |
| [Unit-query event leaves](battle/BATTLE_UNIT_QUERY_EVENT_LEAVES.md) | Complete GetHp/GetMaxHp/GetUnitWork/SetUnitWork/CheckStatus/CheckData bodies, shared TransID pipeline, -1-gate and direct-store asymmetries, extsb status byte | Unit lifetime, provenance, cadence and in-game meanings |
| [Pose-table event leaves](battle/BATTLE_POSE_TABLE_EVENT_LEAVES.md) | Complete CheckDefinePosetype band/threshold table scan and AnimeChangePoseFromTable PartsPtr→pose-disp path with r4 non-reload hazard | Table contents, index meanings, cadence and visual outcomes |
| [Wait/camera/status-switch leaves](battle/BATTLE_WAIT_CAMERA_STATUS_EVENT_LEAVES.md) | Complete StopWaitEvent id-delete/clear, camera no-move bit set/clear with negative guard, fixed status-window word write plus unresolved tail call | Id provenance, camera-word meanings, 800f7514 identity and effects |
| [Speed/fall/landing leaves](overworld/OVERWORLD_SPEED_FALL_LANDING_LEAVES.md) | Complete stick-classifier walk/dash queries, +448 slew limiter, bottomless flag/position stores with dead loads, 3-way landing dispatch, bero float leaf and fall integrator | SDA values, offset meanings, cadence and in-game effects |
| [Hit damage-return sets](battle/BATTLE_HIT_DAMAGE_RETURN_SETS.md) | Complete heap-copy event entry, name-search dispatch, recursive marking walk with +216/+220 children and r5-gated +220 phase | Record/tree provenance, SDA value, lifetimes and effects |
| [Bottomless event chain](overworld/OVERWORLD_BOTTOMLESS_EVENT_CHAIN.md) | Complete triple-converting event entry into mixed store triple with one live table lane, contrasted with all-dead sibling | Table contents, offset meanings, cadence and effects |
| [Map-change init and fade leaves](field/FIELD_MAP_CHANGE_AND_FADE_LEAVES.md) | Complete next-map buffers/shop-gate flag and map/area fade parameter blocks, snapshot-corroborated gp offsets | Buffer lifetimes, bdsw identity, cadence and transitions |
| [Fade entry dispatch](display/DISPLAY_FADE_ENTRY_DISPATCH.md) | Complete normalization, type dispatch tree, slot init blocks and common tail with ISO-read jump tables | Entry lifetimes, name indexing, allocator internals and on-screen fades |
| [Landing query 2](overworld/OVERWORLD_LANDING_QUERY_2.md) | Complete five-probe loop with direction switch, hitVec variant dispatch, lane quantization and angle tail | Flag/offset/table meanings, f5 value, cadence and landing effects |
| [Unit status check](battle/BATTLE_UNIT_CHECK_STATUS.md) | Complete null-gated GetStatus/6-compare/+759 test with exact {0,1} domain | GetStatus internals, code table, ownership and effects |
| [Hit check filter](battle/BATTLE_HIT_CHECK_FILTER.md) | Complete 8+6 arg marshal, hitVec call, six output floats and residue return | Owner/table/lane meanings, cadence and effects |
| [Message search](message/MESSAGE_MSG_SEARCH.md) | Complete two-table binary search with gp fallback and no miss-null | Table ownership, key/value contracts, cadence and effects |
| [Unit pointer getter](battle/BATTLE_GET_UNIT_PTR.md) | Complete NULL/domain table, −1-first shape, asymmetric setter note, 487-site count | Slot lifetimes, base offset, cadence and effects |
| [Unit status dispatch](battle/BATTLE_UNIT_GET_STATUS.md) | Complete 28-slot jump table with per-case byte lanes, paired sign gates, nullable outputs | Code table, lane meanings, cadence and effects |
| [Jump SFX provenance](battle/BATTLE_JUMP_SFX_PROVENANCE.md) | Complete vec3-scratch/key/invocation answers for the packed-Jump request, ISO-read zeros, dead neighbor loads | Runtime writers, signature, value domain and audio output |
| [Water recovery numerics](overworld/OVERWORLD_WATER_RECOVERY_NUMERICS.md) | Complete stage/counter machine, ISO-read SDA set, HP-penalty tick and transitions; authored-ratio/jerk gaps named | Float-chain interiors, flag meanings, cadence and effects |
| [Bottomless recovery numerics](overworld/OVERWORLD_BOTTOMLESS_RECOVERY_NUMERICS.md) | Complete first-tick/map-lane/angle setup, stage machine, fall+land chain and drop-back triple with ISO-read SDA | Lane/flag/table meanings, cadence and effects |
| [Pose call-site survey](overworld/OVERWORLD_POSE_CALL_SITE_SURVEY.md) | Complete 18-site pose-name/gate table with ISO-read literals for recovery/jump/fall/walk/dash | Remaining domains, pipe-paper lifecycle, EvtPose sites |
| [Plane flight motion](overworld/OVERWORLD_PLANE_FLIGHT_MOTION.md) | Complete re-extracted mot_plane flyMain: entry gates, stage machine, sin chains and SDA set; dot-product leaf in traversal doc | chkCollision/table/stage semantics, cadence and flight |
| [Mario key-data update](overworld/OVERWORLD_MARIO_KEY_DATA_UPDATE.md) | Complete pad-byte/flag publication, per-flag zeroing order, key-check gate and edge word; no deadzone in-body | Pad producers, flag meanings, callee internals and effects |
| [Pouch item dispatch](battle/POUCH_GET_ITEM_DISPATCH.md) | Complete type-id routing, slot/capacity cross-check, insert/compaction returns; effects/save/shops open | Subroutine internals, effects, save format and callers |
| [Pouch item removal](battle/POUCH_REMOVE_ITEM.md) | Complete table select/scan/compaction with verified offsets, snapshot-matched | Effects, save format, callers and lifetimes |
| [Coin transaction leaves](battle/POUCH_COIN_TRANSACTION.md) | Complete add/set clamps at +120 and the affordability gate with price-table read; purchase path open | Price contents, purchase callers, flows and effects |
| [Shop setup and flag](field/FIELD_SHOP_SETUP_AND_FLAG.md) | Complete record build, key-item stock admission and work-bit switch; flows open | Prices, buy/sell, gating, presentation and effects |
| [Star-power gauge storage](battle/STAR_POWER_GAUGE_STORAGE.md) | Complete AddAP/SetAP add/floor/cap leaves at SP slots; fill/thresholds open | Fill callers, max writers, save mapping and effects |
| [Shadow driver leaves](display/SHADOW_DRIVER_ACCESSOR_LEAVES.md) | Complete fifteen shadowGet* bodies: three SDA record selectors, ramp texture objects, matrix/projection/colour pointers, 72-stride config table, smartTexObj gate and the 255-division faded colour packer | Record layouts, table meaning, smartTexObj policy, callers and effects |
| [After-reaction queue](battle/BATTLE_AFTER_REACTION_QUEUE.md) | Complete init/entry/release for the 64-record B+0x16F5C queue, duplicate/full-table returns and matched-pair compaction | Main-loop consumer link, value identity, cadence and callers |
| [Status window/camera X](battle/BATTLE_STATUS_WINDOW_CHECK_AND_NOMOVE_X.md) | Complete BattleStatusWindowCheck eight-word gate plus open/close dispatch, camera no-move X bit with equality guard, event wrapper | Word meanings, status-window leaves and effects |
| [Damage event leaves](battle/BTLEVTCMD_DAMAGE_EVENT_LEAVES.md) | Complete SetEventDamage raw-word store and CommandCheckDamage five-operand pipeline with 255 failure exits | Caller identities, operand meanings and damage arithmetic |
| [Weapon/status/wait leaves](battle/BTLEVTCMD_WEAPON_STATUS_WAIT_LEAVES.md) | Complete GetWeaponActionLv byte publish, AcSetDifficulty unit path, AnnounceSetParam 8-byte record store, AfterReactionEntry queue call, StatusWindowOnOff four-way words and StartWaitEvent gate | Operand identities, callee internals, cadence and effects |
| [Hit-vector traversal order](battle/BATTLE_HIT_VEC_TRAVERSAL_ORDER.md) | Complete 4-region/call-census/return skeleton with region graph, dead R4 and NaN-safe gate patterns | Triangle math, chunk ownership, callback identity and effects |
| [Action-class table builder](battle/BATTLE_MAKE_ACT_CLASS_TABLE.md) | Complete kind-gated record regions with tags/ids, flag-pair overwrites, count publication and index clamp | Class/flag/id meanings, table lifetime, msgSearch semantics and effects |
| [Bero normal placement](overworld/OVERWORLD_BERO_NORMAL_PLACEMENT.md) | Complete six-lane int→float setup, degree/radian trig pair, GP publication and bottomless chain with ISO-read SDA values | Lane/table meanings, hitCheckFilter semantics, cadence and placement |
| [Bero length leaf](overworld/OVERWORLD_BERO_LENGTH_LEAF.md) | Complete table/float and low-12 threshold paths into evtSetFloat with ISO-read 30.0/60.0 | Table semantics, provenance, cadence and effects |
| [Stick angle/magnitude](overworld/MARIO_STICK_ANGLE_AND_MAGNITUDE.md) | Complete marioGetStick body: forced-movement substitution, BG_MODE/riding-Yoshi Y gates, distABf/angleABf calls, 70.0 magnitude cap, centered-stick view fallback and revise360 wrap | Field meanings, kpa/yoshi callees, cadence and in-game effects |
| [Jump parameter installation](overworld/MARIO_SET_JUMP_PARAMETERS.md) | Complete marioSetJumpPara 23-entry motion dispatch, three tuple tables, trig/flag scale gate, playerGravity override and Koopa tail with six callers | Tuple slot meanings, flag producers, kpaSetJumpPara internals and cadence |
| [Peak-Y publisher](overworld/MARIO_PEAK_Y_PUBLISHER.md) | Complete two-instruction marioSetPeakYpos copy of position.y into +0x12C with all ten callers | Consumers of +0x12C, apex enforcement, cadence and on-screen effects |
| [Rogueport save-block source binding](field/ROGUEPORT_SAVE_BLOCK_SOURCE_BINDING.md) | Original spawn/Koopa exclusion, bank/S_1/scalar10 and hit/system-event request graph | Contact/hit producer, common save event, animation/backend and card persistence |
| [NPC dimensions, anchors and registration](display/NPC_DIMENSIONS_TALK_ANCHOR_AND_DISPLAY_REGISTRATION.md) | Source dimensions/root+height message anchor and scoped cull/color/alpha/display queue/stay-talk requests | Full NPC/event/window/material/shadow lifecycle and runtime |
| [NPC side-response numerics](field/FIELD_NPC_MARIO_SIDE_RESPONSE_NUMERICS.md) | Exact source polynomial/FMA/cap graph and direct NPC-main side-call gates | SDK/angle/seed/backend/FPSCR and source lifecycle/runtime |
| [NPC talk-ray/display getter](field/FIELD_NPC_TALK_RAY_AND_DISPLAY_DIRECTION.md) | Null callback attribute policy, four backend callback gates and source Mario getter inputs | Full backend/geometry/frame/pose ownership and runtime |
| [Native NPC side collision](field/NATIVE_NPC_SIDE_COLLISION.md) | PMNPC006 flags, scoped source response/history/script placement and wall binding | Full NPC/scene/backend/FPSCR/runtime and hardware |
| [NPC sphere and Mario side collision](field/FIELD_NPC_SPHERE_AND_MARIO_SIDE_COLLISION.md) | Vertical-segment halfwidth query, source side-response predicates and caller flags/wall ordering | Full polynomial/flag/collision backend and runtime |
| [Ordinary NPC talk candidate](field/FIELD_ORDINARY_NPC_TALK_CANDIDATE.md) | Source dimensions, nearest/facing/occlusion/A admission, local event lifecycle and field-root order | Global interaction/encounter/event/pause ownership and runtime |
| [Native NPC talk selection](field/NATIVE_NPC_TALK_SELECTION.md) | Source dimensions/facing/obstruction/nearest rules, retained Mario/NPC order and scoped event wait | Full registry/encounter/side response/event/backend and hardware/runtime |
| [Native Darkly/per-actor talk rules](field/NATIVE_DARKLY_AND_PER_ACTOR_TALK_RULES.md) | PMNPC007 bounded source-order rules, original early actor/message/model conversion and shared services | Later quest/all-story lifetime, original numeric/runtime/backend and hardware |
| [Original NPC wander / east Goomba4](field/FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md) | Four-phase numeric/timing/RNG/sound driver, NPC movement/pose owners and source quest-before-story guard | Regular lifetime/clock/support, shared RNG, quest service and hardware parity |
| [Original item-table selection](message/ITEM_TABLE_SELECTION_WINDOW.md) | Explicit table rows, three-window templates, lifecycle/input, read/effect order and original item icons/US keys | Native draw/help services, complete providers and runtime/hardware parity |
| [Native item selection core](message/NATIVE_ITEM_TABLE_SELECTION_CORE.md) | Packed row/result/event services and source list/sequence controls | Positive quest playback, native GUI and hardware |
| [Native key-item inventory services](unit/NATIVE_KEY_ITEM_INVENTORY_SERVICES.md) | Original121-slot count/removal, event revision/result protocol and live Goomba inventory guard | Full inventory/save/revision/quest binding and hardware |
| [Native Goomba4 ordinary talk](field/NATIVE_GOOMBA4_ORDINARY_TALK.md) | Source-order US story messages with explicit quest guards and v7 knowledge-preserving checkpoint state | Item-selection quests, full saved-switch/inventory state and later lifetimes |
| [Original Goomba4 quest and item selection](field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md) | Exact GSW/GSF guards, reset, fixed/dynamic item tables, inventory wrappers and item-window ownership | Native quest binding and full original-runtime traces |
| [Native stationary NPC candidacy](field/NATIVE_STATIC_NPC_CANDIDACY.md) | Original Chu/Darkly admission/loss yaw, next-pass null interrupts and simulation-owned paper presentation | General interrupt/event profiles, complete talk return, original/hardware parity |
| [Native live wandering / PMNPC008](field/NATIVE_LIVE_NPC_WANDER.md) | Original Goomba4 art/init/retained regular motion, source candidate pause, actual-speed poses and simulation-owned paper matrix | Quest playback, shared source seed/scheduler/clock, full macro/spatial sound and original/hardware parity |
| [Portable NPC wander core](field/NATIVE_NPC_WANDER_CORE.md) | Source phases/RNG/frame/readiness/displacement/sound arithmetic with typed services and retained work | Complete regular/quest/ground profiles, shared seed/cadence and hardware |
| [Rogueport Darkly static/talk bindings](field/ROGUEPORT_DARKLY_STATIC_AND_TALK_BINDINGS.md) | Original placement/flags/bank/poses/dimensions and exact early talk plus later ordered cases | Name quest, dynamic/all-story lifetime and runtime/rendering |
| [Ordinary Rogueport NPC bindings](field/ROGUEPORT_ORDINARY_NPC_BINDINGS.md) | Exact tagged placements/tribes, init/regular/talk owners, banks/poses and ordinary source-facing requests | Wander, tavern fade, complete NPC display/arbitration and runtime |
| [Field battle entry/return](field/FIELD_BATTLE_ENTRY_AND_RETURN.md) | Area/index registration,NPC setup/random draws,first-call start and field-result polling | Service/lifetime/result producers and original transitions |
| [Lecture player attacks](story/LECTURE_PLAYER_ATTACK_FLOW.md) | Default lecture weapon substitution,Jump/Hammer teaching polls,feedback slots,mode1→2/5→6 writers and13-command end requests | Full menu/end/runtime/field controller and original input/presentation traces |
| [Lecture field entry/return](story/LECTURE_FIELD_ENTRY_AND_RETURN.md) | Story10 wall/reload entry,crossing operands,exact US choices/setup3 launch and shared story11 exit | Intersection and party services,persistent writers,battle teardown and original traces |
| [Field intersection/coordinates](field/FIELD_SEGMENT_INTERSECTION.md) | Complete segment/query bodies,endpoint and fused-product branches,used local encoding and lecture Y-copy composition | Full variable namespace,FPSCR/coordinate writers,scheduler and original traces |
| [Mario slit wall-around](field/MARIO_SLIT_CHK_WALL_AROUND.md) | Complete two-sweep 8-direction probe loop, three-way per-direction classification and the two-group triple-run return-0 verdict | Filter descriptor layout, direction-to-index mapping, constant values and callers |
| [Mario roll wall check](field/MARIO_CHECK_WALL_ROLL.md) | Double gate, 14-tick wall-hold re-arm record, front-wall descriptor and early exit into the staying search | Filter descriptors, constant values, consumer of +0x1E4 and callers |
| [Mario roll head search](field/MARIO_SEARCH_HEAD_ROLL.md) | Five-iteration indexed direction dispatch, sin/cos probe vectors with frsp, label selection and conditional clearance store | Dedicated directions 2/3/4, constant values, label strings and callers |
| [Mario staying front-wall search](field/MARIO_SEARCH_FRONT_WALL_STAYING.md) | Complete body: probe-length gate, four staged searchFrontWall probes, 0x2E==19 veto, field-name and Vivian triple gate, unconditional slip correction | Constant values, the 0x800C3D88 field literal, dou10 internals and callers |
| [Mario standing wall check](field/MARIO_CHECK_WALL.md) | Complete body: shared gate/re-arm, 14-tick record, 5-char hit-name hysteresis (10/20) on signed byte, in-place movePos slide, disengage pass | The 0x80049504 name literal, constant values, dou10_yoko_yari internals and callers |
| [Front-wall search](field/SEARCH_FRONT_WALL.md) | Structure level: Vivian==2 early return, status+field-name bypass that extrapolates the descriptor with no probe, partner query block, three groups of three filters | Per-angle filter construction, 9-probe reduction, constant values and field literal |
| [Wall-around slip correction](field/MARIO_CHK_WALL_AROUND.md) | Structure level: Koopa-driven 8-vs-2 probe count, xoris negation, unguarded double divisor, PSVEC magnitude/scale/add and dot normalisation | Four probe groups, correction-vector selection, work doubles and callers |
| [Ground follow distances and harbor fall](partner/FIELD_PARTNER_GROUND_FOLLOW_DISTANCES.md) | Separate wake/speed/history/overlap distances; source five-point support and fall order; harbor regression | Full ring/backend/placement composition and original runtime trajectories |
| [Native partner follow](partner/FIELD_PARTNER_NATIVE_FOLLOW.md) | Ground-homing jump history and wall counter evidence; native independent jump/fall, path repair and screen recovery | Full original homing/terrain/placement composition and Dolphin trajectories |
| [Field party callbacks](partner/FIELD_PARTY_EVENT_CALLBACKS.md) | Raw slot clear/kill dispatch,first-call constructor request,stop/run pointer selection and local mode writes | Constructor/destructor/saved-kind/mode-init lifetimes and original transitions |
| [Field party destruction](partner/FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md) | Complete Kill/Kill2 linked-kind hook dispatch,saved-kind difference,slot/pointer clear and ordered release/free | All-kind hooks,heap/pose internals,index/owner invariants and original traces |
| [Field party constructors](partner/FIELD_PARTY_POSITION_CONSTRUCTORS.md) | Complete main/position/secondary graphs,allocation and init/pose/resource requests,kind1/8 init and coordinate tails | All-kind tables,called services,resource/index/mode lifetime and original transitions |
| [Field party modes](partner/FIELD_PARTY_MODE_INITIALIZATION.md) | Old-mode cleanup/restore graph,flag/vector/pose requests,jump clear and slit-angle loops | Full mode update/pose/paper/hooks/domain and original execution |
| [Field party driver/index](partner/FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md) | Two-slot initialization,count/reread/update order,Mario bit gate,BSS and scoped direct-index census | Global updater/driveParty/callers,alias/REL/index/slot writers and original traces |
| [Field party driver dispatch](partner/FIELD_PARTY_DRIVER_DISPATCH.md) | Complete15-mode graph,early shadow/pending paths,inline destruction and common effect/screen/presentation requests | Called mode/ability/collision/pose/projection engines,writers and original traces |
| [Field party countdown/action](partner/FIELD_PARTY_GLOBAL_COUNTDOWN_AND_ACTION.md) | Complete global flag/key-off/byte-wrap expiry and primary mode0/1/2→3 request | Countdown/input-off writers,ability internals,caller cadence and original traces |
| [Partner walk callback](partner/FIELD_PARTNER_WALK_CALLBACK.md) | Pending history initialization,source selection,field writes and rounded target/speed clamps | Final walkMain/terrain/pose/source-query/numeric environment and original traces |
| [Follower history writer](partner/FIELD_PARTNER_HISTORY_WRITER.md) | Two-bank source selection,ordered append/full guards,unwrapped check,wrap/flags and read-query bodies | Read-index producers,final executor/state domains and original following trajectories |
| [Partner mode1 substates](partner/FIELD_PARTNER_MODE1_SUBSTATES.md) | Complete substate requests,marker/splash byte arithmetic,hazard priority and mode13 change | Called movement/fall/cloud/shadow/splash services,writers and original traces |
| [Partner walkMain dispatch](partner/FIELD_PARTNER_WALK_DISPATCH.md) | Motion-selector/substate tables,marker consumption,angle/splash/hazard tails and actual stay binding | Called homing/jump/fall/behind/pose/state engines and original traces |
| [Partner stay motion](partner/FIELD_PARTNER_STAY_MOTION.md) | Complete local threshold/map/distance decisions,mode0 guard,collision X/Z stores and kind7 pose cycle | Footmark/geometry/direction/terrain/ability services,domain and original trajectories |
| [Partner overlap correction](partner/FIELD_PARTNER_OVERLAP_CORRECTION.md) | Complete ordered box thresholds,bearing adjustment,counter wrap/decay and correction outputs | Trig/angle and dimension/flag/state domains,original trajectories |
| [Partner direction/speed](partner/FIELD_PARTNER_DIRECTION_AND_SPEED.md) | Stateful query,history reset,30-counter state publication,output ABI and vector composition | Source/status/geometry/numeric services,state domains and original traces |
| [Partner speed numerics](partner/FIELD_PARTNER_SPEED_NUMERICS.md) | Fused norm,Gekko estimate/three double refinements,NaN classification and final factor/store | Exact estimate/FPSCR/trig/domain and original numeric comparisons |
| [Partner footmark clear/ID](partner/FIELD_PARTNER_FOOTMARK_CLEAR_AND_ID.md) | Complete two-bank record/index reset,seed choice and flag-word ID predicate | Source getters/status/history consumers/domains and original trajectories |
| [Partner live target distances](partner/FIELD_PARTNER_LIVE_TARGET_DISTANCES.md) | Full live target/repeated-read selection,3D/planar pointer differences and actual stay bindings | Source/status/planar numerics,valid state and original outputs |
| [SDK partner distance numerics](partner/FIELD_PARTNER_SDK_DISTANCE_NUMERICS.md) | Complete paired lane/sum/zero branch and single estimate refinement | GQR/FPSCR/seed/domain and original numeric comparisons |
| [Partner front-wall query](partner/FIELD_PARTNER_FRONT_WALL_QUERY.md) | Complete upper/lower rays,distance quantization,normal projection,hit/null returns and X/Z stores;corrected ray ABI | Full backend/traversal/mesh/numeric and surrounding services,original trajectories |
| [Partner wall filters](partner/FIELD_PARTNER_FRONT_WALL_FILTERS.md) | Ordered three-mask filters,hidden-owner names,global status and height leaves | Owner/status/source/domain/mesh providers and original dynamic collision traces |
| [Partner surrounding-wall binding](partner/FIELD_PARTNER_SURROUNDING_WALL.md) | Mode1/width override,global ownership,unclamped first vector,corrected dot admission and four SDK vector leaves | Full ray/filter/state/math domains and original trajectories |
| [Partner fly height postpass](partner/FIELD_PARTNER_FLY_HEIGHT_POSTPASS.md) | Used stay-tail map/head/ground order,owner admission,ordered smoothing and cached-output dependency | Complete queries,height/flag/source domains and original flight trajectories |
| [Partner ground postpass](partner/FIELD_PARTNER_GROUND_POSTPASS.md) | Used stay-tail head-before-suppression,floor snap/fall,exact support names and motion-change leaf | Complete queries/appear-position,jump/fall and original collision transitions |
| [Partner ceiling query](partner/FIELD_PARTNER_CEILING_QUERY.md) | Four ordered first-success probes,raw height cap and sampled XYZ/null outputs,actual fly/ground consumers | Full backend/math/source domains and original ceiling trajectories |
| [Scalar ray wrapper](field/FIELD_SCALAR_RAY_WRAPPER.md) | Callback/six-float/seven-pointer ABI,null preserves outputs,ordered length/point/normal publication | Full backend scene/traversal/triangle/cull and resource/numeric domains |
| [Partner floor query](partner/FIELD_PARTNER_FLOOR_QUERY.md) | Complete five-ray/order/drop/highest-support/cache graph and actual stay bindings | Full backend/filter/math/source domains and original support trajectories |
| [Partner floor slope numerics](partner/FIELD_PARTNER_FLOOR_SLOPE_NUMERICS.md) | Duplicated angle pipeline,partial reject writes,three-double-refinement norm and pointY quantizer | Angle/frsqrte/FPSCR domains and original numeric output comparison |
| [Partner floor heading](partner/FIELD_PARTNER_FLOOR_HEADING.md) | Complete flag-sensitive cardinal selection,owner offset,quantization and wrap loops | Heading/camera/flag/source domains and original footprints |
| [Partner support appearance](partner/FIELD_PARTNER_SUPPORT_APPEARANCE.md) | Exact12-angle sequential ray origins,sampled output/failure and unconditional caller motion2 request | Motion2 consumers,object/backend/numeric domains and original support trajectories |
| [Partner vector ray wrapper](partner/FIELD_PARTNER_VECTOR_RAY_WRAPPER.md) | Per-call filter ABI,null-output preservation,five quantizers and raw normal publication | Backend/header/cull/resource/platform domains and original ray outputs |
| [Partner screen recovery](partner/FIELD_PARTNER_SCREEN_RECOVERY.md) | Complete control/projection gates,bank0 reset,101-move candidate loop,five heights/raised retry and root commit | Placement3,SDK projection/math/source domains and original off-screen traces |
| [Partner placement3](partner/FIELD_PARTNER_PLACEMENT3.md) | Complete nine-angle raw-ray/classification/NPC fallback/output graph;initialized output even on failure | Motion/caller/scene/NPC/source/numeric domains and original trajectories |
| [Placement3 local math](partner/FIELD_PARTNER_PLACEMENT3_MATH.md) | Complete global-facing/camera/signed-offset heading conversion and normal-inclination leaf | Angle/libm/frsqrte/FPSCR/source domains and original numeric comparisons |
| [NPC horizontal overlap scan](partner/FIELD_NPC_HORIZONTAL_OVERLAP_SCAN.md) | Complete pool/active/count/stride/norm/strict horizontal last-record selection | Pool/lifecycle/GP14/source/numeric domains and original overlaps |
| [Partner jump destinations](partner/FIELD_PARTNER_JUMP_DESTINATION.md) | Used jump/jump2 dispatch,complete main target/history priorities and three index leaves | Live target/front-floor/writer domains and original destinations |
| [Partner jump initialization](partner/FIELD_PARTNER_JUMP_INITIALIZATION.md) | Distinct count/coefficient/speed/velocity and phase-entry/pose/audio request rules | Owner/numeric/pose/audio domains and original ballistics |
| [Partner jump flight/landing](partner/FIELD_PARTNER_JUMP_FLIGHT_AND_LANDING.md) | Complete ordered vertical/floor/horizontal/failsafe and landing graphs for both handlers | Called terrain/source/flag/math domains and original trajectories |
| [Partner live direction/activity](partner/FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md) | Complete live direction q0/repeated-pointer and TargetDistY scalar rules | Target/scalar/status/source/angle domains and original outputs |
| [Partner front-floor service](partner/FIELD_PARTNER_FRONT_FLOOR.md) | Complete upward-clearance/downward-query,heading,quantized output and attribute return | Shared backend/clearance/math/source domains and original trajectories |
| [Partner status/clearance leaves](partner/FIELD_PARTNER_TARGET_STATUS_AND_CLEARANCE.md) | Guarded status-code table,float60 setter and exact direct DOL access/call census | Code/flag/BSS/alias/REL/lifecycle/source domains and original state traces |
| [Animation selectors/restart](display/ANIMATION_SELECTOR_AND_RESTART.md) | Complete ordinary/paper signed selector,resource addressing and restart writes | Resource/Main/linked/name/source/runtime domains |
| [Paper-group reuse](display/ANIMATION_PAPER_GROUP_REUSE.md) | Complete two-pass lookup,null/failure,flags/index/stamps | Pool/resource/lifetime/Main/original visuals |
| [Animation local time/clock](display/ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md) | Complete scaling,borrow/shared/linked writes,rate and literal clock requests | Clock producers/conversion/division/evaluation/numeric/runtime |
| [Partner pre-display](partner/FIELD_PARTNER_PRE_DISPLAY.md) | Complete name/paper/frame/update/render gate/depth/registration order | Animation/camera/queue/source/numeric/original visual domains |
| [Partner pose selectors](partner/FIELD_PARTNER_POSE_REQUEST_SELECTION.md) | Complete name/HP-remap/table/adjacent-color request and two source leaves | Pouch/kind/index/name/resource/lifecycle domains |
| [Goombella ordinary poses](partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md) | Exact normal kind1 pose strings, loops, moving selector, ordinary owner scale and model scale | Native homing/input/facing/sound/HP ownership and original execution |
| [Partner facing](partner/FIELD_PARTNER_DISPLAY_FACING.md) | Complete selection,four interpolation/latch branches and live stores | Flag/target/alias/FP domains and original turning |
| [Partner matrix and draw](partner/FIELD_PARTNER_DISPLAY_MATRIX_AND_PASSES.md) | Complete enable census,ordered aliased transforms,two passes/post-draw rate | SDK matrix/animation/GX/source/runtime backend |
| [Vivian tail/shadow requests](partner/FIELD_PARTNER_VIVIAN_PRESENTATION_AND_SHADOWS.md) | Complete trailing/tail predicate/ray/shadow order | Pose/shadow/queue/source/FP/original visual domains |
| [Slit collision membership](partner/FIELD_PARTNER_SLIT_COLLISION.md) | Complete ordered probe/filter/raw-normal angle-pair/boolean graph,no local output correction | Full backend/source/alias/math/FP/original collision domains |
| [Simple camera direction](camera/FIELD_SIMPLE_CAMERA_DIRECTION.md) | Complete global camera-only quantizer/wrap,without facing override | Global/source/exception/numeric domains |
| [System angle table](field/FIELD_SYSTEM_ANGLE_TABLE.md) | Complete ratio/index/coefficient/quadrant/zero-vector graph | Original table reuse/source/index/FP domains and host parity |
| [US inventory evidence correction](meta/US_MESSAGE_INVENTORY_EVIDENCE_CORRECTIONS.md) | Rechecked actual key/text census and corrected filename-ID/story inference boundary | Pending draft/generator reconciliation and complete story event binding |
| [Partner slit gate](partner/FIELD_PARTNER_SLIT_GATE.md) | Complete admission/entry/travel/rise/exit phase and return graph | Collision/paper/backend/source/numeric/original form domains |
| [Partner roll gate](partner/FIELD_PARTNER_ROLL_GATE.md) | Complete transform/spin/exit interpolation/count and blocking returns | Pose/backend/source/FP domains and original timing |
| [Slit-floor and forced exit](partner/FIELD_PARTNER_SLIT_FLOOR_AND_FORCED_EXIT.md) | Complete getter/clear/activation/cleanup leaves and22 direct bindings | Availability producer,other callers and original lifecycle |
| [Paper request wrappers](partner/FIELD_PARTNER_PAPER_REQUEST_WRAPPERS.md) | Complete handle/group/name marker/light/guarded-release protocol | Animation/resource/lifetime/source/alias backend |
| [Flying history writer](partner/FIELD_PARTNER_FLY_HISTORY_WRITER.md) | Complete three-source/two-bank append/flags/index/publication graph | Source/bank/index/alias/cadence domains and original history traces |
| [Flying history init](partner/FIELD_PARTNER_FLY_HISTORY_INITIALIZATION.md) | Complete seeded bank/record order and selector/motion setup/early return | Pose/source/flag/alias/runtime composition |
| [Cloud/Vivian flying bindings](partner/FIELD_PARTNER_FLY_CALLBACK_BINDINGS.md) | Complete callbacks,kind5/6 records,phase/offset/trailing store/request order | Renderer/pose/lifecycle/numeric domains and original trajectories |
| [Flying source leaves](partner/FIELD_PARTNER_FLY_SOURCE_LEAVES.md) | Complete read-record pointer,owner/buddy speed and repeated absolute-Y selection | Pointer/field/index/source/numeric domains |
| [Flying partner dispatcher](partner/FIELD_PARTNER_FLY_DISPATCH.md) | Complete selector/motion dispatch and splash/countdown/hazard tail | History/source lifecycle,called engines and original execution |
| [Flying behind motion](partner/FIELD_PARTNER_BEHIND_FLY.md) | Complete entry destination/countdown/move/completion/floor output graph | Called pose/terrain/source/math domains and original trajectories |
| [Flying homing entry](partner/FIELD_PARTNER_HOMING_FLY_ENTRY_AND_SPEED.md) | Complete gates/captured/live thresholds/speed requests and exceptional FPR domains | Source/kind/global/slit/roll/FP domains |
| [Flying homing history loop](partner/FIELD_PARTNER_HOMING_FLY_HISTORY_LOOP.md) | Complete live/history decision/retry/speed/far recovery graph | Index/source/backend/termination domains and original cadence |
| [Flying homing collision tail](partner/FIELD_PARTNER_HOMING_FLY_COLLISION_TAIL.md) | Complete contact-state/height/query/publication/early exit graph | Height source/pose/backend/FP domains and original traces |
| [Flying speed writers](partner/FIELD_PARTNER_FLY_SPEED_WRITERS.md) | Complete immediate versus smoothed target/current publication and numeric gates | Source/FP/cadence domains and original speeds |
| [General partner placement](partner/FIELD_PARTNER_GENERAL_PLACEMENT.md) | Complete seed/heading/primary/plane shortcut/floor admission and eight direct callers | Other caller bindings/backend/libm/source/runtime remain open |
| [General placement fallback](partner/FIELD_PARTNER_GENERAL_PLACEMENT_FALLBACK.md) | Fresh slot dispatch,six asymmetric blocks/reseeds/move signs and failure output | Feasible state/alias/backend/numeric domains and original traces |
| [Used trig and plane leaves](field/FIELD_MOVEMENT_TRIG_AND_PLANE_LEAVES.md) | Complete movePos/sincosf conversion/libm requests/fused/store order and global plane query | Libm/FP/global-plane lifecycle and original trajectories |
| [Partner placement4](partner/FIELD_PARTNER_PLACEMENT4.md) | Complete local seed/accumulating ray/admission/output and fallback request | Called backend/libm/source/alias/runtime remain open |
| [Placement4 angle layout](partner/FIELD_PARTNER_PLACEMENT4_ANGLE_LAYOUT.md) | All59 copied words,cursors,live selector and final adjacent-stack reads | Query-header/source/flag/numeric feasibility and original trajectories |
| [Partner fall air](partner/FIELD_PARTNER_FALL_AIR.md) | Complete entry/query-before-integration/default/clamp/damping/exception/horizontal graph | Backend/libm/pose/source/numeric domains and original falling |
| [Partner fall landing](partner/FIELD_PARTNER_FALL_LANDING.md) | Complete flag-dependent snap/motion/sound/footprint cleanup and return3 | Called terrain/audio/index/source domains and original landings |
| [Partner fall facing](partner/FIELD_PARTNER_FALL_FACING.md) | Complete global-facing/camera quantizer and revise360 composition | Heading/camera/FPSCR/source domains and original facing traces |
| [Status-window arbitration](battle/BATTLE_STATUS_WINDOW_ARBITRATION.md) | Four complete local bodies,independent blocker/show fields,event argument dispatch and lecture2740 consumer | Other writers,inner window/render/animation services,lifecycle and original UI observations |
| [Goombella helper roles](story/CHRISTINE_HELPER_ROLES.md) | Distinct Crump NPC/lecture kinds and setup3,init hooks,cross-linked IDs,selector-6,mode/result leaves and counter/attack transitions | Full player/counter dispatch,field entry/return,lifecycle and original traces |
| [Lecture coordinator modes](story/LECTURE_COORDINATOR_MODES.md) | Full professor init/200-command phase graph,exact work/phase transitions,20 US message sites and speaker/choice bindings | Missing player-mode producers,flag consumers,services and full original lecture execution |
| [Input reset owners](input/BATTLE_INPUT_RESET_OWNERS.md) | Direct sampler/reset census,three complete reset helpers and relocated tutorial history injection/case bindings | Indirect/bulk writers,reset lifetime,lecture selection and original multihit traces |
| [System-level/event speed](event/EVENT_SYSTEM_LEVEL_AND_SPEED.md) | Complete level table/getter,multiplier setter,creation-speed regions and scheduler consequences | Full UI/pause caller arguments,type/speed writers,restart and original traces |
| [Physical input producer](input/PHYSICAL_INPUT_PRODUCER.md) | Raw read/copy/error handling,strict48 directions,global edges,scaled24/6 repeats,input-clear chord and motor request order | SDK hardware/polling,control writers and original traces |
| [Battle input cadence/snapshots](input/BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) | General→sequence→private→AC→event order,12-byte write map,wrapper/result gates and shared-sample credit boundaries | Complete pause/main frequency,other samplers/resets,override producers and original timing |
| [Battle jump callbacks](battle/BATTLE_JUMP_EVENT_CALLBACKS.md) | JumpSetting/Position/Continue,integer target resolution,first-call integration,strict floor completion,storage and actual Crump count overrides | Main/pad/event cadence,angle/dist/trig/audio/stage producers and original hit ticks |
| [Boss normal attack](story/HARBOR_BOSS_NORMAL_ATTACK.md) | Full 95-command target/approach/precheck/defense/recovery request graph and missing-label99 boundary | Inner target/movement/input/damage services and original timing |
| [Target event interfaces](battle/BATTLE_TARGET_EVENT_INTERFACES.md) | Seven complete local wrappers/helpers,mode pair map,part ID,side narrowing and sampling ABI | Candidate cores/mode/flag producers,buffer lifetime and original selection traces |
| [Unit entry stats/spawn](unit/UNIT_ENTRY_STATS_AND_SPAWN.md) | Entry slot/publication and HP/FP copy,getter ABI,spawn ordering and init ownership;boss HP5 link | Rest of entry/parts,overrides,lifetime and original comparisons |
| [Action feedback/stage gate](battle/BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md) | Declaration counters/cap,success record,initial reset/order and tutorial flag stage-registration suppression | Other writers/audience/init/flag services and original traces |
| [Harbor boss tutorial events](story/HARBOR_BOSS_TUTORIAL_EVENTS.md) | Installed init/phase/feedback hooks,priority/latches,death-message order and enemy defense binding | Full stats,feedback writers,flag consumers,attack/reward/defeat and original traces |
| [Tutorial state services](battle/BATTLE_TUTORIAL_STATE_SERVICES.md) | Unit-work slots,exact phase/turn checks,raw-mask flags and two relocated feedback-byte readers | Producers/lifetimes,counter semantics and original comparisons |
| [Harbor tutorial configuration](story/HARBOR_TUTORIAL_CONFIGURATION.md) | Prolog/index2 binding,single group/unit,kind/init pointers and17 known setup draws | Tutorial scripts,stat/runtime overrides,rewards and original traces |
| [Harbor opening bindings](story/STORY_HARBOR_OPENING_BINDINGS.md) | Seven original REL spans,story-state entry/write graph,actor setup,20 message/speaker references and battle boundary | Complete scene/event/service conditions,tutorial config and original traces |
| [GSW[0] chapter value table](story/OPENING_GSW0_CHAPTER_VALUE_TABLE.md) | Every GSW[0] comparison literal across the 16 scanned opening events, the 3-command opcode-50 **write set** (`prologue2`→2, `epilogue_evt2`→403, `kurihakase_first_talk`→9), GSW[0] as a GP+174 WORD, **all eight opcode-34 SWITCH ladders** (OR/CASE_END vs equality-chain pair, ascending `<` first-match-wins ladder, BETWEEN+`>=` partition, non-ascending ladders, three missing DEFAULTs), two decomposed range sandwiches, the six-module three-index GSW census and the opcode-decides-operand-role rule; all indices re-verified at nesting depth 0 | Chapter titles, how a fresh save reaches 402, GSW[1420]'s nonzero producer, whether 9 is forward progress, ladder body call meanings, execution traces |
| [Game-wide GSW saved-word census](story/SAVED_STORY_WORD_CENSUS_GSW.md) | All 29 shipped REL modules, 7290 events, 234878 commands: 31 saved words / 2213 references, explicit writer-opcode set, GSW[0] as a 427-store chapter counter over values 2..405 with per-module chapter map and attributed endpoints, 402/403/404/405 producers, GSW[1420] 0..30 domain corrected, GSW[1469] the only bitfield, GSW[1122]/[1123]/[870]/[1321] counters | Chapter titles, reachability/player route, GSW[1420] nonzero producer, GSW[665] writer, save-block serialisation, DOL-side events, execution traces |
| [Child-entry nesting and the flat parse](event/EVENT_CHILD_ENTRY_NESTING_AND_FLAT_PARSE.md) | INLINE/INLINE_ID/BROTHER/BROTHER_ID openers sharing END_INLINE/END_BROTHER, exact pairing counts over 7290 events, zero unbalanced spans, max depth 2, depth test for flat command indices | Child local sharing, opcode 1 vs 107 regions, six depth-2 inner structures, original execution |
| [Opening area init story-state map](story/OPENING_AREA_INIT_STORY_STATE_MAP.md) | gor/muj/tik area inits: GSW[0] read by every init as a multi-way chapter gate, the professor writes it, GSW[1420] as a second word, GSF read/write map with shared GSF[39], six more area exits including the muj_20 bidirectional ship join | Per-comparison chapter values, GSF 5300-5600 operand role, GSF-to-save-block mapping, gor_04, original traces |
| [Opening scene state and persistence](story/OPENING_SCENE_STATE_AND_PERSISTENCE.md) | ISO-read census of dmo/aaa/sys events: GSW[0] is the only saved story word (written by prologue2/epilogue_evt2, read by aaa_00_init_evt); GF/GW are context-local and LW entry-local; decoded branch graph: nine-step sys/evt_start route plus four aaa exits with map/entrance identifiers, and GSW[0] written before the fade/mapchange | Whether the route is linear, opcodes 93/95/108 direction, -1 fade sentinel, gor/muj/tik modules, written values |
| [Original baseline](../ORIGINAL_BEHAVIOR.md) | Disc/DOL identity, evidence rules, subsystem order, basic waits/callback continuations, initial complete-game scope | Most systems beyond the initial recovered contracts |
| [Event runtime](event/EVENT_RUNTIME.md) | Priority/tie order, child/brother relationships, deletion/copy-back, selected stop behavior | Every opcode/variable type, service callbacks, restart/pause lifecycle |
| [Ordinary attack bindings](battle/BATTLE_ORDINARY_ATTACK_BINDINGS.md) | US Jump ID1/Hammer core ID3 setup sites,raw/resolved wrapper ABI and event stream identities | Full branches,unit/weapon/tutorial selection and timing |
| [Hammer weapon branches](battle/BATTLE_HAMMER_WEAPON_BRANCHES.md) | Mask2 target/setup split,pointer-selected charge periods,weapon difficulty and wrapper/core links | Full attack branch/inline/weapon consumer and tutorial/original traces |
| [Ordinary Hammer used trio binding](battle/ORDINARY_HAMMER_USED_TRIO_BINDING.md) | Original wrapper/core, ordinary setup/approach/result-driven impact, ID3 packed dependencies, level1 poses and weapon-specific damage; native no-input/early/charged Hammer and original recovery/removal | Original gauge/audio, nonfresh branches and measured runtime parity |
| [Paragoomba wing fall used trio binding](battle/PARAGOOMBA_WING_FALL_USED_TRIO_BINDING.md) | Original per-hit admission, tracked damage child, ordered grounded part/pose/data/event/sound transition and exact packed leaf ABI | Native fall/recreated art parity and full original victory/reward/field-return graph |
| [Underground trio victory controller and field return](battle/UNDERGROUND_TRIO_VICTORY_CONTROLLER_AND_FIELD_RETURN.md) | Original end-state targets/initial victory ownership, audience bonus/target adjustment, Koura child, ordered status/drop/EXP/export and preserved story continuation | Native remaining end/display/reward/pouch/fade/field services and general nonfresh callback producers |
| [Underground trio drop material and entry pouch](battle/UNDERGROUND_TRIO_DROP_MATERIAL_AND_ENTRY_POUCH.md) | Complete drop HP/FP/item/RNG/badge order, actual setup/drop tables, distinct retained field outputs, scoped initial-stage equipment and resource import | Persistent field/new-game/save pouch lifecycle, ordinary boots grant and field item admission/collection |
| [Underground trio EXP presentation and counting](battle/UNDERGROUND_TRIO_EXP_PRESENTATION_AND_COUNTING.md) | Source08..0b order, nineteen star icons/gates/alpha/positions, original four-frame icon and US point-get phrase textures, effect lifecycle and actual victory banks | Native icon/effect/window/GPU cadence/parity and later level/reward/export/field return |
| [Integer IF/switch](event/EVENT_INTEGER_BRANCHES.md) | Used integer equality,branch scanners,snapshot switch,case no-fallthrough/default/pop | Other VM variants,full contexts and original branch execution |
| [Hammer StickKeepLeft](battle/BATTLE_HAMMER_STICK_KEEP_LEFT.md) | ID3 lifecycle,held-left/stage/release rules,difficulty allowances and finish order | Caller branch/cadence/forced routes,presentation and original comparisons |
| [Action-command callbacks](battle/BATTLE_ACTION_COMMAND_CALLBACKS.md) | Eight resolved parameters/sentinel,raw mode,start and pending-result protocol;Hammer data bindings | Other setters,help internals and complete script/weapon gates |
| [TimingA command](battle/BATTLE_TIMING_A_COMMAND.md) | Complete local lifecycle/window/result and general pad history routing;seven difficulty tables | Producers,impact/cadence/display and original comparisons |
| [Battle input and defense](input/BATTLE_INPUT_AND_DEFENCE.md) | Original history buffers, virtual directions/repeat, complete local guard decision routine and tables | Caller gates, impact sampling, attacks, full damage/status/turn rules |
| [Hit precheck](battle/BATTLE_HIT_PRECHECK.md) | Ordered eligibility, per-copy evasion draws, accuracy reductions and return codes | Caller reactions, status lifetimes, stage-effect producer |
| [Damage calculation](battle/BATTLE_DAMAGE_CALCULATION.md) | Ordered HP/FP callback gates, badge/status/defense arithmetic, result codes, absorption and revival marker | Weapon callbacks, death/revival execution and turn lifecycle |
| [Damage orchestration](battle/BATTLE_DAMAGE_ORCHESTRATION.md) | Complete local request order, superguard variants, event/result selection, retained RNG draws and two gating/selection helpers | Global flag producers, audience/effects, weapon bindings and battle turn callers |
| [Counters](battle/BATTLE_COUNTERS.md) | Complete local selection order, work record, jump-table returns, consumer accumulation/status/direct-damage calls | Other callers, counter event bodies, effect internals and weapon bindings |
| [Damage application](battle/BATTLE_DAMAGE_APPLICATION.md) | Ordered effects/audio, HP/FP mutation, shared FP owner, Pity Flower/Sleep draws, bookkeeping and lethal event-code selection | Helper internals, recovery scheduling and death/revival event execution |
| [Battle event dispatch](battle/BATTLE_EVENT_DISPATCH.md) | Hit cancellation/replacement, data-key gate, wait/phase scheduling, code getters and raw/resolved operands | Installed event graphs, death/revival bodies, controller lifetime and remaining services |
| [Message resources](message/MESSAGE_RESOURCES_AND_LOOKUP.md) | US pair census, loading/index sorting, slot lookup/fallback, command split and lead-byte predicate | Lifetime/callers, full controls/glyphs, duplicate ordering and story event coverage |
| [Message parser records](message/MESSAGE_PARSER_RECORDS.md) | Append records, glyph/line layout, terminal state, wait/style/window controls and selection setup | Font tables, record consumers, ABI ambiguity and story conditions |
| [Message embedded objects](message/MESSAGE_EMBEDDED_OBJECTS.md) | US icon binding table, image geometry versus HM glyph, pose/paper records, annotation layout and literal closing gate, selection argument census | Font/icon/pose/NPC services, library ABI, runtime consumers and generated/non-US inputs |
| [Message playback](message/MESSAGE_PLAYBACK.md) | Clock/scroll/schedule advancement, waits/return codes, full opcode map, history/callbacks, paper and per-call audio admission | State producers/caller cadence, rendering/selection, runtime numerics and service lifetimes |
| [Message window lifecycle](message/MESSAGE_WINDOW_LIFECYCLE.md) | Seven-slot allocation/poll flags, creation/geometry, append/partial replay, deletion/force close and event repeat/continue polling | Rendering/manager cadence, print identity/callback producers, allocators and cleanup |
| [Message window manager](message/MESSAGE_WINDOW_MANAGER.md) | Partial pool reset, ordered live-slot update, caller placement, archive/DVD palette paths and unchecked texture lookup | Outer/display cadence, renderer, allocator/loading services and actual reset/story callers |
| [Selection drawing](message/MESSAGE_SELECTION_DRAWING.md) | Fog/border/text/icon request order, opacity narrowing, geometry and draw-time fused cursor interpolation | Inner renderer/service contracts, display cadence, bindings and original draw comparisons |
| [General window drawing](message/MESSAGE_WINDOW_DRAWING.md) | All type routes, anchor/interpolation, type1 stored geometry, clipping/restoration and indicator pose requests | Border/projection/pose services, actual cadence and original images |
| [Native dialogue bubbles](message/NATIVE_MESSAGE_BUBBLES.md) | Original local masks, ordinary placement/speaker snapshots, tail endpoints, controls/effects and dialogue overlay suppression | Native projection/height/RNG/tail adapters, full message types, indicator, matched original images |
| [Draw record admission](message/MESSAGE_DRAW_RECORD_ADMISSION.md) | Exposed prefix/key result, controls, history visibility/fades, pose/icon requests, glyph admission/emphasis and indicator callback | Inner font/pose/icon/matrix services, cadence and original images |
| [Glyph effects](message/MESSAGE_GLYPH_EFFECTS.md) | Priority gates, two-draw jitter, dynamic matrices/relaxation, clock motion and selection draw/state order | Numeric/trig/matrix/font helpers, seeding/cadence and matched original traces/images |
| [Font draw state](message/FONT_DRAW_STATE.md) | Fourteen helpers, shared raw color/alpha/scale/edge, caller-buffer mutation, palette/start reset and effect GX requests | Inner glyph/GX raster services, palette/texture writers/bindings and original images |
| [Font glyph/setup requests](message/FONT_GLYPH_REQUESTS_AND_SETUP.md) | Ordinary/in-place matrices, edge five-draw/color policy and complete JUT setup requests | Font/page data, HSV/GX/matrix services, producers/cadence and original images |
| [Inner glyph quads](message/FONT_GLYPH_QUADS.md) | WID1 width lookup, signed grid/quad attributes, two unconditional RNG draws, hue requests and FIFO vertex order | Font/page data/loading, GX/FIFO semantics, cadence and original images/seed traces |
| [Font resource setup](message/FONT_RESOURCE_SETUP.md) | Lazy load/probes, descriptor selection, texture requests, original BFN shapes and ordered width patches | DVD/root/service execution, selectors/callers and original images |
| [Font code mapping](message/FONT_CODE_MAPPING.md) | First MAP1 range, four mapping types, sparse search bounds, replacement and byte/halfword wrappers | Text-stream decoding, executed mapping corpus and actual callers |
| [Message width scanner](message/FONT_MESSAGE_WIDTH.md) | Byte/lead routes, LF count, wrapping glyph advances, icon fused width and command LF suppression | Scanf/runtime resources, drawing layout consumers and executed comparisons |
| [Icon dimensions](message/ICON_DIMENSIONS.md) | Ready gates, locale remaps, binary offset directory, both-zero original-ID texture fallback and output order | Resource loading/relocation, valid IDs/configurations and actual draw/animation consumers |
| [Affine matrix services](display/AFFINE_MATRIX_SERVICES.md) | Scale,translation application,paired-single concatenation,rounding boundaries and alias/write order | GQR0/FPSCR runtime state,GX consumers and executed matrix/image comparisons |
| [String pitch drawing](message/FONT_STRING_PITCH_DRAWING.md) | Complete vector/scalar wrappers,byte/tag routing,proportional/fixed matrices,outline/color and pen advancement | Other string callers,matrix/raster/cadence and executed images/RNG |
| [HSV color conversion](display/HSV_COLOR_CONVERSION.md) | Byte input/output, hue sectors, single-precision/fused arithmetic and alpha preservation | Floating environment and executed vertex/image comparisons |
| [Floor numeric service](format/FLOOR_NUMERIC_SERVICE.md) | Binary64 exponent/mantissa branches, signed zero, negative carry and special-value arithmetic | FPSCR/traps and executed numeric comparisons |
| [Display queue/dispatch](display/DISPLAY_QUEUE_AND_DISPATCH.md) | Record/key construction, sort wrapper/comparator, live camera filtering/callbacks, GX request map and depth helper | Outer cadence, camera/matrix/GX services and executed rendering |
| [Display sort implementation](display/DISPLAY_SORT_IMPLEMENTATION.md) | Global scratch, small pivot branches, strided sample selection, partition/recursion, cycle permutation and equal-key reversal | Actual caller cadence, copy service domains and executed sort/draw traces |
| [Message selection](message/MESSAGE_SELECTION.md) | Opening/closing, trigger versus repeat navigation, cancel/confirm order, creation identity, signed event result and window deletion | Renderer/highlighting, input/manager cadence, caller/story conditions and print wait producers |
| [Message window states](message/MESSAGE_WINDOW_STATES.md) | General/clear state transitions, fades/sounds, talk countdown, wait resume, history controls and deletion | Rendering/manager cadence, print callbacks, pose/numeric/input services and story conditions |
| [Message print events](message/MESSAGE_PRINT_EVENTS.md) | Four resolved operands, first-call identity/owner/layer, NPC/sentinel/battle speakers, placement/projection and poll completion | Variants/substitution/callbacks, placement producers, helper services and story call graphs |
| [Message print variants](message/MESSAGE_PRINT_VARIANTS.md) | NPC/layer/placement setters, field/battle partner suffixes, variant first-call gates, append and borrowed key buffers | Insert substitutions/callbacks, character/caller census, buffer/service lifetimes and story conditions |
| [Message insert formatting](message/MESSAGE_INSERT_FORMATTING.md) | Encoded argument count, word resolution, one-to-four sprintf dispatch, direct/append consumers and US percent lexeme census | Original printf ABI/parser, call-site types/counts, other substitutions/callbacks and story conditions |
| [Scan input runtime](format/SCAN_INPUT_RUNTIME.md) | sscanf ABI/empty-input gate and string read/unread/EOF protocol | Numeric/multibyte dependencies and executed caller comparisons |
| [Unsigned32 scan helper](format/SCAN_INTEGER32.md) | Sign/base states, whitespace/width accounting, wrapped magnitude/overflow, prefix failure and pushback | Executed callback corpus, public callers and 64-bit/floating helpers |
| [Unsigned64 scan helper](format/SCAN_INTEGER64.md) | Sign/prefix/count states, divide threshold, word multiply/carry, sticky overflow and unread/return ABI | Executed callback corpus, public callers and floating/multibyte helpers |
| [Multibyte conversion](format/MULTIBYTE_CONVERSION.md) | Complete length/error/continuation checks, halfword decode and canonical-length test, scan size1 composition | Executed byte/access/output corpus and actual wide-format caller coverage |
| [Float scan entry/special values](format/SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md) | Read-before-width, leading signs/whitespace, INF/INFINITY and NAN payload loops, count/pushback distinctions | All finite states/conversion exits, FPSCR and executed float argument corpus |
| [Float decimal scan](format/SCAN_FLOAT_DECIMAL.md) | Mantissa/exponent states,20-digit retention,scale/trim,handoff and status/sign/range exits | Hex states,__dec2num and executed rounding/argument corpus |
| [Float hexadecimal scan](format/SCAN_FLOAT_HEXADECIMAL.md) | Nibble packing/states,exponent wrap,normalization/bit insertion,count rewrite and range/sign exits | __dec2num for decimal path,executed callback/bit corpus and actual callers |
| [Decimal-to-binary special entry](format/DECIMAL_TO_BINARY_SPECIAL_VALUES.md) | Count/first-byte precedence,signed zero/infinity and exact N payload packing | Finite approximation/refinement,execution/FPSCR and scanner composition |
| [Decimal-to-binary approximation](format/DECIMAL_TO_BINARY_APPROXIMATION.md) | Private digit copy,exponent/chunk arithmetic,early exit,five/two scaling and candidate classification | pow,boundary selection/refinement/final rounding and execution corpus |
| [Decimal infinity boundary](format/DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md) | Fixed boundary record,strict magnitude comparison,infinity retention/finite recovery and sign exit | General refinement/ties,pow and executed final-bit corpus |
| [Decimal equality/refinement entry](format/DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md) | Complete numeric-record equality,candidate decimalization,initial direction and first upward neighbor/infinity exit | Remaining up/down search,distances/ties,pow and original execution corpus |
| [Decimal neighbor refinement/ties](format/DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md) | Complete local up/down searches,difference requests,parity ties and asymmetric side selection | __minus_dec/pow,executed iteration/distance/final-bit corpus |
| [Decimal record subtraction](format/DECIMAL_RECORD_SUBTRACTION.md) | Complete copy/alignment/borrow,omitted-tail correction/pointer and normalization flow | Executed caller invariants,distance accuracy,zero/alias cases and pow |
| [Power entry/special cases](format/POWER_ENTRY_AND_SPECIAL_CASES.md) | pow wrapper,zero/NaN order,negative-base parity,exponent/base shortcuts and errno gate | General log/exp/range core,sqrt and executed base-five/FPSCR corpus |
| [Power large-exponent path](format/POWER_LARGE_EXPONENT_PATH.md) | Ordered high-word gates,immediate returns,original-base fused near-one polynomial and split log pair | General log/common exp-range tail and executed power/FPSCR corpus |
| [Power general logarithm](format/POWER_GENERAL_LOGARITHM.md) | Normalization/reference gates,synthetic denominator,fused compensated polynomial and split handoff | Common exponentiation/range/sign tail and executed power/FPSCR corpus |
| [Power exponentiation/range](format/POWER_EXPONENTIATION_AND_RANGE.md) | Complete shared sign/product split,range gates,integer reduction,exp polynomial and scaling | sqrt,executed power/FPSCR and composed conversion corpus |
| [Square-root service](format/SQUARE_ROOT_NUMERIC_SERVICE.md) | Complete wrapper/special/errno gates,subnormal normalization,integer extraction/correction and output bits | Executed accuracy/FPSCR/errno/caller corpus and power composition |
| [Scan directive parser](format/SCAN_DIRECTIVE_PARSER.md) | Suppression/word-wrapped widths, selectors, exact conversion routing, s bitmap and signed scanset ranges | Executed descriptor corpus and numeric/multibyte helpers |
| [Scan formatter consumers](format/SCAN_FORMAT_CONSUMERS.md) | Complete local callbacks, destination fetch/store, conversion counters, partial c, s/[ loops and final EOF result | Numeric/multibyte helpers and original format/argument corpus |
| [Format output runtime](format/FORMAT_OUTPUT_RUNTIME.md) | sprintf/vsprintf termination, string sink, varargs slots, formatter dispatch/string/padding/n/fallback consumers | Wide conversions, memory services and executed call-site comparisons |
| [Format directive parser](format/FORMAT_DIRECTIVE_PARSER.md) | Complete flags/width/precision/length grammar, star argument consumption, width cap, conversion normalization and table map | Wide conversions, original call-site types/counts and executed comparisons |
| [Format integer conversion](format/FORMAT_INTEGER_CONVERSION.md) | 32/64-bit bases/signs/digits, zero/precision rules, alternate prefixes, caller-copy mutation and capacity gate | Floating/wide conversions, caller corpus and executed comparisons |
| [Format decimal rounding](format/FORMAT_DECIMAL_ROUNDING.md) | Record fields, tail ties/carry/trimming, zero-retention parity, decimal preparation and special-value output | Floating environment and executed caller comparisons |
| [Finite decimal layout](format/FORMAT_FINITE_DECIMAL_LAYOUT.md) | g/G mode choice after rounding, scientific exponent/mantissa, fixed digit/zero placement, mutation/sign/capacity contracts | Floating environment and original execution/caller corpus |
| [Binary decimal entry](format/FORMAT_BINARY_DECIMAL_ENTRY.md) | Numeric-to-ASCII request rounding/padding, raw zero/special classification, mantissa factorization and integer-record pipeline | Floating environment, hex-float and executed caller corpus |
| [Decimal multiplication](format/FORMAT_DECIMAL_MULTIPLICATION.md) | Numeric digit convolution, scratch carry, leading exponent, 36-digit copy and tail rounding | Numeric helpers, caller domain and executed comparisons |
| [Decimal powers](format/FORMAT_DECIMAL_POWERS.md) | Sparse seed dispatch, ASCII seed comparison, recursive square/odd correction and digit/count invariants | Floating environment, executed accuracy/callers and full formatting |
| [Binary numeric services](format/FORMAT_BINARY_NUMERIC_SERVICES.md) | Normalization/scaling/splitting, raw sign copy, integer shift/saturation and finite generator power bounds | Floating environment, caller corpus and executed equivalence |
| [Unsigned64 division](format/FORMAT_UNSIGNED64_DIVISION.md) | Leading-zero setup, word shifts, carry/restoring loop, quotient/remainder exits and derived zero-divisor behavior | Original execution/ABI flags, caller corpus and full formatting |
| [Hexadecimal float](format/FORMAT_HEXADECIMAL_FLOAT.md) | Raw exponent/fraction, special sign/case, fixed leading1, truncation, extended precision stack reads and output predicates | Original caller/stack domain, executed output, memory services and full formatting |
| [Announcement messages](battle/BATTLE_ANNOUNCEMENT_MESSAGES.md) | First-call formatting, shared text/timer ownership, trigger/yield protocol and draw geometry | Text lookup/controls, substitution, renderer, parameters and story dialogue |
| [Camera requests](battle/BATTLE_CAMERA_REQUESTS.md) | Mode/speed/zoom/homing callbacks, priority gates, reset side effects and vector sources | Main camera modes, producers, framing/rendering and remaining callbacks |
| [Default action events](battle/BATTLE_DEFAULT_ACTION_EVENTS.md) | Electric/zero-gravity script order, interpolation mode8, offset sentinel, command-init and speed gate | Camera/message callbacks, renderer, other curves and library numerics |
| [Action controller](battle/BATTLE_ACTION_CONTROLLER.md) | All15 local states, command/script routing, retained draws, count/cleanup order and post-event handoff | Command/event/service bodies, flag writers, outer cadence and content |
| [Move controller](battle/BATTLE_MOVE_CONTROLLER.md) | All15 local states, immediate/fallthrough transitions, phase event ordering, actor reset/handoff and announcement drain | Action controller, services/scripts, external channel writers and outer cadence |
| [Phase tables](battle/BATTLE_PHASE_TABLES.md) | Actor/event table membership and sorts, phase wrapper, selected actor/count-consumption regions and phase-end predicate | Full move/action controllers, flag/state writers and outer cadence |
| [Move counts](battle/BATTLE_MOVE_COUNTS.md) | Fast/Slow reset arithmetic, max-count setter, action/guard predicates and turn-first reset caller | Count consumption, outer cadence, field consumers and actor scheduling |
| [Removal and reservations](unit/INVENTORY_REMOVAL_AND_RESERVATIONS.md) | ID/index removal, ordered parallel compaction, reserve insertion/refund and deferred consumption | Item table bindings, all lifecycle callers and badge array consumers |
| [Waits and conclusion](battle/BATTLE_WAITS_AND_CONCLUSION.md) | Unit/battle barriers, queued reactions, reservation consumption and three-group conclusion predicate | Queue/configuration producers, pouch removal, conclusion events/rewards and outer cadence |
| [Recovery events](battle/BATTLE_RECOVERY_EVENTS.md) | Status fallback/override, Happy pose gates and waits, identity/data/pose wrappers | Motion, float coercion, pose/sound data and key8 bindings |
| [Regeneration](battle/BATTLE_REGENERATION.md) | Status HP/FP additions, shared FP ownership, Happy per-copy trials, presentation and wait differences | Happy event bodies, display fields, effects and outer cadence |
| [Periodic damage](battle/BATTLE_PERIODIC_DAMAGE.md) | Poison requests, Burn registration/polling/delayed damage and shadow guard | HP/FP regeneration, numeric sine/effect internals and outer cadence |
| [Turn recovery](battle/BATTLE_TURN_RECOVERY.md) | Ordered status phases, slot scans, event registration/waits, Freeze expiry and direct caller inventory | Periodic effects, outer controller cadence, event bodies and inner wait helper |
| [Status storage and recovery](battle/BATTLE_STATUS_STORAGE.md) | All local getter/setter cases, shared signed pairs, presence/badge override, explicit recovery steps | Infliction/immunity, recovery caller timing, periodic effects and lifecycle |
| [Status presentation](battle/BATTLE_STATUS_PRESENTATION.md) | Complete application tail: effect selection/payloads, positioning, announcements, sound and return semantics | Effect internals, announcement queues and original rendering comparisons |
| [Status infliction](battle/BATTLE_STATUS_INFLICTION.md) | Weapon fields/stacking, ordered wrapper, immunity gates, susceptibility/RNG and mutation prefix | Weapon bindings, effect services and lifecycle |
| [State and inventory](unit/STATE_AND_INVENTORY.md) | Saved-work sizes/accessors, resource setters, capacity and generic item insertion | Full progression, equipment, special items and original save records |
| [Randomness](meta/RANDOMNESS.md) | rand recurrence and irand draw consumption | Seeding, randf, consumer call order and persistence |
| [Ordinary overworld movement](overworld/OVERWORLD_MARIO_MOVEMENT.md) | Retained travel/current-stick air drift, jump phase startup and probe headings | Special modes, math services, source input/runtime comparisons |
| [Ground/jump arbitration](overworld/OVERWORLD_MOTION_ARBITRATION.md) | Same-sample idle/walk/dash jump retries, current launch base/heading, late landing and no local A buffer | Contact/action/pose/lock/modifier domains and original traces |
| [Directional wall movement](overworld/OVERWORLD_WALL_MOVEMENT.md) | Forward/side rays, half-tangent slide, pass-through, two Around passes and stationary ownership | Original ray traversal/numerics, dynamic contacts, special modes and hazards |
| [InterruptStop and Escape result route](battle/BATTLE_INTERRUPT_STOP_FLAGS_AND_ESCAPE_RESULT_ROUTE.md) | Complete raw-gated flag10/20 publisher, scoped Escape branch/tail and annotated DOL event census | Other override domains/full escape/tutorial/result/field/story/save/runtime |
| [Ending admission gates](battle/BATTLE_END_ADMISSION_GATES_AND_CONCLUSION_CALLERS.md) | Scoped turn wait/first-act ending writes, exact dispatch bindings and ten direct conclusion callers | Complete controller/death/override/tutorial/result-field-story-save/runtime |
| [Conclusion group publication](battle/BATTLE_CONCLUSION_GROUP_PUBLICATION_AND_FRESH_READS.md) | Complete three-group/two-scan/status/attribute/fresh-config/count-or-mask service | Actual configs/death/override/end admission/original/native result binding |
| [Battle result phase0 producer](battle/BATTLE_RESULT_PHASE0_PUBLICATION_AND_PRIORITY.md) | Complete set/get/enemy-belong leaves; scoped ending priority/captured I/no-write and9 direct setter sites | Conclusion/death/admission/override meanings/rewards/story/save/runtime binding |
| [Opening GSF notice/pipe/save mapping](story/OPENING_GSF_NOTICE_PIPE_AND_SAVE_STORAGE.md) | Verified233/236 notice writers/1207 pipe-reload/GSW0+4 bank and record mapping | Exhaustive opening census/other consumers/admission/lifetime/original traces |
| [Save continuation caller](memory/SAVE_CONTINUE_GAME_PARTY_AND_DESTINATION_DISPATCH.md) | Complete continueGame restore/slot/party/sound/countdown/switch4/25 destinations/special reset | Upstream admission/children/schemas/story/room/door/original load execution |
| [Live save namespace restore](memory/CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md) | Complete cardCopy2Main/file-pointer leaf; six reverse copies/preserved live GP/reset/string/bit8/scoped caller | Admission/full continueGame/schema/pointer/SDK/namespace lifetime/original round-trip |
| [Single-slot read validation/selection](memory/CARD_SINGLE_SLOT_READ_VALIDATION_AND_SELECTION.md) | Complete read_main;15 phases/header+two record validation/signed timestamp/tie/resident load/error/wait | Live restore/read-all/backup repair/SDK/providers/original round-trip |
| [Card dispatcher/format/check callbacks](memory/CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md) | Complete cardMain/eight callbacks; table/probe/countdown/phases/poll/error/completion/caps | Async children/SDK/read validation/backup/namespaces/runtime/native binding |
| [Original save record assembly/checksum](format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md) | Complete cardWrite; busy/slot/live GP snapshots/six blocks/switch inclusion/version/8800-byte additive checksum | Async IO/read validation/version/namespace/backup/runtime/native binding |
| [Rogueport camera porting coverage](camera/ROGUEPORT_CAMERA_PORTING_COVERAGE_AND_REMAINING_BINDINGS.md) | Consolidated source dependencies; verified44-record/494-triangle/220-pair domain, reset/shift/event evidence and metadata | Full native composition/provider/numeric/original runtime/full-game coverage |
| [Camera road manager/time helper](camera/FIELD_CAMERA_ROAD_MANAGER_COPY_AND_TIMEBASE.md) | Complete manager/getV60FPS; fresh copy/timestamp/progress order/signed divider ABI/quotient rollover/scaling | Authored selection/library/provider/FP/IRQ/original timing/visual binding |
| [Camera road interpolation/current state](camera/FIELD_CAMERA_ROAD_INTERPOLATION_AND_CURRENT_STATE.md) | Two complete bodies; exact easing/nearest-angle/field writes/prefix/active geometry/matrix requests; scoped caller | Authored selection/full manager/math/FP/state domains/original framing |
| [Camera road comparison/rate](camera/FIELD_CAMERA_ROAD_COMPARISON_AND_TRANSITION_RATE.md) | Complete compareCamRoad; integer/float progress gates/mask/metric/sqrt/deadband/output and two caller directions | Geometry/selection/transition/profile weights/FP/room limits/original visuals |
| [Mario camera reset/rate/ID owners](camera/FIELD_MARIO_CAMERA_RATE_ID_AND_RESET_OWNERS.md) | Eight complete leaves; separate follow/shift defaults/raw templates/ordered writes and40 direct DOL caller sites | Caller admission/other writers/REL/indirect/road solver/runtime/native binding |
| [Mario camera shift/control](camera/FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md) | Complete main/update/four controls; biased three-channel shifts/target gates/owner captures/expiration/heading | Rates/defaults/room limits/camera consumers/zoom/trig/pause/runtime/native binding |
| [Complete Mario field update](overworld/MARIO_MAIN_COMPLETE_REQUEST_GRAPH.md) | Complete marioMain local graph; active/badge/room gravity/timer/motion/finish; correct strcmp/camera/HP/sequence targets | Child services/providers/physical cadence/pause/original traces/native binding |
| [Directional movement rate](overworld/OVERWORLD_DIRECTION_MOVEMENT_RATE.md) | Retained heading/camera-offset gain after air cap and quantized pre-camera offset producer | Actual camera ownership/defaults, original trig and terrain/special paths |
| [Mechanics ledger](../MECHANICS.md) | Existing US ordinary field movement/collision/recovery/road evidence | Full physics, partners, dynamic events, matched original gameplay |
| [Asset provenance](../ASSETS.md) | Original sources, conversion choices and reuse boundaries | Complete animation, sound-event and material behavior |

The final system requirements document is **not yet ready to be derived**:
many original behaviors below remain unverified. Its eventual acceptance
requirements must reference recovered contracts and test evidence, rather than
presenting this research backlog as if it were a complete game specification.

## Full-scope completion register

All rows remain open. "Partial" refers to the concrete documents above, not
completion. Reused data removes the need to author assets, but their parsers,
runtime semantics, resource lifetime and event bindings still require recovery.

| System | Current state | Evidence needed to close documentation |
| --- | --- | --- |
| Boot/platform/main loop | Partial order only | Retrace/input timing, initialization/shutdown, timebases, errors and region behavior |
| Event VM and services | Partial | All shipped opcodes, tagged operands, scheduling/lifecycle and every reachable service's effects |
| Story and quests | Not recovered | All original event graphs, prerequisites, branches, message/speaker/control-token bindings and persistent writes |
| Field movement/forms | Partial | Every motion/form, slopes/walls/ceilings, hazards, collision flags, interactions and moving geometry |
| Partners/NPCs | Partial local following/history/forms/queries/presentation;NPC/ability systems open | Remaining motion/AI/abilities/scripts,recruitment/upgrades,source/lifetime/backend domains and original trajectories |
| Maps/transitions | Partial | All entrance types, dynamic objects, loading/REL lifetime, story gates, interior and scripted travel |
| Combat/turn engine | Partial defense, precheck, HP/FP calculators, counter selection and direct application | Unit/part/weapon lifecycle, target/turn ordering, callbacks, status application, hit-event execution, loss/escape/win |
| Action commands | Partial input/defense | Every attack/special/stylish command's timing, difficulty, input state, scoring and effects |
| Audience/stage/rewards | Not recovered | Audience AI, hazards, bingo/SP, XP/drop/progression formulas and conditions |
| Inventory/equipment/economy | Partial | All acquire/remove/sort/use/equip paths, shops/crafting/storage, mail and collectible effects |
| Save/load/progression | Partial in-memory state | Record format/validation/failure flows, complete state lifetimes and load reconstruction |
| RNG | Partial | All generators/seeds, persistence and consumption sites/order |
| UI/messages/pause | Substantial local message/parser/window contracts;pause/focus integration open | Token interpreter, windows/options, input repeat/focus, scene pause/advance and accessibility adaptations |
| Camera/graphics/animation | Formats,field and local display/font/matrix/camera-request contracts | Every track/type, scene/layer/cull/lighting behavior, paper transformations, camera modes, effects |
| Audio/music | Formats/streams only | Game event→macro→voice behavior, track transitions, mix/loops/envelopes/synchronization |
| Chapters/interludes/minigames | Not recovered | Reachable content inventory, per-mode mechanics, optional content and ending/postgame conditions |


## Related registers and audits

Collection-level detail was split out of this index on 2026-10-12; the register table above and the completion register below remain the entry point:

- [Public decompilation audit](meta/PUBLIC_DECOMP_AUDIT.md) — pinned doldecomp/ttyd object census and what it does and does not prove.
- [Gate for the eventual system requirements document](meta/REQUIREMENTS_GATE.md) — evidence rules the eventual requirements must satisfy.
- [ID1 map material, texture/light and TEV contract status](display/ID1_MAP_MATERIAL_AND_TEV_STATUS.md) — dated ID1 delivery summaries; append new ones there, not to this index.

- [Rogueport story object visibility](field/ROGUEPORT_STORY_OBJECT_VISIBILITY.md): recovered ordinary-entry ship, wake, east and west group conditions.
- [Rogueport interior doors](overworld/OVERWORLD_ROGUEPORT_INTERIOR_DOORS.md): original same-map inn/professor records, contact admission, destinations and source event chronology.
- [Native save format](format/NATIVE_SAVE_FORMAT.md): version4 interior state, backward compatibility and atomic validation/recovery.
- [Professor first-talk and fence](story/PROFESSOR_FIRST_TALK_AND_FENCE.md): original door/story gates, 31 message bindings and complete first-talk/fence command requests.
- [Native professor first-talk](story/NATIVE_PROFESSOR_FIRST_TALK.md): original REL execution, native field services and explicit remaining presentation/party dependencies.
- [Native fence/lecture entry](story/NATIVE_PROFESSOR_FENCE_AND_LECTURE_ENTRY.md): original exit scene, paper deformation, collision removal and live crossing poll.
- [Fence paper capture](display/PROLOGUE_FENCE_PAPER_CAPTURE_AND_ANIMATION.md): source quad, sparse keys, rectangle and image completion owners.
- [Temporary Frankly follow](partner/NATIVE_TEMPORARY_FRANKLY_FOLLOW.md): original secondary-party source selection and native terrain adapter.
- [Frankly party lifecycle/fence admission](partner/FIELD_PROFESSOR_TEMPORARY_PARTY_AND_FENCE_ADMISSION.md): source kind11 construction, slot1 history and story9 house exit callback.

- [Native opening and file selection](story/NATIVE_OPENING_FLOW.md)

- [Partner save-block source collision](partner/FIELD_PARTNER_SAVE_BLOCK_COLLISION_BINDING.md) — original floor/front/around name filters and distinct source callbacks.
- [Native save-block lifecycle/event core](field/NATIVE_SAVE_BLOCK_LIFECYCLE_AND_EVENT_CORE.md) — source stages, nested keys and original221-command request graph; native scene/UI/card binding next.
- [Native ordinary NPC presentation](field/NATIVE_NPC_PRESENTATION_AND_SOURCE_DIMENSIONS.md) — source matrix/facing/dimensions/talk requests and original shape surfaces.
- [Native live save-block binding](field/NATIVE_LIVE_SAVE_BLOCK.md) — source hit clocks/events, original System/selection GPU rendering and yielding SD writer.
- [Native save-block retained presentation/write queue](field/NATIVE_SAVE_BLOCK_RETAINED_PRESENTATION.md)
- [Save camera8 letterbox decision lock](camera/SAVE_BLOCK_CAMERA8_LETTERBOX_DECISION_LOCK.md)
- [Save original cue431 and residency](audio/SAVE_BLOCK_431_SOURCE_CUE_AND_RESIDENCY.md)
- [Save-block restart/event identities](display/SAVE_BLOCK_RESTART_CLOCK_AND_EVENT_IDENTITIES.md) — original ratio publication, program spans and callbacks.
- [Save retained window requests](message/SAVE_EVENT_RETAINED_WINDOW_CONTINUE_AND_APPEND.md) — Continue/Add ownership and raw control polling.
- [Native map-object collision](field/NATIVE_MAP_OBJECT_HEAD_COLLISION.md) — original bbox cube and shared head/floor/wall responses.
- [Dynamic object hit registry](field/DYNAMIC_OBJECT_HIT_REGISTRY_AND_RAY_ORDER.md) — source insertion, traversal, query ties and lifetime.
- [Native actor surface pack](display/NATIVE_ACTOR_SURFACE_PACK.md) — original scrolling textures, RGBA and source pass/cull rendering.
- [Save-block UV/material binding](display/SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md) — original texture overrides, matrix and draw passes.
- [Save-block head/hit lifecycle](field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) — source cube collision and hit state machine.
- [Save-block common event](field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md) — original choices, flags and card request graph.
- [Native per-area actor catalog](field/NATIVE_AREA_ACTOR_CATALOG.md) — authored static-init coverage, residency, batching and remaining source bindings.

- [Native lecture choices and reload](story/NATIVE_LECTURE_CHOICES_AND_RELOAD.md): original choices, retained dialogue, used icons and source story/flag reload.
- [Lecture field actors/backend boundary](story/LECTURE_FIELD_ACTOR_VISIBILITY_AND_BACKEND_BOUNDARY.md): NPC visibility and distinct setup3 teaching ownership.
- [Professor story11 reload](story/PROFESSOR_STORY11_RELOAD_AND_LECTURE_REENTRY.md): exact flag1207 and map initialization.
- [Field speaker pose ownership](message/FIELD_SPEAKER_TALK_STAY_POSE_OWNERSHIP.md): glyph countdown and actual talk/stay requests.

- [Native underground pipe route](story/NATIVE_UNDERGROUND_PIPE_ROUTE.md): original departure flag/event, reciprocal map loading and explicit pipe-motion/story gaps.

- [Native underground trio scene](story/NATIVE_UNDERGROUND_TRIO_SCENE.md): original approach poll, actors/poses, prebattle dialogue and retained setup4 boundary.

- [Native setup4 battle introduction](battle/NATIVE_SETUP4_BATTLE_INTRODUCTION.md): original unit/part data, Frankly init/entry/phase and original selected stage objects.

- [Native harbor before-battle](story/NATIVE_HARBOR_BEFORE_BATTLE.md)
- [Native Crump opening actor motion/facing](story/NATIVE_HARBOR_ACTOR_MOTION_AND_FACING.md)
- [Stationary scripted jump](field/FIELD_SCRIPTED_STATIONARY_JUMP.md)

## Native opening integration

- [Native opening voyage callbacks](story/NATIVE_OPENING_VOYAGE_CALLBACKS.md)
- [Native opening voyage scene](story/NATIVE_OPENING_VOYAGE.md)
- [Native opening harbor ship](story/NATIVE_OPENING_HARBOR_SHIP.md)
- [Native Opening House Objects](story/NATIVE_OPENING_HOUSE_OBJECTS.md)
- [Native Opening House Presentation](story/NATIVE_OPENING_HOUSE_PRESENTATION.md)
- [Native Opening Demo Presentation](story/NATIVE_OPENING_DEMO_PRESENTATION.md)
- [Native Opening Demo Map Visibility](story/NATIVE_OPENING_DEMO_MAP_VISIBILITY.md)
- [Native Opening Book Presentation](story/NATIVE_OPENING_BOOK_PRESENTATION.md)
- [Native Opening Playtest Chain](story/NATIVE_OPENING_PLAYTEST_CHAIN.md)
- [Opening Demo Golden Chest Material Check](story/OPENING_DEMO_GOLDEN_CHEST_MATERIAL_CHECK.md)
- [Native Harbor Mario Init](story/NATIVE_HARBOR_MARIO_INIT.md)
- [Battle Fresh Packed Pouch](battle/BATTLE_FRESH_PACKED_POUCH.md)
- [Battle Packed Counter And Hit Event](battle/BATTLE_PACKED_COUNTER_AND_HIT_EVENT.md)

- [Harbor original field battle information](battle/HARBOR_SOURCE_FIELD_INFORMATION.md)

- [Harbor source sequence startup and unit allocation](battle/HARBOR_SOURCE_SEQUENCE_STARTUP.md)
- [Original US startup resource admission](battle/NATIVE_STARTUP_RESOURCE_ADMISSION.md)
- [Native original harbor curtain fade](display/NATIVE_HARBOR_CURTAIN_FADE.md)

- [Harbor source sequence startup](battle/HARBOR_SOURCE_SEQUENCE_STARTUP.md): original init/resource admission and ordinary FirstAct entry/fade/event gates through Turn ownership.
- [Native demo chest effects](effect/NATIVE_DEMO_CHEST_EFFECTS.md)

- [Original item presentation metadata](unit/ITEM_PRESENTATION_METADATA.md): physical table extent, original message keys, null record and icon census.
- [Native item presentation catalog](unit/NATIVE_ITEM_PRESENTATION_CATALOG.md): indexed original metadata pack and shared selector art admission; UI/event binding remains open.
- [Original item-selector frame and decoration](display/ITEM_SELECTOR_WINDOW_FRAME.md): window matrices, separate rectangle centers, nine-slice original texture/material and used blue tab geometry.


Opening title: [ordinary entry, prompt and idle return](display/NATIVE_OPENING_TITLE.md), [demo fades](display/NATIVE_OPENING_DEMO_FADES.md), and [BGM owners](audio/OPENING_TITLE_BGM_OWNERS.md).
- [Native original type0 window frame](display/NATIVE_TYPE0_WINDOW_FRAME.md): shared source geometry and citro3d transform/buffer binding; live EXP caller and future selector frames.

Opening movement: [original Parakarry glide](story/NATIVE_OPENING_NPC_GLIDE.md).

- [Field Mario owner, control depth and battle preparation](field/MARIO_OWNER_CONTROL_DEPTH_AND_BATTLE_PREPARE.md): source constructor memset boundary, persistent field pointer, byte-wrapping control operations, ordered preparation, two-slot party controls and ending1f cleanup.

- [Field party current-index selection and entry gate](partner/FIELD_PARTY_CURRENT_INDEX_SELECTION.md): primary/auxiliary live pointer lookup, word0 mask8 exclusion and canonical pouch unlock before destructive construction.

The [native Switch soundtrack adapter](audio/NATIVE_SWITCH_SOUNDTRACK.md) is
an explicit asset/presentation adaptation, outside recovered US behavior.

- [Native field party registry and ending entry prelude](partner/NATIVE_FIELD_PARTY_REGISTRY.md): owned slots, current-index/control binding and source pouch admission before constructor waits.
- [Original partner appearance position2](partner/FIELD_PARTNER_APPEAR_POSITION2.md): distinct single/dual ray order, raw down-hit admission, retained/live headings and constructor initialization evidence.

Opening audio: [original house sound requests and native PCM](audio/NATIVE_OPENING_HOUSE_SOUNDS.md).

- [Field party entry camera/resource/animation admission](partner/FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md): stored camera ownership, constructor field order, original resource selection, mode2/archive and pose/material admission domains.

- [Native retained explicit-table selector owner](message/NATIVE_ITEM_TABLE_SELECTION_OWNER.md) and [original draw requests](message/ITEM_TABLE_SELECTION_DRAW_REQUESTS.md).

- [New Game dialogue input and house framing](story/NATIVE_OPENING_INPUT_AND_FRAMING.md)

- [Used awakening and prologue event owners](story/NATIVE_OPENING_AWAKENING.md)

- [Window font string/message consumers](message/WINDOW_FONT_STRING_AND_MESSAGE_CONSUMERS.md): source scanner, edge/icon/reset requests and explicit quest text census.

- [Fresh opening GW8 provenance and resolved ship jump operand](story/OPENING_FRESH_GW8_PROVENANCE.md)

- [Field Mario pose handles and material provenance](partner/FIELD_MARIO_POSE_HANDLES_AND_MATERIAL_PROVENANCE.md): exact display selector, original field bank creation, re-entry and authored material/light writers.

- [Native explicit item selector rendering](message/NATIVE_ITEM_SELECTOR_RENDERING.md), [source icon anchors/flags](display/ITEM_SELECTOR_ICON_ANCHORS_AND_FLAGS.md), and [pouch parameter revision](unit/POUCH_PARAMETER_REVISION.md).

- [Native original battle HUD and button-accessible submenu](display/NATIVE_BATTLE_HUD_AND_SUBMENU.md)
- [Harbor stationary jump and encoded event-position admission](story/NATIVE_HARBOR_ENCOUNTER_EVENT_FLOAT_ADMISSION.md)

- [Ordinary party entry first appearance order](partner/FIELD_PARTY_ENTRY_FIRST_APPEARANCE_ORDER.md): exact wrapper scale/display captures, placement-before-index publication and fresh Hello's second placement.

- [Native ordinary partner appearance and Hello](partner/NATIVE_FIELD_PARTY_APPEARANCE.md): live owner/heading/ray services, original single/dual placement policy and delayed current-index publication.

- [Audience sound bank and ending clap](audio/BATTLE_AUDIENCE_SOUND_BANK_AND_ENDING_CLAP.md): exact original bank/init/slot1 producer/stop history and backend dependency.

- [Portable original damage orchestration owner](battle/NATIVE_DAMAGE_ORCHESTRATION_OWNER.md)

- [Native general rule and audience storage](battle/NATIVE_GENERAL_RULE_AND_AUDIENCE_STORAGE.md): all68 selectors, retained unit/helper ownership and corrected inline audience bank.

- [Opening map display](story/OPENING_MAP_DISPLAY.md): original item/pose prelude, alpha callback, native presentation admission and group512 music handoff.

- [Opening map BGM](audio/OPENING_MAP_BGM.md): group512 integer off and original same-group replacement tail.

- [Opening map turn capture](display/OPENING_MAP_TURN_CAPTURE.md): original49/50 sparse geometry, dynamic material inheritance, GPU ownership and final-pose acknowledgment.

- [Opening arrival field departure ownership](story/OPENING_FIELD_DEPARTURE_OWNER.md)

- [Native setup2 registry and separate visual projection](battle/HARBOR_NATIVE_REGISTRY_AND_VISUAL_PROJECTION.md)

- [Native Goomba4 quest2 hand-in](field/NATIVE_GOOMBA4_QUEST2_HAND_IN.md) and [source talk control/pose admission](field/GOOMBA4_QUEST_TALK_CONTROL_AND_POSE_ADMISSION.md).

- [Native audience sound record binding](audio/NATIVE_BATTLE_AUDIENCE_SOUND.md): clap/stop/volume metadata, live count filtering and explicit unavailable-key backend failure.
- [Setup4 initial crowd and pouch gate](battle/AUDIENCE_SETUP4_INITIAL_POPULATION_AND_POUCH_GATE.md): actual setup/rank/capacity/desired count, RNG and zero-weight/key domains.

- [Ending top-bank4 service owners](battle/ENDING_TOP_BANK4_SERVICE_OWNERS_AND_IDLE_DOMAINS.md): real seven-service ownership, used idle domains and exact source sequence setter correction.
- [Field-return counter and fade order](battle/FIELD_RETURN_SEQUENCE_COUNTER_AND_INCOMING_FADE_ORDER.md): signed wrapping delay, state5-only listener tail, fresh GP callbacks/presets and timestamp globals.
- [Native engine completion and return sequence](battle/NATIVE_ENGINE_COMPLETION_AND_RETURN_SEQUENCE.md): ordered engine/lower flag adapter, corrected native sequence bank and portable delayed return/fade branches; real stage/exit providers remain.

- [Setup4 stage-object cache and map requests](battle/SETUP4_STAGE_OBJECT_CACHE_AND_NAMED_MAP_REQUESTS.md): selected5 and hidden builtin2, cache999, exact layer pivots and named map matrix latch/recalculation.
- [Portable stage-object update and bindings](battle/NATIVE_STAGE_OBJECT_UPDATE_AND_BINDINGS.md): full32-record Main, original vertex ownership export and retained native map/renderer frontier.

- [Native scene actor GPU texture lifetime](display/NATIVE_SCENE_ACTOR_GPU_TEXTURE_LIFETIME.md).

- [Native stage-map owner and GPU updates](battle/NATIVE_STAGE_MAP_OWNER_AND_GPU_UPDATES.md): original complete node hierarchy, seven rank0 records, source flags/matrix latches and live revision-driven 3DS updates.
- [Spotlight work, init and Main admission](battle/STAGE_SPOTLIGHT_WORK_INIT_AND_MAIN_ADMISSION.md): real SBSS/bank ownership, five spot records, retained inactive/level-up gate, procedural draw and free frontier.

- [Native Goomba4 quest13](field/NATIVE_GOOMBA4_QUEST13_HAND_IN.md), [native pouch checkpoint v8](format/NATIVE_POUCH_CHECKPOINT_V8.md), and [Trouble Center writers/acquisition](field/TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).

- [Plaza Goomba3 setup/talk](field/GOOMBA3_SOURCE_SETUP_AND_TALK.md), [item/coin frontiers](field/GOOMBA3_ITEM_ENTRY_GET_AND_COIN_SERVICE_FRONTIERS.md), and [native talk/task services](field/NATIVE_GOOMBA3_TALK_AND_TROUBLE_SERVICES.md).

- [Native source stage-floor lookup and jump recovery](battle/NATIVE_STAGE_FLOOR_AND_JUMP_RECOVERY.md).

- [Original NPC map-side/Goomba3 zero-vertical path](field/FIELD_NPC_MAP_SIDE_MOVEMENT_AND_GOOMBA3_ZERO_VERTICAL.md) [native map-side/Goomba3 wandering](field/NATIVE_NPC_MAP_SIDE_AND_GOOMBA3_WANDER.md), and [item95 deferred grant sequence](field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).

- [Original native weapon-menu icons](display/NATIVE_WEAPON_MENU_ICONS.md).

- [Portable forced item95 receive](field/PORTABLE_ITEM95_RECEIVE_SEQUENCE.md), [original window/Mario/icon bindings](field/ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md), [exact packed insertion](unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md), and [type6 effect bank/update/draw](effect/FIELD_ITEM_GET_TYPE6_BANK_UPDATE_AND_DRAW.md).

- [Stage work initialization and inactive/counter Main](battle/STAGE_WORK_INITIALIZATION_AND_INACTIVE_COUNTER_PASS.md): original defaults, scratch free order and four interpolation counter ABIs.
- [Native stage-work storage and counter services](battle/NATIVE_STAGE_WORK_STORAGE_AND_COUNTER_SERVICES.md): scalar initialization, retained cleanup, bound built-in interpolation/finite conversion and remaining active-entry dependencies.

- [Spotlight mutable colour bootstrap and AcHelp admission](battle/SPOTLIGHT_MUTABLE_COLOUR_BOOTSTRAP_AND_AC_HELP_ADMISSION.md): actual BSS clearing, source zero-to-white record colour and help display gates.
- [Native spotlight allocation and ending stage pass](battle/NATIVE_SPOTLIGHT_ALLOCATION_AND_ENDING_STAGE_PASS.md): real five-record map bank, retained session state and source-predicate progress to AudienceMain.

- [Native original item-get visual resources](display/NATIVE_ITEM_GET_VISUAL_RESOURCES.md), [US title expansion/window gates](message/ITEM95_US_TEMPLATE_EXPANSION_AND_WINDOW_ACKNOWLEDGMENT.md), and [effect startup color/quad verification](effect/FIELD_ITEM_GET_TYPE6_STARTUP_COLOR_AND_QUAD_VERIFICATION.md).

- [Audience Main inline owners and ordinary empty domains](battle/AUDIENCE_MAIN_INLINE_OWNERS_AND_ORDINARY_EMPTY_DOMAINS.md): complete subservice order, window/throw side effects and corrected WinSetActive ownership.

- [Native harbor hit-reaction ownership](battle/NATIVE_HARBOR_HIT_REACTION_OWNERSHIP.md).

- [Native original item95 windows](display/NATIVE_ITEM95_WINDOWS.md) and [source frame/display/font order](message/ITEM95_GET_WINDOW_DISPLAY_ORDER_AND_FONT_COLORS.md).

- [Native reaction attributes, facing, flags and data children](battle/NATIVE_REACTION_ATTRIBUTES_FACING_AND_FLAGS.md).

- [Audience Init resources and live-domain bootstrap](battle/AUDIENCE_INIT_ORDER_RESOURCES_AND_LIVE_DOMAIN_BOOTSTRAP.md): complete initializer requests and real TPL/noise/window history.
- [Exact setup4 zero-count operation order](battle/AUDIENCE_SETUP4_ZERO_COUNT_EXACT_OPERATION_ORDER.md): corrected successful admission,107 draws and300 no-draw requests.
- [Nonzero audience target chunks and rounding](battle/AUDIENCE_NONZERO_TARGET_ADJUSTMENT_CHUNKS_AND_ROUNDING.md): source signed chunks, ratio updates and outer cancellation/clamps.
- [Native audience startup and resource owners](battle/NATIVE_AUDIENCE_STARTUP_AND_ORIGINAL_RESOURCE_OWNERS.md): actual initialized empty domain, original TPL payloads and persistent trio RNG.

- [Native original item-get effect](display/NATIVE_ITEM_GET_EFFECT6.md), [GX state/projected texture](effect/FIELD_ITEM_GET_TYPE6_GX_STATE_AND_PROJECTED_TEXTURE.md), and [source camera5 producer/projection](camera/FIELD_ITEM_GET_CAMERA5_PRODUCER_AND_PROJECTION.md).

- [Tutorial damage amount and reaction marker](battle/TUTORIAL_HIT_DAMAGE_AMOUNT_AND_REACTION_MARKER.md)

- [CheckDamage event wrapper and calling-event ownership](battle/CHECK_DAMAGE_EVENT_WRAPPER.md)

- [Tutorial ordinary Crump impact](battle/TUTORIAL_ORDINARY_CRUMP_IMPACT.md)

- [Original item95 registry/icon](field/ITEM95_ENTRY_NAME_REGISTRY_AND_ICON_DRAW.md), [map blend/shadow admission](display/ITEM95_ICON_MAP_BLEND_AND_SHADOW_ADMISSION.md), [native parcel icon provider](display/NATIVE_ITEM95_REGISTRY_AND_ICON.md), and [original fanfare/BGM queries](audio/FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md).

- [Audience window, sound pass and public registry port](battle/AUDIENCE_WINDOW_SOUND_PASS_AND_PUBLIC_REGISTRY_PORT.md): source state updates, persistent registry contract and pending native integration.
- [Live audience sound/window/throw/display admission](battle/AUDIENCE_LIVE_SOUND_WINDOW_THROW_AND_DISPLAY_ADMISSION.md).
- [Ending slot/display and public sound owners](battle/ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md).

- [Horizontal battle camera shake request](battle/CAMERA_HORIZONTAL_SHAKE_REQUEST.md)

- [Original key-item fanfare container loop and stream retirement](audio/FIELD_KEY_ITEM_FANFARE_CONTAINER_LOOP_AND_RETIREMENT.md).

- [Native AudienceMain and public sound owner](battle/NATIVE_AUDIENCE_MAIN_AND_PUBLIC_SOUND_OWNER.md): integrated source recount/throw/window/sound path and next slot boundary.
- [Audience live count, AP leaves and sound failure adapters](battle/AUDIENCE_LIVE_COUNT_AP_EMPTY_LEAVES_AND_SOUND_FAILURE_ADAPTERS.md).

- [Native original attack-command selection](battle/NATIVE_ATTACK_COMMAND_SELECTION.md)

- [Native slot initialization, idle pass and table owners](battle/NATIVE_BREAK_SLOT_INITIALIZATION_IDLE_AND_ARRAY_OWNERS.md): real constructor allocations and next display boundary.
- [Surviving-unit HP gauge and slot heap lifetime](battle/SURVIVING_UNIT_HP_GAUGE_MAIN_AND_SLOT_HEAP_LIFETIME.md).

- [Original fanfare stereo gain/group fade](audio/FIELD_KEY_ITEM_FANFARE_STEREO_GAIN_AND_GROUP_FADE.md) and [native key-item fanfare provider](audio/NATIVE_KEY_ITEM_FANFARE_PROVIDER.md).
- [Surviving-unit status effects, icon gates and part frontier](battle/SURVIVING_UNIT_STATUS_EFFECT_ICON_AND_PART_FRONTIER.md).

- [Native action registration and status-window requests](battle/NATIVE_ACTION_REGISTRATION_AND_STATUS_WINDOW.md)

- [Native HP gauge history and display prefix](battle/NATIVE_HP_GAUGE_HISTORY_AND_DISPLAY_PREFIX.md): original timer producers, gauge math, status gates and pose frontier.
- [Unit alpha construction and display size lanes](battle/UNIT_ALPHA_CONSTRUCTION_AND_DISPLAY_SIZE_STATUS_LANES.md).
- [Forced part-pose display rate and animation frontier](battle/FORCED_PART_POSE_DISPLAY_RATE_AND_ANIMATION_FRONTIER.md).
- [Battle pose pool provisioning and native admission](display/BATTLE_POSE_POOL_PROVISIONING_AND_TYPED_NATIVE_ADMISSION.md).

- [Shared original weapon-table production and eligibility](battle/NATIVE_WEAPON_TABLE_PRODUCTION.md)

- [Source BGM secondary/tail boundaries](audio/FIELD_BGM_SECONDARY_BOUNDARIES_AND_DETACHED_TAIL.md), [plaza BGM/resume owner](audio/ROGUEPORT_PLAZA_INIT_BGM_AND_ITEMGET_RESUME_OWNER.md), [global percentage initialization/slew](audio/FIELD_BGM_GLOBAL_PERCENTAGE_INITIALIZATION_AND_SLEW.md), and [portable/native item-audio envelopes](audio/PORTABLE_FIELD_BGM_ENVELOPES_AND_NATIVE_ITEM_AUDIO.md).

- [Native item-get audio validation, artifacts and handoff](audio/NATIVE_ITEM_GET_AUDIO_VALIDATION_2026_10_14.md).

- [Native battle part storage and source pose directories](display/NATIVE_BATTLE_PART_STORAGE_AND_POSE_DIRECTORY.md) — distinct Mario/Goombella records, original default art and full descriptor identity.
- [Battle pose entry, release and offset services](display/BATTLE_PART_POSE_ENTRY_RELEASE_AND_OFFSET_SERVICES.md) — original three-part Mario construction, resource counts and next effect frontier.
- [Weapon-selection callers and cursor domains](battle/WEAPON_SELECTION_CALLERS_AND_CURSOR_DOMAINS.md) — static callers, type words and unresolved runtime reachability.
- Event recovery: [Jump Continue](event/BTLEVTCMD_JUMP_CONTINUE.md), [Jump Position](event/BTLEVTCMD_JUMP_POSITION.md), [Jump Setting](event/BTLEVTCMD_JUMP_SETTING.md), and [event-entry construction](event/EVT_ENTRY_TYPE_CONSTRUCTION.md).
- [Goomba3 item-get field services and event resume order](field/GOOMBA3_ITEMGET_FIELD_SERVICE_AND_EVENT_RESUME_ORDER.md).

- [Tutorial native weapon-table providers and pouch ownership](battle/TUTORIAL_NATIVE_WEAPON_TABLE_BINDINGS.md)

- [Shared original weapon-menu setup and decision](battle/NATIVE_WEAPON_MENU_SETUP_AND_DECISION.md)

- [Goomba3 portable parcel acquisition owner](field/GOOMBA3_PORTABLE_ACQUISITION_OWNER.md) — real pouch/registry ownership, delayed quest write and native binding frontier.

- [Original weapon-submenu input and audience gate](battle/NATIVE_WEAPON_SUBMENU_INPUT_AND_AUDIENCE_GATE.md)

- [Game acquisition binding and snapshot ownership](field/GOOMBA3_GAME_ACQUISITION_BINDING.md).
- [Item95 motion15 entry, display and deferred stay](field/ITEM95_MARIO_MOTION15_ENTRY_DISPLAY_AND_STAY.md).

- [Native battle pose resource owner](display/NATIVE_BATTLE_POSE_RESOURCE_OWNER.md) — real player-part handles, complete source indices, shared CPU banks and the Main boundary.
- [Part pending modes and automatic status selection](battle/PART_PENDING_POSE_MODES_AND_AUTOMATIC_STATUS_SELECTION.md) — original effect gates, status priority and unchanged extra-part descriptor0.

- [Item95 Mario motion and control provider](field/NATIVE_ITEM95_MARIO_MOTION_PROVIDER.md) — deferred source entry, horizontal-speed mapping, post readiness and front-bank routing.

- [Mario initial sound handles and item-get post stop](audio/MARIO_INITIAL_SOUND_HANDLES_AND_ITEMGET_POST_STOP.md).
- [Native Mario motion sound posts](audio/NATIVE_MARIO_MOTION_SFX_POSTS.md) — source retained handles, field-only NDSP channels, pending/inactive cancellation and native binding.

- [Original item95 camera/key/system fields](field/ITEM95_CAMERA_KEY_AND_SYSTEM_LEVEL_PROVIDERS.md) and [native Goomba3 parcel acquisition](field/NATIVE_GOOMBA3_PARCEL_ACQUISITION.md) — original windows/effect/icon/audio binding, independent simulation owners and isolated native grant validation; remaining providers explicit.

| [AP report construction](battle/BATTLE_AP_RECOVERY_REPORT_BUILD.md) | Allocation/copy, audience-kind sum, original float factors, slot bonus, RNG and caller clear | Native bingo point increment, aftereffect and complete presentation/scheduling |

- [Goomba3 original coin callback](field/GOOMBA3_COIN20_PRESENTATION_CLOCK_WINDOW_AND_RETIREMENT.md), [row219 resource/nonloop retirement](audio/GOOMBA3_COIN_FANFARE_ROW219_RESOURCE_AND_NONLOOP_RETIREMENT.md), and [portable reward/coin binding](field/PORTABLE_GOOMBA3_REWARD_AND_COIN_CALLBACK.md) — source clocks, acknowledged grant and independent borrowed-window lifetime; native providers remain open.

- [Original coin403 animation/visibility/camera](display/COIN403_ICON_ANIMATION_VISIBILITY_AND_CAMERA5.md) and [native coin window/effect7 providers](display/NATIVE_COIN_WINDOW_AND_EFFECT7_PROVIDERS.md) — original text gates, cached glyphs, distinct effect kind and corrected camera5 request, with isolated raster/acquisition evidence.

- [Native original coin403 animation and camera publication](display/NATIVE_COIN403_ANIMATION_AND_CAMERA_PUBLICATION.md) — source hidden frame cycles, original four-frame atlas, icon-before-sound simulation and early retained camera5; isolated rendering/checkpoint evidence.

- [Native original coin fanfare219 provider](audio/NATIVE_COIN_FANFARE_ROW219_PROVIDER.md) — shared group1, exact original PCM/nonloop asset and retained field cursor; natural-EOS evidence and failed detached-tail probes explicitly scoped.

- [Native completion5376 checkpoint v9](format/NATIVE_REWARD5376_CHECKPOINT_V9.md) — independent optional completion, unchanged legacy masks/unknown migration and exact-size atomic recovery; native reward admission remains pending.

- [Native coin-get presentation services](field/NATIVE_COIN_GET_PRESENTATION_SERVICES.md) — retained Game callback/window, original icon/effect/audio/Mario services, independent deletion and explicit raw-clock adaptation; reward admission remains pending.

- [Native McGoomba reward admission](field/NATIVE_GOOMBA3_REWARD_ADMISSION.md) — original quest2/5406 guard, blocking coin20, ordered5376 completion, one outer key hold and same-visit Mario event binding.

- [Original window display/US cache](display/TROUBLE_CENTER_ORIGINAL_WINDOW_DISPLAY_AND_US_CACHE.md) — original frames, clipping, colors/width scales and64 US resources; native backend pending.
- [Original Trouble Center board selection/acceptance](field/TROUBLE_CENTER_BOARD_SELECTION_AND_ACCEPTANCE.md) and [portable menu producer](field/PORTABLE_TROUBLE_CENTER_MENU_PRODUCER.md) — original30-slot unlock order, one-based outputs/debug115 correction, exact input/icon requests; native case/window bindings remain pending.

- [Native original Trouble Center window and US cache](display/NATIVE_TROUBLE_CENTER_WINDOW_AND_US_CACHE.md) — four frames, original64-resource cache, proportional row pen, fresh source state reads and isolated drawing evidence; board admission remains pending.

- [Native retained Trouble Center presentation](display/NATIVE_TROUBLE_CENTER_RETAINED_PRESENTATION.md), [original local icon clock](display/TROUBLE_CENTER_RETAINED_2D_ICONS_AND_LOCAL_CLOCK.md) and [menu SFX bindings](audio/TROUBLE_CENTER_MENU_SFX_BINDINGS.md) — finite menu/icon bridge and original atlas imports; native case/acceptance/audio admission remains open.

- [Original sequence table and field/battle lifecycle](platform/ORIGINAL_SEQUENCE_TABLE_AND_FIELD_BATTLE_LIFECYCLE.md): recovered callback rows, fresh reads and ordered source Init/Main/Exit.
- [Native original sequence owner](platform/NATIVE_ORIGINAL_SEQUENCE_OWNER.md): separate current/requested ownership, genuine native synchronization, field Exit and truthful battle Init hold.

- [Original sound flag owner and ordered set/clear](audio/GAME_SOUND_FLAG_OWNER_AND_ORDERED_SET_CLEAR.md): explicit startup halfword, complete clear leaf and inherited BGM/ENV fade calls.
- [Native shared sound flag owner](audio/NATIVE_SHARED_SOUND_FLAG_OWNER.md): retained field/battle/letterbox state and real missing-fade/GPU continuation limits.

- [Native Trouble Center state/checkpoint v10](format/NATIVE_TROUBLE_CENTER_STATE_CHECKPOINT_V10.md) — lecture, all30 completion flags, working flags/bytes with legacy unknown migration; [original door/case9 admission](field/TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md) — story23 lock, authored collision names and priority/type lifetime.

- [Native finishing draw frame frontier](camera/NATIVE_FINISHING_DRAW_FRAME_BINDING_FRONTIER.md): active-frame binding, GPU library boundaries and original callback inventory.
- [Native finishing camera GPU adapter](camera/NATIVE_FINISHING_CAMERA_GPU_ADAPTER.md): full matrix/rotated rectangle mapping, actual dummy triangle and retained backend restoration.

- [Native class state and shadow frontier](display/NATIVE_CLASS_STATE_AND_SHADOW_FRONTIER.md): independent write masks, bounded late-depth class mapping, cached restoration and genuine shadow callback wait.

- [Shadow complete ordered setup and rejected-record continuations](display/SHADOW_DISPLAY_SETUP_AND_EMPTY_RECORD_CONTINUATIONS.md)
- [Shadow N64 texture44 resource and sampler](display/SHADOW_TEXTURE44_RESOURCE_AND_SAMPLER.md)
- [Shadow collision preparation captures and store order](display/SHADOW_COLLISION_PREPARATION_CAPTURE_AND_STORE_ORDER.md)
- [Native shadow collision cursor and original texture import](display/NATIVE_SHADOW_COLLISION_CURSOR_AND_TEXTURE_IMPORT.md)

- [Native Trouble Center front door/lock notice](field/NATIVE_TROUBLE_CENTER_FRONT_DOOR_AND_LOCK_NOTICE.md) and [original type4 kanban art/material](display/ORIGINAL_KANBAN_TYPE4_WINDOW_AND_LOCK_NOTICE.md) — distinct authored interior/collision state, story23 access and original sign message; complete case/board/animation/audio remains open.

- [Dummy TEV register0 and shadow inheritance](display/DUMMY_DRAW_TEV_REGISTER0_AND_SHADOW_INHERITANCE.md)
- [Shadow camera2 class5 ordered suffix](display/SHADOW_CAMERA2_CLASS5_ORDERED_SUFFIX.md)
- [Native ordered shadow setup and camera2 ray frontier](display/NATIVE_ORDERED_SHADOW_SETUP_AND_CAMERA2_RAY_FRONTIER.md)

- [Native Trouble Center diary and scoped event data](field/NATIVE_TROUBLE_CENTER_DIARY_AND_SCOPED_EVENT_DATA.md), [board provider boundaries](field/TROUBLE_CENTER_NATIVE_BOARD_PROVIDER_BOUNDARIES.md) and [original diary/type7 rendering](display/ORIGINAL_DIARY_TYPE7_WINDOW.md) — original help-window dependency and bounded data-only source closure; board/counter execution remains to bind.

- [Original audience population, seats, entry and animation ownership](battle/AUDIENCE_POPULATION_SEATS_ENTRY_AND_ANIMATION_OWNERS.md)

- [Original Trouble Center event runtime adapter](field/ORIGINAL_TROUBLE_CENTER_EVENT_RUNTIME_ADAPTER.md) — actual installer/board/counter words, immutable callback catalog and known switches; source world/message/menu/case services still require native binding.

- [Shadow downward ray, active battle maps and finite acceptance](field/SHADOW_DOWNWARD_RAY_ACTIVE_BATTLE_MAP_AND_FINITE_ACCEPTANCE.md)
- [Native battle HitObj registry and shadow preparation](battle/NATIVE_BATTLE_HIT_REGISTRY_AND_SHADOW_PREPARATION.md)

- [Audience Sound1/2 and original public pending allocation](audio/AUDIENCE_SOUND_START_AND_PUBLIC_PENDING_ALLOCATION.md)

- [Native retained shadow profile and target tracking](display/NATIVE_SHADOW_PROFILE_TARGET_TRACKING.md) — mutable startup rows, exact camera4 target capture and texture-cache frontier.
- [Shadow projection prefix and zero-counter admission](display/SHADOW_PROJECTION_PREFIX_AND_ZERO_COUNTER_ADMISSION.md) — unconditional GPU order, live GP color and restoration before counter gates.

- [Trouble Center retained presentation owner](field/TROUBLE_CENTER_RETAINED_PRESENTATION_OWNER.md): original event message/menu lifetimes, explicit simulation visits, fixed-world lecture anchor and remaining native bindings.

- [Native Trouble Center contact/presentation](field/NATIVE_TROUBLE_CENTER_CONTACT_AND_PRESENTATION.md): exact-node case admission, retained manager reentry, real pouch and native drawing/audio.
- [Trouble Center direction source correction](event/TROUBLE_CENTER_DIRECTION_CALLBACK_SOURCE_CORRECTION.md): fresh270-degree constants, handler1 endpoint, raw party selector and boot global evidence.

- [Native camera2 shadow projection and shared restoration](display/NATIVE_SHADOW_CAMERA2_PROJECTION_AND_RESTORATION.md) — retained readback/setup, full prefix, source GP color and counter0 End restoration.
- [Citro3D texture cache and viewport command adapter](display/CITRO3D_TEXTURE_CACHE_AND_VIEWPORT_COMMAND_ADAPTER.md) — installed GPU cache clear, deferred texture bindings and viewport/depth conversion.
- [Shadow frustum arithmetic and auxiliary profile owners](display/SHADOW_FRUSTUM_ARITHMETIC_AND_AUXILIARY_PROFILE_OWNERS.md) — original float formulas, auxiliary boot state and shared mutable profiles.
- [Shadow auxiliary setup and shared restore cursors](display/SHADOW_AUXILIARY_SETUP_AND_SHARED_RESTORE_CURSORS.md) — complete class0/2/7 source order, color masks and shared buffers.
- [Cylinder shadow per-record geometry and bounds cadence](display/CYLINDER_SHADOW_PER_RECORD_GEOMETRY_AND_BOUNDS_CADENCE.md) — three passes, original indexed arrays and camera1-only bounds.
- [Audience camera4 class1/class2 callback and empty bank](display/AUDIENCE_CAMERA4_CLASS1_CLASS2_CALLBACK_AND_EMPTY_BANK.md) — actual 17-call GX prefix, 200 fresh record admissions, palette/matrix/quad order.

- [Original audience draw, normals, flip and native atlas clamp](battle/AUDIENCE_DRAW_NORMALS_FLIP_AND_NATIVE_ATLAS_CLAMP.md)

- [Trouble Center actual quest1/4 actors and mutable door copy](field/TROUBLE_CENTER_QUEST1_4_WORLD_ACTORS_AND_DOOR_COPY.md) — original names, init branches, named position writes and house05 copy lifetime.
- [Native Borodo position and all-task cancellation](field/NATIVE_BORODO_POSITION_AND_ALL_TASK_CANCELLATION.md) — PMNPC009 names, retained collision/render position and original30 item lists.

- [Rogueport east Borodo alley camera/occlusion](field/ROGUEPORT_EAST_BORODO_ALLEY_CAMERA_AND_OCCLUSION.md) — source house-versus-alley zone selection and finite cutaway logic.

- [Native RAM admission interval prefilter](event/NATIVE_RAM_ADMISSION_INTERVAL_PREFILTER.md)

- [Garf original talk and registered/hidden actor admission](field/GARF_ORIGINAL_TALK_AND_REGISTERED_HIDDEN_ACTOR_ADMISSION.md) — all source quest/ordinary branches and flag800 admission boundaries.
- [Native Garf init and ordinary talk](field/NATIVE_GARF_INIT_AND_ORDINARY_TALK.md) — original c_monban actor/profile, exact story messages and pending house05 services.

- [Native cylinder shadow provider](display/NATIVE_CYLINDER_SHADOW_PROVIDER.md) — retained per-record matrix/TEV/three-pass/indexed geometry and bounds frontier.
- [Cylinder native cull, logic, blend and late depth](display/CYLINDER_NATIVE_CULL_LOGIC_BLEND_AND_LATE_DEPTH.md) — installed command semantics, source winding and RGBA6/RGBA8 limits.

- [House05 original installer, GW11 and NPC-list consumers](field/HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md) — inline case snapshots, false800 guards and actual material1800 writes.
- [Native retained NPC flags and draw suppression](field/NATIVE_RETAINED_NPC_FLAGS_AND_DRAW_SUPPRESSION.md) — active-name lookup, persistent static/wander flags and diagnostic native verification.

- [Native retained audience empty admission](display/NATIVE_RETAINED_AUDIENCE_EMPTY_ADMISSION.md) — actual guarded callback,16 native setup requests and200 fresh current-entry admissions; nonempty body remains held.

- [Trio lower scene prepared draw replay](display/TRIO_LOWER_SCENE_PREPARED_DRAW_REPLAY.md)

- [Native area door data and GW11](field/NATIVE_AREA_DOOR_DATA_AND_GW11.md) — original72-byte map-owned copy, once-only init and real Q1 parameter writes; cases/material consumers remain separate.

- [Native plain-RAM word access](event/NATIVE_PLAIN_RAM_WORD_FAST_PATH.md)

- [Retained audience item/AP callbacks](display/RETAINED_AUDIENCE_ITEM_AP_CALLBACKS.md) — real GX init,100-record bank admissions, complete local matrix/icon request cursors and native provider boundaries.
- [Ordinary battle part callback, matrix/material/history](display/ORDINARY_BATTLE_PART_CALLBACK_MATRIX_MATERIAL_AND_HISTORY.md) — corrected separate draw/history matrices, pose material stores, three draw passes and mandatory history tail.

- [Native named map flags and house05 init](field/NATIVE_NAMED_MAP_FLAGS_AND_HOUSE05_INIT.md) — actual source interior hide/hit disable, cached Citro3D visibility and query filters.
- [House05 incoming locals and installer frontier](field/HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md) — Bero child dependencies, exact known local overwrites and unresolved case bank.

- [Audience in-range triangle emission](display/AUDIENCE_IN_RANGE_TRIANGLE_EMISSION.md)

- [Native Bero tables and incoming query](field/NATIVE_BERO_TABLES_AND_INCOMING_QUERY.md) — exact original tables and real road-arrival LW1..8 publication, with residual bank still unknown.
- [Bero selected entry and incoming local bank](event/BERO_SELECTED_ENTRY_AND_INCOMING_LOCAL_BANK.md) — complete into_info values and later ordinary/save-load producer boundaries.

- [Mario trio published transform consumption](display/MARIO_TRIO_PUBLISHED_TRANSFORM_CONSUMPTION.md)

- [Retained native ordinary battle part display](display/NATIVE_ORDINARY_BATTLE_PART_DISPLAY.md) — actual borrowed callback matrix/material/pass/history continuation; native inner geometry hold.

- [Native Bero registration and collision rollback](field/NATIVE_BERO_REGISTRATION_AND_COLLISION_ROLLBACK.md) — valid-name registration, indexed/length/current-number values and transaction-owned filters.
- [Bero indexed query, length and registration state](overworld/BERO_INDEXED_INFO_LENGTH_AND_REGISTRATION_STATE.md) — source writer order, SW versus ID, hit flags and fixed-radius placement algebra.

- [Trio source camera4 lens consumption](display/TRIO_SOURCE_CAMERA4_LENS_CONSUMPTION.md)

- [Bero shared case pool, IDs and switch wrappers](overworld/BERO_CASE_POOL_IDS_AND_SWITCH_WRAPPER_LIFETIME.md) — real first-free allocation, separate BeroID/SW, priorities and wrapper lifetime.
- [Native case pool and Trouble installer](field/NATIVE_CASE_POOL_AND_TROUBLE_INSTALLER.md) — retained records, actual installed event reentry and remaining field-owner boundaries.

- [Bero initial-position locals and actor writes](overworld/BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md) — starting-tuple event quantization, actor/camera side effects and corrected recoveryY+37.

- [Native mode2 retained-lens publication](camera/NATIVE_MODE2_RETAINED_LENS_PUBLICATION.md)

- [Native inner pose state and cache submission](display/NATIVE_INNER_POSE_DRAW_STATE_AND_CACHE.md) — retained battle mode/map material state, original/effective pass, facing/context and real cache operations before traversal.

- [Native Bero position and start publication](field/NATIVE_BERO_POSITION_AND_START_PUBLICATION.md) — source placement math, raw floor query and encoded native road starts.
- [House05 outer-init bank and ROAD leaves](field/HOUSE05_OUTER_INIT_BANK_CLOSURE_AND_ROAD_LEAF_CENTERS.md) — actual root producer, source-proven ordinary residual locals and8 original leaf markers.

- [Native field camera4 publication and battle inheritance](camera/NATIVE_FIELD_CAMERA4_PUBLICATION_AND_BATTLE_INHERITANCE.md)

- [Native position callbacks and entry camera reset](field/NATIVE_POSITION_CALLBACKS_AND_ENTRY_CAMERA_RESET.md) — original sequential actor writes, null-party branch and live retained camera/road/shift owners.
- [Retained Bero initial-position event closure](overworld/BERO_RETAINED_INITIAL_POSITION_EVENT_CLOSURE.md) — exact original script span, six ROAD callbacks, mutable globals/recovery and motion25 gate.

- [Native original model group traversal](display/NATIVE_ORIGINAL_MODEL_GROUP_TRAVERSAL.md) — unchanged raw-model companions, source node SDK graph and retained ordinary traversal to real shape rendering.
- [SDK inverse-transpose and point matrix arithmetic](display/ANIMATION_SDK_NORMAL_AND_POINT_MATRICES.md) — singular/no-store behavior, reciprocal-estimate boundary and used seed alias grouping.

- [Audience member control and idle reactions](battle/AUDIENCE_MEMBER_CONTROL_AND_IDLE_REACTIONS.md): original21-state dispatcher, motion tail and whole idle turn/tilt/jump; explicit remaining state providers.

- [Audience idle breathing and decisions](battle/AUDIENCE_IDLE_BREATHING_AND_DECISIONS.md): whole Wait, distinct FPR0/FPR29 provenance, source reaction RNG and preceding-row helper.

- [Native retained Bero initial-position event](field/NATIVE_RETAINED_BERO_INITIAL_POSITION.md) — unchanged original root, mutable globals, recovery/motion25 stores and held entry ownership.

- [Native shape render continuation](display/NATIVE_SHAPE_RENDER_CONTINUATION.md) — retained defaults, actual indexed fans and bounded native camera/math binding.
- [Battle map light banks, presets and fog lifecycle](display/BATTLE_MAP_LIGHT_BANK_AND_PRESET_LIFECYCLE.md) — real32-record bank initialization/reset, selected battle-map fog/ambient, preset camera updates and live pose lighting admission.

- [Audience cheering, celebratory jumps and booing](battle/AUDIENCE_CHEERING_JUMPS_AND_BOOING.md): whole states4/5/6/9 with source RNG, interpolation, repeat and landing order.

- [Generic door entry and preceding gor02 installers](field/GENERIC_DOOR_ENTRY_AND_GOR02_PRECEDING_CASE_INSTALLERS.md) — exact original selectors, snapshots and activation dependencies.
- [Shared field door installer runtime](field/NATIVE_SHARED_FIELD_DOOR_INSTALLERS.md) — area-tagged original roots, retained cases and explicit supplied-bank continuation boundaries.

- [Native battle material-light binding](display/NATIVE_MAP_MATERIAL_LIGHT_BINDING.md) — original map bytes, bank owner, retained light selection and separate RGB/alpha channels.
- [GX channel control and color-register semantics](display/GX_CHANNEL_CONTROL_AND_COLOR_REGISTER_SEMANTICS.md) — original separate RGB/alpha versus combined dispatch, partial register writes and disabled vertex-material passthrough.

- [Audience item hold/throw, damage and escape](battle/AUDIENCE_ITEM_HOLD_THROW_DAMAGE_AND_ESCAPE.md): whole states7/8/10/12, correct item pointers/penalty, escape eligibility and throw-sound registry records.

- [Original gor02 parent, prefix closure and map-copy ordering](field/GOR02_ORIGINAL_PARENT_PREFIX_CLOSURE_AND_MAP_COPY.md).
- [Bero asynchronous group prefetch and actor preparation](overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md).
- [Native original area parent and retirement](field/NATIVE_ORIGINAL_AREA_PARENT_AND_RETIREMENT.md).
- [Native event retirement observation](event/NATIVE_EVENT_RETIREMENT_OBSERVATION.md).

- [Native animation texture binding](display/NATIVE_ANIMATION_TEXTURE_BINDING.md) — original TPL metadata, retained object/UV calls and matching resident atlas ownership.
- [Native animation texture state and atlas identity](display/NATIVE_ANIMATION_TEXTURE_STATE_AND_ATLAS_IDENTITY.md) — PMANIM3/4 live T24 lanes, cross-pose palette checks, exact package dimensions/UV mapping and real GPU residency boundaries.

- [Audience item creation, selection and lifecycle](battle/AUDIENCE_ITEM_CREATION_SELECTION_AND_LIFECYCLE.md): whole creation/100-item Main, player targeting, flight/receipt/bounce/cleanup and original presentation metadata.

- [Native original animation-group prefetch](display/NATIVE_ANIMATION_GROUP_PREFETCH.md).

- [Native last-stage color and fog continuation](display/NATIVE_MAP_MATERIAL_COLOR_AND_FOG.md): source byte arithmetic, retained camera/request order, shared primary map owner and bounded K0/fog GPU state.
- [Battle primary-map material ownership and fog retention](display/BATTLE_PRIMARY_MAP_MATERIAL_OWNERSHIP.md): primary versus background fog, fresh clear/color flags, exact disabled-table load behavior and retained camera pointer.

- [Original Bero key leaves and next ROAD actor frontier](field/BERO_KEY_LEAVES_FRESH_ACTOR_OWNERS_AND_NEXT_ROAD_FRONTIER.md).
- [Native shared Mario keys and background preparation](field/NATIVE_MARIO_KEY_AND_BACKGROUND_PREPARATION.md).

- [Mario background constructors and actual native consumers](field/MARIO_BACKGROUND_PREPARATION_CONSTRUCTORS_AND_LIVE_CONSUMERS.md).

- [Original matrix-icon services](display/ORIGINAL_MATRIX_ICON_SERVICES.md):
  retained whole iconDispGx2/Col, packed descriptor/GP frame selection and actual
  audience callback connection; native resource and iconGX binding remain open.

- [Original iconGX draw service](display/ORIGINAL_ICON_GX_DRAW_SERVICE.md):
  full local icon matrix, texture, material, fog, alpha and primitive sequence;
  linked through matrix-icon wrappers, native resource/raster binding open.

- [Native map TEV continuation and ordinary combiner](display/NATIVE_MAP_TEV_PROGRAM_AND_ORDINARY_COMBINER.md): fixed original request tables, retained optional boundaries/count publication and actual texture/raster/K0 Citro3D routing.
- [Original ordinary TEV SDK and shadow source domain](display/ORIGINAL_ORDINARY_TEV_SDK_AND_SHADOW_SOURCE_DOMAIN.md): retail GXSetTevOp table, three original draw-bank domains, corrected shadow getter ownership and stage-local PICA constant constraints.

- [Native ordinary body/rate/background consumers](field/NATIVE_MARIO_BODY_RATES_AND_BACKGROUND_CONSUMERS.md).

- [Previous-party native admission and retained first-call boundary](partner/BERO_PREVIOUS_PARTY_NATIVE_ADMISSION_AND_FIRST_CALL_RETENTION.md).

- [Native previous-party admission and shared attached position prefix](partner/NATIVE_PREVIOUS_PARTY_AND_SHARED_INITIAL_POSITION_PREFIX.md).

- [Shared Bero normal-position geometry and party owners](field/SHARED_BERO_NORMAL_POSITION_GEOMETRY_AND_PARTY_OWNERS.md) — actual requested-name/ROAD leaf resolution, collision readiness, mutable globals and distinct party current owner.

- [Native original indexed animation fan submission](display/NATIVE_ANIMATION_INDEXED_FAN_SUBMISSION.md): independent source array decoding, retained fan completion, ST11/atlas mapping and actual Citro3D triangle fans.
- [Native indexed-fan numeric and array bridge](display/NATIVE_INDEXED_FAN_NUMERIC_AND_ARRAY_BRIDGE.md): original ST11 texgen input, serialized attribute offsets, one model transform and one atlas adaptation.

- [Native original icon resources](display/NATIVE_ORIGINAL_ICON_RESOURCES.md):
  complete original directory/TPL, lossless cells, registered GP/published camera
  reads and bounded actual native texture residency; material/raster still open.

- [Original stage dark/light callbacks](display/BATTLE_STAGE_DARK_AND_LIGHT_CALLBACKS.md) — unconditional dark quads, retained stage alpha, light-entry loop and exact shared GX state; native callback binding remains open.

- [Shared normal-position and complete collision owner](field/SHARED_BERO_NORMAL_POSITION_AND_COMPLETE_COLLISION_OWNER.md) — staged room provenance, actual named ROAD geometry and same-VM actor/camera publication.

- [Stage-light SDK axis rotation/normalize](display/STAGE_LIGHT_AXIS_ROTATION_AND_NORMALIZE.md) — exact original arithmetic graph and explicit native estimate/libm boundary.
- [Native stage dark/light continuations](display/NATIVE_STAGE_DARK_AND_LIGHT_CONTINUATIONS.md) — retained live stage owners, actual direct quads/triangles and shared depth/blend/channel state.
