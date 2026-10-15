# Original audience attack event children

The retained event adapter implements `_attack_aud` (`801465e0/e4`) and
`_get_attack_aud_no` (`801466c4/ec`). These are actual callees of original
Mario audience scripts: Jump calls the getter at `803733cc` and attack at
`8037350c`; Hammer calls them at `8037381c` and `80373b48`. Both attack sites
pass mode0. Original scripts/data remain unchanged in the private DOL pack.

## Cached chosen holder, not another first-eligible scan

Both events call **GetItemOn2 (`801a2c88/d0`)**, not GetItemOn (`801a2d58`).
GetItemOn2 captures audience A, reads cached slot `A138bc`, independently
resolves that member using fresh global BattleWork, then rereads cached slot
and copies cached XYZ `A138c0/c4/c8`. Its item-kind output is null here, so no
held-index or kind reads occur. Typed private locals represent these original
stack outputs without inventing a mapped guest stack or changing guest storage.
No source cache is rewritten by either event's getter.

The number getter captures all four raw output destinations before querying.
Exact sentinel `-250000000` skips that output, including conversion. Other
slot output is integer; X/Y/Z use the actual platform truncation provider in
original order and write through real EVT value services. A conversion wait
does not repeat a completed earlier output or query source coordinates again.

## Attack ordering and real interruption

Attack captures and evaluates its first two arguments in order. GetItemOn2
then overwrites the private slot/XYZ. Add original float32 (`80422fac`) to local
Y. Only mode exactly1 skips effect/audio; every other signed mode requests
`effHitEntry(0,3,x,y,z,1)` with original float1 (`80422fb0`).

After that child completes, GetPosition (`801a3018/4c`) independently resolves
the captured selected slot through fresh global BattleWork and reloads actual
member `48/4c/50`. Thus sound uses the member's fresh position, without the
local effect's +32Y. Mode other than1 starts original positional key
`802f429c` (`SFX_BTL_DAMAGED_PLIABLE1`), then calls `psndSFX_pit(handle,1023)`.
A completed sound handle **-1 is a legitimate source result**, not unavailable.
Unavailable is represented separately by `nullopt`. Effect providers similarly
acknowledge completed original calls even when the original allocation failed;
this caller ignores the effect's scalar result.

Only after those children complete does BattleAudience_Attack (`801a2bc0/2c`)
resolve that slot using a fresh global and OR actor flags40. No item is cleared
at this event site. The original holding/damage controller consumes flags40,
clears the actual held item and invokes its existing lifecycle observer. Both
physical first-eligible X and a genuinely admitted chosen-slot tap therefore
retain the original selected cache and subsequent source attack mechanism.

## Pitch and native registry ownership

`setQueuedBattleAudienceAttackPitch` implements exact source pitch branches:
handle-1 and empty slot are completed no-ops; occupied queued mask1 narrows to
U16 pitch. Source does not check generation here. Out-of-bank native indexes
and live flags-clear descriptor/engine requests stay unavailable. Hardware
mixing is not required to acknowledge an already completed queued pitch store.

`BattleAudienceAttackEvents` binds both actual function addresses in a real
EventCallbackRegistry. It owns cursors by EventEntry address, real lifetime/id,
command and function. Each invocation snapshots native providers, uses actual
VM actor/value services and platform conversion, and retains operands through
waits. Reentry refuses progress. The backend/VM/providers must survive pending
entries; native event retirement and provider resource cancellation remain
caller responsibilities. No event root, menu readiness or physical input is
manufactured by this adapter.

The supplied narrow Trio integration journal installs the owned backend in the
actual VM registry and binds existing positional sound allocation plus genuine
queued pitch. The hit child stays explicitly unavailable: generic EffectPool
exists, but `effHitEntry` is a wrapper around **effHitN64Entry (`801c6344/270`)**,
whose actual constructor/update/resources have no native implementation yet.
Appending a private damage-notice vector is not its completion. Therefore the
real mode0 scripts still stop before flag40 until that effect provider is bound.
Mode1 completes without effect/audio exactly as source, but neither current
Mario script attack site passes mode1. This is not a claim of playable throw
prevention or lower-screen touch activation.

## Source fingerprints and validation

Read-only US G8ME01 rev0 source bodies:

| Body | SHA256 |
| --- | --- |
| _attack_aud | e8cea688eda1ddadee993f969a0012e0afdc5b2278daae388f6815cf16ca63db |
| _get_attack_aud_no | 743557c7b66826d22f3f5532e0a0d0c958c047281e9f639f0b692146a33e0c38 |
| GetItemOn2 | 39c94fc0d1020abd49331f5476fcb7854e762d478cfbf606b6efb6a99c69757d |
| GetPosition | f10db80b10fca798cbc24a6b83df63f8796b52913e7404e1b4b54d49c6290cf6 |
| Attack | a85aacf96e19d75c997ed49fe94980247b1cd3f6b1024f3795617a8a3c3a2c27 |
| psndSFX_pit | d3b349ba74f742d153191b5dde47b824bd43db843ae8a79a1edc49daec2b9ef8 |

Tests cover every effect/sound/pitch boundary held twice, source cache/local
capture, fresh post-effect member position, source flag40 last, mode1 and all
sentinels, signed truncation and prior-output retention, observer mismatch,
recursive admission, pitch no-op/queued/live-engine domains, and actual VM
registry/local-value stores. Original Jump/Hammer event hashes were independently
recovered from the read-only ISO; no dialogue, story conditions, or assets change.
Related: [selected interruption](NATIVE_SELECTED_AUDIENCE_INTERRUPTION.md),
[type11 selection](RETAINED_ATTACK_SELECTION_AND_AUDIENCE_ADMISSION.md).
