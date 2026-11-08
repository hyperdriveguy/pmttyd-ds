#pragma once
#include "source_window_texture_setup.hpp"
namespace ttyd {
// Shared sparse private sprintf storage; only actually written cells are known.
using SourceIconPath=SourceWindowTexturePath;
struct SourceIconSetupCall {
 uint32_t function=0,callsite=0;std::array<uint32_t,5>arguments{};uint8_t count=0;
 SourceIconPath*destination=nullptr;const SourceIconPath*path=nullptr;
};
struct SourceIconSetupServices {
 // The actual optional memory/wholeUnpack children are reused unchanged.
 SourceWindowTextureSetupServices common;
 std::optional<uint32_t>(*request)(void*,const SourceIconSetupCall&)=nullptr;
};
class SourceIconSetupCursor {
public:
 SourceIconSetupCursor();~SourceIconSetupCursor();
 SourceIconSetupCursor(const SourceIconSetupCursor&)=delete;
 SourceIconSetupCursor&operator=(const SourceIconSetupCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;uint32_t childPending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumeSourceIconSetupEntry(uint32_t,uint32_t,const SourceIconSetupServices&,SourceIconSetupCursor&);
};
// Whole iconInit8001b698/178, _callback_bin8001b810/2c and
// _callback_tpl8001b83c/40. Init schedules its actual two async jobs; it does
// not call its own callbacks or synthesize readiness. Callback arg1 ignored;
// callbackSecondArgument is actual DVD callback userdata, not the file handle.
bool resumeSourceIconSetup(const SourceIconSetupServices&,SourceIconSetupCursor&);
bool resumeSourceIconBinCompletion(uint32_t callbackSecondArgument,const SourceIconSetupServices&,SourceIconSetupCursor&);
bool resumeSourceIconTplCompletion(uint32_t callbackSecondArgument,const SourceIconSetupServices&,SourceIconSetupCursor&);
}
