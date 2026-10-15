# Retained Phase and Move controllers

[The implementation](../../source/battle/battle_phase_move_continuation.cpp)
separates native continuation steps from original sequence channels. Channel 4
holds Phase, 5 Move, 6 Act and 7 conclusion. Encoded high bytes 0x04000000 through
0x07000000 are source state domains. They are not tick counts or render frames.
The caller owns actual memory/event/unit lifetime; fixed callback identities alone
do not pin those allocations. Missing children hold; engaged scalar zero is a
completed source result and may end the current invocation.

Phase captures its dispatch gate once. State 0x04000000 increments channel 4 and
resets Move. States 1..5 in that domain own an actual retained Move child. State
0x04000006 increments channel 3 and resets Phase; others do nothing. A held Move
does not retest the Phase gate.

Move subtracts 0x05000000 using unsigned arithmetic and handles indices 0..14.
Out-of-range completes locally. The
[source state table](../behavior/battle/RETAINED_PHASE_AND_MOVE_CONTROLLERS.md)
records every immediate redispatch versus return. Notable boundaries are:

- State 4 scans all 64 registry slots for nonnull units with +0x294 phase scripts,
  launching each with selector 1 before redispatch.
- State 6 scans 64 eight-byte B+0x220 event pairs, launches one selector-0 +0x298
  script, marks only its ID word -1, increments Move and returns.
- State 8 clears exact per-action bytes/halves/words, retaining U+0x27c mask
  0x80002000. It captures signed actor state before status 27, then commits the ID
  and freshly looks up the actor for selection bit 0x1000. It returns with Move 9;
  Act runs on a later invocation.
- State 10 checks conclusion before cleanup. Phase-end zero without forced flag
  0x2000 returns Move 8; otherwise the per-phase audience child precedes increment.
- State 11 tests only the low reaction byte before its barrier. State 12 runs both
  MsgMain and AnnounceMain, even if the first returns nonzero.

The all-skipped selection and Majinai scalar-zero event paths retain original
unchecked dereferences. Native bindings must admit their real memory/actor domains;
no invented no-actor return or fake event is supplied. Mario command availability
uses independent 200-entry signed-halfword pouch scans at +0x38a for badge IDs
240..246 and 247..253, plus signed boot/hammer levels. Source state captured before
a status child remains captured, while later registry/flag reads stay fresh.

Children own real partial effects; completed siblings never replay due to a later
hold. Explicit reset abandons native metadata without original cleanup or rollback.
Only the simulation owner starts the next completed source invocation.
