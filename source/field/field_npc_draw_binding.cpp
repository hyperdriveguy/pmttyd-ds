#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_main_entry_binding.hpp"
namespace ttyd {
bool FieldNpcMainEntryBinding::advanceDraw(uint32_t callback, uint32_t actor,
                                           FieldNpcDrawCursor &c) {
  if ((c.active && c.owner.transform.user != this) ||
      !poses_.aliases(*owner_->poses().sharedPool(), vm_.memory))
    return false;
  FieldNpcDrawServices s;
  s.transform.user = this;
  s.transform.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
    uint32_t w;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readWord(a, w))
      return {};
    return w;
  };
  s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
    uint8_t w;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readByte(a, w))
      return {};
    return w;
  };
  s.transform.number = [](void *p, FieldNpcMatrixNumber &q) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.drawNumber &&
           b.native_.drawNumber(b.native_.user, b.vm_.memory, q);
  };
  s.transform.matrix = [](void *p, BattleAudienceMatrixRequest &q) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.drawMatrix &&
           b.native_.drawMatrix(b.native_.user, b.vm_.memory, q);
  };
  s.request = [](void *p, const FieldNpcDrawRequest &q,
                 FieldNpcDrawResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x80039cf8 || q.function == 0x8026d464) {
      const auto common = b.services();
      return common.request(p, {q.function, {q.args[0], q.args[1]}}, out.word);
    }
    if (q.function == 0x800406b4 || q.function == 0x80040730) {
      if (q.args[0] >= 256 ||
          !b.poses_.aliases(*b.owner_->poses().sharedPool(), m))
        return false;
      const auto pose = b.poses_.poseAddress(q.args[0]);
      uint32_t flags;
      if (q.function == 0x800406b4)
        return m.writeWord(pose + 0xf0, q.color);
      return m.readWord(pose + 0xe8, flags) &&
             m.writeWord(pose + 0xe8, flags | q.args[1]);
    }
    if (q.function == 0x80039c44) {
      // Actual source getter: return descriptor NAME ADDRESS, not native text.
      uint32_t w, pool, models, resource, index, file, holder, model, directory;
      if (!m.readWord(0x80415dc0, w) || !m.readWord(w + 0x10, pool) ||
          !m.readWord(w, models))
        return false;
      const uint32_t pose = pool + q.args[0] * 0x170;
      if (!m.readWord(pose + 0x10, resource) ||
          !m.readWord(pose + 0x14, index) ||
          !m.readWord(models + (resource << 4) + 8, file) ||
          !m.readWord(file + 0xa0, holder) || !m.readWord(holder, model) ||
          !m.readWord(model + 0x1ac, directory))
        return false;
      out.word = directory + (index << 6);
      return true;
    }
    return b.native_.draw && b.native_.draw(b.native_.user, m, q, out);
  };
  return advanceFieldNpcDraw(callback, actor, s, c);
}
} // namespace ttyd
