#pragma once
#include <cstdint>
#include <memory>
namespace ttyd {
class NativeUiStartup;
// Original primary/N64 setup and independent DVD callbacks over the actual VM.
// Native ROMFS worker tokens/budgets are backend policy, not GameCube timing.
class NativeEffectTextures {
public:
  static std::unique_ptr<NativeEffectTextures>
  create(std::shared_ptr<NativeUiStartup>, const char *romfsRoot);
  ~NativeEffectTextures();
  bool current() const;
  bool begin();
  bool pump(uint32_t byteBudget);
  bool ready() const;
  bool busy() const;
  uint32_t pending() const;

private:
  NativeEffectTextures();
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
};
} // namespace ttyd
