# Original tutorial stage file catalog

`FieldMapFileOwner` accepts an immutable, validated catalog of source DVD paths,
native relative paths, logical lengths and 32-byte transfer lengths. The existing
four-argument binding retains the original two `gor_00` records.
`tutorialStageMapFileCatalog()` adds the genuine `bti_01` base and `stg_00_0`
foreground D/T records selected by the Crump rank-zero source stage.

The file owner copies admitted rows before mapping any manager storage. It rejects
canonical source aliases, duplicate native destinations, unsafe relative paths,
zero lengths, rounded-length overflow and missing or incorrectly sized native
files. Only explicit `./m/` and `././m/` spellings refer to the same source path.
An unlisted or invalid path remains unavailable at the original ConvertPath child
8029fdbc: catalog absence does not prove original FST absence. For a listed file,
a real native open failure after binding can return the source's zero handle.
The finite adapter ordinal is not an original FST entry number.

Source FileOpen80144e88, FileLength80144dcc, FileRead80144e18 and
FileClose80144dd4 still execute the existing retained source controller. The real
stdio file and original worker own reads and retirement. Source slot/table backing
must remain identical; a byte-identical replacement is not the same allocation.
No successful map LoadEntry, graphics-cache flush, light construction, map bank
publication or scene readiness follows from catalog admission.

## Original data and editing

The one-time `tools/import_tutorial_stage_files.py` importer reads G8ME01 revision
zero read-only and requires an empty destination. New editable sources are in
`assets/original/maps/tutorial-stage-source/`, independently of the existing
hierarchy/model/atlas sources and `gor_00` files. Normal manifest `pack` recipes
copy current editable bytes to `maps/tutorial-stage/{bti_01,stg_00_0}/{d,t}`.
Normal builds never reimport or replace edits.

| Original FST path | Logical bytes | Actual DVD transfer bytes | Actual trailing disc bytes |
| --- | ---: | ---: | --- |
| m/bti_01/d | 181042 | 181056 | 00000020af30000000100000000c |
| m/bti_01/t | 58976 | 58976 | none |
| m/stg_00_0/d | 129290 | 129312 | 00000020af30000000080000000c0000004c00000000 |
| m/stg_00_0/t | 119168 | 119168 | none |

Provenance records the original FST offsets, logical SHA-256, transfer SHA-256 and
actual tails for every file. No zero padding, relocation, geometry conversion or
texture reencoding occurs here. Nintendo/Intelligent Systems retain original
rights; tracking these files grants no new license. The ISO and compiled products
are excluded from source history.

## Evidence and integration boundary

The focused native-host test drives the real retained FileOpen/Length/Read/Close
chain and worker against these normal compiled products, compares every transfer
byte, preserves compatibility rows and tests immutable copying, invalid catalogs,
known-file open failure, unknown-path holds and replacement backing rejection.
The Python test independently reimports the read-only original into a temporary
empty destination and checks hashes and nonzero tails; edit-refusal occurs before
opening an ISO. It does not require editable files to remain original forever.

Use the explicit catalog overload when composing the actual process map owner.
The existing actual process heap, map-work bank, source SDK/cache/light services,
Bmap continuation and Stage callbacks remain separately required. These tests are
portable CPU/file ownership evidence, not a native Game activation, GPU receipt,
original execution comparison or completed Crump tutorial.
