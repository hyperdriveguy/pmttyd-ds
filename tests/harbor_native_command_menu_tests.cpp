#include "harbor_native_command_menu.hpp"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>
using namespace ttyd;
namespace {
struct Resources {
  int identities[3]{};
  BattleClassSurfaceGpuResource surfaces;
  BattleFontGpuResource fonts;
  Resources() {
    BattleClassSurfaceTextureServices service{
        this,
        [](void *p, uint32_t slot,
           const char *) -> std::optional<BattleClassSurfaceImport> {
          auto &r = *static_cast<Resources *>(p);
          const unsigned h = slot ? 256 : 128;
          return BattleClassSurfaceImport{
              &r.identities[slot], 256, h, 0, 1, 256, h, false, {0, 1, 1, 0}};
        },
        [](void *, uint32_t, const void *) {}};
    assert(surfaces.load("romfs", service));
    MessageFont font;
    assert(font.load("romfs/font.pmfont"));
    assert(fonts.adopt(
        font, {&identities[2], 512, 256, 0, 1, 512, 256, false, {0, 1, 1, 0}}));
    assert(loadMessageIcons("romfs/message-icons.bin"));
  }
};
} // namespace
int main() {
  Resources resources;
  auto registration = std::make_shared<const int>(1);
  HarborNativeMenuSnapshot s;
  s.facade = &s;
  s.runtime = std::make_shared<const int>(2);
  s.registration = registration;
  NativeFacadeRegistration facade;
  s.facadeRegistration = facade.proof();
  s.menuOwner = std::make_shared<const uint8_t>(0);
  s.battle = 0x8000;
  s.work = 0x9000;
  s.selected = 0;
  s.rows = {{1, 0, 334, true, "Jump"},
            {2, 0, 335, true, "Hammer"},
            {3, 0, 336, false, "Items"},
            {4, 0, 337, true, "Tactics"}};
  const auto lookup = [](uint32_t id) -> const MessageIconFrame * {
    const auto *i = messageIcon(id);
    return i ? &i->frame(0) : nullptr;
  };
  auto prepare = [&](const HarborNativeMenuSnapshot &a) {
    return prepareHarborNativeMenu(a, resources.surfaces.lease(),
                                   resources.fonts.lease(), lookup);
  };
  for (int selected = 0; selected < 4; ++selected) {
    s.selected = selected;
    const auto p = prepare(s);
    assert(p && validHarborNativeMenuPlan(*p) && !p->window);
    assert(p->batch.quads[0].texture == BattleClassTexture::Surfaces);
    // Each unselected row: four source shadow quarters, four foreground
    // quarters, then its actual icon. Texture0 alone is NOT a full circle.
    const auto *region = p->batch.assets->surface(0);
    assert(region);
    for (unsigned row = 0; row < s.rows.size()-1; ++row) {
      const auto first = row*9;
      float left = std::numeric_limits<float>::infinity(), right = -left;
      float bottom = left, top = -left;
      constexpr float expectedUV[4][8] = {
          {0,0,1,0,1,1,0,1}, {0,0,0,1,1,1,1,0},
          {0,1,1,1,1,0,0,0}, {1,1,0,1,0,0,1,0}};
      for (unsigned quarter = 0; quarter < 4; ++quarter) {
        const auto &shadow = p->batch.quads[first+quarter];
        const auto &body = p->batch.quads[first+4+quarter];
        assert(shadow.texture == BattleClassTexture::Surfaces &&
               body.texture == BattleClassTexture::Surfaces);
        for (unsigned vertex = 0; vertex < 4; ++vertex) {
          const auto &a = shadow.vertices[vertex];
          const auto &v = body.vertices[vertex];
          assert(a.rgba == 0x00000080 && v.rgba == 0xc0c0c0ff);
          assert(std::abs((a.x-v.x)-2*(36.f/40)*(4.f/1.3f)) < .0001f);
          assert(std::abs((a.y-v.y)-2*(36.f/40)*(-3.076923131942749f)) < .0001f);
          assert(std::abs(v.u-(region->u0+(region->u1-region->u0)*expectedUV[quarter][vertex*2])) < .000001f);
          assert(std::abs(v.v-(region->v0+(region->v1-region->v0)*expectedUV[quarter][vertex*2+1])) < .000001f);
          left=std::min(left,v.x);right=std::max(right,v.x);
          bottom=std::min(bottom,v.y);top=std::max(top,v.y);
        }
      }
      assert(std::abs(right-left-72)<.0001f && std::abs(top-bottom-72)<.0001f);
      constexpr float offset[4][8] = {
          {-18,18,0,18,0,0,-18,0}, {18,18,18,0,0,0,0,18},
          {-18,0,0,0,0,-18,-18,-18}, {0,0,18,0,18,-18,0,-18}};
      for (unsigned quarter=0; quarter<4; ++quarter)
        for (unsigned vertex=0; vertex<4; ++vertex) {
          const auto &v=p->batch.quads[first+4+quarter].vertices[vertex];
          assert(std::abs(v.x-(left+right)*.5f-offset[quarter][vertex*2]*2)<.0001f);
          assert(std::abs(v.y-(bottom+top)*.5f-offset[quarter][vertex*2+1]*2)<.0001f);
        }
      const auto &icon = p->batch.quads[first+8];
      for (const auto &vertex : icon.vertices) assert(vertex.rgba==0xc0c0c0ff);
    }
    bool gray = false;
    for (const auto &q : p->batch.quads)
      gray |= q.sourceFlags == 48;
    assert(gray);
  }
  s.kind = HarborNativeMenuKind::Weapon;
  s.rows.resize(12, s.rows.front());
  for (int selected = 0; selected < 12; ++selected) {
    s.selected = selected;
    const auto p = prepare(s);
    assert(p && p->window && validHarborNativeMenuPlan(*p));
    unsigned hands = 0;
    for (const auto &q : p->batch.quads)
      hands += q.texture == BattleClassTexture::MessageIcons;
    assert(hands == 1);
  }
  s.kind = HarborNativeMenuKind::Target;
  s.work = 0;
  s.rows = {{2, 1, 0, true, "Lord Crump"}};
  s.selected = 0;
  assert(prepare(s));
  auto original = s;
  assert(sameHarborNativeMenu(s, original));
  for (unsigned change = 0; change < 20; ++change) {
    auto modified = s;
    switch (change) {
    case 0:
      modified.facade = &original;
      break;
    case 1:
      modified.runtime = std::make_shared<int>(2);
      break;
    case 2:
      modified.menuOwner = std::make_shared<const uint8_t>(0);
      break;
    case 3:
      modified.battle++;
      break;
    case 4:
      modified.work++;
      break;
    case 5:
      modified.selected++;
      break;
    case 6:
      modified.type++;
      break;
    case 7:
      modified.kind = HarborNativeMenuKind::Class;
      break;
    case 8:
      modified.rows[0].label = "Different";
      break;
    case 9:
      modified.rows[0].identity++;
      break;
    case 10:
      modified.rows[0].enabled = false;
      break;
    case 11:
      modified.registration = std::make_shared<int>(1);
      break;
    case 12:
      modified.phase++;
      break;
    case 13:
      modified.turn++;
      break;
    case 14:
      modified.modelTick++;
      break;
    case 15:
      modified.facadeRegistration = {};
      break;
    case 16:
      modified.rows[0].sourceName++;
      break;
    case 17:
      modified.rows[0].sourceReason++;
      break;
    case 18:
      modified.rows[0].sourceOrdinal++;
      break;
    case 19:
      modified.rows[0].sourceItem++;
      break;
    }
    assert(!sameHarborNativeMenu(original, modified));
  }
  for (unsigned change = 0; change < 6; ++change) {
    auto bad = s;
    switch (change) {
    case 0:
      bad.selected = -1;
      break;
    case 1:
      bad.selected = 1;
      break;
    case 2:
      bad.rows[0].label = "<system>";
      break;
    case 3:
      bad.rows[0].label = "";
      break;
    case 4:
      bad.rows.clear();
      break;
    case 5:
      bad.rows.resize(65, bad.rows[0]);
      break;
    }
    assert(!prepare(bad));
  }
  s.kind = HarborNativeMenuKind::Weapon;
  s.work = 0x9000;
  s.rows = {{1, 0, 65535, true, "Jump"}};
  assert(!prepare(s));
  s.kind = HarborNativeMenuKind::Target;
  s.rows = {{2, 1, 0, true, "Lord Crump"}};
  auto p = prepare(s);
  assert(p);
  p->batch.quads.back().vertices[0].x = std::numeric_limits<float>::infinity();
  assert(!validHarborNativeMenuPlan(*p));
  p = prepare(s);
  assert(p);
  const auto previousFont = resources.fonts.lease();
  resources.fonts.retire();
  assert(!validHarborNativeMenuPlan(*p) && !prepare(s));
  MessageFont originalFont;
  assert(originalFont.load("romfs/font.pmfont"));
  assert(resources.fonts.adopt(originalFont, {&resources.identities[2],
                                              512,
                                              256,
                                              0,
                                              1,
                                              512,
                                              256,
                                              false,
                                              {0, 1, 1, 0}}));
  assert(resources.fonts.lease().texture == previousFont.texture);
  assert(resources.fonts.lease().generation != previousFont.generation);
  assert(!validHarborNativeMenuPlan(*p) && prepare(s));
  auto mutatedPlan = prepare(s);
  assert(mutatedPlan);
  mutatedPlan->batch.quads.front().texture = BattleClassTexture::Footer;
  assert(!validHarborNativeMenuPlan(*mutatedPlan));
  mutatedPlan = prepare(s);
  assert(mutatedPlan);
  mutatedPlan->batch.quads.front().sourceFlags = 1;
  assert(!validHarborNativeMenuPlan(*mutatedPlan));
  // A provider losing a borrowed GPU generation during CPU preparation cannot
  // expose a drawable prefix, even though earlier glyph/icon quads were built.
  bool retired = false;
  const auto interrupted = prepareHarborNativeMenu(
      s, resources.surfaces.lease(), resources.fonts.lease(), [&](uint32_t id) {
        if (!retired) {
          retired = true;
          resources.fonts.retire();
        }
        return lookup(id);
      });
  assert(retired && !interrupted);
  const auto oldFacade = facade.proof();
  NativeFacadeRegistration copied = facade;
  assert(copied.proof().current());
  facade = copied;
  assert(!oldFacade.current() && copied.proof().current());
  s.facadeRegistration = facade.proof();
  assert(!sameHarborNativeMenu(original, s));
  registration.reset();
  assert(!sameHarborNativeMenu(original, original));
  std::cout
      << "PASS original-art native radial/list/target plans; every "
         "selection/page; missing/control/late resources reject whole batch; "
         "facade/runtime/setup/selection/row receipts reject changes\n";
}
