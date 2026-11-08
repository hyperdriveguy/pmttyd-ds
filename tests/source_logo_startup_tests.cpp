#include "source_logo_startup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t WorkGlobal = 0x80416108, Work = 0x10000, OtherWork = 0x20000,
                   GpGlobal = 0x80415fd0, Gp = 0x30000, OtherGp = 0x40000,
                   Reset = 0x8029b544, Retrace = 0x802a3f58;
struct Fixture {
  std::map<uint32_t, uint32_t> words{
      {WorkGlobal, Work}, {GpGlobal, Gp}, {Gp, 0x100}, {OtherGp, 0x200}};
  std::vector<uint32_t> accepted, resets;
  unsigned resetIndex = 0, retraces = 0, allocations = 0, clears = 0;
  uint32_t dtv = 0, progressive = 0, buttons = 0, mode = 0;
  bool holdAllocation = false, holdRetrace = false, moveWork = false,
       moveGp = false;
  SourceLogoStartupCursor init, exit;
  BattleSequenceEntryServices services() {
    BattleSequenceEntryServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      auto i = f.words.find(a);
      if (i == f.words.end())
        return {};
      return i->second;
    };
    s.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.words[a] = v;
      if (f.moveGp && a == Gp && (v & 8))
        f.words[GpGlobal] = OtherGp;
      return true;
    };
    s.call = [](void *p, BattleSequenceCall &q) {
      auto &f = *static_cast<Fixture *>(p);
      assert(!initializeSourceLogo(f.services(), f.init));
      switch (q.function) {
      case 0x800050b4:
        assert(q.args[0] == Work && q.args[1] == 0 && q.args[2] == 32);
        ++f.clears;
        break;
      case 0x80007388:
        q.result = 0x5678;
        break;
      case 0x8018a61c:
        assert(q.args[0] == 4 && q.args[1] == 0x802c3b68 &&
               q.args[2] == 0x5678);
        if (f.holdAllocation)
          return false;
        ++f.allocations;
        q.result = 0x1234;
        if (f.moveWork)
          f.words[WorkGlobal] = OtherWork;
        break;
      case Reset:
        assert(f.resetIndex < f.resets.size());
        q.result = f.resets[f.resetIndex++];
        break;
      case 0x802a5088:
        q.result = f.dtv;
        break;
      case 0x8029c250:
        q.result = f.progressive;
        break;
      case 0x8029c2c0:
        assert(!q.args[0]);
        break;
      case 0x80005fd0:
        assert(!q.args[0]);
        q.result = f.buttons;
        break;
      case 0x802a4420:
        f.mode = q.args[0];
        break;
      case 0x802a4c28:
        break;
      case Retrace:
        if (f.holdRetrace)
          return false;
        ++f.retraces;
        break;
      case 0x8018a260:
        assert(q.args[0] == 0x1234);
        break;
      default:
        assert(false);
      }
      f.accepted.push_back(q.function);
      return true;
    };
    return s;
  }
};
void retainedAllocationAndVideoHold() {
  Fixture f;
  f.holdAllocation = true;
  f.moveWork = true;
  assert(!initializeSourceLogo(f.services(), f.init) &&
         f.init.pending == 0x8018a61c && f.clears == 1 && f.allocations == 0 &&
         f.words[Work + 4] == 0xffffffff);
  f.holdAllocation = false;
  f.holdRetrace = true;
  f.resets = {0, 0};
  f.progressive = 1;
  assert(!initializeSourceLogo(f.services(), f.init) &&
         f.init.pending == Retrace && f.clears == 1 && f.allocations == 1);
  assert(f.words[OtherWork] == 0x1234 && !f.words.count(Work) &&
         f.words[OtherWork + 0x18] == 0);
  assert(f.mode == 0x8030c230 && f.words[Gp] == 0x100);
  f.holdRetrace = false;
  assert(initializeSourceLogo(f.services(), f.init) && f.retraces == 1 &&
         f.words[Gp] == 0x100);
  assert(f.accepted ==
         std::vector<uint32_t>({0x800050b4, 0x80007388, 0x8018a61c, Reset,
                                0x802a5088, 0x8029c250, 0x8029c2c0, Reset,
                                0x802a4420, 0x802a4c28, Retrace}));
  assert(!exitSourceLogo(f.services(),
                         f.init)); // Wrong parent cursor is rejected.
}
void resetAndProgressiveBranches() {
  // No DTV branch under reset80000000 with bit2clear. Third reset is live.
  Fixture f;
  f.resets = {0x80000000, 0, 0x80000000, 0, 1};
  f.moveGp = true;
  assert(initializeSourceLogo(f.services(), f.init));
  assert(f.mode == 0x8030c26c && f.retraces == 2 && f.words[Gp] == 0x108 &&
         f.words[OtherGp] == 0x204);
  for (uint32_t button : {0u, 0x200u}) {
    Fixture b;
    b.dtv = 1;
    b.buttons = button;
    b.resets = {0, 0};
    assert(initializeSourceLogo(b.services(), b.init) &&
           b.words[Work + 0x18] == (button ? 1u : 0u));
  }
  Fixture p;
  p.dtv = 1;
  p.progressive = 1;
  p.resets = {0, 0};
  assert(initializeSourceLogo(p.services(), p.init) &&
         p.words[Work + 0x18] == 1);
  Fixture i;
  i.resets = {0x80000000, 0, 0x80000000, 0, 0};
  assert(initializeSourceLogo(i.services(), i.init) && i.mode == 0x8030c230 &&
         i.retraces == 1 && i.words[Gp] == 0x104);
  Fixture r;
  r.resets = {0x80000000, 2, 0x80000000, 2};
  assert(initializeSourceLogo(r.services(), r.init) && r.words[Gp] == 0x100 &&
         r.retraces == 1);
}
void orderedExit() {
  constexpr uint32_t children[] = {0x80077bdc, 0x8007fbd8, 0x80013aa4,
                                   0x8001b694, 0x8019df84, 0x8005ee0c,
                                   0x8005ba50, 0x8018a260};
  struct Exit {
    std::vector<uint32_t> done;
    bool held = true;
    uint32_t work = Work;
  } f;
  BattleSequenceEntryServices s;
  s.user = &f;
  s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
    auto &f = *static_cast<Exit *>(p);
    if (a == WorkGlobal)
      return f.work;
    if (a == OtherWork)
      return 0x4321;
    return {};
  };
  s.call = [](void *p, BattleSequenceCall &q) {
    auto &f = *static_cast<Exit *>(p);
    if (q.function == 0x8005ba50) {
      if (f.held)
        return false;
      f.work = OtherWork;
    }
    if (q.function == 0x8018a260)
      assert(q.args[0] == 0x4321);
    f.done.push_back(q.function);
    return true;
  };
  SourceLogoStartupCursor c;
  assert(!exitSourceLogo(s, c) && f.done.size() == 6);
  f.held = false;
  assert(exitSourceLogo(s, c) &&
         f.done ==
             std::vector<uint32_t>(std::begin(children), std::end(children)));
  assert(exitSourceLogo(s, c) && f.done.size() == 8);
}
} // namespace
int main() {
  retainedAllocationAndVideoHold();
  resetAndProgressiveBranches();
  orderedExit();
}
