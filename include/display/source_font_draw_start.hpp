#pragma once
#include <memory>
#include "source_font_state.hpp"
namespace ttyd {
struct SourceFontDrawStartInput {
 uint32_t guestColor=0;
 // Native equivalent of actual caller-owned local; read only at source4d34.
 // Null selects original guest pointer, including actual guest0 domain.
 const uint32_t* privateColor=nullptr;
};
// Actual final GX fog child: type0 disables fog; four source float lanes and
// the persistent BSS color remain captured separately from integer operands.
struct SourceFontDrawStartFog {
 uint32_t function=0x802b8ff4,callsite=0x80074e74,type=0,color=0;
 std::array<float,4> reals{};
};
struct SourceFontDrawStartServices {
 SourceFontStateServices common;
 std::optional<uint32_t> (*camera)(void*)=nullptr;
 bool (*fog)(void*,const SourceFontDrawStartFog&)=nullptr;
};
// Captures pointer identity, not early color/view values. Context, private
// color and provider lifetimes survive holds. Stale ownership cannot revive;
// reset is metadata only and cannot undo already completed GX children.
class SourceFontDrawStartCursor {
public:
 SourceFontDrawStartCursor();~SourceFontDrawStartCursor();
 SourceFontDrawStartCursor(const SourceFontDrawStartCursor&)=delete;
 SourceFontDrawStartCursor&operator=(const SourceFontDrawStartCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceFontDrawStart(const SourceFontDrawStartInput&,const SourceFontDrawStartServices&,SourceFontDrawStartCursor&);
};
// Whole original JUTFont_DrawStart80074bfc/294. Ordinary GX requests and SDK
// scale reuse actual SourceFontStateServices. Texture8041e97c is an ORIGINAL
// STATIC OBJECT ADDRESS, not a loaded pointer or arbitrary native atlas ID.
// Capturing requests is insufficient acknowledgment: linked owners must retain
// all actual completed GX work through a wait and prove real texture residency.
bool resumeSourceFontDrawStart(const SourceFontDrawStartInput&,const SourceFontDrawStartServices&,SourceFontDrawStartCursor&);
bool resumeSourceFontDrawStart(const uint32_t& privateColor,const SourceFontDrawStartServices&,SourceFontDrawStartCursor&);
}
