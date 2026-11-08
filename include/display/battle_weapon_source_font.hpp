#pragma once
#include "battle_font_gx_state.hpp"
#include "source_font_width.hpp"
#include "source_string_font.hpp"
#include "source_scalar_font.hpp"
#include "source_gx_setter_routing.hpp"
namespace ttyd {
// Actual original child arguments, with explicit known native private text.
// Private bytes are not guest addresses and never replace an unknown guest NUL.
struct BattleWeaponSourceFontCall {
 uint32_t function=0;
 std::vector<uint32_t>integers;std::vector<float>reals;
 std::optional<BattleActMatrix>matrix;
 std::optional<std::array<uint8_t,2>>privateText;
 // Actual retained native private-buffer owner (e.g. DrawHelp), preserving
 // unknown cells/fresh reads; never a guest stack-address substitute.
 std::optional<SourceMatrixFontText>sourceText;
};
struct BattleWeaponSourceFontBinding {
 BattleActorServices actors;BattleFontLease font;BattleFontRasterView raster;
 std::shared_ptr<const void>resources;size_t capacity=0;
 std::function<bool()>sourceCurrent,publicationCurrent;
 std::function<std::optional<uint8_t>(uint32_t)>byte;
 std::function<std::optional<uint16_t>(uint32_t)>half;
 std::function<std::optional<uint32_t>()>camera;
 std::function<bool(BattleAudienceMatrixRequest&)>matrix;
 std::function<std::optional<int32_t>(float)>truncate;
 std::function<std::optional<float>(float,float,float)>fma;
 std::function<bool(uint32_t,const BattleFontLease&)>staticTextureCurrent;
 // Must retain the immutable actual inherited snapshot for long publication.
 // A false admission precedes RNG. This callback is not a source child ack.
 std::function<bool()>ordinaryCurrent;
 std::function<uint32_t(uint32_t)>random;
 void*orderedUser=nullptr;
 bool(*reserve)(void*,const BattleFontGlyphPacket&)=nullptr;
 void(*commit)(void*,const BattleFontGlyphPacket&)=nullptr;
 std::shared_ptr<SourceGxSetterRouting>gx;
 // Optional actual original markup helpers; absent reached children hold.
 std::optional<int32_t>(*widthCall)(void*,const SourceFontWidthCall&)=nullptr;
 void*widthUser=nullptr;
};
// Concrete original State→DrawStart/GX + Width + centered/Matrix/scalar font
// composition. Source globals and two-RNG glyphs complete once; GPU submission
// remains the outer mixed publication's responsibility.
class BattleWeaponSourceFont {
public:
 static std::unique_ptr<BattleWeaponSourceFont>create(BattleWeaponSourceFontBinding);
 ~BattleWeaponSourceFont();
 BattleWeaponSourceFont(const BattleWeaponSourceFont&)=delete;
 BattleWeaponSourceFont&operator=(const BattleWeaponSourceFont&)=delete;
 std::optional<uint32_t>request(const BattleWeaponSourceFontCall&);
 uint32_t pending()const;
 void retire();
private:
 BattleWeaponSourceFont()=default;struct State;std::unique_ptr<State>state_;
 bool invoking_=false;
};
}
