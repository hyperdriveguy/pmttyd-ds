# Save-block restart clock publication and exact event identities

US G8ME01 disc0 revision0;2026-10-13. Read-only static DOL/model evidence,
no original execution. Complements
[hit lifecycle](../field/SAVE_BLOCK_HEAD_CONTACT_AND_HIT_LIFECYCLE.md) and
[common save graph](../field/SAVE_BLOCK_COMMON_SAVE_EVENT_AND_CHOICES.md).
No original instruction/event payload bytes are exported into this document.

## Restart and first animation Main publication

`animPoseSetAnim`80041170/0x17c, used with restart1 at both save-block
A_1 and S_1 requests, follows the complete
[selector/restart contract](ANIMATION_SELECTOR_AND_RESTART.md). Resource
`a/MOBJ_SaveBlock` (hash in [UV/material binding](SAVE_BLOCK_ANIMATED_UV_AND_MATERIAL_BINDING.md))
has S_1 nameindex0/serialized descriptor0x3f00 and A_1 nameindex1/descriptor0x5b44.
S_1 control is selector1/start1/end119; A_1 selector0/start0/end15.

For either forced restart:

1. Publish ordinary animation indexpose+0x14, even if the same name was set.
2. Admit the documented linked-paper flag update when linkedID!=-1 and
   poseflags mask2 is set. Ordinary save source has its separate initial pose.
3. Read the source clock quotientQ and store LOW timestamp atpose+0x1c,
   then HIGH at+0x18.
4. Write cacheindex+0x3c=-1, fractioncache+0x40=positive0,
   discretefield+0x80=0, loopratio+0x84=positive0.

The setter does not call animation Main or increment a frame. It does not
clear elapsed+0x20, selectedtime+0x24, current/nextkey indices+0x28/+0x2c,
outputgeometry buffers or localrate+0x44. The loopratio query immediately
returns0 after restart even while other prior sample fields still exist.
Do not infer fresh A_1 geometry solely from the setter's index/reset writes.

[Clock requests](ANIMATION_LOCAL_TIME_AND_CLOCK_REQUESTS.md) select GP HIGH+0x40/
LOW+0x44 when posemode+0xc==0, otherwise GP HIGH+0x38/LOW+0x3c;
compute source hardware divisor, then request64-bit quotient. First subsequent
[Main](ANIMATION_MAIN_ENTRY_AND_TIME.md) reads the clock AGAIN and subtracts
restarttimestamp modulo64bits. It requests unsigned64→float conversion,
separately rounds60*elapsed/1000 and multiplies the retained localrate intoX.
These are source clock/rate requests, not a proven rendering-frame counter.

Let R denote binary32 rounding and valid finite rate1/elapsed be the bounded
normal domain. First/continuing Main publications:

| Clip | Elapsed domain | pose+0x84 ratio | pose+0x24 selectedtime |
| --- | --- | --- | --- |
| A_1 | X<0 | 0 | 0 |
| A_1 | 0<=X<=15 | R(X/15) | X |
| A_1 | X>15 | R(X/15) | 15 |
| S_1 | X<1 | 0 | 1 |
| S_1 | X>=1 | R(R(X−1)/118) | fused X−trunc(ratio)*118 |

If restart and firstMain observe identical source quotient, elapsed0:
A_1 selected0/ratio0; S_1 selected1/ratio0. This is an explicit equal-clock
condition, not a claim that every first Main sees zero wall-clock advancement.
Main's restart cacheindex-1 forces base reset and replay of keys0..current
inclusive. A_1 key0 and S_1 key1 are their respective authored starts.
Texture state remains discrete cumulative; node interpolation is independent.

A_1 ratio1 corresponds source X15 even when outputclamps; later ratio can
exceed1. `animPoseGetLoopTimes`8004079c/0x18 reads+0x84 directly. The save
callback's state1 tests ordered ratio<1 before proceeding; it runs BEFORE
same-iteration animationMain in mobjMain. Preserve previous-publication query
ownership rather than restarting and advancing a source completion counter
within one callback. Fixed simulation ticks are an explicit native clock adapter.

