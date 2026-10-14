# Original Mario and partner pouch parameter revision

US G8ME01 revision0; recovered2026-10-13 from the read-only original ISO DOL.
This closes the parameter-revision leaf dependency of
[Goomba4 quest item removal](../field/ROGUEPORT_GOOMBA4_QUEST_AND_ITEM_SELECTION.md).
These are static source contracts, not runtime observations or hardware parity.

## Mario: pouchReviseMarioParam800d35a8/288

All pouch halfwords are signed reads and low16 stores. In source order:

1. Write maxHP+72 from baseHP+8e, then maxFP+76 from baseFP+90.
2. Scan all200 equipment halfwords+38a through+518, in ascending order.
   Item278 adds5 to maxHP; item280 adds5 to maxFP; all others leave those
   two fields unchanged. Zero does not terminate the scan. Each addition
   stores a halfword, so subsequent reads see the wrapped signed result.
3. If currentHP+70 is signed-greater than maxHP+72, copy maxHP to currentHP.
   Likewise clamp currentFP+74 downward to maxFP+76. There is no healing,
   lower-bound saturation or unrelated resource write.
4. Write remainingBP+92 from baseBP+94. For each of the same200 equipment
   halfwords, read the byte at itemDataTable803108a8 + signedItem*40 +1c,
   sign-extend that byte, subtract it from signed remainingBP, and store low16.
   The source performs this lookup even for item0 and does not clamp BP.

There are no calls, allocation, switches, event operands or return value.
The max-resource scan is unrolled two slots per100 iterations; BP scan is
unrolled eight slots per25 iterations. Those loop forms do not skip any slot.
A facade replacing them with ascending scalar loops must retain the per-store
halfword truncation and signed comparisons.

## Party: pouchRevisePartyParam800d3430/178

Count equipment item279 in all200 signed halfwords. Compute bonus5*count.
For each partner ID1 through7, with record base pouch+14*ID:

- Read signed rank at record+8. Read signed baseHP from
  80315380 +8*ID +2*rank, then store it at record+4.
- Read that stored signed baseHP, add bonus, store low16 at maxHP record+2.
- If signed currentHP record+6 exceeds signed maxHP record+2, copy the latter
  to currentHP. Do not heal or lower-clamp.

ID0 is skipped. Every other partner is revised regardless of its presence
flags. Rank is an unchecked source index; no normalization, unlock check or
rank3 exclusion occurs in this leaf. The source table contains four signed
halfwords per partner, including a rank3 value200:

| ID | Rank0 | Rank1 | Rank2 | Rank3 |
| --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 0 | 0 |
| 1 | 10 | 20 | 30 | 200 |
| 2 | 10 | 15 | 25 | 200 |
| 3 | 20 | 30 | 40 | 200 |
| 4 | 10 | 20 | 30 | 200 |
| 5 | 15 | 25 | 35 | 200 |
| 6 | 15 | 20 | 30 | 200 |
| 7 | 15 | 20 | 25 | 200 |

No partner FP, flags, rank, level or name is written. This leaf has no calls,
allocation, switches, event operands or return value.

## Native admission and event wrapper

`ProloguePouch::removeKeyItem` currently binds only low-level inventory removal;
it cannot represent evt_pouch_remove_item by itself. The event wrapper must run
Mario revision then party revision after successful low-level result1, and
expose event destination0. Failed removal exposes-1 and performs neither
revision. See [inventory removal](INVENTORY_REMOVAL_AND_RESERVATIONS.md).

The current fresh new-file pouch owns all0x5d4 bytes and proves all200
unequipped slots0. Source item0 BP cost is0. A restricted adapter can therefore
admit that explicitly checked domain using the complete writes above and the
original partner table. It must recheck admission before removing a quest item;
rejecting only after removal would mutate inventory without a valid wrapper.
Full equipment support requires the original item table BP-cost field+1c;
name/help/icon presentation data does not supply it. Costs are signed, even
though every nonzero byte in this supplied original table is positive.
A finite native partner table admits source ranks0..3 only unless it also maps
all source memory that an out-of-range rank may address. Bounds validation is
native admission, not a claimed source clamp.

The legacy checkpoint layout does not retain this full pouch or Goomba4's
GSW1420/GSF5405..5407. A low story value proves neither quest state nor inventory
nor equipment; unknown legacy imports must remain unknown. Quest2/13 guards,
source removal-success-independent flag writes, and selector/event lifetimes
remain separate from these leaves. This contract does not imply quest writers,
card migration, selector rendering, Mario pose/wait or key restoration are bound.

## Evidence

Fresh inspect_dol_function.py extraction from the original read-only ISO,
with local symbols as identity annotations, yielded these raw function hashes:

| Bytes | SHA256 |
| --- | --- |
| Mario800d35a8/288 | 975009df7ab363197b595e2a675ef38e92edb1d29bcccd33470eda783670c2e8 |
| Party800d3430/178 | b1a44503af3c0890d3c710c76aa7fcd8dcac6abb4204b38ab4645cef0999c35c |
| Party table80315380/40 | 7bca019f8482bbf58aeef1735bf62276e136728dc6c9f7666301549b67d3a14a |

Lengths in the table are hexadecimal. The340 physical original item rows have
stride40(decimal); concatenated bytes+1c SHA256 is
1a4438d061e35924f4ace71ed70663ff5a3f03e42ae9047e9042955964fbd5cc.
Item0 costs0;278/279/280 cost3/6/3. Extracted ELF/disassembly and cost bytes
remain under /tmp, outside source history. No ISO assets, dialogue, implementation,
shared index, PROGRESS or emulator state were changed by this recovery worker.
