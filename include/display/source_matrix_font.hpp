#pragma once
#include <memory>
#include "battle_audience_matrix.hpp"
#include "window_font.hpp"
namespace ttyd {
struct SourceMatrixFontText {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 // Actual guest bytes or actual native private cells, including known NUL.
 // Unknown/unwritten/outside bytes return nullopt; never insert a terminator.
 std::optional<uint8_t> (*byte)(void*,uint32_t offset)=nullptr;
 // Actual source LHZ lane, required only on reached multibyte route.
 std::optional<uint16_t> (*half)(void*,uint32_t offset)=nullptr;
};
struct SourceMatrixFontGlyph {
 uint16_t glyph=0;uint32_t sourceCode=0;int32_t x=0,y=0;
};
struct SourceMatrixFontServices {
 BattleActorServices actors;void* user=nullptr;std::shared_ptr<const void> lifetime;
 bool (*current)(void*)=nullptr;
 std::optional<uint32_t> (*camera)(void*)=nullptr;
 bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
 bool (*loadMatrix)(void*,const BattleActMatrix&,uint32_t index)=nullptr;
 bool (*tevColor)(void*,uint32_t index,uint32_t rgba)=nullptr;
 // Exact whole JUTFont_DrawPos80074720. Finite original-US atomic owner below.
 bool (*glyph)(void*,const SourceMatrixFontGlyph&)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
 // Actual source guest byte0 only if msgGetCommand returns original null.
 // A private stream cannot replace that faulting read with its own first byte.
 std::optional<uint8_t> (*nullByte)(void*,uint32_t offset)=nullptr;
 std::optional<uint16_t> (*nullHalf)(void*,uint32_t offset)=nullptr;
};
// Shared exact original helpers for the scalar/string siblings. Memory must
// be actual admitted source storage; no synthetic replacement mapping/width.
bool sourceMatrixFontLead(uint32_t code,const BattleActorServices&);
uint16_t sourceMatrixFontMap(uint32_t code,const BattleActorServices&);
uint32_t sourceMatrixFontWidth(uint16_t glyph,const BattleActorServices&);
struct SourceMatrixFontCommandResult {
 bool null=false;uint32_t offset=0;
 std::array<std::optional<uint8_t>,128> name,arguments;
};
class SourceMatrixFontCommandCursor {
public:
 SourceMatrixFontCommandCursor();~SourceMatrixFontCommandCursor();
 SourceMatrixFontCommandCursor(const SourceMatrixFontCommandCursor&)=delete;
 SourceMatrixFontCommandCursor&operator=(const SourceMatrixFontCommandCursor&)=delete;
 bool active()const;bool stale()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend std::optional<SourceMatrixFontCommandResult> resumeSourceMatrixFontCommand(uint32_t,const SourceMatrixFontText&,const SourceMatrixFontServices&,SourceMatrixFontCommandCursor&);
};
// Whole msgGetCommand80081aa0/a0,128-byte bound and true uninitialized output
// cells. Null original return is distinct from unavailable. No fabricated NUL.
std::optional<SourceMatrixFontCommandResult> resumeSourceMatrixFontCommand(uint32_t startOffset,const SourceMatrixFontText&,const SourceMatrixFontServices&,SourceMatrixFontCommandCursor&);
class SourceMatrixFontCursor {
public:
 SourceMatrixFontCursor();~SourceMatrixFontCursor();
 SourceMatrixFontCursor(const SourceMatrixFontCursor&)=delete;
 SourceMatrixFontCursor&operator=(const SourceMatrixFontCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceMatrixFont(const BattleActMatrix&,const SourceMatrixFontText&,const SourceMatrixFontServices&,SourceMatrixFontCursor&);
};
// Whole FontDrawStringMtx80076cdc/358, including its original lead/map/command
// helpers and WID1 advance. Does not substitute FontDrawString80076a10.
bool resumeSourceMatrixFont(const BattleActMatrix&,const SourceMatrixFontText&,const SourceMatrixFontServices&,SourceMatrixFontCursor&);
struct SourceMatrixFontAtomicGlyphServices {
 const MessageFont* font=nullptr;BattleActorServices actors;
 bool* invoking=nullptr; // Stable native owner guard, never guest state.
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 // Actual immutable font/GPU/TEV/matrix owner, including finite US profile.
 bool (*current)(void*)=nullptr;
 // Actual effective TEV color already established by source font state.
 std::optional<uint32_t> (*color)(void*)=nullptr;
 // Pure whole-one-quad admission; after true, callbacks below cannot fail,
 // reenter or change the admitted owner/resources. No RNG before admission.
 bool (*admit)(void*,const WindowFontQuad&)=nullptr;
 uint32_t (*random)(void*,uint32_t)=nullptr;
 void (*emit)(void*,const WindowFontQuad&)=nullptr;
};
// Real finite source glyph raster: validates live original WID/GLY rectangle
// against actual MessageFont, delegates matching one-byte consumer, consumes
// two actual irand64 then emits one admitted quad. Unknown/multibyte/profile
// mismatch rejects before RNG; caller actual current owns profile proof.
bool drawSourceMatrixFontAtomicGlyph(const SourceMatrixFontGlyph&,const SourceMatrixFontAtomicGlyphServices&);
}
