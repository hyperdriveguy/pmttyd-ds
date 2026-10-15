#include "vertex_upload_range.hpp"
#include "platform.hpp"
#include "opening_book_view.hpp"
#include "message_icons.hpp"
#include <algorithm>
#include <cstdio>
#include <cmath>
namespace ttyd {
namespace {constexpr int MaxVertices=12000;
constexpr auto FieldRGB=GPU_WRITEMASK(GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE);
constexpr auto FieldRGBDepth=GPU_WRITEMASK(FieldRGB|GPU_WRITE_DEPTH);
constexpr uint32_t Ink=0xff352d29,Paper=0xffdff2fb,Red=0xff4966d8;
uint32_t rgb(unsigned r,unsigned g,unsigned b,unsigned a=255){return C2D_Color32(r,g,b,a);}}
// Frontend owns resource admission before its scene clock/music start. GPU
// sync precedes retiring field/battle/departure allocations. Title assets may
// remain resident through the attract demo to avoid a later admission stall.
void Renderer::beginFrontend(){
    C3D_FrameSync();freeOpeningDeparture();
    for(auto* room:{&plaza,&stage,&trioBackground,&trioStage})freeRoom(*room);
    // Admit Title before the opening clock/music starts. Preserve these small
    // owners across the attract demo so its white exit performs no cold import.
    for(const char* name:{"title-background","title-logo","title-copyright","title-prompt","title-prompt-pattern"})
        if(!frontImage(name,0,0,0,0)){frontFailure="Title image admission failed";break;}
    if(!loadTitleCurtains())frontFailure="Title curtain admission failed";
    frontActive=true;
}
// preserveMapTurn/preserveTitle retain explicitly borrowed transition assets.
// Other scene allocations retire only after synchronization. Texture wrapper
// copies do not clone GPU handles; cinematic banks need explicit deletion.
void Renderer::releaseFrontend(bool preserveMapTurn,bool preserveTitle){
    C3D_FrameSync();if(!preserveMapTurn)freeMapTurn();freeHeading();if(!preserveTitle)freeTitleCurtains();freeBookCaptures();freeWakeEffects();freeShipCapture();freeDemoEffects();freeRoom(frontRoom);frontMapName.clear();frontHouseObjects.clear();frontShipObjects.clear();frontDrawSnapshot.reset();
    for(auto i=frontTextures.begin();i!=frontTextures.end();){
        if(preserveTitle && i->name.rfind("title-",0)==0){++i;continue;}
        if(i->image.imported){Tex3DS_TextureFree(i->image.imported);C3D_TexDelete(&i->image.texture);}
        i=frontTextures.erase(i);
    }
    for(auto& a:cineActors)if(a.image.imported){Tex3DS_TextureFree(a.image.imported);C3D_TexDelete(&a.image.texture);}
    cineActors.clear();
}
// Native .size sidecar is four uint32 values: original W/H, padded W/H.
// Nonzero dimensions<=1024 must fit padded backing. Zero draw size only admits
// resources. Cropped UVs preserve original pixels; title textures request VRAM.
bool Renderer::frontImage(const char* name,float x,float y,float w,float h,uint8_t alpha) {
    if(frontTextures.empty())frontTextures.reserve(32); // C2D retains texture pointers until flush.
    auto found=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& f){return f.name==name;});
    if(found==frontTextures.end()) {
        FrontTexture pending;pending.name=name;char path[128];
        std::snprintf(path,sizeof(path),"romfs:/%s.size",name);FILE* meta=std::fopen(path,"rb");if(!meta)return false;
        bool ok=std::fread(pending.size.data(),4,4,meta)==4 && std::fgetc(meta)==EOF;std::fclose(meta);
        const auto& z=pending.size;if(!ok || !z[0] || !z[1] || z[0]>z[2] || z[1]>z[3] || z[2]>1024 || z[3]>1024)return false;
        std::snprintf(path,sizeof(path),"romfs:/%s.t3x",name);FILE* f=std::fopen(path,"rb");if(!f)return false;
        pending.image.imported=Tex3DS_TextureImportStdio(f,&pending.image.texture,nullptr,std::strncmp(name,"title-",6)==0);std::fclose(f);
        if(!pending.image.imported)return false;
        C3D_TexSetFilter(&pending.image.texture,GPU_LINEAR,GPU_LINEAR);
        frontTextures.push_back(std::move(pending));found=frontTextures.end()-1;
    }
    const auto& z=found->size;Tex3DS_SubTexture sub{};sub.width=z[0];sub.height=z[1];
    sub.left=0;sub.right=float(z[0])/z[2];sub.top=1;sub.bottom=1-float(z[1])/z[3];
    // Keep original RGB; tint strength1 replaces it with white. Alpha still
    // modulates the source image for scene fades at strength0.
    if(w>0 && h>0){C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(255,255,255,alpha),0);C2D_DrawImageAt({&found->image.texture,&sub},x,y,0,&tint,w/z[0],h/z[1]);}
    return true;
}
// Bottom-screen source text uses half scale at origin(16, 58), clipped to
// Y54..176. Font UVs address 512x256. Record0xfff7 changes source RGBA;
// this helper does not turn non-glyph records into invented dialogue.
void Renderer::frontMessage(const MessagePlayback& message,uint64_t ticks) {
    if(!message.active())return;
    const auto& font=harborMessageFont();
    uint32_t color=0x000000ff;
    for(size_t i=0;i<std::min(message.cursor,message.records.size());++i) {
        const auto& r=message.records[i];if(r.code==0xfff7){color=r.flags;continue;}if(r.code>=font.glyphs.size())continue;
        const auto& glyph=font.glyphs[r.code];const float scale=.5f*r.scale;
        const float x=16+r.x*.5f,y=58-(r.y+message.scroll)*.5f;
        if(y<54 || y+glyph.height*scale>176)continue;
        Tex3DS_SubTexture sub{};sub.width=glyph.width;sub.height=glyph.height;
        sub.left=glyph.x/512.f;sub.right=(glyph.x+glyph.width)/512.f;
        sub.top=1-glyph.y/256.f;sub.bottom=1-(glyph.y+glyph.height)/256.f;
        C2D_ImageTint tint;C2D_PlainImageTint(&tint,rgb(color>>24,(color>>16)&255,(color>>8)&255,message.alpha),1);
        C2D_DrawImageAt({&fontTexture.texture,&sub},x,y,0,&tint,scale,scale);
    }
    if(message.canAdvanceText()) {
        MessageWindowPlan plan;plan.box={-368,132,576,244}; // Current bottom text area:16,54..304,176.
        messageAdvanceIndicator(plan,ticks,true);
    }
}
// Native map residency is separate from story state. Retire prior room
// before allocation to bound linear heap; load failure does not restore its GPU
// resources. Do not add FrameSync inside the active first map frame.
bool Renderer::prepareFrontWorld(const char* map,const OpeningSequence* opening,const OpeningDemo& demo) {
    if(frontMapName!=map) {
        // A cutscene owns one map. Release the preceding scene before staging
        // its successor; retaining both can exhaust the native linear arena.
        // renderFrontend already synchronized before starting this frame.
        // A second FrameSync inside the active first map frame can wait for
        // a presentation that this unfinished frame has not submitted.
        freeRoom(frontRoom);frontMapName.clear();frontHouseObjects.clear();frontShipObjects.clear();
        if(!opening && demo.book.adjusting){
            // The original prior page owners are finished. Keep only the book
            // layers used by final adjust while admitting the marketplace.
            for(auto i=frontTextures.begin();i!=frontTextures.end();){
                const bool keep=i->name.rfind("title-",0)==0 || i->name=="demo-0" || i->name=="demo-1" || i->name=="demo-2" || i->name=="demo-7" || i->name=="demo-12" || i->name=="demo-page-back";
                if(keep){++i;continue;}
                if(i->image.imported){Tex3DS_TextureFree(i->image.imported);C3D_TexDelete(&i->image.texture);}
                i=frontTextures.erase(i);
            }
            for(unsigned n=0;n<4;++n)if(bookCaptureReady[n] && !demo.book.pages[n].active){C3D_TexDelete(&bookCaptureTextures[n]);bookCaptureReady[n]=false;}
        }
        // Phase ownership releases image textures. The source final book
        // adjustment shares its image resources with the newly visible market.
        Room pending;if(!loadMap(map,pending)){freeRoom(pending);if(frontFailure.empty())frontFailure=std::string("Scene map load failed: ")+map;return false;}
        frontRoom=std::move(pending);
        if(std::string(map)=="opening-aaa_00" && !frontHouseObjects.load("romfs:/opening-aaa_00.houseobj",frontRoom.vertexCount)) {
            freeRoom(frontRoom);frontFailure="Original house object bindings unavailable";return false;
        }
        if((std::string(map)=="opening-gor_00" || std::string(map)=="opening-muj_20") && !frontShipObjects.load((std::string("romfs:/")+map+".shipobj").c_str(),frontRoom.vertexCount)){freeRoom(frontRoom);frontFailure="Original harbor ship bindings unavailable";return false;}
        frontMapName=map;frontFailure.clear();
    }
    return true;
}
bool Renderer::frontWorld(const char* map,CameraPoint eye,CameraPoint at,const OpeningSequence* opening,const OpeningDemo& demo) {
    if(opening && opening->part==OpeningPart::Wake && !wakeWorldTarget)return false;
    if(!prepareFrontWorld(map,opening,demo))return false;
    // bgDisp owns a textured background before map geometry. The source
    // requested dmo_00 keeps its own backdrop despite the gor_01 geometry alias.
    const char* background=std::string(map)=="opening-demo-market"?"dmo_00":
        std::string(map)=="opening-gor_00"?"gor_00":
        std::string(map)=="opening-gor_10"?"gor_10":
        std::string(map)=="opening-aaa_00" || std::string(map)=="opening-aaa_00-interior"?"aaa_00":nullptr;
    if(background)C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
    if(background && !frontImage((std::string("opening-bg-")+background).c_str(),48,0,304,240)) {
        frontFailure=std::string("Original scene background unavailable: ")+background;return false;
    }
    VertexUploadRanges upload;
    auto writeObjectVertex=[&](uint32_t i,const std::array<float,3>& p) {
        auto& vertex=frontRoom.vertices[i];
        vertex.x=p[0];vertex.y=p[1];vertex.z=p[2];upload.include(i);
    };
    if(opening && frontHouseObjects.size()) {
        frontHouseObjects.apply(opening->house.doorAngle,opening->house.postboxX,opening->house.treeX,writeObjectVertex);
    }
    if(opening && opening->part==OpeningPart::Arrival){const auto& ship=opening->harbor;
        frontShipObjects.apply(ship.x,ship.y,ship.wakeX,ship.yaw,ship.rock,ship.parts,writeObjectVertex);
    }
    if(opening && opening->part==OpeningPart::Voyage){const auto& ship=opening->voyage;
        frontShipObjects.applyVoyage(ship.position,ship.yaw,ship.rotationZ,ship.parts,writeObjectVertex);
    }
    upload.flush([&](uint32_t first,uint32_t count){
        GSPGPU_FlushDataCache(frontRoom.vertices+first,size_t(count)*sizeof(Vertex));
    });
    // Original actor hierarchy evaluation needs the native stack; keep the
    // large, renderer-only map snapshot in scene-owned heap storage.
    if(!frontDrawSnapshot)frontDrawSnapshot=std::make_unique<Game>();
    // Renderer-only room255 bypasses ordinary field-room gates. Millisecond
    // scene time becomes 60Hz sample ticks; this snapshot never runs simulation.
    auto& snapshot=*frontDrawSnapshot;snapshot.progress.room=255;snapshot.ticks=(opening?opening->now:demo.now)*60/1000;
    snapshot.fieldCameraFrame.eye={eye.x*.01f,eye.y*.01f,eye.z*.01f};snapshot.fieldCameraFrame.at={at.x*.01f,at.y*.01f,at.z*.01f};
    C3D_Mtx projection,view;
    // camInit/camLoadRoad retain the original camera4 lens25; native aspect
    // adapts the viewport without changing the authored eye/aim framing.
    const bool wakeWorld=opening && opening->part==OpeningPart::Wake;
    if(wakeWorld)Mtx_Persp(&projection,C3D_AngleFromDegrees(25),C3D_AspectRatioTop,.01f,327.68f,false);
    else Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(25),C3D_AspectRatioTop,.01f,327.68f,false);
    Mtx_LookAt(&view,FVec3_New(eye.x*.01f,eye.y*.01f,eye.z*.01f),FVec3_New(at.x*.01f,at.y*.01f,at.z*.01f),FVec3_New(0,1,0),false);
    if(opening && opening->part==OpeningPart::Wake)Mtx_Translate(&view,0,opening->wake.viewShiftY*.01f,0,false);
    Mtx_Multiply(&worldTransform,&projection,&view);
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    // Packed event-group mask: bit 0 background/land, bit 1 ship, bit 13 wake.
    // Voyage admits 0/1; arrival exposes ship/wake only outside live paper capture.
    // UINT32_MAX delegates ordinary visibility to the room/source state.
    const uint32_t visibleGroups=opening && opening->part==OpeningPart::Voyage?3u:opening && opening->part==OpeningPart::Arrival?1u | (opening->harbor.visible && !opening->harbor.paperLive && !opening->harbor.paperCapturePending?2u:0u) | (opening->harbor.visible && !opening->harbor.paperLive && !opening->harbor.paperCapturePending?(1u<<13):0u):!opening && demo.bookComplete?demo.market.visibleMapGroups():UINT32_MAX;
    if(opening && opening->part==OpeningPart::Arrival && opening->harbor.paperCapturePending){
        if(!shipCaptureTarget)shipCaptureTarget=C3D_RenderTargetCreate(240,400,GPU_RB_RGBA8,GPU_RB_DEPTH24_STENCIL8);
        if(!shipCaptureTarget || !loadShipPaper()){frontFailure="Original ship capture unavailable";return false;}
        C3D_RenderTargetClear(shipCaptureTarget,C3D_CLEAR_ALL,0,0);C3D_FrameDrawOn(shipCaptureTarget);
        drawMap(false,snapshot,false,&frontRoom,2u,true);drawMap(false,snapshot,true,&frontRoom,2u,true);C3D_FrameDrawOn(top);
    }
    // Matched original fresh-file Voyage exposes EFB rows48..431. Keep the
    // original full-height projection; crop world only before the logo overlay.
    const bool voyageLetterbox=opening && opening->part==OpeningPart::Voyage;
    if(wakeWorld){C3D_SetViewport(0,0,400,240);C3D_SetScissor(GPU_SCISSOR_NORMAL,48,0,352,240);}
    else C3D_SetScissor(GPU_SCISSOR_NORMAL,voyageLetterbox?24:0,48,voyageLetterbox?216:240,352);
    drawMap(false,snapshot,false,&frontRoom,visibleGroups);
    count=0; // Each queued actor retains its own range until frame submission.
    if(cineActors.empty())cineActors.reserve(4);
    // Each cinematic bank retains a sample keyed by pose pointer+selected
    // authored time. Negative frame selects snapshot tick; UV scales1/2/4/8
    // come from exact-EOF sidecars and compensate converted atlas padding.
    auto actor=[&](const char* bank,const char* pose,CameraPoint root,float yaw,float frame=-1,const NpcPresentation* facing=nullptr) {
        auto found=std::find_if(cineActors.begin(),cineActors.end(),[&](const auto& a){return a.name==bank;});
        if(found==cineActors.end()) {
            CineActor pending;pending.name=bank;const std::string base=std::string("romfs:/")+bank;
            if(!pending.animations.load((base+".pmanim").c_str())){frontFailure=std::string("Scene animation load failed: ")+bank;return false;}
            FILE* uv=std::fopen((base+".uvscale").c_str(),"rb");if(!uv)return false;
            bool ok=std::fread(&pending.uvScale,4,1,uv)==1 && std::fgetc(uv)==EOF;std::fclose(uv);
            if(!ok || !(pending.uvScale==1 || pending.uvScale==2 || pending.uvScale==4 || pending.uvScale==8))return false;
            FILE* f=std::fopen((base+".t3x").c_str(),"rb");if(!f)return false;
            pending.image.imported=Tex3DS_TextureImportStdio(f,&pending.image.texture,nullptr,false);std::fclose(f);if(!pending.image.imported){char error[128];std::snprintf(error,sizeof(error),"Scene texture failed: %s (%lu KB free)",bank,(unsigned long)(linearSpaceFree()/1024));frontFailure=error;return false;}
            C3D_TexSetFilter(&pending.image.texture,GPU_LINEAR,GPU_LINEAR);cineActors.push_back(std::move(pending));found=cineActors.end()-1;
        }
        const int first=count;
        const auto* animation=found->animations.pose(pose);
        if(!animation)return false;
        float selected=animation->start;const float time=frame<0?float(snapshot.ticks):frame,length=animation->end-animation->start;
        if(length!=0 && time>=animation->start)selected=animation->loop?time-std::trunc((time-animation->start)/length)*length:std::min(time,animation->end);
        if(found->sampledPose!=animation || found->sampledTime!=selected) {
            if(!animation->evaluate(time,found->sample))return false;
            found->sampledPose=animation;found->sampledTime=selected;
        }
        const auto& sample=found->sample;
        if(sample.vertices.size()>size_t(MaxVertices-count))return false;
        const ActorRootTransform transform({root.x*.01f,root.y*.01f,root.z*.01f},yaw,false);
        for(size_t n=0;n<sample.vertices.size();++n) {
            const auto& v=sample.vertices[n];const auto point=facing?facing->point(v):transform.point(v);
            vertex(point[0],point[1],point[2],v.u,1-(1-v.v)*found->uvScale,0xffffffff,sample.colors.empty()?nullptr:&sample.colors[n]);
        }
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        if(count>first)GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));
        C3D_TexBind(0,&found->image.texture);
        if(sample.batches.empty())C3D_DrawArrays(GPU_TRIANGLES,first,count-first);
        else for(const auto& batch:sample.batches) {
            if(!batch.count || batch.cull==2)continue;
            // Source cull2 suppresses draw; 0/1 select front/back, 3 neither.
            // Pass3 blends without depth writes; pass2 alpha cutoff128 retains
            // opaque paper pixels. Field RGB-only writes preserve shadow alpha.
            const GPU_CULLMODE culls[]={GPU_CULL_FRONT_CCW,GPU_CULL_BACK_CCW,GPU_CULL_NONE,GPU_CULL_NONE};
            C3D_CullFace(culls[batch.cull]);C3D_DepthTest(true,GPU_GEQUAL,batch.pass==3?FieldRGB:FieldRGBDepth);
            C3D_AlphaTest(batch.pass==2,GPU_GEQUAL,128);
            const bool opaque=batch.pass==1 || batch.pass==2;
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,opaque?GPU_ONE:GPU_SRC_ALPHA,opaque?GPU_ZERO:GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,opaque?GPU_ZERO:GPU_ONE_MINUS_SRC_ALPHA);
            C3D_DrawArrays(GPU_TRIANGLES,first+batch.start,batch.count);
        }
        C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,FieldRGBDepth);C3D_AlphaTest(true,GPU_GREATER,8);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
        return true;
    };
    bool ok=true;
    if(!opening && demo.bookComplete) {
        const auto& scene=demo.market;constexpr const char* banks[]={"c_peach","c_nazo","demo-chest"};
        // Retained original speaker tokens associate Peach/stranger with
        // their mouth poses. Zero is the chest, not an invented speaker ID.
        constexpr uint32_t speakers[]={0x2040,0x2048,0};
        for(unsigned n=0;n<3;++n)if(scene.actors[n].visible) {
            const auto& a=scene.actors[n];const bool talking=demo.message.active() && !demo.message.waiting() && demo.speaker==speakers[n];
            ok=actor(banks[n],scene.actorPose(n,talking),{a.position[0],a.position[1],a.position[2]},a.yaw,float(demo.now*60/1000-a.poseTick),n<2?&scene.facing[n]:nullptr) && ok;
        }
    } else if(opening && (opening->part==OpeningPart::House || opening->part==OpeningPart::HouseReturn || opening->part==OpeningPart::MapDisplay)) {
        const auto& scene=opening->house;
        // Source NPC IDs: Luigi 0x474, Parakarry 0x468. Index0 is Mario and
        // uses characterModel below; these tokens are not native pointers.
        constexpr uint32_t keys[]={0,0x474,0x468};
        for(unsigned n=1;n<3;++n)if(scene.actors[n].visible) {
            const auto& a=scene.actors[n];const bool talking=opening->message.active() && !opening->message.waiting() && opening->speaker==keys[n];
            const bool right=a.yaw>0 && a.yaw<180;
            ok=actor(n==1?"c_luigi":"c_paretta",scene.actorPose(n,talking),{a.position[0],a.position[1],a.position[2]},right?180:0,float(opening->now*60/1000-a.poseTick)) && ok;
        }
        if(scene.actors[0].visible) {
            const auto& a=scene.actors[0];const bool right=a.yaw>0 && a.yaw<180;
            const int first=count;ok=characterModel(scene.actorPose(0,false),float(opening->now*60/1000-a.poseTick),{a.position[0]*.01f,a.position[2]*.01f},a.position[1]*.01f,right?180:0,right) && ok;
            auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);if(count>first)GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));C3D_TexBind(0,&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,first,count-first);
        }
    } else if(opening && opening->part==OpeningPart::Arrival && opening->harbor.marioVisible) {
        const int first=count;characterModel("M_S_1",float(snapshot.ticks),{0,2.25f},.15f,180,false);
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);if(count>first)GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));C3D_TexBind(0,&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,first,count-first);
    }
    if(opening && opening->part==OpeningPart::MapDisplay && opening->mapDisplay.poseStage==1){
        // Original icon 314 is billboarded relative to Luigi at engine offset
        //(-25,+15,-3). Retain quantized scale .5498046875 and source factor
        // .60000002384;*.01 converts engine geometry to native world units.
        const auto* icon=messageIcon(314);
        if(!icon)ok=false;
        else if(count+6<=MaxVertices){
            const auto& frame=icon->frame(0);const auto& luigi=opening->house.actors[1];
            const CameraPoint center{(luigi.position[0]-25)*.01f,(luigi.position[1]+15)*.01f,(luigi.position[2]-3)*.01f};
            const float dx=eye.x-at.x,dz=eye.z-at.z,length=std::sqrt(dx*dx+dz*dz);
            const float rx=length>0?dz/length:1,rz=length>0?-dx/length:0;
            constexpr float scale=.5498046875f*.01f;
            const float left=-frame.width*.5f*.60000002384f*scale,right=-left;
            const float high=frame.height*.60000002384f*scale,low=-high;
            const int first=count;
            auto put=[&](float x,float y,float u,float v){vertices[count++]={center.x+x*rx,center.y+y,center.z+x*rz,u,v,1,1,1,1};};
            put(left,low,frame.u0,frame.v1);put(right,low,frame.u1,frame.v1);put(right,high,frame.u1,frame.v0);
            put(left,low,frame.u0,frame.v1);put(right,high,frame.u1,frame.v0);put(left,high,frame.u0,frame.v0);
            auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
            GSPGPU_FlushDataCache(vertices+first,6*sizeof(Vertex));C3D_TexBind(0,&messageIconTexture.texture);C3D_DrawArrays(GPU_TRIANGLES,first,6);
        }else ok=false;
    }
    drawMap(false,snapshot,true,&frontRoom,visibleGroups);
    if(!opening && demo.bookComplete)ok=frontDemoEffects(demo.market.effects,eye,at) && ok;
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);
    C2D_Prepare();C2D_SceneBegin(wakeWorld?wakeWorldTarget:top);
    if(wakeWorld){C2D_SceneSize(400,240,false);C3D_SetViewport(0,0,400,240);}
    return ok;
}
// Scene submission owns completion acknowledgements. Offscreen captures and
// main presentation share this frame; successful CPU draw generation alone is
// insufficient to advance a scene. Negative candidate frames mean not drawn.
void Renderer::renderFrontend(Frontend& f,OpeningDemo& demo,OpeningSequence& opening,const char* notice) {
    // Keep only the current demonstration image/scene family resident. This
    // also bounds a natural (unskipped) storybook run and letter/map overlays.
    const uint32_t phase=f.screen==FrontScreen::Demo?100:
        f.screen==FrontScreen::Opening?200+uint32_t(opening.part==OpeningPart::MapDisplay?OpeningPart::HouseReturn:opening.part):0;
    if(phase!=frontPhase){releaseFrontend(f.screen==FrontScreen::Opening && opening.part==OpeningPart::Voyage && opening.mapTransition.entryRequested && !opening.mapTransition.entryComplete,f.screen==FrontScreen::Demo || f.screen==FrontScreen::Title || f.screen==FrontScreen::Files);frontPhase=phase;}
    if(opening.mapTransition.entryComplete && mapTurnReady){C3D_FrameSync();freeMapTurn();}
    const bool mapWorldReady=!(f.screen==FrontScreen::Opening && opening.part==OpeningPart::Voyage && opening.mapTransition.entryRequested && !opening.mapTransition.entryComplete) || prepareFrontWorld("opening-muj_20",&opening,demo);
    const bool mapCapture=f.screen==FrontScreen::Opening && opening.part==OpeningPart::MapDisplay && opening.mapTransition.requested && !opening.mapTransition.captureReady;
    const bool mapCaptureReady=!mapCapture || prepareMapTurn(opening.mapTransition);
    if(f.screen==FrontScreen::Demo && demo.bookComplete && !demo.book.adjusting &&
       std::any_of(frontTextures.begin(),frontTextures.end(),[](const auto& texture){return texture.name.rfind("demo-",0)==0;})) {
        // Final adjust retires the book alone. The already admitted market,
        // actor samples and map animation clocks continue across this boundary.
        // Synchronize before releasing texture pointers retained by the GPU.
        C3D_FrameSync();freeBookCaptures();
        for(auto texture=frontTextures.begin();texture!=frontTextures.end();) {
            if(texture->name.rfind("demo-",0)!=0){++texture;continue;}
            if(texture->image.imported){Tex3DS_TextureFree(texture->image.imported);C3D_TexDelete(&texture->image.texture);}
            texture=frontTextures.erase(texture);
        }
    }
    // Stage the marketplace and its effect textures while the preceding book
    // frame is still displayed. Chest opening must never perform first-use IO.
    const bool marketReady=f.screen!=FrontScreen::Demo || !demo.bookComplete ||
        (prepareFrontWorld("opening-demo-market",nullptr,demo) && prepareDemoEffects());
    std::array<bool,4> captureRequested{};
    std::array<C3D_RenderTarget*,4> bookTargets{};
    if(f.screen==FrontScreen::Demo && !demo.bookComplete && !demo.failed){
        for(unsigned n=0;n<4;++n)if(demo.book.pages[n].active && demo.book.pages[n].capturePending){
            captureRequested[n]=true;bookTargets[n]=prepareBookCapture(n);
            if(!bookTargets[n])demo.failed=true;
        }
    }
    // SYNCDRAW completes previous-frame consumers before reusing linear
    // arenas. Window cursors reset here; capture backing persists across frames.
    C2D_TextBufClear(textBuffer);C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    windowFrameCount=0;
    windowFontCount=0;
    messageIndicatorCount=0;
    const bool wakeWorld=f.screen==FrontScreen::Opening && opening.part==OpeningPart::Wake;
    const bool wakeReady=!wakeWorld || prepareWakeWorld();
    auto* worldTarget=mapCapture && mapCaptureReady?mapTurnTarget:wakeWorld && wakeReady?wakeWorldTarget:top;
    C3D_RenderTargetClear(worldTarget,C3D_CLEAR_ALL,0,0);C3D_FrameDrawOn(worldTarget);C2D_Prepare();C2D_SceneBegin(worldTarget);
    if((wakeWorld && wakeReady) || (mapCapture && mapCaptureReady)){C2D_SceneSize(400,240,false);C3D_SetViewport(0,0,400,240);}
    uint8_t titleBlackPresented=0;uint32_t titlePresentedType=0;float titlePresentedFrame=-1,titlePresentedScale=0;
    int entryPresented=-1;uint8_t exitPresented=0;
    bool headingEntryPresented=false,headingOpenPresented=false,headingStartedPresented=false,headingExitPresented=false;
    float shipPresented=-1;bool shipCaptureRequested=false;
    uint8_t voyageWhitePresented=0;
    int mapPresented=-1;
    float mapTurnPresented=-1;unsigned mapTurnType=0;
    bool ok=wakeReady && mapCaptureReady && mapWorldReady && marketReady;std::array<float,4> paperPresented{{-1,-1,-1,-1}};
    if(f.screen==FrontScreen::Demo && demo.bookComplete) {
        const auto& scene=demo.market;
        ok=marketReady && frontWorld("opening-demo-market",{scene.eye[0],scene.eye[1],scene.eye[2]},{scene.at[0],scene.at[1],scene.at[2]},nullptr,demo);
        if(demo.book.adjusting){
            ok=frontImage("demo-0",OpeningBookView::desktop[0],OpeningBookView::desktop[1],OpeningBookView::desktop[2],OpeningBookView::desktop[3],uint8_t(int(255.f*(1-demo.book.fade)))) && ok;
            ok=frontBookCover(demo) && ok;
            if(demoPaperReady)ok=frontBookPaper(demo,paperPresented) && ok;
        }
    } else if(f.screen==FrontScreen::Opening) {
        const char* map=opening.map();
        CameraPoint eye{-376,154,65},at{-63,75,-247};
        if(opening.part==OpeningPart::House || opening.part==OpeningPart::HouseReturn){const auto& scene=opening.house;eye={scene.eye[0],scene.eye[1],scene.eye[2]};at={scene.at[0],scene.at[1],scene.at[2]};}
        if(opening.part==OpeningPart::Voyage){eye={0,367,2093};at={0,343,-115};}
        if(opening.part==OpeningPart::Wake){eye=opening.wakeEye;at=opening.wakeAt;}
        if(opening.part==OpeningPart::Arrival){eye=opening.harbor.eye;at=opening.harbor.at;}
        if(opening.part==OpeningPart::MapDisplay){
            if(mapCapture)ok=frontImage("opening-map",48,0,304,240) && ok;
            else ok=frontWorld("opening-aaa_00-interior",eye,at,&opening,demo);
            if(!mapCapture && opening.mapDisplay.drawRequested){
                const bool admitted=frontImage("opening-map",0,0,0,0);
                if(admitted)opening.mapDisplay.admitted();
                ok=admitted && ok;
                if(admitted){const auto alpha=opening.mapDisplay.alpha;ok=frontImage("opening-map",48,0,304,240,alpha) && ok;mapPresented=alpha;}
            }
        }
        else if(opening.part==OpeningPart::Letter)ok=frontImage("opening-letter",88,30,224,148);
        else if(opening.part==OpeningPart::Prologue){ok=frontHeading(opening,headingEntryPresented,headingOpenPresented,headingStartedPresented,headingExitPresented);}
        else if(map){const std::string prefix=opening.part==OpeningPart::HouseReturn || (opening.part==OpeningPart::House && opening.house.interior)?"opening-aaa_00-interior":opening.part==OpeningPart::Arrival?"opening-gor_00":"opening-"+std::string(map);ok=frontWorld(prefix.c_str(),eye,at,&opening,demo);}
        if(opening.part==OpeningPart::Voyage){
            ok=frontVoyageLogo(opening.voyage.logo) && ok;
            if(opening.voyage.headingRequested){voyageWhitePresented=uint8_t(std::clamp(opening.voyage.white*255.f,0.f,255.f));ok=C2D_DrawRectSolid(48,0,0,304,240,rgb(255,255,255,voyageWhitePresented)) && ok;}
        }
        if(opening.part==OpeningPart::Arrival){shipCaptureRequested=opening.harbor.paperCapturePending;ok=frontShipPaper(opening,shipPresented) && ok;}
    } else if(f.screen==FrontScreen::Demo) {
        if(!loadDemoPaper() || !frontImage("demo-page-back",0,0,0,0)){demo.failed=true;ok=false;}
        else demo.paperServicesBound=true;
        ok=frontImage("demo-0",OpeningBookView::desktop[0],OpeningBookView::desktop[1],OpeningBookView::desktop[2],OpeningBookView::desktop[3]) && ok;
        count=0;ok=frontBookCover(demo) && ok;
        if(demoPaperReady)ok=frontBookPaper(demo,paperPresented) && ok;
        for(unsigned n=0;n<4;++n)if(captureRequested[n]){
            auto* target=bookTargets[n];if(!target){ok=false;continue;}
            C2D_Flush();C3D_RenderTargetClear(target,C3D_CLEAR_ALL,0,0);
            C2D_Prepare();C2D_SceneBegin(target);
            const auto& desktop=OpeningBookView::desktop;
            ok=frontImage("demo-0",desktop[0]-OpeningBookView::captureLeft,desktop[1],desktop[2],desktop[3]) && ok;
            ok=frontBookCover(demo,true) && ok;
            std::array<float,4> ignored{{-1,-1,-1,-1}};
            ok=frontBookPaper(demo,ignored,true) && ok;
            C2D_Flush();C2D_Prepare();C2D_SceneBegin(top);
        }
    } else {
        ok=frontImage("title-background",48,0,304,240);
        const bool title=f.screen==FrontScreen::Title;
        if(title)count=0; // One vertex range owner for this title frame.
        const float logoY=title?120-(f.title.logoY-20)*.5f-59.5f:24;
        const float copyrightY=title?120-f.title.copyrightY*.5f-16:208;
        ok=frontImage("title-logo",90,logoY,220,119) && ok;
        if(title){
            // Original two-texture prompt retains source UV scroll and alpha.
            ok=frontTitlePrompt(f.title) && ok;
        }
        ok=frontImage("title-copyright",132,copyrightY,136,32) && ok;
        if(title){
            if(ok && loadTitleCurtains())f.title.admitResources();else ok=false;
            ok=frontTitleCurtain(f.title,titlePresentedType,titlePresentedFrame,titlePresentedScale) && ok;
            if(f.title.whiteProgress>0)ok=C2D_DrawRectSolid(48,0,0,304,240,rgb(240,240,240,f.title.whiteAlpha())) && ok;
            if(f.title.state==21){const auto alpha=f.title.blackAlpha();if(C2D_DrawRectSolid(48,0,0,304,240,rgb(0,0,0,alpha)))titleBlackPresented=alpha;else ok=false;}
        }
    }
    if(f.screen==FrontScreen::Demo){
        // Source demo_init admits demo.tpl before seq_gameMain type9/500.
        // frontImage has imported the actual cover/desk original textures.
        if(ok && !demo.exit.entryFinished){
            demo.exit.admitEntry();
            const auto alpha=demo.exit.entryAlpha();
            if(C2D_DrawRectSolid(48,0,0,304,240,rgb(0,0,0,alpha)))entryPresented=alpha;else ok=false;
        }
        if(demo.exit.requested){
            const auto alpha=demo.exit.alpha();
            if(C2D_DrawRectSolid(48,0,0,304,240,rgb(240,240,240,alpha)))exitPresented=alpha;else ok=false;
        }
    }
    if(f.screen==FrontScreen::Opening && opening.part==OpeningPart::Wake)ok=frontWakeEffects(opening.wake) && ok;
    if(mapCapture){
        C2D_Flush();C3D_RenderTargetClear(top,C3D_CLEAR_ALL,0,0);C3D_FrameDrawOn(top);C2D_Prepare();C2D_SceneBegin(top);
        ok=frontImage("opening-map",48,0,304,240) && ok;
    }
    if(f.screen==FrontScreen::Opening && opening.mapTransition.captureReady){
        if(opening.part==OpeningPart::MapDisplay && opening.mapTransition.requested){mapTurnType=49;ok=frontMapTurn(opening.mapTransition,49,mapTurnPresented) && ok;}
        else if(opening.part==OpeningPart::Voyage && opening.mapTransition.entryRequested && !opening.mapTransition.entryComplete){
            if(ok)opening.mapTransition.admitEntry(opening.now*60/1000);
            mapTurnType=50;ok=frontMapTurn(opening.mapTransition,50,mapTurnPresented) && ok;
        }
    }
    if(f.screen==FrontScreen::Opening && opening.message.active()) {
        if(!frontDrawSnapshot)frontDrawSnapshot=std::make_unique<Game>();
        auto& snapshot=*frontDrawSnapshot;snapshot.ticks=opening.now*60/1000;
        CameraPoint anchor{},mario{};const CameraPoint* supplied=nullptr;
        if(opening.part==OpeningPart::House || opening.part==OpeningPart::HouseReturn) {
            const unsigned id=opening.speaker==0x468?2:1;
            const auto& a=opening.house.actors[id];
            anchor={a.position[0]*.01f,a.position[1]*.01f,a.position[2]*.01f};
            const char* bank=id==2?"c_paretta":"c_luigi";
            for(const auto& gpu:cineActors)if(gpu.name==bank && !gpu.sample.vertices.empty()) {
                // Read the same source pose vertices and root transform that
                // were rendered, avoiding an invented speaker-height offset.
                const bool right=a.yaw>0 && a.yaw<180;
                const ActorRootTransform transform({anchor.x,anchor.y,anchor.z},right?180:0,false);
                float lowX=INFINITY,highX=-INFINITY,lowZ=INFINITY,highZ=-INFINITY;
                for(const auto& v:gpu.sample.vertices) {
                    const auto p=transform.point(v);anchor.y=std::max(anchor.y,p[1]);
                    lowX=std::min(lowX,p[0]);highX=std::max(highX,p[0]);lowZ=std::min(lowZ,p[2]);highZ=std::max(highZ,p[2]);
                }
                anchor.x=(lowX+highX)*.5f;anchor.z=(lowZ+highZ)*.5f;
            }
            const auto& m=opening.house.actors[0];mario={m.position[0]*.01f,m.position[1]*.01f,m.position[2]*.01f};
            supplied=&anchor;
        }
        originalMessage(snapshot,&opening.message,supplied,&mario);
    }
    C2D_Flush();C2D_TargetClear(bottom,Paper);C2D_Prepare();C2D_SceneBegin(bottom);
    if(f.screen==FrontScreen::Demo) {
        if(demo.message.active() && demo.speaker)text(demo.speaker==0x2040?"Peach":"Merchant",16,22,.5f,Ink);
        frontMessage(demo.message,demo.now*60/1000);text(demo.waitingForTitle?"":"A / START: title",16,211,.46f,Ink);
        if(demo.failed)text("Opening resource unavailable",16,188,.46f,Red);
    } else if(f.screen==FrontScreen::Title)text("A / START: select a file",47,108,.6f,Ink);
    else if(f.screen==FrontScreen::Files) {
        text("SELECT A FILE",16,9,.65f,Ink);
        for(unsigned n=0;n<4;++n) {
            const auto& slot=f.slots[n];char line[100];
            if(slot.occupied())std::snprintf(line,sizeof(line),"File %u: %s  HP %d%s",n+1,roomName(slot.progress.room),slot.progress.hp,slot.result==LoadResult::Recovered?" (backup)":"");
            else std::snprintf(line,sizeof(line),"File %u: %s",n+1,slot.result==LoadResult::Invalid?"Invalid save": "New Game");
            button(line,10,42+n*37,300,33,n==f.selected,slot.result==LoadResult::Invalid);
        }
        text("D-pad: select   A: play   B: title",16,195,.44f,Ink);
    } else {
        if(opening.message.waiting())text("A / B: next",16,195,.46f,Ink);
        if(opening.failed)text(opening.failure.c_str(),16,188,.40f,Red);
    }
    if(!ok){text("Original front-end texture unavailable",10,216,.4f,Red);if(!frontFailure.empty())text(frontFailure.c_str(),10,203,.35f,Red);}
    if(notice && *notice)text(notice,10,229,.35f,Ink);
    // Publish GPU commands before capture readback/terminal acknowledgements.
    // Synchronize when the next scene can consume or retire captured resources;
    // otherwise an event could advance with a still-unfinished final image.
    C2D_Flush();C3D_FrameEnd(0);
    if(mapCapture || mapTurnPresented>=0 || (mapPresented==255 && !opening.mapDisplay.opaquePresented) || voyageWhitePresented==255 || shipCaptureRequested || shipPresented>=60 || entryPresented==0 || exitPresented==255 || titlePresentedType || titleBlackPresented==255)C3D_FrameSync();
    if(ok && shipCaptureRequested && !captureOpeningShip(opening)){
        frontFailure="Original opening ship capture failed";ok=false;
    }
    if(f.screen==FrontScreen::Opening){
        if(ok)opening.publishSceneAdmission(opening.part);
        else opening.publishSceneFailure(frontFailure.empty()?"Original opening scene resources unavailable":frontFailure);
    }
    if(ok && voyageWhitePresented==255)opening.voyage.presentedWhite(voyageWhitePresented);
    if(ok && shipPresented>=60)opening.harbor.presented(shipPresented);
    if(ok && (headingEntryPresented || headingOpenPresented || headingStartedPresented || headingExitPresented)){
        C3D_FrameSync();
        if(headingEntryPresented)opening.heading.entryComplete=true;
        if(headingOpenPresented)opening.heading.startFadeComplete=true;
        if(headingStartedPresented)opening.heading.started=true;
        if(headingExitPresented)opening.heading.exitComplete=true;
    }
    if(ok && mapCapture)opening.mapTransition.captured();
    if(ok && mapTurnPresented>=0)opening.mapTransition.presented(mapTurnType,mapTurnPresented);
    if(ok && mapPresented>=0)opening.mapDisplay.presented(uint8_t(mapPresented));
    if(ok)f.title.presentedBlack(titleBlackPresented);
    if(ok && titlePresentedType)f.title.presented(titlePresentedType,titlePresentedFrame,titlePresentedScale);
    if(entryPresented>=0 && ok)demo.exit.entryPresented(uint8_t(entryPresented));
    if(ok)demo.exit.presented(exitPresented);
    for(unsigned n=0;n<4;++n)if(captureRequested[n]){
        if(bookTargets[n])C3D_RenderTargetDelete(bookTargets[n]);
        if(ok)demo.book.captureComplete(n);else demo.failed=true;
    }
    for(unsigned n=0;n<4;++n)if(paperPresented[n]>=180)demo.book.presented(n,paperPresented[n]);
}

}
