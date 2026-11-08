# Native scene actor GPU texture lifetime

This is a native resource policy, not a claim about original GameCube allocation
behavior. Original animation banks, pose metadata, UV scales and texture pixels
remain unchanged. See [assets](../../ASSETS.md). The existing RGBA8 `.t3x`
headers describe quest Mario1024×512, trio Mario512×512 and trio
Goombella512×256:3.5MiB of texture pixels in total, excluding metadata. This
is a resource-size calculation, not measured native free memory.

`Renderer::init` loads the quest13 and trio animation/UV metadata but defers the
three corresponding GPU atlases. Before beginning a GPU frame,
`prepareSceneActorTextures` synchronizes only when residency changes, retires
inactive atlases, and imports real `.t3x` resources for active consumers:

- Trio battle: Mario and Goombella attack atlases.
- Quest13: Mario atlas while `M_N_5B` or `M_N_C` selects it, provided higher
  priority trio/harbor/professor bindings do not own Mario's texture.

Room entry synchronizes and retires these atlases before staging field resources.
The next active consumer can reacquire them. Import failure reports a development
notice and prevents ordinary scene drawing with an absent texture. No fabricated
texture handles or placeholder pixels satisfy admission.

Debug short boot retains the exact checkpoint rejection reason on screen. This
harness text is separate from original story dialogue; missing/invalid saves,
collision, ground, blocked position and renderer import rejection are distinct.
The existing room/collision transaction retains the previous playable room on
failure.

## Frontend-to-field legacy Mario admission

Valid room changes retire the frontend legacy Mario atlas after GPU
synchronization and before destination map/NPC staging. Its original
1024x1024 RGBA8 pixels occupy4MiB; genuine native field Mario uses a separate
source bank. Existing scene preparation imports whichever bank the committed
consumer requires before drawing. A failed Renderer room transaction restores
its former legacy atlas after attempting prior-room restoration. Failed texture
restoration remains a real error; failed prior-room restoration leaves GPU
residency absent. Invalid room IDs cause no retirement.

An isolated native east-checkpoint trace measured199,296 free linear bytes at
Garf's failed524,288-byte allocation, whereas the earlier10MiB report occurred
after rollback. Early legacy retirement raises its actual pre-allocation free
space by exactly4MiB; the same original texture imports/decompresses successfully.
Physical file navigation and Right/Left/idle pass in the private corrected run.
This is native port allocation evidence, not original GameCube or hardware parity.
No original HUD/actor pixels or texture quality are reduced.

The outer main checkpoint lambda can still reject candidate animation-process
retention after a successful Renderer room commit. That separate CPU/GPU
transaction gap is not closed by this resource-ordering fix.

## Evidence and limits

The valid harbor fixture passed host checkpoint/ground/walkability checks but
native import failed at `romfs:/gor_00-32.t3x`. With the three atlases deferred,
Azahar admitted the original harbor and the physical Left/Up/Right crate-gap
route reached setup2's original stage, actors and HUD. Captures are private in
`artifacts/azahar-tutorial-admission-lazy/` and
`artifacts/azahar-tutorial-admission-route/`. The exact reviewed final packed
build repeats the physical route in
`artifacts/azahar-tutorial-admission-final-packed/`; an earlier incomplete review
package rendered black and is not successful admission evidence.

The tutorial stops at the existing source action-service boundary after the
intro. This policy does not implement those services, complete turn scheduling,
prove hardware memory headroom, or add audience/bottom-screen scene rendering.
