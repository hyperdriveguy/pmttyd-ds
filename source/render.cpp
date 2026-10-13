#include "platform.hpp"
#include "world_shbin.h"
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
    vertices=(Vertex*)linearAlloc(sizeof(Vertex)*MaxVertices);
    textBuffer=C2D_TextBufNew(8192);
    if(!vertices || !textBuffer) return false;
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
    return loadMap("map",plaza) && loadMap("stage",stage);
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
    uint32_t groups=0;
    for(auto batch:room.batches) {if(batch.count%3 || ((batch.layer>>8)&65535)>1024 || (batch.layer>>26)>8)return false;groups=std::max(groups,(batch.layer>>8)&65535);}
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
    return true;
}
void Renderer::freeRoom(Room& room) {
    for(auto& texture:room.textures) if(texture.imported) {Tex3DS_TextureFree(texture.imported);C3D_TexDelete(&texture.texture);}
    if(room.vertices)linearFree(room.vertices);
    room=Room{};
}
bool Renderer::changeRoom(uint32_t room) {
    const char* name=roomAsset(room);if(!name)return false;
    Room next;
    if(!loadMap(name,next)) {C3D_FrameSync();freeRoom(next);return false;}
    C3D_FrameSync();freeRoom(plaza);plaza=std::move(next);return true;
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
void Renderer::drawMap(bool battle,const Game& game,bool translucent) {
    auto& room=battle?stage:plaza;
    if(!battle && !translucent)updateCutaways(room,game);
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
        if(!battle && ((batch.layer&255)==2)!=translucent)continue;
        if(!battle && !mapEventGroupVisible(batch.layer>>26,game.progress.originalStory.value_or(0)))continue;
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
void Renderer::vertex(float x,float y,float z,float u,float v,uint32_t color) {
    if(count>=MaxVertices) return;
    vertices[count++]={x,y,z,u,v,(color&255)/255.f,((color>>8)&255)/255.f,((color>>16)&255)/255.f,((color>>24)&255)/255.f};
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
bool Renderer::characterModel(const char* name,float frame,Vec2 root,float rootY,float yaw,bool reflectZ,bool companion) {
    const auto* pose=(companion?partnerAnimations:actorAnimations).pose(name);
    if(!pose || !pose->evaluate(frame,actorSample) || actorSample.vertices.size()>size_t(MaxVertices-count))return false;
    for(const auto& v:actorSample.vertices) {
        const auto point=actorMeshPoint(v,{root.x,rootY,root.z},yaw,reflectZ);
        vertex(point[0],point[1],point[2],v.u,v.v,0xffffffff);
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
    // Original room backdrop, drawn without depth writes before the room.
    const float x0=cam-30,x1=cam+30,y0=-12,y1=24,z=-25;
    vertex(x0,y0,z,0,0,0xffffffff);vertex(x1,y0,z,1,0,0xffffffff);vertex(x1,y1,z,1,1,0xffffffff);
    vertex(x1,y1,z,1,1,0xffffffff);vertex(x0,y1,z,0,1,0xffffffff);vertex(x0,y0,z,0,0,0xffffffff);
    if(!battle) {
        shadow(g.partner,.43f,g.partnerHeight);if(g.progress.room==0)shadow(NpcPosition,.48f);
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
    if(!battle && g.marioPresentation.supported && !(g.marioPresentation.displayFlags&1))
        characterModel(g.marioPresentation.selected,g.marioPresentation.animationFrame(),g.progress.position,g.water.active()?g.water.position[1]:g.groundHeight+g.jumpHeight,
                       g.marioPresentation.drawYaw(),g.marioPresentation.reflectZ());
    const int actorEnd=count;
    if(!battle)characterModel(g.partnerPresentation.name(),g.partnerPresentation.frame(),g.partner,g.partnerHeight,
                             g.partnerPresentation.yaw(),g.partnerPresentation.reflectZ(),true);
    const int partnerEnd=count;
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
    drawMap(battle,g);
    buffers=C3D_GetBufInfo();
    BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
    C3D_TexBind(0,&atlas);
    C3D_DrawArrays(GPU_TRIANGLES,6,legacyCount-6);
    if(actorEnd>legacyCount){C3D_TexBind(0,&characterTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,legacyCount,actorEnd-legacyCount);}
    if(partnerEnd>actorEnd){C3D_TexBind(0,&partnerTextures.texture);C3D_DrawArrays(GPU_TRIANGLES,actorEnd,partnerEnd-actorEnd);}
    if(polygonShadow || airShadowCount) {
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env,C3D_Both,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(env,C3D_Both,GPU_REPLACE);
        C3D_AlphaTest(false,GPU_ALWAYS,0);
        if(polygonShadow) {
            C3D_DepthTest(true,GPU_GEQUAL,FieldRGB);
            C3D_DrawArrays(GPU_TRIANGLES,partnerEnd,count-partnerEnd);
        } else {
            // Source LEQUAL becomes GEQUAL with the reversed native projection.
            // Preserve each record's alpha OR, front-face zero, RGB mask blend.
            C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITE_ALPHA);
            C3D_CullFace(GPU_CULL_FRONT_CCW);
            C3D_ColorLogicOp(GPU_LOGICOP_OR);
            C3D_DrawArrays(GPU_TRIANGLES,partnerEnd,airShadowCount);
            C3D_CullFace(GPU_CULL_BACK_CCW);
            C3D_ColorLogicOp(GPU_LOGICOP_COPY);
            C3D_DrawArrays(GPU_TRIANGLES,partnerEnd+airShadowCount,airShadowCount);
            C3D_CullFace(GPU_CULL_FRONT_CCW);
            C3D_DepthTest(true,GPU_GEQUAL,FieldRGB);
            C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_DST_ALPHA,GPU_ONE_MINUS_DST_ALPHA,GPU_ONE,GPU_ZERO);
            C3D_DrawArrays(GPU_TRIANGLES,partnerEnd+airShadowCount*2,airShadowCount);
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
    if(!battle)drawMap(false,g,true);
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
    panel(8,7,174,28,Paper);
    char line[128];std::snprintf(line,sizeof(line),"Mario  HP %d/10",g.progress.hp);text(line,16,12,.54f,Ink);
    text((g.scene==Scene::Battle || g.scene==Scene::Victory || g.scene==Scene::Defeat)?"BATTLE STAGE":roomName(g.progress.room),205,12,.46f,rgb(255,246,222));
    if(g.scene==Scene::Dialogue) {
        panel(14,163,372,65,Paper);text(g.dialogue(),26,172,.55f,Ink);
        text("A / touch >",308,208,.4f,Ink);
    } else {
        panel(8,207,384,27,Paper);text(g.prompt(),15,212,.43f,Ink);
    }
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
    } else if(g.scene==Scene::Dialogue) {
        text("Conversation",16,64,.76f,Ink);
        button("Next / A",16,105,288,49,true);
        button("Close / B",16,168,288,43,false);
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
    C2D_TextBufClear(textBuffer);
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    const bool battle=g.scene==Scene::Battle || g.scene==Scene::Victory || g.scene==Scene::Defeat;
    C3D_RenderTargetClear(top,C3D_CLEAR_ALL,battle?0xA6CBD8FF:0,0);
    C3D_FrameDrawOn(top);world(g);
    C2D_Prepare();C2D_SceneBegin(top);overlay(g);
    if(g.fade.alpha())C2D_DrawRectSolid(0,0,.99f,400,240,C2D_Color32(0,0,0,g.fade.alpha()));
    C2D_Flush();
    C2D_TargetClear(bottom,Paper);C2D_Prepare();C2D_SceneBegin(bottom);menu(g,m,notice,audioAvailable);
    if(g.fade.alpha())C2D_DrawRectSolid(0,0,.99f,320,240,C2D_Color32(0,0,0,g.fade.alpha()));
    C2D_Flush();
    C3D_FrameEnd(0);
}
void Renderer::shutdown() {
    if(c3dReady) C3D_FrameSync();
    if(textBuffer) C2D_TextBufDelete(textBuffer);
    for(Room* room : {&plaza,&stage}) {
        for(auto& texture:room->textures) if(texture.imported) {Tex3DS_TextureFree(texture.imported);C3D_TexDelete(&texture.texture);}
        if(room->vertices) linearFree(room->vertices);
    }
    if(sky.imported) {Tex3DS_TextureFree(sky.imported);C3D_TexDelete(&sky.texture);}
    if(characterTextures.imported) {Tex3DS_TextureFree(characterTextures.imported);C3D_TexDelete(&characterTextures.texture);}
    if(partnerTextures.imported) {Tex3DS_TextureFree(partnerTextures.imported);C3D_TexDelete(&partnerTextures.texture);}
    if(imported) Tex3DS_TextureFree(imported);
    if(atlasReady) C3D_TexDelete(&atlas);
    if(vertices) linearFree(vertices);
    if(programReady) shaderProgramFree(&program);
    if(shader) DVLB_Free(shader);
    if(top) C3D_RenderTargetDelete(top);
    if(bottom) C3D_RenderTargetDelete(bottom);
    if(c2dReady) C2D_Fini();
    if(c3dReady) C3D_Fini();
}
}
