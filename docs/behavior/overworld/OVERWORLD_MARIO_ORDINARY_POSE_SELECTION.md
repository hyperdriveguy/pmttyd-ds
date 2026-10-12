# Original ordinary Mario pose requests and facing selection

US G8ME01 revision0, motion0/1/2/3/10 and Mario pose/pre-display owners.
Read-only DOL and original a/a_mario,a/a_mario_r metadata. This closes a bounded
ordinary Mario selector; it is not a complete animation renderer or original
runtime observation. See [render scale/root composition](OVERWORLD_ACTOR_RENDER_SCALE_AND_IDLE_BOUNDS.md)
and [frame clock/loop contract](OVERWORLD_MARIO_ANIMATION_CLOCK_AND_LOOPS.md).

## Ordinary requests

Domain: character byte3c=0, normal Mario main80000000 clear, no special pose
lock display2, no transformation/main2000 or pushing/main10000, ordinary motion
entry. HP>5 is the bounded idle baseline; retained speed scale228=1 is the
ordinary dash baseline. Non-entry callbacks need not request/reset a pose.

| Motion | Front pose request | Literal and caller |
| --- | --- | --- |
| 0 stay/idle | **M_S_1** | 80421008,800a6e54–58 |
| 1 walk | **M_W_1** | 80420884,80087b44–48 |
| 2 dash, scale228==1 | **M_R_1** | 80420870,80087774–78 |
| 3 jump | **M_J_1B** | 802c40c8,8009b570–78 |
| 10 fall | **M_J_1C** | 802c40b8,8009affc–8009b000 |

Idle M_Z_1 is a valid asset/default pose, but is **not** the ordinary stay
request. Its previously documented frame0 bounds do not establish M_S_1's
art-scale framing. Bake the requested original pose geometry with its root,
not an arbitrary TPL page crop or default animation index0.

Dash uses M_W_1 when retained speed scale228!=1 (80087780–a8). Walk's special
main2000 requests M_W_7 at80087b00–10. Pushing main10000 uses M_O_1 unless
marioChkPushAnime recognizes an already active push name; checks recur in
walk/dash tails (80087c90–a8,8008793c–58). Those special branches cannot be
inferred from stick magnitude alone. Character1/2 route separate owners.

Idle HP0 requests M_I_S at800a6de0–e4. Low positive HP<=5 and marioChkKey's
positive service result can request M_S_3/display4 at800a6e14–30; this is an
input service, not a badge check. Idle otherwise retains exact M_I_Y rather
than overriding it with M_S_1 (800a6e40–58). Main2000 requests M_U_2 with
start frame10 through marioChgPoseTime (800a6e60–68). Long-idle/head/special
state branches beyond the bounded baseline remain their explicit dependencies.

Jump has main2000/character guards before the request, and re-requests J1B
when its retained phase/counter reaches the source pose branch at8009b638–658.
Motion3's vertical apex/descent can retain J1B until the later motion10 entry
requests J1C; choosing pose solely from vertical velocity would change source
ordering. Motion11's fall entry intentionally skips the ordinary J1C request
at8009afc4–c8. Preserve current/retained motion and entry state.

## Rear selection and actual model files

marioPreDisp80057878 calls marioRearAnime80057cfc for characters0/1 at800578b4,
**before** its pose upload and before marioMakeDispDir. In the bounded normal
flags domain, compute

    heading = revise360(Mario1a0 - Mario19c)
    rear = heading <= 70 or heading >= 290   # inclusive boundaries

The source loads1a0/19c at80057d38–40; comparisons are80057d48–64.
Literal290 is80420060 and70 is80420064. This is **not** the separate draw yaw1ac
or a normal-vector/backface test. Source degrees0=-Z,90=+X,180=+Z,270=-X.
For fixed camera eyeZ>atZ, heading0 is the rear sector.

Source toRearPose800566cc has explicit mappings, not a generic appended suffix:

