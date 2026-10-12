# Event grouped CASE,mask and inclusive range handlers

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete evt_case_or80033b64/168,evt_case_and8003396c/1f8,
evt_case_end800337ec/180,evt_case_flag80033ccc/12c and
evt_case_between80033df8/17c. Static original DOL evidence;no executed script
claim. `python3 tools/verify_event_switch_cases.py` pins all bodies and actual
opcode table. [Switch/scanner contract](EVENT_SWITCH_STACK_AND_CASE_SCANS.md)
defines stack offsets,ordinary cases and both scanner algorithms used below.
[Integer operand contract](EVENT_INTEGER_OPERAND_NAMESPACES.md) defines queries.

## State vocabulary and scope

State is signed8 at E+128+j;switch value V is word E+130+4*j. Ordinary push
writes1. Ordinary accepted cases write0. Group comparisons can write0xff(-1)
or0xfe(-2);CASE_END handles those explicitly. This is an in-memory state machine,
not a host boolean chain. Preserve exact0,exact-1 and exact-2 comparisons rather
than classifying all negative values as consumed. Arbitrary other state values
follow the branches below without validation.

All these handlers return2 on every local exit;no automatic yield. Scans only
write next14 to a retained header,then dispatcher2 fetches it. A scanner hitting
another case header does not evaluate that case until its handler is dispatched.

## CASE_OR43

Capture argument pointer and signed index j from f BEFORE resolving operand0
through evtGetValue. AFTER the query read state S and switch V using retained j.
The candidate query occurs even when S==0 or S==-1.

| Condition in order | State write | Continuation |
| --- | --- | --- |
| S==0 | None | End scanner |
| S!=0 and K==V | -1 | Current next14 unchanged |
| S!=0,K!=V,S==-1 | None | Current next14 unchanged |
| S!=0,K!=V,S!=-1 | None | Next-case scanner |

A successful OR comparison latches-1;later OR mismatches preserve that success
and execute the following body. A mismatch before any success scans to the next
case header36..47 or END49. The scanner does not distinguish another OR from
AND,DEFAULT,ordinary cases or CASE_END46. OR with unexpected S==-2 still
compares and can write-1 on equality;no special failed-AND gate exists here.

Example for a normal OR chain with the body following its final OR:starting1,
first mismatch skips to the next OR;second equality writes-1;remaining OR
mismatches preserve-1 and fall through into the body. CASE_END after the body
then consumes it as described below. This is a static deduction for that layout,
not a census of original scripts or an execution observation.

## CASE_AND44

Argument/index capture,query and post-query state/value reads follow OR's order.
The query also occurs for exact0 and exact-2.

| Condition in order | State write | Continuation |
| --- | --- | --- |
| S==0 | None | End scanner |
| S==-2 | None | Next-case scanner;candidate does not affect outcome |
| S!=0,S!=-2,K==V | -1 | Current next14 unchanged |
| S!=0,S!=-2,K!=V | -2 | Next-case scanner |

Thus-1 is not an AND stop condition:subsequent equality preserves success and
subsequent mismatch replaces it with-2. Once-2 is set,further AND handlers
resolve their candidates but skip their bodies without comparing for acceptance.
No reset to1 occurs until CASE_END's unsuccessful-group path. An unexpected
positive state,or negative state other than-2,also reaches the equality test.

A chain starting1 that matches all members retains-1 and executes its body;
a mismatch latches-2,then scan/dispatch advances across subsequent case headers.
CASE_END46 restores1 on that failure and scans to the next case after itself.
This differs from ordinary state<=0 consumed-case handling.

## CASE_END46:group terminator

No operand query. Capture signed f→j and state S,then:

| State | Write | Continuation |
| --- | --- | --- |
| Exact0 | None | End scanner |
| Exact-1 | State0 | End scanner |
| Every other value,including1 and-2 | State1 | Next-case scanner |

For a successful group,the body preceding CASE_END already ran;CASE_END consumes
it and jumps to END_SWITCH's header. For unsuccessful groups,it restores the
active search state and skips to another case header after the terminator.
CASE_END itself lies in the next-case scanner's36..47 stopping set. It is not
END_SWITCH49 and never pops f. Calling it with1 uses the unsuccessful/reset path;
calling it with an arbitrary unexpected signed byte also uses that path.

## CASE_FLAG45:raw any-bit mask

Capture signed f→j,argument pointer and RAW operand0 M. Read state and value;
there is NO evtGetValue call or variable-mask resolution. If state<=0,run end
scanner. If state>0,test word V&M. Any NONZERO intersection accepts:write state0
and leave next14 unchanged. Zero intersection runs next-case scanner without
state mutation. This is any-bit,not `(V&M)==M`;zero mask never matches.
The operand word's bit pattern is used directly even if it resembles a namespace
tag. All32 bits participate;no byte narrowing or signed comparison.

## CASE_BETWEEN47:two resolved inclusive endpoints

Capture argument pointer A and signed f→j. Resolve word A[0] to low L. Then
load A[1] from the retained A AFTER that first query and resolve it to high H.
After both queries,read state and V using retained j. Therefore both queries
occur even when the case is already consumed;the first query can change the
second operand's stored word before it is loaded. Neither query redirects the
retained argument base or stack index.

If state<=0,run end scanner. Otherwise signed compare L against V;L>V rejects.
Then compare V against H;V>H rejects. If neither is greater,accept L<=V<=H,
write state0 and leave next14 unchanged. Rejection runs next-case scanner
without mutation. Endpoints are not swapped/clamped;L>H cannot accept any
signed32 V. Equality at either endpoint is accepted;no float/range conversion.

## Acceptance cases for implementation

Cover ordinary1,consumed0,OR latch-1,AND latch-2 and unexpected state values;
query-before-gate in OR/AND/BETWEEN;OR mismatch after success;AND query while
already failed;CASE_END success consumes versus failure resets/scans;DEFAULT
and ordinary cases with negative group states;raw mask versus variable-tag
word,zero/partial/high-bit mask;inclusive/equal/reversed/negative endpoints;
second operand mutation between queries,captured index versus post-query state;
next-case scanner stopping on46/47 and skipping48;same-invocation fetch.

Original source layouts/group composition,valid stack depth,numeric/pointer/
external callback domains,other VM opcodes and actual script traces remain open.
Implementing a generic host switch or boolean grouping cannot replace these
local state and continuation contracts without checking the original layouts.
