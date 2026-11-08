#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct EventNativeRuntime;struct EventEntry;
// Actual shared-VM GetResultPrizeLv wrapper. Core grade arithmetic is supplied
// by the existing original actionPrizeLevel implementation, not inferred from AC.
std::optional<int32_t> nativeHarborActionPrize(EventNativeRuntime&,EventEntry&);
}
