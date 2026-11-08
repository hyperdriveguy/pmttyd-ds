# Native Harbor target names

The manual Harbor packed-actor adapter now copies each original part
definition's message-key pointer at `definition + 4` into the live part's
name-pointer field at `part + 8`. This is the same field copy performed by the
complete [original unit constructor](../battle/BATTLE_UNIT_ENTRY_COMPLETE.md)
and its [retained controller](../battle/BATTLE_UNIT_ENTRY_CONTINUATION.md).
The change does not replace or acknowledge that constructor.

The native target-menu snapshot reads this existing source field and resolves
the original US message. For the actual first Crump target, part 1 contains
`btl_un_kanbu1`, whose original text is `Lord Crump`. No actor name or dialogue
is generated. The prior adapter left the field cleared, so the target snapshot
correctly remained unavailable even though source target confirmation worked.

The focused test reaches the actual target state through original scripts,
real imported animation endpoints and source command input. Its class/weapon
presentation acknowledgements are explicitly TEST-only. It verifies the live
part pointer equals the original definition pointer and the projected label
matches the original message resource. Native physical menu and attack evidence
is recorded separately. This is a native presentation policy, not completion of
the original queued TargetDraw callback.
