#include "platform.hpp"
#include <algorithm>
#include <cstdio>
namespace ttyd {
namespace {constexpr uint32_t Ink=0xff352d29,Paper=0xffdff2fb,Red=0xff4966d8;
uint32_t rgb(unsigned r,unsigned g,unsigned b,unsigned a=255){return C2D_Color32(r,g,b,a);}}
void Renderer::beginFrontend(){
    C3D_FrameSync();
    for(auto* room:{&plaza,&stage,&trioBackground,&trioStage})freeRoom(*room);
    frontActive=true;
}
void Renderer::releaseFrontend(){
    C3D_FrameSync();freeRoom(frontRoom);frontMapName.clear();
    for(auto& t:frontTextures)if(t.image.imported){Tex3DS_TextureFree(t.image.imported);C3D_TexDelete(&t.image.texture);}
    frontTextures.clear();
    for(auto& a:cineActors)if(a.image.imported){Tex3DS_TextureFree(a.image.imported);C3D_TexDelete(&a.image.texture);}
    cineActors.clear();
}
bool Renderer::frontImage(const char* name,float x,float y,float w,float h) {
    if(frontTextures.empty())frontTextures.reserve(32); // C2D retains texture pointers until flush.
    auto found=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& f){return f.name==name;});
    if(found==frontTextures.end()) {
        FrontTexture pending;pending.name=name;char path[128];
        std::snprintf(path,sizeof(path),"romfs:/%s.size",name);FILE* meta=std::fopen(path,"rb");if(!meta)return false;
        bool ok=std::fread(pending.size.data(),4,4,meta)==4 && std::fgetc(meta)==EOF;std::fclose(meta);
        const auto& z=pending.size;if(!ok || !z[0] || !z[1] || z[0]>z[2] || z[1]>z[3] || z[2]>1024 || z[3]>1024)return false;
        std::snprintf(path,sizeof(path),"romfs:/%s.t3x",name);FILE* f=std::fopen(path,"rb");if(!f)return false;
        pending.image.imported=Tex3DS_TextureImportStdio(f,&pending.image.texture,nullptr,false);std::fclose(f);
        if(!pending.image.imported)return false;
        C3D_TexSetFilter(&pending.image.texture,GPU_LINEAR,GPU_LINEAR);
        frontTextures.push_back(std::move(pending));found=frontTextures.end()-1;
    }
    const auto& z=found->size;Tex3DS_SubTexture sub{};sub.width=z[0];sub.height=z[1];
    sub.left=0;sub.right=float(z[0])/z[2];sub.top=1;sub.bottom=1-float(z[1])/z[3];
    C2D_DrawImageAt({&found->image.texture,&sub},x,y,0,nullptr,w/z[0],h/z[1]);return true;
}
void Renderer::frontMessage(const MessagePlayback& message) {
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
}
bool Renderer::frontWorld(const char* map,CameraPoint eye,CameraPoint at,const OpeningSequence* opening,const OpeningDemo& demo) {
    if(frontMapName!=map) {
        // A cutscene owns one map. Release the preceding scene before staging
        // its successor; retaining both can exhaust the native linear arena.
        C3D_FrameSync();freeRoom(frontRoom);frontMapName.clear();
        for(auto& t:frontTextures)if(t.image.imported){Tex3DS_TextureFree(t.image.imported);C3D_TexDelete(&t.image.texture);}
        frontTextures.clear();
        Room pending;if(!loadMap(map,pending)){freeRoom(pending);frontFailure=std::string("Scene map load failed: ")+map;return false;}
        frontRoom=std::move(pending);frontMapName=map;frontFailure.clear();
    }
    Game snapshot;snapshot.progress.room=255;snapshot.ticks=(opening?opening->now:demo.now)*60/1000;
    snapshot.fieldCameraFrame.eye={eye.x*.01f,eye.y*.01f,eye.z*.01f};snapshot.fieldCameraFrame.at={at.x*.01f,at.y*.01f,at.z*.01f};
    C3D_Mtx projection,view;
    Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(42),C3D_AspectRatioTop,.1f,80,false);
    Mtx_LookAt(&view,FVec3_New(eye.x*.01f,eye.y*.01f,eye.z*.01f),FVec3_New(at.x*.01f,at.y*.01f,at.z*.01f),FVec3_New(0,1,0),false);
    Mtx_Multiply(&worldTransform,&projection,&view);
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GREATER,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    drawMap(false,snapshot,false,&frontRoom);
    auto actor=[&](const char* bank,const char* pose,CameraPoint root,float yaw) {
        auto found=std::find_if(cineActors.begin(),cineActors.end(),[&](const auto& a){return a.name==bank;});
        if(found==cineActors.end()) {
            CineActor pending;pending.name=bank;const std::string base=std::string("romfs:/")+bank;
            if(!pending.animations.load((base+".pmanim").c_str()))return false;
            FILE* uv=std::fopen((base+".uvscale").c_str(),"rb");if(!uv)return false;
            bool ok=std::fread(&pending.uvScale,4,1,uv)==1 && std::fgetc(uv)==EOF;std::fclose(uv);
            if(!ok || !(pending.uvScale==1 || pending.uvScale==2 || pending.uvScale==4 || pending.uvScale==8))return false;
            FILE* f=std::fopen((base+".t3x").c_str(),"rb");if(!f)return false;
            pending.image.imported=Tex3DS_TextureImportStdio(f,&pending.image.texture,nullptr,false);std::fclose(f);if(!pending.image.imported)return false;
            C3D_TexSetFilter(&pending.image.texture,GPU_LINEAR,GPU_LINEAR);cineActors.push_back(std::move(pending));found=cineActors.end()-1;
        }
        count=0;if(!characterModel(pose,float(snapshot.ticks),{root.x*.01f,root.z*.01f},root.y*.01f,yaw,false,false,&found->animations,found->uvScale))return false;
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        C3D_TexBind(0,&found->image.texture);C3D_DrawArrays(GPU_TRIANGLES,0,count);return true;
    };
    bool ok=true;
    if(!opening && demo.bookComplete) {
        ok=actor("c_peach","P_S_1",{0,0,450},180) && actor("c_nazo","S_1",{-145,0,450},180);
    } else if(opening && (opening->part==OpeningPart::House || opening->part==OpeningPart::HouseReturn)) {
        // Source explicit placements: postbox/outside versus read-letter interior.
        const CameraPoint luigi=opening->part==OpeningPart::House && opening->index<2?CameraPoint{225,30,-75}:opening->part==OpeningPart::House && opening->index==2?CameraPoint{25,30,-155}:CameraPoint{-52,30,-275};
        const bool letterPose=opening->part==OpeningPart::HouseReturn || opening->index==4;
        const bool talking=opening->message.active() && !opening->message.waiting();
        ok=actor("c_luigi",letterPose?(talking?"L_9":"L_8"):(talking?"L_7":"L_5"),luigi,0);
        if(opening->part==OpeningPart::House && opening->index<=1)ok=actor("c_paretta","S_1",{210,25,-30},0) && ok;
        count=0;characterModel("M_S_1",float(snapshot.ticks),opening->part==OpeningPart::House && opening->index<=2?Vec2{-1.50f,-3.30f}:Vec2{-.85f,-3.10f},.30f,180,false);
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);C3D_TexBind(0,&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,0,count);
    } else if(opening && opening->part==OpeningPart::Arrival) {
        count=0;characterModel("M_S_1",float(snapshot.ticks),{0,2.25f},.15f,180,false);
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);C3D_TexBind(0,&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,0,count);
    }
    drawMap(false,snapshot,true,&frontRoom);
    C2D_Prepare();C2D_SceneBegin(top);return ok;
}
void Renderer::renderFrontend(const Frontend& f,const OpeningDemo& demo,const OpeningSequence& opening,const char* notice) {
    // Keep only the current demonstration image/scene family resident. This
    // also bounds a natural (unskipped) storybook run and letter/map overlays.
    const uint32_t phase=f.screen==FrontScreen::Demo?(demo.bookComplete?199:100+demo.image()):
        f.screen==FrontScreen::Opening?200+uint32_t(opening.part):0;
    if(phase!=frontPhase){releaseFrontend();frontPhase=phase;}
    C2D_TextBufClear(textBuffer);C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_RenderTargetClear(top,C3D_CLEAR_ALL,0,0);C3D_FrameDrawOn(top);C2D_Prepare();C2D_SceneBegin(top);
    bool ok=true;
    if(f.screen==FrontScreen::Demo && demo.bookComplete) {
        ok=frontWorld("map",{0,100,834},{0,50,437},nullptr,demo);
    } else if(f.screen==FrontScreen::Opening) {
        const char* map=opening.map();
        CameraPoint eye{-376,154,65},at{-63,75,-247};
        if(opening.part==OpeningPart::House && opening.index<2){eye={145,122,717};at={145,108,-180};}
        else if(opening.part==OpeningPart::House && opening.index==2){eye={-345,154,96};at={-32,75,-217};}
        if(opening.part==OpeningPart::Voyage){eye={0,500,1400};at={0,0,0};}
        if(opening.part==OpeningPart::Wake){eye={0,200,1000};at={0,0,0};}
        if(opening.part==OpeningPart::Arrival){eye={2,225,1015};at={2,50,243};}
        if(opening.part==OpeningPart::MapDisplay)ok=frontImage("opening-map",48,0,304,240);
        else if(opening.part==OpeningPart::Letter)ok=frontImage("opening-letter",88,30,224,148);
        else if(opening.part==OpeningPart::Prologue){ok=frontImage("prologue",48,95,304,55);}
        else if(map){const std::string prefix=opening.part==OpeningPart::HouseReturn || (opening.part==OpeningPart::House && opening.index>=2)?"opening-aaa_00-interior":opening.part==OpeningPart::Arrival?"gor_00":"opening-"+std::string(map);ok=frontWorld(prefix.c_str(),eye,at,&opening,demo);}
    } else if(f.screen==FrontScreen::Demo) {
        char name[24];std::snprintf(name,sizeof(name),"demo-%u",demo.image());
        ok=frontImage(name,48,0,304,240);
        if(demo.narration<5)ok=frontImage("demo-1",124,18,152,186) && ok;
    } else {
        ok=frontImage("title-background",48,0,304,240);
        ok=frontImage("title-logo",90,24,220,119) && ok;
        if(f.screen==FrontScreen::Title)ok=frontImage("title-prompt",128,159,144,16) && ok;
        ok=frontImage("title-copyright",132,215,136,32) && ok;
    }
    C2D_Flush();C2D_TargetClear(bottom,Paper);C2D_Prepare();C2D_SceneBegin(bottom);
    if(f.screen==FrontScreen::Demo) {
        if(demo.message.active() && demo.speaker)text(demo.speaker==0x2040?"Peach":"Merchant",16,22,.5f,Ink);
        frontMessage(demo.message);text("A / START: title",16,211,.46f,Ink);
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
        if(opening.message.active() && opening.speaker)text(opening.speaker==0x468?"Parakarry":"Luigi",16,22,.5f,Ink);
        frontMessage(opening.message);
        if(opening.part==OpeningPart::Voyage)text("A / START: continue",16,195,.46f,Ink);
        else if(opening.message.waiting())text("A / B: next",16,195,.46f,Ink);
        if(opening.failed)text(opening.failure.c_str(),16,188,.40f,Red);
        text("Development sequence / scene services pending",10,215,.36f,Ink);
    }
    if(!ok){text("Original front-end texture unavailable",10,216,.4f,Red);if(!frontFailure.empty())text(frontFailure.c_str(),10,203,.35f,Red);}
    if(notice && *notice)text(notice,10,229,.35f,Ink);
    C2D_Flush();C3D_FrameEnd(0);
}

}
