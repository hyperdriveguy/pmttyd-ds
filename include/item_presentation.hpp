#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace ttyd {
// Original item ID indexes this catalog directly. Keys belong to msgSearch,
// not the debug-name field or authored replacement text.
struct ItemPresentation {
    uint32_t nameAddress=0,descriptionAddress=0;
    uint16_t icon=0;
    std::string nameKey,descriptionKey;
    const char* name() const {return nameAddress?nameKey.c_str():nullptr;}
    const char* description() const {return descriptionAddress?descriptionKey.c_str():nullptr;}
};
class ItemPresentationCatalog {
public:
    bool load(const char* path);
    bool assign(const std::vector<uint8_t>& bytes);
    const ItemPresentation* find(uint32_t itemID) const;
    size_t size() const {return items_.size();}
private:
    std::vector<ItemPresentation> items_;
};
bool loadItemPresentations(const char* path);
const ItemPresentation* itemPresentation(uint32_t itemID);
}
