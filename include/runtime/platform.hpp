#pragma once
#include <atomic>
#include "item_get_windows.hpp"
#include "field_bgm_envelope.hpp"
#include "field_item_get_effect.hpp"
#include "field_item_registry.hpp"
#include "game.hpp"
#include "frontend.hpp"
#include "opening_house_objects.hpp"
#include "opening_ship_objects.hpp"
#include "opening_demo.hpp"
#include "opening_sequence.hpp"
#include "opening_house_pcm.hpp"
#include "visibility.hpp"
#include "uv_animation.hpp"
#include "actor_mesh.hpp"
#include "actor_animation.hpp"
#include "field_air_shadow.hpp"
#include "field_actors.hpp"
#include "npc_presentation.hpp"
#include "battle_curtain_fade.hpp"
#include "battle_startup_resources.hpp"
#include "window_frame.hpp"
#include "window_font.hpp"
#include <memory>
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
    const std::string& resourceFailure() const{return frontFailure;}
    void beginFrontend();
    void advanceTutorialAnimations(Game&);
    void releaseFrontend(bool preserveMapTurn=false,bool preserveTitle=false);
    bool prepareOpeningDeparture(const OpeningHarborScene&,uint64_t fieldTick);
    void render(const Game&, const Metrics&, const char* notice, bool audioAvailable);
    void renderFrontend(Frontend&, OpeningDemo&, OpeningSequence&, const char* notice);
    void shutdown();
    bool bindTutorialStartupResources(HarborTutorial&);
    void releaseTutorialStartupResources();
    struct ActorMetrics {uint32_t visible=0,vertices=0,overflow=0,sampleFailures=0;};
    ActorMetrics fieldActorMetrics() const {return actorMetrics;}
