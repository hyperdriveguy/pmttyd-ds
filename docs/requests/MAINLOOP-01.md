# MAINLOOP-01 — marioMain per-retrace body and field dispatch (P0c)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

- [ ] Complete `marioMain` 8005a7e0 per-retrace body: motion dispatch 801a938c, the six subsystem updates 8026d464 at offsets 864-904, motion-ID threshold chains (0x8/0x14/0x18/0x19/0x1a/0x21) and their semantics, gravity single 1.0/moon 0.5 selection, position save-slot copies and counter 52, input gate (work+4920 mask 0x100f) and the battle-transition state5/seqMain 8002e218 condition and parameter construction.
- [ ] `marioStMain` retrace cadence plus pause/timebase interaction so the native fixed-60Hz accumulator can reproduce original pause behaviour. Pause/RNG sub-asks already filed under AC-01/EVT-01 remain open.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-12),
P0c main-loop/field-dispatch:marioMain8005a7e0 has no inbox request and
MARIO_MAIN.md remains an Evidence-B sketch whose own remaining-gaps list is
exactly what the native field loop needs. source/main.cpp runs a host fixed
60Hz accumulator(1./60,max5 steps,waiting for VBlank)and Game::tick/move/
startBattle substitute a simplified motion dispatch,there is no original
motion-ID→subsystem chain,no battle-transition parameter construction and no
input gate. Please recover the complete marioMain per-retrace body:the motion
dispatch801a938c and the six subsystem updates8026d464 at offsets864-904,the
motion-ID threshold chains(0x8/0x14/0x18/0x19/0x1a/0x21)and their semantics,
gravity single1.0/moon0.5 selection,position save-slot copies and counter52,
the input gate(work+4920 mask0x100f),and the battle-transition state5/seqMain
8002e218 condition and parameter construction. Also close marioStMain retrace
cadence plus pause/timebase interaction so the native accumulator can reproduce
original pause behaviour;GAME_OUTER_LOOP_AND_EVENT_UPDATE_ORDER.md currently
declares measured cadence/original traces open. No lock reserved;pause/RNG
sub-asks already filed under AC-01/EVT-01 remain open.
