# MAINLOOP-01 — marioMain per-retrace body and field dispatch (P0c)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md). Filed by the native implementation source audit on 2026-10-12 and moved here from the JUMP-01 section during the inbox split; text is verbatim.

Open todos:

- [x] Complete local `marioMain` request graph — [complete contract](../behavior/overworld/MARIO_MAIN_COMPLETE_REQUEST_GRAPH.md): active/input/badge gate, exact moon room gravity comparisons, timers, motion equalities/flag order, position ring and eight finish blocks. Corrected target identities replace the earlier sketch; no local battle-state5 assignment.
- [ ] Complete child mechanics and actual pointer/room/badge/input/motion/flag/ring domains; sequence5 ownership and original execution/native binding. Local caller coverage alone does not close these.
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

## 2026-10-13 — Complete local marioMain graph and source corrections

Delivered [complete request graph](../behavior/overworld/MARIO_MAIN_COMPLETE_REQUEST_GRAPH.md)
for the native 2DS XL field-loop dependency. Replaced the incorrect earlier
sketch with a corrected pointer/summary. Six supposed subsystem updates are
strcmp moon-room checks;801a938c is marioPreCamera,8002e218 is seqSetSeq,
800d3d30 is pouchGetHP. Eight conditional sequence requests assign(5,0,0)
after signed HP<=0, with no local battle-state5/parameter construction.
Exact motion groups, character-mode bypasses, flags, timer/ring/finish owners
and fresh reads are recorded; no provisional native cadence used as evidence.

Validation: read-only ISO verifier41 spans/654 anchors/100 calls; independent
all638-word coverage/35 child identities, complete65536-value motion-group and
32767-value timer remainder checks,34 mathematical vectors/eight finish sites/
room names/four floats/pins; Markdown links and scoped diff checks passed.
Docs/evidence only; no make test/all rerun. Temporary raw listings stay /tmp.
Child/provider/sequence/pause/cadence/original traces/full native binding remain
open; MAINLOOP-01 stays OPEN. Other sessions' native changes excluded.

## 2026-10-13 — Mario camera child request closure

[Camera shift/target/control contract](../behavior/camera/FIELD_MARIO_CAMERA_SHIFT_TARGETS_AND_CONTROLS.md)
now closes local marioCamMain/marioUpdateCam/four update-control bodies used by
the recovered field-loop graph.15 spans/294 anchors/17 calls and independent
290-word/27-vector/full s16 domain checks passed. Room camera consumers,
shift-rate/default producers, zoom/event/trig and original pause/cadence/native
binding remain open; MAINLOOP-01 stays OPEN. See OW-CAMERA-01 delivery.
