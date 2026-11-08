# Actual field battle camera producer receipt

`BattleSequenceBinding::battleCameraReceipt()` publishes the existing
`HarborBattleCameraReceipt` only after original `battle_init800738ac` phase 4
completes its `8000e95c(4,2)` child. The actual setter changes the existing shared
camera 4 mode to battle mode 2; its lens, eye, matrices and publication history
remain unchanged. This receipt is separate from source BattleWork camera Init,
CameraMain completion, viewport publication and GPU admission. See the
[field initialization contract](NATIVE_FIELD_BATTLE_INITIALIZATION_AND_SHARED_STORAGE.md)
and [actual source display pass](../display/HARBOR_SOURCE_DISPLAY_PASS.md).

Each completed producer call creates a distinct native epoch. The binding alone
owns its current epoch. Published values retain immutable receipt metadata and
the actual shared CameraManager; they cannot keep the borrowed binding, Game or
VM alive. The weak binding registration and epoch are checked before the first
borrowed access. Binding admission then checks the intrinsic Game/VM facade
registrations and actual sequence/NPC/storage ownership. Finally, the receipt
requires the same actual shared CameraManager and battle mode. Passing a copied
manager with equal fields rejects. Equal mode before the source child does not
produce a receipt. Missing CameraManager holds at the original child, without
publishing a receipt or running later pool allocation.

Observed owner loss, source-mode change, native restore/replacement, or
binding/Game/VM death permanently retires that receipt. Restoring equal fields
or the previous pointer cannot revive an observed stale epoch. A new completed
actual producer is required. A mismatched manager argument alone rejects that
query without poisoning an otherwise current receipt. Later holds in animation,
pool construction, or lower BattleInit preserve the completed source setter and
same receipt; retrying them does not mint another epoch or repeat the mode write.
Original event-manager ReInit preserves the native VM facade and this history.
All lifetime mutation and receipt queries use the pool adoption contract's
single-threaded, quiescent native ownership policy.

The native display owner must consume this getter at the real field/battle
composition boundary, using the same shared manager. It still must execute and
retain genuine CameraMain/Draw, current queue/pass/allocation admission and the
actual GPU providers. This increment does not activate a normal encounter or
acknowledge any missing field, animation, menu, or renderer child.

Tests run actual source Init and FieldInitialize from an explicitly TEST-owned
parent dispatcher position. They prove a mode-only equality has no receipt,
missing manager holds before pool writes, the actual resumed setter preserves
lens/history, later child waits reuse the receipt, wrong equal managers reject,
and original event ReInit preserves it. Native copy, restore, camera replacement,
mode mutation, destruction and retained-publication teardown exercise lifetime
rejection. They are not an authored opening-to-Crump execution trace.
