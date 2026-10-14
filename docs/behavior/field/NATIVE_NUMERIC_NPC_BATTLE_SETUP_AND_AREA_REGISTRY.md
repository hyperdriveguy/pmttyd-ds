# Original numeric NPC battle setup and mutable area lookup

Evidence B, US G8ME01 revision0 DOL: npcSetBattleInfo80043d80..80043e78,
npcSetupBattleInfo80043e78..800440f8, areaDataPtr800391e0..80039278 and
relSetBtlAddr80038fd8..80039088. See the [complete recovered protocol](FIELD_BATTLE_ENTRY_AND_RETURN.md)
and [gor02 setup0 data frontier](GOR02_SETUP_ZERO_BATTLE_INDEX_AND_ORIGINAL_TRIBE_DESCRIPTORS.md).
No original execution or complete encounter equivalence is claimed.

`packedBattleAreaData` traverses original pointer rows at registry+8 in order,
compares case-sensitive NUL-terminated names and returns the first exact record.
`registerPackedBattleArea` updates that record's setup/number pointers at+8/+c;
missing names produce no mutation or allocation. The default registry address is
803057e4; tests may supply an explicitly test-owned registry. This is guest
memory, not a host string-keyed cache or a fabricated initialized DOL registry.

`setPackedNpcBattleInformation` captures actor/index at entry. Index-1 passes
null directly to inner setup without GP/registry access. Otherwise it freshly
loads GP from80415fd0, reads exactly its three area-prefix bytes+12c..12e,
looks up the record and reads its+8 table. Positive indices add0x44 per row,
with32-bit address arithmetic; zero and other negative indices select row0.
No native fallback, missing-table recovery or number-table indirection is added.
Callers must admit the full supported memory graph and numeric domain before
calling; this synchronous helper does not establish Game world readiness.

After inner setup, each nonnull association slot at actor+330,+334,+338,+33c
is freshly read when reached. Copy200 bytes from actor+230 to child+230 in25
pairs. Each pair performs both source word loads before both destination stores.
The byte at child+316 is outside this copy. Preserve ordered operations even for
aliasing pointers and service-visible mutations; a host memcpy has a different
aliasing contract. No actor allocation or ownership is implied by these slots.

The inner helper corrects three earlier transcription errors: C+18 is installed
at information+4; audience width uses the pre-RNG signed bytes but addition
reloads the unsigned first byte after RNG; after clearing actor+316, the final
kind lookup freshly reads information+18. Tests mutate those values at the
observable call/store boundaries and cover exact original harbor setup2, whose
C+18 is20000010. The saved-switch selector, group/item draw order and selected
item-table fresh read remain as documented in the original protocol.

This phase uses existing original private packs without changed conversions.
Host tests cover ordered copies, overlapping associations, late slot mutation,
index routes, null configuration and first exact registry match. An isolated ARM
fixture uses actual setup2 bytes with test-owned registry/actor memory and an
explicit test seed. It does not certify original area-module registration,
Game NPC creation, resource/pose readiness, shared RNG ordering or stage entry.

Next admit the bounded original gor setup0 graph in the shared90 address domain,
register original mutable area records through the module lifecycle and supply
one session-owned RNG to real NPC setup. The harbor pack's95 address domain is
separate. Stage/music pointers published by setup need their own later consumer
admission. Keep field/story/battle seed/interleaving work under RNG-01; source
cold-boot data alone does not establish save/load lifecycle behavior.
