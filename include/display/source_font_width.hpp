#pragma once
#include "source_matrix_font.hpp"
namespace ttyd {
// Logical offsets in the original private frame, never guest addresses. Unknown
// cells remain unknown. One shared array preserves scanf/name/argument aliases.
struct SourceFontWidthPrivate {
 std::array<std::optional<uint8_t>,0x140> bytes{};
 std::optional<uint32_t> read(uint32_t offset,uint32_t size)const;
 bool write(uint32_t offset,uint32_t size,uint32_t value);
};
struct SourceFontWidthCall {
 uint32_t function=0,callsite=0;
 SourceFontWidthPrivate* frame=nullptr;
 // strcmp: {nameOffset, guestLiteral}; scanf: {argsOffset, guestFormat,
 // output0..4}; iconId:{nameOffset}; iconSize:{widthOffset,heightOffset,id16}.
 std::array<uint32_t,7> arguments{};uint8_t count=0;
};
struct SourceFontWidthServices {
 SourceMatrixFontServices common;
 // Whole original child. Holds must retain the same request/private frame and
 // acknowledged effects. Engaged zero is a real return, not unavailable.
 std::optional<int32_t>(*call)(void*,const SourceFontWidthCall&)=nullptr;
 std::optional<float>(*fma)(void*,float,float,float)=nullptr;
};
// Pins borrowed text identity,owners,line destination and service table.
// Successful children/private bytes survive a wait. Sticky context loss needs
// explicit reset; reset discards native progress without rolling back guest STH.
class SourceFontWidthCursor {
public:
 SourceFontWidthCursor();~SourceFontWidthCursor();
 SourceFontWidthCursor(const SourceFontWidthCursor&)=delete;
 SourceFontWidthCursor&operator=(const SourceFontWidthCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend std::optional<uint32_t> resumeSourceFontWidthLine(const SourceMatrixFontText&,uint32_t,const SourceFontWidthServices&,SourceFontWidthCursor&);
};
// Whole FontGetMessageWidthLine8007554c/320. Original optional line-count
// pointer receives an actual STH only when nonzero. Requires admitted original
// actor/BFN/GP memory; optional text/private outputs preserve unavailable reads.
std::optional<uint32_t> resumeSourceFontWidthLine(const SourceMatrixFontText&,uint32_t lineCountAddress,const SourceFontWidthServices&,SourceFontWidthCursor&);
// Whole wrapper80075528/24 passes original null line-count pointer.
std::optional<uint32_t> resumeSourceFontWidth(const SourceMatrixFontText&,const SourceFontWidthServices&,SourceFontWidthCursor&);
}
