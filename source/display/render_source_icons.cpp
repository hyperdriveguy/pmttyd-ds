#include "map_material_color.hpp"
#include "platform.hpp"
#include <algorithm>
namespace ttyd {
C3D_Tex *Renderer::sourceIconTexture() {
  if (!boundSourceIcon)
    return nullptr;
  for (auto &cell : sourceIconCells)
    if (cell.index == *boundSourceIcon && cell.gpu.imported)
      return &cell.gpu.texture;
  return nullptr;
}
// Synchronize before retiring imported descriptor/texture pairs, then clear
// logical bindings and retained packets together. Wrapper copies own no clone.
void Renderer::releaseSourceIcons(bool preserveWeaponPins) {
  // Only the ordinary Game-inactive transition preserves actual weapon pins.
  // Explicit shutdown remains force-all and invalidates the native pin owners.
  if(!preserveWeaponPins)weaponSourceIconPins.clear(true);
  orderedHpCapturePass=0;orderedHpPresentation=nullptr;
  bool any = false;
  for (const auto &cell : sourceIconCells)
    any |= cell.gpu.imported != nullptr;
  if (any)
    C3D_FrameSync();
  for (auto &cell : sourceIconCells) {
    if(preserveWeaponPins && weaponSourceIconPins.retains(cell.index))continue;
    cell.generation.reset();
    if (cell.gpu.imported) {
      Tex3DS_TextureFree(cell.gpu.imported);
      C3D_TexDelete(&cell.gpu.texture);
    }
    cell = {};
  }
  if (finishingShadowProgram.texture == ShadowTexture::SourceIcon)
    finishingShadowProgram.texture.reset();
  pendingSourceIcon.reset();
  boundSourceIcon.reset();
  sourceIconPlan.reset();
  retainedIcons.reset();iconViewport.reset();iconScissor.reset();iconTransform.reset();
}
// Out-of-frame admission services one pending image. Eviction chooses an
// empty/oldest unpinned cell; bound image and both retained passes pin residency.
// A miss holds the source call rather than drawing a substitute texture.
void Renderer::prepareSourceIcons(const Game &game) {
  if (!game.trioBattleActive) {
    releaseSourceIcons(true);
    return;
  }
  retainedIcons.observe(game.trioAttack.sourceIconPass());
  if (!sourceIcons.ready() && !sourceIcons.load("romfs:/source-icons")) {
    frontFailure = "Original icon resources unavailable";
    return;
  }
  if (!pendingSourceIcon)
    return;
  (void)prepareSourceIconImageInternal(*pendingSourceIcon);
}
// Bind only a resident exact image; pendingSourceIcon schedules the later
// admission attempt. Roll back logical binding if downstream GPU setup holds.
bool Renderer::bindSourceIcon(uint32_t index) {
  for (auto &cell : sourceIconCells)
    if (cell.index == index && cell.gpu.imported) {
      const auto previous = boundSourceIcon;
      boundSourceIcon = index;
      ShadowGXRequest request;
      request.kind = ShadowGX::TextureBind;
      request.texture = ShadowTexture::SourceIcon;
      if (!finishingShadowRequest(request)) {
        boundSourceIcon = previous;
        return false;
      }
      cell.stamp = ++sourceIconStamp;
      return true;
    }
  pendingSourceIcon = index;
  return false;
}
// Original US function tokens dispatch only admitted providers; optional0 is
// successful void return, nullopt is a hold. Addresses are never ARM pointers.
// Map work token 85000000(hex) is synthetic guest identity; +4 exposes flags.
std::optional<uint32_t> Renderer::sourceIconCall(const Game &game,
                                                 const SourceIconGXCall &r) {
  // camGetPtr(8) / camGetCurPtr; explicit camera 8 is source UI ownership.
  if (r.function == 0x8000f2fc) {
    if (r.integers != std::array<uint32_t, 8>{8, 0, 0, 0, 0, 0, 0, 0})
      return {};
    return game.trioAttack.sourceIconCamera(8);
  }
  if (r.function == 0x8000f2e4) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    return game.trioAttack.sourceIconCamera();
  }
  // GXLoadPosMtxImm(slot 0): row-major3x4; GXSetCurrentMtx follows below.
  if (r.function == 0x802ba2fc) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    ShadowGXRequest q;
    q.kind = ShadowGX::PositionMatrix;
    for (unsigned row = 0; row < 3; ++row)
      for (unsigned col = 0; col < 4; ++col)
        q.matrix[row][col] = r.matrix[4 * row + col];
    return finishingShadowRequest(q) ? std::optional<uint32_t>(0)
                                     : std::nullopt;
  }
  if (r.function == 0x802ba39c) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    ShadowGXRequest q;
    q.kind = ShadowGX::CurrentMatrix;
    return finishingShadowRequest(q) ? std::optional<uint32_t>(0)
                                     : std::nullopt;
  }
  // mapSetMaterialLight(channel4), using retained original light-bank query.
  if (r.function == 0x800208c0) {
    if (r.integers != std::array<uint32_t, 8>{4, 0, 0, 0, 0, 0, 0, 0})
      return {};
    struct Bridge {
      Renderer *renderer;
      const MapMaterialLightWork *work;
    } bridge{this, game.trioAttack.materialLights()};
    if (!bridge.work)
      return {};
    MapMaterialLightServices services;
    services.user = &bridge;
    services.query = [](void *u, MapLightQuery &q, uint32_t &pending) {
      return mapLightBankQuery(*static_cast<Bridge *>(u)->work, q, pending);
    };
    services.gx = [](void *u, const ShadowGXRequest &q) {
      return static_cast<Bridge *>(u)->renderer->sourceIconState(q);
    };
    uint32_t pending = 0;
    return game.trioAttack.resumeMaterialLight(
               4, {r.reals[0], r.reals[1], r.reals[2]}, services, pending)
               ? std::optional<uint32_t>(0)
               : std::nullopt;
  }
  // mapGetWork returns the synthetic token; mapGetBlend reads flag 2 and
  // colors[0], otherwise source opaque white ffffffff.
  if (r.function == 0x8002b438) {
    return game.trioAttack.materialGroup() ? std::optional<uint32_t>(0x85000000)
                                           : std::nullopt;
  }
  if (r.function == 0x8001d968) {
    const auto *group = game.trioAttack.materialGroup();
    if (!group)
      return {};
    return (group->flags & 2) ? group->colors[0] : 0xffffffffu;
  }
  // mapSetMaterialFog resumes its own retained material continuation.
  if (r.function == 0x800217c8) {
    struct Bridge {
      Renderer *renderer;
      const Game *game;
    } bridge{this, &game};
    MapMaterialColorServices services;
    services.user = &bridge;
    services.group = [](void *u) {
      return static_cast<Bridge *>(u)->game->trioAttack.materialGroup();
    };
    services.currentCamera = [](void *u) {
      const auto *c = static_cast<Bridge *>(u)->game->trioAttack.cameras();
      return c ? c->current() : nullptr;
    };
    services.gx = [](void *u, const ShadowGXRequest &q) {
      return static_cast<Bridge *>(u)->renderer->sourceIconState(q);
    };
    uint32_t pending = 0;
    return game.trioAttack.resumeMaterialColor(r.function, 0, 0, 0, services,
                                               pending)
               ? std::optional<uint32_t>(0)
               : std::nullopt;
  }
  // GXBegin tuple{0x80, vat0, count 4}: admit the whole quad shader plan
  // before the separate retained quad-emission provider can acknowledge it.
  if (r.function == 0x802b5a6c) {
    if (r.integers != std::array<uint32_t, 8>{128, 0, 4, 0, 0, 0, 0, 0})
      return {};
    const auto plan = sourceIconProgram.plan();
    if (!plan || !applySourceIconProgram(*plan))
      return {};
    sourceIconPlan = *plan;
    return 0;
  }
  // GXSetBlendMode/ZCompLoc/AlphaCompare/ZMode go through the independent
  // display-pipeline owner, not the icon material-program state machine.
  if (r.function == 0x802b932c || r.function == 0x802b940c ||
      r.function == 0x802b8d60 || r.function == 0x802b93d8) {
    ShadowGXRequest q;
    q.kind = static_cast<ShadowGX>(r.function);
    for (unsigned i = 0; i < 7; ++i)
      q.values[i] = int32_t(r.integers[i]);
    return finishingShadowRequest(q) ? std::optional<uint32_t>(0)
                                     : std::nullopt;
  }
  if (sourceIconProgram.request(r))
    return 0;
  // Quad emission retains its own provider wait after complete shader binding.
  return sourceIcons.request(r, this, [](void *u, uint32_t index) {
    return static_cast<Renderer *>(u)->bindSourceIcon(index);
  });
}
} // namespace ttyd
