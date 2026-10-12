# Complete effect-pool acquisition and map-work publication helpers

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effEntry8005ec68/68 and mapGetWork8002b438/14. Verification:
`python3 tools/verify_majinai_effect_construction.py`. Read
[Majinai constructor](MAJINAI_EFFECT_CONSTRUCTION.md) and
[name lookup/copy](../battle/JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md).
Local source behavior;no original pool/scene/frame/exception execution claim.

## Acquisition captures one pool and scans exact zero flags

Read effect-manager pointer80415ee0 (r13-7040),then signed count atmanager0,
then record-array pointer atmanager4. Capture count into CTR. Count<=0 skips
scan and uses array BASE. For positive count,scan ascending40-byte records,
reading WORD record0. EXACT0 is free;EVERY nonzero pattern is occupied.
Do not test only bit1 or a separate allocation boolean.

When an available record is found,initialize it below. If all positive-count
records are occupied,the pointer advances after each last test and initialization
still occurs ONE RECORD PAST the declared array. There is no return0,error,
capacity growth,oldest-effect eviction or constructor refusal. Nonpositive
count initializes array base regardless of its old flags. Free capacity and
valid mapped storage are source/platform preconditions,not recovery supplied
by this function. The captured manager/count/array are not reread during scan.

## Exact partial record initialization and preserved name tail

For chosen record E,perform:

| Store order | Field / value |
| --- | --- |
| 1 | E.word0=1 |
| 2 | Reload GP pointer80415fd0;read word GP14,normalize any nonzero word to1;E.word4=that0/1 |
| 3 | E.word14=0 |
| 4 | E.word8=0 |
| 5 | E.wordc=0 |
| 6 | E.word10=0 |
| 7 | E.BYTE18=0 |

Only the first name byte is cleared. Bytes19..27 (15 bytes) retain prior raw
storage;the record is NOT memset40. The empty first byte means an initial
null-terminated name is empty,but does not prove the tail was zeroed or unused.
The field order14 before8/c/10 follows8005ecb8..c4. GP is queried AFTER publishing
flags1;the pool came from its separate manager pointer. No context-equality
check or synchronized pool/GP snapshot.

Record layout accounts for all40 bytes:flags word0,normalized current battle
selector word4,payload-count word8,payload pointerc,update callback10,borrowed
resource/string pointer14 and writable16-byte name18..27. Their broader consumers
and platform ownership remain open. Constructor later overwrites resource/count/
payload/callback and ORs2 into flags;under ordinary source state initial flags
therefore become3. Other flags are not inherited from the old record because
acquisition assigned whole word1 rather than ORing it.

Return chosen E. This helper performs no heap request,payload free,old callback
invocation,resource fetch,render registration,instance-name duplicate check,
cache flush,event scheduling or save operation. It is a slot acquisition and
partial initialization contract. Native ownership must distinguish record
storage from the separately allocated payload.

## mapGetWork uses its own independent base/index globals

At8002b438 read WORD index from8041e664 (r13+1744). Next read WORD array base
from8041e660 (r13+1740). Compute low32(index*0x2f4) and return wrapping
base+that offset. No GP14/context selector read,base/count/index validation,
negative-index check,byte clear or source pointer update. The effect record's
normalized GP14 field does not choose this map work by itself.

Map work stride2f4 is756 bytes. The helper supplies a pointer;it does not tell
this caller the semantic role or valid extent of every field. Majinai constructor
calls it AFTER all payload stores,then reads source WORDffffffff at80428b10
and requests stw at returned pointer+1e (8025ca3c). This writes FOUR bytes at
relative offsets1e..21. It is not a halfword store,a word+20 store or a field
in the effect payload/record. The actual map base alignment,field consumers,
platform alignment/exception semantics and raw alias behavior require separate
evidence;the non-multiple-of4 offset must not be silently rounded or rewritten.

Original-address adapters should preserve this exact byte-address/width request.
A host packed map view can model the four affected bytes without turning the
source access into undefined C++ pointer dereference. Such adaptation is native
platform policy;it does not prove original hardware exception outcomes.
[Packed address principles](../event/EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md)
explain the required distinction between original32-bit words and host pointers.

## Implementation checks and remaining ownership

Fixtures should cover exact-zero versus all other pool flag words,first/multiple
free records,ascending stride40,last-slot success,exhaustion one-past request,
nonpositive count/base behavior,captured count/array versus later globals,
flags1 before GP read,GP14=0/negative/positive normalization,all partial stores
and15 preserved name-tail bytes. Constructor fixtures should observe its later
resource/count/payload/flag writes independently of these acquisition stores.

Map fixtures should cover index-before-base capture,wrapped signed/out-of-domain
index words,stride756,distinct GP14 versus map index,and exact four-byte+1e
write after payload. Native invalid-domain handling must be explicit;original
local functions do not supply safe defaults or rollback.

Pool allocation/capacity/reset writers,current map base/index ownership,update/
draw traversal/deletion/free,heap3 payload lifetime,resource binding and original
frame/exception traces remain open. This delivers local acquisition/publication,
not the complete effect system or full-game memory/rendering requirements.
