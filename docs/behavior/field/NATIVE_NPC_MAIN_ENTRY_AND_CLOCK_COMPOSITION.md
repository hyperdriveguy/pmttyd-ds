# Retained NPC Main entry and clock composition

Original npcMain80049a7c now has a retained native entry/controller through
80049e8c exclusive, with the complete [clock subsection](NATIVE_PACKED_NPC_CLOCK_SECTION.md)
composed at its original point. The [whole parent map](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md)
and [child ABI audit](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md)
remain authoritative. This controller is unfinished: Movement and FieldTail
are explicit frontiers, never parent completion or permission to skip work.
Normal Game/Area activation still requires the rest of the original parent.

## Entry and source record traversal

Capture GP, read its14 context word, select8041e8b4 or8041e8c8 and retain that
bank even when a later provider changes GP. Request actual marioGetPtr and
clear returned Mario+1dc; preserve radial hint+1d8. Read MainCount8041e8ac,
captured bank count+4, compute wrapped MainCount+1, read bank array+c, then
publish the counter. The native bank admission between capture/publication
has no effects and is not an original callee. Positive counts must fit the
actual owned36/16-slot array; invalid domains hold instead of clamping. Zero
and signed-negative counts do not dereference actors.

The source signed index/count comparison and340-stride traversal are retained.
Each actor's single initial flags read supplies active1, query2 and pose8000.
Initial rejection goes straight to increment; it does not clear state10 or
update clocks/history. Do not replace this walk with activeActors(), a visible
catalog or an all-actor clock prepass. The first admitted actor stops at the
unfinished movement frontier before any later actor is processed.

## Actual pose, state/timer and clocks

Read actor104 handle and request source animPoseGetAnimPosePtr80039cf8. Original
20-byte getter reads global W, its pose-array pointer10 and adds handle*170;
it does not test active flags or clip/resource readiness. Native admission
bounds handle0..255 and requires the existing AnimationPoolMemory to alias the
exact same NPC shared BattlePosePool and VM facade. No replacement W/pose
array is created. Even an inactive admitted pose slot is addressable. Set/clear
actual pose word0 bit100 using captured initial actor8000, not a new actor read
following the getter. Typed pose consumers and VM accesses share those bytes.
Original getter body SHA25652d4ae8f83adb0da10b5a1fdda6d55dc0de62726c5af4ed01d7456c4ceeab90e;
read-only listing/ELF:artifacts/npc-main-entry-evidence/.

Fresh main100000 clear ORs state10. Fresh state10000 then clears10. Unsigned
halfword320 decrements if nonzero. The retained clock controller runs here;
its completed reads/stores/numerics do not replay across parent suspension.
Fresh query2 clear after clocks executes source final state10 clear before
incrementing; unlike initial rejection, the earlier pose/timer/clock effects
are retained. No snapshot or movement is invented for that skip path.

## Special-room branch and source operands

Copy prior state100000 to200000, then freshly clear100000. Compare fresh
GP+12c inline bytes against original kpa_03 at802c1394. On a match, read Y and
literal365. Source rejects only Y>365:365, NaN and negative infinity enter;
positive infinity does not. Entering sets100000, then freshly reads state to
decide the first-transition sound. Sound request800d9848 receives original
SFX_KUPPA_3RD_WATER_FALL1 at802c139c and actual actor+8c alias. Missing native
sound remains pending after completed source stores, without repeating them.

Fresh100000 with10000000 clear loads100 then.5, writes captured100 to1d0 and
retains factor.5. Otherwise load factor1 while leaving1d0 unchanged. Fresh
100000 additionally ORs halfword31e with1; ordinary paths do not clear it.
The next source load at49e8c belongs to movement. The retained factor and
source actor/index/pose/clock ownership remain available at that frontier.

## Suspension, admission and remaining work

Each successful primitive read/store/request advances once. False/empty has no
effects; earlier reads, source arguments and completed mutations stay captured.
Provider identity is checked across retries. The binding borrows registry
mapping, VM and animation aliases, and retains the NPC owner. Their lifetime
must cover the entire parent/cursor; actor recycling/reset/room retirement must
discard that cursor first. Existing animation alias guards must match; a
fabricated pose flag copy or a legacy semantic Mario is not a provider.

Movement frontier49e8c and field-tail frontier4b1d0 remain active on repeated
calls with no new effects. There is no public body-completed acknowledgment or
callback that can falsely skip the missing local body. Extend the controller
with original movement/gravity/side/auto-pose, detection/interrupt, display/
material/shadow/history and field tail, then perform genuine parent completion.
This also preserves ordering before another actor is visited. Normal Game still
uses its separate legacy field path; this implementation does not claim full
original area initialization, source cadence or physical/render parity.

Host regression scope: independent ordinary source trace; every-primitive holds
on ordinary, full-clock Kpa, post-clock query-clear and initially rejected paths;
initial flags across a mutating pose getter; retained bank across context change;
signed counts/counter wrapping; unsigned timer; NaN/height boundaries; prior
special state, factor bypass and audio holds. Actual-storage fixture executes
complete cold animInit with explicit allocation/null-file providers, maps all
source-sized pools, enters an original-data actor and supplies an explicitly
admitted fixture Mario. It verifies typed/VM pose aliasing, inactive source
getter slot, alternate16-slot traversal, missing Mario hold and oversized-bank
rejection. This fixture does not certify production cold file/SDK/Mario startup.
No new scan, per-actor allocation or GPU submission is introduced by the entry
controller. See [progress](../../PROGRESS.md) for exact validation and limitations.
