# Original atomic return-held-item event

This owner implements complete btlevtcmd_GetBackItem8010da28/b8. Source pins
cover the complete parent and existing complete registry800f88cc/3c. There is no
Pouch, sound, resource, render or asynchronous child in this body. Original
executable payload remains outside source history.

The parent captures Battle globally before reading/resolving argument0,
translates the actor selector and obtains the unit using that captured Battle.
It reads unit attributes+1c; bit40000000 clear completes without even reading
the held-item field. Otherwise it captures unit+308; zero completes before
reading the information/setup pointers. These are executed original branches,
not native no-item guesses or successful missing-provider substitutions.

For a nonzero item, capture information at Battle+2738 then setup at info+c.
Scan exactly eight setup words from+7c, stopping at the first zero. Store the
captured item at the selected index, then clear unit+308. All eight occupied
slots deliberately select index8/setup+9c in the original source. The owner
preserves that one-past write under its actual mapped writable-domain contract;
it does not clamp, discard, manufacture an item or directly grant a Pouch item.
The setup pointer and item remain captured across later source memory callbacks.

This atomic helper uses the existing complete EVT/memory/actor services. Caller
admission must keep the whole reached domain and provider lifetime stable;
callbacks are synchronous and total, with void acknowledged stores. There is
no partially acknowledged async body to blindly retry. Missing mapping is not
an executed zero. Native Harbor calls it through its existing required packed
memory/actor owner; no guest cleanup or readiness write is added.

Tests compare the full read/getter/translation/registry/store trace for every
occupied count0..8, omit unused memory to prove both closed branches, and mutate
Battle after its capture, held item after its capture and setup after the first
store. Strict host, ASan/UBSan and source/test/Packed ARM syntax pass; only the
existing Packed TU's baseline indentation warning is demoted. A separate actual
lethal CPU fixture uses coherent15584 headers/core plus frozen death/StarPoint/
rotation-offset overlays. Its native GPU/presentation predicates are explicitly
TESTONLY; no normal native pixels or field/result completion is claimed.
Root fresh make test/make all and actual native checks remain necessary.
