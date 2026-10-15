# Trio reward and ending state

[The runtime ending](../../source/battle/trio_attack.cpp) uses source selector
`B+0xf24=0x070000xx`; `B+0xf28` points to `0x2ac` bytes of work allocated from
the actual map arena. See [events/entry/damage](trio-events.md),
[reports/resources](battle-reports-resources.md), [audience sound](battle-audience-sound.md)
and [field session](field-session.md).

This is an immediate scoped state machine. Some state stores precede unavailable
services, so retaining child state does not establish safe replay of the whole
ending. Complete resource export or a finished fade does not prove field return.
Unsupported result, level-up, fog, party or engine-exit domains remain explicit.

| State suffix | Work and boundary |
| --- | --- |
| `00` | Clear source report bytes, require victory result 1, adjust audience, start shell-release root and allocate ending work |
| `05` / `06` | Preserve alternate party branch; require empty reservations, clear statuses 0–24, barrier/clap, data `0x3f` events and actual drop material |
| `08` / `09` | Require no fog, initialize EXP/icons, source camera requests and 120-visit hold |
| `0a` / `0b` | Start actual data `0x40` victory roots and PointGet; transfer remaining Star Points into pouch |
| `0c` / `0d` | Post-count wait, hide reward owner, soft-delete PointGet and wait before dropping effect token |
| `0e` / `10` | Preserve special battle/level-up branches; unsupported paths do not silently skip to ordinary teardown |
| `19` / `1a` / `1b` | Original win voice/pose, wait 60 visits, source type-42 pose requests |
| `1c` / `1d` / `1e` | Crowd-volume fade, field NPC flag `0x100`, fade duration 200ms, wait for actual fade completion |
| `1f` | One-time resource export/free/sequence prefix, then real Mario/party restoration; remaining engine/sequence/exit owner stays pending |

Suffixes and source count values are not render FPS measurements. The fade
service's duration is milliseconds but ticks against the supplied simulation
refresh rate. Requests retained in trace do not prove original camera, crowd,
voice, viewport or GPU service completion.

## Reward storage and counting

Ending `+8/+0xa` are total/remaining signed EXP halfwords; `+0xc` records level-up.
Initial stock EXP floors to 1 and caps at 100, mode 3 doubles it, level 99 or
higher zeroes it, and a final cap restores 100. The zero-EXP presentation branch
is outside the admitted path rather than replaced with a made-up reward.

Nineteen original icon slots represent ten tens and nine ones. They retain
original icon 404 art with opaque native handles, source position/FMA order and
scales. Shown alpha ramps by 64 to 255; the tens-slot phase counter caps at 16.
Hidden records reset alpha/phase but keep their handles. Animation clocks advance
for all slots once per admitted time while the owner flag is set, including hidden
icons. Hiding/deleting presentation does not erase records or fabricate an SDK
icon ABI. PointGet also has an opaque native token, not an original effect pointer.

Ordinary counting transfers one point every four source visits. Held A/B mask
`0x300` adds 1000 to the counter, admitting accelerated counting. The tick sound
is conditional on every eighth counter value, not every transferred point.
Crossing 100 Star Points clears the pouch count and marks level-up work. Source
level limits can terminate remaining points. Post-count delay depends on counter
history, followed by the source 60- or 5-visit soft-delete wait.

## Audio and field ownership

Audience audio uses the actual positional registry, original name/descriptor rows,
story and GP rate. Assigned keys need real handle/volume/stop providers. An
unassigned key records explicit native failure -1; optional zero remains a known
value, distinct from unavailable. Trace entries accompany these effects and do
not replace them. Original source constants and clamping boundaries remain intact.

`endingExported` protects only the completed prefix: unit resource export,
canonical pouch byte publication, ending-work free and sequence change. It does
not protect all later party constructor/appearance effects against replay after
an unavailable callback. Pouch publication preserves exact EXP/HP/FP writes;
it does not heal or rebuild inventory.

Field Mario and partner registries are borrowed real owners. The admitted fresh
Goombella path requires actual free slot, original resource/pose/definition and
material/light owners. It publishes Mario's party index only after first placement,
pose and mode. The original placement helper's scalar return is discarded while
its output vector and pose effects still matter. Fresh Hello repeats placement/HP
work rather than substituting a jump setup. Missing owners leave the original
pending token. Remaining engine `0x800f8e28` and complete sequence/exit services
are unfinished; no final field return is claimed here.
