# Native selected audience interruption

2026-10-14. The selected-slot adapter supports an optional tap on an actually
published harmful holder. It is a native input adaptation assembled from original
US G8ME01 revision0 operations, not an additional original function or completed
normal-game touch binding. Physical X continues to use the unchanged original
first-eligible query, including good-item holders.

Implementation:
[header](../../../include/battle/battle_audience_selected.hpp),
[source](../../../source/battle/battle_audience_selected.cpp),
[tests](../../../tests/battle_audience_selected_tests.cpp).
This contract complements [original X interruption](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md),
[target cleanup](AUDIENCE_TARGET_SELECTION_AND_NATIVE_INTERRUPTION.md),
[item lifecycle](AUDIENCE_ITEM_CREATION_SELECTION_AND_LIFECYCLE.md) and
[lower-screen publication](../display/BATTLE_DUAL_SCREEN_UI_FIDELITY_AUDIT.md).

## Original source basis

All addresses and record offsets below are hexadecimal. Item-kind numbers are
explicitly decimal. This is static read-only ISO evidence, not Dolphin timing,
raster parity or hardware acceptance.

| Original operation | Source address / size | Used behavior |
| --- | --- | --- |
| BattleAudience_GetItemOn | 801a2d58 / 150 | Existence, actual held index, signed item-state eligibility and selected slot/XYZ cache |
| BattleAudience_SetTarget | 801a2bec / 9c | Selected flag20; other existing state7 holders lose their item and return to state0 |
| BattleCommandAttackAudienceCheck | 8011f524 / cc | Captured battle, battle/Mario blockers, UI End, fresh previous type, type11/state29, light and fresh flags |
| BattleAudienceItemCtrlProcess | 801a4f0c / 958 | Actual harmful-kind classification and damage descriptors |

Item Main loads item kind at801a547c. Its comparisons at801a549c/54a4 distinguish
**236 <= kind < 240**;801a54ac loads1 and801a54b0 calls the presentation-type
setter. The four original damage descriptors occupy8039c880/300 bytes, each c0
bytes. SHA256 is
`b10aa7c23a8d3d4e8aeab030ea0771400131d1a10e17ef319bd86c949ad711e1`.
The whole Item Main body SHA256 is
`5e30b324f7fff2f6ad60bdb9756b2c248407af39f7edc8cfb6aef9c323e6ce82`.
This classification follows actual held-item storage, independently of the
original interruption-penalty quirk which consults item0.

For B=current battle, A=B+2858, member R=A+1bc+slot*134 and item
I=A+f25c+held*48. Original GetItemOn requires Rflags mask1 set and masks102 clear,
a signed half R1c other than-1, and signed I4<=4 or I4==7. It does not add an
actor-state7 requirement or test item flags. The adapter preserves that signed
state comparison, including its negative-value branch. Native chosen-input
identity additionally requires a valid slot0..199, item0..99 and an exact match
between R1c and the requested item incarnation. No invalid index is clamped into
an existing item.

Original physical interruption captures B before Trigger400. It rejects
Bef4 mask40000000 and Mario U104 mask10. SetTarget is followed by real UI End.
Only after End does the source read B1c5c, write that value to B1c60, then store
B1c5c=11 and B171c=29. Light(0,1) precedes the fresh Bef4 read/OR4/store.

## Read-only query and explicit cache publication

`inspectSelectedBattleAudienceHolder` takes an already admitted input identity
and an explicit current live proof. Owner token, actual audience allocation,
actor generation and item generation must agree. The proof must establish
visibility, dangerous eligibility and interruptibility; source item kind is
also checked directly. Actual base/member/item getters must resolve the same
requested allocation. A mismatched global, held item, incarnation or eligibility
returns no holder and performs no writes.

A read-only scan must never overwrite the original selected cache.
`cacheSelectedBattleAudienceHolder` therefore re-inspects its input immediately
before publication; an old inspection result cannot authorize later stores.
It writes, in order:

1. A138bc=the selected slot.
2. A138c0=the then-current R48.
3. A138c4=the then-current R4c.
4. A138c8=the single-precision sum of then-current R50 and original804249cc.

Thus a source-memory change triggered by publishing the slot can affect later
XYZ reads. Cached screen rectangles, approximated seats and visual character
positions never replace these original record coordinates. The chosen-slot
adapter needs no fabricated original privateSP08 output; the selected identity
is explicit native input data.

## Retained targeted entry

`resumeSelectedBattleAudienceInterruption` has no Trigger service. It does not
synthesize physical X or rerun first-eligible GetItemOn, which could select a
different spectator. A fresh invocation retains B and the native input identity,
checks original battle/Mario blockers, re-inspects and publishes the selected
cache, then runs the complete existing SetTarget exactly once.

The cursor retains unavailable Mario, UI End and light operations at their
actual service boundary. An acknowledged Mario query is not repeated; a pending
End does not repeat cache publication or SetTarget. Type stores precede a pending
light call and do not replay. Previous type remains fresh after acknowledged End,
and final flags remain fresh after acknowledged light. The original light helper
is a verified return-only leaf; its native provider may acknowledge that actual
leaf without inventing a lighting effect.

The exact input and service binding must survive a continuation. Wrong input or
service ownership is rejected without consuming the original cursor. A current
lifetime/incarnation mismatch cancels that cursor before further source writes;
earlier acknowledged source effects are retained, with no rollback or targeting
of replacement storage. Once SetTarget commits, visibility/eligibility changes
alone do not restart that selection. Each child provider must retain its own
partial work until it acknowledges the same operation.

## Required native integration owners

The portable adapter remains unbound. A caller must provide all of these actual
owners before admitting an optional tap:

- A completed matching lower-screen publication using the actual scene camera,
  acknowledged audience quads, source allocation and real displayed-frame identity.
- Proven visibility/occlusion and harmful/interruptible state. Source draw flags,
  alpha bytes or rectangular quads alone do not establish touched-pixel visibility.
- Actor/item incarnation generations tied to real creation, deletion, allocation
  and cleanup. A seat index, position hash or frame observation cannot invent them.
- Successful one-shot consumption of that displayed-frame input, with the same
  owner/allocation/target identity. A completed entry must not be called again
  with a reused touch edge.
- The actual admitted source menu-interruption call site and command context.
  This adapter does not create input readiness, a player turn, population or an
  AudienceAttack root.
- Real Mario lookup, UI release and class/weapon visibility ownership. Rendering
  queues data; simulation owns these guest mutations and retained service calls.
- The independent original physical-X route. Optional touch never becomes the
  sole means of interrupting a thrower.

The tutorial's fallback camera and empty audience are not replaced with guessed
spectators or source-camera identities. Normal class/weapon/target scheduling,
actual AudienceAttack execution and live throw interception require their own
binding and acceptance evidence.

## Focused verification

Host tests cover harmful-kind boundaries, original signed state/flag admission,
held-index and incarnation mismatches, read-only rejection, source-order cache
reads, actual SetTarget cleanup and retained waits without duplicate guest reads
or writes. They prove that ordinary first-eligible X can choose good slot0 while
an independently admitted targeted input chooses harmful slot5. When both paths
independently choose the same slot, cache/target/type/flag store traces match the
original retained helper, excluding its private query output. These records are
explicit fixture memory, not a grant of story population or an end-to-end battle.

Required full builds, native runtime fixtures and any future pixel/input
acceptance belong in [PROGRESS](../../PROGRESS.md). Focused compilation alone
does not prove those broader outcomes.
