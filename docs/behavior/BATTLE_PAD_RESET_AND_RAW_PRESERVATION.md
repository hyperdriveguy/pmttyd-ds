# Complete battle-pad reset and raw byte preservation

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
Complete BtlPad_WorkInit8011a38c/b0 and checked snapshot copy/write region of
BtlPad_WorkUpdate8011a0ac/2e0. Verify:
`python3 tools/verify_input_cadence_contracts.py`. Read
[cadence/snapshots](BATTLE_INPUT_CADENCE_AND_SNAPSHOTS.md),
[reset owners](BATTLE_INPUT_RESET_OWNERS.md),
[defense consumers](BATTLE_INPUT_AND_DEFENCE.md) and
[physical input producer](PHYSICAL_INPUT_PRODUCER.md).
This answers INPUT-01's byteb clarification;static source evidence only.

## Complete pad-relative508-byte partition

Original pad stride is1fc hex=508 bytes. WorkInit receives raw pad P with no
null,size,channel,scene or initialization check. It calls no service,reads no
old pad values,allocates nothing and has no specified scalar return contract.
The input pointer's valid extent is an owner/platform requirement.

| Relative span | Bytes | WorkInit behavior |
| --- | --- | --- |
| 000..00a | 11 | Clear current snapshot bytes0..a |
| 00b | 1 | PRESERVE current snapshot byteb |
| 00c..016 | 11 | Clear prior snapshot bytes0..a |
| 017 | 1 | PRESERVE prior snapshot byteb |
| 018..08f | 120 | Clear30 pressed-history words |
| 090..107 | 120 | Clear30 repeat-history words |
| 108..17f | 120 | Clear30 held-history words |
| 180..1f7 | 120 | PRESERVE entire release bank |
| 1f8 | 1 | Clear repeat-phase byte |
| 1f9 | 1 | Clear repeat-counter byte |
| 1fa..1fb | 2 | PRESERVE raw tail bytes;no meaning established here |

The routine clears384 bytes and preserves124 bytes of this stride. It is not
memset(P,0,508),not a whole-snapshot assignment and not release-mask reset.
Preservation is source behavior for dirty existing pads,even when a fresh
battle allocation or native default construction happened to initialize them0.
Do not describe the unknown preserved bytes as controller errors or device
status without separate producers/consumers. Their values are not read here.

## Exact snapshot-store loop

Set counter2 and offset0. For each iteration retain Q=P+offset,advance offset
by12,then perform at Q:

1. Store zero HALFWORD at+0 (clears bytes0/1).
2. Store zero BYTE at+2,+3,+4,+5,+6,+7,+8,+9,+a,in that order.
3. Loop to the next snapshot if the two-iteration counter remains nonzero.

The ten store sites are8011a3d0..8011a3f4;stride12 is8011a3cc and loop branch
8011a3f8. There is NO byte+b store,halfword+a store or word+8 store. The
second iteration's Q=P+c therefore preserves P+17 independently of P+b.
The current and prior byteb need not match;WorkInit does not copy either into
the other. Current/prior bytea ARE both explicitly cleared.

For a dirty current bytea=A,current byteb=B,prior bytea=C,prior byteb=D,
reset produces0/B and0/D. A whole-snapshot memset or assignment would erase
B/D and differ. Preserve halfword versus byte width when providing observable
raw memory operations;the first store clears two adjacent bytes as one request.

## Exact history and phase-store loop

After snapshots set count30 and byte offset0. For i=0..29 in ascending order,
retain Q=P+4*i and store zero WORD to Q+18,then Q+90,then Q+108. The three
sites are8011a414/418/41c,with4-byte step8011a410 and loop8011a420. This
INTERLEAVES the three banks by index. It does not memset the entire first bank
then the entire second bank,or shift/rotate any history. For index29 the
addresses are P8c,P104 and P17c. No store reaches P180 release word.

Then byte-store zero to P1f8 at8011a428 and P1f9 at8011a42c. Tail1fa/1fb
has no store. There is no release-bank read or write,physical GP mutation,
controller poll,private/general cross-pad reset or callback reset in this body.
Those are caller and separate input/AC-manager operations.

## WorkUpdate: prior copy versus current trailing preservation

WorkUpdate first reads old current WORD0,then WORD4;stores them to prior+c
and prior+10 respectively. It then reads current WORD8 and stores prior+14.
Thus the full12-byte previous current snapshot is copied to prior,including
current bytesa/b becoming prior16/17. Original copy is three word operations,
not a12-byte hardware resample. No callback occurs between these local loads/
stores,but a raw adapter must preserve their specified order.

After shifting history,the current snapshot writes a low16 button halfword,
six input bytes at2..7,and explicit zero bytes8/9. It supplies no current+a
or+b writes. Therefore after WorkInit,ordinary subsequent updates keep current
bytea0 and current byteb its retained old value;the first update replaces prior
byteb with the retained CURRENT byteb,not its own independently retained value.
Without reset,current bytea may also persist nonzero. There is no automatic
“unknown trailing bytes must be0” policy in WorkUpdate.

Each physical source field can reload GP independently,as the snapshot contract
states. Reset neither selects a channel nor supplies GP input data. Reset and
update are separate operations with different write domains;do not reuse one
blanket snapshot-clear helper for both. Release WORD180 is later updated by
WorkUpdate's current mask path;other29 release-bank words are not shifted by
that updater. Full edge/repeat/defense rules remain linked contracts.

## Caller behavior,acceptance and native limitations

Four direct DOL WorkInit call sites and one shipped gor import are already
pinned in reset owners:general initialization,action-command initialization,
every defense return and the lecture coordinator's injection reset. This full
byte contract applies to the passed pad regardless of which owner calls it.
A private reset does not erase general pad banks or GP physical history.
Construction/allocation zeroing is a different write source and must not be
silently credited to this callback.

Native owner reports preserving each old byteb during init and both trailing
bytes during update,while clearing bytea during init. That matches this local
map;host semantic state still does not prove packed raw tail/coherent byte
access or original hardware/call cadence. Implementation checks should cover:

- Dirty distinct current/prior bytesa/b,exact0/B and0/D reset results,and
  subsequent prior byteb becoming old current byteb during update.
- All90 history words cleared in index-interleaved order,both repeat bytes0,
  all30 release words and both raw tail bytes preserved independently.
- Snapshot halfword/byte store widths and addresses,no+b write,and current
  versus prior/physical producer ownership.
- Init→update,update→defense-reset and repeated reset without introducing
  controller polls,release clear,extra samples or invented metadata meaning.

INPUT-01's reset-byte map is closed. PADRead/Clamp/Reset SDK behavior,physical
error/reconnect/frame-rate/pause/control/motor writers,native projection and
original sampling/impact traces remain open. No emulator observation or timing
measurement is claimed by these static instruction/store checks.
