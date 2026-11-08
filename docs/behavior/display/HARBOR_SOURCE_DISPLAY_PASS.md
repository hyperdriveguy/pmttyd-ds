# Harbor retained source display pass

`HarborSourceDisplayPass` composes existing original source controllers. It is
not the original complete BattleDrawMain (`801024f0`) or a replacement scheduler.
It never samples input, advances a simulation tick, imports a GPU resource,
sets a camera mode, writes a menu readiness byte or acknowledges an opaque
render request. Original physical command input stays in the source Input owner.

## Actual ownership and source boundary

The binding pins the actual shared `CameraManager`, the captured source battle
address/allocation generation and a weak actual tutorial runtime. Each operation
locks that runtime before invoking any borrowed provider. The allocation predicate
must verify the actual source-global battle pointer, current battle allocation,
VM and all provider context lifetimes. Do not capture a public facade by raw
pointer in callbacks which can survive its destruction.

A `HarborBattleCameraReceipt` must come from the **completed** original
`8000e95c(4,2)` child reached in `initializeFieldBattle` (`800738ac`). Its independent
strong lifetime and pure predicate must identify that same actual camera bank
and still-current field/battle ownership. Camera4's numeric mode alone proves
nothing. The later `80041c58` child initializes animation, not camera4. Original
`initializeBattleCamera` writes source battle camera work; it is also not this
receipt. A missing receipt holds without effects. `admitCameraReceipt` permits
one late receipt, outside an active provider; a bound receipt cannot be replaced.
Receipt/process loss after admission invalidates this owner permanently.

The pure current callbacks must not mutate their providers or camera bank.
Provider objects and function identities are fixed at construction. Source
children which return unavailable retain their own partial work and captured
outputs; this parent never retries a different synchronous implementation.

## Caller sequence

1. At an admitted actual source display pass, call `begin` with a nonzero,
   strictly increasing native pass identity. This identity is not guest memory
   or a simulation timestamp.
2. At the **original CommandDisplay child**, call `registerCommands`. Its original
   retained footer cursor and palette effects execute once. This child must call
   `registerBattleCommandDisplay` using the captured current battle address.
3. Run every other reached original registration child against
   `registrationQueue`, preserving their original source order and private
   continuations. No consumer can retain permission to mutate this queue after
   sealing. Registration of command records does not complete BattleDrawMain.
4. Only after all original registration children complete, call
   `sealRegistrations` and then `resume`. Real `CameraManager::main(false,...)`
   completes before real `CameraManager::draw`. Missing camera callback/math/GX
   providers hold exactly where reached. Successful CameraMain is not inferred
   from `viewPublished` or an inactive default cursor.
5. Keep this owner, actual VM, bank, queue and all resource providers fixed while
   Main/Draw holds. Do not run another source tick or reset/replace the queue.
   Source CameraDraw resets its queue **last**, after all actual callbacks and
   dummy draws complete. Only then may a newer pass collect registrations.

`cameraMainCurrent({generation,pass})` proves this owner's successful complete
Main while that exact Draw pass is active. Non-HP callbacks receive the actual
Main/Draw/queue references and source callback identity. Existing Class/Weapon
native Frames must independently enforce their exact `8012210c` / `80121ce8`
callback, actual UI allocation/resources and source camera admission. The display
generation identifies the battle/camera producer; a native Frame may use a
different generation for each actual UI W allocation. Its producer predicate
must validate that captured W generation separately, then query
`cameraMainCurrent` with the captured **display** generation and same pass.
Changing W does not restart a held CameraMain/Draw owner. The
borrowed invocation structure must not escape the call. Their GPU submission
holds must retain this whole pass; no rebuilding a source prefix after false.

## HP precedes menu drawing

Original `registerBattleCommandDisplay` always enters HP `8011fde0`, camera8,
class0, priority902, before menu callbacks at priority500. Original sort keys
therefore dispatch HP before the radial/weapon/target menu. This owner preserves
that order and delegates HP to whole retained `drawBattleEnemyHp`.

That body first queries **camera8**, before reading any of its64 source registry
slots. Each non-null slot checks status27, reloads the actual slot, then invokes
whole retained `checkBattleMonosiri` (`8012ee3c`). The checker reads real global B,
kind, force flag, actual saved-switch query, fresh unit flags and independent
Monosiri bank words in original order. This composition uses the same optional
VM word provider for HP and the checker. `binding.monosiri` supplies the actual
saved-switch provider/context; its word field is unused.

A genuine false Monosiri result skips that unit's position, number and bar work.
Consequently an actual wholly untattled population can finish HP without GPU
providers. Unknown switches/bank/status/memory hold; they are never assumed zero
or untattled. A true result continues the original body, retaining its source
reads and every number/bar/math boundary. Missing reached GPU services hold and
prevent the later class/weapon callback. Native top HUD rendering cannot
acknowledge this source child.

## Validation and remaining integration

The focused fixture compares accepted ordered calls/reads with a direct
composition of the existing original registration, camera and HP controllers.
It holds every external provider twice, checks no duplicate successful children,
HP-before-class, missing words, genuine untattled/no-GPU completion, true
Monosiri/GPU holds, queue replacement, reentry, runtime destruction and receipt
loss/late admission. Fixture camera/resource providers are explicitly TEST
inputs. They do not demonstrate actual original field initialization or native
GPU presentation.

Native integration still needs the real field source receipt, actual CameraMain
battle-frame/math children, real all-registration source callsite, saved-switch
and status providers, source actor/stage/audience callbacks, and actual
Class/Weapon/Target native consumers. Mode3 battle camera interpolation can
mutate source prefixes before an unavailable child; its provider must retain the
whole original action or prove the actual numeric domain before executing it.
Do not generically retry that synchronous function. No normal activation or
complete tutorial claim follows from this module alone.
