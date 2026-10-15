# Native shader binding across graphics lifetimes

`native_graphics_program` owns one actual parsed copy of the existing compiled
world shader for the process lifetime. It supplies a valid shader binding while
individual Renderer/C2D programs are destroyed. It performs no draw, texture
import, source execution, RNG or guest-memory write.

## Observed installed SDK behavior

The actual installed Citro3D1.7.1 archive SHA256 is
`3f34eff859c1f1e60ebe597b6df9cf5e5e74563d9e64ca01a7b3292fde63016a`.
The actual Citro2D archive SHA256 is
`f6c5352a838639c486b806b8dc847c74a1a190d98b09eb469d5c30a02ae6542f`.
The verifier pins both archives: a C2D upgrade could change teardown bindings
even when C3D stays unchanged. ARM disassembly of the linked native probe shows:

- `C3D_BindProgram` retains the prior program pointer in its static context and
  dereferences that program's shader/DVLE metadata when comparing code identities.
  It reads the new program's shader fields before checking the old pointer;
  passing null is not a supported unbind operation in this binary.
- `C3D_Fini` frees command buffers and clears the active flag, while retaining
  the previous program pointer. `C3D_Init` initializes many fields but does not
  clear that pointer. A fresh Renderer instance therefore cannot itself protect
  the SDK from a shader freed during the previous graphics lifetime.
- Actual `C2D_Fini` removes its FrameEnd hook and frees shader/DVLB/linear memory.
  It does not bind another program after the parking bind. Its CPU shader frees
  do not replace Citro3D's cached pointer.

Before this fix, an isolated real Renderer shutdown/fresh-init font probe showed
four nonfatal unmapped Read32 diagnostics in `C3D_BindProgram` during reimport,
although the second generation rendered correctly. This is an actual native
lifetime diagnostic, not a source GameCube behavior or executed-PPC claim.

## Ownership and failure order

Immediately after successful `C3D_Init`, before `C2D_Init` or any Renderer shader
can become cached, call `prepareNativeGraphicsParkingProgram` and bind it.
Preparation parses actual `world_shbin`, checks the real DVLB and first DVLE,
checks `shaderProgramInit` and `shaderProgramSetVsh`, and retains the real SDK
program/instance/DVLB. Any failure stops Renderer initialization. Failed partial
preparation releases its own actual CPU allocations. No scene/C2D program could
have become cached in that failed initialization interval. A prior successful
process owner is already ready and needs no further allocations.

After `C3D_FrameSync`, bind the prepared program before freeing Renderer programs
and before C2D/C3D finish. This operation allocates nothing and uses the actual
public `C3D_BindProgram`; no fake empty program, null bind, private context
layout write or arbitrary pointer reset substitutes for it. Texture/publication
retirement still precedes texture release independently of this shader owner.

The stable program and DVLB remain alive across every graphics fini/reinit until
final process exit. Storage is bounded CPU SDK metadata for one existing shader,
not a per-frame resource or GPU texture/linear allocation. Final CPU destruction
runs only when no subsequent graphics initialization is possible. Actual GPU
shutdown/synchronization remains the caller's responsibility. Host builds report
unavailable because no native graphics service exists there.

## Validation and scope

The isolated native probe uses the actual Renderer import/generation, original
font packets, matrix/scalar source controllers and real native glyph raster.
Its explicit CPU-global/static-object SDK admission and direct boot-camera SDK
view remain test-only, not normal battle entry or queued CameraMain proof. The
initial four unmapped SDK reads are preserved as failed-cleanliness evidence.
The targeted corrected probe checks that retirement invalidates the actual old
font lease, fresh Renderer import rejects all old packets, glyph pixels match
and raster replay consumes no new source reads/RNG. This helper changes native
resource lifetime only; it does not acknowledge a source display callback.

The corrected isolated run completed with terminal0, zero unmapped reads and
zero native exceptions across real retirement/fresh import. All three actual
400x240 target captures were byte-identical (3616 non-background ink pixels),
with230 glyph packets,460 RNG calls and18766 source byte reads unchanged during
replay. The visible fresh-import status confirms old packets were rejected.
This is an Azahar GPU/lifetime check, not physical-device performance evidence.
