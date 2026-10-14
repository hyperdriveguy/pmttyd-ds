# Native BeroEXEC owner and road adapter

A retained shared `FieldBeroExec` now represents original startup0 and the
[resolved OR/AND-clear writer](BERO_EXEC_RETAINED_PRODUCERS_AND_PROLOGUE_TRANSITIONS.md).
The event wrapper captures the argument pointer, reads/resolves operation then
mask, and only then acquires the live native owner. Source nonzero operation
clears, zero sets; all32 bits survive. get_info reset is separately exposed
and must only run after its actual providers complete, never on arbitrary restore.

The owner tracks which bits have known provenance. Unknown checkpoint/transition
history cannot become a fabricated0. A resolved set/clear determines its own mask
bits without claiming the untouched bits. Whole-word reads require all32 known;
masked reads require exactly their requested bits. Native letterbox consumes only
original `(BeroEXEC&1)`, so its typed query bridge can use the exact known bit1
projection without guessing other bits or a whole current word.

Game retains the shared owner across restore, then invalidates unsaved provenance.
Original Bero value bits are not reset by this bookkeeping. Native generic-road
adapter publishes the documented literal requests at its existing milestones:
admitted contact clears10000/sets1; independent inline completion sets10000/clears1;
parent map request independently sets8, including while the inline waits airborne;
arrival begin sets1, and completed travel/case registration clears1/sets10000.
These are scoped adapters to the documented shared script requests, not proof of
full original init-check/NPC/child/timing execution. Existing custom/unresolved
road policies remain separate and do not gain completion from the word stores.

Native cancellation has no recovered clear counterpart and invalidates provenance.
Interior mask choreography lacks the shared original Bero child and invalidates
provenance while active. Restore is not get_info; no false room-table reset is
inserted. The actual full room Bero table/HitObj provider and remaining event
writers still need binding before full global-word history can be certified.

Trio uses this shared owner at its real letterbox Bero wait. Unknown bit1 holds
8007ac40. A genuine resolved clear restores the known bit and resumes the source
consumer, then exposes the separate current-sequence wait8002e210. Game.fieldSequence
is not substituted for that original lifecycle. Explicit letterbox services remain
available for fixtures. Native GPU/field return remain unfinished.

The documented Key39/Ctrl38 mismatch is also corrected: native idle Key service
now reads signed keyDepth(byte39), independently of controlDepth(byte38).
The low-HP idle test exercises nested KeyOff while Ctrl remains0. Other Key
ability/party/sequence adapters remain provisional as described in the
[global-producer audit](../camera/CAMERA_LETTERBOX_GLOBAL_PRODUCERS.md).

Tests cover startup/masks/highbits/negative-operation, unknown/masked knowledge,
get_info-only reset, captured args and fresh post-resolver owner mutation,
checkpoint invalidation, Key39 gating and real finishing resume through Bero to
the independent missing current sequence. Exact native travel captures and check
limits belong in PROGRESS.md; no full source history or original trace is claimed.
