# Field Mario pose handles and material provenance

US G8ME01 disc0 revision0, read-only DOL evidence, 2026-10-13. This closes
local selector, boot creation and relevant material-writer bodies for the
[party entry animation admission](FIELD_PARTY_ENTRY_ANIMATION_ADMISSION.md)
frontier. It does not establish every writer executed on a particular save or
replace the original animation resource/cache/backend domain.

## Selector and persistent owner

`marioAnimeId`800593fc/30 reads global Mario pointer8041e900, then WORD M+4.
If mask10000000 is nonzero it returns2; otherwise mask80000000 returns1;
otherwise0. Effect wins when both bits are set. It neither reads camera/yaw
nor resolves a pose name. These bits belong to WORD4, independently of the
WORD0 mask80000000 used for dot Mario.

The result indexes three signed WORD handles starting M22c:
0=M22c front;1=M230 rear;2=M234 effect. They are registry IDs, not descriptor
indices or resource pointers. EntryMain independently calls this selector for
material and lighting; each selects/read its source Mario handle anew. Preserve
that order instead of caching a constant or substituting Goombella's pose.

## Actual original boot creation

`marioInit`8005b5c8/488 allocates/zeros the field owner. At8005b63c it writes
M3d=0. At8005b658/680/6a8 it requests `animGroupBaseAsync`80039d0c with
(bank,2,0), independently selecting each bank through signed M3d and the
12-byte triplet table80415eb0. At8005b6b4/bc/c4 it writes handles-1 to
M22c/M230/M234. Async group request success does not itself install handles.

Actual creator `marioPoseInit`8005ba50/98 calls animPoseEntry8004176c three
times, each mode2, then stores each returned ID into M22c/M230/M234 in that
order (stores8005ba80/baa8/bad0). The initial Mario pointer remains the store
owner, while second/third selections reread global Mario and signed M3d.
There is no local prior-pose release, negative-return guard or rollback.
Ordinary M3d0 table contents are 802c18a0/18a8/18b4 = `a_mario`,
`a_mario_r`, `e_mario`. These are field resources; battle `b_mario` is a
different owner. Original resource identities and rear-selector producers are
in [ordinary Mario pose selection](../overworld/OVERWORLD_MARIO_ORDINARY_POSE_SELECTION.md).

A census of all loaded DOL text direct linked branches to8005ba50 finds one:
`seq_logoExit`8008746c/44 at80087490. Complete order is fontmgrTexSetup80077bdc,
windowTexSetup8007fbd8, fadeInit80013aa4, iconInit8001b694, envInit8019df84,
effTexSetup8005ee0c, marioPoseInit, then fileFree8018a260 of the logo file
handle. This does not exclude REL/indirect calls; MarioInit does not directly
call marioPoseInit. A native bootstrap must supply actual initialized pose
registry records before constructor material reads, and cannot call an
unloaded bank successful by assigning handle0.

## Relevant material and light writers

Fresh animPoseEntry initializes per-pose materialE8=0 and lightEC=0; this is
a creation value rather than a lifetime invariant. The following writes
change those records, not fields embedded in Mario itself.

* `marioReInit`8005b1d8/3f0: individually tests each signed handle>=0,
  then MaterialFlagOff(handle,ffffffff) at8005b40c/420/434; afterwards
  individually tests again and MaterialLightFlagOn(handle,2) at8005b448/45c/470.
  Material becomes0 and light gains2, preserving other preexisting light bits.
  The owner memset performed elsewhere in this body does not zero the pose
  registry. No new pose creation occurs in these six calls.
* `marioSetCharMode`8005d740/970 ordinary restoration branch: at8005d97c..9b8
  independently selects the old handle by WORD4 and captures its materialE8;
  at8005d9bc..9f8 rereads WORD4 and captures lightEC. After creating the three
  mode2 normal-bank poses, it MaterialFlagOn(savedE8) on all three at
  8005db60/6c/78, then MaterialLightFlagOn(savedEC) on all three at
  8005db84/90/9c. These operands are captured old values, not fixed0 or2.
  This bounded branch must not stand in for the entire character-mode switch.
