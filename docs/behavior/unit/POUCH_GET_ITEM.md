# Pouch item receive dispatcher — corrected identity

The earlier lookup/pointer description at this path was incorrect. Its claimed
800d5520 entry is inside the original US receive function, not an item-record
lookup service. Fresh read-only source recovery pins pouchGetItem at800d50cc;
it mutates pouch storage and returns receive success1/failure0 for item95.
Do not implement a pointer lookup or invented item structure from the old stub.

Use [exact item95/key-array insertion](POUCH_KEY_ITEM95_RECEIVE_INSERTION.md)
for the verified highest-zero-slot/backwards-shift branch and
[receive dispatcher](../battle/POUCH_GET_ITEM_DISPATCH.md) for the separate
resource/badge/upgrade routing. Source receive95 does not invoke Mario/party
parameter revision. [Original item95 presentation](../field/GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md)
opens its windows before calling this leaf and handles full storage separately.
This correction is static source evidence, not an original runtime trace.
