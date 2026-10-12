# Rogueport source story state and custom departure control

US G8ME01 revision 0; static source contract recovered 2026-10-12. Verify with
`python3 tools/verify_overworld_story_state.py`. The verifier pins complete
owner bodies, literal instruction anchors and the continue dispatch table. It
does not execute the original game or establish card-file acceptance.
Read [road callers](OVERWORLD_ROGUEPORT_ROADS.md),
[saved switch storage](EVENT_SAVED_SWITCH_STORAGE.md),
[event namespaces](EVENT_INTEGER_OPERAND_NAMESPACES.md) and
[harbor story bindings](STORY_HARBOR_OPENING_BINDINGS.md).

## Source GSW0 has independent persistent ownership

The operand `0xf5de0180` is GSW(0), read as the full 32-bit GP word at `+0x174`
by `swByteGet(0)` (8008582c). `swByteSet(0,value)` (80085850) preserves a full
word, unlike the byte-backed nonzero GSW indices. It is not a Boolean completion
flag and must not be inferred from native Crump completion or map membership.

`swInit` (80085980–800859f4) zeroes the global flag array at GP+178, global byte
array at GP+578, and local arrays at GP+d78/db8, then explicitly writes zero to
GP+174 at 800859dc–e4. `swReInit` (8008593c–8008597c) resets only the two local
arrays and preserves GSW0. Direct DOL calls to swInit include marioStInit,
loadMain and the special continue branch below; this is not a claim about
indirect or REL callers.

`cardWrite` (800b1e7c–800b2164) copies GP's first 0x13d8 bytes unchanged into
save-slot+8 (800b1f98–800b1ffc). `cardCopy2Main` (800b2388–800b2504) imports the
same 0x13d8 bytes from slot+8 into GP (800b23ac–800b23d8). Its subsequent
runtime-field restorations do not overwrite +174. Both locate a slot at
card buffer+0x2000+slotIndex*0x4000. Therefore the GSW0 word occupies slot+0x17c,
or buffer+0x217c+slotIndex*0x4000, in source big-endian word representation.
These are source buffer offsets, not authorization to bypass card checksums,
version checks, card I/O or the rest of the imported save state.

`continueGame` (800f39dc) imports using cardCopy2Main at 800f39fc. It then
switches on GSW(4) (800f3a74), bounded unsigned 0..24. Its 25-word dispatch table
at 8034c628 selects reset branch 800f3e90 for index 23. That branch calls swInit
at 800f3ecc and explicitly swByteSet(0,0) at 800f3ed0–d8. Ordinary import must
not be described as always resetting the story word.

## The current native harness has no recovered source story value

The source gor_00 initialization resumes different opening scenes for exact
values 2, 3, 4 and 5. Harbor source events write 3, then 4, then 5 before the
battle-success cleanup; a later source scene writes 6. Those distinct writes
are pinned in the harbor contract. They do not establish story 11, 12 or any
other word for a freely positioned native Rogueport harness. Likewise fresh
swInit's zero establishes engine initialization, not the source scenario that
would legitimately enter an arbitrary plaza at zero.

A source save import or an explicitly reproduced source scene/state owner must
supply this word. A native save that has never stored it remains unknown.
Road callbacks whose admission reads GSW0 remain dependent on that missing
owner; silently substituting a guessed word changes actual route/story branches.
This is an unresolved integration dependency, not an invented travel restriction.

## no00_custom requires field-event services, not a generic portal

At gor REL 20c14, the story==5 branch calls, in order:

1. evt_mario_wait_landon.
2. evt_mario_flag_onoff(1,0x100).
3. evt_mario_key_onoff(0).
4. evt_mario_get_pos(0,LW10,LW11,LW12).
5. Subtract 80 from LW12 (Z); call evt_mario_mov_pos2(LW10,LW12,FLOAT(150)).
6. evt_mario_dispflag_onoff(1,1), then evt_party_dispflg_onoff(1,1,1).
7. Set GW(10), operand 0xfaa2b58a, to zero; return LW0=1 (handled).

The callback returns LW0=0 outside that branch. The road caller consumes the
handled result and suppresses generic departure/map change. Event motion/wait
continuations and the harbor scene's shared GW10 consumer are required; this
sequence cannot be collapsed into a direct gor_01 room switch. GW10 belongs to
the current event context, not the persisted GSW array.

The following leaves are independently closed:

- evt_mario_flag_onoff (800e7638) ORs/AND-clears the supplied mask in Mario main
  word+0. Thus this call sets numerical bit 0x100; additional semantics require
  its consumer and must not be invented from the wrapper.
- evt_mario_key_onoff (800e7474) calls marioKeyOff then partyKeyOff for zero,
  and marioKeyOn then partyKeyOn for nonzero. marioKeyOff (8005c1f4) sets main
  bit 8 and increments byte+39 modulo 256. marioKeyOn (8005c1c0) decrements
  that byte and clears main bit 8 only when its signed result is zero. This is
  nested ownership, not a freely resettable Boolean. Both partyKeyOff/On
  (800b7688/8c) are original four-byte return-only bodies.
- evt_mario_dispflag_onoff (800e75a8) ORs/AND-clears the supplied mask in Mario
  display word+4. This call sets bit 1. Full render visibility ownership is
  separate from this literal mutation.
- evt_party_dispflg_onoff (800eba70) interprets raw first argument zero as the
  current party ID; nonzero selects ID 1. It obtains partyGetPtr, returns
  immediately if absent, reads the mask raw from argument two, resolves the
  third argument with evtGetValue and ORs/AND-clears Party+4. Consequently this
  particular (1,1,1) call targets party ID 1, not a generic current-partner slot.

The road verifier separately pins the shipped callback words and relocations.
This addendum does not claim to bind the remaining movement-continuation,
landing-wait, shared-flag consumer or save-import adapters in the native port.
