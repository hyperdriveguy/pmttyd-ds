# Shared original weapon-menu setup and live target decision

The portable setup/decision module implements complete local
BattleCommandDisplay_WeaponSelectMenuSetup80122068/6c and
_btlcmd_SelectWeaponDecide80123210/150. Existing
 tools/verify_weapon_menu_domains.py pins15 complete/linked bodies,234 anchors,
2 data spans and16 scoped DOL calls. No Dolphin execution is claimed.

Setup retains passed battle/type/flags/menu, calls the builder before the cursor
and UI initializer, then reads current menu flags and ORs01000010. Its private
cursor output is separate and is not initialized or replaced with a generic
cursor. Neither setup nor decision publishes menu type. Typed port readiness
is separate from the original scalar contracts and retains incomplete services.

Decision captures the acting unit before rebuilding the table, reloads the row
weapon at wrapped28*index, resolves enemy side from retained unit and reads the
current actor ID for sampling. Direction uses signed owner byte for addressing
and passes the signed direction byte without the target-count helper's negation.
It samples into the live B428 buffer, then resets selected byteab7 before
reading the current preference flags. The optional loop rereads signed count,
permutation and flags; last same-side or first different-side position wins.
It stores selection position, not permutation, and returns original scalar0.

The tutorial's selectWeapon bridge now calls this real decision with its native
builder and packed sampler, exposing actual target unit/part pairs. It rejects
unavailable rows/overlapping roots before admission. An enabled original Jump
produces Crump2/body1 in the live buffer. It does not publish class type, confirm
a target, register an attack, consume HP/FP, advance a turn or complete tutorial
story. Original class/submenu/input and action ownership are still required.

Host fixtures cover retained actor versus current actor after rebuild, signed
owner/direction addressing, post-sampler reset, first-different/last-same,
combined flags, live flags/count mutation, signed permutation/count and
selection position, explicit unresolved services, setup order and flags changed
by the UI provider. Native tutorial checks the real Crump pair and unchanged
resources/attack state. Private affordability buffers remain separate from
this decision's live target buffer. No menu renderer/playability proof follows.

See [original setup/decision](WEAPON_MENU_SETUP_AND_DECISION.md),
[native tutorial providers](TUTORIAL_NATIVE_WEAPON_TABLE_BINDINGS.md), and
[shared table production](NATIVE_WEAPON_TABLE_PRODUCTION.md). Next: original
class/submenu/target input, menu type publication, selected event registration,
player timing/impact and full turn flow. Physical controls remain mandatory;
lower-screen audience taps are optional.
