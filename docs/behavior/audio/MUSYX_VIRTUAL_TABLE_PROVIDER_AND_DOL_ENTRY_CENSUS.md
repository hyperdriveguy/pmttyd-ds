# MusyX virtual table providers and scoped DOL entry census

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). This consolidates the already-complete
[initializer/updater](MUSYX_VIRTUAL_SAMPLE_INIT_UPDATES_AND_POSITION.md),
[start/end notifications](MUSYX_HW_MESSAGE_AND_SAMPLE_NOTIFICATION_ORDER.md)
and [refill](MUSYX_VIRTUAL_SAMPLE_REFILL_DESCRIPTOR_AND_CURSOR.md) with a
scoped original-DOL address-materialization and direct-entry-call census.
Addresses/offsets hexadecimal; counts decimal. Static evidence, not original
execution or proof of absence of every possible provider.

## Known configuration writers and actual unresolved inputs

Virtual table T=80406b00. Across these five complete local bodies:

| Configuration field | Complete local writers | Local consumer boundary |
| --- | --- | --- |
| T.byte0 configured count | vsInit stores0 | Start reclaim/candidate loops read it; first-free uses retained terminal count |
| T.word4 loop size | None | Start loop-buffer request; refill lengths/remainder; updater wrap delta |
| T.halfb48 generation counter | vsInit stores0; start increments low16 per candidate attempt | Start selection/encoding and end/updater comparisons |
| T.wordb4c callback | vsInit stores0 | Start/end/refill invoke it; updater null-gates its scan |

The known local graph supplies no positive configured count, loop-size setter
or nonzero callback installation. This is a statement about the five complete
bodies, not a guarantee that the entire game never supplies them. The table is
BSS: a fresh zero-filled process allocation is distinct from reinitialization,
where vsInit leaves loop size and slot payloads untouched. Do not promote a
loader/BSS expectation into observed original execution or a reinit reset.

Mapping/state/payload writers are already exact in the linked contracts:
initializer maps64 voices to255; start reclaims matching voices/reserves a slot/
publishes map/initializes payload; end and updater clear slot/map; refill stores
its descriptor and advances cursor only after nonzero callback result. These
local writers do not explain the callback's actual byte-transfer work, external
slot-state/remaining initialization or installation policy.

Conditional consequence only: if the known initializer runs and NO additional
provider changes count/callback, start encounters count0, publishes map[v]=255,
clears that voice's two hardware loop words and returnsffffffff; updater sees
callback0 and returns. This does not prove the original game exercised that
state, that every sample uses this subsystem, or that those conditions persist.

## Exact bounded literal-address predicate

The census scans every aligned word in all nonempty original DOL text slots
0..6. For each opcode15 with RA=0 and immediate8040hex, retain its destination
register R and inspect at most the next32 instructions. A candidate requires:

- Base-register field RA==R.
- Opcode in14,32,34,36,38,40,42,44,48,50,52,54 (decimal).
- Immediate in6b00..764fhex, the positive displacement interval for this table.

Stop the lookahead after an instruction whose RT==R and opcode is one of
14,15,32,34,40,42,48,50. A qualifying instruction is recorded before that stop.
Assign owners using original DOL text symbol address/size ranges. This is a
syntactic candidate search, not control-flow or complete pointer propagation.
It intentionally records candidate origins for inspection, not automatic
proof that every memory operation belongs to T.

All28 candidates are table-base `addi` constructions at80406b00, with this
owner distribution:

| Complete local owner | Candidate constructions |
| --- | --- |
| vsInit | 2 |
| vsSampleStartNotify | 12 |
| vsSampleEndNotify | 2 |
| vsUpdateBuffer | 8 |
| vsSampleUpdates | 4 |

No additional DOL owner matches this exact predicate. This does not cover
alternate constant construction, longer register lifetimes, pointer loads/
propagation, callbacks, small-data aliases, block copies, relocations, REL code,
computed/indirect stores or writes through externally supplied pointers.
Consequently it does NOT close the missing configuration/callback provider.

## Exact DOL direct-entry-call census

Independently decode aligned opcode18 instructions with LK=1 throughout the
same DOL text. Sign-extend the relative displacement (or use absolute target
when AA=1), wrap32 the target and match EXACT entry addresses of the five local
bodies. Seven sites match:

| DOL callsite | Owner | Exact target |
| --- | --- | --- |
| 80279790 | synthHWMessageHandler | vsSampleEndNotify |
| 802797c0 | synthHWMessageHandler | vsSampleStartNotify |
| 802797d4 | synthHWMessageHandler | vsSampleEndNotify |
| 802873ac | vsSampleUpdates | vsUpdateBuffer |
| 802873d8 | vsSampleUpdates | vsUpdateBuffer |
| 8028dbcc | sndInit | vsInit |
| 802906e8 | snd_handle_irq | vsSampleUpdates |

This pins the known bootstrap/message/frame graph without inventing a public
configuration API. No claim about calls through pointers, tail branches with
LK=0, middle-of-body targets, generated code, REL imports or caller reachability.
The existing frame contract owns audio critical-section request order; this
census establishes callsites, not physical interrupt cadence.

## Validation and remaining work

[Manifest](../../../tools/musyx_virtual_table_provider_census_evidence.json)
records28 candidate pairs, seven direct calls/owners and three initializer
configuration stores. It pins all nonempty DOL text (two segment fingerprints),
five previously-complete bodies and three caller fingerprints:10 spans,
66 instruction anchors,seven resolved calls. No new complete-body claim is
made. The read-only generator/scanner stays `/tmp/scan_vs_materializations.py`
and `/tmp/make_vs_provider_census.py`; no original body or asset exported.
Generic manifest verification checks bytes/hashes/call targets, not independent
replay/completeness of the bounded scan; its exact predicate is specified above
and was run separately against the read-only ISO.

Remaining: wider indirect/REL/pointer/alias writer and callback installation
search, actual callback implementation/transfer/result contracts, nonzero count/
loop-size/slot-state/remaining providers and valid bounds/reinit/concurrency
invariants, original configuration/refill/playback traces. The scoped literal/
direct-call census is answered; global provider closure and EVT-01 remain OPEN.
