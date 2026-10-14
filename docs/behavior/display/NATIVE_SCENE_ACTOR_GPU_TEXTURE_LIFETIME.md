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
