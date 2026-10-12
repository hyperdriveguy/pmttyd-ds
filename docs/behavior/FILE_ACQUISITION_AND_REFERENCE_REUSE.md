# Complete shared file acquisition and reference reuse

US G8ME01 revision0; documentation agent ID2,2026-10-12. Complete
_fileAlloc8018a2a4/354. Verify with
`python3 tools/verify_file_acquisition.py`. Read
[formatted callers](FILE_FORMATTED_REQUEST_FORWARDING.md),
[release](FILE_HANDLE_RELEASE_AND_EFFECT_CACHE_OWNERSHIP.md) and
[archive/DVD requests](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md).
Static reconstruction only;original I/O,scene and cache execution unmeasured.

## Entry and manager bytes used by this body

Arguments:filename P in r3,type T in r4. Retain both;incoming r5 is unused.
The manager pointer is the live word at afp80418498,initialized to fileWork8041ec68;
r13 initialization8041cf20 is pinned separately. Do not bake a manager snapshot
into operations that the source reloads after a call or publication.

| Object | Offset / width | Local use |
| --- | --- | --- |
| Manager M | 04 / word | Signed archive-mode dispatch value |
| M | 08,0c / words | Used-chain head,tail |
| M | 10,14 / words | Available-chain head,tail |
| Entry E | 00 / byte | State,nonzero lookup admission |
| E | 01 / byte | Low8 requested type on a new acquisition |
| E | 02 / signed halfword | Reference count |
| E | 04 / word | Embedded payload pointer for archive-backed entries |
| E | 20 / NUL string | Stored filename;strcpy has no local bound |
| E | a0 / word | Payload-pointer holder;archive E+4 or smartAlloc header |
| E | a4 / word | Next entry in the selected manager chain |
| E | ac / word | Pending operation gate tested before cached reuse |

These are observed accesses,not a complete entry layout or generation protocol.
No cache-hit test compares requested type,archive mode,scene,context or data size.
The holder role of+a0 follows new-entry writes here;fileFree only tests that
holder address for zero and never dereferences or frees it.

## First matching nonzero-state filename wins

Capture current manager's used head M08. Walk E→E.a4 until null. At each node,
read state byte;zero skips strcmp entirely. For nonzero state call
strcmp(E+20,P). Nonzero comparison advances via the current node's live next
word. Exact0 enters the pending wait without inspecting+a0 or requested T.
No additional filename/type validation,duplicate resolution or manager recapture
occurs inside the scan.

Read E.ac. While nonzero,call OSYieldThread(),then reread THE SAME retained
entry's ac word. No yield occurs when the initial word is0. No timeout,local
cancellation/failure code,event-wait credit or state recheck occurs in this loop.
It is a thread service request,not a promise that one simulation tick elapsed.

After pending becomes0,read CURRENT state byte. Exact3 stores state1,then
count0. Exact2 stores state1 and preserves count. All other states are preserved,
including unknown nonzero bytes or state0 if changed during the wait. Then read
S16(E2),add1,storelow16 and return E. There is NO count reread,clamp,a0 guard,
filename recopy,requested-type update or used-chain reposition on this path.

Examples:state3/countN→state1/count1;state2/count0→state1/count1;
state2/count-1→state1/count0;state1/count32767→state1/count-32768.
Use word arithmetic then truncation to16 bits. These examples are admitted raw
states;normal producers and valid-count domains require separate evidence.

## Cache miss selects an available entry before loading

Reload current manager and capture E=M10. If E is null,call _fileGarbage(1),
reload manager and E=M10. If still null,call _fileGarbage(0),reload manager and
E=M10 again. Then reload manager,capture NEXT=E.a4 and read that manager's
archive mode M04. NO final null guard precedes E.a4. A safe host adapter may
report invalid provisioning explicitly;it must not claim an original graceful
allocation-failure return at this point.

Archive/DVD acquisition and ordered entry initialization are detailed in the
[request contract](FILE_ARCHIVE_AND_DVD_ACQUISITION_REQUESTS.md). E and NEXT
remain captured across archive/garbage-address/DVD/allocator/string calls.
Backend failure returns0 before manager-list publication;the available head is
not popped by this local path. Backend mutations remain possible and require
live address coherence rather than a transactional model invented by the port.

## Successful publication uses repeatedly reloaded managers

After initializing E,reload manager and store M10=retained NEXT. Reload manager;
if current M10==0,store that SAME manager's M14=0. Reload manager again and test
M08. If zero,store its M08=E,then reload manager and store M0c=E. If nonzero,
read its M0c as TAIL,store TAIL.a4=E,then reload manager and store M0c=E.
Finally return retained E.

Thus head/tail stores may land in different managers if providers swap afp at
observable boundaries. The source does not validate TAIL for null,remove E from
another chain,lock lists,check duplicates or undo data/entry writes on failure.
E.a4 was cleared before this publication;it is not a copied stale link.

## Implementation requirements and acceptance

Implement shared entry identity and raw state/count/holder/name/pending/list
bytes. Preserve first-match order,type-independent reuse,thread-yield polling,
state3 count reset versus state2 count preservation,signed16 wrap and precise
manager reload boundaries. The effect driver's allocation result is E,not the
payload itself;texture consumers must resolve its holder through their own
recovered contract. Do not use a unique texture pointer as a reference handle.

Fixtures should cover zero-state skip,duplicate names,different requested types,
pending0 and multiple yields with mutations,state3/state2/unknown-state reuse,
count extremes,each garbage retry outcome,final-null dereference boundary,
archive/DVD failures without local list consumption,and manager swaps at every
publication reload. Test retained E/NEXT versus live heads/tails separately.

Underlying _fileGarbage/async producer/smart allocator/relocation/TPL/scene
ownership,source names/type domains,thread scheduler/platform failures and
original traces remain open under EVT-01/JUMP-01/DMG-02. This body alone cannot
prove cache eviction,complete resource loading or full-game memory fidelity.
