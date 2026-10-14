# Game sound flag owner and ordered set/clear

US G8ME01 disc0 revision0, read-only static recovery 2026-10-14. This
closes the sound-halfword producer/service question associated with
[EVT-01](../../requests/EVT-01.md) and
[letterbox globals](../camera/CAMERA_LETTERBOX_GLOBAL_PRODUCERS.md).

## Retained owner and startup provenance

The owner is unsigned halfword **803df73e**, sound record803df6e8+56.
psndInit800dc4e0/170 establishes r31=803df668 and r3=r31+80 at800dc598;
its `sth r5,0x56(r3)` at800dc608 writes zero, with r5 already zero.
This is an explicit initializer store, not inferred BSS or a native sound
enabled setting. It follows SoundInit800e239c and public SFX/environment
bank initialization. Later psndInit requests still follow the flag store;
knowing that the flag store executed does not certify whole sound bootstrap.
See [bootstrap services](GAME_AUDIO_ALLOCATOR_AND_MUSYX_BOOTSTRAP.md).

A direct branch census over all seven original DOL text sections finds
one psndInit caller: marioStInit8006fb3c at8006fda4. This is a direct-call
census only; indirect callbacks and REL imports are not excluded. Battle
Init does not call psndInit and does not reset this owner. Native battle
entry, checkpoint restoration or preference changes must retain/import
the actual owner rather than silently run startup's zero store again.

## Complete clear leaf

psndClearFlag800dc0f8/18 does exactly:

1. Load unsigned retained halfword.
2. AND with the complement of requested 32-bit r3.
3. Store the low16 result and return.

There are no downstream services, queries, stream restarts or fade requests.
The smallest native-safe binding uses a shared retained uint16 owner and
`flags = uint16_t(uint32_t(flags) & ~mask)`. No GPU/audio backend is needed
for this leaf. An unknown imported value must remain unknown; a clear may
establish the cleared mask bits without inventing other bits. Letterbox's
clear0x20 and field/battle clear0x80 must reach the same retained owner.

## Complete setter, including inherited-bit side effects

psndSetFlag800dc110/8c first loads the unsigned halfword, ORs requested32-bit
mask into it and stores low16 at800dc134. The bit0 test uses that OR result.
If bit0 set, invoke psndBGMOff_f_d800dabac with literal arguments:

1. `(r3=0, r4=1000, r5=0)` at800dc148.
2. `(r3=1, r4=1000, r5=0)` at800dc158.

Then **reload** the owner at800dc15c. If this fresh halfword has bit1 set,
invoke psndENVOff_f_d800d7470:

1. `(r3=0, r4=1000, r5=0)` at800dc174.
2. `(r3=1, r4=1000, r5=0)` at800dc184.

These gates test accumulated retained bits, not the requested mask alone.
Repeated sets can request these services repeatedly. The second group call
does not re-test its gate, while the environment gate does re-read the owner
after both BGM calls. A resumable binding must commit the OR once, retain
the first gate decision, finish each admitted service once, then perform
the fresh environment read. Waiting at a service cannot replay the OR or
earlier completed calls. See
[BGM routing](FIELD_ITEM95_FANFARE_BGM_ROUTING_AND_QUERIES.md) and
[percentage slew](FIELD_BGM_GLOBAL_PERCENTAGE_INITIALIZATION_AND_SLEW.md).

A finite owner with known inherited bits0/1 clear can set0x10,0x20 or0x80
without downstream requests. This is a bounded admission, not a general
replacement for BGM/environment fade services. An unknown gate is a real
provider question rather than permission to treat it as clear.

## Battle Init mask is hexadecimal

seq_battleInit80073fb8 bytes `38 60 00 80` decode `li r3,128`, followed
by the call to psndSetFlag at80073fbc. The requested mask is **0x80
(decimal128)**; writing decimal80 would instead set0x50 and is wrong.
It follows psndENVOff(1024), padRumbleOff(0) and evtStopAll(4) and precedes
the fresh fbatGetPointer query. This local source order must be retained;
the setter's availability alone does not complete the preceding services.
See [battle sequence lifecycle](../platform/ORIGINAL_SEQUENCE_TABLE_AND_FIELD_BATTLE_LIFECYCLE.md).

## Fresh byte evidence and limits

ISO opened `rb` only. Recovered complete spans were mapped using DOL header
and checked US symbol offsets. Ignored metadata:
artifacts/reference/sound-flags/evidence.json. Existing disassembly listings:
artifacts/reference/letterbox-globals/ and sequence-lifecycle/.

| Body | Address/size hex | SHA256 |
| --- | --- | --- |
| psndInit | 800dc4e0/170 | 3e0abb3a40bb3770ab7ad17952694b9d29c945ff9219102ebe5d53fdbe1e0483 |
| psndSetFlag | 800dc110/8c | 5c6a89e80c0716d9981162b651bb95e9ebf7b2ab9f1200ce361d63603e83abe4 |
| psndClearFlag | 800dc0f8/18 | 004dade64a998ad5d80c0c4549a1f9c9b9e40884cddd1be84cbdc3b943cff10c |
| seq_battleInit | 80073f20/f8 | 3753f89ed2fcc71a4457f8995b399d75809f6cf27c8f6dff9da1753145580904 |
| SoundInit | 800e239c/534 | f9286fdf165a2f6c455f49296a8a644eaaabcb9a0eb9d2bb366eefd938a34f15 |

Documentation only; no runtime, emulator, hardware or audible-fidelity
result claimed. Full sound flag writers, imported owner history, native
fade/group routing and whole bootstrap remain separate integration work.
