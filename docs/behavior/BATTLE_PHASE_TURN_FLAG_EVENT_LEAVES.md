# Battle phase/turn/flag event leaves

US G8ME01 revision0; static contracts, no execution or gameplay claim.
Closes the `btlevtcmd_CheckPhase`, `btlevtcmd_get_turn`, and
`btlevtcmd_onoff_battleflag` rows of
[UNADDRESSED_BINARY_FUNCTIONS.md](UNADDRESSED_BINARY_FUNCTIONS.md).

All three live in `artifacts/reference/behavior/` as `.elf`/`.disasm` pairs
with `evidence.json` fingerprint rows (address/size/sha256/annotated branch
sites) and `us-usage.json` namespace rows (`battle_event_cmd.o`). All three
return 2 and ignore their callees' return values. Callee identities below come
from the `evidence.json` annotations, confirmed against the complete
[integer operand contracts](EVENT_INTEGER_OPERAND_NAMESPACES.md)
(`evtGetValue` 80031260/458, `evtSetValue` 80030cfc/400).

## btlevtcmd_CheckPhase (80113708, 112 bytes, 34 lines)

sha256 `36f0a98938bf80ef…`; 3 annotated sites. Full order:

1. `li 4, 4`; retain entry in r29 (`mr 29, 3`); r5 = word at [entry+24].
2. r3 = word at [r13+7280]; r31 = word at [r5+0] (first event word);
   r30 = word at [r5+4] (second event word).
3. `bl 0x800f89a0` = BattleGetSeq with (r3, 4). The (battle,4) shape matches
   [phase tables](BATTLE_PHASE_TABLES.md) (`BattleGetSeq(B,4)`).
4. `cmplw 30, 3`: unsigned compare of second event word against the returned
   sequence. Signedness is immaterial — only the EQ bit is consumed.
5. If equal: evtSetValue(entry=r29, dest=r31, value=0). If different:
   evtSetValue(entry=r29, dest=r31, value=1).
6. Restore r29, return 2.

Net effect: the destination named by the first event word receives
(second-word == BattleGetSeq(B,4)) ? 0 : 1, routed through evtSetValue's
destination bands (a literal-band destination stores nothing and the call
only computes a discarded value). `us-usage.json`: 0 link sites, 3 data-word
candidates, 79 REL imports.

## btlevtcmd_get_turn (801137b8, 52 bytes, 19 lines)

sha256 `1c93dc2a7230bbdb…`; 1 annotated site. Full order:

1. r4 = word at [entry+24]; r5 = word at [r13+7280].
2. r4 = word at [r4+0] (destination operand via one dereference);
   r5 = halfword at [r5+0] (`lha`).
3. `bl 0x80030cfc` = evtSetValue(entry, dest=r4, value=r5).
4. Return 2; evtSetValue's own return is discarded.

Net effect: the signed halfword at [word@r13+7280]+0 is offered to the
destination named by the dereferenced first event word, under evtSetValue
routing (literal-band destinations store nothing). The meaning of the source
halfword is not established here — [r13+7280]'s identity is ungrounded in
prose (the one existing mention, in BATTLE_MAIN.md, misdescribes r13), so it
is cited positionally only. `us-usage.json`: 0 link sites, 37 REL imports.
Layout: spans 801137b8..801137eb, exactly adjacent to the next body.

## btlevtcmd_onoff_battleflag (801137ec, 96 bytes, 30 lines)

sha256 `3b702cfef061f08e…`; 1 annotated site. Full order:

1. r31 = word at [entry+24]; r4 = word at [r31+0].
2. `bl 0x80031260` = evtGetValue(entry, operand=r4) → resolved integer in r3.
3. `cmpwi 3, 1`; r3 = word at [r31+4] (mask); r4 = word at [r13+7280].
4. If resolved value != 1 (`bf 2` on the cleared EQ bit): load word at
   [r4+3828], `or` with mask, store back (set bits). If == 1: load,
   `andc` with mask, store back (clear bits).
5. Return 2.

Net effect: the mask from the second event word is ORed into (first-word !=
1) or ANDC-cleared from (first-word == 1) the word at [word@r13+7280]+3828.
The first word passes full evtGetValue routing, so an immediate operand is
legal: any immediate other than 1 sets bits, exactly 1 clears them; the
comparison is signed (`cmpwi`). Read-modify-write is a single load/op/store
with no lock. `us-usage.json`: 0 link sites, 1 data-word candidate, 19 REL
imports.

## Not established

The identity of [r13+7280] and [r13+7280]+3828, event-arg provenance, call
cadence, and any gameplay meaning of "phase", "turn", or "battleflag" in
these names. The REL-import counts are not caller censuses.
