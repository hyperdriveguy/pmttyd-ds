# Retained cold battle initialization

The portable `resumePackedBattleColdInit` owns the complete US `BattleInit`
parent at `800f8f04..800f9038`, independently of the later `btlseqInit` sequence.
The [original disassembly](BATTLE_INIT.md) defines its order. The allocation and
first clear are exactly `0x19098` bytes; the native tutorial arena's larger
capacity does not change that source length.

The cursor preserves the allocation result, information argument, program
position and stable service registration across an unavailable child. Each
completed allocation, initializer, clear and parent store executes once. The
two SetSeq calls retain separate callsite identities. An engaged scalar zero
completes a child; an unavailable result holds it. A null allocation therefore
reaches the original subsequent memset call with null rather than silently
becoming a successful or retried allocation.

Memory stores require admitted synchronous memory services. Child providers
must retain any partial work of their own, including memset or an initializer.
Every child request carries its actual source function, callsite and explicit
arguments. The source empty action-help initializer is still an explicit child
identity; no missing initializer is declared complete by this parent.

Recursive invocation/reset and changed information/service/lifetime bindings
are rejected. A retired registration remains retired until explicit reset.
Successful completion releases the cursor; its caller owns whether another cold
initialization is appropriate. No source scratch stack is mapped into guest RAM.

`tools/verify_battle_cold_init.py` pins all 312 original bytes and compares all
46 ordered child calls and parent stores against the read-only ISO. Focused
host tests hold each of 17 children twice, compare the complete write/call
history with uninterrupted execution, and exercise reentry, changed binding,
retirement and actual zero returns. These are parent mechanics checks. This
owner alone does not bind native initializers, construct actors, finish the
Crump tutorial or return to the field.
