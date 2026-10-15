#pragma once
#include "source_palette_texture_object.hpp"
#include <map>
namespace ttyd {
// Source's private sprintf destination; no invented guest SP or zeroed suffix.
struct SourceWindowTexturePath {std::map<uint32_t,uint8_t> cells;std::optional<uint8_t> byte(uint32_t offset)const;};
struct SourceWindowTextureSetupCall {
 uint32_t function=0,callsite=0;std::array<uint32_t,5> arguments{};uint8_t count=0;
 SourceWindowTexturePath* destination=nullptr;const SourceWindowTexturePath* path=nullptr;
};
struct SourceWindowTextureSetupServices {
 SourcePaletteTextureObjectServices objects;
 // Exact whole original archive/root/formatter/DVD/heap/assert children.
 // Nullopt is unavailable; engaged zero is an actual return, not a hold.
 // Each child owns its effects through waits. All contexts are pinned by gx.lifetime.
 std::optional<uint32_t>(*request)(void*,const SourceWindowTextureSetupCall&)=nullptr;
};
class SourceWindowTextureSetupCursor {
public:
 SourceWindowTextureSetupCursor();~SourceWindowTextureSetupCursor();
 SourceWindowTextureSetupCursor(const SourceWindowTextureSetupCursor&)=delete;
 SourceWindowTextureSetupCursor&operator=(const SourceWindowTextureSetupCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;uint32_t childPending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceWindowTextureEntry(uint32_t,uint32_t,const SourceWindowTextureSetupServices&,SourceWindowTextureSetupCursor&);
};
bool resumeSourceWindowTextureSetup(const SourceWindowTextureSetupServices&,SourceWindowTextureSetupCursor&);
// Original first callback argument is ignored. secondArgument is actual DVD callback userdata.
bool resumeSourceWindowTextureCompletion(uint32_t secondArgument,const SourceWindowTextureSetupServices&,SourceWindowTextureSetupCursor&);
bool resumeSourceUnpackTexturePalette(uint32_t palette,const SourceWindowTextureSetupServices&,SourceWindowTextureSetupCursor&);
}
