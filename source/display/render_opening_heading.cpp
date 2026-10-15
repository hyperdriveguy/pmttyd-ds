#include "platform.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace ttyd {
// PMHEDC01 validates UV scale 8/two poses. Subtitle comes from original
// msg_stg_seq_prolouge (source spelling retained); 512x32 capture reserves
// two pixels of padding and at most 508 pixels of glyph width.
bool Renderer::loadHeadingResources(){
    if(headingResourcesAttempted)return headingResourcesReady;
    headingResourcesAttempted=true;
    auto fail=[&](const char* why){frontFailure=std::string("Original heading admission: ")+why;return false;};
    for(unsigned i:{0u,1u,2u,3u,19u,20u,21u,22u,23u,24u,25u}){
        const auto name="heading-"+std::to_string(i);if(!frontImage(name.c_str(),0,0,0,0)){frontFailure="Original heading image admission failed: "+name;return false;}
    }
    auto& actor=headingCurtain;
    if(!actor.animations.load("romfs:/heading-curtain.pmanim"))return fail("curtain animation");
    for(const auto* name:{"Z_1","A_1"}){const auto* pose=actor.animations.pose(name);
        if(!pose || pose->loop || pose->start!=0 || pose->end!=(name[0]=='Z'?0:60) || pose->keys.size()!=(name[0]=='Z'?1:121))return fail("curtain pose contract");
    }
    FILE* f=std::fopen("romfs:/heading-curtain.bin","rb");if(!f)return fail("curtain file");
    char magic[8];uint32_t poses=0,textures=0;
    bool valid=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMHEDC01",8) && std::fread(&actor.uvScale,4,1,f)==1 && std::fread(&poses,4,1,f)==1 && std::fread(&textures,4,1,f)==1 && std::fgetc(f)==EOF;
    std::fclose(f);if(!valid || actor.uvScale!=8 || poses!=2 || !textures)return fail("curtain descriptor");
    f=std::fopen("romfs:/heading-curtain.t3x","rb");if(!f)return fail("curtain file");
    actor.image.imported=Tex3DS_TextureImportStdio(f,&actor.image.texture,nullptr,false);std::fclose(f);if(!actor.image.imported)return fail("curtain texture");
    C3D_TexSetFilter(&actor.image.texture,GPU_LINEAR,GPU_LINEAR);
    MessageResource global;if(!global.load("romfs:/messages/global.txt") || !headingSubtitle.open("msg_stg_seq_prolouge",global.find("msg_stg_seq_prolouge"),harborMessageFont(),0))return fail("subtitle message/font");
    const auto& font=harborMessageFont();
    for(const auto& record:headingSubtitle.records)if(record.code<font.glyphs.size())headingSubtitleWidth=std::max(headingSubtitleWidth,float(record.x+font.glyphs[record.code].width+2));
    if(headingSubtitleWidth<=0 || headingSubtitleWidth>508)return fail("subtitle width");
    if(!C3D_TexInitVRAM(&headingSubtitleTexture,512,32,GPU_RGBA8))return fail("subtitle texture");
    headingSubtitleAllocated=true;
    headingSubtitleTarget=C3D_RenderTargetCreateFromTex(&headingSubtitleTexture,GPU_TEXFACE_2D,0,C3D_DEPTHTYPE(-1));
    if(!headingSubtitleTarget)return fail("subtitle target");
    C3D_TexSetFilter(&headingSubtitleTexture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&headingSubtitleTexture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
    headingResourcesReady=true;return true;
}
bool Renderer::captureHeadingSubtitle(){
    if(headingSubtitleCaptured)return true;
    if(!headingResourcesReady || !headingSubtitleTarget)return false;
    C2D_Flush();C3D_RenderTargetClear(headingSubtitleTarget,C3D_CLEAR_ALL,0,0);C3D_FrameDrawOn(headingSubtitleTarget);C2D_Prepare();C2D_SceneBegin(headingSubtitleTarget);
    const auto& font=harborMessageFont();bool ok=true;
    for(const auto& r:headingSubtitle.records){if(r.code>=font.glyphs.size())continue;
        const auto& glyph=font.glyphs[r.code];Tex3DS_SubTexture sub{};sub.width=glyph.width;sub.height=glyph.height;
        sub.left=glyph.x/512.f;sub.right=(glyph.x+glyph.width)/512.f;sub.top=1-glyph.y/256.f;sub.bottom=1-(glyph.y+glyph.height)/256.f;
        C2D_ImageTint tint;C2D_PlainImageTint(&tint,C2D_Color32(255,255,255,255),1);
        ok=C2D_DrawImageAt({&fontTexture.texture,&sub},2+r.x,2-r.y,0,&tint,r.scale,r.scale) && ok;
    }
    C2D_Flush();C3D_FrameDrawOn(top);C2D_Prepare();C2D_SceneBegin(top);
    headingSubtitleCaptured=ok;return ok;
}
// Native 25-degree perspective preserves source heading geometry. Angles
// below use degrees until converted by tau. Ring has 31 sectors: step 360/31,
// UV step 1/31, inner/outer radii 40/54 and centerY=-40; 62 strip triangles.
// ringReveal is 8.8 fixed point over strip vertex indices: /256 counts fully
// revealed vertices; the remainder is the next vertex alpha.
// Returned presentation flags are acknowledged by the outer GPU frame owner.
bool Renderer::frontHeading(OpeningSequence& opening,bool& entryPresented,bool& openPresented,bool& startedPresented,bool& exitPresented){
    if(!loadHeadingResources())return false;
    if(!opening.headingDisplay.stardust.ready){
        if(!opening.headingDisplay.stardust.load("romfs:/heading-stardust.bin") || !frontImage("heading-stardust",0,0,0,0)){frontFailure="Original heading Stardust admission failed";return false;}
    }
    auto& heading=opening.heading;auto& display=opening.headingDisplay;
    if(heading.displayRequested && !heading.displayReady){
        if(!captureHeadingSubtitle()){frontFailure="Original heading subtitle capture failed";return false;}
        display.admit();heading.displayReady=true;
    }
    if(heading.startRequested && display.state==1)display.start();
    count=0;C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    C3D_Mtx projection,view,matrix;Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(25),C3D_AspectRatioTop,1,32768,false);
    Mtx_LookAt(&view,FVec3_New(0,0,500),FVec3_New(0,0,0),FVec3_New(0,1,0),false);Mtx_Multiply(&matrix,&projection,&view);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&matrix);
    C3D_SetScissor(GPU_SCISSOR_NORMAL,0,48,240,352);C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_GREATER,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    auto image=[&](unsigned index)->FrontTexture*{const auto name="heading-"+std::to_string(index);auto it=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& t){return t.name==name;});return it==frontTextures.end()?nullptr:&*it;};
    auto drawRange=[&](C3D_Tex* texture,int first,int length){GSPGPU_FlushDataCache(vertices+first,size_t(length)*sizeof(Vertex));auto* b=C3D_GetBufInfo();BufInfo_Init(b);BufInfo_Add(b,vertices,sizeof(Vertex),3,0x210);C3D_TexBind(0,texture);C3D_DrawArrays(GPU_TRIANGLES,first,length);};
    auto brightness=[](float angle){const float a=std::fmod(angle,360.f);const int axis=int(a>=270?360-a:a>=180?a-180:a>=90?90-a:a);return uint8_t(255-2*axis);};
    auto quad=[&](unsigned texture,float cx,float cy,float half,float angle,uint8_t shade){
        auto* art=image(texture);if(!art)return false;const int first=count;const float a=angle*.01745329238474369f,c=std::cos(a),s=std::sin(a);
        constexpr float points[4][2]={{-1,1},{1,1},{1,-1},{-1,-1}};constexpr unsigned order[]={0,1,2,2,3,0};
        for(unsigned n:order){const float x=points[n][0]*half,y=points[n][1]*half;vertex(cx+c*x,cy+y,-s*x,(points[n][0]+1)*.5f*art->size[0]/art->size[2],1-(1-points[n][1])*.5f*art->size[1]/art->size[3],C2D_Color32(shade,shade,shade,255));}
        drawRange(&art->image.texture,first,6);return true;
    };
    bool ok=true;
    if(display.state){
        const float face=90+display.backgroundAngle;
        ok=quad(display.palette,0,-40,40,face,brightness(face));
        for(const auto& letter:display.letters)if(letter.phase)ok=quad(letter.texture,letter.x,letter.y,22,letter.angle,brightness(letter.angle)) && ok;
        if(headingSubtitleCaptured && display.ringReveal){
            const int first=count;constexpr float step=11.612903594970703f,uvStep=.032258063554763794f,tau=6.2831854820251465f;
            auto point=[&](unsigned n){const unsigned sector=n/2;const float radius=n%2?40:54,angle=(tau*(step*sector))/360.f-display.ringRotation*.01745329238474369f;
                const uint32_t reveal=display.ringReveal/256,partial=display.ringReveal%256;const uint8_t alpha=n<reveal?255:n==reveal?uint8_t(partial):0;
                vertex(std::sin(angle)*radius,-40+std::cos(angle)*radius,0,(2+uvStep*sector*headingSubtitleWidth)/512.f,n%2?1-26.f/32.f:1-2.f/32.f,C2D_Color32(255,255,255,alpha));};
            for(unsigned n=0;n<62;++n){if(n%2){point(n);point(n+2);point(n+1);}else{point(n);point(n+1);point(n+2);}}
            drawRange(&headingSubtitleTexture,first,count-first);
        }
        // Source mode4 texture32: C0*(1-TEXC)+TEXC, alpha=A0*TEXA.
        auto star=std::find_if(frontTextures.begin(),frontTextures.end(),[](const auto& t){return t.name=="heading-stardust";});
        if(star==frontTextures.end())return false;
        C3D_TexEnvSrc(env,C3D_RGB,GPU_CONSTANT,GPU_PRIMARY_COLOR,GPU_TEXTURE0);C3D_TexEnvFunc(env,C3D_RGB,GPU_INTERPOLATE);C3D_TexEnvColor(env,0xffffffff);
        C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
        for(const auto& group:display.stardust.groups)if(group.started && group.age){unsigned visible=0;
            for(const auto& particle:group.particles)if(particle.frame>=0){const int first=count;const auto rgba=display.stardust.color(group,visible++);const auto color=C2D_Color32(rgba[0],rgba[1],rgba[2],rgba[3]);
                constexpr unsigned order[]={0,1,2,0,2,3};for(unsigned n:order){const auto& v=display.stardust.quads[particle.frame][n];vertex(group.x+particle.x+v.x*particle.scale,group.y+particle.y+v.y*particle.scale,v.z*particle.scale,v.s/star->size[2],1-v.t/star->size[3],color);}
                drawRange(&star->image.texture,first,6);
            }
        }
        C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
        if(display.state==2 && !heading.started)startedPresented=ok;
    }
    if(heading.startFadeRequested && !headingCurtainStarted){headingCurtainStarted=true;headingCurtainStart=opening.now*60/1000;}
    if(!heading.startFadeComplete){
        const auto* pose=headingCurtain.animations.pose(headingCurtainStarted?"A_1":"Z_1");const float frame=headingCurtainStarted?float(std::min<uint64_t>(opening.now*60/1000-headingCurtainStart,60)):0;
        if(!pose || !pose->evaluate(frame,headingCurtain.sample))return false;
        const auto& sample=headingCurtain.sample;const int first=count;
        if(sample.vertices.size()>size_t(12000-count))return false;
        C3D_Mtx curtainProjection;Mtx_OrthoTilt(&curtainProjection,0,400,240,0,-100,100,true);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&curtainProjection);
        for(size_t n=0;n<sample.vertices.size();++n){const auto& p=sample.vertices[n];vertex(200+p.x*304/64,120-p.y*5,0,p.u,1-(1-p.v)*headingCurtain.uvScale,0xffffffff,sample.colors.empty()?nullptr:&sample.colors[n]);}
        GSPGPU_FlushDataCache(vertices+first,size_t(count-first)*sizeof(Vertex));auto* b=C3D_GetBufInfo();BufInfo_Init(b);BufInfo_Add(b,vertices,sizeof(Vertex),3,0x210);C3D_TexBind(0,&headingCurtain.image.texture);
        for(const auto& batch:sample.batches){C3D_CullFace(batch.cull==0?GPU_CULL_FRONT_CCW:batch.cull==1?GPU_CULL_BACK_CCW:GPU_CULL_NONE);C3D_AlphaTest(true,batch.pass==2?GPU_GEQUAL:GPU_GREATER,batch.pass==2?128:0);C3D_DrawArrays(GPU_TRIANGLES,first+batch.start,batch.count);}
        if(!headingCurtainStarted && !heading.entryComplete)entryPresented=ok;
        if(headingCurtainStarted && frame==60)openPresented=ok;
    }
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_CullFace(GPU_CULL_NONE);C3D_AlphaTest(true,GPU_GREATER,0);C2D_Prepare();C2D_SceneBegin(top);
    if(heading.exitRequested){if(headingExitStart==UINT64_MAX)headingExitStart=opening.now;
        const uint64_t elapsed=std::min<uint64_t>(opening.now-headingExitStart,500);const auto alpha=uint8_t(elapsed*255/500);
        ok=C2D_DrawRectSolid(48,0,0,304,240,C2D_Color32(0,0,0,alpha)) && ok;if(alpha==255)exitPresented=ok;
    }
    return ok;
}
// Retire subtitle render target before its borrowed texture; reset attempt
// latches and UINT64_MAX epoch so a future scene admits fresh resources.
void Renderer::freeHeading(){
    if(headingCurtain.image.imported){Tex3DS_TextureFree(headingCurtain.image.imported);C3D_TexDelete(&headingCurtain.image.texture);}
    headingCurtain={};headingResourcesReady=headingResourcesAttempted=headingCurtainStarted=false;headingCurtainStart=0;headingExitStart=UINT64_MAX;headingSubtitle={};headingSubtitleWidth=0;
    if(headingSubtitleTarget){C3D_RenderTargetDelete(headingSubtitleTarget);headingSubtitleTarget=nullptr;}
    if(headingSubtitleAllocated){C3D_TexDelete(&headingSubtitleTexture);headingSubtitleAllocated=false;}headingSubtitleCaptured=false;
}
}
