# Native Harbor ending field contribution

The actual entered battle ticket admits the source-ending field pointer, its
NPC+8 load and NPC140 OR100. Admission checks the same VM/context/registry
identity, inactive manager, Battle phase, sealed ticket and live admitted battle.
Unknown, stale, foreign and returned tickets cannot access this contribution.
The context and Encounter facades preserve immutable birth identity before
borrowing host state; copied facades cannot forward the original service.

Only actual field+8 and the admitted live NPC+140 are readable here. The latter
retains its consumed source load across a held continuation. The only allowed
write is that captured value OR100, applied once to actual mapped NPC storage.
This preserves the source inline load/store order if another mutation occurs
between them. Repeat completion verifies the actual written word. No native
scene flag, fake NPC handle or new guest field work is substituted.

The existing source-sequence test now covers actual constructor/start/entered
setup, rejected absent tickets/addresses/unread writes, captured-load mutation,
once-only OR and rejection after return. Original source Wait, control/Post
and result proof tests still run. Focused strict host and changed-source
ASan/UBSan pass; parent core is uninstrumented, leak scanning disabled. ARM
syntax checks cover all three changed implementation TUs. Fresh fullgraph
make test/all and production field/ending activation remain separate.

This contribution neither sets the field result nor acknowledges original
btlseqEnd/Sequence Main. Actual synchronized teardown, canonical pouch export
and field presentation must still precede the sealed returned receipt.
See [field sequence](NATIVE_HARBOR_FIELD_SEQUENCE.md).
