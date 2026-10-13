#pragma once
#include <cstdint>
namespace ttyd {
// Ordinary room initialization from US gor.rel. Scene-controlled ship motion
// and wake activation require subsequent event overrides; see behavior contract.
inline bool mapEventGroupVisible(uint32_t group,int32_t story) {
    switch(group) {
    case 0: return true;
    case 1: return false;
    case 2: return story>=268 && story!=402 && story!=404;
    case 3: return story>=172 && story<=229 && story!=217;
    case 4: return story<164;
    case 5: return story>=164 && story<339;
    case 6: return story>=339;
    case 7: return story>=23;
    case 8: return story<10;
    default: return false;
    }
}
}
