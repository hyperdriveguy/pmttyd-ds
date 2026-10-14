#pragma once
#include "camera_manager.hpp"
namespace ttyd {
enum class LetterboxQuery {BeroExec,Sequence,MarioKey,MarioCtrl,PartyFlags,Rate};
struct CameraLetterboxServices {
    void* user=nullptr;
    // Null source pointer is a successful zero token; nullopt is unavailable.
    std::optional<uintptr_t> (*partySlot0)(void*)=nullptr;
    std::optional<int32_t> (*query)(void*,LetterboxQuery,uintptr_t capturedParty)=nullptr;
    bool (*soundFlag)(void*,bool set,uint16_t mask)=nullptr;
};
struct CameraLetterboxCursor {
    bool active=false;uint8_t stage=0;uintptr_t party=0;uint32_t pending=0;
};
// Original automatic decisions, scalar animation and borrowed registration.
// Availability holds retain preceding stores and the captured party pointer.
bool updateCameraLetterbox(CameraState&,DisplayQueue&,const CameraLetterboxServices&,CameraLetterboxCursor&);
}
