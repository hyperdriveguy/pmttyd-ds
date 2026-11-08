# Actual original icon files and shared process memory

`NativeSourceIconMemory` specializes the audited shared Window-memory model for
its distinct original32-byte state span8041e630..8041e64f. It must not use Window
BSS or substitute a class/Window resource model. Its first bind captures actual
RAM bytes and unknown cells transactionally, including cross-VM rollback. Later
binds share that history. No constructor-zero values or byte-matching readiness
are admitted. Actual source writes progressively make unknown cells available.

Actual heap allocations carry their real addresses, capacity, stable backing and
strong owner. Unknown reserved tails have first refusal and remain unavailable.
Source allocator writes may be adopted as an explicit known prefix; real DVD
reads overwrite only their admitted prefix once. Half/word stores preflight all
cells/mappings before changing bytes. Overlapping guest/backing allocations,
semantic/unextractable mappings, borrowed-RAM mutation, stale owners and recursive
operations reject. VM mappings must retire while each borrowed VM is alive.
Retirement invalidates native reads; it is not an original free, source reset or
completion.

`NativeSourceIconFile` verifies the unchanged original rawBIN8174 and rawTPL677792
bytes with exact sizes and FNV64 pins. `NativeSourceIconAssets` also validates the
existing188-byte constants ABI and a new50-byte `PMICST01` private runtime pack.
It exposes only actual original address spans, never fabricated e630/e634 global
pointers or index+1 SDK objects. The file owner grants no source/GPU readiness.

The new read-only ISO exporter emits the24 actual format bytes at802bf858 and18
actual physical-disc bytes following icon.bin's logical EOF. The original parent
asks DVDRead8192: DVDReadPrio802a040c..043c allows the rounded request below
logicalLength+32. DVDMgrMain801451c0..e0 updates remaining/progress/destination by
the requested count. The rounded8192-byte ISO read has SHA256
d11e2c466938bd426cb0ccc70f5938ef8c3d4dac5b368a66633d071784b7c6bf.
The18 bytes at disc948147838 have SHA256
385fd512ffe41b128bd5ea66415eb4176f7123635c958b7e25b89d45fa40b360.
They are not synthesized padding. The header is converter metadata; format/tail
payload is unchanged original data. The50-byte pack SHA256 is
1623d8d5282ba41ad4b4f75763b19e45d2ee0aadb2cf96fd31baf9d04c5a8bca.

Exporter destinations must not exist. It reads the ISO with `rb`, touches no
editable art, and emits only ignored private runtime output/provenance. Existing
rawBIN/TPL/constants and original editable image exports are unchanged. Original
Nintendo/Intelligent Systems rights remain; these technical assets gain no
redistribution license. Parent integration must add the narrow ASSETS/build
registration notes supplied with the frozen package.

Host tests exercise transactional multi-VM aliases, source writes, stale/lifetime
and recursive cases, partial unknown tails and actual613-image relocation without
inventing either ready flag. The [process owner](NATIVE_SOURCE_ICON_PROCESS.md)
provides the separate actual source Init/callback producer.