| Front, model a/a_mario | Rear, model a/a_mario_r | Front index | Rear index |
| --- | --- | --- | --- |
| M_S_1 | M_S_1R | 2 | 1 |
| M_W_1 | M_W_1R | 4 | 2 |
| M_R_1 | M_R_1R | 5 | 3 |
| M_J_1B | M_J_1BR | 7 | 4 |
| M_J_1C | M_J_1CR | 8 | 5 |

Other pairs such as M_Z_1/M_Z_1R also exist. Already-rear names return no new
rear mapping; the front reverse lookup and retained rear flag preserve them.
Do not call the no-new-mapping result an absent model. toFrontPose800561b8
provides explicit inverse pairs. Rear admission with a match sets display80000000
at80057d78–80 and queues the changed name; ordinary front admission clears
that flag and converts a rear name back at80057f88–fa0.

The no-special domain excludes display70000004 or main2000, which bypasses
this ordinary angle path at80057d20–34. Unknown/nonmapped poses retain source
fallback behavior rather than a fabricated rear animation. The selected name
and selected model handle must agree.

marioAnimeGroupData80415eb0 contains four triplets. Normal variant0 is
(a_mario,a_mario_r,e_mario) at802c18a0/18a8/18b4. marioPoseInit8005ba50 indexes
variant byte3d with stride12, creates each pose with **type2**, and stores the
three handles atMario22c/230/234 (8005ba58–ad0). Variants_l/_w/_wl and effect
poses are separate source assets. PreDisp chooses22c front,230 when display
80000000,234 when display10000000 (80057904–944).

Read-only source file identities:

- a/a_mario:215124 bytes, SHA25661b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293.
- a/a_mario_r:80784 bytes, SHA25602a70dfe8cf7bb614441eb71aee597099d93cd5673cb8a43dcf01bdf09099119.

Rear geometry/texture binding is real authored data. It is not obtained by
mirroring front opaque pixels or changing only a quad's facing flag.

## Request, restart and pre-display order

marioChgPose80058d84 returns unchanged if display2 is set or requested name
matches Mario18 (80058da4–dc8). A changed ordinary request stores18, sets
start frame halfword28=0, clears specific pose display flags and sets pending
Mario0c1000 (80058dd8–df8). No automatic restart every simulation tick.
Rear/front substitutions use the same changed-name/start-frame/pending writes.
Character/event/custom main branches have additional mappings and locks.

PreDisp performs rear selection; if admitted, updates retained display direction;
then handles pending1000/4000. It clears the pending bits, calls
animPoseSetAnim(selectedHandle,name,**force1**) at80057944, optionally uploads a
paper animation, and calls animPoseSetLocalTime(selectedHandle,float(Mario28))
at800579b8. It then calls animPoseMain(selectedHandle) at800579f4 on this same
callback. A front↔rear name change therefore restarts the selected pose using
source start-frame ownership; preserving fractional animation phase across
such a change is not this source path.

## Heading transforms and remaining turn owner

The original draw wrapper still uses the matrix from the existing render
contract: yaw=revise360(Mario1ac−Mario19c); reflection is **Z scale−1** for
revised1ac inclusive90..270, else+1, before draw-root/pivot composition.
Rear-selection input1a0 and drawing input1ac are distinct retained words.
Camera19c is actual retained input-heading offset, not an inferred billboard
angle. Keep engine*.01 actor units and source scalar2 independently of these
heading rules.

[The retained display-direction contract](OVERWORLD_MARIO_DISPLAY_DIRECTION.md)
closes fresh heading0, reinitialization retention, ordinary wall-fallback facing
and the target0/180 turn machine in marioMakeDispDir800583a8. Rear selection
runs before that update; draw consumes the updated1ac. Other direction event/
road writers and special target/flag domains remain explicit dependencies.
A native static-pose atlas remains a projection/bake adaptation unless it includes
required pose yaw/reflection/pivot effects and actual retained turn state.

## Validation

Run `python3 tools/verify_overworld_mario_pose_selection.py`. It checks the full
motion/selector/pre-display/time bodies and exact literal strings, and imports
both original model files read-only to verify the named directory indices,
loop metadata and keyframe times. Finite rear-sector/loop-derived checks do not
execute original SDK/Gekko draw or animation interpolation.
