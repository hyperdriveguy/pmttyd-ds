# Jump SFX provenance (vec3 scratch, sound key, invocations)

US G8ME01 disc0 revision0; static contract, no execution or gameplay claim.
Answers native JUMP-01 request 1 (packed Jump provenance) in
`docs/DOCUMENTATION_REQUESTS.md` using read-only artifacts.

## (a) The 0x802EE470/474/478 block: zero scratch, not a key row

- `us_symbols.csv`: `vec3_802ee470`, battle_event_cmd.o, .rodata, 12 bytes,
  type vec3, value 0.0.
- ISO read through the DOL section map (section 9) at both the file offset
  (dol+0x2eb470) and the mapped address: 16 zero bytes. The slot is zero at
  rest — it cannot be a constant attack-sound key row.
- No reference body reads it by absolute address: the packed
  `btlevtcmd_JumpPosition` listing (311 lines) contains no 0x802E `lis`
  (only 0x802F bias pairs), and no other reference listing names it. Any
  runtime contents come from writers unidentified in the reference corpus.
- Its neighbors ARE read once: at the first SFX call below, words at
  0x802EE458/45C/460 (24/20/16 bytes before the vec3) load into r6/r5/r0 —
  but each is overwritten (by unit floats, see below) before the call, so
  those loads are dead. The vec3 itself is never referenced.

## (b) Sound-key domain and the two psndSFXOn_3D records

`BtlUnit_LoadSeMode` (0x80126588, battle_unit.o) is called with the unit's
+0x18C word as a pointer argument; [unit-sound-mode](BATTLE_UNIT_SOUND_MODE.md)
already establishes it copies U+0x1C4/U+0x1C8 to U+0x18C/U+0x190 for type 2.
This body then reads word U+0x18C (`lwz 3, 396(30)`):

- Zero → skip to the post-SFX join (0x8010d598).
- Nonzero → first `psndSFXOn_3D` (0x800d9848, pmario_sound.o) at 0x8010d594
  with r3 = the key, r4 = stack block sp+56 holding bit-copies of the unit
  floats +340/+344/+348 (the request's "baseline position pointer
  (unit+154–15c)": 0x154 = 340, 0x15C = 348), r5/r6 = the same bits
  reloaded, r7 = the neighbor-table pointer, r0 = a neighbor word. The
  unit floats parked at sp+32/36/40 are dead (nothing reads them for the
  call).
- Second `psndSFXOn_3D` at 0x8010d690: no r3 reload exists between the two
  calls, so it inherits the first call's r3-out as its key; its position
  block rebuilds from [unit+360] plus stack residues. Point-in-time for
  both: inside `JumpPosition` after `LoadSeMode` and the position build —
  no landing/terminal cue exists in this body (it returns 1/0 event codes
  after). Return-value handling and any later cue are not in this body.

## (c) No Gekko PS arithmetic in this body

The only paired-single instructions are `psq_st`/`psq_l` register
spill/restore in the prologue/epilogue. Trigonometry arrives via
`system_angleABf` (0x8000703c), `distABf` (0x8000722c), and same-TU helpers.
The request's "PS-family sin/cos" premise is therefore not supported for
801093a8: there is no PS math op whose identity or FPSCR entry state could
be read here. FPSCR state at entry is unobservable statically.

## Not established

Runtime writers of the vec3 scratch, `psndSFXOn_3D`'s full signature,
sound-key value domain beyond nonzero-gating, and any audio output or
cadence.
