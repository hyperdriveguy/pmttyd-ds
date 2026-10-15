# Audience sound records and envelopes

See [audience work](battle-audience.md) for counts/owners,
[items](battle-audience-items.md) for throw state and [audio services](audio-services.md)
for the backend. These packed source records are separate from native audio voices.
Nullopt means unavailable provider work; public handle -1 is a real failed start.

Twenty-four records begin at `A+0x133fc`, hexadecimal stride 0x24:

| Offset | Meaning |
| --- | --- |
| 0 / 4 / 8 | Flags / public handle / category byte |
| c / 10 | Lifetime / lifetime fade duration |
| 14 / 18 | Explicit envelope elapsed / duration |
| 1c / 1d / 1e | Initial backend volume / live base volume / current percent |
| 1f / 20 | Envelope start / target percentages |

Half counting walks ten rows by ten columns, separately recapturing owner/flags
and actor pointer. It excludes states 12/13/17, unlike actor control's 11/12.
Count buckets below 50/100/150 select categories 1/2/3, otherwise 4. Zero-count
clap/noise leaves the original key unassigned; `assigned=false` preserves that
boundary and does not authorize playing key zero.

Stop requires flag 1, performs the actual backend request, then rereads flags and
clears only bit 1. It retains handle/lifetime/volume bytes. Clap stops lane 1 before
new start and only publishes lifetime/fade/active data for a real non--1 handle.
NoiseAlways counts first, then returns for an already-active lane 2 without restart.
New success uses lifetime -1, base volume and percent 100, then sets audience
flag 0x2000. A missing volume lookup can follow voice activation.

NoiseMain observes battle flag 0x80 edges against audience 0x10000 and schedules
lane-2 fades over 80 source visits. Count zero can stop the recorded category;
a category change calls NoiseAlways, which may return early for an active voice,
then still publishes the new category. Count/flag-dependent base-volume arithmetic
preserves separate binary32 operations and signed conversion before low-byte
storage; no guessed mixer saturation is substituted.

Explicit volume setup captures current percent into start, narrows target to a
byte, resets elapsed/duration and clears the lifetime fade. All-record setup
recaptures source owner per lane. These durations count main visits, not audio
sample frames or measured rendering time.

SoundMain requires actual NoiseMain before visiting **all 24 records without
active-bit gating**. Check result -1 clears active without stop; lifetime zero
stops, negative lifetime is indefinite. A positive explicit envelope increments
elapsed before interpolation/truncation and stores percent as U8. Backend volume
is `baseByte*percent/100`. A lifetime fade can issue a second volume request in
the same visit. Lifetime decrements only after admitted dependent calls. Duration
zero returns the endpoint; interpolation does not clamp step. Provider failure
can therefore follow stores or earlier voice effects; this immediate parent has
no retained replay cursor.

Spatial Sound1 uses wrapped signed total. Its nonzero-count volume needs real
truncation; imbalance position X narrows to unsigned byte before widening to float.
Left zero or total zero uses the source position constant instead. Preserve this
unusual byte route rather than replacing it with signed panning. Sound1 still
requests volume after a real -1 start, matching the local source sequence.

Throw sound first reads actor X to choose lane 10 for X<=0, otherwise 11. After
stopping that lane, Sound2 independently rereads actor position for actual start.
Kind zero selects source key 0x802fa43c, others 0x802fa458. Only an actual successful
handle installs active/lifetime/base-volume state. These helpers do not fabricate
voices or imply complete original spatial/MusyX routing.
