#pragma once
#include <cstdint>
#include <memory>
namespace ttyd {
class NativeUiStartup;
// Shared actual-VM DVD/heap backend for whole original startup drivers. Source
// cursors retain each driver's distinct init, path, size and callback ordering.
class NativeSourceTextureStartup {
public:
  // Existing primary/N64 effect setup, retaining the established API.
  static std::unique_ptr<NativeSourceTextureStartup>
  create(std::shared_ptr<NativeUiStartup>, const char *romfsRoot);
  static std::unique_ptr<NativeSourceTextureStartup>
  createEnvironment(std::shared_ptr<NativeUiStartup>, const char *romfsRoot);
  static std::unique_ptr<NativeSourceTextureStartup>
  createFade(std::shared_ptr<NativeUiStartup>, const char *romfsRoot);
  bool initializeFade();
  ~NativeSourceTextureStartup();
  bool current() const;
  bool
  initializeEnvironment(); // Actual envInit once, before texture admission.
  bool begin(); // Complete source setup/job admission, not callback completion.
  bool pump(uint32_t byteBudget);
  bool ready() const;
  bool busy() const;
  uint32_t pending() const;

private:
  enum class Kind { Effects, Environment, Fade };
  NativeSourceTextureStartup();
  static std::unique_ptr<NativeSourceTextureStartup>
  createProfile(std::shared_ptr<NativeUiStartup>, const char *, Kind);
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
};
} // namespace ttyd
