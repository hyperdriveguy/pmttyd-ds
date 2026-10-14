# NPC setup ordered installation and retained provider frontier

US G8ME01 disc0 revision0, static read-only source evidence for
[EVT-01](../../requests/EVT-01.md). This specifies `evt_npc_setup8005323c`
(`0x288` bytes); it does not allocate or register actors. Existing actor
registration, resources and source addressable actor storage are prerequisites.
See [ordinary NPC bindings](ROGUEPORT_ORDINARY_NPC_BINDINGS.md) and
[setup scheduling](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md#setup-creates-init-first-regular-after-completion).

Primary complete listing: `artifacts/bero-local-evt_npc_setup.txt`.
Symbols: `artifacts/reference/us_symbols.csv`. Called battle/mode functions
were independently read from the ISO DOL and disassembled through anonymous
inspection ELF memory; no original code was executed.

## Invocation and first phase

At 8005324c capture `first`; capture event args at 3250, read raw operand0
at3254 and resolve with evtGetValue80031260 at3258. Capture resulting table
base. Call fbatGetPointer8004829c at3260 even though its result is unused.
Branch on captured first at3264. Every genuine new invocation repeats this
operand resolution and getter; the source has no persistent setup cursor.

First phase walks records of size0x5c, terminating on fresh row+0==0.
For each name, npcNameToPtr_NoAssert80049934 returns an existing actor;
actual null skips the entire row. A missing native provider is distinct from
actual null. For a present actor, the source order is:

1. Load actor+0, row+4, capture actor+28 descriptor, OR/store actor+0.
2. Load actor+4, row+8, OR/store actor+4.
3. Read row+0c. If nonnull, read actor+118; delete its event if nonzero;
   reread row+0c, create evtEntry(script,0,0), set returned event byte+10=0,
   write event+170=actor, read event+15c ID, store actor+118. Null script
   instead stores actor+118=0 without deleting any previous ID.
4. Copy row metadata in the exact order below.
5. Read row+58 and call npcSetBattleInfo80043d80(actor,value).
6. Read the *captured* descriptor+10; if nonnull strcpy8026d5fc into
   actor+4c. Then fresh descriptor+14, conditionally strcpy into actor+6c.
   Then fresh descriptor+8, conditionally strcpy into actor+2c and call
   animPoseSetAnim80041170(fresh actor+104,actor+2c,1).

These are strcpy calls, not comparisons. Descriptor pointers/strings and
actor pose-buffer capacity need native admission; changing actor+28 during
an injected service must not replace the captured descriptor for this row.

| Source reads | Destination stores, in order |
|---|---|
| row+14 | actor+128 |
| row+18 | actor+12c |
| row+10 | actor+124 |
| row+1c,+20,+24,+28 separately | actor+130,+134,+138,+13c separately |
| row+2c | actor+1f8 |
| row+30 then +34, both before stores | actor+1fc then +200 |
| row+38 | actor+204 |
| row+3c then +40, both before stores | actor+208 then +20c |
| row+44 | actor+210 |
| fresh row+3c then +40, both before stores | actor+214 then +218 |
| fresh row+44 | actor+21c |
| float row+48,+4c,+50,+54 separately | actor+220,+224,+228,+22c separately |

After the terminating name read, fbatChangeMode8004828c(1) freshly reads
NPC-work global, then work+168 pointer, then stores halfword1 at that target+0.
Return0. The getter8004829c likewise reads NPC-work global then work+168.
This mode is not Mario key depth or a fabricated success flag.

## Resume polls and regular phase

For first=false, use the table resolved on *this* invocation. Scan from its
base. Each fresh row name goes through asserting npcNameToPtr800499e0;
read actor+118. If nonzero, call evtCheckID80036604; the first live ID
returns0 immediately, without examining later rows. A native hold midway
through the same invocation must not rescan already consumed names/IDs.
A new source invocation after this genuine return0 starts again at row0.

After the poll scan terminates, restart from the captured table base and
resolve each fresh name again. For each actor read actor+124. Null skips
the row without deleting previous regular ID. Nonnull reads actor+11c,
deletes if nonzero, rereads actor+124, creates evtEntry(script,0,0x20),
writes event byte+10=1 and event+170=actor, then publishes event+15c to
actor+11c. Terminating name ends with return2, no mode setter.

The second scan reads the actor's installed regular script, not row+10.
Original init roots may have changed it. Names, pointers and IDs are fresh
at their stated source reads; snapshotting the complete table or actor
state across true source polls changes dynamic-alias behavior.

## Battle subservice is required even for -1

npcSetBattleInfo80043d80 first compares the captured argument to -1.
For -1 it passes null directly to npcSetupBattleInfo80043e78. That helper
clears 200 bytes at actor+230, then clears32 bytes at actor+2ac (a second
overlapping clear), and returns on null. It does not write actor+316 or
main flag10000000 in this null branch.

The outer service then scans actor child pointers+330,+334,+338,+33c in
that order. For each nonnull child it copies200 bytes actor+230..2f7 to
child+230..2f7 as25 iterations: read source word0, read source word1,
store destination word0, store destination word1. Thus -1 is not a no-op.

Other values obtain the first three bytes of NPC-work+12c (fourth local
byte zero), call areaDataPtr800391e0, read returned+8 and advance positive
indices by0x44 each. npcSetupBattleInfo additionally consumes source battle
tables, saved-switch checks800858b4 and shared RNG80005da0, picks weighted
setup/items, updates actor+316 and main10000000 on its populated branch.
Those dependencies require an actual service or explicit unsupported hold;
this note does not claim the full populated battle helper is implemented.

## Native suspension boundary

The real callback's first/resume distinction is not enough to recover from
an unavailable provider after partial effects. A retained native cursor must
capture owner identity, original first, args/table and current row, then
retain each completed source read/effect until this invocation returns0/2.
Do not delete/recreate init events, OR flags again, repeat battle RNG, or
restart pose changes on a provider retry. A true first=true restart resets
that cursor; retirement removes it. Actual source return0 after first phase
or a live-init poll ends the invocation, so the next invocation must freshly
resolve the table and scan, rather than resume a stale poll snapshot.

Two viable native contracts are complete pre-admission of infallible owners
before effects, or a staged cursor with optional providers whose unavailable
result guarantees no effects. Allocation capacity must be checked after an
actual required deletion, since that deletion may itself free the needed
slot; preserve progress at this boundary. All created events are independent
roots with actor context+170, not children of the setup caller. Shared VM
creation, deletion, source ID checks and retirement services are reusable.
Current finite FieldNpcInstance idle/body/wander objects do not own the full
source descriptor, setup metadata, resource readiness or event IDs and must
not be treated as registration proof.
