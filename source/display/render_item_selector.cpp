#include "platform.hpp"
#include "harbor_story.hpp"
#include "item_presentation.hpp"
#include "message_icons.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
void Renderer::selectorClip(const ItemSelectionClip* clip) {
    C2D_Flush();
    if(!clip){C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,0,0);return;}
    // Project source608x480 scissor corners through the same tilted native
    // projection; PICA framebuffer axes differ from displayed screen axes.
    C3D_Mtx projection;Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);
    float left=240,topEdge=400,right=0,bottomEdge=0;
    for(unsigned corner=0;corner<4;++corner) {
        const float x=200+(float(clip->x+(corner&1?clip->width:0))-304)*.5f;
        const float y=120+(float(clip->y+(corner&2?clip->height:0))-240)*.5f;
        const auto p=Mtx_MultiplyFVecH(&projection,FVec3_New(x,y,0));
        const float px=(p.x/p.w+1)*120,py=(p.y/p.w+1)*200;
        left=std::min(left,px);right=std::max(right,px);topEdge=std::min(topEdge,py);bottomEdge=std::max(bottomEdge,py);
    }
    const auto bound=[](float x,unsigned size){return unsigned(std::clamp(std::lround(x),0l,long(size)));};
    C3D_SetScissor(GPU_SCISSOR_NORMAL,bound(left,240),bound(topEdge,400),bound(right,240),bound(bottomEdge,400));
}
bool Renderer::sourceWindowFont(const char* text,WindowFontConsumer consumer,ItemSelectionTextDraw request,
    const WindowFontStyle& supplied,const ItemSelectionClip* clip,const ColorWindowText* colorTokens) {
    const auto& font=harborMessageFont();WindowFontMeasurement measured;
    if(!windowFontVertices || !fontTexture.imported || !measurePlainWindowFont(font,colorTokens?colorTokens->plain.c_str():text,supplied.spacing,measured))return false;
    const size_t quads=measured.glyphs*(supplied.edge?5:1);
    if(!quads)return true;
    if(request.widthLimit>0) {
        const float width=request.scaleX*float(measured.width);
        if(width>request.widthLimit)request.scaleX=request.scaleX*(request.widthLimit/width);
    }
    const unsigned start=windowFontCount*6;
    struct FontContext {Renderer* renderer;ItemSelectionTextDraw request;const ItemSelectionClip* clip;WindowFontConsumer consumer;bool overflow=false;} context{this,request,clip,consumer};
    WindowFontDrawServices services;services.user=&context;
    services.random=[](void* p,uint32_t bound){auto& r=static_cast<FontContext*>(p)->renderer->windowFontRng;r=r*1664525u+1013904223u;return r%bound;};
    services.emitQuad=[](void* p,const WindowFontQuad& q){
        auto& context=*static_cast<FontContext*>(p);auto& renderer=*context.renderer;
        // Preserve every source glyph/RNG request while avoiding native GPU
        // vertices for complete quads outside the source scissor.
        if(context.clip) {
            const auto& r=context.request;const auto& clip=*context.clip;
            const float ox=(context.consumer==WindowFontConsumer::MessageDirect || context.consumer==WindowFontConsumer::StringPitch)?0:r.x;
            const float oy=(context.consumer==WindowFontConsumer::MessageDirect || context.consumer==WindowFontConsumer::StringPitch)?0:r.y;
            const float sx=context.consumer==WindowFontConsumer::MessageDirect?1:r.scaleX;
            const float sy=context.consumer==WindowFontConsumer::MessageDirect?1:r.scaleY;
            if(!windowFontQuadIntersects(q,{ox,oy,sx,sy},
                {float(clip.x-304),float(240-clip.y),float(clip.x-304+clip.width),float(240-clip.y-clip.height)}))return;
        }
        if(renderer.windowFontCount==MaxWindowFontQuads){context.overflow=true;return;}
        auto* vertices=renderer.windowFontVertices+renderer.windowFontCount++*6;
        const bool pitch=context.consumer==WindowFontConsumer::StringPitch;
        const float x0=pitch?std::fma(context.request.scaleX,float(q.left),q.pitchX):float(q.originX)+float(q.left);
        const float x1=pitch?std::fma(context.request.scaleX,float(q.right),q.pitchX):float(q.originX)+float(q.right);
        const float y0=pitch?std::fma(context.request.scaleY,float(q.top),q.pitchY):float(q.originY)+float(q.top);
        const float y1=pitch?std::fma(context.request.scaleY,float(q.bottom),q.pitchY):float(q.originY)+float(q.bottom);
        const float r=(q.rgba>>24)/255.f,g=((q.rgba>>16)&255)/255.f,b=((q.rgba>>8)&255)/255.f,a=(q.rgba&255)/255.f;
        const float u0=q.u0/512.f,u1=q.u1/512.f,v0=1-q.v0/256.f,v1=1-q.v1/256.f;
        vertices[0]={x0,y0,0,u0,v0,r,g,b,a};vertices[1]={x1,y0,0,u1,v0,r,g,b,a};vertices[2]={x1,y1,0,u1,v1,r,g,b,a};
        vertices[3]=vertices[0];vertices[4]=vertices[2];vertices[5]={x0,y1,0,u0,v1,r,g,b,a};
    };
    if(colorTokens) {
        if(consumer!=WindowFontConsumer::MessageDirect || !drawColorWindowFont(font,*colorTokens,supplied,services,int32_t(request.x),int32_t(request.y)))return false;
    } else if(consumer==WindowFontConsumer::StringPitch) {
        if(!drawPlainWindowFontPitch(font,text,supplied,services,request.x,request.y,request.scaleX,request.scaleY))return false;
    } else if(!drawPlainWindowFont(font,text,consumer,supplied,services,int32_t(request.x),int32_t(request.y)))return false;
    if(context.overflow)return false;
    const unsigned count=windowFontCount*6-start;if(!count)return true;
    C2D_Flush();
    GSPGPU_FlushDataCache(windowFontVertices+start,count*sizeof(Vertex));
    C3D_Mtx local,viewport,model,projection,transform;Mtx_Identity(&local);
    if(consumer!=WindowFontConsumer::MessageDirect && consumer!=WindowFontConsumer::StringPitch){Mtx_Translate(&local,request.x,request.y,0,true);Mtx_Scale(&local,request.scaleX,request.scaleY,1);}
    Mtx_Identity(&viewport);Mtx_Translate(&viewport,200,120,0,true);Mtx_Scale(&viewport,.5f,-.5f,1);
    Mtx_Multiply(&model,&viewport,&local);Mtx_OrthoTilt(&projection,0,400,240,0,-1,1,true);Mtx_Multiply(&transform,&projection,&model);
    C3D_BindProgram(&program);auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,windowFontVertices,sizeof(Vertex),3,0x210);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(true,GPU_ALWAYS,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
    for(int i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_TexBind(0,&fontTexture.texture);selectorClip(clip);C3D_DrawArrays(GPU_TRIANGLES,start,count);
    C2D_Prepare();C2D_SceneBegin(top);C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
    C3D_AlphaTest(true,GPU_GREATER,8);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    selectorClip(nullptr);return true;
}
bool Renderer::selectorIcon(uint32_t id,uint64_t ticks,float x,float y,float scale,uint32_t flags,bool centered) {
    const auto* icon=messageIcon(id);if(!icon)return false;
    return selectorIconFrame(icon->frame(ticks),x,y,scale,flags,centered);
}
bool Renderer::selectorIconFrame(const MessageIconFrame& frame,float x,float y,float scale,uint32_t flags,bool centered) {
    Tex3DS_SubTexture sub{};sub.width=frame.width;sub.height=frame.height;
    sub.left=frame.u0;sub.right=frame.u1;sub.top=frame.v0;sub.bottom=frame.v1;
    if(flags&4)std::swap(sub.left,sub.right);
    if(flags&8)std::swap(sub.top,sub.bottom);
    const float width=frame.width*scale*.5f,height=frame.height*scale*.5f;
    // Original Y mirroring moves a bottom-anchored quad below its anchor;
    // swapping UVs alone would leave the paging Up arrow above that anchor.
    const float top=120-y*.5f-height*(centered?.5f:((flags&8)?0.f:1.f));
    C2D_Image image{&messageIconTexture.texture,&sub};C2D_DrawParams params={{200+x*.5f-width*.5f,top,width,height},{0,0},.6f,0};
    if(centered) {
        C2D_Flush();C3D_AlphaTest(true,GPU_GEQUAL,128);C3D_ColorLogicOp(GPU_LOGICOP_COPY);
    }
    const bool drawn=C2D_DrawImage(image,&params,nullptr);
    if(centered) {
        C2D_Flush();C3D_AlphaTest(true,GPU_GREATER,8);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    }
    return drawn;
}
bool Renderer::itemSelector(ItemSelectionPresentation& p) {
    const auto& main=p.owner.main();const auto& font=harborMessageFont();const ItemSelectionClip full{0,0,608,480};
    bool success=true;WindowFontMeasurement measured;
    for(unsigned slot=0;slot<3;++slot)if(p.displaySlots()&(1u<<slot)) {
        auto& window=p.owner.displayWindow(slot);const auto& r=window.rectangle;const auto& sequence=window.sequence;
        selectorClip(nullptr);
        const auto frames=itemSelectionFramePlan(r.x,r.y,r.width,r.height,window.decoration,sequence.rotation,sequence.scale,{255,255,255},sequence.alpha);
        for(unsigned f=0;f<frames.count;++f)success=sourceWindowFrame(frames.frames[f]) && success;
        // Description publishes the selected text even while help's own draw
        // transition gate suppresses its contents.
        if(slot==2)window.helpText=p.description(size_t(main.selected));
        if(sequence.flags&4)continue;
        if(slot==0) {
            const auto clip=itemSelectionRowsClip(window);
            for(size_t row=0;row<main.rows.size();++row) {
                const auto draw=itemSelectionRowDraw(main,row);const auto* item=itemPresentation(draw.item);
                if(!item || draw.flags){success=false;continue;} // Used explicit rows are zero flags.
                selectorClip(&clip);success=selectorIcon(item->icon,p.iconTick,draw.iconX,draw.iconY,draw.iconScale,0,true) && success;
                ItemSelectionTextDraw name{draw.nameX,draw.nameY,1,1,draw.nameWidth,draw.nameColor,false};
                WindowFontStyle style{draw.nameColor,p.palette0,2,255,false};
                success=sourceWindowFont(p.name(row),WindowFontConsumer::MessageMatrix,name,style,&clip) && success;
            }
            selectorClip(nullptr);
            if(measurePlainWindowFont(font,p.heading(),2,measured)) {
                auto heading=itemSelectionHeadingDraw(window,measured.width);
                success=sourceWindowFont(p.heading(),WindowFontConsumer::StringMatrix,heading,{0xffffffff,p.palette0,2,255,true},&full) && success;
            } else success=false;
            const auto cursor=itemSelectionCursorDraw(main);success=selectorIcon(cursor.icon,p.iconTick,cursor.x,cursor.y,cursor.scale,cursor.flags) && success;
            const auto arrows=itemSelectionArrowDraws(main,window,uint32_t(p.iconTick));
            for(unsigned i=0;i<arrows.count;++i){const auto& a=arrows.icons[i];success=selectorIcon(a.icon,p.iconTick,a.x,a.y,a.scale,a.flags) && success;}
        } else if(slot==1) {
            if(measurePlainWindowFont(font,p.header(),2,measured)) {
                const auto header=itemSelectionHeaderDraw(window,measured.width,measured.lineFeeds,p.palette0);
                success=sourceWindowFont(p.header(),WindowFontConsumer::MessageMatrix,header,{p.palette0,p.palette0,2,255,false},&full) && success;
                itemSelectionHeaderAfterDraw(window,measured.lineFeeds);
            } else success=false;
        } else {
            const ItemSelectionClip clip{0,384,608,56};const auto help=itemSelectionHelpDraw(window,p.palette0);
            success=sourceWindowFont(window.helpText,WindowFontConsumer::MessageDirect,help,{p.palette0,p.palette0,2,255,false},&clip) && success;
            const auto icons=itemSelectionHelpIconDraws(window,uint32_t(p.iconTick));
            for(unsigned i=0;i<icons.count;++i){const auto& a=icons.icons[i];success=selectorIcon(a.icon,p.iconTick,a.x,a.y,a.scale,a.flags) && success;}
        }
    }
    selectorClip(nullptr);return success;
}
}
