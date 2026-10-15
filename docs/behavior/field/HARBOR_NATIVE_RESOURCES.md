# Original harbor native resources

The original `gor.rel` child at `0x25290` directly enters Goombella
(`c_pkr`), Crump (`c_kanbu_ibt`) and two subordinates sharing `c_zako_n`.
It applies tribes in that order, then places all four at `(0,-1000,0)`.
These four are not tagged-map rows or invented npcSetup entries. The two
sailors used by the before-battle tail are actual `gor_00` tagged actors:
tribes ガラの悪い水夫 (`c_gara`) and 水夫ボム兵 (`c_suifu_b`).

`tools/harbor_native_npc_assets.py` exports those five original banks, complete
MODEL/TPL companions, source descriptor order and every CPU animation. Counts
are Goombella32, Crump59, subordinate28, first sailor16 and second sailor9
(144 total). The subordinate bank is shared; no duplicate actor/regular ID is
allocated by this resource catalog. `source_tribes` retains the first exact
original DOL tribe row, its address and digest. Every bank manifest records
ISO offsets, source hashes, conversion fingerprint and output hashes.

Crump's actual MODEL texture identity is `c_kanbu`, so the staged paths are
`a/c_kanbu_ibt` type5 and `a/c_kanbu-` type4. The MODEL suffix is not copied
into an invented texture filename. The other four use their model name with
`-` appended for the texture bank. NativeFileCache stages the eight new
Crump/subordinate/sailor files atomically with existing process resources,
before binding any VM. Goombella's original MODEL/TPL already exist there;
they are not staged twice. Acquisition, relocation, shared references and
release continue through the existing real cache owner. No payload arena
limit or guest-address range changed.

The canonical field-player Goombella pack also converts all32 source clips in
source descriptor order. This matters because the party and NPC pose owners
share resource identity: entering the selected eleven-clip bank first would
leave later story clips unavailable even if a second full pack were imported.
The older standalone trio Goombella bank remains a partial bank; arbitrary
callers entering that bank first are not covered by this field-entry guarantee.
Original MODEL/TPL, texture orientation, facing logic and atlas policy remain
unchanged. Full Goombella coverage expands31 to46 cells and source atlas
1024x512 to1024x1024; the existing half-resolution RGBA8/Lanczos policy
produces512x512 runtime textures instead of512x256, with UV scale2→1.
Atlas and UV products must be rebuilt together. Mario's explicit prologue subsets are unchanged. All conversion
products stay outside source history; original copyright and local-use-only
terms apply, with no redistribution rights.

This is resource availability, not direct Entry/SetTribe completion. Source
Entry still needs real animGroupBaseAsync readiness followed by retained
npcEntry; SetTribe must preserve the existing model/handle, perform ordered
stores and forced SetAnim, and return1. Source async result0 ends an invocation;
a native child hold retains it. Synchronous file-cache acquisition alone is
not source async readiness. NPC placement, story visibility, regular-event
creation, Main, Draw/GPU, battle teardown/field return and postbattle flow
remain owned by their original parents. This import does not acknowledge them
or make hidden actors visible.

Fresh complete source evidence and original data are kept locally in
`artifacts/field-npc-direct-entry-evidence/` and
`artifacts/harbor-native-npc-evidence/`. See the request inbox
[EVT-01](../../requests/EVT-01.md),
[DEMO-01](../../requests/DEMO-01.md) and
[RESULT-01](../../requests/RESULT-01.md) for remaining integration work.

Crump's original zero-count UV index streams1..7 contain offsetffbc8fc0.
Native raw-resource admission must not dereference/bounds-check an empty stream
as if it had an index. Only its zero-count UV span check is skipped; selected
stream/run/index checks still bound every actual read. All Crump draw selectors
use stream0. Original bytes and the source relocation/clear writes to all root
words, including these sentinels, are preserved. See
[original array/FIFO reads](../display/ANIMATION_RENDER_ARRAYS_AND_VERTEX_FIFO.md).
A real-resource regression loads all five complete banks and changes Crump's
stream1 count0 to1 to ensure its non-address sentinel is then rejected.

The shared native file cache's live geometry check likewise skips byte compares
for empty UV streams. It still validates exact source-plus-payload relocation
for every root word. The cache fixture verifies all five harbor banks through two
actual VM views, then corrupts/restores Crump's empty-stream root; admission
rejects the wrong relocated word despite its zero stream count.
