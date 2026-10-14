# Native battle material-light owner and continuation

US G8ME01 revision0. Source contracts:
[selection/channels](MAP_MATERIAL_LIGHT_SELECTION_AND_CHANNELS.md),
[bank queries](MAP_CHARACTER_LIGHT_QUERIES.md),
[producer lifecycle](BATTLE_MAP_LIGHT_BANK_AND_PRESET_LIFECYCLE.md), and
[original SDK channel semantics](GX_CHANNEL_CONTROL_AND_COLOR_REGISTER_SEMANTICS.md).
This is a bounded native implementation, not an original image/numeric oracle.

`MapMaterialLightWork` is the current native battle runtime's bank projection.
Its bank1 starts with the source lightInit count32 and32 complete zero96-byte
records. Bank0 is deliberately unavailable: native field authored-light history
has not been imported into this owner. `initializeStageObjects` loads the actual
rank0 battle maps before publishing map material classification; it now also
requires `material-light.bin`. That pack contains unchanged original authored
light-count/fog bytes from both selected maps and the two original fallback
ambient words. Native load sets selector1 and both fog records, preserves the
light array, and repeats source fallback-ambient initialization. It does not
pretend bmapLoad clears lights. An explicit selected-bank reset clears records
without changing count. No later light-entry writer is acknowledged silently.
The runtime is currently created for this fresh battle; full persistent field
bank history, repeat-battle teardown and broader map loading remain unbound.

`resumeMapMaterialLight` retains source flags/origin, selected pointers, ordered
preset/character queries, fallback selection/replacement, per-light processing,
sticky attenuation/mask, final ambient query and each GX request. Successful
steps never replay after a later hold. It preserves duplicate preset pointers,
replaces the whole selected list on the source multi-preset branch, and ORs IDs
for unrecognized light types even though those types initialize no light object.
Typed point/directional/spot processing is an explicit provider boundary; native
currently leaves it unavailable. Preset getters also require their own live
initialization/main producers rather than a guessed direction.

The native bank query admits known counts0..32 and at most8 active records for
near queries. It preserves source scan/filter order and ambient exclusion,
zero-filled output tail, and the source M<=0 first-write behavior. More than8
active records holds at sorter800064fc; distance/sort and mutable selector-query
side effects are not replaced with a host sorting guess. The native bank owner
is stable during these query calls. Unknown banks/presets remain unavailable.
Custom providers can implement the complete source query/per-light graphs;
using this interface does not itself close those dependencies.

For the observed live Goombella flags0 call, the real initialized bank has no
selected/ambient lights. The helper still requests Fog0 white, NumChans1,
ChanAmbColor4 black, ChanCtrl0(disabled,vertex material) then independently
ChanCtrl2(disabled,vertex material). Native retains full ambient4 and both actual
channel-control calls; RGB and alpha material sources are separate. Existing
combined channel4 callers update both. Disabled RGB/alpha passthrough uses the
existing PICA shader's vertex RGBA; it does not multiply by black ambient. The
shadow TEV backend now chooses its alpha material source independently from
RGB, including intermediate mixed state. Lit controls remain rejected until a
real lighting backend is bound. Existing source TEV state is preserved; this
helper does not acknowledge texture/TEV setup or any geometry.

The material continuation belongs to the battle runtime alongside the retained
shape cursor, so discarded battles cannot leave a renderer-owned stale request.
Nested query/GX pending identities propagate through the shape request to the
actual part/camera queue. Part history and queue completion remain gated on
complete original shape submission, not on this lighting helper alone.

Tests inspect original pack bytes and exercise absent/character/ambient records,
source-order near selection, zero/negative capacity, explicit reset versus map
reload, preset duplication/replacement, typed-light holds, sorter holds, held
final alpha write and independent RGB/alpha control state. Test-supplied preset
and active-light records are fixtures, not original execution evidence.
