# OW-PARTNER-01 — Ordinary Goombella follow and field lifecycle

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, as the first of the four "Native delivery and further subsystem requests" blockers (heading kept in [OW-MOVE-01](OW-MOVE-01.md)), and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Ordinary Goombella follow/history/speed/ground/obstacle rules, homing
  distance owners, launch/landing/room/battle lifecycle, field ability activation
  and control locks, before the partial native policies are presented as
  original functionality. Native `followerPath` currently samples every.08,
  caps512 points and teleports on overflow; these are provisional choices.

## Request text (verbatim)

Documentation agent: please also recover these overworld blockers before their
partial native policies are presented as original functionality:

- OW-PARTNER-01: ordinary Goombella follow/history/speed/ground/obstacle rules,
  homing distance owners, launch/landing/room/battle lifecycle, field ability
  activation and control locks. Native followerPath currently samples every.08,
  caps512 points and teleports on overflow; these are provisional choices.

The three sibling blockers filed with this one keep their bullets in
[OW-ROAD-01](OW-ROAD-01.md), [OW-INTERACT-01](OW-INTERACT-01.md) and
[OW-CAMERA-01](OW-CAMERA-01.md).

## Answers and delivery notes (append-only history)

No answer has arrived for this request yet.

### 2026-10-12 — Narrow ordinary animation resource delivery

[Goombella ordinary pose binding](../behavior/partner/FIELD_GOOMBELLA_ORDINARY_POSE_BINDING.md)
decodes the original normal kind1 row: stay1=PKR_S_1, walk2=PKR_W_1,
run3=PKR_R_1; low-HP remaps and seven original loop descriptors are recorded.
Selector0 PKR_Z_1 is a static zero-span pose, not ordinary stay. The complete
moving selector uses current partner+104, owner+228, original global processed
stick class and mask0x100; its literal speed-versus-speed+margin comparison is
preserved. Ordinary Rogueport owner scale1 and original draw scalar2 are bound
to their source writers. Facing/rate composition links existing contracts.

Validation: direct read-only source disassembly,15 strings/7 loop descriptors/
21 opcode anchors/5 constants/model hash; existing partner presentation
verifier passes8 bodies/515 instructions/32 constants/7 spans/2 imports/3 sites.
The request remains OPEN: native follow, event/story/recruitment, room/battle,
terrain, sound and input/turn source binding still need composition and original
runtime verification. No numeric documentation-agent identity is assigned.

Native implementation breadcrumb (2026-10-12): PMANIM3 now preserves animated
visibility and hidden shapes; field Goombella plays original PKR_S_1/W_1/R_1
with the source scalar2 and recovered moving selector. Zero-span PKR_Z_1 is
also packaged. A separate texture pack avoids overflowing Mario's atlas.
Current follower displacement and paper-facing remain native adapters rather
than original P+104 and turn ownership. Source sound, HP, dot-table, room/battle
and event-pose bindings remain open. Host source comparisons and an isolated
Azahar capture verify this scoped presentation, not the full partner lifecycle.

Facing correction (2026-10-12): user reported backward travel. The initial
native facing adapter reversed source targets; replaced it with the ordinary
flags-clear `partyMakeDispDir` target ranges/interpolation/latches. Positive-X
travel has heading90 and targets display180, negative X targets0. Model yaw
and wrapped inclusive90..270 Z reflection now use those source display fields.
Native displacement still adapts the travel-heading producer; special flags,
camera/heading ownership and original runtime turn traces remain open.
