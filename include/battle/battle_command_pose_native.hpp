#pragma once
#include <cstdint>
namespace ttyd {
class EventNativeRuntime;
// Reuses whole original CommandAnimPose with actual VM status and byte-copy.
// Missing name/destination admission returns false BEFORE any guest write.
// Caller holds actual unit/part/table storage stable for this synchronous call.
bool nativeBattleCommandPose(EventNativeRuntime&,uint32_t unit);
}
