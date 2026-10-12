# Event constructor label scan and system-level run check

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete label-scanning regions within root/typed constructors and complete
entry run-check80037738/78. Read
[root constructors](EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[system-level transitions](EVENT_SYSTEM_LEVEL_AND_SPEED.md) and
[dispatcher](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).
Verify: `python3 tools/verify_event_root_constructors.py`.
No executed script/pause observation claim.

## Exact label scanner,including terminal distinction

Before scanning,constructors initialize16 label ID bytes to0xff at entry1c..2b
and16 pointers to0 at2c..68. Start scan pointer p=entry.next14,label count n=0,
label-output pointer q=entry. For each iteration:

1. Read command word at p. Read word at p+4 UNCONDITIONALLY as first operand,
   even when this command has argument count0.
2. opcode=low16(commandWord);count=S16(commandWord>>16).
3. Advance p=wrap32(p+4+4*count),using signed high16 count.
4. Opcode1 stops the scanner immediately. Opcode2 does NOT stop it.
5. If opcode3 (label),store low8(first operand) to entry+1c+n;increment n;
   store advanced p to q+2c;increment q by4.
6. Repeat from p. A computed comparison with16 at the loop tail has no
   conditional exit/store effect; it DOES NOT cap the label count.

All other opcodes,including0,2,brother/inline markers and their commands,are
simply traversed. The scan does not execute callback arguments,branch predicates
or labels. It neither resolves a variable-looking label operand nor checks
opcode/command bounds/pointer/count validity. Label address points AFTER the
label command. Preserve the unconditional first-operand read and signed-count
advance when using an original-memory adapter.

More than16 labels overwrite adjacent entry fields: no actual local rejection,
truncation or dynamically expanded label table. Missing opcode1 can scan beyond
the script span. These are valid-script invariants requiring a port domain
policy,not recovered allocation-failure handling. The constructors have no
returned label-validation success flag.

Duplicate IDs remain in scan order. The existing GOTO dispatch at80031788
scans16 entries comparing signed byte label IDs to a resolved word;the first
matching record wins. A raw label operand outside signed8 range is storedlow8;
it must not be treated as the unmodified input integer by a native lookup.
No change to the script is authorized by the scanner contract.

## Complete evtEntryRunCheck

The entry pointer argument is NOT read or used. Read signed global system-level
word8041e940 (SDA13+1a20),then:

| Level | Request |
| --- | --- |
| 1 | evtStopAll(1) |
| 2 | evtStopAll(2) |
| 3 | evtStopAll(0x10) |
| 4 | evtStopAll(0xef) |
| 0,negative,>=5 | None |

No local start/unstop/reset/check of entry.type occurs;it invokes the all-event
service once with the shown mask and discards its scalar return. evtStopAll
selects current context and iterates active entries,while evtStop applies mask
intersection and relationship traversal as documented in the existing ownership
contracts. Current context can be reloaded independently of the constructor's
captured context. The name “run check” must not become a per-entry predicate
that returns eligibility to a native constructor.

This constructor-time service differs from the full system-level setter's
start-before-stop transition table. Creating an event during level1 can also
stop existing type1 events even if the new event has another type. It does not
clear unrelated stopped flags. Level0 is not an automatic resume-all here.
The call occurs AFTER root live-table append,so an appended event may be stopped
before the manager reaches it;manager flags0x92 then suppress lifetime/credit.

Acceptance cases: label address/low8/count scan,opcode2 followed by later labels,
terminal1,duplicate/negative labels and16-record boundaries;level0/1/2/3/4/5,
all-event rather than argument-only effect,type-mask independence of the helper,
append-before-stop and no implicit resume. Context/level/type writers,other
constructor label paths,full pause UI and original creation/deletion traces
remain open. Full-game requirements must keep those dependencies explicit.
