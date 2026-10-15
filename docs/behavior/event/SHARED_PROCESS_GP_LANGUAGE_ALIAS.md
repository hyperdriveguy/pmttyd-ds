# Shared process GP language alias

`EventGpLanguageAlias` redirects only four destination bytes at GP+`16c..16f`
to the actual registered startup-process GP word. It initializes no language,
copies no GP and never selects a destination GP or alters its source pointer.
The actual startup GP, process ownership and current proof are mandatory.

## Original evidence and native boundary

The original US producer at `8006fd0c..8006fd18` is pinned by
`source_ui_startup_assets.py`: `806d90b038800001380000009083016c` loads actual
GP, loads1 into r4, loads0 into r0 and stores r4 at GP+16c. The existing
[startup prefix](../display/NATIVE_UI_STARTUP_FONT_PREFIX.md) owns that real
one-time language contribution. The alias performs none of it. Font lead,
width and draw helpers read this same actual word; no default US language or
atlas-derived language is supplied. The helper tests execute source storage
writes1 then2 and confirm bidirectional shared reads/writes, without claiming
whole startup production execution.

This alias is native cross-VM ownership policy, not another original PPC
routine. Each registered GP gets a native unique shared registration identity;
old live tokens cannot revive after unregister/register at the same address.
This identity writes no guest byte. Source must remain the selected actual GP,
registered under the captured token, unaliased and current. A source switch,
registration replacement, missing process proof or retirement latches stale.
It requires a fresh explicit owner rather than retargeting or restoring old
private bytes.

## Ownership and topology

Construct one shared helper with the actual source `EventGpAddresses`, captured
source current base, a strong independent source-process/proof lifetime, and a
pure current closure. Source object address must remain stable through this
lifetime. The proof must freeze source ownership/maps through one synchronous
operation and must not mutate GP or resources. `bindLanguageAlias` attaches it
to one actual destination registration. No raw source pointer is used without
its pinned process holder. Retire views before source VM teardown; stale views
remain owned/unavailable until genuine destination GP retirement.

Source and destination must be different `EventGpAddresses` objects. An already
aliased source is rejected; source-current validation checks this mechanically
before any delegated access. This root/star topology rejects self, chains and
precreated cycles without recursive GP reads. If a source is aliased later,
existing views become stale. A helper can be shared by multiple destination
VMs while the actual process remains owned. Use an independent source lifetime
and weak current-context captures; do not create destination→helper→destination
ownership cycles.

## Widths, atomicity and first refusal

`EventGpAddresses` has byte and word entry points; facade halfwords originally
used repeated byte operations. The journal makes all language-overlapping byte,
halfword and word accesses one alias span operation. It snapshots the actual
source word once, preserving big-endian lanes. Writes preflight every adjacent
GP byte, acquire actual source admission once, update the source word once,
then commit only requested neighboring destination bytes without callbacks or
failure. No partial prefix is written on stale admission. Render-dimension
neighbors retain their real binding; timestamps, input, refresh/context and all
other packed GP bytes retain their existing semantics.

Destination registration/binding mutations and GP API reentry are blocked
during the transaction/proof callback. Source current/registration is rechecked
after the proof and before direct source-word access. No callback follows a
successful mutation admission. This is synchronous native admission, not a
rollback transaction against arbitrary concurrent/direct C++ storage writes;
callers preserve semantic registries during existing raw-RAM borrows as before.

The language range remains owned even when unavailable. The EventAddressSpace
journal routes half/span operations to the whole alias, refuses RAM fallback
for stale owned bytes, and rejects new RAM/external mappings over the alias.
An earlier overlapping external mapping cannot supply private zeros after
source loss. Raw-RAM acceleration already excludes GP semantic intervals.
Output read arguments remain unchanged on failure. A GP unregister snapshot
now rejects unavailable alias bytes transactionally instead of exporting
unwritten snapshot elements; unregister without a snapshot still retires the
actual destination normally.

## Binding and validation

Normal integration takes the real source from
`startup->fieldProcess()->process().vm().gpAddresses`, captures its actual
`currentAddress()`, and pins that actual process lifetime. Bind the destination
only after its own actual GP registration. Native total actor/font adapters
also require `alias.current()` in their parent admission before total reads;
optional VM byte/half/word reads already return unavailable on stale ownership.
Do not turn a failed read into0 or bind language1 as a constant.

The unapplied journal changes only EventGp header/source and EventAddressSpace
routing; new implementation/test/contract files are separately registered.
Focused strict host, ASan/UBSan and GNU ARM syntax use one consistent copied
accepted6458 include graph with only the journal overlay. Both original GP and
AddressSpace regression tests are rerun with the actual changed implementations.
Primary read-only ISO producer evidence comes from the pinned accepted6487
exporter; no pack or editable asset is written.

Tests cover source1→2 and destination writes, all byte/half/word boundary
positions, neighboring dimensions/packed bytes, input/timestamps/root pointers,
one proof per operation, atomic stale failures, output preservation, external
fallback rejection, source End/Init ABA and GP switches, missing ownership,
strong process retention, source disappearance during proof, self/chains/cycles,
reentry and self-retirement. This helper supplies no menu readiness, source
callback/queue, source startup scheduling or native GPU acknowledgment.
