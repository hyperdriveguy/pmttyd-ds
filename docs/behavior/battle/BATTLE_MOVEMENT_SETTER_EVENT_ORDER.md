# Complete native-requested movement setter event order

US G8ME01 revision0; ID2,2026-10-12. Complete btlevtcmd_SetMoveSpeed8010d860/78,
btlevtcmd_SetFallAccel8010d770/78 and their two8-byte leaves. Verify
`python3 tools/verify_battle_movement_wrappers.py`;read
[source tokens](BATTLE_MOVEMENT_WRAPPER_SOURCE_TOKENS.md),
[float routing](../event/EVENT_FLOAT_OPERAND_ROUTING.md) and
[actor translation](BATTLE_ACTOR_SELECTOR_TRANSLATION.md).
Native EVT-01/JUMP-01 request;no native code or original execution claim.

## Identical wrapper sequence up to the selected leaf

Retain event E and read WORD E18 as argument pointer A ONCE. Incoming firstCall
is ignored;both wrappers execute all resolutions/stores on every invocation.

1. Read RAW WORD A00;call evtGetValue(E,rawActor).
2. Call BattleTransID(E,resolvedActor),retain translated word I.
3. Read RAW WORD A04 through the RETAINED A,not a fresh E18.
4. Call evtGetFloat(E,rawFloat),retain its f1 result in f31.
5. Capture CURRENT WORD _battleWorkPointer8041eb90 as B.
6. Call BattleGetUnitPtr(B,I),retain returned unit U for the leaf call.
7. Restore retained float to f1;call the selected leaf;ignore its raw return.
8. Return event completion2.

The A04 raw load occurs after translation but before GetFloat. The original
register holding A is then reused to retain I;no later argument pointer is read.
Do not snapshot both operands before GetValue/translation,resolve float before
translation or use a reloaded argument block after providers change E18.

BattleTransID has its OWN earlier global battle read even for pass-through
selectors. That helper's captured battle is not the wrapper's later B. Operand/
translation/GetFloat providers can change global battle before step5;unit lookup
uses the fresh B with retained I. No wrapper-global B is captured at entry as
in the HP event family. Generic actor/query pipelines must preserve this family.

## Float survives lookup;leaf stores only its selected binary32 field

| Wrapper | Called complete leaf | Direct mutation |
| --- | --- | --- |
| SetMoveSpeed | BtlUnit_SetMoveSpeed8012815c/8 | stfs f1 to U170 |
| SetFallAccel | BtlUnit_SetFallAccel8012816c/8 | stfs f1 to U174 |

Each leaf contains only its stfs and return. No clamp,absolute-value conversion,
range/sign/zero/sentinel check,timing update,velocity reset,move-count change,
audio/animation request or other field mutation. Wrapper fmr copies do not add
single rounding;the leaf storage narrows to binary32 according to the original
floating/platform contract. Keep the resolved value across lookup services.

The source Float resolver supplies encoded-literal/variable routing,not IEEE
reinterpretation of raw A04. Nonfinite/rounding/domain fidelity belongs to that
resolver/platform evidence;a direct authored leaf input is not a source-wrapper
observation. The eight used source requests are separately pinned.

## No translated-1 or null-unit continuation branch

Both wrappers STILL execute GetFloat and the fresh B lookup when I==-1. The
complete registry getter returns0 for signed negative/64+ IDs,but the wrapper
then calls its leaf with that raw0. The leaf performs its field store without
a null guard. It does not safely skip or return2 before the store at this boundary.
There is no special system/party/source fallback or delay state.

Do not copy HP's translated-1 early completion into these setters. Native address
providers can report invalid source state explicitly,while retaining the source
unchecked behavior distinction. Current Game/source must establish valid actor,
unit/storage and float domains;the local body alone does not prove them.

## Native acceptance and integration requirements

Use actual shared EventNativeRuntime operand/actor/registry memory,retained A/I/F
and late B. Test GetValue/translation mutations of A04 versus E18,translation's
battle versus later lookup battle,float provider/lookup mutation,all actor selector
branches,translated-1/null lookup,ignored firstCall and leaf-only field bytes.
Preserve raw tokens and event result2;bind original callback addresses explicitly.

These complete local wrappers/leaves close the requested mutation/completion
protocol. Float/actor/getter bodies are linked and separately documented;native
callback registration/Game movement/timing/source writers and original execution
remain EVT-01/JUMP-01 requirements. No motion trajectory or full Jump completion
is established merely by writing speed/fall fields.
