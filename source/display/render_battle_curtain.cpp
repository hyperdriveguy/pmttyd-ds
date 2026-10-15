#include "platform.hpp"
#include <algorithm>
#include <cstring>
namespace ttyd {
// PMCUR001 describes original A_2: 216 triangles/648 bindings, nonlooping
// frames 0..45, UV scale 8. These are pack identities, not native arena budgets.
bool Renderer::loadTutorialCurtain(){
 if(tutorialCurtainAttempted)return tutorialCurtainReady;
 tutorialCurtainAttempted=true;C3D_FrameSync();
 if(!tutorialCurtain.animations.load("romfs:/harbor-curtain.pmanim"))return false;
 auto* pose=tutorialCurtain.animations.pose("A_2");
 if(!pose || pose->start!=0 || pose->end!=45 || pose->loop || pose->bindings.size()!=648)return false;
 FILE* f=std::fopen("romfs:/harbor-curtain.bin","rb");if(!f)return false;
 char magic[8];float scale=0,start=0,end=0;uint32_t triangles=0;
 const bool valid=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMCUR001",8) && std::fread(&scale,4,1,f)==1 && std::fread(&triangles,4,1,f)==1 && std::fread(&start,4,1,f)==1 && std::fread(&end,4,1,f)==1 && std::fgetc(f)==EOF;
 std::fclose(f);if(!valid || scale!=8 || triangles!=216 || start!=0 || end!=45)return false;
 f=std::fopen("romfs:/harbor-curtain.t3x","rb");if(!f)return false;
 tutorialCurtain.image.imported=Tex3DS_TextureImportStdio(f,&tutorialCurtain.image.texture,nullptr,false);std::fclose(f);
 if(!tutorialCurtain.image.imported)return false;
 tutorialCurtain.uvScale=scale;
 C3D_TexSetFilter(&tutorialCurtain.image.texture,GPU_LINEAR,GPU_LINEAR);
 C3D_TexSetWrap(&tutorialCurtain.image.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
 tutorialCurtainReady=pose->evaluate(0,tutorialCurtain.sample);
 return tutorialCurtainReady;
}
// Retained startup-fade owner advances at most once for scene.now. Native
// milliseconds*.06 sample60Hz authored frames; 45 is the source clip endpoint.
// Context 2/OFF_d_maku_left_right is an actual admitted paper resource.
void Renderer::advanceTutorialCurtain(HarborTutorial& scene){
 tutorialFadeOwner=&scene;
 scene.setStartupFade(this,[](void* user,int32_t type,int32_t duration,uint32_t color){auto& r=*static_cast<Renderer*>(user);if(r.tutorialFade.enter(type,duration,color))r.tutorialFadeTick=UINT64_MAX;},[](void* user){return static_cast<Renderer*>(user)->tutorialFade.finished();});
 if(!tutorialFade.active || tutorialFadeTick==scene.now)return;
 tutorialFadeTick=scene.now;
 BattleCurtainPoseServices s;s.user=this;
 s.ready=[](void* user,const char* name,int32_t context){auto& r=*static_cast<Renderer*>(user);return !std::strcmp(name,"OFF_d_maku_left_right") && context==2 && r.loadTutorialCurtain();};
 s.entry=[](void* user,const char*,int32_t)->int32_t{auto& r=*static_cast<Renderer*>(user);r.tutorialCurtainLive=true;r.tutorialFadeMaterial=0;return 0;};
 s.release=[](void* user,int32_t){static_cast<Renderer*>(user)->tutorialCurtainLive=false;};
 s.flagsOn=[](void* user,int32_t,uint32_t flags){static_cast<Renderer*>(user)->tutorialFadeMaterial|=flags;};
 s.flagsOff=[](void* user,int32_t,uint32_t flags){static_cast<Renderer*>(user)->tutorialFadeMaterial&=~flags;};
 s.animation=[](void* user,int32_t,const char*,int32_t){auto& r=*static_cast<Renderer*>(user);r.tutorialFadeEpoch=r.tutorialFadeOwner->now;r.tutorialFadeLoops=0;r.tutorialCurtain.animations.pose("A_2")->evaluate(0,r.tutorialCurtain.sample);};
 s.color=[](void*,int32_t,uint32_t){}; // Source color is retained by tutorialFade.
 s.loopTimes=[](void* user,int32_t){return static_cast<Renderer*>(user)->tutorialFadeLoops;};
 s.main=[](void* user,int32_t){auto& r=*static_cast<Renderer*>(user);const float frame=float(r.tutorialFadeOwner->now-r.tutorialFadeEpoch)*.06f;const auto* pose=r.tutorialCurtain.animations.pose("A_2");if(pose && pose->evaluate(std::min(frame,45.f),r.tutorialCurtain.sample))r.tutorialFadeLoops=frame/45.f;};
 tutorialFade.tick(s);
}
void Renderer::drawTutorialCurtain(){
 if(!tutorialCurtainLive || !tutorialCurtainReady)return;
 const auto& sample=tutorialCurtain.sample;if(sample.vertices.size()!=648 || count>12000-648)return;
 // disp_maku source unit rectangle64x48 ->608x480. Orthographic native
 // image-plane projection adapts that viewport to the304x240 top content.
 C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-100,100,true);
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
 C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_GEQUAL,128);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
 // Source material flag 0x40 selects retained poseColor; otherwise white.
 // Convert source RGBA to native ABGR before modulating each vertex channel.
 const uint32_t rgba=(tutorialFadeMaterial&0x40)?tutorialFade.poseColor:0xffffffff;
 const uint32_t abgr=((rgba&255)<<24)|((rgba&0xff00)<<8)|((rgba&0xff0000)>>8)|(rgba>>24);
 const int first=count;
 for(size_t n=0;n<sample.vertices.size();++n){const auto& p=sample.vertices[n];ActorColor color=sample.colors.empty()?ActorColor{{1,1,1,1}}:sample.colors[n];for(unsigned channel=0;channel<4;++channel)color[channel]*=((abgr>>(channel*8))&255)/255.f;vertex(48+(p.x+32)*304/64,(24-p.y)*5,0,p.u,1-(1-p.v)*tutorialCurtain.uvScale,abgr,&color);}
 GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));
 auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
 C3D_TexBind(0,&tutorialCurtain.image.texture);C3D_DrawArrays(GPU_TRIANGLES,first,count-first);
}
// Detach the borrowed callback owner before clearing retained fade state.
// Caller must retire outstanding GPU users before deleting the texture.
void Renderer::releaseTutorialCurtain(){
 if(tutorialFadeOwner)tutorialFadeOwner->setStartupFade(nullptr,nullptr,nullptr);
 if(tutorialCurtain.image.imported){Tex3DS_TextureFree(tutorialCurtain.image.imported);C3D_TexDelete(&tutorialCurtain.image.texture);}
 tutorialCurtain=CineActor{};tutorialFade={};tutorialFadeOwner=nullptr;tutorialCurtainAttempted=tutorialCurtainReady=tutorialCurtainLive=false;tutorialFadeTick=UINT64_MAX;
}
}
