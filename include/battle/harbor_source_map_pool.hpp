#pragma once
#include "event_memory_binding.hpp"
#include <memory>
#include <optional>
#include <unordered_map>

namespace ttyd {
// Native address/capacity policy only. Actual allocation, nested source pool,
// splitting, clearing and frees execute the original map allocator helpers.
class HarborSourceMapPool {
public:
    static constexpr uint32_t ArenaAddress = 0x84700000;
    static constexpr uint32_t ArenaBytes = 1024 * 1024;
    static std::unique_ptr<HarborSourceMapPool> bind(EventAddressSpace&,
                                                   const EventMemoryServices&);
    ~HarborSourceMapPool();
    HarborSourceMapPool(const HarborSourceMapPool&) = delete;
    HarborSourceMapPool& operator=(const HarborSourceMapPool&) = delete;
    bool current() const;
    uint32_t root() const;
    // Unavailable owner/root holds. Engaged0 is original no-fit/zero-length.
    std::optional<uint32_t> allocate(uint32_t root, uint32_t bytes);
    bool free(uint32_t root, uint32_t address);
    std::shared_ptr<const uint8_t> allocationLifetime(uint32_t address) const;
private:
    HarborSourceMapPool(EventAddressSpace&, const EventMemoryServices&);
    EventAddressSpace& memory_;
    EventMemoryServices services_;
    std::vector<uint8_t> arena_;
    std::array<uint8_t, 20> globals_{}; // Original8041e808..8041e81b.
    std::unordered_map<uint32_t, std::shared_ptr<const uint8_t>> allocations_;
    uint32_t root_ = 0;
    bool arenaMapped_ = false, globalsMapped_ = false;
};
}
