# Retained original battle map loading

`resumePackedBattleMapLoad` implements the whole US `bmapLoad8002892c/17c`
body. The verifier pins all 380 source bytes to SHA-256
`db1c37ba1723eb44e26223fdc78d595381d71abe136fa9c7987691c0024e55d7` in the
read-only revision0 ISO. This source body has no float or RNG operations.

The controller captures the actual base/foreground string addresses before the
first synchronization child. Its stationary state retains original register
values, exact child request operands/callsites and current step across waits.
It never retries an acknowledged prefix. Provider identities and a strong
independent lifetime are pinned; reentry, replacement or retired process owners
are unavailable. The word/store services require admitted mapped source memory,
including unchecked original-address arithmetic. They do not manufacture unknown
bytes or turn invalid guest memory into successful zero reads.

## Original order and data ownership

After synchronization80005ce4, capture WORD8041e660, store active group8041e664=0
and scan signed count at the captured work base. Each iteration reads that count
again; its tail-entry address uses fresh active-group then fresh global-work
loads. Entry advancement still uses the captured base. Preserve that distinction
when another genuine child changes globals. Group stride is2f4, entry stride178.

A nonnull tail buffer calls mapFree8002fdc4 before zeroing offsets168/16c.
The captured entry's smart fields170/178/174 are then zeroed in that order.
A null D pointer skips acquisition cleanup. Otherwise arcDataCheck80078444
selects the source branch: zero result moves D/T through smartEntry801526d4,
writes the actual results, frees T then D and clears their pointers; nonzero
moves the archive pointer through smartEntry, frees it and clears8c. Source
null Smart results are valid returned words and do not invent failure branches.

After the scan, reload the work global, store active group1 and clear that
actual group with original memset800050b4. Call complete `_mapLoad80028f18`
with group/slot0/base, then store count1. Only a nonnull captured foreground
calls slot1 and then stores count2. Missing or held children publish no later
counts. No native atlas or PMMAP resource can acknowledge these source calls.

Void children return readiness with no fabricated scalar result. Archive and
smart children must provide an engaged genuine result before acceptance. A
callback owns its own partial effects across holds. The function table remains
fixed while its callbacks may retain missing inner providers. Caller state,
memory/process/heap mappings and child contexts must outlive the cursor.

## Native integration frontier

The existing `FieldMapSourceOwner` supplies complete Init/GetWork/LoadEntry,
source MapObj construction and exact heap identity checks. This new module does
not modify that foreign continuation graph. A future provider can route the
linked LoadEntry requests to that retained owner while preserving the actual
request and source stack ownership. Real synchronization, smart storage, file
catalog/worker, collision/light and cache providers remain required.

Current production code has no bound FieldMapSourceOwner process instance.
The existing file adapter only admits original gor_00 D/T; bti_01/stg_00_0 need
separate original unrelocated D/T and exact rounded-read bytes plus a genuine
catalog extension. A real source process must own bank1 before StageObjectInit
calls GetWork. Its root chain is work+a8→MapObj+8→joint name, not a synthetic
scene pointer. Original mapGrpFlagOn/Off are separate missing source bodies.

This controller supplies no Game activation, native source-ready flag, scene,
GPU, loaded-resource or complete Crump encounter receipt.

## Focused verification

Strict host tests compare every source read/store, linked call and final memory
with an independent straight-line reference across negative/zero/positive
counts, null D/tail, archive/D+T branches and optional foreground. Every linked
request is held twice, with no repeated operand loads or accepted effects.
Children change actual globals and the live captured count to distinguish fresh
loads from cached values. Tests also cover reentry, changed borrowed name
arguments, unavailable providers and irreversible owner retirement. ASan/UBSan,
ARM syntax and original whole-body source pins are separate checks; test child
receipts do not claim actual native synchronization or smart storage.
