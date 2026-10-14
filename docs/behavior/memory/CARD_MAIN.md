# cardMain

Address800b2508, size678hex (1656 bytes/414 instructions).

Read the [complete dispatcher/callback contract](CARD_DISPATCH_FORMAT_CHECK_AND_COMPLETION.md),
recovered2026-10-13 for [SAVE-01](../../requests/SAVE-01.md). It supersedes the
former provisional sketch's incorrect size/work-field/target descriptions.

The local dispatcher monitors card probe changes, dispatches operations0..8,
delegates write/header/read/read-all/create, and owns format/check-delete phases,
result polling and busy/error/completion publications. The eight completion
callbacks have complete source contracts. Original record construction/checksum
is a [separate writer](../format/CARD_WRITE_RECORD_ASSEMBLY_AND_CHECKSUM.md).

Async child/SDK internals, read validation/corruption/backup/namespace round-trip,
original timing/execution and native binding remain open. Caller coverage alone
does not establish complete persistence or a successfully saved/loaded record.
