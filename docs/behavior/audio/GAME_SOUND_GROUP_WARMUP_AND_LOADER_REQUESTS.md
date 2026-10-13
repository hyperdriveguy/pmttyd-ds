# Game sound group warm-up and loaded-bank push requests

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete psndPushGroup800dc3a8/88,
SoundLoadDVD2PushGroup800e1dd0/11c and SoundLoadDVD2Free800e1d88/48hex.
Read [MusyX group contract](MUSYX_GROUP_PUSH_POP_AND_RESOURCE_LISTS.md).
Actual DVD/resource loading, bank selection, sound initialization and original
execution remain separate; this contract does not claim every group is resident.

## Complete game warm-up step

Read busy WORD8041eb0c. Any nonzero returns1 without other effects.
Otherwise read signed next-group WORD8041eb10:

- >11: store init_f WORD8041eb08=1, call SoundLoadDVD2Free, return0.
- <=11: store busy1; write low8 next-group at8041eb14 and byteff at+1;
  call SoundLoadDVD2PushGroup(&that two-byte list). Ignore its return.
  Freshly read next-group, clear busy0, store next-group+1 with word wrap,
  return1.

The ready flag precedes temporary-buffer free. Negative next-group also takes
the latter branch; there is no0..11 range guard. Under stable initial0, twelve
admitted calls attempt groups0..11 and the next call marks ready/returns0.
These are derived SERVICE INVOCATIONS, not video frames or measured boot time.
Because push return is ignored, ready/index advance is not proof of residency.

## Complete loaded-bank push wrapper

Retain list pointer L and S=803e0138. Read U32 bank index S.e0. >=8 returns0
before any count clear. Otherwise clear word(S+c0+4*initialBank) to0. Bank
selection is reloaded for subsequent request/count updates, not retained.

For nonnull L, read byte IDs untilff. For every byte:
read CURRENT bank, fetch project pointer atS+20+4*bank, directory pointer at
S+40+4*bank, pool pointer atS+0+4*bank. Request
`sndPushGroup(project,byteID,0,directory,pool)`.
Ignore its scalar return; advance list by1, freshly select bank/count and
increment its c0 count. Empty list returns1 after initial clear; failed group
requests still count as attempts. Raw sample-address argument is0, not a
fabricated host sample-bank pointer.

For null L, capture local signed next ID from S.e8. While signed localID<255,
request the same current-bank tuple with U16(localID). Exact sndPushGroup
result1 increments localID, then freshly selected c0 count, then freshly read
S.e8+1. First result!=1 stops with no increments for that request. Exit returns1
EVEN when the first group request failed; S.e8 is not overwritten with the local
loop variable. Negative counters have no guard and low16 ID narrowing remains.

This sequential mode differs from explicit byte-list mode. Neither wrapper
supplies rollback or allocates/decodes original files. Bank pointers/count/cache
writers, original residency and actual source index domains remain prerequisites.

## Complete temporary-buffer free

SoundLoadDVD2Free retains S, reads wordS32c. Null skips. Nonnull requests
__memFree(0,thatPointer)800300c4, then clears S32c to0. It does not clear/free
the bank project/pool/directory pointer arrays, pop groups or reset readiness/
next-group/counter fields. Child heap/source lifetime is separately documented;
there is no new ownership permission for original ISO/resource files.

Validation: [source manifest](../../../tools/musyx_group_lifecycle_evidence.json)
pins all three bodies and current-bank/ready/busy/count/request/free anchors.
Fresh complete listings remain `/tmp/inbox-psndpush.elf`, dvdpush and dvdfree
ELFs from read-only ISO. No original asset/code-body export or native change.
Complete DVD load/init/bank/project/sample residency and original traces remain
OPEN. Do not treat provisional native startup cadence as this game's evidence.
