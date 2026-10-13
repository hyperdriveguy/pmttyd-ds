#pragma once
#include "game.hpp"
#include "frontend.hpp"
#include "opening_demo.hpp"
#include "opening_sequence.hpp"
#include "visibility.hpp"
#include "uv_animation.hpp"
#include "actor_mesh.hpp"
#include "actor_animation.hpp"
#include "field_air_shadow.hpp"
#include "field_actors.hpp"
#include "npc_presentation.hpp"
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>
namespace ttyd {
struct Metrics { float simulationMs=0, renderMs=0, gpuMs=0, fps=0; int steps=0; uint64_t droppedTicks=0; };
class Renderer {
public:
    bool init();
    bool changeRoom(uint32_t room);
    void beginFrontend();
    void releaseFrontend();
    void render(const Game&, const Metrics&, const char* notice, bool audioAvailable);
    void renderFrontend(const Frontend&, const OpeningDemo&, const OpeningSequence&, const char* notice);
    void shutdown();
    struct ActorMetrics {uint32_t visible=0,vertices=0,overflow=0,sampleFailures=0;};
    ActorMetrics fieldActorMetrics() const {return actorMetrics;}
private:
    struct Vertex { float x,y,z,u,v,r,g,b,a; };
    Vertex* vertices=nullptr;
    Vertex* messageTailVertices=nullptr;
    int count=0, transformLoc=0, marioPose=0;
    shaderProgram_s program{};
    C3D_Mtx worldTransform{};
    DVLB_s* shader=nullptr;
    C3D_RenderTarget* top=nullptr;
    C3D_RenderTarget* bottom=nullptr;
    C3D_Tex atlas{};
    Tex3DS_Texture imported=nullptr;
    C2D_TextBuf textBuffer=nullptr;
    struct MapBatch { uint32_t start,count; int32_t texture; uint32_t layer,wrapS,wrapT; };
    struct Texture { C3D_Tex texture{}; Tex3DS_Texture imported=nullptr; };
    struct Occluder {Bounds3 bounds{};std::vector<uint32_t> batches;uint64_t hiddenUntil=0;bool hidden=false;};
    struct FieldActorModelGpu {
        uint32_t model=0;
        Texture texture;
        ActorAnimationPack animations;
        ActorAnimationSample sample;
        uint64_t tick=UINT64_MAX;
        float frame=-1;
        std::string pose;
        int start=0,count=0;
        std::vector<ActorSurfaceBatch> batches;
    };
    struct Room {
        struct NpcDrawState {NpcPresentation presentation;NpcPoseClock clock;};
        std::unordered_map<uint32_t,NpcDrawState> npcs;
        std::vector<MapBatch> batches;
        std::vector<Occluder> occluders;
        std::vector<Texture> textures;
        Vertex* vertices=nullptr;
        uint32_t vertexCount=0;
        std::vector<UVTrack> uvTracks;
        std::vector<MeshTrack> meshTracks;
        uint64_t animationStart=UINT64_MAX;
        std::vector<FieldActorModelGpu> actorModels;
        uint64_t actorStart=UINT64_MAX;
        std::vector<std::pair<uint32_t,Vertex>> fenceOriginal;
        CameraView fenceCaptureView{};
        std::array<float,4> fenceCapturePlane{};
        bool fenceCaptured=false;
        uint64_t fenceTick=UINT64_MAX;
    };
    Room plaza,stage,frontRoom,trioBackground,trioStage;
    std::string frontMapName;
    bool frontActive=false;
    uint32_t frontPhase=UINT32_MAX;
    std::string frontFailure;
    struct CineActor {std::string name;ActorAnimationPack animations;Texture image;float uvScale=1;};
    std::vector<CineActor> cineActors;
    bool frontWorld(const char*,CameraPoint,CameraPoint,const OpeningSequence*,const OpeningDemo&);
    struct FrontTexture {std::string name;Texture image;std::array<uint32_t,4> size{};};
    std::vector<FrontTexture> frontTextures;
    bool frontImage(const char*,float,float,float,float);
    void frontMessage(const MessagePlayback&);
    Texture sky,characterTextures,partnerTextures,fontTexture,harborTexture,messageWindowTexture;
    Texture messagePanel,messageSystemPattern;
    std::array<Texture,8> messageSystemMasks;
    struct SystemVertex {float x,y,z,u,v,s,t,r,g,b,a;};
    SystemVertex* messageSystemVertices=nullptr;
    DVLB_s* messageSystemShader=nullptr;shaderProgram_s messageSystemProgram{};
    bool messageSystemProgramReady=false;
    int messageSystemTransform=0;float messageSystemPhase=0;

