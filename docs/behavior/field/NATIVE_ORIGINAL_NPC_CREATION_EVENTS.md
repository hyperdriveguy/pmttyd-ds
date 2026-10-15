# Original NPC creation callbacks in the native event VM

The native bridge retains complete original US evt_npc_entry800536e4 and
 evt_npc_set_tribe8004e5c0. It binds the actual event entry, argument RAM,
registered actor storage, original tribe table, shared animation file cache and
forced animation selector. It does not complete ordinary Game scheduling or
the full prologue. See [progress](../../PROGRESS.md) and the
[request inbox](../../requests/EVT-01.md).

Entry resolves name and model in source order, reads fresh GP14, and calls the
original asynchronous group parent. Source result0 ends one event poll; the
next visit resolves operands again. Any nonzero result reaches the original
constructor and returns2. Native unavailable requests retain their current
invocation separately from source0. The original model-name guest address is
preserved through the constructor, including its original hoshi fallback.
Source strings and actor allocations are shared at the same guest addresses
between event and animation VMs. No model-name scratch replacement is used.

SetTribe publishes the original descriptor pointer before copying its stay,
talk and optional initial pose. Descriptor and actor captures survive native
holds, while later descriptor fields remain fresh source reads. A nonnull
initial pose reaches actual forced SetAnim using the actor's real +2c buffer.
Metadata reads/stores and final return1 preserve original order. A null initial
pose skips copy/SetAnim. The portable source path publishes a null descriptor
before its unsupported dereference; native admission rejects that unsafe
profile before effects. It never fabricates success.

The registry owner excludes consumers, constructors, reset and setup while a
SetTribe mutation is incomplete. One invocation belongs to one live event ID,
entry and function. Completion is cached until that USER_FUNC consumes its
original scalar; another caller cannot consume it. Retirement quarantines the
retained mutation, without cancellation or rollback. Area checks quarantine
before manager visits and after runManager before completion publication.
Explicit cache pumping and resume do not advance interpreter time or input.

The editable source asset retains gor_00.o::enter_gorotsuki_town_init at
90505208, all e8 original event bytes plus its real scanner prefetch word,
reached source strings and original SDA zero. It creates Goombella, Crump and
two subordinates in original order. Entry2/SetTribe1 pairs yield before the
four final position callbacks hide each at (0,-1000,0). It invents no regular
or initialization event. Original model/tribe names and pointers are preserved.

Tests run that exact child through the actual manager/cache/registry/selector,
with three existing Mario poses. They check source polls, native holds, raw
pointer retention, forced selectors, hidden current/previous positions,
unchanged Mario ownership, consumer exclusion and retirement quarantine.
These are source-derived native checks, not original Dolphin observations,
physical hardware profiling or full ordinary prologue execution.

Native admission requires stable source mappings, strings, services and VM
lifetimes while aliased. It supplies bounded strings, supported field mode and
backed descriptors. Conflicting mappings hold instead of replacing memory.
The source VM and registry must outlive their alias bindings. Game must drain
this owner before its animation/presentation clock gates when integrating the
full original area parent. GPU drawing, scene dialogue/battle and return remain
separate unfinished owners.
