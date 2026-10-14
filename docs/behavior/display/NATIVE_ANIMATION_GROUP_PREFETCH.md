# Original animation group prefetch in native ROMFS

Source: [Bero prefix and exact resources](../overworld/BERO_INFO_PREFIX_ASYNC_GROUPS_SYSTEM_FLAGS_AND_ACTOR_PREPARATION.md),
[original file async semantics](../file/FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md)
and [area parent](../field/NATIVE_ORIGINAL_AREA_PARENT_AND_RETIREMENT.md).

animationGroupBaseAsync implements the original mode0/flags0 path: select archive1,
request type5/flags0 a/group, scan the live unchanged ag2tg table in32-byte steps
with first strcmp match selecting its following32-byte row, then always request
type4/flags0 a/texture-. No match uses the original empty-name fallback a/-.
Both requests occur before readiness selection. Group -1 returns true; group0
returns false; otherwise texture -1 or any nonzero returns true. The helper
retains request/lookup order even when the first provider mutates table state.
Other archive modes and flags are outside this bounded helper's admission.
Native validation requires complete NUL-terminated32-byte rows and a following
row for a match; malformed data cannot produce an invented texture result.

fieldBeroReadMarioPera uses actual EventEntry::waitCount (source+78). firstCall
resets0 even while no provider is installed. A poll at0 requests p_dokan_x and
advances to1 only when ready; a separate invocation at1 requests p_dokan_y and
advances to100 only when ready. Only100 returns2. The helper does not modify
locals or create Mario paper geometry. FieldAreaEvent retains the generic
provider boundary when no typed prefetch owner is supplied, after performing
the required source first-call reset.

AnimationGroupPrefetch is a native cooperative ROMFS cache, not an emulation of
DVD timing or the original FileEntry allocator. open loads the unchanged ag2tg
payload, validates its rows and owns the table. First request opens/measures and
allocates an admitted nonempty file (maximum4MiB), then yields pending. Later
polls read at most4096 bytes per file by default; caller budget is explicit,
positive and at most65536. These limits are native policy, not measured game
constants. Standard stdio uses the existing native ROMFS/libctru filesystem.
Reads are synchronous bounded chunks; no asynchronous OS worker is claimed.
Each original two-request poll can read two chunks. Duplicate completed requests
return a native positive ready token without reloading/referencing a pose.

The cache owns and retains all bytes until its area/resource owner is destroyed;
resident returns only fully read payloads. Partial files, missing files and read
errors return pending with failed diagnostics, never source -1 success. Cache
identity is native path within this one archive owner; it is not the original
FileEntry/refcount domain. Both pipe groups use the one a/p_slit- resident vector.
No decoding, GPU upload, animPose creation or pipe draw is acknowledged here.
The four local source files total124908 bytes including the41984-byte alias table;
three actual requested payloads total82924 bytes. Future areas can add authentic
group/texture sources without hard-coding their alias mapping in the callback.

The converter reads original a/ag2tg.bin, a/p_dokan_x, a/p_dokan_y and a/p_slit-
from the read-only ISO, validates fixed source sizes/SHA256 and writes unchanged
private romfs/field-prefetch/a files plus a provenance manifest. It runs with the
existing door-data asset pipeline. No source bytes enter Git history.

The shared area owner also binds actual get_system_flag80054f3c: capture args,
capture GP, read raw destination, read captured GP word0, then evtSetValue.
EXEC-get8007aa04 instead captures args, reads the known live EXEC owner first,
reads raw destination, then evtSetValue. Unknown EXEC state holds. Existing exact
EXEC-onoff8007aa34 resolves both captured arguments before updating the live owner.
This allows the genuine ordinary info_run to finish prefetch/flags/EXEC and reach
Mario key_onoff800e7474. Its later
[native preparation binding](../field/NATIVE_MARIO_KEY_AND_BACKGROUND_PREPARATION.md)
now uses the shared Game key owner; native background ownership remains pending.

Tests check all -1/0/positive readiness combinations, both unconditional requests,
post-request alias lookup, empty fallback, retained phase transitions, absent
provider reset, unchanged locals, actual source hashes and alias rows, bounded
polling, shared texture identity, byte-for-byte resident payloads and missing-file
holding. The original parent/attached-child test uses an explicit synthetic GP
image and source story402 sound skip, not a new certified Game save state.
The isolated native probe follows that same genuine branch using actual ROMFS
reads and stops at key_onoff; it does not complete area init or enable its cases.
Game's separate runtimes and native actor/audio/pose integration remain open.
