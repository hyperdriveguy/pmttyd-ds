# Battle action-class table builder

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Closes `_btlcmd_MakeActClassTable` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Fingerprint: entry 80124dc8, `.elf` 1656 bytes, 372-line `.disasm`, 9 `bl`
(3 distinct absolute targets). `us-usage.json`: `battle_seq_command.o`,
size 1464, 2 link sites, not fingerprinted. No `evidence.json` row.

Callee identities: `BattleGetUnitPtr` 0x800f88cc ([unit
lookup](BATTLE_ACTOR_SELECTOR_TRANSLATION.md) pipeline), `pouchGetPtr`
0x800d5eb0 (size 8, shared pouch pointer per [weapon power
callbacks](BATTLE_WEAPON_POWER_CALLBACKS.md)), `msgSearch` 0x80081b8c
(msgdrv.o, 224 bytes, no prose contract — cited positionally).

## Setup

Incoming r3 is retained in r23 (call it B; the body reads B+1056 as a unit
id, B+10040, and B+3828 battle-flag words). r29 = absolute 0x802EFC28
(`lis` 0x802F0000 − 984). r30 = `BattleGetUnitPtr(B, [B+1056])` (unit);
r31 = `pouchGetPtr()` (pouch). r27 = B+5916 (header record); the
`cmplwi r27,0` → epilogue edge is vestigial (taken only if B == −5916).
r26 = r27+1176 when [unit+8] == 222, else r27+1188 (record-array base).

## Kind-222 header init (skipped otherwise)

Writes [r27+8]=0, [r27+12]=1, [r27+24]=0, then a series of conditional
overwrites, each writing ([r27+12],[r27+24]) = (0,3) when its test hits
(last hit wins): [unit+260] bit LSB4 (0x10); signed pouch+152 < 1; nonzero
[unit+768]; nonzero word at [B+102480]; [r24+4] bit LSB24, where
r24 = [[B+10040]+12]; [B+3828] & 0x06800000. Then [r27+16] =
`msgSearch(0x802EFC28+1008)`, and halfword [r27+20] = 335/334/336 by signed
pouch+152 == 2 / == 1 / otherwise. Only r3 is loaded per `msgSearch` call;
r4/r5 are residues — the same non-reload hazard as the pose-table leaf.

## Record regions (20-byte stride, r25 counts from 0 or 1)

Each region appends records at r27+r25*20 (r25++ each) with [rec+8] = tag,
[rec+12]/[rec+24] flag pair defaulting per region, [rec+16] =
`msgSearch(0x802EFC28+off)`, [rec+20] = halfword id. Conditional overwrites
again write (0,3):

| Region | Gate on [unit+8] | Tag / msg off / id | Extra gates |
| --- | --- | --- | --- |
| R1 | 224..230 only | 6 / +1028 / 343 | [unit+299] sign, nonzero [unit+316], [B+3828] & 0x04800000 |
| R2 | == 222 only, plus pouch+153 ≥ 1 for the tail | 1 / +1052 / 337/338/339 by pouch+153 == 1/2/other | [unit+260] bit4, nonzero [unit+767], [B+102484], [r24+4] bit25, [B+3828] & 0x05800000 |
| R3a/R3b | == 222, resp. 224..230; bodies instruction-identical | 2 / +740 / 340 | [unit+299] sign, nonzero [unit+318], [B+3828] bits 3..8 kept-mask test |
| R4 | == 222 and nonzero pouch+140 | 4 / +1072 / 341 | [unit+260] bit4, [B+3828] & 0x1B800000 |
| R5 (two records) | [B+3828] bit LSB27 set | 13,id 342 and 0/1/0,id 342 / +1100,+1124 | second record gated on [B+3828] & 0x10800000 for its flag pair |

## Count publication and index clamp

[r26+8] = r25 (record count). Then [r26+0] is clamped: if it is already
below the count it is kept, else it is set to count−1 (`cmpw` + decrement +
store). No return value is constructed.

## Not established

Unit/pouch/flag meanings (222/224..231 classes, 334–343 ids, message-table
contents), table ownership/lifetime, `msgSearch` argument semantics beyond
the loaded pointer, call cadence, and any battle effect. The R3a/R3b
duplication is reported as observed code fact.
