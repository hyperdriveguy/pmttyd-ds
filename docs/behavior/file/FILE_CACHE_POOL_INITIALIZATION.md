# Complete file-entry pool initialization and archive-mode setter

US G8ME01 revision0; ID2,2026-10-12. Complete fileInit8018b6c0/f0 and
fileSetCurrentArchiveType80189d38/c. Verify:
`python3 tools/verify_file_collection.py`;read
[collection](FILE_CACHE_COLLECTION_AND_PRELOAD_AGING.md),
[entry acquisition/layout accesses](FILE_ACQUISITION_AND_REFERENCE_REUSE.md)
and [async publication](FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md).
No original initialization/runtime/memory-capacity trace is claimed.

## Allocation,manager publication and full raw clear

Request __memAlloc(0,0x2c000):heap index0,180224 bytes. No old-pool free,existing
handle release,null allocation check or size/count query occurs. Reload current
manager afp80418498 and store returned base at M00. Reload manager,store archive
mode M04=0;reload manager,store used head08=0;reload manager,store used tail0c=0.

Reload manager/read its current M00 as destination;call
memset(destination,0,0x2c000). This destination can differ from the allocation
return under observable manager/base aliases. No manager-wide memset occurs;
available head/tail have not been published yet. Full pool bytes are cleared,
including name,holder,pending,callback,count and unclassified entry bytes.

## Exactly1024 linked176-byte entries,then live tail termination

After memset,reload manager/read current base M00 into retained cursor B.
Set loop count128. Each iteration performs eight next-word stores in ascending
entry order,then advances cursor by0x580 (eight*0xb0). For j=0..7:
store WORD(cursor+j*0xb0+0xa4)=cursor+(j+1)*0xb0,with32-bit address arithmetic.

Across the loop,1024 entries of176 bytes cover the allocated0x2c000 block.
The final loop store initially points the last entry to ONE PAST the block;
there is no conditional special last iteration. No explicit state/count stores
occur in the loop;their zeros came from the prior memset.

After the loop,reload manager/read its CURRENT base M00;store that same
manager's available head10=base. Reload manager/read current base again;
compute base+0x2bf50 (1023*0xb0);store that manager's available tail14.
Reload manager/read its CURRENT tail14;store WORD(tail+0xa4)=0. This final
termination can target a different base/tail from the cursor used by the loop
if a provider changes manager memory. No null guard protects the final store.

The original capacity is therefore1024 entries under ordinary stable provisioning.
Do not invent a manager count field;this function does not publish one. A native
pool may adapt its storage/address provider,while preserving supported source
capacity,entry stride/raw fields and exact service/memory operation semantics.

## Setter publishes the full archive-mode word without validation

fileSetCurrentArchiveType(T) reloads current manager and stores full word T to
M04. No range check,clamp,list mutation,archive mount,thread yield or old value
return is supplied. The later allocator reads this as a signed dispatch value:
0 one archive probe,1/2 up to three identical probes,all other values DVD path.

Initialization changes mode to0;reinitialization requests a new heap allocation
and clears/publishes new lists without locally releasing old pool/data/pending
handles. Full startup/reinit/scene caller ownership remains a separate graph.
Do not infer leak-free initialization or add a hidden generic shutdown callback
without recovering those owners. Original archive descriptors are provisioned
elsewhere;this pool initializer does not load or mount their contents.

## Implementation acceptance and resource requirements

Check heap0/exact byte size,no allocation guard,all manager reloads,memset's
live base,128 ordered eight-store groups,word wrap,temporary one-past last link,
separately reread head/tail bases and final live tail termination. After stable
initialization,all1024 states/counts/holders/names/callbacks/pending bytes are0;
next links form the complete available chain and used head/tail are0. Preserve
unknown raw bytes rather than asserting a complete interpreted entry schema.

Accept arbitrary setter word values and prove subsequent signed dispatch keeps
the recovered numeric policy. Test manager/base/tail swaps independently;
repeated init must request a fresh allocation with no local old-state cleanup.

The pool's180224-byte allocation is only one resource requirement,not total
RAM needed for the game or an observed hardware budget. Payload smart heap,
DVD manager/worker,archive mounts,relocation/TPL/assets,other pools/platform/
scene lifetimes and original workload/performance remain open. Reuse supplied
original asset data outside source history;this initializer converts none.

## Scoped startup and archive-mode callers

The [17-site DOL census](../../../tools/file_collection_call_sites.json) identifies
one fileInit direct call,marioStInit8006fdac. Ten setter calls occur in
animGroupBaseAsync(3),animEffectAsync(3),animPoseEntry(3),animInit(1). These are
address/caller bindings,not complete animation initialization/resource graphs or
an inferred global setter frequency. Six additional sites call collection.
No REL/indirect/raw-address or original execution census is claimed.
