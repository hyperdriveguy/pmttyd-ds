# Recovery and Happy badge event scripts

US G8ME01 revision0, inspected 2026-10-12. B evidence is original DOL script
words and callback instructions. Friendly opcode/function names are annotations
from the pinned public sources; event execution rules are linked below.
No original execution, pose comparison or Switch equivalence was measured.
See [baseline](../ORIGINAL_BEHAVIOR.md) for identity/provenance. Original scripts
and disassemblies remain outside source history.

| Script | US address / size | Scheduling caller |
| --- | --- | --- |
| subsetevt_status_recovery | 0x80353704 /0xc0 | [turn recovery](BATTLE_TURN_RECOVERY.md) |
| subsetevt_happy_heart | 0x80351a3c /0xd4 | [regeneration](BATTLE_REGENERATION.md) |
| subsetevt_happy_flower | 0x80351b10 /0xd4 | regeneration |

LW(n) denotes local work word n. The caller binds unit ID to event+0x160;
this is distinct from LW(n). The [event runtime](EVENT_RUNTIME.md) and baseline
specify callback results, local variables, child ownership and frame waits.
Fresh evtEntry clears its whole0x1b0-byte entry at0x80037530 and explicitly
zeros all16 locals. The inspected turn callers set event+0x160 without seeding
LW(3) or LW(10). Preserve the different operand sources below.

## Status recovery override and fallback (B)

At script0x80353704:

1. CheckDataOfDataTable(LW(10),destination=LW(0),key8).
2. If LW(0)≠0, RunDataEventChild(LW(10),8), then END_EVENT. This branch skips
   every fallback instruction below, including StartWaitEvent.
3. Otherwise StopWaitEvent(−2).
4. GetBodyId(−2,destination=LW(0)); AnimeChangePoseFromTable(−2,LW(0)).
5. GetPos(−2,LW(0),LW(1),LW(2)), overwriting the body ID in LW(0).
6. SetFallAccel(−2,encoded float operand0xf24a7c19).
7. JumpPosition(−2,LW(0),LW(1),LW(2),30,−1).
8. StartWaitEvent(−2), then END_EVENT. END_SCRIPT follows as a data terminator.

−2 resolves through BattleTransID to this event's owner word+0x160.
LW(10) instead resolves a local integer, then translates as a unit ID.
On the fresh turn-created event it is0. Therefore the data-key override query
uses unit ID0, while fallback operates on the bound recovering unit. This is
an original operand distinction, not an instruction to substitute self for0.
Other callers or child constructors can seed locals differently.

The float token is retained exactly; the public fixed-point float convention
(R evidence) decodes it as409/1024=0.3994140625. Do not silently replace it
with decimal0.4. SetFallAccel resolves the first operand through evtGetValue
and BattleTransID, obtains the second through evtGetFloat, then calls
BtlUnit_SetFallAccel, whose entire US body stores the float at unit+0x174.
Independent recovery of evtGetFloat's full coercion rules remains open.
JumpPosition's continuation/motion implementation is not covered here;30 and−1
are its literal arguments, not yet a measured jump-duration/gravity contract.

No sound callback or dialogue command occurs directly in this script. Data-key
children, pose services and motion/effect services can have additional behavior.

## Happy Heart and Flower pose gate (B)

Both scripts have identical local structure with pose type62 for Heart,63 for
Flower. They do not add HP/FP: their caller already mutated resources and emitted
its recovery effect before event creation.

1. GetBodyId(−2,LW(5)).
2. CheckDefinePosetype(−2,LW(5),poseType,LW(0)).
3. If LW(0)≠0, test CheckStatus(LW(3),9,LW(0)); only if result0 continue.
4. Then test CheckStatus(LW(3),1,LW(0)); only if0 continue.
5. Then test CheckStatus(LW(3),2,LW(0)); only if0 call
   AnimeChangePoseType(−2,LW(5),poseType).
6. Outside all four conditionals, WAIT_FRAME(60), StartWaitEvent(−2), END_EVENT;
   END_SCRIPT follows.

The status queries short-circuit in order Freeze, Sleep, Stop, and use LW(3).
On fresh turn-created entries LW(3)=0, independently of the event owner.
Pose lookup/change and wait restart still use−2. Do not replace all those
operands with one assumed actor ID. There is no StopWaitEvent call in these
scripts, and no pose-play-completion wait. The60-count wait occurs even when
pose support is missing or a status gate rejects the pose change.

WAIT_FRAME follows source scheduler/timescale rules; this literal is not proof
of60 rendered frames or precisely one wall-clock second. StartWaitEvent may
create/restart the wait behavior before END_EVENT deletes this event.

## Data-table callback contracts (B)

CheckDataOfDataTable at0x80109f88,size0x88 receives unit operand, raw output
variable token and key operand. Resolve unit with evtGetValue then BattleTransID,
fetch BattleGetUnitPtr; resolve key with evtGetValue; call BtlUnit_CheckData and
write its result through evtSetValue(outputToken). Return2, completing the
command immediately. Unit validity is assumed after lookup.

