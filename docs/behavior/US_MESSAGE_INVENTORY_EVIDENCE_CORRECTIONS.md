# US message inventory: resource files are not numeric message IDs

US G8ME01 disc0 revision0,ID1,rechecked2026-10-12 with the supplied ISO opened
read-only. This corrects the newly generated inventory/draft claims in
`docs/BINARY_DOCUMENTATION.md`,`docs/MESSAGE_FILES_ANALYSIS.md` and related
PROGRESS entries. Their numerical filename/prefix scans do not supersede the
[recovered loader/lookup contract](MESSAGE_RESOURCES_AND_LOOKUP.md).

The actual loader-visible message resources contain alternating NUL-terminated
KEY and TEXT strings until the first empty-string boundary. They include original
US display/dialogue text and control tokens;they are not only identifier strings
pointing to a missing external dialogue database. Do not discard odd strings
when building the dialogue resource set.

Reproduction through `tools/message_resource_evidence.py` on the current ISO:
260 msg/US resources,13,186 complete key/text pairs. These are resource/pair
counts,not a count of unique globally available keys,executed dialogue branches,
scenes,speakers or story coverage. Original dialogue remains a reused local asset;
this document exports no prose or control-token payload.

A concrete source example:msg/US/gor_00.txt has328 pairs,656 nonempty strings,
terminator at byte59141 and SHA-256
bd39efb45105781a403ca5e889ba8e9fb8dbaaaf0b2bd60af6955e38073acdcc.
Its first key is `mac_0_014`,offset0;the paired text starts offset10,size247,
SHA-2560ae30c8eadb1f9309322c432bb65b181fcefa3efbdf7864f32f1e428729ba0b7.
A filename number0 therefore does not identify a sole message0 in that resource.

`tools/doc_binary_iso.py` currently derives its “Message ID Ranges” from a
regex of the FIRST decimal substring in resource FILENAMES(and only its first50
sorted resource entries). That is a filename-label scan. It does not parse key
strings,resolve message calls or establish31 unique message IDs0..30. The
related draft's assertions about prefix meanings such as room order,postgame,
keypad/ticket/procedure roles are not established by such token/name patterns.
Keep them as unvalidated notes until bound to original resource/event evidence.

Likewise,FST file-size sums or suffix inventories are not complete binary-code
analysis. ISO size minus summed FST payload sizes is not by itself proof of
unused/empty disc space:system structures,alignment and other allocation domains
must be accounted for before assigning that interpretation.

The source owner's pending drafts have a visible correction banner linking here;
only this scoped corrective contract/inbox handoff is committed by ID1. Broad
inventory-tool/draft rewrites and other owners' new files remain separate work.
Use the existing schema3 message census for key/text metadata and the original
loader's actual resource/lookup lifetime when implementing message playback.
Full event order,speaker attribution,branches/persistence and original runtime
still require story contracts;13,186 pairs do not establish complete story fidelity.

ID1 follow-up (2026-10-12):the pending BTLNO_MESSAGE_MAPPING.md and related
PROGRESS “complete dictionary/ID linkage verified” claims are also superseded.
Its Message Text column contains resource KEY strings. Filename/prefix/string
co-occurrence does not establish a runtime battle-ID→message-key/file/text lookup,
correct speaker,branch,scene order or all story content. A visible boundary was
added to that pending draft,which remains unstaged by ID1. Recover original event
and service call bindings before implementing that proposed dictionary;no such
complete lookup is proved by the inventory method described there.
