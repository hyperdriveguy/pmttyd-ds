#include "platform.hpp"
#ifdef __3DS__
namespace ttyd {
// Bind by actual resident Room identity and matching vertex count, not name.
// Revision is the simulation owner's publication token: copy/flush only when
// it changes, leaving opaque/translucent views on the same prepared geometry.
const StageMapObjects* Renderer::updateStageMap(const Game& game,Room& room) {
    if(!game.trioBattleActive || (&room!=&trioBackground && &room!=&trioStage))return nullptr;
    const auto* objects=game.trioAttack.stageMap(&room==&trioBackground);
    if(!objects || objects->vertices()!=room.vertexCount)return nullptr;
    if(room.stageMapRevision!=objects->revision()) {
        objects->apply([&](uint32_t index,const StageMapObjects::Point& p){auto& v=room.vertices[index];v.x=p[0];v.y=p[1];v.z=p[2];});
        GSPGPU_FlushDataCache(room.vertices,room.vertexCount*sizeof(Vertex));
        room.stageMapRevision=objects->revision();
    }
    return objects;
}
}
#endif
