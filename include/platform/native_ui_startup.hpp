#pragma once
#include <memory>
#include <functional>
#include "native_field_animation.hpp"
#include "native_source_font_texture.hpp"
namespace ttyd {
// Native ownership/admission boundaries, not source function identities or
// readiness flags. Null callback admits the already-proved native process scope.
enum class NativeUiStartupBoundary {HeapTransfer,DvdDelivery};
struct NativeUiStartupAdmission {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*admit)(void*,NativeUiStartupBoundary,uint32_t address,uint32_t bytes)=nullptr;
};
// One actual cold field-animation process's source UI startup prefix. It owns
// no Game/Renderer and never acknowledges seq_logoExit, CameraMain or a queue.
class NativeUiStartup:public std::enable_shared_from_this<NativeUiStartup> {
public:
 // Must precede live pose entry/release. Successful cold mappings are permanent
 // process history; failure/retire are native ownership events,not guest resets.
 static std::shared_ptr<NativeUiStartup>create(std::shared_ptr<NativeFieldAnimation>,const char*romfsRoot,NativeUiStartupAdmission={});
 ~NativeUiStartup();
 NativeUiStartup(const NativeUiStartup&)=delete;NativeUiStartup&operator=(const NativeUiStartup&)=delete;
 // Whole original FontManager, real source GP US initialization contribution,
 // actual process heap, real finite ROMFS DVD deliveries and existing SDK body.
 bool resumeFont();uint32_t pending()const;
 bool current()const;
 std::optional<uint32_t>language()const; // LIVE actual process GP+16c, never1 fallback.
 std::optional<NativeSourceFontTextureDescription>fontObject()const;
 std::shared_ptr<NativeSourceFontMemory>fontMemory()const;
 std::shared_ptr<NativeFieldAnimation>fieldProcess()const;
 std::shared_ptr<const void>lifetime()const;
 // Read only actual first-bound source history/immutable imported data.
 std::optional<uint32_t>word(uint32_t)const;
 // Native test/diagnostic counts:actual child effects, not simulation clocks.
 struct Counts {uint32_t gpStores=0,opens=0,reads=0,closes=0,heapAllocations=0,heapTransfers=0;};
 Counts counts()const;
 // Makes later admission unavailable. Source heap and successful cold RAM stay
 // process-owned; this does not run guest free,close a battle or restore a save.
 void retire();
private:
 NativeUiStartup();struct State;std::shared_ptr<State>state_;bool invoking_=false;
};
}
