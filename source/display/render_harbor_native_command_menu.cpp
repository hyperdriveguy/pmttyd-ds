#include "harbor_native_command_menu.hpp"
#include "platform.hpp"
namespace ttyd {
struct Renderer::NativeHarborMenu {
  HarborNativeMenuPlan plan;
  bool submitted = false;
};
bool Renderer::prepareNativeHarborMenu(const Game &game) {
  // Explicit preFrame phase. Retire preceding CPU references without guest
  // writes.
  nativeHarborMenu.reset();
  if (game.fade.alpha())
    return false;
  const auto snapshot = game.harborTutorial.nativeCommandMenu();
  if (!snapshot)
    return false;
  if (!prepareClassMenuDrawing())
    return false;
  const auto surface = classMenuSurfaceLease();
  const auto font = battleFontLease();
  if (!surface || !font.current())
    return false;
  auto plan = prepareHarborNativeMenu(
      *snapshot, *surface, font, [&](uint32_t id) -> const MessageIconFrame * {
        const auto *icon = messageIcon(id);
        return icon ? &icon->frame(game.ticks) : nullptr;
      });
  if (!plan)
    return false;
  nativeHarborMenu = std::make_shared<NativeHarborMenu>();
  nativeHarborMenu->plan = std::move(*plan);
  return true;
}
bool Renderer::drawNativeHarborMenu(const Game &game) {
  const auto state = nativeHarborMenu;
  if (!state || state->submitted)
    return false;
  const auto &p = state->plan;
  constexpr unsigned ClassQuadBudget = 1024, VerticesPerQuad = 6;
  // Complete CPU geometry/resource/arena preflight BEFORE the first GPU effect.
  // The render thread serializes import/retirement and both draws below;
  // neither linked draw allocates, invokes providers, or replaces these
  // resource owners.
  if (!game.harborTutorial.nativeCommandMenuCurrent(p.snapshot) ||
      !validHarborNativeMenuPlan(p) || !c3dReady || !c2dReady ||
      !programReady || !top || transformLoc < 0 || !classVertices ||
      !classSurfaceGpu ||
      !classSurfaceGpu->resource().accepts(p.batch.surfaceLease) ||
      !battleFontGpu.accepts(p.batch.fontLease) || !fontTexture.imported ||
      !messageIconTexture.imported ||
      classVertexCount / VerticesPerQuad > ClassQuadBudget ||
      p.batch.quads.size() >
          ClassQuadBudget - classVertexCount / VerticesPerQuad)
    return false;
  if (p.window && (!windowFrameVertices || !messagePanel.imported ||
                   windowFrameCount >= MaxWindowFrames ||
                   !Tex3DS_GetSubTexture(messagePanel.imported, 0)))
    return false;
  if (game.fade.alpha() || !C3D_FrameDrawOn(top))
    return false;
  if (p.window && !sourceWindowFrame(*p.window))
    return false;
  if (!drawClassMenu(p.batch))
    return false;
  state->submitted = true;
  return true;
}
void Renderer::completeNativeHarborMenu(const Game &game) {
  const auto state = std::move(nativeHarborMenu);
  nativeHarborMenu.reset();
  // Invoked only after actual C3D_FrameEnd. An unavailable/interrupted frame
  // never admits physical input by merely building geometry or beginning a
  // frame.
  if (!state || !state->submitted || game.fade.alpha() ||
      !game.harborTutorial.nativeCommandMenuCurrent(state->plan.snapshot))
    return;
  switch (state->plan.snapshot.kind) {
  case HarborNativeMenuKind::Class:
    game.harborTutorial.classMenuPresented();
    break;
  case HarborNativeMenuKind::Weapon:
    game.harborTutorial.weaponMenuPresented();
    break;
  case HarborNativeMenuKind::Target:
    break; // No original queued target receipt.
  }
}
void Renderer::retireNativeHarborMenu() { nativeHarborMenu.reset(); }
} // namespace ttyd
