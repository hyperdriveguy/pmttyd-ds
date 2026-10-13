# MusyX allocation-aid initialization

US G8ME01 disc0 revision0, recovered2026-10-13 for
[EVT-01](../../requests/EVT-01.md). Complete synthInitAllocationAids80285328/2a0hex.
Addresses/offsets hexadecimal; counts/scalars decimal. Static ordered writes,
not original allocation/audio observations. Read
[synthesis initialization](MUSYX_SYNTH_INITIALIZATION_TYPES_JOBS_AND_CALLBACK.md),
[voice selection/bookkeeping](MUSYX_VOICE_ALLOCATION_SELECTION_AND_BOOKKEEPING.md)
and [voice kill/recycle](MUSYX_VOICE_SOUND_KILL_AND_ALLOCATOR_RECYCLE.md).

## Bases and count reads

The body uses fixed synthInfo803f8540.byte210 for the unsigned voice count.
Allocation aids use base80405b40:

| Domain | Address/shape | Writes in this body |
| --- | --- | --- |
| Free records | 80406940, stride4 | previous byte0, next byte1, flag half2 |
| Priority voice records | 80406340, stride4 | membership half2 only |
| Priority heads | 80406440,256 bytes | every byte255 |
| Priority sort records | 80406540, stride4 | none |
| Free root/insertion marker | bytes8041ef39/8041ef38 | 0/(count-1).low8 |
| Priority root | half8041ef34 | 65535 |
| FX/Music running | bytes8041ef37/8041ef36 | 0/0 |

There are four distinct count reads:80285348 for free-loop admission/bulk,
8028540c for its scalar remainder (unless initial count0),80285450 for endpoints
and priority-membership admission/bulk,802854d8 for that scalar remainder
(unless endpoint count0). There is no captured single-count guarantee,64-slot
cap, null/sndActive/IRQ guard or child call. The stable-count deductions below
require those reads to agree. This body does not establish permitted startup
counts, lifetime, exclusion from concurrent writers or safe reinitialization.

## Free records and endpoint publication

For stable count N>0, initialize ascending ordinals i=0..N-1 with
previous=(i-1).low8, next=(i+1).low8 and flag half2=1. For N>8 the bulk loop
writes8 rows per iteration, floor((N-1)/8) iterations. Otherwise it writes no
bulk rows. The scalar remainder uses a fresh count, ordinal comparison and CTR;
it writes the remaining1..8 rows for stable positive N. Both paths preserve
ascending write order within each row. N=0 at the first read skips both loops.

Then freshly read N at80285450 and publish in this order:

1. Free row0.previous=255 at80285464.
2. Free root8041ef39=0 at80285474.
3. Byte at freeBase+4*N-3=255 at8028547c (last row.next for N>0).
4. Insertion marker8041ef38=(N-1).low8 at80285480.

These endpoint writes are unconditional, before the zero-count branch. N=0
therefore writes row0.previous=255 and byte8040693d=255, sets root0 and marker255.
It does not create a root255 empty free list. This is an exact source address
calculation, not a claim that original execution supplies zero or suffers a
particular failure. For N=1 both row0 links become255 and marker0. Other free
rows are untouched except where these exact stores address memory.

If count changes between reads, bulk/remainder/endpoints can describe different
extents. A fresh remainder count below the current ordinal skips the scalar
loop; no earlier rows are undone. Do not substitute a single snapshot or repair
sentinels without a separately documented native adaptation.

## Priority membership, heads and counters

Reset priority ordinal to0. If endpoint count is positive, clear only half2
of ascending priority voice records. Bulk/remainder structure matches the free
phase: floor((N-1)/8) groups of8 for stable N>8, then a fresh-count scalar
remainder. The scalar phase advances its pointer4 bytes per CTR iteration; it
does not increment the ordinal register inside that loop. Previous/next bytes
remain unchanged. Zero endpoint count skips both membership loops.

Next clear all256 priority head bytes to255 in8 groups of32 ascending stores,
independent of count. Then write FX running0 at802855ac, priority root65535 at
802855b0 and Music running0 at802855b4, in that order. No priority sort record
clear, voice-state clear, hardware request, macro/VID unlink, sample/resource
release or accounting reconstruction occurs here. With counts beyond64, the
source still computes these addresses without a bound; valid count/overlap
preconditions remain open.

## Evidence and remaining bindings

[Evidence manifest](../../../tools/musyx_allocation_aid_initialization_evidence.json)
pins one complete body,142 instruction anchors and zero direct calls. Read-only
ISO verifier checks source identity; eight stable-count/six free-row vectors
record mathematical deductions, including0/1/8/9/64/65/255 boundaries. They do
not execute the original body. Full fresh inspection ELF/listing stays `/tmp`;
no original binary body or asset is exported into source history.

Remaining: voiceSetPriority/voiceFree and other membership/free/sort/count
writers; actual synthInfo counts/call order, valid slot/member/sorted/cycle/
alias/reinitialization/IRQ/concurrency/accounting domains and original startup/
allocation/audio traces. The parent request remains OPEN.