## Exact event-array byte spans and hashes

Addresses below are original DOL loaded data, half-open spans. All bytes were
reread from the supplied disc; no truncation at a name-table annotation or at
the first RETURN is assumed. Child contains several RETURN branches followed
by later commands, and its full terminal END is retained.

| Stream | Start | Exclusive end | Length | Commands | SHA256 |
| --- | --- | --- | --- | --- | --- |
| mobj_save_blk_sysevt | 8030b4ec | 8030b590 | 164 (0xa4) | 16 | `a580e9894d07ae58b8cb24d35b825df91eb9b3d42e003f84cd4e5a15dcaa3752` |
| unk_evt_803bac3c | 803bac3c | 803bac70 | 52 (0x34) | 8 | `b3467d3aa54cf4ec1ff2b9f90de94862d99c4325e5d8805775b4448a95b39b1e` |
| evt_memcard_save | 803ba444 | 803bac3c | 2040 (0x7f8) | 197 | `fd79ccecf3d3d6f041a8e7ab14f6d78776eeb0a642a83a6ad72d877ca6123dac` |

RUN_CHILD links are system→wrapper and wrapper→child at BOTH IF/ELSE sites.
All direct USER_FUNC91 addresses across the three streams, with static encoded
site counts (not execution counts):

| Callback address | Source identity | System sites | Wrapper sites | Child sites |
| --- | --- | --- | --- | --- |
| 800d2090 | evt_msg_select | 0 | 0 | 10 |
| 800d21fc | evt_msg_continue | 0 | 0 | 4 |
| 800d23c4 | evt_msg_print_add | 0 | 0 | 49 |
| 800d284c | evt_msg_print | 1 | 0 | 3 |
| 800e7474 | evt_mario_key_onoff | 2 | 0 | 0 |
| 8025c0b8 | memcard_write | 0 | 0 | 3 |
| 8025c188 | memcard_ipl | 0 | 0 | 1 |
| 8025c19c | memcard_header_write | 0 | 0 | 1 |
| 8025c2bc | memcard_create | 0 | 0 | 2 |
| 8025c2fc | memcard_format | 0 | 0 | 1 |
| 8025c380 | unk_8025c380 (source card cleanup request) | 0 | 0 | 1 |
| 8025c3a4 | unk_8025c3a4 (source two-bit guard/output request) | 0 | 0 | 3 |
| 8025c40c | unk_8025c40c (source recovery-arm request) | 0 | 0 | 2 |
| 8025c454 | memcard_code | 0 | 0 | 6 |

Fourteen unique USER_FUNC callbacks/89 encoded sites total. Wrapper has no
USER_FUNC callback, only VM comparison/child/wait/return. Names startingunk
remain unresolved source symbols; descriptive local roles are from the common
save contract and must not be treated as recovered external API names. General
VM opcode handlers, transitive helper calls and message/card services are not
additional direct callback sites in these three arrays.

## Validation and boundaries

Fresh read-only DOL span SHA256 computation independently reproduces allthree
hashes. `dol_event_inventory.py` extraction pins221commands and directly
enumerates callback sites without exporting original payloads to source.
`verify_animation_selection_time.py` passes5 containing bodies/423 reviewed
words/2constants/2spans/299 direct sites;
`verify_animation_main.py` passes2 bodies/1773words/17constants/5spans/65sites.
Fresh animPoseSetAnim listing remains /tmp/sb-restart.*; stream/callback witness
uses /tmp/sb-common-save.json and /tmp/sb-save-flow.json. These verifiers pin
source dependencies; they do not execute the save object or establish original
millisecond/retrace traces. Original Nintendo/Intelligent Systems rights remain;
no asset redistribution is implied. Docs-only: no native edits/build/test,
Dolphin or hardware claim. Native source event dispatch, rate/clock adapter,
original card services and full story/flag persistence remain open.
