#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include "field_npc_main_entry_binding.hpp"
namespace ttyd {
// Borrow same actual VM/shared pose alias as Main; reject a cursor owned by
// another binding. Core captures invocation args. Bindings/mappings and real
// camera/queue/animation/GPU producer pass outlive holds and deferred draws.
bool FieldNpcMainEntryBinding::advanceDraw(uint32_t callback, uint32_t actor,
                                           FieldNpcDrawCursor &c,
                                           uint32_t camera) {
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
    // Reuse actual pose-pointer getter and bounded guest strcmp admission;
    // no native string is cast to a source address. Inactive pose rows allowed.
    if (q.function == 0x80039cf8 || q.function == 0x8026d464) {
      const auto common = b.services();
      return common.request(p, {q.function, {q.args[0], q.args[1]}}, out.word);
    }
    // Shared pose row(handle0..255):event colorf0;material flagse8. Get4069c,
    // Off40710,On40730 preserve source mask operations, with no active/resource
    // gate. Callback Main/SetAnim/GPU work remains a separate native provider.
    if (q.function == 0x800406b4 || q.function == 0x80040730 ||
        q.function == 0x80040710 || q.function == 0x8004069c) {
      if (q.args[0] >= 256 ||
          !b.poses_.aliases(*b.owner_->poses().sharedPool(), m))
        return false;
      const auto pose = b.poses_.poseAddress(q.args[0]);
      uint32_t flags;
      if (q.function == 0x800406b4)
        return m.writeWord(pose + 0xf0, q.color);
      if (q.function == 0x8004069c)
        return m.readWord(pose + 0xe8, out.word);
      return m.readWord(pose + 0xe8, flags) &&
             m.writeWord(pose + 0xe8, q.function == 0x80040710
                                          ? flags & ~q.args[1]
                                          : flags | q.args[1]);
    }
    // Source GetCurrentAnim pointer graph:W80415dc0,posepool W10,models W0;
    // row stride170,resource10,animation14;modelcache stride16/file+8;
    // filea0->holder->MODEL1ac directory,stride64 per animation. Arithmetic wraps
    //32 bits. Return descriptor NAME ADDRESS,not a copied/resolved model name.
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
    if ((q.function == 0x80041170 || q.function == 0x8003ed2c ||
         q.function == 0x8003b4d8) &&
        b.native_.animation.request)
      return b.native_.animation.request(b.native_.animation.user, m, q, out);
    return b.native_.draw && b.native_.draw(b.native_.user, m, q, out);
  };
  return advanceFieldNpcDraw(callback, actor, s, c, camera);
}
} // namespace ttyd
