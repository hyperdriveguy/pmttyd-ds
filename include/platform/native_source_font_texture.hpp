#pragma once
#include <memory>
#include <functional>
#include "native_source_font_memory.hpp"
#include "source_font_setup.hpp"
#include "source_gx_texture_object.hpp"
namespace ttyd {
class EventNativeRuntime;
struct NativeSourceFontTextureDescription {
 uint32_t object=0,font=0,glyphs=0,image=0;
 uint16_t width=0,height=0;
 std::array<uint8_t,32> objectBytes{};
};
// One actual VM font binding and one source Setup's linked Init/LOD pair.
// Binding/owner MUST retire before the borrowed VM is destroyed, outside RAM
// borrows. lifetime pins actual context/storage, current is a pure long-lived
// allocation predicate (not the short executing-camera predicate).
class NativeSourceFontTexture {
public:
 static std::unique_ptr<NativeSourceFontTexture> bind(EventNativeRuntime&,
  std::shared_ptr<NativeSourceFontMemory>,std::shared_ptr<const void> lifetime,
  std::function<bool()> current);
 ~NativeSourceFontTexture();
 NativeSourceFontTexture(const NativeSourceFontTexture&)=delete;
 NativeSourceFontTexture&operator=(const NativeSourceFontTexture&)=delete;
 // Only actual original font Setup SDK call sites and current source operands.
 // Missing providers retain original work. Engaged0 is a completed void child.
 std::optional<uint32_t> request(const SourceFontSetupCall&);
 // CPU source object proof ONLY; not whole Setup completion, native texture
 // import, matching pixels, GPU residency/frame/queued callback readiness.
 std::optional<NativeSourceFontTextureDescription> object()const;
 bool stale()const;uint32_t pending()const;
 void retire();
private:
 NativeSourceFontTexture();struct State;std::unique_ptr<State>state_;bool invoking_=false,retiring_=false;
};
}
