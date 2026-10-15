#pragma once
#include <memory>
#include <map>
#include <vector>
#include "source_matrix_font.hpp"
namespace ttyd {
// Typed original private sprintf destination. Unknown stack suffix stays absent;
// a real formatter writes exactly its source bytes/NUL, preserving old suffix.
struct SourceFontPath {
 std::map<uint32_t,uint8_t> cells;
 std::optional<uint8_t> byte(uint32_t offset)const;
};
struct SourceFontSetupCall {
 uint32_t function=0,callsite=0;
 std::vector<uint32_t> integers;
 std::vector<float> reals;
 SourceFontPath* formatDestination=nullptr;
 const SourceFontPath* path=nullptr;
};
struct SourceFontSetupServices {
 BattleActorServices actors;void* user=nullptr;
 std::shared_ptr<const void> lifetime;
 bool (*current)(void*)=nullptr;
 // Complete actual root/sprintf/DVD/heap/GX children, with their real writes,
 // ownership and failure behavior. Engaged zero is a real returned zero.
 std::optional<uint32_t> (*request)(void*,const SourceFontSetupCall&)=nullptr;
};
class SourceFontSetupCursor {
public:
 SourceFontSetupCursor();~SourceFontSetupCursor();
 SourceFontSetupCursor(const SourceFontSetupCursor&)=delete;
 SourceFontSetupCursor&operator=(const SourceFontSetupCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceFontSetup(uint32_t,const SourceFontSetupServices&,SourceFontSetupCursor&);
 friend bool resumeSourceFontManagerSetup(const SourceFontSetupServices&,SourceFontSetupCursor&);
 friend bool resumeSourceFontSetupEntry(uint32_t,uint32_t,const SourceFontSetupServices&,SourceFontSetupCursor&);
};
// Whole JUTFontSetup80075004/2f8 and actual fontmgrTexSetup80077bdc/2c caller.
// Neither constructor nor entry invents BSS/global font initialization.
bool resumeSourceFontSetup(uint32_t selector,const SourceFontSetupServices&,SourceFontSetupCursor&);
bool resumeSourceFontManagerSetup(const SourceFontSetupServices&,SourceFontSetupCursor&);
}
