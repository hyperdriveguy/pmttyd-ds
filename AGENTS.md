# Project handoff instructions

Read README.md and docs/PROGRESS.md before changing code. Continue the native
New 2DS XL remake; the current deliverable is an early Rogueport slice. User
preference: original-looking art, original music and sound effects. Use original
ISO assets, with texture scaling, geometry reduction and lighting adjustments as
needed. The initial vector placeholders and generated tones were rejected.

Open /home/carson/games/roms/G8ME01.iso read-only. Track editable original and custom assets under assets/ with provenance and
per-asset licensing. Keep temporary extraction caches and compiled packs outside
source history. Do not distribute or commit the ISO.
Record asset paths, conversion choices and licensing in docs/ASSETS.md.
Normal builds compile assets/manifest.json after the ISO-backed base pack. Never
re-import over checked-in asset edits; new imports require an empty destination.
Original rights are retained; tracking a source asset grants no new license.

Behavior contracts live under docs/behavior/ in per-subsystem folders (battle/,
event/, field/, partner/, message/, format/ and others; see
docs/behavior/README.md). Add new contracts to the folder their subsystem owns,
keep links relative to that location, and do not leave new files at the
docs/behavior root. Only the README.md index stays at that root.

Separate host-testable mechanics from 3DS services. Keep action-command timing
in simulation ticks, not rendering frames. Do not describe provisional constants
as measured behavior. Record Dolphin observations in docs/MECHANICS.md.

Run make test and make all for meaningful code changes. Azahar lives at
/usr/bin/azahar. Use tools/emulator_smoke.py for an isolated profile; display
socket access may require sandbox escalation. Never modify the user's emulator
profile or saves as part of automated testing.

At session end, update docs/PROGRESS.md with completed work, exact validation,
limitations, and next tasks. Full-game work spans sessions; do not report the
remake complete when only a prototype works. Do not delegate to sub-agents unless
the user explicitly asks for delegation.

Story fidelity is mandatory: reproduce the original US English dialogue and
story, including character attribution, order, branches and event conditions.
Extract message text from msg/US in the read-only ISO; retain message IDs and
control tokens. Do not paraphrase, rewrite, invent dialogue, or replace plot
scenes with fabricated scenes. Screen reflow and adapted control prompts may
change presentation only. Development harness notices are not story dialogue.

This project is git versioned. Make incremental commits between working on features and incremental steps.

When running out of tokens, give the human user handoff next steps as well.