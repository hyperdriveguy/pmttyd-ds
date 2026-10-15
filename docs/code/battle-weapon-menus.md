# Battle class and weapon menus

Read [battle commands](battle-commands.md) for the input loop and attack dispatch,
[typed targeting](typed-battle-support.md) for confirmation and
[packed battle state](packed-battle-state.md) for candidate storage. This guide
covers the packed class/weapon table, eligibility, decision, weapon-main and
retained audience-selection helpers. Missing providers are not successful UI
operations; immediate parents can leave prior effects when unavailable.

## Separate row formats

Menu storage begins at `B+0x171c`. Class rows and weapon rows are distinct ABIs.
All offsets in this table are hexadecimal; strides are decimal bytes.

| Field | Class: menu+8, stride 20 | Weapon: menu+0x80, stride 28 |
| --- | --- | --- |
| Type / weapon pointer | +0 | +0 |
| Enabled | +4 | +4 |
| Resolved label | +8 | +8 |
| Icon halfword | +0xc | +0xc |
| Denial reason | +0x10 | +0x18 |
| Ordinal / item ID | — | +0x10 / +0x14 |

Class cursor storage is `menu+0x498` for Mario, `+0x4a4` for others. A wrapped
null menu returns the pouch pointer from the class producer, rather than a count.
Mario always gets Jump; signed boot level below one disables that row without
adding a denial reason. Hammer requires signed hammer level at least one.
Partners 224..230 get type 6; Items use type 2, Stars type 4, Operation type 5,
and the special alternative type 13. The source battle/unit/definition gates
remain inline; unknown mask bits have not received invented semantic names.
Operation and special alternatives independently reread battle flags. A provider
can change them between reads, so one cached mutually exclusive branch is wrong.

Class icons 334..339 select level 1, level 2, otherwise highest; a missing boot
can therefore use the highest icon while disabled. Weapon icons are a separate
unsigned-level mapping: Jump levels 3/2/other give 169/168/167 and Hammer
166/165/164. Do not unify these helpers. Both table producers clamp only the
upper cursor bound. Negative indices are not normalized; count zero can produce
wrapped `count-1` in the class producer.

## Weapon rows and eligibility

Private cursor/count scratch addresses must be nonzero and distinct. The count
is live memory, not a local vector size: providers may mutate it while names,
icons or party entries are produced. Basic Jump/Hammer rows use up to three
freshly checked signed pouch levels (`+0x98/99`). Badge tables at `0x80416d80`
(Jump, eight entries) and `0x80416da0` (Hammer, nine) add one row per equipped
badge ID, regardless of equipped copies. Under the source forced first-row masks,
unit data key 68/69 replaces the weapon *after* icon/name production; it does not
relabel that row.

Item type 2 scans 20 signed pouch halfwords at `+0x192`. Item definitions have
40-byte stride at `0x803108a8`; flag 2 at `+0x10` admits battle use. A missing
weapon pointer uses `0x80365488` with the item's label/icon. An empty inventory
creates weapon `0x80363e60`, icon 362, and writes ordinal -1 then zero. Preserve
both stores. Star type 4 scans 16 pouch bits at `+0x8c` through `0x80355ca8`.
The original post-increment limit remains 21. Party type 6 requires the real
unit-data-key-1 producer. A zero weapon count preserves prior cursor/descriptors;
a positive count publishes the cursor, reduces scroll for six visible rows, then
updates eligibility. Six is a source menu policy, unrelated to screen resolution.

Eligibility clears enabled/reason per row, then queries target count. No weapon
or event script gives zero; failed cost gives -1; a real sampled positive count
enables. Negative values choose reason 2 for Star, otherwise 1. Zero chooses
reason 4 for Jump, 5 for Hammer, otherwise 3. Sampling uses private candidate
storage and the negated enemy-row signed-low-byte direction. The helper is an
eligibility sample, not publication of the live target buffer at `B+0x428`.

Final weapon decision captures the actor before building rows. It samples actual
`B+0x428` using the owner-row direction without negation and resets selected rank
`+0xab7` to zero. Mask `0x10000000` selects the last same-side rank and takes
precedence when both targeting masks are set. Otherwise `0x20000000` selects the
first different-side rank, including neutral side 2. No match retains rank zero.
An engaged return zero is successful source work. Setup must initialize the real
UI before publishing flags `0x01000010`.

## Weapon input and audience interruption

Weapon-main key gating accepts a nonzero marker `B+0x1c74`, otherwise UI byte
`*(B+0x1c78)+0x14`. Marker clearing precedes the confirm-trigger callback, which
can republish the marker. Confirmation is therefore a trigger *or a fresh marker*.
Enabled confirmation ends UI, decides the weapon, sets up targets and publishes
menu 10. Lighting reads weapon fields separately; callbacks can change the row.
Disabled confirmation can publish denial menu 8/reason; ordinary denial with no
reason still returns zero after display work. Forward repeat/edge `0x20000`
returns immediately; reverse `0x10000` is then not polled. Cancel `0x200` uses
menus 15/12 for special item state, otherwise menu 1; source sound ordering stays
at the branch. Every normal return submits display, even with keys denied.
Unavailable children return before that submission.

Audience interruption is checked only on the no-input route and may end UI/set
its marker while returning zero. Its retained child pins memory identity and
private output, captures B once and requires trigger `0x400`, battle mask
`0x40000000` clear and Mario attribute `0x10` clear. Item-on can hold at phase 1;
item -1 cancels. Phase 2 sets the selected target once. Phase 3 can hold for actual
menu closure without replaying that target setup. Phase 4 saves `B+0x1c5c` into
`+0x1c60`, publishes type 11/menu 29, turns off light `(0,1)`, then ORs battle
flag 4. True reports this selected interruption, not an attack root or battle end.
The synchronous wrapper has a temporary cursor and cannot retain these holds.
Concurrent lifecycle-provider changes beyond this frozen source require their
own review; these notes do not acknowledge unimplemented audience services.
