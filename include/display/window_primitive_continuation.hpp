#pragma once
#include "battle_audience_aux_callback.hpp"
#include <memory>
namespace ttyd {
enum class WindowPrimitiveRequestKind {Call,FifoFloat};
struct WindowPrimitiveRequest {
    WindowPrimitiveRequestKind kind=WindowPrimitiveRequestKind::Call;
    uint32_t function=0,callsite=0;
    std::vector<uint32_t> integers;
    std::vector<float> reals;
    std::optional<BattleActMatrix> matrix;
    std::optional<uint32_t> color;
};
struct WindowPrimitiveServices {
    void* user=nullptr;
    bool (*current)(void*)=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint32_t> (*request)(void*,const WindowPrimitiveRequest&)=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
};
class WindowPrimitiveCursor {
public:
    WindowPrimitiveCursor();~WindowPrimitiveCursor();
    WindowPrimitiveCursor(const WindowPrimitiveCursor&)=delete;
    WindowPrimitiveCursor& operator=(const WindowPrimitiveCursor&)=delete;
    bool reset();uint32_t pending()const;
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend bool resumeOriginalWindowPrimitive(uint32_t,const uint32_t&,float,float,float,float,float,const WindowPrimitiveServices&,WindowPrimitiveCursor&);
};
// Whole windowDispGX_Waku_col8007b44c/68c. Actual color reference and context
// stay alive across holds; color is loaded only at the original late site.
// Every request/matrix provider must retain its own partial work on false.
bool resumeOriginalWindowPrimitive(uint32_t type,const uint32_t& color,float x,float y,float width,float height,float border,const WindowPrimitiveServices&,WindowPrimitiveCursor&);
}