private:
    struct Vertex { float x,y,z,u,v,r,g,b,a; };
    Vertex* vertices=nullptr;
    Vertex* messageTailVertices=nullptr;
    Vertex* messageIndicatorVertices=nullptr;
    unsigned messageIndicatorCount=0;
    ActorAnimationPack messageIndicatorAnimations;
    ActorAnimationSample messageIndicatorSample;
    static constexpr unsigned MaxWindowFrames=64; // Native arena capacity, not an original manager limit.
    Vertex* windowFrameVertices=nullptr;
    unsigned windowFrameCount=0;
    bool sourceWindowFrame(const WindowFrameMesh&,bool onBottom=false);
    void battleHud(const Game&);
    void battleMenuRows(const Game&);
    static constexpr unsigned MaxWindowFontQuads=1024; // Native arena capacity.
    Vertex* windowFontVertices=nullptr;
    unsigned windowFontCount=0;
    uint32_t windowFontRng=0x77666e74; // Presentation adapter; original global rand owner remains open.
    bool sourceWindowFont(const char*,WindowFontConsumer,ItemSelectionTextDraw,const WindowFontStyle&,const ItemSelectionClip* clip=nullptr,const ColorWindowText* colorTokens=nullptr);
    void selectorClip(const ItemSelectionClip*);
    bool itemSelector(ItemSelectionPresentation&);
    bool itemGetWindows(ItemGetWindows&,uint32_t sourceFrameCounter);
    bool itemGetEffect(const FieldItemGetEffect6&,uint32_t cameraID,const C3D_Mtx& nativeWorldToClip,const CameraViewport& sourceViewport,float cameraYaw,uint32_t shadowRGB);
    bool prepareItemGetEffect();
    void releaseItemGetEffect();
    bool item95Icon(const FieldItemIcon&,uint32_t cameraID,const C3D_Mtx& nativeWorldToClip,const CameraViewport&,float cameraYaw,uint32_t sourceFrameCounter);
    void releaseItem95Icons();
    Vertex* item95IconVertices=nullptr;unsigned item95IconCount=0;


    bool selectorIcon(uint32_t,uint64_t,float,float,float,uint32_t,bool centered=false);
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
    Vertex* itemGetEffectVertices=nullptr;
    std::array<Texture,2> itemGetEffectTextures;
    DVLB_s* itemGetEffectShader=nullptr;shaderProgram_s itemGetEffectProgram{};
    bool itemGetEffectProgramReady=false;
    int itemGetEffectTransform=0,itemGetEffectProjection=0;
    bool prepareSceneActorTextures(const Game&);
    Texture messageIndicatorTexture;
    std::unique_ptr<BattleStartupResources> tutorialStartupResources;
    std::array<Texture,9> tutorialStartupTextures;
    HarborTutorial* tutorialStartupOwner=nullptr;
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
        uint64_t stageMapRevision=UINT64_MAX;
    };
    Room plaza,stage,frontRoom,trioBackground,trioStage,harborBattleBackground,harborBattleStage;
    std::string frontMapName;
    bool frontActive=false;
    uint32_t frontPhase=UINT32_MAX;
    std::string frontFailure;
    OpeningHouseObjects frontHouseObjects;
    OpeningShipObjects frontShipObjects;
    Room departureRoom;
    OpeningShipObjects departureObjects;
    void drawOpeningDeparture(const Game&,bool translucent);
    void freeOpeningDeparture();
    std::unique_ptr<Game> frontDrawSnapshot;
    struct CineActor {std::string name;ActorAnimationPack animations;Texture image;float uvScale=1;ActorAnimationSample sample;const ActorAnimationPose* sampledPose=nullptr;float sampledTime=-1;};
    std::vector<CineActor> cineActors;
    std::array<CineActor,2> titleCurtains;
    bool titleCurtainsReady=false,titleCurtainsAttempted=false;
    bool frontTitlePrompt(const OpeningTitle&);
    bool loadTitleCurtains();
    bool frontTitleCurtain(OpeningTitle&,uint32_t&,float&,float&);
    void freeTitleCurtains();
    CineActor headingCurtain;
    bool headingResourcesReady=false,headingResourcesAttempted=false;
    bool headingCurtainStarted=false;
    uint64_t headingCurtainStart=0,headingExitStart=UINT64_MAX;
    MessagePlayback headingSubtitle;
    C3D_Tex headingSubtitleTexture{};
    C3D_RenderTarget* headingSubtitleTarget=nullptr;
    bool headingSubtitleAllocated=false,headingSubtitleCaptured=false;
    float headingSubtitleWidth=0;
    bool loadHeadingResources();
    bool captureHeadingSubtitle();
    bool frontHeading(OpeningSequence&,bool&,bool&,bool&,bool&);
    void freeHeading();
    std::array<CineActor,3> tutorialActors;
    bool tutorialReady=false;
    BattleCurtainFade tutorialFade;
    CineActor tutorialCurtain;
    HarborTutorial* tutorialFadeOwner=nullptr;
    uint64_t tutorialFadeEpoch=0,tutorialFadeTick=UINT64_MAX;
    float tutorialFadeLoops=0;
    uint32_t tutorialFadeMaterial=0;
    bool tutorialCurtainAttempted=false,tutorialCurtainReady=false,tutorialCurtainLive=false;
    bool loadTutorialCurtain();
    void advanceTutorialCurtain(HarborTutorial&);
    void drawTutorialCurtain();
    void releaseTutorialCurtain();
    bool loadTutorialPresentation();
    void worldTutorial(const Game&);
    ActorAnimationPack shipPaperAnimations;
    ActorAnimationSample shipPaperSample;
    bool shipPaperReady=false,shipPaperAttempted=false,shipCaptureReady=false;
    C3D_Tex shipCaptureTexture{};
    C3D_RenderTarget* shipCaptureTarget=nullptr;
    uint8_t* shipCapturePixels=nullptr;
    bool loadShipPaper();
    bool captureOpeningShip(const OpeningSequence&);
    bool frontVoyageLogo(const OpeningVoyageLogo&);
    bool frontShipPaper(const OpeningSequence&,float&);
    void freeShipCapture();
    ActorAnimationPack demoPaperAnimations;
    std::array<ActorAnimationSample,4> demoPaperSamples;
    std::array<float,4> demoPaperFrames{{-1,-1,-1,-1}};
    struct DemoPaperSurface {uint32_t capture,pass,cull;};
    std::vector<DemoPaperSurface> shipPaperSurfaces;
    std::array<DemoPaperSurface,124> demoPaperSurfaces{};
    bool demoPaperReady=false,demoPaperAttempted=false;
    std::array<C3D_Tex,4> bookCaptureTextures{};
    std::array<bool,4> bookCaptureReady{};
    uint8_t* bookCapturePixels=nullptr;
    bool captureBookPage(OpeningDemo&,unsigned);
    void freeBookCaptures();
    ActorAnimationPack mapTurnAnimations;ActorAnimationSample mapTurnSample;
    C3D_Tex mapTurnCapture{};C3D_RenderTarget* mapTurnTarget=nullptr;
    bool mapTurnReady=false;Vertex* mapTurnVertices=nullptr;
    bool prepareMapTurn(OpeningMapTransition&);
    bool frontMapTurn(OpeningMapTransition&,unsigned,float&);
    void freeMapTurn();
    C3D_Tex wakeCaptureTexture{};
    bool wakeCaptureReady=false;
    C3D_RenderTarget* wakeWorldTarget=nullptr;
    bool prepareWakeWorld();
    Vertex* wakeEffectVertices=nullptr;
    bool frontWakeEffects(const OpeningWake&);
    void freeWakeEffects();
    std::array<Texture,2> demoEffectTextures;
    bool demoEffectsReady=false;
    bool prepareDemoEffects();
    bool frontDemoEffects(const DemoChestEffects&,CameraPoint,CameraPoint);
    void freeDemoEffects();
    bool loadDemoPaper();
    bool frontBookCover(const OpeningDemo&);
    bool frontBookPaper(OpeningDemo&,std::array<float,4>&);
    bool prepareFrontWorld(const char*,const OpeningSequence*,const OpeningDemo&);
    bool frontWorld(const char*,CameraPoint,CameraPoint,const OpeningSequence*,const OpeningDemo&);
    struct FrontTexture {std::string name;Texture image;std::array<uint32_t,4> size{};};
    std::vector<FrontTexture> frontTextures;
    bool frontImage(const char*,float,float,float,float,uint8_t alpha=255);
    void frontMessage(const MessagePlayback&,uint64_t ticks);
    Texture sky,characterTextures,partnerTextures,fontTexture,harborTexture,messageWindowTexture;
    Texture messagePanel,messageSystemPattern;
    std::array<Texture,8> messageSystemMasks;
    struct SystemVertex {float x,y,z,u,v,s,t,r,g,b,a;};
    SystemVertex* titlePromptVertices=nullptr;
    SystemVertex* messageSystemVertices=nullptr;
    DVLB_s* messageSystemShader=nullptr;shaderProgram_s messageSystemProgram{};
    bool messageSystemProgramReady=false;
    int messageSystemTransform=0;float messageSystemPhase=0;

    Texture harborMarioTexture;
    ActorAnimationPack harborMarioAnimations;
    Texture goombaQuestTexture;ActorAnimationPack goombaQuestAnimations;float goombaQuestUVScale=1;
    Texture fieldItemGetTexture;ActorAnimationPack fieldItemGetAnimations;float fieldItemGetUVScale=1;
    float harborMarioUVScale=1;
    Texture professorMarioTexture;
    Texture trioMarioTexture;ActorAnimationPack trioMarioAnimations;float trioMarioUVScale=1;
    Texture trioPartnerTexture;ActorAnimationPack trioPartnerAnimations;float trioPartnerUVScale=1;
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
    bool prepareBattleMaps(const Game&);
    uint32_t residentFieldRoom=0; // Native GPU residency, separate from simulation area state.
    const StageMapObjects* updateStageMap(const Game&,Room&);
    bool loadFieldActors(uint32_t roomId,Room& room);
    void fieldActors(const Game&);
    void drawFieldActors(bool translucent);
    void freeRoom(Room& room);
    void drawMap(bool battle,const Game& game,bool translucent=false,Room* supplied=nullptr,uint32_t sceneVisibleGroups=UINT32_MAX,bool alphaCapture=false);
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
    void originalMessage(const Game&,const MessagePlayback* supplied=nullptr,
                         const CameraPoint* speakerAnchor=nullptr,const CameraPoint* marioAnchor=nullptr);
    void messageAdvanceIndicator(const MessageWindowPlan&,uint64_t ticks,bool onBottom=false);
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
    void update(Scene scene, bool enabled,bool harborDanger=false,int professorMusic=0,uint32_t room=0,bool thinking=false);
    void updateFrontend(int track,bool enabled);
    void updateFrontend(const OpeningDemo&,bool enabled);
    void updateFrontend(const OpeningTitle&,bool enabled);
    bool updateOpeningHousePcm(const OpeningHouseSounds*,bool enabled);
    void updateFrontend(const OpeningSequence&,bool enabled);
    void shutdown();
    bool available() const { return ready; }
    bool frontendTrackReady(int track);
    bool keyItemFanfareReady();
    bool beginKeyItemFanfare(int32_t gpRate=60); // Fresh native GP60 default; replacement remains external.
    int32_t keyItemFanfareCheck(); // Source query shape: -1 vacant, 0 assigned.
    void releaseKeyItemFanfare(); // Backend release, not source Off201 fade.
    bool fadeKeyItemFanfare(int32_t milliseconds,int32_t gpRate);
    bool tickKeyItemAudio(int32_t gpRate); // One admitted group0/1 BGMMain then SoundSSMain invocation.
    bool retainItemGetFieldMusic(uint8_t sourceBase,int8_t sourceGlobal,bool sourceBypassGlobal);
    bool itemGetFieldMusicSilent();
    bool muteItemGetFieldMusic(int32_t gpRate); // Original Off3800/1000ms.
    bool resumeItemGetFieldMusic(int32_t gpRate); // Original Ona0,NULL/1000ms and synchronous BGMMain.