* `door_dark_flag_sub`800e85f4/258 captures Mario and both registered party
  pointers, then branches on its first integer argument. Nonzero ORs
  materialmask1800 into all three Mario poses and each nonnull party pose;
  zero clears1800. Mario pointer-null skips its trio; there is no signed
  handle validation here. Calls800e864c/658/664 and800e874c/758/764.
  Optional NPC/other actor lists also change their pose material; the map
  list changes object flag40. Those consumers do not mutate Mario lightEC.
* `evt_map_blend_set_flag`800388b0/1d8 evaluates three arguments in order:
  onoff,key,mask. For key `mario`, onoff==1 ORs the mask into all three
  handles; every other onoff clears it. EACH handle is skipped only if
  exactly-1, not for every negative value. Source key comparison uses
  string8041fa4c. Party/NPC key branches have separate owners.
* `npc_light`800e8da4/17c clears1800; `npc_dark`800e8f20/17c ORs1800 on
  the same Mario/party/NPC/map pose family. Preserve a source quirk: after
  capturing Mario in r31, both bodies overwrite r31 with slot1 PartyPtr
  before the guard at800e8dec/800e8f6c. Thus Mario's three writes are gated
  on slot1 nonnull; their individual handles are not validated. Slot0 and
  slot1 party writes have their own pointer guards. Do not silently repair
  this guard to Mario!=null. Each wrapper evaluates its one event operand
  before captures and returns2 after completing the loops.

Neither the initial EC0 nor ReInit EC|2 proves the current material flags at
underground battle return. Exact upstream ReInit/map-door execution and all
used light/blur/paper producers must be replayed or retained in a persistent
pose owner. Other direct DOL callers include evt_light_mario_lighting_onoff,
marioPaperLightOff, marioDispBlur/Sub and menu services. Their existence is
not evidence they ran in the fresh prologue. The current constructor should
retain an explicit source-material-owner frontier until that domain is
provided; source MarioAnimeId itself needs only the persistent WORD4.

## Evidence and scope

SHA256 of complete raw original DOL bodies (address/size, hexadecimal):

| Body | Address/size | SHA256 |
| --- | --- | --- |
| marioAnimeId | 800593fc/30 | 7133e6401056ec9fdba7131f9186e7e7da5228e7207bba72dd39f269c1c0f71c |
| marioPoseInit | 8005ba50/98 | 08c3dce0e69c0c1e23422a3dbdfb3aabe5fc117761965731e76f5c30260f304a |
| marioInit | 8005b5c8/488 | e3976200ee5fc18a245b234f0b88c4095a0de3108c620034b54df1ea545ee6ce |
| marioReInit | 8005b1d8/3f0 | 768a85127d313dcaf6f86c3cbc9b9cc6c42fa3e4f826f72aa494af13ce81f661 |
| marioSetCharMode | 8005d740/970 | 846eaffc30a37986c589881f242709b697684812fcd4d2b1a54b0a3a53b13976 |
| seq_logoExit | 8008746c/44 | 54b7691885ff90c84e987575559e093c599c4c125b11f62e9cc6358bb2da3500 |
| door_dark_flag_sub | 800e85f4/258 | 4fe219b39f3e0999c1cb002c79cd84346ecbbcdd1376a86b8911ac4fa34772be |
| npc_light | 800e8da4/17c | 3c0b9ebcaaaf817f805be24a298b4b7e65367c2c9a320beba9050835887961ec |
| npc_dark | 800e8f20/17c | fceacd6e1c588e2ef2a5283fe88f23869ac03f6ebf03cc99b7f884045b91e207 |
| evt_map_blend_set_flag | 800388b0/1d8 | f84039534e352fa039f3598f3ac9677f822f6bb5791a6b67162ff807d6fa5935 |

Table80415eb0/0c raw802c18a0802c18a8802c18b4 SHA256
8a7a8b512cf150345386614e31c586a9304ce2c1d3358bb02f56d1cc6451e838.
Inspection-only ELF/listings under ignored artifacts/recovered-temp-evidence/
mario-material-*. No original binary/assets committed. Checked local branches,
operands and complete-body hashes; documentation only, no native/test edits,
no make test/all rerun, no shared progress/index edits or commit. This delivery
follows the retained EVT-01 EntryMain camera/resource request rather than
claiming that transitive material/raster providers are complete.
