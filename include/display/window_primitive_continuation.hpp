#pragma once
#include "battle_audience_aux_callback.hpp"
#include <memory>
namespace ttyd {
enum class WindowPrimitiveRequestKind {Call,FifoFloat};
// Call retains original function/site; FifoFloat has one captured real and
// original stfs site, pending FIFO token cc008000. Engaged return0 completes
// either request; nullopt holds without replacing already captured operands.
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
// One invocation pins provider/color identities and all five float BIT
// patterns. Completed terminal PC remains until reset; reset has no guest effects.
// Real context/color lifetime and serialized source mutation belong to caller.
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
