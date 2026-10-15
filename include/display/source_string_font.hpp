#pragma once
#include "source_matrix_font.hpp"
namespace ttyd {
struct SourceStringFontServices {
 SourceMatrixFontServices common;
 // Actual FontGetMessageWidthLine8007554c(text,0), with this stable typed
 // source stream. Missing is not an estimated native label width.
 std::optional<uint32_t>(*width)(void*,const SourceMatrixFontText&,uint32_t lineCountAddress)=nullptr;
};
// Holds borrowed base/text objects and fixed services until complete. Context
// loss latches stale; no missing text byte becomes NUL. Explicit reset abandons
// retained work without undoing emitted glyphs or temporary source color writes.
class SourceStringFontCursor {
public:
 SourceStringFontCursor();~SourceStringFontCursor();
 SourceStringFontCursor(const SourceStringFontCursor&)=delete;
 SourceStringFontCursor&operator=(const SourceStringFontCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceStringFont(const BattleActMatrix&,const SourceMatrixFontText&,const SourceStringFontServices&,SourceStringFontCursor&);
};
// Whole centered FontDrawString80076a10/2cc. This is not StringMatrix80076cdc:
// the width child centers the pen; the drawing loop maps raw bytes, including
// '<' and newline, without parsing commands. Genuine children remain required.
bool resumeSourceStringFont(const BattleActMatrix&,const SourceMatrixFontText&,const SourceStringFontServices&,SourceStringFontCursor&);
}
