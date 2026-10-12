# Mario peak-Y publisher

US G8ME01 disc0 revision0; static contract, no execution, cadence or gameplay
claim. `marioSetPeakYpos` at `0x800968f0` (`mario_sbr.o`, size `0x28`; listing
`artifacts/reference/peakY.disasm`, 10 lines, two instructions plus prologue
and epilogue). The pinned snapshot has **no** definition for it.

## Body

```
800968fc: bl 0x8005e0c0          ; marioGetPtr()
80096900: lfs 0, 144(3)          ; f0 = float [Mario+0x90]
80096904: stfs 0, 300(3)         ; float [Mario+0x12C] = f0
```

The whole effect is a single unconditional copy:
`float [Mario+0x12C] = float [Mario+0x90]`.

| Address | Meaning | Evidence |
| --- | --- | --- |
| `Mario+0x90` | `position.y` of the `Vec position` at `+0x8C` | pinned MarioWork header |
| `Mario+0x12C` | unnamed retained slot inside `unk104[0x134-0x104]` | pinned header, name not published |

There is no comparison, clamp or flag test in this body: "peak" is enforced by
the callers, which invoke it at apex transitions, not by this routine.

## Callers (direct DOL `bl` census, 10 sites)

| Site | Caller |
| --- | --- |
| `0x80093e44` | `marioJump` (`mario_motion.o`) |
| `0x80099948` | `mot_jabara` |
| `0x8009ba70` | `mot_jumpNpc` |
| `0x8009be34` | `mot_jumpSw` |
| `0x8009c144` | `mot_jumpStand` |
| `0x8009c1cc` | `mot_jumpStand` |
| `0x800b5f10` | `yoshi_use` (`party_yoshi.o`) |
| `0x800b5f94` | `yoshi_use` |
| `0x801fe564` | `kpa_jumpNpc` (`koopa.o`) |
| `0x80218ba8` | `kpa_jump` (`koopa_motion.o`) |

The `marioJump` site is already recorded in
[Airborne and landing](OVERWORLD_AIRBORNE_AND_LANDING.md): at the first
nonpositive integrated `V` it publishes current `+0x50` into peak counter
`+0x52`, calls this routine, then integrates the newly selected fall tuple.
So the apex publication precedes the first fall integration in that caller.

## Not established

- Read side of `+0x12C`: no consumer body is documented here.
- Whether every caller writes the slot only at apex transitions (the body
  cannot enforce that), and what happens when several motions interleave.
- Cadence, units, and any on-screen effect.
