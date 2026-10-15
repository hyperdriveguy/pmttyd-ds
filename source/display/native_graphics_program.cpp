#include "native_graphics_program.hpp"
#ifdef __3DS__
#include <3ds.h>
#include <citro3d.h>
#include "world_shbin.h"
namespace ttyd {namespace {
struct ProcessGraphicsProgram {
 DVLB_s* shader=nullptr;
 shaderProgram_s program{};
 bool ready=false,initialized=false;
 ~ProcessGraphicsProgram(){clear();}
 void clear(){
  if(initialized)shaderProgramFree(&program);
  if(shader)DVLB_Free(shader);
  shader=nullptr;program={};ready=initialized=false;
 }
 bool prepare(){
  if(ready)return true;
  shader=DVLB_ParseFile(const_cast<u32*>(reinterpret_cast<const u32*>(world_shbin)),world_shbin_size);
  if(!shader||!shader->numDVLE){clear();return false;}
  const auto result=shaderProgramInit(&program);initialized=true;
  if(R_FAILED(result)||R_FAILED(shaderProgramSetVsh(&program,&shader->DVLE[0]))||!program.vertexShader){clear();return false;}
  ready=true;return true;
 }
};
// Bounded CPU SDK metadata, not GPU texture/linear storage. Its original shader
// bytes have static process lifetime. No per-renderer ownership or shutdown
// free can invalidate Citro3D's cached pointer to this exact stable object.
ProcessGraphicsProgram parking;
}
bool prepareNativeGraphicsParkingProgram(){return parking.prepare();}
bool bindNativeGraphicsParkingProgram(){
 if(!parking.ready)return false;
 C3D_BindProgram(&parking.program);return true;
}
}
#else
namespace ttyd {
bool prepareNativeGraphicsParkingProgram(){return false;}
bool bindNativeGraphicsParkingProgram(){return false;}
}
#endif
