# Native case pool and retained Trouble Center installer

Source baseline: [case allocation and wrapper lifetime](../overworld/BERO_CASE_POOL_IDS_AND_SWITCH_WRAPPER_LIFETIME.md)
and [case9 geometry/lifetime](TROUBLE_CENTER_DOOR_CASE9_GEOMETRY_AND_LIFETIME.md).
This binds the reusable semantic pool and actual isolated Trouble installer;
it does not certify complete field caseMain or original gor_02 incoming locals.

FieldCasePool owns64 contiguous160-byte records, with source offsets through
priority9c checked at compile time. Construction selects the first inactive
record, clears all fields, strips descriptor type8000 into record flag4, sets
active1, retains the source name/argument/callback/script/priority and snapshots
all16 local words. Equal names do not deduplicate. A deleted slot can be reused;
construction then replaces its previous event ID and locals. Case deletion and
reinitialization clear active1 only; they neither kill events nor erase data.
Explicit mode0 event deletion checks and retires the actual live event before
clearing the case; other modes skip event deletion.

The source allocation overruns a full pool and strcpy is unbounded. Native
admission holds for a full pool, embedded-NUL names or names longer than63 bytes,
and invalid indices. These are explicit port capacity policies. The pool is a
semantic C++ owner, not a guest RAM alias of original heap pointers. It therefore
does not certify arbitrary memory alias behavior inside strcpy/memcpy or callback
mutations of the global wp/count/array pointers.

The registration wrapper captures the argument pointer; resolves operands0/1/2
with the value service, applies original evtGetNumber to raw operand3 without
variable dereferencing, then resolves operand4. It snapshots caller locals before
allocation/index output and freshly reads raw operand5 afterward. Nonzero output
uses the setter with the actual first-free index. Bero priority20 differs from
ordinary priority0. Only local words are captured; new events retain root
constructor local flags, user pointers, timers and metadata.

Dispatch is the source separate second pass over active+marked records in pool
order. It checks actual saved event IDs, zeroes dead IDs, creates roots only for
zero IDs with original script/priority/flags20, publishes case index into
field16c, saves actual event ID, copies16 saved locals, and invokes NPC stop only
for priority20. A still-live event suppresses restart; transient mark2 is cleared
on completion in either case. Dispatch does not execute the new event body; the
next manager visit owns that. Missing priority20 stop providers or exhausted
native event capacity hold before construction, preserving the pending mark.
This admission is a native policy; original constructors have no failure return.

TroubleBoardEvent now handles original evt_run_case_evt80070110 internally rather
than asking an external provider to acknowledge it. Running the original retained
Installer script actually allocates board and counter cases, with original names,
script addresses, priority0 and the actual VM caller local snapshot. Explicit
markCase/dispatchCases APIs separate field contact admission from allocation and
later EVT execution. The same retained VM runs those actual installed scripts and
preserves independent descendants and saved state across reentry. Root event IDs
are retained independently of case slots, so deleting or reusing a record cannot
make a still-live event disappear from root liveness bookkeeping. Existing direct
Board/Counter root APIs remain for their current native interaction adapter.

The fixture starts an actual standalone Installer root, so its cleared local bank
is established by that constructor. This does not prove the original full
Rogueport initialization bank was zero: [house05 incoming locals](HOUSE05_INCOMING_LOCALS_AND_INSTALLER_FRONTIER.md)
remain unresolved. The case pool currently belongs to this retained runtime;
sharing one owner across the full field EVT/area/Bero lifecycle is still required.
Game's current contact adapter does not yet install or trigger this pool.

Tests exercise allocation/deletion/reuse/capacity, wrapper read order and fresh
output operand, snapshot-before-output, priority distinction/NPC stop, live-event
suppression, actual IDs, event retirement, held providers and original installer
board/counter event reentry. Native artifact/build evidence belongs in
[progress](../../PROGRESS.md). No new original asset payload is needed.

Next bind complete original field mark-pass gates/contact/name/NPC providers and
shared case/EVT lifetime, then BeroID/switch callers and known incoming banks.
Do not substitute source table indices for allocated case IDs or fabricate
house05 saved locals to bypass these dependencies.
