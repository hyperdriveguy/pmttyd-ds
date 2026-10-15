# Native audience lifecycle incarnations

The per-VM `BattleAudienceIncarnationOwner` records native metadata at committed
original audience lifecycle writes. It proves which observed actor and held-item
incarnations a displayed request names. It does not prove visibility, harmful
eligibility, completed scene publication, or a runnable battle input owner. See
[NATIVE_SELECTED_AUDIENCE_INTERRUPTION.md](NATIVE_SELECTED_AUDIENCE_INTERRUPTION.md)
for the separate selected harmful-holder admission and original physical X path.

## Source boundaries

The original allocation is `A = B + 0x2858`. Actor records are
`A + 0x1bc + slot*0x134` (200 slots); item records are
`A + 0xf25c + index*0x48` (100 slots). Hooks use already captured addresses and
operands. They perform no additional guest reads or writes.

- Init (`801a83f8/1c4`): reset immediately after the actual `0x13914` byte fill.
  Later Init provider unavailability does not undo this completed source reset.
- Entry (`801a3400/1d0`, Entry_Sub `801a31a8/258`): birth after flags `5/85`;
  readiness only after Entry_Sub completes and any paired reservation flags
  `101` are written. A reserved companion is not an independently born actor.
- Delete (`801a3158/50`): clear after each actual actor flags-zero write,
  including the paired companion when source flags `80` require it.
- ItemOn (`801a5aac/1fc`): birth after original flags-or-1; readiness and the
  actual captured holder link after ChangeStatus and the holder's item-index
  store. A failed or incomplete call cannot synthesize readiness on retry.
- ItemMain (`801a4f0c/958`): clear immediately after its shared flags-zero
  write, before the remaining source count/state/unlink writes.
- Holding/damage controller (`80191a48`): clear after its flags-zero write.
- SetTarget (`801a2bec/9c`): clear after each other holder's item flags-zero
  write. The selected holder remains alive under the original behavior.

These are the item writes that lose alive bit 1. Original visible-bit `|2`,
receipt hide `&~2`, and state-6 blinking preserve item incarnation. Bulk Init
clears every incarnation. Generic guest stores and flag polling are not lifetime
producers.

## Native ownership and retained calls

`EventNativeRuntime` constructs one immutable shared owner binding. Its initial
state is unknown. `nativeBattleActorServices` exposes its optional, default-inert
synchronous lifecycle observer. Actual Init reset alone binds an allocation and
mints a fresh shared token. Actual births mint monotonic serials within that
allocation. Unknown pre-existing records remain unknown even if their bytes look
active. Slot reuse cannot inherit a previous actor's item link; a reset token
cannot validate an earlier frame. Overflow and invalid record addresses fail
closed.

Both ordinary audience-check and targeted interruption cursors compare observer
user/function identity while held. The retained Main owner must also compare
both fields because its children can reach lifecycle writes. Providers must not
replace this observer midway through retained work. Hooks add no resumability to
otherwise synchronous Entry or ItemMain implementations.

## Validation and integration frontier

Focused tests exercise actual source Init, Entry, Delete, ItemOn, ItemMain,
holding damage, and SetTarget boundaries; partial Entry, paired reservations,
reuse, receipt hiding, blink flags, observer swaps during holds, per-VM factory
ownership, and complete guest-byte/read/write trace equality with hooks disabled.
The native owner contributes identity only. Normal battle population and completed
scene visibility still require their actual source owners; a complete published
scene must supply real camera projection, actor quads and visibility, and current
simulation admission must recheck the selected harmful holder. No displayed tap
may mutate simulation directly or synthesize physical X. The original physical X
first-eligible behavior is unchanged.
