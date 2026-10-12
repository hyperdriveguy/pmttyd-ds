# Complete event stop/start: descendant-first masks and context captures

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evtStop80036210/3d4,evtStart80035e3c/3d4,
evtStopAll80035c68/94 and evtStartAll80035bd4/94.
Read [deletion](EVENT_DELETE_AND_COPYBACK.md),
[other/ID wrappers](EVENT_CONTROL_OPCODES_AND_ID_SCANS.md),
[system levels](EVENT_SYSTEM_LEVEL_AND_SPEED.md) and
[manager](EVENT_MANAGER_COMPLETE_LOOP.md).
Verify: `python3 tools/verify_event_lifetime_services.py`.
Static local behavior;complete pause/UI and original traces remain open.

## Relationship traversal and postorder mutation

For ordinary valid stable-context storage,the full bodies implement this ordered
recursive algorithm,including compiler-inlined levels specified below:

```text
visit(E,mask):
    select current field/battle context C
    if E.child70 != null: visit(E.child70,mask)
    scan C's slots ascending:
        if active bit1 and candidate.owner74 == E: visit(candidate,mask)
    if U8(E.typec) & mask != 0:
        STOP:  E.flags8 = low8(E.flags8 | 2)
        START: E.flags8 = low8(E.flags8 & ~2)
```

There is no active/null/ID test at visit's own root. The direct child pointer is
followed whenever nonnull;active bit is tested ONLY for entries found in slot
scans. Every nested mutation tests that entry's type byte AFTER its descendants.
The full supplied32-bit mask is used by AND with unsigned type8;high mask bits
cannot intersect a byte. Type mismatch does not prune descendants.
No stopped/credit/timer/context-level predicate prevents traversal;already
stopped or started entries still traverse and may receive the same byte write.
There is no cycle detection/visited set or recursion-depth guard.

Stop/Start differ only in final flag-bit mutation;all relationship scan/context
structure is paired. Bit0x10 child-wait,active1 and other flags are preserved.
No copy-back,deletion,ID/counter change,label reset,credit reset,lifetime update
or immediate dispatch occurs. Start clears a bit;it is not restart or balancing
a nested pause-reference count. Both leaves have no specified scalar result.

## Exact compiler-inline context and scan structure

The original does not uniformly reload GP for every conceptual visit. Let E be
outer entry,D=its direct child,G=D's direct child;B is an attached brother of E,
K=B's direct child,S is an attached brother of B. Context field/battle selection
is field when captured GP.word14==0,otherwise battle. These are the complete
selection sites and recursion boundaries in Stop (Start uses the same offsets
from its own start80035e3c):

| Stop sites | Selection/processing |
| --- | --- |
| 80036230..244 | Load GP/context selector once,C0;capture E.child D |
| 80036250..264 | If D exists,form C1 from that SAME selector;capture D.child G |
| 80036270..284 | If G exists,form C2 from SAME selector;ordinary recursive calls for G.child and active attached brothers found in C2,then mutate G |
| 800362f4..363c4 | Scan attached brothers of D in C1;each match reloads GP at36324 for its inner context,ordinary recursion for match.child and match's attached brothers,then mutate match;after scan mutate D |
| 800363e0..365b4 | Scan attached brothers B of E in C0;each match reloads GP at36410 to select CB |
| 80036430..364c0 | If B.child K exists,select CK from SAME CB selector;ordinary recursion for K.child and attached brothers in CK,then mutate K |
| 800364c4..36588 | Scan attached brothers S of B in CB;each match reloads GP at364e8 to select CS;ordinary recursion for S.child and attached brothers in CS,then mutate S |
| 800365b8..365cc | After all outer work,apply type mask/mutation to E |

Ordinary recursive calls at36294/362c4,36354/36384,36464/36494 and36518/36548
start a new full invocation with new GP capture. No separate exported callback
executes on the compiler-inlined D/G/B/K/S boundaries. Pure local Stop/Start
services have no user-script callbacks that change GP. The simpler recursive
algorithm above is therefore valid for stable-context ownership,while a raw
memory/service adapter must preserve these captured selectors rather than
inventing a fresh context read at EVERY inlined node. Invalid aliases/context
mutation domains are not repaired locally.

Each slot scan captures its selected context's ARRAY pointer when that scan
begins,then advances stride1b0. Reload signed capacity0 on EACH loop condition.
Candidate activity and owner74 are read before recursive work;type is read
when mutation occurs afterward. Do not build one immutable descendant snapshot,
move parent flag writes before descendants or filter an entire subtree by the
root type. Sibling order is physical registry slot order,not priority schedule.

## Complete StopAll/StartAll wrapper loops

Capture GP/current context,array once and supplied mask unchanged. For slots
ascending while slot<fresh signed context.capacity0,read active1. If active,
call evtStop(entry,mask) / evtStart(entry,mask). Advance slot and pointer stride.
No pointer exclusion,ID/type filter or eligibility check in the wrapper;mask
intersection belongs to the called traversal. Calls can visit an attached entry
through one root and again through its own later active slot:there is no dedup.
Context is captured once for the outer scan;each called traversal captures anew.
There is no scalar success return or global speed/system-level store here.

An inactive entry is skipped by All,but can still be reached through an active
entry's raw child70. Other wrappers exclude only a direct pointer,so traversal
from a different root can still change that excluded entry if linked. ID wrappers
supply0xef,not0xff. These are caller-specific policies,not uniform pause-all.

## Manager integration and acceptance

Stopped bit2 excludes a scheduled visit before lifetime and credit additions.
Start does not replenish skipped time/credit or dispatch immediately. If a
callback stops itself DURING its eligible visit,the manager's inner remaining-
credit loop does not recheck flags;more dispatcher invocations can occur that
pass. The next outer visit applies eligibility. Do not replace Stop with an
immediate unconditional host coroutine suspension unsupported by this caller.

Acceptance cases:nonmatching root with matching child/brother,postorder mutation,
inactive direct child versus active scan gate,all slot ordering/repeated visits,
context-capture/inlined-recursion policy,live capacity,mask high bits,flag-bit
preservation,already stopped traversal,Start without restart/refcount,same-pass
remaining credit and outer-visit eligibility. Full pause UI/source flags/context
writers,remaining opcodes/namespaces,valid relationship domain and original
pause/creation/deletion/multicredit traces remain open under EVT-01.
