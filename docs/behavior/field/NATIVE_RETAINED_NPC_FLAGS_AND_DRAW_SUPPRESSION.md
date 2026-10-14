# Native retained NPC main flags and draw suppression

Source [registered/hidden actor admission](GARF_ORIGINAL_TALK_AND_REGISTERED_HIDDEN_ACTOR_ADMISSION.md)
and [actual house05 installer/consumers](HOUSE05_ORIGINAL_INSTALLER_GW11_AND_NPC_LIST_CONSUMERS.md).

Game's exact original-name lookup now searches retained initialized active1
instances in the current room/catalog generation. It does not use draw800,
query2 or the catalog's visible() predicate. Missing/unknown/uninitialized names
remain unavailable. Script-owned Professor writes remain outside the setter's
domain. This is literal-name native ownership, not the full original me/party/
evt_npc_flag_onoff ABI.

setFieldNpcFlags applies OR/AND to the real body and movement flags immediately.
Static profiles retain externally written bits while publishing untouched
catalog bits normally; wandering profiles retain their existing mutable movement
flags and driver writes. Position changes while hidden preserve those flags and
still seed both previous/current roots. Map/catalog synchronization discards
these transient instance writes; they are not new saved switches.

The existing native actor draw path skips main800 before geometry emission.
Hidden actors still update pose/matrix, collision and candidacy under their
existing source gates. Inactive1 slots cease to be name-addressable. No pose,
direction, key item, quest, door record or material flag is changed by hiding.
The bit gate also avoids submitting hidden geometry through Citro3D.

Registration construction/pruning still follows the finite native catalog's
story/interior/system coverage. Consequently this is not full original map/NPC
registration lifetime across those coverage changes. Future source callbacks
also require their own supported movement/physics providers; accepting a main
flag write is not a claim that every resulting source profile is implemented.

House05 must **not** receive a speculative800 hide/show. Its actual manual
installer omits the generic initial hide tail; copied record word0 is0, so all
generic mask1 hide guards are false. Its unguarded NPC-list consumer instead
sets/clears animation-pose material flags1800 via actor+104. Main800 and that
material state are different owners. Parameter selector0 itself only writes
the copied record+34 pointer; it performs neither operation immediately.

Validation covers static/wandering retention, lookup and reposition while hidden,
unchanged side/sphere admission, unlock's40000020 write, inactive lookup and map
reset. An isolated diagnostic-only native build toggles Garf's actual800 through
the same API on B; inspected images show visible/hidden/visible. That temporary
main hook is absent from the committed/normal build. This verifies the native
state/draw path, not a completed original event or house05 interaction. Exact
trees, artifacts, builds and timing are recorded in PROGRESS.md.
