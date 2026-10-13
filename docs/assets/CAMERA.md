# Camera asset provenance

Camera lifecycle evidence, the original camera container pack and the dated camera native-binding entries. Split out of [ASSETS.md](../ASSETS.md) on 2026-10-12; sections are verbatim.

## Camera lifecycle evidence and unchanged visual assets (2026-10-12)

Camera commit dff51f1 changes portable target/heading/projection ownership only;
no map/art/audio conversion choices or asset pack ABI changed. Original camera
containers remain read-only source evidence,not newly compiled camera-road data.
The original map/hit loader's parent scale10 confirms current authored-map*.1
conversion corresponds to engine*.01 native mechanics (5d86268). Source evidence
verifiers read G8ME01.iso read-only;no original executable/assets or generated
captures were staged. Isolated camera captures remain under ignored
artifacts/azahar-camera-ceiling/ and artifacts/azahar-camera-roads/. Original
assets retain Nintendo/Intelligent Systems copyright;local derivative use grants
no redistribution license. Camera-road view/curve/lens binding remains unfinished.

ID1 placement4 evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied the DOL
partyGetAppearPos4 and selected fall caller instructions,angle data
0x802cc430..0x802cc51c,query templates,bias,map identifier and existing named
support table. Only addresses/hashes/constants/analysis metadata were added to
source history;original ELF/disassembly/report remain ignored under
artifacts/reference/behavior. No story text extraction or new conversion policy
was introduced. make all invoked the existing sound/proj/pmario.proj,pool,sdir,
samp audio-map pipeline:3218 GameCube identities,0 Switch,5 slice primary-PCM
fallbacks packed. Existing local-assets/audio-map raw/catalog outputs and RomFS
WAV/audio-map.json derivatives remain unstaged. Original copyright/ownership
remains with its owners;local possession grants no redistribution license.
Exact build log:/tmp/id1-placement4-build.log.

ID1 general-placement evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
partyGetAppearPos and listed movement/trig/plane/query leaves,selected
placement4 caller,and sixteen zero-origin/direction query templates at
0x802cc1cc..0x802cc34c. Source-history evidence is addresses,hashes,constants,
word metadata and analysis;original ELF/listing/reports remain ignored under
artifacts/reference/behavior. No dialogue extraction or new ID1 conversion
policy was introduced. make all invoked existing sound/proj/pmario.proj,pool,
sdir,samp pipeline:3218 GameCube identities,0 Switch,5 slice primary-PCM
fallbacks. Ignored local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json
derivatives stay unstaged. Original rights remain with their owners;this local
analysis/conversion provides no redistribution license.
Exact build log:/tmp/id1-general-placement-build.log.

## Original camera container pack (2026-10-12)

Camera import2ea1c21 reads /home/carson/games/roms/G8ME01.iso read-only and
extracts m/gor_01/c to local-assets/gor_01.c and romfs/map.camera;
m/gor_00/c,m/gor_02/c,m/gor_03/c,m/gor_04/c go to corresponding
local-assets/gor_XX.c and romfs/gor_XX.camera. File sizes/record counts are gor_00:2048/3,gor_01:7700/11,gor_02:11316/14,
gor_03:11392/14,and gor_04:1304/2. All five packed files match their ISO
extents byte-for-byte. Per-file .provenance.json records original disc path,
FST offset,size and hash;romfs/manifest.json retains those extraction records
and camera_containers source-path mappings. No geometry/field endian conversion
or coordinate scaling is applied to these immutable files. Portable native
readers interpret big-endian fields and rawfile offsets on demand;authored
coordinates convert only at documented runtime boundaries. Both absent pointer
sentinels and every unresolved record/field remain preserved.

