# Original shared random-state initialization and writer census

US G8ME01 revision0; recovered2026-10-14 for
[RNG-01](../../requests/RNG-01.md). B = inspected original bytes/instructions;
P = native ownership requirement derived from that evidence. ISO opened read-only.
This is a static lifecycle slice, not a complete random-consumer census or a
matched original/native execution trace. Arithmetic remains in
[RANDOMNESS.md](RANDOMNESS.md).

## Launch state comes from initialized data (B)

The original `rand` state at **8041e3c0 contains 00000001 in the DOL image**.
It is initialized `.sdata`, not zero-filled BSS and not a timer-derived seed.
The four bytes have SHA256
`b40711a88c7039756fb8a73827eabe2c0fe5a0346ca7e0a104adc0fc764f528d`.
The address is in DOL data slot11: file offset3c2360, RAM80414f20,
loaded size9660; the state's DOL-relative file offset is3cb800. The disc DOL
starts at ISO offset20300. These are hexadecimal addresses/offsets/sizes.

`__init_registers`8000539c installs r13=8041cf20 at80005420/24, so
r13+14a0 is exactly8041e3c0. `__start`80005240 calls this routine first,
then calls `__init_data`8000542c at80005258 before main800055e0.
The copy-table row at8000558c is
`{source80414f20,destination80414f20,length964c}`. Source and destination
are equal, so the copy loop skips memcpy for this row; the loaded value1
survives startup. It is not assigned by a separately recovered srand call.

The actual clear table at800055b0 contains precisely:

| Destination | Byte count | End, exclusive |
| --- | --- | --- |
| 803c5360 | 4fbb0 | 80414f10 |
| 8041e580 | ea4 | 8041f424 |
| 80429500 | 40c | 8042990c |

None covers8041e3c0. The broad DOL BSS header interval803c5360+645ac
alone must not be used to assert that this initialized-data word starts at0:
the executable startup uses the separate clear records above. DOL section
loading and an ordinary start at the original entry establish launch seed1;
no emulator-specific memory initialization or observed Dolphin trace is claimed.

## Direct writer and reference census (B, with explicit scope)

Scanned every aligned instruction word in both nonempty original DOL text
sections, `.init`80003100/24e0 and `.text`800055e0/2b9b40. The only words with
base register13 and displacement14a0 are:

| Instruction address | Word | Operation |
| --- | --- | --- |
| 8026c328 | 808d14a0 | rand loads current state into r4 |
| 8026c338 | 900d14a0 | rand stores next state from r0 |

Thus the only statically identified direct runtime writer is
**rand8026c324, store8026c338**. Its update is the shared recurrence already
specified in RANDOMNESS.md. There is no `srand` entry in the inspected symbol
catalogue; symbol absence alone is not the writer proof.

Additional whole-image checks:

- Every aligned loaded DOL word was checked for literal pointer8041e3c0:
  no occurrences.
- Every DOL text instruction with low immediatee3c0 was inspected: all11
  occurrences are r2-relative float loads in effKemuri9Disp/effKemuri9N64Entry,
  not absolute-address construction or state writes. No lis/addi or lis/ori
  construction with that low half was found.
- All29 on-disc `rel/*.rel` modules were scanned. Executable sections have
  zero r13-relative displacement14a0 references; all module0 relocation
  streams have zero targets8041e3c0; loaded module words have zero literal
  pointers8041e3c0. Nearby module0 references found by the broader
  [8041e3b0,8041e3d0) search target only8041e3c8 in las/tik, a different word.

This includes the DOL's new-file/title/load code and every on-disc REL in
that direct-reference search. It does not prove the absence of an arbitrary
computed alias, an indexed store, a bulk operation spanning initialized data,
a debugger write or externally injected state. No complete global pointer
analysis is claimed. Do not convert a negative direct-reference census into
an unconditional full-runtime theorem.

## New file, title/demo and load boundaries (B/P)

`marioStInit`8006fb3c clears GP=[80415fd0] for13d8 bytes at8006fbe8;
that request is a GP reset, not a write to the independent `.sdata` RNG word.
The complete `stg0_00_init`800ee688/70 body contains no direct access to
8041e3c0 or reseed operation. Its called children retain their own effects;
this is not a claim that starting a new file consumes zero random draws.
Likewise no direct title/demo reset writer was found in the whole-DOL/REL
census. Returning to title must not be equated to restarting the executable.

The already recovered [card restore](../memory/CARD_LIVE_NAMESPACE_RESTORE_AND_PRESERVED_GP.md)
copies six record blocks into GP, pouch, field-battle history, bdsw, yuwp and
fixed namespace803bd0d4. No listed destination is the standalone RNG word;
its explicit post-copy writes do not name that word either. Its sole direct
DOL caller is continueGame800f39dc; that function has no direct reseed access.
The [continuation contract](../memory/SAVE_CONTINUE_GAME_PARTY_AND_DESTINATION_DISPATCH.md)
includes a switch23 stg0 reset, which likewise supplies no direct seed store.
This rules out adding a seed field to those recovered record blocks on this
evidence. It does not prove complete save persistence, child alias behavior,
or every runtime side effect of loading a file.

## Retained native ownership for npcEnt battle setup (P)

One process/session owner should retain the actual 32-bit state across field
areas, event-facade retirement/recreation, NPC registration, battle and message
consumers. An original cold executable launch initializes it once to1.
Room entry, npcEntry, event creation and new-file GP initialization supply no
recovered reseed boundary. A restored emulator snapshot is a different boundary:
use the captured actual state rather than silently replacing it with1.

**Seed1 is not the seed immediately before the first Rogueport npcEnt call.**
Earlier boot/title/demo/message/effect consumers may already have advanced it.
A native path that omits or schedules those consumers differently cannot claim
matching original NPC random outputs merely because its cold seed is correct.
Each original npcEnt battle0 consumer must use the same retained owner as
other admitted original consumers, in original request order; no per-NPC,
per-room or cosmetic independent stream is justified. Source-derived draw
counts are necessary but do not establish complete cross-subsystem ordering.

For a provisional native implementation, keep that limitation explicit and
allow a caller-supplied/captured state for deterministic fixtures. Log state
before/after, call site, argument/result and simulation tick when investigating
divergence. Do not draw on failed resource admission or native resume retries.
Do not add new rendering-frame draws to compensate for missing original
consumers. Existing original message/font rendering does consume this stream;
its actual scheduling remains part of the unresolved interleaving contract.

## Reproduction and remaining evidence

Ignored evidence directory `artifacts/field-rng-lifecycle/` contains original
inspection ELF/listings for rand, __start, __init_registers, __init_data,
main, marioStInit, stg0_00_init and continueGame; `dol-state-scan.json` contains
source copy/clear tables and all census hits; `rel-state-scan.json` records
all29 original module hashes and scan results. `scan-rels.py` reproduces the
REL census directly from the read-only ISO. Raw original bytes and listings
are not committed. The inspected DOL extent3d5a80 has SHA256
`93c79aa312ca57c2785d0854a516c554e7683b525f8c3456dad568f4c440451a`.

This documentation-only delivery ran read-only extraction/disassembly and
complete scoped scans; no native build or gameplay test was run. RNG-01 remains
open for computed/bulk-writer proof, other random helpers, full consumer census,
boot/title/demo/load runtime watchpoint traces, save persistence and matched
cross-subsystem draw order. A write watchpoint on8041e3c0 with PC/stack/old/new
values through those paths would strengthen the static writer census; it must
also distinguish actual cold DOL loading from in-process title/new-file flow.
