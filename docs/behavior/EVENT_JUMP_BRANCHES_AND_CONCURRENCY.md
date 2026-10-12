# Jump integer branches, raw masks and two concurrent event forms

US G8ME01 revision0;recovered2026-10-12 by documentation agent ID2.
Evidence B:complete evt_if_large_equal0x8003491c/0xcc,
evt_if_small_equal0x800349e8/0xcc,evt_if_large0x80034ab4/0xcc,
evt_if_not_flag0x8003479c/0xc0,evt_case_large_equal0x80033f74/0x15c,
evt_brother_evt0x8003325c/0x94 and evt_inline_evt0x8003346c/0x15c.
evtEntryType0x80037220/0x28c is fingerprinted;only the relevant association
initialization region is specified here. Full constructor/operand arithmetic
and malformed-event semantics remain open.

## Original dispatcher bindings and comparisons

| Opcode | Original dispatcher target | Predicate |
| --- | --- | --- |
| 27 | 0x80031a7c | Signed resolved lhs>rhs |
| 28 | 0x80031a8c | Signed resolved lhs<=rhs |
| 29 | 0x80031a9c | Signed resolved lhs>=rhs |
| 31 | 0x80031abc | Resolved operand0 AND **raw** operand1 equals0 |
| 41 | 0x80031c10 | Signed stored switch value>=resolved case operand |
| 107 / 109 | 0x80032934 / 0x80032944 | Independent inline start / end |
| 110 / 112 | 0x80032964 / 0x80032984 | Attached brother start / end |

The three signed IF bodies resolve lhs then rhs through evtGetValue. Comparisons
are signed32 words,not unsigned counts or floats. IF_NOT_FLAG resolves only its
first operand;the mask is loaded raw. Its literal2 in Jump tests mask0x2 in the
returned AC word. A variable-looking second operand would remain its encoded
mask,not be looked up. Preserve that difference from two-resolved-operand IF.

True predicates return handler2 and leave the already advanced next pointer.
False branches use the same complete local nested-IF skip policy recovered for
[equal/not-equal IF](EVENT_INTEGER_BRANCHES.md):scan from next pointer,advance
by4+4*signed-high16 count,increment nesting for opcodes12..31,decrement for1
or33,and stop after matching ELSE or terminating END_IF/end. The scan is
unbounded and does not validate operands/stream length. No artificial tick is
added per branch marker.

CASE_LARGE_EQUAL resolves its comparison even after prior match,reads signed
switch depth/state and stored value. With state>0 and stored value>=comparison,
set state0 and execute the following body. On unequal/unmatched,search the next
case/default marker at its switch depth. With state<=0,skip to END_SWITCH.
The same scanner's literal opcode35 handling and nesting rules described in
EVENT_INTEGER_BRANCHES apply;do not infer generic C fallthrough.

In the Jump stylish blocks,CASE>=2 includes exactly2 and larger signed results.
Negative stylish result is a default route. In parent flow,count<=1 includes
negative counts at this VM layer;ordinary valid AC-count lifetime is supplied
by setup/evaluator evidence rather than a VM count clamp.

## Attached brothers: opcodes110/112

evt_brother_evt begins at parent's next-command pointer,which already points
after start110. It scans to the **first** opcode112,advancing each word by
its signed argument count. It has no nested-block counter or bounds check.
Store parent's next pointer after that end marker,then request
evtBrotherEntry(parent,bodyStart,flags0x60). Copy owner word160,type byte0xc,
user pointers154/158 and retained170/150 fields into the returned entry;
there is no local null/failure handling here.

The [brother constructor](EVENT_RUNTIME.md) copies local words/flags,retains
the parent's priority and sets the brother-owner link74 to parent,with null
waiting-parent6c. The parent is not blocked waiting for its brother. Unit/global
work referenced by callbacks remains shared;event locals are copies. The four
Jump110 blocks contain no nested110 block needing a different scanner policy.

At end112,original dispatcher calls evtDelete(currentEntry),then supplies
handler0. No result copy-back to the associated owner occurs because the
brother's waiting-parent6c is null. Deleting the owner recursively deletes
live brothers linked through74. Do not implement these blocks as detached
tasks or as joined child calls that copy local result temporaries back.

## Independent inline: opcodes107/109

evt_inline_evt likewise scans from parent's next pointer to the first109,
stores next pointer after it,then calls
evtEntryType(bodyStart,parentPriority,flags0x60,parentType).
The used constructor region0x800372d8..0x800372e0 sets waiting-parent6c,
waiting-child70 and brother-owner74 to null. This entry has **no owner link**
to the invoking parent and does not make it wait.

The wrapper copies owner160,user pointers154/158,retained170,message-related
fields178..190,the sixteen local words9c..d8 and three local-flag wordsdc..e4.
These are copies/shares as described by their storage type,not references to
the parent's local array. Allocation/label/schedule policy of evtEntryType is
otherwise a dependency;the whole-body fingerprint does not close it.

At end109,the dispatcher calls evtDelete(currentEntry),then supplies handler255
(terminal dispatcher result). The owner deletion's74 search does not identify
this independent entry. Other global cancellation/type policies can still
affect it. Do not assert it always completes or survives a scene change.

The concrete Jump107 block only changes camera speed/zoom around WAIT10 and
has no nesting. It is independent concurrent camera work,unlike the four110
setup/effect/stylish blocks. Their end handlers and lifetimes must remain
distinct even if a native task abstraction could represent both.

## Scoped requirements and verification

| ID | Requirement | Derived acceptance case |
| --- | --- | --- |
| JVM-01 | Preserve signed comparisons and resolved/raw operand policy | Negative loop/count values follow signed29/28;raw mask2 tests flag without operand resolution |
| JVM-02 | Preserve post-marker skip pointers and switch state | False IF skips nested blocks in one eligible invocation;CASE>=2 does not also run default |
| JVM-03 | Keep attached brother and independent inline associations distinct | Both copy locals;only brother links74;owner deletion cancels brother without copying its locals |
| JVM-04 | Preserve concrete parent advance and end handlers | Parent advances after112/109 at creation;end112 deletes/handler0,end109 deletes/handler255 |

The standalone tools/verify_jump_event_flow.py checks these dispatcher bindings,
complete helper fingerprints,instruction sites and exact five Jump block pairs.
These are binary/derived contracts,not original execution or complete VM tests.
Full scheduler credits,pause,type stop policy,allocation failures,tagged arithmetic,
all other opcode variants and actual original parent/event traces remain open.
ISO access is read-only;inspection ELFs/original byte listings remain outside
source history.


## Complete SWITCH/CASE follow-up (2026-10-12)

[Switch stack/scans](EVENT_SWITCH_STACK_AND_CASE_SCANS.md) and
[group/mask/range](EVENT_GROUPED_MASK_AND_RANGE_CASES.md) now close34..49 and
all eleven CASE helpers,including exact group sentinel transitions and actual
dispatch-table ordering. Earlier partial switch/case scope above is superseded
for these local bodies. Scans ignore raw35 while counting34/1;case search stops
36..47 and skips BREAK48. Source/numeric/domain and original traces remain open.
