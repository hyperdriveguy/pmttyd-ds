#include "file_texture_relocation.hpp"
namespace ttyd {
namespace {
bool same(const FileTextureRelocationServices& a,
          const FileTextureRelocationServices& b) {
  return a.memory.user == b.memory.user && a.memory.word == b.memory.word &&
      a.memory.storeWord == b.memory.storeWord &&
      a.memory.storeByte == b.memory.storeByte && a.byte == b.byte &&
      a.user == b.user && a.panic == b.panic;
}
} // namespace
bool resumePackedFileTextureRelocation(uint32_t base,
    const FileTextureRelocationServices& s, FileTextureRelocationCursor& c) {
  const auto& m = s.memory;
  if (c.active) {
    if (base != c.base || !same(s, c.owner))
      return false;
  } else if (m.word(m.user, base) != 0x0020af30) {
    c.active = true;
    c.base = base;
    c.owner = s;
  }
  if (c.active && (!s.panic || !s.panic(s.user, 0x802f9470, 112, 0x802f947c)))
    return false;
  const auto table = m.word(m.user, base + 8);
  if (table < base) {
    uint32_t index = 0;
    m.storeWord(m.user, base + 8, table + base);
    while (uint32_t(uint16_t(index)) < m.word(m.user, base + 4)) {
      uint32_t offset = uint32_t(uint16_t(index)) * 8;
      // Keep slot address captured for the original stwx; all subsequent table
      // dereferences reload the root, including the final marker destination.
      for (const uint32_t flag : {0x23u, 2u}) {
        const auto slot = m.word(m.user, base + 8) + offset;
        const auto relative = m.word(m.user, slot);
        if (relative) {
          m.storeWord(m.user, slot, relative + base);
          const auto descriptor = m.word(m.user,
              m.word(m.user, base + 8) + offset);
          if (!s.byte(m.user, descriptor + flag)) {
            const auto data = m.word(m.user, descriptor + 8);
            m.storeWord(m.user, descriptor + 8, data + base);
            const auto current = m.word(m.user,
                m.word(m.user, base + 8) + offset);
            m.storeByte(m.user, current + flag, 1);
          }
        }
        offset += 4;
      }
      ++index;
    }
  }
  c = {};
  return true;
}
} // namespace ttyd
