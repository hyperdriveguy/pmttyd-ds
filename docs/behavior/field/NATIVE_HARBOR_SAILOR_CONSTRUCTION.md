# Native Harbor sailor construction

This is bounded native story-4/resume construction over an actual field VM and
actual shared native animation process. It supplies real actor births and real
regular-event IDs for the original beforebattle tail. It does not execute whole
field Init, `evt_npc_setup`, either sailor Init event, the four cutscene actors'
Entry child, or the afterbattle event. No field start/wait/result/story command
is acknowledged by this owner.

## Actual source identities

Original US `m/gor_00/s` contains 17 NPC tags. Ordinals 2 and 12 name
`ガラの悪い水夫` and `水夫ボム兵Ｂ`; both use literal `BTLNO_INVALID`. Original
`rel/gor.rel` `npcEnt` is native 905043b4, stride 0x5c. Their rows are 905044c8
and 905047a8, with original regular scripts 90501ca0 and 90503040. The original
beforebattle tail queries those actors with `evt_npc_get_reglid` 8004f894, then
executes its two real event deletions. It does not query Crump's regular ID.

The unchanged tag source is editable at
[gor_00.s](../../../assets/original/harbor-npc-tags/gor_00.s), with adjacent
original-disc provenance and rights notice. The normal `field-npc-tags` recipe
compiles the existing PMFMNPC1 native parsed-input representation; it preserves
Shift-JIS bytes and binary32 numeric source values. It never extracts again over
checked-in edits. Private parsed-record memory is native allocation policy,
not original parser stack identity or a parser/history-gate completion receipt.

## Native constructor and waits

`NativeHarborSailors::prepare` requires the actual same-VM registry binding,
initialized owner/F, field GP mode 0, original immutable script assets and the
same native animation pose pool. It rejects duplicate names and cross-VM binding
before mapping its native parsed inputs. Caller owns VM/registry lifetime and
must destroy this unique constructor first; intrinsic VM registration is checked
before each borrowed VM access. Destroying it does not cancel a held Entry or
pretend to delete an actor. Registry retains unfinished source Entry history.

Each selected row requests its genuine native MODEL/TPL through the existing
animation-group owner, then resumes the real retained registry Entry. Original
model-name bytes are shared into the actual animation process address space.
Successful Entry is never repeated. Once it finishes, a bounded native
constructor policy applies the existing `registerPackedMapNpc` metadata tail
with a typed completed-Entry receipt; the receipt checks exact name/model and
does not allocate another actor. This is not a source instruction/read trace for
whole setupDataBase: its immutable Tribe/model prefix is admitted before Entry
and reread by the existing tail. Source positions, AI/territory words, dimensions,
pose names and battle-configuration clearing come from the actual source tags
and descriptor tables. Missing or changed required inputs hold before that tail.
The supported actual records' -1 battle setup reaches no RNG or saved-flag query.

Finally the native policy stores the actual original regular script pointer and
uses real `createRootEvent` (source `evtEntry` 800374ac), priority 0/flags 0x20.
It records returned ID at actor+0x11c, raw event byte0=1 and field170=actor. This
is a real allocated same-VM context, not an invented ID. Its body is not marked
finished; a missing callback must remain held. Source Init completion is not
inferred or substituted. Event capacity holds retain the completed Entry/tail.

The owner exposes the same `FieldNpcRegularEventHistory` to its actual field
battle binding. Route actual VM retirement snapshots to that history before
source event deletion. A genuinely retired original regular event can then
satisfy the later get_reglid provenance gate. Reset/unload must retire the native
constructor/history first; there is no polling-based NPC incarnation claim.

## Tests and remaining integration

Focused tests use real original NativeFieldAnimation startup, asynchronous file
requests and retained Entry, source tags/Tribes/regular scripts, actual contexts,
source positions/battle clearing, retirement snapshots and cross-VM/mode rejection.
No test acknowledges a missing story child. Host/sanitizer tests use a complete
matching bb6a7c46 core/header graph and coherent feedback RomFS, not larger live
headers with an older archive. ARM syntax covers both new translation units.

The Encounter runtime still needs to own this constructor and the original
four-actor Entry/setup roots before admitting its beforebattle root. Its visual
NPC setters must also reach the actual registry, and case20 must execute actual
NPC battle-info setup. Existing `FieldNpcBattleBinding` can then own original
start/wait/get_reglid with real Mario/party/audio/sequence providers. Completion
must retire real Menu/Nice presentation and Audio, export canonical pouch, then
publish unchanged result 1 only through a matching actual start/return owner.
Original sequence2 plus nonzero information result remain necessary for wait;
this constructor creates neither. The original beforebattle tail and afterbattle
commands must continue with original attribution, IDs, tokens and branches.
