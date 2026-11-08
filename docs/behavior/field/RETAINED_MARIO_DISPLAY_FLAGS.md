# Retained original Mario display flags event

`SourceMarioDisplayFlags` implements whole original US
`evt_mario_dispflag_onoff800e75a8/90`. It captures E+18 then Mario through
8041e900, reads raw on/off argument and resolves evtGetValue80031260. Zero
selects clearing and every nonzero word selects setting. Only then it reads and
resolves the raw mask argument. It reads the original captured actor's extra
flags+4 freshly, stores flags|mask or flags&~mask and returns2. A changed global
Mario pointer during either value resolution does not redirect that final store.
Mask0 still executes the complete source read/store; there is no early shortcut.

Root binds actual event/operand memory and canonical actual Mario extra flags.
The real beforebattle script uses(1,2) at9050672c before original jump and(0,2)
at90506778 afterward. This does not acknowledge that intervening original jump
or directly drive native presentation. The real renderer/source consumer must
observe the canonical flags through its own actual owner; no UI-ready mutation
or source MarioMain receipt is inferred.

The actual event pointer/address/ID/command, callbacks/user and full shared
lifetime identity are fixed across inner holds. Source-current is checked
before event identity and every source operation; loss poisons. Reentry/reset
while held reject without effects. Missing services hold at their first actual
use. False stores have no effects, and value children retain their partial work
in their own owners. Services are stationary during each single-thread callback.

Independent immediate references compare full traces with each external boundary
held twice, zero/positive/negative on values, zero/full/high-bit masks, late
source-flag/global-pointer mutations, identity replacement and lifetime loss.
Strict host, ASan/UBSan and ARM syntax pass; a read-only ISO verifier pins the
complete event and GetPtr bodies. Root owns actual Game binding/native field
execution. No Game/main/frozen/shared-file change is included.
