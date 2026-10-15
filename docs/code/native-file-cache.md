# Native shared file backing and common-menu facade

Implementation: [cache](../../source/platform/native_file_cache.cpp),
[API](../../include/platform/native_file_cache.hpp),
[menu facade](../../source/platform/battle_menu_file_cache.cpp),
[facade API](../../include/platform/battle_menu_file_cache.hpp).
Evidence: [shared source/native boundary](../behavior/file/NATIVE_SHARED_ANIMATION_FILE_CACHE.md).
Source leaves are explained in [file services](file-services.md).

One process cache owns actual big-endian manager/pool/holder/payload bytes shared
by every borrowing VM. Source pool initialization, acquisition, relocation and
release operate on those bytes. The native backend supplies staged ROMFS, empty
archives, stable address tokens, simplified smart headers and zero-filled rounded
tails. It does not implement original SDK placement, DVD workers, movement,
eviction or garbage collection. Missing providers/domains remain unavailable.

| Token / range | Native backing policy |
| --- | --- |
| `0x87000000` | 1024-entry pool (`0x2c000` bytes) |
| `0x87100000` | First payload, at most the 1 MiB before headers |
| `0x87200000 + index*32` | Per-file holder, 12 backed bytes in each reserved slot |
| `0x87500000 .. <0x88000000` | Later retained payloads |
| `0x803e6e78` | Shared filename buffer, stage admits path length <128 plus NUL |
| `0x87400000` | Disc adapter token, not original disc-worker storage |

Staging prepares an entire batch before publication, rejects duplicate/NUL/oversize
paths and never canonicalizes names. Maximum 1024 files follows pool capacity;
`0xafffff` is native payload-size admission, not an original file limit. Rounded
payload vectors remain stable after freeze. First successful bind **or admitted
acquisition**, including one that holds, freezes catalog/yield configuration.
Stage everything before then; even binding an empty catalog freezes it.

Native type 0 is raw, 4 TPL, 5 nested MODEL. TPL admission checks table/descriptor
extents, original unset markers, relative data starts and conflicting overlaps;
identical shared descriptors are allowed. Count >65535 is rejected before the
raw low16-index handler can fail to terminate. MODEL admission checks reached
root/table/child write extents and excludes ordinary conflicting aliases, allowing
shared identical children. Converted unused words may legitimately wrap outside
payload bounds. This is containment of reached relocation accesses, not proof of
all future animation/material/texture semantics. Raw handlers retain broader
source alias domains.

VM bind registers external aliases to the same storage. Failure removes only
that attempt's successfully mapped prefix. Existing VMs/references/lists remain
untouched. Owner must outlive all borrowing views; bind does not supply automatic
view retirement or allocation-generation proof. BE word/half access asserts
unmapped requests rather than manufacturing zero data.

A single retained acquisition owns the shared name buffer/current file. Another
path/type cannot replace a held request. Newly loaded type must match staged
metadata, but real filename-only source hits ignore requested type. Pending reuse
requires an explicitly configured yield callback, set before freeze. There is no
default successful yield or collector. Release acknowledges source fileFree,
including no-op cases, and retains state-2 payload/holder identity for reacquisition.
`contains` checks staged name/type; `ownsPayload` checks original byte extents,
excluding rounded padding. Neither alone proves acquisition or content readiness.

Prologue staging reads unchanged raw MODEL/TPL/auxiliary products, not PMANIM,
pose-directory metadata or GPU atlases. Exact source names include trailing-hyphen
texture keys; e_mario/b_mario share `a/a_mario-`, and c_kuribo uses its own bank.
The batch includes raw vivian.bin/ag2tg.bin type 0. All reads finish before catalog
publication; staging does not acknowledge source startup/acquisition.

Common-menu facade intentionally admits only `./battle/common/battle_common.tpl`,
type 4. Loading requires exact 217920 bytes, TPL magic, descriptor count 97 and
table offset 12, then publishes its candidate process owner. `files()` exposes
that same wider owner for animation staging; it does not construct another manager
at identical addresses. VM binding reads actual root/format strings, bounded 256
bytes, and accepts only `%s/battle/common/battle_common.tpl`. It is not general
printf or path normalization. Closures borrow their binding and actor providers;
those owners must outlive pending resource services and VM views.
