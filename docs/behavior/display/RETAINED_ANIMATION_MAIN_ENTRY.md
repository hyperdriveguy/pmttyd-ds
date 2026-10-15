# Retained animation Main entry and evaluation boundary

US G8ME01 disc 0 revision 0. `advanceAnimationMain` implements original Main
entry `8003ed4c..8003ee04`, a retained evaluation child for the body beginning
`8003ee08`, and the existing [optional facing tail](RETAINED_ANIMATION_PAPER_FACING.md).
This is a composition boundary, not a claim that every evaluation domain or
normal-game NPC rendering has been connected. The complete original Main
`8003ed2c+1940` is pinned by SHA256
`9083ebe5e58f3c2bd56914adfa318296291b5ca0ddc779d2bca7e6e3cbeebd26`;
`tools/verify_animation_main.py` checks the source body and dependencies.

## Captures and ordering

The controller reads the actual source word at `80415dc0` to capture W, then
W+10 (pool B) and W+0 (model cache J). It computes R=B+handle*170 with original
32-bit wrap, reads R+10 and R+14, resolves J+(resourceIndex<<4)+8 to E, E+a0 to
H, H+0 to MODEL D, and D+1ac to the descriptor directory. It captures timeline
T at directory+(animationIndex<<6)+3c. No new active, handle, index, pointer or
null admission test precedes those reads.

Only then it requests GQR writes SPR914..917 with values 00040004, 00050005,
00060006 and 00070007, in order. These are observable provider operations;
missing GQR ownership holds even if T is null. An ARM adapter must implement
the corresponding quantized-load semantics rather than silently acknowledging
hardware writes that its evaluator does not honor. GQR save/restore and numeric
parity remain separate platform concerns.

The controller next reads captured W+e8. Nonzero selects captured W+f8/W+fc
output pointers; zero selects captured R+4c/R+54. It reads fresh R+90. Unless
that is ffffffff, it reads linked flags at captured B+linked*170+4, calls the
linked Main child only when mask 2 is clear, and retains that child until it
completes. There is no local cycle filter, recursion limit or once-per-tick
filter. The recursive owner remains responsible for its real continuation and
lifetime; the controller does not replace recursion with a successful no-op.

Only after the linked branch does captured T==0 exit. That exit skips both
parent evaluation and facing. Otherwise the evaluation child receives captured
W, B, R, D, T, position and normal output pointers. Those captures are not
refreshed after linked mutations or held children. The complete evaluation body
must honor them at its original use points. On successful evaluation the parent
advances permanently to the facing child; a later unavailable camera, source
word or loop yield cannot repeat evaluation or earlier platform effects.

## Provider and continuation contracts

Word/GQR empty/false results have no effects. Linked, evaluation and facing
children may return false after retained progress. They must preserve their own
cursors and resume the same invocation; parent false is not an atomic rollback.
The controller pins input handle and all service identities, including the
facing service, across holds. Every source address calculation uses unsigned
32-bit arithmetic, including handle multiplication and descriptor shifts.
Completion clears the cursor. Suspended state is bounded and allocates no heap.

`BattlePosePool::evaluate` is still a synchronous sampling prefix and may
publish source timeline/cache/output state before returning false. Calling it
again after a failure is not a valid generic retained evaluation adapter.
`BattlePosePool::main` remains unchanged and still rejects flag 8 after its
evaluation prefix. The new controller is not yet installed in normal Game.
A production retained evaluation owner, real platform/linked services and
full source buffer/output domains must precede that integration.

## Source-backed paper sampling admission

The source-backed `View::init` no longer rejects parent flag 8. Inspection of
the complete original Main finds its only parent flag load at `80040540`, in
the optional tail after sampling. The existing private-output, unlinked,
owned-file, bounded-resource and tangent/lifetime checks still apply. This
allows the actual source sampling prefix and its readers to operate on a paper
pose; it does not skip the required optional tail or certify complete Main.
No extraction/conversion, art/audio or story content changes are involved.

## Validation and remaining work

Tests pin the explicit ordinary load/GQR/evaluation trace, every primitive hold
across private/global output, linked/skip and null/non-null routes, provider and
input identity, captured pointers across linked mutation, child progress,
missing owners, and modulo handle arithmetic. A cold native process fixture
loads the original Mario resource, compares pose state bytes and decoded output vectors with
flag 8 clear/set, uses actual VM/pool aliases plus a published field camera,
and holds facing after successful evaluation. Changing the pose epoch while
held proves sampling does not run again. A null timeline with an unavailable
facing owner still executes platform setup and exits without sampling/facing.
The fixture GQR/evaluation providers are explicit test adapters, not production
Main or original execution evidence.

Ignored ISO review evidence:
`artifacts/npc-main-parent-evidence/animPoseMain-prelude-review.txt` and
`animPoseMain-prelude-exact-notes.txt`. See the [entry/time source contract](ANIMATION_MAIN_ENTRY_AND_TIME.md)
and open [EVT-01](../../requests/EVT-01.md).
Next bind retained evaluation to the real captured source owners, implement
platform and linked evaluation ownership, compose setter/Inner/draw/queue/GPU
services, and activate source NPC rendering in normal Game. Original field
visual/event chronology, numeric parity and New 2DS XL performance remain open.
