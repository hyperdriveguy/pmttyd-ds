# Event opcode implementation coverage and remaining integration requirements

US G8ME01 revision0; compiled2026-10-12 by documentation agent ID2.
This is the authoritative local opcode contract index for EVT-01. Every119
entry of table80304fd0 is mapped below to its actual target and a recovered
contract. Names are descriptive labels;the numeric table and behavior contracts
are authoritative. All rows represent static LOCAL dispatcher/helper behavior,
not proof of valid source domains,host binding or executed original equivalence.
`python3 tools/verify_event_dispatch_tail.py` pins all119 table words and the
containing dispatcher hash. Topic-specific verifiers pin their scoped blocks
and complete called helpers;one table/hash pass alone does not prove semantic
coverage. The links below identify the reviewed contracts for each entry.

## Scope and common bindings

[Manager](EVENT_MANAGER_COMPLETE_LOOP.md) supplies lifetime/priority/eligibility/
credits and caller/abort policy. [Dispatch](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md)
supplies fetch/common results and retained continuations. [Integer reader/setter](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[float reader/setter](EVENT_FLOAT_OPERAND_ROUTING.md) and
[saved RAM leaves](EVENT_SAVED_SWITCH_STORAGE.md) supply complete local operand
routing;integer and float bands/coercions must not share an assumed decoder.
[Root/typed](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[labels/run-check](EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md),
[child/brother/inline](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[restart](EVENT_RESTART_PRESERVATION.md),
[deletion/copy-back](EVENT_DELETE_AND_COPYBACK.md) and
[stop/start traversal](EVENT_STOP_START_RELATIONSHIP_TRAVERSAL.md) close local
lifetime services. Numeric/source/platform/domain providers remain required.

The cached opcode is U8(E.a). Values119..255 skip opcode work with default2,
then clear/fetch in the same invocation;they are not a validated-bytecode error.
Header fetch uses low8 opcode but scanners often inspect low16. Higher original
header bits therefore cannot be normalized blindly.0 repeats fetch as specified;
1 does not delete at runtime even though lexical scans treat it as a marker.
No blanket budget,yield,transactional cancellation or script-bound guard exists
in these local bodies. Such host boundaries need explicit domain/policy labels.

## Full numeric table and contract links

| Opcode | Actual target | Local role | Contract |
| --- | --- | --- | --- |
| 0 | 80031704 | Fetch | [Recovered](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 1 | 800329f4 | Common2 tail | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 2 | 80031748 | Delete current | [Recovered](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 3 | 80031758 | Label no-op | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 4 | 80031760 | GOTO | [Recovered](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 5 | 800317d0 | DO | [Recovered](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 6 | 80031808 | NEXT | [Recovered](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 7 | 80031818 | BREAK_LOOP | [Recovered](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 8 | 80031884 | CONTINUE_LOOP | [Recovered](EVENT_GOTO_AND_LOOP_OPCODES.md) |
| 9 | 800318e8 | WAIT_FRAME | [Recovered](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 10 | 80031950 | WAIT_MSEC | [Recovered](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 11 | 80031960 | WAIT_WHILE | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 12 | 8003198c | String equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 13 | 8003199c | String unequal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 14 | 800319ac | String small | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 15 | 800319bc | String large | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 16 | 800319cc | String small/equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 17 | 800319dc | String large/equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 18 | 800319ec | Float equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 19 | 800319fc | Float unequal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 20 | 80031a0c | Float small | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 21 | 80031a1c | Float large | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 22 | 80031a2c | Float small/equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 23 | 80031a3c | Float large/equal | [Recovered](EVENT_IF_FLOAT_AND_STRING_PREDICATES.md) |
| 24 | 80031a4c | Integer equal | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 25 | 80031a5c | Integer unequal | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 26 | 80031a6c | Integer small | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 27 | 80031a7c | Integer large | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 28 | 80031a8c | Integer small/equal | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 29 | 80031a9c | Integer large/equal | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 30 | 80031aac | IF_FLAG | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 31 | 80031abc | IF_NOT_FLAG | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 32 | 80031acc | ELSE | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 33 | 80031b38 | END_IF | [Recovered](EVENT_IF_INTEGER_MASK_AND_CONTINUATIONS.md) |
| 34 | 80031b40 | SWITCH resolved | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 35 | 80031b84 | SWITCH raw | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 36 | 80031bc0 | CASE_EQUAL | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 37 | 80031bd0 | CASE_NOT_EQUAL | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 38 | 80031be0 | CASE_SMALL | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 39 | 80031c00 | CASE_LARGE | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 40 | 80031bf0 | CASE_SMALL_EQUAL | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 41 | 80031c10 | CASE_LARGE_EQUAL | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 42 | 80031c20 | DEFAULT | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 43 | 80031d14 | CASE_OR | [Recovered](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) |
| 44 | 80031d34 | CASE_AND | [Recovered](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) |
| 45 | 80031d44 | CASE_FLAG | [Recovered](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) |
| 46 | 80031d24 | CASE_END | [Recovered](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) |
| 47 | 80031d54 | CASE_BETWEEN | [Recovered](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) |
| 48 | 80031cb0 | SWITCH_BREAK | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 49 | 80031d64 | END_SWITCH | [Recovered](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) |
| 50 | 80031d8c | SET resolved | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 51 | 80031db8 | SET raw | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 52 | 80031dd4 | SET float | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 53 | 80031dfc | ADD integer | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 54 | 80031e38 | SUB integer | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 55 | 80031e74 | MUL integer | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 56 | 80031eb0 | DIV integer | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 57 | 80031eec | MOD coerced | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 58 | 80031f94 | ADD float | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 59 | 80031fd0 | SUB float | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 60 | 8003200c | MUL float | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 61 | 80032048 | DIV float | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 62 | 80032084 | Data cursor | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 63 | 800320a0 | Integer read1 | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 64 | 800320c8 | Integer read2 | [Recovered](EVENT_ASSIGNMENT_ARITHMETIC_AND_DATA_CURSOR.md) |
| 65 | 8003210c | Integer read3 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 66 | 8003216c | Integer read4 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 67 | 800321e8 | Integer indexed | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 68 | 8003221c | Data cursor duplicate | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 69 | 80032238 | IEEE float read1 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 70 | 80032260 | IEEE float read2 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 71 | 800322a4 | IEEE float read3 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 72 | 80032304 | IEEE float read4 | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 73 | 80032380 | Indexed signed word→single | [Recovered](EVENT_MULTIWORD_AND_FLOAT_DATA_READERS.md) |
| 74 | 800323d4 | Set user work | [Recovered](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md) |
| 75 | 800323f0 | Set user flags | [Recovered](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md) |
| 76 | 8003240c | Allocate user work | [Recovered](EVENT_USER_BUFFER_AND_MAP_ALLOCATION.md) |
| 77 | 80032468 | AND resolved | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 78 | 800324a4 | AND raw | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 79 | 800324d8 | OR resolved | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 80 | 80032514 | OR raw | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 81 | 80032548 | 60/1000 conversion | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 82 | 80032588 | 1000/60 conversion | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 83 | 800325cc | Raw address word store | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 84 | 800325e8 | Raw address float store | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 85 | 80032604 | Raw address word load | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 86 | 80032620 | Raw address float load | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 87 | 8003263c | Resolved integer destination | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 88 | 80032670 | Resolved float destination | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 89 | 800326a0 | Double-query integer source | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 90 | 800326d0 | Double-query float source | [Recovered](EVENT_BITWISE_TIME_AND_INDIRECT_MEMORY.md) |
| 91 | 800326fc | USER_FUNC | [Recovered](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) |
| 92 | 80032770 | RUN_EVT | [Recovered](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) |
| 93 | 80032780 | RUN_EVT_ID | [Recovered](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) |
| 94 | 80032790 | Child creation/wait | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 95 | 8003244c | Delete ID | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 96 | 80032828 | Replace script/restart | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 97 | 800327c0 | Set priority | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 98 | 800327e4 | Set speed | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 99 | 80032804 | Set type | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 100 | 8003284c | Stop all | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 101 | 80032868 | Start all | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 102 | 80032884 | Stop others | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 103 | 800328a8 | Start others | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 104 | 800328cc | Stop ID | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 105 | 800328e8 | Start ID | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 106 | 80032904 | Check ID | [Recovered](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md) |
| 107 | 80032934 | INLINE | [Recovered](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) |
| 108 | 80032954 | INLINE_ID | [Recovered](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) |
| 109 | 80032944 | END_INLINE/delete | [Recovered](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) |
| 110 | 80032964 | BROTHER | [Recovered](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) |
| 111 | 80032974 | BROTHER_ID | [Recovered](EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) |
| 112 | 80032984 | END_BROTHER/delete/yield0 | [Recovered](EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md) |
| 113 | 80032994 | No-op | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 114 | 8003299c | No-op | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 115 | 800329a4 | Debug format | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 116 | 800329b4 | Raw E1a4 metadata | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 117 | 800329c8 | No-op | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |
| 118 | 800329d0 | Slot probe then yield | [Recovered](EVENT_DISPATCH_TAIL_AND_DEBUG_FORMATTING.md) |

## Remaining requirements for implementing EVT-01

Complete local opcode coverage removes the generic “other opcode bodies”
blocker. It does NOT close these integration requirements:

| Requirement | Evidence still needed / implementation consequence |
| --- | --- |
| Original sources and callback address translation | Inventory scripts/REL relocations,typed callbacks,caller argument/control layouts and imported data. Resolve original32-bit addresses through providers;do not cast to host pointers. Reuse US message IDs/control tokens and original dialogue ordering. |
| Context and capacity initialization | [Init/reset](EVENT_CONTEXT_INITIALIZATION_AND_RESET.md) now proves two initial256-slot arrays,bank/reset/global preservation;[heap/callers](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) pins SDK allocation and four direct DOL sites. Remaining:GP/context/capacity writers,scene/indirect/REL boundaries,valid stack/label/free-slot domains and original transitions.118 lacks a local capacity test but its initial array extent is now evidenced. |
| Time/pause/input ownership | [Retrace publication](EVENT_RETRACE_TIME_PUBLICATION.md) now supplies raw-TB/admitted-event-clock accumulation,GP10/window/mask gates and refresh-word initialization;[outer loop](GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md) fixes33 subsystem requests and input/battle/event order. Remaining:VI/thread/clock bootstrap,gate/state writers,actual pad/retrace cadence and original multicredit/append/reuse/wait/pause traces. |
| Heap/user-pointer lifetime | Recover root8041e808 initialization,map free/coalescing,scene teardown/ownership and sharing/alias invariants. Local76 replacement and entry deletion do not establish automatic buffer free. |
| Numeric/FPSCR and invalid-domain policy | Establish original arithmetic/NaN/infinity/overflow/fctiwz/division/FPSCR and pointer/index domains. Preserve opcode-specific precision and CR tests;mark native unsupported-domain checks separately. |
| Source/slot/relationship restrictions | Establish nesting/labels/counts/duplicate IDs/cycles/pointer aliases and original source capacities. Scanners have distinct op1/raw35/count rules;local unchecked behavior is not valid-source proof. |
| Original execution acceptance | Obtain original normal Jump/tutorial/child/brother/wait/stop/restart/cancellation/pause and multicredit traces with actual inputs. Compare ordering/state/output against implementation;host tests alone do not prove executed game equivalence. |

These are concrete EVT-01 dependencies,not newly invented behavior. Other
battle/field/message/render/audio/story systems remain independent requests.
All systems must be adequately documented before claiming complete-game
requirements or the full-game remake finished. This index is an implementation
map for the documented VM slice,not the eventual complete-game requirements
artifact. Keep the parent request open until source/domain/runtime bindings and
its acceptance evidence are satisfied.
