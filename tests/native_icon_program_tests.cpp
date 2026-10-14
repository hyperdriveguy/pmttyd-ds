#include "battle_audience_aux_callback.hpp"
#include "map_material_color.hpp"
#include "map_material_light.hpp"
#include "native_icon_program.hpp"
#include <cassert>
#include <map>
using namespace ttyd;
struct Fixture {
  Fixture() {
    work.selector = 1;
    work.battleLoaded = true;
  }
  NativeIconProgram program;
  MapMaterialLightWork work;
  MapMaterialGroup group;
  CameraState camera;
  MapMaterialLightCursor light;
  MapMaterialColorCursor fog;
  std::vector<NativeIconPlan> plans;
  unsigned primitives = 0, blends = 0;
  uint32_t hold = 0;
  bool holdQuad = false;
  bool state(const ShadowGXRequest &r) {
    SourceIconGXCall q;
    q.function = uint32_t(r.kind);
    for (unsigned i = 0; i < 7; ++i)
      q.integers[i] = uint32_t(r.values[i]);
    q.reals = r.floats;
    if (r.kind == ShadowGX::Fog || r.kind == ShadowGX::ChanMaterial ||
        r.kind == ShadowGX::ChanAmbient || r.kind == ShadowGX::TevColor)
      q.integers[1] = r.color;
    return program.request(q);
  }
  SourceIconGXServices services() {
    SourceIconGXServices s;
    s.user = this;
    s.word = [](void *, uint32_t a) -> std::optional<uint32_t> {
      if (a == 0x8041e638 || a == 0x8041e63c || a == 0x8041e630)
        return 1;
      if (a == 0x8041f86c)
        return 0x555555ff;
      if (a == 0x8041f868 || a == 0x8041f870)
        return 0xffffffff;
      return {};
    };
    s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      assert(a == 0x10004);
      return static_cast<Fixture *>(p)->group.flags;
    };
    s.matrix = [](void *, BattleAudienceMatrixRequest &r) {
      BattleAudienceAuxServices math;
      bindAudienceAuxMathAdapter(math);
      return math.matrix(nullptr, r);
    };
    s.call = [](void *p, const SourceIconGXCall &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold == r.function)
        return {};
      switch (r.function) {
      case 0x8000f2fc:
        return 8;
      case 0x8000f2e4:
        return 4;
      case 0x802ba2fc:
      case 0x802ba39c:
      case 0x802b7544:
      case 0x802b7880:
        return 0;
      case 0x80037a80:
        return 10;
      case 0x8002b438:
        return 0x10000;
      case 0x8001d968:
        ++f.blends;
        return f.group.colors[0];
      case 0x800208c0: {
        MapMaterialLightServices a;
        a.user = &f;
        a.query = [](void *u, MapLightQuery &q, uint32_t &pending) {
          return mapLightBankQuery(static_cast<Fixture *>(u)->work, q, pending);
        };
        a.gx = [](void *u, const ShadowGXRequest &q) {
          return static_cast<Fixture *>(u)->state(q);
        };
        return resumeMapMaterialLight(
                   f.work, 4, {r.reals[0], r.reals[1], r.reals[2]}, a, f.light)
                   ? std::optional<uint32_t>(0)
                   : std::nullopt;
      }
      case 0x800217c8: {
        MapMaterialColorServices a;
        a.user = &f;
        a.currentCamera = [](void *u) -> const CameraState * {
          return &static_cast<Fixture *>(u)->camera;
        };
        a.group = [](void *u) -> const MapMaterialGroup * {
          return &static_cast<Fixture *>(u)->group;
        };
        a.gx = [](void *u, const ShadowGXRequest &q) {
          return static_cast<Fixture *>(u)->state(q);
        };
        return resumeMapMaterialColor(r.function, 0, 0, 0, a, f.fog)
                   ? std::optional<uint32_t>(0)
                   : std::nullopt;
      }
      case 0x802b5a6c: {
        const auto plan = f.program.plan();
        if (!plan)
          return {};
        f.plans.push_back(*plan);
        return 0;
      }
      case 0x802b932c:
      case 0x802b940c:
      case 0x802b8d60:
      case 0x802b93d8:
        return 0; // Separate tested display owner.
      default:
        return f.program.request(r) ? std::optional<uint32_t>(0) : std::nullopt;
      }
    };
    s.quad = [](void *p, const SourceIconGXQuad &) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdQuad)
        return false;
      ++f.primitives;
      return true;
    };
    return s;
  }
};
const BattleActMatrix matrix{1, 0, 0, 2, 0, 1, 0, 3, 0, 0, 1, 4};
SourceMatrixIcon icon(uint16_t flags) {
  SourceMatrixIcon i;
  i.flags = flags;
  i.id = 237;
  i.width = i.height = 40;
  i.texture = 1;
  i.rgba = 0x12345678;
  return i;
}
int main() {
  for (uint16_t flags : {0, 4, 8, 16, 20, 32, 48, 64, 128, 0x400, 0x800, 0xc00})
    for (bool blend : {false, true}) {
      Fixture f;
      f.group.flags = blend ? 2 : 0;
      f.group.colors[0] = 0xaabbccdd;
      SourceIconGXCursor c;
      assert(resumeSourceIconGX(matrix, icon(flags), f.services(), c));
      assert(f.primitives == 1 && f.plans.size() == 1);
      const auto &p = f.plans[0];
      assert(p.material == 0xffffffff && p.cull == (flags & 0x400   ? 2u
                                                    : flags & 0x800 ? 1u
                                                                    : 0u));
      const bool overlay = blend && !(flags & (16 | 32 | 64 | 128));
      assert(f.blends == unsigned(overlay));
      if (flags & 32) {
        assert(p.mode == NativeIconPlan::Grayscale && p.tint == 0x555555ff &&
               p.blend == 0x12345678);
        assert(!f.program.plan());
      } // Source post-draw swap reset changes current state, not captured plan.
      else {
        assert(p.mode ==
               (overlay ? NativeIconPlan::Blended : NativeIconPlan::Normal));
        assert(p.tint == 0x12345678);
        if (overlay)
          assert(p.blend == 0xaabbccdd);
      }
    }
  {
    Fixture f;
    for (uint16_t flags : {32, 0, 48, 16, 0}) {
      SourceIconGXCursor c;
      assert(resumeSourceIconGX(matrix, icon(flags), f.services(), c));
    }
    assert(f.plans.size() == 5 && f.primitives == 5);
  }
  for (uint32_t function :
       {0x800208c0u, 0x800217c8u, 0x802b8fccu, 0x802b5a6cu}) {
    Fixture f;
    f.hold = function;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(matrix, icon(0), f.services(), c) &&
           f.primitives == 0);
    f.hold = 0;
    assert(resumeSourceIconGX(matrix, icon(0), f.services(), c) &&
           f.primitives == 1 && f.plans.size() == 1);
  }
  {
    Fixture f;
    f.holdQuad = true;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(matrix, icon(32), f.services(), c));
    assert(f.plans.size() == 1);
    const auto p = f.plans[0];
    f.holdQuad = false;
    assert(resumeSourceIconGX(matrix, icon(32), f.services(), c));
    assert(f.plans.size() == 1 && p.mode == NativeIconPlan::Grayscale &&
           !f.program.plan());
  }
  {
    Fixture f;
    f.group.flags = 1;
    f.group.fogType = 0;
    f.group.fogStart = 10;
    f.group.fogEnd = 20;
    f.camera.nearPlane = 1;
    f.camera.farPlane = 32768;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(matrix, icon(0), f.services(), c));
    assert(c.pending == 0x800217c8 && f.primitives == 0 && f.plans.empty());
  }
  {
    NativeIconProgram p;
    assert(!p.plan());
    SourceIconGXCall r;
    r.function = 0x802b6f78;
    r.integers = {4, 1, 0, 0, 0, 2, 2};
    assert(!p.request(r));
    r = {};
    r.function = 0x802b8ff4;
    r.integers = {2, 0xffffffff};
    assert(!p.request(r));
    r = {};
    r.function = 0x12345678;
    assert(!p.request(r));
  }
  {
    Fixture f;
    f.work.selector = 0;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(matrix, icon(0), f.services(), c));
    assert(c.pending == 0x800208c0 && !f.primitives && f.plans.empty());
    f.work.selector = 1;
    assert(resumeSourceIconGX(matrix, icon(0), f.services(), c));
    assert(f.primitives == 1);
  }
  {
    Fixture f;
    SourceIconGXCall swap;
    swap.function = 0x802b8ce0;
    swap.integers = {0, 2, 1, 0, 3};
    assert(f.program.request(swap));
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(matrix, icon(0), f.services(), c));
    assert(c.pending == 0x802b5a6c && !f.primitives);
    swap.integers = {0, 0, 1, 2, 3};
    assert(f.program.request(swap));
    assert(resumeSourceIconGX(matrix, icon(0), f.services(), c));
    assert(f.primitives == 1);
  }
}
