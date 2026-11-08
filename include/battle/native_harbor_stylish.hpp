#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct EventNativeRuntime;struct EventEntry;
struct NativeHarborStylishCursor {
 uint32_t battle=0,event=0,command=0,pending=0;
 bool started=false,poisoned=false,invoking=false;
};
// Native simulation-visit policy over original stylish work/physical trigger
// history. Unknown source inputs hold before mutation; no frame counter/touch
// remapping or automatic success. Source debug-display request is unsupported.
std::optional<int32_t>nativeHarborStylishStart(EventNativeRuntime&,EventEntry&,bool first,bool physicalCurrent,NativeHarborStylishCursor&);
std::optional<int32_t>nativeHarborStylishResult(EventNativeRuntime&,EventEntry&);
// Exact PointInc prefix and genuine zero star-power gate; no enabled-reel ack.
bool nativeHarborPointIncrementZeroDomain(EventNativeRuntime&);
}
