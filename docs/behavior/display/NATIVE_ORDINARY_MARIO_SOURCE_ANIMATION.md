# Ordinary native Mario source animation

The native field process creates one `NativeFieldAnimation` before creating
Mario's three poses. It stages unchanged original MODEL/TPL files, runs the
existing cold `AnimationStartupProcess`, then enters a_mario, a_mario_r and
e_mario in source order with mode2. Handles are returned by source Entry and
retained by `FieldPartyRegistry`; they are never assigned by presumed indices.
The normal variant's names, triplet and frame-to-millisecond factor are pinned
original DOL data in animation-boot. No ISO or generated pack belongs in Git.

Each simulation visit publishes one rational 60Hz sample to the original
retrace clock prefix. Native policy supplies 162MHz bus/4 and 675000 raw ticks
per visit; these are adapter inputs, not measured original execution. GP38/3c
and GP40/44 use the original freeze/battle gates and signed integer division.
Drawing, texture import, room changes and checkpoint admission do not advance
these clocks. Rendering has no independent animation sampler for this path.

Mario's retained presenter chooses front/rear/effect and counts pending uploads,
including equal-name forced resets. Effect flag10000000 wins over rear80000000.
An admitted upload forces the exact source descriptor, then applies unlinked
SetLocalTime: binary32 frame*original41855555, finite unsigned64 conversion,
wrapped quotient-minus-localtime, low-before-high epoch stores and paper copy.
Main samples actual BE scratch/control/key streams. Inactive handles preserve
their epochs. Unknown converted clips remain unavailable.

The renderer consumes this simulation sample's vertices, colors and material
batches, and imports the matching bank's atlas outside the GPU frame. Each bank
uses its own exported UV scale; a combined legacy atlas is incompatible.
Native and legacy Mario GPU textures are mutually exclusive. After FrameSync,
the ordinary source path releases the legacy atlas before importing its source
bank; nonnative scenes and frontend entry restore the legacy texture. Keeping
both caused an actual front-bank import failure in isolated ARM validation.
Atlas conversion retains original image cells with 512-wide downscaling and
linear filtering. PICA draws use the prepared pass/alpha/depth/cull metadata.
This is the retained CPU mesh adapter, not complete original DisplayQueue,
linked traversal, paper-facing, shadow or lighting equivalence.

Copies used for speculative room admission freeze animation buffers and omit
borrowing source services. After room providers accept, existing role views
reconnect to the original process only when generation/resource/scalars match.
Mario's intentional material-clear/light|2 reinit is allowed and applied only
after admission. Checkpoint loads retain the process/handles/clock and prepare
the new ordinary sample before its first visible frame. No cold restart or
new pose allocation happens on a room/checkpoint handoff.

This step does not activate Goombella's source NPC or party presentation.
Her harbor NPC, auxiliary kind13 and canonical kind1 have distinct event
lifetimes. NPC flag8 still requires the real camera-facing branch; it must not
be cleared to make a sample appear available. Existing original-looking actor
paths continue while those owners are implemented. Original animation release,
full Mario parent memory, VI/window scheduling, physical performance and visual
parity also remain open.

See [source scratch ownership](NATIVE_SOURCE_ANIMATION_SCRATCH.md)
and [request inbox](../../DOCUMENTATION_REQUESTS.md). Read-only activation
notes/disassembly remain under the ignored
`artifacts/animation-field-activation-evidence/` directory.
