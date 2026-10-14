# Portable original forced item95 receive sequence

`FieldItem95Get` binds the finite fresh mode16/mainflags1003 success graph from
[original sequence recovery](GOOMBA3_ITEM95_GET_SEQUENCE_AND_DEFERRED_GRANT.md).
This owner is host-testable and independent of 3DS window/audio/camera services.
Actual resource providers are required before entry; missing readiness cannot
become a successful receive. ItemMain invocation cadence is supplied by its
caller. No fabricated dialogue or render-frame timer substitutes for a service.

First tick executes substates0/1 in the same call: original camera/key/system/
sound holds and motion reset, get-item motion, itemY+50/effect6Y+62, icon flags,
source BGM silence check/routing, name window80313dc8 and help80313df0, then
low-level receive95. Key-item fanfare is requested even when silence suppresses
BGMOff3800. Both windows precede grant; no parameter revision or5405 write is
introduced. The next tick performs substate2 control-off and falls through to3.
Window action must be0 before A/B trigger acknowledgment. There is no added
500ms delay or waiting for a guessed text-finished flag.

Acknowledgment preserves source close/sound/effect/camera/icon order, then
successful system/sound/control/key/stay/camera restoration before deleting all
matching named entries. The owned activebit clears once. Complete ticks do not
repeat grant or teardown. Failure opens original failure window80313e18 after
receive0; acknowledgment performs common teardown, closes failure window,
requests stay/iconflag2 and original selection(kind1,item95,mode1) if supplied.
It remains `DiscardPending` at substate5. Full discard continuation is unbound;
control does not restore, named entries do not delete and quest completion is
not fabricated. Type9 quest hand-in selector is not reused for this menu.

The pouch array provider uses [verified insertion](../unit/POUCH_KEY_ITEM95_RECEIVE_INSERTION.md):
scan zero slots120..0, choose highest, shift preceding slots right, store95 at0.
Gaps/duplicates survive. Full storage returns0 with no writes. The exposed
`insertKeyItem` is this array branch, not the full special-item dispatcher.
Only the original item95 receive service is bound through this portable owner.

Tests check full service ordering, original window addresses and source offsets,
exact one grant using actual packed pouch bytes, silence/playing routing,
window-action/trigger gates, A/B acknowledgment, once-only restoration/name
cleanup, failure-frontier ordering, type1 selection arguments and admission.
Native UI/story acquisition remains unbound until original Mario/window/icon/
effect/audio/registry providers are supplied. [Window and asset recovery](ITEM95_GET_WINDOWS_MARIO_POSE_AND_ASSET_BINDINGS.md)
identifies original M_I_U, icon307/TPL312 and base effect textures44/42;
full effect6 curve/TEV, US template expansion, manager action and lifecycle remain
provider work. This implementation does not claim acquisition is playable,
matched source raster or physical New2DS performance.