The asset fingerprint regeneration also reran existing map/character/texture/
original-music and sound-bank conversion with their established choices;no art
or audio substitute was introduced by this camera change. Build output reported
3218 GameCube identities and5 existing slice fallbacks from the concurrent audio
pipeline;its code/config was not changed in these camera commits. Camera solver
22968ed changes portable behavior only. Captures stay ignored under
artifacts/azahar-camera-assets/. local-assets/,romfs/ and compiled packs were
not staged. Original assets remain copyrighted by Nintendo/Intelligent Systems;
local extraction/conversion conveys no right to distribute these assets or packs.
No ISO or original executable bytes were modified or committed.

ID1 flying behavior/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied flyMain@
party_move.o0x800c7ff8,two flying motion handlers/two speed writers and selected
partyFlyMain caller tail,dispatch0x80315050,map IDs0x802f47b0..0x802f47d0,
zero-vector/bias/numeric constants. Two flyMain names exist in the annotation
CSV;inspection used a temporary namespace-filtered /tmp/id1-party-fly-symbols.csv
rather than choosing Mario's mot_plane.o function. Initial unqualified inspection
rejected ambiguity and produced an empty ignored listing;no binary was changed.
Only metadata/analysis was added to source history;original ELF/listing/reports
remain ignored under artifacts/reference/behavior. No new ID1 art/audio conversion
or story text was introduced. make all invoked existing sound/proj/pmario.proj,
pool,sdir,samp audio-map pipeline:3218 GameCube identities,0 Switch,5 primary-PCM
slice fallbacks. Existing local-assets/audio-map raw/catalog and RomFS WAV/
audio-map.json derivatives remain unstaged. Original rights remain with their
owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-flying-build.log.

ID1 flying history/source/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
partyFlyMain/partyMoveFlyInit,Cloud/Vivian ordinary callbacks,three source leaves
and kind5/6 forty-byte records at0x803144b0/0x803144d8. Only addresses,hashes,
constant/word metadata and analysis were added to source history;original ELF/
listing/report remain ignored under artifacts/reference/behavior. Shared history
buffers are runtime/BSS state,not extracted art or invented source assets.
No original dialogue or new ID1 conversion policy was introduced. make all ran
existing sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube
identities,0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map
raw/catalog and RomFS WAV/audio-map.json derivatives remain unstaged. Original
rights remain with their owners;local analysis/conversion grants no redistribution
license. Exact build log:/tmp/id1-fly-history-build.log.

ID1 slit/roll/paper evidence/build provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
party_slit/party_roll,floor/activation/forced-exit leaves and paper wrappers.
Original resource/animation IDs at0x802f4750..0x802f47b8 and paper-group IDs
0x802cb360..0x802cb390 are preserved as short interface identifiers,not authored
story dialogue. Restore vectors/double bias/constants are pinned as metadata.
Original ELF/listing/report remain ignored under artifacts/reference/behavior;
source history contains only analysis/addresses/hashes/constant metadata. No new
ID1 asset conversion policy or story text was introduced. make all invoked
existing sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube
identities,0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map
raw/catalog and RomFS WAV/audio-map.json derivatives stay unstaged. Original
rights remain with their owners;local analysis/conversion grants no redistribution
license. Exact build log:/tmp/id1-slit-roll-build.log.

## 2026-10-12 — Contact/shift and full camera geometry native binding

Native86150f2/19c5104/93b2817 add camera mechanics without changing original
asset conversion, raw `.camera` bytes, mesh/audio pack formats or source ISO
access. Existing five original camera files remain ignored local RomFS assets;
all44 record identities and494 zone triangles stay authored and unflattened.
No original geometry, texture, dialogue, music or sound bytes were committed.
Ownership/licensing and local-only use remain as recorded above. The actor render
contract now records source model scale/root transforms; no actor rebake or
thumbnail/root conversion is claimed by this step.

