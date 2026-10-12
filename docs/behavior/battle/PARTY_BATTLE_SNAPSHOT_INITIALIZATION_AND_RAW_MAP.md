# Seven-party battle snapshot initialization and preserved bytes

Complete static US G8ME01 revision0 BattlePartyInfoWorkInit800f7c20/0x84.
Native request:[EVT-01](../../DOCUMENTATION_REQUESTS.md#evt-01--native-event-scheduler-and-dispatch-closure-open).
[Lifecycle/import requirements](PARTY_BATTLE_SNAPSHOT_INIT_IMPORT_AND_LIFETIME.md)
links this initializer to previously recovered consumers.
[Verifier](../../../tools/verify_party_snapshot_initialization.py) pins the whole
33-instruction body,one direct caller instruction,two linked body identities,
startup r2,and actual binary32 constant1.0. No original execution or whole
battle/save/source lifecycle proof.

## Record addressing and ordered requests

Retain passed battle B. Load binary32 at80422174 (startup r2=80427440,
signed displacement-21196) into retained f31. Exact source bits3f800000=1.0.
Initialize local kind K224 and retained integer zero Z0. No global battle lookup,
pouch/unit registry query,allocation,or B-null guard.

For each K224..230 inclusive,compute with original word arithmetic:
`R=wrap32(B+0x162a0+44*(K-224))`. Perform in order:

1. `stfs` retained1.0 to R+0.
2. Store WORD(R+0x24)=0.
3. Request memset(R+4,0,30),with LENGTH30 decimal (0x1e).
4. Store HALFWORD(R+0x28)=0.
5. Increment local K by1.
6. Store HALFWORD(R+0x2a)=0.
7. Signed compare K<231;repeat if true.

The second final halfword store occurs after local kind increment but before
loop continuation. Retain R across memset;do not recompute it from incremented
K for that store. All7 fill requests execute even when records already contain
zeros. The constant is loaded ONCE,not reread per record after a service.
There is no bulk308-byte memset or explicit scalar success/count assignment;
rawr3 is the last memset request's return. Callers ignore it.

## Complete44-byte mutation map

| Relative bytes | Width/action | Final direct producer value |
| --- | --- | --- |
| 00..03 | binary32 store | 1.0,bits3f800000 |
| 04..21hex | exactly30-byte memset request | zero bytes from fill service |
| 22..23hex | NO write/request coverage | preserved prior bytes |
| 24..27hex | word store before fill | zero |
| 28..29hex | halfword store after fill | zero |
| 2a..2bhex | halfword store after kind increment | zero |

There are7 records/308 bytes of addressed storage;294 bytes are covered by
these writes/fill requests,14 gap bytes remain untouched. These counts describe
the local instruction/request graph,not observed memory snapshots. Services
or address aliases can have other effects;the fill identity is pinned separately.
Do not describe gap retention as “unknown cleared bytes” or reinitialize the
float to0.0. Typed/big-endian raw bytes must remain separate from host padding.

The original prologue saves f31 with stfd and psq_st,the epilogue restores it
with psq_l then lfd. The recorded1.0 stores are literal scalar stfs values;
no scalar arithmetic,random choice,time tick,or level clamp appears here.
Platform ABI/GQR0 state and helper implementation remain separate evidence.

## Scoped original caller

Aligned direct DOL call search found one call to this entry,at800f9018 inside
BattleInit. Its instruction and containing BattleInit body identity are pinned;
full BattleInit behavior is NOT claimed complete by this slice. This is a static
call site,not a measured invocation count or proof of one reset per battle.
REL/indirect/raw/event callers are outside this scoped search.

[Existing BattleInit stage-gate contract](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md)
places this request after BtlUnit_Init and before BattleStageInit. This local
initializer now closes the previously linked service body. Other BattleInit
branches and source/runtime scheduling still require their own contracts.
