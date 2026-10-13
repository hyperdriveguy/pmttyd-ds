#include "platform.hpp"
#include "harbor_tutorial.hpp"
#include <memory>
namespace ttyd {
bool Renderer::bindTutorialStartupResources(HarborTutorial& tutorial){
    if(tutorialStartupOwner && tutorialStartupOwner!=&tutorial)
        tutorialStartupOwner->setStartupResources(nullptr,nullptr,nullptr);
    tutorialStartupOwner=&tutorial;
    if(!tutorialStartupResources)tutorialStartupResources=std::make_unique<BattleStartupResources>();
    // Rebind after beginFresh resets the event owner. Even failed actual reads
    // install an explicit false readiness provider; no successful placeholder.
    tutorial.setStartupResources(tutorialStartupResources.get(),
        [](void* owner,uint32_t key,int32_t context){return static_cast<BattleStartupResources*>(owner)->paperReady(key,context);},
        [](void* owner,int32_t mode){return static_cast<BattleStartupResources*>(owner)->niceReady(mode);});
    if(!tutorialStartupResources->ready() && !tutorialStartupResources->load("romfs:"))return false;
    // Called before the render frame, while source battle-startup ownership is
    // active. The original banks/poses and actual GPU atlases are resident;
    // paper field captures and effect playback are separate providers.
    bool cached=true;
    for(unsigned i=0;i<9;++i)cached=tutorialStartupResources->resourceReady(tutorialStartupResources->resource(i)->sourceKey,1) && cached;
    if(cached)return true;
    C3D_FrameSync();
    bool ready=true;
    for(unsigned i=0;i<9;++i){
        const bool admitted=tutorialStartupResources->admitTexture(i,[&](unsigned index,const std::string& path){
            auto& slot=tutorialStartupTextures[index];
            if(slot.imported)return true;
            FILE* file=std::fopen(path.c_str(),"rb");if(!file)return false;
            slot.imported=Tex3DS_TextureImportStdio(file,&slot.texture,nullptr,false);std::fclose(file);
            if(!slot.imported)return false;
            C3D_TexSetFilter(&slot.texture,GPU_LINEAR,GPU_LINEAR);
            // Source wrap subdivision and its gutters remain in PMANIM4;
            // this is the converted atlas sampler, not a live capture texture.
            C3D_TexSetWrap(&slot.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
            return true;
        });
        ready=admitted && ready;
    }
    return ready;
}
void Renderer::releaseTutorialStartupResources(){
    if(tutorialStartupOwner){tutorialStartupOwner->setStartupResources(nullptr,nullptr,nullptr);tutorialStartupOwner=nullptr;}
    if(c3dReady)C3D_FrameSync();
    for(unsigned i=0;i<9;++i){
        if(tutorialStartupResources)tutorialStartupResources->releaseTexture(i);
        auto& slot=tutorialStartupTextures[i];
        if(slot.imported){Tex3DS_TextureFree(slot.imported);C3D_TexDelete(&slot.texture);}
        slot=Texture{};
    }
    tutorialStartupResources.reset();
}
}
