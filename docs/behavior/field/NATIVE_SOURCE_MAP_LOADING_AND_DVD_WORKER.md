# Native source map loading and DVD worker ownership

This is a static US G8ME01 revision0 translation and a native platform adapter.
It is not an original execution trace or a claim that Game uses this loader.

## Complete retained source bodies

The map graph now contains 33 complete callable bodies, 4051 original instructions:
initialization, lookup, object construction/recalculation, mapLoad/_mapLoad/
mapUnLoad, tail allocation/free, archive open/membership/delete, TPL unpack/TEXGet,
joint name/path searches, material texture binding, bounds, display-list parent,
mapGetWork, DVD manager open/read/length/close/Main, comparator and qqsort/fsort.
The verifier compares every original word/span hash against the read-only DOL.
The generated graph is compiled C++ control flow, with no runtime instruction
fetch/decoder. This word audit does not independently prove translation semantics.
Halfword reads, update/indexed stores, raw binary32 stores, carry operations,
unsigned comparisons and dynamic comparator calls retain reached primitives.

mapLoad waits for real draw synchronization, clears group0, runs the ENTIRE
_mapLoad tail, then publishes loaded count1. Construction alone never publishes
that count. The full tail includes collision, lights, fog/camera, texture writes,
bounds, animation rows and real display-list production. Missing providers retain
that exact call. Never acknowledge missing children or prepublish a room to make
events proceed. mapUnLoad owns its complete source order, including source free,
hit/light/environment/camera reset, archive membership and descriptor1 deletion.
Freeze event/renderer/collision users over the entire held interval; the outer
sequence's later NPC/EVT reset does not authorize access to freed room backing.

## Native file process

FieldMapFileOwner owns actual 256x98-byte manager slots and the shared original
sort scratch/globals in the same VM as the parent. It owns each native FILE by
its actual source DVDFileInfo address within that table. Physical mapping identity
is rechecked; byte-identical replacement backing does not qualify. Table, sort
scratch and private worker stack survive map unload. Do not share sort storage
with another retained source sort or destroy the process while any user/worker
is pending. Stop/quarantine all users before teardown. Destructor cancellation is
native owner teardown, not source mapUnLoad or DVDMgrDelete semantics.

The original archive descriptor table is 803dbff0, four 28-hex-byte rows.
Preserve signed low-half address formation (803e0000-4010), rather than
misreading it as 803ebff0. Archive storage belongs to its actual process owner;
the source-file fixture supplies a cold empty table. Mounted archives require
real ARC children. Native memcpy admission requires physical and virtual
disjointness; arbitrary alias/semantic-provider storage is outside this adapter.

The explicit native cold bootstrap allocates/zeros this process storage, installs
the table pointer and enables cooperative pumping. It replaces the GameCube OS
thread bootstrap; it does not claim to execute DVDMgrInit/OSCreateThread. Each
pump resumes the complete original DVDMgrMain using its own stationary cursor
and stack. A parent's source OSYieldThread is acknowledged only after that whole
iteration returns. Budgets/holds do not advance simulation. Call native IO and
pumping outside simulation ticks. No background libctru worker/Game integration
is supplied by this milestone; synchronous RomFS IO must not run on a play tick.

Native low-level path/open/read/close replace physical DVD services for exactly
./m/gor_00/{d,t} and their original ././ prefixes. Logical FST lengths remain
1039618/525792. Both complete staged files are admitted before source's unchecked
D path. The D transfer retains 30 exact bytes after logical EOF from the ISO; no
zero-padding is invented. Native transfer writes all requested bytes before
acknowledging the physical read. Real source Main then subtracts requested chunk,
advances destination/transferred and publishes completion in original order.
Nonnegative SDK counts are not substituted for requested chunk accounting.

Source Close ORs08; it does not erase/free a handle. Main attempts pending reads
before close, invokes physical close, then clears98 bytes. New Open cannot reuse
that slot until the worker does so. Mode sorting uses low8 of mode; chunk limiting
uses the full16-bit field, at40000 hex. Cancellation(-3) completes without byte
counters, other negative returns set20/80 while leaving the request pending.
Original source sorting preserves fresh comparator reads, equal-key behavior and
shared scratch across invocations. Arbitrary DVD command callbacks, other files,
mounted archive SDK services and platform interrupt/disc queues remain outside
the native file adapter. Missing callbacks/services hold; they are not ignored.

## Original files and validation scope

The editable [file provenance](../../../assets/original/maps/harbor-files/provenance.json)
records unrelocated D/T, rounded transfer tails, original offsets/hashes and DOL
literal/jump-table spans. New imports require an empty destination. Normal asset
recipes copy those files and compile loader literals after the ISO-backed pack.
The earlier harbor-source payload is already relocated for constructor fixtures;
never feed it to this loader for a second relocation.

The original-file fixture uses the real native file owner, actual source parent
and worker, real map pool tail/front allocations, actual TPL unpack, all 15974
relocation stores, original scene search and the real shared SDK constructor for
all 540 objects. It stops at the genuine hitEntry child, with count0 and the actual
collision A joint. GPU/cache draw-sync receipts and limited libc formatting in
that fixture are explicitly test-only; production owner supplies no success stub.
It does not prove real collision, renderer readiness, full loader completion or
playable room transition. Portable fixtures separately test complete source sort
through 256 elements/duplicate keys, every reached TPL primitive retry, requested
chunk accounting, cancellation/error callbacks, mode cap and close retirement.

Next: real hitEntry/_hitEnt/triangle production and distance math; actual light
owner bridge, camera/material/mapDraw/GX capture; native asynchronous scheduling,
Game composition, event epochs/unload users, original traces and New2DS XL checks.
The full prologue goal remains active.
