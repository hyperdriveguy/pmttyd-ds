#include "platform.hpp"
#include "battle_scene_views.hpp"
namespace ttyd {
void Renderer::captureTrioActor(const Game& game,C3D_Tex& texture,uint32_t first,uint32_t size,bool depth){
    if(!game.trioBattleActive || !size)return;
    trioSceneActors.push_back({vertices,texture,first,size,uint32_t(depth?GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE|GPU_WRITE_DEPTH:GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE),GPU_CULL_NONE,depth});
}
void Renderer::lowerTrioScene(const Game& game){
    if(!game.trioBattleActive || !trioScenePrepared)return;
    BattleSceneMatrix topMatrix;
    for(unsigned row=0;row<4;++row){const auto& r=worldTransform.r[row];topMatrix[row]={r.x,r.y,r.z,r.w};}
    const auto continued=lowerBattleSceneMatrix(topMatrix);
    for(unsigned row=0;row<4;++row){const auto& r=continued[row];worldTransform.r[row]=FVec4_New(r[0],r[1],r[2],r[3]);}
    C3D_BindProgram(&program);auto* attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);
    AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,4);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
    C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(unsigned i=0;i<6;++i)C3D_TexEnvInit(C3D_GetTexEnv(i));
    auto* environment=C3D_GetTexEnv(0);C3D_TexEnvSrc(environment,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(environment,C3D_Both,GPU_MODULATE);
    const auto submit=[&](TrioSceneDraw& draw){
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,draw.stream,sizeof(Vertex),3,0x210);
        C3D_TexBind(0,&draw.texture);C3D_CullFace(draw.cull);
        C3D_DepthTest(draw.depth,draw.depth?GPU_GREATER:GPU_ALWAYS,GPU_WRITEMASK(draw.depthMask));
        C3D_DrawArrays(GPU_TRIANGLES,draw.first,draw.count);
    };
    // Sky precedes opaque map, followed by prepared actors, translucent map,
    // then original audience passes. No source reads or geometry writes here.
    if(!trioSceneActors.empty())submit(trioSceneActors[0]);
    for(auto& draw:trioSceneMaps[0])submit(draw);
    for(unsigned i=1;i<trioSceneActors.size();++i)submit(trioSceneActors[i]);
    for(auto& draw:trioSceneMaps[1])submit(draw);
    drawAudienceScene(game,true);
}
}
