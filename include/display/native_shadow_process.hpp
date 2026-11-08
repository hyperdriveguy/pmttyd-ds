#pragma once
#include "source_shadow_process.hpp"
#include "source_smart_arena.hpp"
#include <memory>
namespace ttyd {
class NativeUiStartup;
// Actual animation VM/SDK heaps and shared UI literals; CPU bootstrap only.
// No native GPU shadow image, draw-sync or moving-file completion is inferred.
class NativeShadowProcess {
public:
 struct Counts {unsigned heap4Allocations=0,heap0Allocations=0,fills=0,flushes=0,objects=0,lods=0;};
 static std::shared_ptr<NativeShadowProcess> create(std::shared_ptr<NativeUiStartup>,const char* root);
 ~NativeShadowProcess();
 // Native instruction budget, not source ticks. False retains original progress.
 bool initialize(unsigned visits=2*1024*1024);
 bool current()const;bool ready()const;bool busy()const;uint32_t pending()const;
 Counts counts()const;
 std::shared_ptr<NativeUiStartup> ui()const;
 SourceSmartArena* smartArena()const;
 SourceShadowProcess* shadow()const;
private:
 NativeShadowProcess();struct State;std::shared_ptr<State> state_;bool invoking_=false;
};
}
