# MusyX sequence speed, volume and group forwarding

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Four complete local bodies:
seqSpeed80273aa0/e4hex, sndSeqSpeed80279dd4/48hex,
seqVolume80273d48/1cchex, sndSeqVolume80279eac/68hex.
Addresses/offsets/masks hexadecimal; loop indices/counts decimal.
Static source requests/stores, not measured tempo, fade or audible timing.
Read [lookup/stop](MUSYX_SEQUENCE_VALIDITY_PRIVATE_ID_AND_STOP.md),
[pause/mute](MUSYX_SEQUENCE_PAUSE_MUTE_AND_DEFERRED_FIELDS.md) and
[continue's missing-ID domain](MUSYX_SEQUENCE_CONTINUE_AND_MISSING_ID_DOMAIN.md).

## Shared inline lookup and argument positions

Both cores inline activeRoot8041ee5c then pausedRoot8041ee58 traversal.
Compare each node.wordc with input ID low31, following word0 successors.
First match returns node.byte9 with the INPUT bit31 inserted. Total miss
returns ffffffff. No cycle/member/private-index validation or identity recheck.
Q=803ebdc0+wrap32((result&7fffffff)*1888hex) selects the instance.
Input tag chooses the immediate/deferred branch; it is not a tag read from
node.wordc or inferred from the root containing that node.

seqSpeed receives (ID,S) in r3/r4. seqVolume receives (V,T,ID,M) in
r3/r4/r5/r6; it retains original ID in r7 for the first synthVolume request.
These are raw register values, not a declaration of caller types/units.
Both cores omit a sequence state-byte gate. Lookup match alone does not prove
that the byte9 index is in0..7; that remains a provider invariant.

## Complete speed stores and missing-ID consequence

seqSpeed has NO result==-1 guard. After lookup it tests bit31 directly.

For untagged match, ascending16 stores write S.low16 into Q halfwords:

| Track ordinal | Offset formula | Endpoints |
| --- | --- | --- |
| 0..15 | 153a + ordinal*38hex | 153a..1882 |

The stores are unrolled; no track-active, state, speed-zero/range or overflow
check and no child call. Other track bytes remain as they were. This is a
halfword field write, not proof of an immediately audible tempo change.

For tagged result, read Q.byteefa, OR20hex, store byteefa, THEN store
S.low16 at Q.halfef8. No state gate, scheduling request or flag normalization.
Existing other flag bits survive. The eventual deferred/track consumer is open.

Miss ffffffff also takes the tagged branch. As in continue,
wrap32(7fffffff*1888)=ffffe778, giving Q=803ea538. Speed therefore reads/
writes byte803eb432 with OR20, then writes halfword803eb430 with S.low16.
Both precede seqInstance803ebdc0. This is a derived address/write order,
not evidence of original execution, resident contents or a corruption trace.
A faithful valid-domain binding needs caller lookup-success guarantees;
checked refusal would be an explicit adapter policy, not this source branch.
The core does not assign an independent status return.

## Complete immediate volume/group request order

seqVolume explicitly recognizes lookup resultffffffff and returns without
synthVolume or instance-field stores. Other results test tagbit31.
For untagged match, calculate Q, load Q.byteed0 and request:

`synthVolume(V,T,U8(Q.byteed0),M,originalID)` at8027917c.

All five argument positions matter: M is raw, originalID is retained; no
parent normalization/clamping/low-nibble reduction in this branch. After this
request, derive the same Q from retained index and scan EXACTLY64 successive
bytes Q+324..363, ascending. For EACH entry i, read its group byte, then freshly
read Q.byteed0. If equal, skip; otherwise request:

`synthVolume(V,T,U8(Q.byte324+i),0,ffffffff)`.

The loop does not cache the primary group, deduplicate secondary groups,
excludeff, check group bounds, or read a track-active/count field. Child requests
can mutate later group entries or the primary byte before their fresh reads.
Retained Q/index/V/T and ordinal persist; lookup is not repeated. The initial
primary request always precedes all64 reads, even when all secondary groups
match. For stable example bytes, all-primary gives0 secondary requests;
alternating primary/other gives32; repeated same other group gives64 calls,
not one. These are source-derived order examples, not recorded execution.

The parent itself does not store an immediate sequence volume field, issue a
sequence-state transition, stop/pause a voice, or claim a fade duration.
synthVolume is fingerprinted here as a linked child; its internal fader/group/
mode behavior remains a separate required complete contract.

## Complete tagged volume mode routing

For tagged successful lookup only, select M.low4. No state-byte gate.

| Low nibble | Ordered local writes at Q | No additional action |
| --- | --- | --- |
| 0 | byteeec=V.low8 | No flag OR or synthVolume |
| 1 | wordefc=0 | No volume store or synthVolume |
| 2 | byteefa=old byteefa OR08; byteeec=V.low8 | No synthVolume |
| 3 | byteefa=old byteefa OR80; byteeec=V.low8 | No synthVolume |
| 4..15 | None | Return |

T is unused in this branch. Modes0/2/3 truncate V at byte store; mode1 ignores
V. High mode bits have no effect on tagged selection. Do not describe the bits
as completed stop/pause/fade execution without the deferred consumer. The OR08
matches tagged pause's bit, while tagged continue clears08; this shared field
relationship does not prove consumer cadence or an audible state transition.
All branches leave core result incidental; no independent success status.

## Complete public IRQ wrappers

sndSeqSpeed retains raw (ID,S), requests hwDisableIrq, seqSpeed(ID,S), then
hwEnableIrq. sndSeqVolume retains raw (V,T,ID,M), requests hwDisableIrq,
seqVolume(V,T,ID,M), then hwEnableIrq. Neither prechecks sndActive/validity,
normalizes input, repairs speed's lookup miss, or preserves a core return across
Enable. Core services themselves make no IRQ request. Ownership/nesting and
original audio critical-context policy remain separate source/domain contracts.

## Validation and remaining requirements

[Manifest](../../../tools/musyx_sequence_speed_volume_evidence.json) pins four
complete bodies plus five linked fingerprints (two ABI save/restore entry
symbols, synthVolume, two IRQ services): nine spans,137 instruction anchors,
ten resolved calls. ABI entry fingerprints alone do not describe their entire
fallthrough save/restore chains. Eight halfword vectors,16 speed offsets,seven
private-address routes,16 tagged modes,15 flag-OR vectors and three64-byte group
order examples are derived checks, not original execution. Fresh full ELFs/
listings stay `/tmp/inbox-seqSpeed.elf`, `/tmp/inbox-sndSeqSpeed.elf`,
`/tmp/inbox-seqVolume.elf`, `/tmp/inbox-sndSeqVolume.elf`.
No original body/asset exported to source history.

Remaining: complete synthVolume and sequence deferred/track consumers, actual
ID/group/field/flag/mode/time providers, caller lookup-success/private-ID/group
bounds/list/alias/IRQ/concurrency invariants and original tempo/volume/miss/audio
traces. These four local services are answered; EVT-01 and full native/source/
runtime/hardware equivalence remain OPEN.
