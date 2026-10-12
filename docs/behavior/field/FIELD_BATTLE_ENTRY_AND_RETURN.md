# NPC battle setup, field entry and return protocol

US G8ME01 revision0, recovered2026-10-12. Evidence B: ten complete DOL bodies
listed below; original harbor caller records and configuration data. No original
execution, transition timing or tutorial equivalence is claimed. This connects
[harbor scene bindings](../story/STORY_HARBOR_OPENING_BINDINGS.md) to
[the actual encounter data](../story/HARBOR_TUTORIAL_CONFIGURATION.md).

| Function | US address | Size |
| --- | --- | --- |
| evt_npc_set_battle_info | 0x8004f090 | 0x64 |
| npcSetBattleInfo | 0x80043d80 | 0xf8 |
| npcSetupBattleInfo | 0x80043e78 | 0x280 |
| evt_npc_battle_start | 0x8004ef54 | 0xb8 |
| evt_npc_wait_battle_end | 0x8004eed8 | 0x7c |
| fbatGetPointer | 0x8004829c | 0xc |
| BattleInformationSetResult | 0x80114508 | 8 |
| BattleInformationGetResult | 0x80114500 | 8 |
| areaDataPtr | 0x800391e0 | 0x98 |
| relSetBtlAddr | 0x80038fd8 | 0xb0 |

## Area registration and NPC configuration index

The area registry begins at0x803057e4; these helpers traverse its pointer array
starting at registry+8, until a null pointer. Each pointed record's word0 is a
name string. areaDataPtr compares that string using strcmp and returns the first
matching record, or null. relSetBtlAddr(name,setupTable,numberTable) uses the same
search; when found, stores setupTable at record+8 and numberTable at+0xc.
No missing-name allocation, case folding or fallback is performed.

The event setter resolves NPC operand0, then setup index operand1 through
 evtGetValue; resolves the NPC through evtNpcNameToPtr and calls
npcSetBattleInfo(npc,index). Return handler2. It does not have a first-call gate.

npcSetBattleInfo receives an NPC pointer and signed index. Index-1 supplies a
null configuration to npcSetupBattleInfo. Otherwise copy the current gp map
name's first three bytes at+0x12c..12e into a local NUL-terminated area name;
call areaDataPtr, then load its+8 setup table **without a null check**. Positive
indices advance by0x44 per record (loop unrolled into groups of eight). Index0
and negative indices other than-1 use the table's first record. There is no index
bound check or lookup through the numberTable for this numeric path.

After setup, copy the NPC's200-byte information block at+0x230 to each nonnull
associated NPC pointer in the four slots+0x330,+0x334,+0x338,+0x33c. The copy
uses25 iterations of two words, beginning at+0x230. The association's game
meaning/lifetime is separate evidence; do not label it a generic battle party
without its producer/consumer. It does not copy the NPC's separate byte+0x316.

## Information block initialization and audience draws

Let B=npc+0x230 and C=selected configuration. Clear200 bytes at B, then clear
32 bytes at B+0x7c again. If C is null or its first word is null, return after
these clears. Other NPC fields/flags are unchanged on that early route.

Store C+0x1c at B+0xac. If its signed value is1 or2, skip audience initialization;
otherwise process16 byte pairs beginning C+0x20. For each pair, compute width
as sign_extend(second byte)-sign_extend(first byte)+1; request irand(width); add the
original **unsigned** first byte and store low8 of the sum at B+0xb0+i. Equal
bounds still request irand(1). Invalid/inverted bounds are not checked locally.
This signed-width/unsigned-add distinction must be preserved for high-byte data.

Set B+4, +0xc, +0x10, +0x14 to0; copy C+8 to B+8 and C+0x40 to B+0xc0.
The load of C+0x18 is overwritten before use in this body; do not infer that it
is installed into the information block here. Other consumers may use that
configuration field separately.

## Group selection and per-unit item selection

For the configuration's C+0xc word, use a saved-switch query only when its signed
value falls inclusively within0xf8405b80..0xf8d9f200. Translate the operand by
adding130000000 and call swGet. Nonzero selects the weighted-list pointer C+0x10;
zero selects C+0x14. A word outside the encoded range selects C+0x14 directly.
This is a dedicated encoded-switch path, not an event-variable resolver.