BtlUnit_CheckData, documented in [event dispatch](BATTLE_EVENT_DISPATCH.md),
returns1 for a matching key even if that key's value pointer is null.
BtlUnit_GetData at0x8012733c,size0x40 scans the same8-byte key/value entries
from unit+0x18, stopping at key0; matching key returns its word value, missing
or null table returns0. It assumes a valid unit and a correctly terminated table.

RunDataEventChild at0x8010a010,size0x9c resolves unit/key similarly and calls
GetData. Value0 returns2 without creating a child. Otherwise call
`evtChildEntry(parent,value,0)`, clear parent's opcode byte+0x0a, write translated
unit ID into child+0x160, and return255. There is no null-allocation check before
writing the returned child. The event runtime's parent/child wait and continuation
rules apply;255 stops this dispatch invocation. The parent later proceeds to
its explicit END_EVENT, rather than running fallback after the child.

A present key8 with null value therefore enters the override branch but creates
no child and still ends the recovery event, skipping the fallback. Key presence
and usable script pointer are separate predicates.

## Wait-event callbacks (B)

StopWaitEvent at0x8010a1fc,size0x70 resolves unit operand via evtGetValue and
BattleTransID, then fetches the unit. If unit+0x290 event ID is nonzero, call
evtDeleteID on it and clear that field. Return2. There is no explicit live-ID
precheck or unit-null guard in this wrapper; deletion semantics are separate.

StartWaitEvent at0x8010a26c,size0x64 resolves/fetches unit the same way. If wait
script pointer unit+0x28c is nonzero, call BattleRunWaitEvent(unit); otherwise
skip. Return2. Its cancellation/replacement and identity behavior is in
[event dispatch](BATTLE_EVENT_DISPATCH.md); do not implement this as only a
pose assignment or treat it as blocking until the wait event finishes.

## Body, status and pose wrapper contracts (B)

GetBodyId at0x8010ead8,size0x74 resolves unit, fetches it, calls
BtlUnit_GetBodyPartsId and writes result to the raw output token through
 evtSetValue; return2. Body-part ID is a data binding, not a hardcoded1.

CheckStatus at0x80105f0c,size0x94 resolves/translates unit operand but reads its
status operand **raw**, narrowing to signed byte. It calls BtlUnit_CheckStatus,
writes normalized0/1 to the raw third operand via evtSetValue and returns2.
These scripts supply literal1/2/9, so the raw-versus-resolved distinction does
not alter those status IDs; it matters to other consumers of this wrapper.

AnimeChangePoseFromTable at0x801064d8,size0x6c resolves unit and part operands,
gets the part through BattleGetUnitPartsPtr and calls btlDispPoseAnime(part),
returning2. Pose state selection and renderer behavior require that callee.

AnimeChangePoseType at0x80106b20,size0x98 resolves unit, part and type; fetches
part; calls BtlUnit_GetPoseNameFromType(part,type), ignoring its returned value,
then BtlUnit_SetAnimType(part,type), and returns2. Preserve both calls/order;
sound/pose lookup internals remain outside this wrapper contract.

CheckDefinePosetype at0x801062e0,size0x114 resolves unit, part and desired type;
its output token is raw. It reads the part's8-byte pose table at+0x1bc.
Desired signed types≤28 use stopType28;29..39 use39;40..69 use69;≥70 immediately
write0. Scan nonnull table entries: an entry type≥70 writes0; a matching desired
type writes1; otherwise an entry equal to stopType writes0; otherwise advance
8 bytes. Null table writes0. Matching takes precedence over the group stop
marker. There is no entry-count bound or pointer-end check in this local scan.
The Happy types62/63 use group stop69. No animation is played by this check.

## Identity translation used here (B)

BattleTransID at0x801253ec,size0xb4 preserves nonnegative IDs.−2 returns
 event+0x160;−1 remains−1;−3 resolves Mario and reads unit+0;−4 resolves partner
and returns its ID or−1 when absent;−5 resolves the helper at0x800f8814 and
reads its unit ID;−6 reads global battle+0x424. Other negatives pass through.
The−5 helper's gameplay role remains a naming/data dependency. Mario/−5 lookup
paths lack a local null fallback; do not infer all special-ID translations are
safe for missing units. This translation does not read the local work words.

## Differential obligations and open scope (P)

Compare key8 absent/present/null-value, owner ID differing from local0,
LW(3)/LW(10) supplied by other constructors, child event stop/resume/ownership,
pose table lacking62/63, group69 encountered before a desired entry, Freeze/
Sleep/Stop short-circuit, a stale wait-event ID, and allocation failure handling.
Confirm the encoded fall acceleration through evtGetFloat, then trace the actual
jump callback and pose/sound behavior against original execution.

The scripts' local branch/call order and these wrapper contracts are recovered.
Remaining: motion continuation, numeric float resolution, pose data/animation
selection and sound services, data-key8 bindings, all other recovery callers,
unit initialization and event wait/conclusion internals. Full status/game
requirements still depend on those contracts and execution comparisons.
