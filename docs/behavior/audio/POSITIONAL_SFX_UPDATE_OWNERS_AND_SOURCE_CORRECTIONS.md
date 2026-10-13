# Positional SFX update owners and source corrections

US G8ME01 disc0 revision0, independently recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete psndMain800dc48c/54 and scoped
psndSFXMain800d9e74/b94 prefix, pending transition and tail. Whole-body
fingerprints do not imply full middle-body semantics. Static evidence only.
Read corrected [provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md),
[stop](BATTLE_POSITIONAL_SFX_OFF.md) and [accessors](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md).

## Complete outer sound update and cooldown owner

Read word8041eb08 (`init_f`, r13+1be8). Zero skips EVERY update request and
cooldown mutation. Nonzero captures G=803df6e8 (`psnd`, extent60hex), then:

1. Read G.byte5c and sign-extend for nonzero test. If nonzero, decrement the
   loaded unsigned byte by1 and store low8 back. No clamp/positive-only test.
2. Call psndBGMMain800db7cc, psndENVMain800d7b64, psndSFXMain800d9e74,
   then SoundMain800e22f4, in that order.
3. Return without a defined scalar status.

At initial cooldown6, six admitted psndMain invocations decrement to0.
This is a derived update-count case, not6 rendering frames or milliseconds.
It only gates the five special provisioning indices, not all SFX. Disabled
init_f preserves the stored byte. Negative signed bytes also decrement with
byte wrap; the local body introduces no repair. Instructions800dc4ac..4bc
close this producer. Earlier notes assigning it to psndSFXMain were wrong.

## Finished-channel sweep in psndSFXMain

Before other processing, scan all40 slots ascending at stride40. Free key-1
skips. flags6 mask1 NONZERO skips this completion sweep; these records are
pending, not backend channels yet. For a live flags1-clear record:

- Select row80316b28+20*(key&1fff); read row.word4.
- Mask80000000 set: SoundSSCheck800dd968(P.word18).
- Else mask40000000 set: SoundSongCheck800e0f94(P.word18).
- Else SoundEfxCheck800e07c0(P.word18).
- Nonzero check preserves fields; exact0 stores P.word0=-1 then P.word18=-1.

No additional engineHandle-1, slot generation or source-row bound check here.
This pass establishes local completion query/clear ordering, not the meanings
of every backend return. Prefix region800d9e98..9f4c is fully inspected.

## Pending-slot transition and preference reset

The later40-slot pass at800da010 first skips free slots, derives the same low13
row index and counts occupied entries. When flags1 is set it clears mask100
at800da044/048, reads unsigned delay P0e, and if nonzero decrements/stores it
then skips the rest of that slot for this pass. This first decrement does not
also dispatch the backend when it reaches0. Zero-on-entry delay admits startup
routes selected by row.word4 high bits.

Startup's backend selection, parameter setters, allocation and spatial/volume
services are NOT fully documented by this scoped delivery. The convergence
at800da36c tests engineHandle P18: exact-1 clears P0/P18 and skips this slot;
otherwise800da38c..394 clears pending flag1, then enters later live processing.
This proves flag1 is a real pending/startup marker and can become0. Its initial
value1 in provisioning does not mean Off always skips engine stops forever.

After the second pass, store low8 occupied count to G5b at800da9ec and reset
G.unsignedHalf54=0 at800da9f0. Duplicated starts can add8 during a pass interval;
the update resets that preference rather than decrementing it. Count increments
before startup/failure processing, so it is not necessarily the final live count.
No early return after loop entry bypasses this tail in the inspected local body.

## Corrections to prior deliveries and implementation consequences

| Prior claim | Verified replacement |
| --- | --- |
| key>>19 / top13 row index | key&1fff / LOW13 row index, in start/stop/pitch/update |
| Off low5 clear skips stop | Off flags bit0 SET skips stop; clear takes backend path |
| Stop table.word0 / low bits | descriptor row+4, numeric80000000 then40000000 set-priority |
| Name-only start ABI | Signed nonnegative raw key or signed-negative string pointer |
| G803df188 | G803df6e8, `psnd` symbol |
| Mute requires nonzero override | Mask4 suppresses only when override r10==0 |
| First20 reuse search | Unrolled20 iterations process all40 slots, retain last free |
| Equal-priority reclaim | Unsigned old halfC STRICTLY GREATER than new row byte10 |
| Dead255 volume/pan clamps | Live low8==255 defaults:127 / row byteD |
| P0 index versus packed conflict | Raw numeric key may carry high flags; row consumers mask low13 |
| P10 repeats converted delay | P10 gets low16(5*preference), independently of P0e |
| Generation wrap breaks Chk | Counter is reloaded unsigned16 before handle shift; wrap stays coherent |
| Cooldown in SFX update | psndMain decrements G5c before BGM/ENV/SFX/backend requests |

Historical request/delivery paragraphs remain append-only; these contracts and
this dated correction supersede their conflicting claims. Native providers must
preserve pending-slot return versus engine handle, queued local pitch, real
row descriptor and deferred completion before selecting original audio data.

Validation: [manifest](../../../tools/positional_sfx_corrections_evidence.json)
pins nine bodies, selected source anchors and table/numeric spans. Run the
existing generic DOL verifier:
`python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`.
ELFs/listings remain `/tmp/inbox-*.elf` / `.disasm`; ISO is read-only.
Full SFX update spatial/volume/start/macro/resource paths and original traces
remain OPEN. This adds no native code, sampled replacement tone or asset export.

## Pending/backend continuation (2026-10-13)

[Complete pending startup requests](POSITIONAL_SFX_PENDING_STARTUP_REQUESTS.md)
now closes800da010..398, including real stream/song/effect routing, spatial
packet extraction, aux and same-pass pending-clear/live entry. [Effect PlayEx](SOUND_EFFECT_PLAY_EX_ALLOCATION_AND_START.md)
closes separate40-record first-free/priority-steal/start/interrupt/final-liveness
behavior; [five controls](SOUND_EFFECT_RECORD_CONTROL_LEAVES.md) close Check,
Volume,Pitch,Aux1 and Stop. These supersede this file's earlier “startup/backend
not documented” scope for those exact local bodies only. calc3D, live main
spatial/volume/fade, MusyX/resources/hardware/source/runtime remain open.
