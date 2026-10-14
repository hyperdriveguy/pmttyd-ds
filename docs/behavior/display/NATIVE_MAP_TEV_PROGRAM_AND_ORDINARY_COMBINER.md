# Native map TEV continuation and ordinary combiner

US G8ME01 revision0. This implementation follows the recovered
[whole local request graph](MAP_TEV_LOCAL_REQUEST_GRAPH_AND_BACKEND_BOUNDARIES.md),
[fixed bases](MAP_TEV_FIXED_STAGE_PROGRAMS.md),
[ordinary base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) and
[retail SDK/source check](ORIGINAL_ORDINARY_TEV_SDK_AND_SHADOW_SOURCE_DOMAIN.md).
It does not establish original pixel or complete material-system equivalence.

The retained portable helper captures slots,mode,flags and model at entry,
initializes B=C=N/I0, and preserves each pending GX request without replaying
completed requests. Signed N<=0 selects the original no-texture base before
mode dispatch. Positive N uses original mode0's high-level GXSetTevOp call,
eight fixed mode1..7/12 literal programs, or the original generic base for
mode10/11/unsigned>12. Fixed counts remain independent of incoming N.

The mandatory K0 stage is appended unless F10000000. Its eight setters retain
original order and inputs. Character shadow and all six middle flag blocks
then occur in original order. Timed bases and projected/depth/pulse/callback
branches require real resumable providers; missing providers hold at their
original dependency boundary instead of omitting effects. Final callback uses
its own retained record: initializes I0 and reloads ONLY K/C/I, preserving B.
Final setters always publish low8(C),low8(K),low8(I), in that order.

Native trio binds actual ShadowWork preparationGate(+10c) and flags for the
character-shadow projection0/texture-null exits. A completed projection getter
is retained across an unavailable enabled-texture dependency. Enabled images
need the real embedded object/holder and projection-texture matrix+7c, which
are not supplied by the ordinary shadow texture or camera-projection matrix.
The fresh primary map group's actual null callback publishes I0; nonnull
callbacks hold pending their real context/body. Current same-owner blocked
queue does not implement arbitrary global-owner replacement or interleaving.

NativeShadowProgram accepts the source ordinary stage0 SDK request and all
eight exact identity/K0 stage1 setters separately. Stage-count2 publication
requires known K0 and the complete255 setter mask. Count1 disables the second
stage without erasing retained source K0 or its stage1 configuration. Indirect
count0 is supported; other values hold. Existing shadow/audience register and
independent color/alpha material-source ownership is preserved.

The actual Citro3D backend uses existing PICA0/1 to preserve independent raster
RGB/alpha sources for source stage0, then PICA2 modulates PREV by K0 for both
channels. K0 is copied into PICA2's own constant register: installed PICA sources
cannot reference reserved PICA5's constant directly. All remaining stages pass
through. Source K0 remains independently retained and every later accepted K0
write/restoration updates the active constant stage. No CPU multiplication
replaces the GPU operation and no primitive is acknowledged by setup alone.
The no-texture raster-alpha pass and generic multiply base have bounded native
state support too; arbitrary fixed/multitexture/GX equations remain unavailable.

Host tests compare ALL150 fixed-program requests against the independently
statically decoded original evidence manifest. They also check held arguments,
input capture, signed/low-byte cases, stage order, dependency waits, callback
selective publication and native setter/count/K0 retention. Synthetic inputs
and permissive providers are test fixtures, not original execution observations.
Native build/trace/smoke evidence is recorded in PROGRESS and EVT-01.

Remaining work includes real original indexed fan submission/UV atlas binding,
projected/timed/depth providers and holders, other GPU equations and enabled fog,
GX/PICA quantization/clamp/cache comparison, original frames, hardware timing,
battle-to-field return and the wider playable prologue. This is implementation
progress in an admitted source path, not a complete rendering-system claim.
