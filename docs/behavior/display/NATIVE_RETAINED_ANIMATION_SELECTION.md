# Retained original ordinary animation selection

`selectAnimation` implements the complete local `animPoseSetAnim` 80041170/17c
from the read-only US G8ME01 body. Its source contract is
[selection/restart](ANIMATION_SELECTOR_AND_RESTART.md). It accepts raw handle,
selector and force words; it does not infer pointers from nonzero values or
restrict the portable parent to valid native IDs. Addresses/multiplications wrap
to low32; counts and selector branches use signed comparisons. Negative selectors
call the original strcmp child interface; nonnegative selectors are indices.
The first equal name wins; exhaustion/out-of-range selects row0, even for an
empty/negative count. The captured count is not reloaded during the scan.

Each reached directory lookup reloads MODEL+1ac before calling strcmp. R and MODEL
are captured before scanning. Force0 alone reads R14; equality returns before any
selection stores, clock requests or literal loads. A nonzero force never reads
R14. The changed/forced path stores R14, reads R90, and only a non-minus-one link
reads current R0. Mask2 admits OR4 of those captured flags. No linked-record
validation or traversal is invented.

The clock trace is mode, hardware bus word, GP pointer, captured GP high/low,
then the original signed 64-bit division request. Timestamp LOW is stored first;
the zero literal at8041fb28 is loaded once; timestamp HIGH follows, then cache
index−1, fraction=captured zero, discrete word0, ratio=same captured zero. These
boundaries remain distinct even when literal/backing storage changes between
resumes. Portable float-literal load/store uses retained bits in the existing
native scalar domain; original FPSCR/signaling-NaN behavior is not emulated.

A cursor retains each acknowledged primitive and child result. Failures may
follow source stores; they do not roll back or restart the function. Resume the
same inputs, service identities and exclusive producer lifetime. Child providers
own partial child continuations. Wrong inputs/services hold without new effects.
A visit budget is native scheduling policy, not an original instruction/time
measurement. The setter does not invoke Main, evaluate geometry, alter rate or
implement the paper/local-time setters.

## Native process and Mario integration

`AnimationStartupProcess::setAnimation` owns one `AnimationNativeSelection` and
pins slot generation, resource, scratch owner and actual VM/alias identity.
Fresh ready/header/lifetime checks precede native primitives. Out-of-range native
handles or missing native owners hold; these are explicit port admission rules,
not original checks added to the portable parent. Original words remain live.
The native divide adapter retains the existing signed source clock policy and
holds on an unsupported divisor. Native strcmp is a retained bytewise adapter:
a missing right byte preserves the already captured left byte; missing storage
or exhausted per-call work holds. Its 256-byte work budget does not limit string
length. This is not a claim of SDK strcmp's original optimized word-load order.
No native pointer is serialized into guest memory.

Selection is a producer barrier: Entry, raw Main, publication and cold advance
refuse overlap; native field tick/release and Game's existing barrier observe it.
A reserved same-handle publication at phase0 can admit its own setup selection;
other handles and later phases refuse it. This API has no independent pass token:
the caller must retain exclusive ownership of that reserved setup and may not
inject another setter during a different owner's phase0. Direct public VM/file/
pool changes and raw Release callers retain the existing caller serialization
contract. The selection owner retires before aliases, scratch, VM and pool.

Mario now executes this original setter before Main when an upload changes.
Its existing name-to-index bridge supplies a numeric selector; arbitrary original
NPC selectors can use the raw process API later. A separate setup flag retains
completed selection across local-time adapter holds. The fresh scaled start time
is validated before integer conversion after such a hold. The existing explicit
local-time bridge remains provisional; full source SetLocalTime/paper setters,
legacy typed pool selector routing, NPC draw activation and original scheduling
are not completed by this change.

## Evidence and checks

The read-only source verifier pins the five setter bodies and their census.
An independent instruction audit recovered the directory reload, bus-before-GP
and interleaved literal-load details now reflected in the source contract.
`animation_selection_tests` checks the exact acknowledged primitive trace and
inserts a hold at every boundary; checks signed/count/fallback/force domains,
raw handle wrapping, captured count, fresh directory/literal values, service
identity, both clock branches and untouched early-return state. Cold original
Mario resources exercise actual name-pointer selection, a partially held strcmp,
native lifecycle/identity exclusion, absent-clock equality, clock repair, and
Mario's asynchronous setup/no-replay path. Existing publication/Game tests remain
required. Exact validation, assets and current performance limits are in
[progress](../../PROGRESS.md). EVT-01 remains open.
