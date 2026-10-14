#include "battle_audience_aux_callback.hpp"
#include "source_icon_gx.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
const BattleActMatrix Identity{1, 0, 0, 10, 0, 1, 0, 20, 0, 0, 1, 30};
struct Fixture {
  std::map<uint32_t, uint32_t> words{
      {0x8041e638, 1},          {0x8041e63c, 2},
      {0x8041e630, 3},          {0x8041f868, 0xffffffff},
      {0x8041f86c, 0x555555ff}, {0x8041f870, 0xffffffff}};
  std::map<uint32_t, uint16_t> halves;
  std::vector<SourceIconGXCall> calls;
  std::vector<BattleAudienceMatrixRequest> matrices;
  std::vector<SourceIconGXQuad> quads;
  uint16_t mapFlags = 0;
  uint32_t camera8 = 8, current = 4, width = 40, height = 32;
  int holdCall = -1, holdMath = -1;
  bool holdQuad = false;
  SourceIconGXServices services() {
    SourceIconGXServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      auto i = f.words.find(a);
      return i == f.words.end() ? std::optional<uint32_t>{} : i->second;
    };
    s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (a == 0x10004)
        return f.mapFlags;
      auto i = f.halves.find(a);
      return i == f.halves.end() ? std::optional<uint16_t>{} : i->second;
    };
    s.matrix = [](void *p, BattleAudienceMatrixRequest &r) {
      auto &f = *static_cast<Fixture *>(p);
      if (int(f.matrices.size()) == f.holdMath)
        return false;
      BattleAudienceAuxServices a;
      bindAudienceAuxMathAdapter(a);
      assert(a.matrix(nullptr, r));
      f.matrices.push_back(r);
      return true;
    };
    s.call = [](void *p, const SourceIconGXCall &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (int(f.calls.size()) == f.holdCall)
        return {};
      f.calls.push_back(r);
      switch (r.function) {
      case 0x8000f2fc:
        assert(r.integers[0] == 8);
        return f.camera8;
      case 0x8000f2e4:
        return f.current;
      case 0x80037a80:
        assert(r.integers[0] == 3 && r.integers[1] == 99);
        return 77;
      case 0x802b72b0:
        assert(r.integers ==
               std::array<uint32_t, 8>({0x80415080, 1, 1, 0, 0, 0, 0, 0}));
        return 88;
      case 0x8002b438:
        return 0x10000;
      case 0x8001d968:
        return 0x11223344;
      case 0x802b76c4:
        return f.width | 0xabcd0000;
      case 0x802b76d4:
        return f.height | 0xabcd0000;
      default:
        return 0;
      }
    };
    s.quad = [](void *p, const SourceIconGXQuad &q) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdQuad)
        return false;
      f.quads.push_back(q);
      return true;
    };
    return s;
  }
  size_t count(uint32_t function) const {
    size_t n = 0;
    for (const auto &r : calls)
      n += r.function == function;
    return n;
  }
  const SourceIconGXCall &find(uint32_t function,
                               unsigned occurrence = 0) const {
    for (const auto &r : calls)
      if (r.function == function) {
        if (!occurrence--)
          return r;
      }
    assert(false);
    return calls[0];
  }
};
SourceMatrixIcon icon(uint16_t flags = 0, uint16_t id = 237,
                      uint32_t rgba = 0xffffffff) {
  SourceMatrixIcon r;
  r.flags = flags;
  r.id = id;
  r.texture = 99;
  r.width = 40;
  r.height = 32;
  r.rgba = rgba;
  return r;
}
bool same(const SourceIconGXCall &a, const SourceIconGXCall &b) {
  return a.function == b.function && a.integers == b.integers &&
         a.reals == b.reals && a.matrix == b.matrix;
}
void equal(const Fixture &actual, const Fixture &expected) {
  assert(actual.calls.size() == expected.calls.size() &&
         actual.matrices.size() == expected.matrices.size() &&
         actual.quads.size() == 1);
  for (unsigned i = 0; i < actual.calls.size(); ++i)
    assert(same(actual.calls[i], expected.calls[i]));
  for (unsigned i = 0; i < actual.matrices.size(); ++i) {
    const auto &a = actual.matrices[i];
    const auto &b = expected.matrices[i];
    assert(a.function == b.function && a.left == b.left && a.right == b.right &&
           a.xyz == b.xyz && a.output == b.output);
  }
  assert(actual.quads[0].positions == expected.quads[0].positions &&
         actual.quads[0].uv == expected.quads[0].uv);
}
} // namespace
int main() {
  {
    Fixture f;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, icon(), f.services(), c));
    assert(!c.active && !c.pending);
    const std::vector<uint32_t> expected = {
        0x8000f2fc, 0x8000f2e4, 0x802ba2fc, 0x802ba39c, 0x80037a80, 0x802b7544,
        0x802b7880, 0x800208c0, 0x802b8fcc, 0x802b8e30, 0x802b88d4, 0x802b8c98,
        0x802b8ab8, 0x802b8e30, 0x802b89e8, 0x802b8a50, 0x802b8960, 0x802b89a4,
        0x8002b438, 0x802b5084, 0x802b4e04, 0x800217c8, 0x802b932c, 0x802b940c,
        0x802b8d60, 0x802b93d8, 0x802b5c8c, 0x802b4520, 0x802b3c08, 0x802b3c08,
        0x802b4558, 0x802b4558, 0x802b5a6c};
    assert(f.calls.size() == expected.size());
    for (unsigned i = 0; i < expected.size(); ++i)
      assert(f.calls[i].function == expected[i]);
    assert(f.matrices.size() == 2 && f.find(0x802ba2fc).matrix == Identity);
    assert(f.find(0x800208c0).integers[0] == 4 &&
           f.find(0x800208c0).reals == std::array<float, 4>({10, 20, 30, 0}));
    assert(f.find(0x802b7544).integers ==
           std::array<uint32_t, 8>({77, 1, 1, 0, 0, 0, 0, 0}));
    assert(f.find(0x802b7880).integers[0] == 77 && !f.count(0x802b76c4) &&
           !f.count(0x802b76d4));
    const float left = (-40.f) * .5f * .6f, top = 32.f * .6f;
    const auto &q = f.quads[0];
    assert(q.positions[0] == std::array<float, 3>({left, top, 0}) &&
           q.positions[2] == std::array<float, 3>({left + 40.f * .6f, 0, 0}));
    assert(q.uv == SourceIconGXQuad{}.uv &&
           f.find(0x802b8d60).integers ==
               std::array<uint32_t, 8>({6, 128, 1, 0, 0, 0, 0, 0}));
  }
  // Every declared mirror combination, for both current camera8 and camera4.
  for (uint16_t flags : {0, 4, 8, 12, 16, 20, 24, 28})
    for (uint32_t camera : {4u, 8u}) {
      Fixture f;
      f.current = camera;
      SourceIconGXCursor c;
      assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
      const auto &m = f.find(0x802ba2fc).matrix;
      float sx = flags & 4 ? -1 : 1, sy = flags & 8 ? -1 : 1;
      if ((flags & 16) && camera != 8)
        sy = -sy;
      assert(m[0] == sx && m[5] == sy && m[3] == 10 && m[7] == 20 &&
             m[11] == 30);
      assert(f.matrices.size() == ((flags & 16) && camera != 8 ? 4u : 2u));
      assert(f.count(0x800208c0) == ((flags & 16) ? 0u : 1u) &&
             f.count(0x800217c8) == ((flags & 16) ? 0u : 1u));
      assert(f.count(0x802b8ff4) == ((flags & 16) ? 1u : 0u));
      if (flags & 16) {
        assert(f.quads[0].positions[0][0] == -20 &&
               f.quads[0].positions[0][1] == 32);
        assert(f.find(0x802b6e54).integers[1] == 0xffffffff &&
               f.find(0x802b93d8, 1).integers[0] == 0);
      }
    }
  for (uint16_t id : {403, 405, 404}) {
    Fixture f;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, icon(0, id), f.services(), c));
    float expected = -20.f * .6f;
    if (id == 403)
      expected *= .8f;
    else if (id == 405)
      expected *= .9f;
    assert(f.quads[0].positions[0][0] == expected);
  }
  for (uint16_t flags : {0, 16, 32, 48})
    for (uint32_t alpha : {0u, 128u, 255u}) {
      Fixture f;
      SourceIconGXCursor c;
      assert(resumeSourceIconGX(Identity, icon(flags, 237, 0x12345600 | alpha),
                                f.services(), c));
      assert(f.find(0x802b932c).integers[0] == (alpha < 255 ? 1u : 0u));
      assert(f.find(0x802b940c).integers[0] == (alpha < 255 ? 1u : 0u));
      assert(f.find(0x802b93d8).integers[2] == (alpha < 255 ? 0u : 1u));
      if (flags & 32) {
        assert(f.count(0x802b8ce0) == 3 && f.count(0x802b8c98) == 6 &&
               !f.count(0x8002b438));
        assert(f.find(0x802b8ab8).integers[1] == 0x555555ff &&
               f.find(0x802b8ab8, 1).integers[1] == (0x12345600 | alpha));
        for (unsigned i = 0; i < 3; ++i) {
          const auto &reset = f.calls[f.calls.size() - 3 + i];
          assert(reset.function == 0x802b8c98 && reset.integers[0] == i &&
                 reset.integers[1] == 0 && reset.integers[2] == 0);
        }
      }
    }
  for (uint16_t flags : {0, 16, 32, 64, 128}) {
    Fixture f;
    f.mapFlags = 2;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
    assert(f.count(0x8001d968) == (flags ? 0u : 1u));
    if (!flags) {
      assert(f.find(0x802b8ab8, 1).integers[0] == 2 &&
             f.find(0x802b8ab8, 1).integers[1] == 0x11223344);
      assert(f.find(0x802b8fcc, 1).integers[0] == 3);
    }
  }
  for (uint16_t flags : {0, 0x400, 0x800, 0xc00}) {
    Fixture f;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
    assert(f.find(0x802b5c8c).integers[0] == (flags & 0x400   ? 2u
                                              : flags & 0x800 ? 1u
                                                              : 0u));
  }
  // Either genuinely absent buffer uses the original1x1 dummy texture. Both
  // zero dimensions query its object; one zero dimension does not query.
  for (unsigned which : {0u, 1u}) {
    Fixture f;
    f.words[which ? 0x8041e63c : 0x8041e638] = 0;
    f.width = f.height = 1;
    auto i = icon();
    i.width = i.height = 0;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, i, f.services(), c));
    assert(f.count(0x802b72b0) == 1 && !f.count(0x80037a80) &&
           !f.count(0x802b7544));
    assert(f.find(0x802b7880).integers[0] == 88 && f.count(0x802b76c4) == 1 &&
           f.count(0x802b76d4) == 1);
  }
  {
    Fixture f;
    auto i = icon();
    i.width = 0;
    SourceIconGXCursor c;
    assert(resumeSourceIconGX(Identity, i, f.services(), c));
    assert(!f.count(0x802b76c4) && f.quads[0].positions[0][0] == 0);
  }
  {
    Fixture f;
    f.words.erase(0x8041e638);
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(Identity, icon(), f.services(), c));
    assert(c.active && f.quads.empty() && !f.count(0x802b72b0));
  }
  // Source object dimensions narrow to U16 and are queried in width/height
  // order only when both descriptor dimensions are zero. A held height query
  // cannot re-read a width that the SDK already returned.
  {Fixture baseline;auto i=icon();i.width=i.height=0;SourceIconGXCursor b;
    assert(resumeSourceIconGX(Identity,i,baseline.services(),b));
    unsigned heightCall=0;while(baseline.calls[heightCall].function!=0x802b76d4)++heightCall;
    Fixture f;f.holdCall=int(heightCall);SourceIconGXCursor c;
    assert(!resumeSourceIconGX(Identity,i,f.services(),c));assert(c.width==40 && c.pending==0x802b76d4);
    f.width=999;f.holdCall=-1;assert(resumeSourceIconGX(Identity,i,f.services(),c));equal(f,baseline);
  }
  // Hold every linked call, SDK matrix and FIFO emission. Already accepted
  // operations are never replayed, including map blend and post-quad reset.
  for (uint16_t flags : {0, 16, 32, 48, 64}) {
    Fixture expected;
    expected.mapFlags = 2;
    SourceIconGXCursor baseline;
    assert(resumeSourceIconGX(Identity, icon(flags), expected.services(),
                              baseline));
    for (unsigned index = 0; index < expected.calls.size(); ++index) {
      Fixture f;
      f.mapFlags = 2;
      f.holdCall = int(index);
      SourceIconGXCursor c;
      assert(!resumeSourceIconGX(Identity, icon(flags), f.services(), c));
      assert(c.pending == expected.calls[index].function);
      f.holdCall = -1;
      assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
      equal(f, expected);
    }
    for (unsigned index = 0; index < expected.matrices.size(); ++index) {
      Fixture f;
      f.mapFlags = 2;
      f.holdMath = int(index);
      SourceIconGXCursor c;
      assert(!resumeSourceIconGX(Identity, icon(flags), f.services(), c));
      f.holdMath = -1;
      assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
      equal(f, expected);
    }
    Fixture f;
    f.mapFlags = 2;
    f.holdQuad = true;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(Identity, icon(flags), f.services(), c));
    assert(c.pending == 0x800197f8);
    f.holdQuad = false;
    assert(resumeSourceIconGX(Identity, icon(flags), f.services(), c));
    equal(f, expected);
  }
  // A held dynamic register write retains the loaded original constant.
  {
    Fixture f;
    auto i = icon(16);
    f.holdCall = 8;
    SourceIconGXCursor c;
    assert(!resumeSourceIconGX(Identity, i, f.services(), c));
    assert(c.pending == 0x802b6e54);
    f.words[0x8041f868] = 0;
    f.holdCall = -1;
    assert(resumeSourceIconGX(Identity, i, f.services(), c));
    assert(f.find(0x802b6e54).integers[1] == 0xffffffff);
  }
  // Real wrapper-to-iconGX connection, through metadata and captured camera
  // concat to an admitted primitive, with the final emitter explicitly held.
  {
    Fixture f;
    f.current = 0x60000;
    f.camera8 = 0x80000;
    f.words[0x8041e634] = 0x90000;
    f.words[0x8041f864] = 0xffffffff;
    f.halves[0x90056] = 0x100;
    f.halves[0x90102] = 40;
    f.halves[0x90104] = 32;
    f.halves[0x90106] = 1;
    f.halves[0x90108] = 99;
    f.halves[0x9010a] = 0;
    const BattleActMatrix view{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    for (unsigned i = 0; i < 12; ++i)
      std::memcpy(&f.words[0x6011c + 4 * i], &view[i], 4);
    auto gx = f.services();
    SourceIconMatrixServices wrapper;
    wrapper.user = &f;
    wrapper.word = gx.word;
    wrapper.half = gx.half;
    wrapper.matrix = gx.matrix;
    wrapper.gx = &gx;
    wrapper.currentCamera = [](void *p) -> std::optional<uint32_t> {
      return static_cast<Fixture *>(p)->current;
    };
    BattleAudienceIconRequest request;
    request.function = 0x8001a6e0;
    request.icon = 42;
    request.matrix = Identity;
    SourceIconMatrixCursor c;
    f.holdQuad = true;
    assert(!resumeSourceIconMatrix(request, wrapper, c));
    assert(c.pending == 0x800197f8 && c.gxCursor.phase == 27);
    const auto calls = f.calls.size(), matrices = f.matrices.size();
    f.current = 0x80000;
    f.halves[0x90108] = 123;
    f.holdQuad = false;
    assert(resumeSourceIconMatrix(request, wrapper, c));
    assert(f.calls.size() == calls && f.matrices.size() == matrices &&
           f.quads.size() == 1);
    assert(f.find(0x80037a80).integers[1] == 99 &&
           f.find(0x802ba2fc).matrix == Identity);
    // An explicit direct provider owns its wait even when GX is available.
    f.calls.clear();
    wrapper.draw = [](void *, const BattleActMatrix &,
                      const SourceMatrixIcon &) { return false; };
    f.current = 0x60000;
    f.halves[0x90108] = 99;
    SourceIconMatrixCursor direct;
    assert(!resumeSourceIconMatrix(request, wrapper, direct));
    assert(direct.pending == 0x800197f8 && !direct.gxCursor.active &&
           f.calls.empty());
  }
}
