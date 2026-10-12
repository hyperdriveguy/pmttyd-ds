# Ordinary Jump source references and root registration

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Static source evidence: default weapon event word,three literal child wrappers,
complete state6 registration region in btlseqAct8011b2dc/8ac,and delivered root/
child constructor and scheduler contracts. Verify with
`python3 tools/verify_jump_source_registry.py` and
`python3 tools/verify_event_root_constructors.py`. Read the
[callback-address census](NORMAL_JUMP_CALLBACK_REGISTRY.md) and
[268-command Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md).
This is conditional local source binding,not proof of actual menu selection,
opening tutorial execution or all source writers.

## Original references and selected-event publication

`marioDefaultWeapon_Jump` at803538a8 has word+b0 at80353958 equal80355cf0,
the ordinary Jump script. The [selected-command routine](BATTLE_ATTACK_EVENT_SELECTION.md)
reads selected weapon W+b0,stores it to actor U2a0 and writes category U2ac=0;
null event pointer receives Wait fallback without replacing W for classification.
Thus when the selected W is the default Jump and that field remains unchanged,
its normal event publication is80355cf0. No arbitrary/native jump timer is part
of this chain. Weapon/menu choice and other raw event-field writers remain open.

Three other known literal DOL references are opcode94 operands at80416c14,
80416c24 and80416c34. They belong respectively to16-byte streams
marioAttackEvent_MiniminiFumi80416c10,NemuraseFumi80416c20 and
FunyafunyaJump80416c30. Each complete stream consists of ChildEvent(80355cf0),
DELETE opcode2,then terminal opcode1;all have the same body hash
88f46e50b34472c41f6064069459d997dfe5c917f38bc495707b520a0b569b43.
No USER_FUNC or SET_SPEED/SET_TYPE/SET_PRIORITY appears in these wrappers.
These are conditional CHILD routes rather than duplicate root registrations.
Their own weapon/menu origins are not closed here.

Opcode94 installs a waiting child with supplied flags0,inherits actor/type/local
values,user addresses and metadata under the complete child contract,and returns
through the dispatcher without deleting the waiting parent. Child priority is
low8(parentPriority+1),speed reads CURRENT global evtSpd rather than parent
speed,and new credit/lifetime begin0. On normal child deletion,parent wait clears
and metadata/locals copy back;the wrapper later executes DELETE. Scheduling is
not synchronous recursive execution of the three-word control flow.

## Action state6 conditional registration graph

Read [complete action controller](BATTLE_ACTION_CONTROLLER.md) for all state
routes and status/attribute/RNG gates. The freshly checked state6 instructions
8011b5e0..b7d8 select the actor twice around optional StatusWindowSystemOff.
The selected script can be overridden or suppressed by the zero-gravity,
category,status and Confuse paths. The ordinary U2a0 route calls
`evtEntry(script,10,0)` at8011b794. The category0x10 checked-pointer route calls
at8011b678;category0x02 unchecked route calls at8011b698. A qualifying normal
Jump pointer can reach those dynamic sites only under their corresponding gates.
Electric/Confuse/zero-gravity scripts are separate alternatives,not Jump roots.

Every state6 root call uses priority10 and flags0. It invokes ROOT evtEntry,
not EntryType,ChildEntry or a unit-specific constructor. Root type is byteef;
initial active flags are1,actor word160 starts-1,relationship words6c/70/74
are0,user pointers154/158 are0 and all entry bytes are cleared first. Label
scan,possible append and run-check happen INSIDE the constructor before it
returns. Because supplied flags0 lacks20,the root does not append to the live
manager table even if manager-active is nonzero. Its next priority-table rebuild
supplies scheduling;construction itself never executes the script.

Root captures CURRENT GP and chooses field context if GP.word14==0,battle for
any nonzero word. It is not passed btlseqAct's retained battle pointer as context.
Battle_init's GP14=1 before ReInit is separately pinned in
[event bootstrap callers](../event/EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md);that source
binding supports ordinary battle context under stable state,not a hardwired
battle-array rule or complete scene transition proof.

## Exact post-constructor identity and actor binding

For nonnull returned entry E,state6 performs:

