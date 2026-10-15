# Source file pool, acquisition, release and relocation

Implementation: [pool](../../source/platform/file_pool.cpp),
[acquisition](../../source/platform/file_acquisition.cpp),
[release](../../source/platform/file_handle_release.cpp),
[TPL relocation](../../source/platform/file_texture_relocation.cpp),
[MODEL relocation](../../source/platform/file_model_relocation.cpp).
APIs: [pool](../../include/platform/file_pool.hpp),
[acquisition](../../include/platform/file_acquisition.hpp),
[release](../../include/platform/file_handle_release.hpp),
[TPL](../../include/platform/file_texture_relocation.hpp),
[MODEL](../../include/platform/file_model_relocation.hpp).
Evidence: [acquisition contract](../behavior/file/FILE_ACQUISITION_AND_REFERENCE_REUSE.md),
[pool initialization](../behavior/file/FILE_CACHE_POOL_INITIALIZATION.md),
[MODEL Set](../behavior/file/FILE_TYPE5_NESTED_POINTER_SETUP.md),
[MODEL Clear](../behavior/file/FILE_TYPE5_NESTED_POINTER_CLEAR.md).

These reusable helpers operate on guest addresses through supplied services.
Native readiness is separate from original scalar results: `nullopt` holds;
engaged zero is an acknowledged result. Boolean child services acknowledge void
operations and own any partial child effects. Acquisition/pool cursors are public
mutable state without reentry guards; caller serializes access and keeps their
compared provider identities stable. Resetting a cursor cannot undo guest writes.

Global word `0x80418498` points to the live manager, normally `0x8041ec68`.
Repeated manager reads preserve source ordering, including publication after child
calls; they must not become a single cached manager pointer.

| Object / offset | Reached meaning |
| --- | --- |
| Manager `+0/+4` | Pool pointer / archive mode |
| Manager `+8/+0xc` | Used-chain head / tail |
| Manager `+0x10/+0x14` | Available-chain head / tail |
| Entry byte `0/1`, halfword `2` | State / low8 type / wrapped reference count |
| Entry `+4/+0x20` | Embedded archive payload pointer / filename |
| Entry `+0xa0/+0xa4/+0xac` | Payload-pointer holder / next link / pending gate |

Pool initialization requests heap 0, `0x2c000` bytes = 1024 entries of stride
`0xb0`. Manager pool/mode/used-chain fields publish before clear can hold. Linking
uses 128 groups of eight; the final one-past link is written then cleared after
available head/tail publication. Tail offset is `0x2bf50`. Engaged allocation zero
is not rejected locally: callers must provide the reached valid address graph.

Acquisition captures used head and scans first nonzero-state matching **filename**.
No type comparison participates in a source hit. Pending word `+0xac` causes real
yield requests and rereads of that same retained entry, without timeout or tick
credit. State 3 reactivates and resets count; state 2 reactivates preserving count.
Then all admitted current states increment low16 without clamping, filename/type
update or chain reposition.

Miss reads available head, then garbage(1), then garbage(0), with fresh manager
reads. Source has no final null guard before dereferencing the selected entry.
Capture its next link before loading. Archive mode 0 probes once, 1/2 three times,
others go directly to disc. Engaged archive zero consumes a probe; unavailable
archive does not. Disc open uses mode 2/flags 0; open zero finishes failure.
Length rounds upward to 32 with word wrapping, retaining the original repeated
alignment. Rounded zero closes then fails. Smart header zero has no local guard;
header word 0 supplies destination and `+8` stores entry backlink. Any engaged read
result, including zero, is ignored by source before close.

Archive initialization uses holder `entry+4`; disc uses the smart header. Result
is the **entry**, not payload. Filename copy, pending clear, relocation and list
publication retain their respective source order; failed/held backend work is
not transactional. Publication pops retained next, clears available tail if
needed, then appends to used chain through separately reloaded managers.

Release tests entry, holder address and state 1, without dereferencing/freeing the
holder. It decrements low16 and changes state to 2 only on a fresh exact-zero
count. Underflow does not clamp. Payload collection, deallocation, pointer clear
and event/VM retirement remain separate owners.

TPL Set checks magic `0x0020af30`. Bad signature can hold on the original panic
(filename `0x802f9470`, line 112, format `0x802f947c`); acknowledgement resumes
**after** the check. Relative table `+8` becomes absolute first. Live full-word
count `+4` compares with low16 index, so a count >65535 can wrap indefinitely in
the raw handler. Table pairs contain image/palette descriptor pointers. Data is
at descriptor `+8`; relocated markers are image `+0x23` and palette `+2`.
Fresh table/descriptor reads after stores preserve alias behavior.

MODEL Set uses root first pointer `+0x14c < base` as relative sentinel, then adds
base to all 25 words through `+0x1ac`, including zero/unused values. Signed-positive
count `+0x148` governs 64-byte records; record `+0x3c` points to a child. Child
first pointer `+0x24` compares against child base; all eight words `+0x24..+0x40`
relocate together. Set reloads child after publishing parent pointer. Clear retains
the absolute child before that store, reverses children first and root/table last.
Do not replace these raw live operations with a precomputed validated rewrite.
See [native cache](native-file-cache.md) for its narrower admission domain.
