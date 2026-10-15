#pragma once
#include <memory>
#include "battle_sub_icon_continuation.hpp"
#include "battle_audience_matrix.hpp"
namespace ttyd {
// Exact original linked request. Native private digit text and guest message
// addresses are distinct; no address is invented for a source stack local.
struct BattleSubIconRasterCall {
 uint32_t function=0,callsite=0;
 std::vector<uint32_t> integers;
 std::vector<float> reals;
 std::optional<BattleActMatrix> matrix;
 std::optional<std::array<uint8_t,2>> privateText;
};
struct BattleSubIconRasterServices {
 BattleActorServices actors;void* user=nullptr;
 std::shared_ptr<const void> lifetime; // Actual immutable provider/context owner.
 bool (*current)(void*)=nullptr;
 // Whole original linked GX/font/icon/msg child. In particular80076cdc must
 // retain FontDrawStringMtx effects;80076a10 is a different source function.
 std::optional<uint32_t> (*request)(void*,const BattleSubIconRasterCall&)=nullptr;
 bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
class BattleSubIconRasterCursor {
public:
 BattleSubIconRasterCursor();~BattleSubIconRasterCursor();
 BattleSubIconRasterCursor(const BattleSubIconRasterCursor&)=delete;
 BattleSubIconRasterCursor&operator=(const BattleSubIconRasterCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumePackedBattleSubIconRaster(const BattleSubIconRasterRequest&,const BattleSubIconRasterServices&,BattleSubIconRasterCursor&);
};
// Whole DrawSubIconSub8011847c/ba8. The borrowed request/base identity is
// immutable and actual provider contexts/assets outlive every held invocation.
bool resumePackedBattleSubIconRaster(const BattleSubIconRasterRequest&,const BattleSubIconRasterServices&,BattleSubIconRasterCursor&);
}
