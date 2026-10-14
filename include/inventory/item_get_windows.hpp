#pragma once
#include "item_selection_runtime.hpp"
#include "message_assets.hpp"
#include "window_font.hpp"
#include <memory>
#include <string>
namespace ttyd {
// Finite valid US direct-message col/red consumers. Unknown/malformed commands
// are refused before any glyph/RNG requests; original tokens remain retained.
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
}
