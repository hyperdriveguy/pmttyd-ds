#pragma once
#include "battle_audience_draw.hpp"
#include "battle_audience_resources.hpp"
namespace ttyd {
struct BattleAudienceVertex {float x=0,y=0,z=0,u=0,v=0;uint32_t rgba=0;};
// Native SDK/libm numerical adapters; source matrices/operation order live in
// the caller. These do not claim paired-single/SDK rounding parity.
void bindAudienceMathAdapters(BattleAudienceDrawServices&);
// Owns per-build CPU vertices/request journal. begin clears presentation
// buffers only; it does not advance audience simulation or release source assets.
class BattleAudienceBackend {
public:
    const BattleAudienceResources* assets=nullptr;
    std::vector<BattleAudienceVertex> vertices;
    std::vector<BattleAudienceGXRequest> requests;
    void begin(){vertices.clear();requests.clear();}
    bool request(const BattleAudienceGXRequest&);
    bool emitQuad(const BattleAudienceQuad&);
};
}
