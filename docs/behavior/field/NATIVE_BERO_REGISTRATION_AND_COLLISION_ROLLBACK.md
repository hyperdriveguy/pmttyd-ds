# Native Bero registration, indexed queries and collision rollback

Builds on [selected-entry binding](NATIVE_BERO_TABLES_AND_INCOMING_QUERY.md).
The [complete source query/state contract](../overworld/BERO_INDEXED_INFO_LENGTH_AND_REGISTRATION_STATE.md)
records exact addresses, writer order and the distinction between BeroSW and
BeroID. This native owner binds functional values and admitted static geometry;
it is not full raw EVT callback memory/store alias execution.

## Actual valid-name get_info prefix

Original _hitEnt clears each constructed HitObj's U16 flags. FieldMapFlags now
retains that halfword separately from map-object flags. Named lookup ignores
runtime1 and excludes80; active ray filtering excludes either1 or80. Its lookup
returns the actual imported collision source-node identity. No disabled1 node
is silently treated as a missing name. Source geometry, hierarchy and member
identity remain the existing validated original map import.

On an actual successful native road entry, FieldBeroInfo validates every
original table name against the destination's live named owner, publishes a
cleared16-slot INFOARR view and the actual used pointers/count, sets only used
BeroSW slots to-1, and performs the actual BeroEXEC reset. Native initialization
runs once per area/resource ownership; later EXEC writers survive subsequent
visits to the completion check. Unused switch state stays unknown in this native
owner, not a fabricated used entry. BeroID/case allocation remains separate.

Missing/flag80/ambiguous names hold native admission before these publications.
The original get_info instead records a mapErrorEntry diagnostic and continues;
that error-ring domain is not acknowledged as complete here. This is explicit
native admission, not an original missing-name early return. Only actual valid
names complete the bound reset. A plain checkpoint restore does not run it.

## Indexed entry, length and current-number values

infoByIndex writes original LW3..14: name,U16 type,direction,position pointer,
entry/default script,ffff0000,BeroSW,authored/default case type,exit/default
script,ffff0000,destination map/name pointers. Other words and local flags are
preserved. Signed negative/out-of-count indices hold as a native capacity
boundary; original callbacks perform unchecked wrapped indexing. No case handle
is invented and no native road enable flag is substituted for BeroSW.

The original20-byte default exit EVT is now retained beside the entry default;
PMBRIO02 includes its actual address. The length query returns signed authored
length converted tofloat, or original kind-specific30/60 defaults for-1. Its
value API returns the original float before evtSetFloat routing; arbitrary EVT
float serialization/alias services remain separate. Existing original cardinal
road lengths are exactly30 and require no rounding substitution.

The native cardinal bridge keeps fixed radius30 for its initial point. Its final
offset now uses the original entry length: start=C+30*outward, end=C-length*outward.
This matches the recovered cardinal algebra while retaining the existing native
center/floor/movement bridge. Full SDK trig/FMA, original Bero globals and encoded
initial_pos LW10..12 are not certified by this adaptation. An isolated test copy
with length45 checks data flow; it is not an authored original-game constant.

Current-number startup0 is original BSS. get_info does not reset it. The actual
native query now feeds the literal set_now_number(LW1) prefix. All32 signed bits
are retained without bounds normalization. Area/resource changes alone preserve
this global semantic value; external checkpoint restore leaves its history
unknown until an actual writer. It is not serialized into a fabricated entry.

## Geometry and filter ownership

CollisionTransition previously swapped geometry/indexes/road attributes but
left the named disabled-hit overlay in the process-global query cache. A rejected
candidate could therefore leak its hit suppression into the restored room; fresh
staging could inherit old source-node masks. Node offsets are local to each map
and can be reused, so their numeric identity alone does not establish lifetime.

The snapshot now swaps all disabled-door/event nodes, their cached room/mask and
the fence filter alongside geometry and road attributes. Fresh import clears all
those filters. Rollback restores the complete old query view immediately, without
requiring a later Game tick to republish masks. A committed destination keeps its
own view. This is native transaction ownership, not a new original-game rule.

Host regression uses the original elevated house05 collision surface, including
same-ID staging and candidate Game mutation: fresh geometry admits it, candidate
init suppresses it, rollback restores the old enabled/disabled state. Separate
query tests check flag1 versus80, valid registration/reset/nonreplay, actual
indexed outputs, current-number lifetime and native authored-length data flow.
Exact build/capture evidence is in [progress](../../PROGRESS.md).

Remaining: map-error diagnostic ring, full original get_info alias ordering,
case allocation/BeroID and actual switch callers, original Mario/party position
callbacks/encoded high locals, and house05 cases/open/close/material/key events.