ID1 slit-collision/math and message-census provenance (2026-10-12):read-only
/home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
unk_800c2010,toMovedirSimple and angleABf@system.o,existing filter/backend
identities,zero-vector templates0x802cb830,bias constants and91-word angleABTBL
0x803041e0(size0x16c). A temporary /tmp/id1-system-angle-symbols.csv selected
system.o rather than same-name pmario_sound.o;initial unqualified inspection
rejected ambiguity. Table remains original reusable data;only span hash/addresses
and short numeric metadata were committed,not an extracted table pack. Original
ELF/listing/report remain ignored under artifacts/reference/behavior. Read-only
message_resource_evidence report confirmed260 msg/US resources/13186 key/text
pairs;only gor_00 key/offset/size/hash metadata is documented,no dialogue prose.
No new ID1 conversion policy was introduced. make all invoked existing
sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube identities,
0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map raw/catalog
and RomFS WAV/audio-map.json derivatives remain unstaged. Original rights remain
with their owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-slit-collision-build.log.

## 2026-10-12 — Authored camera pipeline consumes retained original containers

Native64008d5 uses original camera road/path/zone/lens-metadata identities in
RomFS to calculate the view. Raw camera conversion and all asset packs remain
unchanged; no original bytes or compiled artifacts were committed. File lens
metadata drives the source moving-shift calculation, while live default lens
remains a separate owner. Capture review identified existing cropped actor atlas
root/scale loss; no actor rebake is claimed yet. Original licensing/local-only
provenance remains unchanged.

ID1 partner pre-display/pose/draw evidence/build provenance (2026-10-12):
read-only /home/carson/games/roms/G8ME01.iso US disc0 revision0 supplied complete
local pre-display/draw/facing/pose and HP/color/Vivian-tail predicate bodies.
Original pose-name tables0x80314708(1200 bytes),0x80314bb8(480),
0x80314d98(420) and adjacent color7 access span0x80314f3c(60) are pinned by
hash/address;no eighth color row or replacement pose string was invented.
Original matrices/pose names remain reused local data;underlying anim/GX/frame
services are not implemented by these docs. Map resource ID bom_01 at0x802f89b0,
ray vector,bias and32 binary32 constants are metadata,not story dialogue.
Original ELF/listing/report remain ignored under artifacts/reference/behavior.
No new ID1 art/audio conversion policy was introduced. make all ran existing
sound/proj/pmario.proj,pool,sdir,samp audio-map pipeline:3218 GameCube identities,
0 Switch,5 primary-PCM slice fallbacks. Existing local-assets/audio-map raw/catalog
and RomFS WAV/audio-map.json derivatives remain unstaged. Original rights remain
with their owners;local analysis/conversion grants no redistribution license.
Exact build log:/tmp/id1-predisplay-build.log.

## 2026-10-12 — Recovery projection and actor-bake preflight

Native8ee75c4 changes recovery mechanics/camera publication without changing
RomFS conversions or original pack bytes. Read-only actor preflight extracted
ISO a/a_mario and a/a_mario- to /tmp/pm-mario-root-bake/a_mario and a_mario.tpl;
character_bake produced temporary idle.png from M_Z_1 frame0 (102 triangles),
matching documented model bounds. This scratch bake was inspected only; atlas
root/scale metadata and native quads still need conversion. Scratch/original
bytes stay outside Git. Existing source licensing/local-use attribution applies;
no generated replacement art, music or sounds were introduced.

ID1 animation setter evidence/build provenance (2026-10-12):US G8ME01 ISO
opened read-only. Five original local bodies and their299 direct DOL references
are pinned as metadata. Original binary32 zero0x8041fb28 and factor
16.66666603088379 at0x8041fb40;initial work/global pointers0x80415dc0/
0x80415fd0 are hashed,not represented as live state. Loaded names/resources are
reused original data;no pose/message/art asset or runtime source was exported
into tracked docs. Inspection ELF/listings/report remain ignored under
artifacts/reference/behavior. make all ran the existing sound/proj/pmario.proj,
pool,sdir,samp pipeline:3218 GameCube identities,0 Switch,5 primary-PCM slice
fallbacks. Existing local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json
outputs unstaged;no ID1 conversion policy change. Original rights remain with
owners;local analysis grants no redistribution license. Exact log:
/tmp/id1-animation-build.log.

## 2026-10-12 — Camera event services and ordinary actor pose preflight

