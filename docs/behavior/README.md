# Complete-game behavior documentation work register

**Version scope:** This directory documents the GameCube US binary. Current
Switch 1.0.0 research lives in [the separate Switch collection](../switch/README.md).
Do not import these contracts as established Switch behavior.

Objective: thoroughly document behavior not established by available
decompilation, then produce a system requirements document for the **complete
game**, assuming original models, textures, messages, sound and other data are
reused. No requirement is satisfied merely by listing a subsystem or its symbols.
This collection is in progress as of 2026-10-12.

The [current US completion assessment and priorities](US_COMPLETION_AND_PRIORITIES.md)
supersede older next-task lists: prioritize shipped event/command bindings and
complete gameplay sequences before further general-library edge-case recovery.

## Detailed documents and evidence boundaries

| Document | Concrete coverage | Still incomplete |
| --- | --- | --- |
| [Jump takeoff position](BATTLE_JUMP_TAKEOFF_POSITION.md) | Complete mixed coordinate/sample/closest-X/side-offset/output contract and two actual callers | Candidate cores/RNG/domain/source geometry and original position traces |
| [MoveFrame/MovePosition](BATTLE_MOVE_FRAME_AND_POSITION.md) | Complete count-query versus velocity-adjusted motion,first-call integration,XYZ snap/yield and actual callers | Numeric/source/sound/scheduler composition and original timing |
| [Unit/part position queries](BATTLE_UNIT_PART_POSITION_QUERIES.md) | Complete home/offset/world/scaled hit leaves,registry-part helper and event integer/sentinel wrapper | Actual source/scale/world/lifetime writers and original damage/position traces |
| [Movement sound control](BATTLE_MOVEMENT_SOUND_CONTROL.md) | Complete retained voice/position,alternating signed periods and stop/core wrappers | Sound trigger/playback/resources/source lifetime and original audio traces |
| [Event deletion and copy-back](EVENT_DELETE_AND_COPYBACK.md) | Complete child-first/brother scan,parent copy-back,clear/counter and DeleteID scan | Opcode/domain/source lifetime and original traces |
| [Stop/start traversal](EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md) | Complete descendant-first type masks,inlined context captures and All scans | Pause/UI/domain/source and original runtime traces |
| [Run and ID-output variants](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) | Complete92/93/108/111 copy/order/ownership and output semantics | Lifetime/control/domain and original traces |
| [Control opcodes and ID scans](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) | Child yield,95..106 wrappers,setters,other/ID scan and liveness policies | Full stop/start/delete traversal,other opcodes and original traces |
| [Child/brother/inline construction](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) | Full copy/reset/ownership maps and append,plain107/110 wrappers | ID variants,remaining controls/domain and original traces |
| [Restart preservation](EVENT_RESTART_PRESERVATION.md) | Exact execution reset,label rebuild and preserved locals/links/identity | Caller/control/deletion domains and original restart traces |
| [Root constructors and append](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) | Complete root/typed slot/init/ID/speed/credit and conditional live append | Child/brother/inline/restart/domain and original traces |
| [Label scan and entry run check](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) | Full root label scanner and all-event system-level stop dispatch | Other constructor/source/lifetime and original pause traces |
| [Complete event manager loop](EVENT_MANAGER_COMPLETE_LOOP.md) | Complete context/time delta, priority ordering, eligibility/identity and multicredit/return loop | Constructors/append/initial state,time sources and original traces |
| [Dispatch and wait continuations](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) | Exact fetch/common results, frame/msec waits and USER_FUNC first-call ownership | Other opcodes,numeric/domain/source and original traces |
| [Attack badge recovery tail](BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md) | Complete32-command HP/FP branch/brother/wait/deletion graph | Scheduling/delayed validity and original traces |
| [Badge recovery services](BATTLE_BADGE_RECOVERY_SERVICES.md) | Amount/resource/geometry wrappers, raw actor and narrowing/bookkeeping semantics | Source/lifetime/domain and original traces |
| [Badge recovery presentation](BATTLE_BADGE_RECOVERY_PRESENTATION.md) | Scoped quantized local floats/integer ADD and recovery catalog/request binding | Full namespaces/renderer/audio and original traces |
| [Weapon aftereffect](BATTLE_WEAPON_AFTEREFFECT_DISPATCH.md) | Complete weighted destruction and ordered forced/chance hazard dispatch, actual weapon bytes | Flag/seed/interleaving sources and executed hazards |
| [Stage request leaves](BATTLE_STAGE_AFTEREFFECT_REQUESTS.md) | Complete destruction/nozzle/frame/object event ownership and local helpers | Queued events, source/config/lifetime domains and original traces |
| [Stylish timing/results](BATTLE_STYLISH_TIMING_AND_RESULTS.md) | Complete shared counter, event first-call/polling, input bounds and result readers | Source/scheduler/collision ownership, audience and original traces |
| [Stylish timing icon](BATTLE_STYLISH_TIMING_ICON.md) | Exact eligibility, baseline queue depth and visible-position icon draw requests | Assets/display lifetime/projection and original presentation |
| [Enemy candidate sampling](BATTLE_ENEMY_CANDIDATE_SAMPLING.md) | Complete row geometry, admission/compaction/occlusion, count and permutation ordering | Source/helper/numeric/domain and original traces |
| [Candidate choice and RNG](BATTLE_ENEMY_CANDIDATE_CHOICE.md) | Complete flags/weapon weights, deterministic mode4, tie/draw boundary and shared RNG leaves | Seed/interleaving, helper/domain and original traces |
| [Selected target and weapon](BATTLE_SELECTED_TARGET_AND_WEAPON.md) | Retained selection query, complete cursor dispatch and command/item weapon lookup | Menu/selection producers and original execution |
| [Weapon cost and payment](BATTLE_WEAPON_COST_AND_PAYMENT.md) | FP scaling/reduction, shared owner, AP query/payment and byte action-record arithmetic | Menu cadence, numeric extremes, persistence and original execution |
| [Mario jump motion](BATTLE_MARIO_JUMP_MOTION.md) | Complete local first/resumed-call phases,baseline/visible integration,zero/exact-count landing and sound/AC request ownership | Numeric/audio helper/domain/caller composition and original trajectory/input traces |
| [Mario jump numerics](BATTLE_MARIO_JUMP_NUMERICS.md) | Ordered single/double/fused formulas,mode division/height/phase branches,constants and complete local reviseAngle wrapper | Original sin/fmod/angle/distance/FPSCR execution and composed comparisons |
| [Unit sound-mode loader](BATTLE_UNIT_SOUND_MODE.md) | Complete type/mode partial record initialization,retained gaps and concrete Mario setup playback input | Source/key/macro/voice writers,playback core/resources and original audio traces |
| [Mario Jump parameter](BATTLE_MARIO_JUMP_PARAMETER.md) | Complete integer coordinate resolution,distance correction,raw count writeback and updated stylish caller operands | MarioJumpPosition body,original numeric execution and caller/timing traces |
| [Ordinary Jump event flow](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) | All268 request commands/branches,per-hit flags/count loop,first-attack setup,stylish/feedback and cleanup ownership | Mario-specific motion/service internals,per-hit writers,actual tutorial/cadence and original traces |
| [Jump result/stop services](BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md) | Complete result/count/window/mask/grade wrappers,prize code/byte clamp and deferred TimingA stop | Grade/option/code producers,help UI and actual callers/traces |
| [Jump VM/concurrency](EVENT_JUMP_BRANCHES_AND_CONCURRENCY.md) | Signed comparison/raw mask bodies,CASE>=,attached brother versus independent inline creation/end rules | Full constructor/other opcode/arithmetic/scheduler/pause behavior and original traces |
| [Unit badge/count import](UNIT_BADGE_COUNT_IMPORT.md) | Complete import/reset/scan and numeric byte maps,wrapped special thresholds,scoped Mario/partner/enemy setup bindings | Inventory equip/new-game rules,other callers and executed guard/resource comparisons |
| [Unit status/pouch initialization](UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) | Complete status clear,pouch resource import/revision,party mapping and partial six-record reset with entry callers | Badge/move/snapshot producers,turn/presentation/save lifetime and original traces |
| [Unit part defense/lifetime](UNIT_PART_DEFENSE_AND_LIFETIME.md) | Scoped contiguous part construction/pointer copies,Crump/Mario signed source tables,complete part lookup and deletion request order | Remaining entry/vector/heap/pose services,mutation and original actor lifetime traces |
| [Weapon power callbacks](BATTLE_WEAPON_POWER_CALLBACKS.md) | Complete default/Mario Jump/Hammer power bodies,signed level/index/bonus ordering,ordinary six-word tables and boss base1 | Level/bonus/AC producers,weapon lifetime,unit/part construction and executed hit totals |
| [US completion/priorities](US_COMPLETION_AND_PRIORITIES.md) | Current completion audit,shipped reference census,registered commands and mechanic-first order | Full event feasibility/original execution and all-system closure |
| [Field battle entry/return](FIELD_BATTLE_ENTRY_AND_RETURN.md) | Area/index registration,NPC setup/random draws,first-call start and field-result polling | Service/lifetime/result producers and original transitions |
| [Lecture player attacks](LECTURE_PLAYER_ATTACK_FLOW.md) | Default lecture weapon substitution,Jump/Hammer teaching polls,feedback slots,mode1→2/5→6 writers and13-command end requests | Full menu/end/runtime/field controller and original input/presentation traces |
| [Lecture field entry/return](LECTURE_FIELD_ENTRY_AND_RETURN.md) | Story10 wall/reload entry,crossing operands,exact US choices/setup3 launch and shared story11 exit | Intersection and party services,persistent writers,battle teardown and original traces |
| [Field intersection/coordinates](FIELD_SEGMENT_INTERSECTION.md) | Complete segment/query bodies,endpoint and fused-product branches,used local encoding and lecture Y-copy composition | Full variable namespace,FPSCR/coordinate writers,scheduler and original traces |
| [Field party callbacks](FIELD_PARTY_EVENT_CALLBACKS.md) | Raw slot clear/kill dispatch,first-call constructor request,stop/run pointer selection and local mode writes | Constructor/destructor/saved-kind/mode-init lifetimes and original transitions |
| [Field party destruction](FIELD_PARTY_DESTRUCTION_AND_SAVED_KIND.md) | Complete Kill/Kill2 linked-kind hook dispatch,saved-kind difference,slot/pointer clear and ordered release/free | All-kind hooks,heap/pose internals,index/owner invariants and original traces |
| [Field party constructors](FIELD_PARTY_POSITION_CONSTRUCTORS.md) | Complete main/position/secondary graphs,allocation and init/pose/resource requests,kind1/8 init and coordinate tails | All-kind tables,called services,resource/index/mode lifetime and original transitions |
| [Field party modes](FIELD_PARTY_MODE_INITIALIZATION.md) | Old-mode cleanup/restore graph,flag/vector/pose requests,jump clear and slit-angle loops | Full mode update/pose/paper/hooks/domain and original execution |
| [Field party driver/index](FIELD_PARTY_DRIVER_AND_INDEX_OWNERSHIP.md) | Two-slot initialization,count/reread/update order,Mario bit gate,BSS and scoped direct-index census | Global updater/driveParty/callers,alias/REL/index/slot writers and original traces |
| [Status-window arbitration](BATTLE_STATUS_WINDOW_ARBITRATION.md) | Four complete local bodies,independent blocker/show fields,event argument dispatch and lecture2740 consumer | Other writers,inner window/render/animation services,lifecycle and original UI observations |
| [Goombella helper roles](CHRISTINE_HELPER_ROLES.md) | Distinct Crump NPC/lecture kinds and setup3,init hooks,cross-linked IDs,selector-6,mode/result leaves and counter/attack transitions | Full player/counter dispatch,field entry/return,lifecycle and original traces |
| [Lecture coordinator modes](LECTURE_COORDINATOR_MODES.md) | Full professor init/200-command phase graph,exact work/phase transitions,20 US message sites and speaker/choice bindings | Missing player-mode producers,flag consumers,services and full original lecture execution |
| [Input reset owners](BATTLE_INPUT_RESET_OWNERS.md) | Direct sampler/reset census,three complete reset helpers and relocated tutorial history injection/case bindings | Indirect/bulk writers,reset lifetime,lecture selection and original multihit traces |
| [System-level/event speed](EVENT_SYSTEM_LEVEL_AND_SPEED.md) | Complete level table/getter,multiplier setter,creation-speed regions and scheduler consequences | Full UI/pause caller arguments,type/speed writers,restart and original traces |
| [Physical input producer](PHYSICAL_INPUT_PRODUCER.md) | Raw read/copy/error handling,strict48 directions,global edges,scaled24/6 repeats,input-clear chord and motor request order | SDK hardware/polling,control writers and original traces |
| [Battle input cadence/snapshots](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md) | General→sequence→private→AC→event order,12-byte write map,wrapper/result gates and shared-sample credit boundaries | Complete pause/main frequency,other samplers/resets,override producers and original timing |
| [Battle jump callbacks](BATTLE_JUMP_EVENT_CALLBACKS.md) | JumpSetting/Position/Continue,integer target resolution,first-call integration,strict floor completion,storage and actual Crump count overrides | Main/pad/event cadence,angle/dist/trig/audio/stage producers and original hit ticks |
| [Boss normal attack](HARBOR_BOSS_NORMAL_ATTACK.md) | Full 95-command target/approach/precheck/defense/recovery request graph and missing-label99 boundary | Inner target/movement/input/damage services and original timing |
| [Target event interfaces](BATTLE_TARGET_EVENT_INTERFACES.md) | Seven complete local wrappers/helpers,mode pair map,part ID,side narrowing and sampling ABI | Candidate cores/mode/flag producers,buffer lifetime and original selection traces |
| [Unit entry stats/spawn](UNIT_ENTRY_STATS_AND_SPAWN.md) | Entry slot/publication and HP/FP copy,getter ABI,spawn ordering and init ownership;boss HP5 link | Rest of entry/parts,overrides,lifetime and original comparisons |
| [Action feedback/stage gate](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md) | Declaration counters/cap,success record,initial reset/order and tutorial flag stage-registration suppression | Other writers/audience/init/flag services and original traces |
| [Harbor boss tutorial events](HARBOR_BOSS_TUTORIAL_EVENTS.md) | Installed init/phase/feedback hooks,priority/latches,death-message order and enemy defense binding | Full stats,feedback writers,flag consumers,attack/reward/defeat and original traces |
| [Tutorial state services](BATTLE_TUTORIAL_STATE_SERVICES.md) | Unit-work slots,exact phase/turn checks,raw-mask flags and two relocated feedback-byte readers | Producers/lifetimes,counter semantics and original comparisons |
| [Harbor tutorial configuration](HARBOR_TUTORIAL_CONFIGURATION.md) | Prolog/index2 binding,single group/unit,kind/init pointers and17 known setup draws | Tutorial scripts,stat/runtime overrides,rewards and original traces |
| [Harbor opening bindings](STORY_HARBOR_OPENING_BINDINGS.md) | Seven original REL spans,story-state entry/write graph,actor setup,20 message/speaker references and battle boundary | Complete scene/event/service conditions,tutorial config and original traces |
| [Original baseline](../ORIGINAL_BEHAVIOR.md) | Disc/DOL identity, evidence rules, subsystem order, basic waits/callback continuations, initial complete-game scope | Most systems beyond the initial recovered contracts |
| [Event runtime](EVENT_RUNTIME.md) | Priority/tie order, child/brother relationships, deletion/copy-back, selected stop behavior | Every opcode/variable type, service callbacks, restart/pause lifecycle |
| [Ordinary attack bindings](BATTLE_ORDINARY_ATTACK_BINDINGS.md) | US Jump ID1/Hammer core ID3 setup sites,raw/resolved wrapper ABI and event stream identities | Full branches,unit/weapon/tutorial selection and timing |
| [Hammer weapon branches](BATTLE_HAMMER_WEAPON_BRANCHES.md) | Mask2 target/setup split,pointer-selected charge periods,weapon difficulty and wrapper/core links | Full attack branch/inline/weapon consumer and tutorial/original traces |
| [Integer IF/switch](EVENT_INTEGER_BRANCHES.md) | Used integer equality,branch scanners,snapshot switch,case no-fallthrough/default/pop | Other VM variants,full contexts and original branch execution |
| [Hammer StickKeepLeft](BATTLE_HAMMER_STICK_KEEP_LEFT.md) | ID3 lifecycle,held-left/stage/release rules,difficulty allowances and finish order | Caller branch/cadence/forced routes,presentation and original comparisons |
| [Action-command callbacks](BATTLE_ACTION_COMMAND_CALLBACKS.md) | Eight resolved parameters/sentinel,raw mode,start and pending-result protocol;Hammer data bindings | Other setters,help internals and complete script/weapon gates |
| [TimingA command](BATTLE_TIMING_A_COMMAND.md) | Complete local lifecycle/window/result and general pad history routing;seven difficulty tables | Producers,impact/cadence/display and original comparisons |
| [Battle input and defense](BATTLE_INPUT_AND_DEFENCE.md) | Original history buffers, virtual directions/repeat, complete local guard decision routine and tables | Caller gates, impact sampling, attacks, full damage/status/turn rules |
| [Hit precheck](BATTLE_HIT_PRECHECK.md) | Ordered eligibility, per-copy evasion draws, accuracy reductions and return codes | Caller reactions, status lifetimes, stage-effect producer |
| [Damage calculation](BATTLE_DAMAGE_CALCULATION.md) | Ordered HP/FP callback gates, badge/status/defense arithmetic, result codes, absorption and revival marker | Weapon callbacks, death/revival execution and turn lifecycle |
| [Damage orchestration](BATTLE_DAMAGE_ORCHESTRATION.md) | Complete local request order, superguard variants, event/result selection, retained RNG draws and two gating/selection helpers | Global flag producers, audience/effects, weapon bindings and battle turn callers |
| [Counters](BATTLE_COUNTERS.md) | Complete local selection order, work record, jump-table returns, consumer accumulation/status/direct-damage calls | Other callers, counter event bodies, effect internals and weapon bindings |
| [Damage application](BATTLE_DAMAGE_APPLICATION.md) | Ordered effects/audio, HP/FP mutation, shared FP owner, Pity Flower/Sleep draws, bookkeeping and lethal event-code selection | Helper internals, recovery scheduling and death/revival event execution |
| [Battle event dispatch](BATTLE_EVENT_DISPATCH.md) | Hit cancellation/replacement, data-key gate, wait/phase scheduling, code getters and raw/resolved operands | Installed event graphs, death/revival bodies, controller lifetime and remaining services |
| [Message resources](MESSAGE_RESOURCES_AND_LOOKUP.md) | US pair census, loading/index sorting, slot lookup/fallback, command split and lead-byte predicate | Lifetime/callers, full controls/glyphs, duplicate ordering and story event coverage |
| [Message parser records](MESSAGE_PARSER_RECORDS.md) | Append records, glyph/line layout, terminal state, wait/style/window controls and selection setup | Font tables, record consumers, ABI ambiguity and story conditions |
| [Message embedded objects](MESSAGE_EMBEDDED_OBJECTS.md) | US icon binding table, image geometry versus HM glyph, pose/paper records, annotation layout and literal closing gate, selection argument census | Font/icon/pose/NPC services, library ABI, runtime consumers and generated/non-US inputs |
| [Message playback](MESSAGE_PLAYBACK.md) | Clock/scroll/schedule advancement, waits/return codes, full opcode map, history/callbacks, paper and per-call audio admission | State producers/caller cadence, rendering/selection, runtime numerics and service lifetimes |
| [Message window lifecycle](MESSAGE_WINDOW_LIFECYCLE.md) | Seven-slot allocation/poll flags, creation/geometry, append/partial replay, deletion/force close and event repeat/continue polling | Rendering/manager cadence, print identity/callback producers, allocators and cleanup |
| [Message window manager](MESSAGE_WINDOW_MANAGER.md) | Partial pool reset, ordered live-slot update, caller placement, archive/DVD palette paths and unchecked texture lookup | Outer/display cadence, renderer, allocator/loading services and actual reset/story callers |
| [Selection drawing](MESSAGE_SELECTION_DRAWING.md) | Fog/border/text/icon request order, opacity narrowing, geometry and draw-time fused cursor interpolation | Inner renderer/service contracts, display cadence, bindings and original draw comparisons |
| [General window drawing](MESSAGE_WINDOW_DRAWING.md) | All type routes, anchor/interpolation, type1 stored geometry, clipping/restoration and indicator pose requests | Border/projection/pose services, actual cadence and original images |
| [Draw record admission](MESSAGE_DRAW_RECORD_ADMISSION.md) | Exposed prefix/key result, controls, history visibility/fades, pose/icon requests, glyph admission/emphasis and indicator callback | Inner font/pose/icon/matrix services, cadence and original images |
| [Glyph effects](MESSAGE_GLYPH_EFFECTS.md) | Priority gates, two-draw jitter, dynamic matrices/relaxation, clock motion and selection draw/state order | Numeric/trig/matrix/font helpers, seeding/cadence and matched original traces/images |
| [Font draw state](FONT_DRAW_STATE.md) | Fourteen helpers, shared raw color/alpha/scale/edge, caller-buffer mutation, palette/start reset and effect GX requests | Inner glyph/GX raster services, palette/texture writers/bindings and original images |
| [Font glyph/setup requests](FONT_GLYPH_REQUESTS_AND_SETUP.md) | Ordinary/in-place matrices, edge five-draw/color policy and complete JUT setup requests | Font/page data, HSV/GX/matrix services, producers/cadence and original images |
| [Inner glyph quads](FONT_GLYPH_QUADS.md) | WID1 width lookup, signed grid/quad attributes, two unconditional RNG draws, hue requests and FIFO vertex order | Font/page data/loading, GX/FIFO semantics, cadence and original images/seed traces |
| [Font resource setup](FONT_RESOURCE_SETUP.md) | Lazy load/probes, descriptor selection, texture requests, original BFN shapes and ordered width patches | DVD/root/service execution, selectors/callers and original images |
| [Font code mapping](FONT_CODE_MAPPING.md) | First MAP1 range, four mapping types, sparse search bounds, replacement and byte/halfword wrappers | Text-stream decoding, executed mapping corpus and actual callers |
| [Message width scanner](FONT_MESSAGE_WIDTH.md) | Byte/lead routes, LF count, wrapping glyph advances, icon fused width and command LF suppression | Scanf/runtime resources, drawing layout consumers and executed comparisons |
| [Icon dimensions](ICON_DIMENSIONS.md) | Ready gates, locale remaps, binary offset directory, both-zero original-ID texture fallback and output order | Resource loading/relocation, valid IDs/configurations and actual draw/animation consumers |
| [Affine matrix services](AFFINE_MATRIX_SERVICES.md) | Scale,translation application,paired-single concatenation,rounding boundaries and alias/write order | GQR0/FPSCR runtime state,GX consumers and executed matrix/image comparisons |
| [String pitch drawing](FONT_STRING_PITCH_DRAWING.md) | Complete vector/scalar wrappers,byte/tag routing,proportional/fixed matrices,outline/color and pen advancement | Other string callers,matrix/raster/cadence and executed images/RNG |
| [HSV color conversion](HSV_COLOR_CONVERSION.md) | Byte input/output, hue sectors, single-precision/fused arithmetic and alpha preservation | Floating environment and executed vertex/image comparisons |
| [Floor numeric service](FLOOR_NUMERIC_SERVICE.md) | Binary64 exponent/mantissa branches, signed zero, negative carry and special-value arithmetic | FPSCR/traps and executed numeric comparisons |
| [Display queue/dispatch](DISPLAY_QUEUE_AND_DISPATCH.md) | Record/key construction, sort wrapper/comparator, live camera filtering/callbacks, GX request map and depth helper | Outer cadence, camera/matrix/GX services and executed rendering |
| [Display sort implementation](DISPLAY_SORT_IMPLEMENTATION.md) | Global scratch, small pivot branches, strided sample selection, partition/recursion, cycle permutation and equal-key reversal | Actual caller cadence, copy service domains and executed sort/draw traces |
| [Message selection](MESSAGE_SELECTION.md) | Opening/closing, trigger versus repeat navigation, cancel/confirm order, creation identity, signed event result and window deletion | Renderer/highlighting, input/manager cadence, caller/story conditions and print wait producers |
| [Message window states](MESSAGE_WINDOW_STATES.md) | General/clear state transitions, fades/sounds, talk countdown, wait resume, history controls and deletion | Rendering/manager cadence, print callbacks, pose/numeric/input services and story conditions |
| [Message print events](MESSAGE_PRINT_EVENTS.md) | Four resolved operands, first-call identity/owner/layer, NPC/sentinel/battle speakers, placement/projection and poll completion | Variants/substitution/callbacks, placement producers, helper services and story call graphs |
| [Message print variants](MESSAGE_PRINT_VARIANTS.md) | NPC/layer/placement setters, field/battle partner suffixes, variant first-call gates, append and borrowed key buffers | Insert substitutions/callbacks, character/caller census, buffer/service lifetimes and story conditions |
| [Message insert formatting](MESSAGE_INSERT_FORMATTING.md) | Encoded argument count, word resolution, one-to-four sprintf dispatch, direct/append consumers and US percent lexeme census | Original printf ABI/parser, call-site types/counts, other substitutions/callbacks and story conditions |
| [Scan input runtime](SCAN_INPUT_RUNTIME.md) | sscanf ABI/empty-input gate and string read/unread/EOF protocol | Numeric/multibyte dependencies and executed caller comparisons |
| [Unsigned32 scan helper](SCAN_INTEGER32.md) | Sign/base states, whitespace/width accounting, wrapped magnitude/overflow, prefix failure and pushback | Executed callback corpus, public callers and 64-bit/floating helpers |
| [Unsigned64 scan helper](SCAN_INTEGER64.md) | Sign/prefix/count states, divide threshold, word multiply/carry, sticky overflow and unread/return ABI | Executed callback corpus, public callers and floating/multibyte helpers |
| [Multibyte conversion](MULTIBYTE_CONVERSION.md) | Complete length/error/continuation checks, halfword decode and canonical-length test, scan size1 composition | Executed byte/access/output corpus and actual wide-format caller coverage |
| [Float scan entry/special values](SCAN_FLOAT_ENTRY_SPECIAL_VALUES.md) | Read-before-width, leading signs/whitespace, INF/INFINITY and NAN payload loops, count/pushback distinctions | All finite states/conversion exits, FPSCR and executed float argument corpus |
| [Float decimal scan](SCAN_FLOAT_DECIMAL.md) | Mantissa/exponent states,20-digit retention,scale/trim,handoff and status/sign/range exits | Hex states,__dec2num and executed rounding/argument corpus |
| [Float hexadecimal scan](SCAN_FLOAT_HEXADECIMAL.md) | Nibble packing/states,exponent wrap,normalization/bit insertion,count rewrite and range/sign exits | __dec2num for decimal path,executed callback/bit corpus and actual callers |
| [Decimal-to-binary special entry](DECIMAL_TO_BINARY_SPECIAL_VALUES.md) | Count/first-byte precedence,signed zero/infinity and exact N payload packing | Finite approximation/refinement,execution/FPSCR and scanner composition |
| [Decimal-to-binary approximation](DECIMAL_TO_BINARY_APPROXIMATION.md) | Private digit copy,exponent/chunk arithmetic,early exit,five/two scaling and candidate classification | pow,boundary selection/refinement/final rounding and execution corpus |
| [Decimal infinity boundary](DECIMAL_TO_BINARY_INFINITY_BOUNDARY.md) | Fixed boundary record,strict magnitude comparison,infinity retention/finite recovery and sign exit | General refinement/ties,pow and executed final-bit corpus |
| [Decimal equality/refinement entry](DECIMAL_RECORD_EQUALITY_AND_REFINEMENT_ENTRY.md) | Complete numeric-record equality,candidate decimalization,initial direction and first upward neighbor/infinity exit | Remaining up/down search,distances/ties,pow and original execution corpus |
| [Decimal neighbor refinement/ties](DECIMAL_NEIGHBOR_REFINEMENT_AND_TIES.md) | Complete local up/down searches,difference requests,parity ties and asymmetric side selection | __minus_dec/pow,executed iteration/distance/final-bit corpus |
| [Decimal record subtraction](DECIMAL_RECORD_SUBTRACTION.md) | Complete copy/alignment/borrow,omitted-tail correction/pointer and normalization flow | Executed caller invariants,distance accuracy,zero/alias cases and pow |
| [Power entry/special cases](POWER_ENTRY_AND_SPECIAL_CASES.md) | pow wrapper,zero/NaN order,negative-base parity,exponent/base shortcuts and errno gate | General log/exp/range core,sqrt and executed base-five/FPSCR corpus |
| [Power large-exponent path](POWER_LARGE_EXPONENT_PATH.md) | Ordered high-word gates,immediate returns,original-base fused near-one polynomial and split log pair | General log/common exp-range tail and executed power/FPSCR corpus |
| [Power general logarithm](POWER_GENERAL_LOGARITHM.md) | Normalization/reference gates,synthetic denominator,fused compensated polynomial and split handoff | Common exponentiation/range/sign tail and executed power/FPSCR corpus |
| [Power exponentiation/range](POWER_EXPONENTIATION_AND_RANGE.md) | Complete shared sign/product split,range gates,integer reduction,exp polynomial and scaling | sqrt,executed power/FPSCR and composed conversion corpus |
| [Square-root service](SQUARE_ROOT_NUMERIC_SERVICE.md) | Complete wrapper/special/errno gates,subnormal normalization,integer extraction/correction and output bits | Executed accuracy/FPSCR/errno/caller corpus and power composition |
| [Scan directive parser](SCAN_DIRECTIVE_PARSER.md) | Suppression/word-wrapped widths, selectors, exact conversion routing, s bitmap and signed scanset ranges | Executed descriptor corpus and numeric/multibyte helpers |
| [Scan formatter consumers](SCAN_FORMAT_CONSUMERS.md) | Complete local callbacks, destination fetch/store, conversion counters, partial c, s/[ loops and final EOF result | Numeric/multibyte helpers and original format/argument corpus |
| [Format output runtime](FORMAT_OUTPUT_RUNTIME.md) | sprintf/vsprintf termination, string sink, varargs slots, formatter dispatch/string/padding/n/fallback consumers | Wide conversions, memory services and executed call-site comparisons |
| [Format directive parser](FORMAT_DIRECTIVE_PARSER.md) | Complete flags/width/precision/length grammar, star argument consumption, width cap, conversion normalization and table map | Wide conversions, original call-site types/counts and executed comparisons |
| [Format integer conversion](FORMAT_INTEGER_CONVERSION.md) | 32/64-bit bases/signs/digits, zero/precision rules, alternate prefixes, caller-copy mutation and capacity gate | Floating/wide conversions, caller corpus and executed comparisons |
| [Format decimal rounding](FORMAT_DECIMAL_ROUNDING.md) | Record fields, tail ties/carry/trimming, zero-retention parity, decimal preparation and special-value output | Floating environment and executed caller comparisons |
| [Finite decimal layout](FORMAT_FINITE_DECIMAL_LAYOUT.md) | g/G mode choice after rounding, scientific exponent/mantissa, fixed digit/zero placement, mutation/sign/capacity contracts | Floating environment and original execution/caller corpus |
| [Binary decimal entry](FORMAT_BINARY_DECIMAL_ENTRY.md) | Numeric-to-ASCII request rounding/padding, raw zero/special classification, mantissa factorization and integer-record pipeline | Floating environment, hex-float and executed caller corpus |
| [Decimal multiplication](FORMAT_DECIMAL_MULTIPLICATION.md) | Numeric digit convolution, scratch carry, leading exponent, 36-digit copy and tail rounding | Numeric helpers, caller domain and executed comparisons |
| [Decimal powers](FORMAT_DECIMAL_POWERS.md) | Sparse seed dispatch, ASCII seed comparison, recursive square/odd correction and digit/count invariants | Floating environment, executed accuracy/callers and full formatting |
| [Binary numeric services](FORMAT_BINARY_NUMERIC_SERVICES.md) | Normalization/scaling/splitting, raw sign copy, integer shift/saturation and finite generator power bounds | Floating environment, caller corpus and executed equivalence |
| [Unsigned64 division](FORMAT_UNSIGNED64_DIVISION.md) | Leading-zero setup, word shifts, carry/restoring loop, quotient/remainder exits and derived zero-divisor behavior | Original execution/ABI flags, caller corpus and full formatting |
| [Hexadecimal float](FORMAT_HEXADECIMAL_FLOAT.md) | Raw exponent/fraction, special sign/case, fixed leading1, truncation, extended precision stack reads and output predicates | Original caller/stack domain, executed output, memory services and full formatting |
| [Announcement messages](BATTLE_ANNOUNCEMENT_MESSAGES.md) | First-call formatting, shared text/timer ownership, trigger/yield protocol and draw geometry | Text lookup/controls, substitution, renderer, parameters and story dialogue |
| [Camera requests](BATTLE_CAMERA_REQUESTS.md) | Mode/speed/zoom/homing callbacks, priority gates, reset side effects and vector sources | Main camera modes, producers, framing/rendering and remaining callbacks |
| [Default action events](BATTLE_DEFAULT_ACTION_EVENTS.md) | Electric/zero-gravity script order, interpolation mode8, offset sentinel, command-init and speed gate | Camera/message callbacks, renderer, other curves and library numerics |
| [Action controller](BATTLE_ACTION_CONTROLLER.md) | All15 local states, command/script routing, retained draws, count/cleanup order and post-event handoff | Command/event/service bodies, flag writers, outer cadence and content |
| [Move controller](BATTLE_MOVE_CONTROLLER.md) | All15 local states, immediate/fallthrough transitions, phase event ordering, actor reset/handoff and announcement drain | Action controller, services/scripts, external channel writers and outer cadence |
| [Phase tables](BATTLE_PHASE_TABLES.md) | Actor/event table membership and sorts, phase wrapper, selected actor/count-consumption regions and phase-end predicate | Full move/action controllers, flag/state writers and outer cadence |
| [Move counts](BATTLE_MOVE_COUNTS.md) | Fast/Slow reset arithmetic, max-count setter, action/guard predicates and turn-first reset caller | Count consumption, outer cadence, field consumers and actor scheduling |
| [Removal and reservations](INVENTORY_REMOVAL_AND_RESERVATIONS.md) | ID/index removal, ordered parallel compaction, reserve insertion/refund and deferred consumption | Item table bindings, all lifecycle callers and badge array consumers |
| [Waits and conclusion](BATTLE_WAITS_AND_CONCLUSION.md) | Unit/battle barriers, queued reactions, reservation consumption and three-group conclusion predicate | Queue/configuration producers, pouch removal, conclusion events/rewards and outer cadence |
| [Recovery events](BATTLE_RECOVERY_EVENTS.md) | Status fallback/override, Happy pose gates and waits, identity/data/pose wrappers | Motion, float coercion, pose/sound data and key8 bindings |
| [Regeneration](BATTLE_REGENERATION.md) | Status HP/FP additions, shared FP ownership, Happy per-copy trials, presentation and wait differences | Happy event bodies, display fields, effects and outer cadence |
| [Periodic damage](BATTLE_PERIODIC_DAMAGE.md) | Poison requests, Burn registration/polling/delayed damage and shadow guard | HP/FP regeneration, numeric sine/effect internals and outer cadence |
| [Turn recovery](BATTLE_TURN_RECOVERY.md) | Ordered status phases, slot scans, event registration/waits, Freeze expiry and direct caller inventory | Periodic effects, outer controller cadence, event bodies and inner wait helper |
| [Status storage and recovery](BATTLE_STATUS_STORAGE.md) | All local getter/setter cases, shared signed pairs, presence/badge override, explicit recovery steps | Infliction/immunity, recovery caller timing, periodic effects and lifecycle |
| [Status presentation](BATTLE_STATUS_PRESENTATION.md) | Complete application tail: effect selection/payloads, positioning, announcements, sound and return semantics | Effect internals, announcement queues and original rendering comparisons |
| [Status infliction](BATTLE_STATUS_INFLICTION.md) | Weapon fields/stacking, ordered wrapper, immunity gates, susceptibility/RNG and mutation prefix | Weapon bindings, effect services and lifecycle |
| [State and inventory](STATE_AND_INVENTORY.md) | Saved-work sizes/accessors, resource setters, capacity and generic item insertion | Full progression, equipment, special items and original save records |
| [Randomness](RANDOMNESS.md) | rand recurrence and irand draw consumption | Seeding, randf, consumer call order and persistence |
| [Mechanics ledger](../MECHANICS.md) | Existing US ordinary field movement/collision/recovery/road evidence | Full physics, partners, dynamic events, matched original gameplay |
| [Asset provenance](../ASSETS.md) | Original sources, conversion choices and reuse boundaries | Complete animation, sound-event and material behavior |

The final system requirements document is **not yet ready to be derived**:
many original behaviors below remain unverified. Its eventual acceptance
requirements must reference recovered contracts and test evidence, rather than
presenting this research backlog as if it were a complete game specification.

## Public decompilation audit

Pinned source: [doldecomp/ttyd](https://github.com/doldecomp/ttyd/tree/62131fc31866f85a12e4817a8dc37f0c25eb5f8c),
revision `62131fc31866f85a12e4817a8dc37f0c25eb5f8c`, declared Japanese G8MJ01
revision 0 support. Source existence, build matching and US behavior coverage
are three separate properties.

`tools/decomp_coverage.py` parses `configure.py` with Python AST, without
executing upstream code. It selects explicit `progress_category="game"`
libraries and the configured Rel helper calls; SDK/middleware helpers are
excluded. It records each object declaration, path, source presence/size/hash
and config line. This is an object census, not a percentage of decompiled
functions, instructions, bytes or game behavior.

Results for the pinned snapshot: **427 game-category object declarations**,
421 NonMatching and 6 Matching. **134 have source files; 293 do not**. A present
file may be partial or a stub. The six Matching declarations were not rebuilt
or independently verified against the Japanese game, and say nothing by
themselves about US equivalence.

| Config library/group | Declared objects | Source files present |
| --- | --- | --- |
| battle | 40 | 30 |
| battle_ac | 21 | 2 |
| battle_sac | 8 | 1 |
| data | 4 | 3 |
| driver | 24 | 24 |
| effect / effect_n64 | 76 / 103 | 6 / 2 |
| event | 45 | 15 |
| manager | 8 | 8 |
| mario / motion / party | 10 / 13 / 11 | 6 / 2 / 3 |
| sequence | 10 | 7 |
| unit / unit_party | 6 / 7 | 0 / 1 |
| window | 7 | 1 |
| library named sdk, categorized game by upstream | 4 | 3 |
| MarioSt | 25 | 15 |
| Explicit REL objects: dig / mri / pik / tou2 | 2 / 1 / 1 / 1 | 2 / 1 / 1 / 1 |

The five configured REL source objects are not a complete census of the 29
US ISO modules. The Japanese config even names a module not present in the
US REL inventory (`dig`); do not assign US content by matching names alone.
Cross-region REL coverage and shipped event streams need their own census.

Concrete reviewed incompleteness: the public defense checker is a constant
return stub; `BattleCheckDamage` contains only an initial fragment; most
action-command objects have no source. Conversely, substantial source exists
for the event manager and saved-work accessors despite NonMatching labels.
Research priority should be informed by those distinctions, not by treating
all 421 declarations as equally unknown.

Reproduction (snapshot and reports stay ignored):

```sh
python3 tools/decomp_coverage.py --snapshot artifacts/reference/decomp-62131fc31866f85a12e4817a8dc37f0c25eb5f8c --output artifacts/reference/behavior/decomp-coverage.json
python3 tools/behavior_evidence.py --symbols artifacts/reference/us_symbols.csv --output artifacts/reference/behavior/evidence.json
python3 tools/verify_behavior_contracts.py
```

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
| Partners/NPCs | Not recovered | Following/path/AI rules, field abilities, scripts, recruitment/upgrades and partner-specific collision |
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

## Gate for the eventual system requirements document

For each system, documentation must identify inputs, state and ownership, ordered
updates, outputs, numerical/data rules, exceptional paths, persistence and
cross-system dependencies. Each nontrivial claim needs source revision plus
binary address/data reference or a reproducible original observation. Unknown
branches remain explicit and cannot be hidden by selecting the prototype's rules.

Then derive requirements with stable IDs, traceability to these contracts,
dependencies, reused-data interfaces, numerical/timing constraints, platform
service boundaries and acceptance cases. Include complete playable content,
original story fidelity, asset conversion/runtime limits, save robustness,
input/audio/render integration and New 2DS XL performance verification.

Completion requires covering every shipped reachable system and content mode,
not merely booting the early Rogueport slice or passing its host tests. The
current output satisfies neither that full documentation gate nor the final
requirements gate. Continue the recovery work before marking the goal achieved.
