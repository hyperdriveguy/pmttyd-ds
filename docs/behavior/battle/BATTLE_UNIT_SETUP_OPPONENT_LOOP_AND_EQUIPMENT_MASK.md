# Battle unit setup:opponent provisioning and200-slot equipment scan

Completes [startup actors/imports](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md)
for US G8ME01 Btl_UnitSetup800f8aac/0x2f8.Static evidence only.
[Verifier](../../../tools/verify_battle_unit_setup.py) pins all190 instructions,
seven service identities,and source setup/coordinate/equipment switch bytes.

## Retained group,current count,captured record base

After optional party services,load WORD(retainedG+4) as record pointer R0.
Initialize local ordinal J0 and parallel byte offset O0.At EACH loop test read
CURRENT signed WORD(G+0) count;continue while signedJ<count.No count snapshot,
64-unit admission cap,group-null guard,or successful-entry check is added.

Request BtlUnit_Entry(currentRecord),then store LOW8(J) to returnedU+0x0e,
store WORD(U+0x308)=0,retain U.Reload that byte and sign-extend it as I.
Read WORD(retainedD+0x5c+wrap32(4*I)).Nonzero is the direct override:
store it to U308;read CURRENT WORD(U+0x1c),OR0x40000000,store back.
This branch makes NO EquipItem request.

Zero override reads WORD(D+0x1c+O),stores it to U308;REREADS that source
word as argument5,requests BtlUnit_EquipItem(U,1,newSourceValue).No extra
r3 unit reload is needed in original instructions;it still holds entry return.
Do not collapse the two source reads or import direct-override flag effects
into this equipment branch.

The override index uses SIGNED BYTE readback of stored Low8(J),while fallback
sources use full parallel word offset O=4*J.These can diverge for large counts;
there is no J<128 guard.Do not describe these source arrays as validated bounds
or normalize negative readback to an unsigned ordinal.

## Tail uses retained ordinal offsets and current unit resources

For each emitted unit,derive source D+0x3c+O,advance record pointer by48,
increment J by1 and O by4,then store the previously selected source word to
U+0x30c.The ordering uses OLD O for that load despite local increments.
Read S16(U+0x108),store its low16 to U+0x10c;read S16(U+0x10e),store to
U+0x112.These copy current max/base fields into current HP/FP AFTER equipment/
override services.No minimum/maximum clamp,status clear,additional pouch
import,or constructor wait is inserted here.

Repeat using live G count and current advanced record pointer.Count changes
by services affect later iterations;record base itself is not reread from G4.
A failed unit entry still reaches unchecked stores;full entry allocation/registry
failure policy is in its separate complete contract,not repaired by this caller.

## Acting ID and equipment-mask reset

After loop,store WORD(B+0x420)=-1,then WORD(B+0x163f4)=0,in that order.
Retained P is the pouch pointer acquired before all unit services.No late
pouchGetPtr here.Scan exactly200 SIGNED halfwords at P+0x38a+2*slot,
slot0..199,with current per-slot reads.Convert each to wrapped(itemId-308);
unsigned<=11 dispatches through the full12-word table8034c6f0.Otherwise skip.
Each mapped case rereads current B163f4,ORs its bit,and stores it.

| Signed item ID | Case target | OR bit |
| --- | --- | --- |
| 308 | 800f8ca4 | 0001 |
| 309 | 800f8cb8 | 0002 |
| 310 | 800f8ccc | 0004 |
| 311 | 800f8ce0 | 0008 |
| 312 | 800f8cf4 | 0010 |
| 313 | 800f8d08 | 0080 |
| 314 | 800f8d7c | no write |
| 315 | 800f8d1c | 0100 |
| 316 | 800f8d30 | 0400 |
| 317 | 800f8d44 | 0800 |
| 318 | 800f8d58 | 0200 |
| 319 | 800f8d6c | 1000 |

Literal mapping is nonmonotonic and314 is a no-op;do not generate bit positions
from(itemId-308) or assign guessed badge names.Duplicates still execute each
OR/store;no equipped-count query,sorting,early termination,or random draw.
After200 entries return1 explicitly,regardless of admitted group count or
computed mask.This is local function success,not VM completion2/readiness proof.

## Port/source boundaries

Implement captured record base/definition/pouch,live count,signed byte versus
full offset addressing,branch-specific equipment requests/flags,resource copy
order,acting-ID reset,and exact200-entry mask mapping.Reuse original setup
and pouch data with explicit original/native addresses.Zero/negative group
counts skip only the opponent loop,not fixed actor setup or final scan.

The two previously scoped Btl_UnitSetup import call sites are now closed by a
complete caller contract.End reimport,actual setup callers/source lifetimes,
whole enemy/party resources/init events,query-event reachability,Game/platform/
original execution/full-game requirements remain open.No end-to-end battle
completion is inferred from full local setup code.
