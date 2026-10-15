#pragma once
#include "item_selection_runtime.hpp"
#include "message_assets.hpp"
#include "window_font.hpp"
#include <memory>
#include <string>
namespace ttyd {
// Finite valid US direct-message col/red consumers. Unknown/malformed commands
// are refused before any glyph/RNG requests; original tokens remain retained.
// Per-glyph U64 marker: high32=1 original style, 2 palette0 reset, 0 literal
// low32 RGBA. Commands do not form a nested color stack.
struct ColorWindowText {std::string plain;std::vector<uint64_t> glyphColors;};
bool compileColorWindowText(const MessageFont&,const char*,ColorWindowText&);
bool drawColorWindowFont(const MessageFont&,const ColorWindowText&,const WindowFontStyle&,
                        const WindowFontDrawServices&,int32_t x,int32_t y);
bool measureColorWindowFont(const MessageFont&,const char*,uint32_t,WindowFontMeasurement&);
bool drawColorWindowFont(const MessageFont&,const char*,const WindowFontStyle&,
                        const WindowFontDrawServices&,int32_t x,int32_t y);
bool loadItemGetWindowAssets(const char* root);
struct ItemGetWindowDisplay {
 ItemSelectionRuntimeWindow frame; // Snapshot before display callback mutation.
 const char* text=nullptr;
 const ColorWindowText* compiled=nullptr;
 ItemSelectionTextDraw draw;
 bool content=false,help=false;
};
// Retains item95 parameters/text through AutoDelete closure, even after the
// item event restores control. Shared global slot allocation remains external.
// Two supported definitions80313dc8(main)/80313df0(help); failure80313e18
// and full discard selection remain outside this finite owner. Slots retain
// old transform/alpha on reuse while constructor resets only specified fields.
class ItemGetWindows {
public:
 bool prepare(const MessageFont&);
 int32_t enter(uint32_t sourceDefinition,const char* entryName);
 void closeAutoDelete(int32_t slot);
 int32_t action(int32_t slot) const;
 void managerTick(uint32_t buttonRepeat,const ItemSelectionSequenceServices&);
 ItemGetWindowDisplay display(unsigned slot);
 uint32_t displaySlots() const{return displaySlots_;}
 bool retained() const;
 const ItemSelectionRuntimeWindow& window(unsigned slot) const{return windows_.at(slot);}
 const std::string& title() const{return title_;}
 const char* description() const{return description_;}
 uint32_t palette0=0x1e1414ff; // Mutable original font service input.
private:
 std::array<ItemSelectionRuntimeWindow,2> windows_{};
 std::array<bool,2> help_{};
 std::array<std::string,2> entries_{};
 std::shared_ptr<const MessageResource> messages_;
 const MessageFont* font_=nullptr;
 std::string title_;const char* description_=nullptr;
 ColorWindowText titleFont_,descriptionFont_;
 uint32_t displaySlots_=0;
};
// Original coin window80415e84. Parameter remains borrowed until AutoDelete
// retires the window; the coin event may already have returned2.
// action() exposes busy flag4, not action enum. AutoDelete can leave this
// bit set in an inactive slot; retained() independently proves pointer lifetime.
class CoinGetWindow {
public:
 bool prepare(const MessageFont&,int32_t amount=20);
 int32_t enter(uint32_t sourceDefinition,const int32_t* parameter);
 void closeAutoDelete(int32_t slot);
 int32_t action(int32_t slot) const;
 void managerTick(const ItemSelectionSequenceServices&);
 ItemGetWindowDisplay display();
 bool displayed() const{return displayed_;}
 bool retained() const{return (window_.sequence.flags&1)!=0;}
 const ItemSelectionRuntimeWindow& window() const{return window_;}
 const std::string& title() const{return title_;}
 uint32_t palette0=0x1e1414ff;
private:
 bool cacheTitle(int32_t amount);
 ItemSelectionRuntimeWindow window_;
 std::shared_ptr<const MessageResource> messages_;
 const MessageFont* font_=nullptr;
 const int32_t* parameter_=nullptr;
 std::string title_;
 ColorWindowText compiled_;
 WindowFontMeasurement measured_;
 int32_t amount_=0;
 bool displayed_=false;
};
}