Clear B+0x18. If the list is nonnull, sum positive signed weights at entry+0,
advancing12 bytes until a nonpositive weight. When total>0, draw irand(total),
then subtract successive weights until the result is negative. Store that chosen
entry's+4 group pointer at B+0x18 and its+8 stage pointer at B+0. A null list or
nonpositive total leaves them cleared. No local list bound,overflow protection
or independent normalization is established.

If a group was selected, read its signed unit count at+0 and unit-array pointer
at+4, then clear32 bytes at B+0x1c. Iterate count units with stride0x30. For each
nonnull item-table pointer at unit+0x2c:

- Sum signed halfword weights at entry+4. Continue through an entry when weight>0
  or its word0 item ID is nonzero; stop only when both tests fail. Advance8 bytes.
- When signed total>0, draw irand(total); subtract each signed halfword weight
  until negative. Store chosen item ID at B+0x1c+4*i and the original table
  pointer at B+0x3c+4*i.
- A zero total makes no draw and leaves the cleared selected item field. The
  selected-table field is written only on actual selection.

There are no local unit-count/output-array bounds. The harbor's concrete count1
and zero item table avoid those unsupported domains. Preserve random draw
consumption even when a list has only one possible group.

Finally clear npc+0x316. If a group exists, follow group+4 to first unit, then
unit+0 to its kind record, and copy kind byte+0x12 into npc+0x316. This assumes a
valid first unit even for malformed zero-unit groups. OR npc word0 with
0x10000000. The flag and byte's later field/battle consumers remain open.

## Battle-start callback and its extra read

Start first resolves NPC operand0 and the word at operand+4, then calls
fbatGetPointer. That getter ignores the incoming argument and returns
`gp->word(+0x168)`, where gp is loaded from0x80415fd0. Resolve the NPC name to
pointer next; use field-battle work F and its information block I=F+0x20.
These lookups occur on both initial and resumed calls.

The harbor start record has only one operand after its callback. The original
second read therefore reaches the **next command header**, word0x0001005b for
wait_battle_end. Its resolved value is discarded by the pointer getter. A
portable supported-caller implementation should not invent a second scene
operand or interpret it as a configuration index. Keep this binary read policy
explicit if implementing instruction-level compatibility.

On first-call flag nonzero:

1. Store the NPC pointer at F+8.
2. Request marioFBattlePrepare and marioCtrlOff.
3. Call marioGetPartyId, then partyGetPtr; nonnull partner requests partyCtrlOff.
4. Store result0 at I+0x10 through BattleInformationSetResult.
5. Request psndSetFlag(0x10), then seqSetSeq(4,null,null).
6. Return event0, blocking this USER_FUNC.

On resumed first-call flag0, return2 without repeating initialization. This
callback itself does not poll battle outcome. The sequence driver owns the
transition and synchronization; see [baseline](../../ORIGINAL_BEHAVIOR.md).
Caller request order is recovered here; preparation/partner/audio service
internals are not equivalent implementations merely because their names exist.

## Field-return wait and result protocol

Wait callback gets F and I, then seqGetSeq. If current sequence is not2, return0.
When it is2, read I+0x10 using BattleInformationGetResult; zero still returns0.
**Any nonzero integer** admits cleanup, not only a victory result. The local
setter/getter simply store/load the full32-bit word; outcome meanings require
battle producers and sequence/loss behavior.

On admitted cleanup, request in order: marioCtrlOn; marioGetPartyId/partyGetPtr;
partyCtrlOn when nonnull; L_psndBGM_stop; marioFBattlePost. Return event2.
There is no first-call gate or clearing of the result word in this routine.
A caller that erroneously invokes it again after completion can repeat cleanup.
The next harbor commands reposition actors/camera and write story value5; this
local wait does not itself prove that defeat or escape ever returns to sequence2.

## Verification and remaining scope

Ten function fingerprints and48 instruction anchors pin lookup/setup,storage,
random selection and transition/poll order. Harbor data/prolog spans and original
relocations are checked separately. No original code is linked or executed.

Remaining: full map registry/module lifetime,first-call event scheduling across
sequence changes,NPC association and cleanup,actual result producers/win-loss-
escape handling,Mario/party/control/audio service internals,and tutorial unit
scripts. Validate original state/input/RNG/transition traces,including re-entry,
result0/nonzero and phase interruption. Local contracts do not close full
field/battle/story equivalence or complete-game requirements.
