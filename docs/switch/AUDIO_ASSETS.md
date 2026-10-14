# Switch 1.0.0 audio asset library

Audio is extracted separately from the GameCube library and native RomFS pack.
The input is the existing integrity-verified base-title RomFS described in
[BINARY_BASELINE.md](BINARY_BASELINE.md), at `local-assets/switch/romfs/`.
Neither the NSP nor extracted reference files are modified. Keys, firmware,
emulator profiles and saves are not used by this conversion.

## Coverage and local paths

`local-assets/switch/audio/` is ignored by Git:

- `raw/sound/`: all 670 source sound-tree files, copied and hash-verified,
  including `sound.bfsar`, 577 external Opus streams, 27 user-managed group
  wrappers, 64 compressed data ELFs and the vibration wrapper.
- `raw/banks/`: structured BFSAR/group extraction of NintendoWare FWAR, FWSD
  and FGRP resources, retaining file IDs and contextual variants.
- `raw/waves/`: 9,691 BFWAV members extracted through FWAR tables, with source
  file ID, member ID and original byte span retained in the manifest.
- `pcm/waves/`: all 9,691 decoded bank samples, using the corresponding
  archive/member paths. These include sound effects, voices and shared sources.
- `pcm/stream/`: all 577 decoded external streams, including the remake's
  arrangements and its separate `BGM/O/` assets. Original paths are retained.
- `*.wav.loop`: native-format two little-endian uint32 PCM frame indices,
  start inclusive/end exclusive, only for looping assets.
- `archive-inventory.json`: all 10,256 sound records, 1,449 file records,
  62 wave-archive records, group variants, names, IDs and stream extensions.
- `manifest.json`: original/resource/PCM hashes, source spans, channels/rates/
  frame counts/loops, decoder identity and conversion limitations.

There are 63 extracted FWAR resources: all 62 global archive records plus one
contextual copy. `GROUP_SE_ENV.bfgrp.bin` uses file IDs 15 and 7 for resources
that differ from those IDs in the global archive. Both are retained under
`group-variant-*` paths; no global name or event identity is assigned to them.
The 280 global file entries without payload are group placeholders, not omitted
wave archives. All external streams and every global wave archive are covered.
The source archive's nostalgic stream filenames are flattened, while RomFS
stores them under `stream/BGM/O/`; each alias is matched to exactly one basename
and both archive and actual source paths are retained.

Completed validation: all 10,268 WAV formats/full data lengths/frame counts/
hashes/loop sidecars checked; all 9,691 FWAV rate/channel/count/loop fields
independently matched to original headers. All 670 original and copied source
hashes match. Music has 421 looped/156 nonlooped tracks; bank waves have
807 looped/8,884 nonlooped samples. All 62 global wave-archive IDs are covered.
Decoded WAVs total 11,385,943,684 bytes; copied sound-tree sources total
1,941,989,744 bytes. There are 594 extracted bank/control/group files. All
22,453 generated library files are ignored, with zero tracked originals.
WAV formats retain 48,000 Hz or 22,050 Hz and mono or stereo source channels.

WAVs are PCM16 at original rates and channel counts. vgmstream decodes each
asset once with `-i -W 1`, without added repetitions/fades, resampling,
normalization, crossfades or generated replacements. External Opus loops come
from BFSAR stream extensions, not the loopless standalone Opus headers; BFWAV
loops come from their own headers. The manifest preserves sound names and raw
FWSD/group/control data, but does not render event mixing, pitch, envelopes,
layering, transitions or adaptive music. No gameplay listening, audible-loop,
hardware or runtime-mix equivalence is claimed. Default native builds now
package 18 Switch arrangements with original PCM
rates/channels and loop intervals. The native slice chooses district and battle
selection variants through a presentation adapter; this does not establish
original Switch event routing or adaptive mixing. GameCube SFX remain active.
See [native soundtrack adapter](../behavior/audio/NATIVE_SWITCH_SOUNDTRACK.md).

## Decoder and reproduction

External decoder: [vgmstream](https://github.com/vgmstream/vgmstream/tree/7c520e8f9b078dfde2f84f1b5bcbad0c6c415806),
commit `7c520e8f9b078dfde2f84f1b5bcbad0c6c415806`, CLI `r2117`.
Source/build are ignored under `artifacts/reference/vgmstream/`. The build uses
installed FFmpeg libraries; no global installation is needed:

```sh
cmake -S artifacts/reference/vgmstream -B artifacts/reference/vgmstream/build \
  -DBUILD_V123=OFF -DBUILD_AUDACIOUS=OFF -DUSE_MPEG=OFF -DUSE_VORBIS=OFF \
  -DUSE_G719=OFF -DUSE_CELT=OFF -DUSE_SPEEX=OFF
cmake --build artifacts/reference/vgmstream/build --target vgmstream_cli -j 8
python3 tools/switch_audio.py --workers 8
```

Override inputs with `--romfs`, `--output` and `--decoder`. The tool requires
derivatives outside the source RomFS. `--verify-existing` audits previously
decoded WAV format/full length, source metadata, loops and hashes and rebuilds
the manifest without re-decoding. It is a structural/data-hash audit, not a
fresh decoded-PCM differential comparison. The initial full batch decoded and
checked all 10,268 WAVs; a final informational `-V` status-1 exit interrupted
manifest creation. After correcting that handling, the existing outputs were
audited to complete the manifest.

The authored reader checks typed references, container sizes, nested bounds and
group-local identity conflicts. It reads counted external-group offsets rather
than scanning audio payloads for coincidental magic strings. FGRP logical sizes
can omit final 64-byte padding included in their section/header sizes; both
logical and stored sizes are recorded for internal group files.

## Provenance and rights

Original assets retain their original copyright. Extraction and conversion grant
no redistribution rights; raw assets, decoded WAVs, indexes and manifests stay
ignored. No original executable is linked into the native port.

vgmstream is ISC; its COPYING exactly matches the already committed
`third_party/vgmstream-COPYING`. Its external codec dependencies retain their
own licenses. NintendoWare layout facts were cross-checked against
[libpulsar](https://github.com/p-sam/switch-libpulsar/tree/3d3a057f4aa2578cdb89cab3596967c47de73b73)
(MIT, commit `3d3a057f4aa2578cdb89cab3596967c47de73b73`) and
[Citric Composer](https://github.com/Gota7/Citric-Composer/tree/73439816c9f2cd6f43ab9aa46f89247be8e32942)
(commit `73439816c9f2cd6f43ab9aa46f89247be8e32942`, no top-level license found).
Their snapshots stay ignored; no implementation was copied into the authored
Python parser or linked into the port. Reference licenses do not grant rights
to original title assets.
