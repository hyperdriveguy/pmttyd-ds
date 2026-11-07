#include "battle_weapon_window_bridge.hpp"
#include <cstring>
#include <tuple>
namespace ttyd {
namespace {
bool imageSame(const AnimationTPLImage &a, const AnimationTPLImage &b) {
  return std::tie(a.index, a.imageHeader, a.paletteHeader, a.data, a.format,
                  a.wrapS, a.wrapT, a.minFilter, a.magFilter, a.paletteData,
                  a.paletteFormat, a.width, a.height, a.paletteCount, a.edge,
                  a.minLOD, a.maxLOD) ==
             std::tie(b.index, b.imageHeader, b.paletteHeader, b.data, b.format,
                      b.wrapS, b.wrapT, b.minFilter, b.magFilter, b.paletteData,
                      b.paletteFormat, b.width, b.height, b.paletteCount,
                      b.edge, b.minLOD, b.maxLOD) &&
         std::memcmp(&a.bias, &b.bias, 4) == 0;
}
struct WindowObjectProof {
  std::shared_ptr<NativeUiStartup> startup;
  NativeUiStartup::WindowObject object;
  std::shared_ptr<const void> resources;
  std::function<bool()> longCurrent;
  std::shared_ptr<const SourceGxInheritedSnapshot> gx;
  bool current() const {
    if (!startup || !resources || !longCurrent || !longCurrent() ||
        !startup->current() || !gx || !gx->current())
      return false;
    const auto now = startup->windowObject(object.index);
    return now && now->palette == object.palette &&
           now->address == object.address && now->index == object.index &&
           now->bytes == object.bytes && imageSame(now->image, object.image) &&
           longCurrent() && gx->current();
  }
};
} // namespace
struct BattleWeaponWindowBridge::State {
  BattleWeaponWindowBinding binding;
  std::unique_ptr<BattleWindowPacketProvider> packets;
  WindowPrimitiveServices source;
  WindowPrimitiveCursor cursor;
  const BattleWeaponHelpCall *active = nullptr;
  bool invoking = false, invalid = false;
  std::optional<uint32_t> capturedCamera;
  State(const BattleWindowSurfaceResource &r, BattleWeaponWindowBinding b)
      : binding(std::move(b)) {
    BattleWindowQueryServices q;
    q.user = this;
    q.lifetime = binding.resources;
    q.current = [](void *p) { return static_cast<State *>(p)->current(); };
    q.camera = [](void *p) -> std::optional<uint32_t> {
      auto &s = *static_cast<State *>(p);
      if (!s.current() || !s.binding.currentCamera)
        return {};
      auto camera = s.binding.currentCamera();
      if (!s.current() || !camera || *camera != s.binding.cameraAddress)
        return {};
      s.capturedCamera = camera;
      return camera;
    };
    q.texture =
        [](void *p,
           uint32_t address) -> std::optional<BattleWindowTextureObject> {
      auto &s = *static_cast<State *>(p);
      if (!s.current() || !s.binding.gx)
        return {};
      // The original primitive coerces every argument to object0. Admit only
      // its actual source array address; never convert an atlas/index into a
      // handle.
      const auto object = s.binding.startup->windowObject(0);
      if (!object || object->address != address)
        return {};
      const auto gx = s.binding.gx->capture(1);
      if (!gx || !s.current())
        return {};
      auto proof = std::make_shared<WindowObjectProof>();
      proof->startup = s.binding.startup;
      proof->object = *object;
      proof->resources = s.binding.resources;
      proof->longCurrent = s.binding.longCurrent;
      proof->gx = gx;
      if (!proof->current())
        return {};
      BattleWindowTextureObject out;
      out.address = address;
      out.image = object->image;
      out.lifetime = proof;
      out.user = proof.get();
      out.current = [](void *p) {
        return static_cast<WindowObjectProof *>(p)->current();
      };
      for (unsigned i = 0; i < 8; ++i) {
        uint32_t word = 0;
        for (unsigned j = 0; j < 4; ++j)
          word = word << 8 | object->bytes[4 * i + j];
        out.bytes[i] = word;
      }
      return out;
    };
    BattleWindowPacketSink sink;
    sink.user = this;
    sink.append = [](void *p, BattleWindowPacketPtr packet) {
      auto &s = *static_cast<State *>(p);
      return s.current() && s.binding.append &&
             s.binding.append(std::move(packet)) && s.current();
    };
    packets = std::make_unique<BattleWindowPacketProvider>(r, binding.camera,
                                                           std::move(q), sink);
    source.user = this;
    source.current = [](void *p) { return static_cast<State *>(p)->current(); };
    source.word = [](void *p, uint32_t address) -> std::optional<uint32_t> {
      auto &s = *static_cast<State *>(p);
      if (!s.current())
        return {};
      std::optional<uint32_t> value;
      // Genuine source WindowReady0 must not need a resident atlas or completed
      // SDK object. Shared startup state has first refusal over destination
      // RAM.
      if (s.binding.startup->ownsSourceResource(address, 4))
        value = s.binding.startup->sourceResourceWord(address);
      else if (s.binding.word)
        value = s.binding.word(address);
      return s.current() ? value : std::nullopt;
    };
    source.matrix = [](void *p, BattleAudienceMatrixRequest &q) {
      auto &s = *static_cast<State *>(p);
      return s.current() && s.binding.matrix && s.binding.matrix(q) &&
             s.current();
    };
    source.request =
        [](void *p,
           const WindowPrimitiveRequest &q) -> std::optional<uint32_t> {
      auto &s = *static_cast<State *>(p);
      if (!s.current())
        return {};
      auto result = s.packets->request(q);
      if (result) {
        // Observer metadata is not a second source acknowledgment. Never
        // observe a held setter/FIFO lane or an opaque outer Help/Window
        // parent.
        s.binding.gx->accepted(q);
      }
      return s.current() ? result : std::nullopt;
    };
  }
  bool current() {
    if (invalid)
      return false;
    if (!binding.startup || !binding.resources || !binding.cameraAddress ||
        uint64_t(binding.cameraAddress) + 0x14c > 0x100000000ULL ||
        binding.camera.camera != 8 || !binding.sourceCurrent ||
        !binding.longCurrent || !binding.sourceCurrent() ||
        !binding.longCurrent() || !binding.startup->current() || !binding.gx) {
      invalid = true;
      return false;
    }
    return true;
  }
  std::optional<uint32_t> resume(const BattleWeaponHelpCall &q) {
    if (invoking)
      return {};
    invoking = true;
    struct Guard {
      bool &b;
      ~Guard() { b = false; }
    } g{invoking};
    if (!current() || q.function != 0x8007b44c || q.integers.size() != 1 ||
        q.reals.size() != 5 || !q.privateColor || q.privateText ||
        q.guestText || q.privateOffset)
      return {};
    if (active && active != &q)
      return {};
    active = &q;
    if (!resumeOriginalWindowPrimitive(q.integers[0], *q.privateColor,
                                       q.reals[0], q.reals[1], q.reals[2],
                                       q.reals[3], q.reals[4], source, cursor))
      return {};
    if (!current() || !cursor.reset())
      return {};
    active = nullptr;
    capturedCamera.reset();
    return 0;
  }
};
BattleWeaponWindowBridge::BattleWeaponWindowBridge(
    const BattleWindowSurfaceResource &r, BattleWeaponWindowBinding b)
    : state_(std::make_unique<State>(r, std::move(b))) {}
BattleWeaponWindowBridge::~BattleWeaponWindowBridge() = default;
std::optional<uint32_t>
BattleWeaponWindowBridge::request(const BattleWeaponHelpCall &q) {
  return state_->resume(q);
}
uint32_t BattleWeaponWindowBridge::pending() const {
  return state_->cursor.pending();
}
} // namespace ttyd
