# Native original present-item consumer

Implements whole US `_check_present_item`80120024/1f8, plus packed
BtlUnit_RecoverHp80126bec/24, RecoverFp80126b58/94 and GetMaxFp80126b14/44.
The existing [source verifier](../../../tools/verify_menu_interruptions.py)
pins the consumer, recovery leaves, getters, position/height and numeric spans.
Fresh read-only ISO disassembly also covers the complete MaxFp leaf. See the
[original branch contract](GIFT_ITEM_RECOVERY_AND_MENU_TYPE_WRITER.md).
No original timing, FPSCR or Dolphin execution parity is inferred from static
body inspection or default native math adapters.

## Entry and generic handoff

Capture B before the present-number getter's own fresh global lookup. Signed
I>=339 returns0 before target query; negative I still performs target and unit
queries before dispatch rejection. Target must be signed-1..63. Unit lookup uses
captured B; no additional null guard changes the original recovery paths.
Item0 returns0 after that lookup. Generic admitted items (excluding123/124)
set pending1, or pending2 when the freshly read Input state is12/13. Whole
AllEnd masks flags with ffeeeeee; it does not free UI, clear an item or end every
menu. Previous type is read after that store; then type12/state29 are published
and the helper returns1. It does not create an event or grant inventory.

The native command entry now uses this consumer in place of its prior None-only
admission. Generic present items can reach the actual original menu handoff.
Subsequent attack selection, AudienceMove/Get/Damage execution and receipt
ownership remain separate; a type12 write is not item delivery.

## Recovery and retained effects

Items123/124 recover HP/FP by1 before reading position, signed height and numeric
data. HP stores the wrapped16-bit sum, rereads HP and maximum, and upper clamps
only. FP preserves source partner224..230 sharing, the captured battle, separate
Mario lookups for maximum and actual owner, mandatory own maximum read inside
GetMaxFp, wrapped sum, reread and upper clamp. No lower clamp or semantic unit
cache replaces these packed reads.

GetPos loads/stores X, then Y, then Z; output aliases can change later loads.
The original source loads its conversion bias, scale and5.0 before fsubs; after
that single conversion it loads private Y, X and Z before fmadds and fadds.
The implementation retains those acknowledged operands at their actual source
positions. In particular, it does not reload scale/5 after a numeric-provider
wait, or replace fused multiply/add with an independently rounded product.
Private effect Y is stored before effRecoveryEntry(kind,1,X,Y,Z). The callback
bool denotes execution acknowledgement; this caller ignores the original
effect entry's return value.

Unavailable math/effect providers retain the acknowledged recovery and private
operands. Retrying cannot grant another HP/FP point. Acknowledged effect-entry execution then clears
present number0 through a fresh global owner, leaves target/type/state alone,
and returns0. Return0 can therefore include real resource/effect/clear work.
A cursor rejects a different memory owner/read/store binding or private position.

The native bridge uses its existing declared numeric platform adapters and
reserved private position work. It still has no admitted real effRecoveryEntry
owner/resource/renderer: recovery gifts apply their original resource operation
once and hold there; no fake effect or item clear acknowledges completion.
The existing outer CommandInput unbound latch remains a separate continuation
frontier. Direct helper retention is verified without claiming automatic outer
Main resumption or complete playable gift handling.

## Evidence scope

Portable checks cover signed item/target boundaries and query order, pending1/2,
post-AllEnd previous type, the exact mask, return0 recovery, fresh clear owner,
math/effect waits, wrong-owner rejection, aliased position outputs and load order.
A cancellation case retains2^-23 through fmadds where an unfused product gives0.
Packed HP/FP checks cover overflow, post-store maximum changes, partner kind
boundaries and a different Mario owner between FP lookups. The native bridge
checks real generic metadata and recovery holding at the unbound effect.
Explicit test effect acknowledgement is a fixture service, not a shipped effect.

The existing private harbor event pack already contains original bias802efe78/8
and5.0 at804225f8/4; both were compared byte-for-byte with the read-only ISO.
No new images, dialogue, music, UI style, input or simulation timing were added.
Exact validation and remaining audience/command work are in
[PROGRESS](../../PROGRESS.md).