Nativee278068 adds camera blend/dolly/interpolation services without changing
original asset conversion or RomFS pack bytes. Documentation identified ordinary
Mario idle as M_S_1; an additional read-only preflight using the scratch
/tmp/pm-mario-root-bake/a_mario +a_mario.tpl produced ordinary-idle.png from that
pose's frame0. Bounds/102 triangles match the previous M_Z_1 scratch geometry,
but those names are distinct pose requests and must not be substituted. Front/
rear pose and frame ownership research continues before atlas/root conversion.
No original scratch images/models, converted packs or compiled output were
committed; existing local-only ownership/licensing remains unchanged.

ID1 animation Main/init provenance (2026-10-12):US G8ME01 ISO opened read-only.
Original a/a_mario215124 bytes/SHA256
61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293
rechecked directly;M_Z_1 index0/serialized offseta5fc/control0/0/0/single key
metadata pinned. No pose/model/text/texture payload committed by this research.
Original init requests a/vivian.bin (format802c11d8) and a/ag2tg.bin (802c11e8)
are byte/hash pinned requests,not proof of live load success. Runtime tangent
storage803d9580..803d9848 is generated by tan requests,not an initialized DOL
asset;factor3c8efa33/writer pinned. Original ELFs/disassemblies/report ignored
under artifacts/reference/behavior. make all ran existing sound/proj/pmario.proj,
pool/sdir/samp pipeline:3218 GameCube identities,0 Switch,5 primary-PCM slice
fallbacks;local-assets/audio-map raw/catalog and RomFS WAV/audio-map.json remain
unstaged. No ID1 conversion policy change;local analysis grants no redistribution
license. Exact log:/tmp/id1-animation-main-build.log. Six ID1 documents were
included in concurrent reorganization76263bb;ID1 follow-up records the recovery
and validation provenance without changing other owners' history.

ID1 draw/paper/group evidence/build provenance (2026-10-12):US G8ME01 ISO
opened read-only. Five complete local bodies and148 direct sites pinned as
metadata;original a/a_mario SHA61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293
has resource pass mask1,42 groups,root41/sibling35/child40/shape−1/visibility41/
node984/joint0. This is serialized metadata,not observed loaded scene/geometry.
Original modeling stack803d8e70 and runtime tangent base803d9580 derived using
signed-low lis/addi;prior erroneous803e tangent addresses corrected above and
in Main docs. No original model/pose/texture/text payload exported into tracked
files. ELFs/listings/report remain ignored artifacts/reference/behavior.
make all ran existing sound/proj/pmario.proj,pool/sdir/samp pipeline:3218 GameCube,
0 Switch,5 primary-PCM slice fallbacks;local-assets/audio-map raw/catalog and
RomFS WAV/audio-map.json outputs unstaged. No ID1 conversion policy change;
original rights remain with owners and local analysis grants no redistribution
license. Exact log:/tmp/id1-draw-build.log.

ID1 node matrix/SDK evidence/build provenance (2026-10-12):US G8ME01 ISO
opened read-only. Original a/a_mario identity from preceding Main/draw contracts
rechecked;13 joint/29 transform groups,1008 node floats,root/max index984 are
serialized metadata,not exported node/model payload or observed gameplay.
Original16/8 dispatch tables80306698/803066d8 and affine pair8041e458 are
hash/word pinned;modeling stack803d8e70 derived from signed-low producers,
available symbol extent600 hex=32 matrices. Runtime stack/GQR/FPSCR not measured.
ELFs/listings/report remain ignored under artifacts/reference/behavior.
make all ran existing sound/proj/pmario.proj,pool/sdir/samp pipeline:3218 GameCube
identities,0 Switch,5 primary-PCM slice fallbacks;local-assets/audio-map raw/catalog
and RomFS WAV/audio-map.json outputs unstaged. No ID1 art/audio/mesh conversion
policy change. Existing bake/mesh node formula reconciliation documented for
native owner. Original rights remain with owners;local analysis grants no
redistribution license. Exact log:/tmp/id1-node-build.log.
