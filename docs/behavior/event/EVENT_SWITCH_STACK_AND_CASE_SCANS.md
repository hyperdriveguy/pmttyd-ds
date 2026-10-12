# Event SWITCH stack and simple CASE contracts

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete six simple CASE bodies and scoped dispatcher34..49 regions in
evtmgrCmd800316b8/13b8. Static original DOL evidence;
no original execution or valid script-domain claim. Verify with
`python3 tools/verify_event_switch_cases.py`:eleven full CASE body hashes,
whole dispatcher fingerprint,267 instruction anchors and16 dispatch words.
Read [operand APIs](EVENT_INTEGER_OPERAND_NAMESPACES.md),
[dispatch continuations](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md) and
[group/mask/range](EVENT_GROUPED_MASK_AND_RANGE_CASES.md).

## Actual dispatch table

Targets are read from80304fd0+4*opcode,not inferred from function address order.
This matters because opcode39 precedes40 in the table but its helper follows
40's helper in executable memory;group dispatch is also reordered.

| Opcode | Target | Handler / helper |
| --- | --- | --- |
| 34 | 80031b40 | SWITCH resolved operand0 |
| 35 | 80031b84 | SWITCH raw operand0 |
| 36 | 80031bc0 | CASE_EQUAL;evt_case_equal80034640/15c |
| 37 | 80031bd0 | CASE_NOT_EQUAL;evt_case_not_equal800344e4/15c |
| 38 | 80031be0 | CASE_SMALL;evt_case_small80034388/15c |
| 39 | 80031c00 | CASE_LARGE;evt_case_large800340d0/15c |
| 40 | 80031bf0 | CASE_SMALL_EQUAL;evt_case_small_equal8003422c/15c |
| 41 | 80031c10 | CASE_LARGE_EQUAL;evt_case_large_equal80033f74/15c |
| 42 | 80031c20 | DEFAULT |
| 43 | 80031d14 | CASE_OR;evt_case_or80033b64/168 |
| 44 | 80031d34 | CASE_AND;evt_case_and8003396c/1f8 |
| 45 | 80031d44 | CASE_FLAG;evt_case_flag80033ccc/12c |
| 46 | 80031d24 | CASE_END;evt_case_end800337ec/180 |
| 47 | 80031d54 | CASE_BETWEEN;evt_case_between80033df8/17c |
| 48 | 80031cb0 | SWITCH_BREAK |
| 49 | 80031d64 | END_SWITCH |

Every local handler returns2. The dispatcher clears the cached opcode and
continues fetching in the same invocation. A case match,break or switch pop
introduces no yield or additional manager credit debit.

## Stack and push/pop order

E is current entry. Byte E.f is the stack index;constructors/restart set it0xff.
Index use is signed8. State byte address is E+128+index;value word address is
E+130+4*index. The layout supplies eight state bytes and eight words for indices
0..7. This is storage layout,not proof that original scripts never exceed it.
No push/pop bounds check or state sanitization exists in these bodies.
[Constructor](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md) clear supplies initial
zero storage;[restart](EVENT_RESTART_PRESERVATION.md) resets f without clearing
these arrays. A fresh push overwrites only its selected value/state pair.

SWITCH34 loads operand0 and calls evtGetValue(E,operand0). AFTER the resolver,
read unsigned E.f,increment,store low8 f;reread and sign-extend f;store the
resolved word in its value slot,then store state1. SWITCH35 captures argument
pointer,reads E.f and raw operand0,then uses the same increment/reread/value/
state1 writes. It performs no resolution. Do not turn raw SWITCH into a variable
lookup or snapshot34's index before its resolver call.

END_SWITCH49 reads/sign-extends f,stores state0 at that selected state address,
then rereads unsigned f,decrements and stores low8 f. It leaves the selected
value word intact. Neither BREAK48 nor a skipped CASE pops;they arrange a later
fetch of49,which owns the actual pop.

## Two lexical scanners

Both start at E.next14 (already past the current command),with depth1. On each
step retain HEADER H,read big-endian word W,derive opcode=low16(W),and advance
the scanning pointer by4+4*S16(high16(W)). The argument count is signed16;
scanners do not truncate opcode to the dispatcher's cached byte. Scanning never
resolves operands or executes commands. They have no script bound,null check,
missing-end failure or automatic stop at DELETE2.

**End scanner:**opcode34 OR opcode1 increments depth;opcode49 decrements.
When the decrement makes depth0,store HEADER H in E.next14 and return2.
All other opcodes,including raw SWITCH35,are ignored. The next fetch executes49;
the scanner does not consume the closing command or pop the switch stack.

**Next-case scanner:**opcode35 is explicitly ignored;opcode34 OR opcode1
increments depth. Opcode49 decrements;depth0 stores HEADER H and returns2.
Opcode36..47 at depth1 also stores HEADER H and returns2;at other depths they
are ignored. Opcode48 is skipped;it is SWITCH_BREAK,not a case delimiter.
Other opcodes are ignored. A stopped HEADER is fetched/executed subsequently.
These numeric rules must be preserved:do not count both SWITCH forms as openers,
treat terminal1 as scan termination,or include BREAK48 among case headers.
Nested raw35 is therefore not tracked by these local scanners. Whether original
scripts rely on a restricted nesting form remains a source/runtime dependency.

## Simple CASE36..41

Capture argument pointer and unsigned f;sign-extend the captured f to index j;
resolve operand0 via evtGetValue. AFTER resolution read signed state byte and
switch value V using the retained j. Even a consumed/nonpositive state resolves
the candidate first. Resolver side effects cannot redirect j,though they can
change the state/value read afterward.

If state<=0,run the end scanner with no state write. If state>0,compare the
resolved candidate K against V using signed32 `cmpw`. Accepted comparisons:

| Opcode | Acceptance expressed using switch value V |
| --- | --- |
| 36 | V==K |
| 37 | V!=K |
| 38 | V<K |
| 39 | V>K |
| 40 | V<=K |
| 41 | V>=K |

The instruction compares K against V;for example SMALL accepts the greater
condition K>V. On acceptance write state0 using retained j and continue2 without
changing next14. On rejection run the next-case scanner without changing state.
No float reader,threshold coercion,enum narrowing or implicit fallthrough into
later ordinary cases is present. After an accepted body reaches another simple
case,that case still resolves its operand and then skips to END_SWITCH.

DEFAULT42 captures signed f,reads signed state,and uses state>0 as its entire
predicate. Positive writes state0;nonpositive runs the end scanner. It reads no
operand and calls no resolver. SWITCH_BREAK48 unconditionally runs the end
scanner regardless of state;it does not set state0 before doing so.

## Implementation acceptance cases and boundaries

Cover table order39/40 and43..48;resolved/raw push,resolver-mutated f,0xff→0
index,clear-before-pop and retained value;negative signed comparisons,equality
edges,consumed-case resolution and captured-index behavior;DEFAULT without
operand query;BREAK without immediate pop;scanner stopped HEADER execution,
signed high16 stride,nested34,ignored35,counted1,nonterminal2 and skipped48.

Grouped sentinels-1/-2 are real states;their treatment differs from ordinary
state<=0 and is specified in the companion contract. Host stack capacity,
malformed bytecode behavior,pointer/alias validity,source-script restrictions,
remaining opcode domains and original traces remain EVT-01 work. These complete
local contracts do not establish a complete native VM.
