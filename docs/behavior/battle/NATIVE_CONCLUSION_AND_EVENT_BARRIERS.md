# Portable original battle conclusion and event barriers

US G8ME01 disc0 revision0, source instruction contracts plus native portable
checks. This closes local services, not the entire battle-end graph or field
return. See [original contracts](BATTLE_WAITS_AND_CONCLUSION.md) and
[end result priority](UNDERGROUND_TRIO_TARGETS_TURNS_AND_VICTORY.md#exact-victory-result1-and-completion-publication).

`battle_conclusion.hpp/.cpp` supplies four host-testable services over caller-owned
original-address memory and required status/event/reaction/enemy-side providers.
No 3DS service, dialogue text, victory shortcut or HP inference is embedded.

BattleCheckConcluded8011aa34 clears and refreshes each of three group result words.
Configuration0 immediately concludes; bit1 requires a member without status27,
20000 or10000000; bit2 requires40000 before status27 and then re-reads10000000.
The second configuration test re-reads the original halfword after the first scan.
Bit1 reads attributes once after status; bit2 reloads after status. Provider
mutation tests therefore preserve source read/call ordering rather than folding
all state into an immutable snapshot. Separate members may satisfy both scans.
Two concluded groups return1; otherwise current flags10/20 can force1 without
skipping the earlier group writes. HP is never read by these scans.

BattleWaitAllActiveEvtEnd_NoBgSetEndWait8011abb4 scans all64 unit slots and each
29c/2a8/2b4/2bc handle in order. Finished IDs clear; live IDs remain. A live ID
does not skip later cleanup. Only when every scanned ID is finished does the
required original after-reaction provider run; its nonzero return holds the
barrier. Outer BattleWaitAllActiveEvtEnd8011ace8 then checks battle handles
182b4/182c4/180fc/182c8/182c0 in order, clearing finished IDs and returning
immediately on the first live one. Wait290 is intentionally outside this barrier.
A missing queue/effect/reaction provider must not be replaced with assumed0.

`battleInitialEndResult` implements only btlseqEnd80215234's initial state
07000000 selection region8021529c..344. It resolves Mario from the actual
registry, captures signed own membership, calls original enemy-side provider,
then reads flags. Priority is40→4,20→5,own conclusion→3,enemy conclusion→1,
10→4,else no selection. Even an override branch still performs Mario/side
resolution before the flags branch. It returns an optional result and writes
nothing. The caller must own actual admission to the end state and the source
FieldBattleInfo setter, subsequent reward/status/pouch/cleanup and completion
publication. An optional1 is not authorization to return to the field.

Read-only inspection pinned actual BattleCheckConcluded full instructions and
the initial end region in ignored artifacts/harbor-{conclusion,end-result}.elf.
Portable checks cover status27 rather than HP0, helper exclusion, simultaneous
loss/victory priority, overriding flags, unrelated configuration bits, provider
attribute mutation, complete unit cleanup after a live ID, after-reaction hold
and outer early return. Full original-runtime traces remain absent.

Current harbor semantic init/phase owner does not yet share the packed combat
registry. These services are the reusable foundation for that migration. Source
commands/attack/damage/death/reward/end graph and sequence2/result field-return
barrier must complete before the opening encounter publishes a genuine result.
