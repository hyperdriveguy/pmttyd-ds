#pragma once
#include "source_icon_matrix.hpp"
namespace ttyd {
// Original source units; flags/icon narrow to16 bits, alpha to8. Scale is
// uniform model scale; the GPU adapter performs world conversion separately.
struct SourceIconPositionRequest {
    uint32_t flags=0,icon=0,alpha=255;float scale=1;std::array<float,3> position{};
};
// Three matrices retain composed translation/rotation/scale plus nested iconGX
// progress. total/frame/remaining describe authored animation delays, not FPS.
struct SourceIconPositionCursor {
    bool active=false,queued=false;uint32_t phase=0,pending=0,camera=0,metadata=0,descriptor=0,step=0,color=0,selected=0,gp=0,total=0,frame=0;
    int32_t remaining=0;
    SourceIconPositionRequest request;SourceMatrixIcon icon;BattleAudienceMatrixRequest matrix;
    std::array<BattleActMatrix,3> matrices{};SourceIconGXCursor gxCursor;
};
// iconDispGx(8001a918) supplies alpha255 to complete iconDispGxAlpha8001a93c.
// Input position is read at translation, after initial camera/loaded-buffer calls.
bool resumeSourceIconPosition(const SourceIconPositionRequest&,const SourceIconMatrixServices&,SourceIconPositionCursor&);
}
