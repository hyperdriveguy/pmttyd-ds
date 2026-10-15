# Audience items, selected holders and lifetimes

Read [audience work](battle-audience.md) for actors/population and
[audience sound](battle-audience-sound.md) for throw lanes. Actor item states
7/8/10/12 and item-flight states 0..7 are different domains. This implementation
publishes original presentation metadata; it does not grant inventory or replace
the corresponding presentation/damage events.

## Source item flow

One hundred records begin at `A+0xf25c`, hexadecimal stride 0x48:

| Offset | Meaning |
| --- | --- |
| 0 / 4 / 8 | Flags / flight state / timer |
| c / 10 | Holder slot / item kind |
| 14..1c / 20 / 24 | XYZ / size / angle |
| 28..30 / 34..3c | Velocity / acceleration |
| 40 / 44 | Target unit / presentation mode |

Creation narrows kind to U16; IDs 121..235 require switch 235. Species table,
seat column 4..15, actor state exclusions and flag 8 must admit before the supplied
free-slot scan. The source limit is not silently clamped to 100. ItemBirth follows
active publication; ItemReady follows actual actor state 7 and held-index linkage.
Actor held index is signed halfword `+0x1c`; -1 is not an item record.

Actor ItemOn state 7 owns held state-1 countdown and changes actor to state 8
before throw sound. Hit flag 0x40 can clear/notify/unlink a held item and change
the actor to state 10 before a linked target penalty. The source penalty checks
item slot zero, not that actor's held item. Damage state 10 uses an eight-step
scale cycle. Escape state 12 treats species 11 as a falling branch; others retain
raw-RNG trajectory and sound effects before deletion. Escape eligibility has its
own source flags/state exclusions, independent of native visibility.

Flight main clears records for missing actors or actor states 13/19/21/12.
Clear observes ItemClear after flag zero but before timer/state/unlink suffix.
These immediate bodies have no retained suffix; missing providers can follow
source stores, RNG or the first of two sound-stop calls.

| Flight state | Local work |
| --- | --- |
| 0 | Select zero-kind via weighted table, timer=index+180, initialize position, fall through |
| 1 | Hold/interpolate from actor render position, then choose player target and launch |
| 2 | Positive-timer integration; timer 20 resets actor; zero advances state 3 for next visit |
| 3 | Impact gates, original presentation metadata and harmful weapon copy |
| 4 / 5 | Bounce integration then ground tests |
| 6 | 60-count fade, blinking below 30, then cleanup |
| 7 | Five-call ordinary-item delay then cleanup |

State 1 does not decrement its held timer itself. Its five-step lift clamps only
the upper step; early negative steps remain. Player-target scan examines 64 battle
slots, performing original body/part queries before side/HP gates. No candidates
returns engaged zero without RNG; unavailable/invalid provider output is distinct.
Launch duration scales with X distance; real truncation/FMA produce ticks/gravity.
Kind 239 rotates during flight.

Impact checks status 18, shadow/flags and inventory room before publishing target/
kind. Harmful kinds 236..239 copy an original 0xc0-byte weapon record; ordinary
121..235 use presentation state 7. The impact counter saturates at three. Inventory
room counts 10 or 20 pouch slots according to original key item 1, not item-bank
capacity. Cleanup stops lanes 10 and 11 before actor reset and item clear. No
successful local branch alone proves the inventory/event service completed.

## Original first-eligible query and cancellation

GetItemOn scans 200 actors: flag 1, no 0x102, held index not -1 and signed item
state at most 4 or equal to 7. It adds no item-active-bit gate. Match writes cached
slot only when index output is nonzero, always writes cached XYZ, then optional
XYZ outputs and freshly queried kind. Outputs may alias source work. No match
unconditionally writes -1 through index output, including address zero. Native
callers must admit the actual source output domain rather than adding a shortcut.

SetTarget marks the chosen actor 0x20 without rechecking eligibility. It cancels
only other existing actors in state 7, clearing their actual item and observing
ItemClear, unlinking and resetting actor state. It retains fresh global lookups.
The physical X path remains this original first-eligible query.

## Native provenance and chosen-slot interruption

Lifecycle hooks are synchronous observations after real source writes. They must
not read/mutate guest work or invoke source services. Reset admits one nonzero
whole 0x13914-byte audience span and creates a distinct token. Exact record
alignment is required; ActorBirth/Ready and ItemBirth/Ready are distinct, while
Reserved is an occupied paired neighbor rather than an independent birth.

One serial supplies all 200 actor and 100 item generations. Generation zero is
unknown. U64 exhaustion clears identities and blocks births until actual Reset,
preventing wraparound reuse. ItemReady captures its holder's then-current actor
generation. Identity requires both observed lifecycles alive/ready and a matching
link now; the same pointers/kind after slot reuse are insufficient. This native
metadata neither produces original population nor proves visibility/eligibility.

Selected-holder inspection is read-only. It requires exact token/generations,
actual displayed-frame visibility/danger/interruptibility, source flag/held/state
gates and current harmful kind 236..239. Cache publication reinspects immediately;
a previous result is not authorization to write slot/XYZ later. Rejected owner
checks precede stores, but once writes begin no rollback hides earlier effects.

The retained selected interruption fixes input/providers across holds and checks
lifetime on every call. Before committing selection it checks battle/Mario flags
and current holder eligibility. After SetTarget, UI/light holds preserve that
committed choice rather than restarting visibility/eligibility tests; lifetime
still revalidates. It saves prior type, publishes type 11/menu 29, calls lightOff
(0,1), then sets battle mask 4. False rejects, nullopt holds a required provider,
true completes this tail. Later lifetime rejection does not undo prior SetTarget.
It creates neither an attack root nor a player turn/input-ready state.
