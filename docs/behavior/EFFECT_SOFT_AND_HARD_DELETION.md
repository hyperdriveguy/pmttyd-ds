# Complete effect soft/hard deletion and raw Maya payload release

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete effSoftDelete8005e9c0/70,effDelete8005ea30/4c and
 effDeleteMayaAnim8005e468/28. Verify:`python3 tools/verify_effect_deletion.py`:
six complete/linked body identities,57 instruction anchors,SDA span and271
scoped DOL direct sites. Heap/fog identities are linked;original update/render/
scene execution is not measured. Read
[caller/lifetime implications](EFFECT_DELETE_CALLERS_AND_LIFETIME.md),
[pool acquisition](EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md) and
[SDK payload free](SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md).

## Hard deletion8005ea30:payload free before flag clear

Input is effect RECORD pointer E,not its payload pointer. Retain E. Null E
returns immediately WITHOUT any record/global/heap request. Otherwise read
WORD E.flags0. If exact0,return without reading payload or clearing other fields.
There is no requirement flag bit0 be set;any nonzero word is admitted.

For admitted E,read payload D=word(E+c),call __memFree(heapIndex3,D),then
store WORD E.flags0=0. Those are ordered operations;flags remain nonzero until
AFTER the free request returns. No payload-null guard,context/ownership check,
count/resource validation,callback dispatch or caller-specific branch. Heap
index3 is hardcoded;E.context4 does not choose a different free family.

No other effect record bytes are cleared. E.c retains the old/stale payload
address,E8 count,E10 update callback,E14 resource and E18..27 instance name
remain unchanged. The record itself is not freed or memset40. No payload/record
copy-back,event deletion,audio stop,message cancellation or map-word reset is
requested locally. Scalar return is not a success boolean contract.

An active record with D0 still calls __memFree(3,0). The SDK wrapper/free family
has no map-style null recovery,as the linked allocator contract states. Native
allocation/address validity is an explicit precondition or unsupported-domain
policy;do not invent graceful null-payload deletion as original behavior.

Under unchanged source storage,repeated hard deletion finds flags0 and skips
a second free. This is a flag gate,not a universal double-free detector. External
raw writers or reused-slot lifetimes remain separate;the stale pointer alone
does not make the record active. Preserve captured E across the free call and
clear its flag afterward rather than reselecting a global pool record.

## Soft deletion8005e9c0:mask02 chooses deferred request

Retain E;null returns immediately. Read flags F=word(E0);exact0 returns without
any further mutation/query. For nonzero F,test numeric mask0x02 (PPC
rlwinm0,0,0,30,30 at8005e9e4):

- If `(F & 0x02)!=0`,store E0=`F|0x04` and return. No payload read or heap
  free occurs. The store still happens when0x04 was already set.
- Otherwise perform the immediate payload-release path below.

The first branch preserves EVERY existing flag bit,ORs0x04 and leaves the
record occupied. It does not clear active-bit0,name,payload,count,callback or
resource. It does not invoke the effect update function to finish immediately.
Which update honors0x04 and when it calls hard deletion remain effect-specific
contracts. “Soft delete” is not a fixed fade duration or deferred-free scheduler
owned by this helper.

For the nonzero/no02 branch,the original code repeats null-E and nonzero-F
comparisons with the SAME captured values;it does not reread memory flags.
Read E.c;call __memFree(3,payload);then E0=0. This has the same storage effects
as hard deletion,implemented INLINE rather than calling effDelete. There is no
payload-null guard or cleanup of the remaining record bytes. No scalar contract.

| Initial flags | Soft-delete local behavior | Hard-delete local behavior |
| --- | --- | --- |
| 0 | No read of payload/no writes | Same |
| 1 | Free payload,then flags0 | Same |
| 2 | Write6,no payload read/free | Free payload,then flags0 |
| 3 | Write7,no payload read/free | Free payload,then flags0 |
| 4 | Free payload,then flags0 | Same |
| 6 or7 | Write same flags again,no free | Free payload,then flags0 |
| Any nonzero lacking02 | Free payload,then flags0 | Same |
| Any nonzero containing02 | Preserve word OR04,no free | Free payload,then flags0 |

These are numeric masks,not inferred driver capability/type enums. A record
can be admitted with bit0 absent because the gate is the entire word!=0. The
Majinai constructor ordinarily publishes flags3;its soft deletion therefore
sets7 and leaves allocation intact until an update/driver actually releases it.

## Raw Maya payload release8005e468

Input is RAW pointer P. Move P to second argument;call __memFree(heapIndex3,P).
There is no null or effect-record flags test,no E.c dereference,no flag/name
clear,no callback and no record lifetime update. Its annotation does not make
it interchangeable with hard/soft deletion. Passing a pool-record address to
this helper would request SDK release of THAT address rather than its payload.
Actual Maya/animation ownership and callers require separate source evidence.

## Implementation acceptance and open boundaries

Check null record/exact-zero flags early paths,nonzero words with bit0 absent,
every02/04 combination,repeated OR writes,soft no payload access versus hard
payload load,heap3 exact arguments,flags clear AFTER free and all other bytes
preserved. Verify soft fallback's repeated captured comparisons rather than
fresh flags reads,raw Maya pointer policy,null payload requests and stale name/
pointer/count/callback/resource after hard release. Native exception handling
must be documented as adaptation,not recovered local safety checks.

Effect-specific04 consumption,update/draw/free cadence,pool/context/scene/raw
writer ownership,SDK/platform/alignment domains,Maya resource lifetimes and
original runtime traces remain open under EVT-01/JUMP-01/DMG-02. This closes
these three leaves,not the full effect destruction or rendering system.
