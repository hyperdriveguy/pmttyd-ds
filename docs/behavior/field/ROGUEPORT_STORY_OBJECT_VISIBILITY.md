# Rogueport ordinary-entry story object visibility

US G8ME01 disc0/revision0, static evidence B from the read-only ISO. This
adapter covers recovered map group bit1 requests in outdoor room initialization;
it does not execute the complete room event graph or scene overrides.

`rel/gor.rel` SHA-256:
`d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a`.
The original init span fingerprints are retained in
[road evidence](../../../tools/overworld_road_evidence.json). All offsets below
are REL file offsets. Story is encoded operand `0xf5de0180`, adapted to the
native persistent `originalStory` value; see
[harbor entry bindings](../story/STORY_HARBOR_OPENING_BINDINGS.md).

## Recovered initialization requests

| Source subtree | Requests and ordinary-entry visible condition |
| --- | --- |
| Harbor `S_fune_a`, `S_a_hikinami` | Hide at 0x2f378/0x2f390. Later scripted arrivals/departures remain separate overrides. |
| Harbor `S_fune_b` | Hide at 0x2f3bc. Nested story !=402, !=404, >=268 at 0x2f758..0x2f770 runs child 0x2bbc8, which clears bit1 at 0x2bbc8. Visible when story **>=268 except402/404**. |
| Harbor `S_fune_c` | Hide at 0x2f3e8; switch case range172..229 clears bit1 at 0x2f6ac. Earlier separate case217 takes precedence. Visible172..229 except217. The bit2 clear at0x2f42c is a separate flag, not visibility. |
| Harbor `S_c_hikinami` | Hide at0x2f400; later wake animation/scene requests remain unbound. |
| West `S_kou_kusas01/02/03` | Hide all at0x5a698..0x5a6c8. First matching CASE_LESS selectors56/112/164 reveal01;215/261/339 reveal02;373/401/default reveal03. Thus visible01 below164,02 from164 to338,03 from339. |
| East `S_ie_04_naka` | IF_LESS(story,23) at0x4d874 hides subtree at0x4d880. |
| East `S_saku04` | IF_GREATER_EQUAL(story,10) at0x4daf0 hides subtree at0x4dafc. |

## Native representation

[Converter groups](../../../tools/map_visibility.py) identify source subtrees.
Descendants inherit group identity; siblings inherit their parent's identity.
Material batching splits at group boundaries so a hidden ship cannot suppress
unrelated dock geometry sharing its material. Bits26..31 of batch metadata
carry the event group separately from layer, cutaway and GX culling fields.
An absent native story word uses the conservative story0 visibility envelope
without writing or inferring a persistent story value.
[Portable visibility](../../../include/map_visibility.hpp) selects the group
from persistent story state on every draw, including translucent draws. No
story-state change requires geometry extraction or deletion. Collision and
animation bindings retain their independent source data.

DOL callback0x80038468 resolves scope/action/name and reads the raw flag mask.
Scope1/action1 calls0x8001edd0, then0x8001f1fc to OR flags into the named
group and descendants; action0 calls0x8001ecb8 then0x8001eee8 to clear them.
The source recursive helper walks child and sibling links below the selected
root. This is why source subtree ownership, rather than a material filter,
is required.

## Validation and limitations

[Tests](../../../tests/test_map_visibility.py) exercise prologue states2..6,
ship thresholds/special cases, west first-match thresholds and east conditions.
Read-only ISO conversion checks compare all vertex records before/after
partitioning and prologue-visible geometry against independent subtree omission;
wall/ground/head packs must be byte-identical. Packed fields remain independent.

These are static/native checks, not Dolphin observations. Full bit2/camera
cutaway semantics, NPC/item/door activation, harbor story2 arrival and later
ship/wake choreography need their original event bindings. The station retains
its existing ordinary-entry gondora_2/hako_1 omission. No claim of complete
prologue scene or full-game parity is made.
