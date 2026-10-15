#include "battle_class_draw_adapter.hpp"
namespace ttyd {
bool drawBattleClassBackendIcons(uint32_t work, uint32_t rows,
    const std::vector<BattleActDrawSlot>& slots,
    const BattleClassRasterServices& source, BattleClassBackend& backend) {
  struct Context {
    const BattleClassRasterServices* source;
    BattleClassBackend* backend;
    BattleClassRasterServices raster;
  } context{&source, &backend, source};
  // Only the camera returns a guest object address. Backend texture/font/icon
  // requests represent their own concrete native resources, not fabricated
  // successful source calls. Never replace a missing source camera with ID8.
  context.raster.user = &context;
  context.raster.request = [](void* user, const BattleClassRasterRequest& q) {
    auto& c = *static_cast<Context*>(user);
    if (q.function == BattleClassRasterService::Camera)
      return c.source->request ? c.source->request(c.source->user, q)
                               : std::optional<uint32_t>{};
    return c.backend->request(q);
  };
  context.raster.truncate = [](void* user, float value) {
    const auto& s = *static_cast<Context*>(user)->source;
    return s.truncate ? s.truncate(s.user, value) : std::optional<int32_t>{};
  };
  context.raster.identity = [](void* user) {
    const auto& s = *static_cast<Context*>(user)->source;
    return s.identity ? s.identity(s.user) : std::optional<BattleActMatrix>{};
  };
  // math.user and memory.user remain unchanged. These may deliberately name
  // owners distinct from the source camera/numeric callback context.
  BattleActIconDrawServices icons;
  icons.memory = source.memory;
  icons.user = &context;
  icons.bubble = [](void* user, uint32_t w, uint32_t row,
                    const BattleActMatrix& matrix, float parameter) {
    return drawPackedBattleClassBubble(w, row, matrix, parameter,
        static_cast<Context*>(user)->raster);
  };
  icons.circle = [](void* user, uint32_t, uint32_t row,
                    const BattleActMatrix& matrix,
                    const std::array<float, 3>& position, float parameter) {
    return drawPackedBattleClassCircle(row, matrix, position, parameter,
        static_cast<Context*>(user)->raster);
  };
  return drawPackedBattleActIcons(work, rows, slots, icons) && backend.complete();
}
} // namespace ttyd
