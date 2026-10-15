# evt_npc_wait_battle_end — corrected source identity

The former text at this path described an unrelated instruction sequence and
incorrectly identified both callbacks as 0x8004efbc. It is superseded by the
[complete field entry/return contract](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).

The actual start callback is 0x8004ef54/0xb8; wait is 0x8004eed8/0x7c.
Start first resolves its NPC operand and the following word, then requests
sequence4 and returns0 on its initial visit; resumed visits return2 after the
lookups. Wait admits cleanup only at sequence2 with any nonzero source result.
It does not classify victory or produce a result. See the linked contract for
ordered services, raw extra read, ownership and remaining runtime limitations.
