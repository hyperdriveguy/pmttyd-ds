# Complete battle actor selector translation and system-unit query

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BattleTransID801253ec/b4 and BattleGetSystemPtr800f8814/60.
Verify:`python3 tools/verify_battle_data_child.py`;Mario/partner/registry bodies
are linked identities with [complete query contracts](BATTLE_RULE_THRESHOLD_AND_UNIT_QUERIES.md)
and [registry policy](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md).
Read [unit-data child service](BATTLE_UNIT_DATA_CHILD_CONTINUATION.md) for the
ordinary Jump(-2,7) consumer. Static evidence,not original identity traces.

## Exact signed selector table and capture order

Inputs are event E and signed word selector S. Compare S against0,then read
CURRENT global battle B from8041eb90 at801253fc BEFORE taking the nonnegative
branch. Even ordinary indices and -2/unknown-negative routes perform this global
read. No B/E null guard or selector range/identity validation.

| S | Return operation |
| --- | --- |
| >=0 | Return original word S,without registry lookup |
| -1 | Return -1 |
| -2 | Read and return word E+160 (bound actor) |
| -3 | Call BattleGetMarioPtr(captured B);dereference returned unit+0 and return word |
| -4 | Call BattleGetPartyPtr(captured B);null returns-1,otherwise return unit.word0 |
| -5 | Call BattleGetSystemPtr(captured B);dereference returned unit+0 and return word |
| -6 | Read and return captured B.word424 |
| <=-7 | Return original word S unchanged |

Only -4 has a missing-query fallback. -3/-5 dereference null if their query
returns null;do not normalize all sentinels to-1. -2 returns raw E160 without
checking kind,active entry,registry existence,ID equality or sign. -6 is B424,
not the current actor B420 and not E160. It remains a separate source-owned
field. For other paths no E dereference occurs;null E alone is therefore not
always the local fault boundary. Returned unit.word0 is the query result's
actual stored identity,not the slot index at which the query found it.

The service does not resolve event operand namespaces itself,write fields,
consume RNG,choose targets,allocate events or retain a unit pointer for its
caller. Most wrappers first invoke evtGetValue and then call this translator.
A later BattleGetUnitPtr can freshly recapture global B2;the translator's B
is not implicitly shared with that lookup. Preserve both read points. A native
conversion from every negative number to a enum/safe sentinel would differ.

## Complete system-unit query

BattleGetSystemPtr(B) retains EXPLICIT B,scans indices0..63 ascending and calls
BattleGetUnitPtr(B,index) for each. Skip null. For each nonnull U,read signed
word kind U8 and admit exactly220. Return FIRST match,else null after64.
There is no belong-byte test,flag08 rejection,HP/life/visibility/status filter,
pouch lookup,current actor selection or special global pointer fallback.

This differs from GetMario's kind222/belong0 query and GetParty's224..230/
flag08-clear/belong0 query. System lookup can admit kind220 with nonzero belong
or zero/negative HP. Other unit fields are not read for local admission. The
name “system” does not add an immortal/active/current-scene predicate. All three
queries use the explicit captured registry;their returned identities may differ
from scan slots if other source writers alter unit.word0.

## Consumer obligations and acceptance

For RunDataEventChild(-2,7),the root's later actor binding supplies E160.
The resulting I is preserved across subsequent unit/key queries and written
into the returned child AFTER its constructor inherited the original parent
actor. Calling the service for another S can deliberately override that inherited
actor;there is no additional “same owner as parent” guard.

Fixtures should cover every selector boundary0,-1..-7 and large signed values;
unconditional global read;E160 versus B420/B424;Mario/system absence versus
partner null fallback;returned identity versus slot;current B/B2 changes;and
system query first/multiple matches,all64 misses,null slots,kind219/220/221,
nonzero belong,flags and HP0/negative remaining admitted. Pointer/address service
exceptions must be explicit native domain policy,not claimed original fallbacks.

Unit/actor/registry/current helper writers,lifetimes,packed memory providers,
source reachability and original execution remain open. This is a complete
local translation/query contract,not a guarantee every translated ID is valid.
