# Battle pose-table event leaves

US G8ME01 revision0; static contracts, no execution or gameplay claim.
Closes `btlevtcmd_CheckDefinePosetype` and `btlevtcmd_AnimeChangePoseFromTable`
in [UNADDRESSED_BINARY_FUNCTIONS.md](../binary/UNADDRESSED_BINARY_FUNCTIONS.md).

Both resolve an actor through `evtGetValue(arg0)` → `BattleTransID` →
`BattleGetUnitPartsPtr` (0x800f8874, complete /40 in [position
queries](BATTLE_UNIT_PART_POSITION_QUERIES.md)), with callee identities from
`evidence.json` annotations. Both return 2.

## btlevtcmd_CheckDefinePosetype (801062e0, 276 bytes, 75 lines)

sha256 `e2b430ba812b…`. Inputs: arg0 actor, arg1 resolved index word, arg2
resolved value word, arg3 destination word (raw, loaded before dispatch).
After the PartsPtr call, arg2 is resolved and compared as **signed** integers
(`cmpwi`) against band edges; arg3 (dest) is loaded first so it survives:

- value >= 70: `evtSetValue(entry, dest, 0)` immediately, no table scan.
- Else threshold r7 = 28 (value <= 28), 39 (29..39), or 69 (40..69).
- Scan: r6 = word at [part+444]; each step loads [r6]:
  - word >= 70: write 0, done.
  - word == value: write 1, done.
  - word == threshold: write 0, done.
  - else r6 += 8 and repeat while r6 != 0 (`cmplwi 6,0` / `bf`).
- The r6 != 0 back-edge cannot trigger by +8 increments from a nonzero base;
  termination in practice comes from a >= 70 word. The first load has no null
  guard. The `b 0x801063cc` at 80106390 is statically unreachable (the
  preceding `b 0x801063e0` always diverts). Writes go through evtSetValue
  destination routing.

## btlevtcmd_AnimeChangePoseFromTable (801064d8, 108 bytes, 33 lines)

sha256 `a3df22081790…`. Order: entry retained in r30; `evtGetValue([block+0])`
→ `BattleTransID` → `evtGetValue([block+4])` → `BattleGetUnitPartsPtr(actor,
resolved)` → `btlDispPoseAnime(part, r4)` (0x801010f4, annotated) → return 2.
No destination write, no evtSetValue, no result test.

Register hazard, stated exactly: r4 is **not** reloaded after the PartsPtr
call, so the second argument to `btlDispPoseAnime` is whatever the callee
left in r4 under the PPC volatile-register convention — the resolved word is
not re-established. Contrast the documented single-argument call shape
`btlDispPoseAnime(P)` in [recovery events](BATTLE_RECOVERY_EVENTS.md) and
[status infliction](BATTLE_STATUS_INFLICTION.md): only the part pointer is
provably delivered here.

`us-usage.json` (namespace `battle_event_cmd.o`): CheckDefinePosetype 3
data-word candidates, 0 REL imports; AnimeChangePoseFromTable 24 data-word
candidates, 373 REL imports. Both fingerprinted.

## Not established

Table contents/length at [part+444], index/value/pose-type meanings, call
cadence, and any visual outcome. The unreachable branch and the never-false
null test are reported as observed code facts.
