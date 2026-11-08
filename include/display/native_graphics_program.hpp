#pragma once
namespace ttyd {
// Actual existing world shader, independently parsed and process-owned. Prepare
// after C3D_Init and BEFORE C2D_Init/any scene program can become cached.
// All fallible allocation is here; failure must stop renderer initialization.
bool prepareNativeGraphicsParkingProgram();
// No allocation/draw/RNG. Requires live C3D; false means never prepared. Park
// after GPU synchronization before freeing scene/C2D shaders. The shader/DVLB
// remain alive across every C3D_Fini/reinit until process exit.
// Host build deliberately reports unavailable; it has no graphics context.
bool bindNativeGraphicsParkingProgram();
}
