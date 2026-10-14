# Native shared sound flag owner

[Original producer and exact call order](GAME_SOUND_FLAG_OWNER_AND_ORDERED_SET_CLEAR.md)
now drive `SourceSoundFlags`, a retained unsigned16-bit owner. Constructor zero
represents the explicit psndInit halfword store, not the whole original audio
bootstrap. No battle-entry or field/checkpoint reset clears this owner. Game
restore and successful staged native checkpoint replacement retain its identity.
Checkpoint files do not serialize original sound history; launching a new process
starts this native boot owner rather than reconstructing the old session flags.

Original ClearFlag is a complete leaf: read unsignedhalf, AND-clear requested
32-bit mask, store low16. It never starts streams or calls fades. SetFlag ORs
and stores once, captures mask1 admission, requests BGM fades groups0 then1
with(duration1000,flags0), reloads live half for mask2, and requests ENV fades
groups0 then1. A cursor preserves stores, branches and completed calls across
missing providers; fresh mask2 reload happens after both BGM calls. Upper mask
bits are discarded at the original halfword store.

Field battle-start0x10, item-get0x80 on/off and finishing letterbox0x20 use this
same owner. The previous uint32 harness word and startBattle reset were removed.
Current native writers preserve inherited masks1/2 clear, so these supported
SetFlag calls complete without downstream fades. Nonzero inherited gates stay
pending at genuine BGM/ENV fade service identities; their actual native group/
routing providers are not invented. The legacy field-battle void service adapter
is supported only in this masks1/2-clear domain; it does not implement event
suspension for arbitrary inherited fade calls. Item and letterbox callers retain
their explicit set cursors. Full original sound-writer census/history is open.

Trio finishing defaults to the shared sound provider: current4 disables the
letterbox, ClearFlag0x20 completes, signed scalar animation and the21st queue
registration execute, and real camDraw then waits at GX viewport802ba56c. It
retains20 previously collected records and the source Draw cursor; no fabricated
GPU success or direct battle teardown. Deliberately removing the leaf provider
in the integration test reproduces800dc0f8; restoring the actual owner advances
without an injected sequence/sound override or repeated collection/registration.

Host coverage includes width/truncation, inherited gates, all group order/args,
mutations across BGM/ENV callbacks, held branch/mask capture, clear leaf, native
item writers and restore identity. Validation logs/limits live in the progress
archive. Missing full battle Init/Main/Exit, deferred raster callbacks, original
Key/ability services, exact event timing, audible parity and physical performance
remain independent work. Scalar flags alone do not certify original audio.
