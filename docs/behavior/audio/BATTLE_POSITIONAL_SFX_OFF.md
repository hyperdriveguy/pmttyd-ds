# Positional SFX stop: corrected complete local contract

US G8ME01 disc0 revision0; psndSFXOff800d95f8/0xc8, independently reinspected
2026-10-13. This replaces earlier incorrect mask/branch/table interpretations.
Evidence is static original instructions; no playback/runtime equivalence claim.
See [provisioning](POSITIONAL_SFX_SLOT_PROVISIONING.md),
[accessors](BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md) and
[update ownership](POSITIONAL_SFX_UPDATE_OWNERS_AND_SOURCE_CORRECTIONS.md).

## Ordered stop contract

1. If raw handle==-1, return before slot access. Otherwise resolve
   P=803df988+40*(handle&ff). There is no local index/generation check.
2. Read key=P.word0. If key==-1, return WITHOUT writes.
3. Read flags=P.unsignedHalf6. If flags&1 is NONZERO, write P.word0=-1,
   then P.word18=-1, and return WITHOUT stopping an engine channel.
4. Otherwise read engineHandle=P.word18; derive row=80316b28+
   20*(key&1fff). If engineHandle==-1, skip engine stop but still clear.
5. Read row.word4. If mask80000000 is set, call SoundSSStopCh800ddff8;
   otherwise if mask40000000 is set, call SoundSongStopCh800e1188;
   otherwise call SoundEfxStop800e0c38. Pass engineHandle in r3.
6. Write P.word0=-1, then P.word18=-1. Return; no defined scalar output.

All offsets/masks above are hexadecimal. Table stride40/20 in the expressions
is decimal. Both high descriptor bits set select SoundSSStopCh because that
check comes first. The flags1 path represents a queued slot here; it is not
proof of an engine voice and must not be stopped as one. Update transitions
and actual voice creation remain separate contracts.

## Source anchors and correction

| Site | Instruction consequence |
| --- | --- |
| 800d963c /640 | clrlwi.31 retains ONE low bit; branch-equal proceeds to engine stop |
| 800d9648 /64c | Nonzero flag1 path clears key then engine handle without stop |
| 800d9658 | clrlwi19 retains LOW13 bits, key&0x1fff; it is not key>>19 |
| 800d9678 | Read descriptor at row+4, not name pointer at row+0 |
| 800d967c /968c | PPC bit0/1 are numeric masks80000000/40000000; set selects SS/song |
| 800d96a4 /6a8 | Common ordered clear after engine request or engine-handle-1 |

The 40-slot BSS array has extent1600 bytes; handle's low8 field permits
addresses beyond it for invalid handles. Do not claim those invalid inputs
occur in shipped content. Off does not validate the allocation halfword+4;
only Chk performs that comparison.

Verify the linked [evidence manifest](../../../tools/positional_sfx_corrections_evidence.json)
with `python3 tools/verify_overworld_roads.py --manifest tools/positional_sfx_corrections_evidence.json`.
The existing verifier supports generic DOL spans/anchors; this manifest contains
no road/REL/map assertions. Regenerate complete listing with inspect_dol_function
into `/tmp/inbox-sfxoff.elf`. Source identity and instruction checks do not
establish backend internals, other field writers or original execution.
