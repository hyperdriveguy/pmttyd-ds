#include "mario_action_voice_pcm.hpp"
#include "positional_sfx_start.hpp"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
uint32_t word(const std::vector<uint8_t> &b, unsigned offset) {
  return uint32_t(b.at(offset)) << 24 | uint32_t(b.at(offset + 1)) << 16 |
         uint32_t(b.at(offset + 2)) << 8 | b.at(offset + 3);
}
struct Fixture {
  std::shared_ptr<PositionalSfxRegistry> bank =
      std::make_shared<PositionalSfxRegistry>();
  std::shared_ptr<const void> runtime = std::make_shared<int>(1),
                              backend = std::make_shared<int>(2);
  std::array<std::shared_ptr<const uint8_t>, 40> owned{};
  std::array<MarioActionVoicePcmStatus, 4> states{};
  std::array<unsigned, 4> queued{};
  std::array<int16_t, 16> data{};
  std::vector<uint8_t> header;
  uint32_t dol = 0;
  unsigned starts = 0, stops = 0, mixes = 0, samples = 0;
  std::array<uint8_t, 4> volumes{};
  bool available = true, missing = false, reenter = false;
  int failStop = 0, failQueue = 0;
  MarioActionVoicePcm player;
  Fixture() {
    std::ifstream f("/home/carson/games/roms/G8ME01.iso", std::ios::binary);
    assert(f);
    std::vector<uint8_t> h(0x440);
    f.read(reinterpret_cast<char *>(h.data()), h.size());
    assert(f && std::string(reinterpret_cast<char *>(h.data()), 6) == "G8ME01");
    dol = word(h, 0x420);
    header.resize(256);
    f.seekg(dol);
    f.read(reinterpret_cast<char *>(header.data()), header.size());
    assert(f);
  }
  PositionalSfxRow row(uint32_t index) {
    const uint32_t address = 0x80316b28 + 20 * index;
    for (unsigned i = 0; i < 18; ++i) {
      const auto base = word(header, 0x48 + 4 * i),
                 size = word(header, 0x90 + 4 * i);
      if (base <= address && uint64_t(address) + 20 <= uint64_t(base) + size) {
        std::ifstream f("/home/carson/games/roms/G8ME01.iso", std::ios::binary);
        f.seekg(uint64_t(dol) + word(header, 4 * i) + address - base);
        PositionalSfxRow result;
        f.read(reinterpret_cast<char *>(result.bytes.data()), 20);
        assert(f);
        return result;
      }
    }
    std::abort();
  }
  std::string name(uint32_t index) {
    const uint32_t address = row(index).word(0);
    for (unsigned i = 0; i < 18; ++i) {
      const auto base = word(header, 0x48 + 4 * i),
                 size = word(header, 0x90 + 4 * i);
      if (base <= address && address < uint64_t(base) + size) {
        std::ifstream file("/home/carson/games/roms/G8ME01.iso",
                           std::ios::binary);
        file.seekg(uint64_t(dol) + word(header, 4 * i) + address - base);
        std::string result;
        for (unsigned n = 0;
             n < 128 && uint64_t(address) + n < uint64_t(base) + size; ++n) {
          char byte = 0;
          file.get(byte);
          assert(file);
          if (!byte)
            return result;
          result.push_back(byte);
        }
        std::abort();
      }
    }
    std::abort();
  }
  int32_t on(unsigned key, bool mine = true) {
    PositionalSfxStartServices s;
    s.user = this;
    s.row = [](void *p, uint32_t index) -> std::optional<PositionalSfxRow> {
      return static_cast<Fixture *>(p)->row(index);
    };
    s.story = [](void *) -> std::optional<int32_t> { return 3; };
    s.rate = [](void *) -> std::optional<int32_t> { return 60; };
    s.truncate = [](void *, float f) -> std::optional<int32_t> {
      return int32_t(std::trunc(f));
    };
    PositionalSfxStartRequest q;
    q.key = int32_t(key);
    const auto h = startPositionalSfx(*bank, q, s);
    assert(h && *h >= 0);
    if (mine)
      owned[unsigned(*h) & 255] = bank->slotLifetime(unsigned(*h) & 255);
    return *h;
  }
  MarioActionVoicePcmServices services() {
    MarioActionVoicePcmServices s;
    s.registry = bank;
    s.runtime = runtime;
    s.backend = backend;
    s.user = this;
    s.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter) {
        assert(!f.player.service(true));
        assert(!f.player.retire(f.runtime));
      }
      return f.available;
    };
    s.owns = [](void *p, unsigned i,
                const std::shared_ptr<const uint8_t> &birth) {
      auto &f = *static_cast<Fixture *>(p);
      return i < f.owned.size() && f.owned[i] == birth;
    };
    s.sample = [](void *p,
                  uint32_t row) -> std::optional<MarioActionVoiceSample> {
      auto &f = *static_cast<Fixture *>(p);
      ++f.samples;
      assert(marioActionVoicePcmFile(row));
      if (f.missing)
        return {};
      return MarioActionVoiceSample{f.data.data(), uint32_t(f.data.size()),
                                    22050, f.backend};
    };
    s.queue = [](void *p, unsigned i, const MarioActionVoiceSample &,
                 uint8_t volume, uint8_t pan) {
      auto &f = *static_cast<Fixture *>(p);
      assert(i < 4 && volume <= 127 && pan <= 127);
      if (f.failQueue) {
        --f.failQueue;
        return false;
      }
      ++f.starts;
      f.volumes[i] = volume;
      ++f.queued[i];
      f.states[i] = MarioActionVoicePcmStatus::Playing;
      return true;
    };
    s.status = [](void *p,
                  unsigned i) -> std::optional<MarioActionVoicePcmStatus> {
      return static_cast<Fixture *>(p)->states[i];
    };
    s.mix = [](void *p, unsigned voice, uint8_t volume, uint8_t pan) {
      auto &f = *static_cast<Fixture *>(p);
      assert(volume <= 127 && pan <= 127);
      f.volumes[voice] = volume;
      ++f.mixes;
      return true;
    };
    s.stop = [](void *p, unsigned) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.failStop) {
        --f.failStop;
        return false;
      }
      ++f.stops;
      return true;
    };
    return s;
  }
};
} // namespace
int main() {
  for (unsigned row = 0; row < 190; ++row) {
    const bool expected = (row >= 165 && row <= 169) ||
                          (row >= 171 && row <= 175) ||
                          (row >= 177 && row <= 181);
    assert(bool(marioActionVoicePcmFile(row)) == expected);
  }
  for (unsigned row : {165u, 166u, 167u, 168u, 169u, 171u, 172u, 173u, 174u,
                       175u, 177u, 178u, 179u, 180u, 181u}) {
    Fixture f;
    assert(marioActionVoicePcmDescriptor(row) == f.row(row).word(4));
    assert(marioActionVoicePcmName(row) == f.name(row));
    const auto handle = f.on(row);
    assert(f.player.bind(f.services()));
    assert(f.player.service(true) && f.starts == 1 && f.player.playing() == 1);
    assert(f.player.service(true) && f.starts == 1);
    f.states[0] = MarioActionVoicePcmStatus::Done;
    assert(f.player.service(true) && f.player.playing() == 0 &&
           f.bank->check(handle) == -1 &&
           !f.bank->slotLifetime(unsigned(handle) & 255));
    assert(f.player.retire(f.runtime));
  }
  {
    Fixture f;
    const auto foreign = f.on(181, false);
    const auto own = f.on(165);
    assert(f.player.bind(f.services()) && f.player.service(false) &&
           f.starts == 1);
    assert(f.volumes[0] == 0 && f.player.service(true) && f.starts == 1 &&
           f.volumes[0] > 0);
    assert(f.player.service(false) && f.starts == 1 && f.volumes[0] == 0);
    f.states[0] = MarioActionVoicePcmStatus::Done;
    assert(f.player.service(false));
    assert(f.bank->check(own) == -1 && f.bank->check(foreign) == 0);
    assert(f.player.retire(f.runtime));
  }
  {
    Fixture f;
    f.on(165);
    assert(f.player.bind(f.services()));
    f.missing = true;
    assert(!f.player.service(true) && f.starts == 0);
    f.missing = false;
    f.failQueue = 2;
    assert(!f.player.service(true) && !f.player.service(true) && f.starts == 0);
    assert(f.player.service(true) && f.starts == 1);
    f.failStop = 2;
    f.states[0] = MarioActionVoicePcmStatus::Done;
    assert(!f.player.service(true) && !f.player.service(true) &&
           f.player.playing() == 1 && f.starts == 1);
    assert(f.player.service(true) && f.player.playing() == 0);
    assert(f.player.retire(f.runtime));
  }
  {
    Fixture f;
    for (unsigned r = 165; r <= 169; ++r)
      f.on(r);
    assert(f.player.bind(f.services()) && f.player.service(true) &&
           f.starts == 4);
    f.states[0] = MarioActionVoicePcmStatus::Done;
    assert(f.player.service(true) && f.starts == 5 && f.player.playing() == 4);
    assert(!f.player.retire(std::make_shared<int>(1)));
    f.failStop = 1;
    assert(!f.player.retire(f.runtime) && f.player.playing() == 4);
    assert(f.player.retire(f.runtime) && f.player.playing() == 0);
  }
  {
    Fixture f;
    const auto old = f.on(165);
    assert(f.player.bind(f.services()) && f.player.service(true));
    assert(f.bank->off(old, {}));
    f.bank->allocationGeneration = uint16_t((uint32_t(old) >> 8) - 1);
    const auto replacement = f.on(171, false);
    assert((replacement & 255) == (old & 255) && replacement == old);
    assert(f.player.service(true) && f.stops == 1 &&
           f.bank->check(replacement) == 0 && f.starts == 1);
    assert(f.player.retire(f.runtime));
  }
  {
    Fixture f;
    const auto h = f.on(165);
    assert(f.player.bind(f.services()) && f.player.service(true));
    assert(f.bank->setVolume(h, 200));
    assert(!f.player.service(true) && f.starts == 1 && f.stops == 1);
    assert(f.bank->setVolume(h, 117));
    assert(f.player.service(true) && f.starts == 1 && f.bank->check(h) == 0);
    assert(f.player.retire(f.runtime));
  }
  {
    Fixture f;
    f.on(165);
    f.reenter = true;
    assert(f.player.bind(f.services()) && f.player.service(true));
    f.available = false;
    assert(!f.player.service(true) && f.starts == 1);
    assert(f.player.retire(f.runtime) && f.stops == 1);
  }
  std::cout
      << "PASS real original15 rows/source On births; owned vs foreign slots; "
         "zero-mix real completion; holds/no replay;4voice bound; actual "
         "Off/reuse/generation ABA; retirement/reentry/stale guards\n";
}
