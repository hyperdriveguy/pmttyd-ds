# Complete type4 texture-palette address setup

US G8ME01 revision0; ID2,2026-10-12. Complete type4 block8018ab88..8018ac7c
in fileGarbageDataAdrSet,plus linked dispatch. Verify
`python3 tools/verify_file_type4_relocation.py`;read
[clear operation](FILE_TYPE4_TEXTURE_PALETTE_ADDRESS_CLEAR.md),
[type routes](FILE_RELOCATION_TYPE_DISPATCH.md),
[payload movement](FILE_PAYLOAD_MOVE_AND_ADDRESS_REQUESTS.md).
Containing-body/panic identities do not establish full type5 or platform bodies.
No original texture loading/movement/rendering trace is claimed.

## Signature is checked even when table already appears absolute

Retain payload base B. Read WORD B00. Require exact0020af30. On mismatch call
OSPanic(filename802f9470,line112,format802f947c),with the original strings
`filemgr.c` and `invalid version number for texture palette`. The caller then
falls through to the pointer guard if that backend returns;no explicit local
failure return or recheck exists. Preserve required diagnostic service behavior;
do not silently accept another format version or invent a replacement palette.

Read WORD B08 as TABLEVALUE,compare UNSIGNED to B. If TABLEVALUE>=B,skip entire
handler after the signature check. Otherwise store B08=retained TABLEVALUE+B
with32-bit wrap. No table-null/size/alignment guard or explicit relocated flag
is used for this root pointer. A relative0 becomes B;it is not preserved as null.

## Low16 index and live word count drive two-pointer descriptor records

Initialize full word index I=0. At each loop condition,read CURRENT WORD B04
as COUNT;compare unsigned LOW16(I)<COUNT. If false,exit. If true,compute
O=8*LOW16(I) (equivalent original rotate/mask0007fff8);process table slots O and
O+4 in order,then increment full I by1 with word wrap and repeat the live count
read. No captured-count or ordinary unsigned32-index loop is equivalent for
all raw inputs. COUNT>65535 can keep the wrapped low16 index below COUNT and
revisit records indefinitely while the count remains unchanged. No local cap.

Every table access rereads B08. No descriptor array copy,texture decode or
platform image upload occurs in this handler. Stable source records use8 bytes:
first descriptor pointer,second descriptor pointer;their concrete data/flag
fields below establish the texture/palette address roles used here.

## First descriptor uses data word08 and flag byte23

Read current table[O]. Exact0 skips this slot entirely. For nonzero value Q,
store table[O]=Q+B with word wrap. Reread B08 and table[O] as descriptor D.
Read BYTE D23. Any nonzero flag skips its data conversion;only exact0 converts.
Read WORD D08,store it plus B (not plus D). Then REREAD B08/table[O] and store
BYTE that current descriptor23=1. Flag target can differ under physical aliases;
do not substitute retained D for the final flag store.

The descriptor pointer is ALWAYS converted when its slot is nonzero once root
admission succeeds,even when D23 is already nonzero. There is no per-descriptor
relative/absolute range guard. Its data word has no null preservation:0 becomes B.

## Second descriptor uses data word08 and flag byte02

Read CURRENT B08 and table[O+4]. Exact0 skips. Otherwise add B to that pointer
and store it into its table slot. Reread root/table slot as D;read BYTE D02.
Any nonzero flag skips data conversion. Exact0 reads D08,adds B,stores D08;
then rereads root/table slot and stores current descriptor02=1.

First-slot data/flag/table mutation can affect the second slot or later records.
No marker besides descriptor bytes23/02 is written. Width,height,format,filter,
LOD,palette-entry count and other raw bytes are untouched by this local block,
except through source aliases. No heap allocation/free,flush,DVD or user callback.

## Native requirements and acceptance

Implement original32-bit modular arithmetic and memory coherence across root,
table slots,descriptor/data and flag bytes. Preserve signature-before-guard,
retained root guard value,live count/low16 indexing,zero table-slot skips,
unconditional nonzero descriptor conversion,data flag exact0 admission and
fresh final flag target. Resource names/type producers and valid counts must
come from original source evidence;do not label arbitrary edge fixtures measured.

Check signature match/mismatch/backend-return path,root below/equal/above B,
COUNT0/65535/65536 and live changes,zero/nonzero slots,flags00/01/80/ff,data0,
word wrap,shared descriptors and aliases changing table/flag targets. A native
provider can reject invalid source domains explicitly,while keeping original
uncapped/unchecked semantics in the fidelity contract.

Type5 relocation,complete TPL decoding/rendering/source provisioning,platform
panic/cache/draw/worker/scene and original traces remain EVT-01/JUMP-01/DMG-02
work. Asset data remain reused outside source history;this block transforms
addresses only and does not create or rescale texture content.

## Concrete US effect texture source witness

Read-only original e/us/effect_us.tpl is106880 bytes,SHA256
93ef5d667ded662d6abf13133082bef138f9c88132365ff8b11f8abf2129dd14. Its header
has magic0020af30,count82,relative table0c. All82 first slots are nonzero and
point to82 distinct image headers;all image flag23 bytes are0. All second
palette slots are0. The verifier pins this source summary and fingerprint,
not execution of Set/Clear or hardware rendering. This asset does not exercise
nonzero palette descriptors;that branch requires other source witnesses/traces.
No bytes extracted,converted or committed. Existing [asset provenance](../../ASSETS.md)
records this reused original texture source and source-history/licensing boundary.
