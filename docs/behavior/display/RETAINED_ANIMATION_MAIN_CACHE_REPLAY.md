# Retained original Main cumulative cache replay

Implements the complete nonnull Main region `8003f0f4..80040084`, composed
with [scalar evaluation](RETAINED_ANIMATION_MAIN_TIMELINE.md) through the
`advanceAnimationMainCache` owner. This is a reusable animation service, not
full Main completion or activation of the normal Game NPC path. EVT-01 stays
open. Interpolation `80040084..80040540`, final publications, linked/platform
owners and GPU integration remain future work.

## Source evidence

Read-only US ISO region `8003f0f4+f90` SHA256
`deddfdb5cd40f62358936c795006712e2d7e951a0040a47b881e6c1e8e5099cb`.
Original word-copy `80005634+20` SHA256
`6a96905f372a9bc9bc6e23382c850a53d8b26d550d048de3054b9b129c8249c7`;
byte-copy `8000519c+50` SHA256
`2ae30d938e412adf2364d7d2c9768f1b36c4b0bcefaf934345ec62b428b2a046`.
Scale `8041fbac+4`, bits `3d800000`, SHA256
`2625276c57610c65a55868a40c431d1fd6651984b8ee92c081b49a98a0e9d884`.
Private listings and read-only review remain under ignored
`artifacts/npc-main-parent-evidence/animation-main-cache-replay-*`.
No PPC listing or original asset is committed.

## Entry, copies and ranges

Capture current R28, old R3c, next R2c, fraction R30, then MODEL counts
Df0/f8/13c/140/12c in source order. Dispatch only after all count reads.
Same-key fresh R40 greater than captured fraction loads fresh ZERO and stores
R40; unordered comparisons do not store. No key publication occurs here.

Rebuild captures T28 and its unsigned pointer range before any base copy.
For each captured nonzero count, freshly read destination before source:
R48/D154 (12 bytes per position), R50/D15c (12 per auxiliary), R58/D1a0
(byte), R60/D1a4 (4 per node), R68/D190 (24 per texture). Lengths wrap
modulo32. Word copies run forward with original do/while count `n>>2`;
zero executes the first body then wraps the counter. Byte copies return for
zero; unsigned source<destination chooses backward even for disjoint spans.
Loaded copy values survive held stores. Fresh R3c after copies is set to zero
only if still ffffffff. Then load scale even when the key range is empty.
Forward replay captures fresh T28 and scale, with old+1/current+1 modulo32
endpoints. Both paths use unsigned pointer termination, not sanitized indices.

## Five streams and observable ordering

Every key reads all five channel offset/count/base/current-cache headers,
even for zero MODEL counts or empty streams. Key offsets 4/c/1c/24/14 map
to T2c/30/38/3c/34, R48/50/58/60/68, widths 4/4/2/4/12.
All pointer arithmetic wraps32; no sparse-index/count clamp is invented.
Derived loop counts follow the original unsigned arithmetic. Texture-derived
count zero after an accepted range enters the original CTR-zero remainder:
2^32 entries, cooperatively retained, rather than empty success.

- Position and auxiliary: index byte advances cumulative destination; capture
  all three GQR4 deltas before any destination float read. Each lane reads old,
  computes single-precision FMA with scale as first operand, then stores.
- Byte: index then increment bytes, old destination byte, modulo256 store.
- Node: index, quantized delta, old destination float, FMA and store.
- Texture: index/increment, destination byte/store; destination+4 before
  source+4/add/store. At +8 the first three entries of each bulk4 read
  destination before source; fourth bulk and every remainder read source
  before destination. Float additions are unscaled, not FMA. Earlier stores
  may alias later source loads, so no predecode/reorder is allowed.

Destination indices accumulate within each channel and reset at each key.
Repeated indices apply repeatedly. Bulk4/bulk8 versus remainder position is
retained. The result exposes captured current/next/fraction/counts only after
completion. Final R3c=current and R40=fraction belong after interpolation at
80040538/8004053c and are deliberately absent from this child.

## Ownership, holds and native execution

Every unavailable primitive has no effects. Controller false may follow
successful writes or a scheduling yield. Retain identical input, service
identities, cursor and producer pass. Copy/entry/key yields have no total
iteration cap; yielding is a native scheduling choice, not measured game
behavior. The composed owner never replays completed scalar evaluation when
replay holds. Fixed-size cursors allocate nothing per invocation.

The live binding retains the shared pool and borrows the actual VM, aliases
and platform quantization thunk. Every primitive checks current ownership.
No detached fallback is supplied. Quantized loads require actual GQR4
00060006; missing/wrong state holds at that boundary, while earlier ordinary
reads/copies may already have progressed. Raw-word VM services remain usable;
future synchronous admitted nonalias bulk acceleration must preserve the exact
read/write graph and end raw borrows before callbacks/yields.

## Validation and limits

Trace fixtures cover all dispatches/channels, bulk/remainder sizes, every
primitive hold, input/service identity, copy overlaps/directions, source
mutation/alias ordering, zero model counts and wrapped counters. An actual
cold MarioM_W/17-tick VM/pool fixture compares all five replay caches with
the admitted source-backed sampler; it holds at GQR, mutates epoch, resumes
without scalar replay, and verifies final cache publications remain pending.
The same assertions run in a temporary native ARM harness; exact run results
and artifact hashes are recorded in [progress](../../PROGRESS.md).

Source operation ordering is established locally. PowerPC NaN payload,
exception/alignment behavior, original visual parity and physical New 2DS XL
performance remain unmeasured. A normal startup capture does not validate
field animation or full-game completion.
