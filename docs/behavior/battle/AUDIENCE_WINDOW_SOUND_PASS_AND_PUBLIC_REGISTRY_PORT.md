# Audience window, sound pass, and public registry port

2026-10-14, original US G8ME01. Portable mechanics compiled for host and3DS;
the full AudienceMain provider is not yet bound into the native completion
pass. The existing ending still holds at801a836c. This is not a display/audio
backend or completed battle-return claim.

The source contracts are [live audience operations](AUDIENCE_LIVE_SOUND_WINDOW_THROW_AND_DISPLAY_ADMISSION.md),
[next slot/display owners](ENDING_NEXT_SLOT_DISPLAY_AND_PUBLIC_SOUND_REGISTRY.md),
[sound accessors](../audio/BATTLE_POSITIONAL_SFX_SLOT_ACCESSORS.md), and
[sound stop](../audio/BATTLE_POSITIONAL_SFX_OFF.md).

`packedBattleAudienceWindowMain` implements WinCtrlProcess801a4570 on actual
packed owners: BaseGetPtr and independent WinGetPtr, signed count follower,
fresh post-store count clamp, wrapping timer with only the source branch's
bound, and original mode4 polynomial/constants. Explicit single rounding
boundaries avoid fused host arithmetic changing source interpolation. This
updates window state; the queued Win display callback remains separate.

`PositionalSfxRegistry` owns40 typed records. Construction supplies boot-zero
fields and the public psndInit writes key/engine=-1,flags=0. Reinitialization
preserves generation, volume, distance, pitch, position and global allocation
counter. Chk validates generation and key; volume/distance/Off deliberately
do not validate generation. Handle0 can be valid. Out-of-bank indices retain
an unavailable-provider result instead of claiming the source returns-1.
Pending Off clears key/engine without engine stop; started Off selects SS,
song or effect by original descriptor bits, requests its actual provider, then
clears. An unavailable stop preserves the slot. No synthetic On allocation,
voice provisioning, source address ABI or NDSP-channel equivalence is supplied.
This owner must persist in the real global audio lifetime when integrated.

`packedBattleAudienceSoundMain` captures the initial BaseGetPtr, calls a real
NoiseMain provider, then gets and walks24 current records without a flags gate.
It checks each public handle, clears only bit1 on rejected handles, stops
zero-lifetime handles, runs explicit ramps before independently admitted
positive-lifetime fades, and decrements only positive lifetimes. Ramp and fade
share their source fields, so the fade's start/end writes affect the following
tick's ramp. Mode0 preserves single rounding; caller conversion is an explicit
provider, including exceptional fctiwz domains. Failed providers retain earlier
stores and stop the pass. Native NoiseMain, Ctrl/throw, item/AP providers and
the persistent global sound owner integration remain next work.

Host tests use packed memory and the real typed checker, including initialized
raw0 records sharing a public slot, a stop invalidating later aliases, flags0
with active clocks, simultaneous ramp/fade, lifetime-1, owner changes during
NoiseMain, unavailable providers, generation65535, stale handles accepted by
non-Chk accessors, public reinitialization retention, stop descriptor priority,
count reload and asymmetric timer bounds/wrap. Native compilation validates
portability; no emulator or physical audio/render parity claim is made here.