private:
    bool ready=false,titleResourceAttempted=false,titleResourceReady=false;
    OpeningHousePcmPlayer openingHousePcm;
    struct Clip { int16_t* data=nullptr; uint32_t samples=0; ndspWaveBuf wave{}; };
    Clip clips[8];
    Clip openingMapSound;uint32_t openingMapSoundRate=0;bool openingMapSoundAttempted=false;
    void updateOpeningMapTurn(const OpeningSequence&,bool);
    struct MusicStream {
        FILE* file=nullptr;
        int track=-1;
        bool loop=false,ended=false;
        uint32_t rate=32000,channels=2,position=0,loopStart=0,loopEnd=0;
        ndspWaveBuf buffers[3]{};
    };
    MusicStream music[3];
    bool keyItemResourceAttempted=false,keyItemResourceReady=false;
    int keyItemSlot=-1,keyItemTailSlot=-1;
    FieldBgmEnvelope keyItemEnvelope;
    FieldStreamFade keyItemTail;
    int itemGetFieldSlot=-1,itemGetFieldTrack=-1;
    FieldBgmEnvelope itemGetFieldEnvelope;
    bool itemGetBgmMain(int32_t gpRate); // Caller holds musicMutex; source group order0 then1.
    void retireKeyItemFanfare(); // Caller holds musicMutex.
    unsigned musicSlot=0;
    int musicTrack=-1;
    bool switchSoundtrack=false;
    int16_t* streamData=nullptr;
    void stopMusic(unsigned slot);
    void switchTrack(int track,bool preservePrevious=false);
    void startMusic(unsigned slot,int track,uint8_t volume=127);
    void frontendMusic(const std::array<int,3>& tracks,const std::array<uint8_t,3>& volumes);
    void fill(MusicStream&,unsigned slot,ndspWaveBuf&);
    void musicMix(unsigned slot,uint8_t volume);
    LightLock musicMutex;LightEvent musicWake;Thread musicThread=nullptr;
    std::atomic<bool> musicWorkerRun{false};
    void serviceMusic();
};
}
