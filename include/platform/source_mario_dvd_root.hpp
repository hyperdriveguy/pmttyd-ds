#pragma once
#include <cstdint>
namespace ttyd {
// Original _init_registers initializes r2 to this ABI base. Native callers may
// supply it only for the admitted original address domain, not as a disk path.
constexpr uint32_t OriginalMarioDvdSourceR2=0x80427440;
// Whole getMarioStDvdRoot80007388/8: addi r3,r2,0x8054; blr.
// No source memory read, mutable state, cache/root readiness or OS-cwd query.
uint32_t sourceMarioDvdRoot(uint32_t actualSourceR2);
}
