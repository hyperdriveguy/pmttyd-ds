# Target-status classification and front-clearance parameter writer

US G8ME01 disc0 revision0,ID1,recovered2026-10-12. Evidence B:complete
nokonokoGetStatus0x800d08d8,size0xc0,and unk_80151d20,size0xc. The first
supports [live selection](FIELD_PARTNER_LIVE_DIRECTION_AND_ACTIVITY.md),the
second writes [front-clearance](FIELD_PARTNER_FRONT_FLOOR.md)'s parameter.
These are separate services;full lifecycle/source domains remain open.

## Guarded status-code classifier

Input Q null returns0. Unsigned Q+31 must equal2 and Q+0&0x100 be SET;
otherwise return0. Admitted objects use unsigned byte Q+39:

| Inclusive code interval,in original order | Result |
| --- | --- |
| 0..2 |1 |
| 10..13 |2 |
| 20..21 |3 |
| 50..71 |4 |
| 80..100 |5 |
| 110..114 |6 |
| Other admitted codes |7 |

The body reads no driver+34,motion+36,coordinates,history or global Mario,
and does not mutate Q. Status3 means the guarded kind2/flag/code20..21 route,
not every object's driver/motion byte3. Ability/code/flag producers and their
gameplay meanings remain open. Both guards and boundaries are pinned.

## Float setter and scoped DOL census

unk_80151d20 loads original60.0 at0x8042318c,stores it as FLOAT to SDA+1cf0
(0x8041ec10),then returns. No argument,increment/decrement or conversion occurs.
Navigation symbols name the four-byte BSS slot touch_wall_ct;the name does not
override actual lfs/stfs/fadds usage. Do not declare60 a measured timer duration
or an invariant runtime value without complete initialization/writer/lifetime
evidence.

A read-only scan of all seven US DOL text sections finds eight direct D-form
float loads from r13+1cf0 and this one float store,no matching scoped direct
address-construction/integer access. Readers:getFrontFloor0x8014b6b0;
beside0x8014dc24;continue-behind0x8014e5d8;behind0x8014ea7c/ec4c/ed68;
homing-fly0x8014fab8;homing-walk0x801507a4. Their containing motion bodies
are NOT closed by that census.

One direct linked DOL branch targets the setter:partyReInit0x800bd688.
Selected tail evidence places it after optional kind7 request0x800bd684.
ReInit is fingerprinted with SELECTED tail scope;earlier branches/all callers
and its complete initialization remain open. No REL,alias/indirect/platform/
BSS-writer census is implied.

tools/verify_field_partner_live_services.py re-scans direct DOL accesses and
linked calls against exact pinned lists,not merely known-site existence. It
also checks local bodies,constant60 and selected ReInit tail anchors. This
bounded census is not original execution or a closed runtime-value invariant.

Next:status/code/flag and BSS/alias/REL/constructor lifetimes,all clearance
consumers,source/platform/numeric domains and original target/clearance traces.
Full-game documentation and requirements remain incomplete.
