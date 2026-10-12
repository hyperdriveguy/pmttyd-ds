# Confusion selector: class candidates and menu-type publication

Complete static US G8ME01 revision0 BattleSetConfuseAct8011f62c/0x610.
Companions:[weapon candidates](CONFUSION_WEAPON_SELECTION_AND_TARGET_POLICY.md),
[operations/party/event outputs](CONFUSION_OPERATIONS_PARTY_AND_EVENT_OUTPUTS.md).
Native request:[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Verifier](../../../tools/verify_confusion_action.py) pins the complete388
instructions,eight called service identities,the nine-way operation table,and
four original charge-event pointer words. No original execution or global query
reachability claim. RNG requests are literal original irand calls,not a new
uniform host distribution.

## Entry,actor admission,and route-specific mutation

Arguments B and U are retained;M=wrap32(B+0x171c). Read U8(U+0x20);
exactly3 clears that byte BEFORE any service. Request HelpSetDispType(0).
Then read S8(U+0x0c). Nonzero returns fixed script803532b0
(`subsetevt_confuse_flustered`). Zero reads signed WORD(U+8);admit exactly222
or224..230 inclusive. Other kinds return the same script.

Only after actor admission,if U8(M+0x550)!=0,clear BYTE(U+0x20),then
BYTE(M+0x550). There is no U-null guard or capture of current global actor
at this entry;subsequent called builders/cursor helpers have their own owners.
The function returns a script address or null. It does not register an event,
assign a unit event/category,or return VM completion2.

## Shared candidate-list mechanics

All four local candidate scans use this exact representation:

- Private cursor output at SP08;private halfword list starts SP0c.
- Capture cursor's signed WORD(C+8) once as N.
- Local ordinal J increments as a WORD,but addressing/comparison use Low8(J).
  Continue while signed Low8(J)<N. A count>255 can prevent termination;no
  native count cap or proof of source bounds is supplied by this caller.
- When admitted,write source ordinal as a halfword at list+2*Low8(K),then
  increment candidate count K. The mask is0x1fe after rotating K left1.
- After the scan,write halfword0xffff at list+2*Low8(K),even when K0.
- Use Low8(K) as the irand bound. If that byte is zero,no random call occurs.
  Otherwise call irand(bound) once,read SIGNED halfword from list+wrap32(2*raw
  random return),and store that value as WORD(currentSP08Cursor+0).

The list does not have a guarded capacity;it shares an80-byte frame with saved
registers at+0x44. Do not replace its low-byte addressing with an unbounded host
vector and describe malformed-count behavior as original safe admission.
For valid small counts,these mechanics select among admitted source ordinals,
not compacted table positions. Keep original RNG interleaving;[RNG accounting](BATTLE_FORTUNE_INIT_AND_CALL_BINDINGS.md)
explains why original modulo irand differs from a host uniform distribution.

## Class scan and fresh type/enable reads

Request MakeActClassTable(B),then GetCursorPtr(M,14,&SP08). Ignore returns.
Capture its current count. For each class ordinal,read WORD(M+20*Low8(J)+0x0c)
enabled first. Zero skips type read. Nonzero reads class type+8;exclude exactly4
and2. Every other type,including5/13,can enter the local list if enabled.
No current-state,type-range,or weapon check occurs during this scan.

Low8(candidateCount)==0 returns flustered803532b0. Otherwise randomize once
and store the selected source ordinal in the current cursor. Then reload
cursor/index,derive M+20*index,read its CURRENT type,and store WORD(M+0x540).
Reload cursor/index AGAIN,read enabled CURRENTLY. Zero returns flustered;
the type write is not rolled back. This complete type store is8011f788.

Reload CURRENT M540 and route it signed:

| Type | Local route |
| --- | --- |
| 0,1,2,4,6,7,8 | weapon build/selection |
| 5 | operation build/selection |
| all other words | flustered803532b0 |

Initial candidate exclusion alone does not remove2/4 from this compiled route:
irand or shared-memory mutation can change a row between filtering and the
fresh type read. Under unchanged ordinary class production,only0/1/6 enter
weapon selection,5 operations,and13 returns flustered after publication.
These are conditional source summaries,not a measured distribution or global
reachability proof. No Input-state29 write occurs in this selector.

## Port requirements and source boundary

Implement retained passed actor/menu,entry mutations and short circuits,
captured count/low-byte list mechanics,original RNG requests,fresh cursor reads,
type publication before enabled recheck,and literal script/null returns.
Use [complete class producer](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md)
and original assets. Type13 can be written here but this path returns flustered;
it does not itself make a selected-weapon query with type13. Wider source-event/
query domains,party delegates,and native Game/runtime remain open.