1. At8011b7a4 read E.ID15c;at8011b7a8 store it to actor U2a8.
2. At8011b7ac read word U0 (unit registry identity);at8011b7b0 store it to E160.
3. Read actor wait-event ID U290 at8011b7b4. Nonzero calls evtDeleteID at
   8011b7c0,then clears U290 at8011b7c8. Zero skips both operations.
4. Advance action channel6 and exit. Null E skips all these bindings/deletion.

E160 therefore becomes U0 AFTER root label scanning/run-check. It is not
initialized directly as Mario222,actor selector-2,or retained B420. Scripts'
-2 owner resolution uses this bound entry field and existing BattleTransID
services. Do not conflate kind,selector,slot,unit identity and event identity.
The root takes pre-increment global event ID,then increments the global;zero
wrap correction follows run-check. U2a8 copies the actual returned entry ID.

Speed164 captures current global evtSpd804151ec during construction;credit168
and lifetime00/04 initialize0. Initial DOL evtSpd1.0 and evtID1 are static
initializers,not proof of immutable values at this registration. Later setters
and restart have their documented policies;no priority/type/speed mutation is
performed by state6 after root return. Parent/inline/child paths in Jump have
separate inherited/post-construction field behavior. See
[root constructor](../event/EVENT_ROOT_CONSTRUCTORS_AND_APPEND.md),
[descendants](../event/EVENT_CHILD_BROTHER_AND_INLINE_CONSTRUCTION.md),
[run/ID variants](../event/EVENT_RUN_AND_ID_OUTPUT_VARIANTS.md) and
[setter leaves](../event/EVENT_CONTROL_OPCODES_AND_ID_SCANS.md).

## Constructor run-check source correction for the native owner

Fresh complete evtEntryRunCheck80037738/78 reads word at r13+1a20 at80037744.
With r13=8041cf20,this is8041e940,the REQUESTED system-level enum. It does NOT
call marioStGetSystemLevel or read GP18. Exact recognized1/2/3/4 request
StopAll masks1/2/10/ef;other values request no stop. The entry argument is unused.
The [label/run-check contract](../event/EVENT_LABEL_SCAN_AND_ENTRY_RUN_CHECK.md) already
states this correctly. marioStGetSystemLevel separately returns whole GP18;
that is a different service in [system-level ownership](../event/EVENT_SYSTEM_LEVEL_AND_SPEED.md).

Current include/event_creation.hpp's comment says EventCreationRuntime.systemLevel
is the whole GP18 mask. That source assertion conflicts with80037744. Native
owner should bind REQUESTED8041e940 to constructor run-check,and keep GP18 for
the actual getter/clock gate. Example:request3 commonly leaves GP18 low nibble7;
constructor must dispatch StopAll10 from enum3,whereas binding7 yields no match.
Host semantic tests can pass while this source binding remains incorrect.
ID2 changes documentation/evidence only;native header/binding is left to its owner.

## Implementation acceptance and remaining scope

Check default weapon+b0 publication,each wrapper child opcode/parent waiting,
state6 gates and exact root10/flags0/typeef,global context selection,pre-increment
ID/global speed capture,initial actor-1 before returned-entry actor assignment,
E.ID→U2a8 then U0→E160 before wait deletion,flags0 no live append,and requested
enum versus GP mask. Compare root priority10 with wrapper child11,global versus
parent speed,and full body links before claiming original Jump timing.

Menu/weapon/source writers,complete field→battle sequence,unit data7 child
selection,other dynamically requested scripts,status/camera/audio/audience/
damage/domain callbacks,global ID/speed/context writers,packed provider binding
and original execution remain EVT-01/JUMP-01 requirements. This answers the
conditional registration parameters and scoped direct registry census;it does
not prove every reachable Jump caller or the early battle's fidelity.


[Complete data-child service/Mario key7](BATTLE_UNIT_DATA_CHILD_CONTINUATION.md)
now supplies the local dynamic lookup and original803729c8→key7→80416bd0
source chain. The tiny child still allocates/waits/copies back,and its source
span plus trailing word is absent from the current three-stream pack. Other
unit/table writers,dynamic source domains and original execution remain open.