    Texture harborMarioTexture;
    ActorAnimationPack harborMarioAnimations;
    float harborMarioUVScale=1;
    Texture professorMarioTexture;
    Texture trioMarioTexture;ActorAnimationPack trioMarioAnimations;float trioMarioUVScale=1;
    Texture professorMapTexture;
    Texture messageIconTexture;
    void messageIconImage(uint32_t id,uint64_t ticks,float centerX,float bottomY,float scale,uint8_t alpha);
    std::array<float,2> professorMapSize{};
    ActorAnimationPack professorMarioAnimations;
    ActorAnimationPack professorFenceAnimations;
    ActorAnimationSample professorFenceSample;
    ActorAnimationPack undergroundAnimations;Texture undergroundTexture;float undergroundUVScale=1;
    float professorMarioUVScale=1;
    FieldActorAssets fieldActorAssets;
    NpcAngleTable npcAngles;
    ActorMetrics actorMetrics;
    std::array<std::array<float,3>,2> harborActorMetrics{};
    std::array<std::array<float,3>,2> harborTalkOffsets{};
    mutable std::vector<float> messageDynamic;
    mutable std::unordered_map<uint64_t,std::vector<float>> messageDynamicWindows;
    mutable std::string messageIdentity;
    mutable uint64_t messageSerial=0;
    uint64_t messageWindowSerial=0;
    unsigned messagePlacement=0;
    CameraPoint messageTalkAnchor;
    uint32_t messageRng=0x6d736777; // Presentation RNG adapter, separate from mechanics.
    ActorAnimationPack actorAnimations,partnerAnimations;
    ActorAnimationSample actorSample;
    FieldShadowGeometry shadowGeometry;
    bool loadMap(const char* name,Room& room);
    bool loadFieldActors(uint32_t roomId,Room& room);
    void fieldActors(const Game&);
    void drawFieldActors(bool translucent);
    void freeRoom(Room& room);
    void drawMap(bool battle,const Game& game,bool translucent=false,Room* supplied=nullptr);
    void updateCutaways(Room& room,const Game& game);
    bool c3dReady=false,c2dReady=false,programReady=false,atlasReady=false;
    void world(const Game&);
    void overlay(const Game&);
    void menu(const Game&,const Metrics&,const char*,bool);
    void quad(Vec2 a,Vec2 b,float y0,float y1,uint32_t color);
    void box(float x,float z,float w,float d,float h,uint32_t color);
    void ground(float x0,float z0,float x1,float z1,float y,uint32_t color);
    void character(int index,Vec2 p,float height,float width,float facing,float lift=0,float tilt=0);
    bool characterModel(const char* pose,float frame,Vec2 root,float rootY,float yaw,bool reflectZ,bool companion=false,const ActorAnimationPack* custom=nullptr,float uvScale=1,const ProfessorSceneActor* battleActor=nullptr);
    void marioGroundShadow(const FieldShadowRecord&);
    void harborActors(const Game&);
    void originalMessage(const Game&,const MessagePlayback* supplied=nullptr);
    void messageBorder(const MessageWindowPlan&,uint8_t alpha,int type=0);
    int marioAirShadow(const FieldShadowRecord&);
    void shadow(Vec2 p,float radius,float floor=0);
    void vertex(float x,float y,float z,float u,float v,uint32_t color,const ActorColor* rgba=nullptr);
    void text(const char*,float x,float y,float scale,uint32_t color);
    void panel(float x,float y,float w,float h,uint32_t color);
    void button(const char*,float x,float y,float w,float h,bool active,bool disabled=false);
};
class Audio {
public:
    bool init();
    void play(const Game& game);
    void update(Scene scene, bool enabled,bool harborDanger=false,int professorMusic=0,uint32_t room=0);
    void updateFrontend(int track,bool enabled);
    void shutdown();
    bool available() const { return ready; }
private:
    bool ready=false;
    struct Clip { int16_t* data=nullptr; uint32_t samples=0; ndspWaveBuf wave{}; };
    Clip clips[6];
    FILE* music=nullptr;
    int musicTrack=-1;
    bool musicLoop=false,musicEnded=false;
    uint32_t musicRate=32000, musicChannels=2, musicPosition=0, loopStart=0, loopEnd=0;
    int16_t* streamData=nullptr;
    ndspWaveBuf stream[3]{};
    void switchTrack(int track);
    void fill(ndspWaveBuf& buffer);
};
}
