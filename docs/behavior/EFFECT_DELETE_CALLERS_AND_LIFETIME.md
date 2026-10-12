# Effect deletion caller evidence,name lifetime and fog correction

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
[Complete leaves](EFFECT_SOFT_AND_HARD_DELETION.md) pin exact soft/hard behavior.
[Machine-readable271-site census](../../tools/effect_delete_call_sites.json)
contains original site,target and containing-symbol annotations. Verify:
`python3 tools/verify_effect_deletion.py`. Census is static DOL code evidence,
not an inventory of every original scene event or runtime invocation.

## Exact direct-call census and its limits

Scan aligned linked-branch words in the seven DOL text sections,decode relative/
absolute branch displacement,and retain these two exact targets:

| Target | Meaning | DOL sites |
| --- | --- | --- |
| 8005e9c0 | effSoftDelete | 30 |
| 8005ea30 | effDelete | 241 |

The271 `(site,target)` pairs are independently recomputed by the verifier.
Containing-symbol names help an implementation owner locate the next source
contract;they do not prove that caller's branch conditions,arguments,frequency
or complete body. REL imports/indirect pointers,raw flags/record resets,pool
reinitialization,animation frees and unaligned instruction data are excluded.
Maya release is a separate full local leaf;this target census does not count
its call sites. No broad “all game effects free this way at scene exit” claim.

## Fog expiry requests SOFT deletion,then forgets its pointer

The [complete fog/first-process contract](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md)
contains a direct call at80147c80 to8005e9c0. That is effSoftDelete,not
 effDelete. The earlier fog summary mislabeled the helper name;the numeric
address and containing-body fingerprint were correct. The wording is now
corrected and this distinction must guide native binding.

For a nonzero timer that decrements/reloads to signed<=0,FogEndCheck reads
pointer B+182bc. Nonnull requests effSoftDelete(pointer),then stores B182bc=0.
If the record's current flags include02,the leaf only ORs04;the payload and
occupied record can survive AFTER the battle work has forgotten its pointer.
If flags are nonzero without02,it frees immediately then clears record flags.
If flags0,it performs no payload release,while fog still clears its own pointer.
Zero incoming timer skips even the effect-pointer read,as the earlier complete
contract states. There is no caller-side check that deletion actually freed.

Therefore native fog expiry must not bind this address unconditionally to a
hard-free operation. Timing,fade/update consumption and actual scene lifetime
still need the relevant fog effect's update/source contract. The source does
not promise a particular number of frames between request and final release.
This correction changes interpretation of a service request,not the recovered
fog timer arithmetic or its ignored-result caller protocol.

## Global name polling remains active through soft request

[effNameToPtr](JUMP_MAJINAI_EFFECT_AND_NESTED_SOURCES.md) admits any nonzero
record flags and returns the first equal instance name. Soft02→OR04 leaves
flags nonzero and preserves the name,so it remains searchable. Hard deletion
flags0 makes that slot invisible to name lookup even though name bytes remain
unchanged. A different same-name active slot can still be returned.

The battle Majinai wrapper's E78 is only a nonzero gate;it polls by name,not by
retained pointer equality. A soft request alone does not unblock that wait.
An actual hard release or other driver mutation making ALL same-name candidates
ineligible can do so,under the name lookup/record rules. Deleting one duplicate
is not guaranteed to end the wait. The constructor's flags3 makes its ordinary
soft-delete path7;which Majinai update transition consumes04 remains open.

Pool acquisition tests exact flags0 and can reuse a hard-deleted slot despite
stale payload/name-tail/metadata. It then clears its specified fields and only
the first name byte. This is record-address reuse,not a stable generation/ID
handle. Source owners retaining an old E pointer can refer to a later object
at that address;there is no generation check in these deletion/name helpers.
Do not invent automatic pointer invalidation or exclusive name ownership.

## Payload free is distinct from event/scene ownership

Hard deletion and soft immediate fallback free heap3 payload through __memFree,
then clear only record.flags. They do not free the record bank,close animations,
delete event E78 owners,stop sound roots,clear battle fog fields,cancel message
windows or reset map-work markers. Those owners need their own service graph.
A parent script can delete while an independent audio/effect request continues;
blanket native destructor cleanup is not proven by these leaves.

The [SDK free contract](SDK_HEAP_RELEASE_AND_CALLER_CENSUS.md) supplies allocation-
list unlink,address-sorted free insertion and coalescing. Map _mapFree has a
different header/null/size policy and is not the effect heap3 release operation.
Native providers must retain source heap handle/address/lifetime and explicit
invalid-domain behavior rather than treating every original pointer as a host
`delete` target or resetting a semantic object wholesale.

Acceptance combines leaf fixtures with original callers:03→07 soft name remains
visible;hard release hides flags while retaining name bytes;duplicate name still
visible after one deletion;fog forgets pointer AFTER soft request,without an
assumed completed free;slot reuse and stale raw pointers;payload free versus
record/event/scene cleanup;and exact30/241 scoped DOL targets. Original update/
scene/backend/callback/REL/raw writer and runtime traces remain requirements.
