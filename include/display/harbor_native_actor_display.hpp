#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct HarborNativeBodyDisplayAttributes {
 uint32_t unit=0,part=0;
};
struct HarborNativeBodyDisplayServices {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
};
// The manual Harbor presentation admits Crump/Goombella1part and Mario3parts.
// This is a native representation bound, not an original source loop limit.
constexpr unsigned HarborNativeMaximumBodyParts=3;
std::optional<HarborNativeBodyDisplayAttributes>harborNativeBodyDisplayAttributes(
 uint32_t unit,const HarborNativeBodyDisplayServices&);
// Source80102c60/64(U104) and80102c6c/70(P1ac) independently suppress ordinary
// queue registration. This does not acknowledge that queue or its shadow pass.
bool harborNativeBodyDrawAllowed(const HarborNativeBodyDisplayAttributes&);
}
