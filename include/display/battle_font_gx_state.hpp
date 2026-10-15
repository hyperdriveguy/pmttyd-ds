#pragma once
#include "battle_font_gpu.hpp"
#include "source_font_draw_start.hpp"
namespace ttyd {
struct BattleFontGxBinding {
 BattleFontGlyphSink* sink=nullptr;
 BattleFontLease font;
 std::shared_ptr<const void> resources;
 std::function<bool()> sourceCurrent,publicationCurrent;
 // Must prove this exact source Init/Setup static object is backed by the
 // actual loaded font allocation and this actual resident atlas generation.
 std::function<bool(uint32_t,const BattleFontLease&)> staticTextureCurrent;
};
// Native coverage masks, not guest GX registers:18 fixed setters and four
// descriptor/format lanes. Matrices/color/fog must be supplied independently.
struct BattleFontGxSnapshot {
 uint32_t fixedState=0,descriptors=0,formats=0;
 bool unsupported=false;
 std::optional<BattleActMatrix> textureMatrix,positionMatrix;
 std::optional<uint32_t> color,currentMatrix;
 std::optional<SourceFontDrawStartFog> fog;
};
// Finite ordinary original-US GX backend state, not a request recorder. Its
// state directly gates the real glyph sink; load/TEV writes reach that sink.
// It does not acknowledge source parents or outer queued GPU publication.
class BattleFontGxState {
public:
 BattleFontGxState()=default;
 BattleFontGxState(const BattleFontGxState&)=delete;
 BattleFontGxState& operator=(const BattleFontGxState&)=delete;
 bool bind(BattleFontGxBinding);
 bool request(const SourceFontStateCall&);
 bool fog(const SourceFontDrawStartFog&);
 bool loadMatrix(const BattleActMatrix&,uint32_t slot);
 bool tevColor(uint32_t slot,uint32_t rgba);
 // Ready only for the exact admitted US profile and current static texture.
 // Inspection emits no packets/RNG; unsupported profiles hold until a genuine
 // first DrawStart setter rebuilds state. Lost ownership stays stale.
 bool ordinaryProfile()const;
 std::optional<BattleFontGxSnapshot> snapshot()const;
 void retire();
private:
 BattleFontGxBinding binding_;
 BattleFontGxSnapshot state_;
 bool bound_=false;mutable bool invoking_=false,stale_=false;
 bool source()const;
 bool texture()const;
 bool load(const BattleActMatrix&,uint32_t);
 bool color(uint32_t,uint32_t);
};
}
