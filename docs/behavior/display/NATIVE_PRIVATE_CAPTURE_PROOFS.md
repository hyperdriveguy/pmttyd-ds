# Private raw publication capture proof

The [atomic native mesh owner](NATIVE_ATOMIC_MESH_PUBLICATION.md) can accelerate
phase20 scalar decoding within one closed raw RAM borrow. `configureMain` defaults
`privateCapture` to false. An explicit true additionally promises that its current/
pass predicate is independent of every private publication candidate/cursor write.
Purity alone is insufficient: a pure predicate could inspect changing candidate
values. The normal NativeFieldAnimation predicate reads only Mario active/pass
state and opts in. Ordinary public callers retain the fresh per-primitive guard.

Fast admission proves, on EVERY borrow:

- Full availability before entry: original current/pass, header, process structure,
  slot generation, resource and scratch owners still match.
- The actual current global W-root word and its admitted24-byte W header are raw,
  complete nonsemantic storage. This proof uses a fresh root, independently of
  the earlier metadata capture's W pointer.
- Every nonempty captured channel extent is complete raw/nonsemantic RAM. Existing
  admitted counts/nonwrapping bounds remain in effect; zero-sized channels need
  no source mapping. Missing later channels do not erase a readable scalar prefix.
- The entire publication object and all six vector spans written during decode
  (positions, normals, visibility, nodes, textures, surface transforms) are
  physically disjoint from EVERY VM owned and external RAM region. Checking only
  current sources would miss aliases into W/pose/other future sources. Mappings
  and independent backing owners remain fixed for the existing raw-borrow lifetime.

`RawRamAccess::disjointStorage` proves only physical separation from registered
owned/external VM storage. It does not claim arbitrary semantic-provider storage
is private. The full admission above excludes semantic source/guard reads, and
predicate independence plus ordinary native object ownership complete the proof.
The method stores no pointer and rejects null nonempty or overflowing host spans.
Empty write spans are harmless. Host storage must stay alive throughout the borrow.

Inside the admitted slice, guest reads cannot mutate state, private candidate/
cursor writes cannot alter guest source/guard bytes, and the independent predicate
cannot change as a result of decode. Thus repeating ownership checks per scalar
adds no information. The owner still reads fresh bytes in the same order, applies
the same BE conversion and finite checks, and preserves every candidate-store,
lane/channel transition, read counter and visit budget. A bad float still writes
the private lane then holds at that lane as before; repair reads it again.
There is no float memcpy, source-value cache, channel memcpy, operation reordering,
source replay or change to simulation/render timing.

Any failed proof falls back to the existing scalar path rather than holding at
an earlier boundary. The fast and structural flags clear by RAII on every return,
including exceptions; no pointer, raw proof or comparison result survives a yield.
Assembly, static geometry checks and final publication occur outside this lease
and keep their original full checks. Metadata capture phases remain scalar.
`fastSlices()` is diagnostic activation evidence, not source behavior or time.

## Validation and scope

Tests compare identical original Mario resources under opt-in and scalar modes
with varied budgets, matching phase/completion/read counts and complete published
samples. Existing every-visit hold/static mismatch/global output/linked/null/
lifetime/Game tests run with fast opt-in. Additional tests alias candidate and
publication-object storage into unrelated guest mappings, verify scalar fallback,
remove aliases and observe renewed fast admission. Missing later channels preserve
readable prefixes; nonfinite data repairs at the same scalar frontier; stale pass
between resumes reads nothing. Raw tests cover owned/external physical storage,
empty/null ranges, overlap boundaries and disjoint caller storage. Native probes
run the same assertions on ARM. Full evidence and phase timings are in
[progress](../../PROGRESS.md) and the ignored publication-fast manifest.

This is native lookup/guard acceleration. It does not establish original Draw/
Inner/queue/material/pixel parity, complete NPC activation, or physical New2DS XL
performance. The original assets, numeric operation order, clock and simulation
barriers remain unchanged. Other source renderer/service work stays open.
