#pragma once
#include <memory>
#include <functional>
#include "native_field_animation.hpp"
#include "native_source_font_texture.hpp"
#include "source_gx_inherited_state.hpp"
#include "native_source_window_memory.hpp"
#include "animation_texture.hpp"
namespace ttyd {
// Native ownership/admission boundaries, not source function identities or
// readiness flags. Null callback admits the already-proved native process scope.
enum class NativeUiStartupBoundary {HeapTransfer,DvdDelivery,WindowArchive,WindowJob,WindowCallback};
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
 // Original Setup acknowledgment is job admission; completion is a separate
 // actual file-delivery/callback pump. resumeWindow composes them serially.
 bool beginWindow();bool pumpWindow();bool resumeWindow();bool windowReady()const;
 struct WindowObject {uint32_t palette=0,address=0,index=0;std::array<uint8_t,32>bytes{};AnimationTPLImage image;};
 std::shared_ptr<NativeSourceWindowMemory>windowMemory()const;
 std::optional<WindowObject>windowObject(uint32_t index)const;
 struct WindowCounts {uint32_t archiveDescriptors=0,opens=0,heapAllocations=0,heapTransfers=0,jobs=0,reads=0,callbacks=0,closes=0,objectClears=0;};
 WindowCounts windowCounts()const;
 // First refusal includes unknown/retired intersections. Reads expose only
 // actual current process resources, including genuine sourceReady0 history.
 bool ownsSourceResource(uint32_t address,uint32_t bytes=1)const;
 std::optional<uint8_t>sourceResourceByte(uint32_t)const;
 std::optional<uint16_t>sourceResourceHalf(uint32_t)const;
 std::optional<uint32_t>sourceResourceWord(uint32_t)const;
 bool current()const;
 std::optional<uint32_t>language()const; // LIVE actual process GP+16c, never1 fallback.
 std::optional<NativeSourceFontTextureDescription>fontObject()const;
 std::shared_ptr<NativeSourceFontMemory>fontMemory()const;
 std::shared_ptr<NativeFieldAnimation>fieldProcess()const;
 // Actual process source-GX history; loaded packet snapshots pin independent
 // long proof and survive legitimate later source setters/camera.finish.
 std::shared_ptr<SourceGxInheritedState>gxHistory()const;
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
