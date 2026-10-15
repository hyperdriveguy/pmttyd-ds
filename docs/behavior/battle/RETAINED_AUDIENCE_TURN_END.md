# Retained audience TurnEnd parent

`resumePackedBattleAudienceTurnEnd` implements the whole ordinary no-argument
`BattleAudience_Case_TurnEnd8019f298/168` parent. It composes the existing
[retained target graph](RETAINED_PINCH_AND_AUDIENCE_TARGET_GRAPH.md), original
phase-list leaf801a1520/7c, NumToTarget801a1c60/7c, its count helper801a1bf4/6c,
GetExist801a29c0/64 and GetPtr801a8664/3c. This is the audience child of real
Turn state4d, not a replacement for Turn/Main cadence or battle completion.
The original [turn-boundary and queue recovery](UNDERGROUND_GOOMBA_TRIO_SETUP4_AND_RETURN.md)
provides the wider context.

## Original captures and effects

Entry captures global B, obtains Mario from that captured B, independently
gets audience A from fresh global B, and calls pouchGetPtr even though the
returned pointer is unused. It reads B2738 field info, then audience flags,
info+c setup, setup+18 group, and group+4 sets. Only **after all those loads**
does the saved audience flag40000 skip the remaining graph. Pointer loads and
helper side effects are not omitted on that early branch.

Chance at captured A13908 receives wrapping word+3, is reread signed, and is
conditionally clamped to100. The source group count is signed for loop entry;
positive count scans original set records at stride30, adding each referenced
kind's unsigned byte+0e. It does not count surviving native enemies. The low32
sum doubled becomes the signed numerator for division by Mario's unsigned
level byte+d. Threshold is at least5. Unavailable division retains those
operands and earlier stores; no new level/group/chance read occurs on retry.

For current signed half B0>=threshold, a fresh audience base supplies baseline
for original multiplier80424984 (`bd4ccccc`), then bias80424988 (`00000000`).
The whole target update is retained. Afterwards a fresh half B0==threshold
requests category6. Mandatory irand(100) follows regardless of the chance.
Its accepted result is compared with a **fresh** chance from captured A. A
successful chance rereads B0 for category1 admission, then requests
4,9,10,11,12,13 in order and clears captured A13908. NumToTarget always follows
that branch. No RNG draw is consumed twice after a later wait.

Each phase-list request freshly captures global audience A. It scans the
captured signed count in order, returns without writes for a duplicate, or
appends at captured count and increments a **freshly reloaded** count. There
is no invented queue capacity/category clamp or sorting.

## Target count and remaining population provider

NumToTarget independently captures A and checks its actual flag40000. Otherwise
it adds fresh desired+offset in source order, retains the single result across
source fctiwz admission, clamps signed result below0 then above signed capacity,
and calls NumToTargetSub. That helper's initial BaseGetPtr result is unused.
Every GetExist also makes an unused BaseGetPtr call before GetPtr obtains a
fresh global base. A qualifying record requires flag1 set and flags100/2 clear.
Its state byte+19 comes from **another fresh GetPtr**; state12 is excluded.
Flag10 does not exclude it. The 200-slot count is subtracted with word wrap.
These independent helper captures remain distinct even when globals change.

AddAudienceNum's entry prefix freshly captures B, independently captures A,
gets pouch P, and reads U16 P8c. Only actual P8c==0, amount==0, or actual
Bef4 bit100 completes without population work, in that order. This is not an
empty-scene or visibility shortcut. A nonempty case calls the optional actual
remaining-source provider with a stable `BattleAudienceAddNumInvocation`
containing the captured B/A/P, amount and capacity. That provider owns the
original suffix from801a176c through return; it must retain all successful
allocation/removal/RNG effects and must not recapture the entry globals.
Missing provider holds at801a1708. Its integer return is discarded by source;
the native bool acknowledges whole suffix completion only. The population
suffix is **not** implemented by this module, and no success is fabricated.

## Native ownership and tests

Actual memory, division, fctiwz, RNG and population owners remain stable through
holds. Division and conversion callbacks report their supported source numeric
domain; no zero divisor/conversion result is invented. Wrong service identity,
recursive resume and reset during providers are refused before source reads.
A missing provider can remain unavailable; changing its table during a held
call is not an admission mechanism. The outer runtime must retain actual VM
lifetime and schedule this one invocation from the real Turn parent. Native
reset discards metadata, not source stores or guest cleanup.

Read-only source extraction/evidence is recorded at ignored
`artifacts/audience-turn-end-source-proof.json`, with seven body SHA256s and
original multiplier/zero/bias bytes. The complete TurnEnd body hash is
`5de5cd55446c6dfbe4107e9791517657d0fd417cce5ecb2dbb545ae4d72939be`.
Listings under `artifacts/audience-turn-*-source.*` are inspection-only ISO
extracts. Original rights/provenance remain unchanged.

Tests use original Trio constants and a mutable guest-memory fixture. They
cover the source group threshold46, signed chance wrap/clamp, source suppression
load order, conditional Target/category6/category1, mandatory RNG and exact
category order/duplicates/count reload. Holds cover division, target conversion,
RNG, NumToTarget conversion and nonempty population without prefix replay;
provider identity/reentry and stable captured invocation are checked. Mutation
cases distinguish captured versus fresh turns/chance/globals, source GetExist
and GetPtr ownership, record flags/state and all three true early population
gates. These are source-derived host checks, not original execution, active
battle scheduling, native pixels, audio, hardware timing or performance proof.

## Native integration status

Commit e78d2325 binds actual VM numeric conversion/division and runtime RNG.
Only the original AddAudienceNum early gates bypass its still-unbound nonempty
population suffix. Legacy audienceTurnEnd is not retried as this source child.
See [validation and cadence limits](../../PROGRESS.md).
