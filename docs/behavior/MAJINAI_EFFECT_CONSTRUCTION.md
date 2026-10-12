# Complete Majinai effect construction and initial payload

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effMajinaiEntry8025c980/f4,effEntry8005ec68/68 and mapGetWork8002b438/14.
Verify:`python3 tools/verify_majinai_effect_construction.py`:five complete/linked
body identities,92 instructions,five spans and three exact DOL direct callers.
Allocator/update identities are linked;effect motion/draw/delete/platform and
original execution are NOT recovered by a containing-body fingerprint.
Read [pool/map ownership](EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md),
[caller/name wait](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md),
[SDK allocator](EVENT_BOOTSTRAP_HEAP_AND_RESET_CALLERS.md) and
[pool bootstrap](MEMORY_POOL_BOOTSTRAP_AND_CLEAR.md).

## Constructor inputs and ordered publication

Inputs are word K in r3 and coordinates X/Y/Z in f1/f2/f3. Retain all four
before calling effEntry(). Retain returned pool record E. No local null/pool
exhaustion test. Then perform these calls/stores in order:

1. E.word14=pointer80302fa0 to original null-terminated string `Majinai`.
2. E.word8=1 (payload record count);RELOAD E.word8.
3. Compute payload request wrap32(reloaded count<<5);call
   __memAlloc(heapIndex3,payloadBytes). Ordinary count1 requests32 bytes.
4. E.wordc=returned payload pointer D.
5. Load update callback address8025c5ec (effMajinaiMain);load original
   binary32 constant1.0 from80428b3c;store callback to E.word10.
6. Read E.flags0,OR numeric2,store back. This occurs BEFORE any payload store.
7. Initialize retained returned payload D through the table below.
8. Call mapGetWork();load WORDffffffff from80428b10;store that WORD at
   returned map pointer+1e,then return retained E.

EffEntry already published flags1/context/zero fields before heap allocation.
There is no whole-record transaction,late-only publication,rollback/free/null
branch or failed-allocation scalar result. If the allocator returns0,the
constructor still publishes E.c=0,callback/flags and requests payload stores at0.
This is original request order,not a safe native failure policy. Valid pool/heap
storage or explicit unsupported-domain handling is a platform requirement.

The payload size uses RELOADED E8,not a hardcoded32 request. An observable alias
or provider can change that count between its store and load. Actual caller's
ordinary path stores1,so32 is concrete source arithmetic,not a measured allocation.
The allocator separately zeroes/flushes successfully returned requested bytes;
the constructor's following writes initialize the fields explicitly.

## Complete ordinary32-byte payload initialization

Payload D is the allocation RESULT retained in r3,not a fresh E.c lookup.
Writes occur in the exact listed order. Coordinates narrow through stfs;no
addition,clamp,round-to-engine-grid or unit/camera position query occurs here.

| Offset | Width / value | Instruction |
| --- | --- | --- |
| 00 | WORD K supplied by caller | 8025ca14 |
| 04 | binary32 X | 8025ca18 |
| 08 | binary32 Y | 8025ca1c |
| 0c | binary32 Z | 8025ca20 |
| 10 | binary32+1.0 (3f800000) | 8025ca24 |
| 14 | WORD255 (000000ff) | 8025ca28 |
| 18 | WORD0 | 8025ca2c |
| 1c | WORD-1 (ffffffff) | 8025ca30 |

These cover exactly32 bytes when reloaded count1. The constructor initializes
ONE payload record;it does not loop through a changed larger request. Unwritten
extra allocation bytes retain allocator/source clear behavior. K remains a full
word,not a byte enum;fields14/18/1c are word stores,not compact byte fields.
Do not give them opacity/phase/timer semantics until their update/read consumers
are recovered. The initial table is a data/layout contract,not the effect's
complete visual animation or lifetime.

E14 is a borrowed resource/identity string pointer. E18 is the SEPARATE writable
instance name buffer managed by effSetName. The battle wrapper later names the
record `mjef_coinup`;it does not replace the borrowed `Majinai` pointer. Payload
D and pool record E have separate storage/ownership and lifetime. The constructor
does not copy model/texture/material resources or bind a native mesh renderer.

## Concrete battle call and direct caller evidence

_battle_majinai_effect calls this at8011466c with K0 and coordinates(0,10,0),
then stores E to event78 and requests name assignment. Its raw script actor-3
is ignored,as the linked wrapper specifies. Thus the constructor receives those
constant coordinates,not Mario's current position. Its later motion/frame basis
belongs to effMajinaiMain and draw code,which remain separate.

The scoped DOL census has three direct linked calls to effMajinaiEntry:

| Site | Containing caller |
| --- | --- |
| 80043528 | _majinai_effect |
| 8006e2e4 | evt_eff |
| 8011466c | _battle_majinai_effect |

This pins direct sites only. Field/generic callers' operands/conditions,REL and
indirect dispatch tables,all global/raw writers and actual render/update cadence
are not inferred from these caller annotations. Reusing this constructor for
all contexts requires those source contracts,not just the known battle vector.

Acceptance:input K/coordinate retention,pool acquisition before resource/count,
count1 store/reload and original heap3 request,payload-result pointer versus
E.c alias,callback/flag publication before payload,all eight store widths/order,
original constants/string identities,map-work query after payload and exact
four-byte+1e publication,retained E return,null and exhausted-domain requests.
Effect main/draw/delete/resources,global driver/map/pool/heap/source ownership,
numeric/platform exception policy and original effect/frame traces remain open.


[Complete update state machine](MAJINAI_EFFECT_UPDATE_STATE_MACHINE.md) and
[draw/material requests](MAJINAI_EFFECT_DRAW_AND_POSE_REQUESTS.md) now establish
state18,alpha14,pose1c and scale10 local consumers,soft04 cleanup and normal
phase progression. Source driver/resource/pose/render/matrix/numeric/platform
and original runtime remain open.
