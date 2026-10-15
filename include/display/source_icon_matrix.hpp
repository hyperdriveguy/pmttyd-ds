#pragma once
#include "source_icon_gx.hpp"
#include <optional>
namespace ttyd {
// Providers borrow user/gx only during the synchronous resume call. The
// cursor retains values and nested progress, not the stack callback context.
struct SourceIconMatrixServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
    std::optional<uint32_t> (*currentCamera)(void*)=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*draw)(void*,const BattleActMatrix&,const SourceMatrixIcon&)=nullptr;
    const SourceIconGXServices* gx=nullptr;
};
// phase/pending identify exact retry boundary; matrix result survives a later
// geometry hold. Completion clears active and pending, not source register state.
struct SourceIconMatrixCursor {
    bool active=false;uint32_t phase=0,pending=0,camera=0,metadata=0,descriptor=0;
    BattleAudienceIconRequest request;SourceMatrixIcon icon;
    BattleAudienceMatrixRequest matrix;SourceIconGXCursor gxCursor;
};
// Whole local iconDispGx2(8001a6e0) / iconDispGxCol(8001a4bc).
// Packed source globals and GP are explicit inputs. Acknowledges loaded-buffer
// early returns, but never treats a missing provider as a completed draw.
bool resumeSourceIconMatrix(const BattleAudienceIconRequest&,const SourceIconMatrixServices&,SourceIconMatrixCursor&);
}
