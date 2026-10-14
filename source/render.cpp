#include "battle_actor_rotation.hpp"
#include "platform.hpp"
#include "paper_fence_warp.hpp"
#include "message_icons.hpp"
#include "item_presentation.hpp"
#include "world_shbin.h"
#include "message_system_shbin.h"
#include "field_camera.hpp"
#include "map_visibility.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
constexpr int MaxVertices=12000;
constexpr auto FieldRGB=GPU_WRITEMASK(GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE);
constexpr auto FieldRGBDepth=GPU_WRITEMASK(FieldRGB|GPU_WRITE_DEPTH);
// Provisional legacy billboard/shadow framing for unbound presentation domains.
// Ordinary Mario uses original model coordinates and the source scalar2.
constexpr float LegacyPaperCharacterScale=.16f;
constexpr uint32_t Ink=0xff352d29, Paper=0xffdff2fb, Gold=0xff62c7f3, Red=0xff4966d8;
uint32_t rgb(unsigned r,unsigned g,unsigned b,unsigned a=255) { return C2D_Color32(r,g,b,a); }
uint32_t shade(uint32_t c,float factor) { return rgb((c&255)*factor,((c>>8)&255)*factor,((c>>16)&255)*factor); }
}
bool Renderer::init() {
    if(!loadItemPresentations("romfs:/item-presentations.bin"))return false;
    if(!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE)) return false;
    c3dReady=true;
    if(!C2D_Init(2048)) return false;
    c2dReady=true;
    top=C3D_RenderTargetCreate(240,400,GPU_RB_RGBA8,GPU_RB_DEPTH24_STENCIL8);
    bottom=C2D_CreateScreenTarget(GFX_BOTTOM,GFX_LEFT);
    if(!top || !bottom) return false;
    constexpr uint32_t flags=GX_TRANSFER_FLIP_VERT(0)|GX_TRANSFER_OUT_TILED(0)|GX_TRANSFER_RAW_COPY(0)|GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)|GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8)|GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);
    C3D_RenderTargetSetOutput(top,GFX_TOP,GFX_LEFT,flags);
    shader=DVLB_ParseFile((uint32_t*)world_shbin,world_shbin_size);
    if(!shader) return false;
    shaderProgramInit(&program);programReady=true;
    shaderProgramSetVsh(&program,&shader->DVLE[0]);
    transformLoc=shaderInstanceGetUniformLocation(program.vertexShader,"transform");
    messageSystemShader=DVLB_ParseFile((uint32_t*)message_system_shbin,message_system_shbin_size);
    if(!messageSystemShader)return false;
    shaderProgramInit(&messageSystemProgram);messageSystemProgramReady=true;
    shaderProgramSetVsh(&messageSystemProgram,&messageSystemShader->DVLE[0]);
    messageSystemTransform=shaderInstanceGetUniformLocation(messageSystemProgram.vertexShader,"transform");
    messageSystemVertices=(SystemVertex*)linearAlloc(sizeof(SystemVertex)*24);
    vertices=(Vertex*)linearAlloc(sizeof(Vertex)*MaxVertices);
    messageTailVertices=(Vertex*)linearAlloc(sizeof(Vertex)*12);
    messageIndicatorVertices=(Vertex*)linearAlloc(sizeof(Vertex)*256);
    if(!messageIndicatorVertices || !messageIndicatorAnimations.load("romfs:/message-indicator.pmanim"))return false;
    FILE* indicatorFile=std::fopen("romfs:/message-indicator.t3x","rb");if(!indicatorFile)return false;
    messageIndicatorTexture.imported=Tex3DS_TextureImportStdio(indicatorFile,&messageIndicatorTexture.texture,nullptr,false);std::fclose(indicatorFile);
    if(!messageIndicatorTexture.imported)return false;
    C3D_TexSetFilter(&messageIndicatorTexture.texture,GPU_LINEAR,GPU_LINEAR);
    C3D_TexSetWrap(&messageIndicatorTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    windowFrameVertices=(Vertex*)linearAlloc(sizeof(Vertex)*54*MaxWindowFrames);
    textBuffer=C2D_TextBufNew(8192);
    if(!vertices || !messageTailVertices || !windowFrameVertices || !messageSystemVertices || !textBuffer) return false;
    for(unsigned i=0;i<10;++i) {
        std::string name=i==0?"panel":i==1?"system-pattern":std::to_string(40+i);
        auto& texture=i==0?messagePanel:i==1?messageSystemPattern:messageSystemMasks[i-2];
        FILE* file=std::fopen(("romfs:/message-window-"+name+".t3x").c_str(),"rb");if(!file)return false;
        texture.imported=Tex3DS_TextureImportStdio(file,&texture.texture,nullptr,false);std::fclose(file);
        if(!texture.imported)return false;
        C3D_TexSetFilter(&texture.texture,GPU_LINEAR,GPU_LINEAR);
        C3D_TexSetWrap(&texture.texture,i==1?GPU_REPEAT:GPU_CLAMP_TO_EDGE,i==1?GPU_REPEAT:GPU_CLAMP_TO_EDGE);
    }
    FILE* f=std::fopen("romfs:/characters.t3x","rb");
    if(!f) return false;
    imported=Tex3DS_TextureImportStdio(f,&atlas,nullptr,false);std::fclose(f);
    if(!imported) return false;
    atlasReady=true;
    C3D_TexSetFilter(&atlas,GPU_LINEAR,GPU_LINEAR);
    C3D_TexSetWrap(&atlas,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    FILE* skyFile=std::fopen("romfs:/sky.t3x","rb");if(!skyFile)return false;
    sky.imported=Tex3DS_TextureImportStdio(skyFile,&sky.texture,nullptr,false);std::fclose(skyFile);
    if(!sky.imported)return false;
    C3D_TexSetFilter(&sky.texture,GPU_LINEAR,GPU_LINEAR);
    if(!actorAnimations.load("romfs:/characters.pmanim"))return false;
    if(!partnerAnimations.load("romfs:/partner.pmanim"))return false;
    if(!harborMarioAnimations.load("romfs:/harbor-mario.pmanim"))return false;
    FILE* harborUV=std::fopen("romfs:/harbor-mario.bin","rb");if(!harborUV)return false;
    char harborMagic[8];bool harborValid=std::fread(harborMagic,8,1,harborUV)==1 && !std::memcmp(harborMagic,"PMHUV001",8) && std::fread(&harborMarioUVScale,4,1,harborUV)==1 && std::fgetc(harborUV)==EOF;
    std::fclose(harborUV);if(!harborValid || !(harborMarioUVScale==1 || harborMarioUVScale==2 || harborMarioUVScale==4 || harborMarioUVScale==8))return false;
    FILE* harborMario=std::fopen("romfs:/harbor-mario.t3x","rb");if(!harborMario)return false;
    harborMarioTexture.imported=Tex3DS_TextureImportStdio(harborMario,&harborMarioTexture.texture,nullptr,false);std::fclose(harborMario);
    if(!harborMarioTexture.imported)return false;
    C3D_TexSetFilter(&harborMarioTexture.texture,GPU_LINEAR,GPU_LINEAR);
    if(!professorMarioAnimations.load("romfs:/professor-mario.pmanim"))return false;
    if(!professorFenceAnimations.load("romfs:/professor-fence.pmanim"))return false;
    FILE* storyUV=std::fopen("romfs:/professor-mario.bin","rb");if(!storyUV)return false;
    char storyMagic[8];
    const bool storyValid=std::fread(storyMagic,8,1,storyUV)==1 && !std::memcmp(storyMagic,"PMSUV1",8) &&
        std::fread(&professorMarioUVScale,4,1,storyUV)==1 && std::fgetc(storyUV)==EOF;
    std::fclose(storyUV);
    if(!storyValid || !(professorMarioUVScale==1 || professorMarioUVScale==2 || professorMarioUVScale==4 || professorMarioUVScale==8))return false;
    FILE* storyTexture=std::fopen("romfs:/professor-mario.t3x","rb");if(!storyTexture)return false;
    professorMarioTexture.imported=Tex3DS_TextureImportStdio(storyTexture,&professorMarioTexture.texture,nullptr,false);std::fclose(storyTexture);
    if(!professorMarioTexture.imported)return false;
    C3D_TexSetFilter(&professorMarioTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&professorMarioTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    FILE* mapMetrics=std::fopen("romfs:/professor-map.bin","rb");if(!mapMetrics)return false;
    char mapMagic[8];const bool mapValid=std::fread(mapMagic,8,1,mapMetrics)==1 && !std::memcmp(mapMagic,"PMSICON1",8) &&
        std::fread(professorMapSize.data(),8,1,mapMetrics)==1 && std::fgetc(mapMetrics)==EOF;
    std::fclose(mapMetrics);
    if(!mapValid || !(professorMapSize[0]>0 && professorMapSize[0]<=1024 && professorMapSize[1]>0 && professorMapSize[1]<=1024))return false;
    FILE* mapTexture=std::fopen("romfs:/professor-map.t3x","rb");if(!mapTexture)return false;
    professorMapTexture.imported=Tex3DS_TextureImportStdio(mapTexture,&professorMapTexture.texture,nullptr,false);std::fclose(mapTexture);
    if(!professorMapTexture.imported)return false;
    FILE* iconTexture=std::fopen("romfs:/message-icons.t3x","rb");if(!iconTexture)return false;
    messageIconTexture.imported=Tex3DS_TextureImportStdio(iconTexture,&messageIconTexture.texture,nullptr,false);std::fclose(iconTexture);
    if(!messageIconTexture.imported)return false;
    C3D_TexSetFilter(&messageIconTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&messageIconTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    C3D_TexSetFilter(&professorMapTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&professorMapTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    if(!undergroundAnimations.load("romfs:/underground-goombas.pmanim"))return false;
    FILE* undergroundMetrics=std::fopen("romfs:/underground-goombas.bin","rb");if(!undergroundMetrics)return false;
    const bool validUnderground=std::fread(&undergroundUVScale,4,1,undergroundMetrics)==1 && std::fgetc(undergroundMetrics)==EOF;
    std::fclose(undergroundMetrics);if(!validUnderground)return false;
    FILE* undergroundAtlas=std::fopen("romfs:/underground-goombas.t3x","rb");if(!undergroundAtlas)return false;
    undergroundTexture.imported=Tex3DS_TextureImportStdio(undergroundAtlas,&undergroundTexture.texture,nullptr,false);std::fclose(undergroundAtlas);
    if(!undergroundTexture.imported)return false;
    C3D_TexSetFilter(&undergroundTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&undergroundTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    if(!trioMarioAnimations.load("romfs:/trio-attack-mario.pmanim"))return false;
    FILE* trioUV=std::fopen("romfs:/trio-attack-mario.bin","rb");if(!trioUV)return false;
    char trioMagic[8];const bool validTrio=std::fread(trioMagic,8,1,trioUV)==1 && !std::memcmp(trioMagic,"PMSUV1",8) && std::fread(&trioMarioUVScale,4,1,trioUV)==1 && std::fgetc(trioUV)==EOF;
    std::fclose(trioUV);if(!validTrio)return false;
    FILE* trioAtlas=std::fopen("romfs:/trio-attack-mario.t3x","rb");if(!trioAtlas)return false;
    trioMarioTexture.imported=Tex3DS_TextureImportStdio(trioAtlas,&trioMarioTexture.texture,nullptr,false);std::fclose(trioAtlas);
    if(!trioMarioTexture.imported)return false;
    C3D_TexSetFilter(&trioMarioTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&trioMarioTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    if(!trioPartnerAnimations.load("romfs:/trio-attack-goombella.pmanim"))return false;
    FILE* trioPartnerUV=std::fopen("romfs:/trio-attack-goombella.bin","rb");if(!trioPartnerUV)return false;
    const bool validPartner=std::fread(trioMagic,8,1,trioPartnerUV)==1 && !std::memcmp(trioMagic,"PMSUV1",8) && std::fread(&trioPartnerUVScale,4,1,trioPartnerUV)==1 && std::fgetc(trioPartnerUV)==EOF;
    std::fclose(trioPartnerUV);if(!validPartner)return false;
    FILE* trioPartnerAtlas=std::fopen("romfs:/trio-attack-goombella.t3x","rb");if(!trioPartnerAtlas)return false;
    trioPartnerTexture.imported=Tex3DS_TextureImportStdio(trioPartnerAtlas,&trioPartnerTexture.texture,nullptr,false);std::fclose(trioPartnerAtlas);if(!trioPartnerTexture.imported)return false;
    C3D_TexSetFilter(&trioPartnerTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&trioPartnerTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    if(!loadMap("bti_01",trioBackground) || !loadMap("stg_00_1",trioStage))return false;
    if(!fieldActorAssets.load("romfs:/field-actors.pmnpc"))return false;
    if(!npcAngles.load("romfs:/npc-angle.bin"))return false;
    if(!shadowGeometry.load("romfs:/shadows.pmshadow"))return false;
    FILE* characterFile=std::fopen("romfs:/characters-mesh.t3x","rb");if(!characterFile)return false;
    characterTextures.imported=Tex3DS_TextureImportStdio(characterFile,&characterTextures.texture,nullptr,false);std::fclose(characterFile);
    if(!characterTextures.imported)return false;
    C3D_TexSetFilter(&characterTextures.texture,GPU_LINEAR,GPU_LINEAR);
    C3D_TexSetWrap(&characterTextures.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    FILE* partnerFile=std::fopen("romfs:/partner-mesh.t3x","rb");if(!partnerFile)return false;
    partnerTextures.imported=Tex3DS_TextureImportStdio(partnerFile,&partnerTextures.texture,nullptr,false);std::fclose(partnerFile);
    if(!partnerTextures.imported)return false;
    C3D_TexSetFilter(&partnerTextures.texture,GPU_LINEAR,GPU_LINEAR);
    C3D_TexSetWrap(&partnerTextures.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    for(auto entry:{std::pair<const char*,Texture*>{"romfs:/font.t3x",&fontTexture},{"romfs:/harbor-actors.t3x",&harborTexture},{"romfs:/message-window.t3x",&messageWindowTexture}}) {
        FILE* file=std::fopen(entry.first,"rb");if(!file)return false;
        entry.second->imported=Tex3DS_TextureImportStdio(file,&entry.second->texture,nullptr,false);std::fclose(file);
        if(!entry.second->imported)return false;
        C3D_TexSetFilter(&entry.second->texture,GPU_LINEAR,GPU_LINEAR);
        C3D_TexSetWrap(&entry.second->texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    }
    FILE* metrics=std::fopen("romfs:/harbor-actors.bin","rb");if(!metrics)return false;
    char magic[8];uint32_t n=0;
    const bool valid=std::fread(magic,8,1,metrics)==1 && !std::memcmp(magic,"PMHNPC2",8) && std::fread(&n,4,1,metrics)==1 && n==2 && std::fread(harborActorMetrics.data(),sizeof(harborActorMetrics),1,metrics)==1 && std::fread(harborTalkOffsets.data(),sizeof(harborTalkOffsets),1,metrics)==1 && std::fgetc(metrics)==EOF;
    std::fclose(metrics);if(!valid)return false;
    for(const auto& m:harborActorMetrics)for(float v:m)if(!std::isfinite(v) || std::fabs(v)>10)return false;
    for(const auto& m:harborTalkOffsets)for(float v:m)if(!std::isfinite(v) || std::fabs(v)>10)return false;
    return loadMap("map",plaza) && loadFieldActors(0,plaza) && loadMap("stage",stage);
}
bool Renderer::loadMap(const char* name,Room& room) {
    char path[96];std::snprintf(path,sizeof(path),"romfs:/%s.batches",name);
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    uint32_t batchCount=0,textureCount=0;
    bool ok=std::fread(&batchCount,4,1,f)==1 && std::fread(&textureCount,4,1,f)==1;
    if(!ok || batchCount>512 || textureCount>256) {std::fclose(f);return false;}
    room.batches.resize(batchCount);room.textures.resize(textureCount);
    ok=std::fread(room.batches.data(),sizeof(MapBatch),batchCount,f)==batchCount;
    std::fclose(f);if(!ok)return false;
    std::snprintf(path,sizeof(path),"romfs:/%s.vertices",name);
    f=std::fopen(path,"rb");if(!f)return false;
    std::fseek(f,0,SEEK_END);long bytes=std::ftell(f);std::rewind(f);
    if(bytes<=0 || bytes>16*1024*1024 || bytes%sizeof(Vertex)) {std::fclose(f);return false;}
    room.vertexCount=bytes/sizeof(Vertex);room.vertices=(Vertex*)linearAlloc(bytes);
    ok=room.vertices && std::fread(room.vertices,1,bytes,f)==size_t(bytes);std::fclose(f);if(!ok)return false;
    for(auto b:room.batches) if(b.start>room.vertexCount || b.count>room.vertexCount-b.start || b.texture<-1 || b.texture>=int32_t(textureCount))return false;
    for(auto batch:room.batches)if(batch.layer>>26==8)
        for(uint32_t i=0;i<batch.count;++i)room.fenceOriginal.push_back({batch.start+i,room.vertices[batch.start+i]});
    uint32_t groups=0;
    for(auto batch:room.batches) {if(batch.count%3 || ((batch.layer>>8)&65535)>1024 || (batch.layer>>26)>12)return false;groups=std::max(groups,(batch.layer>>8)&65535);}
    std::snprintf(path,sizeof(path),"romfs:/%s.uvanim",name);
    if(!loadUVAnimation(path,room.vertexCount,room.uvTracks))return false;
    std::snprintf(path,sizeof(path),"romfs:/%s.meshanim",name);
    if(!loadMeshAnimation(path,room.vertexCount,room.meshTracks))return false;
    room.occluders.resize(groups+1);
    for(auto& occluder:room.occluders) {
        float hi=std::numeric_limits<float>::max();occluder.bounds={{hi,hi,hi},{-hi,-hi,-hi}};
    }
    for(uint32_t n=0;n<batchCount;++n) {
        auto batch=room.batches[n];uint32_t group=(batch.layer>>8)&65535;if(!group)continue;
        auto& occluder=room.occluders[group];occluder.batches.push_back(n);
        for(uint32_t k=batch.start;k<batch.start+batch.count;++k) {
            auto v=room.vertices[k];auto& b=occluder.bounds;
            b.min.x=std::min(b.min.x,v.x);b.max.x=std::max(b.max.x,v.x);
            b.min.y=std::min(b.min.y,v.y);b.max.y=std::max(b.max.y,v.y);
            b.min.z=std::min(b.min.z,v.z);b.max.z=std::max(b.max.z,v.z);
        }
    }
    for(uint32_t n=0;n<textureCount;++n) {
        std::snprintf(path,sizeof(path),"romfs:/%s-%lu.t3x",name,(unsigned long)n);
        auto& slot=room.textures[n];
        f=std::fopen(path,"rb");if(!f)return false;
        slot.imported=Tex3DS_TextureImportStdio(f,&slot.texture,nullptr,false);std::fclose(f);
        if(!slot.imported)return false;
        C3D_TexSetFilter(&slot.texture,GPU_LINEAR,GPU_LINEAR);
    }
    GSPGPU_FlushDataCache(room.vertices,size_t(room.vertexCount)*sizeof(Vertex));
    return true;
}
void Renderer::freeRoom(Room& room) {
    for(auto& texture:room.textures) if(texture.imported) {Tex3DS_TextureFree(texture.imported);C3D_TexDelete(&texture.texture);}
    for(auto& model:room.actorModels)if(model.texture.imported){Tex3DS_TextureFree(model.texture.imported);C3D_TexDelete(&model.texture.texture);}
    if(room.vertices)linearFree(room.vertices);
    room=Room{};
}
bool Renderer::changeRoom(uint32_t room) {
    releaseFrontend();
    if(frontActive) {
        Room pendingStage,pendingBackground,pendingTrio;
        if(!loadMap("stage",pendingStage) || !loadMap("bti_01",pendingBackground) || !loadMap("stg_00_1",pendingTrio)) {
            C3D_FrameSync();freeRoom(pendingStage);freeRoom(pendingBackground);freeRoom(pendingTrio);return false;
        }
        stage=std::move(pendingStage);trioBackground=std::move(pendingBackground);trioStage=std::move(pendingTrio);
        frontActive=false;
    }
    const char* name=roomAsset(room);if(!name)return false;
    Room next;
    if(!loadMap(name,next) || !loadFieldActors(room,next)) {C3D_FrameSync();freeRoom(next);return false;}
    C3D_FrameSync();freeRoom(plaza);plaza=std::move(next);return true;
}
bool Renderer::loadFieldActors(uint32_t roomId,Room& room) {
    for(uint32_t index=0;index<fieldActorAssets.models().size();++index) {
        bool used=roomId==5 && fieldActorAssets.models()[index].asset=="professor";
        for(const auto& actor:fieldActorAssets.actors())if(actor.room==roomId && actor.model==index){used=true;break;}
        if(!used)continue;
        room.actorModels.emplace_back();auto& gpu=room.actorModels.back();gpu.model=index;
        const auto& model=fieldActorAssets.models()[index];
        const auto base=std::string("romfs:/")+model.asset;
        if(!gpu.animations.load((base+".pmanim").c_str()))return false;
        for(const auto& actor:fieldActorAssets.actors())if(actor.room==roomId && actor.model==index &&
            !(actor.kind==0?selectNpcAnimation(gpu.animations,actor.pose.c_str()):gpu.animations.pose(actor.pose.c_str())))return false;
        FILE* f=std::fopen((base+".t3x").c_str(),"rb");if(!f)return false;
        gpu.texture.imported=Tex3DS_TextureImportStdio(f,&gpu.texture.texture,nullptr,false);std::fclose(f);
        if(!gpu.texture.imported)return false;
        C3D_TexSetFilter(&gpu.texture.texture,GPU_LINEAR,GPU_LINEAR);
        C3D_TexSetWrap(&gpu.texture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    }
    return true;
}
void Renderer::fieldActors(const Game& g) {
    if(plaza.actorStart==UINT64_MAX || g.ticks<plaza.actorStart)plaza.actorStart=g.ticks;
    for(auto& gpu:plaza.actorModels) {
        gpu.start=count;
        gpu.batches.clear();
        const auto& model=fieldActorAssets.models()[gpu.model];
        for(const auto& sourceActor:fieldActorAssets.actors()) {
            auto actor=sourceActor;
            if(actor.model!=gpu.model)continue;
            const bool scripted=model.asset=="professor" && (g.progress.room==2 || g.progress.room==5) && g.storyPresentation().triggered;
            if(scripted) {
                if(actor.storyLow!=8 || g.storyPresentation().actors[2].hidden)continue;
                actor.position=g.storyPresentation().actors[2].position;actor.yaw=g.storyPresentation().actors[2].yaw;
                actor.pose=g.storyPresentation().message.talking && g.storyPresentation().speaker==2?"T_1":g.trioBattleActive?g.storyPresentation().actors[2].pose:g.storyPresentation().actors[2].moving?"W_1":"S_1";
            } else if(!actor.visible(g.progress.room,g.progress.originalStory,g.fieldDoor.interiorMask,g.fieldSystemFlags))continue;
            const auto* live=actor.kind==0 && !scripted?g.fieldNpcState(actor.key):nullptr;
            if(live && live->initialized){if(!live->presentationReady)continue;actor.position={live->body.root.x,live->body.root.y,live->body.root.z};actor.yaw=live->movement.yaw;actor.pose=live->pose;}
            uint32_t npcFlags=live && live->initialized?live->body.flags:actor.npcFlags;
            if(scripted)npcFlags=(npcFlags&~g.storyPresentation().actors[2].flagMask)|
                (g.storyPresentation().actors[2].flags&g.storyPresentation().actors[2].flagMask);
            if(actor.kind==0 && npcFlags && (npcFlags&3)!=3)continue;
            float frame=float(g.ticks-plaza.actorStart);
            if(actor.kind==0 && g.fieldActorTalk.message.active() && g.fieldActorTalk.actor.key==actor.key)
                actor.yaw=g.fieldActorTalk.actor.yaw;
            if(g.saveObjects)if(const auto* object=g.saveObjects->find(actor.key)){actor.pose=object->pose;frame=object->frame;}
            const NpcPresentation* npc=nullptr;
            if(actor.kind==0) {
                const auto* selected=selectNpcAnimation(gpu.animations,actor.pose.c_str());
                if(!selected){++actorMetrics.sampleFailures;continue;}
                actor.pose=selected->name;
                std::string talkPose;
                if(!actor.talkPose.empty())talkPose=selectNpcAnimation(gpu.animations,actor.talkPose.c_str())->name;
                if(live && live->initialized) {
                    npc=&live->presentation;frame=60.f*float(g.ticks*1000/60-live->poseAt)/1000.f;
                } else {
                    auto& state=plaza.npcs[actor.key];auto* mutableNpc=&state.presentation;npc=mutableNpc;
                    const uint64_t now=g.ticks*1000/60;
                    if(now<state.clock.poseAt)state={};
                    if(npcFlags&0x8000)mutableNpc->flags|=0x100;else mutableNpc->flags&=~0x100u;
                    frame=state.clock.update(actor.pose,talkPose,
                        g.fieldActorTalk.actor.key==actor.key?&g.fieldActorTalk.message:nullptr,now);
                    actor.pose=state.clock.pose;
                    NpcMatrixInput input;input.root=actor.position;input.yaw=actor.yaw;input.drawScale=actor.scale;
                    input.flags=npcFlags;
                    NpcCameraService camera;camera.user=const_cast<FieldCameraFrame*>(&g.fieldCameraFrame);
                    camera.read=[](void* p) {
                        const auto& frame=*static_cast<FieldCameraFrame*>(p);
                        return NpcCamera{{frame.eye.x*100,frame.eye.y*100,frame.eye.z*100},{frame.at.x*100,frame.at.y*100,frame.at.z*100}};
                    };
                    if(!mutableNpc->prepare(input,npcAngles,camera)){++actorMetrics.sampleFailures;continue;}
                }
            }
            ++actorMetrics.visible;
            if(gpu.tick!=g.ticks || gpu.pose!=actor.pose || gpu.frame!=frame) {
                const auto* pose=gpu.animations.pose(actor.pose.c_str());
                if(!pose || !pose->evaluate(frame,gpu.sample)){++actorMetrics.sampleFailures;continue;}
                gpu.tick=g.ticks;gpu.pose=actor.pose;gpu.frame=frame;
            }
            if(gpu.sample.vertices.size()>size_t(MaxVertices-count)){++actorMetrics.overflow;continue;}
            const ActorRootTransform root({actor.position[0]*.01f,actor.position[1]*.01f,actor.position[2]*.01f},actor.yaw,false,actor.scale*.01f);
            const uint32_t first=uint32_t(count);
            for(size_t i=0;i<gpu.sample.vertices.size();++i) {
                const auto& v=gpu.sample.vertices[i];const auto p=npc?npc->point(v):root.point(v);
                vertex(p[0],p[1],p[2],v.u,1-(1-v.v)*model.uvScale,0xffffffff,gpu.sample.colors.empty()?nullptr:&gpu.sample.colors[i]);
            }
            if(gpu.sample.batches.empty()) {
                const uint8_t pass=(model.drawMask&1u) || (model.drawMask&4u)?2:(model.drawMask&2u)?1:3;
                gpu.batches.push_back({first,uint32_t(count)-first,0,pass});
            }
            else for(auto batch:gpu.sample.batches){batch.start+=first;gpu.batches.push_back(batch);}
            actorMetrics.vertices+=uint32_t(gpu.sample.vertices.size());
        }
        gpu.count=count-gpu.start;
    }
}
void Renderer::drawFieldActors(bool translucent) {
    for(auto& model:plaza.actorModels)for(const auto& batch:model.batches) {
        if((batch.pass==3)!=translucent || !batch.count || batch.cull==2)continue;
        C3D_TexBind(0,&model.texture.texture);
        const GPU_CULLMODE culls[]={GPU_CULL_FRONT_CCW,GPU_CULL_BACK_CCW,GPU_CULL_NONE,GPU_CULL_NONE};
        C3D_CullFace(culls[batch.cull]);
        C3D_DepthTest(true,batch.pass?GPU_GEQUAL:GPU_GREATER,translucent?FieldRGB:FieldRGBDepth);
        if(batch.pass==2) {
            C3D_AlphaTest(true,GPU_GEQUAL,128);
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,GPU_ZERO,GPU_ONE,GPU_ZERO);
        } else {
            C3D_AlphaTest(!translucent,GPU_GREATER,8);
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
        }
        C3D_DrawArrays(GPU_TRIANGLES,batch.start,batch.count);
    }
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,FieldRGBDepth);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
}
void Renderer::updateCutaways(Room& room,const Game& g) {
    Point3 eye{g.fieldCameraFrame.eye.x,g.fieldCameraFrame.eye.y,g.fieldCameraFrame.eye.z};
    const float actorY=g.water.active()?g.water.position[1]:g.groundHeight+g.jumpHeight;
    const Point3 targets[]={
        {g.progress.position.x,actorY+.12f,g.progress.position.z},
        {g.progress.position.x,actorY+.65f,g.progress.position.z},
        {g.partner.x,g.partnerHeight+.3f,g.partner.z}
    };
    for(auto& occluder:room.occluders) {
        bool blocked=false;
        for(auto target:targets) {
            if(blocked || !segmentHitsBounds(eye,target,occluder.bounds))continue;
            for(auto n:occluder.batches) {
                auto batch=room.batches[n];
                for(uint32_t k=batch.start;k<batch.start+batch.count;k+=3) {
                    auto a=room.vertices[k],b=room.vertices[k+1],c=room.vertices[k+2];
                    if(segmentHitsTriangle(eye,target,{a.x,a.y,a.z},{b.x,b.y,b.z},{c.x,c.y,c.z})) {blocked=true;break;}
                }
                if(blocked)break;
            }
        }
        if(blocked)occluder.hiddenUntil=g.ticks+8;
        occluder.hidden=blocked || g.ticks<occluder.hiddenUntil;
    }
}
void Renderer::drawMap(bool battle,const Game& game,bool translucent,Room* supplied,uint32_t sceneVisibleGroups) {
    auto& room=supplied?*supplied:battle?stage:plaza;
    if(!supplied && !battle && !translucent && game.progress.room==2 && game.professorStory.captureRequested && !room.fenceOriginal.empty()) {
        const auto& scene=game.professorStory;
        if(!room.fenceCaptured) {
            room.fenceCaptureView=game.fieldCameraFrame.view;
            float xmin=INFINITY,ymin=INFINITY,xmax=-INFINITY,ymax=-INFINITY;
            int32_t left=INT32_MAX,top=INT32_MAX,right=INT32_MIN,bottom=INT32_MIN;
            for(const auto& saved:room.fenceOriginal) {
                const auto& p=saved.second;const float v[]={p.x,p.y,p.z};
                float xy[2];for(unsigned axis=0;axis<2;++axis) {
                    const auto& row=room.fenceCaptureView[axis];xy[axis]=row[0]*v[0]+row[1]*v[1]+row[2]*v[2]+row[3];
                }
                xmin=std::min(xmin,xy[0]);xmax=std::max(xmax,xy[0]);ymin=std::min(ymin,xy[1]);ymax=std::max(ymax,xy[1]);
                const auto screen=projectCameraRoot({p.x,p.y,p.z},game.fieldCameraFrame.view,game.fieldCameraFrame.projection,game.fieldCameraFrame.viewport);
                if(screen.valid){left=std::min(left,screen.x);right=std::max(right,screen.x);top=std::min(top,screen.y);bottom=std::max(bottom,screen.y);}
            }
            if(xmax>xmin && ymax>ymin && right>left && bottom>top) {
                room.fenceCapturePlane={xmin,ymin,xmax-xmin,ymax-ymin};room.fenceCaptured=true;
                scene.captureBounds={left,top,right-left,bottom-top};scene.captureReady=true;
            }
        }
        if(room.fenceCaptured && scene.fenceVisible && room.fenceTick!=game.ticks) {
            room.fenceTick=game.ticks;
            const auto* pose=professorFenceAnimations.pose("A_1");
            const float time=scene.fencePose=="A_1"?float(scene.now-scene.fencePoseAt)*.06f:0;
            if(pose && pose->evaluate(time,professorFenceSample)) {
                std::array<ActorVector,4> corners{};
                for(size_t i=0;i<pose->bindings.size();++i)if(pose->bindings[i].position<4) {
                    const auto& v=professorFenceSample.vertices[i];corners[pose->bindings[i].position]={v.x,v.y,v.z};
                }
                for(const auto& saved:room.fenceOriginal) {
                    auto& vertex=room.vertices[saved.first];const auto& original=saved.second;
                    const auto p=paperFenceWarp({original.x,original.y,original.z},room.fenceCaptureView,room.fenceCapturePlane,corners);
                    vertex.x=p[0];vertex.y=p[1];vertex.z=p[2];
                }
                for(auto batch:room.batches)if(batch.layer>>26==8)GSPGPU_FlushDataCache(room.vertices+batch.start,batch.count*sizeof(Vertex));
            }
        }
    }
    if(!supplied && !battle && !translucent)updateCutaways(room,game);
    if(!translucent) {
        if(room.animationStart==UINT64_MAX)room.animationStart=game.ticks;
        for(const auto& track:room.meshTracks) {
            const auto& matrix=track.frames[(game.ticks-room.animationStart)%track.frames.size()];
            for(auto binding:track.bindings) {
                auto p=transformedPosition(matrix,binding);
                auto& v=room.vertices[binding.vertex];v.x=p[0];v.y=p[1];v.z=p[2];
            }
            for(size_t n=0;n<track.bindings.size();) {
                uint32_t first=track.bindings[n].vertex,last=first;++n;
                while(n<track.bindings.size() && track.bindings[n].vertex==last+1)last=track.bindings[n++].vertex;
                GSPGPU_FlushDataCache(room.vertices+first,(last-first+1)*sizeof(Vertex));
            }
        }
        for(const auto& track:room.uvTracks) {
            const auto& matrix=track.frames[(game.ticks-room.animationStart)%track.frames.size()];
            for(auto binding:track.bindings) {
                auto uv=transformedUV(matrix,binding);
                auto& v=room.vertices[binding.vertex];v.u=uv[0];v.v=uv[1];
            }
            // Bindings are contiguous per original material batch. Flush each run,
            // keeping cache maintenance proportional to animated geometry.
            for(size_t n=0;n<track.bindings.size();) {
                uint32_t first=track.bindings[n].vertex,last=first;++n;
                while(n<track.bindings.size() && track.bindings[n].vertex==last+1)last=track.bindings[n++].vertex;
                GSPGPU_FlushDataCache(room.vertices+first,(last-first+1)*sizeof(Vertex));
            }
        }
    }
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,room.vertices,sizeof(Vertex),3,0x210);
    const GPU_TEXTURE_WRAP_PARAM wraps[]={GPU_CLAMP_TO_EDGE,GPU_REPEAT,GPU_MIRRORED_REPEAT};
    for(auto batch:room.batches) {
        if(!battle && game.progress.room==2 && batch.layer>>26==8 && game.professorStory.fenceHidden && !game.professorStory.fenceVisible)continue;
        if((!battle || supplied) && ((batch.layer&255)==2)!=translucent)continue;
        if(!battle) {
            const auto eventGroup=batch.layer>>26;
            if(sceneVisibleGroups!=UINT32_MAX) {if(eventGroup>=32 || !(sceneVisibleGroups&(1u<<eventGroup)))continue;}
            else if(!mapEventGroupVisible(eventGroup,game.progress.originalStory.value_or(0),game.fieldDoor.interiorMask))continue;
        }
        uint32_t group=(batch.layer>>8)&65535;
        if(!battle && group && room.occluders[group].hidden)continue;
        C3D_Tex* texture=batch.texture<0?&atlas:&room.textures[batch.texture].texture;
        C3D_TexSetWrap(texture,wraps[std::min<uint32_t>(batch.wrapS,2)],wraps[std::min<uint32_t>(batch.wrapT,2)]);
        C3D_TexBind(0,texture);
        // GX winding maps to the opposite PICA enum in this view transform.
        const GPU_CULLMODE culls[]={GPU_CULL_BACK_CCW,GPU_CULL_FRONT_CCW,GPU_CULL_NONE,GPU_CULL_NONE};
        C3D_CullFace(culls[(batch.layer>>24)&3]);
        C3D_DepthTest(true,GPU_GREATER,(batch.layer&255)==2?(battle?GPU_WRITE_COLOR:FieldRGB):(battle?GPU_WRITE_ALL:FieldRGBDepth));
        C3D_DrawArrays(GPU_TRIANGLES,batch.start,batch.count);
    }
    C3D_CullFace(GPU_CULL_NONE);
    C3D_DepthTest(true,GPU_GREATER,battle?GPU_WRITE_ALL:FieldRGBDepth);
}
void Renderer::vertex(float x,float y,float z,float u,float v,uint32_t color,const ActorColor* rgba) {
    if(count>=MaxVertices) return;
    vertices[count++]={x,y,z,u,v,(color&255)/255.f,((color>>8)&255)/255.f,((color>>16)&255)/255.f,((color>>24)&255)/255.f};
    if(rgba){auto& vertex=vertices[count-1];vertex.r=(*rgba)[0];vertex.g=(*rgba)[1];vertex.b=(*rgba)[2];vertex.a=(*rgba)[3];}
}
void Renderer::quad(Vec2 a,Vec2 b,float y0,float y1,uint32_t c) {
    float u=496.f/512,v=1-240.f/256;
    vertex(a.x,y0,a.z,u,v,c);vertex(b.x,y0,b.z,u,v,c);vertex(b.x,y1,b.z,u,v,c);
    vertex(b.x,y1,b.z,u,v,c);vertex(a.x,y1,a.z,u,v,c);vertex(a.x,y0,a.z,u,v,c);
}
void Renderer::ground(float x0,float z0,float x1,float z1,float y,uint32_t c) {
    float u=496.f/512,v=1-240.f/256;
    vertex(x0,y,z0,u,v,c);vertex(x1,y,z0,u,v,c);vertex(x1,y,z1,u,v,c);
    vertex(x1,y,z1,u,v,c);vertex(x0,y,z1,u,v,c);vertex(x0,y,z0,u,v,c);
}
void Renderer::box(float x,float z,float w,float d,float h,uint32_t c) {
    ground(x,z,x+w,z+d,h,shade(c,1.08f));
    quad({x,z+d},{x+w,z+d},0,h,c);
    quad({x,z},{x,z+d},0,h,shade(c,.70f));
    quad({x+w,z+d},{x+w,z},0,h,shade(c,.82f));
    quad({x+w,z},{x,z},0,h,shade(c,.74f));
}
void Renderer::character(int index,Vec2 p,float h,float w,float face,float lift,float tilt) {
    h*=LegacyPaperCharacterScale;w*=LegacyPaperCharacterScale;tilt*=LegacyPaperCharacterScale;
    const int cell=(index==0 && marioPose>0)?marioPose:index;
    float left=cell*.25f,right=left+.25f;
    if(face>0) std::swap(left,right);
    const float topV=(index==0 && marioPose>0)?.5f:1.f,bottomV=topV-.5f;
    float x0=p.x-w*.5f,x1=p.x+w*.5f,y0=lift,y1=lift+h;
    const uint32_t white=0xffffffff;
    vertex(x0,y0,p.z,left,bottomV,white);vertex(x1,y0,p.z,right,bottomV,white);vertex(x1+tilt,y1,p.z,right,topV,white);
    vertex(x1+tilt,y1,p.z,right,topV,white);vertex(x0+tilt,y1,p.z,left,topV,white);vertex(x0,y0,p.z,left,bottomV,white);
}
bool Renderer::characterModel(const char* name,float frame,Vec2 root,float rootY,float yaw,bool reflectZ,bool companion,const ActorAnimationPack* custom,float uvScale,const ProfessorSceneActor* battleActor) {
    const auto* pose=(custom?*custom:companion?partnerAnimations:actorAnimations).pose(name);
    if(!pose || !pose->evaluate(frame,actorSample) || actorSample.vertices.size()>size_t(MaxVertices-count))return false;
    const ActorRootTransform rootTransform(battleActor?ActorVector{}:ActorVector{root.x,rootY,root.z},yaw,reflectZ);
    const BattleActorRotation rotation(battleActor?battleActor->battleRotation:ActorVector{},battleActor?battleActor->battlePivot:ActorVector{});
    for(const auto& v:actorSample.vertices) {
        auto point=rootTransform.point(v);
        if(battleActor){for(unsigned axis=0;axis<3;++axis)point[axis]*=battleActor->battleScale[axis];const auto p=rotation.point(point);point={root.x+p[0]+battleActor->battleDisplayOffset[0]*.01f,rootY+p[1]+battleActor->battleDisplayOffset[1]*.01f,root.z+p[2]-.05f+battleActor->battleDisplayOffset[2]*.01f};}
        vertex(point[0],point[1],point[2],v.u,1-(1-v.v)*uvScale,0xffffffff);
    }
    return true;
}
void Renderer::shadow(Vec2 p,float r,float floor) {
    r*=LegacyPaperCharacterScale;
    float x0=p.x-r,x1=p.x+r,z0=p.z-r*.45f,z1=p.z+r*.45f;

    uint32_t white=0xffffffff;
    vertex(x0,floor+.012f,z0,0,.5f,white);vertex(x1,floor+.012f,z0,.25f,.5f,white);vertex(x1,floor+.012f,z1,.25f,.25f,white);
    vertex(x1,floor+.012f,z1,.25f,.25f,white);vertex(x0,floor+.012f,z1,0,.25f,white);vertex(x0,floor+.012f,z0,0,.5f,white);
}
void Renderer::harborActors(const Game& g) {
    for(unsigned i=1;i<4;++i) {
        const auto& actor=g.harborStory.actors[i];const unsigned cell=i==1?0:1;
        const auto& m=harborActorMetrics[cell];const float x=actor.x*.01f+m[0],y=actor.y*.01f+m[1],z=actor.z*.01f;
        const float left=cell*.5f,right=left+.5f;
        vertex(x,y,z,left,0,0xffffffff);vertex(x+m[2],y,z,right,0,0xffffffff);vertex(x+m[2],y+m[2],z,right,1,0xffffffff);
        vertex(x+m[2],y+m[2],z,right,1,0xffffffff);vertex(x,y+m[2],z,left,1,0xffffffff);vertex(x,y,z,left,0,0xffffffff);
    }
}
void Renderer::originalMessage(const Game& g,const MessagePlayback* supplied,const CameraPoint* suppliedAnchor,const CameraPoint* suppliedMario) {
    const bool fieldTalk=g.fieldActorTalk.message.active();
    const bool professor=g.storyPresentation().locked();
    const bool tutorial=g.harborTutorial.running();
    const bool encounter=!tutorial && g.harborEncounter.locked();
    const auto& message=supplied?*supplied:tutorial?g.harborTutorial.message:encounter?g.harborEncounter.message:professor?g.storyPresentation().message:fieldTalk?g.fieldActorTalk.message:g.harborStory.message;if(!message.active())return;
    const auto& font=harborMessageFont();
    // Unproject tilted NDC into the same pixel space as the 2D backend.
    // Reuse the actual world matrix: a parallel eye/FOV calculation can drift
    // from the handheld projection's rotation/aspect conventions.
    C3D_Mtx screenProjection;Mtx_OrthoTilt(&screenProjection,0,400,240,0,-1,1,true);
    const auto origin=Mtx_MultiplyFVecH(&screenProjection,FVec3_New(0,0,0));
    const auto screenX=Mtx_MultiplyFVecH(&screenProjection,FVec3_New(1,0,0));
    const auto screenY=Mtx_MultiplyFVecH(&screenProjection,FVec3_New(0,1,0));
    const float ax=screenX.x-origin.x,ay=screenX.y-origin.y;
    const float bx=screenY.x-origin.x,by=screenY.y-origin.y,det=ax*by-ay*bx;
    auto project=[&](CameraPoint p,float& x,float& y) {
        const auto clip=Mtx_MultiplyFVecH(&worldTransform,FVec3_New(p.x,p.y,p.z));
        if(clip.w<=0 || det==0)return false;
        const float nx=clip.x/clip.w-origin.x,ny=clip.y/clip.w-origin.y;
        const float pixelX=(nx*by-ny*bx)/det,pixelY=(ax*ny-ay*nx)/det;
        x=(pixelX-200)/MessageWindowScale;y=(120-pixelY)/MessageWindowScale;
        return std::isfinite(x) && std::isfinite(y);
    };
    float anchorX=0,anchorY=0,marioX=0,marioY=0;unsigned mode=0;
    const bool newWindow=(!supplied || suppliedAnchor) && messageWindowSerial!=message.serial;
    if(suppliedAnchor) {
        messageTalkAnchor=*suppliedAnchor;
        if(project(messageTalkAnchor,anchorX,anchorY) && suppliedMario && project(*suppliedMario,marioX,marioY))
            mode=message.windowType==1?7:messageSpeakerMode(anchorX,marioX);
    }
    if(newWindow && !supplied && tutorial && g.harborTutorial.speaker>=0 && g.harborTutorial.speaker<3) {
        const auto id=unsigned(g.harborTutorial.speaker);const auto& a=g.harborTutorial.actors[id];
        messageTalkAnchor={a.position[0]*.01f,a.position[1]*.01f,a.position[2]*.01f};
        const auto* pose=tutorialActors[id].animations.pose(a.pose.c_str());
        if(pose && pose->evaluate(float(g.harborTutorial.now-a.poseAt)*.06f,actorSample))for(const auto& v:actorSample.vertices)messageTalkAnchor.y=std::max(messageTalkAnchor.y,a.position[1]*.01f+v.y*.02f);
        const auto& mario=g.harborTutorial.actors[1].position;
        if(project(messageTalkAnchor,anchorX,anchorY) && project({mario[0]*.01f,mario[1]*.01f,mario[2]*.01f},marioX,marioY))mode=message.windowType==1?7:messageSpeakerMode(anchorX,marioX);
    }
    if(newWindow && !supplied && !tutorial && professor) {
        const auto& a=g.storyPresentation().actors[g.storyPresentation().speaker];const auto& root=g.storyPresentation().speakerRoot;
        messageTalkAnchor={root[0]*.01f,root[1]*.01f,root[2]*.01f};
        const ActorAnimationPack* animations=&partnerAnimations;
        const char* name=a.moving?"PKR_W_1":"PKR_S_1";
        if(g.storyPresentation().speaker==2) {
            animations=nullptr;name="T_1";
            for(const auto& gpu:plaza.actorModels)if(fieldActorAssets.models()[gpu.model].asset=="professor")animations=&gpu.animations;
        }
        if(g.storyPresentation().underground && g.storyPresentation().speaker>=3) {
            animations=&undergroundAnimations;name=a.pose.c_str();
        }
        const auto* pose=animations?animations->pose(name):nullptr;
        float npcHeight=0;
        if(g.storyPresentation().speaker==2)for(const auto& actor:fieldActorAssets.actors())
            if(fieldActorAssets.models()[actor.model].originalName=="c_kuri_h"){npcHeight=actor.npcHeight;break;}
        if(npcHeight>0)messageTalkAnchor.y+=npcHeight*.01f;
        if(!npcHeight && pose && pose->evaluate(float(g.ticks),actorSample) && !actorSample.vertices.empty()) {
            const bool right=a.yaw>0 && a.yaw<180;
            const ActorRootTransform transform({messageTalkAnchor.x,messageTalkAnchor.y,messageTalkAnchor.z},right?180:0,right);
            CameraPoint low{INFINITY,INFINITY,INFINITY},high{-INFINITY,-INFINITY,-INFINITY};
            for(const auto& v:actorSample.vertices){const auto p=transform.point(v);low.x=std::min(low.x,p[0]);low.z=std::min(low.z,p[2]);high.x=std::max(high.x,p[0]);high.y=std::max(high.y,p[1]);high.z=std::max(high.z,p[2]);}
            messageTalkAnchor={(low.x+high.x)*.5f,high.y,(low.z+high.z)*.5f};
        }
        if(project(messageTalkAnchor,anchorX,anchorY) && project({g.progress.position.x,g.groundHeight,g.progress.position.z},marioX,marioY))
            mode=message.windowType==1?7:messageSpeakerMode(anchorX,marioX);
    }
    if(newWindow && !supplied && fieldTalk && !professor) {
        const auto& actor=g.fieldActorTalk.actor;
        messageTalkAnchor={actor.position[0]*.01f,actor.position[1]*.01f,actor.position[2]*.01f};
        if(actor.npcHeight>0)messageTalkAnchor=NpcPresentation::talkAnchor(actor.position,actor.npcHeight);
        if(project(messageTalkAnchor,anchorX,anchorY) && project({g.progress.position.x,g.groundHeight+g.jumpHeight,g.progress.position.z},marioX,marioY))
            mode=message.windowType==1?7:messageSpeakerMode(anchorX,marioX);
    }
    for(unsigned i=0;newWindow && !supplied && !tutorial && !fieldTalk && !professor && i<g.harborStory.actors.size();++i) {
        const auto& actor=g.harborStory.actors[i];if(actor.key!=(encounter?g.harborEncounter.speaker:g.harborStory.speaker))continue;
        const auto& root=encounter?g.harborEncounter.speakerRoot:g.harborStory.speakerRoot;
        messageTalkAnchor={root[0]*.01f,root[1]*.01f,root[2]*.01f};
        if(i==0) {
            // Import the rendered pose bounds rather than an authored height.
            const auto* pose=partnerAnimations.pose(actor.moving?"PKR_W_1":"PKR_S_1");
            if(pose && pose->evaluate(float(g.ticks),actorSample) && !actorSample.vertices.empty()) {
                CameraPoint low{INFINITY,INFINITY,INFINITY},high{-INFINITY,-INFINITY,-INFINITY};
                const ActorRootTransform rootTransform({messageTalkAnchor.x,messageTalkAnchor.y,messageTalkAnchor.z},actor.display,actor.display==180);
                for(const auto& v:actorSample.vertices) {
                    const auto p=rootTransform.point(v);
                    low.x=std::min(low.x,p[0]);low.z=std::min(low.z,p[2]);high.x=std::max(high.x,p[0]);high.y=std::max(high.y,p[1]);high.z=std::max(high.z,p[2]);
                }
                messageTalkAnchor={(low.x+high.x)*.5f,high.y,(low.z+high.z)*.5f};
            }
        } else {
            const auto& offset=harborTalkOffsets[i==1?0:1];
            messageTalkAnchor.x+=offset[0];messageTalkAnchor.y+=offset[1];messageTalkAnchor.z+=offset[2];
        }
        if(project(messageTalkAnchor,anchorX,anchorY) &&
           project({g.progress.position.x,g.groundHeight+g.jumpHeight,g.progress.position.z},marioX,marioY))
            mode=message.windowType==1?7:messageSpeakerMode(anchorX,marioX);
        break;
    }
    if(newWindow){messageWindowSerial=message.serial;messagePlacement=mode;}
    if(!project(messageTalkAnchor,anchorX,anchorY))mode=0;
    else mode=supplied && !suppliedAnchor?0:messagePlacement;
    if(message.windowType==11 || message.windowType==5 || message.windowType==3)mode=0;
    if(message.placementOverride>=0)mode=unsigned(message.placementOverride);
    auto plan=messageWindowPlan(message.geometry,mode,anchorX,anchorY,message.alpha,message.windowType);
    stretchMessageTail(plan,anchorX,anchorY);
    if(message.windowType!=11)messageBorder(plan,message.alpha,message.windowType);
    if(!plan.textVisible)return;
    auto& messageDynamic=messageDynamicWindows[message.serial];
    if(messageDynamic.empty())for(const auto& r:message.records)messageDynamic.push_back(r.value);
    while(messageDynamic.size()<message.records.size())messageDynamic.push_back(message.records[messageDynamic.size()].value);
    uint32_t color=0x000000ff;
    for(size_t i=0;i<std::min(message.cursor,message.records.size());++i) {
        const auto& r=message.records[i];if(r.code==0xfff7){color=r.flags;continue;}
        if(r.code==0xfff8) {
            messageIconImage(r.flags,g.ticks,200+(plan.textX+r.x)*MessageWindowScale,120-(plan.textY+message.scroll+r.y)*.5f,r.scale,message.alpha);
            continue;
        }
        if(r.code>=font.glyphs.size())continue;
        if(float(message.scroll+r.y)>=std::fma(32.f,r.scale,-10.f))continue;
        const auto& glyph=font.glyphs[r.code];
        float x=200+(plan.textX+r.x)*MessageWindowScale,y=120-(plan.textY+float(r.y)+message.scroll-2)*.5f;
        const float dynamic=(r.flags&1)?messageDynamic[i]:1;
        const float scaleX=MessageWindowScale*r.scale*dynamic,scaleY=.5f*((r.flags&0x10000) && !(r.flags&1)?1:r.scale)*dynamic;
        if(r.flags&2) {
            auto jitter=[&](){messageRng^=messageRng<<13;messageRng^=messageRng>>17;messageRng^=messageRng<<5;return (messageRng%10000)%3;};
            x+=jitter()*MessageWindowScale;y-=jitter()*.5f;
        } else if((r.flags&4) && !(r.flags&1)) {
            const float phase=.01f*float(g.ticks*1000/60-message.openedAt)-.02f*(plan.textX+r.x);
            x+=3.2f*std::cos(phase)*MessageWindowScale;y+=3.2f*std::sin(phase)*.5f;
        }
        x+=float(glyph.width>>1)*(1-dynamic)*r.scale*MessageWindowScale;
        y+=12*(1-dynamic)*r.scale*.5f;
        const bool system=message.windowType==3 || message.windowType==11;
        const float clipTop=message.windowType==5?0:120-plan.textY*.5f-(system?4:0);
        const float clipBottom=message.windowType==5?240:clipTop+(system?message.geometry.height-50:message.windowType==1?126:100)*.5f;
        // Relax admitted dynamic records even when the window clips the image.
        if(r.flags&1)messageDynamic[i]=std::fma(.5f,dynamic-1,1.f);
        const auto crop=messageGlyphCrop(x,y,glyph.width*scaleX,glyph.height*scaleY,clipTop,clipBottom);
        if(!crop.visible)continue;
        Tex3DS_SubTexture sub{};sub.width=glyph.width;sub.height=glyph.height;
        sub.left=(glyph.x+glyph.width*crop.u0)/512.f;sub.right=(glyph.x+glyph.width*crop.u1)/512.f;
        sub.top=1-(glyph.y+glyph.height*crop.v0)/256.f;sub.bottom=1-(glyph.y+glyph.height*crop.v1)/256.f;
        C2D_Image image{&fontTexture.texture,&sub};C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(color>>24,(color>>16)&255,(color>>8)&255,(color&255)*message.alpha/255),1.f);
        const C2D_DrawParams params={{crop.x,crop.y,crop.width,crop.height},{0,0},.5f,0};
        C2D_DrawImage(image,&params,&tint);
    }
    if(message.canAdvanceText())messageAdvanceIndicator(plan,g.ticks);
}
void Renderer::messageAdvanceIndicator(const MessageWindowPlan& plan,uint64_t ticks,bool onBottom) {
    const auto* pose=messageIndicatorAnimations.pose("HOS_C_1");
    if(!pose || !pose->evaluate(float(ticks),messageIndicatorSample) ||
       messageIndicatorSample.vertices.size()>256-messageIndicatorCount)return;
    const unsigned first=messageIndicatorCount;
    const float rootX=plan.box.x+plan.box.width-52,rootY=plan.box.y-plan.box.height+31;
    for(const auto& v:messageIndicatorSample.vertices) {
        // Original animPoseDraw: yaw180, scale5; original model Y is up.
        messageIndicatorVertices[messageIndicatorCount++]={200+(rootX-v.x*5)*MessageWindowScale,120-(rootY+v.y*5)*.5f,0,v.u,1-(1-v.v)*8,1,1,1,1};
    }
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,messageIndicatorVertices,sizeof(Vertex),3,0x210);
    GSPGPU_FlushDataCache(messageIndicatorVertices+first,sizeof(Vertex)*(messageIndicatorCount-first));
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,onBottom?320:400,240,0,-1,1,true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
    C3D_AlphaTest(true,GPU_GEQUAL,128);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_TexBind(0,&messageIndicatorTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,first,messageIndicatorCount-first);
    C2D_Prepare();C2D_SceneBegin(onBottom?bottom:top);
}
void Renderer::messageIconImage(uint32_t id,uint64_t ticks,float centerX,float bottomY,float scale,uint8_t alpha) {
    const auto* icon=messageIcon(id);if(!icon)return;const auto& frame=icon->frame(ticks);
    Tex3DS_SubTexture sub{};sub.width=frame.width;sub.height=frame.height;
    sub.left=frame.u0;sub.right=frame.u1;sub.top=frame.v0;sub.bottom=frame.v1;
    const float width=frame.width*scale*MessageWindowScale,height=frame.height*scale*.5f;
    C2D_Image image{&messageIconTexture.texture,&sub};C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(255,255,255,alpha),0);
    C2D_DrawParams params={{centerX-width*.5f,bottomY-height,width,height},{0,0},.6f,0};C2D_DrawImage(image,&params,&tint);
}
void Renderer::messageBorder(const MessageWindowPlan& plan,uint8_t alpha,int type) {
    const auto& b=plan.box;
    if(type==5) {
        const auto* original=Tex3DS_GetSubTexture(messagePanel.imported,0);
        C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(255,255,255,alpha),0);
        const float x[]={0,30,b.width-30,b.width},y[]={0,30,b.height-30,b.height};
        for(int row=0;row<3;++row)for(int col=0;col<3;++col) {
            auto sub=*original;sub.width=24;sub.height=24;
            const float du=(original->right-original->left)/3,dv=(original->bottom-original->top)/3;
            sub.left=original->left+col*du;sub.right=sub.left+du;sub.top=original->top+row*dv;sub.bottom=sub.top+dv;
            C2D_Image image{&messagePanel.texture,&sub};
            C2D_DrawImageAt(image,200+(b.x+x[col])*MessageWindowScale,120-(b.y-y[row])*.5f,.4f,&tint,
                (x[col+1]-x[col])*MessageWindowScale/24,(y[row+1]-y[row])*.5f/24);
        }
        return;
    }
    if(type==3) {
        messageSystemPhase+=.005f;if(messageSystemPhase>10)messageSystemPhase-=10;
        const float w=std::max(b.width,32.f),h=std::max(b.height,32.f);
        const float quads[4][4]={{0,0,w-32,h-32},{0,h-32,w-32,32},{w-32,0,32,h-32},{w-32,h-32,32,32}};
        constexpr unsigned corners[6][2]={{0,0},{1,0},{1,1},{0,0},{1,1},{0,1}};
        for(unsigned q=0;q<4;++q)for(unsigned n=0;n<6;++n) {
            const float dx=quads[q][2]*corners[n][0],dy=quads[q][3]*corners[n][1];
            const float x=quads[q][0]+dx,y=quads[q][1]+dy;
            messageSystemVertices[q*6+n]={200+(b.x+x)*MessageWindowScale,120-(b.y-y)*.5f,0,
                messageSystemPhase+x/128,1+messageSystemPhase-y/64,dx/32,1-dy/32,1,1,1,alpha/255.f};
        }
        C2D_Flush();C3D_BindProgram(&messageSystemProgram);
        auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
        AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);
        AttrInfo_AddLoader(attrs,2,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,3,GPU_FLOAT,4);
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,messageSystemVertices,sizeof(SystemVertex),4,0x3210);
        GSPGPU_FlushDataCache(messageSystemVertices,sizeof(SystemVertex)*24);
        C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);
        C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,messageSystemTransform,&projection);
        C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_GREATER,0);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
        for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
        auto* env=C3D_GetTexEnv(0);
        C3D_TexEnvSrc(env,C3D_RGB,GPU_TEXTURE2,GPU_TEXTURE0,GPU_TEXTURE2);
        C3D_TexEnvOpRgb(env,GPU_TEVOP_RGB_SRC_COLOR,GPU_TEVOP_RGB_SRC_COLOR,GPU_TEVOP_RGB_SRC_ALPHA);
        C3D_TexEnvFunc(env,C3D_RGB,GPU_INTERPOLATE);
        C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE0,GPU_TEXTURE1);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
        env=C3D_GetTexEnv(1);C3D_TexEnvSrc(env,C3D_Alpha,GPU_PREVIOUS,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
        C3D_TexBind(0,&messageSystemPattern.texture);
        for(unsigned q=0;q<4;++q) {
            C3D_TexBind(1,&messageSystemMasks[q].texture);C3D_TexBind(2,&messageSystemMasks[q+4].texture);
            C3D_DrawArrays(GPU_TRIANGLES,q*6,6);
        }
        C3D_TexBind(1,nullptr);C3D_TexBind(2,nullptr);C2D_Prepare();C2D_SceneBegin(top);return;
    }
    auto image=[&](unsigned x,unsigned y,unsigned w,unsigned h) {
        Tex3DS_SubTexture sub{};sub.width=w;sub.height=h;sub.left=x/1024.f;sub.right=(x+w)/1024.f;sub.top=1-y/512.f;sub.bottom=1-(y+h)/512.f;return sub;
    };
    C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(255,255,255,alpha),0.f);
    // Shadow before fill; original ordinary indices13..16 and9..12.
    for(unsigned pass=0;pass<2;++pass) {
        if(plan.tail) {
            // Source sheared GX quad, retaining horizontal texture rows. A
            // rotated sprite changes the triangle's pointing axis at steep
            // angles and incorrectly magnifies its transparent margins.
            C2D_Flush();
            auto sub=image(600,pass?0:64,48,64);
            constexpr float uv[6][2]={{0,0},{1,0},{1,1},{0,0},{1,1},{0,1}};
            for(unsigned n=0;n<6;++n) {
                const float u=uv[n][0],v=uv[n][1];const auto p=messageTailMaskPoint(plan,u,v,pass==0);
                messageTailVertices[pass*6+n]={200+p.x*MessageWindowScale,120-p.y*.5f,0,
                    sub.left+(sub.right-sub.left)*u,sub.top+(sub.bottom-sub.top)*v,1,1,1,alpha/255.f};
            }
            C3D_BindProgram(&program);
            auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
            AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
            auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,messageTailVertices,sizeof(Vertex),3,0x210);
            C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
            C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
            C3D_AlphaTest(true,GPU_GREATER,8);
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
            for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
            auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
            C3D_TexBind(0,&messageWindowTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,pass*6,6);
            C2D_Prepare();C2D_SceneBegin(top);
        }
        auto sub=type==1?image(600,pass?128:192,232,64):image(0,pass?0:176,560,176);
        C2D_Image bubble{&messageWindowTexture.texture,&sub};
        C2D_DrawImageAt(bubble,200+b.x*MessageWindowScale,120-b.y*.5f,.4f,&tint,b.width*MessageWindowScale/sub.width,b.height*.5f/sub.height);
    }
}
void Renderer::marioGroundShadow(const FieldShadowRecord& record) {
    const auto points=fieldShadowPolygon(record);
    constexpr int triangles[]={0,1,2,2,1,3,2,3,4,4,3,5}; // Original strip triangulation.
    for(int index:triangles) {
        const auto p=points[index];vertex(p.x*.01f,p.y*.01f,p.z*.01f,0,0,record.color);
    }
}
int Renderer::marioAirShadow(const FieldShadowRecord& record) {
    const auto mesh=fieldAirShadowVertices(shadowGeometry,record,fieldAirShadowPlan(record));
    if(mesh.empty() || mesh.size()*3>size_t(MaxVertices-count))return 0;
    // Source pass1 vertex material, pass2 constant zero, pass3 global material.
    // CPU material selection adapts the common untextured GX TEV program.
    for(int pass=0;pass<3;++pass)for(const auto& v:mesh) {
        uint32_t color=0;
        if(pass==0)color=rgb(v.color[0],v.color[1],v.color[2],v.color[3]);
        if(pass==2)color=record.color;
        vertex(v.point.x*.01f,v.point.y*.01f,v.point.z*.01f,0,0,color);
        if(pass==0)vertices[count-1].a*=record.alpha/255.f;
    }
    return int(mesh.size());
}
void Renderer::world(const Game& g) {
    count=0;
    bool battle=g.scene==Scene::Battle || g.scene==Scene::Victory || g.scene==Scene::Defeat;
    marioPose=g.jumpHeight>.01f?1:0;
    if(battle && g.phase==Phase::Windup)marioPose=g.selection==0?1:2;
    float cam=battle?0:g.cameraX;
    C3D_Mtx projection,view,transform;
    const auto& frame=g.fieldCameraFrame;
    // The handheld viewport adapts source aspect; camera eye/at/lens and
    // gameplay screen cache retain their original owners.
    Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(battle?FieldFov:frame.fov),C3D_AspectRatioTop,battle?.1f:frame.nearPlane,battle?80:frame.farPlane,false);
    Mtx_LookAt(&view,FVec3_New(battle?cam:frame.eye.x,battle?2.8f:frame.eye.y,battle?5.5f:frame.eye.z),FVec3_New(battle?cam:frame.at.x,battle?1.f:frame.at.y,battle?-.4f:frame.at.z),FVec3_New(0,1,0),false);
    Mtx_Multiply(&transform,&projection,&view);
    worldTransform=transform;
    // Original room backdrop, drawn without depth writes before the room.
    const float x0=cam-30,x1=cam+30,y0=-12,y1=24,z=-25;
    vertex(x0,y0,z,0,0,0xffffffff);vertex(x1,y0,z,1,0,0xffffffff);vertex(x1,y1,z,1,1,0xffffffff);
    vertex(x1,y1,z,1,1,0xffffffff);vertex(x0,y1,z,0,1,0xffffffff);vertex(x0,y0,z,0,0,0xffffffff);
    if(!battle) {
        if(!g.harborStory.enabled)shadow(g.partner,.43f,g.partnerHeight);
        if(g.progress.room==0)shadow(NpcPosition,.48f);
        if(g.progress.room==0 && !g.progress.crumpDefeated) shadow(CrumpPosition,.65f);
        struct Actor { int index;Vec2 p;float h,w,face,lift,tilt; };
        float bob=std::sin(g.ticks*.22f)*.018f;
        std::array<Actor,4> actors{{
            {0,g.progress.position,1.45f,1.45f*g.turnScale,g.facing,g.water.active()?g.water.position[1]:g.groundHeight+g.jumpHeight,bob},
            {1,g.partner,1.05f,1.05f,g.facing,g.partnerHeight,bob},
            {3,NpcPosition,1.4f,1.4f,1,0,0},
            {2,CrumpPosition,1.9f,1.9f,-1,0,bob}
        }};
        std::sort(actors.begin(),actors.end(),[](const Actor& a,const Actor& b){return a.p.z<b.p.z;});
        for(auto a:actors) if((a.index<2 || g.progress.room==0) && (a.index!=2 || !g.progress.crumpDefeated)) {
            if(a.index==1)continue; // Original animated Goombella is drawn with her own texture pack.
            if(a.index==0 && (g.marioPresentation.supported || (g.marioPresentation.displayFlags&1)))continue;
            character(a.index,a.p,a.h,a.w,a.face,a.lift,a.tilt);
        }
    } else {
        Vec2 mario{-1.7f,0},crump{1.7f,0};float lift=0,tilt=0;
        if(g.phase==Phase::Windup && g.selection==0) {
            float t=std::min(1.f,g.phaseTick/float(BattleRules::JumpImpact));
            mario.x += 2.9f*t; lift=2.1f*std::sin(t*3.14159265f);
        }
        if(g.phase==Phase::Impact) {
            float t=std::min(1.f,g.phaseTick/42.f);mario.x+=2.9f*(1-t);
            lift=g.actionSuccess?.9f*std::sin(t*3.14159265f):0;
        }
        if(g.phase==Phase::Windup && g.selection==1) tilt=-.25f;
        if(g.phase==Phase::EnemyWindup) {
            float t=std::min(1.f,g.phaseTick/float(BattleRules::EnemyImpact));
            crump.x-=2.9f*t;
        }
        if(g.phase==Phase::EnemyImpact) crump.x-=2.9f*(1-std::min(1.f,g.phaseTick/36.f));
        shadow(mario,.55f);shadow(crump,.75f);shadow({-2.5f,.1f},.45f);
        character(1,{-2.5f,.1f},1.05f,1.05f,1);
        character(0,mario,1.55f,1.55f,1,lift,tilt);
        character(2,crump,2.1f,2.1f,-1);
    }
    const int legacyCount=count;
    if(!battle && g.harborEncounter.locked()) {
        const auto& a=g.harborEncounter.actors[0];const bool right=a.yaw>0 && a.yaw<180;
        characterModel(a.moving?"M_W_1":a.pose.c_str(),float(g.harborEncounter.now-a.poseAt)*.06f,g.progress.position,g.groundHeight,right?180:0,right,false,&harborMarioAnimations,harborMarioUVScale);
    } else if(!battle && (g.trioBattleActive || g.storyPresentation().locked())) {
        const auto& a=g.storyPresentation().actors[0];const bool right=a.yaw>0 && a.yaw<180;
        characterModel(a.moving?"M_W_1":a.pose.c_str(),float(g.storyPresentation().now-a.poseAt)*.06f,g.trioBattleActive?Vec2{a.position[0]*.01f,a.position[2]*.01f}:g.progress.position,g.trioBattleActive?a.position[1]*.01f:g.groundHeight,
                       right?180:0,right,false,g.trioBattleActive?&trioMarioAnimations:&professorMarioAnimations,g.trioBattleActive?trioMarioUVScale:professorMarioUVScale);
    } else if(!battle && g.marioPresentation.supported && !(g.marioPresentation.displayFlags&1))
        characterModel(g.marioPresentation.selected,g.marioPresentation.animationFrame(),g.progress.position,g.water.active()?g.water.position[1]:g.groundHeight+g.jumpHeight,
                       g.marioPresentation.drawYaw(),g.marioPresentation.reflectZ());
    const int actorEnd=count;
    if(!battle && (g.trioBattleActive || g.storyPresentation().locked())) {
        const auto& a=g.storyPresentation().actors[1];const bool right=a.yaw>0 && a.yaw<180;
        if(g.trioBattleActive) {
            const float cameraAngle=npcAngles.angle(g.fieldCameraFrame.eye.x,g.fieldCameraFrame.eye.z,g.fieldCameraFrame.at.x,g.fieldCameraFrame.at.z);
            if(!a.hidden)characterModel(g.storyPresentation().message.talking && g.storyPresentation().speaker==1?"PKR_T_1":a.pose.c_str(),float(g.storyPresentation().now-a.poseAt)*.06f,{a.position[0]*.01f,a.position[2]*.01f},a.position[1]*.01f,right?180:0,battleMirrorZ(right?180:0,a.battleRotation[1],cameraAngle,a.battleFlags),true,&trioPartnerAnimations,trioPartnerUVScale,&a);
        } else if(!a.hidden)characterModel(g.storyPresentation().message.talking && g.storyPresentation().speaker==1?"PKR_T_1":a.moving?"PKR_W_1":"PKR_S_1",float(g.ticks),g.partner,g.partnerHeight,right?180:0,right,true);
    } else if(!battle && !g.harborStory.enabled)characterModel(g.partnerPresentation.name(),g.partnerPresentation.frame(),g.partner,g.partnerHeight,
                             g.partnerPresentation.yaw(),g.partnerPresentation.reflectZ(),true);
    if(!battle && g.harborStory.enabled && g.harborStory.triggered) {
        const auto& a=g.harborStory.actors[0];
        characterModel(a.moving?"PKR_W_1":"PKR_S_1",float(g.ticks),{a.x*.01f,a.z*.01f},a.y*.01f,a.display,a.display==180,true);
    }
    const int partnerEnd=count;
    actorMetrics={};
    if(!battle)fieldActors(g);
    const int fieldActorEnd=count;
    if(!battle && g.storyPresentation().underground) {
        for(unsigned i=3;i<6;++i) {
            const auto& a=g.storyPresentation().actors[i];if(a.hidden)continue;
            const bool right=a.yaw>0 && a.yaw<180;std::string pose=a.pose;
            if((a.flags&0x200) && g.storyPresentation().message.talking && g.storyPresentation().speaker==i)pose=std::string(i==3?"KUR":i==4?"PTK":"TGK")+"_T_1";
            const float cameraAngle=npcAngles.angle(g.fieldCameraFrame.eye.x,g.fieldCameraFrame.eye.z,g.fieldCameraFrame.at.x,g.fieldCameraFrame.at.z);
            const bool mirror=g.trioBattleActive?battleMirrorZ(right?180:0,a.battleRotation[1],cameraAngle,a.battleFlags):right;
            characterModel(pose.c_str(),float(g.storyPresentation().now-a.poseAt)*.06f,{a.position[0]*.01f,a.position[2]*.01f},a.position[1]*.01f,right?180:0,mirror,false,&undergroundAnimations,undergroundUVScale,g.trioBattleActive?&a:nullptr);
        }
    }
    const int undergroundEnd=count;
    if(!battle && !g.trioBattleActive && g.storyPresentation().locked() && g.storyPresentation().itemVisible) {
        const auto& s=g.storyPresentation();const auto& p=s.itemPosition;
        // Native billboard bridge for the source icon/position/scale requests.
        const ActorRootTransform transform({p[0]*.01f,p[1]*.01f,p[2]*.01f},g.fieldCameraFrame.yaw,false,.01f);
        const float w=professorMapSize[0]*s.itemScale,h=professorMapSize[1]*s.itemScale;
        constexpr unsigned indices[]={0,1,2,2,1,3};
        const float points[4][4]={{-w*.5f,0,0,0},{w*.5f,0,1,0},{-w*.5f,h,0,1},{w*.5f,h,1,1}};
        for(unsigned index:indices){const auto& v=points[index];const auto q=transform.point({v[0],v[1],0,v[2],v[3]});vertex(q[0],q[1],q[2],v[2],v[3],0xffffffff);}
    }
    const int professorItemEnd=count;
    if(!battle && g.harborStory.enabled && g.harborStory.triggered)harborActors(g);
    const int harborEnd=count;
    bool polygonShadow=false;int airShadowCount=0;
    if(!battle) {
        const float rootY=g.water.active()?g.water.position[1]:g.groundHeight+g.jumpHeight;
        auto request=marioShadowRequest({g.progress.position.x*100,rootY*100,g.progress.position.z*100},
                                       g.groundMetadataHeight*100,g.groundContact.sourceNode!=0,
                                       g.marioPresentation.displayFlags,uint32_t(g.fieldIdle.mainFlags));
        FieldShadowServices services{nullptr,[](void*,FieldWallVector p,FieldWallVector d,float length){return fieldShadowRay(p,d,length);}};
        if(prepareFieldShadow(request,services)) {
            if(fieldShadowPolygonAdmitted(request)){marioGroundShadow(request);polygonShadow=true;}
            else airShadowCount=marioAirShadow(request);
        }
    }

    if(count)GSPGPU_FlushDataCache(vertices,size_t(count)*sizeof(Vertex));
    C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
    C3D_CullFace(GPU_CULL_NONE);
    C3D_DepthTest(true,GPU_GREATER,battle?GPU_WRITE_ALL:FieldRGBDepth);
    C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    C3D_TexBind(0,&atlas);
    for(int n=0;n<6;++n) C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_DepthTest(false,GPU_ALWAYS,battle?GPU_WRITE_COLOR:FieldRGB);
    C3D_TexBind(0,&sky.texture);C3D_DrawArrays(GPU_TRIANGLES,0,6);
    C3D_DepthTest(true,GPU_GREATER,battle?GPU_WRITE_ALL:FieldRGBDepth);
    if(g.trioBattleActive){drawMap(true,g,false,&trioBackground);drawMap(true,g,false,&trioStage);}else drawMap(battle,g);
    buffers=C3D_GetBufInfo();
    BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
    C3D_TexBind(0,&atlas);
    C3D_DrawArrays(GPU_TRIANGLES,6,legacyCount-6);
    if(actorEnd>legacyCount){C3D_TexBind(0,g.trioBattleActive?&trioMarioTexture.texture:g.harborEncounter.locked()?&harborMarioTexture.texture:g.storyPresentation().locked()?&professorMarioTexture.texture:&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,legacyCount,actorEnd-legacyCount);}
    if(partnerEnd>actorEnd){C3D_TexBind(0,g.trioBattleActive?&trioPartnerTexture.texture:&partnerTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,actorEnd,partnerEnd-actorEnd);}
    if(!battle)drawFieldActors(false);
    if(undergroundEnd>fieldActorEnd){C3D_TexBind(0,&undergroundTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,fieldActorEnd,undergroundEnd-fieldActorEnd);}
    if(professorItemEnd>undergroundEnd){C3D_TexBind(0,&professorMapTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,undergroundEnd,professorItemEnd-undergroundEnd);}
    if(harborEnd>professorItemEnd){C3D_TexBind(0,&harborTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,professorItemEnd,harborEnd-professorItemEnd);}
    if(polygonShadow || airShadowCount) {
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env,C3D_Both,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(env,C3D_Both,GPU_REPLACE);
        C3D_AlphaTest(false,GPU_ALWAYS,0);
        if(polygonShadow) {
            C3D_DepthTest(true,GPU_GEQUAL,FieldRGB);
            C3D_DrawArrays(GPU_TRIANGLES,harborEnd,count-harborEnd);
        } else {
            // Source LEQUAL becomes GEQUAL with the reversed native projection.
            // Preserve each record's alpha OR, front-face zero, RGB mask blend.
            C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITE_ALPHA);
            C3D_CullFace(GPU_CULL_FRONT_CCW);
            C3D_ColorLogicOp(GPU_LOGICOP_OR);
            C3D_DrawArrays(GPU_TRIANGLES,harborEnd,airShadowCount);
            C3D_CullFace(GPU_CULL_BACK_CCW);
            C3D_ColorLogicOp(GPU_LOGICOP_COPY);
            C3D_DrawArrays(GPU_TRIANGLES,harborEnd+airShadowCount,airShadowCount);
            C3D_CullFace(GPU_CULL_FRONT_CCW);
            C3D_DepthTest(true,GPU_GEQUAL,FieldRGB);
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_DST_ALPHA,GPU_ONE_MINUS_DST_ALPHA,GPU_ONE,GPU_ZERO);
            C3D_DrawArrays(GPU_TRIANGLES,harborEnd+airShadowCount*2,airShadowCount);
        }
        C3D_CullFace(GPU_CULL_NONE);
        C3D_AlphaTest(true,GPU_GREATER,8);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
        C3D_DepthTest(true,GPU_GREATER,FieldRGBDepth);
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    }
    // Original stage400000 opaques,500000 shadows,600000 translucent.
    if(!battle){drawFieldActors(true);if(g.trioBattleActive){drawMap(true,g,true,&trioBackground);drawMap(true,g,true,&trioStage);}else drawMap(false,g,true);}
}
void Renderer::text(const char* str,float x,float y,float scale,uint32_t color) {
    C2D_Text t;C2D_TextParse(&t,textBuffer,str);C2D_TextOptimize(&t);
    C2D_DrawText(&t,C2D_WithColor,x,y,.5f,scale,scale,color);
}
void Renderer::panel(float x,float y,float w,float h,uint32_t color) {
    C2D_DrawRectSolid(x+2,y+3,.4f,w,h,rgb(36,28,32,110));
    C2D_DrawRectSolid(x,y,.4f,w,h,Ink);
    C2D_DrawRectSolid(x+2,y+2,.4f,w-4,h-4,color);
}
void Renderer::button(const char* label,float x,float y,float w,float h,bool active,bool disabled) {
    panel(x,y,w,h,disabled?rgb(207,201,184):active?Gold:Paper);
    text(label,x+7,y+8,.48f,disabled?rgb(135,129,116):Ink);
}
void Renderer::overlay(const Game& g) {
    if(g.harborTutorial.running()){originalMessage(g);if(!tutorialReady || g.harborTutorial.failed || g.harborTutorial.introComplete)text(!tutorialReady?"Development notice: tutorial presentation unavailable":g.prompt(),25,112,.43f,Ink);return;}
    if(g.harborEncounter.locked()){originalMessage(g);if(g.harborEncounter.failed || g.harborEncounter.pendingService)text(g.prompt(),25,112,.43f,Ink);return;}
    if(g.saveObjects && (g.saveLocked() || g.saveObjects->presentation.message.active())) {
        const auto& p=g.saveObjects->presentation;originalMessage(g,&p.message);
        const auto& choice=p.selection;
        if(choice.active()) {
            originalMessage(g,&choice.message);const auto& box=choice.message.geometry;
            choice.cursorY=std::fma(.30000001192092896f,box.y-float(31*choice.current+58)-choice.cursorY,choice.cursorY);
            messageIconImage(0x1f8,g.ticks,200+(box.x+2)*MessageWindowScale,120-choice.cursorY*.5f,1,uint8_t(choice.opacity));
        }
        return;
    }
    if(g.trioBattleActive || g.fieldActorTalk.message.active() || g.harborStory.locked() || g.storyPresentation().locked()) {
        originalMessage(g);
        const auto& choice=g.storyPresentation().selection;
        if(choice.active()) {
            originalMessage(g,&choice.message);
            const auto& box=choice.message.geometry;
            choice.cursorY=std::fma(.30000001192092896f,box.y-float(31*choice.current+58)-choice.cursorY,choice.cursorY);
            messageIconImage(0x1f8,g.ticks,200+(box.x+2)*MessageWindowScale,120-choice.cursorY*.5f,1,uint8_t(choice.opacity));
        }
        if(g.harborStory.failed || g.storyPresentation().failed)text("Development notice: source scene unavailable",25,112,.45f,Ink);
        if(g.trioBattleActive && g.trioCommands.state==TrioBattleCommands::State::Target) {
            const auto pair=g.trioCommands.selected();
            panel(16,20,164,30,Paper);text(g.trioCommands.targetName(pair[0]),25,28,.55f,Ink);
        }
        if(g.trioBattleActive && g.trioAttack.rewardPresentationActive()) {
            // Source camera8 submits the reward UI after the world. Keep the
            // native two-dimensional layer independent of the stage depth.
            C2D_Flush();C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
            // Original windowDispGX_Waku_col type0: nine texture0 quads with
            // ten-unit borders and FFFFFF80 modulation, adapted to400x240.
            if(g.trioAttack.rewardIcons()){
            sourceWindowFrame(type0WindowFrame(-76.8999939f,21,165.7999878f,24.1999989f,10,{255,255,255,128}));
            for(const auto& icon:*g.trioAttack.rewardIcons())if(!icon.hidden)
                messageIconImage(404,icon.animationTick,200+icon.position[0]*MessageWindowScale,120-icon.position[1]*.5f,icon.scale,icon.alpha);
            }
            if(const auto* effect=g.trioAttack.rewardEffect())for(const auto& q:pointGetQuads(*effect)){
                const auto* asset=messageIcon(0x10000+q.texture);if(!asset)continue;const auto& frame=asset->frames[0];
                Tex3DS_SubTexture sub{};sub.width=frame.width;sub.height=frame.height;
                sub.left=frame.u0+(frame.u1-frame.u0)*q.u0;sub.right=frame.u0+(frame.u1-frame.u0)*q.u1;sub.top=frame.v0;sub.bottom=frame.v1;
                C2D_Image image{&messageIconTexture.texture,&sub};C2D_ImageTint tint;
                C2D_PlainImageTint(&tint,q.shadow?rgb(0,0,0,128):rgb(255,255,255,255),q.shadow?1:0);
                C2D_DrawParams params={{200+(q.centerX-q.width*.5f)*MessageWindowScale,120-(q.bottomY+q.height)*.5f,q.width*MessageWindowScale,q.height*.5f},{0,0},.7f,0};
                C2D_DrawImage(image,&params,&tint);
            }
            if(g.trioAttack.pendingFunction())text("Development notice: original battle return pending",25,28,.43f,Ink);
            C2D_Flush();C2D_Prepare();C2D_SceneBegin(top);
        } else if(g.trioBattleActive && (g.trioCommands.state==TrioBattleCommands::State::AttackReady || (g.trioCommands.state==TrioBattleCommands::State::Closed && g.trioAttack.active()))) {
            if(g.trioAttack.pendingFunction()){panel(16,20,368,30,Paper);text("Development notice: source damage services pending",25,28,.43f,Ink);}
            else if(g.trioAttack.active()) {panel(16,20,170,30,Paper);text(g.trioCommands.state==TrioBattleCommands::State::AttackReady && g.trioCommands.attack && g.trioCommands.attack->weapon==0x80353de8?"Hold left, then release":"A: action command",25,28,.5f,Ink);}
            else text(g.trioAttack.finished()?"Development notice: turn scheduling pending":"Development notice: original attack event pending",25,112,.45f,Ink);
        }
        if(g.storyPresentation().pendingService)text("Development notice: original battle backend pending",25,112,.45f,Ink);
        return;
    }
    if(g.scene==Scene::Dialogue) {
        const auto plan=messageWindowPlan({},0,0,0,255);messageBorder(plan,255);
        text(g.dialogue(),64,28,.5f,Ink);messageAdvanceIndicator(plan,g.ticks);return;
    }
    panel(8,7,174,28,Paper);
    char line[128];std::snprintf(line,sizeof(line),"Mario  HP %d/10",g.progress.hp);text(line,16,12,.54f,Ink);
    text((g.scene==Scene::Battle || g.scene==Scene::Victory || g.scene==Scene::Defeat)?"BATTLE STAGE":roomName(g.progress.room),205,12,.46f,rgb(255,246,222));
    panel(8,207,384,27,Paper);text(g.prompt(),15,212,.43f,Ink);
    if(g.scene==Scene::Battle) {
        std::snprintf(line,sizeof(line),"Lord Crump  HP %d/5",g.enemyHP);
        panel(216,40,176,27,Paper);text(line,223,45,.45f,Ink);
        if(g.phase==Phase::Impact && g.selection!=2) {
            std::snprintf(line,sizeof(line),"%s  -%d",g.actionSuccess?"NICE!":"Hit",g.lastDamage);text(line,230,106,.72f,Gold);
        }
        if(g.phase==Phase::EnemyImpact) text(g.guarded?"GUARD!":"Ouch!",108,109,.72f,Gold);
    }
    if(g.scene==Scene::Victory || g.scene==Scene::Defeat) {
        panel(74,87,252,64,Paper);
        text(g.scene==Scene::Victory?"VICTORY!":"TRY AGAIN",96,96,.9f,Ink);
        text(g.scene==Scene::Victory?"Development encounter complete.":"A: retry   B: leave battle",92,128,.43f,Ink);
    }
}
void Renderer::menu(const Game& g,const Metrics& m,const char* notice,bool audioAvailable) {
    if(g.harborTutorial.running())return;
    if(g.trioBattleActive && g.trioAttack.rewardPresentationActive()) {
        if(g.trioAttack.pendingFunction())text("Development notice: original battle return pending",12,76,.42f,Ink);
        return;
    }
    if(g.trioBattleActive && g.trioCommands.state!=TrioBattleCommands::State::Closed) {
        const auto& commands=g.trioCommands;
        if(commands.state==TrioBattleCommands::State::Command) {
            text("Choose an attack",12,76,.63f,Ink);
            for(unsigned n=0;n<(commands.actor==2?1u:2u);++n)
                button(commands.commandName(n),8+160*n,100,144,58,commands.command==n);
            text("Left/right: select   A: targets",12,174,.44f,Ink);
        } else if(commands.state==TrioBattleCommands::State::Target) {
            text("Choose a target",12,76,.63f,Ink);
            for(int n=0;n<commands.count();++n)
                button(commands.targetName(commands.target(n)[0]),4+106*n,100,102,58,commands.candidates.bytes[0xab7]==n);
            text("Left/right: select   A: confirm   B: back",12,174,.4f,Ink);
        } else {
            if(g.trioAttack.active()) {
                text(commands.commandName(commands.command),12,76,.63f,Ink);
                text(g.trioAttack.pendingFunction()?"Development notice: source damage services pending":g.trioCommands.state==TrioBattleCommands::State::AttackReady && g.trioCommands.attack && g.trioCommands.attack->weapon==0x80353de8?"Hold Circle Pad left, then release":"A: action command",12,174,.42f,Ink);
            } else {
                text(g.trioAttack.finished()?"Development notice: turn scheduling pending":"Development notice: original attack event pending",12,76,.42f,Ink);
                if(!g.trioAttack.finished())text("B: return to target selection",12,174,.44f,Ink);
            }
        }
        return;
    }
    if(g.harborEncounter.locked() || g.harborStory.locked() || g.storyPresentation().locked() || g.scene==Scene::Dialogue)return;
    panel(6,5,308,32,Ink);text("ROGUEPORT FIELD NOTES",16,11,.59f,Paper);
    if(g.scene==Scene::Battle) {
        text("Partner: Goombella",12,49,.44f,Ink);
        if(g.phase==Phase::Select) {
            text("Choose an attack",12,76,.63f,Ink);
            button("Jump",8,100,96,58,g.selection==0);
            button("Hammer",110,100,96,58,g.selection==1);
            button("Mushroom",212,100,100,58,g.selection==2,g.progress.mushrooms<=0 || g.progress.hp==10);
            text("Jump: A. Hammer: hold Circle Pad left.",12,174,.44f,Ink);
            text("Mushroom restores 5 HP; uses your turn.",12,192,.42f,Ink);
        } else {
            const bool enemy=g.phase==Phase::EnemyWindup || g.phase==Phase::EnemyImpact;
            const bool winding=g.phase==Phase::Windup || g.phase==Phase::EnemyWindup;
            const int target=enemy?BattleRules::EnemyImpact:BattleRules::JumpImpact;
            text(enemy?"GUARD":g.selection==0?"JUMP":"HAMMER",12,80,.82f,Ink);
            panel(14,119,292,28,Ink);
            if(!enemy && g.selection==1) {
                const auto& hammer=g.hammerAction();
                const float f=winding?std::clamp((hammer.stage-1)/float(hammer.chargeSteps),0.f,1.f):1.f;
                C2D_DrawRectSolid(18,123,.5f,284*f,20,Gold);
                for(int n=1;n<hammer.chargeSteps;++n)
                    C2D_DrawRectSolid(18+284*n/float(hammer.chargeSteps),122,.5f,2,22,Ink);
                const bool ready=winding && hammer.stage==hammer.chargeSteps+1 && hammer.countdown>0 &&
                    hammer.period-hammer.countdown+1<=HammerCommand::releaseAllowance(3);
                text(!winding?(g.actionSuccess?"Nice!":"Attack finished."):
                    ready?"Release Circle Pad now!":"Hold Circle Pad left to charge.",14,161,.48f,Ink);
            } else {
                float f=winding?std::min(1.f,g.phaseTick/float(target+8)):1;
                C2D_DrawRectSolid(18,123,.5f,284*f,20,Gold);
                float marker=18+284*target/float(target+8);
                C2D_DrawRectSolid(marker-2,118,.5f,4,31,Red);
                text(winding?"Press A at the marker.":enemy?(g.guarded?"Guard!":"Hit taken."):
                    (g.actionSuccess?"Nice!":"Attack finished."),14,161,.52f,Ink);
            }
            text("Timing guide is a prototype assist.",14,190,.43f,Ink);
        }
    } else if(g.scene==Scene::FieldDefeat) {
        text("GAME OVER",16,64,.9f,Ink);
        button("Load save / A",16,107,288,48,true);
    } else if(g.water.active()) {
        char line[64];std::snprintf(line,sizeof(line),"Mario  %d/10 HP",g.progress.hp);
        text(line,16,64,.72f,Ink);text("Partner: Goombella",16,105,.57f,Ink);
    } else if(g.scene==Scene::Victory || g.scene==Scene::Defeat) {
        text(g.scene==Scene::Victory?"Battle complete":"Mario needs another try",16,64,.7f,Ink);
        button(g.scene==Scene::Victory?"Return to plaza / A":"Retry / A",16,107,288,48,true);
        if(g.scene==Scene::Defeat) button("Return to plaza / B",16,169,288,43,false);
    } else {
        const char* tabs[]={"Status","Map","Items","Setup"};
        for(int n=0;n<4;++n) button(tabs[n],8+76*n,44,72,38,int(g.menu)==n);
        if(g.menu==Menu::Status) {
            char line[96];std::snprintf(line,sizeof(line),"Mario  %d/10 HP",g.progress.hp);text(line,16,94,.72f,Ink);
            text("Partner: Goombella",16,121,.57f,Ink);
            text(g.progress.crumpDefeated?"Crump encounter complete":"Find Crump east of the plaza",16,147,.46f,Ink);
            button("Save progress / Y",16,177,288,39,true);
        } else if(g.menu==Menu::Items) {
            char line[96];std::snprintf(line,sizeof(line),"Mushroom x%d",g.progress.mushrooms);text(line,16,101,.73f,Ink);
            text("Restores 5 HP.",16,135,.56f,Ink);
            button("Use mushroom / Y",16,177,288,39,true,g.progress.mushrooms<=0 || g.progress.hp==10);
        } else if(g.menu==Menu::Settings) {
            text("A: jump / interact    X: partner",16,97,.47f,Ink);
            text("D-pad: tabs     Y: selected menu",16,123,.47f,Ink);
            text(audioAvailable?"Audio service ready":"Audio unavailable; play continues",16,149,.45f,Ink);
            button(g.progress.sound?"Sound: ON / Y":"Sound: OFF / Y",16,177,288,39,true);
        } else {
            panel(18,93,284,113,rgb(205,183,142));
            const Rect bounds=collisionBounds();
            float width=std::max(.1f,bounds.x1-bounds.x0),depth=std::max(.1f,bounds.z1-bounds.z0);
            float scale=std::min(270.f/width,100.f/depth);
            float originX=160-(bounds.x0+bounds.x1)*scale*.5f,originZ=149-(bounds.z0+bounds.z1)*scale*.5f;
            const auto& walls=collisionWalls();
            for(size_t n=0;n<walls.size();++n) if(wallAtLevel(n,g.groundHeight)) {
                auto wall=walls[n];
                C2D_DrawLine(originX+wall.x0*scale,originZ+wall.z0*scale,rgb(143,103,76),originX+wall.x1*scale,originZ+wall.z1*scale,rgb(143,103,76),1,.5f);
            }
            C2D_DrawRectSolid(originX+g.progress.position.x*scale-3,originZ+g.progress.position.z*scale-3,.6f,6,6,Red);
            C2D_DrawRectSolid(originX+g.partner.x*scale-2,originZ+g.partner.z*scale-2,.6f,4,4,Gold);
            if(g.progress.room==0 && !g.progress.crumpDefeated) C2D_DrawRectSolid(originX+CrumpPosition.x*scale-3,originZ+CrumpPosition.z*scale-3,.6f,6,6,rgb(130,74,140));
            text(roomName(g.progress.room),18,207,.38f,Ink);
        }
    }
    if(notice && *notice) {panel(10,217,300,20,Paper);text(notice,16,218,.4f,Ink);}
    else {
        char line[160];std::snprintf(line,sizeof(line),"%.0f fps | sim %.2f / CPU %.1f / GPU %.1f ms",m.fps,m.simulationMs,m.renderMs,m.gpuMs);
        text(line,9,225,.34f,Ink);
    }
}
void Renderer::render(const Game& g,const Metrics& m,const char* notice,bool audioAvailable) {
    for(auto i=messageDynamicWindows.begin();i!=messageDynamicWindows.end();) {
        const auto id=i->first;const auto& story=g.storyPresentation();
        const bool keep=id==story.message.serial || id==story.selection.message.serial || id==g.harborStory.message.serial ||
            id==g.harborEncounter.message.serial || id==g.harborTutorial.message.serial || id==g.fieldActorTalk.message.serial || (g.saveObjects && (id==g.saveObjects->presentation.message.serial || id==g.saveObjects->presentation.selection.message.serial));
        if(!keep)i=messageDynamicWindows.erase(i);else ++i;
    }
    C2D_TextBufClear(textBuffer);
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    windowFrameCount=0;
    messageIndicatorCount=0;
    const bool battle=g.scene==Scene::Battle || g.scene==Scene::Victory || g.scene==Scene::Defeat;
    C3D_RenderTargetClear(top,C3D_CLEAR_ALL,battle?0xA6CBD8FF:0,0);
    C3D_FrameDrawOn(top);if(g.harborTutorial.running())worldTutorial(g);else world(g);
    C2D_Prepare();C2D_SceneBegin(top);overlay(g);
    if(g.trioBattleActive && g.trioAttack.endingFadeAlpha())C2D_DrawRectSolid(0,0,.99f,400,240,C2D_Color32(0,0,0,g.trioAttack.endingFadeAlpha()));
    if(g.fade.alpha())C2D_DrawRectSolid(0,0,.99f,400,240,C2D_Color32(0,0,0,g.fade.alpha()));
    C2D_Flush();
    C2D_TargetClear(bottom,Paper);C2D_Prepare();C2D_SceneBegin(bottom);menu(g,m,notice,audioAvailable);
    if(g.trioBattleActive && g.trioAttack.endingFadeAlpha())C2D_DrawRectSolid(0,0,.99f,320,240,C2D_Color32(0,0,0,g.trioAttack.endingFadeAlpha()));
    if(g.fade.alpha())C2D_DrawRectSolid(0,0,.99f,320,240,C2D_Color32(0,0,0,g.fade.alpha()));
    if(g.trioBattleActive && g.trioAttack.pendingFunction() && g.trioAttack.endingFadeAlpha()==255){
        C2D_Flush();C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
        text("Development notice: field restoration pending",12,76,.42f,rgb(255,255,255));
    }
    C2D_Flush();
    C3D_FrameEnd(0);
}
void Renderer::shutdown() {
    if(c3dReady) C3D_FrameSync();
    releaseTutorialCurtain();
    releaseTutorialStartupResources();
    if(textBuffer) C2D_TextBufDelete(textBuffer);
    releaseFrontend();
    for(Room* room : {&plaza,&stage,&trioBackground,&trioStage,&harborBattleBackground,&harborBattleStage})freeRoom(*room);
    for(auto& actor:tutorialActors)if(actor.image.imported){Tex3DS_TextureFree(actor.image.imported);C3D_TexDelete(&actor.image.texture);}
    if(sky.imported) {Tex3DS_TextureFree(sky.imported);C3D_TexDelete(&sky.texture);}
    if(characterTextures.imported) {Tex3DS_TextureFree(characterTextures.imported);C3D_TexDelete(&characterTextures.texture);}
    if(partnerTextures.imported) {Tex3DS_TextureFree(partnerTextures.imported);C3D_TexDelete(&partnerTextures.texture);}
    if(trioPartnerTexture.imported){Tex3DS_TextureFree(trioPartnerTexture.imported);C3D_TexDelete(&trioPartnerTexture.texture);}
    if(trioMarioTexture.imported){Tex3DS_TextureFree(trioMarioTexture.imported);C3D_TexDelete(&trioMarioTexture.texture);}
    if(harborMarioTexture.imported){Tex3DS_TextureFree(harborMarioTexture.imported);C3D_TexDelete(&harborMarioTexture.texture);}
    if(professorMarioTexture.imported) {Tex3DS_TextureFree(professorMarioTexture.imported);C3D_TexDelete(&professorMarioTexture.texture);}
    if(professorMapTexture.imported) {Tex3DS_TextureFree(professorMapTexture.imported);C3D_TexDelete(&professorMapTexture.texture);}
    if(undergroundTexture.imported){Tex3DS_TextureFree(undergroundTexture.imported);C3D_TexDelete(&undergroundTexture.texture);}
    if(messageIconTexture.imported) {Tex3DS_TextureFree(messageIconTexture.imported);C3D_TexDelete(&messageIconTexture.texture);}
    for(auto* texture:{&fontTexture,&harborTexture,&messageWindowTexture})if(texture->imported){Tex3DS_TextureFree(texture->imported);C3D_TexDelete(&texture->texture);}
    if(imported) Tex3DS_TextureFree(imported);
    if(atlasReady) C3D_TexDelete(&atlas);
    if(vertices) linearFree(vertices);
    if(messageTailVertices) linearFree(messageTailVertices);
    if(messageIndicatorVertices)linearFree(messageIndicatorVertices);
    if(messageIndicatorTexture.imported){Tex3DS_TextureFree(messageIndicatorTexture.imported);C3D_TexDelete(&messageIndicatorTexture.texture);}
    if(windowFrameVertices)linearFree(windowFrameVertices);
    if(messageSystemVertices)linearFree(messageSystemVertices);
    for(auto* texture:{&messagePanel,&messageSystemPattern})if(texture->imported){Tex3DS_TextureFree(texture->imported);C3D_TexDelete(&texture->texture);}
    for(auto& texture:messageSystemMasks)if(texture.imported){Tex3DS_TextureFree(texture.imported);C3D_TexDelete(&texture.texture);}
    if(messageSystemProgramReady)shaderProgramFree(&messageSystemProgram);
    if(messageSystemShader)DVLB_Free(messageSystemShader);
    if(programReady) shaderProgramFree(&program);
    if(shader) DVLB_Free(shader);
    if(top) C3D_RenderTargetDelete(top);
    if(bottom) C3D_RenderTargetDelete(bottom);
    if(c2dReady) C2D_Fini();
    if(c3dReady) C3D_Fini();
}
}
