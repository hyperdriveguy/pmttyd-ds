# Port code guide

This directory explains implementation choices and how to read the port's code.
Recovered original behavior and its evidence remain in
[behavior contracts](../behavior/README.md); conversion/provenance remain in
[assets](../ASSETS.md). Start with the [source map](../../source/README.md),
[header map](../../include/README.md) and [architecture](../ARCHITECTURE.md).

| Guide | Implementation covered |
| --- | --- |
| [Runtime ownership](runtime.md) | Native main loop, physical input, audio and frontend |
| [Checkpoint bytes](save-format.md) | Native save encoder, decoder and file replacement |
| [Battle core](battle-core.md) | Action commands, private input, status, resources and damage accounting |
| [Event VM](event-vm.md) | Scheduler, operand tags, opcodes, address mappings and allocator services |

## Comment conventions

Explain units, ownership, valid domains and reasons for unusual ordering at the
code that needs them. Give repeated policy values descriptive constants/enums
when that improves the API; a nearby comment/table is sufficient for binary
layout offsets, original opcode numbers and recovered masks. Preserve source
numeric ordering and bit patterns when documenting recovered routines.

Distinguish original constants from native presentation/scheduling policy and
provisional adapters. An original address is an emulated 32-bit token, not a
host pointer. Keep unidentified fields as `field<offset>` and explain the known
read/write behavior; do not invent a semantic name. Explain why a hold resumes
at a particular operation, what state survives, and which provider is missing.
Avoid comments that merely repeat the C++ statement.

## Documentation audit status

The current pass covers runtime main/input/save, the event VM implementation,
and core battle action/status/resource/damage helpers.
Audio/frontend service notes are included in the runtime guide. The large
`Game` composition, rendering/animation, field, battle, message, inventory and
story implementations still require a systematic inline numeric/behavior audit;
existing comments and contracts in those areas do not imply complete coverage.
Progress and exact validation are recorded in [PROGRESS](../PROGRESS.md).
