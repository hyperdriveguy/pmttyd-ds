# Closed cold-storage initializers

`initializePackedBattleColdStorage` executes nine complete original US storage
bodies: action-help init, private pad reset, four general pad initialization,
action manager initialization, after-reaction queue initialization, unit registry
initialization, battle icons, status-message work and party snapshot work. The
source function token selects the body; unsupported display/stage/audience/
break-slot functions return unavailable without effects.

These are synchronous memory-only bodies after expanding their closed pad,
registry and memset callees. Their providers must admit every reached guest
access before entry. They do not retry a late asynchronous resource operation.
The surrounding [cold parent](BATTLE_COLD_INIT_CONTINUATION.md) owns any hold
between initializers. A completed void body returns engaged0; Unit_Init returns1.
The action-help body's sole `blr` requires no memory service.

Pad reset preserves all raw bytes required by the
[pad contract](BATTLE_PAD_RESET_AND_RAW_PRESERVATION.md), including both byte-b
cells, the release-history bank and trailing raw bytes. Snapshot half/byte
widths and interleaved pressed/repeat/held stores retain source order. General
pad init rereads the global battle pointer for each of four pads; private action
init uses its explicit captured argument and then sets the five source fields.

The queue uses64 eight-byte records, storing key−1 then payload0. Unit init
clears only the64 pointer-registry cells. Icon init clears exactly2496 bytes at
B182cc. Message init clears600 bytes at B18da0, then writes the first byte of
28 twelve-byte records to255. Party init captures scalar80422174 once, then
initializes seven44-byte records with separate30-byte status clears. The two
bytes at record offsets34/35 remain untouched.

`tools/verify_battle_cold_storage.py` pins all nine complete original bodies and
the original party scalar from the read-only ISO. Actual-VM tests check dirty
pad preservation, store widths/order, fresh general-pad owners, exact record
boundaries/gaps and unknown-body denial. This does not bind native startup or
prove display resources, actor construction, tutorial playback or field return.
