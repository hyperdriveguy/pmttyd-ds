# Original weapon-submenu Main/End and physical tutorial input

The shared Main/End implementation follows complete source80121d0c/35c and
80121cb0/38. It preserves captured battle/type/unit, key admission, separate
private cursor output, eligibility before input, pending-byte/word clearing,
confirmation before navigation, enabled/disabled/reason ownership, ordered
End/decision/target/light requests, signed cursor arithmetic, boundary Trigger
versus interior Repeat, forward priority, special Item cancellation, audience
result0 and exactly one common display on every completed local path.

End requests UI End before masking only flag10 from current menu flags. Linked
friend/enemy light and UI Main bodies are actual blr stubs; native callbacks
for those exact leaves perform no extra work. Source sounds retain raw IDs/
keys (2520,2521,802efe88,802efe98), not generated tones. Sound playback remains
an external consumer.

The complete key-admission leaf80119df8/2c checks pending first, then current
UI-work readiness byte14. Native tutorial Open projects confirmed class/type
and source setup, allocating a private UI-work view. It leaves readiness clear
until an explicit presented signal. The bridge does not derive readiness from
elapsed ticks. Actual original UI constructor/animation and class control remain
unbound; Game does not yet display/activate this submenu after intro.

General physical BattlePadWork is now provisioned and sampled separately from
private defense/action history. Trigger and Repeat use latest source general
pad fields. Native input runs the real Main, exposes cursor/target/reason state,
and can confirm with A or cancel with B. Tests prove no confirmation before
presented acknowledgement, actual Crump2/body1 target setup and unchanged HP/
result/no attack root. Native UI End/target setup are immediate presentation
projections, not proof of original UI lifecycle or full player execution.

The shared complete audience interruption8011f524/cc preserves captured battle,
X-mask400 early return, gravity/Mario blockers, actual item-output provider,
previous type after ActMenuEnd, type11/state29, light request and live flags OR4.
No item value is fabricated. The tutorial now binds whole GetItemOn, complete
[SetTarget and retained native UI interruption](AUDIENCE_TARGET_SELECTION_AND_NATIVE_INTERRUPTION.md).
Trigger0 takes its source early return; an unknown UI owner holds without
repeating acknowledged target work. This does not close whole outer command
scheduling, actual AudienceAttack/throw prevention or lower-screen taps.

Host fixtures cover admission/display, simultaneous confirmation/cancel,
reason reread after End, pending republish/sound omission, navigation boundaries/
priority/post-repeat cursor reread, all cancel variants, audience result0,
UI flag preservation and source interruption publication/retention. Existing
menu and interruption verifiers pin full static US bodies/anchors. No Dolphin
or native menu rendering/animation/playability observation is claimed.

See [source Main admission](WEAPON_MENU_MAIN_CONFIRMATION_AND_LIGHT_REQUESTS.md),
[navigation/End](WEAPON_MENU_MAIN_NAVIGATION_CANCELLATION_AND_END.md),
[audience writer](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md), and
[native setup/decision](NATIVE_WEAPON_MENU_SETUP_AND_DECISION.md).
Next: original class/target controls and UI, command type/selection ownership,
player action/timing/impact/turns and real presentation/audio/audience consumers.
Keep original menu/HUD assets and physical input; touch must remain optional.
