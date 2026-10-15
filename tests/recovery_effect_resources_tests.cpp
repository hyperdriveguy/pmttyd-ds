#include "recovery_effect_resources.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace ttyd;
namespace {
void le(std::vector<uint8_t> &b, uint32_t value) {
  for (unsigned i = 0; i < 4; ++i)
    b.push_back(uint8_t(value >> (8 * i)));
}
std::vector<uint8_t> pack() {
  std::vector<uint8_t> b{'P', 'M', 'R', 'E', 'C', '0', '0', '1'};
  le(b, 2);
  le(b, 0x1000);
  le(b, 8);
  le(b, 0xfffffffc);
  le(b, 4);
  for (uint8_t v : {0x80, 0x41, 0x7a, 0x44, 0x12, 0x34, 0x56, 0x78, 1, 2, 3, 4})
    b.push_back(v);
  return b;
}
bool read(RecoveryEffectResources &r, const std::vector<uint8_t> &b) {
  FILE *f = std::tmpfile();
  assert(f);
  assert(std::fwrite(b.data(), 1, b.size(), f) == b.size());
  std::rewind(f);
  const bool result = r.read(f);
  std::fclose(f);
  return result;
}
void malformed() {
  RecoveryEffectResources r;
  const auto good = pack();
  assert(read(r, good));
  assert(r.spans().size() == 2 && r.word(0x1000) == 0x80417a44 &&
         r.half(0x1005) == 0x3456 && r.byte(0x1007) == 0x78 &&
         r.word(0xfffffffc) == 0x01020304 && r.byte(0xffffffff) == 4);
  assert(!r.word(0xfffffffd) && !r.half(0xffffffff) && !r.word(0x1005) &&
         !r.byte(0x1008) && !r.byte(0));
  // Every truncation fails transactionally; existing immutable owner survives.
  for (unsigned i = 0; i < good.size(); ++i) {
    assert(!read(r, {good.begin(), good.begin() + i}));
    assert(r.word(0x1000) == 0x80417a44);
  }
  auto wrong = good;
  wrong[0] = 'X';
  assert(!read(r, wrong));
  wrong = good;
  wrong.push_back(0);
  assert(!read(r, wrong));
  wrong = good;
  wrong[8] = 65;
  assert(!read(r, wrong));
  wrong = good;
  wrong[8] = 0;
  assert(!read(r, wrong));
  wrong = good;
  wrong[16] = 0;
  assert(!read(r, wrong));
  wrong = good;
  wrong[24] = 5;
  assert(!read(r, wrong)); // Wrapped end.
  wrong = good;
  wrong[20] = 0;
  wrong[21] = 0x10;
  wrong[22] = wrong[23] = 0;
  assert(!read(r, wrong)); // Overlap.
  assert(!r.load("this-file-does-not-exist") && r.word(0x1000) == 0x80417a44);
}
void original() {
  RecoveryEffectResources r;
  assert(r.load("romfs/recovery-effect.bin") && r.spans().size() == 9);
  assert(r.word(0x80417a44) == 0 && r.word(0x80417a48) == 0x429e0000 &&
         r.word(0x80417a4c) == 0x80417a3c);
  assert(r.word(0x80417a3c) == 1 && r.word(0x80417a40) == 0x80417a2c &&
         r.word(0x80417a2c) == 10 && r.word(0x80417a30) == 0x80371fa8 &&
         r.word(0x80417a34) == 1 && r.word(0x80417a38) == 0x80417a28);
  assert(r.word(0x80417a28) == 0x804179fc && r.half(0x804179fc) == 4);
  assert(r.word(0x802c1f18) == 0x43300000 && r.word(0x802c1f1c) == 0x80000000 &&
         r.word(0x8042019c) == 0x447a0000 && r.word(0x804201a0) == 0);
  assert(r.half(0x80371fa8) == 0 && r.half(0x80371fa8 + 9 * 178) == 79);
  const char *name = "Recovery";
  for (unsigned i = 0; i < std::strlen(name) + 1; ++i)
    assert(r.byte(0x802f3c00 + i) == uint8_t(name[i]));
  assert(!r.word(0x800000f8) &&
         !r.word(0x80415fd0)); // Dynamic providers are not fabricated.
}
} // namespace
int main(int argc, char **) {
  if (argc > 1)
    original();
  else
    malformed();
}
